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

#ifndef __RFX_CHANNEL_MANAGER_H__
#define __RFX_CHANNEL_MANAGER_H__

#include "RfxLog.h"
#include "rfx_properties.h"
#include "RfxDefs.h"
#include "RfxChannel.h"
#include <telephony/mtk_ril.h>

#define RIL_SUPPORT_CHANNELS_MAX_NAME_LEN 32

#define RIL_TTY_CHANNEL_IMS "dev/ccci_vts"

class RfxChannelManager {
  private:
    RfxChannelManager();

  public:
    static void init();
    static RfxSender* getSender(int channelId);
    static RfxChannel* getChannel(int channelId);
    static int getSupportChannels();
    static const char* channelIdToString(int channelId);
    static const char* proxyIdToString(int proxyId);
    static void setChannelFdForGT(int* channelFd);
    static int getChannelFdForGT(int channelId);
    static void urcRegisterDone();
    static int getMainChannelSim();

  private:
    void initMuxPath();
    void switchMuxPath(int majorSim);
    void initOtherChannel();

  private:
    static RfxChannelManager* sSelf;
    RfxChannel* mChannels[RIL_SUPPORT_CHANNELS];
    static int sFdsForGt[RIL_SUPPORT_CHANNELS];
    char muxPath[RIL_SUPPORT_CHANNELS][RIL_SUPPORT_CHANNELS_MAX_NAME_LEN];
    char muxPathInit[RIL_SUPPORT_CHANNELS][RIL_SUPPORT_CHANNELS_MAX_NAME_LEN] = {
            "/dev/radio/pttynoti",   "/dev/radio/pttycmd1",   "/dev/radio/pttycmd2",
            "/dev/radio/pttycmd3",   "/dev/radio/pttycmd4",   "/dev/radio/atci1",
            "/dev/radio/pttycmd7",   "/dev/radio/pttycmd8",   "/dev/radio/pttycmd9",
            "/dev/radio/pttycmd10",  "/dev/radio/pttycmd11",  "/dev/radio/pttyims",

            "/dev/radio/ptty2noti",  "/dev/radio/ptty2cmd1",  "/dev/radio/ptty2cmd2",
            "/dev/radio/ptty2cmd3",  "/dev/radio/ptty2cmd4",  "/dev/radio/atci2",
            "/dev/radio/ptty2cmd7",  "/dev/radio/ptty2cmd8",  "/dev/radio/ptty2cmd9",
            "/dev/radio/ptty2cmd10", "/dev/radio/ptty2cmd11", "/dev/radio/ptty2ims",

            "/dev/radio/ptty3noti",  "/dev/radio/ptty3cmd1",  "/dev/radio/ptty3cmd2",
            "/dev/radio/ptty3cmd3",  "/dev/radio/ptty3cmd4",  "/dev/radio/atci3",
            "/dev/radio/ptty3cmd7",  "/dev/radio/ptty3cmd8",  "/dev/radio/ptty3cmd9",
            "/dev/radio/ptty3cmd10", "/dev/radio/ptty3cmd11", "/dev/radio/ptty3ims",

            "/dev/radio/ptty4noti",  "/dev/radio/ptty4cmd1",  "/dev/radio/ptty4cmd2",
            "/dev/radio/ptty4cmd3",  "/dev/radio/ptty4cmd4",  "/dev/radio/atci4",
            "/dev/radio/ptty4cmd7",  "/dev/radio/ptty4cmd8",  "/dev/radio/ptty4cmd9",
            "/dev/radio/ptty4cmd10", "/dev/radio/ptty4cmd11", "/dev/radio/ptty4ims"};
    Mutex mUrcMutex;
    static int sMainSim;
    int mUrcCount;
};

#endif
