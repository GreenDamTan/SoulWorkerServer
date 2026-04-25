#include "Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <vector>

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

namespace {
std::uint64_t GreenDamTan_GetTickCount64() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

std::int64_t GreenDamTan_GetCurDateSec() {
    return static_cast<std::int64_t>(std::time(nullptr));
}
}

void CPartyMatchginMember::Clear() {
    m_pCurServer = nullptr;
    m_stMemberInfo = {};
    m_nExp = 0;
    m_nState = 0;
}

CPartyMatching::CPartyMatching() {
    m_byProcess = 1;
}

void CPartyMatching::Init() {
    m_bSendSucc = false;
    std::memset(m_stMatchingUser, 0, sizeof(m_stMatchingUser));
    m_dwMachingID = 0;
    m_dw64CheckTick = 0;
    m_shAveLevel = 0;
    SetMatchingState(0);
    m_byProcess = 1;
    m_dwLeaderActorID = 0;
    m_dwMazeID = 0;
    m_dwPortalID = 0;
    m_dwJumpID = 0;
    std::memset(m_bCheck, 0, sizeof(m_bCheck));
}

void CPartyMatching::CheckFullUser() {
    for (int i = 0; i < 4; ++i) {
        if (!m_stMatchingUser[i].m_pCurServer) {
            return;
        }
    }
    LeaderSelect();
    SendMatchingWait();
}

bool CPartyMatching::OnUpdate() {
    if (m_byProcess == 0) {
        if (!m_bSendSucc) {
            LogHelper::LogError("game.relay",
                                "PartyMatching OnUpdate invalid process - ( State %u / Process %u )",
                                static_cast<unsigned int>(m_byState),
                                static_cast<unsigned int>(m_byProcess));
            SendMatchingExit(0, 1u, 0u);
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
        return MatchingCheck();
    }
    if (m_byState == 2) {
        MatchingWait();
        return true;
    }

    if (!m_bSendSucc) {
        LogHelper::LogError("game.relay",
                            "PartyMatching OnUpdate invalid state - ( State %u / Process %u )",
                            static_cast<unsigned int>(m_byState),
                            static_cast<unsigned int>(m_byProcess));
        SendMatchingExit(0, 1u, 0u);
    }
    return false;
}

bool CPartyMatching::MatchingPossible() {
    int currentCount = 0;
    std::uint32_t soloActorID = 0;
    for (const CPartyMatchginMember& member : m_stMatchingUser) {
        if (member.m_stMemberInfo.dwMemberID == 0) {
            continue;
        }
        ++currentCount;
        soloActorID = member.m_stMemberInfo.dwMemberID;
    }

    if (currentCount >= 2) {
        if (m_byLimitCount != 0 && currentCount < m_byLimitCount) {
            SendMatchingExit(0, 3u, 0u);
            SetMatchingState(3u);
            m_byProcess = 0;
            return false;
        }

        SendMatchingCheck();
        SetMatchingState(1u);
        m_dw64CheckTick = GreenDamTan_GetTickCount64() + 10000;
        return true;
    }

    SendMatchingExit(soloActorID, 3u, 0u);
    SetMatchingState(3u);
    m_byProcess = 0;
    return false;
}

bool CPartyMatching::MatchingCheck() {
    int currentCount = 0;
    bool mazeEnter = true;

    for (int index = 0; index < 4; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }
        if (m_bCheck[index] == 10) {
            mazeEnter = false;
            break;
        }
        ++currentCount;
    }

    if (!mazeEnter || (m_byLimitCount != 0 && m_byLimitCount > currentCount)) {
        SendMatchingExit(0, 1u, 0u);
        SetMatchingState(3u);
        m_byProcess = 0;
        return false;
    }

    LeaderSelect();
    SendMatchingWait();
    return true;
}

void CPartyMatching::MatchingWait() {
    SendMatchingStart();
}

void CPartyMatching::SendMatchingCheck() {
    for (const CPartyMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer) {
            continue;
        }

        XSendPacket packet(0xF4u, 0x22u);
        packet.XParse << member.m_stMemberInfo.dwMemberID;
        member.m_pCurServer->SendEx(packet);
    }
}

void CPartyMatching::SendMatchingWait() {
    m_dw64CheckTick = GreenDamTan_GetTickCount64() +
                      TXSingleton<XRelayServer>::Instance()->GetPartyMatchingConfig().GetMatchingWaitMs();
    SetMatchingState(2u);

    for (const CPartyMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer) {
            continue;
        }

        XSendPacket packet(0xF4u, 0x24u);
        packet.XParse << member.m_stMemberInfo.dwMemberID;
        packet.XParse << m_dwLeaderActorID;
        member.m_pCurServer->SendEx(packet);
    }
}

void CPartyMatching::SendMatchingStart() {
    m_byProcess = 2;

    std::uint8_t userCount = 0;
    for (const CPartyMatchginMember& member : m_stMatchingUser) {
        if (member.m_stMemberInfo.dwMemberID != 0) {
            ++userCount;
        }
    }

    if (userCount < 2u || (m_byLimitCount != 0 && m_byLimitCount > userCount)) {
        SendMatchingExit(0, 3u, 0u);
        SetMatchingState(3u);
        m_byProcess = 0;
        return;
    }

    XSendDBPacket sendPacket(nullptr, 4u, 0x13u);
    sendPacket.XParse << m_dwMachingID;
    sendPacket.XParse << m_dwLeaderActorID;
    for (const CPartyMatchginMember& member : m_stMatchingUser) {
        sendPacket.XParse << member.m_stMemberInfo.dwMemberID;
    }
    TXSingleton<XRelayServer>::Instance()->SendDBGame(sendPacket);
}

