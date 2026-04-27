#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"

#include <algorithm>
#include <chrono>
#include <cwchar>
#include <random>
#include <vector>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

namespace {
std::uint64_t GreenDamTan_GetTickCount64() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}
}

CModeMazeMatchingMgr& CModeMazeMatchingMgr::Instance() {
    static CModeMazeMatchingMgr manager;
    return manager;
}

// 对齐 IDA 0x140037D90: ?AddModeMazeMatchingWait@CModeMazeMatchingMgr@@QEAA_NAEAUPS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ@@AEAHPEAVCServer@@@Z
bool CModeMazeMatchingMgr::AddModeMazeMatchingWait(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& enterReq,
                                                   int& nError,
                                                   CServer* pServer) {
    if (!pServer) {
        nError = 53201;
        return false;
    }

    auto member = std::make_shared<CModeMazeMatchginMember>(pServer);
    if (!member) {
        nError = 53201;
        LogHelper::LogError("game.contents",
                            "ModeMaze::AddModeMazeMatchingWait() pMember Is NULL Map(%d), UAID(%d), UCID(%d)",
                            static_cast<int>(enterReq.wModeMazeID),
                            static_cast<int>(enterReq.stMemberInfo.dwUAID),
                            static_cast<int>(enterReq.stMemberInfo.dwActorID));
        return false;
    }

    member->m_stMemberInfo = enterReq.stMemberInfo;
    member->SetRank(enterReq.wRank);  // 对齐 IDA: 使用 SetRank，零值转为哨兵
    m_mapMatchingWait[enterReq.stMemberInfo.dwActorID] = member;
    LogHelper::LogDebug("game.contents",
                        "ModeMaze::AddModeMazeMatchingWait() AddWait UCID(%u)",
                        static_cast<unsigned int>(enterReq.stMemberInfo.dwActorID));

    // 对齐 IDA: 添加 DB 日志 (main=28, sub=1)
    XRelayServer* pRelayServer = TXSingleton<XRelayServer>::Instance();
    pRelayServer->SendDBLog(
        static_cast<int>(enterReq.stMemberInfo.dwUAID),
        static_cast<int>(enterReq.stMemberInfo.dwActorID),
        28, 1,
        0,
        static_cast<int>(enterReq.wModeMazeID),
        0, 0, 0, 0, 0, L"");

    nError = 0;
    return true;
}

bool CModeMazeMatchingMgr::FindModeMazeMatching(std::uint32_t dwActorID) {
    return m_mapMatchingWait.find(dwActorID) != m_mapMatchingWait.end();
}

