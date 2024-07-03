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

#ifndef __RMC_DC_PDN_MANAGER_H__
#define __RMC_DC_PDN_MANAGER_H__

#include "RfxBaseHandler.h"
#include "RmcDataDefs.h"
#include "RmcDcUtility.h"

class RmcDcPdnManager : public RfxBaseHandler {
  public:
    RmcDcPdnManager(int slot_id, int channel_id);
    virtual ~RmcDcPdnManager();

  public:
    int getPdnTableSize();
    bool validateAid(int aid);

    // Get Pdn info of the specified AID.
    PdnInfo getPdnInfo(int aid);
    void setPdnInfo(int aid, PdnInfo* info);
    void clearPdnInfo(int aid);
    void clearAllPdnInfo();

    // A set of set functions to update PDN attributes of a specific AID.
    void updatePdnActiveStatus(int aid, int pdnActiveStatus);
    void updateApnName(int aid, const char* apnName);
    void updateTransIntfId(int aid, int transIntfId);
    void updateIpAddress(int aid, const char* ipv4Addr, int index, const char* ipv6Addr);
    void updatePcscfAddress(int aid, int index, const char* pcscfAddr);
    void updateIpv4Dns(int aid, int index, const char* v4Dns);
    void updateIpv6Dns(int aid, int index, const char* v6Dns);
    void updateIpv4Address(int aid, const char* ipv4Addr);
    void updateIpv6Address(int aid, int index, const char* ipv6Addr);
    void updateDns(int aid, int v4DnsNumber, int v6DnsNumber, const char** v4Dns,
                   const char** v6Dns);
    void updateMtu(int aid, int mtu);
    void updateRat(int aid, int rat);
    void updateBearerId(int aid, int bearerId);

    void setAid(int index, int aid);
    void setPrimaryAid(int index, int primaryAid);
    void setAidAndPrimaryAid(int index, int aid, int primaryAid);
    void setIsDedicateBearer(int aid, bool isDedicateBearer);
    void setIsEmergency(int aid, bool isEmergency);
    void setReason(int aid, int reason);
    void setDeactReason(int aid, int deactReason);
    void setSscMode(int aid, int sscMode);
    void setPdnType(int aid, int pdnType);
    void setSignalingFlag(int aid, int flag);
    void setProfileId(int aid, int profileId);

    // A set of get functions to get PDN attributes of a specific AID.
    int getPdnActiveStatus(int aid);
    char* getApnName(int aid);
    int getTransIntfId(int aid);
    int getAid(int index);
    int getPrimaryAid(int index);
    char* getIpv4Dns(int aid, int index);
    char* getIpv6Dns(int aid, int index);
    int getMtu(int aid);
    int getSignalingFlag(int aid);
    char* getIpv4Address(int aid);
    char* getIpv6Address(int aid, int index);
    int getReason(int aid);
    int getDeactReason(int aid);
    int getSscMode(int aid);
    int getPdnType(int aid);
    int getProfileId(int aid);
    bool isDedicateBearer(int aid);
    void initAtCmds();

  private:
    void initPdnTable();
    int getModemSupportPdnNumber();
    void clearPdnInfo(PdnInfo* info);

    void dumpAllPdnInfo();
    // PdnInfo to string.
    // TODO: public and move to utility class if other class needs it.
    static String8 pdnInfoToString(PdnInfo* pdnInfo);

    // Update status key to notify PDN status, requested by AGPS.
    void updateAndNotifyPdnConnectStatusChanged();

  private:
    int mPdnTableSize;
    PdnInfo* m_pPdnInfo;
    int mNumOfConnectedPdn;
    static const int DEFAULT_PDN_TABLE_SIZE = 11;

    // Add variable to config internal debug info.
    static const bool DEBUG_MORE_INFO = false;
};

// Validate whether the aid is valid, return TRUE if it is valid.
// Use it before access PDN info to avoid bad memory access behavior.
inline bool RmcDcPdnManager::validateAid(int aid) { return (aid >= 0 && aid < mPdnTableSize); }

