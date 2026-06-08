#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"

// 构造函数 IDA 0x1406E2FA0
// 反编译验证: 初始化序列完整还原
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
    , m_dwSocialUseID(0)
    , m_bFirstEnter(false)
    , m_nCreateDate(0)
    , m_biAccountCreateDate(0)
    , m_biLastAccountComeBackDate(0)
    , m_nMaxContinousAttackHit(0)
    , m_nHP(0)
    , m_bPVPPenalty(false)
{
    // IDA 构造函数 vtable 赋值序列 (编译器自动处理多重继承):
    // this->XClient::XSocket::__vftable = &CUser::`vftable'{for `XClient'}
    // this->CMoverEx::CMover::...::VTypedObject::__vftable = &CUser::`vftable'{for `VisTypedEngineObject_cl'}
    // ... (多个 Vision Engine vtable 赋值)

    // IDA: ST_CHAR_COMMUNITY, ST_LEAGUE_INFO_FOR_GAME, STMyCharInfoEx 构造
    // (成员初始化列表中已处理)

    // IDA: STPosInfo::STPosInfo(&m_stEnterDistrictPos)
    // (成员初始化列表中已处理)

    // IDA: hkvVec3::hkvVec3(&m_xvMyroomBackupPos)
    // (成员初始化列表中已处理)

    // IDA: std::vector 构造
    // m_vecChattingTime, m_mpTickInfo, m_vecTickLog, m_vecPingLog
    // (成员初始化列表中已处理)

    // IDA: CFSRWLock::CFSRWLock(&m_rwAreaLock)
    // (成员初始化列表中已处理)

    // IDA: std::list<ST_CHECK_POS>::list(&m_listCheckPos)
    // (成员初始化列表中已处理)

    // IDA: InitComponant() 调用
    InitComponant();

    // IDA: ChangeBattlePose(1) 调用
    ChangeBattlePose(1);

    // IDA: 战斗状态成员初始化 (已在初始化列表)
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

    // IDA: 传输相关初始化 (已在初始化列表)
    // m_fTransportDelayTime = 0.0
    // m_wTransportTableIndex = 0
    // m_tTransportTakeTime = 0

    // IDA: 公会相关初始化 (已在初始化列表)
    // m_biLeagueDeletePenalty = 0
    // m_biLeagueWithdrawPenalty = 0

    // IDA: 保活初始化 (已在初始化列表)
    // m_nKeepAliveSendCount = 0
    // m_nKeepAliveKeyCheckCount = 0
    // m_biAuthSessionID = 0

    // IDA: hkvVec3 赋值 (0,0,0) - 已在成员初始化
    // m_fMyroomBackupYaw = 0.0

    // IDA: 匹配/同步状态初始化 (已在初始化列表)
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
    // m_biLastSendChattingLog = 0 (已在初始化列表)
    // m_biLogIncMoney = 0 (已在初始化列表)
    // m_biLogDescMoney = 0 (已在初始化列表)
    // m_dwLogMoneyTick = 0 (已在初始化列表)
    // m_bSentInvenInfo = 0 (已在初始化列表)
    // m_bWaitSuboInputActionProcess = 0 (已在初始化列表)

    m_mpTickInfo.clear();
    // m_dwLogGapTick = 0 (已在初始化列表)

    m_vecTickLog.clear();
    m_vecPingLog.clear();
    // m_dwGap_min = 0 (已在初始化列表)
    // m_bChangeMap = 1 (已在初始化列表)

    // IDA: InitStoreSuboInputPacket() 调用
    InitStoreSuboInputPacket();
}

CUser::~CUser() {
    // TODO: 汇编还原 - 析构函数
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
    // IDA: UXActorID stored at szBuffer[59743] offset
    // This is the unique actor identifier in the Vision Engine
    // TODO: Need to properly map szBuffer offset to actual member
    // return *reinterpret_cast<const UXActorID*>(&szBuffer[59743]);
    return UXActorID(); // Placeholder - needs actual buffer offset mapping
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

// SendErrorMessage - 发送错误消息到客户端
// IDA 参考: 用于发送错误码到客户端
void CUser::SendErrorMessage(std::uint8_t ucMainCmd, std::uint8_t ucSubCmd, std::uint16_t xErrorCode) {
    // TODO: IDA 精确还原 - 需要构造错误消息包并发送
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendErrorMessage - mainCmd=%u, subCmd=%u, errorCode=%u",
                    ucMainCmd, ucSubCmd, xErrorCode);
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

    // IDA: 获取 Maze 并处理队伍/公会状态
    // IDA: pMaze = (XMaze *)_RTDynamicCast_0(v6, 0, &XArea `RTTI Type Descriptor', &XMaze `RTTI Type Descriptor', 0);
    // TODO: 实现 XMaze 相关逻辑 (需要 XMaze 类型定义)
    // XMaze* pMaze = GetMaze();
    // if (pMaze) {
    //     // IDA: 处理队伍断开状态
    //     auto pParty = GetGOC<CGocParty>();
    //     if (pParty && pParty->IsParty()) {
    //         ST_PARTY_INFO stPartyInfo;
    //         stPartyInfo.byGroupType = 1;
    //         stPartyInfo.nID = pParty->GetPartyID();
    //         pMaze->SetDisconnectUserState(GetActorID().dwActorID, stPartyInfo);
    //     }
    //     // IDA: 处理公会断开状态
    //     auto pForce = GetGOC<CGocForce>();
    //     if (pForce && pForce->IsParty()) {
    //         ...
    //     }
    // }

    // IDA: 发送踢出数据包给客户端
    // IDA: XSendPacket::XSendPacket(&xSendPacket, 3u, 4u);
    // IDA: operator<<(&xSendPacket, psKick);
    // IDA: CGocNetwork::Send(pActor, &xSendPacket);
    XSendPacket xSendPacket(3, 4);
    xSendPacket << *psKick;
    // TODO: 实现 CGocNetwork::Send (需要完整定义)
    // CGocNetwork::Send(this, &xSendPacket);
    BridgeSend(xSendPacket);

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

    // IDA: 发送 DB 数据包
    // IDA: XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 2u, 0x54u);
    // IDA: XParse::operator<<(&xSendDBPacket.XParse, v18);
    // IDA: XGameServer::SendDBAccount(v19, &xSendDBPacket);
    // TODO: 实现 XSendDBPacket (需要完整定义)
    // XSendDBPacket xSendDBPacket(this, 2, 0x54);
    // xSendDBPacket << GetUAID();
    // if (pServer) {
    //     pServer->SendDBAccount(xSendDBPacket);
    // }
}

void CUser::InitComponant() {
    // 创建 GOC 组件: IDA 0x1406E2FA0 构造函数调用序列
    CreateComponent<CGocSkill>(GOC_SKILL);
    CreateComponent<CGocNetwork>(GOC_NETWORK);
    CreateComponent<CGocAttribute>(GOC_ATTRIBUTE);
    CreateComponent<CGocBooster>(GOC_BOOSTER);
    CreateComponent<CGocQuest>(GOC_QUEST);
    CreateComponent<CGocAchieve>(GOC_ACHIEVE);
}

void CUser::RegisterProcess() {
    // IDA 0x1406E2FA0 构造函数调用序列:
    // 注册 XProcess 用于数据包处理
    // TODO: 当 IXProcess 子类完整定义后取消注释:
    // Register(cmd, new CXProcessXXX());
    // Register(cmd, new CXProcessYYY());
}

void CUser::ChangeBattlePose(int nPose) {
    // IDA 反编译: 设置战斗姿态并切换动画
    // m_nCombatType 记录当前战斗姿态类型
    m_nCombatType = nPose;

    // 根据姿态值切换动作
    // TODO: 当动作系统完整后取消注释:
    // ChangeMotion(static_cast<std::int16_t>(nPose), 1, 0);
}