bool CModeMazeMatchingMgr::CheckModeMazeOpenTime(std::uint16_t wModeMazeID) {
    // 对齐 IDA 0x1400373B0: 检查运营活动时间窗口
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    // TODO: 推测结果 - 原始 IDA 调用 GetOperationInfoTable(wModeMazeID, nWorldID) 带两个参数
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
    if (m_eMatchingState == eMODE_MAZE_MATCHING_STATE::WAIT) {
        return true;
    }

    // 如果状态不为 NONE，返回 false
    if (m_eMatchingState != eMODE_MAZE_MATCHING_STATE::NONE) {
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
            m_eMatchingState = eMODE_MAZE_MATCHING_STATE::WAIT;
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

// 对齐 IDA 0x1400395C0: ?ExitMatching@CModeMazeMatchingMgr@@QEAA_NAEAUPS_MODE_MAZE_MATCHING_EXIT@@@Z
bool CModeMazeMatchingMgr::ExitMatching(PS_MODE_MAZE_MATCHING_EXIT& stExit) {
    XRelayServer* pRelayServer = TXSingleton<XRelayServer>::Instance();
    std::shared_ptr<CUserPartyInfo> pUser = pRelayServer->GetPartyUser(stExit.dwExitUCID);

    bool bSendPacket = false;

    // 对齐 IDA: 检查用户存在且奖励状态非零
    if (pUser && pUser->GetRewardState() != 0) {
        // 对齐 IDA: 检查匹配状态为 MAZE_CREATE (3)
        if (pUser->GetMatchingState() == 3) {
            bSendPacket = true;

            // 对齐 IDA: 从用户获取 MatchingID 并查找对应的 CModeMazeMatching
            std::uint32_t dwMatchingID = pUser->GetMatchingID();
            auto it = m_mapMatchingInfo.find(dwMatchingID);
            if (it != m_mapMatchingInfo.end() && it->second) {
                // 对齐 IDA: 调用 CModeMazeMatching::ExitMatching，原因硬编码为 3
                it->second->ExitMatching(stExit.dwExitUCID, stExit.dwExitUAID, 3);
            }
        }
    }

    // 对齐 IDA: 查找 m_mapMatchingWait 中的成员
    auto waitIt = m_mapMatchingWait.find(stExit.dwExitUCID);
    if (waitIt != m_mapMatchingWait.end() && waitIt->second) {
        std::shared_ptr<CModeMazeMatchginMember> pMember = waitIt->second;

        // 对齐 IDA: 如果未发送包，在此发送退出包和 DB 日志
        if (!bSendPacket) {
            stExit.byReason = 3;

            XSendPacket packet(0xFDu, 3u);
            packet.XParse << pMember->GetActorID();
            packet << stExit;

            if (pMember->m_pCurServer) {
                pMember->m_pCurServer->SendEx(packet);
            }

            // 对齐 IDA: 发送 DB 日志 (main=28, sub=2)
            pRelayServer->SendDBLog(
                static_cast<int>(pMember->GetUAID()),
                static_cast<int>(pMember->GetActorID()),
                28, 2,
                0,
                static_cast<int>(m_wModeMazeID),
                0, 0,
                static_cast<int>(stExit.byReason),
                0, 0, L"");
        }

        m_mapMatchingWait.erase(waitIt);
    }

    // 对齐 IDA: 清理用户匹配状态
    if (pUser) {
        pUser->SetMatchingState(false);
        pUser->SetMatchingID(0, 0);
    }

    return true;
}

void CModeMazeMatchingMgr::MatchingRemoveUser(std::uint32_t dwUCID, std::uint32_t dwUAID) {
    PS_MODE_MAZE_MATCHING_EXIT exitInfo{};
    exitInfo.dwExitUCID = dwUCID;
    exitInfo.dwExitUAID = dwUAID;
    ExitMatching(exitInfo);
}

// 对齐 IDA 0x140039C60: ?ModeMazeMatchingEvent@CModeMazeMatchingMgr@@QEAAXAEAUPS_SERVER_MODE_MAZE_MATCHING_EVENT@@@Z
void CModeMazeMatchingMgr::ModeMazeMatchingEvent(PS_SERVER_MODE_MAZE_MATCHING_EVENT& eventInfo) {
    // 对齐 IDA: 检查是否已有事件匹配
    if (m_pEventModeMazeMatching) {
        LogHelper::LogInfo("game.contents",
                           "Already EventModeMazeMatching - ( MatchingID %d / EventID %d )",
                           static_cast<int>(m_dwMatchingID),
                           static_cast<int>(m_pEventModeMazeMatching->GetEventRoomID()));
        return;
    }

    // 对齐 IDA: 创建新的匹配实例
    m_pEventModeMazeMatching = std::make_shared<CModeMazeMatching>();
    ++m_dwMatchingID;
    if (!m_pEventModeMazeMatching->AutoMatchingCreate(static_cast<std::uint16_t>(eventInfo.nModeMazeID),
                                                       m_dwMatchingID,
                                                       static_cast<std::uint32_t>(eventInfo.nID))) {
        LogHelper::LogInfo("game.contents",
                           "Fail Create EventModeMazeMatching - ( MatchingID %d / EventID %d )",
                           static_cast<int>(eventInfo.nModeMazeID),
                           static_cast<int>(eventInfo.nID));
        m_pEventModeMazeMatching.reset();
        return;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (const unsigned long actorID : eventInfo.vecInfo) {
        // 对齐 IDA: 获取用户对象
        const std::shared_ptr<CUserObject> user = relayServer.GetUser(static_cast<std::uint32_t>(actorID));
        if (!user) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Is NULL Map(%d), EventID(%d), UCID(%d)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<int>(actorID));
            return;
        }

        // 对齐 IDA: 获取用户Party信息
        const std::shared_ptr<CUserPartyInfo> partyUser = relayServer.GetPartyUser(static_cast<std::uint32_t>(actorID));
        if (!partyUser) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Is NULL Map(%d), EventID(%d), UCID(%d)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<int>(actorID));
            return;
        }

        // 对齐 IDA: 获取用户所在服务器
        CServer* memberServer = relayServer.GetServer(user->GetServerID());
        if (!memberServer) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Server Is NULL Map(%d), EventID(%d), UCID(%d)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<int>(actorID));
            return;
        }

        // 对齐 IDA: 检查奖励状态（非GetMatchingState）
        if (partyUser->GetRewardState() != 0) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Matching State Error Map(%d), EventID(%d), UCID(%d)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<int>(actorID));
            return;
        }

        // 对齐 IDA: 检查用户是否已在等待列表
        if (FindModeMazeMatching(static_cast<std::uint32_t>(actorID))) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching User Already Wait Map(%d), EventID(%d), UCID(%d)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<int>(actorID));
            return;
        }

        // 对齐 IDA: 创建成员对象
        auto member = std::make_shared<CModeMazeMatchginMember>(memberServer);
        if (!member) {
            LogHelper::LogError("game.contents",
                                "EventModeMazeMatching pMember Is NULL Map(%d), EventID(%d), UCID(%d)",
                                eventInfo.nModeMazeID,
                                eventInfo.nID,
                                static_cast<int>(actorID));
            return;
        }

        // 对齐 IDA: 填充成员信息
        member->m_pCurServer = memberServer;
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
        member->SetRank(0);  // 对齐 IDA: 事件成员 rank 设为 0，转为哨兵值 0xFA00

        // 对齐 IDA: 加入匹配
        std::shared_ptr<CModeMazeMatchginMember> pMemberShared = member;
        m_pEventModeMazeMatching->AutoMatchingEnter(pMemberShared);

        // 对齐 IDA: 更新用户状态
        partyUser->SetMatchingState(1);
        partyUser->SetMatchingID(0, 3u);

        // 对齐 IDA: 发送响应包
        PS_MODE_MAZE_MATCHING_ENTER_RES stResult{};
        stResult.dwActorID = user->GetMatchingID();
        stResult.wModeMazeID = static_cast<std::uint16_t>(eventInfo.nModeMazeID);

        XSendPacket packet(0xFDu, 1u);
        packet << stResult;
        memberServer->SendEx(packet);
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
    case eMODE_MAZE_MATCHING_STATE::WAIT:
        if (m_dw64UpdateTick < GreenDamTan_GetTickCount64()) {
            if (m_n64MatchingWaitRemain > 0 &&
                m_n64MatchingWaitRemain < static_cast<std::int64_t>(GreenDamTan_GetTickCount64())) {
                m_n64MatchingWaitRemain = 0;
                SetMatchingState(eMODE_MAZE_MATCHING_STATE::MAKE_LIST);
                LogHelper::LogInfo("game.contents",
                                   "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                                   static_cast<int>(m_wModeMazeID),
                                   static_cast<int>(m_eMatchingState));
            }
            m_dw64UpdateTick = GreenDamTan_GetTickCount64() + 1000;
        }
        break;
    case eMODE_MAZE_MATCHING_STATE::MAKE_LIST:
        if (m_dw64UpdateTick < GreenDamTan_GetTickCount64()) {
            SetMatchingState(eMODE_MAZE_MATCHING_STATE::MAZE_CREATE);
            LogHelper::LogInfo("game.contents",
                               "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                               static_cast<int>(m_wModeMazeID),
                               static_cast<int>(m_eMatchingState));
            ProcessWaitList();
            m_dw64UpdateTick = GreenDamTan_GetTickCount64() + 1000;
        }
        break;
    case eMODE_MAZE_MATCHING_STATE::MAZE_CREATE:
        ProcessMazeMake();
        break;
    case eMODE_MAZE_MATCHING_STATE::MAZE_DESTROY:
        DestroyMatchingWait();
        SetMatchingState(eMODE_MAZE_MATCHING_STATE::NONE);
        LogHelper::LogInfo("game.contents",
                           "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                           static_cast<int>(m_wModeMazeID),
                           static_cast<int>(m_eMatchingState));
        break;
    default:
        break;
    }
}

