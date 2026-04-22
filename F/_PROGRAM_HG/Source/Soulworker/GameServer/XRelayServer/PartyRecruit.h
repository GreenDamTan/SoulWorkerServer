#pragma once

#include <cstdint>
#include <set>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CServer;

class CPartyRecruit {
public:
    void GetRecruitInfo(ST_PARTY_RECRUIT_INFO& stRecruit) const;
    void SetRecruitDate();
    void ClearRecruitDate();
    void ApplyMemberClear();
    void GetMember(std::vector<std::uint32_t>& vecMember) const;
    void GetPartyMemberList(ST_PARTY_RECRUIT_APPLY_INFO& stInfo);
    std::uint32_t GetMasterID() const { return m_stPartyRecruit.dwMasterUCID; }
    std::uint32_t GetPartyID() const { return m_stPartyRecruit.dwPartyID; }
    void SetCID(std::uint32_t dwPartyID) { m_stPartyRecruit.dwPartyID = dwPartyID; }
    std::uint8_t GetPartyGroupType() const { return m_stPartyRecruit.byPartyGroupType; }
    int GetMemberCount() const { return static_cast<int>(m_setMember.size()); }
    std::int64_t GetRecruitDate() const { return m_biRecruitDate; }
    std::int64_t GetRecruitPenalty() const { return m_biRecruitPenalty; }
    int GetApplyCount() const;
    bool IsApplied(std::uint32_t dwActorID) const;
    std::uint8_t RecruitApply(ST_APPLY_MEMBER& stApply);
    bool RecruitAccept(CServer* pServer, std::uint32_t dwAcceptID);
    void SetRecruitInfo(std::uint32_t dwRecruitID, std::uint32_t dwMasterID, const ST_PARTY_RECRUIT& stRecruit);
    void AddMember(std::uint32_t dwActorID);
    void RemoveMember(std::uint32_t dwActorID);
    void DelApplyMember(std::uint32_t dwActorID, bool bSend);
    void SendApplyUserList(CServer* pServer, std::uint32_t dwActorID) const;

    ST_PARTY_RECRUIT m_stPartyRecruit{};
    std::set<std::uint32_t> m_setMember;
    std::int64_t m_biRecruitDate = 0;
    std::int64_t m_biRecruitPenalty = 0;
    ST_APPLY_MEMBER_LIST m_stApplicantList{};
};
