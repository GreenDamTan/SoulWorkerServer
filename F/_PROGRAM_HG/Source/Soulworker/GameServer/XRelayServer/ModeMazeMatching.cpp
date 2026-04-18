#include "Soulworker/GameServer/XRelayServer/ModeMazeMatching.h"

#include <algorithm>
#include <chrono>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

namespace {
std::uint64_t GreenDamTan_GetTickCount64() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}
}

bool CModeMazeMatching::AutoMatchingCreate(std::uint16_t wMapID,
                                           std::uint32_t dwMatchingID,
                                           std::uint32_t dwEventRoomID) {
    m_dwMatchingID = dwMatchingID;
    m_wMapID = wMapID;
    m_dwEventRoomID = dwEventRoomID;
    m_dw64WaitTime = 1000;
    m_dw64CheckTick = GreenDamTan_GetTickCount64() + 1000;
    m_byState = 0;
    m_byProcess = 1;
    m_bSendSucc = false;
    LogHelper::LogInfo("game.contents",
                       "ModeMazeMatching MatchingCreate - ( MatchingIID %u / ModeMaze %u )",
                       static_cast<unsigned int>(m_dwMatchingID),
                       static_cast<unsigned int>(m_wMapID));
    return true;
}

bool CModeMazeMatching::AutoMatchingEnter(const std::shared_ptr<CModeMazeMatchginMember>& pMember) {
    if (!pMember || m_byState || static_cast<int>(m_listMatchingUser.size()) >= m_nMaxMember) {
        return false;
    }

    m_listMatchingUser.push_back(pMember);
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching MatchingEnter - ( MatchingID %u / UCID %u )",
                        static_cast<unsigned int>(m_dwMatchingID),
                        static_cast<unsigned int>(pMember->GetActorID()));
    return true;
}

bool CModeMazeMatching::OnUpdate() {
    if (m_byProcess == 0) {
        if (!m_bSendSucc) {
            LogHelper::LogError("game.contents",
                                "ModeMazeMatching [state : %d, process : %d]",
                                static_cast<int>(m_byState),
                                static_cast<int>(m_byProcess));
            SendMatchingExit(0, 1);
        }
        return false;
    }

    if (m_byProcess == 2) {
        return true;
    }

    if (m_dw64CheckTick > GreenDamTan_GetTickCount64()) {
        return true;
    }

    if (m_byState == 0) {
        return MatchingPossible();
    }

    if (m_byState == 1) {
        MatchingWait();
        return true;
    }

    if (!m_bSendSucc) {
        LogHelper::LogError("game.contents",
                            "ModeMazeMatching [state : %d, process : %d]",
                            static_cast<int>(m_byState),
                            static_cast<int>(m_byProcess));
        SendMatchingExit(0, 1);
    }
    return false;
}

bool CModeMazeMatching::MatchingPossible() {
    SendMatchingWait();
    return true;
}

void CModeMazeMatching::MatchingWait() {
    MakeOperationMaze();
}

bool CModeMazeMatching::MakeOperationMaze() {
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching MakeOperationMaze Start - ( MatchingID %u )",
                        static_cast<unsigned int>(m_dwMatchingID));

    m_byProcess = 2;
    if (GetMemberCount() < m_nMinMember) {
        SendMatchingExit(0, 2);
        SetMatchingState(2);
        m_byProcess = 0;
        return false;
    }

    ST_CREATE_MODE_MAZE createModeMaze{};
    createModeMaze.wReqMapID = m_wMapID;
    createModeMaze.dwMatchingID = m_dwMatchingID;
    createModeMaze.nModeType = m_nModeType;
    createModeMaze.dwEventRoomID = m_dwEventRoomID;

    std::map<std::uint32_t, std::uint16_t> serverCounts;
    std::uint32_t masterServerID = 0;
    std::uint16_t topCount = 0;
    int firstJumpID = 0;
    bool firstJumpAssigned = false;

    for (const auto& member : m_listMatchingUser) {
        if (!member) {
            continue;
        }

        createModeMaze.vecEnterMember.push_back(member->m_stMemberInfo);
        if (!firstJumpAssigned) {
            firstJumpID = member->m_stMemberInfo.nFirstJumpID;
            firstJumpAssigned = true;
        }

        if (member->m_pCurServer) {
            const std::uint32_t serverID = member->m_pCurServer->GetServerID();
            const std::uint16_t count = static_cast<std::uint16_t>(++serverCounts[serverID]);
            if (count > topCount) {
                topCount = count;
                masterServerID = serverID;
            }
        }
    }

    createModeMaze.dwMasterServerID = masterServerID;
    createModeMaze.nJumpID = firstJumpID;

    XSendPacket packet(0xF2u, 0x49u);
    packet << createModeMaze;
    TXSingleton<XRelayServer>::Instance()->GetControlSocket().Send(packet);

    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching MakeOperationMaze End - ( MatchingID %u / MemberCound %d )",
                        static_cast<unsigned int>(m_dwMatchingID),
                        GetMemberCount());
    SetMatchingState(2);
    return true;
}

bool CModeMazeMatching::ExitMatching(std::uint32_t dwActorID,
                                     std::uint32_t dwUAID,
                                     std::uint8_t byReason) {
    static_cast<void>(dwUAID);
    if (m_byProcess != 1) {
        return false;
    }

    SendMatchingExit(dwActorID, byReason);
    for (auto it = m_listMatchingUser.begin(); it != m_listMatchingUser.end(); ++it) {
        if (*it && (*it)->GetActorID() == dwActorID) {
            m_listMatchingUser.erase(it);
            break;
        }
    }

    if (m_listMatchingUser.empty()) {
        m_byState = 2;
        m_byProcess = 0;
    }
    return true;
}

void CModeMazeMatching::SendMatchingWait() {
    m_byState = 1;
    m_dw64CheckTick = GreenDamTan_GetTickCount64() + m_dw64WaitTime;
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching SendMatchingWait - ( MatchingID %u / MemberCount %zu )",
                        static_cast<unsigned int>(m_dwMatchingID),
                        m_listMatchingUser.size());
}

void CModeMazeMatching::SendMatchingExit(std::uint32_t dwExitActorID, std::uint8_t byReason) {
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching SendMatchingExit - ( MatchingID %u / ExitUCID %u / Reason %u )",
                        static_cast<unsigned int>(m_dwMatchingID),
                        static_cast<unsigned int>(dwExitActorID),
                        static_cast<unsigned int>(byReason));
}

void CModeMazeMatching::SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze) {
    for (const auto& member : m_listMatchingUser) {
        if (!member || !member->m_pCurServer) {
            continue;
        }

        stCreateModeMaze.dwUserID = member->GetActorID();
        stCreateModeMaze.dwUAID = member->GetUAID();
        stCreateModeMaze.wReqMapID = m_wMapID;
        stCreateModeMaze.dwMatchingID = m_dwMatchingID;
        stCreateModeMaze.nModeType = m_nModeType;
        stCreateModeMaze.dwEventRoomID = m_dwEventRoomID;
        stCreateModeMaze.nJumpID = member->m_stMemberInfo.nFirstJumpID;

        XSendPacket packet(0xFDu, 0x06u);
        packet.XParse << member->GetActorID();
        packet << stCreateModeMaze;
        member->m_pCurServer->SendEx(packet);
    }

    m_bSendSucc = true;
    m_byProcess = 0;
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching SendCreateMatchingModeMaze - ( MatchingID %u / MemberCount %zu )",
                        static_cast<unsigned int>(m_dwMatchingID),
                        m_listMatchingUser.size());
}
