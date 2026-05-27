#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

// TODO: 汇编还原 - 构造函数 IDA 0x1406E2FA0
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
    // IDA 构造函数序列:
    // 1. XClient::XClient(this)
    // 2. CMoverEx::CMoverEx(&this->CMoverEx)
    // 3-10. vtable assignments for Vision Engine interfaces
    // 11-13. ST_CHAR_COMMUNITY/ST_LEAGUE_INFO/STMyCharInfoEx construction
    // 14-25. member initializations
    // 26. InitComponant()
    // 27. ChangeBattlePose(1)
    // 35-37. SetInfo() / RegisterProcess()

    GreenDamTan_log(__FILE__, __FUNCTION__, "CUser constructed - stub");

    // TODO: 汇编还原 - vtable 赋值序列
    // this->XClient::XSocket::__vftable = (CUser_vtbl *)&CUser::`vftable'{for `XClient'}
    // this->CMoverEx::CMover::VisBaseEntity_cl::VisObject3D_cl::VisTypedEngineObject_cl::VTypedObject::__vftable = ...
    // ... (7 more vtable assignments)

    InitComponant();
    ChangeBattlePose(1);
    // TODO: SetInfo() 和 RegisterProcess() 调用
}

CUser::~CUser() {
    // TODO: 汇编还原 - 析构函数
}

// GetUAID - 获取用户UAID
// IDA 0x14070AF80
std::uint32_t CUser::GetUAID() const {
    // IDA 0x14070AF80: return this->m_stCharInfo.dwUAID
    return m_stCharInfo.dwUAID;
}

// Kickout - 踢出用户
// IDA 地址待确认
void CUser::Kickout(PS_KICK_USER_INFO* psKick, bool bDirect) {
    // TODO: 从 IDA 实现完整逻辑
    // 1. 设置踢出信息
    // 2. 如果 bDirect 为 true，立即断开连接
    // 3. 否则发送踢出消息给客户端
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
    // TODO: 汇编还原 - IDA 0x1400855E0
    m_bMatchingState = bState;
}

void CUser::SetSocialUseID(std::uint32_t dwID) {
    // IDA 0x14018FC60: this->m_dwSocialUseID = dwID
    m_dwSocialUseID = dwID;
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
int CUser::GetHP() {
    return m_nHP;
}

// SetHP IDA 0x1406F4880
// 设置HP并同步到 CGocAttribute 组件
void CUser::SetHP(int nHP) {
    int nMaxHP = GetMaxHP();
    int nFinalHP = nHP;

    // HP 不能超过 MaxHP
    if (nHP > nMaxHP) {
        nFinalHP = nMaxHP;
    }

    // 更新 HP 值
    m_nHP = nFinalHP;

    // 同步到 CGocAttribute 组件
    // TODO: 当 CGocAttribute 完整定义后取消注释:
    // GetGOC<CGocAttribute>()->SetHP(m_nHP);
}

// DamageProcessHP IDA 0x1406F42C0
// 处理伤害并返回是否死亡
int CUser::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                           int nUnk1, std::uint8_t byUnk1, std::uint8_t byUnk2) {
    // TODO: CheckDedicatedMonster 检查
    // if (CheckDedicatedMonster(dwID, nSkillID, nDamage, byDamageFlag, byHitParts) == 1)
    //     return 0;

    // 获取最大 HP
    float fMaxHP = static_cast<float>(GetMaxHP());

    // 获取当前 HP
    float fCurHP = static_cast<float>(m_nHP);

    // 如果已经死亡 (HP=0)，直接返回
    if (fCurHP == 0.0f) {
        return 1;
    }

    // 计算最终 HP
    float fFinalHP = fCurHP - static_cast<float>(nDamage);
    if (fFinalHP < 0.0f) {
        fFinalHP = 0.0f;
    }

    int nFinalHP = static_cast<int>(fFinalHP);

    // 只处理有效伤害 (nDamage >= 0)
    if (nDamage >= 0) {
        // TODO: 调用 CGocAttribute 方法

        // HP 百分比检测 - 触发被动技能
        if (fCurHP > 0.0f && fFinalHP > 0.0f) {
            float fCurRate = (fCurHP / fMaxHP) * 100.0f;
            float fFinalRate = (fFinalHP / fMaxHP) * 100.0f;

            // HP 降到 50% 以下 - 触发被动技能 54
            if (fCurRate > 50.0f && fFinalRate <= 50.0f) {
                // TODO: CheckPassiveSkill(1, 54);
                GreenDamTan_log(__FILE__, __FUNCTION__, "HP below 50pct - trigger passive 54");
            }

            // HP 降到 20% 以下 - 触发被动技能 50
            if (fCurRate > 20.0f && fFinalRate <= 20.0f) {
                // TODO: CheckPassiveSkill(1, 50);
                GreenDamTan_log(__FILE__, __FUNCTION__, "HP below 20pct - trigger passive 50");
            }
        }

        // 死亡时处理 HP 吸收
        if (fCurHP > 0.0f && nFinalHP <= 0) {
            // TODO: 获取攻击者并处理 HP/SG 吸收
            // CMoverEx* pAttackMover = CMover::GetMoverObject(this, dwID);
            // if (pAttackMover) {
            //     // 处理 HP 吸收
            //     // 处理 SG 吸收
            // }
            GreenDamTan_log(__FILE__, __FUNCTION__, "Player died");
        }

        // 更新 HP
        m_nHP = nFinalHP;

        // 同步到 CGocAttribute 组件
        // TODO: 当 CGocAttribute 完整定义后取消注释:
        // GetGOC<CGocAttribute>()->SetHP(m_nHP);

        // 返回是否死亡
        return (nFinalHP == 0) ? 1 : 0;
    }

    return 0;
}

