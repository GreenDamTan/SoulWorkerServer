#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

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
    // TODO: 从 IDA 实现完整逻辑
    // 返回用户的 UAID (通常是账号ID或唯一标识符)
    return 0;
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
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "InitComponant - stub");
}

void CUser::RegisterProcess() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "RegisterProcess - stub");
}

void CUser::ChangeBattlePose(int nPose) {
    // TODO: 汇编还原 - IDA
}

void CUser::SetInfo() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetInfo - stub");
}

void CUser::InitStoreSuboInputPacket() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "InitStoreSuboInputPacket - stub");
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
    // TODO: 汇编还原 - IDA 0x140082D20
    return L"";
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
    // TODO: 汇编还原 - IDA 0x14018FC60
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
    // IDA 0x14070A490:
    // if (*(_QWORD *)&this->szBuffer[61511]) return **(unsigned __int16 **)&this->szBuffer[61511];
    // else return 0;
    // TODO: 需要确认正确的成员偏移
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

    // TODO: 同步到 CGocAttribute 组件
    // CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
    // CGocAttribute::SetHP(pAttr, (float)nFinalHP);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SetHP called");
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

        // 返回是否死亡
        return (nFinalHP == 0) ? 1 : 0;
    }

    return 0;
}

// ApplySkillDamageFrame IDA 0x1406F6140
// 应用技能伤害帧
void CUser::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                  std::uint8_t byAttackTargetCnt) {
    // TODO: 获取技能表
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    // if (!pSkillTable) return;

    // TODO: 根据 Use_Position 字段处理
    // 如果 Use_Position == 2，调用 Akashic 对象的方法

    // TODO: 获取攻击判定触发器
    // AttackJudgmentTrigger* pTrigger = CMoverEx::GetAttackJudgmentEvent(this, nTriggerIdx);
    // if (!pTrigger) return;

    // TODO: 遍历攻击目标并应用伤害

    GreenDamTan_log(__FILE__, __FUNCTION__, "ApplySkillDamageFrame called");
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
            // TODO: ChangeCombatType(0)
            GreenDamTan_log(__FILE__, __FUNCTION__, "ChangeCombatType(0) - stub");
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
            // TODO: return IsHitDown() ? 1 : 0;
            return 0;

        case 4:
            // 类型4: 检查是否反击命中
            // IDA: return this->IsCounterAttackHit(this);
            return 0;

        case 5:
            // 类型5: 检查技能解锁buff
            // IDA: return CMover::IsActivateSkillUnlockBuff(this, pTBSkill);
            return 1;

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
    // TODO: 需要检查 XActor::IsStatus 并调用 ChangeMotion_3
    GreenDamTan_log(__FILE__, __FUNCTION__, "CancelSkill - stub");
}

// GetSkillLevel IDA 0x140189040 (CMoverEx::GetSkillLevel)
// 获取当前技能等级
std::uint8_t CUser::GetSkillLevel() {
    // IDA 反编译:
    // if (this->m_pCurSkillTableRef)
    //     return this->m_pCurSkillTableRef->Skill_LV;
    // else
    //     return 0;
    // TODO: 需要从 CMoverEx 基类获取 m_pCurSkillTableRef
    return 0;
}

// GetSkillCoolDownRate IDA 0x1402C7240 (CMover::GetSkillCoolDownRate)
// 获取技能冷却速率修正
float CUser::GetSkillCoolDownRate() {
    // IDA 反编译:
    // return this->m_fSkillCoolDownRate;
    // TODO: 需要从 CMover 基类获取 m_fSkillCoolDownRate
    return 0.0f;
}

// SetSkillCoolDownRate - 设置技能冷却速率修正
void CUser::SetSkillCoolDownRate(float fRate) {
    // TODO: 设置 CMover::m_fSkillCoolDownRate
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetSkillCoolDownRate - stub");
}