std::uint8_t CPartyMatching::SendMatchingExit(std::uint32_t dwActorID,
                                              std::uint8_t byReason,
                                              std::uint32_t dwUAID) {
    int totalLevel = 0;
    std::uint8_t remainingCount = 0;
    std::uint8_t exitLevel = 0;

    for (int index = 0; index < 4; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }

        if (m_stMatchingUser[index].m_stMemberInfo.dwMemberID != dwActorID && dwActorID != 0) {
            totalLevel += m_stMatchingUser[index].m_stMemberInfo.byLevel;
            ++remainingCount;
        }

        XSendPacket packet(0xF4u, 0x21u);
        packet.XParse << m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        if (dwActorID == m_stMatchingUser[index].m_stMemberInfo.dwMemberID) {
            exitLevel = m_stMatchingUser[index].m_stMemberInfo.byLevel;
            packet.XParse << m_stMatchingUser[index].m_stMemberInfo.dwMemberID;
        } else {
            packet.XParse << dwActorID;
        }
        packet.XParse << byReason;
        m_stMatchingUser[index].m_pCurServer->SendEx(packet);
    }

    for (int index = 0; index < 4; ++index) {
        if (!m_stMatchingUser[index].m_pCurServer) {
            continue;
        }
        if (dwActorID != 0 && dwActorID != m_stMatchingUser[index].m_stMemberInfo.dwMemberID) {
            continue;
        }

        if (const std::shared_ptr<CUserPartyInfo> partyUser =
                TXSingleton<XRelayServer>::Instance()->GetPartyUser(m_stMatchingUser[index].m_stMemberInfo.dwMemberID)) {
            partyUser->SetMatchingState(false);
            partyUser->SetMatchingID(0, 0);
        }
        m_stMatchingUser[index].Clear();
    }

    if (dwUAID != 0) {
        TXSingleton<XRelayServer>::Instance()->SendDBLog(static_cast<int>(dwUAID),
                                                         static_cast<int>(dwActorID),
                                                         22,
                                                         11,
                                                         exitLevel,
                                                         0,
                                                         static_cast<int>(m_dwMazeID),
                                                         0,
                                                         0,
                                                         static_cast<std::int64_t>(m_dwMachingID),
                                                         0,
                                                         L"");
    } else if (byReason == 3u) {
        TXSingleton<XRelayServer>::Instance()->SendDBLog(0,
                                                         static_cast<int>(dwActorID),
                                                         22,
                                                         10,
                                                         exitLevel,
                                                         0,
                                                         static_cast<int>(m_dwMazeID),
                                                         0,
                                                         0,
                                                         static_cast<std::int64_t>(m_dwMachingID),
                                                         0,
                                                         L"");
    }

    if (totalLevel != 0 && remainingCount != 0) {
        m_shAveLevel = static_cast<std::int16_t>(totalLevel / remainingCount);
    }
    return remainingCount;
}

bool CPartyMatching::AutoMatchingExit(std::uint32_t dwActorID,
                                      std::uint8_t byReason,
                                      std::uint32_t dwUAID) {
    if (m_byProcess != 1 || m_byState == 3u) {
        return false;
    }

    for (int index = 0; index < 4; ++index) {
        if (dwActorID != m_stMatchingUser[index].m_stMemberInfo.dwMemberID) {
            continue;
        }

        if (SendMatchingExit(dwActorID, byReason, dwUAID) != 0u && (byReason == 0u || byReason == 2u)) {
            return true;
        }
        break;
    }

    if (m_byState == 2u) {
        m_byProcess = 1;
    } else {
        SetMatchingState(3u);
        m_byProcess = 0;
    }
    return true;
}

// 对齐 IDA 0x14009B900: AutoMatchingEnter - 匹配成员加入
bool CPartyMatching::AutoMatchingEnter(ST_PARTY_MEMBER stMemberInfo,
                                        std::int64_t nExp,
                                        std::uint32_t dwEnterMazeID,
                                        int nState,
                                        CServer* pServer) {
    if (m_dwMazeID != dwEnterMazeID) {
        return false;
    }
    if (m_byState != 0) {
        return false;
    }
    if (m_byProcess != 1) {
        return false;
    }
    if (m_shAveLevel + 5 < stMemberInfo.byLevel || stMemberInfo.byLevel < m_shAveLevel - 5) {
        return false;
    }

    // 检查好友屏蔽
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    for (int i = 0; i < 4; ++i) {
        if (m_stMatchingUser[i].m_pCurServer) {
            if (relayServer->IsFriendBlock(m_stMatchingUser[i].m_stMemberInfo.dwMemberID,
                                           stMemberInfo.dwMemberID) == 1) {
                return false;
            }
            if (relayServer->IsFriendBlock(stMemberInfo.dwMemberID,
                                           m_stMatchingUser[i].m_stMemberInfo.dwMemberID) == 1) {
                return false;
            }
        }
    }

    // 添加到空位
    for (int j = 0; j < 4; ++j) {
        if (!m_stMatchingUser[j].m_pCurServer) {
            m_stMatchingUser[j].m_stMemberInfo = stMemberInfo;
            m_stMatchingUser[j].m_pCurServer = pServer;
            m_stMatchingUser[j].m_nExp = nExp;
            m_stMatchingUser[j].m_nState = nState;
            SendMatchingInfo(stMemberInfo.dwMemberID);
            return true;
        }
    }
    return false;
}