// ApplySkillDamageFrame IDA 0x1406F6140
// 应用技能伤害帧
void CUser::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                  std::uint8_t byAttackTargetCnt) {
    // 委托给 CGocSkill 组件处理技能伤害帧
    // TODO: 当 CGocSkill 完整定义后取消注释:
    // CGocSkill* pSkill = GetGOC<CGocSkill>();
    // if (pSkill) {
    //     pSkill->ApplyDamageFrame(nSkillID, nTriggerIdx, byAttackTargetCnt);
    //     return;
    // }

    // 后备: 直接获取技能表并处理
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTable) return;

    // TODO: 根据 Use_Position 字段处理
    // 如果 Use_Position == 2，调用 Akashic 对象的方法

    // TODO: 获取攻击判定触发器
    // AttackJudgmentTrigger* pTrigger = CMoverEx::GetAttackJudgmentEvent(this, nTriggerIdx);
    // if (!pTrigger) return;

    // TODO: 遍历攻击目标并应用伤害
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

    GreenDamTan_log(__FILE__, __FUNCTION__, "PreSkillProcess - stub");
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

    // TODO: 遍历条件并检查
    // int conditionCount = GetConditionNumber(nSkillIndex);
    // int successCount = 0;
    // for (each condition) {
    //     if (condition fulfilled) successCount++;
    // }
    // return conditionCount == successCount;

    return true;
}

// ============================================================================
// 核心虚函数实现 (IDA 反编译)
// ============================================================================

// OnUpdate - 更新循环
// IDA 0x1406ED290
// 这是一个非常大的函数，处理玩家状态更新、数据同步、组件更新等
void CUser::OnUpdate(float fDeltaTime) {
    // IDA 反编译摘要:
    // 1. 检查踢出超时 (szBuffer[61031] 存储踢出时间)
    // 2. 检查 DB 加载状态并同步
    // 3. 调用 CMover::OnUpdate
    // 4. 根据状态标志发送各种数据包
    // 5. 更新所有组件 (CGocInventory, CGocAchieve, etc.)
    // 6. 发送保活和位置检查

    // TODO: 完整实现需要:
    // - 检查 m_dwKickoutTime 超时
    // - 调用 CheckDBLoad_All / SendSyncDBLoad
    // - 调用 CMover::OnUpdate(fDeltaTime)
    // - 根据状态标志发送各种数据包:
    //   - SendCharacterInfo (szBuffer[60619] & 1, szBuffer[60627] & 1)
    //   - SendInventory (szBuffer[60619] & 2, szBuffer[60627] & 2)
    //   - SendBank, SendQuickSlotInfo, etc.
    // - 更新所有组件 OnUpdate
    // - 调用 OnPassiveCheck, SendKeepAlive, CheckCharacterLocation

    // 基类更新
    // CMover::OnUpdate(fDeltaTime);

    // 组件更新 (IDA 反编译序列)
    // CGocAttribute::OnUpdate(fDeltaTime)
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

    // 被动技能检查
    // OnPassiveCheck(fDeltaTime);

    // 保活和位置检查
    // SendKeepAlive();
    // CheckCharacterLocation();
    // SendMoneyLog(0);
    // SendTickLog();
    // SendAll();

    GreenDamTan_log(__FILE__, __FUNCTION__, "OnUpdate called");
}

// BridgeSend - 发送数据包
// IDA 0x1406E8B50
// 加锁、检查状态、加密并发送数据包
bool CUser::BridgeSend(XSendPacket& xSendPacket) {
    // IDA 反编译:
    // 1. 获取锁 CSimpleLock::Owner
    // 2. 检查是否处于 eStateChangeServer 状态
    // 3. 检查缓冲区大小，如果 >= 65534 则先发送累积数据
    // 4. 设置 usTos = 1
    // 5. 调用 XSendPacket::Encrypt 加密
    // 6. 更新缓冲区偏移

    // TODO: 完整实现需要:
    // - 获取发送锁
    // - 检查状态
    // - 检查缓冲区溢出，必要时调用 XIOCPServer::XSend
    // - 加密数据包
    // - 更新缓冲区偏移

    // 简化实现
    if (IsState(eStateChangeServer)) {
        return false;
    }

    // 设置目标
    xSendPacket.usTos = 1;

    // TODO: 实际加密和发送逻辑
    // XSendPacket::Encrypt(buffer, &usOutSize)

    GreenDamTan_log(__FILE__, __FUNCTION__, "BridgeSend called");
    return true;
}

