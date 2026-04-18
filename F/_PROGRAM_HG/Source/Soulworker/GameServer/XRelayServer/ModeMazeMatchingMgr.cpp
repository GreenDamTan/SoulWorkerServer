#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"

#include <algorithm>
#include <chrono>
#include <cwchar>
#include <vector>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

namespace {
std::uint64_t GreenDamTan_GetTickCount64() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

enum GreenDamTan_ModeMazeMatchingState {
    GREENDAMTAN_MODE_MAZE_MATCHING_NONE = 0,
    GREENDAMTAN_MODE_MAZE_MATCHING_WAIT = 1,
    GREENDAMTAN_MODE_MAZE_MATCHING_MAKE_LIST = 2,
    GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_CREATE = 3,
    GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_DESTROY = 4,
};
}

CModeMazeMatchingMgr& CModeMazeMatchingMgr::Instance() {
    static CModeMazeMatchingMgr manager;
    return manager;
}

bool CModeMazeMatchingMgr::AddModeMazeMatchingWait(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq,
                                                   int& nError,
                                                   CServer* pServer) {
    if (!pServer) {
        nError = 53201;
        return false;
    }

    auto member = std::make_shared<CModeMazeMatchginMember>(pServer);
    member->m_stMemberInfo = enterReq.stMemberInfo;
    member->m_wRank = enterReq.wRank;
    m_mapMatchingWait[enterReq.stMemberInfo.dwActorID] = member;
    LogHelper::LogDebug("game.contents",
                        "ModeMaze::AddModeMazeMatchingWait() AddWait UCID(%u)",
                        static_cast<unsigned int>(enterReq.stMemberInfo.dwActorID));
    nError = 0;
    return true;
}

bool CModeMazeMatchingMgr::EnterMatching(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq,
                                         CServer* pServer) {
    int nError = 0;
    if (!AddModeMazeMatchingWait(enterReq, nError, pServer)) {
        LogHelper::LogError("game.contents",
                            "ModeMazeMatching EnterMatching fail - ( UCID %u / Error %d )",
                            static_cast<unsigned int>(enterReq.stMemberInfo.dwActorID),
                            nError);
        return false;
    }

    m_wModeMazeID = enterReq.wModeMazeID;
    m_wMaxEnterCount = std::max<std::uint16_t>(m_wMaxEnterCount, 8);
    m_wMinEnterCount = std::max<std::uint16_t>(m_wMinEnterCount, 4);
    if (m_eMatchingState == GREENDAMTAN_MODE_MAZE_MATCHING_NONE) {
        m_n64MatchingWaitRemain = static_cast<std::int64_t>(GreenDamTan_GetTickCount64() + 1000);
        SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_WAIT);
        m_dw64UpdateTick = GreenDamTan_GetTickCount64() + 1000;
    }
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching EnterMatching - ( UCID %u / ModeMaze %u )",
                        static_cast<unsigned int>(enterReq.stMemberInfo.dwActorID),
                        static_cast<unsigned int>(enterReq.wModeMazeID));
    return true;
}

bool CModeMazeMatchingMgr::ExitMatching(PS_MODE_MAZE_MATCHING_EXIT& exitInfo) {
    const auto waitIt = m_mapMatchingWait.find(exitInfo.dwExitUCID);
    if (waitIt != m_mapMatchingWait.end()) {
        m_mapMatchingWait.erase(waitIt);
    }

    for (auto& [matchingID, matching] : m_mapMatchingInfo) {
        static_cast<void>(matchingID);
        if (matching && matching->ExitMatching(exitInfo.dwExitUCID, exitInfo.dwExitUAID, exitInfo.byReason)) {
            return true;
        }
    }
    return true;
}

void CModeMazeMatchingMgr::MatchingRemoveUser(std::uint32_t dwUCID, std::uint32_t dwUAID) {
    PS_MODE_MAZE_MATCHING_EXIT exitInfo{};
    exitInfo.dwExitUCID = dwUCID;
    exitInfo.dwExitUAID = dwUAID;
    ExitMatching(exitInfo);
}

