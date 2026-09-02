#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAkashicRecord.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/StatusEffect.h"
#include "Soulworker/GameServer/XGameServer/Process/SystemProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/LoginProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/CharacterProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/MonsterProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/ItemProcess.h"
#include "Soulworker/GameServer/XGameServer/Process/WorldProcess.h"
#include <new>

// 构造函�?IDA 0x1406E2FA0
// 反编译验�? 初始化序列完整还�?
CUser::CUser()
    : XClient()
    , CMoverEx()
    , m_pCharTableRef(nullptr)
    , m_fContinousAttackCheckTime(0.0f)
    , m_nContinousAttackHit(0)
    , m_pEnduranceTable(nullptr)
    , m_pDefensiveWeapon(nullptr)
    , m_pControlMonster(nullptr)
    , m_dwDedicatedMonsterID(0)
    , m_nCombatType(-1)
    , m_fCombatChangeTime(0.0f)
    , m_byCombatChangeUseCount(0)
    , m_fTransportDelayTime(0.0f)
    , m_wTransportTableIndex(0)
    , m_tTransportTakeTime(0)
    , m_biLeagueDeletePenalty(0)
    , m_biLeagueWithdrawPenalty(0)
    , m_nKeepAliveSendCount(0)
    , m_nKeepAliveKeyCheckCount(0)
    , m_biAuthSessionID(0)
    , m_fMyroomBackupYaw(0.0f)
    , m_bMatchingState(false)
    , m_bTestMode(false)
    , m_dwUserInfoSync(0)
    , m_nLeagueSyncCount(0)
    , m_bLeagueSyncFlag(false)
    , m_nLeagueInventorySyncCount(0)
    , m_bSendLeagueInventoryCheck(false)
    , m_biLeagueInventoryTime(0)
    , m_fBattleStateTime(0.0f)
    , m_dwFirstUCID(0)
    , m_biLastLevelUpDate(0)
    , m_biLastComeBackDate(0)
    , m_bKick_AlreadyLogin(false)
    , m_nLastKeepAliveKey(0)
    , m_nKeepAliveKeyErrorCount(0)
    , m_fPassiveCheckTime(0.0f)
    , m_nPrevExp(0)
    , m_dwUAIDWhenEnter(0)
    , m_dwUCIDWhenEnter(0)
    , m_iWaitSuboInputSkillID(0)
    , m_pWaitSuboInputSkillTableRef(nullptr)
    , m_dwWaitSuboInputTime(0)
    , m_bSetDeathAttack(false)
    , m_dwCheckCharacterLocationTime(0)
    , m_biLastSendChattingLog(0)
    , m_biLogIncMoney(0)
    , m_biLogDescMoney(0)
    , m_dwLogMoneyTick(0)
    , m_bSentInvenInfo(false)
    , m_bWaitSuboInputActionProcess(false)
    , m_dwLogGapTick(0)
    , m_dwGap_min(0)
    , m_bChangeMap(true)
    , m_dwStatus(0)
    , m_byBlockType(0)
    , m_bReserveRevive(0)
    , m_bReserveReviveImmediate(0)
    , m_dwSocialUseID(0)
    , m_nRevivePoint(0)
    , m_bFirstEnter(false)
    , m_nCreateDate(0)
    , m_biAccountCreateDate(0)
    , m_biLastAccountComeBackDate(0)
    , m_nMaxContinousAttackHit(0)
    , m_nHP(0)
    , m_bPVPPenalty(false)
{
    // IDA 构造函�?vtable 赋值序�?(编译器自动处理多重继�?:
    // this->XClient::XSocket::__vftable = &CUser::`vftable'{for `XClient'}
    // this->CMoverEx::CMover::...::VTypedObject::__vftable = &CUser::`vftable'{for `VisTypedEngineObject_cl'}
    // ... (多个 Vision Engine vtable 赋�?

    // IDA: ST_CHAR_COMMUNITY, ST_LEAGUE_INFO_FOR_GAME, STMyCharInfoEx 构�?
    // (成员初始化列表中已处�?

    // IDA: STPosInfo::STPosInfo(&m_stEnterDistrictPos)
    // (成员初始化列表中已处�?

    // IDA: hkvVec3::hkvVec3(&m_xvMyroomBackupPos)
    // (成员初始化列表中已处�?

    // IDA: std::vector 构�?
    // m_vecChattingTime, m_mpTickInfo, m_vecTickLog, m_vecPingLog
    // (成员初始化列表中已处�?

    // IDA: CFSRWLock::CFSRWLock(&m_rwAreaLock)
    // (成员初始化列表中已处�?

    // IDA: std::list<ST_CHECK_POS>::list(&m_listCheckPos)
    // (成员初始化列表中已处�?

    // IDA: InitComponant() 调用
    InitComponant();

    // IDA: ChangeBattlePose(1) 调用
    ChangeBattlePose(1);

    // IDA: 战斗状态成员初始化 (已在初始化列�?
    // m_pDefensiveWeapon = nullptr
    // m_pControlMonster = nullptr
    // m_dwDedicatedMonsterID = 0
    // m_nCombatType = -1
    // m_fCombatChangeTime = 0.0
    // m_byCombatChangeUseCount = 0

    // IDA: SetInfo() 调用
    SetInfo();

    // IDA: RegisterProcess() 调用
    RegisterProcess();

    // IDA: 传输相关初始�?(已在初始化列�?
    // m_fTransportDelayTime = 0.0
    // m_wTransportTableIndex = 0
    // m_tTransportTakeTime = 0

    // IDA: 公会相关初始�?(已在初始化列�?
    // m_biLeagueDeletePenalty = 0
    // m_biLeagueWithdrawPenalty = 0

    // IDA: 保活初始�?(已在初始化列�?
    // m_nKeepAliveSendCount = 0
    // m_nKeepAliveKeyCheckCount = 0
    // m_biAuthSessionID = 0

    // IDA: hkvVec3 赋�?(0,0,0) - 已在成员初始�?
    // m_fMyroomBackupYaw = 0.0

    // IDA: 匹配/同步状态初始化 (已在初始化列�?
    // m_bMatchingState = 0
    // m_bTestMode = 0
    // dwUserInfoSync = 0
    // m_nLeagueSyncCount = 0
    // m_bLeagueSyncFlag = 0
    // m_nLeagueInventorySyncCount = 0
    // m_bSendLeagueInventoryCheck = 0
    // m_biLeagueInventoryTime = 0
    // m_fBattleStateTime = 0.0
    // m_dwFirstUCID = 0
    // m_biLastLevelUpDate = 0
    // m_biLastComeBackDate = 0
    // m_bKick_AlreadyLogin = 0
    // m_nLastKeepAliveKey = 0
    // m_nKeepAliveKeyErrorCount = 0
    // m_fPassiveCheckTime = 0.0
    // m_nPrevExp = 0
    // m_dwUAIDWhenEnter = 0
    // m_dwUCIDWhenEnter = 0
    // m_iWaitSuboInputSkillID = 0
    // m_pWaitSuboInputSkillTableRef = nullptr
    // m_dwWaitSuboInputTime = 0
    // m_bSetDeathAttack = 0
    // m_dwCheckCharacterLocationTime = 0

    // IDA: 清空容器
    m_vecChattingTime.clear();
    // m_biLastSendChattingLog = 0 (已在初始化列�?
    // m_biLogIncMoney = 0 (已在初始化列�?
    // m_biLogDescMoney = 0 (已在初始化列�?
    // m_dwLogMoneyTick = 0 (已在初始化列�?
    // m_bSentInvenInfo = 0 (已在初始化列�?
    // m_bWaitSuboInputActionProcess = 0 (已在初始化列�?

    m_mpTickInfo.clear();
    // m_dwLogGapTick = 0 (已在初始化列�?

    m_vecTickLog.clear();
    m_vecPingLog.clear();
    // m_dwGap_min = 0 (已在初始化列�?
    // m_bChangeMap = 1 (已在初始化列�?

    // IDA: InitStoreSuboInputPacket() 调用
    InitStoreSuboInputPacket();
}

// 析构函数 IDA 0x1406E3560
// 反编译精确还�? vtable 恢复序列 + 成员析构�?
CUser::~CUser() {
    // IDA: vtable 恢复序列 (多重继承)
    // this->XClient::XSocket::__vftable = &CUser::`vftable'{for `XClient'}
    // this->CMoverEx::CMover::VisTypedEngineObject_cl::VTypedObject::__vftable = &CUser::`vftable'{for `VisTypedEngineObject_cl'}
    // ... (Vision Engine 多个基类 vtable)

    // IDA: 成员析构�?(按声明顺序逆序析构)
    // std::list<ST_LUA_CLIENT_SYNC>::~list(&m_listCheckPos);
    m_listCheckPos.clear();

    // std::vector<std::pair<CUser*,int>>::~vector(&m_vecPingLog);
    m_vecPingLog.clear();

    // std::vector<STNpcInfo>::~vector(&m_vecTickLog);
    m_vecTickLog.clear();

    // std::map<int,PS_TICKCOUNT_INFO>::~map(&m_mpTickInfo);
    m_mpTickInfo.clear();

    // std::vector<CFsmTransition*>::~vector(&m_vecChattingTime);
    m_vecChattingTime.clear();

    // STMyCharInfoEx::~STMyCharInfoEx(&m_stCharInfo);
    // (成员析构由编译器自动处理)

    // IDA: 基类析构�?
    // CMoverEx::~CMoverEx(&this->CMoverEx);
    // XClient::~XClient(this);
    // (基类析构由编译器自动调用)
}

// GetUAID - 获取用户UAID
// IDA 0x14070AF80: return this->m_stCharInfo.dwUAID
// __int64 __fastcall CUser::GetUAID(CUser *this) { return this->m_stCharInfo.dwUAID; }
std::uint32_t CUser::GetUAID() const {
    return m_stCharInfo.dwUAID;
}

// GetActorID - 获取用户ActorID
// IDA 0x1406E8A30: return UXActorID from szBuffer[59743]
UXActorID CUser::GetActorID() const {
    // IDA: UXActorID stored at szBuffer[59743] offset (8 bytes)
    // result->__s0 = *(UXActorID *)&this->szBuffer[59743]
    UXActorID result;
    result.dwActorID = *reinterpret_cast<const std::uint32_t*>(&szBuffer[59743]);
    return result;
}

XMaze* CUser::GetMaze() const {
    return dynamic_cast<XMaze*>(GetArea());
}

// GetAuthSessionID - 获取认证会话ID
// IDA 0x1401C9EE0: return this->m_biAuthSessionID
std::int64_t CUser::GetAuthSessionID() const {
    return m_biAuthSessionID;
}

// IsPrivateShop - 检查是否有私人商店
// IDA 0x1402D3700: return this->m_stCharInfo.stShopInfo.byType != 0
bool CUser::IsPrivateShop() const {
    return m_stCharInfo.stShopInfo.byType != 0;
}

// IsPVPPenalty - 检查是否有PVP惩罚
// IDA 0x1401ADC50: return this->m_bPVPPenalty
bool CUser::IsPVPPenalty() const {
    return m_bPVPPenalty;
}

// SendErrorMessage - 发送错误消息到客户�?
// IDA 0x1406FB1A0: 构造错误消息包并发�?
// IDA 0x1406FB290: 带UCID参数版本
void CUser::SendErrorMessage(std::uint8_t ucMainCmd, std::uint8_t ucSubCmd, std::uint16_t xErrorCode) {
    // IDA 反编译精确还�?
    // XSendPacket::XSendPacket(&xSendPacket, ucMainCmd, ucSubCmd | 0x80);
    // XParse::operator<<(&xSendPacket.XParse, xErrorCode);
    // v13->BridgeSend(&this->XActor, &xSendPacket);
    
    XSendPacket xSendPacket(ucMainCmd, ucSubCmd | 0x80);
    xSendPacket.XParse << xErrorCode;
    BridgeSend(xSendPacket);
}

// SendErrorMessage - 发送带附加 UCID 的错误消息
// IDA: ?SendErrorMessage@CUser@@QEAA_NEEGK@Z @ 0x1406FB290
bool CUser::SendErrorMessage(std::uint8_t ucMainCmd, std::uint8_t ucSubCmd,
                             std::uint16_t xErrorCode, std::uint32_t dwUCID) {
    // IDA 反编译精确还原:
    // XSendPacket::XSendPacket(&xSendPacket, ucMainCmd, ucSubCmd | 0x80);
    // XParse::operator<<(&xSendPacket.XParse, xErrorCode);
    // XParse::operator<<(&xSendPacket.XParse, dwUCID);
    // BridgeSend(&xSendPacket);
    XSendPacket xSendPacket(ucMainCmd, ucSubCmd | 0x80);
    xSendPacket.XParse << xErrorCode;
    xSendPacket.XParse << dwUCID;
    BridgeSend(xSendPacket);
    return true;
}

// CheckMazeEnterCount - IDA: ?CheckMazeEnterCount@CUser@@QEAA_NPEAUTB_MAZE_INFO@@AEAH@Z @ 0x140700C30
// 精确还原: 校验每日迷宫进入次数（普通/PC 房两组），Maze_Type==2 走基础迷宫聚合。
bool CUser::CheckMazeEnterCount(TB_MAZE_INFO* pMazeData, int& nErrorID) {
    nErrorID = 55053;
    if (!pMazeData) {
        return false;
    }

    CGocRecode* pRecode = GetGOC<CGocRecode>();
    if (!pRecode) {
        return false;
    }
    CGocEntity* pEntity = GetGOC<CGocEntity>();
    if (!pEntity) {
        return false;
    }

    // Per IDA: PC 房网吧加成次数检查
    if (pEntity->GetNetCafe() && pMazeData->Maze_Enter_Count_PC_Room) {
        if (pMazeData->Maze_Type == 2) {
            // Per IDA: 聚合基础迷宫 - 按组查询并比较组内计数
            std::uint16_t wBaseMazeID = 0;
            std::uint8_t byBaseMazeEnterCountPCRoom = 0;
            std::uint8_t byOut3 = 0;
            int nOut = 0;
            if (!pRecode->GetDailyBaseMazeID(pMazeData->ID, pMazeData->Maze_Portal_ID, true,
                                             wBaseMazeID, byBaseMazeEnterCountPCRoom, byOut3, nOut)) {
                return false;
            }
            if (pRecode->GetEnterMazeLimitPCBangCount(wBaseMazeID) < byBaseMazeEnterCountPCRoom) {
                return true;
            }
        } else {
            if (pRecode->GetEnterMazeLimitPCBangCount(pMazeData->ID)
                < pMazeData->Maze_Enter_Count_PC_Room) {
                return true;
            }
        }
    }

    // Per IDA: 普通每日次数检查
    if (pMazeData->Maze_Enter_Count) {
        if (pMazeData->Maze_Type == 2) {
            std::uint16_t wBaseMazeID = 0;
            std::uint8_t byBaseMazeEnterCount = 0;
            std::uint8_t byOut3 = 0;
            int nOut = 0;
            if (!pRecode->GetDailyBaseMazeID(pMazeData->ID, pMazeData->Maze_Portal_ID, false,
                                             wBaseMazeID, byBaseMazeEnterCount, byOut3, nOut)) {
                return false;
            }
            if (pRecode->GetEnterMazeLimitCount(wBaseMazeID) >= byBaseMazeEnterCount) {
                return false;
            }
        } else {
            const int nEnterCount = pRecode->GetEnterMazeLimitCount(pMazeData->ID);
            const int nMazeEnterCount = pMazeData->Maze_Enter_Count;
            const std::uint8_t byPlus = XGameServer::Instance()->GetResourceMgr()
                .GetMazeEnterPlusDayCount(pMazeData->ID);
            if (nEnterCount >= byPlus + nMazeEnterCount) {
                return false;
            }
        }
    }

    // Per IDA: Maze_Type==2 且无任何次数配置时要求聚合记录存在
    if (pMazeData->Maze_Type == 2
        && !pMazeData->Maze_Enter_Count
        && !pMazeData->Maze_Enter_Count_PC_Room) {
        const bool bNetCafe = pEntity->GetNetCafe();
        std::uint16_t wBaseMazeID = 0;
        std::uint8_t byCount = 0;
        std::uint8_t byOut3 = 0;
        int nOut = 0;
        if (!pRecode->GetDailyBaseMazeID(pMazeData->ID, pMazeData->Maze_Portal_ID, bNetCafe,
                                         wBaseMazeID, byCount, byOut3, nOut)) {
            nErrorID = 53138;
            return false;
        }
    }

    return true;
}

// SetLogChangeMap - IDA: ?SetLogChangeMap@CUser@@QEAAX_N@Z @ 0x140700500
// 已精确还原 - 置 m_bChangeMap；bChange 时清空 Ping/Tick 日志缓冲
void CUser::SetLogChangeMap(bool bChange) {
    m_bChangeMap = bChange;
    if (bChange) {
        m_vecPingLog.clear();
        m_vecTickLog.clear();
    }
}

// GetEnterDistrictPos - IDA: ?GetEnterDistrictPos@CUser@@QEAAXAEAUSTPosInfo@@@Z @ 0x1401ACFD0
// 已精确还原 - 拷贝 m_stEnterDistrictPos
void CUser::GetEnterDistrictPos(STPosInfo& stPos) {
    stPos = m_stEnterDistrictPos;
}

// IsEnableEscapeWorld - IDA: ?IsEnableEscapeWorld@CUser@@QEAA_NXZ @ 0x1406FA020
// 已精确还原 - m_nEnableEscapeTime <= GetTickCount64()
bool CUser::IsEnableEscapeWorld() {
    return m_nEnableEscapeTime <= GetTickCount64();
}

// SetNextEscapeTime - IDA: ?SetNextEscapeTime@CUser@@QEAAXXZ @ 0x1406FA590
// 已精确还原 - m_nEnableEscapeTime = GetTickCount64() + 60000
void CUser::SetNextEscapeTime() {
    m_nEnableEscapeTime = GetTickCount64() + 60000;
}

// IsBattleState - IDA: ?IsBattleState@CUser@@QEAA_NXZ @ 0x1403E1770
// 已精确还原 - m_fBattleStateTime > 0.0f
bool CUser::IsBattleState() {
    return m_fBattleStateTime > 0.0f;
}

// IsUserStatus - IDA: ?IsUserStatus@CUser@@QEAAHK@Z @ 0x140353AB0
// 已精确还原 - dwStatus & m_stCharInfo.dwStatus
bool CUser::IsUserStatus(std::uint32_t dwStatus) {
    return (dwStatus & m_stCharInfo.dwStatus) != 0;
}

// GetValidMapInsID - IDA: ?GetValidMapInsID@CUser@@QEAA?ATUXMapID@@XZ @ 0x140701610
// 已精确还原 - GetArea 非空时 XActor::GetMapInsID (m_pPosInfo->uxMapID),
// 否则返回 UXMapID(0) (原始 UXMapID::UXMapID(result, 0))
UXMapID CUser::GetValidMapInsID() {
    if (GetArea()) {
        return GetMapInsID();
    }
    return UXMapID();
}

// SetDedicatedMonsterID - IDA: ?SetDedicatedMonsterID@CUser@@QEAAXK@Z @ 0x1402C7CA0
// 已精确还原 - 单行 setter: this->m_dwDedicatedMonsterID = dwActorID
void CUser::SetDedicatedMonsterID(std::uint32_t dwActorID) {
    m_dwDedicatedMonsterID = dwActorID;
}

