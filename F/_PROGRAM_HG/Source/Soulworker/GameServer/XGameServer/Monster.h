#pragma once

#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>

// 前置声明
struct TB_MONSTER;
class CAi;
struct STMonsterInfo;

// CMonster 继承自 CMoverEx (60392 bytes)
// IDA 确认大小: 60896 bytes
class CMonster : public CMoverEx {
public:
    CMonster();
    virtual ~CMonster();

    // 来自 IDA 符号的方法
    TB_MONSTER* GetMobTableRef();
    UXActorID GetParentID();
    CAi* GetAi();
    void SetSummonType(std::uint8_t byType);
    void Reset();
    void Init();

protected:
    // === IDA 确认的成员变量 (offset from CMoverEx end, 60392+) ===

    // offset 60392: m_mapReservedMotion (std::map<short,short>, 32 bytes)
    std::map<std::int16_t, std::int16_t> m_mapReservedMotion;

    // offset 60424: m_stMonsterInfo (STMonsterInfo, 128 bytes)
    STMonsterInfo* m_stMonsterInfo_dummy;  // TODO: 需要定义 STMonsterInfo 结构

    // offset 60552: m_pMobTableRef (TB_MONSTER*)
    TB_MONSTER* m_pMobTableRef;

    // offset 60560: m_pAi (CAi*)
    CAi* m_pAi;

    // offset 60568: m_arDamageMeter (std::map<ulong,tagDamageMeter>, 32 bytes)
    std::map<std::uint32_t, void*> m_arDamageMeter;

    // offset 60600-60608: NextSkill/SendMove
    int m_nNextSkillID;
    std::int16_t m_shLastSendMoveYaw;
    int m_bNeedSendMoveStop;

    // offset 60612-60620: Move/Yaw
    void* m_vLastTargetMovePos_dummy;  // tagMOVE_POS
    float m_fInitYaw;

    // offset 60624-60640: Debug
    hkvVec3 m_vDebugMessagePos;

    // offset 60640-60672: GroupAggro
    void* m_xGroupAggro_dummy;  // CGroupAggro (32 bytes)

    // offset 60672-60680: Aggro
    float m_fLastAggroCheckTime;
    int m_bChangedAggro;
    std::uint32_t m_dwGuardID;

    // offset 60684: GazeAnimType
    std::uint8_t m_byGazeAnimType;

    // offset 60688-60744: TraceHPState
    void* m_xTraceHPState_dummy[7];  // CTraceHPState (56 bytes)

    // offset 60744-60752: Spawn/Group
    int m_nSpawnBoxID;
    int m_nGroupID;
    bool m_bShowChangedAI;

    // offset 60760-60768: SpawnedTime
    std::uint64_t m_dwSpawnedTime64;
    int m_bSuicide;

    // offset 60772-60780: Guard/Killer
    std::uint32_t m_dwGuardMonsterID;
    int m_nMoveType;
    std::uint32_t m_dwKillerID;

    // offset 60784-60792: Weapon
    void* m_pWeaponTableRef;  // TB_DEFENSIVE_WEAPON*

    // offset 60792-60800: LifeTime
    float m_fLifeTime;
    int m_nHPPerSec;
    int m_nLastTime;

    // offset 60804-60816: ProtectSkill
    int m_nProtectSkill;
    int m_eSkillType;  // tagTB_SKILL_TYPE
    int m_nProtectSkillDamage;
    int m_bApplyLevel;

    // offset 60820-60828: Summon
    std::uint8_t m_bySummonType;
    float m_fSummonLifeTime;
    int m_nHitCount;

    // offset 60832-60840: Script
    void* m_pScriptInst;  // IVScriptInstance*
    int m_naCount;
    bool m_bChangeAiScript;
    bool m_bCallScriptDie;

    // offset 60848-60856: DamageCount
    float m_fLastDamageCountTime;
    float m_fBossHPLogTime;
    int m_nCheckBossHP;

    // offset 60860-60868: ReserveDie
    bool m_bReserveDie;
    float m_fElapsedDieTime;
    std::uint8_t m_byAngleAttackType;

    // offset 60872-60896: TickCount/WrongPos
    std::uint64_t m_dwTickCountReceiveWrongPos;
    hkvVec3 m_vStoredWrongPos;

    // Total size: 60896 bytes (verified from IDA)
};
