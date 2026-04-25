#include "Soulworker/GameServer/XRelayServer/ForceMatching.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <vector>

namespace {
std::shared_ptr<CForceMatching> GreenDamTan_FindMatching(std::map<std::uint32_t, std::shared_ptr<CForceMatching>>& matchingMap,
                                                         std::uint32_t matchingID) {
    const auto it = matchingMap.find(matchingID);
    if (it == matchingMap.end()) {
        return nullptr;
    }
    return it->second;
}

void GreenDamTan_SendEmptyMatchingInfo(CServer* pServer, std::uint32_t actorID) {
    if (!pServer) {
        return;
    }

    ST_FORCE_MATCHING_INFO matchingInfo{};
    XSendPacket sendPacket(0xFAu, 0x13u);
    sendPacket.XParse << actorID;
    sendPacket.XParse << 0;
    sendPacket.XParse << actorID;
    sendPacket << matchingInfo;
    pServer->SendEx(sendPacket);
}
}

CForceMatching::CForceMatching() {
    Init();
}

void CForceMatchginMember::Clear() {
    m_pCurServer = nullptr;
    m_stMemberInfo = {};
    m_nExp = 0;
    m_nState = 0;
}

void CForceMatching::Init() {
    std::memset(this, 0, sizeof(*this));
    SetMatchingState(0);
    m_byProcess = 1;
}

bool CForceMatching::CheckMazeOpenTime() {
    const std::time_t now = std::time(nullptr);
    if (m_biCheckMazeOpen <= static_cast<std::int64_t>(now)) {
        m_biCheckMazeOpen = static_cast<std::int64_t>(now) + 4;
        if (!TXSingleton<XRelayServer>::Instance()->GetMazeOpenControl().CheckMazeOpenTime(m_dwMazeID)) {
            return false;
        }
    }
    return true;
}

bool CForceMatching::OnUpdate() {
    if (m_byProcess == 0) {
        if (!m_bSendSucc) {
            LogHelper::LogError("game.contents",
                                "ForceMatching OnUpdate invalid process - ( State %u / Process %u )",
                                static_cast<unsigned int>(m_byState),
                                static_cast<unsigned int>(m_byProcess));
            SendMatchingExit(0, 1u, 0u);
        }
        return false;
    }

    if (m_byProcess == 2) {
        return true;
    }

    if (!CheckMazeOpenTime()) {
        LogHelper::LogError("game.contents",
                            "CheckMazeOpenTime - ( State %u / Process %u )",
                            static_cast<unsigned int>(m_byState),
                            static_cast<unsigned int>(m_byProcess));
        SendMatchingExit(0, 0u, 0u);
        return false;
    }

    if (m_dw64CheckTick > GetTickCount64()) {
        return true;
    }

    if (m_byState == 0) {
        return MatchingPossible();
    }
    if (m_byState == 1) {
        return MatchingCheck();
    }
    if (m_byState == 2) {
        MatchingWait();
        return true;
    }

    if (!m_bSendSucc) {
        LogHelper::LogError("game.contents",
                            "ForceMatching OnUpdate invalid state - ( State %u / Process %u )",
                            static_cast<unsigned int>(m_byState),
                            static_cast<unsigned int>(m_byProcess));
        SendMatchingExit(0, 1u, 0u);
    }
    return false;
}

std::uint8_t CForceMatching::GetMatchingUserCount() {
    std::uint8_t count = 0;
    for (const CForceMatchginMember& member : m_stMatchingUser) {
        if (member.m_pCurServer) {
            ++count;
        }
    }
    return count;
}

// 对齐 IDA: 指针改为引用
bool CForceMatching::CheckAutoMatchingEnter(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& stMemberInfo,
                                             std::uint32_t dwEnterMazeID,
                                             CServer* pServer) {
    static_cast<void>(pServer);
    if (m_dwMazeID != dwEnterMazeID) {
        return false;
    }
    if (m_byState != 0 || m_byProcess != 1) {
        return false;
    }
    if (m_shAveLevel + 5 < stMemberInfo.byLevel || stMemberInfo.byLevel < m_shAveLevel - 5) {
        return false;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (const CForceMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer) {
            continue;
        }
        if (relayServer.IsFriendBlock(member.m_stMemberInfo.dwMemberID, stMemberInfo.dwUCID)) {
            return false;
        }
        if (relayServer.IsFriendBlock(stMemberInfo.dwUCID, member.m_stMemberInfo.dwMemberID)) {
            return false;
        }
    }
    return true;
}

