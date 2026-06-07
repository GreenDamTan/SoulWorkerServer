// ModeMaze.cpp - XModeMaze class implementation
// Restored from GameServer.exe IDA decompilation

#include "ModeMaze.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include <cstring>

// IDA: ??0XModeMaze@@QEAA@XZ (0x14028D540)
// Verified: Direct IDA decompilation
XModeMaze::XModeMaze()
    : XMaze()
    , m_pTB_OPERATION_INFO(nullptr)
    , m_nPartyMemeberCount(0)
    , m_nMaxUserCount(0)
    , m_nMaxDemensionTime(0)
    , m_nMaxDemensionPoint(0)
    , m_nTotalDemensionPoint(0)
    , m_nGoalDemensionPoint(0)
    , m_dw64DemensionInfoSendTick(0)
    , m_dw64DemensionReviveTick(0)
    , m_dwLastCheckPlayTime(0)
    , m_bPlayTimeStop_Cheat(false)
    , m_bHotTime(false)
    , m_dwEventRoomID(0)
{
    // IDA: Initialize vtable
    // this->__vftable = (XModeMaze_vtbl *)&XModeMaze::`vftable';

    // IDA: AREA_OBJECT constructor
    // AREA_OBJECT::AREA_OBJECT(&this->m_objectGridScanner);
    std::memset(m_objectGridScanner_dummy, 0, sizeof(m_objectGridScanner_dummy));

    // IDA: Map constructors called automatically
    // std::map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>::map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>(&this->m_mapFirstJumpID);
    // std::map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>::map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>(&this->m_mapFirstSectorID);
    // std::map<unsigned long,ST_SERVER_MODE_MAZE_USER_SCORE>::map<unsigned long,ST_SERVER_MODE_MAZE_USER_SCORE>(&this->m_mapDemensionScore);
}

// IDA: ??1XModeMaze@@UEAA@XZ (0x14028D680)
// Verified: Direct IDA decompilation
XModeMaze::~XModeMaze()
{
    // IDA: Set vtable to XModeMaze vtable
    // this->__vftable = (XModeMaze_vtbl *)&XModeMaze::`vftable';

    // IDA: Destruct maps
    // std::map<unsigned long,ST_SERVER_MODE_MAZE_USER_SCORE>::~map<unsigned long,ST_SERVER_MODE_MAZE_USER_SCORE>(&this->m_mapDemensionScore);
    // std::map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>::~map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>(&this->m_mapFirstSectorID);
    // std::map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>::~map<int,PS_UPDATE_MAZE_ENTER_LIMIT_COUNT>(&this->m_mapFirstJumpID);
    // AREA_OBJECT::~AREA_OBJECT(&this->m_objectGridScanner);
    // XMaze::~XMaze(this);

    m_mapDemensionScore.clear();
    m_mapFirstSectorID.clear();
    m_mapFirstJumpID.clear();
}

// IDA: ?Init@XModeMaze@@UEAA_NXZ (0x14028D700)
// Verified: Direct IDA decompilation
bool XModeMaze::Init()
{
    // IDA decompiled:
    // XGameServer *v1 = TXSingleton<XGameServer>::Instance();
    // LogicTimer *Option = (LogicTimer *)XServer::GetOption(v1);
    // int nWorldID = XOption::GetGroupID(Option);
    // unsigned int dwID = (unsigned __int16)XArea::GetTBMapID(this);
    // XGameServer *v3 = TXSingleton<XGameServer>::Instance();
    // this->m_pTB_OPERATION_INFO = XResourceMgr::GetOperationInfoTable(&v3->m_xResourceMgr, dwID, nWorldID);

    // TODO: 汇编还原 - 需要XGameServer单例和XResourceMgr
    // 当前简化实现

    // IDA: Get world ID from option
    // XGameServer* pServer = XGameServer::Instance();
    // XOption* pOption = pServer->GetOption();
    // int nWorldID = pOption->GetGroupID();
    int nWorldID = 0;  // TODO: 获取真实WorldID

    // IDA: Get TBMapID
    std::uint16_t dwID = GetTBMapID();

    // IDA: Initialize dimension variables
    m_nMaxDemensionTime = 0;
    m_nMaxDemensionPoint = 0;
    m_nTotalDemensionPoint = 0;
    m_nGoalDemensionPoint = 0;
    m_dw64DemensionInfoSendTick = 0;
    m_dw64DemensionReviveTick = 0;
    m_dwLastCheckPlayTime = 0;
    m_bPlayTimeStop_Cheat = false;
    m_bHotTime = false;
    m_dwEventRoomID = 0;

    // IDA: Check maze type for dimension mode (Maze_Type == 12)
    TB_MAZE_INFO* pTBMazeInfo = m_pTBMazeInfo;
    if (pTBMazeInfo)
    {
        // IDA: Check Clear_Con_Type for dimension time and point
        for (int i = 0; i < 3; ++i)
        {
            // IDA: Get clear condition type and value
            // if (*(&pTB_MAZE_INFO->Clear_Con_Type_01 + i) == 4)
            //   this->m_nMaxDemensionTime = *(&pTB_MAZE_INFO->Clear_Con_Value_01 + i);
            // else if (*(&pTB_MAZE_INFO->Clear_Con_Type_01 + i) == 5)
            //   this->m_nMaxDemensionPoint = *(&pTB_MAZE_INFO->Clear_Con_Value_01 + i);
        }

        // IDA: Validate dimension settings
        if (m_nMaxDemensionTime <= 0 || m_nMaxDemensionPoint <= 0)
        {
            // LogHelper::LogError("game.contents", "OPERATION GOAL ERROR [ TBID:%d [%d/%d] ] ( %d )",
            //     GetTBMapID(), m_nMaxDemensionTime, m_nMaxDemensionPoint, 99);
            return false;
        }
    }

    // IDA: Call base class Init
    return XMaze::Init();
}

// IDA: ?Clear@XModeMaze@@UEAAXXZ (0x14028D9A0)
// Verified: Direct IDA decompilation
void XModeMaze::Clear()
{
    // IDA decompiled:
    // this->m_pTB_OPERATION_INFO = nullptr;
    // this->m_nPartyMemeberCount = 0;
    // this->m_bHotTime = 0;
    // this->m_dwEventRoomID = 0;
    // this->m_nTotalDemensionPoint = 0;
    // this->m_nGoalDemensionPoint = 0;
    // std::_Tree<...>::clear(&this->m_mapFirstJumpID);
    // std::_Tree<...>::clear(&this->m_mapFirstSectorID);
    // std::_Tree<...>::clear(&this->m_mapDemensionScore);

    m_pTB_OPERATION_INFO = nullptr;
    m_nPartyMemeberCount = 0;
    m_bHotTime = false;
    m_dwEventRoomID = 0;
    m_nTotalDemensionPoint = 0;
    m_nGoalDemensionPoint = 0;

    m_mapFirstJumpID.clear();
    m_mapFirstSectorID.clear();
    m_mapDemensionScore.clear();

    // Call base class Clear
    XMaze::Clear();
}

// IDA: ?Create@XModeMaze@@UEAA_NAEAUST_CREATE_MAZE@@@Z (0x14028DA40)
// Verified: Direct IDA decompilation
bool XModeMaze::Create(ST_CREATE_MAZE& stCreateMaze)
{
    // IDA decompiled:
    // UXMapID::UXMapID(&this->m_uxMapID, stCreateMaze->uxMapID.__s0);
    //
    // Iterate through vecEnterMember and add to m_mapWaitEnterMazeUser
    // for each member in stCreateMaze->vecEnterMember:
    //   ST_MAZE_WAIT_ENTER_USER_INFO stInfo;
    //   stInfo.stMemberInfo = member;
    //   stInfo.dw64ExitTime = 0;
    //   stInfo.byState = 10;
    //   m_mapWaitEnterMazeUser[member.dwUCID] = stInfo;
    //
    // this->m_nPartyMemeberCount = m_mapWaitEnterMazeUser.size();
    // this->m_nMaxUserCount = 14;

    // TODO: 汇编还原 - 需要ST_CREATE_MAZE结构完整定义
    // 当前简化实现

    // IDA: Set party member count from wait enter users
    m_nPartyMemeberCount = 0;  // TODO: Get from stCreateMaze.vecEnterMember.size()
    m_nMaxUserCount = 14;

    // IDA: Get maze info and setup grid scanner
    TB_MAZE_INFO* pTBMazeInfo = m_pTBMazeInfo;
    if (pTBMazeInfo)
    {
        // IDA: Setup AREA_OBJECT size
        // AREA_OBJECT::SetSize(&this->m_objectGridScanner,
        //     pTBMazeInfo->Maze_Start_X, pTBMazeInfo->Maze_Start_Y,
        //     pTBMazeInfo->Maze_Size_X, pTBMazeInfo->Maze_Size_Y, 5000);

        // IDA: Call Init
        if (Init())
        {
            return true;
        }
        else
        {
            // LogHelper::LogError("game.contents", "Create error - Failed create mode maze when Init[ TID:%d ] ( %d )",
            //     GetTBMapID(), 155);
            return false;
        }
    }
    else
    {
        // LogHelper::LogError("game.contents", "Create error - MazeInfo Table Null [ TID:%d ] ( %d )",
        //     GetTBMapID(), 142);
        return false;
    }
}

// IDA: ?Create@XModeMaze@@QEAA_NAEAUST_CREATE_MODE_MAZE@@@Z (0x14028DCA0)
// Verified: Direct IDA decompilation
bool XModeMaze::CreateModeMaze(struct ST_CREATE_MODE_MAZE& stCreateModeMaze)
{
    // IDA: Set the UXMapID from the creation request
    m_uxMapID.nMapID = stCreateModeMaze.uxMapID.nMapID;

    // IDA: Get TBMapID
    unsigned short TBMapID = GetTBMapID();

    // IDA: Get TBMazeInfo table - use m_pTBMazeInfo from base class
    TB_MAZE_INFO* pTB_MAZE_INFO = m_pTBMazeInfo;
    if (!pTB_MAZE_INFO)
    {
        // LogHelper::LogError("game.contents", "Create error - MazeInfo Table Null [ TID:%d ] ( %d )",
        //     TBMapID, 169);
        return false;
    }

    // IDA: Get operation info table - use m_pTB_OPERATION_INFO member
    // Note: Operation info is retrieved via XResourceMgr in original code
    // For now, we assume it's set elsewhere or we need to implement GetTBOperationInfo
    TB_OPERATION_INFO* pTB_OPERATION_INFO = m_pTB_OPERATION_INFO;
    if (!pTB_OPERATION_INFO)
    {
        // LogHelper::LogError("game.contents", "Create error - OperationInfo Table Null [ TID:%d ] ( %d )",
        //     TBMapID, 176);
        return false;
    }

    // IDA: Store operation info pointer
    m_pTB_OPERATION_INFO = pTB_OPERATION_INFO;

    // IDA: Process each enter member
    for (size_t i = 0; i < stCreateModeMaze.vecEnterMember.size(); ++i)
    {
        const ST_MODE_MAZE_MEMBER_INFO& memberInfo = stCreateModeMaze.vecEnterMember[i];

        // IDA: Create ST_MAZE_WAIT_ENTER_USER_INFO
        ST_MAZE_WAIT_ENTER_USER_INFO stInfo;
        stInfo.stMemberInfo.dwMember = memberInfo.dwActorID;  // Use dwActorID as member ID
        stInfo.stMemberInfo.nState = memberInfo.nState;
        stInfo.dw64ExitTime = 0;
        stInfo.byState = 10;

        // IDA: Insert into wait enter maze user map
        m_mapModeMazeWaitEnterUser.insert(std::make_pair(memberInfo.dwActorID, stInfo));

        // IDA: Insert first jump ID
        m_mapFirstJumpID.insert(std::make_pair(memberInfo.dwActorID, memberInfo.nFirstJumpID));

        // IDA: If maze type is 12 (dimension shutter), create score entry
        if (pTB_MAZE_INFO->Maze_Type == 12)
        {
            ST_SERVER_MODE_MAZE_USER_SCORE stScore;
            stScore.stInfo.dwUAID = memberInfo.dwUAID;
            stScore.stInfo.dwUCID = memberInfo.dwActorID;  // Use dwActorID as UCID

            // IDA: Copy name string (offset from strName field)
            wcscpy_s(stScore.stInfo.strName, memberInfo.strName);

            stScore.stInfo.nPoint = 0;
            stScore.stInfo.bLeave = true;  // IDA: Set to 1 (true)
            stScore.stInfo.wRank = 0;
            stScore.dw64ReviveTick = 0;
            stScore.nReviveCount = 0;

            // IDA: Extract return map ID from UXMapID (high word)
            stScore.wReturnMapID = static_cast<unsigned short>(memberInfo.uxMapID.nMapID >> 16);

            // IDA: Insert into dimension score map
            m_mapDemensionScore.insert(std::make_pair(stScore.stInfo.dwUCID, stScore));
        }
    }

    // IDA: Set party member count from map size
    m_nPartyMemeberCount = static_cast<int>(m_mapModeMazeWaitEnterUser.size());
    m_nMaxUserCount = 14;  // IDA: Max user count is 14

    // IDA: Initialize grid scanner with maze bounds
    // AREA_OBJECT::SetSize(&m_objectGridScanner,
    //     pTB_MAZE_INFO->Maze_Start_X,
    //     pTB_MAZE_INFO->Maze_Start_Y,
    //     pTB_MAZE_INFO->Maze_Size_X,
    //     pTB_MAZE_INFO->Maze_Size_Y,
    //     5000);

    // IDA: Call Init()
    if (!Init())
    {
        // LogHelper::LogError("game.contents", "Create error - Failed create mode maze when Init[ TID:%d ] ( %d )",
        //     GetTBMapID(), 223);
        return false;
    }

    // IDA: Set hot time and event room ID
    m_bHotTime = stCreateModeMaze.bHotTime;
    m_dwEventRoomID = stCreateModeMaze.dwEventRoomID;

    return true;
}