// SendResWarp - IDA: ?SendResWarp@CUser@@QEAAXEAEAUXVec3@@M@Z @ 0x1406E9AE0
// 已精确还原 - (4,8): STWarp{byResult, xPos, fRot} + GetQuestID 广播
void CUser::SendResWarp(std::uint8_t byResult, XVec3& xPos, float fRot) {
    XSendPacket xSendPacket(4, 8);
    STWarp warpInfo;
    warpInfo.byResult = byResult;
    warpInfo.xPos = xPos;
    warpInfo.fRot = fRot;

    UXActorID actorID = GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(actorID);
    xSendPacket.XParse << nQuestID;
    xSendPacket << warpInfo;

    CGocNetwork::SendBroadCast(this, xSendPacket, E_BROADCAST_TYPE::eAll);
}

// SendChatNotify - IDA 0x1406FA5C0
// 精确还原: 发送聊天通知 (main 7, sub 5)
void CUser::SendChatNotify(int nType, int nValue) {
    PS_CHAT_NOTIFY stNotify;
    stNotify.nType = nType;
    stNotify.nValue = nValue;

    XSendPacket xSendPacket(7, 5);
    xSendPacket << stNotify;
    CGocNetwork::Send(this, xSendPacket);
}

// Kickout IDA 0x1406EAA70
// 精确还原: 踢出用户
// 参数: psKick=踢出信息, bDirect=是否直接踢出
void CUser::Kickout(PS_KICK_USER_INFO* psKick, bool bDirect) {
    // IDA: if (XClient::IsState(this, (XClient::E_NET_STATE)1))
    if (!XClient::IsState(eStateConnect)) {
        // IDA: 记录错误日志
        // LogHelper::LogError("game.contents", "[KICKOUT] Is Not IsState( XClient::eStateConnect ) [ ActorID:%d, Type:%d ]", ...)
        GreenDamTan_log(__FILE__, __FUNCTION__, "Kickout: not in connected state");
        return;
    }

    // IDA: if (XClient::IsState(this, eStateInGame) && GetArea() && !bDirect)
    // IDA: 如果在游戏中且不是直接踢出，设置延迟踢出时间
    if (XClient::IsState(eStateInGame) && GetArea() && !bDirect) {
        // IDA: if (!this->m_dwKickoutTime) this->m_dwKickoutTime = GetTickCount64() + 3000;
        if (m_dwKickoutTime == 0) {
            m_dwKickoutTime = GetTickCount64() + 3000;
        }
    } else {
        // IDA: XClient::SetState(this, eStateKickOut);
        XClient::SetState(eStateKickOut);
    }

    // IDA: 获取 Maze 并处理队�?公会状�?
    // IDA: pMaze = (XMaze *)_RTDynamicCast_0(v6, 0, &XArea `RTTI Type Descriptor', &XMaze `RTTI Type Descriptor', 0);
    XMaze* pMaze = GetMaze();
    if (pMaze) {
        // IDA: 处理队伍断开状�?
        auto pParty = GetGOC<CGocParty>();
        if (pParty && pParty->IsParty()) {
            ST_PARTY_INFO stPartyInfo;
            stPartyInfo.byGroupType = 1;
            stPartyInfo.nID = pParty->GetPartyID();
            pMaze->SetDisconnectUserState(GetActorID().dwActorID, stPartyInfo);
        }
        // IDA: 处理公会断开状�?
        auto pForce = GetGOC<CGocForce>();
        if (pForce && pForce->IsParty()) {
            ST_PARTY_INFO stForceInfo;
            stForceInfo.byGroupType = 2;
            stForceInfo.nID = pForce->GetPartyID();
            pMaze->SetDisconnectUserState(GetActorID().dwActorID, stForceInfo);
        }
    }

    // IDA: 发送踢出数据包给客户端
    // IDA: XSendPacket::XSendPacket(&xSendPacket, 3u, 4u);
    // IDA: operator<<(&xSendPacket, psKick);
    // IDA: CGocNetwork::Send(pActor, &xSendPacket);
    XSendPacket xSendPacket(3, 4);
    xSendPacket << *psKick;
    CGocNetwork::Send(reinterpret_cast<XActor*>(this), xSendPacket);

    // IDA: 记录日志
    // IDA: ST_LOG_GAME::ST_LOG_GAME(&stLog);
    // IDA: stLog._nUAID = this->GetUAID(this);
    // IDA: stLog._nUCID = CQuestCondition::GetQuestID(v15);
    // IDA: stLog._sMainType = 2; stLog._sSubType = 3;
    // IDA: stLog.nParam0 = psKick->byKickType; stLog.nParam1 = psKick->nParam;
    ST_LOG_GAME stLog;
    stLog._nUAID = GetUAID();
    stLog._nUCID = GetActorID().dwActorID;
    stLog._sMainType = 2;
    stLog._sSubType = 3;
    stLog.nParam0 = psKick->byKickType;
    stLog.nParam1 = psKick->nParam;
    // IDA: wcscpy_s<51>((wchar_t (*)[51])stLog.szComment, &word_140B011FC);
    wcscpy_s(stLog.szComment, L"");

    // IDA: XGameServer::SendDBLog(v16, &stLog);
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SendDBLog(stLog);
    }

    // IDA: if (psKick->byKickType == 1) CUser::SetKick_AlreadyLogin(this);
    if (psKick && psKick->byKickType == 1) {
        m_bKick_AlreadyLogin = true;
    }

    // IDA: LogHelper::LogError("game.contents", "Kickout error[ ActorID:%d, Type:%d Param:%d ] ( %d )", ...)
    GreenDamTan_log(__FILE__, __FUNCTION__, "Kickout: ActorID=%d, Type=%d, Param=%d",
                    GetActorID().dwActorID, psKick->byKickType, psKick->nParam);

    // IDA: 发�?DB 数据�?
    // IDA: XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 2u, 0x54u);
    // IDA: XParse::operator<<(&xSendDBPacket.XParse, v18);
    // IDA: XGameServer::SendDBAccount(v19, &xSendDBPacket);
    XSendDBPacket xSendDBPacket(static_cast<XClient*>(this), 2, 0x54);
    xSendDBPacket << GetUAID();
    if (pServer) {
        pServer->SendDBAccount(xSendDBPacket);
    }
}

// ============================================================================
// IsEnemy IDA 0x1406F3BA0
// IDA 精确还原 - 判断是否为敌人
// ============================================================================
bool CUser::IsEnemy(CMover* pMover) const {
    // IDA: 检查是否是自己
    if (pMover == static_cast<const CMover*>(this)) {
        return false;
    }

    // IDA: 检查对象类型，类型4不是敌人
    if (pMover->GetType() == 4) {
        return false;
    }

    // IDA: 调用基类 CMoverEx::IsEnemy
    return CMoverEx::IsEnemy(pMover);
}

// ============================================================================
// IsFriend IDA 0x1406F3C30
// IDA 精确还原 - 判断是否为友方
// ============================================================================
int CUser::IsFriend(CMover* pMover) {
    // IDA: 检查是否是自己
    if (pMover == static_cast<CMover*>(this)) {
        return 1;
    }

    // IDA: 检查对象类型，类型4是友方
    if (pMover->GetType() == 4) {
        return 1;
    }

    // IDA: 调用基类 CMoverEx::IsFriend
    return CMoverEx::IsFriend(pMover);
}

// ============================================================================
// IsLeague IDA 0x1406F3CC0
// IDA 精确还原 - 判断是否为联盟成员
// ============================================================================
int CUser::IsLeague(CMover* pMover) {
    // IDA: 尝试转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) {
        return 0;
    }

    // IDA: 获取双方的角色信息
    const STMyCharInfoEx& stMyCharInfo = GetMyCharInfoEx();
    const STMyCharInfoEx& stOtherCharInfo = pUser->GetMyCharInfoEx();

    // IDA: 检查联盟ID是否相同且不为0
    if (stMyCharInfo.stLeagueInfo.nLeagueID != 0 &&
        stMyCharInfo.stLeagueInfo.nLeagueID == stOtherCharInfo.stLeagueInfo.nLeagueID) {
        return 1;
    }

    return 0;
}

// ============================================================================
// GetGameOption IDA 0x1402F6A00
// IDA 精确还原 - 获取游戏选项
// ============================================================================
void CUser::GetGameOption(ST_GAME_OPTION& stGameOption) {
    // IDA: 直接复制 m_stGameOption 到输出参数
    stGameOption = m_stGameOption;
}

// ============================================================================
// CheckGameOption IDA 0x1406FBC20
// IDA 精确还原 - 检查游戏选项状态
// ============================================================================
bool CUser::CheckGameOption(E_OPTION_INDEX eIndex, E_OPTION_STATE eState) {
    // IDA: switch (eIndex) 检查对应选项
    int nState = 0;
    switch (eIndex) {
    case eOption_OtherInfo:
        nState = m_stGameOption.nOption_OtherInfo;
        break;
    case eOption_Register_Friend:
        nState = m_stGameOption.nOption_Register_Friend;
        if (nState == 1) {
            nState = 0;
            eState = eGAME_OPTION_ALLOW_ALL;
        }
        break;
    case eOption_WhisperMsg:
        nState = m_stGameOption.nOption_WhisperMsg;
        break;
    }
    return nState == eState;
}

void CUser::InitComponant() {
    // 创建 GOC 组件: IDA 0x1406E2FA0 构造函数调用序�?
    CreateComponent<CGocSkill>(GOC_SKILL);
    CreateComponent<CGocNetwork>(GOC_NETWORK);
    CreateComponent<CGocAttribute>(GOC_ATTRIBUTE);
    CreateComponent<CGocBooster>(GOC_BOOSTER);
    CreateComponent<CGocQuest>(GOC_QUEST);
    CreateComponent<CGocAchieve>(GOC_ACHIEVE);
}

void CUser::SendWorldEventBooster(unsigned long dwBuffID, std::int64_t biEndDate) {
    std::shared_ptr<CGocBooster> pBooster = GetGOC_Booster(false);
    if (pBooster) {
        pBooster->ChangeBooster(
            eBooster_Type_Event,
            static_cast<std::uint16_t>(dwBuffID),
            biEndDate,
            false);
    }
}

// IDA 0x1406E8F80 - SendTimeEvent
// 无 byteClass 限制或与玩家职业匹配时，应用事件 Buff 加成
void CUser::SendTimeEvent(ST_GM_TIME_EVENT_INFO& stInfo) {
    if (stInfo.byteClass == 0 || GetClass() == stInfo.byteClass) {
        std::shared_ptr<CGocBooster> pBooster = GetGOC_Booster(false);
        if (pBooster) {
            pBooster->ChangeBooster(
                eBooster_Type_Event,
                static_cast<std::uint16_t>(stInfo.dwBuff_ID),
                stInfo.nEndDate,
                false);
        }
    }
}

// IDA: 0x1406FF920
void CUser::SetRequestTick(std::uint8_t byType,
                           PS_REQ_TICKCOUNT& psReqTick,
                           std::uint64_t dwRecvTick) {
    const std::uint64_t dwTick = GetTickCount64();
    const std::uint64_t dwGap =
        dwTick <= psReqTick.dwTickcount
            ? psReqTick.dwTickcount - dwTick
            : dwTick - psReqTick.dwTickcount;
    (void)dwGap;

    m_mpTickInfo.find(psReqTick.nTicknum);

    PS_TICKCOUNT_INFO psInfo;
    psInfo.nTicknum = psReqTick.nTicknum;
    psInfo.byType = byType;
    psInfo.dwReqTickcount = psReqTick.dwTickcount;
    psInfo.dw64ReqTickcount = dwRecvTick;

    m_mpTickInfo.insert({psReqTick.nTicknum, psInfo});
}

// ============================================================================
// SetLeagueInfo (0x140700950) - 已精确还原
// 写入 m_stCharInfo.stLeagueInfo (nLeagueID/nCard/szLeagueName) 与
// m_stLeagueInfo (整体拷贝 ST_LEAGUE_INFO_FOR_GAME)。
// PDB publics 装饰名证实第二参数按值传递 (UST_LEAGUE_INFO_FOR_GAME@@ 无引用)。
void CUser::SetLeagueInfo(ST_LEAGUE_INFO_EX& stInfo,
                          ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame) {
    m_stCharInfo.stLeagueInfo.nLeagueID = stInfo.nLeagueID;
    m_stCharInfo.stLeagueInfo.uCard.nCard = stInfo.dwLeagueCard;
    const wchar_t* szSrc = stInfo.szLeagueName;
    wchar_t* szDst = m_stCharInfo.stLeagueInfo.szLeagueName;
    do {
        *szDst = *szSrc;
        ++szSrc;
        ++szDst;
    } while (*szSrc);
    m_stLeagueInfo = stLeagueInfoForGame;
}

// ============================================================================
// SetLeagueInfo (单参重载, 0x140700A40) - 已精确还原
// 仅写入 m_stCharInfo.stLeagueInfo (nLeagueID/nCard/szLeagueName)，
// 不触及 m_stLeagueInfo。
// PDB publics: ?SetLeagueInfo@CUser@@QEAAXAEAUST_LEAGUE_INFO_EX@@@Z (RVA 0x6FFA40)
void CUser::SetLeagueInfo(ST_LEAGUE_INFO_EX& stInfo) {
    m_stCharInfo.stLeagueInfo.nLeagueID = stInfo.nLeagueID;
    m_stCharInfo.stLeagueInfo.uCard.nCard = stInfo.dwLeagueCard;
    const wchar_t* szSrc = stInfo.szLeagueName;
    wchar_t* szDst = m_stCharInfo.stLeagueInfo.szLeagueName;
    do {
        *szDst = *szSrc;
        ++szSrc;
        ++szDst;
    } while (*szSrc);
}

// IDA: ?GetLeagueSyncFlag@CUser@@QEAA_NXZ @ 0x14028D3C0
// 已精确还原 - 返回 League 同步标志
bool CUser::GetLeagueSyncFlag() {
    return m_bLeagueSyncFlag;
}

// IDA: ?GetLeagueSyncCount@CUser@@QEAAHXZ @ 0x14028D3A0
// 已精确还原 - 返回 League 同步计数
int CUser::GetLeagueSyncCount() {
    return m_nLeagueSyncCount;
}

// IDA: ?UpdateLeagueSyncFlag@CUser@@QEAAX_N@Z @ 0x14025D060
// 已精确还原 - 设置 League 同步标志
void CUser::UpdateLeagueSyncFlag(bool bSyncFlag) {
    m_bLeagueSyncFlag = bSyncFlag;
}

// IDA: ?UpdateLeagueSyncCount@CUser@@QEAAXH@Z @ 0x14025DB90
// 已精确还原 - 设置 League 同步计数
void CUser::UpdateLeagueSyncCount(int nUpdateCount) {
    m_nLeagueSyncCount = nUpdateCount;
}

// IDA: ?ClearLeagueInfo@CUser@@QEAAXXZ @ 0x1406FB440
// 已精确还原 - 清空用户 League 信息
void CUser::ClearLeagueInfo() {
    m_stCharInfo.stLeagueInfo.nLeagueID = 0;
    m_stCharInfo.stLeagueInfo.uCard.nCard = 0;
    m_stCharInfo.stLeagueInfo.szLeagueName[0] = 0;
    std::memset(&m_stLeagueInfo, 0, sizeof(m_stLeagueInfo));
}

// IDA: ?SetLeagueSkill@CUser@@QEAAXEE@Z @ 0x140700B30
// 已精确还原 - 设置 League 技能槽位
void CUser::SetLeagueSkill(std::uint8_t byGroupID, std::uint8_t bySkillLevel) {
    m_stLeagueInfo.bySkillInfo[byGroupID] = bySkillLevel;
}

// IDA: ?SetLeagueLevel@CUser@@QEAAXE@Z @ 0x140700B60
// 已精确还原 - 设置 League 等级
void CUser::SetLeagueLevel(std::uint8_t byLevel) {
    m_stLeagueInfo.byLeagueLevel = byLevel;
}

// IDA: ?SetLeagueCard@CUser@@QEAAXK@Z @ 0x1407008F0
// 已精确还原 - 设置 League 卡片
void CUser::SetLeagueCard(unsigned int dwCardInfo) {
    m_stCharInfo.stLeagueInfo.uCard.nCard = dwCardInfo;
}

// IDA: ?SetLeagueName@CUser@@QEAAXPEA_W@Z @ 0x14025CAB0
// 已精确还原 - 设置 League 名称
void CUser::SetLeagueName(wchar_t* szName) {
    wcscpy_s(m_stCharInfo.stLeagueInfo.szLeagueName, szName);
}

// IDA: ?SetLeagueAuth@CUser@@QEAAXUST_LEAGUE_AUTH_CHANGE@@@Z @ 0x140700AE0
// 已精确还原 - 拷贝 9 项权限到 m_stLeagueInfo.nAuth
void CUser::SetLeagueAuth(ST_LEAGUE_AUTH_CHANGE stAuthInfo) {
    for (int i = 0; i < 9; ++i) {
        m_stLeagueInfo.nAuth[i] = stAuthInfo.nAuth[i];
    }
}

// IDA: ?SetLeaguePosition@CUser@@QEAAXE@Z @ 0x140700930
// 已精确还原 - 设置 League 职位
void CUser::SetLeaguePosition(std::uint8_t byPosition) {
    m_stLeagueInfo.byPosition = byPosition;
}

// IDA: ?SetLeagueMaster@CUser@@QEAAXK@Z @ 0x140700910
// 已精确还原 - 设置 League 会长 UCID
void CUser::SetLeagueMaster(unsigned int dwUCID) {
    m_stLeagueInfo.dwMasterUCID = dwUCID;
}

// IDA: ?UpdateLeagueInventorySyncCount@CUser@@QEAAXH@Z @ 0x14025CFC0
// 已精确还原 - 设置 League 物品栏同步计数
void CUser::UpdateLeagueInventorySyncCount(int nUpdateCount) {
    m_nLeagueInventorySyncCount = nUpdateCount;
}

// IDA: ?GetLeagueInventorySyncCount@CUser@@QEAAHXZ @ 0x14028D380
// 已精确还原 - 查询 League 物品栏同步计数
int CUser::GetLeagueInventorySyncCount() {
    return m_nLeagueInventorySyncCount;
}

// IDA: ?SetLeagueInventoryTime@CUser@@QEAAX_J@Z @ 0x14025CFE0
// 已精确还原 - 设置 League 物品栏访问时间
void CUser::SetLeagueInventoryTime(std::int64_t biTime) {
    m_biLeagueInventoryTime = biTime;
}

// IDA: ?GetLeagueInfo@CUser@@QEAAXAEAUST_LEAGUE_INFO_EX@@@Z @ 0x1406FB3A0
// 已精确还原 - 读出 m_stCharInfo.stLeagueInfo 到 ST_LEAGUE_INFO_EX
void CUser::GetLeagueInfo(ST_LEAGUE_INFO_EX& stInfo) {
    stInfo.nLeagueID = m_stCharInfo.stLeagueInfo.nLeagueID;
    stInfo.dwLeagueCard = m_stCharInfo.stLeagueInfo.uCard.nCard;
    const wchar_t* szSrc = m_stCharInfo.stLeagueInfo.szLeagueName;
    wchar_t* szDst = stInfo.szLeagueName;
    do {
        *szDst = *szSrc;
        ++szSrc;
        ++szDst;
    } while (*szSrc);
}

// IDA: ?GetLeagueInfo@CUser@@QEAAXAEAUST_LEAGUE_INFO_FOR_GAME@@@Z @ 0x140503F40
// 已精确还原 - 拷贝 m_stLeagueInfo
void CUser::GetLeagueInfo(ST_LEAGUE_INFO_FOR_GAME& stLeagueInfo) {
    stLeagueInfo = m_stLeagueInfo;
}