// 对齐 IDA 0x14009BC20: AutoMatchingCreate - 创建匹配
void CPartyMatching::AutoMatchingCreate(ST_PARTY_MEMBER stMemberInfo,
                                        std::int64_t nExp,
                                        int nState,
                                        std::uint32_t dwMatchingID,
                                        std::uint32_t dwMazeID,
                                        std::uint32_t dwPortalID,
                                        std::uint32_t dwJumpID,
                                        CServer* pServer) {
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();

    // 获取自动匹配配置
    int nAutoMatchingCreate = 0;
    TB_COMMON* pTB_Common = relayServer->GetResourceMgr().GetTB_COMMON(0x7531u);
    if (pTB_Common) {
        nAutoMatchingCreate = static_cast<int>(pTB_Common->Value);
    }

    m_dwMachingID = dwMatchingID;
    m_shAveLevel = stMemberInfo.byLevel;
    m_dwMazeID = dwMazeID;
    m_dwPortalID = dwPortalID;
    m_dwJumpID = dwJumpID;
    m_dw64CheckTick = GreenDamTan_GetTickCount64() + static_cast<std::uint64_t>(nAutoMatchingCreate) * 1000;
    SetMatchingState(0);
    m_byProcess = 1;

    // 设置第一个成员
    m_stMatchingUser[0].m_stMemberInfo = stMemberInfo;
    m_stMatchingUser[0].m_pCurServer = pServer;
    m_stMatchingUser[0].m_nExp = nExp;
    m_stMatchingUser[0].m_nState = nState;
    m_byLimitCount = 0;

    // 获取迷宫配置
    TB_MAZE_INFO* pTBMaze = relayServer->GetResourceMgr().GetTB_MAZE_INFO(m_dwMazeID);
    if (pTBMaze) {
        if (pTBMaze->Admission_Member == 2) {
            m_byLimitCount = 2;
        } else if (pTBMaze->Admission_Member == 3) {
            m_byLimitCount = 4;
        }
    }

    SendMatchingInfo(stMemberInfo.dwMemberID);
}

// 对齐 IDA 0x14009C030: SendMatchingInfo - 发送匹配信息
void CPartyMatching::SendMatchingInfo(std::uint32_t dwActorID) {
    ST_MATCHING_INFO stMatchingInfo{};
    stMatchingInfo.dwMatchingID = m_dwMachingID;

    for (int i = 0; i < 4; ++i) {
        stMatchingInfo.stMemberInfo[i] = m_stMatchingUser[i].m_stMemberInfo;
    }

    stMatchingInfo.nRemainTick = static_cast<int>((m_dw64CheckTick - GreenDamTan_GetTickCount64()) / 1000);

    int nAveValue = 0;
    int nUserCount = 0;

    for (int j = 0; j < 4; ++j) {
        if (m_stMatchingUser[j].m_pCurServer) {
            if (m_stMatchingUser[j].m_stMemberInfo.dwMemberID == dwActorID) {
                nAveValue += m_stMatchingUser[j].m_stMemberInfo.byLevel;
                ++nUserCount;
            }

            XSendPacket packet(0xF4u, 0x20u);
            packet.XParse << m_stMatchingUser[j].m_stMemberInfo.dwMemberID;
            packet.XParse << 1;
            packet.XParse << dwActorID;
            packet << stMatchingInfo;
            m_stMatchingUser[j].m_pCurServer->SendEx(packet);
        }
    }

    // TODO: 需人工审查 - IDA 原始代码不检查 nUserCount==0
    m_shAveLevel = static_cast<std::int16_t>(nAveValue / nUserCount);
}

void CPartyMatching::AutoMatchingAccept(std::uint32_t dwActorID,
                                        CServer* pServer,
                                        std::uint8_t byCheck,
                                        std::uint32_t dwUAID) {
    static_cast<void>(pServer);

    bool allAccept = true;
    for (int index = 0; index < 4; ++index) {
        if (dwActorID == m_stMatchingUser[index].m_stMemberInfo.dwMemberID) {
            if (byCheck != 0) {
                m_bCheck[index] = 100;
            } else {
                m_bCheck[index] = 10;
                SendMatchingExit(0, 1u, dwUAID);
                SetMatchingState(3u);
                m_byProcess = 0;
            }
        }

        if (m_stMatchingUser[index].m_pCurServer && m_bCheck[index] == 0) {
            allAccept = false;
        }
    }

    if (allAccept) {
        LeaderSelect();
        SendMatchingWait();
    }
}

void CPartyMatching::LeaderSelect() {
    std::int16_t maxLevel = 0;
    std::int64_t maxExp = 0;

    for (const CPartyMatchginMember& member : m_stMatchingUser) {
        if (!member.m_pCurServer) {
            continue;
        }

        if (maxLevel < member.m_stMemberInfo.byLevel ||
            (maxLevel == member.m_stMemberInfo.byLevel && maxExp < member.m_nExp)) {
            m_dwLeaderActorID = member.m_stMemberInfo.dwMemberID;
            maxLevel = member.m_stMemberInfo.byLevel;
            maxExp = member.m_nExp;
        }
    }
}