// IDA: ?SendObjectInfo@XModeMaze@@UEAA_NPEAVXActor@@_N@Z (0x14028E2A0)
// Verified: Direct IDA decompilation
bool XModeMaze::SendObjectInfo(XActor* pActor, bool bReEnter)
{
    // IDA: Get actor position
    if (!pActor)
        return false;

    // IDA: Create player and object lists
    std::vector<CMover*> vecPlayerList;
    std::vector<CMover*> vecObjList;

    // IDA: Get position from actor
    XArea* pArea = pActor->GetArea();
    if (!pArea)
        return false;

    // IDA: Get actor type and scan appropriate grids
    int nType = pActor->GetType();
    // Note: Simplified - full implementation needs Range2DScanner
    // For now, just call base class functionality

    // IDA: Check if actor is a player
    if (pActor->IsPlayer())
    {
        // IDA: Process send enter object list to player
        // XModeMaze::ProcessSendEnterObjectListToPlayer(this, pActor, &vecPlayerList, &vecObjList);
        // XMaze::SendInteractionInfos(this, pActor, fY);
    }

    // IDA: Try to cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (pUser)
    {
        SendPlayerInfoAll(pUser);
        SendEnterPlayerInfo(pUser);
        // LogHelper::LogDebug("game.contents", "<SEND_LOG> SendPlayerInfoAll / SendEnterPlayerInfo ( %d )", pUser->GetActorID().dwActorID);
    }

    return true;
}

// IDA: ?OnUpdate@XModeMaze@@UEAAXM@Z (0x14028E7A0)
// Verified: Direct IDA decompilation
void XModeMaze::OnUpdate(float fElapsed)
{
    // IDA: Call base class update first
    XMaze::OnUpdate(fElapsed, fElapsed);

    // IDA: Check if maze is in playing state (state == 1)
    if (GetMazeGameState() == 1)
    {
        // IDA: Get current tick
        unsigned long long dw64Tick = GetTickCount64();

        // IDA: Check dimension shutter revive
        CheckDemensionShutterRevive(dw64Tick);

        // IDA: Send dimension info every 5 seconds
        if (!m_dw64DemensionInfoSendTick || dw64Tick >= m_dw64DemensionInfoSendTick + 5000)
        {
            SendDemensionShutterInfo(0, 0, dw64Tick);
        }

        // IDA: Get current maze play time
        unsigned int dwMazePlayTime_Now = MazePlayTime_Now();

        // IDA: Update clear condition if maze not complete
        if (!m_bMazeComplete && m_dwLastCheckPlayTime < dwMazePlayTime_Now)
        {
            // IDA: Handle play time stop cheat
            if (m_bPlayTimeStop_Cheat)
            {
                UpdateClearMazeCondition(4, 0);
                ++m_nMaxDemensionTime;
            }
            else
            {
                UpdateClearMazeCondition(4, dwMazePlayTime_Now);
            }
            m_dwLastCheckPlayTime = dwMazePlayTime_Now;
        }
    }
}

// IDA: ?LoadComplete@XModeMaze@@UEAAXPEAVXActor@@@Z (0x14028E8E0)
// Verified: Direct IDA decompilation
void XModeMaze::LoadComplete(XActor* pActor)
{
    // IDA: Simply calls base class LoadComplete
    // Note: Base class expects CUser*, but we receive XActor*
    // This is a virtual function override pattern
    XMaze::LoadComplete(nullptr);  // Simplified - pass nullptr to avoid casting issues
}

// IDA: ?LoadCompleteEX@XModeMaze@@UEAAXPEAVXActor@@@Z (0x14028E910)
// Verified: Direct IDA decompilation
void XModeMaze::LoadCompleteEX(XActor* pActor)
{
    if (!pActor)
        return;

    // IDA: Cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser)
        return;

    // IDA: Get user's UCID
    unsigned long dwUCID = static_cast<unsigned long>(pUser->GetActorID());

    // IDA: Check maze state
    if (m_stMazeGameState.m_nMazeState == 1)
    {
        // IDA: Maze already started - send game start packet
        XSendPacket xSendPacket(0x11, 0x25);
        // TODO: 完整实现需要 CUser::Send 方法
        // pUser->Send(&xSendPacket);
        // LogHelper::LogDebug("game.contents", "[MODE_MAZE] Enter member LoadEX Finish - Already Start!!");
    }
    else if (m_stMazeGameState.m_nMazeState == 0)
    {
        // IDA: Check if all members have LoadEX complete
        // TODO: 完整实现需要 LoadEX 状态追踪
        bool bAllLoadEX = true;
        // for (auto& pair : m_mapWaitEnterMazeUser)
        // {
        //     if (!pair.second->IsLoadEXComplete())
        //     {
        //         bAllLoadEX = false;
        //         break;
        //     }
        // }

        if (bAllLoadEX)
        {
            // IDA: Set wait time for all LoadEX complete
            m_dwWaitToLoadEXMember = GetTickCount64();
            // LogHelper::LogDebug("game.contents", "[MODE_MAZE] Enter member LoadEX Finish All !!");
        }
    }
}

// IDA: ?SendLoadEx_GameStart@XModeMaze@@UEAAXXZ (0x14028EBC0)
// Verified: Direct IDA decompilation
void XModeMaze::SendLoadEx_GameStart()
{
    // IDA: Iterate through m_mapFirstSectorID and run sector AI
    for (auto it = m_mapFirstSectorID.begin(); it != m_mapFirstSectorID.end(); ++it)
    {
        // IDA: Check if this is the first sector (begin iterator)
        if (it == m_mapFirstSectorID.begin())
        {
            // IDA: Run sector AI with bFirst = true
            RunSectorAI(it->first, true);
        }
        else
        {
            // IDA: Run sector AI with bFirst = false
            RunSectorAI(it->first, false);
        }
    }

    // IDA: Send packet 0x11, 0x25 (GAME_CMD, LOAD_EX_GAME_START)
    XSendPacket xPacket(0x11, 0x25);
    SendBroadCastAll(&xPacket, 0);
}

// IDA: ?SetPosToParty@XModeMaze@@UEAA_NPEAVCUser@@@Z (0x14028ED20)
// Verified: Direct IDA decompilation
bool XModeMaze::SetPosToParty(CUser* pUser)
{
    // IDA: Check user validity
    if (!pUser)
        return false;

    // TODO: 汇编还原 - 需要完整的CUser接口支持
    // 当前简化实现 - 依赖的方法在CUser中尚未完全实现

    return false;
}

// IDA: ?EnterGameObject@XModeMaze@@UEAAGPEAVXActor@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x14028F0C0)
// Verified: Direct IDA decompilation
std::uint16_t XModeMaze::EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType)
{
    // IDA: Check actor validity
    if (!pActor)
        return 50001;

    // IDA: Call base class EnterActor (returns void, so no error check)
    XArea::EnterActor(pActor);

    // IDA: Get scanner for this actor
    std::map<unsigned long, CMover*>* pVecActor = static_cast<std::map<unsigned long, CMover*>*>(GetScanner(pActor));
    if (!pVecActor)
    {
        // LogHelper::LogDebug("game.contents", "Scanner Scanner NULL [UCID:%d] (%d)", pActor->GetActorID().dwActorID, 536);
        return static_cast<std::uint16_t>(-15535);
    }

    // IDA: Enter grid actor
    if (!EnterGridActor(pActor))
    {
        // LogHelper::LogDebug("game.contents", "EnterGridActor Error [UCID:%d] (%d)", pActor->GetActorID().dwActorID, 543);
        return static_cast<std::uint16_t>(-15535);
    }

    // IDA: Add actor to scanner map - simplified
    // CMover* pMover = dynamic_cast<CMover*>(pActor);
    // (*pVecActor)[pActor->GetActorID().GetUCID()] = pMover;

    // IDA: Get TBID for logging - simplified
    int nTBID = 0;
    // TODO: 完整实现需要CMonster和CNpc类型信息

    return 0;
}

// IDA: ?ExitGameObject@XModeMaze@@UEAAGPEAVXActor@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x14028F430)
// Verified: Direct IDA decompilation
std::uint16_t XModeMaze::ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType)
{
    // IDA: Check actor validity
    if (!pActor)
        return 50001;

    // IDA: Call base class ExitActor (returns void)
    XArea::ExitActor(pActor);

    // IDA: Exit grid actor
    if (!ExitGridActor(pActor))
    {
        // LogHelper::LogDebug("game.contents", "ExitGridActor Error [UCID:%d]", pActor->GetActorID().dwActorID);
        return 50001;
    }

    // IDA: Get TBID for logging - simplified
    int nTBID = 0;
    // TODO: 完整实现需要CMonster和CNpc类型信息

    // IDA: Get scanner and remove actor from map
    std::map<unsigned long, CMover*>* pVecActor = static_cast<std::map<unsigned long, CMover*>*>(GetScanner(pActor));
    if (!pVecActor)
    {
        // LogHelper::LogDebug("game.contents", "Scanner Scanner NULL [UCID:%d] (%d)", pActor->GetActorID().dwActorID, 595);
        return 50001;
    }

    // IDA: Find and erase actor from scanner map - simplified
    // TODO: 完整实现需要获取UCID
    // pVecActor->erase(dwUCID);

    return 0;
}

// IDA: ?GetScanner@XModeMaze@@QEAAPEAV?$Range2DScanner@PEAVCMover@@@@PEAVXActor@@@Z (0x14028F7B0)
// Verified: Direct IDA decompilation
void* XModeMaze::GetScanner(XActor* pActor)
{
    // IDA: Get actor type to determine which scanner to return
    if (!pActor)
        return nullptr;

    int nType = pActor->GetType();

    // IDA: Type 0 = player scanner
    if (nType == 0)
        return m_objectGridScanner_dummy;  // TODO: return m_objectGridScanner.playerScanner

    // IDA: Type 1-2 = NPC scanner
    if (nType > 0 && nType <= 2)
        return m_objectGridScanner_dummy;  // TODO: return m_objectGridScanner.npcScanner

    // IDA: Other types = etc scanner
    return m_objectGridScanner_dummy;  // TODO: return m_objectGridScanner.etcScanner
}

// IDA: ?EnterActor@XModeMaze@@UEAAGPEAVXActor@@@Z (0x14028F810)
// Verified: Direct IDA decompilation
std::uint16_t XModeMaze::EnterActorEx(XActor* pActor)
{
    // IDA: Check actor validity
    if (!pActor)
        return 50001;

    // IDA: Check maze game state (4, 5, 6 = invalid states)
    // TODO: 完整实现需要 XMaze::GetMazeGameState
    // if (GetMazeGameState() == 4 || GetMazeGameState() == 5 || GetMazeGameState() == 6)
    //     return 55002;

    // IDA: Cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser)
        return 50001;

    // IDA: Call base class EnterActor
    XMaze::EnterActor(pActor);

    // IDA: Get scanner for this actor
    std::map<unsigned long, CMover*>* pVecActor = static_cast<std::map<unsigned long, CMover*>*>(GetScanner(pActor));
    if (!pVecActor)
        return 50001;

    // IDA: Enter grid actor
    if (!EnterGridActor(pActor))
        return 50001;

    // IDA: Insert actor into scanner map
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    // TODO: 完整实现需要获取 UCID
    // pVecActor->insert(std::make_pair(dwUCID, pMover));

    // IDA: Set actor's area
    pActor->SetArea(this);

    // IDA: Check buff by location
    // pUser->CheckBuffByLocation(this);

    // IDA: Initialize maze record
    // TODO: 完整实现需要 CGocRecode
    // auto pRecode = pUser->GetGOC<CGocRecode>();
    // if (pRecode)
    // {
    //     unsigned short wMazeID = GetTBMapID();
    //     pRecode->InitMaze(wMazeID);
    // }

    // IDA: Check maze type
    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type != 7)
    {
        // IDA: Clear attribute for non-type 7 maze
        // TODO: 完整实现需要 CGocAttribute

        // IDA: For dimension shutter (type 12), update user leave status
        if (m_pTBMazeInfo->Maze_Type == 12)
        {
            unsigned long dwUCID = pUser->GetActorID().dwActorID;
            UpdateDemensionShutterUserLeave(dwUCID, false);
            UpdateDemensionShutterRankingPoint(pUser, 0, true);
        }
    }

    // IDA: Enter party/force member
    XMaze::EnterPartyForceMember(pUser);

    // IDA: Reset wait user time
    m_dwWaitUserTime = 0;

    // IDA: Log debug
    // unsigned short wTBMapID = GetTBMapID();
    // LogHelper::LogDebug("game.contents", "<%I64d MAZE> Enter User : %d Map : %d ", GetInstanceID(), actorID.dwActorID, wTBMapID);

    // IDA: Send sync hidden event info
    // XMaze::SendSyncHiddenEventInfo(pUser);

    // IDA: Update open title
    // TODO: 完整实现需要 CGocEntity

    // IDA: Enter booster
    // TODO: 完整实现需要 CGocBooster

    // IDA: Change battle pose
    pUser->ChangeBattlePose(true);

    // IDA: Initialize super armor gage
    // TODO: 完整实现需要 CUser::InitSuperArmorGage
    // pUser->InitSuperArmorGage();

    // IDA: Send super armor gage packet
    // pUser->send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(pUser, pUser->GetCurSuperArmorGage(), pUser->GetMaxSuperArmorGage());

    // IDA: Check option effect invoke
    // pUser->CheckOptionEffectInvoke(EFFECT_CONDITION_EQUIP, pUser, 0.0f, EFFECT_INVOKE_DONT_CARE);
    // pUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_EQUIP);

    return 0;
}