inline void RmcDcPdnManager::updateApnName(int aid, const char* apnName) {
    RFX_ASSERT(validateAid(aid));
    strncpy(m_pPdnInfo[aid].apn, apnName, MAX_APN_NAME_LENGTH - 1);
}

inline void RmcDcPdnManager::updateTransIntfId(int aid, int transIntfId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].transIntfId = transIntfId;
}

inline void RmcDcPdnManager::updateMtu(int aid, int mtu) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].mtu = mtu;
}

inline void RmcDcPdnManager::updateRat(int aid, int rat) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].rat = rat;
}

inline void RmcDcPdnManager::setSignalingFlag(int aid, int flag) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].signalingFlag = flag;
}

inline void RmcDcPdnManager::updateBearerId(int aid, int bearerId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].bearerId = bearerId;
}

inline void RmcDcPdnManager::updatePcscfAddress(int aid, int index, const char* pcscfAddr) {
    RFX_ASSERT(validateAid(aid));
    RFX_ASSERT(index >= 0 && index < MAX_PCSCF_NUMBER);
    RFX_ASSERT(pcscfAddr != NULL);
    strncpy(m_pPdnInfo[aid].pcscf[index], pcscfAddr, MAX_IPV6_ADDRESS_LENGTH - 1);
}

inline void RmcDcPdnManager::updateIpv4Dns(int aid, int index, const char* v4Dns) {
    RFX_ASSERT(validateAid(aid));
    if (RmcDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmcDcPdnManager", "[%d][%s] index%d is out of bound on aid%d", m_slot_id,
                      __FUNCTION__, index, aid);
            return;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    RFX_ASSERT(v4Dns != NULL);
    strncpy(m_pPdnInfo[aid].dnsV4[index], v4Dns, MAX_IPV4_ADDRESS_LENGTH - 1);
}

inline void RmcDcPdnManager::updateIpv6Dns(int aid, int index, const char* v6Dns) {
    RFX_ASSERT(validateAid(aid));
    if (RmcDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmcDcPdnManager", "[%d][%s] index%d is out of bound on aid%d", m_slot_id,
                      __FUNCTION__, index, aid);
            return;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    RFX_ASSERT(v6Dns != NULL);
    strncpy(m_pPdnInfo[aid].dnsV6[index], v6Dns, MAX_IPV6_ADDRESS_LENGTH - 1);
}

inline void RmcDcPdnManager::updateIpv4Address(int aid, const char* ipv4Addr) {
    RFX_ASSERT(validateAid(aid));
    RFX_ASSERT(ipv4Addr != NULL);
    strncpy(m_pPdnInfo[aid].addressV4, ipv4Addr, MAX_IPV4_ADDRESS_LENGTH - 1);
}

inline void RmcDcPdnManager::updateIpv6Address(int aid, int index, const char* ipv6Addr) {
    RFX_ASSERT(validateAid(aid));
    if (RmcDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_IPV6_ADDRESS_NUMBER) {
            RFX_LOG_E("RmcDcPdnManager", "[%d][%s] index%d is out of bound on aid%d", m_slot_id,
                      __FUNCTION__, index, aid);
            return;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_IPV6_ADDRESS_NUMBER);
    RFX_ASSERT(ipv6Addr != NULL);
    strncpy(m_pPdnInfo[aid].addressV6[index], ipv6Addr, MAX_IPV6_ADDRESS_LENGTH - 1);
}

inline void RmcDcPdnManager::setAid(int index, int aid) {
    RFX_ASSERT(validateAid(index));
    m_pPdnInfo[index].aid = aid;
}

inline void RmcDcPdnManager::setPrimaryAid(int index, int primaryAid) {
    RFX_ASSERT(validateAid(index));
    m_pPdnInfo[index].primaryAid = primaryAid;
}

inline void RmcDcPdnManager::setAidAndPrimaryAid(int index, int aid, int primaryAid) {
    RFX_ASSERT(validateAid(index));
    m_pPdnInfo[index].aid = aid;
    m_pPdnInfo[index].primaryAid = primaryAid;
}