void CPartyMatching::CreateMazeMatching(std::uint32_t dwPartyID) {
    // 构造 PS_PARTY_INFO
    PS_PARTY_INFO stCreateParty{};
    stCreateParty.dwMaster = m_dwLeaderActorID;
    stCreateParty.dwPartyID = dwPartyID;
    stCreateParty.byPartyType = 1;

    // 构造 ST_CREATE_MAZE
    ST_CREATE_MAZE stCreateMaze{};
    stCreateMaze.wReqMapID = static_cast<std::uint16_t>(m_dwMazeID);
    stCreateMaze.nPortalID = static_cast<int>(m_dwPortalID);
    stCreateMaze.nJumpID = static_cast<int>(m_dwJumpID);

    // 遍历成员填充 party 和 maze 信息
    for (int i = 0; i < 4; ++i) {
        if (!m_stMatchingUser[i].m_pCurServer) {
            continue;
        }

        stCreateParty.vecPartyMember.push_back(m_stMatchingUser[i].m_stMemberInfo);

        ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
        stMemberInfo.dwMember = m_stMatchingUser[i].m_stMemberInfo.dwMemberID;
        stMemberInfo.nState = m_stMatchingUser[i].m_nState;
        stCreateMaze.vecEnterMember.push_back(stMemberInfo);
    }

    // 通过 CPartyManager 创建内存对象并发送 DB 请求
    TXSingleton<XRelayServer>::Instance()->GetPartyManager().CreatePartyMatching(stCreateParty);

    // 填充 maze 中的 party/force 信息
    stCreateMaze.stPartyInfo.byGroupType = 1;
    stCreateMaze.stPartyInfo.nID = dwPartyID;

    // 构造空的 PS_FORCE_INFO（原版用默认构造）
    PS_FORCE_INFO stCreateForce{};

    // 通过 control socket 发送 0xF2/0x43 创建迷宫请求
    XSendPacket xSendPacket(0xF2u, 0x43u);
    xSendPacket << stCreateMaze;
    xSendPacket << stCreateParty;
    xSendPacket << stCreateForce;
    xSendPacket.XParse << m_dwMachingID;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetControlSocket().Send(xSendPacket);
}

void CPartyMatchingMgr::OnUpdate() {
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

    const std::int64_t currentDate = GreenDamTan_GetCurDateSec();
    std::vector<std::uint32_t> recruitToDelete;
    ST_PARTY_RECRUIT_DEL_LIST deleteList{};

    for (const auto& [recruitID, recruit] : m_mpRecruit) {
        if (!recruit || recruit->GetRecruitDate() == 0 || currentDate <= recruit->GetRecruitDate()) {
            continue;
        }

        recruitToDelete.push_back(recruitID);

        ST_PARTY_RECRUIT_DEL deleteInfo{};
        deleteInfo.dwPartyID = recruit->GetPartyID();
        deleteInfo.dwMasterID = recruit->GetMasterID();
        deleteInfo.dwRecruitID = recruitID;
        deleteList.vecInfo.push_back(deleteInfo);
    }

    for (std::uint32_t recruitID : recruitToDelete) {
        DeletePartyRecruit(recruitID);
    }

    if (!deleteList.vecInfo.empty()) {
        XSendPacket packet(0xF4u, 0x26u);
        packet << deleteList;
        TXSingleton<XRelayServer>::Instance()->SendPacketAll(packet);
    }
}

void CPartyMatchingMgr::DeletePartyRecruit(std::uint32_t dwRecruitID) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(dwRecruitID);
    if (!recruit) {
        return;
    }

    if (const std::shared_ptr<CUserPartyInfo> userParty =
            TXSingleton<XRelayServer>::Instance()->GetPartyUser(recruit->GetMasterID())) {
        userParty->SetRecruitPenalty();
    }

    std::vector<std::uint32_t> applicantIDs;
    applicantIDs.reserve(std::size(recruit->m_stApplicantList.stInfo));
    for (const ST_APPLY_MEMBER& applicant : recruit->m_stApplicantList.stInfo) {
        if (applicant.stMember.dwMemberID != 0) {
            applicantIDs.push_back(applicant.stMember.dwMemberID);
        }
    }
    for (const std::uint32_t applicantID : applicantIDs) {
        if (const std::shared_ptr<CUserPartyInfo> userParty =
                TXSingleton<XRelayServer>::Instance()->GetPartyUser(applicantID)) {
            userParty->ClearApplyParty(dwRecruitID);
        }
    }

    recruit->ApplyMemberClear();
    recruit->ClearRecruitDate();

    std::vector<std::uint32_t> memberIDs;
    recruit->GetMember(memberIDs);
    for (std::uint32_t memberID : memberIDs) {
        if (const std::shared_ptr<CUserPartyInfo> userParty = TXSingleton<XRelayServer>::Instance()->GetPartyUser(memberID)) {
            userParty->SetRecruitDate(0);
        }
        m_mpRecruitUser.erase(memberID);
    }

    m_mpRecruit.erase(dwRecruitID);
}

void CPartyMatchingMgr::ClearRecruitDate(std::uint32_t dwUCID) {
    const std::uint32_t recruitID = FindRecruitID(dwUCID);
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(recruitID);
    if (recruit && recruit->GetMasterID() == dwUCID) {
        recruit->ClearRecruitDate();
    }
}

std::uint32_t CPartyMatchingMgr::FindRecruitID(std::uint32_t dwUCID) {
    const auto it = m_mpRecruitUser.find(dwUCID);
    return it == m_mpRecruitUser.end() ? 0u : it->second;
}

std::shared_ptr<CPartyRecruit> CPartyMatchingMgr::FindRecruitPtr(std::uint32_t dwRecruitID) {
    const auto it = m_mpRecruit.find(dwRecruitID);
    return it == m_mpRecruit.end() ? std::shared_ptr<CPartyRecruit>{} : it->second;
}