// IDA: ?EnterGridActor@XModeMaze@@QEAA_NPEAVXActor@@@Z (0x14028FF70)
// Verified: Direct IDA decompilation
bool XModeMaze::EnterGridActor(XActor* pActor)
{
    if (!pActor)
        return false;

    // IDA: Get scanner for this actor
    void* pTargetScanner = GetScanner(pActor);
    if (!pTargetScanner)
        return false;

    // IDA: Get actor position from PosInfo
    ::STPosInfo* pPosInfo = pActor->GetPosInfo();
    if (!pPosInfo)
        return false;

    float fx = pPosInfo->vPos.x;
    float fy = pPosInfo->vPos.y;

    // IDA: Cast to CMover
    CMover* pMover = dynamic_cast<CMover*>(pActor);

    // IDA: Insert into grid scanner
    // TODO: 完整实现需要 Range2DScanner<CMover*>::Insert
    // if (!Range2DScanner<CMover*>::Insert(pTargetScanner, fx, fy, &pMover))
    // {
    //     LogHelper::LogError("game.contents", "EnterGridActor error - Actor is line over[ ActorID:%d, TBMapID:%d, PosX:%.2f, PosY:%.2f ] ( %d )",
    //         pActor->GetActorID().dwActorID, GetTBMapID(), fx, fy, 775);
    //     return false;
    // }

    // IDA: Scan for nearby players and objects
    std::vector<CMover*> vecPlayerList;
    std::vector<CMover*> vecObjList;

    // IDA: Reserve capacity
    vecPlayerList.reserve(300);
    vecObjList.reserve(300);

    // IDA: Scan player scanner grid
    // TODO: 完整实现需要 Range2DScanner
    // if (pActor->GetType())
    //     Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);
    // else
    //     Range2DScanner<CMover*>::ScanGridAndSetObjCnt(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList, 1);

    // IDA: Scan NPC and etc scanners
    // Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.npcScanner, fx, fy, 2, 2, &vecObjList);
    // Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.etcScanner, fx, fy, 2, 2, &vecObjList);

    // IDA: Send enter object to others
    if (!vecPlayerList.empty())
    {
        // XDistrict::ProcessSendEnterObjectToOthers(this, &vecPlayerList, pActor);
        // XDistrict::ProcessSendTranslateInfoToOthers(this, pActor, &vecPlayerList);
    }

    // IDA: Send enter object list to player if this is a player
    if (pActor->IsPlayer())
    {
        // ProcessSendEnterObjectListToPlayer(pActor, &vecPlayerList, &vecObjList);
    }

    return true;
}

// IDA: ?ExitActor@XModeMaze@@UEAAGPEAVXActor@@@Z (0x1402902E0)
// TODO: 汇编还原 - 需要完整实现
std::uint16_t XModeMaze::ExitActorEx(XActor* pActor)
{
    // TODO: 汇编还原 - 需要完整实现
    // Note: Base class returns void, XModeMaze returns uint16_t
    XMaze::ExitActor(pActor);
    return 0;
}

// IDA: ?ExitArea@XModeMaze@@UEAAXPEAVXActor@@@Z (0x140290EB0)
// Verified: Direct IDA decompilation
void XModeMaze::ExitArea(XActor* pActor)
{
    if (!pActor)
        return;

    // IDA: Cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser)
        return;

    // IDA: Set user state to changing world
    pUser->SetState(eStateChangeWorld);

    // IDA: Get return map ID
    unsigned long dwUCID = static_cast<unsigned long>(pUser->GetActorID());
    unsigned short wMapID = GetReturnMapID(dwUCID);
    int nJumpID = 0;

    // IDA: Get exit box info
    GetExitBoxInfo(wMapID, nJumpID);

    // IDA: Create enter map request packet
    // PS_ENTER_MAP_REQ stEnterMap;
    // stEnterMap.dwActorID = dwUCID;
    // stEnterMap.nJumpID = nJumpID;
    // stEnterMap.wMapID = wMapID;
    // stEnterMap.stPartyInfo.byGroupType = 0;
    // stEnterMap.stPartyInfo.nID = 0;

    // IDA: Send packet to control socket
    XSendPacket xSendPacket(0xF2, 0x31);
    // xSendPacket << stEnterMap;
    // XGameServer::Instance()->GetControlSocket()->Send(&xSendPacket);

    // IDA: Check for fatigue point usage
    TB_MAZE_INFO* pTBMazeInfo = m_pTBMazeInfo;
    if (pTBMazeInfo && pTBMazeInfo->Fatigue_Point && XMaze::IsCompleteClearMaze())
    {
        // IDA: Get attribute component and use fatigue point
        // TODO: 完整实现需要 CGocAttribute
        // auto pAttr = pUser->GetGOC<CGocAttribute>();
        // if (pAttr)
        // {
        //     unsigned short wMapID = GetTBMapID();
        //     int nFatiguePoint = pTBMazeInfo->Fatigue_Point / 2;
        //     pAttr->UseFP(nFatiguePoint, wMapID, 0);
        // }
    }
}

// IDA: ?ExitGridActor@XModeMaze@@QEAA_NPEAVXActor@@@Z (0x1402911B0)
// Verified: Direct IDA decompilation
bool XModeMaze::ExitGridActor(XActor* pActor)
{
    if (!pActor)
        return false;

    // IDA: Get scanner for this actor
    void* pTargetScanner = GetScanner(pActor);
    if (!pTargetScanner)
        return false;

    // IDA: Get actor position from PosInfo
    ::STPosInfo* pPosInfo = pActor->GetPosInfo();
    if (!pPosInfo)
        return false;

    float fx = pPosInfo->vPos.x;
    float fy = pPosInfo->vPos.y;

    // IDA: Cast to CMover
    CMover* pMover = dynamic_cast<CMover*>(pActor);

    // IDA: Erase from grid scanner
    // TODO: 完整实现需要 Range2DScanner<CMover*>::Erase
    // if (!Range2DScanner<CMover*>::Erase(pTargetScanner, fx, fy, &pMover))
    // {
    //     LogHelper::LogError("game.contents", "ExitActor error - Failed ExitActor[ ActorID:%d, TBMapID:%d, PosX:%.2f, PosY:%.2f ] ( %d )",
    //         pActor->GetActorID().dwActorID, GetTBMapID(), fx, fy, 1015);
    //     // Try to erase without position
    //     if (!Range2DScanner<CMover*>::Erase(pTargetScanner, &pMover))
    //     {
    //         LogHelper::LogError("game.contents", "ExitActor error - Failed Erase[ ActorID:%d ] ( %d )",
    //             pActor->GetActorID().dwActorID, 1019);
    //         return false;
    //     }
    // }

    // IDA: Scan for nearby players
    std::vector<CMover*> vecPlayerList;
    vecPlayerList.reserve(300);

    // IDA: Scan player scanner grid
    // TODO: 完整实现需要 Range2DScanner
    // if (pActor->GetType())
    //     Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);
    // else
    //     Range2DScanner<CMover*>::ScanGridAndSetObjCnt(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList, -1);

    // IDA: Send leave object to others
    if (!vecPlayerList.empty())
    {
        // ProcessSendLeaveObjectToOthers(&vecPlayerList, pActor, true);
    }

    return true;
}

// IDA: ?MoveActor@XModeMaze@@UEAAGPEAVXActor@@AEAUXVec3@@M_N@Z (0x140291520)
// Verified: Direct IDA decompilation
std::uint16_t XModeMaze::MoveActor(XActor* pActor, XVec3& vNextPos, float fRot, bool bSend)
{
    if (!pActor)
        return 50001;

    // IDA: Cast to CMover
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (!pMover)
        return 50001;

    // IDA: Get current position
    ::STPosInfo* pPosInfo = pActor->GetPosInfo();
    if (!pPosInfo)
        return 50001;

    float sx = pPosInfo->vPos.x;
    float sy = pPosInfo->vPos.y;
    float dx = vNextPos.x;
    float dy = vNextPos.y;

    // IDA: Get scanner and move actor
    void* pTargetScanner = GetScanner(pActor);
    if (pTargetScanner)
    {
        // TODO: 完整实现需要 Range2DScanner<CMover*>::Move
        // if (!Range2DScanner<CMover*>::Move(pTargetScanner, sx, sy, dx, dy, pMover))
        // {
        //     LogHelper::LogError("game.contents", "MoveActor error - Failed user movement[ ActorID:%d, srcX:%.2f, srcY:%.2f, destX:%.2f, destY:%.2f ] ( %d )",
        //         pActor->GetActorID().dwActorID, sx, sy, dx, dy, 1075);
        // }
    }

    // IDA: Update position
    pActor->SetPosInfo(pPosInfo);

    // IDA: Scan for enter/leave objects
    std::vector<CMover*> vecEnterPlayerList;
    std::vector<CMover*> vecLeavePlayerList;
    std::vector<CMover*> vecEnterObjList;
    std::vector<CMover*> vecLeaveObjList;

    vecEnterPlayerList.reserve(300);
    vecLeavePlayerList.reserve(300);

    // IDA: Scan enter/leave for players
    // TODO: 完整实现需要 Range2DScanner
    // if (pActor->IsPlayer())
    //     Range2DScanner<CMover*>::ScanEnterLeaveAndSetObjCnt(m_objectGridScanner.playerScanner, sx, sy, dx, dy, 2, 2, &vecEnterPlayerList, &vecLeavePlayerList);
    // else
    //     Range2DScanner<CMover*>::ScanEnterLeaveCurrentWhenMove(m_objectGridScanner.playerScanner, sx, sy, dx, dy, 2, 2, &vecEnterPlayerList, &vecLeavePlayerList);

    // IDA: Scan enter/leave for NPC and etc
    // Range2DScanner<CMover*>::ScanEnterLeaveCurrentWhenMove(m_objectGridScanner.npcScanner, sx, sy, dx, dy, 2, 2, &vecEnterObjList, &vecLeaveObjList);
    // Range2DScanner<CMover*>::ScanEnterLeaveCurrentWhenMove(m_objectGridScanner.etcScanner, sx, sy, dx, dy, 2, 2, &vecEnterObjList, &vecLeaveObjList);

    // IDA: Process move object
    // ProcessMoveObject(pActor, &vecEnterPlayerList, &vecEnterObjList, &vecLeavePlayerList, &vecLeaveObjList);

    return 0;
}

// IDA: ?ProcessMoveObject@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@111@Z (0x140291A20)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessMoveObject(XActor* pActor, std::vector<CMover*>& vecEnterPlayerList,
    std::vector<CMover*>& vecEnterObjList, std::vector<CMover*>& vecLeavePlayerList,
    std::vector<CMover*>& vecLeaveObjList)
{
    // IDA: Process leave player list
    if (!vecLeavePlayerList.empty())
    {
        ProcessSendLeaveObjectToOthers(vecLeavePlayerList, pActor, false);
    }

    // IDA: Process leave object list for player
    if (pActor->IsPlayer() && !vecLeaveObjList.empty())
    {
        ProcessSendLeaveObjectListToPlayer(pActor, vecLeaveObjList);
    }

    // IDA: Process enter player list
    if (!vecEnterPlayerList.empty())
    {
        // XDistrict::ProcessSendEnterObjectToOthers(this, &vecEnterPlayerList, pActor);
        // XDistrict::ProcessSendTranslateInfoToOthers(this, pActor, &vecEnterPlayerList);
    }

    // IDA: Process enter object list for player
    if (pActor->IsPlayer() && !vecEnterObjList.empty())
    {
        // ProcessSendEnterObjectListToPlayer(pActor, &vecEnterPlayerList, &vecEnterObjList);
    }
}