// 对齐 IDA 0x14001CEF0: ?SendMatchingInfo@CForceMatching@@QEAAXK@Z
void CForceMatching::SendMatchingInfo(std::uint32_t dwActorID) {
    int nAveValue = 0;
    int nUserCount = 0;
    ST_FORCE_MATCHING_INFO matchingInfo{};
    matchingInfo.dwMatchingID = m_dwMachingID;

    for (int index = 0; index < 8; ++index) {
        matchingInfo.stMemberInfo[index] = m_stMatchingUser[index].m_stMemberInfo;
    }
    matchingInfo.nRemainTick = static_cast<int>((m_dw64CheckTick - GetTickCount64()) / 1000);

    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        // 对齐 IDA: 仅当 dwMemberID == dwActorID 时累加等级
        if (m_stMatchingUser[index].m_stMemberInfo.dwMemberID == dwActorID) {
            nAveValue += m_stMatchingUser[index].m_stMemberInfo.byLevel;
            ++nUserCount;
        }

        XSendPacket sendPacket(0xFAu, 0x13u);
        sendPacket.XParse << m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        sendPacket.XParse << 1;
        sendPacket.XParse << dwActorID;
        sendPacket << matchingInfo;
        m_stMatchingUser[index].m_pCurServer->SendEx(sendPacket);
    }

    // 对齐 IDA: 无除零保护（调用方保证 dwActorID 至少匹配一个成员）
    // TODO: 需人工审查 - IDA 原始代码不检查 nUserCount==0
    m_shAveLevel = static_cast<std::int16_t>(nAveValue / nUserCount);
}

// 对齐 IDA: 指针改为引用
bool CForceMatching::AutoMatchingEnter(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& stMemberInfo,
                                        std::uint32_t dwEnterMazeID,
                                        CServer* pServer) {
    if (!CheckAutoMatchingEnter(stMemberInfo, dwEnterMazeID, pServer)) {
        return false;
    }

    int slot = -1;
    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            slot = index;
            break;
        }
    }
    if (slot < 0) {
        return false;
    }

    ST_FORCE_MEMBER forceMember{};
    forceMember.dwMemberID = stMemberInfo.dwUCID;
    forceMember.byClass = stMemberInfo.byClass;
    forceMember.byLevel = stMemberInfo.byLevel;
    forceMember.byAwaken = stMemberInfo.byAwaken;
    forceMember.dwProfilePhotoID = stMemberInfo.dwProfilePhotoID;
    std::wmemcpy(forceMember.strName, stMemberInfo.strName, std::size(forceMember.strName));
    forceMember.uxMapID = stMemberInfo.uxMapID;

    m_stMatchingUser[slot].m_stMemberInfo = forceMember;
    m_stMatchingUser[slot].m_pCurServer = pServer;
    m_stMatchingUser[slot].m_nExp = stMemberInfo.nExp;
    m_stMatchingUser[slot].m_nState = stMemberInfo.nState;
    SendMatchingInfo(stMemberInfo.dwUCID);
    return true;
}

void CForceMatching::CheckFullUser() {
    for (const CForceMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer) {
            return;
        }
    }
    LeaderSelect();
    SendMatchingWait();
}

// 对齐 IDA 0x14001CCB0: ?AutoMatchingAccept@CForceMatching@@QEAAXKPEAVCServer@@EK@Z
// 签名: void AutoMatchingAccept(uint32_t dwActorID, CServer* pServer, uint8_t byCheck) - 3参数
void CForceMatching::AutoMatchingAccept(std::uint32_t dwActorID, CServer* pServer, std::uint8_t byCheck) {
    static_cast<void>(pServer);  // 对齐 IDA: pServer 参数未使用

    bool allAccepted = true;
    for (int index = 0; index < 8; ++index) {
        if (dwActorID == m_stMatchingUser[index].m_stMemberInfo.dwMemberID) {
            if (byCheck == 0) {
                m_bCheck[index] = 10;
                allAccepted = false;
                break;
            }
            m_bCheck[index] = 100;
        }
        if (m_stMatchingUser[index].m_pCurServer && m_bCheck[index] != 100) {
            allAccepted = false;
        }
    }

    if (allAccepted) {
        LeaderSelect();
        SendMatchingWait();
    }
}

