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

#include <mtk_log.h>
#include "WpfaDriver.h"
#include "WpfaCcciSender.h"
#include "WpfaDriverUtilis.h"

#define WPFA_D_LOG_TAG "WpfaCcciSender"

void WpfaCcciSender::CcciMsgHandler::handleMessage(const Message& message) {
    WPFA_UNUSED(message);
    sender->processMessage(msg);
}

WpfaCcciSender::WpfaCcciSender(int fd) : m_fd(fd) {
    mtkLogD(WPFA_D_LOG_TAG, "init");
    mNeedWaitLooper = true;
    sem_init(&mWaitLooperSem, 0, 0);
    memset(&m_threadId, 0, sizeof(pthread_t));
}

void WpfaCcciSender::enqueueCcciMessage(const sp<WpfaDriverMessage>& msg) {
    waitLooper();
    mtkLogD(WPFA_D_LOG_TAG, "enqueueCcciMessage: %s", msg->toString().string());
    WpfaCcciSender* sender = this;
    sp<MessageHandler> handler = new CcciMsgHandler(sender, msg);
    m_looper->sendMessage(handler, m_dummy_msg);
}

void WpfaCcciSender::enqueueCcciMessageFront(const sp<WpfaDriverMessage>& msg) {
    waitLooper();
    mtkLogD(WPFA_D_LOG_TAG, "enqueueCcciMessageFront: %s", msg->toString().string());
    WpfaCcciSender* sender = this;
    sp<MessageHandler> handler = new CcciMsgHandler(sender, msg);
    m_looper->sendMessageAtTime(0, handler, m_dummy_msg);
}

void WpfaCcciSender::processMessage(const sp<WpfaDriverMessage>& msg) {
    int retval = 0;
    int length_write = 0;
    int try_cnt = 0;
    const int k_max_try_cnt = 20;

    // TODO: remark, only for UT
    mtkLogD(WPFA_D_LOG_TAG, "processMessage: %s", msg->toString().string());

    ccci_msg_send_t* mCcciMsgSend = (ccci_msg_send_t*)calloc(1, sizeof(ccci_msg_send_t));

    retval = driverMsgToCcciMsg(msg, mCcciMsgSend);
    if (retval != 0) {
        mtkLogE(WPFA_D_LOG_TAG, "driverMsgToCcciMsg fail!!");
        free(mCcciMsgSend);
        return;
    }

    // send message from CCCI
    for (try_cnt = 0; try_cnt < k_max_try_cnt; try_cnt++) {
        length_write = write(m_fd, mCcciMsgSend->buffer, mCcciMsgSend->buffer_size);
        if (length_write == mCcciMsgSend->buffer_size) {
            retval = 0;
            break;
        }

        retval = -1;
        mtkLogD(WPFA_D_LOG_TAG, "write ccci, try_cnt: %d, msg_id: 0x%x, length_write: %d", try_cnt,
                mCcciMsgSend->msg.hdr.msg_id, length_write);
        usleep(2 * 1000);
    }

    free(mCcciMsgSend);

    // test mode
    if (WPFA_DRIVER_TEST_MODE_ENABLE) {
        uint16_t msgId = msg->getMsgId();
        WpfaDriverUtilis* mWpfaDriverUtilis = WpfaDriverUtilis::getInstance();
        if ((mWpfaDriverUtilis != NULL) && (msgId == MSG_A2M_WPFA_VERSION)) {
            mWpfaDriverUtilis->testStartNormal(msg);
        }
    }
}

