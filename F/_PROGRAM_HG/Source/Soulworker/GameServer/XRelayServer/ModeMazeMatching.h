#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CServer;

class CModeMazeMatchginMember {
public:
    CModeMazeMatchginMember() = default;
    explicit CModeMazeMatchginMember(CServer* pServer) : m_pCurServer(pServer) {}

    std::uint32_t GetActorID() const { return m_stMemberInfo.dwActorID; }
    std::uint32_t GetUAID() const { return m_stMemberInfo.dwUAID; }
    std::uint16_t GetRank() const { return m_wRank; }

    CServer* m_pCurServer = nullptr;
    ST_MODE_MAZE_MEMBER_INFO m_stMemberInfo{};
    std::uint16_t m_wRank = 0;
};

class CModeMazeMatching {
public:
    friend class CModeMazeMatchingMgr;

    bool AutoMatchingCreate(std::uint16_t wMapID, std::uint32_t dwMatchingID, std::uint32_t dwEventRoomID);
    bool AutoMatchingEnter(const std::shared_ptr<CModeMazeMatchginMember>& pMember);
    bool OnUpdate();
    bool MatchingPossible();
    void MatchingWait();
    bool MakeOperationMaze();
    bool ExitMatching(std::uint32_t dwActorID, std::uint32_t dwUAID, std::uint8_t byReason);
    void SendMatchingWait();
    void SendMatchingExit(std::uint32_t dwExitActorID, std::uint8_t byReason);
    void SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze);

    std::uint32_t GetMatchingID() const { return m_dwMatchingID; }
    std::uint8_t GetMatchingState() const { return m_byState; }
    std::uint8_t GetMatchingProcess() const { return m_byProcess; }
    std::uint32_t GetEventRoomID() const { return m_dwEventRoomID; }
    int GetMemberCount() const { return static_cast<int>(m_listMatchingUser.size()); }
    void SetMatchingState(std::uint8_t byState) { m_byState = byState; }

private:
    std::list<std::shared_ptr<CModeMazeMatchginMember>> m_listMatchingUser;
    std::uint32_t m_dwMatchingID = 0;
    std::uint16_t m_wMapID = 0;
    int m_nModeType = 0;
    std::uint8_t m_byState = 0;
    std::uint8_t m_byProcess = 0;
    std::uint64_t m_dw64CheckTick = 0;
    bool m_bSendSucc = false;
    int m_nMinMember = 4;
    int m_nMaxMember = 8;
    std::uint64_t m_dw64WaitTime = 0;
    std::uint32_t m_dwEventRoomID = 0;
};
