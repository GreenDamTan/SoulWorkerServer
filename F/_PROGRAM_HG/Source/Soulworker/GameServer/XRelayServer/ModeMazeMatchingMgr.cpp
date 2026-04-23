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

bool CModeMazeMatchingMgr::FindModeMazeMatching(std::uint32_t dwActorID) {
    return m_mapMatchingWait.find(dwActorID) != m_mapMatchingWait.end();
}

bool CModeMazeMatchingMgr::CheckModeMazeOpenTime(std::uint16_t wModeMazeID) {
    // 对齐 IDA 0x1400373B0: 检查运营活动时间窗口
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    TB_OPERATION_INFO* pTB_OPERATION_INFO = relayServer->GetResourceMgr().GetTB_OPERATION_INFO(wModeMazeID);
    if (!pTB_OPERATION_INFO) {
        return false;
    }

    // 设置成员数量参数
    if (m_wModeMazeID == 0) {
        m_wModeMazeID = wModeMazeID;
        m_wMaxEnterCount = static_cast<std::uint16_t>(pTB_OPERATION_INFO->Max_Member);
        m_wMinEnterCount = static_cast<std::uint16_t>(pTB_OPERATION_INFO->Min_Member);
    }

    if (m_wModeMazeID != wModeMazeID) {
        LogHelper::LogError("game.contents",
                            "CheckModeMazeOpenTime - MazeID Error( ModeMaze %d/%d )",
                            static_cast<int>(m_wModeMazeID),
                            static_cast<int>(wModeMazeID));
        return false;
    }

    // 如果已在等待状态，返回 true
    if (m_eMatchingState == GREENDAMTAN_MODE_MAZE_MATCHING_WAIT) {
        return true;
    }

    // 如果状态不为 NONE，返回 false
    if (m_eMatchingState != GREENDAMTAN_MODE_MAZE_MATCHING_NONE) {
        return false;
    }

    // 获取当前时间
    const std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));
    std::tm tmCurr{};
#ifdef _WIN32
    localtime_s(&tmCurr, &tCurr);
#else
    localtime_r(&tCurr, &tmCurr);
#endif

    // 辅助 lambda: 检查单个 HotTime 窗口
    auto checkHotTime = [&](int nStart, int nEnd) -> bool {
        if (nStart <= 0 || nEnd <= 0) {
            return false;
        }
        const int nStartHour = nStart / 60;
        const int nStartMin = nStart % 60;
        const int nEndHour = nEnd / 60;
        const int nEndMin = nEnd % 60;

        // 验证时间值有效性
        if (nStartHour >= 24 || nStartMin >= 60 || nEndHour >= 24 || nEndMin >= 60) {
            LogHelper::LogError("game.contents",
                                "CheckModeMazeOpenTime - HotTime Error( ModeMaze %d )",
                                static_cast<int>(wModeMazeID));
            return false;
        }

        // 构造今天的开始和结束时间戳
        std::tm tmStart = tmCurr;
        tmStart.tm_hour = nStartHour;
        tmStart.tm_min = nStartMin;
        tmStart.tm_sec = 0;
        const std::time_t tStart = std::mktime(&tmStart);

        std::tm tmEnd = tmCurr;
        tmEnd.tm_hour = nEndHour;
        tmEnd.tm_min = nEndMin;
        tmEnd.tm_sec = 0;
        const std::time_t tEnd = std::mktime(&tmEnd);

        // 检查当前时间是否在窗口内
        if (tCurr >= tStart && tCurr < tEnd) {
            m_n64MatchingWaitRemain = static_cast<std::int64_t>(tEnd);
            m_eMatchingState = GREENDAMTAN_MODE_MAZE_MATCHING_WAIT;
            LogHelper::LogInfo("game.contents",
                               "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                               static_cast<int>(wModeMazeID),
                               static_cast<int>(m_eMatchingState));
            LogHelper::LogInfo("game.contents",
                               "Set ModeMazeOpenTime - ( ModeMaze %d / %dH %dM %dS )",
                               static_cast<int>(wModeMazeID),
                               nEndHour, nEndMin, 0);
            return true;
        }
        return false;
    };

    // 检查三个 HotTime 窗口
    if (checkHotTime(pTB_OPERATION_INFO->HotTime_Start_1st, pTB_OPERATION_INFO->HotTime_End_1st)) {
        return true;
    }
    if (checkHotTime(pTB_OPERATION_INFO->HotTime_Start_2nd, pTB_OPERATION_INFO->HotTime_End_2nd)) {
        return true;
    }
    if (checkHotTime(pTB_OPERATION_INFO->HotTime_Start_3rd, pTB_OPERATION_INFO->HotTime_End_3rd)) {
        return true;
    }

    return false;
}

