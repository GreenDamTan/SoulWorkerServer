// WorldProcess.cpp
// CWorldProcess - World(地图/世界) 请求处理器实现
// 对齐 IDA GameServer.exe；原始归属 process/worldprocess.cpp
// (PDB cvdump MD5 3DD7FD4089D879CFE722CC61A40CA3D7)

#include "WorldProcess.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/GameSockets.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/XWorldResMgr.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocHelper.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GOComponent.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/SocialItemObject.h"
#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XGameServer/XItemFactory.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/GameServer/XGameServer/CForce.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNpcCredit.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAkashicRecord.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include <cmath>
#include <string>
#include <functional>

// ============================================================================
// GetWorldType - IDA @ 0x1406281E0
// 已精确还原 - nMapID >= 1000 时返回 nMapID/10000 - 1，否则返回 0
// ============================================================================
int GetWorldType(int nMapID) {
    if (nMapID >= 1000)
        return nMapID / 10000 - 1;
    return 0;
}

// ============================================================================
// CWorldProcess 构造 - IDA @ 0x140628210
// 已精确还原 - 基类构造 -> vtable -> SetCmd(4) -> SetName("CWorldProcess")
// ============================================================================
CWorldProcess::CWorldProcess() {
    SetCmd(4);
    SetName("CWorldProcess");
}

// ============================================================================
// CWorldProcess 析构 - IDA @ 0x1406282D0
// 已精确还原 - 仅基类析构（vtable 指回本类析构器后转基类）
// ============================================================================
CWorldProcess::~CWorldProcess() {
}

// ============================================================================
// Parse - IDA @ 0x140628300
// 已精确还原 - subcmd 分发表：
//   1/0x41 -> ReqWorldEnter（返回其结果）
//   4 -> ReqWorldVersion；6 -> ReqWorldObjectInfo；7 -> ReqWorldWarp
//   0xB -> ReqWorldCheckSector；0xD -> ReqWorldWarpOut
//   0x19 -> ReqGameOut；0x31 -> ReqWorldMoverInfo；0x33 -> ReqWorldEscape
//   0x40 -> ReqWorldDistrictTransport（返回其结果）
//   0x61 -> ReqWorldSkipInTutorial02；default -> true
// 其余分支处理器本批以 TODO stub 落地，后续批次逐个从 IDA 反编译还原。
// ============================================================================
bool CWorldProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case 1:
    case 0x41:
        return ReqWorldEnter(xPacket);
    case 4:
        ReqWorldVersion(xPacket);
        return true;
    case 6:
        ReqWorldObjectInfo(xPacket);
        return true;
    case 7:
        ReqWorldWarp(xPacket);
        return true;
    case 0xB:
        ReqWorldCheckSector(xPacket);
        return true;
    case 0xD:
        ReqWorldWarpOut(xPacket);
        return true;
    case 0x19:
        ReqGameOut(xPacket);
        return true;
    case 0x31:
        ReqWorldMoverInfo(xPacket);
        return true;
    case 0x33:
        ReqWorldEscape(xPacket);
        return true;
    case 0x40:
        return ReqWorldDistrictTransport(xPacket);
    case 0x61:
        ReqWorldSkipInTutorial02(xPacket);
        return true;
    default:
        return true;
    }
}

// ============================================================================
// ReqWorldEnterToOther - IDA @ 0x14062CE60
// 已精确还原 - pUser 空或无 Area 返回 0；
// 解析 PS_ENTER_MAP_REQ + STPosInfo + dwTargetID；
// lambda12 (0x14062D1B0) DoJob: IsLive/GetArea 门 ->
// LogDebug 进图日志 -> GetWorldType 三分支:
//   非 0 非 2 (副本区): GetStartPortalID 失败 -> LogError 961 + (4,1,0xD6DB)；
//     成功 -> SetState(eStateChangeWorld) 前直接转发 (0xF2,0x54) 控制
//   == 2 (MyRoom): nJumpID==0 时 GetStartPortalID 失败 -> LogError 948 + 0xD6DB
//   == 0 (普通区): GetTB_DISTRICT 空 -> LogError 925 + 55003；
//     nJumpID==0 时 GetStartPortalID 失败 -> LogError 935 + 0xD6DB
// 通过后: SetState(eStateChangeWorld) + 回填 dwActorID +
//   GetGOC<CGocParty>/GetGOC<CGocForce> 组团信息
//   (party: byGroupType=1; force: byGroupType=2, 均带 GetPartyID) ->
//   (0xF2,0x54) << stEnterMap << stPosInfo << dwTargetID ->
//   CGameControlSocket::SendCmd(4,1)。
// + lambda192 递减。
// ============================================================================
bool CWorldProcess::ReqWorldEnterToOther(XPacket& xPacket) {
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 0;
    if (!pUser->GetArea())
        return 0;

    PS_ENTER_MAP_REQ stEnterMap{};
    STPosInfo stPosInfo{};
    std::uint32_t dwTargetID = 0;
    xPacket >> stEnterMap;
    xPacket >> stPosInfo;
    xPacket.XParse >> dwTargetID;

    // Per IDA lambda12 (0x14062D1B0): 捕获 pUser/stEnterMap/stPosInfo/dwTargetID
    std::function<void()> func = [pUser, stEnterMap, stPosInfo, dwTargetID]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea())
            return;

        PS_ENTER_MAP_REQ _stEnterMap = stEnterMap;
        STPosInfo _stPosInfo = stPosInfo;

        LogHelper::LogDebug("game.contents",
            "ReqWorldEnterToOther[ ActorID:%d, MapID:%d ]",
            pUser->GetActorID().dwActorID, _stEnterMap.wMapID);

        int nMoveType = GetWorldType(_stEnterMap.wMapID);
        if (nMoveType) {
            if (nMoveType != 2) {
                // 副本区: 直接转发控制服
                STPosInfo vPortalPos{};
                if (XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                        _stEnterMap.wMapID, &_stEnterMap.nJumpID, &vPortalPos)) {
                    XSendPacket xSendPacket(0xF2, 0x54);
                    xSendPacket << _stEnterMap;
                    xSendPacket << _stPosInfo;
                    xSendPacket.XParse << dwTargetID;
                    XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1);
                } else {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnterToOther error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, _stEnterMap.wMapID, 961);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser),
                        4, 1, 0xD6DB);
                }
                return;
            }
            // MyRoom: nJumpID 为 0 时查起始传送门
            STPosInfo vRoomPos{};
            if (!_stEnterMap.nJumpID) {
                if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                        _stEnterMap.wMapID, &_stEnterMap.nJumpID, &vRoomPos)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnterToOther error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d ) ",
                        pUser->GetActorID().dwActorID, _stEnterMap.wMapID, 948);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser),
                        4, 1, 0xD6DB);
                    return;
                }
            }
        } else {
            // 普通区: 需有 TB_DISTRICT 表
            if (!XGameServer::Instance()->GetResourceMgr().GetTB_DISTRICT(
                    _stEnterMap.wMapID)) {
                LogHelper::LogError("game.contents",
                    "ReqWorldEnterToOther error - No Table TB_DISTRICT[ ActorID:%d, MapID:%d ] ( %d ) ",
                    pUser->GetActorID().dwActorID, _stEnterMap.wMapID, 925);
                pUser->SendErrorMessage(4, 1, 55003);
                return;
            }
            STPosInfo posInfo{};
            if (!_stEnterMap.nJumpID) {
                if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                        _stEnterMap.wMapID, &_stEnterMap.nJumpID, &posInfo)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnterToOther error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, _stEnterMap.wMapID, 935);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser),
                        4, 1, 0xD6DB);
                    return;
                }
            }
        }

        pUser->SetState(eStateChangeWorld);
        _stEnterMap.dwActorID = pUser->GetActorID().dwActorID;

        CGocParty* pParty = pUser->GetGOC<CGocParty>();
        CGocForce* pForce = pUser->GetGOC<CGocForce>();
        if (!pParty || !pParty->IsParty()) {
            if (pForce && pForce->IsParty()) {
                _stEnterMap.stPartyInfo.byGroupType = 2;
                _stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
            }
        } else {
            _stEnterMap.stPartyInfo.byGroupType = 1;
            _stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
        }

        XSendPacket packet(0xF2, 0x54);
        packet << _stEnterMap;
        packet << _stPosInfo;
        packet.XParse << dwTargetID;
        XGameServer::Instance()->GetControlSocket().SendCmd(
            &packet, pUser, 4, 1);
    };

    if (!pUser)
        return 0;
    if (!pUser->GetArea())
        return 0;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// 以下处理器为类骨架占位；本批次尚未反编译，逐个从 IDA 还原时替换。
// ============================================================================