bool CPartyMatchingMgr::GetPartyRecruitInfo(std::uint32_t dwUCID, ST_PARTY_RECRUIT_INFO& stPartyRecruit) {
    const std::uint32_t recruitID = FindRecruitID(dwUCID);
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(recruitID);
    if (!recruit) {
        return false;
    }

    recruit->GetRecruitInfo(stPartyRecruit);
    return true;
}

std::uint8_t CPartyMatchingMgr::ReqPartyRecruitApply(ST_PARTY_RECRUIT_APPLY& stApply, ST_APPLY_MEMBER& stMember) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(stApply.dwRecruitID);
    if (!recruit) {
        return 1;
    }
    if (recruit->m_stPartyRecruit.byPartyGroupType != stApply.byPartyGroupType) {
        return 1;
    }

    stMember.nRegDate = static_cast<int>(GreenDamTan_GetCurDateSec() + 1800);
    return recruit->RecruitApply(stMember);
}

// 对齐 IDA: 返回void, shared_ptr按值传递, 最后参数为引用
void CPartyMatchingMgr::ReqPartyRecruitCreate(std::shared_ptr<CUserPartyInfo> pUserParty,
                                              ST_PARTY_RECRUIT& stRecruit,
                                              std::uint32_t& dwRecruitID) {
    if (!pUserParty) {
        return;
    }

    const std::uint32_t dwMasterID = pUserParty->GetActorID();
    stRecruit.nRemainTime = 1800;

    // 创建招募对象
    std::shared_ptr<CPartyRecruit> pPartyRecruit = std::make_shared<CPartyRecruit>();
    pPartyRecruit->SetRecruitInfo(m_dwRecruitID, dwMasterID, stRecruit);

    // 插入到m_mpRecruit
    m_mpRecruit[m_dwRecruitID] = pPartyRecruit;

    // 根据byPartyGroupType处理
    if (stRecruit.byPartyGroupType == 1) {
        // Party类型
        std::shared_ptr<CParty> pParty = TXSingleton<XRelayServer>::Instance()->GetPartyManager().GetParty(dwMasterID);
        if (!pParty) {
            // Party不存在，只添加请求者
            pPartyRecruit->SetCID(0);
            pPartyRecruit->SetRecruitDate();
            pUserParty->SetRecruitDate(pPartyRecruit->GetRecruitDate());
            AddRecruitMember(m_dwRecruitID, dwMasterID);
        } else {
            // Party存在，遍历成员并添加
            pPartyRecruit->SetCID(pParty->GetPartyID());
            // 获取Party成员列表并遍历
            ST_PARTY_MEMBER_LIST stMemberList;
            pParty->GetPartyMemberList(stMemberList);
            for (const auto& member : stMemberList.vecInfo) {
                if (member.dwMemberID == 0) {
                    continue;
                }
                const std::shared_ptr<CUserPartyInfo> pMemberPartyUser =
                    TXSingleton<XRelayServer>::Instance()->GetPartyUser(member.dwMemberID);
                if (pMemberPartyUser) {
                    pMemberPartyUser->SetRecruitDate(pPartyRecruit->GetRecruitDate());
                }
                AddRecruitMember(m_dwRecruitID, member.dwMemberID);
            }
        }
    } else if (stRecruit.byPartyGroupType == 2) {
        // Force类型
        std::shared_ptr<CForce> pForce = TXSingleton<XRelayServer>::Instance()->GetForceManager().GetForce(dwMasterID);
        if (!pForce) {
            // Force不存在，只添加请求者
            pPartyRecruit->SetCID(0);
            pPartyRecruit->SetRecruitDate();
            pUserParty->SetRecruitDate(pPartyRecruit->GetRecruitDate());
            AddRecruitMember(m_dwRecruitID, dwMasterID);
        } else {
            // Force存在，遍历成员并添加
            pPartyRecruit->SetCID(pForce->GetForceID());
            // 获取Force成员列表并遍历
            ST_PARTY_MEMBER_LIST vecMembers;
            pForce->GetForceMemberList(vecMembers);
            for (const auto& member : vecMembers.vecInfo) {
                if (member.dwMemberID == 0) {
                    continue;
                }
                const std::shared_ptr<CUserPartyInfo> pMemberPartyUser =
                    TXSingleton<XRelayServer>::Instance()->GetPartyUser(member.dwMemberID);
                if (pMemberPartyUser) {
                    pMemberPartyUser->SetRecruitDate(pPartyRecruit->GetRecruitDate());
                }
                AddRecruitMember(m_dwRecruitID, member.dwMemberID);
            }
        }
    }

    // 赋值返回并递增ID
    dwRecruitID = m_dwRecruitID;
    ++m_dwRecruitID;
}

bool CPartyMatchingMgr::ReqRecruitAccept(std::uint32_t dwRecruitID,
                                         ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT& stAccept,
                                         CServer* pServer) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(dwRecruitID);
    if (!recruit) {
        return false;
    }
    if (recruit->GetMasterID() != stAccept.dwMasterUCID) {
        return false;
    }
    if (recruit->m_stPartyRecruit.byPartyGroupType != stAccept.byPartyGroupType) {
        return false;
    }
    return recruit->RecruitAccept(pServer, stAccept.dwTargetUCID);
}

