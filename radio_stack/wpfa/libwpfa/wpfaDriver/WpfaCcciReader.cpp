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

#include "WpfaCcciReader.h"
#include "WpfaControlMsgHandler.h"

#define WPFA_D_LOG_TAG "WpfaCcciReader"

WpfaCcciReader::WpfaCcciReader(int fd) : mFd(fd) {
    mtkLogD(WPFA_D_LOG_TAG, "init");
    memset(&m_threadId, 0, sizeof(pthread_t));
}

bool WpfaCcciReader::threadLoop() {
    m_threadId = pthread_self();
    readerLoopForCcciData();
    return true;
}

void WpfaCcciReader::readerLoopForCcciData() {
    int count = 0;
    int bodySize = 0;
    uint16_t msgId = 0;
    uint16_t msgParam = 0;
    WpfaCcciDataHeader ccciDataHeader;
    ccci_msg_body_t* body = NULL;
    ccci_msg_ul_ip_pkt_body_t* IpPktbody = NULL;

    ccci_msg_hdr_t* header = (ccci_msg_hdr_t*)calloc(1, CCCI_HEADER_SIZE);
    if (header == NULL) {
        mtkLogE(WPFA_D_LOG_TAG, "OOM");
        return;
    }
    mtkLogD(WPFA_D_LOG_TAG, "readerLoop run!");

    for (;;) {
        // get header
        memset(header, 0, sizeof(ccci_msg_hdr_t));
        do {
            count = read(mFd, header, sizeof(ccci_msg_hdr_t));
            if (count < 0 && errno != EINTR) {
                mtkLogE(WPFA_D_LOG_TAG, "header read end: %d (err: %d - %s)\n", count, errno,
                        strerror(errno));
                free(header);
                return;
            } else {
                mtkLogD(WPFA_D_LOG_TAG, "header read end: %d\n", count);
            }
        } while (count < 0 && errno == EINTR);

        // For non-ViLte project, imspty will transform to dev/null
        // It will trigger read with count=0, the data can be ignored
        if (count == 0) {
            continue;
        }

        ccciDataHeader = WpfaCcciDataHeaderEncoder::decodeHeader(header);
        mtkLogD(WPFA_D_LOG_TAG, "ccciDataHeader msgId: %d", ccciDataHeader.getMsgId());

        // for all read indication, receive the msg size data is size of ccci_msg_body_t
        body = (ccci_msg_body_t*)calloc(1, sizeof(ccci_msg_body_t));
        if (body == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "OOM");
            free(header);
            return;
        }

        // get size by mseeage Id
        // bodySize = WpfaDriverMessage::getCcciMsgBodySize(msgId);
        bodySize = sizeof(ccci_msg_body_t);

        // get message body if the message is not UL IP PKT
        if (ccciDataHeader.getMsgType() != CCCI_UL_IP_PKT_MSG) {
            do {
                count = read(mFd, body, bodySize);
                if (count < 0 && errno != EINTR) {
                    mtkLogE(WPFA_D_LOG_TAG, "body read end: %d (err: %d - %s)\n", count, errno,
                            strerror(errno));
                    free(body);
                    free(header);
                    return;
                } else {
                    mtkLogD(WPFA_D_LOG_TAG, "body read end: count=%d\n", count);
                }
            } while (count < 0 && errno == EINTR);
        }

        if (ccciDataHeader.getMsgType() == CCCI_CTRL_MSG) {
            handleShmCtrlEvent(ccciDataHeader);
            continue;
        } else if (ccciDataHeader.getMsgType() == CCCI_IP_TABLE_MSG) {
            // get message id
            msgId = ccciDataHeader.getMsgId();

            // create data object
            switch (msgId) {
                case MSG_M2A_REG_DL_FILTER:
                    handleFilterRuleCtrlEvent(ccciDataHeader, WpfaDriverRegFilterData(body, 1));
                    break;

                case MSG_M2A_DEREG_DL_FILTER:
                    handleFilterRuleCtrlEvent(ccciDataHeader, WpfaDriverDeRegFilterData(body, 1));
                    break;

                case MSG_M2A_WPFA_VERSION:
                    handleFilterRuleCtrlEvent(ccciDataHeader, WpfaDriverVersionData(body, 1));
                    break;

                default:
                    mtkLogE(WPFA_D_LOG_TAG, "can't hadle this msg id!");
                    // todo: assert
            }

            // send message to dispatcher
            // handleFilterRuleCtrlEvent(ccciDataHeader, body);
            free(body);
        } else if (ccciDataHeader.getMsgType() == CCCI_UL_IP_PKT_MSG) {
            // MD UL AP Path Via TTY instead of SHM (For M2 RD IT purpose)
            // get message id
            msgId = ccciDataHeader.getMsgId();
            msgParam = ccciDataHeader.getParams();

            mtkLogD(WPFA_D_LOG_TAG, "UP_IP_PKT size=%d", msgParam);

            IpPktbody = (ccci_msg_ul_ip_pkt_body_t*)calloc(1, sizeof(ccci_msg_ul_ip_pkt_body_t));
            if (IpPktbody == NULL) {
                mtkLogE(WPFA_D_LOG_TAG, "OOM");
                free(header);
                return;
            }

            // get message body
            do {
                count = read(mFd, IpPktbody, msgParam);
                if (count < 0 && errno != EINTR) {
                    mtkLogE(WPFA_D_LOG_TAG, "body read end: %d (err: %d - %s)\n", count, errno,
                            strerror(errno));
                    free(IpPktbody);
                    free(header);
                    return;
                } else {
                    mtkLogD(WPFA_D_LOG_TAG, "body read end: count=%d\n", count);
                }
            } while (count < 0 && errno == EINTR);

            switch (msgId) {
                case MSG_M2A_UL_IP_PKT:
                    handleFilterRuleCtrlEvent(ccciDataHeader, WpfaDriverULIpPkt(IpPktbody, 1));
                    break;
                default:
                    mtkLogE(WPFA_D_LOG_TAG, "can't hadle this msg id!");
            }
            free(IpPktbody);

        } else {
            mtkLogE(WPFA_D_LOG_TAG, "cannot handle this msgType: %d", ccciDataHeader.getMsgType());
            free(header);
            return;
        }
    }
    free(header);
}

void WpfaCcciReader::handleFilterRuleCtrlEvent(WpfaCcciDataHeader header, WpfaDriverBaseData data) {
    // create Message and sent to WpfaControlMsgHandler
    int msgId = header.getMsgId();
    mtkLogD(WPFA_D_LOG_TAG, "handleFilterRuleCtrlEvent, msgId:%d", msgId);

    sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(
            header.getMsgId(), header.getTid(), header.getMsgType(), header.getParams(), data);
    WpfaControlMsgHandler::enqueueDriverMessage(msg);
}

void WpfaCcciReader::handleShmCtrlEvent(WpfaCcciDataHeader header) {
    // create Message(only header) and sent to WpfaControlMsgHandler
    mtkLogD(WPFA_D_LOG_TAG, "handleShmCtrlEvent, msgId:%d", header.getMsgId());
    sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(
            header.getMsgId(), header.getTid(), header.getMsgType(), header.getParams());
    WpfaControlMsgHandler::enqueueDriverMessage(msg);
}