// ============================================================================
// ReqWorldEnter - IDA @ 0x1406284D0
// 已精确还原 - 解析 PS_ENTER_MAP_REQ；
// lambda14 (ctor 0x14062CD00) 捕获 [CUser*&, PS_ENTER_MAP_REQ const&, CWorldProcess*]；
// pUser 空或无 Area 返回 0；二次校验后 IncrementJobCount ->
// DoJob(GetMapInsID) + lambda192 递减 DoJob -> return 1。
// lambda14 巨型体（见下方 lambda 体展开）：
//   [门禁] pUser 空或 IsLive/GetArea 失败返回；IsBit_OR(0xE0) 返回；
//     IsMatching -> (4,1,55003)；
//   [社交卡匹配] District 区域且 SocialUseID 对应 GetSocialType()==3 ->
//     LogError "[WORLD] Failed ReqWorldEnter - Play social cardmatch" + (4,1,55093)；
//   [交易中] GetTradeState -> (4,1,55003)；
//   [控制服不可发] IsCanSend 失败 -> LogError + (4,1,50010)；
//   [同图检查] 当前 MapID==请求 MapID -> 返回；
//   [21111->21112 引导] ST_LOG_GAME(6,10) 记录 + Episode 0x186A2 强制完成链
//     (ResetQuestAll -> AcceptQuestByForce -> CompleteQuestByForce)；
//   [通用] stEnterMap.dwActorID/dwUAID 回填 + LogDebug 进图日志 +
//     AllHelperRelease + Roguelike 后图 (GetRoguelikeNextMap, 失败 LogError 221)；
//   [MyRoom 分支 nMoveType==2] CheckEnterMapPortalPos ->
//     nJumpID=GetJumpID；30031 Thing 事件 (IsEnterTheThingEvent/GetEventJumpID)；
//     Area 空或 SoulWeapon 未装备 -> (4,1,0xD6DB)/(4,1,0xD6E8)；
//   [副本分支 nMoveType!=0!=2] GetStartPortalID 失败 -> LogError 348 + 0xD6DB；
//     SoulWeapon -> 0xD6E8；TB_MAZE_INFO 空 -> 0xD6E0；CheckMazeOpenTime ->
//     0xD714；CheckAdmissionMember -> LogError 386 + nErrorCode；
//     Req_Min_Lv (CGocAttribute::GetState) -> LogError 396 + 0xD6E4；
//     IsClearMaze -> LogError + 55003；组队 (0xF2,0x32) 直接转发；
//     Force 组队 (0xF2,0x32)；Roguelike 地图 (13/19) + RoguelikeState ->
//     (0xF2,0x77)；CheckEnterMapPortalPos(451)；NeedQuest_ID ->
//     FindEpisode/IsCompleteEpisode -> LogError 461 + 0xD6FD；
//     CheckMazeEnterCount -> LogError 471 + nErrorID(55053)；
//     Fatigue_Point CanUseFP -> 0xD704(带UCID)；NeedItem_ID 物品清点
//     (GetSameItems 循环减 GetCount) 失败 -> LogError 517 + (4,0x41,0xD6FB,ItemID)；
//   [普通区分支 nMoveType==0] CheckEnterMapPortalPos(240) ->
//     Maze 区域 (GetMazeType 13/19 或 wMapID==0) GetExitDistrictID ->
//     GetRestartState -> LogError + 0xD71C ->
//     [LABel_182 公共尾] SetState(eStateChangeWorld)+SetLogChangeMap(1)；
//     wMapID/10000==2 -> ST_CREATE_MAZE (0xF2,0x21) (bNoMoveServer 时 nCreateType=2)；
//     否则 (0xF2,0x31) + party/force 组团 (byGroupType 1/2) +
//       Force_Use==1 时脱离 Force；
//     SendCmd 失败 -> ClearState(eStateChangeWorld)；
//   [普通区非 Maze] TB_DISTRICT 空 -> LogError 286 + 55003；
//     NeedQuest_ID 未完成/未接 -> LogError 296 + 55037 ->
//     LABel_182 公共尾；
//   [普通区退出口] wMapID==0 且 WorldType==2 (MyRoom 检查) ->
//     GetEnterDistrictPos + GetStartPortalID 失败 -> LogError 277 + 0xD6DB ->
//     TB_DISTRICT/NeedQuest 检查链 -> LABel_182。
// ============================================================================
bool CWorldProcess::ReqWorldEnter(XPacket& xPacket) {
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    PS_ENTER_MAP_REQ stRecvEnterMap{};
    xPacket >> stRecvEnterMap;

    // Per IDA lambda14 (0x14062CD00 ctor): 捕获 pUser / stEnterMap / this
    CWorldProcess* pProcess = this;
    std::function<void()> func = [pUser, stRecvEnterMap, pProcess]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea())
            return;
        if (pUser->IsBit_OR(static_cast<XClient::E_NET_STATE>(0xE0)))
            return;
        if (pUser->IsMatching()) {
            pUser->SendErrorMessage(4, 1, 55003);
            return;
        }

        PS_ENTER_MAP_REQ stEnterMap = stRecvEnterMap;

        // [社交卡匹配] District 内使用社交道具且类型 3 (卡牌匹配) 时禁止
        XArea* pArea = pUser->GetArea();
        XDistrict* pDistrict = dynamic_cast<XDistrict*>(pArea);
        if (pDistrict) {
            std::uint32_t dwSocialUseID = pUser->GetSocialUseID();
            if (dwSocialUseID) {
                CSocialItemObject* pSocialObject =
                    pDistrict->FindSocialItemObject(dwSocialUseID);
                if (pSocialObject && pSocialObject->GetSocialType() == 3) {
                    LogHelper::LogError("game.contents",
                        "[WORLD] Failed ReqWorldEnter - Play social cardmatch[UCID:%d]",
                        pUser->GetActorID().dwActorID);
                    pUser->SendErrorMessage(4, 1, 55093);
                    return;
                }
            }
        }

        // [交易中禁止]
        CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
        if (pInven && pInven->GetTradeState()) {
            pUser->SendErrorMessage(4, 1, 55003);
            return;
        }

        // [控制服不可发送]
        if (!CGameControlSocket::IsCanSend(
                &XGameServer::Instance()->GetControlSocket())) {
            LogHelper::LogError("game.contents",
                "[WORLD] Failed ReqWorldEnter - CONTROL_SOCKET.IsCanSend( ) %d",
                pUser->GetActorID().dwActorID);
            pUser->SendErrorMessage(4, 1, 50010);
            return;
        }

        // [同图检查]
        int nCurMapID = pUser->GetMapInsID().nMapID;
        if (nCurMapID == static_cast<int>(stEnterMap.wMapID))
            return;

        // [21111 -> 21112 教学引导: 强制完成 Episode 0x186A2]
        if (nCurMapID == 21111 && stEnterMap.wMapID == 21112) {
            XMaze* pMaze = dynamic_cast<XMaze*>(pUser->GetArea());
            if (pMaze) {
                ST_LOG_GAME stLog{};
                stLog._nUAID = static_cast<int>(pUser->GetUAID());
                stLog._nUCID = CQuestCondition::GetQuestID(
                    pUser->GetActorID().dwActorID);
                stLog._sMainType = 6;
                stLog._sSubType = 10;
                stLog.nParam0 = pMaze->GetTBMapID();
                stLog.nParam1 = pMaze->GetLastSectorID();
                stLog.nParam2 = (pMaze->GetTBMapID() == 21111) ? 1 : 2;
                stLog.nParam3 = static_cast<int>(pMaze->MazePlayTime_Now());
                stLog.nParam4 = 1;
                XGameServer::Instance()->SendDBLog(stLog);
            }
            CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
            if (pQuest && !pQuest->IsCompleteEpisode(0x186A2)) {
                pQuest->ResetQuestAll();
                pQuest->AcceptQuestByForce(0x186A2);
                pQuest->CompleteQuestByForce(0x186A2);
            }
        }

        // [通用回填与日志]
        stEnterMap.dwActorID = pUser->GetActorID().dwActorID;
        stEnterMap.dwUAID = pUser->GetUAID();
        LogHelper::LogDebug("game.contents", "ReqWorldEnter[ ActorID:%d, MapID:%d ]",
            pUser->GetActorID().dwActorID, stEnterMap.wMapID);

        CGocHelper* pHelper = pUser->GetGOC<CGocHelper>();
        if (pHelper)
            pHelper->AllHelperRelease();

        bool bNoMoveServer = false;

        // [Roguelike 后续地图]
        XMaze* pMazeCur = dynamic_cast<XMaze*>(pUser->GetArea());
        if (pMazeCur) {
            int nNextMapID = 0;
            if (!pMazeCur->GetRoguelikeNextMap(
                    pUser, nNextMapID, stEnterMap.stEnterDistrictPos)) {
                LogHelper::LogError("game.contents",
                    "ReqWorldEnter error -  pMaze->GetRoguelikeNextMap( nNextMapID ) == false [ ActorID:%d] ( %d )",
                    pUser->GetActorID().dwActorID, 221);
                CGocNetwork::SendErrorMessage(
                    static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                return;
            }
            if (nNextMapID > 0) {
                stEnterMap.wMapID = static_cast<std::uint16_t>(nNextMapID);
                bNoMoveServer = true;
            }
        }

        int nMoveType = GetWorldType(stEnterMap.wMapID);
        if (nMoveType != 0) {
            if (nMoveType == 2) {
                // === MyRoom 分支 ===
                if (XGameServer::Instance()->GetWorldResMgr().CheckEnterMapPortalPos(
                        pUser, stEnterMap.wMapID, stEnterMap.nPortalID)) {
                    int nTBMapID = pUser->GetArea()->GetTBMapID();
                    stEnterMap.nJumpID = XGameServer::Instance()
                        ->GetWorldResMgr().GetJumpID(nTBMapID, stEnterMap.nPortalID);
                    if (stEnterMap.wMapID == 30031) {
                        if (!pProcess->IsEnterTheThingEvent()) {
                            LogHelper::LogError("game.contents",
                                "ReqWorldEnter error - Failed enter Thething raid[UCID:%d, MapID:%d] ( %d )",
                                pUser->GetActorID().dwActorID,
                                stEnterMap.wMapID, 321);
                            return;
                        }
                        stEnterMap.nJumpID = pProcess->GetEventJumpID(
                            static_cast<int>(
                                (pUser->GetMapInsID().nMapID << 16) >> 48));
                    }
                    if (!pUser->GetArea()) {
                        CGocNetwork::SendErrorMessage(
                            static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                        return;
                    }
                    if (pUser->stMyCharInfoEx()->stSoulWeapon.dwItemID == -1) {
                        CGocNetwork::SendErrorMessage(
                            static_cast<CMover*>(pUser), 4, 1, 0xD6E8);
                        return;
                    }
                } else {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - CheckEnterMapPortalPos [ ActorID:%d, TBID:%d, POTAL:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID,
                        stEnterMap.nPortalID, 308);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                    return;
                }
            } else {
                // === 副本(迷宫)分支 ===
                STPosInfo vPortalPos{};
                if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                        stEnterMap.wMapID, &stEnterMap.nJumpID, &vPortalPos)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 348);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                    return;
                }
                if (pUser->stMyCharInfoEx()->stSoulWeapon.dwItemID == -1) {
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6E8);
                    return;
                }
                TB_MAZE_INFO* pTBMazeInfo = XGameServer::Instance()
                    ->GetResourceMgr().GetTB_MAZE_INFO(stEnterMap.wMapID);
                if (!pTBMazeInfo) {
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6E0);
                    return;
                }
                if (!XGameServer::Instance()->GetResourceMgr().CheckMazeOpenTime(
                        stEnterMap.wMapID)) {
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD714);
                    return;
                }
                int nErrorCode = 0;
                if (!pProcess->CheckAdmissionMember(
                        pTBMazeInfo->Admission_Member, true, nErrorCode)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - Wrong Admission when enter maze[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 386);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, nErrorCode);
                    return;
                }
                if (pTBMazeInfo->Req_Min_Lv) {
                    CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
                    int nReqMinLv = pTBMazeInfo->Req_Min_Lv;
                    // Per IDA: 0x1401AD9B0 (folded COMDAT: CGameWorldMode::GetState
                    // == CGocAttribute::GetLevel, 同一 mov eax,[rcx+N] 体)
                    if (pAttr && nReqMinLv > pAttr->GetLevel()) {
                        LogHelper::LogError("game.contents",
                            "ReqWorldEnter error - wrong Level[ ActorID:%d, TBID:%d ] ( %d )",
                            pUser->GetActorID().dwActorID, stEnterMap.wMapID, 396);
                        CGocNetwork::SendErrorMessage(
                            static_cast<CMover*>(pUser), 4, 1, 0xD6E4);
                        return;
                    }
                }
                CGocRecode* pRecode = pUser->GetGOC<CGocRecode>();
                if (!pRecode
                    || !pRecode->IsClearMaze(pTBMazeInfo->Check_Clear_Maze)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - IsClearMaze ( %d ) [ ActorID:%d ]",
                        pTBMazeInfo->Check_Clear_Maze, pUser->GetActorID().dwActorID);
                    pUser->SendErrorMessage(4, 1, 55003);
                    return;
                }
                // [组队中: 直接转发控制服 (0xF2,0x32)]
                CGocParty* pParty = pUser->GetGOC<CGocParty>();
                if (pParty && pParty->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 1;
                    stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
                    XSendPacket xSendPacket(0xF2, 0x32);
                    xSendPacket << stEnterMap;
                    XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1);
                    return;
                }
                // [Force 组队中: 直接转发控制服 (0xF2,0x32)]
                CGocForce* pForce = pUser->GetGOC<CGocForce>();
                if (pForce && pForce->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 2;
                    stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
                    XSendPacket xSendPacket(0xF2, 0x32);
                    xSendPacket << stEnterMap;
                    XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1);
                    return;
                }
                // [Roguelike 迷宫 (类型 13/19) 且处于 Roguelike 状态: (0xF2,0x77)]
                bool bRoguelikeState = false;
                CGocEntity* pEntityChk = pUser->GetGOC<CGocEntity>();
                if (pEntityChk)
                    bRoguelikeState = pEntityChk->IsRoguelikeState();
                if (bRoguelikeState
                    && (pTBMazeInfo->Maze_Type == 13
                        || pTBMazeInfo->Maze_Type == 19)) {
                    XSendPacket xSendPacket(0xF2, 0x77);
                    xSendPacket << stEnterMap;
                    XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1);
                    return;
                }
                if (!XGameServer::Instance()->GetWorldResMgr()
                        .CheckEnterMapPortalPos(
                            pUser, stEnterMap.wMapID, stEnterMap.nPortalID)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - CheckEnterMapPortalPos [ ActorID:%d, TBID:%d, POTAL:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID,
                        stEnterMap.nPortalID, 451);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                    return;
                }
                if (pTBMazeInfo->NeedQuest_ID) {
                    CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
                    if (!pQuest
                        || (!pQuest->FindEpisode(pTBMazeInfo->NeedQuest_ID)
                            && !pQuest->IsCompleteEpisode(
                                   pTBMazeInfo->NeedQuest_ID))) {
                        LogHelper::LogError("game.contents",
                            "ReqWorldEnter error - No Have Quest To Enter Maze[ ActorID:%d ] ( %d )",
                            pUser->GetActorID().dwActorID, 461);
                        CGocNetwork::SendErrorMessage(
                            static_cast<CMover*>(pUser), 4, 1, 0xD6FD);
                        return;
                    }
                }
                int nErrotID = 55053;
                if (!pUser->CheckMazeEnterCount(pTBMazeInfo, nErrotID)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - Enter Limit Count[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 471);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, nErrotID);
                    return;
                }
                if (pTBMazeInfo->Fatigue_Point) {
                    CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
                    if (pAttr && !pAttr->CanUseFP(pTBMazeInfo->Fatigue_Point)) {
                        CGocNetwork::SendErrorMessage(
                            static_cast<CMover*>(pUser), 4, 1, 0xD704,
                            pUser->GetActorID().dwActorID);
                        return;
                    }
                }
                // [NeedItem_ID 物品持有清点]
                bool bCheckItem = false;
                if (pTBMazeInfo->NeedItem_ID && !bNoMoveServer) {
                    TB_ITEM* pTBItem = XGameServer::Instance()
                        ->GetResourceMgr().GetTB_ITEM(pTBMazeInfo->NeedItem_ID);
                    if (pTBItem) {
                        TB_ITEM_CLASSIFY* pTBClassify =
                            XGameServer::Instance()->GetResourceMgr()
                                .GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
                        if (pTBClassify) {
                            CGocInventory* pInvenPtr = pUser->GetGOC<CGocInventory>();
                            if (pInvenPtr) {
                                XBaseInventory* pInventory =
                                    pInvenPtr->GetTBInvenPtr(
                                        pTBClassify->Item_Inven_Type);
                                if (pInventory) {
                                    int nNeedCount = pTBMazeInfo->NeedItem_Value;
                                    std::vector<std::shared_ptr<CItem>> vecItems;
                                    pInventory->GetSameItems(
                                        pTBMazeInfo->NeedItem_ID, &vecItems);
                                    for (auto& pItem : vecItems) {
                                        if (pItem) {
                                            nNeedCount -= pItem->GetCount();
                                            if (nNeedCount <= 0) {
                                                bCheckItem = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (!bCheckItem
                    && !(pTBMazeInfo->NeedItem_ID == 0 || bNoMoveServer)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - No Item[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 517);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 0x41, 0xD6FB,
                        pTBMazeInfo->NeedItem_ID);
                    return;
                }
            }
        } else {
            // === 普通区(频道图)分支 ===
            if (!XGameServer::Instance()->GetWorldResMgr().CheckEnterMapPortalPos(
                    pUser, stEnterMap.wMapID, stEnterMap.nPortalID)) {
                LogHelper::LogError("game.contents",
                    "ReqWorldEnter error - CheckEnterMapPortalPos [ ActorID:%d, TBID:%d, POTAL:%d ] ( %d )",
                    pUser->GetActorID().dwActorID, stEnterMap.wMapID,
                    stEnterMap.nPortalID, 240);
                CGocNetwork::SendErrorMessage(
                    static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                return;
            }
            XMaze* pMazeArea = dynamic_cast<XMaze*>(pUser->GetArea());
            if (pMazeArea) {
                // [迷宫区域内退出]
                std::uint8_t byMazeType = pMazeArea->GetMazeType();
                if (byMazeType == 13 || byMazeType == 19 || !stEnterMap.wMapID) {
                    std::uint32_t dwUCID = CQuestCondition::GetQuestID(
                        pUser->GetActorID().dwActorID);
                    if (!pMazeArea->GetExitDistrictID(
                            dwUCID, stEnterMap.wMapID, stEnterMap.nJumpID,
                            stEnterMap.vNextPos)) {
                        return;
                    }
                }
                if (pMazeArea->GetRestartState(pUser)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - GetRestartState() == MAZE_RESTART_STATE_AGREE [ ActorID:%d, TBID:%d, POTAL:%d ]",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID,
                        stEnterMap.nPortalID);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD71C);
                    return;
                }

                // ---- LABEL_182: 公共进图发送尾 ----
                pUser->SetState(XClient::eStateChangeWorld);
                pUser->SetLogChangeMap(true);

                if (stEnterMap.wMapID / 10000 == 2) {
                    // MyRoom 系地图: 走创建迷宫链 (0xF2,0x21)
                    ST_CREATE_MAZE stMaze{};
                    stMaze.stPartyInfo = stEnterMap.stPartyInfo;
                    stMaze.dwUserID = CQuestCondition::GetQuestID(
                        pUser->GetActorID().dwActorID);
                    stMaze.dwUAID = pUser->GetUAID();
                    stMaze.wReqMapID = stEnterMap.wMapID;
                    stMaze.nJumpID = stEnterMap.nJumpID;
                    stMaze.stEnterDistrictPos = stEnterMap.stEnterDistrictPos;
                    if (bNoMoveServer)
                        stMaze.nCreateType = 2;
                    ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
                    stMemberInfo.dwMember = stMaze.dwUserID;
                    CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
                    if (pEntity)
                        stMemberInfo.nState = pEntity->GetNetCafe() ? 1 : 0;
                    stMaze.vecEnterMember.push_back(stMemberInfo);
                    XSendPacket packet(0xF2, 0x21);
                    packet << stMaze;
                    if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                            &packet, pUser, 4, 1))
                        pUser->ClearState(XClient::eStateChangeWorld);
                    return;
                }

                // 普通迷宫退出: 组团信息回填 + (0xF2,0x31)
                stEnterMap.dwActorID = pUser->GetActorID().dwActorID;
                CGocParty* pParty = pUser->GetGOC<CGocParty>();
                CGocForce* pForce = pUser->GetGOC<CGocForce>();
                if (pParty && pParty->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 1;
                    stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
                } else if (pForce && pForce->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 2;
                    stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
                    TB_DISTRICT* pTBDistrict = XGameServer::Instance()
                        ->GetResourceMgr().GetTB_DISTRICT(stEnterMap.wMapID);
                    if (pTBDistrict && pTBDistrict->Force_Use == 1) {
                        stEnterMap.stPartyInfo.byGroupType = 0;
                        stEnterMap.stPartyInfo.nID = 0;
                        pForce->Leave();
                    }
                }
                XSendPacket xSendPacket(0xF2, 0x31);
                xSendPacket << stEnterMap;
                if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1))
                    pUser->ClearState(XClient::eStateChangeWorld);
                return;
            }

            if (stEnterMap.wMapID != 0) {
                TB_DISTRICT* pTBDistrict = XGameServer::Instance()
                    ->GetResourceMgr().GetTB_DISTRICT(stEnterMap.wMapID);
                if (!pTBDistrict) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - No Table TB_DISTRICT[ ActorID:%d, MapID:%d ] ( %d ) ",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 286);
                    pUser->SendErrorMessage(4, 1, 55003);
                    return;
                }
                if (pTBDistrict->NeedQuest_ID) {
                    CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
                    if (!pQuest
                        || (!pQuest->IsCompleteEpisode(pTBDistrict->NeedQuest_ID)
                            && !pQuest->FindEpisode(pTBDistrict->NeedQuest_ID))) {
                        LogHelper::LogError("game.contents",
                            "ReqWorldEnter error - Invalid quest condition[ ActorID:%d, MapID:%d ] ( %d )",
                            pUser->GetActorID().dwActorID, stEnterMap.wMapID, 296);
                        pUser->SendErrorMessage(4, 1, 55037);
                        return;
                    }
                }
                // ---- LABEL_182: 公共进图发送尾 (普通区) ----
                pUser->SetState(XClient::eStateChangeWorld);
                pUser->SetLogChangeMap(true);

                if (stEnterMap.wMapID / 10000 == 2) {
                    ST_CREATE_MAZE stMaze{};
                    stMaze.stPartyInfo = stEnterMap.stPartyInfo;
                    stMaze.dwUserID = CQuestCondition::GetQuestID(
                        pUser->GetActorID().dwActorID);
                    stMaze.dwUAID = pUser->GetUAID();
                    stMaze.wReqMapID = stEnterMap.wMapID;
                    stMaze.nJumpID = stEnterMap.nJumpID;
                    stMaze.stEnterDistrictPos = stEnterMap.stEnterDistrictPos;
                    if (bNoMoveServer)
                        stMaze.nCreateType = 2;
                    ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
                    stMemberInfo.dwMember = stMaze.dwUserID;
                    CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
                    if (pEntity)
                        stMemberInfo.nState = pEntity->GetNetCafe() ? 1 : 0;
                    stMaze.vecEnterMember.push_back(stMemberInfo);
                    XSendPacket packet(0xF2, 0x21);
                    packet << stMaze;
                    if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                            &packet, pUser, 4, 1))
                        pUser->ClearState(XClient::eStateChangeWorld);
                    return;
                }

                stEnterMap.dwActorID = pUser->GetActorID().dwActorID;
                CGocParty* pParty = pUser->GetGOC<CGocParty>();
                CGocForce* pForce = pUser->GetGOC<CGocForce>();
                if (pParty && pParty->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 1;
                    stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
                } else if (pForce && pForce->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 2;
                    stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
                    TB_DISTRICT* pTBDistrictForce = XGameServer::Instance()
                        ->GetResourceMgr().GetTB_DISTRICT(stEnterMap.wMapID);
                    if (pTBDistrictForce && pTBDistrictForce->Force_Use == 1) {
                        stEnterMap.stPartyInfo.byGroupType = 0;
                        stEnterMap.stPartyInfo.nID = 0;
                        pForce->Leave();
                    }
                }
                XSendPacket xSendPacket(0xF2, 0x31);
                xSendPacket << stEnterMap;
                if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1))
                    pUser->ClearState(XClient::eStateChangeWorld);
                return;
            }

            // [wMapID==0: 取上次进入位置]
            int nWorldTypeArea = 0;
            XArea* pAreaChk = pUser->GetArea();
            if (pAreaChk)
                nWorldTypeArea = pAreaChk->GetWorldType();
            if (nWorldTypeArea != 2) {
                STPosInfo posInfo{};
                pUser->GetEnterDistrictPos(posInfo);
                stEnterMap.wMapID = posInfo.sWorldID;
                if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                        posInfo.sWorldID, &stEnterMap.nJumpID, &posInfo)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 277);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                    return;
                }
                TB_DISTRICT* pTBDistrict = XGameServer::Instance()
                    ->GetResourceMgr().GetTB_DISTRICT(stEnterMap.wMapID);
                if (!pTBDistrict) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnter error - No Table TB_DISTRICT[ ActorID:%d, MapID:%d ] ( %d ) ",
                        pUser->GetActorID().dwActorID, stEnterMap.wMapID, 286);
                    pUser->SendErrorMessage(4, 1, 55003);
                    return;
                }
                if (pTBDistrict->NeedQuest_ID) {
                    CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
                    if (!pQuest
                        || (!pQuest->IsCompleteEpisode(pTBDistrict->NeedQuest_ID)
                            && !pQuest->FindEpisode(pTBDistrict->NeedQuest_ID))) {
                        LogHelper::LogError("game.contents",
                            "ReqWorldEnter error - Invalid quest condition[ ActorID:%d, MapID:%d ] ( %d )",
                            pUser->GetActorID().dwActorID, stEnterMap.wMapID, 296);
                        pUser->SendErrorMessage(4, 1, 55037);
                        return;
                    }
                }
                // ---- LABEL_182: 公共进图发送尾 (回退位置) ----
                pUser->SetState(XClient::eStateChangeWorld);
                pUser->SetLogChangeMap(true);
                stEnterMap.dwActorID = pUser->GetActorID().dwActorID;
                CGocParty* pParty = pUser->GetGOC<CGocParty>();
                CGocForce* pForce = pUser->GetGOC<CGocForce>();
                if (pParty && pParty->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 1;
                    stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
                } else if (pForce && pForce->IsParty()) {
                    stEnterMap.stPartyInfo.byGroupType = 2;
                    stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
                }
                XSendPacket xSendPacket(0xF2, 0x31);
                xSendPacket << stEnterMap;
                if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                        &xSendPacket, pUser, 4, 1))
                    pUser->ClearState(XClient::eStateChangeWorld);
                return;
            }
            LogHelper::LogError("game.contents",
                "ReqWorldEnter error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                pUser->GetActorID().dwActorID, stEnterMap.wMapID, 277);
            CGocNetwork::SendErrorMessage(
                static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
            return;
        }

        // ---- 副本/MyRoom 分支通过校验后的公共进图发送尾 (LABEL_181 -> LABEL_182) ----
        pUser->SetState(XClient::eStateChangeWorld);
        pUser->SetLogChangeMap(true);

        if (stEnterMap.wMapID / 10000 == 2) {
            ST_CREATE_MAZE stMaze{};
            stMaze.stPartyInfo = stEnterMap.stPartyInfo;
            stMaze.dwUserID = CQuestCondition::GetQuestID(
                pUser->GetActorID().dwActorID);
            stMaze.dwUAID = pUser->GetUAID();
            stMaze.wReqMapID = stEnterMap.wMapID;
            stMaze.nJumpID = stEnterMap.nJumpID;
            stMaze.stEnterDistrictPos = stEnterMap.stEnterDistrictPos;
            if (bNoMoveServer)
                stMaze.nCreateType = 2;
            ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
            stMemberInfo.dwMember = stMaze.dwUserID;
            CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
            if (pEntity)
                stMemberInfo.nState = pEntity->GetNetCafe() ? 1 : 0;
            stMaze.vecEnterMember.push_back(stMemberInfo);
            XSendPacket packet(0xF2, 0x21);
            packet << stMaze;
            if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                    &packet, pUser, 4, 1))
                pUser->ClearState(XClient::eStateChangeWorld);
            return;
        }

        stEnterMap.dwActorID = pUser->GetActorID().dwActorID;
        CGocParty* pParty = pUser->GetGOC<CGocParty>();
        CGocForce* pForce = pUser->GetGOC<CGocForce>();
        if (pParty && pParty->IsParty()) {
            stEnterMap.stPartyInfo.byGroupType = 1;
            stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
        } else if (pForce && pForce->IsParty()) {
            stEnterMap.stPartyInfo.byGroupType = 2;
            stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
        }
        XSendPacket xSendPacket(0xF2, 0x31);
        xSendPacket << stEnterMap;
        if (!XGameServer::Instance()->GetControlSocket().SendCmd(
                &xSendPacket, pUser, 4, 1))
            pUser->ClearState(XClient::eStateChangeWorld);
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192: 任务完成递减计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldVersion - IDA @ 0x14062F150
// 已精确还原 - pUser 空或无 Area 返回 0；不解析包体；
// lambda18 (0x14062F300) DoJob: IsLive/GetArea 门(两重) ->
// GetTB_VERSION(1) 空: 不发送；
// 否则 (4,4) << GetRevision() << MainVer << SubVer << DataVer ->
// CGocNetwork::Send。
// + lambda192 递减。
// ============================================================================
bool CWorldProcess::ReqWorldVersion(XPacket& xPacket) {
    (void)xPacket;
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 0;
    if (!pUser->GetArea())
        return 0;

    pUser->IncrementJobCount();

    // Per IDA lambda18 (0x14062F300)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;

            XSendPacket xSendRet(4, 4);
            TB_VERSION* pVersion =
                XGameServer::Instance()->GetResourceMgr().GetTB_VERSION(1);
            if (pVersion) {
                int nRevision = XGameServer::Instance()->GetResourceMgr().GetRevision();
                xSendRet.XParse << nRevision;
                xSendRet.XParse << pVersion->MainVer;
                xSendRet.XParse << pVersion->SubVer;
                xSendRet.XParse << pVersion->DataVer;
                CGocNetwork::Send(pUser, xSendRet);
            }
        });

    // Per IDA lambda192: 任务完成递减计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldObjectInfo - IDA @ 0x14062B250 (publics RVA 0x62A250 为重复映射，