inline void RmcDcPdnManager::setIsEmergency(int aid, bool isEmergency) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].isEmergency = isEmergency;
}

inline void RmcDcPdnManager::setIsDedicateBearer(int aid, bool isDedicateBearer) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].isDedicateBearer = isDedicateBearer;
}

inline void RmcDcPdnManager::setReason(int aid, int reason) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].reason = reason;
}

inline void RmcDcPdnManager::setDeactReason(int aid, int deactReason) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].deactReason = deactReason;
}

inline void RmcDcPdnManager::setSscMode(int aid, int sscMode) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].sscMode = sscMode;
}

inline void RmcDcPdnManager::setPdnType(int aid, int pdnType) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].pdnType = pdnType;
}

inline void RmcDcPdnManager::setProfileId(int aid, int profileId) {
    RFX_ASSERT(validateAid(aid));
    m_pPdnInfo[aid].profileId = profileId;
}

inline int RmcDcPdnManager::getPdnActiveStatus(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].active;
}

inline int RmcDcPdnManager::getTransIntfId(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].transIntfId;
}

inline char* RmcDcPdnManager::getApnName(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].apn;
}

inline int RmcDcPdnManager::getAid(int index) {
    RFX_ASSERT(validateAid(index));
    return m_pPdnInfo[index].aid;
}

inline int RmcDcPdnManager::getPrimaryAid(int index) {
    RFX_ASSERT(validateAid(index));
    return m_pPdnInfo[index].primaryAid;
}

inline char* RmcDcPdnManager::getIpv4Dns(int aid, int index) {
    RFX_ASSERT(validateAid(aid));
    if (RmcDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmcDcPdnManager", "[%d][%s] index%d is out of bound on aid%d", m_slot_id,
                      __FUNCTION__, index, aid);
            return NULL;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    return m_pPdnInfo[aid].dnsV4[index];
}

inline char* RmcDcPdnManager::getIpv6Dns(int aid, int index) {
    RFX_ASSERT(validateAid(aid));
    if (RmcDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_DNS_ADDRESS_NUMBER) {
            RFX_LOG_E("RmcDcPdnManager", "[%d][%s] index%d is out of bound on aid%d", m_slot_id,
                      __FUNCTION__, index, aid);
            return NULL;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_DNS_ADDRESS_NUMBER);
    return m_pPdnInfo[aid].dnsV6[index];
}

inline int RmcDcPdnManager::getMtu(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].mtu;
}

inline int RmcDcPdnManager::getSignalingFlag(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].signalingFlag;
}

inline char* RmcDcPdnManager::getIpv4Address(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].addressV4;
}

inline char* RmcDcPdnManager::getIpv6Address(int aid, int index) {
    RFX_ASSERT(validateAid(aid));
    if (RmcDcUtility::isUserBuild()) {
        if (index >= MAX_NUM_IPV6_ADDRESS_NUMBER) {
            RFX_LOG_E("RmcDcPdnManager", "[%d][%s] index%d is out of bound on aid%d", m_slot_id,
                      __FUNCTION__, index, aid);
            return NULL;
        }
    }
    RFX_ASSERT(index >= 0 && index < MAX_NUM_IPV6_ADDRESS_NUMBER);
    return m_pPdnInfo[aid].addressV6[index];
}

inline int RmcDcPdnManager::getPdnType(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].pdnType;
}

inline int RmcDcPdnManager::getProfileId(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].profileId;
}

inline int RmcDcPdnManager::getReason(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].reason;
}

inline int RmcDcPdnManager::getDeactReason(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].deactReason;
}

inline int RmcDcPdnManager::getSscMode(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].sscMode;
}

inline bool RmcDcPdnManager::isDedicateBearer(int aid) {
    RFX_ASSERT(validateAid(aid));
    return m_pPdnInfo[aid].isDedicateBearer;
}

#endif
