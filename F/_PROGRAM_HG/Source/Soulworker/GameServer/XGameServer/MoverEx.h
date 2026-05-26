#pragma once

#include "Soulworker/GameServer/XGameServer/Mover.h"
#include <cstdint>
#include <vector>
#include <map>

// 前置声明
struct TB_SKILL;
class TB_AURA;
class TB_AKASHIC_RECORDS;
class TB_DECK_BONUS;
class TB_DIVERGENCE;
class CWayPoint;
class CSector;
class VGameTrapObject;
class hkaiPointCloudSilhouetteGenerator;
struct VAnimationInfo;
struct VActionResourceLump;
struct VString;
struct hkvVec3;

// TODO: 推测结果 - 来自 IDA struct CMoverEx (60392 bytes)
// CMoverEx 继承自 CMover (58592 bytes)
// 是 Vision Engine 实体的扩展类
class CMoverEx : public CMover {
public:
    CMoverEx();
    virtual ~CMoverEx();

    // 来自 IDA 符号的方法
    float GetSkillLoopTime();
    void ResetAddExpFromOptionEffect();
    int GetAddExpFromOptionEffect();
    void SetOwnerID(std::uint32_t dwID);
    std::uint32_t GetOwnerID();
    void ResetAddEtherFromOptionEffect();
    int GetAddEtherFromOptionEffect();
    void ResetAddMoneyFromOptionEffect();
    int GetAddMoneyFromOptionEffect();

    // 战斗姿态
    virtual void ChangeCombatType(int nType, float fParam1, float fParam2);
    virtual void SetCombatType(int nType);
    int GetCombatType();
    virtual bool IsControlMonster();
    virtual void SetControlMonsterFlag(bool bFlag);
    virtual void SetControlMonster(const hkvVec3& vPos, std::uint32_t dwID1, std::uint32_t dwID2,
                                    float fParam1, float fParam2, char* pStr, bool bFlag);

    // 技能
    virtual void SetSkillTable(TB_SKILL* pTable);
    TB_SKILL* GetSkillTable();
    std::uint8_t GetSkillLevel();
    std::uint8_t GetSkillChargeStep();

    // 状态
    bool IsBattlePose();
    void SetBattlePose(bool bPose);
    virtual void ChangeBattlePose(bool bPose1, bool bPose2);

    // 其他
    float GetLookPitch();
    float GetMovingYaw();
    int GetMaxHP();
    void SetAkashicObject(CMoverEx* pObject);
    bool IsCounterSuccessFrame();
    void SetSilhoutte(hkaiPointCloudSilhouetteGenerator* pSilhouette);
    hkaiPointCloudSilhouetteGenerator* GetSilhoutte();
    void SetAmountOfHeal(float fAmount);
    void SetWayPointID(int nID);
    void SetAggroLevelOrder(std::uint8_t byLevel);
    void SetUpdateRotation(bool bUpdate);
    std::uint8_t GetStandType();
    CWayPoint* GetWayPoint();

    // 条件
    void SetPvpCondition(int nCondition);
    int GetPvpCondition();
    void SetActionCondition(int nCondition);
    int GetActionCondition();
    void ClearActionCondition(int nCondition);
    bool IsActionCondition(int nCondition);
    bool IsPvpCondition(int nType);

protected:
    // === IDA 确认的成员变量 (offset from CMover end, 58592+) ===
    // CMoverEx 成员从偏移 58592 开始

    // offset 58592: m_fLastChangeAnimationTime (float)
    float m_fLastChangeAnimationTime;

    // offset 58596: m_bBattlePose (bool)
    bool m_bBattlePose;

    // offset 58600: m_fMovingYaw (float)
    float m_fMovingYaw;

    // offset 58604: m_fSummonAkashicYaw (float)
    float m_fSummonAkashicYaw;

    // offset 58608: m_fLookPitch (float)
    float m_fLookPitch;

    // offset 58612: m_fCheckMoveStopTime (float)
    float m_fCheckMoveStopTime;

    // offset 58616: m_fMoveDelayTime (float)
    float m_fMoveDelayTime;

    // offset 58620: m_bCancelMoving (int)
    int m_bCancelMoving;

    // offset 58624: m_fLastStartRotTime (float)
    float m_fLastStartRotTime;

    // offset 58628: m_fStartRotWaitTime (float)
    float m_fStartRotWaitTime;

    // offset 58632-58634: m_bStartRotation, m_bQuickTurn, m_bKeepLookTarget (bool x3)
    bool m_bStartRotation;
    bool m_bQuickTurn;
    bool m_bKeepLookTarget;

    // offset 58636-58656: 速度相关 (float x5)
    float m_fDefWalkSpeed;
    float m_fDefRunSpeed;
    float m_fDefTurnSpeed;
    float m_fBackupTurnSpeed;
    float m_fTurnSpeedRate;
    float m_fQuickTurnSpeed;

    // offset 58660-58684: 僵直相关
    float m_fHitFreezeTime;
    int m_iStiffenCount;
    float m_fStiffenRate;
    float m_fStiffenTime;
    float m_fStiffenDelayTime;
    float m_fStiffenImmuneTime;