void CModeMazeMatchingMgr::ModeMazeMatchingEvent(PS_SERVER_MODE_MAZE_MATCHING_EVENT& eventInfo) {
    if (!m_pEventModeMazeMatching) {
        m_pEventModeMazeMatching = std::make_shared<CModeMazeMatching>();
        ++m_dwMatchingID;
        m_pEventModeMazeMatching->AutoMatchingCreate(static_cast<std::uint16_t>(eventInfo.nModeMazeID),
                                                     m_dwMatchingID,
                                                     static_cast<std::uint32_t>(eventInfo.nID));
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (const unsigned long actorID : eventInfo.vecInfo) {
        const std::shared_ptr<CUserObject> user = relayServer.GetUser(static_cast<std::uint32_t>(actorID));
        if (!user) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Is NULL Map(%d), EventID(%d), UCID(%u)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<unsigned int>(actorID));
            return;
        }

        const std::shared_ptr<CUserPartyInfo> partyUser = relayServer.GetPartyUser(static_cast<std::uint32_t>(actorID));
        if (!partyUser) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Party Is NULL Map(%d), EventID(%d), UCID(%u)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<unsigned int>(actorID));
            return;
        }

        CServer* memberServer = relayServer.GetServer(user->GetServerID());
        if (!memberServer) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Server Is NULL Map(%d), EventID(%d), UCID(%u)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<unsigned int>(actorID));
            return;
        }

        if (partyUser->GetMatchingID() != 0 || partyUser->GetMatchingState() != 0) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Matching State Error Map(%d), EventID(%d), UCID(%u)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<unsigned int>(actorID));
            return;
        }

        auto member = std::make_shared<CModeMazeMatchginMember>(memberServer);
        member->m_stMemberInfo.dwActorID = user->GetCID();
        member->m_stMemberInfo.dwUAID = user->GetUAID();
        member->m_stMemberInfo.wMapID = user->GetMapID();
        member->m_stMemberInfo.byClass = user->GetClass();
        member->m_stMemberInfo.byLevel = user->GetLevel();
        member->m_stMemberInfo.byAwaken = user->GetAwaken();
        member->m_stMemberInfo.dwProfilePhotoID = user->GetProfilePhoto();
        member->m_stMemberInfo.uxMapID = user->GetMapIns();
        const std::wstring name = user->GetName();
        std::wcsncpy(member->m_stMemberInfo.strName, name.c_str(), 20);
        member->m_stMemberInfo.strName[20] = L'\0';
        member->m_wRank = 0;

        if (!m_pEventModeMazeMatching->AutoMatchingEnter(member)) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching AutoMatchingEnter Fail Map(%d), EventID(%d), UCID(%u)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<unsigned int>(actorID));
            return;
        }

        partyUser->SetMatchingState(true);
        partyUser->SetMatchingID(0, 3u);
    }

    LogHelper::LogDebug("game.contents",
                        "EventModeMazeMatching Event - ( EventID %d / ModeMaze %d / Count %zu )",
                        eventInfo.nID,
                        eventInfo.nModeMazeID,
                        eventInfo.vecInfo.size());
}

void CModeMazeMatchingMgr::SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze) {
    if (stCreateModeMaze.dwEventRoomID != 0 && m_pEventModeMazeMatching) {
        m_pEventModeMazeMatching->SendCreateMatchingModeMaze(stCreateModeMaze);
        return;
    }

    const auto it = m_mapMatchingInfo.find(stCreateModeMaze.dwMatchingID);
    if (it != m_mapMatchingInfo.end() && it->second) {
        it->second->SendCreateMatchingModeMaze(stCreateModeMaze);
    }
}

