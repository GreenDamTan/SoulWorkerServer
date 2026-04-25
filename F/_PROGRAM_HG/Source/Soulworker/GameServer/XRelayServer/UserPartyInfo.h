#pragma once

#include <algorithm>
#include <cstdint>
#include <ctime>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CUserPartyInfo {
public:
    CUserPartyInfo() = default;
    explicit CUserPartyInfo(std::uint32_t dwActorID) : m_dwActorID(dwActorID) {}

    // 对齐 IDA: GetActorID 返回 TUXActorID (按值)，源码保留 uint32_t 兼容调用者
    // 对齐 IDA: QEAA = 非const
    std::uint32_t GetActorID() { return m_dwActorID; }
    // 对齐 IDA: QEAAKXZ = 非const
    std::uint32_t GetMatchingID() { return m_dwMatchingID; }
    std::uint8_t GetMatchingState() { return m_byType; }
    // 对齐 IDA: QEAAHXZ = 非const，返回 H (int32_t)
    std::int32_t GetRemainRecruitPenalty() {
        const std::int64_t remain = m_biRecruitPenalty - GreenDamTan_GetCurDateSec();
        return remain > 0 ? static_cast<std::int32_t>(remain) : 0;
    }
    std::uint8_t GetRewardState() { return m_byRewardState; }
    void SetRewardState(std::uint8_t byRewardState) { m_byRewardState = byRewardState; }
    void SetRecruitPenalty() { m_biRecruitPenalty = GreenDamTan_GetCurDateSec() + 600; }
    // 对齐 IDA: SetCID(K) 与 SetActorID 功能相同，IDA 中名为 SetCID
    void SetCID(std::uint32_t dwActorID) { m_dwActorID = dwActorID; }
    void SetActorID(std::uint32_t dwActorID) { m_dwActorID = dwActorID; }
    void SetServerID(std::uint32_t dwServerID) { m_dwServerID = dwServerID; }
    void SetMatchingID(std::uint32_t dwMatchingID, std::uint8_t byType = 0) {
        m_dwMatchingID = dwMatchingID;
        m_byType = byType;
        m_bMatchingState = (dwMatchingID != 0 || byType != 0);
    }
    void SetMatchingState(bool bMatchingState) {
        m_bMatchingState = bMatchingState;
        if (!bMatchingState && m_dwMatchingID == 0) {
            m_byType = 0;
        }
    }
    void SetRecruitDate(std::int64_t biRecruitDate) { m_biRecruitDate = biRecruitDate; }
    // 对齐 IDA: QEAAEXZ = 非const
    std::uint8_t GetApplyRecruitCount() {
        std::uint8_t byCount = 0;
        for (std::uint32_t recruitID : m_dwApplyRecruitID) {
            if (recruitID != 0) {
                ++byCount;
            }
        }
        return byCount;
    }
    // 对齐 IDA: QEAA_NK = 非const
    bool IsApplyRecruit(std::uint32_t dwRecruitID) {
        for (std::uint32_t recruitID : m_dwApplyRecruitID) {
            if (recruitID == dwRecruitID) {
                return false;
            }
        }
        return true;
    }
    // 对齐 IDA: QEAA_NXZ = 非const
    bool CanApplyRecruit() {
        for (std::uint32_t recruitID : m_dwApplyRecruitID) {
            if (recruitID == 0) {
                return true;
            }
        }
        return false;
    }
    void ClearApplyParty(std::uint32_t dwRecruitID) {
        for (std::uint32_t& recruitID : m_dwApplyRecruitID) {
            if (recruitID == dwRecruitID) {
                recruitID = 0;
            }
        }
    }
    void ClearRecruitParty(bool bUserSend);
    void SetApplyRecruitID(std::uint32_t dwRecruitID) {
        for (std::uint32_t& recruitID : m_dwApplyRecruitID) {
            if (recruitID == 0) {
                recruitID = dwRecruitID;
                return;
            }
        }
    }
    void SetApplyRecruitID(std::size_t index, std::uint32_t dwRecruitID) {
        if (index < std::size(m_dwApplyRecruitID)) {
            m_dwApplyRecruitID[index] = dwRecruitID;
        }
    }
    // 对齐 IDA: QEAAXAEAV = 非const方法
    void GetMYApplyRecruitInfo(ST_PARTY_RECRUIT_LIST& stRecruitMyApplyList) {
        stRecruitMyApplyList.vecInfo.clear();
        stRecruitMyApplyList.bLast = true;
        for (std::uint32_t recruitID : m_dwApplyRecruitID) {
            if (recruitID == 0) {
                continue;
            }

            ST_PARTY_RECRUIT recruit{};
            recruit.dwRecruitID = recruitID;
            stRecruitMyApplyList.vecInfo.push_back(recruit);
        }
    }
    void DelPartyRecruit(std::uint32_t dwRecruitID, bool bPartySend);
    // 对齐 IDA 0x1400D6B00: 参数 G = unsigned short
    void SyncChagneMapForParty(std::uint16_t wMapID);
    // 对齐 IDA 0x1400D6C00: 通知已申请的招募该用户升级
    void SyncChagneLevelForParty(std::uint8_t byLevel);
    // 对齐 IDA 0x1400D73A0: Logout 需要调用 ClearRecruitParty 和清理招募日期
    void Logout();

private:
    static std::int64_t GreenDamTan_GetCurDateSec() {
        return static_cast<std::int64_t>(std::time(nullptr));
    }

    std::uint32_t m_dwActorID = 0;
    std::uint32_t m_dwApplyRecruitID[5] = {};
    bool m_bMatchingState = false;
    std::uint32_t m_dwServerID = 0;
    std::uint32_t m_dwMatchingID = 0;
    std::uint8_t m_byType = 0;
    std::uint8_t m_byRewardState = 0;
    std::int64_t m_biRecruitDate = 0;
    std::int64_t m_biRecruitPenalty = 0;
};
