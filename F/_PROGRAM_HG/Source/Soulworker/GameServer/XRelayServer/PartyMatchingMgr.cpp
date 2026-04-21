#include "Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h"

#include <algorithm>
#include <chrono>
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

bool CPartyMatchingMgr::ReqPartyRecruitCreate(const std::shared_ptr<CUserPartyInfo>& pUserParty,
                                              const ST_PARTY_RECRUIT& stRecruit,
                                              std::uint32_t* pdwRecruitID) {
    if (!pUserParty) {
        return false;
    }

    const std::uint32_t actorID = pUserParty->GetActorID();
    if (actorID == 0) {
        return false;
    }

    std::shared_ptr<CPartyRecruit> recruit = std::make_shared<CPartyRecruit>();
    recruit->SetRecruitInfo(stRecruit);
    recruit->AddMember(actorID);
    recruit->SetRecruitDate();

    const std::uint32_t recruitID = ++m_dwRecruitID;
    recruit->m_stPartyRecruit.dwRecruitID = recruitID;
    recruit->m_stPartyRecruit.dwMasterUCID = actorID;
    recruit->m_stPartyRecruit.nRemainTime = 1800;

    m_mpRecruit[recruitID] = recruit;
    m_mpRecruitUser[actorID] = recruitID;
    pUserParty->SetRecruitDate(recruit->GetRecruitDate());

    if (pdwRecruitID) {
        *pdwRecruitID = recruitID;
    }
    return true;
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

bool CPartyMatchingMgr::ReqPartyRecruitDel(std::uint32_t dwActorID) {
    ST_PARTY_RECRUIT_INFO recruitInfo{};
    if (!GetPartyRecruitInfo(dwActorID, recruitInfo) || recruitInfo.stRecruit.dwMasterUCID != dwActorID) {
        return false;
    }

    DeletePartyRecruit(recruitInfo.stRecruit.dwRecruitID);
    return true;
}

void CPartyMatchingMgr::SendPartyRecruitList(std::uint32_t dwActorID, CServer* pServer) {
    if (!pServer) {
        return;
    }

    ST_PARTY_RECRUIT_INFO_LIST recruitList{};
    for (const auto& [recruitID, recruit] : m_mpRecruit) {
        static_cast<void>(recruitID);
        if (!recruit || recruit->GetRecruitDate() == 0) {
            continue;
        }

        ST_PARTY_RECRUIT_INFO recruitInfo{};
        recruit->GetRecruitInfo(recruitInfo);
        if (recruitInfo.stRecruit.dwRecruitID == 0) {
            continue;
        }
        recruitList.vecInfo.push_back(std::move(recruitInfo));
    }

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

    sendInfo.dwMasterUCID = recruit->GetMasterID();
    sendInfo.byPartyGroupType = recruit->GetPartyGroupType();
    sendInfo.nResult = recruit->GetPartyMemberList(sendInfo.stMemberList) ? 0 : 53021;

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

void CPartyMatching::SendCreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_PARTY_INFO& stPartyInfo) {
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
}

void CPartyMatchingMgr::SendCreateMatchingMaze(std::uint32_t dwMatchingID,
                                                ST_CREATE_MAZE& stCreateMaze,
                                                PS_PARTY_INFO& stPartyInfo) {
    // 查找 matching
    const auto it = m_mpAutoMatching.find(dwMatchingID);
    if (it == m_mpAutoMatching.end() || !it->second) {
        return;
    }

    // dispatch 到 CPartyMatching::SendCreateMatchingMaze
    it->second->SendCreateMatchingMaze(stCreateMaze, stPartyInfo);
}