void CModeMazeMatchingMgr::OnUpdate() {
    if (m_pEventModeMazeMatching && !m_pEventModeMazeMatching->OnUpdate()) {
        LogHelper::LogInfo("game.contents",
                           "Delete EventModeMazeMatching - ( ModeMaze %d / Process %d / State %d / EventID %u )",
                           static_cast<int>(m_wModeMazeID),
                           static_cast<int>(m_pEventModeMazeMatching->GetMatchingProcess()),
                           static_cast<int>(m_pEventModeMazeMatching->GetMatchingState()),
                           static_cast<unsigned int>(m_pEventModeMazeMatching->GetEventRoomID()));
        m_pEventModeMazeMatching.reset();
    }

    switch (m_eMatchingState) {
    case GREENDAMTAN_MODE_MAZE_MATCHING_WAIT:
        if (m_dw64UpdateTick < GreenDamTan_GetTickCount64()) {
            if (m_n64MatchingWaitRemain > 0 &&
                m_n64MatchingWaitRemain < static_cast<std::int64_t>(GreenDamTan_GetTickCount64())) {
                m_n64MatchingWaitRemain = 0;
                SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_MAKE_LIST);
                LogHelper::LogInfo("game.contents",
                                   "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                                   static_cast<int>(m_wModeMazeID),
                                   m_eMatchingState);
            }
            m_dw64UpdateTick = GreenDamTan_GetTickCount64() + 1000;
        }
        break;
    case GREENDAMTAN_MODE_MAZE_MATCHING_MAKE_LIST:
        if (m_dw64UpdateTick < GreenDamTan_GetTickCount64()) {
            SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_CREATE);
            LogHelper::LogInfo("game.contents",
                               "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                               static_cast<int>(m_wModeMazeID),
                               m_eMatchingState);
            ProcessWaitList();
            m_dw64UpdateTick = GreenDamTan_GetTickCount64() + 1000;
        }
        break;
    case GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_CREATE:
        ProcessMazeMake();
        break;
    case GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_DESTROY:
        DestroyMatchingWait();
        SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_NONE);
        LogHelper::LogInfo("game.contents",
                           "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                           static_cast<int>(m_wModeMazeID),
                           m_eMatchingState);
        break;
    default:
        break;
    }
}

