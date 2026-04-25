#include "Soulworker/GameServer/XRelayServer/ModeMazeMatching.h"

#include <algorithm>
#include <chrono>
#include <random>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

namespace {
std::uint64_t GreenDamTan_GetTickCount64() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}
}

// 对齐 IDA 0x140032B00: ?AutoMatchingCreate@CModeMazeMatching@@QEAA_NGKK@Z
bool CModeMazeMatching::AutoMatchingCreate(std::uint16_t wMapID,
                                           std::uint32_t dwMatchingID,
                                           std::uint32_t dwEventRoomID) {
    XRelayServer* pRelayServer = TXSingleton<XRelayServer>::Instance();

    // 对齐 IDA: 先验证 MazeID 是否有效
    if (!pRelayServer->GetResourceMgr().GetTB_MAZE_INFO(wMapID)) {
        return false;
    }

    // 对齐 IDA: 从 TB_OPERATION_INFO 读取成员数和等待时间
    // TODO: 推测结果 - 原始 IDA 调用 GetOperationInfoTable(wMapID, nWorldID) 带两个参数
    // 当前重建使用 GetTB_OPERATION_INFO(wMapID) 单参数查找
    int nWorldID = static_cast<int>(pRelayServer->GetOption().GetGroupID());
    (void)nWorldID;

    TB_OPERATION_INFO* pTB_OPERATION_INFO = pRelayServer->GetResourceMgr().GetTB_OPERATION_INFO(static_cast<unsigned int>(wMapID));
    if (!pTB_OPERATION_INFO) {
        return false;
    }

    m_dwMatchingID = dwMatchingID;
    m_wMapID = wMapID;
    // 对齐 IDA: 从表读取 Min_Member / Max_Member
    m_nMinMember = pTB_OPERATION_INFO->Min_Member;
    m_nMaxMember = pTB_OPERATION_INFO->Max_Member;
    // 对齐 IDA: m_dw64WaitTime = 1000 * Matching_Wating_Time
    m_dw64WaitTime = 1000 * pTB_OPERATION_INFO->Matching_Wating_Time;
    m_dw64CheckTick = GreenDamTan_GetTickCount64() + 1000;
    SetMatchingState(0);
    m_dwEventRoomID = dwEventRoomID;
    LogHelper::LogInfo("game.contents",
                       "ModeMazeMatching MatchingCreate - ( MatchingIID %d / ModeMaze %d )",
                       static_cast<int>(m_dwMatchingID),
                       static_cast<int>(m_wMapID));
    return true;
}