    // offset 58684-58736: 动画/跳跃相关
    int m_bAnimSpeedTrigger;
    float m_fFlyVelocity;
    float m_fJumpDelta;
    float m_fJumpDelayTime;
    float m_fFlyingStayTime;
    float m_fJumpTime;
    float m_fJumpHeight;
    float m_fPrevJumpHeight;
    float m_fFlyDirValue;
    float m_fFlyGravity;
    float m_fFlyMaxHeight;
    int m_bJumpAnim;
    std::uint8_t m_byAniProcessLinkType;
    std::uint8_t m_byMoveDirAnim;

    // offset 58736: m_pCurSkillTableRef (TB_SKILL*)
    TB_SKILL* m_pCurSkillTableRef;

    // offset 58744-58764: 攻击/技能相关
    int m_bAttackKeyPress;
    std::uint8_t m_bySkillAnimStep;
    std::uint8_t m_bySkillAnimCount;
    float m_fSkillLoopTime;
    float m_fSkillBlendEndTime;
    std::uint8_t m_bySkillChargeStep;
    std::uint8_t m_bySkillChargeMaxStep;
    float m_fSkillChargeChangeTime;

    // offset 58768-58784: 转向相关
    int m_bHasTurnMotion;
    int m_bHasBigTurn;
    int m_bSkipReplayTime;
    int m_nShieldHP;
    VString m_strSpecialDamage;

    // offset 58792-58824: Phase 相关
    std::uint8_t m_byPhaseType;
    std::uint8_t m_byPhaseStep;
    std::uint8_t m_byMaxPhaseStep;
    VString m_strPhaseChangeAnim;
    std::uint8_t m_byPhaseCondition;
    std::uint32_t m_dwPhaseConditionValue;
    float m_fPhaseDurationTime;
    std::int16_t m_nPlayPhaseMotion;
    std::int16_t m_nBuffMotion;

    // offset 58824-58836: 待机相关
    std::uint8_t m_byStandType;
    std::uint8_t m_byBattleModeAnim;
    float m_fIdleCheckTime;
    int m_nIdleMotionChance;

    // offset 58836-58856: 陷阱/相机相关
    hkvVec3 m_vTrapPos;
    VGameTrapObject* m_pMouseOnTrap;
    hkaiPointCloudSilhouetteGenerator* m_pSilhouet;
    hkvVec3 m_vCamDir;

    // offset 58880-58912: Akashic/Aura 相关
    TB_AKASHIC_RECORDS* m_pCurAkashicRecord;
    CMoverEx* m_pAkashicObject;
    TB_AURA* m_pAuraSkill;
    float m_fAuraCheckTime;
    float m_fAuraLifeTime;

    // offset 58912-58920: m_byAggroLevelOrder (uint8) + padding
    std::uint8_t m_byAggroLevelOrder;

    // offset 58916-58928: 治疗/死亡相关
    float m_fAmountOfHeal;
    std::uint8_t m_byDieReason;
    int m_nDieDamage;

    // offset 58928-58960: 技能位置备份
    hkvVec3 m_vBackupSkillPos;
    hkvVec3 m_vBackupSkillRotate;
    int m_bMoveSkillPrefab;

    // offset 58960-59008: Event/CommonPos 列表
    VPList m_EventObjectList;
    VPList m_CommonPosBoxList;

    // offset 59056-59096: 隐身相关
    hkvVec3 m_vBeforePos;
    std::uint32_t m_dwInvisibleFlag;
    float m_fInvisibleEndTime;
    int m_nInvisibleConditionType;
    int m_nInvisibleConditionVal[4];

    // offset 59096-59132: 目标/传送相关
    std::vector<std::uint32_t> m_vPreTargetList;
    int m_iLastWarpPoint;
    int m_bSystemActor;
    int m_bChangedStat;

    // offset 59140-59160: 充能相关
    float m_fSkillTotalChargeTime;
    bool m_bExistSuboCombo;
    float m_fSuboComboWaitTime;
    float m_fSuboComboCheckTime;

    // offset 59160-59192: Subo Combo 计数
    int m_iSuboComboMaxCount;
    int m_iSuboComboCheckCount;
    int m_iCurLeftSuboDescCount;
    int m_iCurRightSuboDescCount;
    int m_iMaxLeftSuboDescCount;
    int m_iMaxRightSuboDescCount;
    void* m_pSuboComboTrigger;  // SubordinationComboTrigger*

    // offset 59192-59256: Attach 相关
    bool m_bCheckAttachToAttacker;
    CMoverEx* m_pAttachToAttacker;
    hkvVec3 m_vAttachedDir;
    float m_fAttachedDirDist;
    float m_fAttachedDuration;
    float m_fAttachedDistance;
    hkvVec3 m_vAttachedOffset;
    float m_fAttachedAngle;
    int m_iApplyWeightRank;

    // offset 59252-59260: Direction/Counter 相关
    int m_bDisableDirectionToTargetSkill;
    int m_eDieType;  // DIE_TYPE enum
    bool m_bFlyDie;
    bool m_bCounterSuccessFrame;
    bool m_bEnableCounuter;

