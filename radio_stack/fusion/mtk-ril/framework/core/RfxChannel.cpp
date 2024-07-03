/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RfxChannel.h"
#include "RfxChannelManager.h"

#define RFX_LOG_TAG "RfxChannel"

RfxChannel::RfxChannel(int channelId, char* muxPath) : mChannelId(channelId), mMuxPath(muxPath) {
    if (RfxRilUtils::getRilRunMode() == RIL_RUN_MODE_MOCK) {
        RFX_LOG_D(RFX_LOG_TAG, "RfxChannel, change ChannelFd For GT");
        mFd = RfxChannelManager::getChannelFdForGT(channelId);
    } else {
        if (strlen(mMuxPath) > 0) {
            do {
                mFd = open(mMuxPath, O_RDWR);
                if (mFd < 0) {
                    perror("opening AT interface. retrying...");
                    RFX_LOG_E(RFX_LOG_TAG, "could not connect to %s: %s", mMuxPath,
                              strerror(errno));
                    sleep(10);
                    /* never returns */
                } else {
                    struct termios ios;
                    tcgetattr(mFd, &ios);
                    ios.c_lflag = 0; /* disable ECHO, ICANON, etc... */
                    ios.c_iflag = 0;
                    tcsetattr(mFd, TCSANOW, &ios);
                }
            } while (mFd < 0);
        } else {
            mFd = -1;
            RFX_LOG_E(RFX_LOG_TAG, "path is empty, set fd to -1");
        }
    }

    mContext = new RfxChannelContext();
    mReader = new RfxReader(mFd, mChannelId, mContext);
    mSender = new RfxSender(mFd, mChannelId, mContext);
}

void RfxChannel::run() {
    mReader->run(String8::format("RfxReader_%d", mChannelId).string());
    mSender->run(String8::format("RfxSender_%d", mChannelId).string());
}

void RfxChannel::enqueueMessage(const sp<RfxMclMessage>& msg) { mSender->enqueueMessage(msg); }

void RfxChannel::enqueueMessageFront(const sp<RfxMclMessage>& msg) {
    mSender->enqueueMessageFront(msg);
}