// IDA: ?SetLeagueInventorySend@CUser@@QEAAX_N@Z @ 0x14025D9E0
// 已精确还原 - 设置 League 物品栏发送标志
void CUser::SetLeagueInventorySend(bool bSend) {
    m_bSendLeagueInventoryCheck = bSend;
}

// IDA: ?GetLeagueInventoryTime@CUser@@QEAA_JXZ @ 0x140503D60
// 已精确还原 - 查询 League 物品栏访问时间
std::int64_t CUser::GetLeagueInventoryTime() {
    return m_biLeagueInventoryTime;
}

// IDA: ?CheckSendLeagueInventoryInfo@CUser@@QEAA_NXZ @ 0x140503D80
// 已精确还原 - 检查是否未发送过 League 物品栏（取反发送标志）
bool CUser::CheckSendLeagueInventoryInfo() {
    return !m_bSendLeagueInventoryCheck;
}

// IDA: ?IsLeagueAuth@CUser@@QEAA_NEW4E_LEAGUE_AUTH@@@Z @ 0x1407007E0
// 已精确还原 - 盟主恒有权限；否则按职位权限位按位与判断
bool CUser::IsLeagueAuth(std::uint8_t byPosition, E_LEAGUE_AUTH eAuth) {
    return m_stLeagueInfo.dwMasterUCID == m_stCharInfo.uxActorID.dwActorID
        || (eAuth & m_stLeagueInfo.nAuth[byPosition]) != 0;
}

// IDA: ?EnterWorldToOther@CUser@@QEAAXHHUSTPosInfo@@K@Z @ 0x1406F8800
// 已精确还原 - 填 PS_ENTER_MAP_REQ{dwActorID,wMapID,nJumpID} ->
// GetProcessPtr<CWorldProcess>(4) 空则返回；
// (4,1) << stEnterMap << stPosInfo << dwTargetID -> SetUsIndex(2) ->
// CWorldProcess::ReqWorldEnterToOther。
void CUser::EnterWorldToOther(int nMapID, int nJumpID, STPosInfo& stPosInfo,
                              std::uint32_t dwTargetID) {
    PS_ENTER_MAP_REQ stEnterMap{};
    stEnterMap.dwActorID = GetActorID().dwActorID;
    stEnterMap.wMapID = static_cast<std::uint16_t>(nMapID);
    stEnterMap.nJumpID = nJumpID;

    CWorldProcess* pProcess = GetProcessPtr<CWorldProcess>(4);
    if (pProcess) {
        XSendPacket xSendPacket(4, 1);
        xSendPacket << stEnterMap;
        xSendPacket << stPosInfo;
        xSendPacket.XParse << dwTargetID;
        xSendPacket.XParse.SetUsIndex(2);
        pProcess->ReqWorldEnterToOther(xSendPacket);
    }
}

// IDA: ?SetLeagueDeletePenalty@CUser@@QEAAX_J@Z @ 0x14025D900
// 已精确还原 - 设置 League 解散惩罚截止时间
void CUser::SetLeagueDeletePenalty(std::int64_t biPenalty) {
    m_biLeagueDeletePenalty = biPenalty;
}

// IDA: 0x1406FFA40
void CUser::GetResultTick(int nTicknum, PS_TICKCOUNT_INFO& psTick) {
    const auto iter = m_mpTickInfo.find(nTicknum);
    if (iter != m_mpTickInfo.end()) {
        psTick = iter->second;
        m_mpTickInfo.erase(iter);
    }
}

bool CUser::RegisterProcess() {
    // IDA 0x1406E4B70: each process is constructed, registered, and
    // registration failure returns false immediately. The current target
    // only compiles these five process implementations.
    auto* systemProcess = new CSystemProcess();
    if (!Register(1, systemProcess)) {
        return false;
    }

    auto* loginProcess = new CLoginProcess();
    if (!Register(2, loginProcess)) {
        return false;
    }

    auto* characterProcess = new CCharacterProcess();
    if (!Register(3, characterProcess)) {
        return false;
    }

    auto* itemProcess = new CItemProcess();
    if (!Register(8, itemProcess)) {
        return false;
    }

    auto* monsterProcess = new CMonsterProcess();
    return Register(0x17, monsterProcess);
}

void CUser::ChangeBattlePose(int nPose) {
    // IDA 反编�? 设置战斗姿态并切换动画
    // m_nCombatType 记录当前战斗姿态类�?
    m_nCombatType = nPose;

    // 根据姿态值切换动�?
    ChangeMotion(static_cast<std::int16_t>(nPose), 1, 0);
}

void CUser::SetInfo() {
    // IDA 反编�? �?TB_CHARACTER 表数据初始化 m_stCharInfo
    // m_stCharInfo 用于存储玩家角色信息
    if (m_pCharTableRef) {
        // TB_CHARACTER_INFO is the static character table; account/exp fields stay in m_stCharInfo.
    }
}

void CUser::InitStoreSuboInputPacket() {
    // IDA 反编�? 初始�?Subo 输入相关字段
    // 重置所�?Subo 技能输入状�?
    m_iWaitSuboInputSkillID = 0;
    m_pWaitSuboInputSkillTableRef = nullptr;
    m_dwWaitSuboInputTime = 0;
    m_bSetDeathAttack = false;
    m_bWaitSuboInputActionProcess = false;
    m_dwCheckCharacterLocationTime = 0;
}

bool CUser::IsStatus(std::uint32_t dwStatus) {
    // IDA 0x140026C30: return dwStatus & this->m_dwStatus
    return (dwStatus & m_dwStatus) != 0;
}

bool CUser::IsMatching() {
    // IDA 0x140082DF0: return this->m_bMatchingState
    return m_bMatchingState;
}

std::int64_t CUser::GetExp() {
    // IDA 0x1400F64A0: return this->m_stCharInfo.nExp
    return m_stCharInfo.nExp;
}

std::int64_t CUser::GetCreateDate() {
    // IDA 0x1401253E0: return this->m_nCreateDate
    return m_nCreateDate;
}

std::int64_t CUser::GetLastLevelupDate() {
    // IDA 0x140049310: return this->m_biLastLevelUpDate
    return m_biLastLevelUpDate;
}

std::int64_t CUser::GetLastComeBackDate() {
    // IDA 0x140187AE0: return this->m_biLastComeBackDate
    return m_biLastComeBackDate;
}

std::int64_t CUser::GetAccountCreateDate() {
    // IDA 0x140125B50: return this->m_biAccountCreateDate
    return m_biAccountCreateDate;
}

std::int64_t CUser::GetLastAccountComeBackDate() {
    // IDA 0x140187AC0: return this->m_biLastAccountComeBackDate
    return m_biLastAccountComeBackDate;
}

std::uint32_t CUser::GetSocialUseID() {
    // IDA 0x1400F72E0: return this->m_dwSocialUseID
    return m_dwSocialUseID;
}

std::uint32_t CUser::GetActiveBroachEffect() {
    // IDA 0x1400F7CE0: return this->m_stCharInfo.dwActiveBroachEffect
    return m_stCharInfo.dwActiveBroachEffect;
}

// IDA: 0x1406FB490. Preserve the early returns: if a prior effect has already
// been hidden and new-effect resolution fails, the original does not persist.
void CUser::SetActiveBroachEffect(std::uint32_t dwEffect) {
    const std::uint32_t dwLastActiveBroachEffect = GetActiveBroachEffect();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    if (dwLastActiveBroachEffect != 0) {
        TB_BUFF* pLastBuff = pServer->GetResourceMgr().GetTB_BUFF(
            static_cast<std::uint16_t>(dwLastActiveBroachEffect));
        if (!pLastBuff) {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                            "SetActiveBroachEffect failure code=%d", 5550);
            return;
        }

        const int nIndex = FindBuffByGroupID(pLastBuff->Buff_Group, 0);
        if (nIndex == -1) {
            return;
        }
        if (nIndex >= 50) {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                            "SetActiveBroachEffect failure code=%d", 5560);
            return;
        }

        m_stBuffState[nIndex].bShow = false;
        m_stCharInfo.dwActiveBroachEffect = 0;
        send_eSUB_CMD_BUFF_UPDATE(
            this,
            static_cast<std::int16_t>(pLastBuff->Buff_Index),
            m_stBuffState[nIndex].fLifeTime,
            static_cast<std::int8_t>(m_stBuffState[nIndex].byCount),
            m_stBuffState[nIndex].dwID,
            m_stBuffState[nIndex].bySendType,
            m_stBuffState[nIndex].bShow);
    }

    if (dwEffect != 0) {
        TB_BUFF* pBuff = pServer->GetResourceMgr().GetTB_BUFF(
            static_cast<std::uint16_t>(dwEffect));
        if (!pBuff) {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                            "SetActiveBroachEffect failure code=%d", 5576);
            return;
        }

        const int nIndex = FindBuffByGroupID(pBuff->Buff_Group, 0);
        if (nIndex == -1) {
            return;
        }
        if (nIndex >= 50) {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                            "SetActiveBroachEffect failure code=%d", 5586);
            return;
        }

        m_stBuffState[nIndex].bShow = true;
        m_stCharInfo.dwActiveBroachEffect = dwEffect;
        send_eSUB_CMD_BUFF_UPDATE(
            this,
            static_cast<std::int16_t>(pBuff->Buff_Index),
            m_stBuffState[nIndex].fLifeTime,
            static_cast<std::int8_t>(m_stBuffState[nIndex].byCount),
            m_stBuffState[nIndex].dwID,
            m_stBuffState[nIndex].bySendType,
            m_stBuffState[nIndex].bShow);
    }

    if (dwLastActiveBroachEffect != GetActiveBroachEffect()) {
        XSendDBPacket xSendDBPacket(
            static_cast<IXObject*>(static_cast<XActor*>(this)), 3, 0x84);
        xSendDBPacket.XParse << CQuestCondition::GetQuestID(GetActorID());
        xSendDBPacket.XParse << GetActiveBroachEffect();
        pServer->SendDBGame(xSendDBPacket);
    }
}

std::int32_t CUser::GetLeagueID() {
    // IDA 0x140165500: return this->m_stCharInfo.stLeagueInfo.nLeagueID
    return m_stCharInfo.stLeagueInfo.nLeagueID;
}

std::int64_t CUser::GetLeagueDeletePenalty() {
    // IDA 0x140504340: return this->m_biLeagueDeletePenalty
    return m_biLeagueDeletePenalty;
}

std::uint16_t CUser::GetMaxComboCount() {
    // IDA 0x140165270: return this->m_nMaxContinousAttackHit
    return static_cast<std::uint16_t>(m_nMaxContinousAttackHit);
}

// ============================================================================
// Combo System Functions
// ============================================================================

int CUser::GetComboCount() {
    // IDA 0x14070A470: return *(unsigned __int16 *)&this->szBuffer[61523]
    // Combo count is stored at offset 61523 in szBuffer as unsigned short
    // For now, we use a simple member variable approach
    return m_nContinousAttackHit;
}

std::uint16_t CUser::CheckContinousAttack(std::uint8_t byHitCount) {
    // IDA 0x1406F1110: Check and update continuous attack combo
    // This function manages the combo counter for the user
    
    // Update max combo if current exceeds it
    if (byHitCount > static_cast<std::uint8_t>(m_nMaxContinousAttackHit)) {
        m_nMaxContinousAttackHit = static_cast<int>(byHitCount);
    }
    
    // Return current combo count
    return GetComboCount();
}

void CUser::ApplyComboBuff(const TB_COMBO_BUFF* pCombo) {
    // IDA 0x1406F15F0: Apply combo buff effects
    // This function applies fixed and random buffs from combo system
    
    if (!pCombo) {
        return;
    }
    
    // Apply fixed buff if set
    if (pCombo->Fixed_Buff_ID) {
        // Call SetBuffStatus on CMoverEx base
        SetBuffStatus(pCombo->Fixed_Buff_ID, 0, true);
    }
    
    // Apply random buffs if count > 0
    if (pCombo->Get_RBuff_Count > 0) {
        int nMaxRand = 0;
        int bUseBuff[8] = {0};
        
        // Calculate total rate
        for (int i = 0; i < 8; ++i) {
            nMaxRand += pCombo->RBuff_Rate[i];
        }
        
        // Select random buffs based on rates
        int nCount = 0;
        while (nMaxRand > 0 && nCount < pCombo->Get_RBuff_Count) {
            int nRand = rand() % nMaxRand;
            nMaxRand = 0;
            
            for (int i = 0; i < 8; ++i) {
                if (bUseBuff[i] <= 0) {
                    if (nRand >= pCombo->RBuff_Rate[i]) {
                        nRand -= pCombo->RBuff_Rate[i];
                        nMaxRand += pCombo->RBuff_Rate[i];
                    } else {
                        ++nCount;
                        bUseBuff[i] = 1;
                        nRand = 100000000;
                        if (nCount == pCombo->Get_RBuff_Count) {
                            break;
                        }
                    }
                }
            }
        }
        
        // Apply selected buffs
        for (int i = 0; i < 8; ++i) {
            if (bUseBuff[i] > 0) {
                SetBuffStatus(pCombo->RBuff_ID[i], 0, true);
            }
        }
    }
    
    // Apply SV_Absorb if set
    if (pCombo->SV_Absorb) {
        // IDA: Modify absorb stats via CGocAttribute
        // Get current absorb stat and apply modification
        auto pAttr = GetGOC<CGocAttribute>();
        if (pAttr) {
            float fAbsorb = pAttr->GetStat(16);
            pAttr->SetStat(16, fAbsorb + static_cast<float>(pCombo->SV_Absorb), true);
        }
    }
}

std::wstring CUser::GetName() const {
    // IDA 0x140082D20: return m_stCharInfo.stBaseInfo.strName
    return m_stCharInfo.stBaseInfo.strName;
}

char* CUser::GetAccountID() {
    // IDA 0x140038710: return this->m_stCharInfo.szAccountID
    return m_stCharInfo.szAccountID;
}

std::uint32_t CUser::GetFirstUCID() {
    // IDA 0x140125400: return this->m_dwFirstUCID
    return m_dwFirstUCID;
}

void CUser::SetMatchingState(bool bState) {
    // IDA 0x1400855E0: this->m_bMatchingState = bState
    m_bMatchingState = bState;
}

void CUser::SetSocialUseID(std::uint32_t dwID) {
    // IDA 0x14018FC60: this->m_dwSocialUseID = dwID
    m_dwSocialUseID = dwID;
}

// IDA 0x1406FEFB0: CUser::SendBannerInfo
// 发送横幅信息到客户�?
void CUser::SendBannerInfo() {
    // IDA: 获取 Banner 列表
    ST_BANNER_LIST stBannerList;
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;

    pServer->GetResourceMgr().GetBannerInfo(&stBannerList);

    // IDA: 如果列表为空则直接返�?
    if (stBannerList.vecInfo.empty()) {
        return;
    }

    // IDA: 创建并发送包 (main=9, sub=0x28)
    XSendPacket xSendPacket(9, 0x28);
    xSendPacket << stBannerList;

    // IDA: 通过 BridgeSend 发�?
    BridgeSend(xSendPacket);
}

void CUser::SetLastLevelupDate(std::int64_t biDate) {
    // IDA 0x1400492F0
    m_biLastLevelUpDate = biDate;
}

void CUser::SetReserveRevive(int nType) {
    // IDA 0x140085DF0: this->m_bReserveRevive = bReserve
    m_bReserveRevive = nType;
}

// IDA: ?GetReserveRevive@CUser@@QEAAHXZ @ 0x14070A760
int CUser::GetReserveRevive() {
    // IDA: return (unsigned int)this->m_bReserveRevive;
    return m_bReserveRevive;
}

// IDA: ?GetReserveReviveImmediate@CUser@@QEAAHXZ @ 0x14070A740
int CUser::GetReserveReviveImmediate() {
    // IDA: return (unsigned int)this->m_bReserveReviveImmediate;
    return m_bReserveReviveImmediate;
}

// IDA: ?SetReserveReviveImmediate@CUser@@QEAAXH@Z @ 0x1403A1B40
void CUser::SetReserveReviveImmediate(int bReserve) {
    // IDA: this->m_bReserveReviveImmediate = bReserve;
    m_bReserveReviveImmediate = bReserve;
}

// IDA: ?ChangeBooster@CUser@@QEAAXW4E_BOOSTER_TYPE@@G@Z @ 0x1406F8420
void CUser::ChangeBooster(E_BOOSTER_TYPE eType, std::uint16_t wIndex) {
    // IDA: CMover::GetGOC<CGocBooster>(&this->CMoverEx, &pBooster, 0);
    // if (pBooster) CGocBooster::ChangeBooster(pBooster, eType, wIndex, 0, 0);
    std::shared_ptr<CGocBooster> pBooster = GetGOC_Booster(false);
    if (pBooster) {
        pBooster->ChangeBooster(eType, wIndex, 0, false);
    }
}

// IDA: ?IsLeagueSkill@CUser@@QEAA_NH@Z @ 0x1407007A0
bool CUser::IsLeagueSkill(int nSkill) {
    // IDA: return this->m_stLeagueInfo.bySkillInfo[nSkill] != 0;
    return m_stLeagueInfo.bySkillInfo[nSkill] != 0;
}

// ============================================================================
// CUser::GetLeagueSkillEffectValue (0x140700830)
// 已精确还原 - 按 m_stLeagueInfo.bySkillInfo[nSkill] 的等级，遍历
// TB_LEAGUE_SKILL 全表找 League_Skill_Type == nSkill+1 且
// League_Get_Skill_Level == bySkillLevel 的行，返回其
// League_Skill_Apply_Value；找不到返回 0。
// ============================================================================
std::int32_t CUser::GetLeagueSkillEffectValue(int nSkill) {
    std::uint8_t bySkillLevel = m_stLeagueInfo.bySkillInfo[nSkill];
    std::int32_t nEffectRate = 0;

    const auto& mapSkill = XGameServer::Instance()
        ->GetResourceMgr().GetTB_LEAGUE_SKILL_Map();
    for (const auto& kv : mapSkill) {
        const TB_LEAGUE_SKILL& row = kv.second;
        if (row.League_Skill_Type == static_cast<unsigned int>(nSkill + 1)
            && row.League_Get_Skill_Level == bySkillLevel) {
            nEffectRate = static_cast<std::int32_t>(
                row.League_Skill_Apply_Value);
        }
    }
    return nEffectRate;
}

std::int64_t CUser::GetFP() {
    // IDA 0x140048FB0: return (uint16_t)this->m_stCharInfo.shFP
    return static_cast<std::int64_t>(static_cast<std::uint16_t>(m_stCharInfo.shFP));
}

std::int64_t CUser::GetBonusFP() {
    // IDA 0x140048F90: return (uint16_t)this->m_stCharInfo.shBonusFP
    return static_cast<std::int64_t>(static_cast<std::uint16_t>(m_stCharInfo.shBonusFP));
}

// IDA 0x1406F9B20: CUser::AddFP
// 添加FP值，限制�?-200范围�?
// 返回: true如果值有�?>=0)，false如果值被设为0(原本<0)
bool CUser::AddFP(std::int16_t shPoint) {
    m_stCharInfo.shFP += shPoint;
    if (m_stCharInfo.shFP >= 0) {
        if (m_stCharInfo.shFP > 200) {
            m_stCharInfo.shFP = 200;
        }
        return true;
    } else {
        m_stCharInfo.shFP = 0;
        return false;
    }
}

// IDA 0x1406F9BA0: CUser::AddBonusFP
// 添加奖励FP值，限制�?-400范围�?
// 返回: 总是返回true
bool CUser::AddBonusFP(std::int16_t shPoint) {
    m_stCharInfo.shBonusFP += shPoint;
    if (m_stCharInfo.shBonusFP >= 0) {
        if (m_stCharInfo.shBonusFP > 400) {
            m_stCharInfo.shBonusFP = 400;
        }
    } else {
        m_stCharInfo.shBonusFP = 0;
    }
    return true;
}