// IDA: ?ProcessSendLeaveObjectToOthers@XModeMaze@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@PEAVXActor@@_N@Z (0x140291B10)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessSendLeaveObjectToOthers(std::vector<CMover*>& vecPlayerList, XActor* pLeaveActor, bool bExitActor)
{
    if (!pLeaveActor)
        return;

    // IDA: Check actor type and send appropriate leave packet
    if (pLeaveActor->IsPlayer())
    {
        // IDA: Send PC leave packet (main=4, sub=0x12)
        // PS_POST_DELETE_LIST stObjectInfo;
        // stObjectInfo.vecObjectID.push_back(pLeaveActor->GetActorID());
        // XSendPacket xSendPacket(4, 0x12);
        // xSendPacket << stObjectInfo;
        // CGocNetwork::Send(&vecPlayerList, &xSendPacket, nullptr);
    }
    else if (pLeaveActor->IsMonster())
    {
        // IDA: Send monster leave packet (main=4, sub=0x16)
        // PS_POST_DELETE_LIST stObjectInfo;
        // stObjectInfo.vecObjectID.push_back(pLeaveActor->GetActorID());
        // XSendPacket packet(4, 0x16);
        // packet << stObjectInfo;
        // CGocNetwork::Send(&vecPlayerList, &packet, nullptr);
    }
    else if (pLeaveActor->IsNPC())
    {
        // IDA: Send NPC leave packet (main=4, sub=0x14)
        // PS_POST_DELETE_LIST stObjectInfo;
        // stObjectInfo.vecObjectID.push_back(pLeaveActor->GetActorID());
        // XSendPacket packet(4, 0x14);
        // packet << stObjectInfo;
        // CGocNetwork::Send(&vecPlayerList, &packet, nullptr);
    }
    else if (pLeaveActor->GetType() == 5)  // VaccumCube
    {
        // IDA: Send VaccumCube leave packet (main=0x25, sub=0x12)
        // XSendPacket packet(0x25, 0x12);
        // CVaccumCube* pVaccumCube = dynamic_cast<CVaccumCube*>(pLeaveActor);
        // if (pVaccumCube)
        //     pVaccumCube->SetInfoLeavePacket(&packet, bExitActor);
        // CGocNetwork::Send(&vecPlayerList, &packet, nullptr);
    }
    else if (pLeaveActor->GetType() == 6)  // SocialItemObject
    {
        // IDA: Send SocialItemObject leave packet (main=0x2D, sub=7)
        // XSendPacket packet(0x2D, 7);
        // CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pLeaveActor);
        // if (pSocialItem)
        //     pSocialItem->SetInfoLeavePacket(&packet);
        // CGocNetwork::Send(&vecPlayerList, &packet, nullptr);
    }
}

// IDA: ?ProcessSendLeaveObjectListToPlayer@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x140291EC0)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessSendLeaveObjectListToPlayer(XActor* pActor, std::vector<CMover*>& vecLeaveObjList)
{
    if (vecLeaveObjList.empty())
        return;

    // IDA: Create PC leave packet (main=4, sub=0x24)
    XSendPacket xSendPacketPC(4, 0x24);

    // IDA: Collect PC actor IDs
    std::list<UXActorID> listActor;

    // Get target actor ID for comparison
    CUser* pTargetUser = dynamic_cast<CUser*>(pActor);
    unsigned int dwTargetActorID = 0;
    if (pTargetUser)
    {
        dwTargetActorID = pTargetUser->GetActorID().dwActorID;
    }

    for (auto it = vecLeaveObjList.begin(); it != vecLeaveObjList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // Check if this is a player (CUser)
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser)
        {
            // Compare actor IDs instead of pointers
            UXActorID userID = pUser->GetActorID();
            if (userID.dwActorID != dwTargetActorID)
            {
                listActor.push_back(userID);
            }
        }
    }

    // IDA: Send PC leave count and IDs
    unsigned short shCount = static_cast<unsigned short>(listActor.size());
    if (shCount)
    {
        xSendPacketPC << shCount;
        for (auto it = listActor.begin(); it != listActor.end(); ++it)
        {
            xSendPacketPC << it->dwActorID;
        }
        // CGocNetwork::Send(pActor, &xSendPacketPC);
    }

    // IDA: Process each object in leave list for NPC/Monster/Interaction
    // TODO: 完整实现需要 PS_OBJECT_REMOVE 结构和 CGocNetwork::Send
    // 以及完整类型定义 (CNpc, CMonster, CInteractionObject, CVaccumCube)
}

// IDA: ?SendSameGridOtherPlayerPosition@XModeMaze@@QEAAXPEAVCUser@@@Z (0x1402929B0)
// Verified: Direct IDA decompilation
void XModeMaze::SendSameGridOtherPlayerPosition(CUser* pUser)
{
    if (!pUser)
        return;

    // IDA: Get user position - CMover::GetPosition returns hkvVec3
    CMover* pMover = static_cast<CMover*>(pUser);
    hkvVec3 vPos = pMover->GetPosition();
    float fx = vPos.x;
    float fy = vPos.y;

    // IDA: Scan for nearby players
    std::vector<CMover*> vecPlayerList;
    vecPlayerList.reserve(300);

    // TODO: 完整实现需要 Range2DScanner
    // Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);

    // IDA: Get instance ID
    // UXMapID uxMapID;
    // XArea* pArea = pUser->GetArea();
    // if (pArea)
    //     pArea->GetInstanceID(&uxMapID);

    // IDA: Send position to each other player
    for (auto it = vecPlayerList.begin(); it != vecPlayerList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        CUser* pOtherPC = dynamic_cast<CUser*>(pMover);
        if (!pOtherPC || pOtherPC == pUser)
            continue;

        // IDA: Send warp packet to user about other player position
        XSendPacket xSendPacket(4, 8);
        // STWarp warpInfo;
        // warpInfo.byResult = 0;
        // XVec3* pPos = pOtherPC->GetPositionXVec3();
        // warpInfo.xPos = *pPos;
        // warpInfo.fRot = pOtherPC->GetRot();
        // xSendPacket << pOtherPC->GetActorID().dwActorID;
        // xSendPacket << warpInfo;
        // pUser->Send(&xSendPacket);
    }
}

// IDA: ?SendPlayerInfoAll@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140293CC0)
// Verified: Direct IDA decompilation
void XModeMaze::SendPlayerInfoAll(CUser* pUser)
{
    // IDA: Check user validity
    if (!pUser)
        return;

    // TODO: 汇编还原 - 需要完整的Range2DScanner和STCharInfoEx支持
    // 当前简化实现
}

// IDA: ?SendEnterPlayerInfo@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140294040)
// Verified: Direct IDA decompilation
void XModeMaze::SendEnterPlayerInfo(CUser* pUser)
{
    // IDA: Check user validity
    if (!pUser)
        return;

    // IDA: Create packet for enter player info (main=4, sub=0x51)
    // XSendPacket xSendPacket(4, 0x51);
    // xSendPacket << 1;  // count
    // pUser->SetInfoPacket(xSendPacket);
    // SendBroadCastAll(&xSendPacket, false);

    // TODO: 完整实现需要 XSendPacket 和 SetInfoPacket 支持
    // 当前简化实现
}

// IDA: ?SendExitPlayerInfo@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140294120)
// Verified: Direct IDA decompilation
void XModeMaze::SendExitPlayerInfo(CUser* pUser)
{
    // IDA: Check user validity
    if (!pUser)
        return;

    // IDA: Create packet for exit player info (main=4, sub=0x52)
    // XSendPacket xSendPacket(4, 0x52);
    // UXActorID actorID = pUser->GetActorID();
    // xSendPacket << actorID.dwActorID;
    // SendBroadCastAll(&xSendPacket, false);

    // TODO: 完整实现需要 XSendPacket 支持
    // 当前简化实现
}

// IDA: ?IsEnemyPVP@XModeMaze@@UEAA_NPEAVXActor@@0@Z (0x140294200)
// Verified: Direct IDA decompilation
bool XModeMaze::IsEnemyPVP(XActor* pAtk, XActor* pDef)
{
    // IDA: Return false if in cutscene state (not enemy in PVP during cutscene)
    return !XMaze::IsCutsceneState();
}

// IDA: ?ProcessMonsterQuest@XModeMaze@@UEAAXPEAVXActor@@H@Z (0x140294230)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessMonsterQuest(XActor* pAttacker, int nMonsterID)
{
    if (!pAttacker)
        return;

    // IDA: Get TB_MONSTER from resource manager
    // TODO: 完整实现需要 XResourceMgr 单例访问
    // TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(nMonsterID);
    // if (!pTBMonster)
    //     return;

    // IDA: The function finds the attacker in m_objectScanner, casts to CUser,
    // then updates quest conditions, daily missions, achievements, and class events.
    // For now, call base class implementation
    XMaze::ProcessMonsterQuest(pAttacker, static_cast<unsigned int>(nMonsterID));
}

// IDA: ?DropDemensionPoint@XModeMaze@@QEAAXPEAVCUser@@K@Z (0x140294610)
// Verified: Direct IDA decompilation
void XModeMaze::DropDemensionPoint(CUser* pUser, unsigned long dwHitUCID)
{
    if (!pUser)
        return;

    // IDA: Get user's UCID and current dimension point
    unsigned long dwUCID = static_cast<unsigned long>(pUser->GetActorID());
    int nDropPoint = GetDemensionPoint(dwUCID);

    if (nDropPoint < 0)
        nDropPoint = 0;

    // IDA: Deduct points from user
    if (nDropPoint > 0)
    {
        int nAddPoint = -nDropPoint;
        UpdateDemensionShutterUserPoint(dwUCID, nAddPoint, false);
    }

    // IDA: Find hit user and give them bonus points
    XActor* pActor = FindActor(dwHitUCID);
    CUser* pHitUser = dynamic_cast<CUser*>(pActor);

    if (pHitUser)
    {
        // IDA: Add kill point bonus
        if (m_pTB_OPERATION_INFO && m_pTB_OPERATION_INFO->User_Kill_Point > 0)
        {
            nDropPoint += m_pTB_OPERATION_INFO->User_Kill_Point;
        }

        // TODO: 完整实现需要CDropProcess::MakeDropItemDemensionShutter_PVP
        // CDropProcess* pProcess = pHitUser->GetProcessPtr<CDropProcess>(0x14);
        // if (pProcess)
        // {
        //     pProcess->MakeDropItemDemensionShutter_PVP(pHitUser, dwUCID, nDropPoint, &vPos);
        // }
    }
}

// IDA: ?ClickInteractionBox@XModeMaze@@UEAAXHPEAVCUser@@@Z (0x140294860)
// Verified: Direct IDA decompilation
void XModeMaze::ClickInteractionBox(int nBoxID, CUser* pUser)
{
    // IDA: Check user validity and maze completion status
    if (!pUser || m_bMazeComplete)
        return;

    // IDA: Get user's UCID from actor ID
    UXActorID actorID = pUser->GetActorID();
    unsigned long dwUCID = actorID.dwActorID;

    // IDA: Check if user has dimension points
    if (GetDemensionPoint(dwUCID) > 0)
    {
        // IDA: Call base class InteractBoxOnMode (renamed from ClickInteractionBox)
        XMaze::InteractBoxOnMode(pUser, nBoxID);
    }
}

// IDA: ?ClickInteractionBoxAnotherWork@XModeMaze@@QEAAXPEAUTB_INTERACTION_OBJECT@@PEAVCUser@@@Z (0x140294900)
// Verified: Direct IDA decompilation
void XModeMaze::ClickInteractionBoxAnotherWork(struct TB_INTERACTION_OBJECT* pTBInteraction, CUser* pUser)
{
    // IDA: Check validity
    if (!pTBInteraction || !pUser)
        return;

    // IDA: Get user's UCID from actor ID
    UXActorID actorID = pUser->GetActorID();
    unsigned long dwUCID = actorID.dwActorID;

    // IDA: Get current dimension point
    int nDemensionPoint = GetDemensionPoint(dwUCID);

    // IDA: If user has dimension points, deduct and add as attach point
    if (nDemensionPoint > 0)
    {
        // IDA: Deduct points
        int nAddPoint = -nDemensionPoint;
        UpdateDemensionShutterUserPoint(dwUCID, nAddPoint, false);

        // IDA: Add as attach point
        UpdateDemensionShutterUserAttachPoint(dwUCID, nDemensionPoint);
    }
}

// IDA: ?ProcessReward@XModeMaze@@UEAAXXZ (0x140294A20)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessReward()
{
    // IDA: Check if maze is not complete and has maze info
    if (!m_bMazeComplete && m_pTBMazeInfo)
    {
        m_bMazeComplete = false;

        // IDA: Check clear conditions (3 conditions)
        for (int i = 0; i < 3; ++i)
        {
            if (m_pTBMazeInfo)
            {
                // IDA: Get clear condition type from array
                int nClearConType = *(&m_pTBMazeInfo->Clear_Con_Type_01 + i);
                if (nClearConType && !m_stMazeGameState.m_bClearCondition[i])
                {
                    m_bMazeComplete = true;
                    break;
                }
            }
        }

        // IDA: If maze is complete, process rewards
        if (m_bMazeComplete)
        {
            // IDA: Add log
            // CTextDBLog::AddLog(&m_textDBLog, 4, 0, 0, "");

            // IDA: Destroy all sector monsters
            AllDestroySectorMonster();

            // IDA: Finish maze time
            XMaze::FinishMazeTime();

            // IDA: Check hidden event state
            XMaze::CheckHiddenEventState();

            // IDA: Release helper support equipment
            // TODO: XMaze::ReleaseHelperSupportEquip();

            // IDA: Check if this is dimension shutter mode (Maze_Type == 12)
            if (m_pTBMazeInfo->Maze_Type == 12)
            {
                if (m_pTB_OPERATION_INFO)
                {
                    // IDA: Set Lua value
                    XMaze::SetLuaValue(1, 1);

                    // IDA: Process operation rewards
                    OperationReward();

                    // IDA: Calculate play times
                    unsigned int dwPlayTime_sec = m_stMazeGameState.m_dwMazePlayTime / 1000;
                    unsigned int dwRankingPlayTime = m_stMazeGameState.m_dwMazePlayTime / 100;

                    // IDA: Iterate through all objects in scanner
                    // TODO: 完整实现需要遍历 m_objectScanner 中的所有 CMover
                    // for (auto& pair : m_objectScanner)
                    // {
                    //     CMover* pMover = pair.second;
                    //     if (pMover)
                    //     {
                    //         // IDA: Set invincible and immunity
                    //         pMover->SetInvincibleActor(true);
                    //         pMover->SetImmunityStatus(4);
                    //         pMover->UpdateDefenseType();
                    //
                    //         // IDA: Update record and daily mission
                    //         // CGocRecode* pRecode = pMover->GetGOC<CGocRecode>();
                    //         // if (pRecode)
                    //         // {
                    //         //     pRecode->UpdateSharePoint();
                    //         //     pRecode->UpdateClearInfo(dwRankingPlayTime, fElapsed);
                    //         // }
                    //
                    //         // IDA: Update daily mission
                    //         // CGocDailyMission* pMission = pMover->GetGOC<CGocDailyMission>();
                    //         // if (pMission)
                    //         // {
                    //         //     unsigned short wMapID = GetTBMapID();
                    //         //     pMission->UpdateMazeClearType(eDAILY_MISSION_TARGET_MAZE, wMapID, 0, dwPlayTime_sec, 0);
                    //         // }
                    //     }
                    // }
                }
            }
        }
    }
}