void CModeMazeMatchingMgr::ProcessWaitList() {
    LogHelper::LogInfo("game.contents", "Start ProcessWaitList - ( ModeMaze %d )", static_cast<int>(m_wModeMazeID));

    const std::size_t waitCount = m_mapMatchingWait.size();
    if (waitCount < m_wMinEnterCount) {
        SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_DESTROY);
        LogHelper::LogInfo("game.contents",
                           "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                           static_cast<int>(m_wModeMazeID),
                           m_eMatchingState);
        LogHelper::LogError("game.contents", "ModeMaze::ProcessWaitList() Lack Wait Count(%d)", static_cast<int>(waitCount));
        return;
    }

    std::vector<std::shared_ptr<CModeMazeMatchginMember>> members;
    members.reserve(waitCount);
    for (const auto& [actorID, member] : m_mapMatchingWait) {
        static_cast<void>(actorID);
        if (member) {
            members.push_back(member);
        }
    }

    std::sort(members.begin(), members.end(), [](const auto& lhs, const auto& rhs) {
        if (!lhs || !rhs) {
            return static_cast<bool>(lhs);
        }
        if (lhs->GetRank() != rhs->GetRank()) {
            return lhs->GetRank() < rhs->GetRank();
        }
        return lhs->GetActorID() < rhs->GetActorID();
    });

    const int matchingCount = static_cast<int>((members.size() + m_wMaxEnterCount - 1) / m_wMaxEnterCount);
    int nLastMatchingMemberCount = static_cast<int>(members.size() % m_wMaxEnterCount);
    int nNeedLastMatchingMemberCount = 0;
    if (nLastMatchingMemberCount > 0 && nLastMatchingMemberCount < m_wMinEnterCount) {
        nNeedLastMatchingMemberCount = m_wMinEnterCount - nLastMatchingMemberCount;
        nLastMatchingMemberCount = m_wMinEnterCount;
    }

    LogHelper::LogInfo("game.contents",
                       "..ing ProcessWaitList - ( ModeMaze(%d) / match:%d / NeedLast:%d / NeedLast:%d )",
                       static_cast<int>(m_wModeMazeID),
                       matchingCount,
                       nLastMatchingMemberCount,
                       nNeedLastMatchingMemberCount);

    std::size_t cursor = 0;
    for (int matchingIndex = 1; matchingIndex <= matchingCount && cursor < members.size(); ++matchingIndex) {
        auto matching = std::make_shared<CModeMazeMatching>();
        ++m_dwMatchingID;
        if (!matching->AutoMatchingCreate(m_wModeMazeID, m_dwMatchingID, 0)) {
            SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_MAZE_DESTROY);
            LogHelper::LogInfo("game.contents",
                               "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                               static_cast<int>(m_wModeMazeID),
                               m_eMatchingState);
            LogHelper::LogError("game.contents", "MatchingCreate fail ModeMazeID:%d", static_cast<int>(m_wModeMazeID));
            return;
        }

        int maxEnterCount = m_wMaxEnterCount;
        if (nNeedLastMatchingMemberCount > 0 && matchingIndex == matchingCount - 1) {
            maxEnterCount = static_cast<int>(m_wMaxEnterCount) - nNeedLastMatchingMemberCount;
        } else if (nLastMatchingMemberCount > 0 && matchingIndex == matchingCount) {
            maxEnterCount = nLastMatchingMemberCount;
        }

        for (int j = 0; j < maxEnterCount && cursor < members.size(); ++j, ++cursor) {
            matching->AutoMatchingEnter(members[cursor]);
        }

        m_mapMatchingInfo[m_dwMatchingID] = matching;
        LogHelper::LogInfo("game.contents",
                           "..ing ProcessWaitList InsertMatchingInfo - ( ModeMaze(%d) / MatchingID:%d / Member:%d )",
                           static_cast<int>(m_wModeMazeID),
                           static_cast<int>(m_dwMatchingID),
                           matching->GetMemberCount());
    }

    m_mapMatchingWait.clear();
    LogHelper::LogInfo("game.contents", "End ProcessWaitList - ( ModeMaze %d )", static_cast<int>(m_wModeMazeID));
}

void CModeMazeMatchingMgr::ProcessMazeMake() {
    std::vector<std::uint32_t> deleteMatchingIDs;

    for (const auto& [matchingID, matching] : m_mapMatchingInfo) {
        if (!matching) {
            deleteMatchingIDs.push_back(matchingID);
            continue;
        }

        if (!matching->OnUpdate()) {
            LogHelper::LogInfo("game.contents",
                               "Delete ModeMazeMatching - ( ModeMaze %d / Process %d / State %d )",
                               static_cast<int>(m_wModeMazeID),
                               static_cast<int>(matching->GetMatchingProcess()),
                               static_cast<int>(matching->GetMatchingState()));
            deleteMatchingIDs.push_back(matchingID);
        }
    }

    for (std::uint32_t matchingID : deleteMatchingIDs) {
        m_mapMatchingInfo.erase(matchingID);
    }

    if (m_mapMatchingInfo.empty()) {
        DestroyMatchingWait();
        SetMatchingState(GREENDAMTAN_MODE_MAZE_MATCHING_NONE);
        LogHelper::LogInfo("game.contents",
                           "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                           static_cast<int>(m_wModeMazeID),
                           m_eMatchingState);
    }
}

void CModeMazeMatchingMgr::DestroyMatchingWait() {
    m_mapMatchingInfo.clear();
    m_mapMatchingWait.clear();
    m_n64MatchingWaitRemain = 0;
    m_dw64UpdateTick = 0;
    m_wModeMazeID = 0;
    m_wMaxEnterCount = 8;
    m_wMinEnterCount = 4;
    LogHelper::LogInfo("game.contents",
                       "ModeMazeMatching DestroyMatching - ( ModeMaze %d / State %d )",
                       static_cast<int>(m_wModeMazeID),
                       m_eMatchingState);
}

void CModeMazeMatchingMgr::SetMatchingState(int state) {
    m_eMatchingState = state;
}