void CForceMatching::LeaderSelect() {
    std::int16_t maxLevel = 0;
    std::int64_t maxExp = 0;

    for (int index = 0; index < 8; ++index) {
        const CForceMatchginMember& member = m_stMatchingUser[index];
        if (!member.m_pCurServer) {
            continue;
        }

        if (member.m_stMemberInfo.byLevel > maxLevel
            || (member.m_stMemberInfo.byLevel == maxLevel && member.m_nExp > maxExp)) {
            m_dwLeaderActorID = member.m_stMemberInfo.dwMemberID;
            maxLevel = member.m_stMemberInfo.byLevel;
            maxExp = member.m_nExp;
        }
    }
}

std::uint8_t CForceMatching::SendMatchingExit(std::uint32_t dwActorID,
                                              std::uint8_t byReason,
                                              std::uint32_t dwUAID) {
    int totalLevel = 0;
    std::uint8_t remainCount = 0;
    std::uint8_t exitLevel = 0;

    for (CForceMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer || member.m_stMemberInfo.dwMemberID == 0) {
            continue;
        }

        if (member.m_stMemberInfo.dwMemberID != dwActorID && dwActorID != 0) {
            totalLevel += member.m_stMemberInfo.byLevel;
            ++remainCount;
        }

        XSendPacket sendPacket(0xFAu, 0x14u);
        sendPacket.XParse << member.m_stMemberInfo.dwMemberID;
        if (dwActorID == member.m_stMemberInfo.dwMemberID) {
            exitLevel = member.m_stMemberInfo.byLevel;
            sendPacket.XParse << member.m_stMemberInfo.dwMemberID;
        } else {
            sendPacket.XParse << dwActorID;
        }
        sendPacket.XParse << byReason;
        member.m_pCurServer->SendEx(sendPacket);
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (CForceMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer || member.m_stMemberInfo.dwMemberID == 0) {
            continue;
        }
        if (dwActorID != 0 && member.m_stMemberInfo.dwMemberID != dwActorID) {
            continue;
        }

        if (const std::shared_ptr<CUserPartyInfo> partyUser = relayServer.GetPartyUser(member.m_stMemberInfo.dwMemberID)) {
            partyUser->SetMatchingState(false);
            partyUser->SetMatchingID(0, 0);
        }
        member.Clear();
    }

    if (dwUAID != 0) {
        relayServer.SendDBLog(static_cast<int>(dwUAID),
                              static_cast<int>(dwActorID),
                              23,
                              11,
                              static_cast<int>(exitLevel),
                              0,
                              static_cast<int>(m_dwMazeID),
                              0,
                              0,
                              static_cast<std::int64_t>(m_dwMachingID),
                              0,
                              L"");
    } else if (byReason == 3) {
        relayServer.SendDBLog(0,
                              static_cast<int>(dwActorID),
                              23,
                              10,
                              static_cast<int>(exitLevel),
                              0,
                              static_cast<int>(m_dwMazeID),
                              0,
                              0,
                              static_cast<std::int64_t>(m_dwMachingID),
                              0,
                              L"");
    }

    // 对齐 IDA: 当 totalLevel==0 或 remainCount==0 时返回 0
    if (totalLevel == 0 || remainCount == 0) {
        return 0;
    }

    m_shAveLevel = static_cast<std::int16_t>(totalLevel / remainCount);

    LogHelper::LogDebug("game.contents",
                        "ForceMatching SendMatchingExit - ( MatchingID %u / ExitUCID %u / Reason %u / Remain %u )",
                        static_cast<unsigned int>(m_dwMachingID),
                        static_cast<unsigned int>(dwActorID),
                        static_cast<unsigned int>(byReason),
                        static_cast<unsigned int>(remainCount));
    return remainCount;
}


void CForceMatching::SendMatchingCheck() {
    SetMatchingState(1);
    m_dw64CheckTick = GetTickCount64() + 10000;

    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        XSendPacket packet(0xFAu, 0x15u);
        packet.XParse << m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        packet.XParse << m_dwMazeID;
        m_stMatchingUser[index].m_pCurServer->SendEx(packet);
    }
}

void CForceMatching::SendMatchingReset(std::uint8_t byReason) {
    ++m_nResetCount;
    m_dwLeaderActorID = 0;
    for (std::uint8_t& checkState : m_bCheck) {
        checkState = 0;
    }
    SetMatchingState(0);
    m_dw64CheckTick = GetTickCount64() + 60000;
    const int remainTick = static_cast<int>((m_dw64CheckTick - GetTickCount64()) / 1000);

    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        XSendPacket packet(0xFAu, 0x16u);
        packet.XParse << m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        packet.XParse << remainTick;
        packet.XParse << byReason;
        packet.XParse << m_nResetCount;
        m_stMatchingUser[index].m_pCurServer->SendEx(packet);
    }
}