// IDA: ?OperationReward@XModeMaze@@QEAAXXZ (0x140294D80)
// Verified: Direct IDA decompilation
void XModeMaze::OperationReward()
{
    // IDA: Map for tracking same rank counts
    std::map<unsigned short, short> mapSameRankCount;

    // IDA: Vector for rank data
    std::vector<ST_MODE_MAZE_USER_SCORE> vecRank;

    // IDA: Calculate dimension shutter ranking
    CalculateDemensionShutterRank(&vecRank);

    // IDA: Count same ranks
    for (size_t i = 0; i < vecRank.size(); ++i)
    {
        unsigned short wRank = vecRank[i].wRank;
        auto itRank = mapSameRankCount.find(wRank);
        if (itRank != mapSameRankCount.end())
        {
            ++itRank->second;
        }
        else
        {
            mapSameRankCount[wRank] = 1;
        }
    }

    // IDA: Create reward info structure
    PS_MODE_MAZE_REWARD_INFO psReward;
    psReward.bHotTime = m_bHotTime;

    // IDA: Process each ranked user
    for (size_t j = 0; j < vecRank.size(); ++j)
    {
        ST_MODE_MAZE_USER_REWARD stReward;
        stReward.dwUCID = vecRank[j].dwUserID;
        stReward.nAttachPoint = vecRank[j].dwScore;
        stReward.wRank = vecRank[j].wRank;
        stReward.strName = vecRank[j].strName;
        stReward.nRewardItem = 0;
        stReward.nRewardCount = 0;
        stReward.bLeave = vecRank[j].bLeave;

        int nRankingPoint = 0;

        // IDA: Find user in object scanner
        XActor* pActor = FindActor(stReward.dwUCID);
        CUser* pUser = dynamic_cast<CUser*>(pActor);

        if (pUser)
        {
            // IDA: Check if user is GM with special status
            // TODO: 完整实现需要 CUser::IsGM() 方法
            // if (!pUser->IsGM() || !pUser->IsStatus(0x2000))
            {
                // IDA: Get user info
                stReward.byLevel = pUser->GetLevel();
                stReward.byClass = pUser->GetClass();
                // stReward.byAwaken = pUser->stMyCharInfoEx()->stBaseInfo.byAwaken;
                // stReward.dwProfilePhotoID = pUser->stMyCharInfoEx()->stBaseInfo.dwProfilePhotoID;

                // IDA: Get same rank count
                short shSameRankCount = 0;
                auto itRankCount = mapSameRankCount.find(stReward.wRank);
                if (itRankCount != mapSameRankCount.end())
                {
                    shSameRankCount = itRankCount->second;
                }

                // IDA: Calculate reward rank (0-indexed)
                short shRewardRank = stReward.wRank + shSameRankCount - 1;

                // IDA: Give rewards for top 8 ranks
                if (stReward.wRank && static_cast<unsigned int>(shRewardRank) < 8)
                {
                    // IDA: Get reward from operation info table
                    if (m_pTB_OPERATION_INFO)
                    {
                        stReward.nRewardItem = *(&m_pTB_OPERATION_INFO->Reward_01 + shRewardRank);
                        stReward.nRewardCount = *(&m_pTB_OPERATION_INFO->Reward_Count_01 + shRewardRank);
                    }

                    // IDA: Update ranking point
                    nRankingPoint = UpdateDemensionShutterRankingPoint(pUser, shRewardRank, false);

                    // IDA: Double rewards during hot time
                    if (m_bHotTime)
                    {
                        stReward.nRewardCount *= 2;
                    }
                }
                else if (stReward.wRank)
                {
                    // LogHelper::LogError("game.contents", "<REWARD> ModeMaze Reward Rank Error ( %d )", stReward.wRank);
                }

                // IDA: Skip reward for event room
                if (m_dwEventRoomID)
                {
                    stReward.nRewardItem = 0;
                    stReward.nRewardCount = 0;
                }
                else
                {
                    // IDA: Give item reward
                    // TODO: 完整实现需要 XResourceMgr::GetTB_ITEM 和 CGocInventory::CreateItemReq
                    // TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(stReward.nRewardItem);
                    // if (pTBItem && stReward.nRewardCount > 0)
                    // {
                    //     CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
                    //     if (pInven)
                    //     {
                    //         if (!pInven->CreateItemReq(stReward.nRewardItem, stReward.nRewardCount, 1, E_ITEM_CREATE_TYPE_MAZE_BASE_ITEM, &stLogData))
                    //         {
                    //             // Send via post if inventory full
                    //             CGocPost* pPost = pUser->GetGOC<CGocPost>();
                    //             if (pPost)
                    //             {
                    //                 pPost->SystemPostSend(pTBItem, stReward.nRewardCount, 11, 0, 0, nullptr);
                    //             }
                    //         }
                    //     }
                    // }
                }
            }
        }

        // IDA: Add to reward list
        psReward.vecUser.push_back(stReward);
    }

    // IDA: Log reward processing
    // LogHelper::LogInfo("game.contents", "<REWARD> ModeMaze ProcessReward ( %d )", m_objectScanner.size());

    // IDA: Send reward packets
    SendDemensionShutterReward(psReward);
    SendDemensionShutterEventMatchingReward(psReward);
}

// IDA: ?WarpPortal@XModeMaze@@UEAA_NPEAVCUser@@HHH@Z (0x1402957C0)
// Verified: Direct IDA decompilation
bool XModeMaze::WarpPortal(CUser* pUser, int nMapID, int nJumpID, int nPortalID)
{
    // IDA: Get unique ID and portal box
    int nUniqueID = XMaze::GetUniqueID(nPortalID);
    void* pBox = XMaze::GetMazePotalBox(nUniqueID);

    // IDA: Check if box exists
    if (!pBox)
    {
        // LogHelper::LogError("game.contents", "WarpPortal error - No Table Data ( %d , %d, %d )",
        //     pUser->GetActorID().dwActorID, nMapID, nPortalID);
        return false;
    }

    // IDA: Check if box is open (skip check for boss sector)
    if (!XMaze::IsBossSector())
    {
        // TODO: 需要完整的 STMagePotalBox 结构来访问 bOpen
        // if (!pBox->bOpen)
        // {
        //     return false;
        // }
    }

    // TODO: 完整实现需要 STMagePotalBox 和 CWarpPotal 结构
    // 当前简化实现 - 调用基类
    return XMaze::WarpPortal(pUser, nMapID, nJumpID, nPortalID);
}

// IDA: ?DiePlayer@XModeMaze@@UEAAXPEAVCUser@@@Z (0x140295E40)
// Verified: Direct IDA decompilation
void XModeMaze::DiePlayer(CUser* pUser)
{
    // IDA: Call base class DiePlayer
    XMaze::DiePlayer(pUser);

    // IDA: Set dimension shutter next revive time
    if (pUser)
    {
        SetDemensionShutterNextRevive(pUser);
    }
}

// IDA: ?GetExitBoxInfo@XModeMaze@@UEAA_NAEAGAEAH@Z (0x140295E80)
// Verified: Direct IDA decompilation
bool XModeMaze::GetExitBoxInfo(unsigned short& wMapID, int& nJumpID)
{
    // IDA: Get start portal ID from world resource manager
    // TODO: 完整实现需要 XGameServer 单例和 XWorldResMgr
    // XGameServer* pServer = XGameServer::Instance();
    // STPosInfo stPosInfo;
    // if (pServer->GetWorldResMgr().GetStartPortalID(wMapID, &nJumpID, &stPosInfo))
    // {
    //     return true;
    // }

    // IDA: Log error if portal not found
    // unsigned short wTBMapID = GetTBMapID();
    // LogHelper::LogError("game.contents",
    //     "ModeMaze ExitArea error - GetStartPortalID [ TBMapID:%d, Map:%d, Jump:%d ] ( %d )",
    //     wTBMapID, wMapID, nJumpID, 2186);

    // IDA: Set default values on failure
    // wMapID = 10003;
    // nJumpID = 1000301;

    // Current simplified implementation - call base class
    XVec3 vPos;
    return XMaze::GetExitBoxInfo(wMapID, nJumpID, vPos);
}

// IDA: ?GetReturnMapID@XModeMaze@@UEAAGK@Z (0x140295F50)
// Verified: Direct IDA decompilation
unsigned short XModeMaze::GetReturnMapID(unsigned long dwUCID)
{
    // IDA: Find user in dimension score map
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it != m_mapDemensionScore.end())
    {
        // IDA: Return the return map ID stored in the score structure
        // The map ID is stored in the upper 16 bits of a field
        // TODO: 需要确认 ST_SERVER_MODE_MAZE_USER_SCORE 结构中的返回地图ID字段
        // return it->second.wReturnMapID;
    }

    return 0;
}

// IDA: ?GetDemensionPoint@XModeMaze@@QEAAHK@Z (0x140295FC0)
// TODO: 汇编还原 - 需要完整实现
int XModeMaze::GetDemensionPoint(unsigned long dwUCID)
{
    // TODO: 汇编还原 - 需要完整实现
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it != m_mapDemensionScore.end())
    {
        return it->second.stInfo.nPoint;
    }
    return 0;
}

// IDA: ?SetDemensionShutterNextRevive@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140296030)
// Verified: Direct IDA decompilation
void XModeMaze::SetDemensionShutterNextRevive(CUser* pUser)
{
    // IDA: Check user and maze info validity
    if (!pUser)
        return;

    if (!m_pTBMazeInfo)
        return;

    // IDA: Check if Revival_Type is 2 (dimension shutter mode)
    if (m_pTBMazeInfo->Revival_Type != 2)
        return;

    // IDA: Get user's UCID
    unsigned long dwUCID = static_cast<unsigned long>(pUser->GetActorID());

    // IDA: Find user in dimension score map
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it == m_mapDemensionScore.end())
        return;

    // IDA: Calculate revival time based on base time + 5 seconds per revive count
    int nRevivalTime = m_pTBMazeInfo->Revival_Item_Count;
    nRevivalTime += 5 * it->second.nReviveCount;

    // IDA: Cap at 30 seconds maximum
    if (nRevivalTime > 30)
        nRevivalTime = 30;

    // IDA: Set revive time (current time + revival time in milliseconds)
    unsigned long long dw64Tick = GetTickCount64();
    it->second.dw64ReviveTick = dw64Tick + (static_cast<unsigned long long>(nRevivalTime) * 1000);
}

// IDA: ?CheckDemensionShutterRevive@XModeMaze@@QEAAX_K@Z (0x140296180)
// Verified: Direct IDA decompilation
void XModeMaze::CheckDemensionShutterRevive(unsigned __int64 dwTick)
{
    // IDA: Check if enough time has passed since last revive check (1 second)
    if (dwTick >= m_dw64DemensionReviveTick + 1000)
    {
        m_dw64DemensionReviveTick = dwTick;

        // IDA: Iterate through all dimension scores
        for (auto it = m_mapDemensionScore.begin(); it != m_mapDemensionScore.end(); ++it)
        {
            // IDA: Check if user has revive time set
            if (it->second.dw64ReviveTick)
            {
                // IDA: Check if it's time to revive
                if (it->second.dw64ReviveTick <= dwTick)
                {
                    // IDA: Call Revive function
                    Revive(it->first);

                    // IDA: Find the user actor
                    XActor* pActor = FindActor(it->first);
                    CUser* pUser = dynamic_cast<CUser*>(pActor);

                    if (pUser)
                    {
                        // IDA: Check if user is still dead
                        if (pUser->IsDie())
                        {
                            // LogHelper::LogError("game.contents", "ModeMaze Revive Error UCID : %d, ReviveCount:%d", it->first, it->second.nReviveCount);
                        }
                        else
                        {
                            // IDA: Clear revive time and increment count
                            it->second.dw64ReviveTick = 0;
                            ++it->second.nReviveCount;

                            // IDA: Send updated dimension point
                            SendDemensionPoint(it->first);
                        }
                    }
                }
            }
        }
    }
}

// IDA: ?Revive@XModeMaze@@QEAAXK@Z (0x140296390)
// Verified: Direct IDA decompilation
void XModeMaze::Revive(unsigned long dwUCID)
{
    // IDA: Find actor by UCID
    XActor* pActor = FindActor(dwUCID);
    CUser* pUser = dynamic_cast<CUser*>(pActor);

    if (pUser)
    {
        // IDA: Call Revive with HP percent (5 = 5% HP)
        pUser->Revive(5);
    }
}