// 对齐 IDA: 按值传递 shared_ptr
bool CModeMazeMatching::AutoMatchingEnter(std::shared_ptr<CModeMazeMatchginMember> pMember) {
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

// 对齐 IDA 0x140033AA0: ?MakeOperationMaze@CModeMazeMatching@@QEAA_NXZ
bool CModeMazeMatching::MakeOperationMaze() {
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching MakeOperationMaze Start - ( MatchingID %d )",
                        static_cast<int>(m_dwMatchingID));

    m_byProcess = 2;

    // 对齐 IDA: 先检查 E_SERVER_OPTION_OPERATION_MAZE 是否启用
    XRelayServer* pRelayServer = TXSingleton<XRelayServer>::Instance();
    if (!pRelayServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_OPERATION_MAZE)) {
        SendMatchingExit(0, 1);
        SetMatchingState(2);
        m_byProcess = 0;
        return false;
    }

    if (GetMemberCount() < m_nMinMember) {
        SendMatchingExit(0, 2);
        SetMatchingState(2);
        m_byProcess = 0;
        return false;
    }

    // 对齐 IDA: 获取 GroupID 作为 nWorldID 参数
    int nWorldID = static_cast<int>(pRelayServer->GetOption().GetGroupID());
    (void)nWorldID;  // TODO: 推测结果 - 当前 GetTB_OPERATION_INFO 不接受 worldID 参数

    // 对齐 IDA: 查找 TB_OPERATION_INFO 表
    // TODO: 推测结果 - 原始 IDA 调用 GetOperationInfoTable(dwMapID, nWorldID) 带两个参数
    // 当前重建使用 GetTB_OPERATION_INFO(dwMapID) 单参数查找
    TB_OPERATION_INFO* pTB_OPERATION_INFO = pRelayServer->GetResourceMgr().GetTB_OPERATION_INFO(static_cast<unsigned int>(m_wMapID));
    if (!pTB_OPERATION_INFO) {
        LogHelper::LogError("game.contents", "TB_OPERATION_INFO Error (%d)", static_cast<int>(m_wMapID));
        return false;
    }

    // 对齐 IDA: 收集跳点 ID 并随机打乱
    std::vector<int> vecJumpBoxID;
    for (int i = 0; i < pTB_OPERATION_INFO->Max_Member; ++i) {
        int nJumpID = (&pTB_OPERATION_INFO->User_Start_Point_1)[i];
        vecJumpBoxID.push_back(nJumpID);
    }
    // 对齐 IDA: 原始使用 std::random_shuffle (C++14 已弃用)，等效为 std::shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vecJumpBoxID.begin(), vecJumpBoxID.end(), g);

    ST_CREATE_MODE_MAZE stCreateModeMaze{};
    stCreateModeMaze.wReqMapID = m_wMapID;
    stCreateModeMaze.dwMatchingID = m_dwMatchingID;
    stCreateModeMaze.bHotTime = false;
    stCreateModeMaze.dwEventRoomID = m_dwEventRoomID;

    // 对齐 IDA: 按 serverID 统计成员数 (原始类型 map<ulong, short>)
    std::map<std::uint32_t, short> mapServerIDList;
    int sh = 0;

    for (const auto& pMember : m_listMatchingUser) {
        if (sh >= pTB_OPERATION_INFO->Max_Member) {
            break;
        }

        if (!pMember) {
            ++sh;
            continue;
        }

        // 对齐 IDA: 从随机打乱的跳点数组分配 JumpID
        pMember->m_stMemberInfo.nFirstJumpID = vecJumpBoxID[sh];
        stCreateModeMaze.vecEnterMember.push_back(pMember->m_stMemberInfo);

        if (pMember->m_pCurServer) {
            std::uint32_t dwServerID = pMember->m_pCurServer->GetServerID();
            auto itMap = mapServerIDList.find(dwServerID);
            if (itMap != mapServerIDList.end()) {
                ++itMap->second;
            } else {
                mapServerIDList[dwServerID] = 1;
            }
        }

        ++sh;
    }

    // 对齐 IDA: 找出成员最多的 serverID 作为 MasterServerID
    short shTopCount = 0;
    for (const auto& [dwServerID, nCount] : mapServerIDList) {
        if (nCount > shTopCount) {
            stCreateModeMaze.dwMasterServerID = dwServerID;
            shTopCount = nCount;
        }
    }

    XSendPacket packet(0xF2u, 0x49u);
    packet << stCreateModeMaze;
    pRelayServer->GetControlSocket().Send(packet);

    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching MakeOperationMaze End - ( MatchingID %d / MemberCound %d )",
                        static_cast<int>(m_dwMatchingID),
                        GetMemberCount());
    SetMatchingState(2);
    return true;
}