// IDA 函数表真体边界 0x14062B250-0x14062B3EF)
// 已精确还原 - 客户端加载完成进图收尾通知。
// GetClientPtr 空 -> 0; GetArea 空 -> 0; IncrementJobCount ->
// DoJob(GetMapInsID, lambda2) -> DoJob(GetMapInsID, lambda192 递减) -> 1。
// lambda2 体 (??R_lambda2_ @ 0x14062B3F0, 捕获 [pUser]):
//   四重门 (pUser/IsLive/GetArea) ->
//   SetClientLoadComplete(1) + SetLogChangeMap(0) ->
//   GetArea()->SetUser(pUser) (vtable+136) ->
//   GetArea()->IsDistrict (vtable+192) -> XDistrict::SetObjectInfoReq(pUser)
//   (0x1402D0FE0: m_listWaitForRecvInfo.push_back) ->
//   GetGOC<CGocAkashicRecord>: SetUserLoad(true) (COMDAT 0x14019C1D0 与
//     CItem::SetUseCount 共体) + ThinkAkashicPassive ->
//   GetGOC<CGocAttribute>: IsFullStat -> SetStartStatEnterWorld(双参:
//     nWorldType, IsFullStat) -> SetFullStat(false) -> SetStartRegStat(true)
//     -> SendOriginStatAll -> SendStatAll -> SetEchelonLevelBooster。
// ============================================================================
bool CWorldProcess::ReqWorldObjectInfo(XPacket& xPacket) {
    (void)xPacket;
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda2 (0x14062B3F0): 捕获 pUser
    std::function<void()> func = [pUser]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea())
            return;

        pUser->SetClientLoadComplete(true);
        pUser->SetLogChangeMap(false);

        // Per IDA: GetArea() 非空 -> vtable+136 SendObjectInfo(user) 虚分派
        XArea* pArea = pUser->GetArea();
        if (pArea)
            pArea->SendObjectInfo(static_cast<XActor*>(pUser));
        // Per IDA: GetArea() -> vtable+192 IsDistirct 为真 ->
        // RTDynamicCast(XDistrict) -> SetObjectInfoReq(user)
        if (pArea && pArea->IsDistirct()) {
            XDistrict* pDistrict = dynamic_cast<XDistrict*>(pArea);
            if (pDistrict)
                pDistrict->SetObjectInfoReq(pUser);
        }

        CGocAkashicRecord* pAkashic = pUser->GetGOC<CGocAkashicRecord>();
        if (pAkashic) {
            pAkashic->SetUserLoad(true);
            pAkashic->ThinkAkashicPassive();
        }

        CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
        XArea* pArea2 = pUser->GetArea();
        bool bIsFullStat = pUser->IsFullStat();
        int nWorldType = pArea2 ? pArea2->GetWorldType() : 0;
        if (pAttr) {
            pAttr->SetStartStatEnterWorld(nWorldType, bIsFullStat);
            pUser->SetFullStat(false);
            pAttr->SetStartRegStat(true);
            pAttr->SendOriginStatAll();
            pAttr->SendStatAll();
            pAttr->SetEchelonLevelBooster();
        }
    };

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192: 任务完成递减计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldWarp - IDA @ 0x14062B770
// 已精确还原 - 解析 nMapID/nJumpID/nPotalID；
// pUser 空或无 Area 返回 0；IncrementJobCount ->
// DoJob(GetMapInsID, lambda4) -> DecrementJobCount DoJob -> return 1。
// lambda4 体 (??R_lambda4_ @ 0x14062B9C0, 捕获 [pUser, nMapID, nJumpID, nPotalID]):
//   四重门 (pUser/IsLive/GetArea/GetArea);
//   IsGM && IsUserStatus(0x2000) -> LogError 667 return;
//   nMapID != GetWorldID -> SendErrorMessage(4,7,55014) + LogError 674 return;
//   GetArea()->IsMaze() && nPotalID > 0:
//     XMaze cast -> WarpPortal(pUser, nMapID, nJumpID, nPotalID) 失败 ->
//       SendErrorMessage(4,7,55014);
//   否则 (非迷宫或 nPotalID<=0):
//     GetTBMapID()==30031 && nPotalID==10012:
//       GetEnterDistrictPos -> sWorldID==30021: nPrevRevivePoint=GetRevivePoint;
//       否则 sWorldID==10002 -> 改 10031 -> GetStartPortalID 失败:
//         LogError(UCID, MapID) + 硬编码 STWarp (0xF2,0x31) wMapID=10003
//         pos(10228,10058,90) -> controlSocket Send;
//       成功: PS_ENTER_MAP_REQ{dwActorID=GetQuestID(GetActorID), nJumpID=nPrevRevivePoint,
//         wMapID=posInfo.sWorldID, vNextPos} -> (0xF2,0x31) -> controlSocket Send;
//     否则: GetWorldID -> GetPortalPos 成功:
//       pArea->MoveActor(pUser, posInfo.vPos) + MoveingValueClear +
//       ChangeMotion(1,1,16) + SendResWarp(0, GetPositionXVec3(), GetPosInfo()->fRot);
//       失败: LogError 744 + SendResWarp(1, GetPositionXVec3(), 0.0)。
// ============================================================================
bool CWorldProcess::ReqWorldWarp(XPacket& xPacket) {
    int nMapID = -1;
    int nJumpID = 0;
    int nPotalID = 0;
    xPacket.XParse >> nMapID;
    xPacket.XParse >> nJumpID;
    xPacket.XParse >> nPotalID;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda4 (0x14062B9C0)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, nMapID, nJumpID, nPotalID]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            if (!pUser->GetArea())
                return;

            if (pUser->IsGM() && pUser->IsUserStatus(0x2000)) {
                LogHelper::LogError(
                    "game.contents",
                    "ReqWorldWarp error - Game manager can not sector warp( %s ) ( %d )",
                    "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\Process\\WorldProcess.cpp",
                    667);
                return;
            }

            if (nMapID != static_cast<std::int16_t>(pUser->GetWorldID())) {
                pUser->SendErrorMessage(4, 7, 55014);
                LogHelper::LogError(
                    "game.contents",
                    "ReqWorldWarp error - Fault parameter when move through warp ( %s ) ( %d )",
                    "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\Process\\WorldProcess.cpp",
                    674);
                return;
            }

            XArea* pArea = pUser->GetArea();
            if (pArea->IsMaze() && nPotalID > 0) {
                XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                if (pMaze) {
                    if (!pMaze->WarpPortal(pUser, nMapID, nJumpID, nPotalID)) {
                        pUser->SendErrorMessage(4, 7, 55014);
                    }
                }
                return;
            }

            if (pArea->GetTBMapID() == 30031 && nPotalID == 10012) {
                // 特殊: 事件地图经 10012 传送门退出
                STPosInfo posInfo;
                pUser->GetEnterDistrictPos(posInfo);

                int nPrevRevivePoint = 0;
                if (posInfo.sWorldID == 30021) {
                    nPrevRevivePoint = pUser->GetRevivePoint();
                } else {
                    int nJumpIDOut = 0;
                    if (posInfo.sWorldID == 10002)
                        posInfo.sWorldID = 10031;

                    int nMapIDOut = posInfo.sWorldID;
                    if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                            nMapIDOut, &nJumpIDOut, &posInfo)) {
                        int sWorldID = posInfo.sWorldID;
                        LogHelper::LogError(
                            "game.contents",
                            "ReqWorldWarp error - Cant find Portal Pos[UCID:%d, MapId:%d]",
                            static_cast<int>(pUser->GetActorID().dwActorID), sWorldID);

                        // Per IDA: 硬编码兜底回 10003 安全点
                        PS_ENTER_MAP_REQ st{};
                        st.dwActorID = CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);
                        st.nJumpID = 0;
                        st.wMapID = 10003;
                        st.vNextPos = XVec3{10228.0f, 10058.0f, 90.0f};

                        XSendPacket xSendPacket(0xF2, 0x31);
                        xSendPacket << st;
                        XGameServer::Instance()->GetControlSocket().Send(xSendPacket);
                        return;
                    }
                }

                PS_ENTER_MAP_REQ psEnterMap{};
                psEnterMap.dwActorID = CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);
                psEnterMap.nJumpID = nPrevRevivePoint;
                psEnterMap.wMapID = posInfo.sWorldID;
                psEnterMap.vNextPos = posInfo.vPos;

                XSendPacket xSendPacket(0xF2, 0x31);
                xSendPacket << psEnterMap;
                XGameServer::Instance()->GetControlSocket().Send(xSendPacket);
            } else {
                int nWorldID = static_cast<std::int16_t>(pUser->GetWorldID());
                STPosInfo posInfo;
                if (XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
                        nWorldID, nJumpID, &posInfo)) {
                    pArea->MoveActor(static_cast<XActor*>(pUser), posInfo.vPos);
                    pUser->MoveingValueClear();
                    pUser->ChangeMotion(1, 1, 16);
                    pUser->SendResWarp(0, pUser->GetPositionXVec3(),
                                      pUser->GetPosInfo()->fRot);
                } else {
                    LogHelper::LogError(
                        "game.contents",
                        "ReqWorldWarp error - Cant find Portal Pos ( %s ) ( %d )",
                        "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\Process\\WorldProcess.cpp",
                        744);
                    pUser->SendResWarp(1, pUser->GetPositionXVec3(), 0.0f);
                }
            }
        });

    // Per IDA lambda5 (DecrementJobCount)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldCheckSector - IDA @ 0x14062C440