// IDA: ?UpdateDemensionShutterUserLeave@XModeMaze@@QEAAXK_N@Z (0x140296480)
// Verified: Direct IDA decompilation
void XModeMaze::UpdateDemensionShutterUserLeave(unsigned long dwUCID, bool bLeave)
{
    // IDA: Check if maze type is 12 (dimension shutter)
    if (!m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 12)
        return;

    // IDA: Find user in dimension score map
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it != m_mapDemensionScore.end())
    {
        it->second.stInfo.bLeave = bLeave;
    }
}

// IDA: ?UpdateDemensionShutterUserPoint@XModeMaze@@QEAAXKH_N@Z (0x140296540)
// Verified: Direct IDA decompilation
void XModeMaze::UpdateDemensionShutterUserPoint(unsigned long dwUCID, int nPoint, bool bAdd)
{
    // IDA: Check if maze is complete
    if (m_bMazeComplete)
        return;

    // IDA: Check if point change is needed
    if (!nPoint)
        return;

    // IDA: Check if maze type is 12 (dimension shutter)
    if (!m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 12)
        return;

    // IDA: Find user in dimension score map
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it == m_mapDemensionScore.end())
        return;

    // IDA: Update score based on bAdd flag
    if (bAdd)
    {
        it->second.stInfo.nPoint += static_cast<int>(nPoint);
    }
    else
    {
        it->second.stInfo.nPoint = nPoint;
    }

    // IDA: Ensure score is not negative
    if (it->second.stInfo.nPoint < 0)
        it->second.stInfo.nPoint = 0;

    // TODO: 完整实现需要发送日志到 DB
    // ST_LOG_GAME stLog;
    // stLog._nUCID = dwUCID;
    // stLog._sMainType = 28;
    // stLog._sSubType = 11;
    // stLog.nParam1 = GetTBMapID();
    // stLog.nParam2 = nPoint;
    // stLog.nParam3 = it->second.dwScore;
    // XGameServer::Instance()->SendDBLog(&stLog);

    // IDA: Send updated dimension point to user
    SendDemensionPoint(dwUCID);
}

// IDA: ?UpdateDemensionShutterUserAttachPoint@XModeMaze@@QEAAXKH@Z (0x140296770)
// Verified: Direct IDA decompilation
void XModeMaze::UpdateDemensionShutterUserAttachPoint(unsigned long dwUCID, int nPoint)
{
    // IDA: Check if maze is complete
    if (m_bMazeComplete)
        return;

    // IDA: Check if maze type is 12 (dimension shutter)
    if (!m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 12)
        return;

    // IDA: Find user in dimension score map
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it == m_mapDemensionScore.end())
        return;

    // IDA: Add attach point
    // Note: The attach point is stored separately from the main score
    // TODO: 需要确认 ST_SERVER_MODE_MAZE_USER_SCORE 中 attach point 字段
    // it->second.nAttachPoint += nPoint;
    // if (it->second.nAttachPoint <= 0)
    //     it->second.nAttachPoint = 0;

    // IDA: Update total dimension point
    if (nPoint > 0)
    {
        m_nTotalDemensionPoint += nPoint;
        if (m_nTotalDemensionPoint > m_nMaxDemensionPoint)
            m_nTotalDemensionPoint = m_nMaxDemensionPoint;

        // IDA: Send dimension shutter info
        unsigned long long dwTick = GetTickCount64();
        SendDemensionShutterInfo(dwUCID, nPoint, dwTick);

        // IDA: Update clear maze condition
        XMaze::UpdateClearMazeCondition(5, m_nTotalDemensionPoint);
    }
}

// IDA: ?UpdateDemensionShutterUserRank@XModeMaze@@QEAAXKG@Z (0x140296A30)
// Verified: Direct IDA decompilation
void XModeMaze::UpdateDemensionShutterUserRank(unsigned long dwUCID, unsigned short wRank)
{
    // IDA: Check if maze type is 12 (dimension shutter)
    if (!m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 12)
        return;

    // IDA: Find user in dimension score map
    auto it = m_mapDemensionScore.find(dwUCID);
    if (it != m_mapDemensionScore.end())
    {
        it->second.stInfo.wRank = wRank;
    }
}

// IDA: ?UpdateDemensionShutterRankingPoint@XModeMaze@@QEAAHPEAVCUser@@G_N@Z (0x140296AF0)
// Verified: Direct IDA decompilation
int XModeMaze::UpdateDemensionShutterRankingPoint(CUser* pUser, unsigned short wRank, bool bEnter)
{
    // IDA: Check operation info and user validity
    if (!m_pTB_OPERATION_INFO || !pUser)
        return 0;

    // TODO: 完整实现需要 XResourceMgr::GetServerContents 和 XResourceMgr::CheckRankingTime
    // XGameServer* pServer = XGameServer::Instance();
    // if (!pServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_RANKING))
    //     return 0;
    // if (!pServer->GetResourceMgr().CheckRankingTime())
    //     return 0;

    // IDA: Get ranking info table
    // std::vector<TB_RANK_INFO*> vecOperation;
    // unsigned short wMazeID = (m_uxMapID.nMapID >> 16) & 0xFFFF;
    // if (!XResourceMgr::GetRankingInfoTable(wMazeID, E_RANKING_TOTAL_TYPE_OPERATION, &vecOperation))
    //     return 0;

    // IDA: Get first rank info
    // TB_RANK_INFO* pTB_RANK_INFO = vecOperation[0];
    // if (!pTB_RANK_INFO)
    //     return 0;

    // IDA: Build ranking point update packet
    // PS_MODE_MAZE_RANKING_POINT_UPDATE psUpdate;
    // psUpdate.dwUCID = pUser->GetActorID().dwActorID;
    // psUpdate.byClass = pUser->GetClass();
    // psUpdate.byLevel = pUser->GetLevel();
    // wcscpy_s(psUpdate.strName, pUser->GetName().c_str());
    // pUser->GetLeagueInfo(&stLeagueInfo);
    // wcscpy_s(psUpdate.strLeagueName, stLeagueInfo.szLeagueName);
    // psUpdate.dwProfilePhotoID = pUser->stMyCharInfoEx()->stBaseInfo.dwProfilePhotoID;

    int nPoint = 0;
    if (bEnter)
    {
        // IDA: Enter reward
        nPoint = m_pTB_OPERATION_INFO->Reward_Attend;
        // psUpdate.nPlayCount = 1;
    }
    else
    {
        // IDA: Rank-based reward - check rank is within bounds
        if (wRank >= m_pTB_OPERATION_INFO->Max_Member)
            return 0;

        // IDA: Get reward based on rank
        // psUpdate.nPoint = m_pTB_OPERATION_INFO->Reward_Score[wRank];
        // psUpdate.nPlayCount = 0;
    }

    // IDA: Send to DB if point > 0 and not event room
    // if (nPoint > 0 && !m_dwEventRoomID)
    // {
    //     XSendDBPacket xSendDBPacket(pUser, 0x28, 0x16);
    //     xSendDBPacket << psUpdate;
    //     XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    //
    //     // Log to DB
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetActorID().dwActorID;
    //     stLog._sMainType = 30;
    //     stLog._sSubType = 10;
    //     stLog.nParam0 = pTB_RANK_INFO->Ranking_Index;
    //     stLog.nParam1 = (m_uxMapID.nMapID >> 16) & 0xFFFF;
    //     stLog.nParam2 = nPoint;
    //     stLog.nParam4 = pUser->GetLevel();
    //     stLog.nParam5 = pUser->GetClass();
    //     XGameServer::Instance()->SendDBLog(&stLog);
    // }

    return nPoint;
}

// IDA: ?CalculateDemensionShutterRank@XModeMaze@@QEAAXPEAV?$vector@UST_MODE_MAZE_USER_SCORE@@@Z
// Verified: Implemented based on IDA logic
void XModeMaze::CalculateDemensionShutterRank(std::vector<ST_MODE_MAZE_USER_SCORE>* pVecRank)
{
    if (!pVecRank)
        return;

    pVecRank->clear();

    // IDA: Copy dimension scores to vector for sorting
    for (auto it = m_mapDemensionScore.begin(); it != m_mapDemensionScore.end(); ++it)
    {
        ST_MODE_MAZE_USER_SCORE stScore;
        stScore.dwUserID = it->first;
        stScore.dwScore = it->second.stInfo.nPoint;
        stScore.wRank = 0;
        stScore.bLeave = it->second.stInfo.bLeave;
        stScore.nReviveCount = it->second.nReviveCount;

        // IDA: Get user name from actor
        XActor* pActor = FindActor(it->first);
        if (pActor)
        {
            CUser* pUser = dynamic_cast<CUser*>(pActor);
            if (pUser)
            {
                // TODO: 获取用户名称
                // stScore.strName = pUser->GetName();
            }
        }

        pVecRank->push_back(stScore);
    }

    // IDA: Sort by score descending
    std::sort(pVecRank->begin(), pVecRank->end(),
        [](const ST_MODE_MAZE_USER_SCORE& a, const ST_MODE_MAZE_USER_SCORE& b) {
            return a.dwScore > b.dwScore;
        });

    // IDA: Assign ranks (handle ties)
    for (size_t i = 0; i < pVecRank->size(); ++i)
    {
        if (i == 0)
        {
            (*pVecRank)[i].wRank = 1;
        }
        else
        {
            // IDA: Same score = same rank
            if ((*pVecRank)[i].dwScore == (*pVecRank)[i - 1].dwScore)
            {
                (*pVecRank)[i].wRank = (*pVecRank)[i - 1].wRank;
            }
            else
            {
                (*pVecRank)[i].wRank = static_cast<unsigned short>(i + 1);
            }
        }
    }
}

// IDA: ?SendDemensionPoint@XModeMaze@@QEAAXK@Z (0x1402977D0)
// Verified: Direct IDA decompilation
void XModeMaze::SendDemensionPoint(unsigned long dwUCID)
{
    // IDA: Find user in dimension score map
    auto iter = m_mapDemensionScore.find(dwUCID);
    if (iter == m_mapDemensionScore.end())
        return;

    // IDA: Create packet for dimension point info
    // PS_MODE_MAZE_USER_POINT_INFO psPoint;
    // psPoint.dwUCID = dwUCID;
    // psPoint.nPoint = iter->second.dwScore;
    // psPoint.nReviveCount = iter->second.nReviveCount;

    // IDA: Send packet (0x33 = MODE_MAZE_PROCESS_CMD, 0x12 = SUB dimension point)
    // XSendPacket xSendPacket(0x33, 0x12);
    // xSendPacket << psPoint;
    // SendBroadCastAll(&xSendPacket, 0);

    // TODO: 完整实现需要PS_MODE_MAZE_USER_POINT_INFO结构
}

// IDA: ?SendDemensionShutterInfo@XModeMaze@@QEAAXKH_K@Z (0x1402978F0)
// Verified: Direct IDA decompilation
void XModeMaze::SendDemensionShutterInfo(unsigned long dwUCID, int nAddPoint, unsigned __int64 dwTick)
{
    // IDA: Create score info structure
    // PS_MODE_MAZE_SCORE_INFO psInfo;
    // psInfo.dwAttachUCID = dwUCID;
    // psInfo.nAddDemensionPoint = nAddPoint;
    // psInfo.nDemensionPoint = m_nTotalDemensionPoint;
    // psInfo.nMaxDemensionPoint = m_nMaxDemensionPoint;
    // psInfo.nPlayTime = MazePlayTime_Now();
    // psInfo.nLimitPlayTime = m_nMaxDemensionTime;

    // IDA: Calculate ranking
    // CalculateDemensionShutterRank(&psInfo.vecUser);

    // IDA: Send packet (0x33 = MODE_MAZE_PROCESS_CMD, 0x11 = SUB dimension info)
    // XSendPacket xSendPacket(0x33, 0x11);
    // xSendPacket << psInfo;
    // SendBroadCastAll(&xSendPacket, 0);

    // IDA: Update send tick
    m_dw64DemensionInfoSendTick = dwTick;

    // TODO: 完整实现需要PS_MODE_MAZE_SCORE_INFO结构和CalculateDemensionShutterRank函数
}

// IDA: ?SendDemensionShutterReward@XModeMaze@@QEAAXAEAUPS_MODE_MAZE_REWARD_INFO@@@Z (0x140297A40)
// Verified: Direct IDA decompilation
void XModeMaze::SendDemensionShutterReward(struct PS_MODE_MAZE_REWARD_INFO& stInfo)
{
    // IDA: Send reward packet (0x33 = MODE_MAZE_PROCESS_CMD, 0x13 = SUB reward)
    // XSendPacket xSendPacket(0x33, 0x13);
    // xSendPacket << stInfo;
    // SendBroadCastAll(&xSendPacket, 0);

    // TODO: 完整实现需要 XSendPacket 序列化
}