// 对齐 IDA 0x140032D20: ?ExitMatching@CModeMazeMatching@@QEAA_NKKE@Z
bool CModeMazeMatching::ExitMatching(std::uint32_t dwActorID,
                                     std::uint32_t dwUAID,
                                     std::uint8_t byReason) {
    if (m_byProcess != 1) {
        return false;
    }

    // 对齐 IDA: 查找退出成员以获取 UAID 用于 DB 日志
    std::uint32_t dwExitUAID = dwUAID;
    for (const auto& member : m_listMatchingUser) {
        if (member && member->GetActorID() == dwActorID) {
            if (dwExitUAID == 0) {
                dwExitUAID = member->GetUAID();
            }
            break;
        }
    }

    // 对齐 IDA: SendMatchingExit 已包含从列表移除成员的逻辑
    SendMatchingExit(dwActorID, byReason);

    // 对齐 IDA: 发送 DB 日志 (main=28, sub=2)
    XRelayServer* pRelayServer = TXSingleton<XRelayServer>::Instance();
    pRelayServer->SendDBLog(
        static_cast<int>(dwExitUAID),
        static_cast<int>(dwActorID),
        28, 2,
        static_cast<int>(m_dwMatchingID),
        static_cast<int>(m_wMapID),
        0, 0,
        static_cast<int>(byReason),
        0, 0, L"");

    if (m_listMatchingUser.empty()) {
        SetMatchingState(2);
        m_byProcess = 0;
    }
    return true;
}

void CModeMazeMatching::SendMatchingWait() {
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching SendMatchingWait - ( MatchingID %u )",
                        static_cast<unsigned int>(m_dwMatchingID));
    SetMatchingState(1);
    m_dw64CheckTick = GreenDamTan_GetTickCount64() + m_dw64WaitTime;

    // 构建成员信息列表
    std::vector<ST_MODE_MAZE_MEMBER_INFO> vecMemberInfo;
    for (const auto& member : m_listMatchingUser) {
        if (member && member->m_pCurServer && member->GetActorID() != 0) {
            vecMemberInfo.push_back(member->m_stMemberInfo);
        }
    }

    // 向每个成员发送等待包
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    for (const auto& member : m_listMatchingUser) {
        if (!member || !member->m_pCurServer || member->GetActorID() == 0) {
            continue;
        }

        PS_MODE_MAZE_MATCHING_WAIT psWait{};
        psWait.dwMatchingID = m_dwMatchingID;
        psWait.wModeMazeID = m_wMapID;
        psWait.dwActorID = member->GetActorID();
        psWait.vecMemberInfo = vecMemberInfo;

        XSendPacket packet(0xFDu, 5u);
        packet << psWait;
        member->m_pCurServer->SendEx(packet);

        // 记录DB日志
        relayServer->SendDBLog(static_cast<int>(member->GetUAID()),
                               static_cast<int>(member->GetActorID()),
                               28, 5,
                               static_cast<int>(m_dwMatchingID),
                               static_cast<int>(m_wMapID),
                               0, 0, 0, 0, 0, L"");
    }
}

void CModeMazeMatching::SendMatchingExit(std::uint32_t dwExitActorID, std::uint8_t byReason) {
    // 循环1: 查找退出成员的UAID
    std::uint32_t dwExitUAID = 0;
    for (const auto& member : m_listMatchingUser) {
        if (member && member->GetActorID() == dwExitActorID) {
            dwExitUAID = member->GetUAID();
            break;
        }
    }

    // 循环2: 向所有成员发送退出包
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    for (const auto& member : m_listMatchingUser) {
        if (!member || !member->m_pCurServer) {
            continue;
        }

        PS_MODE_MAZE_MATCHING_EXIT psExit{};
        if (dwExitActorID != 0) {
            psExit.dwExitUCID = dwExitActorID;
            psExit.dwExitUAID = dwExitUAID;
        } else {
            psExit.dwExitUCID = member->GetActorID();
            psExit.dwExitUAID = member->GetUAID();
        }
        psExit.byReason = byReason;

        XSendPacket packet(0xFDu, 3u);
        packet.XParse << member->GetActorID();
        packet << psExit;
        member->m_pCurServer->SendEx(packet);
    }

    // 循环3: 如果指定了退出成员，从列表中移除并更新状态
    if (dwExitActorID != 0) {
        for (auto it = m_listMatchingUser.begin(); it != m_listMatchingUser.end(); ++it) {
            if (*it && (*it)->GetActorID() == dwExitActorID) {
                // 更新用户状态
                std::shared_ptr<CUserPartyInfo> pUserParty = relayServer->GetPartyUser(dwExitActorID);
                if (pUserParty) {
                    pUserParty->SetMatchingState(0);
                    pUserParty->SetMatchingID(0, 0);
                }
                m_listMatchingUser.erase(it);
                break;
            }
        }
    }

    // 循环4: 如果列表为空或没有指定退出成员，清理所有成员状态
    if (m_listMatchingUser.empty() || dwExitActorID == 0) {
        for (const auto& member : m_listMatchingUser) {
            if (member) {
                std::shared_ptr<CUserPartyInfo> pUserParty = relayServer->GetPartyUser(member->GetActorID());
                if (pUserParty) {
                    pUserParty->SetMatchingState(0);
                    pUserParty->SetMatchingID(0, 0);
                }
            }
        }
        m_listMatchingUser.clear();
        SetMatchingState(2);
        m_bSendSucc = true;
        m_byProcess = 0;
    }
}