// CheckSkillSkipType IDA 0x14037E490 (CMoverEx::CheckSkillSkipType)
// 检查技能跳过类型
// Skill_Motion_Skip_Type: 1=检查状态1, 2=检查动作1或3-6, 3=总是跳过
bool CUser::CheckSkillSkipType(std::uint32_t nSkillID) {
    // IDA 反编译:
    // pSkillTbl = XResourceMgr::GetTB_SKILL(..., nSkillID);
    // if (!pSkillTbl) return 0;
    // switch (pSkillTbl->Skill_Motion_Skip_Type) {
    //   case 1: return XActor::IsStatus(this, 1);
    //   case 2: return (m_nMotionClass == 1 || (m_nMotionClass >= 3 && m_nMotionClass <= 6));
    //   case 3: return 1;
    // }
    // return 0;

    // TODO: 获取技能表并检查 Skill_Motion_Skip_Type
    return false;
}

// IsCanSkill IDA 0x14037FB80 (CMoverEx::IsCanSkill)
// 检查是否可以使用技能 (不能有状态0x40000000或0x80000000)
bool CUser::IsCanSkill() {
    // IDA 反编译:
    // return !XActor::IsStatus(&this->XActor, 0x40000000u)
    //     && !XActor::IsStatus(&this->XActor, 0x80000000);
    // TODO: 需要检查 XActor 状态
    return true;
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
    // IDA 反编译:
    // this->m_pCurSkillTableRef = pSkillRef;
    // TODO: 设置 CMoverEx::m_pCurSkillTableRef
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetSkillTable - stub");
}

// ============================================================================
// CGocSkill 相关方法 (通过组件访问)
// ============================================================================

// IsHaveSkill - 检查是否拥有指定技能
bool CUser::IsHaveSkill(int nSkillID) {
    // TODO: 获取 CGocSkill 组件并检查
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     return pSkillComp->IsHaveSkill(nSkillID);
    // }
    return false;
}

// LearnSkill - 学习新技能
bool CUser::LearnSkill(int nSkillID, bool bUseCheat, int nTicknum) {
    // TODO: 获取 CGocSkill 组件并学习
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     return pSkillComp->LearnSkill(nSkillID, bUseCheat, nTicknum);
    // }
    GreenDamTan_log(__FILE__, __FUNCTION__, "LearnSkill - stub");
    return false;
}

// ResetSkill - 重置技能点
void CUser::ResetSkill(bool bUseCheat, int nTicknum) {
    // TODO: 获取 CGocSkill 组件并重置
    // CGocSkill* pSkillComp = GetGOC<CGocSkill>();
    // if (pSkillComp) {
    //     pSkillComp->ResetSkill(bUseCheat, nTicknum);
    // }
    GreenDamTan_log(__FILE__, __FUNCTION__, "ResetSkill - stub");
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
void CUser::SetPassiveSkillStat(std::uint16_t wBuffID) {
    // 获取 Buff 表
    // TODO: TB_BUFF* pBuffTable = XResourceMgr::GetTB_BUFF(wBuffID);
    // if (!pBuffTable) return;

    // 如果有效果类型1或持续时间，设置buff状态
    // 否则设置属性效果

    GreenDamTan_log(__FILE__, __FUNCTION__, "SetPassiveSkillStat - stub");
}

// ClearPassiveSkillStat - 清除被动技能属性
void CUser::ClearPassiveSkillStat(std::uint16_t wBuffID) {
    // 获取 Buff 表
    // TODO: TB_BUFF* pBuffTable = XResourceMgr::GetTB_BUFF(wBuffID);
    // if (!pBuffTable) return;

    // 清除buff状态或属性效果

    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearPassiveSkillStat - stub");
}

// CheckPassiveSkill - 检查并触发被动技能
void CUser::CheckPassiveSkill(std::uint8_t byType, std::uint8_t byParam) {
    // TODO: 遍历被动技能并检查触发条件
    // for (auto& pSkill : m_vPassiveSkill) {
    //     TB_SKILL* pTbl = pSkill->GetTableRef();
    //     if (pTbl && pTbl->Passive_Type == byType) {
    //         // 触发被动技能效果
    //     }
    // }
    GreenDamTan_log(__FILE__, __FUNCTION__, "CheckPassiveSkill - stub");
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