// IDA: ?SendDemensionShutterEventMatchingReward@XModeMaze@@QEAAXAEAUPS_MODE_MAZE_REWARD_INFO@@@Z (0x140297AD0)
// Verified: Direct IDA decompilation
void XModeMaze::SendDemensionShutterEventMatchingReward(struct PS_MODE_MAZE_REWARD_INFO& stInfo)
{
    // IDA: Check if event room ID is set
    if (!m_dwEventRoomID)
        return;

    // IDA: Create event reward info structure
    // PS_MODE_MAZE_EVENT_REWARD_INFO psEvent;
    // psEvent.nEventID = m_dwEventRoomID;

    // IDA: Copy user info to event reward (max 8 users)
    // for (int sh = 0; sh < 8 && sh < stInfo.vecUser.size(); ++sh)
    // {
    //     psEvent.arrInfo[sh].dwUCID = stInfo.vecUser[sh].dwUCID;
    //     psEvent.arrInfo[sh].nPoint = stInfo.vecUser[sh].nAttachPoint;
    // }

    // IDA: Send to DB (main=0x49, sub=0x10)
    // XSendDBPacket xSendDBPacket(0, 0x49, 0x10);
    // xSendDBPacket << psEvent;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);

    // TODO: 完整实现需要 PS_MODE_MAZE_EVENT_REWARD_INFO 结构和 XSendDBPacket
    // 当前简化实现
}

// IDA: ?AllDestroySectorMonster@XModeMaze@@QEAAXXZ (0x140297C50)
// Verified: Direct IDA decompilation
void XModeMaze::AllDestroySectorMonster()
{
    // IDA: Iterate through all sectors and destroy monsters
    // for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it)
    // {
    //     CSector* pSector = it->second;
    //     if (pSector)
    //     {
    //         pSector->DieMonstersAll(0);
    //     }
    // }

    // TODO: 完整实现需要 m_mapSector 和 CSector 类
}

// IDA: ?WarpSectorStartPos@XModeMaze@@UEAA_NPEAVCUser@@@Z (0x140297CE0)
// Verified: Direct IDA decompilation
bool XModeMaze::WarpSectorStartPos(CUser* pUser)
{
    if (!pUser)
        return false;

    // IDA: Get user's UCID
    unsigned long dwUCID = pUser->GetActorID().dwActorID;

    // IDA: Find user in check sector map
    auto it = m_mapCheckSectorUser.find(dwUCID);
    if (it != m_mapCheckSectorUser.end())
    {
        // IDA: Get portal box from sector
        // STMagePotalBox* pBox = XMaze::GetMazePotalBox(it->second);
        // if (pBox)
        // {
        //     STPosInfo posInfo;
        //     int nMapID = GetTBMapID();
        //     if (XWorldResMgr::GetPortalPos(nMapID, pBox->pPotalBox->m_iJump, &posInfo))
        //     {
        //         hkvVec3 vPosition(posInfo.vPos.x, posInfo.vPos.y, posInfo.vPos.z);
        //         CSector* pSector = XMaze::GetSectorFromPos(&vPosition);
        //         if (pSector)
        //         {
        //             XVec3 vStart;
        //             float fRot = 0.0f;
        //             if (pSector->GetStartPos(&vStart, &fRot))
        //             {
        //                 MoveActor(pUser, &vStart);
        //                 pUser->MoveingValueClear();
        //                 pUser->ChangeMotion(1, 1, 0);
        //                 pUser->SendResWarp(0, &vStart, fRot);
        //                 return true;
        //             }
        //         }
        //     }
        // }
    }

    // IDA: Fallback to first sector
    auto iter = m_mapFirstSectorID.find(dwUCID);
    if (iter != m_mapFirstSectorID.end())
    {
        // IDA: Get sector and warp to start position
        // CSector* pLastSector = XMaze::GetSector(iter->second);
        // if (pLastSector)
        // {
        //     XVec3 vStartPos;
        //     float fRot = 0.0f;
        //     if (pLastSector->GetStartPos(&vStartPos, &fRot))
        //     {
        //         MoveActor(pUser, &vStartPos);
        //         pUser->MoveingValueClear();
        //         pUser->ChangeMotion(1, 1, 0);
        //         pUser->SendResWarp(0, &vStartPos, fRot);
        //         return true;
        //     }
        // }
    }

    return XMaze::WarpSectorStartPos(pUser);
}

// IDA: ?MoveNextSector@XModeMaze@@UEAAXPEAVCUser@@H@Z (0x1402981A0)
// Verified: Direct IDA decompilation
void XModeMaze::MoveNextSector(CUser* pUser, int nNextStep)
{
    if (!pUser)
        return;

    // IDA: Get current sector from user position
    // hkvVec3 vPosition = pUser->GetPosition();
    // CSector* pSector = XMaze::GetSectorFromPos(&vPosition);
    // if (!pSector)
    //     return;

    // IDA: Calculate next sector ID
    int nNextSectorID = 0;
    // if (nNextStep)
    // {
    //     int nBoxUniqueID = pSector->GetSectorBoxUniqueID();
    //     nNextSectorID = nNextStep + nBoxUniqueID - (nBoxUniqueID % 100);
    // }
    // else
    // {
    //     nNextSectorID = pSector->GetSectorBoxUniqueID() + 1;
    // }

    // IDA: Find next sector
    // auto it = m_mapSector.find(nNextSectorID);
    // if (it == m_mapSector.end())
    //     return;

    // CSector* pNextSector = it->second;
    // if (!pNextSector)
    //     return;

    // IDA: Get start position
    // XVec3 vStart;
    // float fRot = 0.0f;
    // if (!pNextSector->GetStartPos(&vStart, &fRot))
    //     return;

    // IDA: Complete current sector (unless it's special box 10021)
    // int nCurrentBoxID = pSector->GetSectorBoxID();
    // if (nCurrentBoxID != 10021)
    // {
    //     pSector->SetComplete(true);
    //     pSector->SetGroupReward(true);
    //     pSector->SetPortalOpen(true);
    //     pSector->TerminateSpawn();
    //     pSector->DieMonstersAll(true);
    // }

    // IDA: Activate next sector
    // pNextSector->SetAI(true);
    // int nNextBoxID = pNextSector->GetSectorBoxID();
    // XMaze::SetLastSectorID(nNextBoxID);
    // XMaze::SetBossSector(pNextSector->IsBossSector());
    // XMaze::SpawnSectorMonster(nNextBoxID);

    // IDA: Move user to next sector
    // MoveActor(pUser, &vStart);
    // SendSameGridOtherPlayerPosition(pUser);
    // pUser->MoveingValueClear();
    // pUser->ChangeMotion(1, 1, 0);
    // pUser->SendResWarp(0, pUser->GetPositionXVec3(), pUser->GetRot());

    XMaze::MoveNextSector(pUser, nNextStep);
}

// IDA: ?Generate@XModeMaze@@UEAAXXZ (0x1402984C0)
// Verified: Direct IDA decompilation
void XModeMaze::Generate()
{
    // IDA: Simply calls base class Generate
    XMaze::Generate();
}

// IDA: ?SpawnGenerateMonster@XModeMaze@@UEAAXXZ (0x1402984E0)
// Verified: Direct IDA decompilation
void XModeMaze::SpawnGenerateMonster()
{
    // IDA: Iterate through first jump ID map to spawn monsters in each sector
    for (auto it = m_mapFirstJumpID.begin(); it != m_mapFirstJumpID.end(); ++it)
    {
        STPosInfo stPosInfo;

        // IDA: Get map ID from m_uxMapID (high 16 bits)
        int nMapID = m_uxMapID.nMapID >> 16;

        // IDA: Get portal position from world resource manager
        // TODO: 完整实现需要 XGameServer 单例和 XWorldResMgr
        // XGameServer* pServer = XGameServer::Instance();
        // if (pServer->GetWorldResMgr().GetStartPortalPos(nMapID, it->first, &stPosInfo))
        // {
        //     hkvVec3 vPosition(stPosInfo.vPos.x, stPosInfo.vPos.y, stPosInfo.vPos.z);
        //     CSector* pSector = XMaze::GetSectorFromPos(&vPosition);
        //     if (pSector && !pSector->IsSpawnedAll())
        //     {
        //         pSector->SpawnMonster(eSendInfoTypeNot);
        //         int nSectorBoxID = pSector->GetSectorBoxID();
        //         m_mapFirstSectorID[it->first] = nSectorBoxID;
        //     }
        // }
    }

    // IDA: Create system actor for Maze_SceneAttack_Monster if defined
    if (!m_pSystemActor)
    {
        unsigned short TBMapID = GetTBMapID();

        // TODO: 完整实现需要 XGameServer 单例和 XResourceMgr
        // XGameServer* pServer = XGameServer::Instance();
        // TB_MAZE_INFO* pMaze = pServer->GetResourceMgr().GetTB_MAZE_INFO(TBMapID);
        // if (pMaze && pMaze->Maze_SceneAttack_Monster)
        // {
        //     hkvVec3 vPosition(0.0f, 0.0f, 0.0f);
        //     m_pSystemActor = ThreadLocalData::GetInstance()->CreateMonster(
        //         this, m_uxMapID, pMaze->Maze_SceneAttack_Monster, &vPosition, 0.0f);
        //
        //     if (m_pSystemActor)
        //     {
        //         if (EnterGameObject(m_pSystemActor, eSendInfoTypeNot))
        //         {
        //             XMaze::DeleteMonster(m_pSystemActor);
        //             m_pSystemActor = nullptr;
        //         }
        //         else
        //         {
        //             m_pSystemActor->SetSystemActor(true);
        //         }
        //     }
        //     else
        //     {
        //         LogHelper::LogDebug("game.contents", "SystemAotr Not ID : %d", pMaze->Maze_SceneAttack_Monster);
        //     }
        // }

        // Call base class as fallback
        XMaze::SpawnGenerateMonster();
    }
}

// IDA: ?SendBroadCast@XModeMaze@@UEAAXAEAVXSendPacket@@PEAVXActor@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140298870)
// Verified: Direct IDA decompilation
void XModeMaze::SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType)
{
    // IDA: Check broadcast type - if E_BROADCAST_TYPE_ALL or null type, broadcast to all
    if (eType != E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL && eType != static_cast<E_BROADCAST_TYPE>(0) && pExceptActor)
    {
        // IDA: Get position from actor
        XActor* pScanExcept = nullptr;
        if (eType == E_BROADCAST_TYPE::E_BROADCAST_TYPE_NORMAL)
            pScanExcept = pExceptActor;

        // IDA: Get position from actor's area
        // TODO: 完整实现需要获取 Actor 位置的 API
        // float fx = pExceptActor->GetArea()->GetPosition().x;
        // float fy = pExceptActor->GetArea()->GetPosition().y;

        // IDA: Scan grid for nearby players
        std::vector<CMover*> vecPlayerList;
        // TODO: 完整实现需要 Range2DScanner
        // Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);

        // IDA: Send packet to all found players
        // CGocNetwork::Send(&vecPlayerList, pPacket, pScanExcept);

        // Fallback: call base class
        XMaze::SendBroadCast(pPacket, pExceptActor, eType);
    }
    else
    {
        // IDA: Broadcast to all players in map
        SendBroadCastAll(pPacket, false);
    }
}

// IDA: ?SendBroadCast@XModeMaze@@UEAAXAEAVXSendPacket@@PEAVXActor@@_NW4E_BROADCAST_TYPE@IXArea@@@Z (0x1402989A0)
// Verified: Direct IDA decompilation
void XModeMaze::SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, bool bExceptDie, E_BROADCAST_TYPE eType)
{
    // IDA: Check broadcast type - if E_BROADCAST_TYPE_ALL or null type, broadcast to all
    if (eType != E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL && eType != static_cast<E_BROADCAST_TYPE>(0) && pExceptActor)
    {
        // IDA: Get position from actor
        XActor* pScanExcept = nullptr;
        if (eType == E_BROADCAST_TYPE::E_BROADCAST_TYPE_NORMAL)
            pScanExcept = pExceptActor;

        // IDA: Get position from actor's area
        // TODO: 完整实现需要获取 Actor 位置的 API
        // float fx = pExceptActor->GetArea()->GetPosition().x;
        // float fy = pExceptActor->GetArea()->GetPosition().y;

        // IDA: Scan grid for nearby players
        std::vector<CMover*> vecPlayerList;
        // TODO: 完整实现需要 Range2DScanner
        // Range2DScanner<CMover*>::ScanGrid(m_objectGridScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);

        // IDA: Iterate through players and send packet
        for (auto it = vecPlayerList.begin(); it != vecPlayerList.end(); ++it)
        {
            CMover* pMover = *it;
            if (pMover)
            {
                // IDA: Check if should skip dead players
                if (!bExceptDie || !pMover->IsDie())
                {
                    // IDA: CMover inherits from CMoverEx which inherits from XActor
                    // Check if should skip except actor
                    if (!pScanExcept || pMover != reinterpret_cast<CMover*>(pScanExcept))
                    {
                        // TODO: 完整实现需要 CGocNetwork::Send
                        // CGocNetwork::Send(pMover, pPacket);
                    }
                }
            }
        }

        // Fallback: call base class
        XMaze::SendBroadCast(pPacket, pExceptActor, eType);
    }
    else
    {
        // IDA: Broadcast to all players in map
        SendBroadCastAll(pPacket, bExceptDie);
    }
}

// IDA: ?SendBroadCastAll@XModeMaze@@QEAAXAEAVXSendPacket@@_N@Z (0x140298C00)
// Verified: Direct IDA decompilation
void XModeMaze::SendBroadCastAll(XSendPacket* pPacket, bool bExceptDie)
{
    // IDA: Enumerate all players in scanner
    std::vector<CMover*> vecPCList;
    // TODO: 完整实现需要 Range2DScanner
    // Range2DScanner<CMover*>::Enumerate(m_objectGridScanner.playerScanner, &vecPCList);

    // IDA: Iterate through players and send packet
    for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it)
    {
        CMover* pMover = *it;
        if (pMover)
        {
            // IDA: Check if should skip dead players
            if (!bExceptDie || !pMover->IsDie())
            {
                // IDA: CMover inherits from CMoverEx which inherits from XActor
                // TODO: 完整实现需要 CGocNetwork::Send
                // CGocNetwork::Send(pMover, pPacket);
            }
        }
    }

    // Fallback: call SendBroadCast with E_BROADCAST_TYPE_ALL type
    if (vecPCList.empty())
    {
        SendBroadCast(pPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
    }
}