// 已精确还原 - 解析 nMapID + nCheckSectorID；
// pUser 空或无 Area 返回 0；
// lambda10 (0x14062CAF0) DoJob: IsLive/GetArea 门(两重) ->
// GetMoverObject(nCheckSectorID) 空: 不发送；
// GetType==0 (PC): (4,0x21)<<1 -> SetInfoPacket -> BridgeSend；
// GetType==2 (NPC): (4,0x23)<<1 -> SetInfoPacket -> BridgeSend -> SyncMove；
// GetType==1 (Monster): (4,0x22)<<1 -> SetInfoPacket -> BridgeSend。
// + lambda192 递减。
// ============================================================================
bool CWorldProcess::ReqWorldCheckSector(XPacket& xPacket) {
    int nMapID = 0;
    int nCheckSectorID = 0;
    xPacket.XParse >> nMapID;
    xPacket.XParse >> nCheckSectorID;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return 0;
    if (!pUser->GetArea())
        return 0;

    pUser->IncrementJobCount();

    // Per IDA lambda10 (0x14062CAF0)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, nCheckSectorID]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;

            CMover* pMover = pUser->GetMoverObject(nCheckSectorID);
            if (!pMover)
                return;

            int nType = pMover->GetType();
            if (nType == 0) {
                // PC
                XSendPacket xSendPacketPC(4, 0x21);
                xSendPacketPC.XParse << 1;
                pMover->SetInfoPacket(xSendPacketPC);
                pUser->BridgeSend(xSendPacketPC);
            } else if (nType == 2) {
                // NPC
                XSendPacket xSendPacketNPC(4, 0x23);
                xSendPacketNPC.XParse << 1;
                pMover->SetInfoPacket(xSendPacketNPC);
                pUser->BridgeSend(xSendPacketNPC);
                pMover->SyncMove();
            } else if (nType == 1) {
                // Monster
                XSendPacket xSendPacketM(4, 0x22);
                xSendPacketM.XParse << 1;
                pMover->SetInfoPacket(xSendPacketM);
                pUser->BridgeSend(xSendPacketM);
            }
        });

    // Per IDA lambda192: 任务完成递减计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldWarpOut - IDA @ 0x14062C1A0
