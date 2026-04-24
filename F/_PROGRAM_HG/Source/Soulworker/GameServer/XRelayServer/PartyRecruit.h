#pragma once

#include <cstdint>
#include <set>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CServer;

class CPartyRecruit {
public:
    // 对齐 IDA: QEAAXAEAUST_PARTY_RECRUIT_INFO@@@Z = 非const方法
    void GetRecruitInfo(ST_PARTY_RECRUIT_INFO& stRecruit);
    void SetRecruitDate();
    void ClearRecruitDate();
    void ApplyMemberClear();
    // 对齐 IDA: QEAAXAEAVvector@@@Z = 非const方法
    void GetMember(std::vector<std::uint32_t>& vecMember);
    void GetPartyMemberList(ST_PARTY_RECRUIT_APPLY_INFO& stInfo);
    // 对齐 IDA: QEAAKXZ = 非const方法
    std::uint32_t GetMasterID() { return m_stPartyRecruit.dwMasterUCID; }
    std::uint32_t GetPartyID() { return m_stPartyRecruit.dwPartyID; }
    void SetCID(std::uint32_t dwPartyID) { m_stPartyRecruit.dwPartyID = dwPartyID; }
    // 对齐 IDA: QEAAEXZ = 非const方法
    std::uint8_t GetPartyGroupType() { return m_stPartyRecruit.byPartyGroupType; }
    // 对齐 IDA: QEAAHXZ = 非const方法
    int GetMemberCount() { return static_cast<int>(m_setMember.size()); }
    std::int64_t GetRecruitDate() { return m_biRecruitDate; }
    std::int64_t GetRecruitPenalty() { return m_biRecruitPenalty; }
    int GetApplyCount();
    bool IsApplied(std::uint32_t dwActorID);
    std::uint8_t RecruitApply(ST_APPLY_MEMBER& stApply);
    bool RecruitAccept(CServer* pServer, std::uint32_t dwAcceptID);
    void SetRecruitInfo(std::uint32_t dwRecruitID, std::uint32_t dwMasterID, ST_PARTY_RECRUIT& stRecruit);  // 对齐 IDA: AEAU = 非const引用
    void GetRecruitInfo(ST_PARTY_RECRUIT& stRecruit);  // 对齐 IDA: 第二个重载, AEAU = 非const引用
    void AddMember(std::uint32_t dwActorID);
    void RemoveMember(std::uint32_t dwActorID);
    void DelApplyMember(std::uint32_t dwActorID, bool bSend);
    void SendApplyUserList(CServer* pServer, std::uint32_t dwActorID);
    // 对齐 IDA 0x1400AF0B0: ApplyMemberLevelUp(uint32_t dwActorID, uint8_t byLevel)
    void ApplyMemberLevelUp(std::uint32_t dwActorID, std::uint8_t byLevel);
    // 对齐 IDA 0x1400AF270: ApplyMemberMapMove(uint32_t dwActorID, uint32_t dwMapID)
    void ApplyMemberMapMove(std::uint32_t dwActorID, std::uint32_t dwMapID);

    ST_PARTY_RECRUIT m_stPartyRecruit{};
    std::set<std::uint32_t> m_setMember;
    std::int64_t m_biRecruitDate = 0;
    std::int64_t m_biRecruitPenalty = 0;
    ST_APPLY_MEMBER_LIST m_stApplicantList{};
};