void CPartyMatchingMgr::ReqRecruitReject(std::uint32_t dwRecruitID, std::uint32_t dwTargetUCID) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(dwRecruitID);
    if (!recruit) {
        return;
    }
    recruit->DelApplyMember(dwTargetUCID, true);
}

// 对齐 IDA 0x14009EC90: 返回void
void CPartyMatchingMgr::ReqPartyRecruitDel(std::uint32_t dwActorID) {
    const std::uint32_t recruitID = FindRecruitID(dwActorID);
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(recruitID);
    if (!recruit) {
        return;
    }
    if (recruit->GetMasterID() != dwActorID) {
        return;
    }

    // 设置惩罚并清理日期（不完全删除recruit）
    const std::shared_ptr<CUserPartyInfo> pUser =
        TXSingleton<XRelayServer>::Instance()->GetPartyUser(recruit->GetMasterID());
    if (pUser) {
        pUser->SetRecruitPenalty();
    }
    recruit->ClearRecruitDate();
}

void CPartyMatchingMgr::SendPartyRecruitList(std::uint32_t dwActorID, CServer* pServer) {
    if (!pServer) {
        return;
    }

    ST_PARTY_RECRUIT_LIST recruitList{};
    const std::int64_t biCurDate = GreenDamTan_GetCurDateSec();

    for (const auto& [recruitID, recruit] : m_mpRecruit) {
        static_cast<void>(recruitID);
        if (!recruit || recruit->GetRecruitDate() == 0) {
            continue;
        }

        // 过期检查：当前时间超过recruitDate则跳过
        if (biCurDate > recruit->GetRecruitDate()) {
            continue;
        }

        ST_PARTY_RECRUIT_INFO recruitInfo{};
        recruit->GetRecruitInfo(recruitInfo);
        if (recruitInfo.stRecruit.dwRecruitID == 0) {
            continue;
        }

        // 计算剩余时间
        recruitInfo.stRecruit.nRemainTime = static_cast<int>(recruit->GetRecruitDate() - biCurDate);
        recruitList.vecInfo.push_back(recruitInfo.stRecruit);

        // 分批发送：每30条发一次
        if (recruitList.vecInfo.size() >= 30) {
            recruitList.bLast = 0;
            XSendPacket packet(0xF4u, 0x2Bu);
            packet.XParse << dwActorID;
            packet << recruitList;
            pServer->SendEx(packet);
            recruitList.vecInfo.clear();
        }
    }

    // 发送剩余的记录（bLast=1表示最后一批）
    recruitList.bLast = 1;
    XSendPacket packet(0xF4u, 0x2Bu);
    packet.XParse << dwActorID;
    packet << recruitList;
    pServer->SendEx(packet);
}

void CPartyMatchingMgr::SendPartyRecruitMyApplyList(std::uint32_t dwActorID, CServer* pServer) {
    if (!pServer) {
        return;
    }

    const std::shared_ptr<CUserPartyInfo> partyUser = TXSingleton<XRelayServer>::Instance()->GetPartyUser(dwActorID);
    if (!partyUser) {
        return;
    }

    ST_PARTY_RECRUIT_LIST recruitList{};
    partyUser->GetMYApplyRecruitInfo(recruitList);

    XSendPacket packet(0xF4u, 0x2Cu);
    packet.XParse << dwActorID;
    packet << recruitList;
    pServer->SendEx(packet);
}

void CPartyMatchingMgr::SendPartyRecruitApplyList(std::uint32_t dwActorID, CServer* pServer) {
    if (!pServer) {
        return;
    }

    const std::uint32_t recruitID = FindRecruitID(dwActorID);
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(recruitID);
    if (!recruit) {
        ST_APPLY_MEMBER_LIST emptyInfo{};
        XSendPacket packet(0xF4u, 0x2Du);
        packet.XParse << dwActorID;
        packet << emptyInfo;
        pServer->SendEx(packet);
        LogHelper::LogError("game.contents",
                            "[PARTY_RECRUIT] Failed ReqPartyRecruitApplyList - NULL == pRecruit %u ",
                            static_cast<unsigned int>(recruitID));
        return;
    }

    recruit->SendApplyUserList(pServer, dwActorID);
}

void CPartyMatchingMgr::SendPartyRecruitApplyInfo(std::uint32_t dwActorID,
                                                  std::uint32_t dwRecruitID,
                                                  CServer* pServer) {
    if (!pServer) {
        return;
    }

    ST_PARTY_RECRUIT_APPLY_INFO sendInfo{};
    sendInfo.dwActorID = dwActorID;
    sendInfo.dwRecruitID = dwRecruitID;

    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(dwRecruitID);
    if (!recruit) {
        sendInfo.nResult = 53021;

        XSendPacket packet(0xF4u, 0x30u);
        packet << sendInfo;
        pServer->SendEx(packet);
        LogHelper::LogError("game.contents",
                            "[PARTY_RECRUIT] Failed ReqPartyRecruitApplyInfo - NULL == pParty %u ",
                            static_cast<unsigned int>(dwRecruitID));
        return;
    }

    recruit->GetPartyMemberList(sendInfo);

    XSendPacket packet(0xF4u, 0x30u);
    packet << sendInfo;
    pServer->SendEx(packet);
}

void CPartyMatchingMgr::AddRecruitMember(std::uint32_t dwRecruitID, std::uint32_t dwMemberID) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(dwRecruitID);
    if (!recruit) {
        LogHelper::LogError("game.contents",
                            "[PARTY_RECRUIT] Failed AddRecruitMember - NULL == Recruit %u",
                            static_cast<unsigned int>(dwRecruitID));
        return;
    }

    recruit->AddMember(dwMemberID);
    m_mpRecruitUser[dwMemberID] = dwRecruitID;
}