// 已精确还原 - 不解析包体；
// GetClientPtr 后先构造 lambda6 (ctor 0x140632F50, 捕获 [pUser])，
// 再 pUser 空返回 0；vtable GetArea 空返回 0；
// IncrementJobCount -> DoJob(GetMapInsID, lambda6) ->
// lambda7(DecrementJobCount) DoJob -> return 1。
// lambda6 体 (??R_lambda6_ @ 0x14062C340):
//   pUser 非空 && IsLive && GetArea ->
//   dynamic_cast<XMaze*> -> GetActorID().dwActorID ->
//   GetWarpPotal -> CWarpPotal::RemoveWarpPotal(dwActorID)。
// ============================================================================
bool CWorldProcess::ReqWorldWarpOut(XPacket& xPacket) {
    (void)xPacket;

    CUser* pUser = GetClientPtr();
    // Per IDA: lambda6 构造先于 pUser 判空（捕获引用）
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda6 (0x14062C340)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;

            XMaze* pMaze = dynamic_cast<XMaze*>(pUser->GetArea());
            if (pMaze) {
                std::uint32_t dwActorID = pUser->GetActorID().dwActorID;
                CWarpPotal* pWarpPotal = pMaze->GetWarpPotal();
                pWarpPotal->RemoveWarpPotal(dwActorID);
            }
        });

    // Per IDA lambda7 (DecrementJobCount, ctor 0x140632F80)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldMoverInfo - IDA @ 0x14062C8F0
// 已精确还原 - 解析 nActorID；
// pUser 空或无 Area 返回 0；IncrementJobCount ->
// DoJob(GetMapInsID, lambda10) -> DecrementJobCount DoJob -> return 1。
// lambda10 体 (??R_lambda10_ @ 0x14062CAF0, 捕获 [pUser, nActorID]):
//   双重 IsLive/GetArea 门 -> GetMoverObject(nActorID):
//     Type 0 (PC): (4,0x21)<<1 -> SetInfoPacket -> BridgeSend;
//     Type 2 (NPC): (4,0x23)<<1 -> SetInfoPacket -> BridgeSend -> SyncMove;
//     Type 1 (Monster): (4,0x22)<<1 -> SetInfoPacket -> BridgeSend。
// ============================================================================
bool CWorldProcess::ReqWorldMoverInfo(XPacket& xPacket) {
    int nActorID = 0;
    xPacket.XParse >> nActorID;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda10 (0x14062CAF0)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, nActorID]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;

            CMover* pMover = pUser->GetMoverObject(nActorID);
            if (!pMover)
                return;

            int nType = pMover->GetType();
            if (nType == 0) {
                // PC
                XSendPacket xSendPacketPC(4, 0x21);
                xSendPacketPC.XParse << 1;
                pMover->SetInfoPacket(xSendPacketPC);
                pUser->BridgeSend(xSendPacketPC);
            } else if (nType == 2) {
                // NPC
                XSendPacket xSendPacketNPC(4, 0x23);
                xSendPacketNPC.XParse << 1;
                pMover->SetInfoPacket(xSendPacketNPC);
                pUser->BridgeSend(xSendPacketNPC);
                pMover->SyncMove();
            } else if (nType == 1) {
                // Monster
                XSendPacket xSendPacketM(4, 0x22);
                xSendPacketM.XParse << 1;
                pMover->SetInfoPacket(xSendPacketM);
                pUser->BridgeSend(xSendPacketM);
            }
        });

    // Per IDA lambda11 (DecrementJobCount)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldEscape - IDA @ 0x14062EC90
// 已精确还原 - 不解析包体；
// pUser 空返回 0；lambda80 ctor (捕获 [pUser, this]) 后 pUser 空返回 0；
// GetArea 空返回 0；IncrementJobCount ->
// DoJob(GetMapInsID, lambda16) -> lambda17 (DecrementJobCount) DoJob -> return 1。
// lambda16 体 (??R_lambda16_ @ 0x14062EE70, 捕获 [pUser, this]):
//   双重 IsLive/GetArea 门 -> IsEnableEscapeWorld:
//     false -> SendErrorMessage(0x33, 0xD705);
//     true -> dynamic_cast<XMaze*>, (pMaze && GetTBMapID()==22061)
//             || !IsBattleState:
//       false -> SendErrorMessage(0x33, 0xD712);
//       true -> GetArea()->EscapeActor(pUser):
//         true -> (4,0x33)<<1 -> BridgeSend -> SetNextEscapeTime;
//         false -> SendErrorMessage(0x33, 0xD706)。
// ============================================================================
bool CWorldProcess::ReqWorldEscape(XPacket& xPacket) {
    (void)xPacket;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda16 (0x14062EE70)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, this]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;

            if (!pUser->IsEnableEscapeWorld()) {
                SendErrorMessage(0x33, 0xD705);
                return;
            }

            XMaze* pMaze = dynamic_cast<XMaze*>(pUser->GetArea());
            if ((pMaze && static_cast<std::uint16_t>(pMaze->GetTBMapID()) == 22061)
                || !pUser->IsBattleState()) {
                XArea* pArea = pUser->GetArea();
                if (pArea->EscapeActor(pUser)) {
                    XSendPacket xSendPacket(4, 0x33);
                    xSendPacket.XParse << 1;
                    pUser->BridgeSend(xSendPacket);
                    pUser->SetNextEscapeTime();
                } else {
                    SendErrorMessage(0x33, 0xD706);
                }
            } else {
                SendErrorMessage(0x33, 0xD712);
            }
        });

    // Per IDA lambda17 (DecrementJobCount)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqGameOut - IDA @ 0x14062F500
// 已精确还原 - 不解析包体；
// pUser 空或无 Area 返回 0；IncrementJobCount ->
// DoJob(GetMapInsID, lambda20) -> lambda21 (DecrementJobCount) DoJob -> return 1。
// lambda20 体 (??R_lambda20_ @ 0x14062F6A0, 捕获 [pUser]):
//   pUser/IsLive/GetArea 门 -> XMaze cast ->
//   Party: IsParty -> ST_PARTY_INFO{byGroupType=1, nID=GetPartyID} ->
//     SetDisconnectUserState(GetQuestID(GetActorID()), stPartyInfo);
//   Force: IsParty -> ST_PARTY_INFO{byGroupType=2, nID=GetPartyID} ->
//     SetDisconnectUserState(同上)。
// ============================================================================
bool CWorldProcess::ReqGameOut(XPacket& xPacket) {
    (void)xPacket;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda20 (0x14062F6A0)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;

            XMaze* pMaze = dynamic_cast<XMaze*>(pUser->GetArea());
            if (!pMaze)
                return;

            // Per IDA: Party 断线状态登记
            auto pParty = pUser->GetGOC<CGocParty>();
            if (pParty && pParty->IsParty()) {
                ST_PARTY_INFO stPartyInfo{};
                stPartyInfo.byGroupType = 1;
                stPartyInfo.nID = pParty->GetPartyID();
                pMaze->SetDisconnectUserState(
                    CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID),
                    stPartyInfo);
            }

            // Per IDA: Force 断线状态登记 (IsForce/GetForceID 为 CGocParty 基类方法)
            auto pForce = pUser->GetGOC<CGocForce>();
            if (pForce && pForce->IsForce()) {
                ST_PARTY_INFO stPartyInfo{};
                stPartyInfo.byGroupType = 2;
                stPartyInfo.nID = pForce->GetForceID();
                pMaze->SetDisconnectUserState(
                    CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID),
                    stPartyInfo);
            }
        });

    // Per IDA lambda21 (DecrementJobCount)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldDistrictTransport - IDA @ 0x14062F910