void CForceMatching::SendMatchingStart() {
    m_byProcess = 2;

    std::uint8_t userCount = 0;
    for (int index = 0; index < 8; ++index) {
        if (m_stMatchingUser[index].m_stMemberInfo.dwMemberID != 0) {
            ++userCount;
        }
    }

    if (userCount < 2) {
        SendMatchingExit(0, 3u, 0u);
        SetMatchingState(3);
        m_byProcess = 0;
        return;
    }

    PS_DB_FORCE_MATCHING_CREATE createInfo{};
    createInfo.stCreateForce.dwMatchingID = m_dwMachingID;
    createInfo.stCreateForce.dwLeaderUCID = m_dwLeaderActorID;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (int index = 0; index < 8; ++index) {
        createInfo.stCreateForce.dwMemberUCID[index] = m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        if (createInfo.stCreateForce.dwMemberUCID[index] == 0) {
            continue;
        }

        UXActorID memberActor{};
        memberActor.dwActorID = createInfo.stCreateForce.dwMemberUCID[index];

        if (relayServer.GetPartyManager().GetParty(memberActor)) {
            const std::uint32_t partyID = relayServer.GetPartyManager().GetPartyID(memberActor);
            createInfo.setDeleteParty.insert(partyID);
            relayServer.GetPartyManager().DeleteParty(partyID);
        }

        const std::shared_ptr<CForce> pForce = relayServer.GetForceManager().GetForce(memberActor);
        if (pForce) {
            const std::uint32_t forceID = pForce->GetForceID();
            createInfo.setDeleteForce.insert(forceID);
            relayServer.GetForceManager().DeleteForce(forceID);
        }
    }

    IXObject* objectOwner = nullptr;
    XSendDBPacket sendPacket(objectOwner, 8u, 0xDu);
    sendPacket << createInfo;
    relayServer.SendDBGame(sendPacket);
}


bool CForceMatching::MatchingPossible() {
    int currentCount = 0;
    for (int index = 0; index < 8; ++index) {
        if (m_stMatchingUser[index].m_stMemberInfo.dwMemberID != 0) {
            ++currentCount;
        }
    }

    if (currentCount > 3) {
        SendMatchingCheck();
        return true;
    }
    if (m_nResetCount >= 3) {
        SendMatchingExit(0, 4u, 0u);
        SetMatchingState(3);
        m_byProcess = 0;
        return false;
    }

    SendMatchingReset(2u);
    return true;
}

bool CForceMatching::MatchingCheck() {
    bool mazeEnter = true;
    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }
        if (m_bCheck[index] == 10) {
            mazeEnter = false;
            break;
        }
    }

    if (mazeEnter) {
        LeaderSelect();
        SendMatchingWait();
        return true;
    }
    if (m_nResetCount < 3) {
        SendMatchingReset(1u);
        return true;
    }

    SendMatchingExit(0, 4u, 0u);
    SetMatchingState(3);
    m_byProcess = 0;
    return false;
}

void CForceMatching::MatchingWait() {
    SendMatchingStart();
}

void CForceMatching::CreateMazeMatching(std::uint32_t dwForceID) {
    PS_FORCE_INFO createForce{};
    createForce.dwMaster = m_dwLeaderActorID;
    createForce.dwForceID = dwForceID;
    createForce.byForceType = 1;

    ST_CREATE_MAZE createMaze{};
    createMaze.wReqMapID = static_cast<std::uint16_t>(m_dwMazeID);
    createMaze.nPortalID = static_cast<int>(m_dwPortalID);
    createMaze.nJumpID = static_cast<int>(m_dwJumpID);

    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        createForce.vecForceMember.push_back(m_stMatchingUser[index].m_stMemberInfo);

        ST_ENTER_MAZE_MEMBER_INFO enterMember{};
        enterMember.dwMember = m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        enterMember.nState = m_stMatchingUser[index].m_nState;
        createMaze.vecEnterMember.push_back(enterMember);
    }

    TXSingleton<XRelayServer>::Instance()->GetForceManager().CreateForceMatching(createForce);

    // 对齐 IDA: stPartyInfo 设置
    createMaze.stPartyInfo.byGroupType = 2;
    createMaze.stPartyInfo.nID = dwForceID;

    // 对齐 IDA 0x14001E720: 数据包序列化顺序
    // IDA: << stCreateMaze << stCreateParty << stCreateForce << XParse(m_dwMachingID)
    PS_PARTY_INFO emptyParty{};
    XSendPacket sendPacket(0xF2u, 0x43u);
    sendPacket << createMaze;
    sendPacket << emptyParty;
    sendPacket << createForce;
    sendPacket.XParse << m_dwMachingID;  // 对齐 IDA: XParse 写入在最后
    TXSingleton<XRelayServer>::Instance()->GetControlSocket().Send(sendPacket);
}