bool CModeMazeMatchingMgr::EnterMatching(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq,
                                         CServer* pServer) {
    if (!pServer) {
        return false;
    }

    PS_MODE_MAZE_MATCHING_ENTER_RES stResult{};
    stResult.dwActorID = enterReq.stMemberInfo.dwActorID;
    stResult.wModeMazeID = enterReq.wModeMazeID;

    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    std::shared_ptr<CUserPartyInfo> pUserParty = relayServer->GetPartyUser(stResult.dwActorID);

    // 检查用户是否存在
    if (!pUserParty) {
        stResult.nError = 51001;
        XSendPacket packet(0xFDu, 1u);
        packet << stResult;
        pServer->SendEx(packet);
        return false;
    }

    // 检查奖励领取状态
    if (pUserParty->GetRewardState() != 0) {
        stResult.nError = 53206;
        XSendPacket packet(0xFDu, 1u);
        packet << stResult;
        pServer->SendEx(packet);
        return false;
    }

    // 检查是否已在匹配中
    if (FindModeMazeMatching(stResult.dwActorID)) {
        stResult.nError = 53206;
        XSendPacket packet(0xFDu, 1u);
        packet << stResult;
        pServer->SendEx(packet);
        return false;
    }

    // 检查模式迷宫开放时间
    if (!CheckModeMazeOpenTime(enterReq.wModeMazeID)) {
        stResult.nError = 53213;
        XSendPacket packet(0xFDu, 1u);
        packet << stResult;
        pServer->SendEx(packet);
        return false;
    }

    // 添加到等待列表
    int nError = 0;
    if (AddModeMazeMatchingWait(enterReq, nError, pServer)) {
        pUserParty->SetMatchingState(1);
        pUserParty->SetMatchingID(0, 3u);
        XSendPacket packet(0xFDu, 1u);
        packet << stResult;
        pServer->SendEx(packet);
        return true;
    }

    XSendPacket packet(0xFDu, 1u);
    packet << stResult;
    pServer->SendEx(packet);
    return false;
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
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();

    // 循环1: 遍历所有匹配，向成员发送退出包
    for (const auto& [matchingID, matching] : m_mapMatchingInfo) {
        if (!matching) {
            continue;
        }

        // 获取成员列表
        std::vector<std::uint32_t> vecMember;
        for (const auto& member : matching->m_listMatchingUser) {
            if (member && member->GetActorID() != 0) {
                vecMember.push_back(member->GetActorID());
            }
        }

        // 向每个成员发送退出包
        for (std::uint32_t actorID : vecMember) {
            auto waitIt = m_mapMatchingWait.find(actorID);
            if (waitIt == m_mapMatchingWait.end() || !waitIt->second || !waitIt->second->m_pCurServer) {
                continue;
            }

            const auto& member = waitIt->second;
            PS_MODE_MAZE_MATCHING_EXIT psExit{};
            psExit.dwExitUCID = member->GetActorID();
            psExit.dwExitUAID = member->GetUAID();
            psExit.byReason = 2;

            XSendPacket packet(0xFDu, 3u);
            packet.XParse << member->GetActorID();
            packet << psExit;
            member->m_pCurServer->SendEx(packet);

            // 记录DB日志
            relayServer->SendDBLog(static_cast<int>(member->GetUAID()),
                                   static_cast<int>(member->GetActorID()),
                                   28, 2,
                                   0, static_cast<int>(m_wModeMazeID),
                                   0, 0, static_cast<int>(psExit.byReason),
                                   0, 0, L"");

            // 更新用户状态
            std::shared_ptr<CUserPartyInfo> pUserParty = relayServer->GetPartyUser(actorID);
            if (pUserParty) {
                pUserParty->SetMatchingState(0);
                pUserParty->SetMatchingID(0, 0);
            }

            m_mapMatchingWait.erase(waitIt);
        }
    }

    m_mapMatchingInfo.clear();

    // 循环2: 处理剩余的等待用户
    for (const auto& [actorID, member] : m_mapMatchingWait) {
        if (!member || !member->m_pCurServer) {
            continue;
        }

        PS_MODE_MAZE_MATCHING_EXIT psExit{};
        psExit.dwExitUCID = member->GetActorID();
        psExit.dwExitUAID = member->GetUAID();
        psExit.byReason = 2;

        XSendPacket packet(0xFDu, 3u);
        packet.XParse << member->GetActorID();
        packet << psExit;
        member->m_pCurServer->SendEx(packet);

        // 记录DB日志
        relayServer->SendDBLog(static_cast<int>(member->GetUAID()),
                               static_cast<int>(member->GetActorID()),
                               28, 2,
                               0, static_cast<int>(m_wModeMazeID),
                               0, 0, static_cast<int>(psExit.byReason),
                               0, 0, L"");

        // 更新用户状态
        std::shared_ptr<CUserPartyInfo> pUserParty = relayServer->GetPartyUser(actorID);
        if (pUserParty) {
            pUserParty->SetMatchingState(0);
            pUserParty->SetMatchingID(0, 0);
        }
    }

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