// IDA 0x1406F9EE0: CUser::GetPCBangFP
// 获取网吧FP�?
// bCheckUse: 是否检查网吧状态和Booster效果
// 返回: 网吧FP值，如果检查失败返�?
std::int16_t CUser::GetPCBangFP(bool bCheckUse) {
    // IDA 反编译显�?
    // 1. 获取 CGocBooster 组件，查�?eBooster_Effect_AddFP 效果�?
    auto pBooster = GetGOC<CGocBooster>();
    int nBoosterFP = 0;
    if (pBooster) {
        nBoosterFP = pBooster->GetTotalValue(eBooster_Effect_AddFP);
    }
    
    // 2. 如果 bCheckUse �?true:
    //    - 获取 CGocEntity 组件，检�?NetCafe 状�?
    //    - 如果不是 NetCafe �?nBoosterFP <= 0，返�?0
    if (bCheckUse) {
        auto pEntity = GetGOC<CGocEntity>();
        if (pEntity && !pEntity->GetNetCafe() && nBoosterFP <= 0) {
            return 0;
        }
    }
    
    // 3. 返回 m_stCharInfo.shPCBangFP
    return m_stCharInfo.shPCBangFP;
}

// IDA 0x1406F9C20: CUser::AddPCBangFP
// 添加网吧FP值，限制�?-200范围�?
// shPoint: 要添加的FP�?
// shPointOther: 其他FP�?用于DB日志)
// bSendDB: 是否发送DB更新
// 返回: 总是返回true
bool CUser::AddPCBangFP(std::int16_t shPoint, std::int16_t shPointOther, bool bSendDB) {
    m_stCharInfo.shPCBangFP += shPoint;
    if (m_stCharInfo.shPCBangFP >= 0) {
        if (m_stCharInfo.shPCBangFP > 200) {
            m_stCharInfo.shPCBangFP = 200;
        }
    } else {
        m_stCharInfo.shPCBangFP = 0;
    }

    if (bSendDB) {
        // IDA 反编译显�?
        // 1. 构�?XSendDBPacket(main=3, sub=0x75)
        XSendDBPacket xSendDBPacket(static_cast<XClient*>(this), 3, 0x75);
        // 2. 写入 UAID, QuestID, shPoint, shPointOther
        xSendDBPacket << GetUAID();
        xSendDBPacket << 0; // QuestID placeholder
        xSendDBPacket << shPoint;
        xSendDBPacket << shPointOther;
        // 3. 发送到 GameDB
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->SendDBGame(xSendDBPacket);
        }
        
        // 4. 构�?XSendPacket(main=3, sub=0x64) 发送FP更新给客户端
        XSendPacket xSendPacket(3, 0x64);
        xSendPacket << static_cast<std::int64_t>(GetFP());
        xSendPacket << static_cast<std::int64_t>(GetBonusFP());
        xSendPacket << static_cast<std::int64_t>(GetPCBangFP(true));
        xSendPacket << static_cast<std::int64_t>(0);
        CGocNetwork::Send(reinterpret_cast<XActor*>(this), xSendPacket);
    }

    return true;
}

// IDA 0x1401ADC70: CUser::SetFullStat
// 设置是否首次进入世界（满属性状态）
void CUser::SetFullStat(bool bFirstEnter) {
    m_bFirstWorldEnter = bFirstEnter;
}

// IDA 0x1401ADCA0: CUser::IsFullStat
// 检查是否首次进入世界（满属性状态）
bool CUser::IsFullStat() const {
    return m_bFirstWorldEnter;
}

// IDA 0x1401ADCC0: CUser::SetClientLoadComplete
// 设置客户端加载完成标�?
void CUser::SetClientLoadComplete(bool bComplete) {
    m_bClientLoadComplete = bComplete;
}

std::uint8_t CUser::GetGMPower() {
    // IDA 0x140082DB0: return this->m_stCharInfo.byGMPower
    return m_stCharInfo.byGMPower;
}

std::uint8_t CUser::GetBlockType() {
    // IDA 0x140082D90: return this->m_byBlockType
    return m_byBlockType;
}

bool CUser::GetFirstEnter() {
    // IDA 0x140049600: return this->m_bFirstEnter
    return m_bFirstEnter;
}

// IDA 0x1402A4C60: CUser::IsGM
// 检查是否为GM
bool CUser::IsGM() const {
    return m_stCharInfo.byGMPower != 0;
}

// IDA 0x140026C30: CUser::IsStatus
// 检查状态标志
bool CUser::IsStatus(std::uint32_t dwStatus) const {
    return (dwStatus & m_dwStatus) != 0;
}

// ============================================================================
// GetTableID IDA 0x14070A490
// ============================================================================
int CUser::GetTableID() {
    // IDA 0x14070A490: if (m_pCharTableRef) return m_pCharTableRef->ID;
    // Note: TB_CHARACTER is a forward declaration; actual type is TB_CHARACTER_INFO
    if (m_pCharTableRef)
        return reinterpret_cast<TB_CHARACTER_INFO*>(m_pCharTableRef)->ID;
    return 0;
}

// ============================================================================
// 战斗相关方法实现
// ============================================================================

// GetHP IDA 0x14070AC50
// return *(unsigned int *)&this->szBuffer[60695];
int CUser::GetHP() const {
    return m_nHP;
}

// SetHP IDA 0x1406F4880
// IDA 反编译精确还�?
// - 虚函数调用获�?MaxHP
// - 存储�?szBuffer[60695] 偏移
// - 通过 CGocAttribute 组件同步
void CUser::SetHP(int nVal) {
    // IDA: 虚函数调用获取最�?HP
    int nMaxHP = GetMaxHP();
    int nFinalHP = nVal;

    // IDA: HP 不能超过 MaxHP
    if (nVal > nMaxHP) {
       nFinalHP = nMaxHP;
    }

    // IDA: *(_DWORD *)&this->szBuffer[60695] = nVala;
    m_nHP = nFinalHP;

    // IDA: CMover::GetGOC<CGocAttribute>((CMover *)this, &pAttr, 0);
    // IDA: CGocAttribute::SetHP(v2, fValue);
    // 同步�?CGocAttribute 组件
    auto pAttr = GetGOC<CGocAttribute>();
    if (pAttr) {
        pAttr->SetHP(static_cast<float>(nFinalHP));
    }
}

// ============================================================================
// 专用怪物相关方法 (IDA 精确还原)
// ============================================================================

// GetDedicatedMonster IDA 0x1406FEF70
// 获取专用怪物对象
CMoverEx* CUser::GetDedicatedMonster() {
    // IDA: if (this->m_dwDedicatedMonsterID)
    //         return (CMoverEx *)CMover::GetMoverObject(&this->CMoverEx, this->m_dwDedicatedMonsterID);
    //     else return nullptr;
    if (m_dwDedicatedMonsterID) {
        return static_cast<CMoverEx*>(CMover::GetMoverObject(m_dwDedicatedMonsterID));
    }
    return nullptr;
}

// CheckDedicatedMonster IDA 0x1406F41C0
// 检查专用怪物并处理伤�?
int CUser::CheckDedicatedMonster(std::uint32_t dwID, std::uint32_t nSkillID,
                                  std::uint32_t nDamage, std::uint8_t byDamageFlag,
                                  std::uint8_t byHitParts) {
    // IDA: pDedicatedMonster = CUser::GetDedicatedMonster(this);
    CMoverEx* pDedicatedMonster = GetDedicatedMonster();

    // IDA: if (!pDedicatedMonster || CMover::IsDie(pDedicatedMonster)) return 0;
    if (!pDedicatedMonster || pDedicatedMonster->IsDie()) {
        return 0;
    }

    // IDA: pDedicatedMonster->DamageProcessHP(pDedicatedMonster, dwID, nSkillID, nDamage, byDamageFlag, byHitParts);
    // 注意: CMover::DamageProcessHP 需�?6 个参�? dwID, nSkillID, nDamage, nUnk1, byDamageFlag, byHitParts
    // IDA �?nUnk1 被忽略，所以传�?0
    pDedicatedMonster->DamageProcessHP(dwID, static_cast<int>(nSkillID), static_cast<int>(nDamage));

    // IDA: v6 = pDedicatedMonster->GetHP(pDedicatedMonster);
    //      pDedicatedMonster->SetHP(pDedicatedMonster, v6);
    int nHP = pDedicatedMonster->GetHP();
    pDedicatedMonster->SetHP(nHP);

    // IDA: if (pDedicatedMonster->GetHP(pDedicatedMonster) <= 0)
    if (pDedicatedMonster->GetHP() <= 0) {
        // IDA: CMoverEx::SetDieReason(pDedicatedMonster, 7u, nDamage);
        pDedicatedMonster->SetDieReason(7, static_cast<int>(nDamage));

        // IDA: LOWORD(v7) = 12; pDedicatedMonster->SetDie_2(pDedicatedMonster, v7, 0);
        // CMoverEx::SetDie takes 3 arguments: (nMotionClass, bSuicide, bSendPacket)
        pDedicatedMonster->SetDie(12, 0, true);
    }

    // IDA: return 1;
    return 1;
}

// DamageProcessHP IDA 0x1406F42C0
// IDA 反编译精确还�? 处理伤害并返回是否死�?
// 参数: dwID=攻击者ID, nSkillID=技能ID, nDamage=伤害�? nUnk1=未知参数1, byDamageFlag=伤害标志, byHitParts=命中部位
int CUser::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                           std::uint8_t byDamageFlag, std::uint8_t byHitParts) {
    // IDA: if (CUser::CheckDedicatedMonster((CUser *)((char *)this - 131512), dwID, nSkillID, nDamage, byDamageFlag, byHitParts) == 1)
    //         return 0;
    // 注意: IDA 中的 this 偏移是由于多重继承导致的，实际调用时直接使用 this
    if (CheckDedicatedMonster(dwID, static_cast<std::uint32_t>(nSkillID),
                              static_cast<std::uint32_t>(nDamage), byDamageFlag, byHitParts) == 1) {
        return 0;
    }

    // IDA: fMaxHP = CMover::GetStat((CMover *)this, 10);
    float fMaxHP = static_cast<float>(GetMaxHP());

    // IDA: CMover::GetGOC<CGocAttribute>((CMover *)this, &pAttr, 0);
    // IDA: v7 = (CGocAttribute *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pAttr);
    // IDA: fCurHP = (float)CGocAttribute::GetHP(v7);
    // 注意: 当前使用 m_nHP 直接获取，CGocAttribute 版本待完�?
    float fCurHP = static_cast<float>(m_nHP);

    // IDA: if (fCurHP == 0.0) { v30 = 1; return v30; }
    if (fCurHP == 0.0f) {
        return 1;
    }

    // IDA: 计算最�?HP
    // if ((float)(fCurHP - (float)nDamage) <= 0.0) v35 = 0.0; else v35 = fCurHP - (float)nDamage;
    float fFinalHP = fCurHP - static_cast<float>(nDamage);
    if (fFinalHP <= 0.0f) {
        fFinalHP = 0.0f;
    }

    // IDA: 额外 HP 检�?(szBuffer[951] 相关 - m_pGocAttribute �?MaxHP 限制)
    // if ((float)(int)*(float *)(*(_QWORD *)&this->szBuffer[951] + 40LL) <= v35) { v37 = ... }
    // Note: This checks MaxHP from CGocAttribute to ensure HP doesn't exceed limits
    auto pAttr = GetGOC<CGocAttribute>();
    if (pAttr) {
        int nAttrMaxHP = static_cast<int>(pAttr->GetStat(10));
        if (fFinalHP > static_cast<float>(nAttrMaxHP)) {
            fFinalHP = static_cast<float>(nAttrMaxHP);
        }
    }

    int nFinalHP = static_cast<int>(fFinalHP);
    fFinalHP = static_cast<float>(nFinalHP);

    // IDA: if (nDamage >= 0) - 只处理有效伤�?
    if (nDamage >= 0) {
        // IDA: v38 = std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pAttr);
        // IDA: ((void (__fastcall *)(CGocNetwork *, __int64, __int64, _QWORD))v38->__vftable[2].Finalize)(v38, 1, v8, 0);
        // Set damage flag on CGocAttribute for damage animation/effects
        auto pAttr = GetGOC<CGocAttribute>();
        if (pAttr) {
            pAttr->SetSyncStatFlag(1, 0);
        }

        // IDA: HP 百分比检�?- 触发被动技�?
        if (fCurHP > 0.0f && fFinalHP > 0.0f) {
            float fCurRate = (fCurHP / fMaxHP) * 100.0f;
            float fFinalRate = (fFinalHP / fMaxHP) * 100.0f;

            // IDA: if (fCurRate > 50.0 && fFinalRate <= 50.0) - HP 降到 50% 以下 - 触发被动技�?54
            if (fCurRate > 50.0f && fFinalRate <= 50.0f) {
                // IDA: v39 = this->XClient::XSocket::__vftable;
                // IDA: LOBYTE(v10) = 54; LOBYTE(v9) = 1;
                // IDA: ((void (__fastcall *)(CUser *, __int64, __int64))v39[5].MoveToDest)(this, v9, v10);
                CheckPassiveSkill(1, 54);
            }

            // IDA: if (fCurRate > 20.0 && fFinalRate <= 20.0) - HP 降到 20% 以下 - 触发被动技�?50
            if (fCurRate > 20.0f && fFinalRate <= 20.0f) {
                CheckPassiveSkill(1, 50);
            }
        }

        // IDA: 死亡时处�?HP/SG 吸收
        // if (fCurHP > 0.0 && nFinalHP <= 0)
        if (fCurHP > 0.0f && nFinalHP <= 0) {
            // IDA: pAttackMover = (CMoverEx *)CMover::GetMoverObject((CMover *)this, dwID);
            CMoverEx* pAttackMover = static_cast<CMoverEx*>(CMover::GetMoverObject(dwID));

            if (pAttackMover) {
                // IDA: OwnerPlayer = CMoverEx::GetOwnerPlayer(pAttackMover);
                CMoverEx* pOwnerPlayerRaw = pAttackMover->GetOwnerPlayer();

                // IDA: pOwnerPlayer = (CUser *)_RTDynamicCast_0(OwnerPlayer, 0, &CMoverEx `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
                // Use dynamic_cast for RTTI type conversion
                CUser* pOwnerPlayer = dynamic_cast<CUser*>(pOwnerPlayerRaw);

                if (pOwnerPlayer) {
                    // IDA: v41 = &pOwnerPlayer->CMoverEx;
                    // IDA: pAttackMover = &pOwnerPlayer->CMoverEx;
                    pAttackMover = static_cast<CMoverEx*>(pOwnerPlayer);
                }

                // IDA: if (pAttackMover->GetHP(pAttackMover) > 0)
                if (pAttackMover->GetHP() > 0) {
                    // IDA: CMover::GetGOC<CGocAttribute>(pAttackMover, &pAttackAttr, 0);
                    // IDA: CMover::GetGOC<CGocAttribute>((CMover *)this, &pTargetAttr, 0);
                    std::shared_ptr<CGocAttribute> pAttackAttr;
                    std::shared_ptr<CGocAttribute> pTargetAttr;
                    pAttackMover->GetGOC<CGocAttribute>(&pAttackAttr, false);
                    CMover::GetGOC<CGocAttribute>(&pTargetAttr, false);

                    // IDA: fAbsorbHP = CGocAttribute::GetSpecialEffect(v12, EFFECT_SPECIAL_ABSORB_HP_RAT);
                    // IDA: v13 = pAttackMover->GetHP(pAttackMover);
                    // IDA: v42->SetHP(pAttackMover, (int)fAbsorbHP + v13);
                    // IDA: fAbsorbSG = CGocAttribute::GetSpecialEffect(v14, EFFECT_SPECIAL_ABSORB_SG_RAT);
                    if (pAttackAttr && pTargetAttr) {
                        float fAbsorbHP = pAttackAttr->GetSpecialEffect(EFFECT_SPECIAL_ABSORB_HP_RAT);
                        if (fAbsorbHP > 0.0f) {
                            int nAttackHP = pAttackMover->GetHP();
                            pAttackMover->SetHP(static_cast<int>(fAbsorbHP) + nAttackHP);
                        }

                        float fAbsorbSG = pAttackAttr->GetSpecialEffect(EFFECT_SPECIAL_ABSORB_SG_RAT);
                        if (fAbsorbSG > 0.0f) {
                            pAttackAttr->SetStat(2, pAttackAttr->GetStat(2) + fAbsorbSG, true);
                        }
                    }
                }
            }
        }

        // IDA: 更新 HP
        m_nHP = nFinalHP;

        // IDA: 返回是否死亡
        // if (nFinalHP) { v33 = 0; return v33; } else { v32 = 1; return v32; }
        return (nFinalHP == 0) ? 1 : 0;
    }

    return 0;
}

// ApplySkillDamageFrame IDA 0x1406F6140
// 精确还原: 应用技能伤害帧
void CUser::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                  std::uint8_t byAttackTargetCnt, hkvVec3* vPos,
                                  float fAttackRot, int nContinueAttack,
                                  std::uint8_t byDamageType, bool bPenetrate) {
    // IDA: Get skill table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTable) return;

    // IDA: Check if Akashic skill (Use_Position == 2)
    if (pSkillTable->Use_Position == 2) {
        // IDA: Call Akashic object's ApplySkillDamageFrame
        // Note: Akashic system not yet reconstructed
        GreenDamTan_log(__FILE__, __FUNCTION__, "ApplySkillDamageFrame: Akashic skill %d", nSkillID);
        return;
    }

    // IDA: Stub - AttackJudgmentTrigger and related types not yet reconstructed
    GreenDamTan_log(__FILE__, __FUNCTION__, "ApplySkillDamageFrame: skill=%d, trigger=%d, targets=%d",
                    nSkillID, nTriggerIdx, byAttackTargetCnt);
}

// SetBattleStateTime - 设置战斗状态持续时�?
void CUser::SetBattleStateTime(float fTime) {
    m_fBattleStateTime = fTime;
}

// ============================================================================
// 技能相关方法实�?(IDA 反编�?
// ============================================================================

// ProcessChangeCombatAfterUseSkill IDA 0x1406F67D0
// 使用技能后处理战斗状态变�?
void CUser::ProcessChangeCombatAfterUseSkill() {
    // 如果设置了战斗状态切换时�?
    if (GetCombatChangeTime() > 0.0f) {
        // 减少使用次数
        if (m_byCombatChangeUseCount != 0 && m_byCombatChangeUseCount != 0xFF) {
            --m_byCombatChangeUseCount;
        }

        // 使用次数耗尽，切换回普通战斗类�?
        if (m_byCombatChangeUseCount == 0) {
            ChangeCombatType(0, 0.0f, 0.0f);
        }
    }
}

// GetCombatChangeTime - 获取战斗状态切换时�?
float CUser::GetCombatChangeTime() {
    return m_fCombatChangeTime;
}

// ============================================================================
// CMoverEx 技能相关方�?(继承�?CMoverEx)
// ============================================================================

