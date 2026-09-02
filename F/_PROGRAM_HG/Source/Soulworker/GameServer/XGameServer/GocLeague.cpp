// Guild/League System Implementation
// Decompiled from IDA GameServer.exe
// Target: GameServer.exe
// Port: 10004

#include "GocLeague.h"
#include "User.h"
#include "LeagueMember.h"
#include "UtilFunc.h"
#include "Process/ShopProcess.h"
#include "..\XSCommon\Packet.h"
#include "..\XSCommon\XSendPacket.h"
#include "..\XSCommon\GocNetwork.h"
#include "..\XGameServer\XGameServer.h"
#include "..\XGameServer\CommunitySocket.h"
#include "..\XGameServer\ThreadLocalData.h"
#include "..\XGameServer\LogicThreadManager.h"
#include "..\XSCommon\STDef.h"
#include <cstring>

// ============================================================================
// CGocLeague Implementation
// ============================================================================

// Address: 0x1400FA870
CGocLeague::CGocLeague()
{
    // Constructor - Hidden C++ exception states
    // Initialize base GOComponent
}

// Address: 0x1400FA8B0
CGocLeague::~CGocLeague()
{
    // Destructor
}

// Address: 0x1400FA920
void CGocLeague::Init()
{
    // Initialize guild component
    // Clear member data and reset invite state
    if (m_stLeagueMember)
    {
        memset(m_stLeagueMember, 0, sizeof(CLeagueMember));
    }
    m_dwInviteActorID = 0;
}

// Address: 0x1402AC5A0
int CGocLeague::GetFamilyID()
{
    // IDA: return 13
    return 13;
}

// ============================================================================
// CLeagueMember Implementation
// ============================================================================

// Address: 0x14028A5C0
CLeagueMember::CLeagueMember()
{
    // Constructor - Hidden C++ exception states
    // Initialize member map
    Clear();
}

// Address: 0x14028A600
void CLeagueMember::Clear()
{
    // Clear all guild members
    m_mapMembers.clear();
    m_nSyncCount = 0;
    m_nInventorySyncCount = 0;
}

// Address: 0x14028A630
void CLeagueMember::AddLeagueMember(CUser* pUser)
{
    // Add a guild member to tracking map
    if (!pUser)
        return;
    
    // Get actor ID
    UXActorID actorID;
    // TODO: Call pUser->GetActorID(&actorID);
    
    // Insert into member map
    m_mapMembers[actorID.dwActorID] = pUser;
}

// Address: 0x14028A930
void CLeagueMember::JoinLeagueUser(
    ST_LEAGUE_MEMBER_EX stMemberEx,
    ST_LEAGUE_INFO_UPDATE stInfoUpdate,
    unsigned char byApplyState)
{
    // Broadcast new member joining to all guild members
    // Hidden C++ exception states: #wind=3
    
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember)
            continue;
        
        // Skip if sync flag is set
        if (CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        // Skip if same UCID as joining member
        if (stMemberEx.dwUCID == pMember->GetLeagueID())
            continue;
        
        // Update sync count
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send join notification packet (0x22, 0x17)
        XSendPacket xSendPacket(0x22, 0x17);
        xSendPacket << stMemberEx;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
        
        // Send info update packet (0x22, 0x43)
        XSendPacket packet(0x22, 0x43);
        packet << stInfoUpdate;
        CGocNetwork::Send(&pMember->XActor, &packet);
        
        // If apply state is 1, send additional applicant notification
        if (byApplyState == 1)
        {
            XSendPacket v20(0x22, 0x26);
            v20.XParse << stMemberEx.dwUCID;
            CGocNetwork::Send(&pMember->XActor, &v20);
        }
    }
}

// Address: 0x14028BF00
void CLeagueMember::KickoutLeagueMember(
    int dwReqUCID,
    unsigned int dwKickoutUCID,
    ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    // Broadcast kick notification to all guild members
    // Hidden C++ exception states: #wind=2
    
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember)
            continue;
        
        // Skip requester and kicked member
        if (dwReqUCID == pMember->GetLeagueID() || 
            dwKickoutUCID == pMember->GetLeagueID())
            continue;
        
        // Skip if sync flag set
        if (CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        // Update sync count
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send kick notification packet (0x22, 0x41)
        XSendPacket xSendPacket(0x22, 0x41);
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << dwKickoutUCID;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
        
        // Send info update packet (0x22, 0x43)
        XSendPacket xSendPacket2(0x22, 0x43);
        xSendPacket2 << stUpdateInfo;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket2);
    }
    
    // Find and remove kicked user
    auto it = m_mapMembers.find(dwKickoutUCID);
    if (it != m_mapMembers.end())
    {
        CUser* pKickoutUser = it->second;
        if (pKickoutUser)
        {
            // Clear guild info from kicked user
            CUser::ClearLeagueInfo(pKickoutUser);
        }
        m_mapMembers.erase(it);
    }
}

// Address: 0x14028C6D0
void CLeagueMember::Levelup(
    int nLeagueID,
    unsigned char byLevel,
    char bySkillPoint,
    PS_AUTO_SKILL psAutoSkillInfo)
{
    // Broadcast guild level up to all members
    // Hidden C++ exception states: #wind=1
    
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember || CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        // Update member's guild level
        CUser::SetLeagueLevel(pMember, byLevel);
        
        // Update guild skills
        for (int i = 0; i < 8; ++i)
        {
            if (psAutoSkillInfo.bySkillInfo[i])
            {
                CUser::SetLeagueSkill(pMember, i, psAutoSkillInfo.bySkillInfo[i]);
            }
        }
        
        // Update sync count
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send level up packet (0x22, 0x52)
        XSendPacket xSendPacket(0x22, 0x52);
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << byLevel;
        xSendPacket.XParse << bySkillPoint;
        xSendPacket << psAutoSkillInfo;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
    }
}

// Address: 0x14028C590
void CLeagueMember::Wealth(ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    // Broadcast guild wealth update to all members
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember || CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send wealth update packet (0x22, 0x55)
        XSendPacket xSendPacket(0x22, 0x55);
        xSendPacket << stUpdateInfo;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
    }
}

// ============================================================================
// CLeagueProcess Implementation
// ============================================================================

// Address: 0x1404EBB20
CLeagueProcess::CLeagueProcess()
{
    // Constructor - Hidden C++ exception states
    // Initialize TXProcess base
    // Set command ID to 0x22
    // Set name to "CLeagueProcess"
}

// Address: 0x1404EBBE0
CLeagueProcess::~CLeagueProcess()
{
    // Destructor
}

// Address: 0x1404EBC10
bool CLeagueProcess::Parse(XPacket& xPacket)
{
    // Main packet routing for guild operations
    // Routes based on sub-command
    
    unsigned char bySubCmd = XPacket::GetSubCmd(&xPacket);
    
    switch (bySubCmd)
    {
        case 0x02: return ReqLeagueCreate(xPacket);
        case 0x03: return ReqLeagueDelete(xPacket);
        case 0x08: return ReqLeagueSearch(xPacket);
        case 0x10: return ReqLeagueOverlapName(xPacket);
        case 0x11: return ReqLeagueApplicant(xPacket);
        case 0x12: return ReqLeagueInfo(xPacket);
        case 0x13: return ReqLeagueWithDraw(xPacket);
        case 0x14: return ReqLeagueKick(xPacket);
        case 0x15: return ReqLeagueInvite(xPacket);
        case 0x16: return ReqLeagueInviteAccept(xPacket);
        case 0x17: return ReqLeagueInviteReject(xPacket);
        case 0x18: return ReqLeagueBoard(xPacket);
        case 0x19: return ReqLeagueApplicantAccept(xPacket);
        case 0x1A: return ReqLeagueApplicantReject(xPacket);
        case 0x1C: return ReqLeagueList(xPacket);
        case 0x1D: return ReqLeagueAuthChange(xPacket);
        case 0x1E: return ReqLeagueNoticeChange(xPacket);
        case 0x1F: return ReqLeagueNameChange(xPacket);
        case 0x20: return ReqLeagueCardChange(xPacket);
        case 0x21: return ReqLeaguePositionNameChange(xPacket);
        case 0x22: return ReqLeagueMemberPositionChange(xPacket);
        case 0x23: return ReqLeagueOpenOrNot(xPacket);
        case 0x24: return ReqLeagueRecruitNotice(xPacket);
        case 0x26: return ReqLeagueDelegate(xPacket);
        case 0x27: return ReqLeagueSkillLearn(xPacket);
        case 0x29: return ReqLeagueInventoryMove(xPacket);
        case 0x2A: return ReqLeagueInventoryInfo(xPacket);
        default: return true;
    }
}

// ============================================================================
// ReqLeagueSearch - IDA @ 0x1404EBF40
// 已精确还原 - 解析 ST_REQ_LEAGUE_SEARCH；GetClientPtr 空返回 0。
// lambda0 (0x1404EC170) DoJob: IsLive 门 -> 取 UCID -> 按 nState 分支
// (0=按会长名 szMasterName, 非0=按公会名 szLeagueName) 长度检查
// (len<2 时 SendErrorMessage(0x22,0x20,57032) + LogError "Shortage league
// name") -> (0xF6,0x18) 发送 stSearch 副本 + UCID ->
// CCommunitySocket::SendCmd(0x22,0x20)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueSearch(XPacket& xPacket)
{
    ST_REQ_LEAGUE_SEARCH stSearch;
    xPacket >> stSearch;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Per IDA lambda0 (0x1404EC170)
    std::function<void()> func = [pUser, stSearch]() {
        if (!pUser || !pUser->IsLive())
            return;

        std::uint32_t dwUCID = pUser->GetActorID().dwActorID;

        std::size_t nLen = 0;
        const wchar_t* szCheck = nullptr;
        if (stSearch.nState) {
            szCheck = stSearch.szMasterName;
        } else {
            szCheck = stSearch.szLeagueName;
        }
        while (szCheck[nLen] != 0) {
            ++nLen;
        }
        if (nLen < 2) {
            pUser->SendErrorMessage(0x22, 0x20, 57032);
            LogHelper::LogError("game.contents",
                "ReqLeagueSearch error - Shortage league name[ UCID:%d, %s ]",
                dwUCID,
                stSearch.nState ? "(master name)" : "(league name)");
            return;
        }

        XSendPacket xSendPacket(0xF6, 0x18);
        ST_REQ_LEAGUE_SEARCH stLocal = stSearch;
        xSendPacket << stLocal;
        xSendPacket.XParse << dwUCID;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x22, 0x20);
    };

    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueCreate - IDA @ 0x1404EC720