// 对齐 IDA 0x14001DA20: ?SendCreateMatchingMaze@CForceMatching@@QEAAXUST_CREATE_MAZE@@UPS_FORCE_INFO@@@Z
void CForceMatching::SendCreateMatchingMaze(ST_CREATE_MAZE stCreateMaze, PS_FORCE_INFO stForceInfo) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        stCreateMaze.dwUserID = m_stMatchingUser[index].m_stMemberInfo.dwMemberID;

        XSendPacket packet(0xFAu, 0x18u);
        packet << stCreateMaze;
        packet << stForceInfo;
        m_stMatchingUser[index].m_pCurServer->SendEx(packet);

        // 对齐 IDA: GetPartyUser 结果决定 DB 日志发送逻辑
        const std::shared_ptr<CUserPartyInfo> pUserForce = relayServer.GetPartyUser(m_stMatchingUser[index].m_stMemberInfo.dwMemberID);
        if (!pUserForce) {
            // 对齐 IDA: GetPartyUser 失败时发送 UAID=0 的 DB 日志
            relayServer.SendDBLog(
                0,
                static_cast<int>(m_stMatchingUser[index].m_stMemberInfo.dwMemberID),
                23,
                10,
                static_cast<int>(m_stMatchingUser[index].m_stMemberInfo.byLevel),
                1,
                static_cast<int>(m_dwMazeID),
                static_cast<int>(stForceInfo.dwMaster),
                0,
                static_cast<std::int64_t>(m_dwMachingID),
                static_cast<std::int64_t>(stForceInfo.dwForceID),
                L"");
        } else {
            // 对齐 IDA: GetPartyUser 成功时清理匹配状态
            pUserForce->SetMatchingState(false);
            pUserForce->SetMatchingID(0, 0);

            // 对齐 IDA: 仅当 GetUser 也成功时发送带 UAID 的 DB 日志
            const std::shared_ptr<CUserObject> pUser = relayServer.GetUser(m_stMatchingUser[index].m_stMemberInfo.dwMemberID);
            if (pUser) {
                int uaid = static_cast<int>(pUser->GetUAID());
                relayServer.SendDBLog(
                    uaid,
                    static_cast<int>(m_stMatchingUser[index].m_stMemberInfo.dwMemberID),
                    23,
                    10,
                    static_cast<int>(m_stMatchingUser[index].m_stMemberInfo.byLevel),
                    1,
                    static_cast<int>(m_dwMazeID),
                    static_cast<int>(stForceInfo.dwMaster),
                    0,
                    static_cast<std::int64_t>(m_dwMachingID),
                    static_cast<std::int64_t>(stForceInfo.dwForceID),
                    L"");
            }
        }
    }

    m_bSendSucc = true;
    m_byProcess = 0;
}



void CForceMatching::SendMatchingWait() {
    SetMatchingState(2);
    m_dw64CheckTick = GetTickCount64() + 10000;

    for (int index = 0; index < 8; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        XSendPacket packet(0xFAu, 0x17u);
        packet.XParse << m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        packet.XParse << m_dwLeaderActorID;
        m_stMatchingUser[index].m_pCurServer->SendEx(packet);
    }
}