// 已精确还原 - 解析 PS_DISTRICT_TRANSPORT_REQ；
// pUser 空或无 Area 返回 0 (判空在 parse 前)；
// 二次校验后 IncrementJobCount ->
// DoJob(GetMapInsID, lambda22) -> lambda23 (DecrementJobCount) DoJob -> return 1。
// lambda22 体 (??R_lambda22_ @ 0x14062FBF0, 捕获 [pUser, psDistrictTransport, this]):
//   [门禁] pUser/IsLive；IsMatching -> (4,0x40,55003)；
//     IsState(eStateNoProcess) 失败直接返回；
//     控制服 IsCanSend 失败 -> LogError + (4,0x40,50010)；
//     当前 TBMapID == 请求 wMapID (同图) 直接返回；
//   [表校验] GetTB_TRANSPORT_INFO(wTransportID) 空 -> LogError 1379 + 58002；
//     GetArea 空 -> LogError 1386 + 55003；
//     Check_Level 且等级不足 -> LogError 1396 + 55031；
//     Check_Quest -> GocQuest IsCompleteEpisode 失败 -> LogError 1408 + 55037；
//     Check_Achivement -> GetTB_ACHIEVEMENT 空 -> LogError 1420 + 58002；
//       GocAchieve CheckAchieveComplete(Complete_Bit) 失败 -> LogError 1428 + 55037；
//   [Type 1: NPC 传送] FindActor(dwNpcID) 空或非 NPC -> LogError 1444 + 58000；
//     CNpc cast 空 -> LogError 1452 + 58000；GetTB_NPC 空 -> LogError 1460 + 58002；
//     NPC_Function_Type_[0..2] 找 8 -> 失败 LogError 1477 + 55003；
//     用户/NPC 距离检查 (fabs > 1000) -> LogError 1489 + 58000；
//     GetTB_DISTRICT_TRANSPORT(NPC_Function_ID) 空 -> LogError 1497 + 58002；
//     Transport_Info_ID_[0..4] 含 wTransportID 否则 LogError 1514 + 55003；
//   [Type 2: 物品传送] stEnterMap.wMapID=Arrival_ID；GetTB_ITEM(dwTransportItemID)
//     空 -> LogError 1526 + 55003；GetTB_ITEM_CLASSIFY 空 -> LogError 1534 + 55003；
//     Item_Use_Type != 112 -> LogError 1541 + 52002；
//     GocInventory GetItem 失败 -> LogError 1549/1557 + 55003/52002；
//     Cooltime_Group 且 GetCoolTime > GetCurDate -> LogError 1564 + 52602；
//     byCoolTimeGroup/dwCoolTimeValue 记录；
//   [费用] Need_Gold > 0: GocNpcCredit GetNpcCreditBenefit(nNpcID,6) +
//     GocBooster GetTotalRate(eBooster_Effect_ReduceDec) 折扣率 ->
//     nReduceMoney 后 GetMoney < nNeedGold -> LogError 1601/1608 + 55003/58001；
//   [目标区] GetWorldType(wMapID):
//     2: nJumpID=nPortalID，为 0 -> LogError 1656 + 0xD6DB；
//        SoulWeapon 未装备 -> LogError 1663 + 0xD6E8；
//     0: GetTB_DISTRICT 空 -> LogError 1623 + 55003；
//        NeedQuest_ID -> IsCompleteEpisode/FindEpisode 双失败 -> LogError 1633 + 55037；
//        nJumpID==0 -> GetStartPortalID 失败 -> LogError 1644 + 0xD6DB；
//   [组团] Party -> byGroupType=1；Force -> byGroupType=2 且 wMapID!=11001 否则
//     0xCD8B；
//   [扣费] nNeedGold>0 -> AddMoney(-nNeedGold, 0x21, wTransportID)；
//   [Type 2 同图] XDistrict cast -> IsEnableEscapeWorld 失败 52602 ->
//     EscapeActor -> SetNextEscapeTime + AddCoolTime；失败 55046；非 XDistrict 55003；
//   [异图] SetState(eStateChangeWorld) + AllHelperRelease +
//     (0xF2,0x31) SendCmd(4,1)：成功 -> NPC 信用点 (TB_CREDIT_RAISE 10*GroupID+9
//     nRand 提升 UpdateNpcCredit) + Type1 UpdateCollect(0x38,1) /
//     Type2 AddCoolTime；失败 ClearState；
// ============================================================================
bool CWorldProcess::ReqWorldDistrictTransport(XPacket& xPacket) {
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    PS_DISTRICT_TRANSPORT_REQ psDistrictTransport{};
    xPacket >> psDistrictTransport;

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda22 (0x14062FBF0)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, psDistrictTransport, this]() {
            if (!pUser || !pUser->IsLive())
                return;

            if (pUser->IsMatching()) {
                pUser->SendErrorMessage(4, 0x40, 55003);
                return;
            }

            if (!pUser->IsState(XClient::eStateNoProcess)) {
                // Per IDA: 控制服可达性
                if (!XGameServer::Instance()->GetControlSocket().IsCanSend()) {
                    LogHelper::LogError(
                        "game.contents",
                        "[WORLD] Failed ReqWorldDistrictTransport - CONTROL_SOCKET.IsCanSend( ) %d",
                        static_cast<int>(pUser->GetActorID().dwActorID));
                    pUser->SendErrorMessage(4, 0x40, 50010);
                    return;
                }

                // Per IDA: 同图检查 (当前 TBMapID == 请求 wMapID)
                if (pUser->GetArea()->GetTBMapID() != psDistrictTransport.stEnterMap.wMapID) {
                    PS_ENTER_MAP_REQ stEnterMap = psDistrictTransport.stEnterMap;
                    stEnterMap.dwActorID =
                        CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);

                    LogHelper::LogDebug(
                        "game.contents",
                        "<Recv> ReqWorldDistrictTransport [ ActorID:%d, MapID:%d ]",
                        static_cast<int>(pUser->GetActorID().dwActorID),
                        static_cast<int>(stEnterMap.wMapID));

                    TB_TRANSPORT_INFO* pTB_TRANSPORT_INFO =
                        XGameServer::Instance()->GetResourceMgr().GetTB_TRANSPORT_INFO(
                            psDistrictTransport.wTransportID);
                    if (!pTB_TRANSPORT_INFO) {
                        LogHelper::LogError(
                            "game.item",
                            "District Transport error - TB_TRANSPORT_INFO Table NULL [ UCID:%d ] [ TableID:%d ] ( %d )",
                            static_cast<int>(pUser->GetActorID().dwActorID),
                            static_cast<int>(psDistrictTransport.wTransportID), 1379);
                        pUser->SendErrorMessage(4, 0x40, 58002);
                        return;
                    }

                    if (!pUser->GetArea()) {
                        LogHelper::LogError(
                            "game.item",
                            "District Transport error - Area NULL [ UCID:%d ] ( %d )",
                            static_cast<int>(pUser->GetActorID().dwActorID), 1386);
                        pUser->SendErrorMessage(4, 0x40, 55003);
                        return;
                    }

                    if (pTB_TRANSPORT_INFO->Check_Level &&
                        pUser->GetLevel() < pTB_TRANSPORT_INFO->Check_Level) {
                        LogHelper::LogError(
                            "game.item",
                            "District Transport error - Check Level [ UCID:%d ] [ Level:%d/%d ] ( %d )",
                            static_cast<int>(pUser->GetActorID().dwActorID),
                            static_cast<int>(pUser->GetLevel()),
                            static_cast<int>(pTB_TRANSPORT_INFO->Check_Level), 1396);
                        pUser->SendErrorMessage(4, 0x40, 55031);
                        return;
                    }

                    if (pTB_TRANSPORT_INFO->Check_Quest) {
                        auto pQuest = pUser->GetGOC<CGocQuest>();
                        if (!pQuest ||
                            !pQuest->IsCompleteEpisode(pTB_TRANSPORT_INFO->Check_Quest)) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid quest condition[ ActorID:%d, QuestID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(pTB_TRANSPORT_INFO->Check_Quest), 1408);
                            pUser->SendErrorMessage(4, 0x40, 55037);
                            return;
                        }
                    }

                    if (pTB_TRANSPORT_INFO->Check_Achivement) {
                        TB_ACHIEVEMENT* pTB_ACHIEVEMENT =
                            XGameServer::Instance()->GetResourceMgr().GetTB_ACHIEVEMENT(
                                pTB_TRANSPORT_INFO->Check_Achivement);
                        if (!pTB_ACHIEVEMENT) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - TB_ACHIEVEMENT Table NULL [ UCID:%d ] [ TableID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(pTB_TRANSPORT_INFO->Check_Achivement), 1420);
                            pUser->SendErrorMessage(4, 0x40, 58002);
                            return;
                        }
                        auto pAchieve = pUser->GetGOC<CGocAchieve>();
                        if (!pAchieve ||
                            !pAchieve->CheckAchieveComplete(pTB_ACHIEVEMENT->Complete_Bit)) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid quest condition[ ActorID:%d, AchieveID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(pTB_TRANSPORT_INFO->Check_Achivement), 1428);
                            pUser->SendErrorMessage(4, 0x40, 55037);
                            return;
                        }
                    }

                    CNpc* pNpc = nullptr;
                    std::uint8_t byCoolTimeGroup = 0;
                    std::uint64_t dwCoolTimeValue = 0;

                    if (pTB_TRANSPORT_INFO->Transport_Type == 1) {
                        // Type 1: NPC 传送
                        XActor* pActor =
                            pUser->GetArea()->FindActor(psDistrictTransport.dwNpcID);
                        if (!pActor || pActor->GetType() != 1) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - NPC Find Fail [ UCID:%d ] [ NpcID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.dwNpcID), 1444);
                            pUser->SendErrorMessage(4, 0x40, 58000);
                            return;
                        }
                        pNpc = dynamic_cast<CNpc*>(pActor);
                        if (!pNpc) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - NPC Find Fail [ UCID:%d ] [ NpcID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.dwNpcID), 1452);
                            pUser->SendErrorMessage(4, 0x40, 58000);
                            return;
                        }

                        TB_NPC* pTB_NPC = XGameServer::Instance()->GetResourceMgr().GetTB_NPC(
                            static_cast<std::uint32_t>(pNpc->GetTableID()));
                        if (!pTB_NPC) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - NPC Table NULL [ UCID:%d ] [ NpcID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.dwNpcID), 1460);
                            pUser->SendErrorMessage(4, 0x40, 58002);
                            return;
                        }

                        int nIndex_Npc = -1;
                        for (int i = 0; i < 3; ++i) {
                            if ((&pTB_NPC->NPC_Function_Type_01)[i] == 8) {
                                nIndex_Npc = i;
                                break;
                            }
                        }
                        if (nIndex_Npc < 0) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - TB_NPC Table Function Find Fail [ UCID:%d ] [ NpcID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.dwNpcID), 1477);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }

                        // Per IDA: 用户与 NPC 距离检查 (1000 单位)
                        XVec3 vUserPos = pUser->GetPositionXVec3();
                        XVec3 vNPCPos = pNpc->GetPositionXVec3();
                        if (std::fabs(vUserPos.x - vNPCPos.x) > 1000.0f ||
                            std::fabs(vUserPos.y - vNPCPos.y) > 1000.0f ||
                            std::fabs(vUserPos.z - vNPCPos.z) > 1000.0f) {
                            LogHelper::LogError(
                                "game.item",
                                "CheckNpc error - User/NPC Pos Check [ UCID:%d ] [ TableID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(
                                    (&pTB_NPC->NPC_Function_ID_01)[nIndex_Npc]), 1489);
                            pUser->SendErrorMessage(4, 0x40, 58000);
                            return;
                        }

                        TB_DISTRICT_TRANSPORT* pTB_DISTRICT_TRANSPORT =
                            XGameServer::Instance()->GetResourceMgr()
                                .GetTB_DISTRICT_TRANSPORT(
                                    (&pTB_NPC->NPC_Function_ID_01)[nIndex_Npc]);
                        if (!pTB_DISTRICT_TRANSPORT) {
                            LogHelper::LogError(
                                "game.item",
                                "CheckNpc error - TB_DISTRICT_TRANSPORT Table NULL [ UCID:%d ] [ TableID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(
                                    (&pTB_NPC->NPC_Function_ID_01)[nIndex_Npc]), 1497);
                            pUser->SendErrorMessage(4, 0x40, 58002);
                            return;
                        }

                        bool bCheckTransportID = false;
                        for (int j = 0; j < 5; ++j) {
                            if ((&pTB_DISTRICT_TRANSPORT->Transport_Info_ID_01)[j] ==
                                psDistrictTransport.wTransportID) {
                                bCheckTransportID = true;
                                break;
                            }
                        }
                        if (!bCheckTransportID) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - pTB_DISTRICT_TRANSPORT Table Transport Find Fail [ UCID:%d ] [ TransportID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.wTransportID), 1514);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }
                    } else if (pTB_TRANSPORT_INFO->Transport_Type == 2) {
                        // Type 2: 物品传送
                        stEnterMap.wMapID = pTB_TRANSPORT_INFO->Arrival_ID;

                        TB_ITEM* pTB_ITEM = XGameServer::Instance()->GetResourceMgr().GetTB_ITEM(
                            psDistrictTransport.dwTransportItemID);
                        if (!pTB_ITEM) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - pTB_ITEM Table Transport Find Fail [ UCID:%d ] [ TransportID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.wTransportID), 1526);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }

                        TB_ITEM_CLASSIFY* pTB_ITEM_CLASSIFY =
                            XGameServer::Instance()->GetResourceMgr().GetTB_ITEM_CLASSIFY(
                                pTB_ITEM->Item_Classify_Index);
                        if (!pTB_ITEM_CLASSIFY) {
                            LogHelper::LogError(
                                "game.item",
                                "District Transport error - pTB_ITEM_CLASSIFY Table Transport Find Fail [ UCID:%d ] [ TransportID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(psDistrictTransport.wTransportID), 1534);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }

                        if (pTB_ITEM_CLASSIFY->Item_Use_Type != 112) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid Item_Use_Type [ ActorID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID), 1541);
                            pUser->SendErrorMessage(4, 0x40, 52002);
                            return;
                        }

                        auto pInven = pUser->GetGOC<CGocInventory>();
                        if (!pInven) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid Inven [ ActorID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID), 1549);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }

                        auto pTransportItem =
                            pInven->GetItem(pTB_ITEM_CLASSIFY->Item_Inven_Type,
                                            static_cast<int>(
                                                psDistrictTransport.dwTransportItemID));
                        if (!pTransportItem) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid TransportItem [ ActorID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID), 1557);
                            pUser->SendErrorMessage(4, 0x40, 52002);
                            return;
                        }

                        if (pTB_ITEM->Cooltime_Group) {
                            std::uint64_t dwCoolTime =
                                pInven->GetCoolTime(pTB_ITEM->Cooltime_Group,
                                                    pTB_ITEM->Cooltime_Save != 0);
                            if (dwCoolTime >
                                static_cast<std::uint64_t>(
                                    XGameServer::Instance()->GetCurDate())) {
                                LogHelper::LogError(
                                    "game.contents",
                                    "ReqWorldDistrictTransport error - Item CoolTime [ ActorID:%d ] ( %d )",
                                    static_cast<int>(pUser->GetActorID().dwActorID), 1564);
                                pUser->SendErrorMessage(4, 0x40, 52602);
                                return;
                            }
                        }
                        byCoolTimeGroup = pTB_ITEM->Cooltime_Group;
                        dwCoolTimeValue = pTB_ITEM->Cooltime_Value;
                    }

                    // Per IDA: 费用计算与折扣
                    int nNeedGold = static_cast<int>(pTB_TRANSPORT_INFO->Need_Gold);
                    if (nNeedGold > 0) {
                        float fRate = 0.0f;

                        auto pNpcCredit = pUser->GetGOC<CGocNpcCredit>();
                        if (pNpc && pNpcCredit) {
                            int nNpcID = pNpc->GetTableID();
                            int NpcCreditBenefit =
                                pNpcCredit->GetNpcCreditBenefit(nNpcID, 6);
                            fRate += static_cast<float>(NpcCreditBenefit);
                        }

                        if (fRate > 0.0f)
                            fRate = fRate / 10000.0f;

                        auto pBooster = pUser->GetGOC<CGocBooster>();
                        if (pBooster) {
                            float TotalRate =
                                pBooster->GetTotalRate(eBooster_Effect_ReduceDec);
                            fRate += TotalRate;
                        }

                        if (fRate != 0.0f) {
                            int nReduceMoney =
                                static_cast<int>(static_cast<float>(nNeedGold) * fRate);
                            nNeedGold -= nReduceMoney;
                        }

                        auto pInven = pUser->GetGOC<CGocInventory>();
                        if (!pInven) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid Inven [ ActorID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID), 1601);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }

                        if (pInven->GetMoney() < nNeedGold) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - Invalid Inven [ ActorID:%d ] ( %d )",
                                static_cast<int>(pUser->GetActorID().dwActorID), 1608);
                            pUser->SendErrorMessage(4, 0x40, 58001);
                            return;
                        }
                    }

                    // Per IDA: 目标区检查
                    int nMoveType = GetWorldType(stEnterMap.wMapID);
                    if (nMoveType) {
                        if (nMoveType == 2) {
                            stEnterMap.nJumpID = stEnterMap.nPortalID;
                            if (!stEnterMap.nPortalID) {
                                LogHelper::LogError(
                                    "game.contents",
                                    "ReqWorldDistrictTransport error - Cant Find PortalPos When Enter BattleCry[ UCID:%d, MapID:%d) ( %d )",
                                    static_cast<int>(pUser->GetActorID().dwActorID),
                                    static_cast<int>(stEnterMap.wMapID), 1656);
                                CGocNetwork::SendErrorMessage(pUser, 4, 0x40, 0xD6DB);
                                return;
                            }
                            if (pUser->stMyCharInfoEx()->stSoulWeapon.dwItemID == -1) {
                                LogHelper::LogError(
                                    "game.contents",
                                    "ReqWorldDistrictTransport error - No Weapon Enter BattleCry[ UCID:%d, MapID:%d) ( %d )",
                                    static_cast<int>(pUser->GetActorID().dwActorID),
                                    static_cast<int>(stEnterMap.wMapID), 1663);
                                CGocNetwork::SendErrorMessage(pUser, 4, 0x40, 0xD6E8);
                                return;
                            }
                        }
                    } else {
                        TB_DISTRICT* pTBDistrict =
                            XGameServer::Instance()->GetResourceMgr().GetTB_DISTRICT(
                                stEnterMap.wMapID);
                        if (!pTBDistrict) {
                            LogHelper::LogError(
                                "game.contents",
                                "ReqWorldDistrictTransport error - No Table TB_DISTRICT[ ActorID:%d, MapID:%d ] ( %d ) ",
                                static_cast<int>(pUser->GetActorID().dwActorID),
                                static_cast<int>(stEnterMap.wMapID), 1623);
                            pUser->SendErrorMessage(4, 0x40, 55003);
                            return;
                        }

                        if (pTBDistrict->NeedQuest_ID) {
                            auto pQuest = pUser->GetGOC<CGocQuest>();
                            if (!pQuest ||
                                (!pQuest->IsCompleteEpisode(pTBDistrict->NeedQuest_ID) &&
                                 !pQuest->FindEpisode(pTBDistrict->NeedQuest_ID))) {
                                LogHelper::LogError(
                                    "game.contents",
                                    "ReqWorldDistrictTransport error - Invalid quest condition[ ActorID:%d, MapID:%d ] ( %d )",
                                    static_cast<int>(pUser->GetActorID().dwActorID),
                                    static_cast<int>(stEnterMap.wMapID), 1633);
                                pUser->SendErrorMessage(4, 0x40, 55037);
                                return;
                            }
                        }

                        STPosInfo posInfo;
                        if (!stEnterMap.nJumpID) {
                            int nMapID = stEnterMap.wMapID;
                            if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                                    nMapID, &stEnterMap.nJumpID, &posInfo)) {
                                LogHelper::LogError(
                                    "game.contents",
                                    "ReqWorldDistrictTransport error - Cant Find PortalPos When create Maze[ ActorID:%d, MapID:%d ] ( %d )",
                                    static_cast<int>(pUser->GetActorID().dwActorID),
                                    static_cast<int>(stEnterMap.wMapID), 1644);
                                CGocNetwork::SendErrorMessage(pUser, 4, 0x40, 0xD6DB);
                                return;
                            }
                        }
                    }

                    stEnterMap.dwActorID =
                        CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);

                    // Per IDA: 组团信息
                    auto pParty = pUser->GetGOC<CGocParty>();
                    auto pForce = pUser->GetGOC<CGocForce>();
                    if (pParty && pParty->IsParty()) {
                        stEnterMap.stPartyInfo.byGroupType = 1;
                        stEnterMap.stPartyInfo.nID =
                            static_cast<int>(pParty->GetPartyID());
                    } else if (pForce && pForce->IsForce()) {
                        stEnterMap.stPartyInfo.byGroupType = 2;
                        stEnterMap.stPartyInfo.nID = static_cast<int>(pForce->GetForceID());
                        if (stEnterMap.wMapID != 11001) {
                            CGocNetwork::SendErrorMessage(pUser, 4, 0x40, 0xCD8B);
                            return;
                        }
                    }

                    // Per IDA: 扣费
                    if (nNeedGold > 0) {
                        auto pInven = pUser->GetGOC<CGocInventory>();
                        if (pInven) {
                            pInven->AddMoney(
                                -static_cast<std::int64_t>(nNeedGold), 0x21,
                                static_cast<int>(psDistrictTransport.wTransportID), 0,
                                false);
                        }
                    }

                    if (pTB_TRANSPORT_INFO->Transport_Type == 2 &&
                        static_cast<std::uint16_t>(
                            (pUser->GetMapInsID().nMapID << 16) >> 48) ==
                            stEnterMap.wMapID) {
                        // Per IDA: Type 2 同图走 EscapeActor
                        XDistrict* pDistrict =
                            dynamic_cast<XDistrict*>(pUser->GetArea());
                        if (pDistrict) {
                            if (pUser->IsEnableEscapeWorld()) {
                                if (pDistrict->EscapeActor(
                                        static_cast<XActor*>(pUser))) {
                                    pUser->SetNextEscapeTime();
                                    auto pInven = pUser->GetGOC<CGocInventory>();
                                    if (pInven && byCoolTimeGroup) {
                                        pInven->AddCoolTime(byCoolTimeGroup,
                                                            dwCoolTimeValue, false);
                                    }
                                } else {
                                    pUser->SendErrorMessage(4, 0x40, 55046);
                                }
                            } else {
                                pUser->SendErrorMessage(4, 0x40, 52602);
                            }
                        } else {
                            pUser->SendErrorMessage(4, 0x40, 55003);
                        }
                    } else {
                        // Per IDA: 异图走控制服换图
                        pUser->SetState(XClient::eStateChangeWorld);

                        auto pHelper = pUser->GetGOC<CGocHelper>();
                        if (pHelper) {
                            pHelper->AllHelperRelease();
                        }

                        XSendPacket packet(0xF2, 0x31);
                        packet << stEnterMap;
                        if (XGameServer::Instance()->GetControlSocket().SendCmd(
                                &packet, pUser, 4, 1)) {
                            // Per IDA: NPC 信用点提升
                            if (pNpc && pNpc->GetNpcTableRef()) {
                                int nNpcCreditIndex =
                                    10 * pNpc->GetNpcTableRef()->Npc_Group_ID + 9;
                                TB_CREDIT_RAISE* pTBCreditRaise =
                                    XGameServer::Instance()->GetResourceMgr()
                                        .GetTB_CREDIT_RAISE(nNpcCreditIndex);
                                if (pTBCreditRaise) {
                                    int nMax = pTBCreditRaise->Raise_Count_Max;
                                    int nMin = pTBCreditRaise->Raise_Count_Min;
                                    std::uint8_t byPoint = static_cast<std::uint8_t>(
                                        XGameServer::Instance()->GetItemFactory().nRand(
                                            nMin, nMax));
                                    auto pNpcCredit = pUser->GetGOC<CGocNpcCredit>();
                                    if (pNpcCredit) {
                                        pNpcCredit->UpdateNpcCredit(
                                            static_cast<std::uint32_t>(
                                                pNpc->GetTableID()),
                                            static_cast<std::int32_t>(byPoint));
                                    }
                                } else {
                                    LogHelper::LogError(
                                        "game.contents",
                                        "ReqWorldDistrictTransport error - No Table TB_CREDIT_RAISE[Index:%d]",
                                        nNpcCreditIndex);
                                }
                            }

                            if (pTB_TRANSPORT_INFO->Transport_Type == 1) {
                                auto pAchievePtr = pUser->GetGOC<CGocAchieve>();
                                if (pAchievePtr) {
                                    pAchievePtr->UpdateCollect(0x38, 1, 0);
                                }
                            } else if (pTB_TRANSPORT_INFO->Transport_Type == 2) {
                                auto pInven = pUser->GetGOC<CGocInventory>();
                                if (pInven && byCoolTimeGroup) {
                                    pInven->AddCoolTime(byCoolTimeGroup, dwCoolTimeValue,
                                                        false);
                                }
                            }
                        } else {
                            pUser->ClearState(XClient::eStateChangeWorld);
                        }
                    }
                }
            }
        });

    // Per IDA lambda23 (DecrementJobCount)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// ReqWorldEnterByForce - IDA @ 0x14062DA90
