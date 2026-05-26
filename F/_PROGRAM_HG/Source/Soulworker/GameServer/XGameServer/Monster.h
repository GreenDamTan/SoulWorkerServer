#pragma once

#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/GroupAggro.h"
#include "Soulworker/GameServer/XGameServer/STMonsterInfo.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>

// 前置声明
struct TB_MONSTER;
class CAi;

// CMonster 继承自 CMoverEx (60392 bytes)
// IDA 确认大小: 60896 bytes
class CMonster : public CMoverEx {
public:
    CMonster();
    virtual ~CMonster();

    // === IDA 反编译确认的方法 ===

    // GetParentID IDA 0x14009F170 - 获取父 ActorID
    UXActorID GetParentID();

    // GetAi IDA 0x14009F1A0 - 获取 AI 对象
    CAi* GetAi();

    // SetSummonType IDA 0x14009F1E0 - 设置召唤类型
    void SetSummonType(std::uint8_t byType);

    // GetGroupAggro IDA 0x140198DC0 - 获取群体仇恨对象
    CGroupAggro* GetGroupAggro();

    // GetCallScriptDie IDA 0x140199230 - 获取脚本死亡调用标志
    bool GetCallScriptDie();

    // NotifyRemoved IDA 0x14018BBB0 - 通知移除（虚函数）
    virtual void NotifyRemoved();

    // GetTableID IDA 0x140364AD0 - 获取表 ID
    int GetTableID();

    // GetMobTableRef IDA 0x140016EF0 - 获取怪物表引用
    TB_MONSTER* GetMobTableRef();

    // SetTablePtr IDA 0x1403558A0 - 设置表指针
    void SetTablePtr(TB_MONSTER* pTBMonster);

    // GetHP IDA 0x140364D60 - 获取 HP
    int GetHP();

    // Reset IDA 0x140354D20 - 重置
    void Reset();

    // Init IDA 0x140355900 - 初始化
    void Init();

    // 动画相关
    virtual void ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos);
    bool CheckSuperArmorMotion(std::int16_t nMotionClass);
    void CheckProtectSkillUI();

protected:
    // === IDA 确认的成员变量 (offset from CMoverEx end, 60392+) ===

    // offset 60392: m_mapReservedMotion (std::map<short,short>, 32 bytes)
    std::map<std::int16_t, std::int16_t> m_mapReservedMotion;

    // offset 60424: m_stMonsterInfo (STMonsterInfo, 128 bytes)
    STMonsterInfo m_stMonsterInfo;

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
    CGroupAggro m_xGroupAggro;

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