// 对齐 IDA 0x140034170: ?SendCreateMatchingModeMaze@CModeMazeMatching@@QEAAXAEAUST_CREATE_MODE_MAZE@@@Z
void CModeMazeMatching::SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze) {
    // 对齐 IDA: 获取 Reward_Attend 作为排名积分
    XRelayServer* pRelayServer = TXSingleton<XRelayServer>::Instance();
    int nWorldID = static_cast<int>(pRelayServer->GetOption().GetGroupID());
    (void)nWorldID;  // TODO: 推测结果 - 当前 GetTB_OPERATION_INFO 不接受 worldID 参数

    int nRankingPoint = 0;
    TB_OPERATION_INFO* pTB_OPERATION_INFO = pRelayServer->GetResourceMgr().GetTB_OPERATION_INFO(static_cast<unsigned int>(m_wMapID));
    if (pTB_OPERATION_INFO) {
        nRankingPoint = pTB_OPERATION_INFO->Reward_Attend;
    }

    for (const auto& member : m_listMatchingUser) {
        if (!member || !member->m_pCurServer) {
            continue;
        }

        // 对齐 IDA: 发送 DB 日志 (main=28, sub=4)
        pRelayServer->SendDBLog(
            static_cast<int>(member->GetUAID()),
            static_cast<int>(member->GetActorID()),
            28, 4,
            static_cast<int>(m_dwMatchingID),
            static_cast<int>(m_wMapID),
            nRankingPoint,
            0,
            static_cast<int>(member->m_stMemberInfo.nFirstJumpID),
            0,
            static_cast<std::int64_t>(stCreateModeMaze.uxMapID.nMapID),
            L"");

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

        // 对齐 IDA: 日志输出
        LogHelper::LogInfo("game.contents",
                           "send eSUB_CMD_SERVER_MODE_MAZE_MATCHING_CREATE_MAZE [ ActorID:%d, MapID:%lld, JumpID:%d ] ( %d )",
                           member->GetActorID(),
                           static_cast<long long>(stCreateModeMaze.uxMapID.nMapID),
                           stCreateModeMaze.nJumpID,
                           418);

        // 对齐 IDA: 清理用户匹配状态
        std::shared_ptr<CUserPartyInfo> pUserParty = pRelayServer->GetPartyUser(member->GetActorID());
        if (pUserParty) {
            pUserParty->SetMatchingState(0);
            pUserParty->SetMatchingID(0, 0);
        }
    }

    m_bSendSucc = true;
    m_byProcess = 0;
    LogHelper::LogDebug("game.contents",
                        "ModeMazeMatching SendCreateMatchingModeMaze - ( MatchingID %d / MemberCound %d )",
                        static_cast<int>(m_dwMatchingID),
                        GetMemberCount());
}

// 对齐 IDA 0x140034580: 获取匹配成员列表
void CModeMazeMatching::GetMatchingMember(std::vector<std::uint32_t>& vecMember) {
    for (const auto& member : m_listMatchingUser) {
        if (member) {
            vecMember.push_back(member->GetActorID());
        }
    }
}