void CPartyMatchingMgr::DeleteRecruitMember(std::uint32_t dwRecruitID, std::uint32_t dwMemberID) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(dwRecruitID);
    if (!recruit) {
        return;
    }

    recruit->RemoveMember(dwMemberID);
    m_mpRecruitUser.erase(dwMemberID);
}

void CPartyMatchingMgr::CreateParty(PS_REQ_PARTY_CREATE& stPartyReq) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(stPartyReq.dwRecruitID);
    if (!recruit) {
        return;
    }

    if (recruit->GetPartyGroupType() == 1) {
        recruit->SetCID(stPartyReq.dwPartyID);

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> userParty = relayServer.GetPartyUser(stPartyReq.memberInfo.dwMemberID);
        if (userParty) {
            userParty->SetRecruitDate(recruit->GetRecruitDate());
            userParty->DelPartyRecruit(stPartyReq.dwRecruitID, true);
        }

        AddRecruitMember(stPartyReq.dwRecruitID, stPartyReq.memberInfo.dwMemberID);
    } else {
        LogHelper::LogError("game.contents",
                            "[PARTY] FAILED CreateForce : E_PARTY_GROUP_TYPE_PARTY - ( RecruitID:%d, Type:%d ) ",
                            static_cast<int>(stPartyReq.dwRecruitID),
                            static_cast<int>(recruit->GetPartyGroupType()));
    }
}

void CPartyMatchingMgr::CreateForce(PS_REQ_FORCE_CREATE& stForceReq) {
    const std::shared_ptr<CPartyRecruit> recruit = FindRecruitPtr(stForceReq.dwRecruitID);
    if (!recruit) {
        return;
    }

    if (recruit->GetPartyGroupType() == 2) {
        recruit->SetCID(stForceReq.dwForceID);

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> userParty = relayServer.GetPartyUser(stForceReq.memberInfo.dwMemberID);
        if (userParty) {
            userParty->SetRecruitDate(recruit->GetRecruitDate());
            userParty->DelPartyRecruit(stForceReq.dwRecruitID, true);
        }

        AddRecruitMember(stForceReq.dwRecruitID, stForceReq.memberInfo.dwMemberID);
    } else {
        LogHelper::LogError("game.contents",
                            "[FORCE] FAILED CreateForce : E_PARTY_GROUP_TYPE_FORCE - ( RecruitID:%d, Type:%d ) ",
                            static_cast<int>(stForceReq.dwRecruitID),
                            static_cast<int>(recruit->GetPartyGroupType()));
    }
}

void CPartyMatchingMgr::ResPartyMatchingCreate(std::uint32_t dwMatchingID, std::uint32_t dwPartyID) {
    // 在 m_mpAutoMatching 中查找对应的 matching 对象
    const auto it = m_mpAutoMatching.find(dwMatchingID);
    if (it == m_mpAutoMatching.end() || !it->second) {
        return;
    }

    // 调用 CreateMazeMatching 创建迷宫匹配
    it->second->CreateMazeMatching(dwPartyID);
}

void CPartyMatching::SendCreateMatchingMaze(ST_CREATE_MAZE stCreateMaze, PS_PARTY_INFO stPartyInfo) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    for (int i = 0; i < 4; ++i) {
        if (!m_stMatchingUser[i].m_pCurServer) {
            continue;
        }

        // 设置 dwUserID 为当前成员 ID
        stCreateMaze.dwUserID = m_stMatchingUser[i].m_stMemberInfo.dwMemberID;

        // 发送 0xF4/0x42 包给每个成员
        XSendPacket packet(0xF4u, 0x42u);
        packet << stCreateMaze;
        packet << stPartyInfo;
        m_stMatchingUser[i].m_pCurServer->SendEx(packet);

        // 获取成员的 UserPartyInfo
        const std::shared_ptr<CUserPartyInfo> pUserParty =
            relayServer.GetPartyUser(m_stMatchingUser[i].m_stMemberInfo.dwMemberID);

        if (!pUserParty) {
            // 无效的 pUserParty，直接发 DBLog
            relayServer.SendDBLog(0,
                                   static_cast<int>(m_stMatchingUser[i].m_stMemberInfo.dwMemberID),
                                   22,
                                   10,
                                   static_cast<int>(m_stMatchingUser[i].m_stMemberInfo.byLevel),
                                   1,
                                   static_cast<int>(m_dwMazeID),
                                   static_cast<int>(stPartyInfo.dwMaster),
                                   0,
                                   static_cast<std::int64_t>(m_dwMachingID),
                                   static_cast<std::int64_t>(stPartyInfo.dwPartyID),
                                   L"");
            continue;
        }

        // 清理匹配状态
        pUserParty->SetMatchingState(false);
        pUserParty->SetMatchingID(0, 0);

        // 获取 UserObject 以获取 UAID
        const std::shared_ptr<CUserObject> pUser = relayServer.GetUser(m_stMatchingUser[i].m_stMemberInfo.dwMemberID);
        if (pUser) {
            relayServer.SendDBLog(static_cast<int>(pUser->GetUAID()),
                                   static_cast<int>(m_stMatchingUser[i].m_stMemberInfo.dwMemberID),
                                   22,
                                   10,
                                   static_cast<int>(m_stMatchingUser[i].m_stMemberInfo.byLevel),
                                   1,
                                   static_cast<int>(m_dwMazeID),
                                   static_cast<int>(stPartyInfo.dwMaster),
                                   0,
                                   static_cast<std::int64_t>(m_dwMachingID),
                                   static_cast<std::int64_t>(stPartyInfo.dwPartyID),
                                   L"");
        }
    }

    // 对齐 IDA: 设置发送成功标志并重置处理状态
    m_bSendSucc = true;
    m_byProcess = 0;
}

