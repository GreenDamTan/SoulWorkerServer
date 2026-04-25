#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatching.h"

class CServer;

// 对齐 IDA: 匹配状态枚举
enum class eMODE_MAZE_MATCHING_STATE : int {
    NONE = 0,
    WAIT = 1,
    MAKE_LIST = 2,
    MAZE_CREATE = 3,
    MAZE_DESTROY = 4,
};

class CModeMazeMatchingMgr {
public:
    static CModeMazeMatchingMgr& Instance();

    bool EnterMatching(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq, CServer* pServer);
    bool AddModeMazeMatchingWait(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq,
                                 int& nError,
                                 CServer* pServer);
    // 对齐 IDA 0x1400395C0: ?ExitMatching@CModeMazeMatchingMgr@@QEAA_NAEAUPS_MODE_MAZE_MATCHING_EXIT@@@Z
    bool ExitMatching(PS_MODE_MAZE_MATCHING_EXIT& stExit);
    void MatchingRemoveUser(std::uint32_t dwUCID, std::uint32_t dwUAID);
    void ModeMazeMatchingEvent(PS_SERVER_MODE_MAZE_MATCHING_EVENT& eventInfo);
    void SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze);
    void OnUpdate();
    bool FindModeMazeMatching(std::uint32_t dwActorID);
    bool CheckModeMazeOpenTime(std::uint16_t wModeMazeID);
    // 对齐 IDA: QEAA = public
    void ProcessWaitList();
    void ProcessMazeMake();
    void DestroyMatchingWait();
    void SetMatchingState(eMODE_MAZE_MATCHING_STATE state);
    // 对齐 IDA: 作弊函数 (QEAA = public)
    void ModeMazeTime_Cheat(int nID, int nStart, int nEnd);

private:

    std::uint32_t m_dwMatchingID = 0;
    std::int64_t m_n64MatchingWaitRemain = 0;
    eMODE_MAZE_MATCHING_STATE m_eMatchingState = eMODE_MAZE_MATCHING_STATE::NONE;
    std::uint64_t m_dw64UpdateTick = 0;
    std::uint16_t m_wModeMazeID = 0;
    std::uint16_t m_wMaxEnterCount = 8;
    std::uint16_t m_wMinEnterCount = 4;
    std::map<std::uint32_t, std::shared_ptr<CModeMazeMatchginMember>> m_mapMatchingWait;
    std::map<std::uint32_t, std::shared_ptr<CModeMazeMatching>> m_mapMatchingInfo;
    std::shared_ptr<CModeMazeMatching> m_pEventModeMazeMatching;
};
