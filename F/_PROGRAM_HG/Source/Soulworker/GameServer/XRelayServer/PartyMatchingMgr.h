#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/PartyRecruit.h"
#include "Soulworker/GameServer/XRelayServer/UserPartyInfo.h"

class CServer;

class CPartyMatchginMember {
public:
    void Clear();

    CServer* m_pCurServer = nullptr;
    ST_PARTY_MEMBER m_stMemberInfo{};
    std::int64_t m_nExp = 0;
    int m_nState = 0;
};

class CPartyMatching {
public:
    CPartyMatching();

    bool OnUpdate();
    bool MatchingPossible();
    bool MatchingCheck();
    void MatchingWait();
    void SendMatchingCheck();
    void SendMatchingWait();
    void SendMatchingStart();
    std::uint8_t SendMatchingExit(std::uint32_t dwActorID, std::uint8_t byReason, std::uint32_t dwUAID);
    bool AutoMatchingExit(std::uint32_t dwActorID, std::uint8_t byReason, std::uint32_t dwUAID);
    void AutoMatchingAccept(std::uint32_t dwActorID, CServer* pServer, std::uint8_t byCheck, std::uint32_t dwUAID);
    void LeaderSelect();

    std::uint32_t GetMatchingID() const { return m_dwMachingID; }
    void SetMatchingState(std::uint8_t byState) { m_byState = byState; }

    CPartyMatchginMember m_stMatchingUser[4]{};
    std::uint32_t m_dwMachingID = 0;
    std::uint64_t m_dw64CheckTick = 0;
    std::int16_t m_shAveLevel = 0;
    std::uint8_t m_byState = 0;
    std::uint8_t m_byProcess = 0;
    std::uint32_t m_dwLeaderActorID = 0;
    std::uint32_t m_dwMazeID = 0;
    std::uint32_t m_dwPortalID = 0;
    std::uint32_t m_dwJumpID = 0;
    bool m_bSendSucc = false;
    std::uint8_t m_bCheck[4] = {};
    std::uint8_t m_byLimitCount = 0;
};

class CPartyMatchingMgr {
public:
    void OnUpdate();
    bool GetPartyRecruitInfo(std::uint32_t dwUCID, ST_PARTY_RECRUIT_INFO& stPartyRecruit);
    std::uint8_t ReqPartyRecruitApply(ST_PARTY_RECRUIT_APPLY& stApply, ST_APPLY_MEMBER& stMember);
    bool ReqRecruitAccept(std::uint32_t dwRecruitID,
                          ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT& stAccept,
                          CServer* pServer);
    void ReqRecruitReject(std::uint32_t dwRecruitID, std::uint32_t dwTargetUCID);
    void DeletePartyRecruit(std::uint32_t dwRecruitID);
    void ClearRecruitDate(std::uint32_t dwUCID);
    bool ReqPartyRecruitCreate(const std::shared_ptr<CUserPartyInfo>& pUserParty,
                               const ST_PARTY_RECRUIT& stRecruit,
                               std::uint32_t* pdwRecruitID);
    bool ReqPartyRecruitDel(std::uint32_t dwActorID);
    void SendPartyRecruitList(std::uint32_t dwActorID, CServer* pServer);
    void SendPartyRecruitMyApplyList(std::uint32_t dwActorID, CServer* pServer);
    void SendPartyRecruitApplyList(std::uint32_t dwActorID, CServer* pServer);
    void SendPartyRecruitApplyInfo(std::uint32_t dwActorID, std::uint32_t dwRecruitID, CServer* pServer);

private:
    friend class CUserPartyInfo;
    friend class CPartyProcess;

    std::uint32_t FindRecruitID(std::uint32_t dwUCID);
    std::shared_ptr<CPartyRecruit> FindRecruitPtr(std::uint32_t dwRecruitID);

    std::map<std::uint32_t, std::shared_ptr<CPartyMatching>> m_mpAutoMatching;
    std::uint32_t m_dwRecruitID = 0;
    std::map<std::uint32_t, std::shared_ptr<CPartyRecruit>> m_mpRecruit;
    std::map<std::uint32_t, std::uint32_t> m_mpRecruitUser;
};