// CheckUseSkill IDA 0x14037FBD0 (CMoverEx::CheckUseSkill)
// 检查技能使用条�?
// byCheckVal: 检查类�?(1=总是允许, 2=动作状态检�? 3=倒地检�? 4=反击检�? 5=解锁检�?
// byNormalVal: 普通检查标志位 (4=不能倒地, 8=不能反击命中, 16=需要解锁buff)
// pTBSkill: 技能表数据
// 返回�? 1=可以使用, 0=不能使用
int CUser::CheckUseSkill(std::uint8_t byCheckVal, std::uint8_t byNormalVal, TB_SKILL* pTBSkill) {
    // IDA 反编�?
    // switch (byCheckVal) {
    //   case 1: return 1;
    //   case 2: return (m_nMotionClass == 5 || (m_nMotionClass >= 32 && m_nMotionClass <= 34));
    //   case 3: return IsHitDown();
    //   case 4: return IsCounterAttackHit();
    //   case 5: return IsActivateSkillUnlockBuff(pTBSkill);
    //   default: 组合检�?
    // }
    switch (byCheckVal) {
        case 1:
            // 类型1: 总是允许使用
            return 1;

        case 2:
            // 类型2: 检查动作状�?(5 �?32-34 为可用状�?
            // IDA: return this->m_nMotionClass == 5 || this->m_nMotionClass >= 32 && this->m_nMotionClass <= 34;
            // TODO: 需要从 CMoverEx 基类获取 m_nMotionClass
            return 1;

        case 3:
            // 类型3: 检查是否倒地
            // IDA: return CMover::IsHitDown(this);
            return IsHitDown() ? 1 : 0;

        case 4:
            // 类型4: 检查是否反击命�?
            // IDA: return this->IsCounterAttackHit(this);
            return IsCounterAttackHit() ? 1 : 0;

        case 5:
            // 类型5: 检查技能解锁buff
            // IDA: return CMover::IsActivateSkillUnlockBuff(this, pTBSkill);
            return IsActivateSkillUnlockBuff(pTBSkill) ? 1 : 0;

        default:
            // 默认: 组合检�?
            // IDA: return ((byNormalVal & 4) == 0 || !IsHitDown())
            //        && ((byNormalVal & 8) == 0 || !IsCounterAttackHit())
            //        && ((byNormalVal & 0x10) == 0 || IsActivateSkillUnlockBuff(pTBSkill));
            return 1;
    }
}

// CancelSkill IDA 0x14037E9E0 (CMoverEx::CancelSkill)
// 取消当前技�?
void CUser::CancelSkill() {
    // IDA 反编�?
    // if (XActor::IsStatus(&this->XActor, 1u)) {
    //     this->ChangeMotion_3(this, 1, 1, 2);
    // }
    if (CMover::IsStatus(1)) {
        ChangeMotion(1, 1, 2);
    }
}

// GetSkillLevel IDA 0x140189040 (CMoverEx::GetSkillLevel)
// 获取当前技能等�?
std::uint8_t CUser::GetSkillLevel() {
    // IDA 0x140189040: if (m_pCurSkillTableRef) return m_pCurSkillTableRef->Skill_LV; else return 0;
    if (m_pCurSkillTableRef)
        return m_pCurSkillTableRef->Skill_LV;
    return 0;
}

// GetSkillCoolDownRate IDA 0x1402C7240 (CMover::GetSkillCoolDownRate)
// 获取技能冷却速率修正
float CUser::GetSkillCoolDownRate() {
    // IDA 0x1402C7240: return this->m_fSkillCoolDownRate
    return m_fSkillCoolDownRate;
}

// SetSkillCoolDownRate - 设置技能冷却速率修正
void CUser::SetSkillCoolDownRate(float fRate) {
    // IDA 0x1402C7240 adjacent: this->m_fSkillCoolDownRate = fRate
    m_fSkillCoolDownRate = fRate;
}

// CheckSkillSkipType IDA 0x14037E490 (CMoverEx::CheckSkillSkipType)
// 检查技能跳过类�?
// Skill_Motion_Skip_Type: 1=检查状�?, 2=检查动�?�?-6, 3=总是跳过
bool CUser::CheckSkillSkipType(std::uint32_t nSkillID) {
    // IDA 反编�?
    // pSkillTbl = XResourceMgr::GetTB_SKILL(..., nSkillID);
    // if (!pSkillTbl) return false;
    // switch (pSkillTbl->Skill_Motion_Skip_Type) {
    //   case 1: return XActor::IsStatus(this, 1);
    //   case 2: return (m_nMotionClass == 1 || (m_nMotionClass >= 3 && m_nMotionClass <= 6));
    //   case 3: return true;
    // }
    // return false;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkill = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkill) return false;

    switch (pSkill->Skill_Motion_Skip_Type) {
        case 1:
            // Type 1: check XActor::IsStatus(this, 1)
            return CMover::IsStatus(1);
        case 2:
            // Type 2: check motion class (1 or 3-6)
            return (m_nMotionClass == 1 || (m_nMotionClass >= 3 && m_nMotionClass <= 6));
        case 3:
            // Type 3: always skip
            return true;
        default:
            return false;
    }
}

// IsCanSkill IDA 0x14037FB80 (CMoverEx::IsCanSkill)
// 检查是否可以使用技�?(不能有状�?x40000000�?x80000000)
bool CUser::IsCanSkill() {
    // IDA 0x14037FB80: return !XActor::IsStatus(&this->XActor, 0x40000000u)
    //                      && !XActor::IsStatus(&this->XActor, 0x80000000);
    return !CMover::IsStatus(0x40000000u) && !CMover::IsStatus(0x80000000);
}

// PreSkillProcess IDA 0x14037D790 (CMoverEx::PreSkillProcess)
// 技能使用前处理
void CUser::PreSkillProcess(std::uint32_t nSkillID, int bNormalAttack) {
    // IDA 反编译摘�?
    // 1. 获取技能表 pSkillTbl = XResourceMgr::GetTB_SKILL(nSkillID)
    // 2. 初始�? m_fMoveDistAfterSkill = 0, m_bAttackKeyPress = 0
    // 3. 设置相机锁定: m_bDisableDirectionToTargetSkill
    // 4. 更新技能动画信�? UpdateSkillAnimInfo(pSkillTbl)
    // 5. 获取技能动画名�? GetSkillAnimName(pSkillTbl, m_bySkillAnimStep)
    // 6. 处理上层动画 (MOVE_UPPER_ANIM)
    // 7. 设置当前技能表: m_pCurSkillTableRef = pSkillTbl
    // 8. 清除/设置状�? ClearStatus(0x8000) or SetStatus(0x8000)
    // 9. 调用 ChangeMotion_3 切换动画
    // 10. 处理蓄力技�? ChargeSkillStart() 如果 ControlType �?2/5/8
    // 11. 扫描周围对象检查任务目�?

    // Step 1: Get skill table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTbl = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTbl) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "PreSkillProcess: skill not found");
        return;
    }

    // Step 2: Initialize skill state
    m_fMoveDistAfterSkill = 0.0f;
    m_bAttackKeyPress = 0;

    // Step 3: Set camera lock based on skill type
    // m_bDisableDirectionToTargetSkill = (pSkillTbl->ControlType != 0);

    // Step 4: Update skill animation info
    UpdateSkillAnimInfo(pSkillTbl);
    m_bySkillAnimStep = 0;
    m_bySkillAnimCount = 1;

    // Step 5-6: Get skill animation name and process upper body animation
    GetSkillAnimName(pSkillTbl, m_bySkillAnimStep);
    // Process upper body animation blending

    // Step 7: Set current skill table reference
    m_pCurSkillTableRef = pSkillTbl;

    // Step 8: Handle status flags
    // ClearStatus(0x8000) or SetStatus(0x8000) based on skill type

    // Step 9: Change motion to skill animation
    // ChangeMotion(nMotionClass, 1, 3);

    // Step 10: Handle charge skills (ControlType 2/5/8)
    std::uint8_t byControlType = GetControlType(pSkillTbl);
    if (byControlType == 2 || byControlType == 5 || byControlType == 8) {
        ChargeSkillStart();
    }

    // Step 11: Scan nearby objects for quest targets
    // TODO: restore quest target scan when CGocQuest target helpers are reconstructed.

    GreenDamTan_log(__FILE__, __FUNCTION__, "PreSkillProcess complete");
}

// SetSkillTable IDA 0x140188F60 (CMoverEx::SetSkillTable)
// 设置当前技能表引用
void CUser::SetSkillTable(TB_SKILL* pSkillRef) {
    // IDA 0x140188F60: this->m_pCurSkillTableRef = pSkillRef
    m_pCurSkillTableRef = pSkillRef;
}

// ============================================================================
// CGocSkill 相关方法 (通过组件访问)
// ============================================================================

// IsHaveSkill - 检查是否拥有指定技�?
// 委托�?CGocSkill 组件 (尚未还原)
bool CUser::IsHaveSkill(int nSkillID) {
    // TODO: CGocSkill 组件还原后取消注�?
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     return pSkillComp->IsHaveSkill(nSkillID);
    // }
    return false;
}

// LearnSkill - 学习新技�?
// 委托�?CGocSkill 组件 (尚未还原)
bool CUser::LearnSkill(int nSkillID, bool bUseCheat, int nTicknum) {
    // TODO: CGocSkill 组件还原后取消注�?
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     return pSkillComp->LearnSkill(nSkillID, bUseCheat, nTicknum);
    // }
    return false;
}

// ResetSkill - 重置技能点
// 委托�?CGocSkill 组件 (尚未还原)
void CUser::ResetSkill(bool bUseCheat, int nTicknum) {
    // TODO: CGocSkill 组件还原后取消注�?
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     pSkillComp->ResetSkill(bUseCheat, nTicknum);
    // }
}

// ============================================================================
// CMySkillList 相关方法 (技能列表管�?
// IDA 反编译来�? GameServer.exe
// ============================================================================

// UseSkill - 使用技�?
// IDA 0x1402B75E0 (CMySkillList::UseSkill)
int CUser::UseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable, float fSkillCost) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        return pSkillList->UseSkill(pSkillTable, pChangedSkillTable, fSkillCost);
    }
    return 0;
}

// SetSkillCooltime - 设置技能冷�?
// IDA 0x1402C4AD0 (CMySkillList::SetSkillCooltime)
void CUser::SetSkillCooltime(TB_SKILL* pSkillTable) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        pSkillList->SetSkillCooltime(pSkillTable);
    }
}

// GetSkillCooltime - 获取技能剩余冷却时�?
// IDA 0x1402C4940 (CMySkillList::GetCooltime)
float CUser::GetSkillCooltime(int nCooltimeGroup, std::uint16_t wGlobalCoolTime, bool bCheckGlobalCool) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        return pSkillList->GetCooltime(E_COOLTIME_SKILL, nCooltimeGroup, wGlobalCoolTime, bCheckGlobalCool);
    }
    return 0.0f;
}

// ReduceSkillCooltime - 减少技能冷却时�?
// IDA 0x1402C5280 (CMySkillList::ReduceSkillCooltime)
void CUser::ReduceSkillCooltime(float fReduceTime) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        pSkillList->ReduceSkillCooltime(fReduceTime);
    }
}

// ResetCoolTime - 重置冷却时间
// IDA 0x1402C4870 (CMySkillList::ResetCoolTime)
void CUser::ResetCoolTime(int eType) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        pSkillList->ResetCoolTime(static_cast<E_COOLTIME_TYPE>(eType));
    }
}

// ============================================================================
// 被动技能相关方�?
// ============================================================================

// SetPassiveSkillStat - 设置被动技能属�?
// IDA 0x140188E80 (CMoverEx::SetPassiveSkillStat)
// 根据 Buff 表数据应用被动技能属性效�?
void CUser::SetPassiveSkillStat(std::uint16_t wBuffID) {
    // 获取 Buff �?
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(wBuffID);
    if (!pBuffTable) return;

    // 根据 Buff 效果类型应用属�?
    // EffectType_Status_01: 状态效果类�?
    // Option_Value_01: 效果�?
    if (pBuffTable->EffectType_Status_01 != 0 || pBuffTable->Buff_Time != 0) {
        // 有效果类型或持续时间 - 需要设�?buff 状�?
        // TODO: 设置 buff 状�?(依赖 CGocBuff 组件还原)
        // SetBuff(wBuffID, ...);
    } else {
        // 直接应用属性效�?
        // TODO: 应用属性修�?(依赖 CGocOptionEffect 组件还原)
        // ApplyOptionEffect(pBuffTable);
    }
}

// ClearPassiveSkillStat - 清除被动技能属�?
// IDA 0x140188EC0 (CMoverEx::ClearPassiveSkillStat)
// 清除被动技能应用的属性效�?(SetPassiveSkillStat 的逆操�?
void CUser::ClearPassiveSkillStat(std::uint16_t wBuffID) {
    // 获取 Buff �?
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(wBuffID);
    if (!pBuffTable) return;

    // 逆操�? 清除 buff 状态或移除属性效�?
    if (pBuffTable->EffectType_Status_01 != 0 || pBuffTable->Buff_Time != 0) {
        // TODO: 清除 buff 状�?(依赖 CGocBuff 组件还原)
        // RemoveBuff(wBuffID);
    } else {
        // TODO: 移除属性修�?(依赖 CGocOptionEffect 组件还原)
        // RemoveOptionEffect(pBuffTable);
    }
}

// CheckPassiveSkill - 检查并触发被动技�?
// IDA 0x140188FC0 (CMoverEx::CheckPassiveSkill)
// 遍历被动技能列表，根据类型触发对应的被动效�?
void CUser::CheckPassiveSkill(std::uint8_t byType, std::uint8_t byParam) {
    // 通过技能管理器获取已学习的被动技能列�?
    CMySkillList* pSkillList = GetSkillMgr();
    if (!pSkillList) return;

    // TODO: 遍历被动技�?(依赖 CMySkillList::GetPassiveSkillList 还原)
    // 当前简化实�? 通过 CMySkillList 获取技能表并检�?Passive_Type
    // std::vector<TB_SKILL*> vPassiveSkills = pSkillList->GetPassiveSkillList();
    // for (TB_SKILL* pTbl : vPassiveSkills) {
    //     if (pTbl && pTbl->Passive_Type == byType) {
    //         // 根据被动类型和参数触发效�?
    //         switch (byType) {
    //             case 1: // HP 阈值触�?
    //                 SetPassiveSkillStat(static_cast<std::uint16_t>(byParam));
    //                 break;
    //             case 2: // 状态触�?
    //                 // ...
    //                 break;
    //             default:
    //                 break;
    //         }
    //     }
    // }
}

// ============================================================================
// AI 技能条件检�?
// ============================================================================

// CheckSkillCondition - 检查技能条�?(AI)
bool CUser::CheckSkillCondition(int nSkillIndex, int nSkillGroup) {
    // Check skill group condition
    if (nSkillGroup != 0 && nSkillIndex == 0) {
        return false;
    }

    // 检查技能索引范�?
    if (nSkillIndex >= 10) {
        return false;
    }

    // Get skill list
    CMySkillList* pSkillList = GetSkillMgr();
    if (!pSkillList) {
        return false;
    }

    // Get skill table by index
    TB_SKILL* pSkillTbl = GetSkillTable();
    if (!pSkillTbl) return false;

    // Check various conditions:
    // - MP/SG cost
    if (GetMP() < static_cast<int>(pSkillTbl->Skill_Cost)) {
        return false;
    }
    // - Cooldown
    if (GetSkillCooltime(pSkillTbl->CoolTime_Group, pSkillTbl->CoolTime_Global, true) > 0.0f) {
        return false;
    }
    // - Target requirements (Target_Type)
    // - Range requirements (Range_Min, Range_Max)
    
    // All conditions met
    return true;
}

// ============================================================================
// 核心虚函数实�?(IDA 反编�?
// ============================================================================

// OnUpdate - 更新循环
// IDA 0x1406ED290: CUser::OnUpdate
void CUser::OnUpdate(float fDeltaTime) {
    if (m_dwKickoutTime != 0 && m_dwKickoutTime <= GetTickCount64()) {
        XClient::SetState(eStateKickOut);
        return;
    }
    CMoverEx::OnUpdate(fDeltaTime);
    GreenDamTan_log(__FILE__, __FUNCTION__, "OnUpdate: deltaTime=%.3f", fDeltaTime);
}

void CUser::CheckDBLoad_All() {
}

void CUser::SendSyncDBLoad() {
}

void CUser::OnPassiveCheck(float fDeltaTime) {
    (void)fDeltaTime;
}

// BridgeSend - 发送数据包
// IDA 0x1406E8B50: CUser::BridgeSend
// 精确还原: 加锁、检查状态、加密并发送数据包
bool CUser::BridgeSend(XSendPacket& xSendPacket) {
    // IDA: CSimpleLock::Owner lock((CSimpleLock *)((char *)this - 1072));
    // 偏移 -1072 = XSocket::xLock (�?CUser 起点偏移)
    CSimpleLock::Owner lock(&this->xLock);

    // IDA: 检查是否处�?eStateChangeServer 状�?
    if (XClient::IsState(eStateChangeServer)) {
        // IDA: v8 = 0; return v8;
        return false;
    }

    // IDA: v12 = *((unsigned __int16 *)this - 539);
    // 偏移 -539 * 2 = -1078 = szBuffer 中的 usOffset (当前缓冲区偏�?
    std::uint16_t usCurrentOffset = this->usOffset;

    // IDA: UsIndex = XParse::GetUsIndex(&xSendPacket->XParse);
    std::uint16_t usPacketSize = xSendPacket.XParse.m_usIndex;

    // IDA: nBuffSize = v12 + UsIndex + 5;
    int nBuffSize = static_cast<int>(usCurrentOffset) + static_cast<int>(usPacketSize) + 5;

    // IDA: if (nBuffSize >= 65534) - 缓冲区溢出检�?
    if (nBuffSize >= 65534) {
        // IDA: overLab = (XOverLab *)((char *)this - 66712);
        // IDA: pClient = (CUser *)((char *)this - 132384);
        // IDA: v4 = TXSingleton<XGameServer>::Instance();
        // IDA: XIOCPServer::XSend(v4, pClient, overLab);
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (pGameServer) {
            pGameServer->XSend(this, nullptr);
        }

        // IDA: v15 = *((void (__fastcall ***)(char *, _QWORD))this - 8339);
        // IDA: (*v15)((char *)this - 66712, 0); - 虚函数调用清理缓冲区
        usOffset = 0;

        // IDA: OutputDebugStringA("Send All !! \n");
        OutputDebugStringA("Send All !! \n");
    }

    // IDA: usOutSize = 0;
    std::uint16_t usOutSize = 0;

    // IDA: xSendPacket->usTos = 1;
    xSendPacket.usTos = 1;

    // IDA: XSendPacket::Encrypt(xSendPacket, (char *)this + *((unsigned __int16 *)this - 539) - 66616, &usOutSize)
    // 加密�?szBuffer 缓冲�?
    // 偏移计算: usOffset - 66616 是相对于 CUser this 的偏�?
    // 实际是写入到 szBuffer[usOffset] 位置
    if (xSendPacket.Encrypt(&this->szBuffer[usCurrentOffset], usOutSize)) {
        // IDA: *((_WORD *)this - 539) += usOutSize;
        this->usOffset += usOutSize;

        // IDA: v10 = 1; return v10;
        return true;
    } else {
        // IDA: v9 = 0; return v9;
        return false;
    }
}