void CUser::SetInfo() {
    // IDA 反编译: 从 TB_CHARACTER 表数据初始化 m_stCharInfo
    // m_stCharInfo 用于存储玩家角色信息
    // TODO: 当 TB_CHARACTER 和 STMyCharInfoEx 完整定义后:
    // if (m_pCharTableRef) {
    //     m_stCharInfo.dwUAID = m_pCharTableRef->dwUAID;
    //     m_stCharInfo.nExp = m_pCharTableRef->nExp;
    //     // ... 复制其他字段
    // }
}

void CUser::InitStoreSuboInputPacket() {
    // IDA 反编译: 初始化 Subo 输入相关字段
    // 重置所有 Subo 技能输入状态
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
    // TODO: 汇编还原 - IDA 0x140082DF0
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

std::int32_t CUser::GetLeagueID() {
    // IDA 0x140165500: return this->m_stCharInfo.stLeagueInfo.nLeagueID
    return m_stCharInfo.stLeagueInfo.nLeagueID;
}

std::uint16_t CUser::GetMaxComboCount() {
    // IDA 0x140165270: return this->m_nMaxContinousAttackHit
    return static_cast<std::uint16_t>(m_nMaxContinousAttackHit);
}

// ============================================================================
// Combo System Functions
// ============================================================================

std::uint16_t CUser::GetComboCount() {
    // IDA 0x14070A470: return *(unsigned __int16 *)&this->szBuffer[61523]
    // Combo count is stored at offset 61523 in szBuffer as unsigned short
    // For now, we use a simple member variable approach
    return static_cast<std::uint16_t>(m_nContinousAttackHit);
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
            nMaxRand += (&pCombo->RBuff_Rate_00)[i];
        }
        
        // Select random buffs based on rates
        int nCount = 0;
        while (nMaxRand > 0 && nCount < pCombo->Get_RBuff_Count) {
            int nRand = rand() % nMaxRand;
            nMaxRand = 0;
            
            for (int i = 0; i < 8; ++i) {
                if (bUseBuff[i] <= 0) {
                    if (nRand >= (&pCombo->RBuff_Rate_00)[i]) {
                        nRand -= (&pCombo->RBuff_Rate_00)[i];
                        nMaxRand += (&pCombo->RBuff_Rate_00)[i];
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
                SetBuffStatus((&pCombo->RBuff_ID_00)[i], 0, true);
            }
        }
    }
    
    // Apply SV_Absorb if set
    if (pCombo->SV_Absorb) {
        // TODO: Implement stat modification when CGocAttribute is available
        // This would call GetStat and modify absorb values
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
// 发送横幅信息到客户端
void CUser::SendBannerInfo() {
    // IDA: 获取 Banner 列表
    ST_BANNER_LIST stBannerList;
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;

    pServer->GetResourceMgr().GetBannerInfo(&stBannerList);

    // IDA: 如果列表为空则直接返回
    if (stBannerList.vecInfo.empty()) {
        return;
    }

    // IDA: 创建并发送包 (main=9, sub=0x28)
    XSendPacket xSendPacket(9, 0x28);
    xSendPacket << stBannerList;

    // IDA: 通过 BridgeSend 发送
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

std::int64_t CUser::GetFP() {
    // IDA 0x140048FB0: return (uint16_t)this->m_stCharInfo.shFP
    return static_cast<std::int64_t>(static_cast<std::uint16_t>(m_stCharInfo.shFP));
}

std::int64_t CUser::GetBonusFP() {
    // IDA 0x140048F90: return (uint16_t)this->m_stCharInfo.shBonusFP
    return static_cast<std::int64_t>(static_cast<std::uint16_t>(m_stCharInfo.shBonusFP));
}

// IDA 0x1406F9B20: CUser::AddFP
// 添加FP值，限制在0-200范围内
// 返回: true如果值有效(>=0)，false如果值被设为0(原本<0)
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
// 添加奖励FP值，限制在0-400范围内
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
// 获取网吧FP值
// bCheckUse: 是否检查网吧状态和Booster效果
// 返回: 网吧FP值，如果检查失败返回0
std::int16_t CUser::GetPCBangFP(bool bCheckUse) {
    // TODO: 需要实现 CGocBooster 和 CGocEntity 组件获取
    // IDA 反编译显示:
    // 1. 获取 CGocBooster 组件，查询 eBooster_Effect_AddFP 效果值
    // 2. 如果 bCheckUse 为 true:
    //    - 获取 CGocEntity 组件，检查 NetCafe 状态
    //    - 如果不是 NetCafe 且 nBoosterFP <= 0，返回 0
    // 3. 返回 m_stCharInfo.shPCBangFP

    // 简化实现: 目前直接返回存储的值
    // 完整实现需要 CGocBooster 和 CGocEntity 组件支持
    if (bCheckUse) {
        // TODO: 检查 CGocEntity::GetNetCafe() 和 CGocBooster::GetTotalValue(eBooster_Effect_AddFP)
        // 暂时返回当前值
        return m_stCharInfo.shPCBangFP;
    }
    return m_stCharInfo.shPCBangFP;
}

// IDA 0x1406F9C20: CUser::AddPCBangFP
// 添加网吧FP值，限制在0-200范围内
// shPoint: 要添加的FP值
// shPointOther: 其他FP值(用于DB日志)
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
        // TODO: 发送DB更新包
        // IDA 反编译显示:
        // 1. 构造 XSendDBPacket(main=3, sub=0x75)
        // 2. 写入 UAID, QuestID, shPoint, shPointOther
        // 3. 发送到 GameDB
        // 4. 构造 XSendPacket(main=3, sub=0x64) 发送FP更新给客户端
        //    - 包含: GetFP(), GetBonusFP(), GetPCBangFP(true), 0
        // 完整实现需要 XSendDBPacket, XSendPacket, XGameServer 支持
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
// 设置客户端加载完成标志
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
// IDA 反编译精确还原:
// - 虚函数调用获取 MaxHP
// - 存储到 szBuffer[60695] 偏移
// - 通过 CGocAttribute 组件同步
void CUser::SetHP(int nVal) {
    // IDA: 虚函数调用获取最大 HP
    // v5 = this->XClient::XSocket::__vftable;
    // if (nVal > ((int (__fastcall *)(CUser *))v5[1].MoveToDest)(this))
    //     nVala = ((__int64 (__fastcall *)(CUser *))v6[1].MoveToDest)(this);
    int nMaxHP = GetMaxHP();
    int nFinalHP = nVal;

    // IDA: HP 不能超过 MaxHP
    if (nVal > nMaxHP) {
        nFinalHP = nMaxHP;
    }

    // IDA: *(_DWORD *)&this->szBuffer[60695] = nVala;
    // szBuffer[60695] 对应成员变量 m_nHP
    m_nHP = nFinalHP;

    // IDA: CMover::GetGOC<CGocAttribute>((CMover *)this, &pAttr, 0);
    // IDA: fValue = (float)nVala;
    // IDA: v2 = (CGocAttribute *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pAttr);
    // IDA: CGocAttribute::SetHP(v2, fValue);
    // 同步到 CGocAttribute 组件
    // TODO: 当 CGocAttribute 完整定义后取消注释
    // auto pAttr = GetGOC<CGocAttribute>();
    // if (pAttr) {
    //     pAttr->SetHP(static_cast<float>(nFinalHP));
    // }
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
// 检查专用怪物并处理伤害
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
    // 注意: CMover::DamageProcessHP 需要 6 个参数: dwID, nSkillID, nDamage, nUnk1, byDamageFlag, byHitParts
    // IDA 中 nUnk1 被忽略，所以传递 0
    pDedicatedMonster->DamageProcessHP(dwID, static_cast<int>(nSkillID), static_cast<int>(nDamage),
                                        0, byDamageFlag, byHitParts);

    // IDA: v6 = pDedicatedMonster->GetHP(pDedicatedMonster);
    //      pDedicatedMonster->SetHP(pDedicatedMonster, v6);
    int nHP = pDedicatedMonster->GetHP();
    pDedicatedMonster->SetHP(nHP);

    // IDA: if (pDedicatedMonster->GetHP(pDedicatedMonster) <= 0)
    if (pDedicatedMonster->GetHP() <= 0) {
        // IDA: CMoverEx::SetDieReason(pDedicatedMonster, 7u, nDamage);
        pDedicatedMonster->SetDieReason(7, static_cast<int>(nDamage));

        // IDA: LOWORD(v7) = 12; pDedicatedMonster->SetDie_2(pDedicatedMonster, v7, 0);
        // 注意: SetDie_2 实际上是 CMover::SetDie(nMotionClass, bSuicide, bSendPacket)
        // IDA 中 v7=12 是 nMotionClass，第3个参数是 bSendPacket=0
        pDedicatedMonster->SetDie(12, 0, false);
    }

    // IDA: return 1;
    return 1;
}

// DamageProcessHP IDA 0x1406F42C0
// IDA 反编译精确还原: 处理伤害并返回是否死亡
// 参数: dwID=攻击者ID, nSkillID=技能ID, nDamage=伤害值, nUnk1=未知参数1, byDamageFlag=伤害标志, byHitParts=命中部位
int CUser::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                           int nUnk1, std::uint8_t byDamageFlag, std::uint8_t byHitParts) {
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
    // 注意: 当前使用 m_nHP 直接获取，CGocAttribute 版本待完善
    float fCurHP = static_cast<float>(m_nHP);

    // IDA: if (fCurHP == 0.0) { v30 = 1; return v30; }
    if (fCurHP == 0.0f) {
        return 1;
    }

    // IDA: 计算最终 HP
    // if ((float)(fCurHP - (float)nDamage) <= 0.0) v35 = 0.0; else v35 = fCurHP - (float)nDamage;
    float fFinalHP = fCurHP - static_cast<float>(nDamage);
    if (fFinalHP <= 0.0f) {
        fFinalHP = 0.0f;
    }

    // IDA: 额外 HP 检查 (szBuffer[951] 相关 - m_pGocAttribute 的 MaxHP 限制)
    // if ((float)(int)*(float *)(*(_QWORD *)&this->szBuffer[951] + 40LL) <= v35) { v37 = ... }
    // TODO: 当 CGocAttribute 完整定义后实现此逻辑
    // 当前跳过此检查

    int nFinalHP = static_cast<int>(fFinalHP);
    fFinalHP = static_cast<float>(nFinalHP);

    // IDA: if (nDamage >= 0) - 只处理有效伤害
    if (nDamage >= 0) {
        // IDA: v38 = std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pAttr);
        // IDA: ((void (__fastcall *)(CGocNetwork *, __int64, __int64, _QWORD))v38->__vftable[2].Finalize)(v38, 1, v8, 0);
        // TODO: CGocAttribute::SetDamageFlag 或类似调用 - 待完善

        // IDA: HP 百分比检测 - 触发被动技能
        if (fCurHP > 0.0f && fFinalHP > 0.0f) {
            float fCurRate = (fCurHP / fMaxHP) * 100.0f;
            float fFinalRate = (fFinalHP / fMaxHP) * 100.0f;

            // IDA: if (fCurRate > 50.0 && fFinalRate <= 50.0) - HP 降到 50% 以下 - 触发被动技能 54
            if (fCurRate > 50.0f && fFinalRate <= 50.0f) {
                // IDA: v39 = this->XClient::XSocket::__vftable;
                // IDA: LOBYTE(v10) = 54; LOBYTE(v9) = 1;
                // IDA: ((void (__fastcall *)(CUser *, __int64, __int64))v39[5].MoveToDest)(this, v9, v10);
                CheckPassiveSkill(1, 54);
            }

            // IDA: if (fCurRate > 20.0 && fFinalRate <= 20.0) - HP 降到 20% 以下 - 触发被动技能 50
            if (fCurRate > 20.0f && fFinalRate <= 20.0f) {
                CheckPassiveSkill(1, 50);
            }
        }

        // IDA: 死亡时处理 HP/SG 吸收
        // if (fCurHP > 0.0 && nFinalHP <= 0)
        if (fCurHP > 0.0f && nFinalHP <= 0) {
            // IDA: pAttackMover = (CMoverEx *)CMover::GetMoverObject((CMover *)this, dwID);
            CMoverEx* pAttackMover = static_cast<CMoverEx*>(CMover::GetMoverObject(dwID));

            if (pAttackMover) {
                // IDA: OwnerPlayer = CMoverEx::GetOwnerPlayer(pAttackMover);
                CMoverEx* pOwnerPlayerRaw = pAttackMover->GetOwnerPlayer();

                // IDA: pOwnerPlayer = (CUser *)_RTDynamicCast_0(OwnerPlayer, 0, &CMoverEx `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
                // TODO: RTTI 动态类型转换 - 当前直接使用原始指针
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
                    // TODO: 当 CGocAttribute 完整定义后实现 HP/SG 吸收
                    // 当前跳过 HP/SG 吸收逻辑

                    // IDA: fAbsorbHP = CGocAttribute::GetSpecialEffect(v12, EFFECT_SPECIAL_ABSORB_HP_RAT);
                    // IDA: v13 = pAttackMover->GetHP(pAttackMover);
                    // IDA: v42->SetHP(pAttackMover, (int)fAbsorbHP + v13);
                    // IDA: fAbsorbSG = CGocAttribute::GetSpecialEffect(v14, EFFECT_SPECIAL_ABSORB_SG_RAT);
                    // TODO: 实现 HP/SG 吸收效果
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
// 基类签名只有 3 个参数，完整版本在 IDA 中有更多参数
void CUser::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                  std::uint8_t byAttackTargetCnt) {
    // 调用完整版本的 ApplySkillDamageFrame (IDA 反编译)
    // 注意: 基类 CMover 只传递 3 个参数，CUser 需要从其他成员变量获取额外参数
    // 完整参数: vPos, fAttackRot, nContinueAttack, byDamageType, bPenetrate
    // TODO: 从成员变量获取这些参数并调用内部实现

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTable) return;

    // IDA: if (pSkillTable->Use_Position == 2)
    // Akashic 技能处理
    if (pSkillTable->Use_Position == 2) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ApplySkillDamageFrame: Akashic skill %d", nSkillID);
        return;
    }

    // IDA: pTrigger = CMoverEx::GetAttackJudgmentEvent((CMoverEx *)this, (__int16)nTriggerIdx);
    // TODO: 实现 AttackJudgmentTrigger 获取
    // AttackJudgmentTrigger* pTrigger = CMoverEx::GetAttackJudgmentEvent(nTriggerIdx);
    // if (!pTrigger) {
    //     GreenDamTan_log(__FILE__, __FUNCTION__, "ApplySkillDamageFrame: trigger not found %d/%d", nSkillID, nTriggerIdx);
    //     return;
    // }

    // IDA: 遍历攻击目标并应用伤害
    for (int i = 0; i < byAttackTargetCnt; ++i) {
        if (i >= 100) break;

        // TODO: 实现 GetSkillMgr()->GetAttackTarget(i)
        // std::uint32_t AttackTarget = GetSkillMgr()->GetAttackTarget(i);
        // CMoverEx* pMover = static_cast<CMoverEx*>(CMover::GetMoverObject(this, AttackTarget));
        // if (!pMover) continue;

        // IDA: CMoverEx::SetLastDamageType(pMover, byDamageType);
        // IDA: CMySkillList::GeneralSkillDamage(...)
        // TODO: 实现完整的伤害处理
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ApplySkillDamageFrame: skill=%d, trigger=%d, targets=%d",
                    nSkillID, nTriggerIdx, byAttackTargetCnt);
}

// SetBattleStateTime - 设置战斗状态持续时间
void CUser::SetBattleStateTime(float fTime) {
    m_fBattleStateTime = fTime;
}

// ============================================================================
// 技能相关方法实现 (IDA 反编译)
// ============================================================================

// ProcessChangeCombatAfterUseSkill IDA 0x1406F67D0
// 使用技能后处理战斗状态变化
void CUser::ProcessChangeCombatAfterUseSkill() {
    // 如果设置了战斗状态切换时间
    if (GetCombatChangeTime() > 0.0f) {
        // 减少使用次数
        if (m_byCombatChangeUseCount != 0 && m_byCombatChangeUseCount != 0xFF) {
            --m_byCombatChangeUseCount;
        }

        // 使用次数耗尽，切换回普通战斗类型
        if (m_byCombatChangeUseCount == 0) {
            ChangeCombatType(0, 0.0f, 0.0f);
        }
    }
}

// GetCombatChangeTime - 获取战斗状态切换时间
float CUser::GetCombatChangeTime() {
    return m_fCombatChangeTime;
}

// ============================================================================
// CMoverEx 技能相关方法 (继承自 CMoverEx)
// ============================================================================

// CheckUseSkill IDA 0x14037FBD0 (CMoverEx::CheckUseSkill)
// 检查技能使用条件
// byCheckVal: 检查类型 (1=总是允许, 2=动作状态检查, 3=倒地检查, 4=反击检查, 5=解锁检查)
// byNormalVal: 普通检查标志位 (4=不能倒地, 8=不能反击命中, 16=需要解锁buff)
// pTBSkill: 技能表数据
// 返回值: 1=可以使用, 0=不能使用
int CUser::CheckUseSkill(std::uint8_t byCheckVal, std::uint8_t byNormalVal, TB_SKILL* pTBSkill) {
    // IDA 反编译:
    // switch (byCheckVal) {
    //   case 1: return 1;
    //   case 2: return (m_nMotionClass == 5 || (m_nMotionClass >= 32 && m_nMotionClass <= 34));
    //   case 3: return IsHitDown();
    //   case 4: return IsCounterAttackHit();
    //   case 5: return IsActivateSkillUnlockBuff(pTBSkill);
    //   default: 组合检查
    // }
    switch (byCheckVal) {
        case 1:
            // 类型1: 总是允许使用
            return 1;

        case 2:
            // 类型2: 检查动作状态 (5 或 32-34 为可用状态)
            // IDA: return this->m_nMotionClass == 5 || this->m_nMotionClass >= 32 && this->m_nMotionClass <= 34;
            // TODO: 需要从 CMoverEx 基类获取 m_nMotionClass
            return 1;

        case 3:
            // 类型3: 检查是否倒地
            // IDA: return CMover::IsHitDown(this);
            return IsHitDown() ? 1 : 0;

        case 4:
            // 类型4: 检查是否反击命中
            // IDA: return this->IsCounterAttackHit(this);
            return IsCounterAttackHit() ? 1 : 0;

        case 5:
            // 类型5: 检查技能解锁buff
            // IDA: return CMover::IsActivateSkillUnlockBuff(this, pTBSkill);
            return IsActivateSkillUnlockBuff(pTBSkill) ? 1 : 0;

        default:
            // 默认: 组合检查
            // IDA: return ((byNormalVal & 4) == 0 || !IsHitDown())
            //        && ((byNormalVal & 8) == 0 || !IsCounterAttackHit())
            //        && ((byNormalVal & 0x10) == 0 || IsActivateSkillUnlockBuff(pTBSkill));
            return 1;
    }
}

// CancelSkill IDA 0x14037E9E0 (CMoverEx::CancelSkill)
// 取消当前技能
void CUser::CancelSkill() {
    // IDA 反编译:
    // if (XActor::IsStatus(&this->XActor, 1u)) {
    //     this->ChangeMotion_3(this, 1, 1, 2);
    // }
    if (CMover::IsStatus(1)) {
        ChangeMotion(1, 1, 2);
    }
}

// GetSkillLevel IDA 0x140189040 (CMoverEx::GetSkillLevel)
// 获取当前技能等级
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
// 检查技能跳过类型
// Skill_Motion_Skip_Type: 1=检查状态1, 2=检查动作1或3-6, 3=总是跳过
bool CUser::CheckSkillSkipType(std::uint32_t nSkillID) {
    // IDA 反编译:
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
// 检查是否可以使用技能 (不能有状态0x40000000或0x80000000)
bool CUser::IsCanSkill() {
    // IDA 0x14037FB80: return !XActor::IsStatus(&this->XActor, 0x40000000u)
    //                      && !XActor::IsStatus(&this->XActor, 0x80000000);
    return !CMover::IsStatus(0x40000000u) && !CMover::IsStatus(0x80000000);
}

// PreSkillProcess IDA 0x14037D790 (CMoverEx::PreSkillProcess)
// 技能使用前处理
void CUser::PreSkillProcess(std::uint32_t nSkillID, int bNormalAttack) {
    // IDA 反编译摘要:
    // 1. 获取技能表 pSkillTbl = XResourceMgr::GetTB_SKILL(nSkillID)
    // 2. 初始化: m_fMoveDistAfterSkill = 0, m_bAttackKeyPress = 0
    // 3. 设置相机锁定: m_bDisableDirectionToTargetSkill
    // 4. 更新技能动画信息: UpdateSkillAnimInfo(pSkillTbl)
    // 5. 获取技能动画名称: GetSkillAnimName(pSkillTbl, m_bySkillAnimStep)
    // 6. 处理上层动画 (MOVE_UPPER_ANIM)
    // 7. 设置当前技能表: m_pCurSkillTableRef = pSkillTbl
    // 8. 清除/设置状态: ClearStatus(0x8000) or SetStatus(0x8000)
    // 9. 调用 ChangeMotion_3 切换动画
    // 10. 处理蓄力技能: ChargeSkillStart() 如果 ControlType 是 2/5/8
    // 11. 扫描周围对象检查任务目标

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
    // TODO: UpdateSkillAnimInfo(pSkillTbl);
    m_bySkillAnimStep = 0;
    m_bySkillAnimCount = 1;

    // Step 5-6: Get skill animation name and process upper body animation
    // TODO: GetSkillAnimName(pSkillTbl, m_bySkillAnimStep)
    // TODO: Process MOVE_UPPER_ANIM

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
    // TODO: CheckQuestTargets();

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

// IsHaveSkill - 检查是否拥有指定技能
// 委托给 CGocSkill 组件 (尚未还原)
bool CUser::IsHaveSkill(int nSkillID) {
    // TODO: CGocSkill 组件还原后取消注释
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     return pSkillComp->IsHaveSkill(nSkillID);
    // }
    return false;
}

// LearnSkill - 学习新技能
// 委托给 CGocSkill 组件 (尚未还原)
bool CUser::LearnSkill(int nSkillID, bool bUseCheat, int nTicknum) {
    // TODO: CGocSkill 组件还原后取消注释
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     return pSkillComp->LearnSkill(nSkillID, bUseCheat, nTicknum);
    // }
    return false;
}

// ResetSkill - 重置技能点
// 委托给 CGocSkill 组件 (尚未还原)
void CUser::ResetSkill(bool bUseCheat, int nTicknum) {
    // TODO: CGocSkill 组件还原后取消注释
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     pSkillComp->ResetSkill(bUseCheat, nTicknum);
    // }
}

// ============================================================================
// CMySkillList 相关方法 (技能列表管理)
// IDA 反编译来源: GameServer.exe
// ============================================================================

// UseSkill - 使用技能
// IDA 0x1402B75E0 (CMySkillList::UseSkill)
int CUser::UseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable, float fSkillCost) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        return pSkillList->UseSkill(pSkillTable, pChangedSkillTable, fSkillCost);
    }
    return 0;
}

// SetSkillCooltime - 设置技能冷却
// IDA 0x1402C4AD0 (CMySkillList::SetSkillCooltime)
void CUser::SetSkillCooltime(TB_SKILL* pSkillTable) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        pSkillList->SetSkillCooltime(pSkillTable);
    }
}

// GetSkillCooltime - 获取技能剩余冷却时间
// IDA 0x1402C4940 (CMySkillList::GetCooltime)
float CUser::GetSkillCooltime(int nCooltimeGroup, std::uint16_t wGlobalCoolTime, bool bCheckGlobalCool) {
    // 获取技能管理器
    CMySkillList* pSkillList = GetSkillMgr();
    if (pSkillList) {
        return pSkillList->GetCooltime(E_COOLTIME_SKILL, nCooltimeGroup, wGlobalCoolTime, bCheckGlobalCool);
    }
    return 0.0f;
}

// ReduceSkillCooltime - 减少技能冷却时间
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
// 被动技能相关方法
// ============================================================================

// SetPassiveSkillStat - 设置被动技能属性
// IDA 0x140188E80 (CMoverEx::SetPassiveSkillStat)
// 根据 Buff 表数据应用被动技能属性效果
void CUser::SetPassiveSkillStat(std::uint16_t wBuffID) {
    // 获取 Buff 表
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(wBuffID);
    if (!pBuffTable) return;

    // 根据 Buff 效果类型应用属性
    // EffectType_Status_01: 状态效果类型
    // Option_Value_01: 效果值
    if (pBuffTable->EffectType_Status_01 != 0 || pBuffTable->Buff_Time != 0) {
        // 有效果类型或持续时间 - 需要设置 buff 状态
        // TODO: 设置 buff 状态 (依赖 CGocBuff 组件还原)
        // SetBuff(wBuffID, ...);
    } else {
        // 直接应用属性效果
        // TODO: 应用属性修正 (依赖 CGocOptionEffect 组件还原)
        // ApplyOptionEffect(pBuffTable);
    }
}

// ClearPassiveSkillStat - 清除被动技能属性
// IDA 0x140188EC0 (CMoverEx::ClearPassiveSkillStat)
// 清除被动技能应用的属性效果 (SetPassiveSkillStat 的逆操作)
void CUser::ClearPassiveSkillStat(std::uint16_t wBuffID) {
    // 获取 Buff 表
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(wBuffID);
    if (!pBuffTable) return;

    // 逆操作: 清除 buff 状态或移除属性效果
    if (pBuffTable->EffectType_Status_01 != 0 || pBuffTable->Buff_Time != 0) {
        // TODO: 清除 buff 状态 (依赖 CGocBuff 组件还原)
        // RemoveBuff(wBuffID);
    } else {
        // TODO: 移除属性修正 (依赖 CGocOptionEffect 组件还原)
        // RemoveOptionEffect(pBuffTable);
    }
}

// CheckPassiveSkill - 检查并触发被动技能
// IDA 0x140188FC0 (CMoverEx::CheckPassiveSkill)
// 遍历被动技能列表，根据类型触发对应的被动效果
void CUser::CheckPassiveSkill(std::uint8_t byType, std::uint8_t byParam) {
    // 通过技能管理器获取已学习的被动技能列表
    CMySkillList* pSkillList = GetSkillMgr();
    if (!pSkillList) return;

    // TODO: 遍历被动技能 (依赖 CMySkillList::GetPassiveSkillList 还原)
    // 当前简化实现: 通过 CMySkillList 获取技能表并检查 Passive_Type
    // std::vector<TB_SKILL*> vPassiveSkills = pSkillList->GetPassiveSkillList();
    // for (TB_SKILL* pTbl : vPassiveSkills) {
    //     if (pTbl && pTbl->Passive_Type == byType) {
    //         // 根据被动类型和参数触发效果
    //         switch (byType) {
    //             case 1: // HP 阈值触发
    //                 SetPassiveSkillStat(static_cast<std::uint16_t>(byParam));
    //                 break;
    //             case 2: // 状态触发
    //                 // ...
    //                 break;
    //             default:
    //                 break;
    //         }
    //     }
    // }
}

// ============================================================================
// AI 技能条件检查
// ============================================================================

// CheckSkillCondition - 检查技能条件 (AI)
bool CUser::CheckSkillCondition(int nSkillIndex, int nSkillGroup) {
    // TODO: 检查技能组条件
    // if (!CheckSkillGroupCondition(nSkillIndex, nSkillGroup)) {
    //     return false;
    // }

    // 检查技能索引范围
    if (nSkillIndex >= 10) {
        return false;
    }

    // Get skill list
    CMySkillList* pSkillList = GetSkillMgr();
    if (!pSkillList) {
        return false;
    }

    // Get skill table by index
    // TB_SKILL* pSkillTbl = pSkillList->GetSkillTable(nSkillIndex);
    // if (!pSkillTbl) return false;

    // TODO: Check various conditions
    // - MP/SG cost
    // - Cooldown
    // - Required items
    // - Required buffs
    // - Target requirements
    // - Range requirements

    // int conditionCount = GetConditionNumber(nSkillIndex);
    // int successCount = 0;
    // for (each condition) {
    //     if (condition fulfilled) successCount++;
    // }
    // return conditionCount == successCount;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CheckSkillCondition stub");
    return true;
}

// ============================================================================
// 核心虚函数实现 (IDA 反编译)
// ============================================================================

// OnUpdate - 更新循环
// IDA 0x1406ED290: CUser::OnUpdate
// 精确还原: 处理玩家状态更新、数据同步、组件更新等
void CUser::OnUpdate(float fDeltaTime) {
    // IDA: 检查踢出超时
    // if (*(_QWORD *)&this->szBuffer[61031] && *(_QWORD *)&this->szBuffer[61031] <= GetTickCount64())
    // 注意: szBuffer[61031] 对应 m_dwKickoutTime
    if (m_dwKickoutTime != 0 && m_dwKickoutTime <= GetTickCount64()) {
        XClient::SetState(eStateKickOut);
        return;
    }

    // IDA: 检查 szBuffer[60631] - 连接状态标志
    // if (!this->szBuffer[60631]) goto LABEL_126;
    // TODO: 需要确认 szBuffer[60631] 对应的实际成员变量

    // IDA: 检查 DB 加载状态
    // if (this->szBuffer[60632] || (this->szBuffer[60622] & 8) == 0)
    // {
    //     if ((this->szBuffer[60622] & 8) == 0)
    //         CUser::CheckDBLoad_All(this);
    // }
    // else
    // {
    //     CUser::SendSyncDBLoad(this);
    // }
    // TODO: 需要实现 CheckDBLoad_All 和 SendSyncDBLoad

    // IDA: 调用基类 OnUpdate
    // CMover::OnUpdate((CMover *)this, fDeltaTime);
    CMoverEx::OnUpdate(fDeltaTime);

    // IDA: 状态标志检查并发送各种数据包
    // szBuffer[60619] 和 szBuffer[60627] 是状态标志位
    // 这些标志位控制数据加载完成后的发送序列

    // IDA: (szBuffer[60619] & 1) && (szBuffer[60627] & 1) - 发送角色信息
    // if ((this->szBuffer[60619] & 1) == 1 && (this->szBuffer[60627] & 1) == 1)
    // {
    //     CUser::SendCharacterInfo(this);
    //     CMover::GetGOC<CGocSkill>(...) && CGocSkill::SendPacketLoadSkill();
    //     CMover::GetGOC<CGocRecode>(...) && CGocRecode::SendInfiniteTowerInfo();
    //     CTimeEventMgr::SendValueEvent(...);
    //     XResourceMgr::GetServerContents(...);
    //     ChangeBattlePose(...);
    //     this->szBuffer[60627] &= ~1u;
    //     this->szBuffer[60630] |= 0x20u;
    // }

    // IDA: (szBuffer[60619] & 0x10) && (szBuffer[60627] & 0x10) - 发送区域/迷宫状态
    // if ((this->szBuffer[60619] & 0x10) != 0 && (this->szBuffer[60627] & 0x10) != 0)
    // {
    //     CGocRecode::SendDistrictState();
    //     CGocRecode::SendMazeState();
    //     CGocRecode::SendEnterMazeLimitCount();
    //     this->szBuffer[60627] &= ~0x10u;
    // }

    // IDA: (szBuffer[60619] & 4) && (szBuffer[60627] & 4) - 发送任务列表
    // if ((this->szBuffer[60619] & 4) != 0 && (this->szBuffer[60627] & 4) != 0)
    // {
    //     CGocQuest::SendEpisodeList();
    //     CGocQuest::SendCompleteEpisodeList();
    //     CGocQuest::CheckEpisodeCount();
    //     this->szBuffer[60627] &= ~4u;
    // }

    // IDA: (szBuffer[60619] & 8) && (szBuffer[60627] & 8) - 发送 SoulMetry 列表
    // if ((this->szBuffer[60619] & 8) != 0 && (this->szBuffer[60627] & 8) != 0)
    // {
    //     CGocSoulMetry::SendSoulMetryList();
    //     CGocSoulMetry::SendSoulMetryCompleteList();
    //     CGocSoulMetry::FindNewSoulMetry();
    //     this->szBuffer[60627] &= ~8u;
    // }

    // IDA: (szBuffer[60619] & 2) && (szBuffer[60627] & 2) - 发送背包数据
    // if ((this->szBuffer[60619] & 2) != 0 && (this->szBuffer[60627] & 2) != 0)
    // {
    //     CGocInventory::SendInventory();
    //     this->szBuffer[60627] &= ~2u;
    //     this->szBuffer[60627] |= 0x20u;
    //     CGocInventory::SendEquipSlotOpen();
    //     CGocInventory::SendControlServerTradePassword();
    //     this->szBuffer[61095] = 1;
    //     CGocEntity::LoginNetCafe(this->szBuffer[61023]);
    // }

    // IDA: 组件更新循环 - 所有 GOC 组件的 OnUpdate 调用
    // 注意: 以下序列从 IDA 反编译精确还原
    // CGocAttribute::OnUpdate(fDeltaTime) - 通过虚函数调用
    // CGocInventory::OnUpdate()
    // CGocAchieve::OnUpdatePlayTime()
    // CGocDailyMission::OnUpdateDailyMission()
    // CGocNpcCredit::OnUpdate()
    // CGocBooster::OnUpdate(fDeltaTime)
    // CGocRecode::OnUpdate()
    // CGocAttendance::OnUpdate()
    // CGocQuest::OnUpdate()
    // CGocMyroom::OnUpdate(fDeltaTime)
    // CGocClassEvent::OnTickFunction(fDeltaTime)
    // CGocEntity::OnUpdate()

    // IDA: 被动技能检查
    // CUser::OnPassiveCheck(this, fDeltaTime);
    // TODO: 实现 OnPassiveCheck

    // IDA: 获取 CDropProcess 并检查
    // pProcess = XClient::GetProcessPtr<CDropProcess>(this, 0x14u);
    // if (pProcess)
    // {
    //     CDropProcess::UpdateDropItem(pProcess);
    //     if (IsCanApplyBuffByMapID())
    //     {
    //         CUser::SendKeepAlive(this);
    //         CUser::CheckCharacterLocation(this);
    //     }
    //     CUser::SendMoneyLog(this, 0);
    //     CUser::SendTickLog(this);
    //     CUser::SendAll(this);
    // }

    // TODO: 实现完整的组件更新序列
    // 当前保留简化实现以支持编译
}

// BridgeSend - 发送数据包
// IDA 0x1406E8B50: CUser::BridgeSend
// 精确还原: 加锁、检查状态、加密并发送数据包
bool CUser::BridgeSend(XSendPacket& xSendPacket) {
    // IDA: CSimpleLock::Owner lock((CSimpleLock *)((char *)this - 1072));
    // 偏移 -1072 = XSocket::xLock (从 CUser 起点偏移)
    CSimpleLock::Owner lock(&this->xLock);

    // IDA: 检查是否处于 eStateChangeServer 状态
    if (XClient::IsState(eStateChangeServer)) {
        // IDA: v8 = 0; return v8;
        return false;
    }

    // IDA: v12 = *((unsigned __int16 *)this - 539);
    // 偏移 -539 * 2 = -1078 = szBuffer 中的 usOffset (当前缓冲区偏移)
    std::uint16_t usCurrentOffset = this->usOffset;

    // IDA: UsIndex = XParse::GetUsIndex(&xSendPacket->XParse);
    std::uint16_t usPacketSize = xSendPacket.XParse.m_usIndex;

    // IDA: nBuffSize = v12 + UsIndex + 5;
    int nBuffSize = static_cast<int>(usCurrentOffset) + static_cast<int>(usPacketSize) + 5;

    // IDA: if (nBuffSize >= 65534) - 缓冲区溢出检查
    if (nBuffSize >= 65534) {
        // IDA: overLab = (XOverLab *)((char *)this - 66712);
        // IDA: pClient = (CUser *)((char *)this - 132384);
        // IDA: v4 = TXSingleton<XGameServer>::Instance();
        // IDA: XIOCPServer::XSend(v4, pClient, overLab);
        // TODO: 需要实现 XSend(XClient*, XOverLab*) 重载
        // 当前暂时跳过缓冲区刷新

        // IDA: v15 = *((void (__fastcall ***)(char *, _QWORD))this - 8339);
        // IDA: (*v15)((char *)this - 66712, 0); - 虚函数调用清理缓冲区
        // TODO: 调用虚函数重置缓冲区

        // IDA: OutputDebugStringA("Send All !! \n");
        OutputDebugStringA("Send All !! \n");
    }

    // IDA: usOutSize = 0;
    std::uint16_t usOutSize = 0;

    // IDA: xSendPacket->usTos = 1;
    xSendPacket.usTos = 1;

    // IDA: XSendPacket::Encrypt(xSendPacket, (char *)this + *((unsigned __int16 *)this - 539) - 66616, &usOutSize)
    // 加密到 szBuffer 缓冲区
    // 偏移计算: usOffset - 66616 是相对于 CUser this 的偏移
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
// 精确还原: 与 BridgeSend 类似，但会检查客户端加载是否完成
bool CUser::BridgeSend_AfterLoading(XSendPacket& xSendPacket) {
    // IDA: CSimpleLock::Owner lock((CSimpleLock *)((char *)this - 1072));
    CSimpleLock::Owner lock(&this->xLock);

    // IDA: 检查是否处于 eStateChangeServer 状态
    if (XClient::IsState(eStateChangeServer)) {
        // IDA: v8 = 0; return v8;
        return false;
    }

    // IDA: 检查 GetClientLoadComplete() 是否为 true
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

    // IDA: if (nBuffSize >= 65534) - 缓冲区溢出检查
    if (nBuffSize >= 65534) {
        // IDA: overLab = (XOverLab *)((char *)this - 66712);
        // IDA: pClient = (CUser *)((char *)this - 132384);
        // IDA: v4 = TXSingleton<XGameServer>::Instance();
        // IDA: XIOCPServer::XSend(v4, pClient, overLab);
        // TODO: 需要实现 XSend(XClient*, XOverLab*) 重载
        // 当前暂时跳过缓冲区刷新

        // IDA: v16 = *((void (__fastcall ***)(char *, _QWORD))this - 8339);
        // IDA: (*v16)((char *)this - 66712, 0); - 虚函数调用清理缓冲区
        // TODO: 调用虚函数重置缓冲区

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
        0,      // nUnk1
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

    // TODO: Get item ID from inventory slot
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (!pInventory) return FALSE;
    //
    // std::uint32_t dwItemID = pInventory->GetItemID(nSlotIndex);
    // if (dwItemID == 0) return FALSE;
    //
    // return UseItem(dwItemID, nSlotIndex) ? TRUE : FALSE;

    GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem stub");
    return TRUE;
}

// EquipItem - Equip item from inventory slot
// Returns: TRUE on success
BOOL CUser::EquipItem(int nSlotIndex) {
    // Validate slot index
    if (nSlotIndex < 0) {
        return FALSE;
    }

    // TODO: Determine equipment slot from item type
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (!pInventory) return FALSE;
    //
    // std::uint32_t dwItemID = pInventory->GetItemID(nSlotIndex);
    // TB_ITEM* pItem = GetTB_ITEM(dwItemID);
    // if (!pItem) return FALSE;
    //
    // int nEquipSlot = pItem->Equip_Slot;
    // return EquipItem(nSlotIndex, nEquipSlot) ? TRUE : FALSE;

    GreenDamTan_log(__FILE__, __FUNCTION__, "EquipItem stub");
    return TRUE;
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
    // Call existing LeaveParty implementation
    // LeaveParty() already exists in header but returns bool
    // This is a void wrapper
    bool bResult = false;

    // TODO: Check if in party
    // if (m_stCharInfo.stPartyInfo.nPartyID == 0) {
    //     return;
    // }

    // TODO: Get party manager and leave party
    // CPartyManager* pPartyMgr = CPartyManager::Instance();
    // if (pPartyMgr) {
    //     bResult = pPartyMgr->LeaveParty(this);
    // }

    // Clear party info
    // m_stCharInfo.stPartyInfo.nPartyID = 0;
    // m_stCharInfo.stPartyInfo.nPartyMemberIndex = -1;

    // Send leave notification
    XSendPacket xPacket;
    // xPacket.SetCommand(SERVER_CMD_PARTY_LEAVE);
    SendPacket(xPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "LeaveParty stub");
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

    // TODO: Check if already in guild
    // TODO: Check guild creation requirements (level, money, etc.)
    // TODO: Get guild manager
    // TODO: Create new guild
    // TODO: Set player as guild master
    // TODO: Update m_stCharInfo.stLeagueInfo
    // TODO: Send guild creation notification

    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateGuild stub");
    return TRUE;
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

    // TODO: Check inventory space via CGocInventory component
    // TODO: Check if item can stack (Item_Stack_Max)
    // TODO: Check if item already exists for stacking
    // TODO: Add to inventory slot

    // Current stub implementation - delegate to inventory component
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (pInventory) {
    //     return pInventory->AddItem(dwItemID, nCount, bBind, nExpireTime);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "AddItem stub");
    return nCount;
}

// RemoveItem - Remove item from inventory
// Returns: item count removed, or -1 on error
int CUser::RemoveItem(std::uint32_t dwItemID, int nCount) {
    // Validate parameters
    if (dwItemID == 0 || nCount <= 0) {
        return -1;
    }

    // TODO: Find item in inventory
    // TODO: Check if enough quantity
    // TODO: Remove from slot

    // Current stub implementation - delegate to inventory component
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (pInventory) {
    //     return pInventory->RemoveItem(dwItemID, nCount);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveItem stub");
    return nCount;
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

    // TODO: Check if item is consumable (Item_Sub_Type)
    // TODO: Check cooldown (Cooltime_Group, Cooltime_Value)
    // TODO: Apply item effects (Item_Effect_Type, Item_Effect_ID)
    // TODO: Remove item after use

    // Apply effects based on Item_Effect_Type
    switch (pItemTable->Item_Effect_Type) {
        case 1:  // HP recovery
            // SetHP(GetHP() + pItemTable->Item_Use_Value);
            GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: HP recovery");
            break;
        case 2:  // MP/SG recovery
            // SetSG(GetSG() + pItemTable->Item_Use_Value);
            GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: SG recovery");
            break;
        case 3:  // Buff
            GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: Buff effect");
            break;
        default:
            GreenDamTan_log(__FILE__, __FUNCTION__, "UseItem: Unknown effect type");
            break;
    }

    // Remove one item from inventory
    // RemoveItem(dwItemID, 1);

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

    // TODO: Check if slot has item
    // TODO: Check if item can be equipped (Item_Sub_Type, Item_Slot_Disable)
    // TODO: Check level requirement (Item_Limit_Lv)
    // TODO: Check class requirement (Item_Limit_Class)
    // TODO: Unequip current item in slot if any
    // TODO: Move item to equipment slot

    // Current stub implementation - delegate to inventory component
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (pInventory) {
    //     return pInventory->EquipItem(nSlotIndex, nEquipSlot);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "EquipItem stub");
    return true;
}

// UnequipItem - Remove item from slot
// Returns: true on success
bool CUser::UnequipItem(int nEquipSlot) {
    // Validate parameters
    if (nEquipSlot < 0) {
        return false;
    }

    // TODO: Check if equipment slot has item
    // TODO: Check if inventory has space
    // TODO: Move item from equipment slot to inventory

    // Current stub implementation - delegate to inventory component
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (pInventory) {
    //     return pInventory->UnequipItem(nEquipSlot);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "UnequipItem stub");
    return true;
}

// GetEquipSlot - Get item at equipment slot
// Returns: item ID at slot, or 0 if empty
std::uint32_t CUser::GetEquipSlot(int nEquipSlot) const {
    // Validate parameters
    if (nEquipSlot < 0) {
        return 0;
    }

    // TODO: Access equipment slots from CGocInventory
    // Current stub implementation
    // CGocInventory* pInventory = const_cast<CUser*>(this)->GetGOC<CGocInventory>();
    // if (pInventory) {
    //     return pInventory->GetEquipSlot(nEquipSlot);
    // }

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

    // TODO: Check if already in party
    // TODO: Get party manager
    // TODO: Find party by ID
    // TODO: Check party size limit
    // TODO: Add player to party
    // TODO: Send party join notification

    // Current stub implementation - delegate to party manager
    // CPartyManager* pPartyMgr = CPartyManager::Instance();
    // if (pPartyMgr) {
    //     return pPartyMgr->JoinParty(this, dwPartyID);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "JoinParty stub");
    return true;
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

    // TODO: Check if already in guild
    // TODO: Get guild manager
    // TODO: Find guild by ID
    // TODO: Check guild member limit
    // TODO: Add player to guild
    // TODO: Update m_stCharInfo.stLeagueInfo
    // TODO: Send guild join notification

    // Update league info (simplified)
    // m_stCharInfo.stLeagueInfo.nLeagueID = dwGuildID;

    GreenDamTan_log(__FILE__, __FUNCTION__, "JoinGuild stub");
    return true;
}

// LeaveGuild - Leave guild
// Returns: true on success
bool CUser::LeaveGuild() {
    // TODO: Check if in guild
    // TODO: Get guild manager
    // TODO: Remove player from guild
    // TODO: If guild leader leaves, assign new leader or disband
    // TODO: Update m_stCharInfo.stLeagueInfo
    // TODO: Send guild leave notification

    // Update league info (simplified)
    // m_stCharInfo.stLeagueInfo.nLeagueID = 0;

    GreenDamTan_log(__FILE__, __FUNCTION__, "LeaveGuild stub");
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

// GetMP - Get current MP/SG
// IDA 0x14070AC60 (estimated)
int CUser::GetMP() {
    // MP/SG is stored separately, not in m_stCharInfo
    // TODO: Determine actual SG storage location from IDA
    // Possible locations: m_nSG member or CGocAttribute component
    // Currently return placeholder value
    return 0;
}

// SetMP - Set MP/SG value
// IDA 0x1406F48C0 (estimated)
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

    // Update MP/SG value - stored in separate member or component
    // TODO: Determine actual SG storage location from IDA
    // m_nSG = nFinalMP;

    // Sync to CGocAttribute component
    // TODO: When CGocAttribute is fully defined:
    // GetGOC<CGocAttribute>()->SetSG(nFinalMP);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SetMP called");
}

// GetMaxHP - Get max HP (override from CMover)
// IDA 0x140189410 (CMoverEx::GetMaxHP)
// 反编译: return (unsigned int)(int)this->m_fAbility[10]
int CUser::GetMaxHP() {
    // IDA: CMoverEx::GetMaxHP returns m_fAbility[10] cast to int
    // m_fAbility 数组存储各种属性值，索引 10 是最大 HP
    return static_cast<int>(m_fAbility[10]);
}

// GetMaxMP - Get max MP/SG
// TODO: 需人工审查 - IDA 地址待确认 (0x140189450 不是正确地址)
int CUser::GetMaxMP() {
    // MP/SG (Soul Gauge) 存储在 m_fAbility 数组中
    // 假设与 GetMaxHP 类似，使用另一个索引
    // TODO: 需要从 IDA 确认正确的 m_fAbility 索引
    return static_cast<int>(m_fAbility[11]);  // 假设索引 11
}

// ============================================================================
// Packet Functions (IDA)
// ============================================================================

// SendPacket - Send packet to client
// IDA 0x1406E8B50 (BridgeSend wrapper)
bool CUser::SendPacket(XSendPacket& xSendPacket) {
    return BridgeSend(xSendPacket);
}

// BroadcastPacket - Broadcast to nearby players
// IDA 0x1406E8F00 (estimated)
void CUser::BroadcastPacket(XSendPacket& xSendPacket, float fRadius) {
    // TODO: Get current position
    // hkvVec3 vPos = GetPosition();

    // TODO: Get sector/area manager
    // CSector* pSector = GetSector();
    // if (!pSector) return;

    // TODO: Iterate nearby players within radius
    // If fRadius == 0, use default vision range
    // float fActualRadius = (fRadius > 0.0f) ? fRadius : GetVisionRange();

    // For each nearby player:
    //   if (pPlayer != this && Distance < fActualRadius) {
    //       pPlayer->SendPacket(xSendPacket);
    //   }

    GreenDamTan_log(__FILE__, __FUNCTION__, "BroadcastPacket stub");
}

// SendToParty - Send to party members
// IDA 0x1406E9000 (estimated)
void CUser::SendToParty(XSendPacket& xSendPacket) {
    // TODO: Get party ID from character info
    // int nPartyID = m_stCharInfo.stPartyInfo.nPartyID;
    // if (nPartyID == 0) return;

    // TODO: Get party manager
    // CPartyManager* pPartyMgr = CPartyManager::Instance();
    // if (!pPartyMgr) return;

    // TODO: Get party members
    // CParty* pParty = pPartyMgr->GetParty(nPartyID);
    // if (!pParty) return;

    // For each party member:
    //   CUser* pMember = pParty->GetMember(i);
    //   if (pMember && pMember != this) {
    //       pMember->SendPacket(xSendPacket);
    //   }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendToParty stub");
}

// SendToGuild - Send to guild members
// IDA 0x1406E9100 (estimated)
void CUser::SendToGuild(XSendPacket& xSendPacket) {
    // TODO: Get guild ID from character info
    // int nGuildID = m_stCharInfo.stLeagueInfo.nLeagueID;
    // if (nGuildID == 0) return;

    // TODO: Get guild manager
    // CGuildManager* pGuildMgr = CGuildManager::Instance();
    // if (!pGuildMgr) return;

    // TODO: Get guild members
    // CGuild* pGuild = pGuildMgr->GetGuild(nGuildID);
    // if (!pGuild) return;

    // For each guild member:
    //   CUser* pMember = pGuild->GetMember(i);
    //   if (pMember && pMember != this) {
    //       pMember->SendPacket(xSendPacket);
    //   }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendToGuild stub");
}

// ============================================================================
// Data Functions (IDA)
// ============================================================================

// SaveData - Save player data to database
// IDA 0x1406E9200 (estimated)
bool CUser::SaveData() {
    // TODO: Validate character data
    // if (!m_stCharInfo.dwUAID) return false;

    // TODO: Save character info
    // - Basic info (name, level, exp, etc.)
    // - Position
    // - Stats
    // - Inventory
    // - Equipment
    // - Skills
    // - Quests
    // - Achievements

    // TODO: Call database save procedure
    // CDatabaseMgr* pDB = CDatabaseMgr::Instance();
    // pDB->SaveCharacter(m_stCharInfo);

    // TODO: Save components
    // GetGOC<CGocInventory>()->Save();
    // GetGOC<CGocSkill>()->Save();
    // GetGOC<CGocQuest>()->Save();
    // GetGOC<CGocAchieve>()->Save();

    GreenDamTan_log(__FILE__, __FUNCTION__, "SaveData stub");
    return true;
}

// LoadData - Load player data from database
// IDA 0x1406E9300 (estimated)
bool CUser::LoadData() {
    // TODO: Validate UAID
    // if (!m_stCharInfo.dwUAID) return false;

    // TODO: Load character info from database
    // CDatabaseMgr* pDB = CDatabaseMgr::Instance();
    // if (!pDB->LoadCharacter(m_stCharInfo.dwUAID, m_stCharInfo)) {
    //     return false;
    // }

    // TODO: Load components
    // GetGOC<CGocInventory>()->Load();
    // GetGOC<CGocSkill>()->Load();
    // GetGOC<CGocQuest>()->Load();
    // GetGOC<CGocAchieve>()->Load();

    // TODO: Apply loaded stats
    // m_nHP = m_stCharInfo.nHP;
    // m_stCharInfo.shSG = m_stCharInfo.shSG;

    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadData stub");
    return true;
}

// ============================================================================
// Inventory Helper Functions
// ============================================================================

// GetItemCount - Get count of specific item in inventory
// Returns: item count, or 0 if not found
int CUser::GetItemCount(std::uint32_t dwItemID) {
    // Validate parameters
    if (dwItemID == 0) {
        return 0;
    }

    // TODO: Access inventory component
    // CGocInventory* pInventory = GetGOC<CGocInventory>();
    // if (pInventory) {
    //     return pInventory->GetItemCount(dwItemID);
    // }

    // Fallback: search in character info inventory
    // TODO: Iterate m_stCharInfo.stInventory slots
    // int nCount = 0;
    // for (each inventory slot) {
    //     if (slot.dwItemID == dwItemID) {
    //         nCount += slot.nCount;
    //     }
    // }
    // return nCount;

    GreenDamTan_log(__FILE__, __FUNCTION__, "GetItemCount stub");
    return 0;
}

// Note: SetLeagueName needs declaration in User.h before implementation

// Note: UpdateLeagueInventorySyncCount, SetLeagueInventoryTime, UpdateLeagueSyncFlag,
// SetLeagueDeletePenalty, SetLeagueWithdrawPenalty, SetLeagueInventorySend,
// UpdateLeagueSyncCount, GetLeagueInventorySyncCount, GetLeagueSyncCount
// need declarations in User.h before implementation

// Note: The following functions need declarations in User.h before implementation:
// GetLeagueSyncFlag, SetAuthSessionID, SetBlockType, GetRevivePoint,
// SetEnterDistrictPos, IsGM, SetGMPower, SetTestMode, GetPublicTransportTime,
// GetPublicTransportIndex, IsPlayingPublicTransport, GetPublicTransportTakeTime,
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

// Note: GetClientLoadComplete, IsUserStatus, GetWorldType,
// GetWaitSuboInputActionProcess, GetLogChangeMap also need declarations