    // offset 59264-59412: Counter 相关
    float m_fMinCounterRange;
    float m_fMaxCounterRange;
    float m_fCounterAngle;
    char m_szCounterAnimName[128];
    int m_iCounterProbability;
    float m_fCounterDuration;

    // offset 59412-59448: Charging 相关
    bool m_bChargingStart;
    float m_fChargingInputAnimSpeed;
    float m_fChargingInputMaxAddMultiple;
    float m_fChargingInputDuration;
    float m_fChargingInputTime;
    float m_fChargingInputPressTime;
    float m_fChargingInputCalcMultiple;

    // offset 59440-59454: Control Monster / Charging Value
    bool m_bControlMonster;
    float m_fLeftChargingValue;
    float m_fRightChargingValue;
    std::uint8_t m_byLastDamageType;
    bool m_bOnDie;
    bool m_bDieAttack;

    // offset 59456-59512: WayPoint / Sector
    CWayPoint m_xWayPoint;
    CSector* m_pSector;

    // offset 59520-59544: Skill Skip / Divergence
    float m_fSkillSkipCoolTime;
    float m_fUpdateStatBatchTime;
    TB_DIVERGENCE* m_pCurDivergenceTableRef;
    std::uint32_t m_dwDivergenceSkillID;
    std::uint8_t m_byGrapStep;

    // offset 59544-59580: Grap 相关
    VString m_szAttachBoneName;
    CMoverEx* m_pGrapParent;
    CMoverEx* m_pGrapTarget;

    // offset 59568-59608: Drop 相关
    float m_fDropSpeedX;
    float m_fDropSpeedY;
    float m_fDropSpeedZ;
    hkvVec3 m_vGrapDropPos;
    std::uint8_t m_stDropOffset_dummy[8];  // tagMOVE_POS
    void* m_pDetachTrigger;  // DetachTrigger*

    // offset 59608-59624: Auto Rotation
    float m_fAutoRotaionSpeed;
    float m_fAutoRotaionTime;

    // offset 59624-59632: Owner / Absorb
    std::uint32_t m_dwOwnerID;
    bool m_bAllowAbsorbSG;
    int m_nAccumulateDamage;

    // offset 59632-60208: Deck Bonus / Skill Anim
    TB_DECK_BONUS* m_pCurDeckBonusRef;
    char m_szTempSkillAnimName[512];

    // offset 60152-60188: Hit Parts / Change Mob
    std::uint8_t m_sHitParts_dummy[26];  // SHitPartsInfo[2]
    bool m_bReserveChange;
    std::uint32_t m_dwChangeMobNewID;
    std::uint32_t m_dwChangeMobTableID;
    float m_fMoveDistAfterSkill;

    // offset 60192-60244: Test Damage / Phase Motion
    int m_eTestDamageType;  // E_TEST_DAMAGE
    std::uint8_t m_byFixedMaxDamage;
    bool m_bUpdateRotation;
    std::uint8_t m_byPhaseMotionStep;
    float m_fPhaseStepMaxTime;
    float m_fMultipleDamageOnce;
    bool m_bApplyMultipleDamageOnce;
    float m_fMultipleAbsorbSG;

    // offset 60244-60292: SA Break / Option Effect
    std::uint8_t m_bySABreakMotionType;
    float m_fSABreakTime;
    float m_fSABreakLoopTime;
    bool m_bSABreakLoopMotion;
    bool m_bShowSABreakMotion;
    float m_fSABreakLoopMotionTime;
    int m_nAddMoneyFromOptionEffect;
    int m_nAddEtherFromOptionEffect;
    int m_nAddExpFromOptionEffect;
    int m_nCombatType;

    // offset 60292-60324: Link Skill
    bool m_bLinkSkillOn;
    float m_fLinkSkillDuration;
    void* m_pLinkSkillTrigger;  // LinkSkillTrigger*

    // offset 60324-60356: Check Attack Skill
    bool m_bCheckAttackSkillOn;
    float m_fCheckAttackSkillDuration;
    void* m_pCheckAttackSkillTrigger;  // CheckAttackSkillTrigger*

    // offset 60356-60388: Option Effect / Delay Buff
    std::vector<void*> m_vecOptionEffect;  // std::vector<SOptionEffect*>
    std::vector<void*> m_vecDelayBuff;     // std::vector<SDelayBuff>

    // offset 60388-60400: Summon / Motion
    bool m_bSummonMonsterApplyRot;
    std::int16_t m_nNowChangingMotionClass;
    int m_iChangeMotionPos;
    int m_iChangeMotionType;
    std::uint8_t m_iChangeMotionTime_dummy[16];  // _SYSTEMTIME

    // offset 60380-60392: Conditions
    int m_iActionCondition;
    int m_iPvpCondition;

    // IDA 构造函数体调用
    void RemoveAllOptionEffect();
    void RemoveAllDefenseChangeInfo();
    void Reset();

    // Total size: 60392 bytes (verified from IDA)
};