// 已精确还原 - 解析 ST_REQ_LEAGUE_CREATE；pUser 空或无 GetArea 返回 0；
// IncrementJobCount 后 lambda2 (0x1404EC9B0) DoJob:
// IsLive 门 -> CheckNpc(失败 SendErrorMessage(0x22,1,0xDED1)) ->
// 名称长度链: <2 "Be short of"(57011) / >8 "too long"(57011) ->
// GetGOC<CGocInventory>: Level<20 "Be Short of Level"(57012) ->
// Money<100000 "Be Short of Money"(57013) -> GetLeagueID!=0
// "Aleady Joined"(57008) -> GetLeagueDeletePenalty>0 时 (0x22,0x45)
// 发送剩余时间 -> 否则构造 PS_LEAGUE_CREATE_FOR_SERVER
// (dwActorID/szMasterName=stBaseInfo.strName/nCreateDate/byClass/byAwaken/
// dwProfilePhotoID/byLevel/sWorldID) (0xF6,1) SendCmd(0x22,1)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueCreate(XPacket& xPacket)
{
    ST_REQ_LEAGUE_CREATE stCreate;
    xPacket >> stCreate;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda2 (0x1404EC9B0)
    std::function<void()> func = [pUser, stCreate, this]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (!CheckNpc(stCreate.dwNpcID, 7, 0)) {
            CGocNetwork::SendErrorMessage(pUser, 0x22u, 1u, 0xDED1u);
            return;
        }

        ST_REQ_LEAGUE_CREATE stLeagueCreate = stCreate;
        std::size_t nLen = 0;
        while (stLeagueCreate.szName[nLen] != 0) {
            ++nLen;
        }
        if (nLen < 2) {
            LogHelper::LogError("game.contents",
                "ReqLeagueCreate error - Be short of League Name[ ActorID:%d, LeagueName:%ls ] ( %d )",
                pUser->GetActorID().dwActorID, stLeagueCreate.szName, 220);
            pUser->SendErrorMessage(0x22, 1, 57011);
            return;
        }
        if (nLen > 8) {
            LogHelper::LogError("game.contents",
                "ReqLeagueCreate error - League Name is too long[ ActorID:%d, LeagueName:%ls ] ( %d )",
                pUser->GetActorID().dwActorID, stLeagueCreate.szName, 226);
            pUser->SendErrorMessage(0x22, 1, 57011);
            return;
        }

        CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
        if (!pInven)
            return;

        if (pUser->stMyCharInfoEx()->byLevel < 20) {
            LogHelper::LogError("game.contents",
                "ReqLeagueCreate error - Be Short of Level[ ActorID:%d, Level:%d ] ( %d )",
                pUser->GetActorID().dwActorID,
                pUser->stMyCharInfoEx()->byLevel, 234);
            pUser->SendErrorMessage(0x22, 1, 57012);
            return;
        }
        if (pInven->GetMoney() < 100000) {
            LogHelper::LogError("game.contents",
                "ReqLeagueCreate error - Be Short of Money[ ActorID:%d, Money:%lld ] ( %d )",
                pUser->GetActorID().dwActorID,
                static_cast<long long>(pInven->GetMoney()), 241);
            pUser->SendErrorMessage(0x22, 1, 57013);
            return;
        }
        if (pUser->GetLeagueID() != 0) {
            LogHelper::LogError("game.contents",
                "ReqLeagueCreate error - Aleady Joined a League[ ActorID:%d, LeagueID:%d ] ( %d )",
                pUser->GetActorID().dwActorID, pUser->GetLeagueID(), 248);
            pUser->SendErrorMessage(0x22, 1, 57008);
            return;
        }
        if (pUser->GetLeagueDeletePenalty() > 0) {
            std::int64_t biPenalty = pUser->GetLeagueDeletePenalty();
            LogHelper::LogError("game.contents",
                "ReqLeagueCreate error - League Delete Penalty[ ActorID:%d, Penalty:%lld ] ( %d )",
                pUser->GetActorID().dwActorID,
                static_cast<long long>(biPenalty), 255);

            std::int64_t biRemainTime =
                biPenalty - XGameServer::Instance()->GetCurDate();
            XSendPacket packet(0x22, 0x45);
            packet.XParse << biRemainTime;
            CGocNetwork::Send(pUser, packet);
            return;
        }

        PS_LEAGUE_CREATE_FOR_SERVER stLeagueCreateForServer;
        ST_REQ_LEAGUE_CREATE stCreateInfo = stLeagueCreate;
        stLeagueCreateForServer.stCreateInfo = stCreateInfo;
        stLeagueCreateForServer.dwActorID = pUser->GetActorID().dwActorID;
        wcscpy_s(stLeagueCreateForServer.szMasterName,
                 pUser->stMyCharInfoEx()->stBaseInfo.strName);
        stLeagueCreateForServer.nCreateDate =
            static_cast<int>(XGameServer::Instance()->GetCurDate());
        stLeagueCreateForServer.byClass =
            pUser->stMyCharInfoEx()->stBaseInfo.byClass;
        stLeagueCreateForServer.byAwaken =
            pUser->stMyCharInfoEx()->stBaseInfo.byAwaken;
        stLeagueCreateForServer.dwProfilePhotoID =
            pUser->stMyCharInfoEx()->stBaseInfo.dwProfilePhotoID;
        stLeagueCreateForServer.byLevel = pUser->stMyCharInfoEx()->byLevel;
        stLeagueCreateForServer.sWorldID =
            pUser->stMyCharInfoEx()->stPosInfo.sWorldID;

        XSendPacket xSendPacket(0xF6, 1);
        xSendPacket << stLeagueCreateForServer;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x22, 1);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueDelete - IDA @ 0x1404ED240
// 已精确还原 - 解析 dwNPCID；先取 pUser/dwActorID/nLeagueID 再检查空与
// GetArea；lambda4 (0x1404ED490) DoJob: IsLive 门 ->
// CheckNpc(ActorID, dwNPCID, 7) 失败 SendErrorMessage(0x22,2,0xDED1) ->
// nLeagueID==0 时 LogDebug "Not League Member" + 57014 ->
// 否则 (0xF6,2) 发送 dwActorID/nLeagueID/GetCurDate ->
// SendCmd(0x22,2)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueDelete(XPacket& xPacket)
{
    std::uint32_t dwNPCID = 0;
    xPacket.XParse >> dwNPCID;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    std::uint32_t dwActorID = pUser->GetActorID().dwActorID;
    std::uint32_t nLeagueID = static_cast<std::uint32_t>(
        pUser->GetLeagueID());

    if (!pUser->GetArea())
        return false;

    // Per IDA lambda4 (0x1404ED490)
    std::function<void()> func = [pUser, nLeagueID, dwNPCID, dwActorID, this]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (!CheckNpc(dwNPCID, 7, 0)) {
            CGocNetwork::SendErrorMessage(pUser, 0x22u, 2u, 0xDED1u);
            return;
        }

        if (nLeagueID) {
            std::int64_t biCurDate = XGameServer::Instance()->GetCurDate();
            XSendPacket xSendPacket(0xF6, 2);
            xSendPacket.XParse << dwActorID;
            xSendPacket.XParse << nLeagueID;
            xSendPacket.XParse << biCurDate;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 2);
        } else {
            LogHelper::LogDebug("game.league",
                "Not League Member [ LeagueID:%d ]", nLeagueID);
            pUser->SendErrorMessage(0x22, 2, 57014);
        }
    };

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueApplicant - IDA @ 0x1404ED680
// 已精确还原 - 解析 ST_LEAGUE_APPLICANT；pUser 空或无 GetArea 返回 0；
// lambda5 (0x1401F4E10, A0x93366633 与社区侧共享) DoJob:
// IsLive+GetArea 门 -> (0x22,0x23) 发送 stApplicant 副本 + biRemainTime=0 ->
// ST_LOG_GAME{15,6, nParam0=stApplicant.nLeagueID, szComment=L"리그 "} ->
// SendDBLog。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueApplicant(XPacket& xPacket)
{
    ST_LEAGUE_APPLICANT stApplicant;
    xPacket >> stApplicant;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda5 (0x1401F4E10)
    std::function<void()> func = [pUser, stApplicant]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea())
            return;

        std::int64_t biRemainTime = 0;
        XSendPacket xSendPacket(0x22, 0x23);
        ST_LEAGUE_APPLICANT stLocal = stApplicant;
        xSendPacket << stLocal;
        xSendPacket.XParse << biRemainTime;
        CGocNetwork::Send(pUser, xSendPacket);

        ST_LOG_GAME stLog = {};
        stLog._nUAID = static_cast<int>(pUser->GetUAID());
        stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
        stLog._sMainType = 15;
        stLog._sSubType = 6;
        stLog.nParam0 = stLocal.nLeagueID;
        // IDA word_140B75C50 = L"리그 "
        std::wcscpy(stLog.szComment, L"리그 ");
        XGameServer::Instance()->SendDBLog(stLog);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueInfo - IDA @ 0x1404EDC80
// 已精确还原 - 不解析字段；pUser 空返回 0；GetArea 空返回 0；
// IncrementJobCount 后 lambda8 (0x1404EDE30) DoJob:
// IsLive 门 -> LogError "ReqLeagueInfo - [ucid:%d, leagueid:%d]" ->
// GetLeagueID!=0 时 (0xF6,6) 发送 dwActorID/nLeagueID ->
// SendCmd(0x22,7)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueInfo(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Per IDA lambda8 (0x1404EDE30)
    std::function<void()> func = [pUser]() {
        if (!pUser || !pUser->IsLive())
            return;

        int dwActorID = static_cast<int>(pUser->GetActorID().dwActorID);
        int nLeagueID = pUser->GetLeagueID();
        LogHelper::LogError("game.contents",
            "ReqLeagueInfo - [ucid:%d, leagueid:%d]", dwActorID, nLeagueID);
        if (nLeagueID) {
            XSendPacket xSendPacket(0xF6, 6);
            xSendPacket.XParse << dwActorID;
            xSendPacket.XParse << nLeagueID;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 7);
        }
    };

    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueWithDraw - IDA @ 0x1404EDF90
// 已精确还原 - 不解析字段；pUser 空返回 0；先取 nLeagueID/dwActorID；
// GetArea 空返回 0；lambda10 (0x1404EE1C0) DoJob:
// IsLive 门 -> nLeagueID!=0 时 (0xF6,8) 发送 dwActorID/nLeagueID/
// GetCurDate -> SendCmd(0x22,9)；否则 SendErrorMessage(0x22,9,57005) +
// LogDebug "Not League Member"。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueWithDraw(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    int nLeagueID = pUser->GetLeagueID();
    std::uint32_t dwActorID = pUser->GetActorID().dwActorID;

    // Per IDA lambda10 (0x1404EE1C0)
    std::function<void()> func = [pUser, nLeagueID, dwActorID]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (nLeagueID) {
            std::int64_t biCurDate = XGameServer::Instance()->GetCurDate();
            XSendPacket xSendPacket(0xF6, 8);
            xSendPacket.XParse << dwActorID;
            xSendPacket.XParse << nLeagueID;
            xSendPacket.XParse << biCurDate;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 9);
        } else {
            pUser->SendErrorMessage(0x22, 9, 57005);
            LogHelper::LogDebug("game.league",
                "Not League Member [LeagueID:%d]", nLeagueID);
        }
    };

    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueKick - IDA @ 0x1404EE340
// 已精确还原 - 解析 dwTargetActorID；pUser 空返回 0；先取
// nLeagueID/dwActorID；GetArea 空返回 0；
// lambda12 (0x1404EE5E0) DoJob: IsLive 门 ->
// dwTargetActorID==0: LogDebug "Fault TagetUser UCID" + 57003 ->
// nLeagueID==0: LogDebug "Not League Member [ReqUCID]" + 57016 ->
// 否则 (0xF6,9) 发送 dwActorID/dwTargetActorID/nLeagueID ->
// SendCmd(0x22,9)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueKick(XPacket& xPacket)
{
    std::uint32_t dwTargetActorID = 0;
    xPacket.XParse >> dwTargetActorID;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    int nLeagueID = pUser->GetLeagueID();
    std::uint32_t dwActorID = pUser->GetActorID().dwActorID;

    // Per IDA lambda12 (0x1404EE5E0)
    std::function<void()> func = [pUser, dwTargetActorID, nLeagueID,
                                  dwActorID]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (dwTargetActorID) {
            if (nLeagueID) {
                XSendPacket xSendPacket(0xF6, 9);
                xSendPacket.XParse << dwActorID;
                xSendPacket.XParse << dwTargetActorID;
                xSendPacket.XParse << nLeagueID;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x22, 9);
            } else {
                LogHelper::LogDebug("game.league",
                    "Not League Member [ReqUCID:%d]", dwActorID);
                pUser->SendErrorMessage(0x22, 9, 57016);
            }
        } else {
            LogHelper::LogDebug("game.league",
                "Fault TagetUser UCID [TargetUCID:%d", dwTargetActorID);
            pUser->SendErrorMessage(0x22, 9, 57003);
        }
    };

    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueInvite - IDA @ 0x1404EE790