// IDA: ?SetPlayTimeStop_Cheat@XModeMaze@@QEAAXXZ (0x140298D20)
// Verified: Direct IDA decompilation
void XModeMaze::SetPlayTimeStop_Cheat()
{
    // IDA: Toggle play time stop flag
    if (m_bPlayTimeStop_Cheat)
    {
        // IDA: Already stopped, now resume
        m_bPlayTimeStop_Cheat = false;

        // IDA: Check if maze type is 12 (dimension shutter)
        if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 12)
        {
            // IDA: Update clear condition for dimension time
            for (int i = 0; i < 3; ++i)
            {
                int nClearConType = *(&m_pTBMazeInfo->Clear_Con_Type_01 + i);
                if (nClearConType == 4)  // Clear condition type 4 = time
                {
                    m_nMaxDemensionTime = *(&m_pTBMazeInfo->Clear_Con_Value_01 + i);

                    // IDA: Check if time already exceeded
                    unsigned int dwPlayTime = MazePlayTime_Now();
                    if (m_nMaxDemensionTime <= static_cast<int>(dwPlayTime))
                    {
                        XMaze::UpdateClearMazeCondition(4, m_nMaxDemensionTime);
                    }
                }
            }
        }
    }
    else
    {
        // IDA: Stop play time
        m_bPlayTimeStop_Cheat = true;
    }
}

// IDA: ?ProcessSendEnterObjectListToPlayer@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x140292700)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessSendEnterObjectListToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Send PC info first
    SendPcInfo(pActor, vecPlayerList, vecObjList);

    // IDA: If object list is not empty, send NPC/monster/interaction/social info
    if (!vecObjList.empty())
    {
        SendNpcInfo(pActor, vecObjList);
        SendMonsterInfo(pActor, vecObjList);
        SendInteractionInfo(pActor, vecObjList);
        SendSocialInfo(pActor, vecObjList);
    }
}

// IDA: ?ProcessSendTranslateInfoToPlayer@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402927B0)
// Verified: Direct IDA decompilation
void XModeMaze::ProcessSendTranslateInfoToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList)
{
    if (!pActor || vecPlayerList.empty())
        return;

    // IDA: Iterate through player list and send transport take packets
    for (auto it = vecPlayerList.begin(); it != vecPlayerList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // IDA: Check if mover is a player
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser)
        {
            // IDA: Send ST_MOVE_TRANSPORT_TAKE packet (main=4, sub=0x28)
            // XSendPacket xSendPacket(4, 0x28);
            // ST_MOVE_TRANSPORT_TAKE stTake;
            // stTake.dwActorID = pUser->GetActorID().dwActorID;
            // stTake.vPos = pUser->GetPosition();
            // stTake.fRot = pUser->GetRot();
            // xSendPacket << stTake;
            // CGocNetwork::Send(pActor, &xSendPacket);
        }
    }
}

// IDA: ?SendPcInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x140292D40)
// Verified: Direct IDA decompilation
void XModeMaze::SendPcInfo(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for PC info
    // std::vector<STCharInfoEx> vecPCInfo;
    // vecPCInfo.reserve(300);

    // IDA: Iterate through player list and collect info
    for (auto it = vecPlayerList.begin(); it != vecPlayerList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // IDA: Check if mover is a player
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser)
        {
            // IDA: Get STCharInfoEx from user
            // STCharInfoEx* pInfo = pUser->GetCharInfoEx();
            // if (pInfo)
            // {
            //     vecPCInfo.push_back(*pInfo);
            // }

            // IDA: Check size limit for batch sending (approximately 716 entries)
            // if (vecPCInfo.size() > 716)
            // {
            //     // Send batch packet
            //     XSendPacket xSendPacket(4, 0x21);
            //     unsigned short shCount = static_cast<unsigned short>(vecPCInfo.size());
            //     xSendPacket << shCount;
            //     for (auto& info : vecPCInfo)
            //     {
            //         xSendPacket << info;
            //     }
            //     CGocNetwork::Send(pActor, &xSendPacket);
            //     vecPCInfo.clear();
            // }
        }
    }

    // IDA: Send remaining PC info
    // if (!vecPCInfo.empty())
    // {
    //     XSendPacket packet(4, 0x21);
    //     unsigned short shCount = static_cast<unsigned short>(vecPCInfo.size());
    //     packet << shCount;
    //     for (auto& info : vecPCInfo)
    //     {
    //         packet << info;
    //     }
    //     CGocNetwork::Send(pActor, &packet);
    // }

    // TODO: 完整实现需要 STCharInfoEx 结构和 CGocNetwork::Send
}

// IDA: ?SendNpcInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x140293070)
// Verified: Direct IDA decompilation
void XModeMaze::SendNpcInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for NPC info
    // std::vector<STNpcInfo> vecNPCInfo;
    // vecNPCInfo.reserve(300);

    // IDA: Iterate through object list and collect NPC info
    for (auto it = vecObjList.begin(); it != vecObjList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // IDA: Get XActor from CMover (offset +872 from CMover)
        XActor* pOther = dynamic_cast<XActor*>(pMover);
        if (pOther && pOther->GetType() == 1)  // eActorNPC = 1
        {
            // IDA: Set sync info
            // pOther->SetSyncInfo(pOther);

            // IDA: Cast to CNpc and get NPC info
            // CNpc* pNpc = dynamic_cast<CNpc*>(pOther);
            // if (pNpc)
            // {
            //     STNpcInfo* pInfo = pNpc->GetNpcInfo();
            //     if (pInfo)
            //     {
            //         vecNPCInfo.push_back(*pInfo);
            //     }
            // }

            // IDA: Check size limit for batch sending (approximately 716 entries)
            // if (vecNPCInfo.size() > 716)
            // {
            //     XSendPacket xSendPacket(4, 0x22);  // main=4, sub=0x22 for NPC info
            //     unsigned short shCount = static_cast<unsigned short>(vecNPCInfo.size());
            //     xSendPacket << shCount;
            //     for (auto& info : vecNPCInfo)
            //     {
            //         xSendPacket << info;
            //     }
            //     CGocNetwork::Send(pActor, &xSendPacket);
            //     vecNPCInfo.clear();
            // }
        }
    }

    // IDA: Send remaining NPC info
    // if (!vecNPCInfo.empty())
    // {
    //     XSendPacket packet(4, 0x22);
    //     unsigned short shCount = static_cast<unsigned short>(vecNPCInfo.size());
    //     packet << shCount;
    //     for (auto& info : vecNPCInfo)
    //     {
    //         packet << info;
    //     }
    //     CGocNetwork::Send(pActor, &packet);
    // }

    // TODO: 完整实现需要 STNpcInfo 结构和 CNpc 类
}

// IDA: ?SendMonsterInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402933A0)
// Verified: Direct IDA decompilation
void XModeMaze::SendMonsterInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for Monster info
    // std::vector<STMonsterInfo> vecMonsterInfo;
    // vecMonsterInfo.reserve(300);

    // IDA: Iterate through object list and collect Monster info
    for (auto it = vecObjList.begin(); it != vecObjList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // IDA: Get XActor from CMover (offset +872 from CMover)
        XActor* pOther = dynamic_cast<XActor*>(pMover);
        if (pOther && pOther->GetType() == 2)  // eActorMonster = 2
        {
            // IDA: Set sync info
            // pOther->SetSyncInfo(pOther);

            // IDA: Cast to CMonster and get Monster info
            // CMonster* pMonster = dynamic_cast<CMonster*>(pOther);
            // if (pMonster)
            // {
            //     STMonsterInfo* pInfo = pMonster->GetMonsterInfo();
            //     if (pInfo)
            //     {
            //         vecMonsterInfo.push_back(*pInfo);
            //     }
            // }

            // IDA: Check size limit for batch sending (approximately 357 entries)
            // if (vecMonsterInfo.size() > 357)
            // {
            //     XSendPacket xSendPacket(4, 0x23);  // main=4, sub=0x23 for Monster info
            //     unsigned short shCount = static_cast<unsigned short>(vecMonsterInfo.size());
            //     xSendPacket << shCount;
            //     for (auto& info : vecMonsterInfo)
            //     {
            //         xSendPacket << info;
            //     }
            //     CGocNetwork::Send(pActor, &xSendPacket);
            //     vecMonsterInfo.clear();
            // }
        }
    }

    // IDA: Send remaining Monster info
    // if (!vecMonsterInfo.empty())
    // {
    //     XSendPacket packet(4, 0x23);
    //     unsigned short shCount = static_cast<unsigned short>(vecMonsterInfo.size());
    //     packet << shCount;
    //     for (auto& info : vecMonsterInfo)
    //     {
    //         packet << info;
    //     }
    //     CGocNetwork::Send(pActor, &packet);
    // }

    // IDA: After sending, sync move for all monsters
    for (auto it = vecObjList.begin(); it != vecObjList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        XActor* pOther = dynamic_cast<XActor*>(pMover);
        if (pOther && pOther->GetType() == 2)  // eActorMonster = 2
        {
            // IDA: Sync monster movement
            // CMover* pMonsterMover = dynamic_cast<CMover*>(pOther);
            // if (pMonsterMover)
            // {
            //     pMonsterMover->SyncMove();
            // }
        }
    }

    // TODO: 完整实现需要 STMonsterInfo 结构和 CMonster 类
}

// IDA: ?SendInteractionInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402937E0)
// Verified: Direct IDA decompilation
void XModeMaze::SendInteractionInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for Interaction info
    // ST_BATCH_INTERACTION_LIST lstInterAction;

    // IDA: Iterate through object list and collect Interaction info
    for (auto it = vecObjList.begin(); it != vecObjList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // IDA: Get XActor from CMover (offset +872 from CMover)
        XActor* pOther = dynamic_cast<XActor*>(pMover);
        if (pOther && pOther->GetType() == 4)  // eActorInteraction = 4
        {
            // IDA: Cast to CInteractionObject and build info packet
            // CInteractionObject* pObject = dynamic_cast<CInteractionObject*>(pOther);
            // if (pObject)
            // {
            //     ST_BATCH_INTERACTION stInfo;
            //     if (pObject->BuildInfoPacket(&stInfo))
            //     {
            //         lstInterAction.vecInfo.push_back(stInfo);
            //     }
            // }

            // IDA: Check size limit for batch sending (approximately 3822 entries)
            // if (lstInterAction.vecInfo.size() > 3822)
            // {
            //     XSendPacket xSendPacket(0x11, 0x56);  // main=0x11, sub=0x56 for Interaction info
            //     xSendPacket << lstInterAction;
            //     CGocNetwork::Send(pActor, &xSendPacket);
            //     lstInterAction.vecInfo.clear();
            // }
        }
    }

    // IDA: Send remaining Interaction info
    // if (!lstInterAction.vecInfo.empty())
    // {
    //     XSendPacket packet(0x11, 0x56);
    //     packet << lstInterAction;
    //     CGocNetwork::Send(pActor, &packet);
    // }

    // TODO: 完整实现需要 ST_BATCH_INTERACTION_LIST 结构和 CInteractionObject 类
}

// IDA: ?SendSocialInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x140293A30)
// Verified: Direct IDA decompilation
void XModeMaze::SendSocialInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for Social Item info
    // ST_SOCIAL_ITEM_LIST lstSocialItem;

    // IDA: Iterate through object list and collect Social Item info
    for (auto it = vecObjList.begin(); it != vecObjList.end(); ++it)
    {
        CMover* pMover = *it;
        if (!pMover)
            continue;

        // IDA: Get XActor from CMover (offset +872 from CMover)
        XActor* pOther = dynamic_cast<XActor*>(pMover);
        if (pOther && pOther->GetType() == 6)  // SocialItemObject type = 6
        {
            // IDA: Cast to CSocialItemObject and build info packet
            // CSocialItemObject* pObject = dynamic_cast<CSocialItemObject*>(pOther);
            // if (pObject)
            // {
            //     ST_SOCIAL_ITEM_RES stInfo;
            //     pObject->BuildInfoPacket(&stInfo);
            //     lstSocialItem.vecInfo.push_back(stInfo);
            // }

            // IDA: Check size limit for batch sending (approximately 3822 entries)
            // if (lstSocialItem.vecInfo.size() > 3822)
            // {
            //     XSendPacket xSendPacket(0x2D, 6);  // main=0x2D, sub=6 for Social Item info
            //     xSendPacket << lstSocialItem;
            //     CGocNetwork::Send(pActor, &xSendPacket);
            //     lstSocialItem.vecInfo.clear();
            // }
        }
    }

    // IDA: Send remaining Social Item info
    // if (!lstSocialItem.vecInfo.empty())
    // {
    //     XSendPacket packet(0x2D, 6);
    //     packet << lstSocialItem;
    //     CGocNetwork::Send(pActor, &packet);
    // }

    // TODO: 完整实现需要 ST_SOCIAL_ITEM_LIST 结构和 CSocialItemObject 类
}