// 对齐 IDA 0x14001CA30: 初始化匹配并添加第一个成员
void CForceMatching::AutoMatchingCreate(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& stMemberInfo,
                                         std::uint32_t dwMatchingID,
                                         std::uint32_t dwEnterMazeID,
                                         std::uint32_t dwPortalID,
                                         std::uint32_t dwJumpID,
                                         CServer* pServer) {
    m_dwMachingID = dwMatchingID;
    m_shAveLevel = stMemberInfo.byLevel;
    m_dwMazeID = dwEnterMazeID;
    m_dwPortalID = dwPortalID;
    m_dwJumpID = dwJumpID;
    m_dw64CheckTick = GetTickCount64() + 180000;
    SetMatchingState(0);
    m_byProcess = 1;

    ST_FORCE_MEMBER forceMember{};
    forceMember.dwMemberID = stMemberInfo.dwUCID;
    forceMember.byClass = stMemberInfo.byClass;
    forceMember.byAwaken = stMemberInfo.byAwaken;
    forceMember.dwProfilePhotoID = stMemberInfo.dwProfilePhotoID;
    forceMember.byLevel = stMemberInfo.byLevel;
    std::wmemcpy(forceMember.strName, stMemberInfo.strName, std::size(forceMember.strName));
    forceMember.uxMapID = stMemberInfo.uxMapID;

    m_stMatchingUser[0].m_stMemberInfo = forceMember;
    m_stMatchingUser[0].m_pCurServer = pServer;
    m_stMatchingUser[0].m_nExp = stMemberInfo.nExp;
    m_stMatchingUser[0].m_nState = stMemberInfo.nState;
    m_byLimitCount = 8;
    m_nResetCount = 0;

    SendMatchingInfo(stMemberInfo.dwUCID);
}

bool CForceMatching::AutoMatchingExit(std::uint32_t dwActorID,
                                      std::uint8_t byReason,
                                      std::uint32_t dwUAID) {
    if (m_byProcess != 1) {
        return false;
    }
    if (m_byState == 2 || m_byState == 3) {
        return false;
    }

    for (const CForceMatchginMember& member : m_stMatchingUser) {
        if (member.m_stMemberInfo.dwMemberID != dwActorID) {
            continue;
        }

        if (SendMatchingExit(dwActorID, byReason, dwUAID) && (byReason == 0 || byReason == 2)) {
            return true;
        }
        break;
    }

    if (m_byState == 2) {
        m_byProcess = 1;
    } else {
        SetMatchingState(3);
        m_byProcess = 0;
    }
    return true;
}

void CForceMatchingMgr::CreateMatching(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER stMemberInfo,
                                       std::uint32_t dwMazeID,
                                       std::uint32_t dwPortalID,
                                       std::uint32_t dwJumpID,
                                       CServer* pServer,
                                       std::uint32_t& dwOutMatchingID) {
    // 对齐 IDA 0x140020B90: 调用 AutoMatchingCreate 而非 AutoMatchingEnter
    const std::shared_ptr<CForceMatching> matching = std::make_shared<CForceMatching>();
    ++m_dwMatchingID;
    matching->AutoMatchingCreate(stMemberInfo, m_dwMatchingID, dwMazeID, dwPortalID, dwJumpID, pServer);
    m_mpAutoMatching[m_dwMatchingID] = matching;
    dwOutMatchingID = m_dwMatchingID;
}

bool CForceMatchingMgr::EnterMatching(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER stMemberInfo,
                                      std::uint32_t dwMazeID,
                                      CServer* pServer,
                                      std::uint32_t& dwOutMatchingID) {
    for (auto& [matchingID, matching] : m_mpAutoMatching) {
        static_cast<void>(matchingID);
        if (!matching) {
            continue;
        }
        if (matching->AutoMatchingEnter(stMemberInfo, dwMazeID, pServer)) {
            dwOutMatchingID = matching->GetMatchingID();
            matching->CheckFullUser();
            return true;
        }
    }
    return false;
}

