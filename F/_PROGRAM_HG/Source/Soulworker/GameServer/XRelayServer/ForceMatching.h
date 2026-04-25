#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/Force.h"

class CServer;

class CForceMatchginMember {
public:
    CForceMatchginMember() = default;
    void Clear();

    CServer* m_pCurServer = nullptr;
    ST_FORCE_MEMBER m_stMemberInfo{};
    std::int64_t m_nExp = 0;
    int m_nState = 0;
};

class CForceMatching {
public:
    CForceMatching();

    // 对齐 IDA 0x14001CCB0: ?AutoMatchingAccept@CForceMatching@@QEAAXKPEAVCServer@@EK@Z
    // 签名: void AutoMatchingAccept(uint32_t dwActorID, CServer* pServer, uint8_t byCheck) - 3参数
    void AutoMatchingAccept(std::uint32_t dwActorID, CServer* pServer, std::uint8_t byCheck);
    // 对齐 IDA: 指针改为引用
    bool AutoMatchingEnter(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& stMemberInfo, std::uint32_t dwEnterMazeID, CServer* pServer);
    bool AutoMatchingExit(std::uint32_t dwActorID, std::uint8_t byReason, std::uint32_t dwUAID);
    // 对齐 IDA: 初始化匹配并添加第一个成员
    void AutoMatchingCreate(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& stMemberInfo,
                            std::uint32_t dwMatchingID,
                            std::uint32_t dwEnterMazeID,
                            std::uint32_t dwPortalID,
                            std::uint32_t dwJumpID,
                            CServer* pServer);
    // 对齐 IDA: 指针改为引用
    bool CheckAutoMatchingEnter(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& stMemberInfo, std::uint32_t dwEnterMazeID, CServer* pServer);
    bool CheckMazeOpenTime();
    void CheckFullUser();
    std::uint32_t GetMatchingID() { return m_dwMachingID; }  // 对齐 IDA: QEAAKXZ 非const
    std::uint8_t GetMatchingUserCount();  // 对齐 IDA: QEAAEXZ 非const
    void Init();
    void LeaderSelect();
    bool MatchingCheck();
    bool MatchingPossible();
    void MatchingWait();
    bool OnUpdate();
    void CreateMazeMatching(std::uint32_t dwForceID);
    void SendCreateMatchingMaze(ST_CREATE_MAZE stCreateMaze, PS_FORCE_INFO stForceInfo);  // 对齐 IDA: 按值传递
    void SendMatchingCheck();
    std::uint8_t SendMatchingExit(std::uint32_t dwActorID, std::uint8_t byReason, std::uint32_t dwUAID);
    void SendMatchingInfo(std::uint32_t dwActorID);
    void SendMatchingReset(std::uint8_t byReason);
    void SendMatchingStart();
    void SendMatchingWait();

    CForceMatchginMember m_stMatchingUser[8]{};
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
    std::uint8_t m_bCheck[8] = {};
    std::uint8_t m_byLimitCount = 0;
    int m_nResetCount = 0;
    std::int64_t m_biCheckMazeOpen = 0;

private:
    // 对齐 IDA: AEAAXE = private void(uint8_t)
    void SetMatchingState(std::uint8_t byState) { m_byState = byState; }
};

class CForceMatchingMgr {
public:
    void CreateMatching(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER stMemberInfo,  // 对齐 IDA: 按值传递
                        std::uint32_t dwMazeID,
                        std::uint32_t dwPortalID,
                        std::uint32_t dwJumpID,
                        CServer* pServer,
                        std::uint32_t& dwOutMatchingID);
    // 对齐 IDA: 添加 dwUAID 参数
    bool CheckMatching(std::uint32_t dwActorID, std::uint8_t byCheck, CServer* pServer, std::uint32_t dwUAID);
    bool EnterMatching(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER stMemberInfo,  // 对齐 IDA: 按值传递
                       std::uint32_t dwMazeID,
                       CServer* pServer,
                       std::uint32_t& dwOutMatchingID);
    bool EnterMatching(PS_SERVER_FORCE_MATCHING_ENTER psEnter,  // 对齐 IDA: 按值传递
                       PS_SERVER_FORCE_MATCHING_ENTER_MEMBER psMaster,  // 对齐 IDA: 按值传递
                       CServer* pServer,
                       std::uint8_t byPartyGroupType,
                       std::uint32_t& dwOutMatchingID,
                       std::uint8_t& byCreate);
    bool ExitMatching(std::uint32_t dwActorID, std::uint32_t dwMatchingID, std::uint8_t byReason, std::uint32_t dwUAID);
    void MatchingRemoveUser(std::uint32_t dwMatchingID, std::uint32_t dwUCID);
    void OnUpdate();
    void ResForceMatchingCreate(std::uint32_t dwMatchingID, std::uint32_t dwForceID);
    void SendCreateMatchingMaze(std::uint32_t dwMatchingID, ST_CREATE_MAZE stCreateMaze, PS_FORCE_INFO stForceInfo);  // 对齐 IDA: 按值传递

private:
    std::uint32_t m_dwMatchingID = 0;
    std::map<std::uint32_t, std::shared_ptr<CForceMatching>> m_mpAutoMatching;
    std::map<std::uint32_t, std::shared_ptr<CForceMatching>> m_mpAutoMatching_Waiter;
};