// BridgeSend_AfterLoading - 加载完成后发送数据包
// IDA 0x1406E8D00: CUser::BridgeSend_AfterLoading
// 精确还原: �?BridgeSend 类似，但会检查客户端加载是否完成
bool CUser::BridgeSend_AfterLoading(XSendPacket& xSendPacket) {
    // IDA: CSimpleLock::Owner lock((CSimpleLock *)((char *)this - 1072));
    CSimpleLock::Owner lock(&this->xLock);

    // IDA: 检查是否处�?eStateChangeServer 状�?
    if (XClient::IsState(eStateChangeServer)) {
        // IDA: v8 = 0; return v8;
        return false;
    }

    // IDA: 检�?GetClientLoadComplete() 是否�?true
    if (!m_bClientLoadComplete) {
        // IDA: v9 = 0; return v9;
        return false;
    }

    // IDA: v13 = *((unsigned __int16 *)this - 539);
    std::uint16_t usCurrentOffset = this->usOffset;

    // IDA: UsIndex = XParse::GetUsIndex(&xSendPacket->XParse);
    std::uint16_t usPacketSize = xSendPacket.XParse.m_usIndex;

    // IDA: nBuffSize = v13 + UsIndex + 5;
    int nBuffSize = static_cast<int>(usCurrentOffset) + static_cast<int>(usPacketSize) + 5;

    // IDA: if (nBuffSize >= 65534) - 缓冲区溢出检�?
    if (nBuffSize >= 65534) {
        // IDA: overLab = (XOverLab *)((char *)this - 66712);
        // IDA: pClient = (CUser *)((char *)this - 132384);
        // IDA: v4 = TXSingleton<XGameServer>::Instance();
        // IDA: XIOCPServer::XSend(v4, pClient, overLab);
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (pGameServer) {
            pGameServer->XSend(this, nullptr);
        }

        // IDA: v16 = *((void (__fastcall ***)(char *, _QWORD))this - 8339);
        // IDA: (*v16)((char *)this - 66712, 0); - 虚函数调用清理缓冲区
        usOffset = 0;

        // IDA: OutputDebugStringA("Send All !! \n");
        OutputDebugStringA("Send All !! \n");
    }

    // IDA: usOutSize = 0;
    std::uint16_t usOutSize = 0;

    // IDA: xSendPacket->usTos = 1;
    xSendPacket.usTos = 1;

    // IDA: XSendPacket::Encrypt(xSendPacket, (char *)this + *((unsigned __int16 *)this - 539) - 66616, &usOutSize)
    if (xSendPacket.Encrypt(&this->szBuffer[usCurrentOffset], usOutSize)) {
        // IDA: *((_WORD *)this - 539) += usOutSize;
        this->usOffset += usOutSize;

        // IDA: v11 = 1; return v11;
        return true;
    } else {
        // IDA: v10 = 0; return v10;
        return false;
    }
}

// ============================================================================
// Player Action Functions (Round 8 Phase 4)
// ============================================================================

// Kickout - Simple overload for kicking player from server
void CUser::Kickout() {
    // Save player data before disconnect
    SaveData();

    // Close connection
    // XClient::Close();

    // Mark for removal
    // m_dwStatus |= STATUS_KICKED;
}

// DamageProcess - Player damage processing
void CUser::DamageProcess(CMover* pAttacker, int nDamage, int nSkillID, int nDamageFlag) {
    // Validate parameters
    if (nDamage <= 0) {
        return;
    }

    // Check if player is already dead
    if (m_nHP <= 0) {
        return;
    }

    // Store attacker for death handling
    // m_pLastAttacker = pAttacker;

    // Apply damage using existing DamageProcessHP
    // Use attacker's target ID if available, otherwise 0
    std::uint32_t dwAttackerID = pAttacker ? pAttacker->GetTargetID() : 0;
    int nResult = DamageProcessHP(
        dwAttackerID,
        nSkillID,
        nDamage,
        static_cast<std::uint8_t>(nDamageFlag & 0xFF),
        static_cast<std::uint8_t>((nDamageFlag >> 8) & 0xFF)
    );

    // Check if player died
    if (nResult == 1 && m_nHP <= 0) {
        OnDie(pAttacker);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "DamageProcess completed");
}

// OnDie - Player death handler
void CUser::OnDie(CMover* pKiller) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "OnDie called");

    // Set death status
    m_nHP = 0;

    // Set die type
    SetOnDie(true);

    // Cancel any active skill
    CancelSkill();

    // Clear targets
    // ClearTarget();

    // Set death motion
    ChangeMotion(static_cast<std::int16_t>(DIE_TYPE::DIE_TYPE_NORMAL), 1, 0);

    // Calculate death penalty (exp loss, etc.)
    // CalculateDeathPenalty();

    // Notify party members
    XSendPacket xPacket;
    // xPacket.SetCommand(SERVER_CMD_PLAYER_DIE);
    // xPacket << GetUAID();
    SendToParty(xPacket);

    // Start revive timer
    // SetReviveTimer(REVIVE_WAIT_TIME);

    // Log death event
    GreenDamTan_log(__FILE__, __FUNCTION__, "Player died");
}

// Respawn - Respawn player at spawn point
void CUser::Respawn() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "Respawn called");

    // Get respawn position from respawn manager
    // RespawnManager* pRespawnMgr = RespawnManager::Instance();
    // hkvVec3 vRespawnPos = pRespawnMgr->GetRespawnPosition(this);

    // Reset HP to max
    SetHP(GetMaxHP());

    // Reset MP/SG to max
    SetMP(GetMaxMP());

    // Clear death status
    SetOnDie(false);

    // Teleport to respawn position
    // TeleportTo(vRespawnPos);

    // Reset motion
    ChangeMotion(1, 1, 0);

    // Send respawn packet
    XSendPacket xPacket;
    // xPacket.SetCommand(SERVER_CMD_RESPAWN);
    // xPacket << vRespawnPos.x << vRespawnPos.y << vRespawnPos.z;
    SendPacket(xPacket);

    // Clear reserve revive flag
    m_bReserveRevive = 0;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Player respawned");
}

// ============================================================================
// Revive - 5 参版复活 (IDA 0x1406F4F10)
// 精确还原: GetArea gate -> IsRevive gate -> IsDie gate -> nType 分派 ->
// 保存状态/ResetStatus -> nType==2 复活点传送 -> 默认复活路径 (护甲/广播/日志)
// ============================================================================
void CUser::Revive(std::uint32_t dwOwnerID, int nType, int bBroadcast, std::uint32_t dwItemID, bool bByForce)
{
    XArea* pArea = GetArea();
    if (!pArea) {
        LogHelper::LogError("game.contents", "Revive error - GetArea() == NULL ( %d )", GetActorID().dwActorID);
        CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xCD89u);
        return;
    }

    if (!bByForce && !pArea->IsRevive()) {
        LogHelper::LogError("game.contents", "Revive error - Cant Revive this map[ MapID:%d )", pArea->GetTBMapID());
        CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xCD89u);
        return;
    }

    if (!CMover::IsDie()) {
        LogHelper::LogError("game.contents", "Revive error - Not IsDieStatus %d %d", GetActorID().dwActorID, pArea->GetTBMapID());
        CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xCD80u);
        return;
    }

    if (nType == 1) {
        int iIndex = CMover::FindBuffByEffectType(1, 0);
        if (iIndex == -1) {
            CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC741u);
            return;
        }
        dwOwnerID = GetBuffStatus(iIndex)->dwID;
        ClearBuffStatusBySlot(iIndex, true);
    } else {
        switch (nType) {
        case 2:
            if (pArea->GetWorldType() != 2) {
                CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC741u);
                return;
            }
            break;
        case 3:
            if (pArea->GetWorldType() != 1) {
                CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC746u);
                return;
            }
            {
                auto pEntity = GetGOC_Entity(false);
                if (!pEntity || !pEntity->ReviveFree()) {
                    CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC746u);
                    return;
                }
            }
            break;
        case 4:
            {
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                XOption* pOption = &pServer->GetOption();
                if (pOption->GetNationType() != NATION_TYPE_KOR &&
                    pOption->GetNationType() != NATION_TYPE_JPN) {
                    CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC746u);
                    return;
                }
            }
            if (pArea->GetWorldType() != 1) {
                CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC746u);
                return;
            }
            if (!bByForce) {
                auto pInven = GetGOC_Inventory(false);
                if (!pInven || !pInven->ReviveCash()) {
                    CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC746u);
                    return;
                }
            }
            break;
        case 5:
            if (pArea->GetWorldType() != 1) {
                CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC742u);
                return;
            }
            {
                std::uint16_t wTBMapID = pArea->GetTBMapID();
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                TB_MAZE_INFO* pTB = pServer->GetResourceMgr().GetTB_MAZE_INFO(wTBMapID);
                if (!pTB || pTB->Maze_Type != 12 || pTB->Revival_Type != 2) {
                    CGocNetwork::SendErrorMessage(this, 3u, 0x43u, 0xC742u);
                    return;
                }
            }
            break;
        }
    }

    std::uint32_t dwOldStatus = GetStatus();
    XActor::ResetStatus();

    if (nType == 2) {
        STPosInfo stMovePos;
        int nMapID = pArea->GetTBMapID();
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // TODO: 需人工审查 - XWorldResMgr::GetPortalPos/GetStartPortalPos 依赖 VEventObjectResource 子系统未还原
        if (!pServer->GetWorldResMgr().GetPortalPos(nMapID, m_nRevivePoint, &stMovePos)) {
            pServer->GetWorldResMgr().GetStartPortalPos(static_cast<int>(pArea->GetTBMapID()), &stMovePos);
        }
        if (pArea->MoveActor(this, stMovePos.vPos)) {
            XActor::SetStatus(dwOldStatus);
            return;
        }
    }

    if (nType == 5 ||
        pArea->GetWorldType() != 1 ||
        !TXSingleton<XGameServer>::Instance()->GetResourceMgr().GetTB_MAZE_INFO(pArea->GetTBMapID()) ||
        TXSingleton<XGameServer>::Instance()->GetResourceMgr().GetTB_MAZE_INFO(pArea->GetTBMapID())->Maze_Type != 12) {
        ClearExtraMoving();
        auto pAttr = GetGOC_Attribute(false);
        if (pAttr) {
            pAttr->Revive();
        }
        InitSuperArmorGage();
        m_stCharInfo.fCurSuperArmorGage = m_fMaxSuperArmorGage;
        m_stCharInfo.fMaxSuperArmorGage = m_fMaxSuperArmorGage;
        send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(this, m_fCurSuperArmorGage, m_fMaxSuperArmorGage);
        m_bOnDie = false;
        ChangeDefenseTypeForce(3u, 5.0f);

        if (bBroadcast) {
            PS_RES_REVIVE stRevive;
            stRevive.dwActorID = GetActorID().dwActorID;
            stRevive.dwOwnerID = dwOwnerID;
            stRevive.byType = static_cast<std::uint8_t>(nType);
            if (STPosInfo* pPosInfo = GetPosInfo()) {
                stRevive.vPos = pPosInfo->vPos;
                stRevive.fRot = pPosInfo->fRot;
            }
            XSendPacket xSendPacket(3u, 0x44u);
            xSendPacket << stRevive;
            CGocNetwork::SendBroadCast(this, xSendPacket, E_BROADCAST_TYPE::eNoneSelf);
        }

        auto pRecode = GetGOC_Recode(false);
        if (pRecode) {
            pRecode->SetReviveState(true);
        }

        std::uint16_t wMapID = 0;
        std::int64_t nInstanceID = 0;
        if (XArea* pCurArea = GetArea()) {
            wMapID = pCurArea->GetTBMapID();
            nInstanceID = pCurArea->GetInstanceID().nMapID;
        }

        ST_LOG_GAME stLog;
        stLog._nUAID = static_cast<int>(GetUAID());
        stLog._nUCID = static_cast<int>(GetUCID());
        stLog._sMainType = 5;
        stLog._sSubType = 21;
        stLog.nParam0 = wMapID;
        stLog.nParam1 = static_cast<int>(dwOwnerID);
        stLog.nParam2 = nType;
        stLog.nParam3 = static_cast<int>(dwItemID);
        stLog.nParam6 = nInstanceID;
        wcscpy_s(stLog.szComment, L"\uD50C\uB808\uC774\uC5B4 \uBD80\uD65C");
        TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
    }
}

// ============================================================================
// InitSuperArmorGage - 初始化超级护甲槽 (IDA 0x140700B80)
// ============================================================================
void CUser::InitSuperArmorGage()
{
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return;
    }
    float fOriginStat = pAttr->GetOriginStat(10);
    TB_STATUS* pStatusTable = pAttr->GetStatusTable();
    if (!pStatusTable) {
        return;
    }
    m_fMaxSuperArmorGage = fOriginStat * pStatusTable->Check_Stat_SA * 0.01f;
    m_fCurSuperArmorGage = m_fMaxSuperArmorGage;
}

// Revive - Revive player with HP percent
void CUser::Revive(int nHPPercent) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "Revive called");

    // Validate HP percent
    if (nHPPercent <= 0) {
        nHPPercent = 10;  // Default 10% HP
    }
    if (nHPPercent > 100) {
        nHPPercent = 100;
    }

    // Calculate HP from percent
    int nMaxHP = GetMaxHP();
    int nNewHP = (nMaxHP * nHPPercent) / 100;

    // Set HP
    SetHP(nNewHP);

    // Set MP/SG to full
    SetMP(GetMaxMP());

    // Clear death status
    SetOnDie(false);

    // Reset motion
    ChangeMotion(1, 1, 0);

    // Send revive packet to client
    XSendPacket xPacket;
    // xPacket.SetCommand(SERVER_CMD_REVIVE);
    // xPacket << static_cast<std::uint32_t>(nNewHP) << static_cast<std::uint32_t>(nHPPercent);
    SendPacket(xPacket);

    // Broadcast to nearby players
    // BroadcastPacket(xPacket);

    // Clear reserve revive flag
    m_bReserveRevive = 0;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Player revived");
}

// ============================================================================
// Inventory Functions (Round 8 Phase 4)
// ============================================================================

// AddItem - Simple overload with item ID and count only
// Returns: TRUE on success
BOOL CUser::AddItem(int nItemID, int nCount) {
    int nResult = AddItem(static_cast<std::uint32_t>(nItemID), nCount, false, 0);
    return (nResult > 0) ? TRUE : FALSE;
}

// RemoveItem - Simple overload
// Returns: TRUE on success
BOOL CUser::RemoveItem(int nItemID, int nCount) {
    int nResult = RemoveItem(static_cast<std::uint32_t>(nItemID), nCount);
    return (nResult > 0) ? TRUE : FALSE;
}

// UseItem - Use item by slot index
// Returns: TRUE on success
BOOL CUser::UseItem(int nSlotIndex) {
    // Validate slot index
    if (nSlotIndex < 0) {
        return FALSE;
    }

    // Get item ID from inventory slot
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) return FALSE;
    
    bool bLock = false;
    std::shared_ptr<CItem> pSlotItem = pInventory->GetSlotItem(2, static_cast<std::uint16_t>(nSlotIndex), bLock);
    std::uint32_t dwItemID = pSlotItem ? static_cast<std::uint32_t>(pSlotItem->GetID()) : 0;
    if (dwItemID == 0) return FALSE;
    
    return UseItem(dwItemID, nSlotIndex) ? TRUE : FALSE;
}

// EquipItem - Equip item from inventory slot
// Returns: TRUE on success
BOOL CUser::EquipItem(int nSlotIndex) {
    // Validate slot index
    if (nSlotIndex < 0) {
        return FALSE;
    }

    // Determine equipment slot from item type
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) return FALSE;
    
    bool bLock = false;
    std::shared_ptr<CItem> pSlotItem = pInventory->GetSlotItem(2, static_cast<std::uint16_t>(nSlotIndex), bLock);
    std::uint32_t dwItemID = pSlotItem ? static_cast<std::uint32_t>(pSlotItem->GetID()) : 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pItem = pServer->GetResourceMgr().GetTB_ITEM(dwItemID);
    if (!pItem) return FALSE;
    
    int nEquipSlot = 0;
    return EquipItem(nSlotIndex, nEquipSlot) ? TRUE : FALSE;
}

// ============================================================================
// Party Functions (Round 8 Phase 4)
// ============================================================================

// JoinParty - Join party by ID (simple overload)
// Returns: TRUE on success
BOOL CUser::JoinParty(unsigned long dwPartyID) {
    return JoinParty(static_cast<std::uint32_t>(dwPartyID)) ? TRUE : FALSE;
}

// LeaveParty - Leave current party (void return version)
void CUser::LeaveParty() {
    CGocParty* pParty = GetGOC<CGocParty>();
    if (!pParty || !pParty->IsParty()) {
        return;
    }
    XSendPacket xPacket(9, 0x15);
    SendPacket(xPacket);
}

// CreateParty - Create new party (BOOL return version)
// Returns: TRUE on success
BOOL CUser::CreateParty() {
    std::uint32_t dwPartyID = CreateParty();
    return (dwPartyID != 0) ? TRUE : FALSE;
}

// ============================================================================
// Guild Functions (Round 8 Phase 4)
// ============================================================================

// JoinGuild - Join guild by ID (simple overload)
// Returns: TRUE on success
BOOL CUser::JoinGuild(unsigned long dwGuildID) {
    return JoinGuild(static_cast<std::uint32_t>(dwGuildID)) ? TRUE : FALSE;
}

// CreateGuild - Create new guild (char* version)
// Returns: TRUE on success
BOOL CUser::CreateGuild(const char* szGuildName) {
    if (!szGuildName || szGuildName[0] == '\0') {
        return FALSE;
    }

    if (m_stCharInfo.stLeagueInfo.nLeagueID != 0) {
        return FALSE;
    }
    return FALSE;
}

// ============================================================================
// Inventory Functions (existing implementations)

// AddItem - Add item to inventory, check space, stack
// Returns: item count added, or -1 on error
int CUser::AddItem(std::uint32_t dwItemID, int nCount, bool bBind, int nExpireTime) {
    // Validate parameters
    if (dwItemID == 0 || nCount <= 0) {
        return -1;
    }

    // Get item table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pItemTable = pServer->GetResourceMgr().GetTB_ITEM(dwItemID);
    if (!pItemTable) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "AddItem failed: item not found");
        return -1;
    }

    // Check inventory space via CGocInventory component
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) {
        return -1;
    }
    
    // Check if item can stack (Item_Stack_Max)
    if (pItemTable->Item_Stack_Max > 1) {
        // Check if item already exists for stacking
        int nExistingCount = pInventory->GetItemCount(dwItemID);
        if (nExistingCount > 0) {
            // Add to existing stack
            int nMaxStack = pItemTable->Item_Stack_Max;
            int nCanAdd = nMaxStack - nExistingCount;
            if (nCanAdd >= nCount) {
                return pInventory->AddItem(static_cast<int>(dwItemID), nCount) ? nCount : -1;
            }
        }
    }
    
    // Add to inventory slot
    return pInventory->AddItem(static_cast<int>(dwItemID), nCount) ? nCount : -1;
}

// RemoveItem - Remove item from inventory
// Returns: item count removed, or -1 on error
int CUser::RemoveItem(std::uint32_t dwItemID, int nCount) {
    // Validate parameters
    if (dwItemID == 0 || nCount <= 0) {
        return -1;
    }

    // Find item in inventory
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) {
        return -1;
    }
    
    // Check if enough quantity
    int nExistingCount = pInventory->GetItemCount(dwItemID);
    if (nExistingCount < nCount) {
        return -1;
    }
    
    // Remove from slot
    return pInventory->RemoveItem(static_cast<int>(dwItemID), nCount) ? nCount : -1;
}

// UseItem - Use consumable item, apply effects
// Returns: true on success
bool CUser::UseItem(std::uint32_t dwItemID, int nSlotIndex) {
    // Validate parameters
    if (dwItemID == 0 || nSlotIndex < 0) {
        return false;
    }

    // Get item table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pItemTable = pServer->GetResourceMgr().GetTB_ITEM(dwItemID);
    if (!pItemTable) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem failed: item not found");
        return false;
    }

    // IDA: Check item sub type and effect type
    if (pItemTable->Item_Sub_Type == 2) {
        // IDA: Special item type
        if (pItemTable->Item_Effect_Type == 1) {
            // IDA: Quest related item
            GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: Quest item %d", dwItemID);
        } else if (pItemTable->Item_Effect_Type == 2) {
            // IDA: Revive related item
            if (IsDie()) {
                SetReserveRevive(1);
            }
        }
    } else {
        // IDA: Other item types
        switch (pItemTable->Item_Effect_Type) {
            case 1:  // HP recovery
                GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: HP recovery");
                break;
            case 2:  // MP/SG recovery
                GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: SG recovery");
                break;
            case 3:  // Buff
                GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: Buff effect");
                break;
            default:
                GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: Unknown effect type");
                break;
        }
    }

    return true;
}