void CPartyMatchingMgr::SendCreateMatchingMaze(std::uint32_t dwMatchingID,
                                                ST_CREATE_MAZE stCreateMaze,
                                                PS_PARTY_INFO stPartyInfo) {
    // 查找 matching
    const auto it = m_mpAutoMatching.find(dwMatchingID);
    if (it == m_mpAutoMatching.end() || !it->second) {
        return;
    }

    // dispatch 到 CPartyMatching::SendCreateMatchingMaze
    it->second->SendCreateMatchingMaze(stCreateMaze, stPartyInfo);
}

// ============================================================================
// 对齐 IDA: 新增匹配管理方法
// ============================================================================

// 对齐 IDA: 第一个参数按值传递, 最后参数为引用
bool CPartyMatchingMgr::EnterMatching(ST_PARTY_MEMBER stMemberInfo, std::int64_t nExp,
                                       std::uint32_t wReqMapID, int nState, CServer* pServer,
                                       std::uint32_t& dwOutMatchingID) {
    // 对齐 IDA: 尝试进入已有匹配
    if (!pServer) {
        return false;
    }

    // 遍历现有匹配，寻找可加入的
    for (const auto& [matchingID, matching] : m_mpAutoMatching) {
        if (!matching) {
            continue;
        }

        // 检查地图 ID 是否匹配
        if (matching->m_dwMazeID != wReqMapID) {
            continue;
        }

        // 检查是否已满
        int currentCount = 0;
        for (const CPartyMatchginMember& member : matching->m_stMatchingUser) {
            if (member.m_stMemberInfo.dwMemberID != 0) {
                ++currentCount;
            }
        }

        if (currentCount >= 4) {
            continue;
        }

        // 找到可用位置
        for (int i = 0; i < 4; ++i) {
            if (matching->m_stMatchingUser[i].m_stMemberInfo.dwMemberID == 0) {
                matching->m_stMatchingUser[i].m_stMemberInfo = stMemberInfo;
                matching->m_stMatchingUser[i].m_nExp = nExp;
                matching->m_stMatchingUser[i].m_nState = nState;
                matching->m_stMatchingUser[i].m_pCurServer = pServer;
                dwOutMatchingID = matchingID;
                return true;
            }
        }
    }

    return false;
}

// 对齐 IDA: 第一个参数按值传递, portal/jump为uint32_t, 最后参数为引用
void CPartyMatchingMgr::CreateMatching(ST_PARTY_MEMBER stMemberInfo, std::int64_t nExp,
                                        std::uint32_t wReqMapID, int nState,
                                        std::uint32_t dwPortalID, std::uint32_t dwJumpID, CServer* pServer,
                                        std::uint32_t& dwOutMatchingID) {
    // 对齐 IDA 0x14009DAD0: 创建新匹配
    if (!pServer) {
        return;
    }

    // 生成新的 MatchingID
    static std::uint32_t s_dwMatchingIDCounter = 1;
    const std::uint32_t newMatchingID = s_dwMatchingIDCounter++;

    // 创建新的 CPartyMatching
    std::shared_ptr<CPartyMatching> pMatching = std::make_shared<CPartyMatching>();

    // 对齐 IDA: 调用 AutoMatchingCreate 进行完整初始化
    pMatching->AutoMatchingCreate(stMemberInfo, nExp, nState, newMatchingID, wReqMapID, dwPortalID, dwJumpID, pServer);

    // 插入到 m_mpAutoMatching
    m_mpAutoMatching[newMatchingID] = pMatching;

    dwOutMatchingID = newMatchingID;
}

// 对齐 IDA 0x14009DDB0: 返回bool, 参数(dwActorID, dwMatchingID, byReason, dwUAID)
bool CPartyMatchingMgr::ExitMatching(std::uint32_t dwActorID, std::uint32_t dwMatchingID,
                                      std::uint8_t byReason, std::uint32_t dwUAID) {
    const auto it = m_mpAutoMatching.find(dwMatchingID);
    if (it == m_mpAutoMatching.end()) {
        return false;
    }
    return it->second->AutoMatchingExit(dwActorID, byReason, dwUAID);
}

// 对齐 IDA 0x14009DE70: 返回bool, 参数(dwActorID, byCheck, pServer, dwUAID)
bool CPartyMatchingMgr::CheckMatching(std::uint32_t dwActorID, std::uint8_t byCheck,
                                       CServer* pServer, std::uint32_t dwUAID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    const std::shared_ptr<CUserPartyInfo> pUser = relayServer.GetPartyUser(dwActorID);
    if (!pUser) {
        return false;
    }

    // 检查用户是否在匹配中且有有效的匹配状态
    if (pUser->GetMatchingID() != 0 && pUser->GetMatchingState() != 0) {
        const std::uint32_t matchingID = pUser->GetMatchingID();
        const auto it = m_mpAutoMatching.find(matchingID);
        if (it == m_mpAutoMatching.end()) {
            return false;
        }
        it->second->AutoMatchingAccept(dwActorID, pServer, byCheck, dwUAID);
        return true;
    }
    return false;
}