// 已精确还原 - 解析 ST_REQ_LEAGUE_INVITE；先取 pUser/nLeagueID；
// pUser 空返回 0；GetArea 空返回 0；
// lambda14 (0x1404EEA70) DoJob: IsLive 门 ->
// nLeagueID!=0: stInvite.dwActorID=ActorID, nLeagueID=LeagueID,
// szReqName=GetName() -> (0xF6,0xC) -> SendCmd(0x22,0x13) ->
// 否则 SendErrorMessage(0x22,0x13,57016) + LogDebug "Can't Invite User"。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueInvite(XPacket& xPacket)
{
    ST_REQ_LEAGUE_INVITE stInvite;
    xPacket >> stInvite;

    CUser* pUser = GetClientPtr();
    int nLeagueID = pUser ? pUser->GetLeagueID() : 0;

    // Per IDA lambda14 (0x1404EEA70)
    std::function<void()> func = [pUser, stInvite, nLeagueID]() {
        if (!pUser || !pUser->IsLive())
            return;

        ST_REQ_LEAGUE_INVITE stLocal = stInvite;
        if (nLeagueID) {
            stLocal.dwActorID = pUser->GetActorID().dwActorID;
            stLocal.nLeagueID = nLeagueID;

            XSendPacket xSendPacket(0xF6, 0xC);
            std::wstring szName = pUser->GetName();
            wcscpy_s(stLocal.szReqName, szName.c_str());
            xSendPacket << stLocal;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x13);
        } else {
            stLocal.nResult = 57005;
            pUser->SendErrorMessage(0x22, 0x13, 57016);
            LogHelper::LogDebug("game.league",
                "Can't Invite User [ LeagueID:%d ]", nLeagueID);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueInviteAccept - IDA @ 0x1404EECF0
// 已精确还原 - 解析 ST_REQ_LEAGUE_INVITE_ACCEPT，捕获 biJoinDate =
// GetCurDate()；pUser 空返回 0；GetArea 空返回 0；
// lambda16 (0x1404EEFB0) DoJob: IsLive 门 ->
// GetLeagueID != 0: LogDebug "Aleady Joined League" + 57008 ->
// 否则 (0xF6,0xD) << stInvite 副本 << biJoinDate ->
// SendCmd(0x22,0x14)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueInviteAccept(XPacket& xPacket)
{
    ST_REQ_LEAGUE_INVITE_ACCEPT stInvite{};
    xPacket >> stInvite;

    CUser* pUser = GetClientPtr();
    std::int64_t biJoinDate = XGameServer::Instance()->GetCurDate();

    // Per IDA lambda16 (0x1404EEFB0)
    std::function<void()> func = [pUser, stInvite, biJoinDate]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetLeagueID()) {
            LogHelper::LogDebug("game.league",
                "Aleady Joined League [LeagueID:%d]", pUser->GetLeagueID());
            pUser->SendErrorMessage(0x22, 0x14, 57008);
        } else {
            XSendPacket xSendPacket(0xF6, 0xD);
            xSendPacket << stInvite;
            xSendPacket.XParse << biJoinDate;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x14);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueInviteReject - IDA @ 0x1404EF150
// 已精确还原 - 解析 ST_REQ_LEAGUE_INVITE_REJECT；pUser 空返回 0；
// GetArea 空返回 0；
// lambda18 (0x1404EF370) DoJob: IsLive 门 -> stReject 副本 ->
// dwTargetUCID == 0: LogDebug "Not Exist Target" + 51006 ->
// 否则 szTargetName 覆盖为 pUser 自身名字 -> (0xF6,0x10) << stReject ->
// SendCmd(0x22,0x15)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueInviteReject(XPacket& xPacket)
{
    ST_REQ_LEAGUE_INVITE_REJECT stReject{};
    xPacket >> stReject;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda18 (0x1404EF370)
    std::function<void()> func = [pUser, stReject]() {
        if (!pUser || !pUser->IsLive())
            return;

        ST_REQ_LEAGUE_INVITE_REJECT stLocal = stReject;
        if (stLocal.dwTargetUCID) {
            // 目标名覆盖为请求者自身名字（原始逐字符拷贝至 NUL）
            const wchar_t* szSrc = pUser->GetName().c_str();
            wchar_t* szDst = stLocal.szTargetName;
            do {
                *szDst = *szSrc;
                ++szSrc;
                ++szDst;
            } while (*szSrc);

            XSendPacket xSendPacket(0xF6, 0x10);
            xSendPacket << stLocal;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x15);
        } else {
            LogHelper::LogDebug("game.league",
                "Not Exist Target [UCID:%d]", 0);
            pUser->SendErrorMessage(0x22, 0x15, 51006);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueBoard - IDA @ 0x1404EF5B0
// 已精确还原 - 解析 ST_LEAGUE_BOARD；先取 dwActorID/nLeagueID；
// pUser 空返回 0；GetArea 空返回 0；
// lambda20 (0x1404EF8E0) DoJob: IsLive 门 ->
// nLeagueID == 0: 57005 + LogDebug "Not League Member [UCID]" ->
// 否则 stBoard 副本、biEnrollDate = GetCurDate() -> (0xF6,0x14) <<
// stBoard << dwActorID << nLeagueID -> SendCmd(0x22,8)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueBoard(XPacket& xPacket)
{
    ST_LEAGUE_BOARD stBoard{};
    xPacket >> stBoard;

    CUser* pUser = GetClientPtr();
    std::uint32_t dwActorID = pUser->GetActorID().dwActorID;
    int nLeagueID = pUser->GetLeagueID();

    // Per IDA lambda20 (0x1404EF8E0)
    std::function<void()> func = [pUser, nLeagueID, stBoard]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (nLeagueID) {
            ST_LEAGUE_BOARD stLeagueBoard = stBoard;
            stLeagueBoard.biEnrollDate =
                XGameServer::Instance()->GetCurDate();
            XSendPacket xSendPacket(0xF6, 0x14);
            xSendPacket << stLeagueBoard;
            xSendPacket.XParse << pUser->GetActorID().dwActorID;
            xSendPacket.XParse << nLeagueID;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 8);
        } else {
            pUser->SendErrorMessage(0x22, 8, 57005);
            LogHelper::LogDebug("game.league",
                "Not League Member [UCID:%d]", pUser->GetActorID().dwActorID);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueApplicantAccept - IDA @ 0x1404EFB10
// 已精确还原 - 解析 ST_REQ_LEAGUE_APPLICANT_ACCEPT；先取 dwActorID；
// pUser 空返回 0；GetArea 空返回 0；
// lambda22 (0x1404EFDF0) DoJob: IsLive 门 -> stAccept 副本 ->
// nLeagueID == 0: LogError "Not Exist League" + 57016 ->
// 否则 biJoinDate = GetCurDate() -> (0xF6,0x16) << stAccept <<
// dwActorID -> SendCmd(0x22,0x18)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueApplicantAccept(XPacket& xPacket)
{
    ST_REQ_LEAGUE_APPLICANT_ACCEPT stAccept{};
    xPacket >> stAccept;

    CUser* pUser = GetClientPtr();
    std::uint32_t dwActorID = pUser->GetActorID().dwActorID;

    // Per IDA lambda22 (0x1404EFDF0)
    std::function<void()> func = [pUser, stAccept, dwActorID]() {
        if (!pUser || !pUser->IsLive())
            return;

        ST_REQ_LEAGUE_APPLICANT_ACCEPT stLocal = stAccept;
        if (stLocal.nLeagueID) {
            stLocal.biJoinDate = XGameServer::Instance()->GetCurDate();
            XSendPacket xSendPacket(0xF6, 0x16);
            xSendPacket << stLocal;
            xSendPacket.XParse << dwActorID;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x18);
        } else {
            LogHelper::LogError("game.contents",
                "ReqLeagueApplicantAccept error - Not Exist League"
                "[ ActorID:%d, LeagueID:%d] ( %d )",
                pUser->GetActorID().dwActorID, stLocal.nLeagueID, 615);
            pUser->SendErrorMessage(0x22, 0x18, 57016);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueApplicantReject - IDA @ 0x1404EFFD0
// 已精确还原 - 解析 ST_REQ_LEAGUE_APPLICANT_REJECT；pUser 空返回 0；
// GetArea 空返回 0；
// lambda24 (0x1404F01C0) DoJob: pUser 且 IsLive -> stReject 副本 ->
// dwUCID 覆盖为 pUser->GetActorID().dwActorID ->
// (0xF6,0x17) << stReject -> SendCmd(0x22,0x19)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueApplicantReject(XPacket& xPacket)
{
    ST_REQ_LEAGUE_APPLICANT_REJECT stReject{};
    xPacket >> stReject;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda24 (0x1404F01C0)
    std::function<void()> func = [pUser, stReject]() {
        if (!pUser || !pUser->IsLive())
            return;

        ST_REQ_LEAGUE_APPLICANT_REJECT stLocal = stReject;
        stLocal.dwUCID = pUser->GetActorID().dwActorID;
        XSendPacket xSendPacket(0xF6, 0x17);
        xSendPacket << stLocal;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x22, 0x19);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueOverlapName - IDA @ 0x1404EC460
// 已精确还原 - GetWString(szLeagueName,10) -> 长度检查：
// len < 2 / len > 8 -> LogError(172) + 57033；
// IsUsableNameFilter 失败 -> LogError(180) + 57035；
// CheckValidString(GetNationType()) 失败 -> LogError(189) + 57035；
// 通过则 XSendDBPacket(pUser, 7, 0x12) << szLeagueName -> SendDBGame。
// ============================================================================
bool CLeagueProcess::ReqLeagueOverlapName(XPacket& xPacket)
{
    wchar_t szLeagueName[10] = {};
    std::int16_t sLen = 0;
    xPacket.XParse.GetWString(szLeagueName, 10, sLen);

    CUser* pUser = GetClientPtr();
    std::size_t uLen = std::wcslen(szLeagueName);

    if (uLen < 2 || uLen > 8) {
        LogHelper::LogError("game.contents",
            "ReqLeagueOverlapName error - Failed Valid Name"
            "[ LeagueName:%ls ] ( %d )", szLeagueName, 172);
        pUser->SendErrorMessage(0x22, 0x21, 57033);
        return false;
    }

    if (!UtilFunc::IsUsableNameFilter(szLeagueName)) {
        LogHelper::LogError("game.contents",
            "ReqLeagueOverlapName error - Failed Filter Valid Name"
            "[ LeagueName:%ls ] ( %d )", szLeagueName, 180);
        pUser->SendErrorMessage(0x22, 0x21, 57035);
        return false;
    }

    NATION_TYPE eNationType = XGameServer::Instance()->GetOption().GetNationType();
    if (!UtilFunc::CheckValidString(szLeagueName, eNationType)) {
        LogHelper::LogError("game.contents",
            "ReqLeagueOverlapName error - Failed Valid Name"
            "[ LeagueName:%ls ] ( %d )", szLeagueName, 189);
        pUser->SendErrorMessage(0x22, 0x21, 57035);
        return false;
    }

    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 7, 0x12);
    xSendDBPacket.XParse << std::wstring(szLeagueName);
    XGameServer::Instance()->SendDBGame(xSendDBPacket);

    return true;
}

// ============================================================================
// ResLeagueOverlapName - IDA @ 0x1404F0380
// 已精确还原 - DB 响应：解析 nErrorCode + GetWString(szLeagueName,10)；
// pUser 空返回 0；GetArea 空返回 0；
// lambda26 (0x1404F05D0) DoJob: IsLive 门 ->
// nErrorCode == 0: (0x22,0x21) << 0 -> CGocNetwork::Send ->
// nErrorCode == 1: (0x22,0x21) << 1 -> CGocNetwork::Send ->
// 其他 nErrorCode 不发送。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ResLeagueOverlapName(XPacket& xPacket)
{
    int nErrorCode = 0;
    xPacket.XParse >> nErrorCode;
    wchar_t szLeagueName[10] = {};
    std::int16_t sLen = 0;
    xPacket.XParse.GetWString(szLeagueName, 10, sLen);

    CUser* pUser = GetClientPtr();

    // Per IDA lambda26 (0x1404F05D0)
    std::function<void()> func = [pUser, nErrorCode]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (nErrorCode == 0 || nErrorCode == 1) {
            XSendPacket xSendPacket(0x22, 0x21);
            xSendPacket.XParse << nErrorCode;
            CGocNetwork::Send(pUser ? static_cast<XActor*>(pUser)
                                   : nullptr, &xSendPacket);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueList - IDA @ 0x1404F0780
// 已精确还原 - 解析 byType；先取 dwActorID；pUser 空返回 0；
// GetArea 空返回 0；
// lambda28 (0x1404F0A10) DoJob: pUser 且 IsLive ->
// (0xF6,0x24) << dwActorID << byType -> SendCmd(0x22,3)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueList(XPacket& xPacket)
{
    std::uint8_t byType = 0;
    xPacket.XParse >> byType;

    CUser* pUser = GetClientPtr();
    std::uint32_t dwActorID = pUser->GetActorID().dwActorID;

    // Per IDA lambda28 (0x1404F0A10)
    std::function<void()> func = [pUser, dwActorID, byType]() {
        if (!pUser || !pUser->IsLive())
            return;

        XSendPacket xSendPacket(0xF6, 0x24);
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << byType;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x22, 3);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueAuthChange - IDA @ 0x1404F0B10
// 已精确还原 - 解析 ST_LEAGUE_AUTH_CHANGE；pUser 空返回 0；
// GetArea 空返回 0；
// lambda30 (0x1404F0DB0) DoJob: IsLive 门 ->
// GetLeagueID == 0: LogDebug "Not Exist League" + 57016 ->
// 否则 (0xF6,0x28) << stChange << LeagueID << dwActorID ->
// SendCmd(0x22,0x32)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueAuthChange(XPacket& xPacket)
{
    ST_LEAGUE_AUTH_CHANGE stChange{};
    xPacket >> stChange;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda30 (0x1404F0DB0)
    std::function<void()> func = [pUser, stChange]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetLeagueID()) {
            XSendPacket xSendPacket(0xF6, 0x28);
            xSendPacket << stChange;
            xSendPacket.XParse << pUser->GetLeagueID();
            xSendPacket.XParse << pUser->GetActorID().dwActorID;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x32);
        } else {
            LogHelper::LogDebug("game.league",
                "Not Exist League [LeagueID:%d]", pUser->GetLeagueID());
            pUser->SendErrorMessage(0x22, 0x28, 57016);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueNoticeChange - IDA @ 0x1404F0F90
// 已精确还原 - 解析 ST_LEAGUE_NOTICE；pUser 空返回 0；
// GetArea 空返回 0；
// lambda32 (0x1404F1230) DoJob: IsLive 门 ->
// stNotice.nLeagueID != GetLeagueID:
//   LogDebug "Fault League ID" + 57016 ->
// 名长 > 0x320: LogDebug "Notice Size Over" ->
// 否则 biEnrollDate = GetCurDate() ->
// (0xF6,0x23) << stNotice << dwActorID -> SendCmd(0x22,0x29)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueNoticeChange(XPacket& xPacket)
{
    ST_LEAGUE_NOTICE stNotice{};
    xPacket >> stNotice;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda32 (0x1404F1230)
    std::function<void()> func = [pUser, stNotice]() {
        if (!pUser || !pUser->IsLive())
            return;

        ST_LEAGUE_NOTICE stLocal = stNotice;
        if (stLocal.nLeagueID == pUser->GetLeagueID()) {
            std::size_t uLen = std::wcslen(stLocal.szNotice);
            if (uLen <= 0x320) {
                stLocal.biEnrollDate = XGameServer::Instance()->GetCurDate();
                XSendPacket xSendPacket(0xF6, 0x23);
                xSendPacket << stLocal;
                xSendPacket.XParse << pUser->GetActorID().dwActorID;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x22, 0x29);
            } else {
                LogHelper::LogDebug("game.league", "Notice Size Over");
            }
        } else {
            LogHelper::LogDebug("game.league",
                "Fault League ID [LeagueID:%d]", stLocal.nLeagueID);
            pUser->SendErrorMessage(0x22, 0x29, 57016);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// CheckNameChangeItem - IDA @ 0x1404F1470
// 已精确还原 - 改名道具检查：
// pUser 空返回 52011；GOC 空返回 52011；槽位非法返回 52011；
// 物品空或锁定 -> LogError 814 + 52011；
// TB_ITEM 空 -> LogError 824 + 52004；TB_ITEM_CLASSIFY 空 -> LogError 831 + 52004；
// Item_Use_Type != 66 -> LogError 837 + 52002；
// ReduceItem3(invenType, slot, 1, 0x78) 失败 -> LogError 844 + 52010；
// 成功 -> *psUpdateList = psUpdateItemList，返回 0。
// ============================================================================
int CLeagueProcess::CheckNameChangeItem(
    PS_ITEM_SLOT_INFO psItemInfo,
    PS_RES_STORAGE_INFO& psUpdateList)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 52011;

    CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    if (!pInven)
        return 52011;

    if (!pInven->IsValidSlotPos(psItemInfo.shSlotPos, 0)) {
        return 52011;
    }

    bool bLock = false;
    std::shared_ptr<CItem> pItem =
        pInven->GetSlotItem(psItemInfo.byInvenType, psItemInfo.shSlotPos, bLock);
    if (!pItem || bLock) {
        LogHelper::LogError("game.league",
            "ReqLeagueNameChange error - Item is NULL or Lock[ UCID:%d ]( %d )",
            pUser->GetActorID().dwActorID, 814);
        return 52011;
    }

    int nItemID = pItem->GetID();
    XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
    TB_ITEM* pTB_Item = resourceMgr.GetTB_ITEM(nItemID);
    if (!pTB_Item) {
        LogHelper::LogError("game.league",
            "ReqLeagueNameChange error - No Table TB_ITEM[ ItemID:%d ]( %d )",
            nItemID, 824);
        return 52004;
    }

    TB_ITEM_CLASSIFY* pTB_ItemClassyfy =
        resourceMgr.GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    if (!pTB_ItemClassyfy) {
        LogHelper::LogError("game.league",
            "ReqLeagueNameChange error - No Table TB_ITEM_CLASSIFY[ ItemID:%d ]( %d )",
            nItemID, 831);
        return 52004;
    }

    if (pTB_ItemClassyfy->Item_Use_Type != 66) {
        LogHelper::LogError("game.league",
            "ReqLeagueNameChange error - Dismatch item_use_type[ ItemID:%d, Type:%d ]( %d )",
            nItemID, pTB_ItemClassyfy->Item_Use_Type, 837);
        return 52002;
    }

    PS_RES_STORAGE_INFO psUpdateItemList{};
    int nSlot = pItem->GetSlot();
    std::uint8_t byInvenType = pItem->GetInvenType();
    if (!pInven->ReduceItem3(byInvenType, static_cast<std::int16_t>(nSlot),
                             1, 0x78, psUpdateItemList)) {
        LogHelper::LogError("game.item",
            "ReqLeagueNameChange error - Failed ReduceItem3[ ActorID:%d, ItemID:%d ] ( %d )",
            pUser->GetActorID().dwActorID, nItemID, 844);
        return 52010;
    }

    psUpdateList = psUpdateItemList;
    return 0;
}

// ============================================================================
// SendLeagueNameChangeMsg - IDA @ 0x1404F1910
// 已精确还原 - 改名结果回包：
// pUser 空直接返回；构造 PS_RES_LEAGUE_NAME_CHANGE：
// nLeagueID = GetLeagueID(pUser)、nResult = 入参、
// szLeagueName 逐字符拷贝自 psChangeInfo（至 NUL）；
// (0x22,0x58) << psRes -> CGocNetwork::Send(pUser)。
// ============================================================================
void CLeagueProcess::SendLeagueNameChangeMsg(
    int nResult,
    PS_REQ_LEAGUE_NAME_CHANGE psNameChange)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return;

    PS_RES_LEAGUE_NAME_CHANGE psResNameChangeInfo{};
    psResNameChangeInfo.nLeagueID = pUser->GetLeagueID();
    psResNameChangeInfo.nResult = nResult;

    const wchar_t* szSrc = psNameChange.szLeagueName;
    wchar_t* szDst = psResNameChangeInfo.szLeagueName;
    do {
        *szDst = *szSrc;
        ++szSrc;
        ++szDst;
    } while (*szSrc);

    XSendPacket xSendPacket(0x22, 0x58);
    xSendPacket << psResNameChangeInfo;
    CGocNetwork::Send(static_cast<XActor*>(pUser), &xSendPacket);
}

// ============================================================================
// ReqLeagueNameChange - IDA @ 0x1404F1AA0
// 已精确还原 - 解析 PS_REQ_LEAGUE_NAME_CHANGE；pUser 空返回 0；
// GetArea 空返回 0；
// lambda34 (0x1404F1D30) DoJob: IsLive 门 ->
// GetGOC 空或 GetReqLeagueNameChange 已挂起 -> 直接释放返回 ->
// 否则 SetReqLeagueNameChange(true) ->
// LeagueID==0 或不匹配 stChange.nLeagueID:
//   SetReqLeagueNameChange(false) + SendLeagueNameChangeMsg(57015) + LogError 906 ->
// CheckNameChangeItem > 0:
//   SetReqLeagueNameChange(false) + SendLeagueNameChangeMsg(nResult) ->
// IsUsableNameFilter 失败:
//   Set(false) + UnLockList + Msg(57035) + LogError 924 ->
// CheckValidString 失败: Set(false) + UnLockList + Msg(57035) + LogError 936 ->
// 名长 <2: Set(false) + UnLockList + Msg(57033) + LogError 948 ->
// 名长 >8: Set(false) + UnLockList + Msg(57033) + LogError 959 ->
// 通过: PS_LEAGUE_NAME_CHANGE_SERVER{dwUCID,nLeagueID,psUpdateItemList,szLeagueName}
//   -> (0xF6,0x25) -> SendCmd(0x22,0x30)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueNameChange(XPacket& xPacket)
{
    PS_REQ_LEAGUE_NAME_CHANGE stChange{};
    xPacket >> stChange;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda34 (0x1404F1D30): 捕获 pUser/stChange/this
    std::function<void()> func = [this, pUser, stChange]() {
        if (!pUser || !pUser->IsLive())
            return;

        CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
        if (!pInven || pInven->GetReqLeagueNameChange())
            return;

        pInven->SetReqLeagueNameChange(true);

        if (pUser->GetLeagueID() && pUser->GetLeagueID() == stChange.nLeagueID) {
            PS_RES_STORAGE_INFO psUpdateItemList{};
            int nResult = CheckNameChangeItem(stChange.psChangeItemInfo,
                                             psUpdateItemList);
            if (nResult <= 0) {
                if (UtilFunc::IsUsableNameFilter(stChange.szLeagueName)) {
                    NATION_TYPE eNationType =
                        XGameServer::Instance()->GetOption().GetNationType();
                    if (UtilFunc::CheckValidString(stChange.szLeagueName,
                                                   eNationType)) {
                        std::size_t uLen = std::wcslen(stChange.szLeagueName);
                        if (uLen < 2) {
                            LogHelper::LogError("game.contents",
                                "ReqLeagueNameChange error - Shortage league name size[ ActorID:%d, LeagueName:%ls ] ( %d )",
                                pUser->GetActorID().dwActorID, stChange.szLeagueName, 948);
                            pInven->SetReqLeagueNameChange(false);
                            pInven->UnLockList(psUpdateItemList);
                            SendLeagueNameChangeMsg(57033, stChange);
                        } else if (uLen > 8) {
                            LogHelper::LogError("game.contents",
                                "ReqLeagueNameChange error - Over league name size[ ActorID:%d, LeagueName:%ls ] ( %d )",
                                pUser->GetActorID().dwActorID, stChange.szLeagueName, 959);
                            pInven->SetReqLeagueNameChange(false);
                            pInven->UnLockList(psUpdateItemList);
                            SendLeagueNameChangeMsg(57033, stChange);
                        } else {
                            PS_LEAGUE_NAME_CHANGE_SERVER psChangeNameServer{};
                            psChangeNameServer.dwUCID =
                                pUser->GetActorID().dwActorID;
                            psChangeNameServer.nLeagueID = pUser->GetLeagueID();
                            psChangeNameServer.psUpdateItemList = psUpdateItemList;
                            wcscpy_s(psChangeNameServer.szLeagueName,
                                     stChange.szLeagueName);
                            XSendPacket xSendPacket(0xF6, 0x25);
                            xSendPacket << psChangeNameServer;
                            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                                &xSendPacket, pUser, 0x22, 0x30);
                        }
                    } else {
                        LogHelper::LogError("game.contents",
                            "ReqLeagueNameChange error - Failed Valid Name[ LeagueName:%ls ] ( %d )",
                            stChange.szLeagueName, 936);
                        pInven->SetReqLeagueNameChange(false);
                        pInven->UnLockList(psUpdateItemList);
                        SendLeagueNameChangeMsg(57035, stChange);
                    }
                } else {
                    LogHelper::LogError("game.contents",
                        "ReqLeagueNameChange error - Failed Filter Valid Name[ LeagueName:%ls ] ( %d )",
                        stChange.szLeagueName, 924);
                    pInven->SetReqLeagueNameChange(false);
                    pInven->UnLockList(psUpdateItemList);
                    SendLeagueNameChangeMsg(57035, stChange);
                }
            } else {
                pInven->SetReqLeagueNameChange(false);
                SendLeagueNameChangeMsg(nResult, stChange);
            }
        } else {
            pInven->SetReqLeagueNameChange(false);
            SendLeagueNameChangeMsg(57015, stChange);
            LogHelper::LogError("game.league",
                "ReqLeagueNameChange error - Failed league id[ LeagueID:%d ]( %d )",
                pUser->GetLeagueID(), 906);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueCardChange - IDA @ 0x1404F25E0
// 已精确还原 - 解析 PS_REQ_LEAGUE_CARD；pUser 空返回 0；GetArea 空返回 0；
// lambda36 (0x1404F27E0) DoJob: IsLive 门 ->
// LeagueID==0 或 != psCard.nLeagueID:
//   LogDebug "Failed league id"(999) + 57016 ->
// GetLeagueInfo 后 dwLeagueCard != 0 时执行道具链：
//   GOC 空返回；槽位非法 LogError 1022；物品空/锁 52012(1033)；
//   TB_ITEM 空 52004(1043)；TB_ITEM_CLASSIFY 空 52004(1052)；
//   Item_Use_Type != 98 52002(1060)；
//   ReduceItem3(type,slot,1,0x46) 失败 52011(1069)；
//   ST_LOG_GAME{_sSubType=22, nParam4=itemID, nParam6=serial} ->
//   UpdateItemEnd(0x46, updateList, stLog) 失败 52011(1082)+UnLockList ->
// 最后 (0xF6,0x26) << psCard << dwActorID << vecUpdateItem ->
// SendCmd(0x22,0x31)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueCardChange(XPacket& xPacket)
{
    PS_REQ_LEAGUE_CARD psCard{};
    xPacket >> psCard;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda36 (0x1404F27E0)
    std::function<void()> func = [pUser, psCard]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (!pUser->GetLeagueID() || pUser->GetLeagueID() != psCard.nLeagueID) {
            LogHelper::LogDebug("game.league",
                "ReqLeagueCardChange error - Failed league id[ LeagueID:%d ]( %d )",
                pUser->GetLeagueID(), 999);
            pUser->SendErrorMessage(0x22, 0x31, 57016);
            return;
        }

        PS_RES_STORAGE_INFO vecUpdateItem{};
        ST_LEAGUE_INFO_EX stLeagueInfo{};
        pUser->GetLeagueInfo(stLeagueInfo);

        if (stLeagueInfo.dwLeagueCard) {
            CGocInventory* pInventory = pUser->GetGOC<CGocInventory>();
            if (!pInventory) {
                LogHelper::LogDebug("game.league",
                    "ReqLeagueCardChange error - No Inventory[ UCID:%d ]( %d )",
                    pUser->GetActorID().dwActorID, 1016);
                return;
            }

            if (!pInventory->IsValidSlotPos(psCard.shSlot, 0)) {
                LogHelper::LogError("game.league",
                    "ReqLeagueCardChange error - Fault Inventory Pos[Pos:%d]( %d )",
                    psCard.shSlot, 1022);
                return;
            }

            bool bLock = false;
            std::shared_ptr<CItem> pItem =
                pInventory->GetSlotItem(0xD, psCard.shSlot, bLock);
            if (!pItem || bLock) {
                pUser->SendErrorMessage(0x22, 0x31, 52012);
                LogHelper::LogDebug("game.league",
                    "ReqLeagueCardChange error - Item is NULL or Lock[ UCID:%d ]( %d )",
                    pUser->GetActorID().dwActorID, 1033);
                return;
            }

            int nItemID = pItem->GetID();
            XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
            TB_ITEM* pTB_Item = resourceMgr.GetTB_ITEM(nItemID);
            if (!pTB_Item) {
                LogHelper::LogDebug("game.league",
                    "ReqLeagueCardChange error - No Table TB_ITEM[ ItemID:%d ]( %d )",
                    nItemID, 1043);
                pUser->SendErrorMessage(0x22, 0x31, 52004);
                return;
            }

            TB_ITEM_CLASSIFY* pTB_ItemClassyfy =
                resourceMgr.GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
            if (!pTB_ItemClassyfy) {
                LogHelper::LogDebug("game.league",
                    "ReqLeagueCardChange error - No Table TB_ITEM_CLASSIFY[ ItemID:%d ]( %d )",
                    nItemID, 1052);
                pUser->SendErrorMessage(0x22, 0x31, 52004);
                return;
            }

            if (pTB_ItemClassyfy->Item_Use_Type != 98) {
                LogHelper::LogDebug("game.league",
                    "ReqLeagueCardChange error - Dismatch item_use_type[ ItemID:%d, Type:%d ]( %d )",
                    nItemID, pTB_ItemClassyfy->Item_Use_Type, 1060);
                pUser->SendErrorMessage(0x22, 0x31, 52002);
                return;
            }

            int nSlot = pItem->GetSlot();
            std::uint8_t byInvenType = pItem->GetInvenType();
            if (!pInventory->ReduceItem3(byInvenType,
                                         static_cast<std::int16_t>(nSlot),
                                         1, 0x46, vecUpdateItem)) {
                LogHelper::LogError("game.item",
                    "ReqLeagueCardChange error - [ ActorID:%d, ItemID:%d ] ( %d )",
                    pUser->GetActorID().dwActorID, nItemID, 1069);
                pUser->SendErrorMessage(0x22, 0x31, 52011);
                return;
            }

            ST_LOG_GAME stLog{};
            stLog._sSubType = 22;
            stLog.nParam4 = nItemID;
            stLog.nParam6 = pItem->GetSerial();

            if (!pInventory->UpdateItemEnd(0x46, vecUpdateItem, stLog)) {
                LogHelper::LogError("game.item",
                    "ReqLeagueCardChange error - [ ActorID:%d, ItemID:%d ] ( %d )",
                    pUser->GetActorID().dwActorID, nItemID, 1082);
                pInventory->UnLockList(vecUpdateItem);
                pUser->SendErrorMessage(0x22, 0x31, 52011);
                return;
            }
        }

        XSendPacket xSendPacket(0xF6, 0x26);
        xSendPacket << psCard;
        xSendPacket.XParse << pUser->GetActorID().dwActorID;
        xSendPacket << vecUpdateItem;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x22, 0x31);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeaguePositionNameChange - IDA @ 0x1404F30C0
// 已精确还原 - 解析 ST_LEAGUE_POSITION_NAME_CHANGE；pUser 空返回 0；
// GetArea 空返回 0；
// lambda38 (0x1404F32E0) DoJob: IsLive 门 ->
// 名长 <1 或 >4: LogDebug "League Position Name Size is 4" + 57027 ->
// IsUsableNameFilter 失败: LogDebug "Valid Filter Name" + 57035 ->
// CheckValidString 失败: LogDebug + 57035 ->
// GetLeagueID == 0: LogDebug "Fault League ID" + 57016 ->
// 否则 (0xF6,0x27) << stChange << dwActorID << LeagueID ->
// SendCmd(0x22,0x33)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeaguePositionNameChange(XPacket& xPacket)
{
    ST_LEAGUE_POSITION_NAME_CHANGE stChange{};
    xPacket >> stChange;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda38 (0x1404F32E0)
    std::function<void()> func = [pUser, stChange]() {
        if (!pUser || !pUser->IsLive())
            return;

        std::size_t uLen = std::wcslen(stChange.szLeagueName);
        if (uLen < 1 || uLen > 4) {
            LogHelper::LogDebug("game.league",
                "League Position Name Size is 4 [NAME:%ls]",
                stChange.szLeagueName);
            pUser->SendErrorMessage(0x22, 0x33, 57027);
            return;
        }

        NATION_TYPE eNationType =
            XGameServer::Instance()->GetOption().GetNationType();
        if (!UtilFunc::IsUsableNameFilter(stChange.szLeagueName)) {
            LogHelper::LogDebug("game.league",
                "ReqLeaguePositionNameChange error - Valid Filter Name [NAME:%ls]",
                stChange.szLeagueName);
            pUser->SendErrorMessage(0x22, 0x33, 57035);
            return;
        }

        if (!UtilFunc::CheckValidString(stChange.szLeagueName, eNationType)) {
            LogHelper::LogDebug("game.league",
                "League Position Name Size is 4 [NAME:%ls]",
                stChange.szLeagueName);
            pUser->SendErrorMessage(0x22, 0x33, 57035);
            return;
        }

        if (pUser->GetLeagueID()) {
            XSendPacket xSendPacket(0xF6, 0x27);
            xSendPacket << stChange;
            xSendPacket.XParse << pUser->GetActorID().dwActorID;
            xSendPacket.XParse << pUser->GetLeagueID();
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x33);
        } else {
            LogHelper::LogDebug("game.league",
                "Fault League ID [LeagueID:%d]", pUser->GetLeagueID());
            pUser->SendErrorMessage(0x22, 0x33, 57016);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueMemberPositionChange - IDA @ 0x1404F36A0
// 已精确还原 - 解析 ST_LEAGUE_MEMBER_POSITION；pUser 空返回 0；
// GetArea 空返回 0；
// lambda40 (0x1404F3890) DoJob: IsLive 门 ->
// GetLeagueID == 0: LogDebug "Not Exist League" + 57016 ->
// dwActorID == 0: LogError "Fault target user ucid"(1173) + 57016 ->
// 否则 (0xF6,0x37) << stPosition << LeagueID << dwActorID ->
// SendCmd(0x22,0x39)。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueMemberPositionChange(XPacket& xPacket)
{
    ST_LEAGUE_MEMBER_POSITION stPosition{};
    xPacket >> stPosition;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda40 (0x1404F3890)
    std::function<void()> func = [pUser, stPosition]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetLeagueID()) {
            if (stPosition.dwActorID) {
                XSendPacket xSendPacket(0xF6, 0x37);
                xSendPacket << stPosition;
                xSendPacket.XParse << pUser->GetLeagueID();
                xSendPacket.XParse << pUser->GetActorID().dwActorID;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x22, 0x39);
            } else {
                LogHelper::LogError("game.league",
                    "ReqLeagueMemberPositionChange error - Fault target user ucid[ UCID:%d ](%d)",
                    stPosition.dwActorID, 1173);
                pUser->SendErrorMessage(0x22, 0x37, 57016);
            }
        } else {
            LogHelper::LogDebug("game.league",
                "Not Exist League [LeagueID:%d]", pUser->GetLeagueID());
            pUser->SendErrorMessage(0x22, 0x37, 57016);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueOpenOrNot - IDA @ 0x1404F3AC0
// 已精确还原 - 解析 ST_LEAGUE_OPEN；pUser 空返回 0；GetArea 空返回 0；
// lambda44 (0x1404F3CB0) DoJob: IsLive 门 ->
// GetLeagueID != stOpen.nLeagueID:
//   LogError "Dismatch leagueID"(1219) + 57005 ->
// 否则 (0xF6,0x45) << stOpen << dwActorID -> SendCmd(0x22,0x46)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueOpenOrNot(XPacket& xPacket)
{
    ST_LEAGUE_OPEN stOpen{};
    xPacket >> stOpen;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda44 (0x1404F3CB0)
    std::function<void()> func = [pUser, stOpen]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetLeagueID() == stOpen.nLeagueID) {
            XSendPacket xSendPacket(0xF6, 0x45);
            xSendPacket << stOpen;
            xSendPacket.XParse << pUser->GetActorID().dwActorID;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0x46);
        } else {
            LogHelper::LogError("game.league",
                "ReqLeagueOpenOrNot error - Dismatch leagueID ( %d )", 1219);
            pUser->SendErrorMessage(0x22, 0x46, 57005);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueRecruitNotice - IDA @ 0x1404F3E40
// 已精确还原 - 解析 ST_LEAGUE_RECRUIT_NOTICE；pUser 空返回 0；
// GetArea 空返回 0；
// lambda46 (0x1404F4100) DoJob: IsLive 门 ->
// GetLeagueID != stNotice.nLeagueID:
//   LogError "Dismatch leagueID"(1247) + 57005 ->
// 名长 > 0x33: LogError "Over size recruit notice"(1254) + 57039 ->
// 否则 biRegDate = GetCurDate() ->
// (0xF6,0x46) << dwActorID << stNotice -> SendCmd(0x22,0x47)。
// 注意包序：dwActorID 在结构体之前。+ lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueRecruitNotice(XPacket& xPacket)
{
    ST_LEAGUE_RECRUIT_NOTICE stRecruitNotice{};
    xPacket >> stRecruitNotice;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda46 (0x1404F4100)
    std::function<void()> func = [pUser, stRecruitNotice]() {
        if (!pUser || !pUser->IsLive())
            return;

        ST_LEAGUE_RECRUIT_NOTICE stLocal = stRecruitNotice;
        if (pUser->GetLeagueID() == stLocal.nLeagueID) {
            std::size_t uLen = std::wcslen(stLocal.szNotice);
            if (uLen <= 0x33) {
                stLocal.biRegDate = XGameServer::Instance()->GetCurDate();
                XSendPacket xSendPacket(0xF6, 0x46);
                xSendPacket.XParse << pUser->GetActorID().dwActorID;
                xSendPacket << stLocal;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x22, 0x47);
            } else {
                LogHelper::LogError("game.league",
                    "ReqLeagueRecruitNotice error - Over size recruit notice( %d )", 1254);
                pUser->SendErrorMessage(0x22, 0x47, 57039);
            }
        } else {
            LogHelper::LogError("game.league",
                "ReqLeagueRecruitNotice error - Dismatch leagueID[ UCID:%d, LeagueID:%d] ( %d )",
                pUser->GetActorID().dwActorID, pUser->GetLeagueID(), 1247);
            pUser->SendErrorMessage(0x22, 0x47, 57005);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// CheckNpc - IDA @ 0x1404F4380
// 已精确还原 - PDB 签名 ?CheckNpc@CLeagueProcess@@QEAA_NKEH@Z：
// (uint dwNpcID, uchar byNpcType, int nNpcFunID)。
// pUser 无 Area 返回 false；FindActor(dwNpcID) 空或类型 != 1
// (E_ACTOR_TYPE NPC) -> LogError 1278 + false；
// GetProcessPtr<CShopProcess>(9) 空 -> LogError 1285 + false；
// 取 CNpc 的商店 ID（vtable+0x168），
// GetShopID(shopID, byNpcType) != nNpcFunID ->
// LogError 1292 + false；否则 true。
// ============================================================================
bool CLeagueProcess::CheckNpc(unsigned int dwNpcID, unsigned char byNpcType, int nNpcFunID)
{
    CUser* pUser = GetClientPtr();
    if (!pUser->GetArea())
        return false;

    XArea* pArea = pUser->GetArea();
    XActor* pNpc = pArea->FindActor(dwNpcID);
    if (pNpc && pNpc->GetType() == 1) {
        CShopProcess* pProcess = pUser->GetProcessPtr<CShopProcess>(9);
        if (pProcess) {
            // IDA: RTTI cast XActor -> CNpc 后 vtable+0x168 调用
            // 即 ?GetTableID@CNpc@@UEAAHXZ（CMover::GetTableID 虚分派）
            CMover* pMoverNpc = static_cast<CMover*>(static_cast<CMoverEx*>(pNpc));
            int nTableID = pMoverNpc->GetTableID();
            int nNPCFunctionID = pProcess->GetShopID(nTableID, byNpcType);
            if (nNpcFunID == nNPCFunctionID) {
                return true;
            } else {
                LogHelper::LogError("game.contents",
                    "CheckNpc error - Different FunctionID each other[ NpcId:%d, NpcFunc:%d, TB:%d ] ( %d )",
                    pMoverNpc->GetID(), nNpcFunID, nNPCFunctionID, 1292);
                return false;
            }
        } else {
            LogHelper::LogError("game.contents",
                "CheckNpc error - Not exist shop process( %d )", 1285);
            return false;
        }
    } else {
        LogHelper::LogError("game.contents",
            "CheckNpc error - Npc location or type[ NPC:%d ] ( %d )", dwNpcID, 1278);
        return false;
    }
}

// ============================================================================
// ReqLeagueDelegate - IDA @ 0x1404F4570
// 已精确还原 - 解析 PS_REQ_LEAGUE_DELEGATE；pUser 空返回 0；
// GetArea 空返回 0；
// lambda48 (0x1404F4780) DoJob:
// CheckNpc(dwNpcID, 7, 0) 失败:
//   LogError "Not league npc"(1312) + SendErrorMessage(0xDED1) ->
// GetLeagueID != psDelegate.nLeagueID 或 == 0:
//   LogError "LeagueID sync error"(1323) + SendErrorMessage(0xDEAD) ->
// 否则 bGMDelegate = (NationType==4 && GetGMPower()!=0) ->
// (0xF6,7) << psDelegate << dwUCID << bGMDelegate -> SendCmd(0x22,0xA)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueDelegate(XPacket& xPacket)
{
    PS_REQ_LEAGUE_DELEGATE psDelegate{};
    xPacket >> psDelegate;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda48 (0x1404F4780)
    std::function<void()> func = [this, pUser, psDelegate]() {
        if (!CheckNpc(psDelegate.dwNpcID, 7, 0)) {
            LogHelper::LogError("game.contents",
                "ReqLeagueDelegate error - Not league npc[ %d ] ( %d )",
                psDelegate.dwNpcID, 1312);
            CGocNetwork::SendErrorMessage(pUser ? &pUser->CMoverEx : nullptr,
                                         0x22, 0xA, 0xDED1);
            return;
        }

        int nLeagueID = pUser->GetLeagueID();
        std::uint32_t dwUCID = pUser->GetActorID().dwActorID;
        if (nLeagueID == psDelegate.nLeagueID && nLeagueID) {
            char bGMDelegate = 0;
            NATION_TYPE eNationType =
                XGameServer::Instance()->GetOption().GetNationType();
            if (eNationType == 4)
                bGMDelegate = pUser->GetGMPower() != 0;

            XSendPacket xSendPacket(0xF6, 7);
            xSendPacket << psDelegate;
            xSendPacket.XParse << dwUCID;
            xSendPacket.XParse << bGMDelegate;
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x22, 0xA);
        } else {
            LogHelper::LogError("game.contents",
                "ReqLeagueDelegate error - LeagueID sync error[ %d ] ( %d )",
                psDelegate.nLeagueID, 1323);
            CGocNetwork::SendErrorMessage(pUser ? &pUser->CMoverEx : nullptr,
                                         0x22, 0xA, 0xDEAD);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueSkillLearn - IDA @ 0x1404F4A40
// 已精确还原 - 解析 PS_REQ_LEAGUE_SKILL；pUser 空返回 0；
// GetArea 空返回 0；
// lambda50 (0x1404F4C40) DoJob: IsLive 门 ->
// LeagueID 不匹配或为 0: LogError 1360 + SendErrorMessage(0xDEAD) ->
// dwUCID 不匹配: LogError 1367 + SendErrorMessage(0xDEAD) ->
// GetTB_LEAGUE_SKILL(bySkillIndex) 空: LogError 1375 + 0xDED6 ->
// nGroupID <= 0: LogError 1384 + 0xDED6 ->
// 非盟主: LogError 1397 + 0xDEB7 ->
// bySkillInfo[group-1] >= Level_Max: LogError 1407 + 0xDEDA ->
// Get_Skill_Level > byLeagueLevel: LogError 1417 + 0xDED7 ->
// 否则 (0xF6,0x53) << psSkill -> SendCmd(0x22,0x53)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueSkillLearn(XPacket& xPacket)
{
    PS_REQ_LEAGUE_SKILL psSkillInfo{};
    xPacket >> psSkillInfo;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda50 (0x1404F4C40)
    std::function<void()> func = [pUser, psSkillInfo]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetLeagueID() == psSkillInfo.nLeagueID
            && pUser->GetLeagueID()) {
            if (pUser->GetActorID().dwActorID == psSkillInfo.dwUCID) {
                std::uint32_t dwIndex = psSkillInfo.bySkillIndex;
                XResourceMgr& resourceMgr =
                    XGameServer::Instance()->GetResourceMgr();
                TB_LEAGUE_SKILL* pTB_LeagueSkillInfo =
                    resourceMgr.GetTB_LEAGUE_SKILL(dwIndex);
                if (pTB_LeagueSkillInfo) {
                    int nGroupID = pTB_LeagueSkillInfo->League_Skill_Group;
                    if (nGroupID > 0) {
                        ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};
                        pUser->GetLeagueInfo(stLeagueInfoForGame);
                        if (stLeagueInfoForGame.dwMasterUCID
                            == pUser->GetActorID().dwActorID) {
                            if (stLeagueInfoForGame.bySkillInfo[nGroupID - 1]
                                < pTB_LeagueSkillInfo->League_Skill_Level_Max) {
                                if (pTB_LeagueSkillInfo->League_Get_Skill_Level
                                    <= stLeagueInfoForGame.byLeagueLevel) {
                                    XSendPacket xSendPacket(0xF6, 0x53);
                                    xSendPacket << psSkillInfo;
                                    XGameServer::Instance()->GetCommunitySocket().SendCmd(
                                        &xSendPacket, pUser, 0x22, 0x53);
                                } else {
                                    CGocNetwork::SendErrorMessage(
                                        pUser ? &pUser->CMoverEx : nullptr,
                                        0x22, 0x53, 0xDED7);
                                    LogHelper::LogError("game.contents",
                                        "ReqLeagueSkillLearn error - No Table TB_LEAGUE_SKILL[Index:%d]( %d )",
                                        psSkillInfo.bySkillIndex, 1417);
                                }
                            } else {
                                CGocNetwork::SendErrorMessage(
                                    pUser ? &pUser->CMoverEx : nullptr,
                                    0x22, 0x53, 0xDEDA);
                                LogHelper::LogError("game.contents",
                                    "ReqLeagueSkillLearn error - No Table TB_LEAGUE_SKILL[Index:%d]( %d )",
                                    psSkillInfo.bySkillIndex, 1407);
                            }
                        } else {
                            CGocNetwork::SendErrorMessage(
                                pUser ? &pUser->CMoverEx : nullptr,
                                0x22, 0x53, 0xDEB7);
                            LogHelper::LogError("game.contents",
                                "ReqLeagueSkillLearn error - No Table TB_LEAGUE_SKILL[Index:%d]( %d )",
                                psSkillInfo.bySkillIndex, 1397);
                        }
                    } else {
                        CGocNetwork::SendErrorMessage(
                            pUser ? &pUser->CMoverEx : nullptr,
                            0x22, 0x53, 0xDED6);
                        LogHelper::LogError("game.contents",
                            "ReqLeagueSkillLearn error - No Table TB_LEAGUE_SKILL[Index:%d]( %d )",
                            psSkillInfo.bySkillIndex, 1384);
                    }
                } else {
                    CGocNetwork::SendErrorMessage(
                        pUser ? &pUser->CMoverEx : nullptr,
                        0x22, 0x53, 0xDED6);
                    LogHelper::LogError("game.contents",
                        "ReqLeagueSkillLearn error - No Table TB_LEAGUE_SKILL[Index:%d]( %d )",
                        psSkillInfo.bySkillIndex, 1375);
                }
            } else {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x53, 0xDEAD);
                LogHelper::LogError("game.contents",
                    "ReqLeagueSkillLearn error - Invalid UCID[ LeagueID:%d, UCID:%d ]( %d )",
                    pUser->GetLeagueID(), pUser->GetActorID().dwActorID, 1367);
            }
        } else {
            CGocNetwork::SendErrorMessage(
                pUser ? &pUser->CMoverEx : nullptr,
                0x22, 0x53, 0xDEAD);
            LogHelper::LogError("game.contents",
                "ReqLeagueSkillLearn error - Invalid LeagueID[ LeagueID:%d ]( %d )",
                pUser->GetLeagueID(), 1360);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueInventoryMove - IDA @ 0x1404F5250
// 已精确还原 - 解析 PS_REQ_ITEM_MOVE_LEAGUE_INVEN；
// lambda52 (0x1404F54D0) DoJob: IsLive 门 ->
// CheckNpc(dwNpcID,7,0) 失败: LogError 1445 + SendErrorMessage(0x22,0x54,0xDED1) ->
// IsMatching 时: GOC 空则 return；GetMazeNeedItemID==nSrcItemID:
//   LogError 1461 + CUser::SendErrorMessage(0x22,0x54,52067) ->
// LeagueID 无效或不匹配: SendErrorMessage(0xDEAD) + LogError 1470 ->
// GetLeagueInfo -> GetTB_LEAGUE_INFO(byLeagueLevel) 空: 0xDEDB + LogError 1481 ->
// IsLeagueSkill(1) 失败: 0xDED6 + LogError 1489 ->
// byPosition!=100 时: >7 -> 0xDEAE + LogError 1500；
//   IsLeagueAuth(byPosition,eStoreInAuth) 失败: (0x22,0x56,0xDEAE) + LogError 1507 ->
// byType==0 (存入): byDestInvenType!=15 -> 0xCB21 + LogError 1521；
//   shDestSlotPos<0 或 >=League_Inventory -> 0xCB2C + LogError 1530；
//   CheckLeagueInventoryIn>0 -> SendErrorMessage(nResult) + LogError 1539；
//   通过 -> psStorageInfo={15,shDestSlotPos,stItemInfo} ->
// byType==1 (取出): byDestInvenType in {2,13,4}: CheckLeagueInventoryOut>0 ->
//   SendErrorMessage(nResult) + LogError 1562；否则 0xDEAD + LogError 1553；
//   通过 -> psStorageInfo={byDestInvenType,shDestSlotPos} ->
// 其他 (仓内移动): src/dest 均 15: shDestSlotPos 校验(0xCB2C/LogError 1584) ->
//   CheckLeagueInventoryInMove>0 -> SendErrorMessage(nResult) + LogError 1593；
//   否则 0xDEAD + LogError 1576 ->
// 公共尾: psReqItemMoveInfo=捕获副本 -> (0xF6,0x61) << dwActorID << st ->
// SendCmd(0x22,0x54)。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueInventoryMove(XPacket& xPacket)
{
    PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReqItemMove{};
    xPacket >> psReqItemMove;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda52 (0x1404F54D0): 捕获 pUser/psReqItemMove 副本/this
    std::function<void()> func = [this, pUser, psReqItemMove]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (!CheckNpc(psReqItemMove.dwNpcID, 7, 0)) {
            LogHelper::LogError("game.contents",
                "ReqLeagueInventoryMove error - Not league npc[ %d ] ( %d )",
                psReqItemMove.dwNpcID, 1445);
            CGocNetwork::SendErrorMessage(
                pUser ? &pUser->CMoverEx : nullptr,
                0x22, 0x54, 0xDED1);
            return;
        }

        if (pUser->IsMatching()) {
            CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
            if (!pInven)
                return;
            int nMazeNeedID = pInven->GetMazeNeedItemID();
            if (nMazeNeedID == psReqItemMove.nSrcItemID) {
                LogHelper::LogError("game.item",
                    "ReqLeagueInventoryMove error - Not possible when matching[UCID:%d] (%d)",
                    pUser->GetActorID().dwActorID, 1461);
                pUser->SendErrorMessage(0x22, 0x54, 52067);
                return;
            }
        }

        if (!pUser->GetLeagueID() || pUser->GetLeagueID() != psReqItemMove.nLeagueID) {
            CGocNetwork::SendErrorMessage(
                pUser ? &pUser->CMoverEx : nullptr,
                0x22, 0x54, 0xDEAD);
            LogHelper::LogError("game.contents",
                "ReqLeagueInventoryMove error - Invalid LeagueID[ LeagueID:%d ]( %d )",
                psReqItemMove.nLeagueID, 1470);
            return;
        }

        ST_LEAGUE_INFO_FOR_GAME stLeagueInfo{};
        pUser->GetLeagueInfo(stLeagueInfo);
        XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
        TB_LEAGUE_INFO* pTB_LeagueInfo =
            resourceMgr.GetTB_LEAGUE_INFO(stLeagueInfo.byLeagueLevel);
        if (!pTB_LeagueInfo) {
            CGocNetwork::SendErrorMessage(
                pUser ? &pUser->CMoverEx : nullptr,
                0x22, 0x54, 0xDEDB);
            LogHelper::LogError("game.contents",
                "ReqLeagueInventoryMove error - No Table TB_LEAGUE_INFO[ Level:%d ]( %d )",
                stLeagueInfo.byLeagueLevel, 1481);
            return;
        }

        if (!pUser->IsLeagueSkill(1)) {
            CGocNetwork::SendErrorMessage(
                pUser ? &pUser->CMoverEx : nullptr,
                0x22, 0x54, 0xDED6);
            LogHelper::LogError("game.contents",
                "ReqLeagueInventoryMove error - No learned skill WAREHOUSE( %d )", 1489);
            return;
        }

        std::uint8_t byPosition = stLeagueInfo.byPosition;
        if (stLeagueInfo.byPosition != 100) {
            if (byPosition > 7) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, 0xDEAE);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryMove error - Fault positoin[ Position:%d ]( %d )",
                    byPosition, 1500);
                return;
            }
            if (!pUser->IsLeagueAuth(byPosition, eStoreInAuth)) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDEAE);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - No Authority[ UCID:%d, Position:%d ]( %d )",
                    pUser->GetActorID().dwActorID, byPosition, 1507);
                return;
            }
        }

        STItem stItemInfo{};
        PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME psResMoveItemInfo{};

        if (psReqItemMove.byType == 0) {
            // 存入: 源为个人背包，目标必须是仓库(15)
            if (psReqItemMove.byDestInvenType != 15) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, 0xCB21);
                LogHelper::LogError("game.item",
                    "ReqLeagueInventoryMove error - Fault slot position[ InvenType:%d]( %d )",
                    psReqItemMove.byDestInvenType, 1521);
                return;
            }
            if (psReqItemMove.shSrcSlotPos < 0
                || psReqItemMove.shDestSlotPos < 0
                || psReqItemMove.shDestSlotPos >= pTB_LeagueInfo->League_Inventory) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, 0xCB2C);
                LogHelper::LogError("game.item",
                    "ReqLeagueInventoryMove error - Fault slot position[ SRC:%d, DEST:%d ]( %d )",
                    psReqItemMove.shSrcSlotPos, psReqItemMove.shDestSlotPos, 1530);
                return;
            }
            int nResult = CheckLeagueInventoryIn(psReqItemMove, stItemInfo,
                                                 psResMoveItemInfo);
            if (nResult > 0) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, static_cast<std::uint16_t>(nResult));
                LogHelper::LogError("game.item",
                    "CheckLeagueInventoryIn error - Failed move item To League Inventory( %d )",
                    1539);
                return;
            }
            psResMoveItemInfo.psStorageInfo.byInvenType = 15;
            psResMoveItemInfo.psStorageInfo.shSlotPos = psReqItemMove.shDestSlotPos;
            psResMoveItemInfo.psStorageInfo.stItem = stItemInfo;
        } else if (psReqItemMove.byType == 1) {
            // 取出: 目标必须是个人背包类型 (2/13/4)
            if (psReqItemMove.byDestInvenType == 2
                || psReqItemMove.byDestInvenType == 13
                || psReqItemMove.byDestInvenType == 4) {
                int nResult = CheckLeagueInventoryOut(psReqItemMove);
                if (nResult > 0) {
                    CGocNetwork::SendErrorMessage(
                        pUser ? &pUser->CMoverEx : nullptr,
                        0x22, 0x54, static_cast<std::uint16_t>(nResult));
                    LogHelper::LogError("game.item",
                        "ReqLeagueInventoryMove error - Failed move item To League Inventory( %d )",
                        1562);
                    return;
                }
                psResMoveItemInfo.psStorageInfo.byInvenType = psReqItemMove.byDestInvenType;
                psResMoveItemInfo.psStorageInfo.shSlotPos = psReqItemMove.shDestSlotPos;
            } else {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, 0xDEAD);
                LogHelper::LogError("game.item",
                    "ReqLeagueInventoryMove error - Fault slot position[ InvenType:%d]( %d )",
                    psReqItemMove.byDestInvenType, 1553);
                return;
            }
        } else {
            // 仓内移动: 源/目标都必须是仓库(15)
            if (psReqItemMove.byDestInvenType == 15
                && psReqItemMove.bySrcInvenType == 15) {
                if (psReqItemMove.shSrcSlotPos < 0
                    || psReqItemMove.shDestSlotPos < 0
                    || psReqItemMove.shDestSlotPos < pTB_LeagueInfo->League_Inventory) {
                    int nResult = CheckLeagueInventoryInMove(psReqItemMove);
                    if (nResult > 0) {
                        CGocNetwork::SendErrorMessage(
                            pUser ? &pUser->CMoverEx : nullptr,
                            0x22, 0x54, static_cast<std::uint16_t>(nResult));
                        LogHelper::LogError("game.item",
                            "ReqLeagueInventoryMove error - Failed move item To League Inventory( %d )",
                            1593);
                        return;
                    }
                    psResMoveItemInfo.psStorageInfo.byInvenType = psReqItemMove.byDestInvenType;
                    psResMoveItemInfo.psStorageInfo.shSlotPos = psReqItemMove.shDestSlotPos;
                } else {
                    CGocNetwork::SendErrorMessage(
                        pUser ? &pUser->CMoverEx : nullptr,
                        0x22, 0x54, 0xCB2C);
                    LogHelper::LogError("game.item",
                        "ReqLeagueInventoryMove error - Fault slot position[ SRC:%d, DEST:%d ]( %d )",
                        psReqItemMove.shSrcSlotPos, psReqItemMove.shDestSlotPos, 1584);
                    return;
                }
            } else {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, 0xDEAD);
                LogHelper::LogError("game.item",
                    "ReqLeagueInventoryMove error - Fault slot position[ InvenType:%d]( %d )",
                    psReqItemMove.byDestInvenType, 1576);
                return;
            }
        }

        // 公共尾: 回填请求副本并发往 Community
        psResMoveItemInfo.psReqItemMoveInfo = psReqItemMove;
        XSendPacket xSendPacket(0xF6, 0x61);
        xSendPacket.XParse << pUser->GetActorID().dwActorID;
        xSendPacket << psResMoveItemInfo;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x22, 0x54);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// ReqLeagueInventoryInfo - IDA @ 0x1404F62C0
// 已精确还原 - 解析 PS_REQ_LEAGUE_INVEN_INFO；
// lambda54 (0x1404F64D0) DoJob: IsLive 门 ->
// bSync 初始 true; CheckSendLeagueInventoryInfo 为 false(已发过)且
//   CompareLeagueInventoryCount(nLeagueID,dwUCID) 为 true 时 bSync=0 ->
// bSync=0 且 GetLeagueInventoryTime()-5 > GetCurDate(): 冷却未过直接 return；
// 其余进入校验:
// CheckNpc(dwNpcID,7,0) 失败: LogError 1647 + (0x22,0x56,0xDED1) ->
// LeagueID 无效: (0x22,0x56,0xDEAD) + LogError 1656 ->
// GetTB_LEAGUE_INFO 空: 0xDEDB + LogError 1667 ->
// shStartPos<0 或 shEndPos>League_Inventory: 0xDEDB + LogError 1675
//   (LogError 打印 StartPos/EndPos 均取 shEndPos，与原始一致) ->
// IsLeagueSkill(1) 失败: 0xDED6 + LogError 1683 ->
// byPosition==100 直接过；>7: 0xDEAE + LogError 1694；
//   IsLeagueAuth(byPosition,eStoreInAuth) 通过 ->
// SetLeagueInventorySend(true) -> (0xF6,0x60) << psInvenInfo <<
//   dwActorID -> SendCmd(0x22,0x56)；无权限: (0x22,0x54,0xDEAE)+LogError 1701。
// + lambda192 递减。
// ============================================================================
bool CLeagueProcess::ReqLeagueInventoryInfo(XPacket& xPacket)
{
    PS_REQ_LEAGUE_INVEN_INFO psInvenInfo{};
    xPacket >> psInvenInfo;

    CUser* pUser = GetClientPtr();

    // Per IDA lambda54 (0x1404F64D0): 捕获 pUser/psInvenInfo 副本/this
    std::function<void()> func = [this, pUser, psInvenInfo]() {
        if (!pUser || !pUser->IsLive())
            return;

        bool bSync = true;
        if (!pUser->CheckSendLeagueInventoryInfo()) {
            // 已发送过: 同步计数一致时置 bSync=0（进入冷却时间判定）
            int nLeagueID = pUser->GetLeagueID();
            std::uint32_t dwUCID = pUser->GetActorID().dwActorID;
            if (ThreadLocalData::GetInstance()
                    ->CompareLeagueInventoryCount(nLeagueID, dwUCID)) {
                bSync = false;
            }
        }

        if (!bSync
            && pUser->GetLeagueInventoryTime() - 5
                > XGameServer::Instance()->GetCurDate()) {
            // 冷却未过: 跳过本次仓库信息请求
            return;
        }
        {
            if (!CheckNpc(psInvenInfo.dwNpcID, 7, 0)) {
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - Not league npc[ %d ] ( %d )",
                    psInvenInfo.dwNpcID, 1647);
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDED1);
                return;
            }

            if (!pUser->GetLeagueID()
                || pUser->GetLeagueID() != psInvenInfo.nLeagueID) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDEAD);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - Invalid LeagueID[ LeagueID:%d ]( %d )",
                    psInvenInfo.nLeagueID, 1656);
                return;
            }

            ST_LEAGUE_INFO_FOR_GAME stLeagueInfo{};
            pUser->GetLeagueInfo(stLeagueInfo);
            XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
            TB_LEAGUE_INFO* pTB_LeagueInfo =
                resourceMgr.GetTB_LEAGUE_INFO(stLeagueInfo.byLeagueLevel);
            if (!pTB_LeagueInfo) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDEDB);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - No Table TB_LEAGUE_INFO[ Level:%d ]( %d )",
                    stLeagueInfo.byLeagueLevel, 1667);
                return;
            }

            if (psInvenInfo.shStartPos < 0
                || psInvenInfo.shEndPos > pTB_LeagueInfo->League_Inventory) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDEDB);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - Fault inventory slot[ StartPos:%d, EndPos:%d ]( %d )",
                    psInvenInfo.shEndPos, psInvenInfo.shEndPos, 1675);
                return;
            }

            if (!pUser->IsLeagueSkill(1)) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDED6);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - No Table TB_LEAGUE_INFO[ StartPos:%d, EndPos:%d ]( %d )",
                    psInvenInfo.shEndPos, psInvenInfo.shEndPos, 1683);
                return;
            }

            std::uint8_t byPosition = stLeagueInfo.byPosition;
            if (stLeagueInfo.byPosition == 100)
                goto SendInfo;

            if (byPosition > 7) {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x56, 0xDEAE);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryInfo error - Fault positoin[ Position:%d ]( %d )",
                    byPosition, 1694);
                return;
            }

            if (pUser->IsLeagueAuth(byPosition, eStoreInAuth)) {
            SendInfo:
                pUser->SetLeagueInventorySend(true);
                XSendPacket xSendPacket(0xF6, 0x60);
                xSendPacket << psInvenInfo;
                xSendPacket.XParse << pUser->GetActorID().dwActorID;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x22, 0x56);
            } else {
                CGocNetwork::SendErrorMessage(
                    pUser ? &pUser->CMoverEx : nullptr,
                    0x22, 0x54, 0xDEAE);
                LogHelper::LogError("game.contents",
                    "ReqLeagueInventoryMove error - No Authority[ UCID:%d, Position:%d ]( %d )",
                    pUser->GetActorID().dwActorID, byPosition, 1701);
            }
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    XClient::IncrementJobCount(pUser);
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// CheckLeagueInventoryIn - IDA @ 0x1404F6C40
// 已精确还原 - pUser 空 -> 51001；GOC 空 -> 52001；
// nSrcItemID==-1 -> 52011；GetTB_ITEM 空 -> 52004；
// GetSlotItem(bySrcInvenType,shSrcSlotPos) 空或锁 -> 52011；
// GetSlot()!=shSrcSlotPos -> 52012；IsBind -> 52024；
// Item_Limit_Sell_Type in {4,5,6,7} -> 52044；
// GetItem().eFlag==2 -> 52011；GetInvenType() not in {2,4,13} -> 52011；
// *stItemInfo = GetItem()；GetBroachList(psResItemMoveInfo.psItemBroachList)；
// GetSocketList(psResItemMoveInfo.psItemSocketList)；
// IsResealPackage(stItemInfo->nItemID): GetPackageInfo 后
//   vecInfo.size() 传 IsResealPackageCount 失败 -> 52011；
//   成功 push_back 到 psItemPackageList.vecInfo；
// 最后 SetLock(bySrcInvenType,shSrcSlotPos,2)；返回 0。
// ============================================================================
int CLeagueProcess::CheckLeagueInventoryIn(
    PS_REQ_ITEM_MOVE_LEAGUE_INVEN psItemMove,
    STItem& stItemInfo,
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& psResMoveItemInfo)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 51001;

    CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    if (!pInven)
        return 52001;

    if (psItemMove.nSrcItemID == -1) {
        return 52011;
    }

    XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
    TB_ITEM* pTB_SrcItemInfo = resourceMgr.GetTB_ITEM(psItemMove.nSrcItemID);
    if (!pTB_SrcItemInfo) {
        return 52004;
    }

    bool bSrcLock = false;
    std::shared_ptr<CItem> pSrcItem =
        pInven->GetSlotItem(psItemMove.bySrcInvenType, psItemMove.shSrcSlotPos,
                            bSrcLock);
    if (!pSrcItem || bSrcLock) {
        return 52011;
    }

    if (pSrcItem->GetSlot() != psItemMove.shSrcSlotPos) {
        return 52012;
    }

    if (pSrcItem->IsBind()) {
        return 52024;
    }

    TB_ITEM* pSrcItemTable = pSrcItem->GetItemTable();
    if (pSrcItemTable->Item_Limit_Sell_Type == 4
        || pSrcItemTable->Item_Limit_Sell_Type == 5
        || pSrcItemTable->Item_Limit_Sell_Type == 6
        || pSrcItemTable->Item_Limit_Sell_Type == 7) {
        return 52044;
    }

    STItem stSrcItem{};
    if (pSrcItem->GetItem(&stSrcItem)->eFlag == 2) {
        return 52011;
    }

    std::uint8_t bySrcInvenType = pSrcItem->GetInvenType();
    if (bySrcInvenType != 2 && bySrcInvenType != 4 && bySrcInvenType != 13) {
        return 52011;
    }

    pSrcItem->GetItem(&stSrcItem);
    stItemInfo = stSrcItem;
    pSrcItem->GetBroachList(psResMoveItemInfo.psResItemMoveInfo.psItemBroachList);
    pSrcItem->GetSocketList(psResMoveItemInfo.psResItemMoveInfo.psItemSocketList);

    if (pInven->IsResealPackage(stItemInfo.nItemID)) {
        PS_ITEM_PACKAGE psPackageInfo{};
        pSrcItem->GetPackageInfo(psPackageInfo);
        int nCount = static_cast<int>(psPackageInfo.vecInfo.size());
        if (!pInven->IsResealPackageCount(stItemInfo.nItemID, nCount)) {
            return 52011;
        }
        psResMoveItemInfo.psResItemMoveInfo.psItemPackageList.vecInfo
            .push_back(psPackageInfo);
    }

    pInven->SetLock(psItemMove.bySrcInvenType, psItemMove.shSrcSlotPos, 2);
    return 0;
}