int WpfaCcciSender::driverMsgToCcciMsg(const sp<WpfaDriverMessage>& driverMsg,
                                       ccci_msg_send_t* ccciMsgSend) {
    int retValue = 0;  // 0: success, 1: fail

    ccci_msg_hdr_t* header = &ccciMsgSend->msg.hdr;
    // ccci_msg_body_t *body = &ccciMsgSend->msg.body;

    header->msg_id = driverMsg->getMsgId();
    header->t_id = driverMsg->getTid();
    header->msg_type = driverMsg->getType();
    header->param_16bit = driverMsg->getParams();

    mtkLogD(WPFA_D_LOG_TAG, "driverMsgToCcciMsg Header msgId=%d tid=%d type=%d", header->msg_id,
            header->t_id, header->msg_type);

    if (header->msg_type == CCCI_CTRL_MSG) {
        // date size to be writed
        // ccciMsgSend->buffer_size = CCCI_HEADER_SIZE;
        ccciMsgSend->buffer_size = sizeof(ccci_msg_t);
        mtkLogD(WPFA_D_LOG_TAG, "ccciMsgSend->buffer_size=%d", ccciMsgSend->buffer_size);
    } else if (header->msg_type == CCCI_IP_TABLE_MSG) {
        uint16_t msgId = header->msg_id;
        switch (msgId) {
            case MSG_A2M_WPFA_INIT: {
                // no mseeage bode only header
                // ccciMsgSend->buffer_size = CCCI_HEADER_SIZE;
                break;
            }
            case MSG_A2M_WPFA_VERSION: {
                const wifiproxy_ap_md_filter_ver_t* verMsg =
                        (const wifiproxy_ap_md_filter_ver_t*)driverMsg->getData()->getData();
                wifiproxy_ap_md_filter_ver_t* version = &ccciMsgSend->msg.body.u.version;
                version->ap_filter_ver = verMsg->ap_filter_ver;
                version->md_filter_ver = verMsg->md_filter_ver;
                version->dl_buffer_size = verMsg->dl_buffer_size;
                version->ul_buffer_size = verMsg->ul_buffer_size;
                // ccciMsgSend->buffer_size = CCCI_HEADER_SIZE + CCCI_BODY_FILTER_VER_SIZE;
                mtkLogD(WPFA_D_LOG_TAG, "version Body apVer=%d mdVer=%d", version->ap_filter_ver,
                        version->md_filter_ver);
                break;
            }
            case MSG_A2M_REG_REPLY: {
                const wifiproxy_a2m_reg_reply_t* regAcceptMsg =
                        (const wifiproxy_a2m_reg_reply_t*)driverMsg->getData()->getData();
                wifiproxy_a2m_reg_reply_t* regAccept = &ccciMsgSend->msg.body.u.reg_reply_id;
                regAccept->fid = regAcceptMsg->fid;
                regAccept->error_cause = regAcceptMsg->error_cause;
                // ccciMsgSend->buffer_size = CCCI_HEADER_SIZE + CCCI_BODY_REG_ACCEPT_SIZE;
                mtkLogD(WPFA_D_LOG_TAG, "regAccept Body fid=%d error_cause=%d", regAccept->fid,
                        regAccept->error_cause);
                break;
            }
            case MSG_A2M_DEREG_REPLY: {
                const wifiproxy_a2m_dereg_reply_t* deRegAcceptMsg =
                        (const wifiproxy_a2m_dereg_reply_t*)driverMsg->getData()->getData();
                wifiproxy_a2m_dereg_reply_t* deRegAccept = &ccciMsgSend->msg.body.u.dereg_reply_id;
                deRegAccept->fid = deRegAcceptMsg->fid;
                deRegAccept->error_cause = deRegAcceptMsg->error_cause;
                // ccciMsgSend->buffer_size = CCCI_HEADER_SIZE + CCCI_BODY_REG_ACCEPT_SIZE;
                mtkLogD(WPFA_D_LOG_TAG, "deRegAccept Body fid=%d error_cause=%d", deRegAccept->fid,
                        deRegAccept->error_cause);
                break;
            }
            default: {
                mtkLogE(WPFA_D_LOG_TAG, "can't hadle this msg_id=%d", driverMsg->getMsgId());
                retValue = 1;
            }
        }
        ccciMsgSend->buffer_size = sizeof(ccci_msg_t);
        mtkLogD(WPFA_D_LOG_TAG, "ccciMsgSend->buffer_size=%d", ccciMsgSend->buffer_size);

    } else {
        mtkLogE(WPFA_D_LOG_TAG, "cannot handle msgType: %d", driverMsg->getType());
        retValue = 1;
    }
    return retValue;
}

bool WpfaCcciSender::threadLoop() {
    mtkLogD(WPFA_D_LOG_TAG, "threadLoop. WpfaCcciSender init");
    m_looper = Looper::prepare(0);
    sem_post(&mWaitLooperSem);
    m_threadId = pthread_self();

    int result;
    do {
        result = m_looper->pollAll(-1);
        mtkLogD(WPFA_D_LOG_TAG, "threadLoop, result = %d", result);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);

    WPFA_D_ASSERT(0);  // Can't go here
    return true;
}

sp<Looper> WpfaCcciSender::waitLooper() {
    mWaitLooperMutex.lock();
    if (mNeedWaitLooper) {
        mtkLogD(WPFA_D_LOG_TAG, "waitLooper() begin");
        sem_wait(&mWaitLooperSem);
        mNeedWaitLooper = false;
        sem_destroy(&mWaitLooperSem);
        mtkLogD(WPFA_D_LOG_TAG, "waitLooper() end");
    }
    mWaitLooperMutex.unlock();
    return m_looper;
}