// 已精确还原 - 解析 PS_ENTER_MAP_REQ；lambda14 与 ReqWorldEnter 共用同一
// lambda 符号 (ctor 0x14062CD00, body 0x14062DD90)，按原始二进制在此处
// 独立实例化一份；pUser 空或无 Area 返回 0；解析后二次校验再
// IncrementJobCount -> DoJob(GetMapInsID) + lambda192 递减 DoJob -> return 1。
// ============================================================================
bool CWorldProcess::ReqWorldEnterByForce(XPacket& xPacket) {
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    PS_ENTER_MAP_REQ stRecvEnterMap{};
    xPacket >> stRecvEnterMap;

    // Per IDA lambda14 (0x14062DD90, ByForce 专属体): 捕获 pUser / stEnterMap / this
    CWorldProcess* pProcess = this;
    std::function<void()> func = [pUser, stRecvEnterMap, pProcess]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()
            || !pUser->GetArea())
            return;

        PS_ENTER_MAP_REQ stEnterMap = stRecvEnterMap;
        stEnterMap.dwActorID = CQuestCondition::GetQuestID(
            pUser->GetActorID());
        LogHelper::LogDebug("game.contents", "<WORLD> %d ",
            pUser->GetActorID().dwActorID, stEnterMap.wMapID);

        int nMoveType = GetWorldType(stEnterMap.wMapID);
        if (nMoveType == 0) {
            // ---- District 分支 ----
            if (!XGameServer::Instance()->GetResourceMgr().GetTB_DISTRICT(
                    stEnterMap.wMapID)) {
                LogHelper::LogError("game.contents",
                    "ReqWorldEnterByForce error - No Table TB_DISTRICT[ ActorID:%d, MapID:%d ] ( %d )",
                    pUser->GetActorID().dwActorID,
                    static_cast<int>(stEnterMap.wMapID), 1042);
                pUser->SendErrorMessage(4, 1, 55003);
                return;
            }
            STPosInfo posInfo{};
            if (!stRecvEnterMap.nJumpID) {
                if (!XGameServer::Instance()->GetWorldResMgr()
                        .GetStartPortalID(stEnterMap.wMapID,
                            &stEnterMap.nJumpID, &posInfo)) {
                    LogHelper::LogError("game.contents",
                        "ReqWorldEnterByForce error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                        pUser->GetActorID().dwActorID,
                        static_cast<int>(stEnterMap.wMapID), 1052);
                    CGocNetwork::SendErrorMessage(
                        static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                    return;
                }
            }
            // Per IDA: goto LABEL_48 (公共进图尾)
        } else if (nMoveType != 2) {
            // ---- 副本分支 (nMoveType 1/3) ----
            STPosInfo posInfo{};
            if (!XGameServer::Instance()->GetWorldResMgr()
                    .GetStartPortalID(stEnterMap.wMapID,
                        &stEnterMap.nJumpID, &posInfo)) {
                LogHelper::LogError("game.contents",
                    "ReqWorldEnterByForce error - Cant Find PortalPos When create Maze[ ActorID:%d, TBID:%d ] ( %d )",
                    pUser->GetActorID().dwActorID,
                    static_cast<int>(stEnterMap.wMapID), 1087);
                CGocNetwork::SendErrorMessage(
                    static_cast<CMover*>(pUser), 4, 1, 0xD6DB);
                return;
            }
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            if (pGocParty && pGocParty->IsParty()) {
                std::shared_ptr<CParty> pParty = pGocParty->GetParty();
                if (pParty) {
                    // Per IDA: v57.uxMapID (posInfo 的 UXMapID) 为当前实例
                    if (pParty->EnterMazeByForce(pUser, posInfo.uxMapID,
                                                  &stEnterMap)) {
                        std::uint32_t dwAgreeActor =
                            CQuestCondition::GetQuestID(
                                pUser->GetActorID());
                        // Per IDA: 0x14002F250 ICF 后 SetEnterMazeResponse
                        if (pParty->SetEnterMazeResponse(dwAgreeActor)
                            == 1) {
                            // Per IDA: GetEnterMazeRequest (0x14002F250 ICF)
                            // 读 stMazeInfo.wMapID (offset +16)
                            std::uint16_t wReqMapID = pParty
                                ->GetEnterMazeRequest()
                                ->stMazeInfo.wMapID;
                            if (!XGameServer::Instance()
                                    ->GetResourceMgr()
                                    .GetTB_MAZE_INFO(wReqMapID)) {
                                LogHelper::LogError("game.contents",
                                    "ReqWorldEnterByForce error - Different maze data[ TBID:%d ] ( %d ) ",
                                    static_cast<int>(wReqMapID), 1106);
                                pUser->SendErrorMessage(0x11, 0x41, 55003);
                                return;
                            }
                            pParty->CreateMazeReq();
                        }
                    }
                }
                return;
            }
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocForce && pGocForce->IsParty()) {
                // Per IDA: GetParty 返回基类 shared_ptr，后续按 CForce 使用
                std::shared_ptr<CForce> pForce =
                    std::static_pointer_cast<CForce>(pGocForce->GetParty());
                if (pForce) {
                    if (pForce->EnterMazeByForce(pUser, posInfo.uxMapID,
                                                  &stEnterMap)) {
                        std::uint32_t dwQuestID =
                            CQuestCondition::GetQuestID(
                                pUser->GetActorID());
                        if (pForce->SetEnterMazeResponse(dwQuestID) == 1) {
                            std::uint16_t wReqMapID = pForce
                                ->GetEnterMazeRequest()
                                ->stMazeInfo.wMapID;
                            if (!XGameServer::Instance()
                                    ->GetResourceMgr()
                                    .GetTB_MAZE_INFO(wReqMapID)) {
                                LogHelper::LogError("game.contents",
                                    "ReqWorldEnterByForce error - Different maze data[ TBID:%d ] ( %d ) ",
                                    static_cast<int>(wReqMapID), 1132);
                                pUser->SendErrorMessage(0x11, 0x41, 55003);
                                return;
                            }
                            pForce->CreateMazeReq();
                        }
                    }
                }
                return;
            }
            // Per IDA: 无队伍无 Force - goto LABEL_48
        } else {
            // ---- MyRoom 分支 (nMoveType == 2) ----
            if (stEnterMap.wMapID == 30031
                && !pProcess->IsEnterTheThingEvent()) {
                LogHelper::LogError("game.contents",
                    "ReqWorldEnter error - Failed enter Thething raid[UCID:%d, MapID:%d] ( %d )",
                    pUser->GetActorID().dwActorID,
                    static_cast<int>(stEnterMap.wMapID), 1065);
                return;
            }
            int nTBMapID = pUser->GetArea()->GetTBMapID();
            stEnterMap.nJumpID = XGameServer::Instance()
                ->GetWorldResMgr().GetJumpID(nTBMapID,
                                             stEnterMap.nPortalID);
            if (!stEnterMap.nJumpID) {
                STPosInfo posInfo{};
                if (!XGameServer::Instance()->GetWorldResMgr()
                        .GetStartPortalID(stEnterMap.wMapID,
                            &stEnterMap.nJumpID, &posInfo)) {
                    // Per IDA: GetJumpID 与 GetStartPortalID 双失败 - 直接返回
                    return;
                }
            }
        }

        // ---- LABEL_48: 公共进图发送尾 ----
        pUser->SetState(XClient::eStateChangeWorld);
        pUser->SetLogChangeMap(true);

        if (stEnterMap.wMapID / 10000 == 2) {
            ST_CREATE_MAZE stMaze{};
            stMaze.stPartyInfo = stEnterMap.stPartyInfo;
            stMaze.dwUserID = CQuestCondition::GetQuestID(
                pUser->GetActorID());
            stMaze.dwUAID = pUser->GetUAID();
            stMaze.wReqMapID = stEnterMap.wMapID;
            stMaze.nJumpID = stEnterMap.nJumpID;
            stMaze.stEnterDistrictPos = stEnterMap.stEnterDistrictPos;
            ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
            stMemberInfo.dwMember = CQuestCondition::GetQuestID(
                pUser->GetActorID());
            CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
            stMemberInfo.nState = pEntity ? (pEntity->GetNetCafe() ? 1 : 0)
                                           : 0;
            stMaze.vecEnterMember.push_back(stMemberInfo);
            XSendPacket packet(0xF2, 0x21);
            packet << stMaze;
            XGameServer::Instance()->GetControlSocket().SendCmd(
                &packet, pUser, 0x11, 0x41);
        } else {
            stEnterMap.dwActorID = CQuestCondition::GetQuestID(
                pUser->GetActorID());
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocParty && pGocParty->IsParty()) {
                stEnterMap.stPartyInfo.byGroupType = 1;
                stEnterMap.stPartyInfo.nID = pGocParty->GetPartyID();
            } else if (pGocForce && pGocForce->IsParty()) {
                stEnterMap.stPartyInfo.byGroupType = 2;
                stEnterMap.stPartyInfo.nID = pGocForce->GetPartyID();
            }
            XSendPacket xSendPacket(0xF2, 0x31);
            xSendPacket << stEnterMap;
            XGameServer::Instance()->GetControlSocket().SendCmd(
                &xSendPacket, pUser, 0x11, 0x41);
        }
    };

    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192: 任务完成递减计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}