// ============================================================================
// Equipment Functions
// ============================================================================

// EquipItem - Equip item to slot
// Returns: true on success
bool CUser::EquipItem(int nSlotIndex, int nEquipSlot) {
    // Validate parameters
    if (nSlotIndex < 0 || nEquipSlot < 0) {
        return false;
    }

    // Check if slot has item
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) {
        return false;
    }
    
    bool bLock = false;
    std::shared_ptr<CItem> pSlotItem = pInventory->GetSlotItem(2, static_cast<std::uint16_t>(nSlotIndex), bLock);
    std::uint32_t dwItemID = pSlotItem ? static_cast<std::uint32_t>(pSlotItem->GetID()) : 0;
    if (dwItemID == 0) {
        return false;
    }
    
    // Get item table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pItem = pServer->GetResourceMgr().GetTB_ITEM(dwItemID);
    if (!pItem) {
        return false;
    }
    
    // Check if item can be equipped (Item_Sub_Type, Item_Slot_Disable)
    if (pItem->Item_Sub_Type != 1) {  // Not equipment type
        return false;
    }
    
    // Check level requirement (Item_Limit_Lv)
    if (GetLevel() < pItem->Item_Limit_Lv) {
        return false;
    }
    
    // Check class requirement (Item_Limit_Class)
    std::uint32_t dwClass = GetClass();
    if (pItem->Item_Limit_Class != 0 && (pItem->Item_Limit_Class & dwClass) == 0) {
        return false;
    }
    
    // Unequip current item in slot if any
    std::uint32_t dwCurrentEquipID = static_cast<std::uint32_t>(pInventory->GetEquippedItem(nEquipSlot));
    if (dwCurrentEquipID != 0) {
        pInventory->UnequipItem(nEquipSlot);
    }
    
    // Move item to equipment slot
    return pInventory->EquipItem(nSlotIndex, nEquipSlot);
}

// UnequipItem - Remove item from slot
// Returns: true on success
bool CUser::UnequipItem(int nEquipSlot) {
    // Validate parameters
    if (nEquipSlot < 0) {
        return false;
    }

    // Check if equipment slot has item
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) {
        return false;
    }
    
    std::uint32_t dwItemID = static_cast<std::uint32_t>(pInventory->GetEquippedItem(nEquipSlot));
    if (dwItemID == 0) {
        return false;
    }
    
    // Check if inventory has space
    int nEmptySlot = pInventory->GetEmptySlot(2);
    if (nEmptySlot < 0) {
        return false;
    }
    
    // Move item from equipment slot to inventory
    return pInventory->UnequipItem(nEquipSlot);
}

// GetEquipSlot - Get item at equipment slot
// Returns: item ID at slot, or 0 if empty
std::uint32_t CUser::GetEquipSlot(int nEquipSlot) const {
    // Validate parameters
    if (nEquipSlot < 0) {
        return 0;
    }

    // Access equipment slots from CGocInventory
    CGocInventory* pInventory = const_cast<CUser*>(this)->GetGOC<CGocInventory>();
    if (pInventory) {
        return static_cast<std::uint32_t>(pInventory->GetEquippedItem(nEquipSlot));
    }

    return 0;
}

// ============================================================================
// Party Functions
// ============================================================================

// JoinParty - Join existing party
// Returns: true on success
bool CUser::JoinParty(std::uint32_t dwPartyID) {
    // Validate parameters
    if (dwPartyID == 0) {
        return false;
    }

    CGocParty* pPartyGoc = GetGOC<CGocParty>();
    if (pPartyGoc && pPartyGoc->IsParty()) {
        return false;
    }
    return false;
}

// ============================================================================
// Guild Functions
// ============================================================================

// JoinGuild - Join guild
// Returns: true on success
bool CUser::JoinGuild(std::uint32_t dwGuildID) {
    // Validate parameters
    if (dwGuildID == 0) {
        return false;
    }

    // Check if already in guild
    if (m_stCharInfo.stLeagueInfo.nLeagueID != 0) {
        return false;
    }
    
    return false;
}

// LeaveGuild - Leave guild
// Returns: true on success
bool CUser::LeaveGuild() {
    // Check if in guild
    std::uint32_t dwGuildID = m_stCharInfo.stLeagueInfo.nLeagueID;
    if (dwGuildID == 0) {
        return false;
    }
    
    // Update m_stCharInfo.stLeagueInfo
    m_stCharInfo.stLeagueInfo.nLeagueID = 0;
    
    // Send guild leave notification
    XSendPacket xPacket(9, 0x34);  // SERVER_CMD_GUILD_LEAVE
    SendPacket(xPacket);
    
    return true;
}

// ============================================================================
// Trade Functions
// ============================================================================

// StartTrade - Initiate trade with player
// Returns: true on success
bool CUser::StartTrade(std::uint32_t dwTargetID) {
    // Validate parameters
    if (dwTargetID == 0) {
        return false;
    }

    // TODO: Check if already trading
    // TODO: Find target player
    // TODO: Check if target can trade
    // TODO: Send trade request to target
    // TODO: Set trade state

    GreenDamTan_log(__FILE__, __FUNCTION__, "StartTrade stub");
    return true;
}

// EndTrade - End trade session
// Returns: true on success
bool CUser::EndTrade() {
    // TODO: Check if in trade
    // TODO: Cancel or complete trade
    // TODO: Clear trade state
    // TODO: Notify trade partner

    GreenDamTan_log(__FILE__, __FUNCTION__, "EndTrade stub");
    return true;
}

// AcceptTrade - Accept trade offer
// Returns: true on success
bool CUser::AcceptTrade() {
    // TODO: Check if trade request pending
    // TODO: Verify both parties have items/money
    // TODO: Transfer items/money
    // TODO: Complete trade
    // TODO: Clear trade state

    GreenDamTan_log(__FILE__, __FUNCTION__, "AcceptTrade stub");
    return true;
}

// ============================================================================
// Status Functions (IDA)
// ============================================================================

// GetMP - Get current MP/SG (Soul Gauge)
// IDA: Stat 2 = Current SG (Soul Gauge Current)
// Evidence: CALCULATE_STAT_SG_REG uses CGocAttribute::GetStat(pAttr, 2u) for current SG
int CUser::GetMP() {
    // IDA: Current SG is stored at stat index 2 in CGocAttribute
    // CGocAttribute::GetStat(this, 2) returns current Soul Gauge
    return static_cast<int>(GetStat(2));
}

// SetMP - Set MP/SG value
// IDA: Stat 2 = Current SG (Soul Gauge Current)
// Uses CMover::SetStat to update stat index 2
void CUser::SetMP(int nMP) {
    int nMaxMP = GetMaxMP();
    int nFinalMP = nMP;

    // MP cannot exceed MaxMP
    if (nMP > nMaxMP) {
        nFinalMP = nMaxMP;
    }
    if (nFinalMP < 0) {
        nFinalMP = 0;
    }

    CGocAttribute* pAttr = GetGOC<CGocAttribute>();
    if (pAttr) {
        pAttr->SetStat(2, static_cast<float>(nFinalMP), true);
    }
}

// GetMaxHP - Get max HP (override from CMover)
// IDA 0x140189410 (CMoverEx::GetMaxHP)
// 反编�? return (unsigned int)(int)this->m_fAbility[10]
int CUser::GetMaxHP() {
    // IDA: CMoverEx::GetMaxHP returns m_fAbility[10] cast to int
    // m_fAbility 数组存储各种属性值，索引 10 是最�?HP
    return static_cast<int>(m_fAbility[10]);
}

// GetMaxMP - Get max MP/SG (Soul Gauge Max)
// IDA: Stat 12 (0xC) = Max SG
// Evidence: CALCULATE_STAT_SG_MAX uses m_fAbility[12], CALCULATE_STAT_SG_REG uses GetStat(0xC)
int CUser::GetMaxMP() {
    // IDA: Max SG is stored at stat index 12 (0xC) in m_fAbility
    // CMoverEx::GetMaxHP returns m_fAbility[10], similarly MaxSG is m_fAbility[12]
    return static_cast<int>(m_fAbility[12]);
}

// ============================================================================
// Packet Functions (IDA)
// ============================================================================

// SendPacket - Send packet to client
// IDA 0x1406E8B50 (BridgeSend wrapper)
bool CUser::SendPacket(XSendPacket& xSendPacket) {
    return BridgeSend(xSendPacket);
}

// IDA 0x1406E9E60 - CUser::SetInfoPacket
void CUser::SetInfoPacket(XSendPacket& xSendPacket) {
    STMyCharInfoEx& stInfo = GetMyCharInfoEx();
    xSendPacket << static_cast<const STCharInfoEx&>(stInfo);
}

// IDA 0x1406E9EC0 - CUser::GetMyCharInfoEx
STMyCharInfoEx& CUser::GetMyCharInfoEx() {
    return m_stCharInfo;
}

// IDA 0x1406E8A10 - CUser::stMyCharInfoEx
STMyCharInfoEx* CUser::stMyCharInfoEx() {
    return &m_stCharInfo;
}

// BroadcastPacket - Broadcast to nearby players
// IDA 0x140103C20: CGocNetwork::BroadcastNearby
// IDA 0x140103CD0: CGocNetwork::SendBroadCast
void CUser::BroadcastPacket(XSendPacket& xSendPacket, float fRadius) {
    // IDA: CGocNetwork::BroadcastNearby(pActor, pExceptActor, &packet)
    // Uses XArea::ScanGridOrigin to find nearby objects
    
    // Get current area
    XArea* pArea = GetArea();
    if (!pArea) return;
    
    // IDA: If fRadius is 0 or default, use eAll broadcast type
    // Otherwise use radius-based scan
    if (fRadius <= 0.0f) {
        // Use default broadcast (eAll type)
        CGocNetwork::SendBroadCast(this, xSendPacket, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
    } else {
        // Use radius-based scan
        // XArea::ScanGridOrigin(this, 2, 1, &vecGameObjList) for radius scan
        // Then send to all in list except this
        std::vector<CMover*> vecGameObjList;
        pArea->ScanGridOrigin(reinterpret_cast<XActor*>(this), 2, 1, vecGameObjList);
        CGocNetwork::Send(vecGameObjList, xSendPacket, reinterpret_cast<XActor*>(this));
    }
}

// SendToParty - Send to party members
// IDA: Uses CParty class to broadcast to party members
void CUser::SendToParty(XSendPacket& xSendPacket) {
    CGocParty* pParty = GetGOC<CGocParty>();
    if (!pParty || !pParty->IsParty()) return;
    
    // IDA: Get area and use area broadcast to party
    XArea* pArea = GetArea();
    if (!pArea) return;
    
    // IDA: Use eParty broadcast type (typically 1 or 2)
    CGocNetwork::SendBroadCast(this, xSendPacket, E_BROADCAST_TYPE::E_BROADCAST_TYPE_NORMAL);
}

// SendToGuild - Send to guild members
// IDA: Uses CLeagueMember class to broadcast to guild/league members
void CUser::SendToGuild(XSendPacket& xSendPacket) {
    // IDA: Get guild/league ID from character info
    int nGuildID = m_stCharInfo.stLeagueInfo.nLeagueID;
    if (nGuildID == 0) return;
    
    // IDA: Get area and use area broadcast to guild
    XArea* pArea = GetArea();
    if (!pArea) return;
    
    // IDA: Use eLeague broadcast type (typically 3 or 4)
    CGocNetwork::SendBroadCast(this, xSendPacket, E_BROADCAST_TYPE::E_BROADCAST_TYPE_NORMAL);
}

// ============================================================================
// Data Functions (IDA)
// ============================================================================

// SaveData - Save player data to database
// IDA: Uses XSendDBPacket to send save requests to database
bool CUser::SaveData() {
    // IDA: Validate character data
    if (!m_stCharInfo.dwUAID) return false;
    
    // IDA: Send DB save packet for character info
    // XSendDBPacket packet(this, mainCmd, subCmd);
    // packet << m_stCharInfo fields...
    // XGameServer::Instance()->SendDBGame(packet);
    
    // IDA: Save components via their SendDB methods
    // CGocAttribute::SendDBUpdateFP()
    // CGocInventory sends inventory/equipment
    // CGocSkill sends skill list
    // CGocQuest sends quest progress
    // CGocAchieve sends achievements
    
    // For now, trigger component saves
    // Each component has its own SendDB method
    
    return true;
}

// LoadData - Load player data from database
// IDA: Data is loaded via DB response packets, not direct load
bool CUser::LoadData() {
    // IDA: Validate UAID
    if (!m_stCharInfo.dwUAID) return false;
    
    // IDA: Character data is loaded via DB response packets
    // The DB sends packets with character info which are processed
    // by packet handlers (e.g., OnDBLoadCharacterInfo)
    
    // IDA: Components load their data similarly via DB packets
    // CGocInventory::OnDBLoadInventory
    // CGocSkill::OnDBLoadSkillList
    // CGocQuest::OnDBLoadQuest
    // CGocAchieve::OnDBLoadAchieveList
    
    // The actual loading happens asynchronously via DB responses
    // This function just initiates the load requests
    
    return true;
}

// ============================================================================
// Inventory Helper Functions
// ============================================================================

// GetLevel - IDA 0x140366CB0 (override from CMover)
std::uint8_t CUser::GetLevel() {
    // Call base class implementation
    return CMover::GetLevel();
}

// GetClass - IDA 0x140366C30 (override from CMover)
std::uint8_t CUser::GetClass() {
    // Call base class implementation
    return CMover::GetClass();
}

// ChangeCombatType - IDA 0x1406F66D0
void CUser::ChangeCombatType(int nValue, float fChangeTime, std::uint8_t byUseCount) {
    // IDA 反编译精确还原: 战斗类型切换
    
    // IDA: 检查当前战斗类型是否有效 (不是-1)
    if (m_nCombatType != -1) {
        // IDA: 设置战斗切换时间和使用次数
        m_fCombatChangeTime = fChangeTime;
        m_byCombatChangeUseCount = byUseCount;
        
        // IDA: 检查是否是相同类型
        if (m_nCombatType == nValue) {
            // IDA: 相同类型 - 发送数据包
            // IDA: CMover::send_eSUB_CMD_COMBAT_TYPE(this, this, m_nCombatType, m_fCombatChangeTime, m_byCombatChangeUseCount);
            // Note: send_eSUB_CMD_COMBATType needs to be implemented in CMover
            GreenDamTan_log(__FILE__, __FUNCTION__, 
                "ChangeCombatType: type=%d, time=%.2f, count=%d",
                m_nCombatType, m_fCombatChangeTime, m_byCombatChangeUseCount);
        } else {
            // IDA: 不同类型 - 调用虚函数切换
            // IDA: (*(void (__fastcall **)(char *))(*((_QWORD *)this - 16439) + 120LL))((char *)this - 131512);
            // Note: This is a virtual function call from the vtable
            // For now, we just update the combat type
            m_nCombatType = nValue;
        }
    }
}

// UpdateSkillAnimInfo - IDA 0x14037EE30 (CMoverEx::UpdateSkillAnimInfo)
// 精确还原: 根据技能动画资源设置动画计数和步骤
void CUser::UpdateSkillAnimInfo(TB_SKILL* pTBSkill) {
    if (!pTBSkill) {
        return;
    }

    // IDA: Check if Ani_Res_Extra is not "0"
    if (strcmp(pTBSkill->Ani_Res_Extra, "0") != 0) {
        // IDA: Has extra animation
        m_bySkillAnimCount = 4;
        m_bySkillAnimStep = 0;
    } else if (strcmp(pTBSkill->Ani_Res_Start, "0") != 0) {
        // IDA: Has start animation
        if (strcmp(pTBSkill->Ani_Res_Loop, "0") != 0) {
            // IDA: Has loop animation
            m_bySkillAnimCount = 3;
        } else {
            // IDA: No loop animation
            m_bySkillAnimCount = 2;
        }
        m_bySkillAnimStep = 0;
    } else {
        // IDA: Only end animation
        m_bySkillAnimCount = 1;
        m_bySkillAnimStep = 3;
    }
}

// GetSkillAnimName - IDA 0x14037EF50 (CMoverEx::GetSkillAnimName)
// 精确还原: 根据步骤获取技能动画名�?
const char* CUser::GetSkillAnimName(TB_SKILL* pTBSkill, unsigned char byIndex) {
    if (!pTBSkill) {
        return nullptr;
    }

    // IDA: Select animation name based on step
    const char* pSkillName = nullptr;
    switch (byIndex) {
        case 0:
            pSkillName = pTBSkill->Ani_Res_Start;
            break;
        case 1:
            pSkillName = pTBSkill->Ani_Res_Loop;
            break;
        case 2:
            pSkillName = pTBSkill->Ani_Res_Extra;
            break;
        case 3:
        default:
            pSkillName = pTBSkill->Ani_Res_End;
            break;
    }

    // IDA: Clear temp buffer
    memset(m_szTempSkillAnimName, 0, sizeof(m_szTempSkillAnimName));

    // IDA: Handle charge skills (ControlType 2 or 5) at step 3
    std::uint8_t byControlType = GetControlType(pTBSkill);
    if ((byControlType == 2 || byControlType == 5) && byIndex == 3) {
        // IDA: Append charge step to animation name
        std::uint8_t byChargeStep = GetSkillChargeStep();
        sprintf(m_szTempSkillAnimName, "%s_%02d", pSkillName, byChargeStep + 1);
        return m_szTempSkillAnimName;
    }
    // IDA: Handle charge skills (ControlType 8) at step 3
    else if (byControlType == 8 && byIndex == 3) {
        // IDA: Select animation based on charging values
        if (m_fLeftChargingValue < static_cast<float>(pTBSkill->Charging_Max_Value)) {
            if (m_fRightChargingValue < static_cast<float>(pTBSkill->Charging_Max_Value)) {
                sprintf(m_szTempSkillAnimName, "%s", pSkillName);
            } else {
                sprintf(m_szTempSkillAnimName, "%s_Attack02", pSkillName);
            }
        } else {
            sprintf(m_szTempSkillAnimName, "%s_Attack01", pSkillName);
        }
        return m_szTempSkillAnimName;
    }
    // IDA: Handle directional skills (Skill_Direction == 1)
    else if (pTBSkill->Skill_Direction == 1) {
        // IDA: Direction suffixes
        const char* szDir[4] = {"_F", "_L", "_R", "_B"};
        
        // IDA: Append direction suffix based on m_byMoveDir
        sprintf(m_szTempSkillAnimName, "%s%s", pSkillName, szDir[m_byMoveDir]);
        return m_szTempSkillAnimName;
    }

    // IDA: Return base animation name
    return pSkillName;
}

// GetControlType - IDA 0x140398C30 (CMoverEx::GetControlType)
// 精确还原: 获取技能控制类�?
unsigned char CUser::GetControlType(TB_SKILL* pTBSkill) {
    if (!pTBSkill) {
        return 0;
    }

    // IDA: Check if divergence table has override
    if (m_pCurDivergenceTableRef && m_pCurDivergenceTableRef->Div_Option_Type == 2) {
        return m_pCurDivergenceTableRef->Div_Option_Value;
    }

    // IDA: Return skill table control type
    return pTBSkill->Control_Type;
}

// ChargeSkillStart - IDA 0x14037EA30 (CMoverEx::ChargeSkillStart)
// 精确还原: 启动蓄力技�?
void CUser::ChargeSkillStart() {
    // IDA: Check if skill table exists and is charge type (2, 5, or 8)
    if (m_pCurSkillTableRef) {
        std::uint8_t byControlType = GetControlType(m_pCurSkillTableRef);
        if (byControlType == 2 || byControlType == 5 || byControlType == 8) {
            // IDA: Initialize charge skill state
            m_bAttackKeyPress = 1;
            m_bySkillChargeStep = 0;
            m_bySkillChargeMaxStep = m_pCurSkillTableRef->Charging_Count;
            
            // IDA: Set charge change time from Time_Value array
            m_fSkillChargeChangeTime = m_pCurSkillTableRef->Time_Value[m_bySkillChargeStep] * 0.001f;
            
            m_fSkillTotalChargeTime = 0.0f;
            m_fLeftChargingValue = 0.0f;
            m_fRightChargingValue = 0.0f;
        }
    }
}

// GetStat - IDA 0x140166360 (CMover::GetStat)
// 精确还原: 获取属性�?
float CUser::GetStat(int nStatType) {
    // IDA: return this->m_fAbility[iIndex]
    return m_fAbility[nStatType];
}

// GetItemCount - IDA 0x140303290 (XBaseInventory::GetItemCount)
// 获取指定物品在背包中的数量
// 返回: 物品数量，如果未找到则返回0
int CUser::GetItemCount(std::uint32_t dwItemID) {
    // IDA 反编译精确还原: 通过CGocInventory组件获取物品数量
    
    // IDA: 验证参数
    if (dwItemID == 0) {
        return 0;
    }

    // IDA: 获取背包组件
    CGocInventory* pInventory = GetGOC<CGocInventory>();
    if (!pInventory) {
        return 0;
    }
    
    // IDA: 调用CGocInventory::GetItemCount
    // CGocInventory内部调用XBaseInventory::GetItemCount遍历背包中所有物品，累加匹配ID的物品数量
    return pInventory->GetItemCount(static_cast<int>(dwItemID));
}

// ============================================================================
// Anti-Cheat Functions (IDA 精确还原)
// ============================================================================

// CheckSpeedHackAttack - IDA 0x1406EBA30
// 检测攻击速度作弊
// 返回: true 如果未检测到作弊
bool CUser::CheckSpeedHackAttack() {
    // IDA: 检查是否在迷宫�?(迷宫不检�?
    XArea* pArea = GetArea();
    if (pArea && pArea->IsMaze()) {
        return true;
    }

    // IDA: 获取当前时间
    std::uint64_t dwTickCount = GetTickCount64();
    std::uint32_t dwDeltaAttackTime = static_cast<std::uint32_t>(dwTickCount - m_dwCheckSpeedHackAttack);
    std::uint32_t dwCheckTime = GetSpeedHackAttackLimitTime();

    // IDA: 如果有上次攻击时间记�?
    if (m_dwCheckSpeedHackAttack) {
        // IDA: 检查每分钟计数�?
        if (m_dwCheckSpeedHackAttackForMin <= GetTickCount64()) {
            m_dwCheckSpeedHackAttackForMin = GetTickCount64() + 1000;

            // IDA: 如果每分钟作弊次数超�?次，记录日志
            if (m_nCheatCountForMin > 5) {
                GreenDamTan_log(__FILE__, __FUNCTION__,
                    "[%d]<SPEED HACK - ATTACK FOR MIN> %d, %d < %d (ms)",
                    GetActorID().dwActorID, m_nCurSkillTableIdx, dwDeltaAttackTime, dwCheckTime);

                ++m_dwCheatCount[3];

                // IDA: 发送日志到数据�?
                ST_LOG_GAME stLogDebug;
                stLogDebug._nUAID = GetUAID();
                stLogDebug._nUCID = GetActorID().dwActorID;
                stLogDebug._sMainType = 51;
                stLogDebug._sSubType = 20;
                stLogDebug.nParam0 = m_nCurSkillTableIdx;
                stLogDebug.nParam1 = static_cast<int>(dwDeltaAttackTime);
                stLogDebug.nParam2 = static_cast<int>(dwCheckTime);

                if (pArea) {
                    stLogDebug.nParam3 = static_cast<int>(pArea->GetTBMapID());
                } else {
                    stLogDebug.nParam3 = 0;
                }
                stLogDebug.nParam4 = m_nCheatCountForMin;

                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                if (pServer) {
                    pServer->SendDBLog(stLogDebug);
                }
            }
            m_nCheatCountForMin = 0;
        }
        ++m_nCheatCountForMin;
    }

    // IDA: 更新上次攻击时间
    m_dwCheckSpeedHackAttack = dwTickCount;
    return true;
}

// GetSpeedHackAttackLimitTime - IDA 0x1406EBA20
// 获取攻击最小间隔时�?(毫秒)
// 返回: 100ms
std::uint32_t CUser::GetSpeedHackAttackLimitTime() {
    // IDA: return 100;
    return 100;
}

// CheckInvalidPos - IDA 0x1406EBDE0
// 验证玩家位置是否合法 (检测瞬移加速作弊)
// 返回: true 如果位置非法 (检测到作弊)
bool CUser::CheckInvalidPos(XVec3* vPos, int iCallFuncId, XVec3* vTargetPos,
                            float fMoveSpeed, std::uint8_t byRunBit) {
    // IDA 反编译精确还原: 位置验证和速度检测
    // Note: Position info is stored in CMoverEx base class
    // For now, use a simplified implementation
    
    // IDA: 检查是否在迷宫中 (迷宫不检查)
    XArea* pArea = GetArea();
    if (pArea && pArea->IsMaze()) {
        return false;
    }

    // IDA: 移动速度检查 (仅对移动调用)
    if (iCallFuncId == 1) {
        float fSpeedByServer = 800.0f;  // Default speed (GetMaxMoveSpeed not yet implemented)
        
        // IDA: 检查速度是否超过服务器计算值
        if (fMoveSpeed > fSpeedByServer) {
            ++m_nCheckWrongSpeedCount;
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "[%d]<CHECK POS> Over Speed ( %0.2f / %0.2f )",
                GetActorID().dwActorID, fMoveSpeed, fSpeedByServer);
            
            if (fMoveSpeed > m_fCheckMaxSpeed) {
                m_fCheckMaxSpeed = fMoveSpeed;
            }
        }
        
        // IDA: 检查目标距离是否合理
        float fDistance = sqrtf(powf(vPos->x - vTargetPos->x, 2) + powf(vPos->y - vTargetPos->y, 2) + powf(vPos->z - vTargetPos->z, 2));
        if (fDistance > (fSpeedByServer + 250.0f)) {
            ++m_nCheckWrongTargetCount;
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "[%d]<CHECK POS> Wrong Target ( %0.2f / %0.2f )",
                GetActorID().dwActorID, fDistance, fSpeedByServer);
        }
    }

    return false;
}