// 对齐 IDA 0x140037FF0: ?ProcessWaitList@CModeMazeMatchingMgr@@QEAAXXZ
void CModeMazeMatchingMgr::ProcessWaitList() {
    LogHelper::LogInfo("game.contents", "Start ProcessWaitList - ( ModeMaze %d )", static_cast<int>(m_wModeMazeID));

    const std::size_t waitCount = m_mapMatchingWait.size();
    if (waitCount < m_wMinEnterCount) {
        SetMatchingState(eMODE_MAZE_MATCHING_STATE::MAZE_DESTROY);
        LogHelper::LogInfo("game.contents",
                           "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                           static_cast<int>(m_wModeMazeID),
                           static_cast<int>(m_eMatchingState));
        LogHelper::LogError("game.contents", "ModeMaze::ProcessWaitList() Lack Wait Count(%d)", static_cast<int>(waitCount));
        return;
    }

    // 对齐 IDA: 按 Rank 排序的成员列表
    std::vector<std::shared_ptr<CModeMazeMatchginMember>> members;
    members.reserve(waitCount);
    for (const auto& [actorID, member] : m_mapMatchingWait) {
        static_cast<void>(actorID);
        if (member) {
            members.push_back(member);
        }
    }

    // 对齐 IDA: 按 Rank 排序（低 Rank 优先）
    std::sort(members.begin(), members.end(), [](const auto& lhs, const auto& rhs) {
        if (!lhs || !rhs) {
            return static_cast<bool>(lhs);
        }
        if (lhs->GetRank() != rhs->GetRank()) {
            return lhs->GetRank() < rhs->GetRank();
        }
        return lhs->GetActorID() < rhs->GetActorID();
    });

    // 对齐 IDA: 计算匹配数量（整数除法 + 余数检查）
    int nMatchingCount = static_cast<int>(members.size() / m_wMaxEnterCount);
    int nLastMatchingMemberCount = static_cast<int>(members.size() % m_wMaxEnterCount);
    int nNeedLastMatchingMemberCount = 0;
    if (nLastMatchingMemberCount > 0) {
        ++nMatchingCount;
        if (nLastMatchingMemberCount < m_wMinEnterCount) {
            nNeedLastMatchingMemberCount = m_wMinEnterCount - nLastMatchingMemberCount;
            nLastMatchingMemberCount = m_wMinEnterCount;
        }
    }

    LogHelper::LogInfo("game.contents",
                       "..ing ProcessWaitList - ( ModeMaze(%d) / match:%d / NeedLast:%d / NeedLast:%d )",
                       static_cast<int>(m_wModeMazeID),
                       nMatchingCount,
                       nLastMatchingMemberCount,
                       nNeedLastMatchingMemberCount);

    // 对齐 IDA: 40 成员排名优先逻辑
    std::vector<std::shared_ptr<CModeMazeMatchginMember>> vecRankList;
    std::size_t nVecCount = 0;

    if (members.size() >= 40) {
        // 对齐 IDA: 取前 40 个按 Rank 排序的成员
        for (int i = 0; i < 40 && static_cast<std::size_t>(i) < members.size(); ++i) {
            vecRankList.push_back(members[i]);
        }

        if (vecRankList.size() == 40) {
            // 对齐 IDA: 对这 40 个排名成员进行随机洗牌
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(vecRankList.begin(), vecRankList.end(), g);
        } else {
            // 对齐 IDA: 如果不足 40 个，清空并重新从 Rank 排序开始
            vecRankList.clear();
        }
    }

    LogHelper::LogInfo("game.contents",
                       "..ing ProcessWaitList - ( ModeMaze(%d) / ranker:%zu )",
                       static_cast<int>(m_wModeMazeID),
                       vecRankList.size());

    // 对齐 IDA: 创建匹配并分配成员
    std::size_t orderedIndex = vecRankList.size();  // 从 Rank 排序列表的 ranker 数量之后开始
    for (int matchingIndex = 1; matchingIndex <= nMatchingCount; ++matchingIndex) {
        auto matching = std::make_shared<CModeMazeMatching>();
        ++m_dwMatchingID;
        if (!matching->AutoMatchingCreate(m_wModeMazeID, m_dwMatchingID, 0)) {
            SetMatchingState(eMODE_MAZE_MATCHING_STATE::MAZE_DESTROY);
            LogHelper::LogInfo("game.contents",
                               "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                               static_cast<int>(m_wModeMazeID),
                               static_cast<int>(m_eMatchingState));
            LogHelper::LogError("game.contents", "MatchingCreate fail ModeMazeID:%d", static_cast<int>(m_wModeMazeID));
            return;
        }

        int wMaxEnterCount = m_wMaxEnterCount;
        if (nNeedLastMatchingMemberCount > 0 && matchingIndex == nMatchingCount - 1) {
            wMaxEnterCount = m_wMaxEnterCount - nNeedLastMatchingMemberCount;
        } else if (nLastMatchingMemberCount > 0 && matchingIndex == nMatchingCount) {
            wMaxEnterCount = nLastMatchingMemberCount;
        }

        for (int j = 0; j < wMaxEnterCount; ++j) {
            // 对齐 IDA: 优先从 vecRankList 分配，然后从 members 继续分配
            if (nVecCount < vecRankList.size()) {
                matching->AutoMatchingEnter(vecRankList[nVecCount]);
                ++nVecCount;
            } else if (orderedIndex < members.size()) {
                matching->AutoMatchingEnter(members[orderedIndex]);
                ++orderedIndex;
            } else {
                break;
            }
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
            // 对齐 IDA: 日志第一个参数是 matchingID (GetMatchingID), 不是 m_wModeMazeID
            LogHelper::LogInfo("game.contents",
                               "Delete ModeMazeMatching - ( ModeMaze %d / Process %d / State %d )",
                               static_cast<int>(matching->GetMatchingID()),
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
        SetMatchingState(eMODE_MAZE_MATCHING_STATE::NONE);
        LogHelper::LogInfo("game.contents",
                           "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                           static_cast<int>(m_wModeMazeID),
                           static_cast<int>(m_eMatchingState));
    }
}

void CModeMazeMatchingMgr::DestroyMatchingWait() {
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();

    // 循环1: 遍历所有匹配，向成员发送退出包
    for (const auto& [matchingID, matching] : m_mapMatchingInfo) {
        if (!matching) {
            continue;
        }

        // 获取成员列表 (对齐 IDA: 使用 GetMatchingMember 方法)
        std::vector<std::uint32_t> vecMember;
        matching->GetMatchingMember(vecMember);

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
                       static_cast<int>(m_eMatchingState));
}

// 对齐 IDA: 使用 eMODE_MAZE_MATCHING_STATE 枚举
void CModeMazeMatchingMgr::SetMatchingState(eMODE_MAZE_MATCHING_STATE state) {
    m_eMatchingState = state;
}

// 对齐 IDA 0x140039B30: GM作弊函数，设置活动时间
// TODO: 推测结果 - 原始 IDA 调用 GetOperationInfoTable(nID, nWorldID) 带两个参数
void CModeMazeMatchingMgr::ModeMazeTime_Cheat(int nID, int nStart, int nEnd) {
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    int nWorldID = static_cast<int>(relayServer->GetOption().GetGroupID());
    (void)nWorldID;

    TB_OPERATION_INFO* pTB_OPERATION_INFO = relayServer->GetResourceMgr().GetTB_OPERATION_INFO(static_cast<unsigned int>(nID));

    if (!pTB_OPERATION_INFO) {
        return;
    }

    pTB_OPERATION_INFO->HotTime_Start_1st = nStart;
    pTB_OPERATION_INFO->HotTime_End_1st = nEnd;

    DestroyMatchingWait();
    SetMatchingState(eMODE_MAZE_MATCHING_STATE::NONE);

    LogHelper::LogInfo("game.contents",
                       "Change ModeMazeMatching State - ( ModeMaze %d / State %d )",
                       static_cast<int>(m_wModeMazeID),
                       static_cast<int>(m_eMatchingState));

    PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO psInfo{};
    psInfo.dwModeMazeID = pTB_OPERATION_INFO->ID;
    psInfo.nHotTime_Start_1st = pTB_OPERATION_INFO->HotTime_Start_1st;
    psInfo.nHotTime_End_1st = pTB_OPERATION_INFO->HotTime_End_1st;
    psInfo.nHotTime_Start_2nd = pTB_OPERATION_INFO->HotTime_Start_2nd;
    psInfo.nHotTime_End_2nd = pTB_OPERATION_INFO->HotTime_End_2nd;
    psInfo.nHotTime_Start_3rd = pTB_OPERATION_INFO->HotTime_Start_3rd;
    psInfo.nHotTime_End_3rd = pTB_OPERATION_INFO->HotTime_End_3rd;

    relayServer->SendOperationTimeInfo(psInfo);
}