// ============================================================================
// ReqWorldSkipInTutorial02 - IDA @ 0x140632240
// 已精确还原 - 解析 nMapID；
// pUser 空或无 Area 返回 0；IncrementJobCount ->
// DoJob(GetMapInsID, lambda24) -> DecrementJobCount DoJob -> return 1。
// lambda24 体 (??R_lambda24_ @ 0x140632430, 捕获 [pUser, nMapID]):
//   四重门 -> IsGM && IsUserStatus(0x2000) -> LogError 1804;
//   nMapID != GetWorldID -> SendErrorMessage(4,7,55014) + LogError 1811;
//   GetArea()->IsMaze -> XMaze cast -> GetTutorial && GetTBMapID()==21112:
//     GetGOC<CGocQuest> 非空:
//       !IsCompleteEpisode(0x186A7):
//         !FindEpisode(0x186A5) && !IsCompleteEpisode(0x186A5) ->
//           MoveNpcToWayPoint(101001, 4109);
//         ResetQuestAll(0x186A5) + AcceptQuestByForce(0x186A7) +
//         CompleteQuestByForce(0x186A7);
//     MoveNextSector(pUser, 2);
//     否则 LogError 1824。
// ============================================================================
bool CWorldProcess::ReqWorldSkipInTutorial02(XPacket& xPacket) {
    int nMapID = -1;
    xPacket.XParse >> nMapID;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // Per IDA lambda24 (0x140632430)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, nMapID]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            if (!pUser->GetArea())
                return;

            if (pUser->IsGM() && pUser->IsUserStatus(0x2000)) {
                LogHelper::LogError(
                    "game.contents",
                    "ReqWorldWarp error - Game manager can not sector warp( %s ) ( %d )",
                    "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\Process\\WorldProcess.cpp",
                    1804);
                return;
            }

            if (nMapID != static_cast<std::int16_t>(pUser->GetWorldID())) {
                pUser->SendErrorMessage(4, 7, 55014);
                LogHelper::LogError(
                    "game.contents",
                    "ReqWorldWarp error - Fault parameter when move through warp ( %s ) ( %d )",
                    "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\Process\\WorldProcess.cpp",
                    1811);
                return;
            }

            XArea* pArea = pUser->GetArea();
            if (!pArea->IsMaze())
                return;

            XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
            if (!pMaze)
                return;

            if (pMaze->GetTutorial() && pMaze->GetTBMapID() == 21112) {
                auto pQuest = pUser->GetGOC<CGocQuest>();
                if (pQuest) {
                    if (!pQuest->IsCompleteEpisode(0x186A7)) {
                        if (!pQuest->FindEpisode(0x186A5) &&
                            !pQuest->IsCompleteEpisode(0x186A5)) {
                            pMaze->MoveNpcToWayPoint(101001, 4109);
                        }
                        pQuest->ResetQuestAll();
                        pQuest->AcceptQuestByForce(0x186A7);
                        pQuest->CompleteQuestByForce(0x186A7);
                    }
                }
                pMaze->MoveNextSector(pUser, 2);
            } else {
                LogHelper::LogError(
                    "game.contents",
                    "ReqWorldWarp error - Tutorial sector move req but in wrong sector ( %s ) ( %d )",
                    "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\Process\\WorldProcess.cpp",
                    1824);
            }
        });

    // Per IDA lambda25 (DecrementJobCount)
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// CheckAdmissionMember - IDA @ 0x140632770
// 已精确还原 - 按 Admission_Member 类型校验组队状态。
// nType==1 (solo): 已在队伍 -> *nErrorCode=55032, false;
// nType==2 (需在队伍): 不在队伍 -> 55033, false;
// nType==3 (入队): 不在队伍 -> 55033; 队伍已满且 !bBreakInto -> 55051;
// nType==4: 直接 -> 55052, false;
// nType==5 (Force): 不在 Force -> 53156; GetForceUserCount<4 -> 53126;
// 尾部 (nType!=5): 已在 Force -> 55067, false; 其余返回 true。
// ============================================================================
bool CWorldProcess::CheckAdmissionMember(int nType, bool bBreakInto,
                                          int& nErrorCode) {
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    if (nType == 1) {
        CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
        if (pGocParty && pGocParty->IsParty()) {
            nErrorCode = 55032;
            return false;
        }
    } else {
        switch (nType) {
        case 2: {
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            if (!pGocParty || !pGocParty->IsParty()) {
                nErrorCode = 55033;
                return false;
            }
            break;
        }
        case 3: {
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            if (pGocParty) {
                if (!pGocParty->IsParty()) {
                    nErrorCode = 55033;
                    return false;
                }
                if (pGocParty->IsFull() && !bBreakInto) {
                    nErrorCode = 55051;
                    return false;
                }
            }
            break;
        }
        case 4:
            nErrorCode = 55052;
            return false;
        case 5: {
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocForce) {
                if (!pGocForce->IsParty()) {
                    nErrorCode = 53156;
                    return false;
                }
                if (pGocForce->GetForceUserCount() < 4) {
                    nErrorCode = 53126;
                    return false;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    if (nType != 5) {
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (pGocForce && pGocForce->IsParty()) {
            nErrorCode = 55067;
            return false;
        }
    }
    return true;
}

// ============================================================================
// IsEnterTheThingEvent - IDA @ 0x140632B10 (AEAA 私有)
// 已精确还原 - The Thing 事件 (TB_MAZE_INFO 0x754F) 进入资格检查:
//   pUser 空 -> false;
//   GetTB_MAZE_INFO(0x754F) 空 -> LogError(MapID) + SendErrorMessage(4,1,55002)
//     + return false;
//   Req_Min_Lv > GetLevel -> SendErrorMessage(4,1,55062) -> false;
//   GetArea()->GetWorldType() (vtable+56):
//     !type || type==2 || type==3, 且
//     (非 CBattleZone) || (IsInSafetyZone && type==2 && GetTBMapID()!=30031)
//       -> true;
//     否则 SendErrorMessage(4,1,55063) -> false。
// ============================================================================
bool CWorldProcess::IsEnterTheThingEvent() {
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    TB_MAZE_INFO* pTB_MazeInfo =
        XGameServer::Instance()->GetResourceMgr().GetTB_MAZE_INFO(0x754F);
    if (!pTB_MazeInfo) {
        LogHelper::LogError(
            "game.contents",
            "IsEnterTheThingEvent error - Invalid map id[MapID:%d]( %d )",
            static_cast<int>(pUser->GetMapInsID().nMapID >> 32), 1952);
        pUser->SendErrorMessage(4, 1, 55002);
        return false;
    }

    if (pTB_MazeInfo->Req_Min_Lv > pUser->GetLevel()) {
        pUser->SendErrorMessage(4, 1, 55062);
        return false;
    }

    XArea* pArea = pUser->GetArea();
    int nCurMoveType = pArea->GetWorldType();
    if ((!nCurMoveType || nCurMoveType == 2 || nCurMoveType == 3)) {
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (!pBattleZone ||
            (pBattleZone->IsInSafetyZone(pUser) && nCurMoveType == 2 &&
             pBattleZone->GetTBMapID() != 30031)) {
            return true;
        }
    }

    pUser->SendErrorMessage(4, 1, 55063);
    return false;
}

// ============================================================================
// GetEventJumpID - IDA @ 0x140632D70 (AEAAHH 单参私有)
// 已精确还原 - The Thing 事件专用 JumpID 映射表。
// 10003/10051/10002 -> 3002101; 10021/10061 -> 3002102;
// 10031 -> 3002103; 10041/11001/30021 -> 3002104; 其他 -> 0。
// 调用点 (ReqWorldEnter lambda14 0x140629a64): 实参为
// GetMapInsID().nMapID << 16 >> 48 (短 MapID)。
// ============================================================================
int CWorldProcess::GetEventJumpID(int nMapID) {
    switch (nMapID) {
    case 10003:
    case 10051:
    case 10002:
        return 3002101;
    case 10021:
    case 10061:
        return 3002102;
    case 10031:
        return 3002103;
    case 10041:
    case 11001:
    case 30021:
        return 3002104;
    default:
        return 0;
    }
}