// ============================================================================
// CheckLeagueInventoryOut - IDA @ 0x1404F72A0
// 已精确还原 - pUser 空 -> 51001；GOC 空 -> 52001；
// nSrcItemID==-1 -> 52011；GetTB_ITEM 空 -> 52004；
// GetTB_ITEM_CLASSIFY 空 -> 52004；GetTBInvenPtr(Item_Inven_Type) 空 -> 52001；
// 目标槽 GetSlotItem(byDestInvenType,shDestSlotPos) 非空或锁 -> 52002；
// GetEmptySlotCount==0 -> 52010；
// shSrcSlotPos/shDestSlotPos<0 或 shDestSlotPos>=GetOpenSlotNum -> 52012；
// 通过 -> SetLock(byDestInvenType,shDestSlotPos,2)；返回 0。
// ============================================================================
int CLeagueProcess::CheckLeagueInventoryOut(PS_REQ_ITEM_MOVE_LEAGUE_INVEN psItemMove)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 51001;

    CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    if (!pInven)
        return 52001;

    if (psItemMove.nSrcItemID == -1) {
        return 52011;
    }

    XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
    TB_ITEM* pTB_SrcItemInfo = resourceMgr.GetTB_ITEM(psItemMove.nSrcItemID);
    if (!pTB_SrcItemInfo)
        return 52004;

    TB_ITEM_CLASSIFY* pTB_ItemClassify =
        resourceMgr.GetTB_ITEM_CLASSIFY(pTB_SrcItemInfo->Item_Classify_Index);
    if (!pTB_ItemClassify)
        return 52004;

    XBaseInventory* pBaseInventory =
        pInven->GetTBInvenPtr(pTB_ItemClassify->Item_Inven_Type);
    if (!pBaseInventory)
        return 52001;

    bool bDestLock = false;
    std::shared_ptr<CItem> pItemPtr =
        pInven->GetSlotItem(psItemMove.byDestInvenType,
                            psItemMove.shDestSlotPos, bDestLock);
    if (pItemPtr || bDestLock) {
        return 52002;
    }

    if (!pBaseInventory->GetEmptySlotCount()) {
        return 52010;
    }

    int nOpenSlot = pBaseInventory->GetOpenSlotNum();
    if (psItemMove.shSrcSlotPos < 0
        || psItemMove.shDestSlotPos < 0
        || psItemMove.shDestSlotPos >= nOpenSlot) {
        return 52012;
    }

    pInven->SetLock(psItemMove.byDestInvenType, psItemMove.shDestSlotPos, 2);
    return 0;
}

