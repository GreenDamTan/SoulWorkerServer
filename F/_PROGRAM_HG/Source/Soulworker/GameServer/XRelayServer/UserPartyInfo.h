#pragma once

#include <algorithm>
#include <cstdint>
#include <ctime>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CUserPartyInfo {
public:
    CUserPartyInfo() = default;
    explicit CUserPartyInfo(std::uint32_t dwActorID) : m_dwActorID(dwActorID) {}

    std::uint32_t GetActorID() const { return m_dwActorID; }
    std::uint32_t GetMatchingID() const { return m_dwMatchingID; }
    std::uint8_t GetMatchingState() const { return m_byType; }
    std::int64_t GetRemainRecruitPenalty() const {
        const std::int64_t remain = m_biRecruitPenalty - GreenDamTan_GetCurDateSec();
        return remain > 0 ? remain : 0;
    }
    void SetRecruitPenalty() { m_biRecruitPenalty = GreenDamTan_GetCurDateSec() + 600; }
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
    std::uint8_t GetApplyRecruitCount() const {
        std::uint8_t byCount = 0;
        for (std::uint32_t recruitID : m_dwApplyRecruitID) {
            if (recruitID != 0) {
                ++byCount;
            }
        }
        return byCount;
    }
    bool IsApplyRecruit(std::uint32_t dwRecruitID) const {
        for (std::uint32_t recruitID : m_dwApplyRecruitID) {
            if (recruitID == dwRecruitID) {
                return false;
            }
        }
        return true;
    }
    bool CanApplyRecruit() const {
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
    void ClearRecruitParty(bool bUserSend) {
        static_cast<void>(bUserSend);
        for (std::uint32_t& recruitID : m_dwApplyRecruitID) {
            recruitID = 0;
        }
    }
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
    void GetMYApplyRecruitInfo(ST_PARTY_RECRUIT_LIST& stRecruitMyApplyList) const {
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
    void Logout() {
        m_bMatchingState = false;
        m_dwMatchingID = 0;
        m_byType = 0;
        m_biRecruitDate = 0;
    }

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
    std::int64_t m_biRecruitDate = 0;
    std::int64_t m_biRecruitPenalty = 0;
};

