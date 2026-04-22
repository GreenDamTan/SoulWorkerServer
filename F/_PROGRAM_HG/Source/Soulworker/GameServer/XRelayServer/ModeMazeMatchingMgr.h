#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatching.h"

class CServer;

class CModeMazeMatchingMgr {
public:
    static CModeMazeMatchingMgr& Instance();

    bool EnterMatching(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq, CServer* pServer);
    bool AddModeMazeMatchingWait(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq,
                                 int& nError,
                                 CServer* pServer);
    bool ExitMatching(PS_MODE_MAZE_MATCHING_EXIT& exitInfo);
    void MatchingRemoveUser(std::uint32_t dwUCID, std::uint32_t dwUAID);
    void ModeMazeMatchingEvent(PS_SERVER_MODE_MAZE_MATCHING_EVENT& eventInfo);
    void SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze);
    void OnUpdate();
    bool FindModeMazeMatching(std::uint32_t dwActorID);
    bool CheckModeMazeOpenTime(std::uint16_t wModeMazeID);

private:
    void ProcessWaitList();
    void ProcessMazeMake();
    void DestroyMatchingWait();
    void SetMatchingState(int state);

    std::uint32_t m_dwMatchingID = 0;
    std::int64_t m_n64MatchingWaitRemain = 0;
    int m_eMatchingState = 0;
    std::uint64_t m_dw64UpdateTick = 0;
    std::uint16_t m_wModeMazeID = 0;
    std::uint16_t m_wMaxEnterCount = 8;
    std::uint16_t m_wMinEnterCount = 4;
    std::map<std::uint32_t, std::shared_ptr<CModeMazeMatchginMember>> m_mapMatchingWait;
    std::map<std::uint32_t, std::shared_ptr<CModeMazeMatching>> m_mapMatchingInfo;
    std::shared_ptr<CModeMazeMatching> m_pEventModeMazeMatching;
};