// ============================================================================
// CheckLeagueInventoryInMove - IDA @ 0x1404F75E0
// 已精确还原 - pUser 空 -> 51001；GOC 空 -> 52001；
// nSrcItemID==-1 -> 52011；GetTB_ITEM 空 -> 52004；有表 -> 0。
// ============================================================================
int CLeagueProcess::CheckLeagueInventoryInMove(PS_REQ_ITEM_MOVE_LEAGUE_INVEN psItemMove)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 51001;

    CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    if (!pInven)
        return 52001;

    if (psItemMove.nSrcItemID == -1) {
        return 52011;
    }

    XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
    if (resourceMgr.GetTB_ITEM(psItemMove.nSrcItemID)) {
        return 0;
    }
    return 52004;
}

// ============================================================================
// DBLeagueParse - IDA @ 0x1404F0330
// 已精确还原 - SubCmd==18 转发 ResLeagueOverlapName；其余返回 true。
// ============================================================================
bool CLeagueProcess::DBLeagueParse(XPacket& xPacket)
{
    if (xPacket.GetSubCmd() == 18)
        return ResLeagueOverlapName(xPacket);
    return true;
}

// ============================================================================
// CGocRecode Extension for Guild Wealth
// ============================================================================

// Note: SendLeagueWealth is part of CGocRecode class
// Address: 0x1401588F0
// This function sends guild wealth contribution to community server
// See implementation in GocRecode.cpp

namespace CGocRecodeExtension
{
    void SendLeagueWealth(CMover* pMover, short shExp, int nGold)
    {
        // Hidden C++ exception states: #wind=1
        
        // Cast to CUser
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (!pUser)
            return;
        
        // Check if user is in a guild
        int nLeagueID = CUser::GetLeagueID(pUser);
        if (nLeagueID <= 0)
            return;
        
        // Calculate guild contribution (1% of gold)
        int nGuildGold = nGold >= 0 ? (int)((double)nGold * 0.01) : 0;
        
        // Prepare guild wealth packet
        PS_LEAGUE_WEALTH_FOR_SERVER psLeagueWealthInfo;
        psLeagueWealthInfo.dwUCID = pUser->GetActorID().dwActorID;
        psLeagueWealthInfo.nLeagueID = nLeagueID;
        psLeagueWealthInfo.shExp = shExp;
        psLeagueWealthInfo.nGold = nGuildGold;
        
        // Send to community server (0xF6, 0x55)
        XSendPacket xSendPacket(0xF6, 0x55);
        xSendPacket << psLeagueWealthInfo;
        
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer)
        {
            pServer->m_communitySocket.SendCheck(&xSendPacket);
        }
    }
}