bool CForceMatchingMgr::EnterMatching(PS_SERVER_FORCE_MATCHING_ENTER psEnter,
                                      PS_SERVER_FORCE_MATCHING_ENTER_MEMBER psMaster,
                                      CServer* pServer,
                                      std::uint8_t byPartyGroupType,
                                      std::uint32_t& dwOutMatchingID,
                                      std::uint8_t& byCreate) {
    if (!pServer) {
        return false;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    const std::shared_ptr<CUserPartyInfo> masterUser = relayServer.GetPartyUser(psMaster.dwUCID);
    if (!masterUser) {
        GreenDamTan_SendEmptyMatchingInfo(pServer, psMaster.dwUCID);
        return false;
    }

    if (byPartyGroupType == 0) {
        masterUser->SetMatchingState(true);
        if (!EnterMatching(psMaster, psEnter.stCreateMaze.wReqMapID, pServer, dwOutMatchingID)) {
            CreateMatching(psMaster,
                           psEnter.stCreateMaze.wReqMapID,
                           static_cast<std::uint32_t>(psEnter.stCreateMaze.nPortalID),
                           static_cast<std::uint32_t>(psEnter.stCreateMaze.nJumpID),
                           pServer,
                           dwOutMatchingID);
            byCreate = 1;
        }
        masterUser->SetMatchingID(dwOutMatchingID, 2u);
        relayServer.SendDBLog(static_cast<int>(psMaster.dwUAID),
                              static_cast<int>(psMaster.dwUCID),
                              23,
                              9,
                              static_cast<int>(psEnter.stCreateMaze.wReqMapID >> 8),
                              static_cast<int>(byCreate),
                              0,
                              0,
                              0,
                              static_cast<std::int64_t>(dwOutMatchingID),
                              0,
                              L"");
        return true;
    }

    for (auto& [matchingID, matching] : m_mpAutoMatching) {
        static_cast<void>(matchingID);
        if (!matching) {
            continue;
        }

        const std::size_t enterCount = psEnter.vecMember.size();
        if (matching->GetMatchingUserCount() + enterCount > 8) {
            continue;
        }

        bool canEnter = true;
        for (auto& member : psEnter.vecMember) {
            if (!matching->CheckAutoMatchingEnter(member, psEnter.stCreateMaze.wReqMapID, pServer)) {
                canEnter = false;
                break;
            }
        }
        if (!canEnter) {
            continue;
        }

        for (auto& member : psEnter.vecMember) {
            matching->AutoMatchingEnter(member, psEnter.stCreateMaze.wReqMapID, pServer);
            if (const std::shared_ptr<CUserPartyInfo> memberUser = relayServer.GetPartyUser(member.dwUCID)) {
                memberUser->SetMatchingState(true);
                memberUser->SetMatchingID(matching->GetMatchingID(), 2u);
            }
        }

        dwOutMatchingID = matching->GetMatchingID();
        matching->CheckFullUser();
        byCreate = 0;
        return true;
    }

    CreateMatching(psMaster,
                   psEnter.stCreateMaze.wReqMapID,
                   static_cast<std::uint32_t>(psEnter.stCreateMaze.nPortalID),
                   static_cast<std::uint32_t>(psEnter.stCreateMaze.nJumpID),
                   pServer,
                   dwOutMatchingID);
    if (dwOutMatchingID == 0) {
        GreenDamTan_SendEmptyMatchingInfo(pServer, psMaster.dwUCID);
        return false;
    }

    const std::shared_ptr<CForceMatching> matching = GreenDamTan_FindMatching(m_mpAutoMatching, dwOutMatchingID);
    if (!matching) {
        GreenDamTan_SendEmptyMatchingInfo(pServer, psMaster.dwUCID);
        return false;
    }

    masterUser->SetMatchingState(true);
    masterUser->SetMatchingID(dwOutMatchingID, 2u);

    for (auto& member : psEnter.vecMember) {
        if (member.dwUCID == psMaster.dwUCID) {
            continue;
        }
        matching->AutoMatchingEnter(member, psEnter.stCreateMaze.wReqMapID, pServer);
        if (const std::shared_ptr<CUserPartyInfo> memberUser = relayServer.GetPartyUser(member.dwUCID)) {
            memberUser->SetMatchingState(true);
            memberUser->SetMatchingID(dwOutMatchingID, 2u);
        }
    }

    byCreate = 1;
    return true;
}
// 对齐 IDA 0x140021680: ?CheckMatching@CForceMatchingMgr@@QEAA_NKEPEAVCServer@@K@Z
bool CForceMatchingMgr::CheckMatching(std::uint32_t dwActorID, std::uint8_t byCheck, CServer* pServer, std::uint32_t dwUAID) {
    static_cast<void>(dwUAID);

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    const std::shared_ptr<CUserPartyInfo> partyUser = relayServer.GetPartyUser(dwActorID);
    if (!partyUser) {
        return false;
    }

    const std::uint32_t matchingID = partyUser->GetMatchingID();
    // 对齐 IDA: 使用 GetRewardState() 非 GetMatchingState()
    if (matchingID == 0 || partyUser->GetRewardState() == 0) {
        return false;
    }

    std::shared_ptr<CForceMatching> matching = GreenDamTan_FindMatching(m_mpAutoMatching, matchingID);
    if (!matching) {
        matching = GreenDamTan_FindMatching(m_mpAutoMatching_Waiter, matchingID);
    }
    if (!matching) {
        return false;
    }

    matching->AutoMatchingAccept(dwActorID, pServer, byCheck);
    return true;
}

void CForceMatchingMgr::OnUpdate() {
    std::vector<std::uint32_t> matchingToDelete;
    matchingToDelete.reserve(m_mpAutoMatching.size());

    for (const auto& [matchingID, matching] : m_mpAutoMatching) {
        if (!matching) {
            matchingToDelete.push_back(matchingID);
            continue;
        }
        if (!matching->OnUpdate()) {
            matchingToDelete.push_back(matching->GetMatchingID());
        }
    }

    for (std::uint32_t matchingID : matchingToDelete) {
        m_mpAutoMatching.erase(matchingID);
    }
}

void CForceMatchingMgr::ResForceMatchingCreate(std::uint32_t dwMatchingID, std::uint32_t dwForceID) {
    const std::shared_ptr<CForceMatching> matching = GreenDamTan_FindMatching(m_mpAutoMatching, dwMatchingID);
    if (!matching) {
        return;
    }

    matching->CreateMazeMatching(dwForceID);
}

void CForceMatchingMgr::SendCreateMatchingMaze(std::uint32_t dwMatchingID,
                                               ST_CREATE_MAZE stCreateMaze,
                                               PS_FORCE_INFO stForceInfo) {
    const std::shared_ptr<CForceMatching> matching = GreenDamTan_FindMatching(m_mpAutoMatching, dwMatchingID);
    if (!matching) {
        return;
    }

    matching->SendCreateMatchingMaze(stCreateMaze, stForceInfo);
}

bool CForceMatchingMgr::ExitMatching(std::uint32_t dwActorID,
                                     std::uint32_t dwMatchingID,
                                     std::uint8_t byReason,
                                     std::uint32_t dwUAID) {
    auto it = m_mpAutoMatching.find(dwMatchingID);
    if (it == m_mpAutoMatching.end() || !it->second) {
        it = m_mpAutoMatching_Waiter.find(dwMatchingID);
        if (it == m_mpAutoMatching_Waiter.end() || !it->second) {
            return false;
        }
    }

    return it->second->AutoMatchingExit(dwActorID, byReason, dwUAID);
}

// 对齐 IDA 0x140021C90: ?MatchingRemoveUser@CForceMatchingMgr@@QEAAXKK@Z
void CForceMatchingMgr::MatchingRemoveUser(std::uint32_t dwMatchingID, std::uint32_t dwUCID) {
    if (dwMatchingID == 0) {
        return;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    bool bPartyGroup = false;

    // 对齐 IDA: 检查用户是否有队伍
    UXActorID uActorID{};
    uActorID.dwActorID = dwUCID;
    std::shared_ptr<CParty> pParty = relayServer.GetPartyManager().GetParty(uActorID);
    if (pParty) {
        bPartyGroup = true;
        PS_PARTY_INFO partyInfo{};
        pParty->GetPartyInfo(partyInfo);
        for (const ST_PARTY_MEMBER& member : partyInfo.vecPartyMember) {
            const std::shared_ptr<CUserPartyInfo> pPartyMember = relayServer.GetPartyUser(member.dwMemberID);
            if (pPartyMember) {
                const std::uint32_t memberMatchingID = pPartyMember->GetMatchingID();
                const std::uint32_t memberActorID = member.dwMemberID;  // 对齐 IDA: GetPartyID
                ExitMatching(memberActorID, memberMatchingID, 2u, 0u);
            }
        }
    }

    // 对齐 IDA: 检查用户是否有公会
    std::shared_ptr<CForce> pForce = relayServer.GetForceManager().GetForce(uActorID);
    if (pForce) {
        bPartyGroup = true;
        PS_FORCE_INFO forceInfo{};
        pForce->GetForceInfo(forceInfo);
        for (const ST_FORCE_MEMBER& member : forceInfo.vecForceMember) {
            const std::shared_ptr<CUserPartyInfo> pForceMember = relayServer.GetPartyUser(member.dwMemberID);
            if (pForceMember) {
                const std::uint32_t memberMatchingID = pForceMember->GetMatchingID();
                const std::uint32_t memberActorID = member.dwMemberID;  // 对齐 IDA: GetPartyID
                ExitMatching(memberActorID, memberMatchingID, 2u, 0u);
            }
        }
    }

    // 对齐 IDA: 如果没有队伍/公会，直接退出用户匹配
    if (!bPartyGroup) {
        ExitMatching(dwUCID, dwMatchingID, 2u, 0u);
    }
}