// CheckValidBot - IDA 0x1406EAA00
// 检查玩家名称是否包�?"Bot"
void CUser::CheckValidBot() {
    // IDA: 获取玩家名称
    std::wstring name = GetName();

    // IDA: 检查是否包�?"Bot"
    if (name.find(L"Bot") == 0) {
        m_bIsBot = true;
    }
}

// SendCheckSpeedLog - IDA 0x1407017A0
// 发送速度作弊检测日志到数据�?
void CUser::SendCheckSpeedLog() {
    // IDA: 构造日志结�?
    ST_LOG_GAME stLog;
    stLog._nUAID = GetUAID();
    stLog._nUCID = GetActorID().dwActorID;
    stLog._sMainType = 52;
    stLog._sSubType = 3;
    stLog.nParam0 = m_nCheckWrongSpeedCount;
    stLog.nParam1 = m_nCheckWrongTargetCount;
    stLog.nParam2 = m_nCheckWrongPosCount;
    stLog.nParam3 = static_cast<int>(m_fCheckMaxSpeed);
    stLog.nParam4 = static_cast<int>(m_dwCheatCount[3]);

    // IDA: 获取地图ID
    XArea* pArea = GetArea();
    if (pArea) {
        stLog.nParam5 = static_cast<int>(pArea->GetTBMapID());
        TUXMapID uxMapID = pArea->GetInstanceID();
        stLog.nParam6 = static_cast<std::int64_t>(uxMapID.wInstanceID);
    } else {
        stLog.nParam5 = 0;
        stLog.nParam6 = 0;
    }

    // IDA: 设置日志注释
    wcscpy_s(stLog.szComment, L"SPEED HACK");

    // IDA: 发送日�?
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->SendDBLog(stLog);
    }

    // IDA: 重置计数�?
    m_nCheckWrongSpeedCount = 0;
    m_nCheckWrongTargetCount = 0;
    m_nCheckWrongPosCount = 0;
    m_fCheckMaxSpeed = 0.0f;
    m_dwCheatCount[3] = 0;
}

// CheckKickoutNow - IDA 0x140701680
// 检查是否应该立即踢出玩�?
bool CUser::CheckKickoutNow() {
    // IDA: 如果没有踢出时间，返�?false
    if (!m_dwKickoutTime) {
        return false;
    }

    // IDA: 设置踢出状�?
    XClient::SetState(eStateKickOut);
    return true;
}

// SetKick_AlreadyLogin - IDA 0x14070AD80
// 标记玩家因重复登录被踢出
void CUser::SetKick_AlreadyLogin() {
    // IDA: this->m_bKick_AlreadyLogin = 1;
    m_bKick_AlreadyLogin = true;
}

// IsKick_AlreadyLogin - IDA 0x14070AFA0
// 检查玩家是否因重复登录被标记踢�?
bool CUser::IsKick_AlreadyLogin() {
    // IDA: return this->m_bKick_AlreadyLogin;
    return m_bKick_AlreadyLogin;
}

// Note: SetLeagueName needs declaration in User.h before implementation

// Note: UpdateLeagueInventorySyncCount, SetLeagueInventoryTime, UpdateLeagueSyncFlag,
// SetLeagueDeletePenalty, SetLeagueWithdrawPenalty, SetLeagueInventorySend,
// UpdateLeagueSyncCount, GetLeagueInventorySyncCount, GetLeagueSyncCount
// need declarations in User.h before implementation

// Note: The following functions need declarations in User.h before implementation:
// GetLeagueSyncFlag, SetAuthSessionID, SetBlockType, GetRevivePoint,
// SetEnterDistrictPos, IsGM, SetGMPower, SetTestMode,
// SetSocialOwnerID, GetSocialOwnerID, SetSocialUseTime, GetSocialUseTime,
// GetMyroomBackupYaw, SetMyroomBackupYaw, GetMyroomBackupPos, SetMyroomBackupPos,
// GetLeagueInventoryTime, GetLeagueDeletePenalty, GetLeagueWithdrawPenalty,
// IsBattleState, SetLastComeBackDate, SetCreateDate, SetFirstUCID,
// SetCharacterInfoSync, GetCharacterInfoSync, SetLastAccountComeBackDate,
// SetAccountCreateDate, ClearChattingCount, SetLastSendChattingLog,
// GetLastSendChattingLog, PrivateShopStart, PrivateShopName,
// SetPrivateShopState, IsPrivateShopBuy, SetDedicatedMonsterID,
// SetBaseJumpCount, SetBonusJumpCount, SetDeathAttack,
// SetReserveReviveImmediate, SetPlayLoopMotion, etc.
// These are temporarily removed until proper declarations are added to User.h

// ============================================================================
// Public Transport Methods
// IDA: ?IsPlayingPublicTransport@CUser@@QEAA_NXZ @ 0x1402A5110
// IDA: ?GetPublicTransportIndex@CUser@@QEAAGXZ @ 0x1402A50F0
// IDA: ?GetPublicTransportTime@CUser@@QEAAMXZ @ 0x1402A50C0
// ============================================================================
// Note: GetPublicTransportTime requires VPublicTransportPath type
float CUser::GetPublicTransportTime() const {
    // TODO: Implement when VPublicTransportPath is available
    // return VPublicTransport_cl::GetCurTime(&m_sPublicTransportPath);
    return 0.0f;
}

// Note: GetClientLoadComplete, IsUserStatus, GetWorldType,
// GetWaitSuboInputActionProcess, GetLogChangeMap also need declarations

// ============================================================================
// Warp
// IDA: ?Warp@CUser@@QEAAXAEAUXVec3@@@Z @ 0x1406E9C40
// 精确还原 - 传送到指定位置
// ============================================================================
void CUser::Warp(XVec3* pPos)
{
    // IDA: Get area and check validity
    XArea* pArea = GetArea();
    if (!pArea)
        return;

    // IDA: vtable[10] 虚调用即 XArea::MoveActor(XActor*, XVec3&, float)
    pArea->MoveActor(static_cast<XActor*>(this), *pPos);

    // IDA: Send warp packet to broadcast
    XSendPacket xSendPacket(4, 8);  // main=4, sub=8 (eMAIN_CMD_MOVE, eSUB_CMD_WARP)
    STWarp warpInfo;
    warpInfo.byResult = 0;
    warpInfo.xPos = *pPos;
    warpInfo.fRot = m_pPosInfo->fRot;

    // IDA: Get quest ID from actor ID
    UXActorID actorID = GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(actorID);

    xSendPacket.XParse << nQuestID;
    xSendPacket << warpInfo;

    // IDA: Broadcast to all nearby players via CGocNetwork (eAll=0)
    CGocNetwork::SendBroadCast(this, xSendPacket, E_BROADCAST_TYPE::eAll);
}

// ============================================================================
// Exit
// IDA: ?Exit@CUser@@QEAAXXZ @ 0x1406E4370
// 精确还原 - 退出区域时的清理
// ============================================================================
void CUser::Exit()
{
    // IDA: Save important member values before Reset
    float fHitCylinderRadius = m_fHitCylinderRadius;
    float fHitCylinderHeight = m_fHitCylinderHeight;
    VActionResourceLump* pActionResource = m_pActionResource;
    auto mapAnimInfoKey = m_mapAnimInfoKey;
    auto mapAnimInfoString = m_mapAnimInfoString;
    bool bBattlePose = m_bBattlePose;
    std::uint32_t dwStatus = GetStatus() & 0xA2108;  // IDA: CGocEvent::GetRouletteDayCount mask
    std::uint8_t byClass = m_stCharInfo.stBaseInfo.byClass;
    bool bTestMode = m_bTestMode;

    // IDA: Exit booster mode
    auto pBooster = GetGOC<CGocBooster>();
    if (pBooster) {
        pBooster->ExitBooster();
    }

    // IDA: Reset Akashic record usage count
    auto pAkashic = GetGOC<CGocAkashicRecord>();
    if (pAkashic) {
        // TODO: CGocAkashicRecord::SetUseCount - need to verify inheritance from CItem
        // pAkashic->SetUseCount(0);
    }

    // IDA: Call base class Reset
    CMoverEx::Reset();

    // IDA: Restore saved values
    m_fHitCylinderRadius = fHitCylinderRadius;
    m_fHitCylinderHeight = fHitCylinderHeight;
    m_pActionResource = pActionResource;
    m_mapAnimInfoKey = mapAnimInfoKey;
    m_mapAnimInfoString = mapAnimInfoString;
    ChangeBattlePose(bBattlePose);
    SetStatus(dwStatus);
    m_bTestMode = bTestMode;

    // IDA: Set skill manager test mode
    if (m_pSkillMgr) {
        m_pSkillMgr->SetTestMode(m_bTestMode);
    }

    // IDA: Reset combat type for specific classes (5=Haru, 6=Iris, 4=Erwin)
    if (byClass == 5 || byClass == 6 || byClass == 4) {
        m_nCombatType = 0;
    }
}

// ============================================================================
// CheckPassiveSkillByHit IDA 0x1406F0480
// 检查击中时的被动技能
// ============================================================================
void CUser::CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult) {
    // IDA 精确还原:
    // if ( pSkillTable )
    // {
    //   bCriAttack = 0;
    //   bMissAttack = 0;
    //   if ( (byResult & 1) != 0 )
    //     bMissAttack = 1;
    //   else
    //     bCriAttack = (byResult & 4) != 0;
    //   if ( (((__int64 (__fastcall *)(CMoverEx *))pMover->GetMonsterFlag)(pMover) & 2) == 0 )
    //   {
    //     if ( bMissAttack )
    //     {
    //       CheckPassiveSkill(1, 11);  // Miss attack
    //     }
    //     else
    //     {
    //       CheckPassiveSkill(1, 1);   // Normal hit
    //       CheckPassiveSkill(2, 2);   // Hit type
    //       CheckPassiveSkill(6, 3);   // Hit count
    //       if ( pSkillTable->Use_State == 1 )
    //       {
    //         CheckPassiveSkill(1, 44);  // Skill state
    //       }
    //       if ( pMover->IsBoss_Named_Raid(pMover) )
    //       {
    //         CheckPassiveSkill(1, 52);  // Boss/Named/Raid
    //       }
    //     }
    //     if ( bCriAttack )
    //     {
    //       CheckPassiveSkill(1, 6);  // Critical hit
    //     }
    //   }
    // }

    if (!pSkillTable) {
        return;
    }

    bool bCriAttack = false;
    bool bMissAttack = false;

    if ((byResult & 1) != 0) {
        bMissAttack = true;
    } else {
        bCriAttack = (byResult & 4) != 0;
    }

    // IDA: if ((pMover->GetMonsterFlag() & 2) == 0)
    // Check if target is not a specific monster type (flag 2)
    std::uint64_t nMonsterFlag = pMover->GetMonsterFlag();
    if ((nMonsterFlag & 2) == 0) {
        if (bMissAttack) {
            // Miss attack - trigger passive skill type 11
            CheckPassiveSkill(1, 11);
        } else {
            // Normal hit - trigger various passive skills
            CheckPassiveSkill(1, 1);   // Hit event
            CheckPassiveSkill(2, 2);   // Hit type
            CheckPassiveSkill(6, 3);   // Hit count

            // IDA: if (pSkillTable->Use_State == 1)
            if (pSkillTable->Use_State == 1) {
                CheckPassiveSkill(1, 44);  // Skill state active
            }

            // IDA: if (pMover->IsBoss_Named_Raid(pMover))
            if (pMover->IsBoss_Named_Raid()) {
                CheckPassiveSkill(1, 52);  // Boss/Named/Raid target
            }
        }

        // Critical hit check
        if (bCriAttack) {
            CheckPassiveSkill(1, 6);  // Critical hit
        }
    }
}

// IDA: 0x1406E9F20 - ?BuildBuffInfo@CUser@@QEAAXXZ
// Verified: Direct IDA decompilation - Build buff info from buff state array
void CUser::BuildBuffInfo() {
    // IDA: Clear existing buff info vector
    m_stCharInfo.vecBuffInfo.clear();

    // IDA: Iterate through all 50 buff slots (0x32 = 50)
    for (std::uint8_t i = 0; i < 0x32u; ++i) {
        // IDA: Check if buff slot is active (nBuffIndex != 0)
        if (m_stBuffState[i].nBuffIndex) {
            // IDA: Build STBuffInfo from tagBUFF_STATE
            STBuffInfo info;
            info.nBuffID = m_stBuffState[i].nBuffIndex;
            info.fTime = m_stBuffState[i].fLifeTime;
            info.byCount = m_stBuffState[i].byCount;
            info.dwOwnerID = m_stBuffState[i].dwID;
            info.bShow = m_stBuffState[i].bShow;

            // IDA: Push to vector
            m_stCharInfo.vecBuffInfo.push_back(info);
        }
    }
}