// BridgeSend_AfterLoading - 加载完成后发送数据包
// IDA 0x1406E8D00
// 与 BridgeSend 类似，但会检查客户端加载是否完成
bool CUser::BridgeSend_AfterLoading(XSendPacket& xSendPacket) {
    // IDA 反编译:
    // 1. 获取锁
    // 2. 检查是否处于 eStateChangeServer 状态
    // 3. 检查 GetClientLoadComplete() 是否为 true
    // 4. 检查缓冲区大小
    // 5. 加密并发送

    // TODO: 完整实现需要:
    // - 获取发送锁
    // - 检查状态
    // - 检查 GetClientLoadComplete()
    // - 检查缓冲区溢出
    // - 加密数据包

    if (IsState(eStateChangeServer)) {
        return false;
    }

    // TODO: 检查客户端加载是否完成
    // if (!GetClientLoadComplete()) {
    //     return false;
    // }

    // 设置目标
    xSendPacket.usTos = 1;

    GreenDamTan_log(__FILE__, __FUNCTION__, "BridgeSend_AfterLoading called");
    return true;
}

// ============================================================================
// IDA 还原函数: GetAuthSessionID, IsPrivateShop, IsPVPPenalty
// ============================================================================

// GetAuthSessionID - 获取认证会话ID
// IDA 0x1401C9EE0
std::int64_t CUser::GetAuthSessionID() const {
    return m_biAuthSessionID;
}

// IsPrivateShop - 检查是否在私人商店模式
// IDA 0x1402D3700
// Note: STCharInfo::stShopInfo.STPrivateShopInfo::byType
bool CUser::IsPrivateShop() const {
    return m_stCharInfo.stShopInfo.byType != 0;
}

// IsPVPPenalty - 检查是否存在PVP惩罚
// IDA 0x1401ADC50
bool CUser::IsPVPPenalty() const {
    return m_bPVPPenalty;
}

// ============================================================
// GetActorID - 获取角色ActorID
// IDA 0x1406E8A30
// ============================================================
UXActorID CUser::GetActorID() const {
    return m_stCharInfo.uxActorID;
}

// ============================================================================
// Inventory Functions
// ============================================================================

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

// LeaveParty - Leave current party
// Returns: true on success
bool CUser::LeaveParty() {
    // TODO: Check if in party
    // TODO: Get party manager
    // TODO: Remove player from party
    // TODO: If party leader leaves, assign new leader or disband
    // TODO: Send party leave notification

    // Current stub implementation - delegate to party manager
    // CPartyManager* pPartyMgr = CPartyManager::Instance();
    // if (pPartyMgr) {
    //     return pPartyMgr->LeaveParty(this);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "LeaveParty stub");
    return true;
}

// CreateParty - Create new party
// Returns: party ID on success, or 0 on error
std::uint32_t CUser::CreateParty() {
    // TODO: Check if already in party
    // TODO: Get party manager
    // TODO: Create new party
    // TODO: Set player as leader
    // TODO: Send party creation notification

    // Current stub implementation - delegate to party manager
    // CPartyManager* pPartyMgr = CPartyManager::Instance();
    // if (pPartyMgr) {
    //     return pPartyMgr->CreateParty(this);
    // }

    static std::uint32_t s_nNextPartyID = 1;
    std::uint32_t dwPartyID = s_nNextPartyID++;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateParty stub");
    return dwPartyID;
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

// CreateGuild - Create new guild
// Returns: guild ID on success, or 0 on error
std::uint32_t CUser::CreateGuild(const std::wstring& strName) {
    // Validate parameters
    if (strName.empty()) {
        return 0;
    }

    // TODO: Check if already in guild
    // TODO: Check guild creation requirements (level, money, etc.)
    // TODO: Get guild manager
    // TODO: Create new guild
    // TODO: Set player as guild master
    // TODO: Update m_stCharInfo.stLeagueInfo
    // TODO: Send guild creation notification

    // Current stub implementation - delegate to guild manager
    // CGuildManager* pGuildMgr = CGuildManager::Instance();
    // if (pGuildMgr) {
    //     return pGuildMgr->CreateGuild(this, strName);
    // }

    static std::uint32_t s_nNextGuildID = 1;
    std::uint32_t dwGuildID = s_nNextGuildID++;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateGuild stub");
    return dwGuildID;
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
