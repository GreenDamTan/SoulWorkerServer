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

    void Init();
    bool OnUpdate();
    bool MatchingPossible();
    bool MatchingCheck();
    void MatchingWait();
    void SendMatchingCheck();
    void SendMatchingWait();
    void SendMatchingStart();
    void CheckFullUser();
    std::uint8_t SendMatchingExit(std::uint32_t dwActorID, std::uint8_t byReason, std::uint32_t dwUAID);
    bool AutoMatchingExit(std::uint32_t dwActorID, std::uint8_t byReason, std::uint32_t dwUAID);
    // 对齐 IDA 0x14009B900: QEAA_NUST_PARTY_MEMBER@@_JKHPEAVCServer@@@Z
    // U = by-value struct, _J = int64, K = uint32, H = int
    bool AutoMatchingEnter(ST_PARTY_MEMBER stMemberInfo, std::int64_t nExp,
                           std::uint32_t dwEnterMazeID, int nState, CServer* pServer);
    // 对齐 IDA 0x14009BDD0: QEAA X UST_PARTY_MEMBER@@ _J E K @Z
    // 注意：参数顺序 (dwActorID, pServer, byCheck, dwUAID)
    void AutoMatchingAccept(std::uint32_t dwActorID, CServer* pServer, std::uint8_t byCheck, std::uint32_t dwUAID);
    // 对齐 IDA 0x14009BC20: QEAAX UST_PARTY_MEMBER@@ _J H K K K K PEAVCServer@@@Z
    // 注意：参数顺序 (stMemberInfo, nExp, nState, dwMatchingID, dwMazeID, dwPortalID, dwJumpID, pServer)
    void AutoMatchingCreate(ST_PARTY_MEMBER stMemberInfo, std::int64_t nExp, int nState,
                            std::uint32_t dwMatchingID, std::uint32_t dwMazeID,
                            std::uint32_t dwPortalID, std::uint32_t dwJumpID,
                            CServer* pServer);
    void LeaderSelect();
    void CreateMazeMatching(std::uint32_t dwPartyID);
    void SendCreateMatchingMaze(ST_CREATE_MAZE stCreateMaze, PS_PARTY_INFO stPartyInfo);  // 对齐 IDA: 按值传递
    // 对齐 IDA 0x14009C030: QEAAXK@Z
    void SendMatchingInfo(std::uint32_t dwActorID);

    // 对齐 IDA: QEAAKXZ = 非const方法
    std::uint32_t GetMatchingID() { return m_dwMachingID; }

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

private:
    // 对齐 IDA: AEAAXE = private void(uint8_t)
    void SetMatchingState(std::uint8_t byState) { m_byState = byState; }
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
    void ReqPartyRecruitCreate(std::shared_ptr<CUserPartyInfo> pUserParty,  // 对齐 IDA: by-value shared_ptr
                               ST_PARTY_RECRUIT& stRecruit,
                               std::uint32_t& dwRecruitID);  // 对齐 IDA: 引用而非指针
    void ReqPartyRecruitDel(std::uint32_t dwActorID);  // 对齐 IDA: 返回void
    void SendPartyRecruitList(std::uint32_t dwActorID, CServer* pServer);
    void SendPartyRecruitMyApplyList(std::uint32_t dwActorID, CServer* pServer);
    void SendPartyRecruitApplyList(std::uint32_t dwActorID, CServer* pServer);
    void SendPartyRecruitApplyInfo(std::uint32_t dwActorID, std::uint32_t dwRecruitID, CServer* pServer);
    void AddRecruitMember(std::uint32_t dwRecruitID, std::uint32_t dwMemberID);
    void DeleteRecruitMember(std::uint32_t dwRecruitID, std::uint32_t dwMemberID);
    void CreateParty(PS_REQ_PARTY_CREATE& stPartyReq);
    void CreateForce(PS_REQ_FORCE_CREATE& stForceReq);
    void ResPartyMatchingCreate(std::uint32_t dwMatchingID, std::uint32_t dwPartyID);
    void SendCreateMatchingMaze(std::uint32_t dwMatchingID,
                                 ST_CREATE_MAZE stCreateMaze,
                                 PS_PARTY_INFO stPartyInfo);  // 对齐 IDA 0x14009E480: 第二、三参数按值传递

    // 对齐 IDA: 新增匹配管理方法
    std::uint32_t FindRecruitID(std::uint32_t dwUCID);
    std::shared_ptr<CPartyRecruit> FindRecruitPtr(std::uint32_t dwRecruitID);
    bool EnterMatching(ST_PARTY_MEMBER stMemberInfo,  // 对齐 IDA: by-value
                       std::int64_t nExp,
                       std::uint32_t wReqMapID,
                       int nState,
                       CServer* pServer,
                       std::uint32_t& dwOutMatchingID);  // 对齐 IDA: 引用而非指针
    void CreateMatching(ST_PARTY_MEMBER stMemberInfo,  // 对齐 IDA: by-value
                        std::int64_t nExp,
                        std::uint32_t wReqMapID,
                        int nState,
                        std::uint32_t dwPortalID,  // 对齐 IDA: uint32_t not int
                        std::uint32_t dwJumpID,    // 对齐 IDA: uint32_t not int
                        CServer* pServer,
                        std::uint32_t& dwOutMatchingID);  // 对齐 IDA: 引用而非指针
    bool ExitMatching(std::uint32_t dwActorID, std::uint32_t dwMatchingID,  // 对齐 IDA: 返回bool, 参数(dwActorID, dwMatchingID, byReason, dwUAID)
                      std::uint8_t byReason, std::uint32_t dwUAID);

    // 对齐 IDA 0x14009F640: 从匹配中移除用户
    void MatchingRemoveUser(std::uint32_t dwMatchingID, std::uint32_t dwUCID) {
        if (dwMatchingID) {
            ExitMatching(dwUCID, dwMatchingID, 2, 0);
        }
    }

    bool CheckMatching(std::uint32_t dwActorID, std::uint8_t byCheck,  // 对齐 IDA: 返回bool, 参数(pServer, dwUAID) 顺序调整
                       CServer* pServer, std::uint32_t dwUAID);

    std::map<std::uint32_t, std::shared_ptr<CPartyMatching>> m_mpAutoMatching;
    std::uint32_t m_dwRecruitID = 0;
    std::map<std::uint32_t, std::shared_ptr<CPartyRecruit>> m_mpRecruit;
    std::map<std::uint32_t, std::uint32_t> m_mpRecruitUser;
};
