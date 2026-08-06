#pragma once

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"  // 需要完整类型 CMover
#include "Soulworker/GameServer/XGameServer/Physics.h"  // tagATTACK_RANGE (36 bytes)
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <string>

// 前置声明
class VActionResourceLump;
class VManagedResource;
struct tagHIT_COLLISION_DATA;
struct tagHIT_TRACE_BONE_NAME_DATA;
struct TB_CHARACTER_INFO;
struct TB_MONSTER;
struct TB_NPC;
struct TB_AKASHIC_RECORDS;
struct TB_SKILL;

// tagREACTION_INFO - 反应信息结构 (88 bytes)
// PDB LF_FIELDLIST 0x78395: Size 88, LF_STRUCTURE 0x78396
struct tagREACTION_INFO {
    std::int32_t iTargetType;          // offset 0
    std::int32_t iTargetStatus;        // offset 4
    std::int32_t iTargetGrade;         // offset 8
    bool bUseTargetWeight;             // offset 12
    bool bApplyPcSABreak;              // offset 13
    std::uint8_t _pad0[2];             // offset 14
    float fDamageRate;                 // offset 16
    std::int32_t iBuffID;              // offset 20
    std::int32_t iAuraID;              // offset 24
    std::int32_t iReactionType;        // offset 28
    float fReactionDist;               // offset 32
    float fReactionHeight;             // offset 36
    float fReactionSpeed;              // offset 40
    std::int32_t iReactionArrow;       // offset 44
    float fReactionHeightAir;          // offset 48
    float fReactionAngle;              // offset 52
    float fSlowRate;                   // offset 56
    float fSlowTime;                   // offset 60
    float fSlowDelayTime;              // offset 64
    float fHitFreezeTime;              // offset 68
    bool bCheckCounter;                // offset 72
    std::uint8_t _pad1[3];             // offset 73
    std::int32_t iConditionBuffID;     // offset 76
    float fConditionBuffDamageRate;    // offset 80
    bool bIgnoreTargetInvincible;      // offset 84
    std::uint8_t _pad2[3];             // offset 85

    tagREACTION_INFO()
        : iTargetType(0), iTargetStatus(0), iTargetGrade(0)
        , bUseTargetWeight(false), bApplyPcSABreak(false)
        , _pad0{0, 0}, fDamageRate(0.0f), iBuffID(0), iAuraID(0)
        , iReactionType(0), fReactionDist(0.0f), fReactionHeight(0.0f)
        , fReactionSpeed(0.0f), iReactionArrow(0), fReactionHeightAir(0.0f)
        , fReactionAngle(0.0f), fSlowRate(0.0f), fSlowTime(0.0f)
        , fSlowDelayTime(0.0f), fHitFreezeTime(0.0f), bCheckCounter(false)
        , _pad1{0, 0, 0}, iConditionBuffID(0), fConditionBuffDamageRate(0.0f)
        , bIgnoreTargetInvincible(false), _pad2{0, 0, 0} {}
};

// tagHIT_EFFECT - 命中特效信息 (48 bytes)
// PDB LF_FIELDLIST 0x67F88: Size 48, LF_STRUCTURE 0x67F89
struct tagHIT_EFFECT {
    bool bTraceFXView;                // offset 0
    float fTraceFXRotation;           // offset 4
    std::int32_t iBattleMaterialID;   // offset 8
    float fVictimMoveDist;            // offset 12
    std::int32_t iVictimShakingValue; // offset 16
    float fVictimShakingTime;         // offset 20
    bool bHitShaderView;              // offset 24
    std::int32_t iHitShaderR;         // offset 28
    std::int32_t iHitShaderG;         // offset 32
    std::int32_t iHitShaderB;         // offset 36
    float fHitShaderStrength;         // offset 40
    float fHitShaderLifeTime;         // offset 44

    tagHIT_EFFECT()
        : bTraceFXView(false), fTraceFXRotation(0.0f), iBattleMaterialID(0)
        , fVictimMoveDist(0.0f), iVictimShakingValue(0), fVictimShakingTime(0.0f)
        , bHitShaderView(false), iHitShaderR(0), iHitShaderG(0), iHitShaderB(0)
        , fHitShaderStrength(0.0f), fHitShaderLifeTime(0.0f) {}
};

// tagGRAP_INFO - 抓取信息结构 (164 bytes)
// PDB LF_FIELDLIST 0x68224: Size 164, LF_STRUCTURE 0x68225
struct tagGRAP_INFO {
    char strAttachBoneName[128];      // offset 0
    hkvVec3 vecOffset;                // offset 128
    hkvVec3 vecDropPos;               // offset 140
    float qYaw;                       // offset 152
    float qPitch;                     // offset 156
    float qRoll;                      // offset 160

    tagGRAP_INFO() : vecOffset(), vecDropPos(), qYaw(0.0f), qPitch(0.0f), qRoll(0.0f) {
        std::memset(strAttachBoneName, 0, sizeof(strAttachBoneName));
    }
};

// tagPROJECTILE_INFO - 投射物信息结构 (1096 bytes)
// PDB LF_FIELDLIST 0x75C2F: Size 1096, LF_STRUCTURE 0x75C30
struct tagPROJECTILE_INFO {
    hkvVec3 vStartPos;                // offset 0
    hkvVec3 vDir;                     // offset 12
    float fMoveSpeed;                 // offset 24
    float fLifeTime;                  // offset 28
    char szProjParticle[128];         // offset 32
    char szProjDisapearParticle[128]; // offset 160
    char szProjExplodeParticle[128];  // offset 288
    bool bIsGravity;                  // offset 416
    std::int32_t eDirectionType;      // offset 420
    bool bIsCollision;                // offset 424
    bool bIsMovingToTarget;           // offset 425
    std::int16_t RandomPattern;       // offset 426
    float fRandomRadius;              // offset 428
    float fRandomRadiusMin;           // offset 432
    float fRandomAngle;               // offset 436
    hkvVec3 vRandomPos;               // offset 440
    std::int32_t nRandomCount;        // offset 452
    float fRandomDelayTime;           // offset 456
    std::int32_t nLinkedSkillID;      // offset 460
    float fRadius;                    // offset 464
    bool bIsContinousHit;             // offset 468
    float fContinousTime;             // offset 472
    float fDelayTime;                 // offset 476
    bool bIsTargetGuided;             // offset 480
    float fTargetUpdateTime;          // offset 484
    float fTargetUpdateAngle;         // offset 488
    std::int16_t shAttachToTarget;    // offset 492
    bool bUseExplodeTrap;             // offset 494
    bool bUseSubWeapon;               // offset 495
    bool bTargetOnly;                 // offset 496
    std::int16_t eRotationType;       // offset 498
    float fRotationSpeed;             // offset 500
    bool bMeshRotation;               // offset 504
    bool bEffectRotation;             // offset 505
    std::int16_t shExplodeDecision;   // offset 506
    char szProjExplodeSound[128];     // offset 508
    char szProjAttachSound[128];      // offset 636
    char szProjStartSound[128];       // offset 764
    char szProjPathAnimName[128];     // offset 892
    float fCreateDelayTime;           // offset 1020
    tagATTACK_RANGE sCollisionRange;  // offset 1024
    float fExplodeDelayTime;          // offset 1060
    bool bPenetrate;                  // offset 1064
    std::int32_t iPenetrateCount;     // offset 1068
    std::int16_t shExplodeFaction;    // offset 1072
    std::int16_t shExplodeTrapMakeType; // offset 1074
    bool bApplyEffectWhenPenetrate;   // offset 1076
    std::int16_t shRandomShootDirX;   // offset 1078
    std::int16_t shRandomShootDirY;   // offset 1080
    std::int32_t iTargetPriorityType; // offset 1084
    float fTargetFindTime;            // offset 1088
    bool bApplyExplodeOrientation;    // offset 1092
    bool bApplyDisappearOrientation;  // offset 1093
    bool bEffectShowAlways;           // offset 1094

    tagPROJECTILE_INFO()
        : vStartPos(), vDir(), fMoveSpeed(0.0f), fLifeTime(0.0f)
        , bIsGravity(false), eDirectionType(0), bIsCollision(false)
        , bIsMovingToTarget(false), RandomPattern(0), fRandomRadius(0.0f)
        , fRandomRadiusMin(0.0f), fRandomAngle(0.0f), vRandomPos()
        , nRandomCount(0), fRandomDelayTime(0.0f), nLinkedSkillID(0)
        , fRadius(0.0f), bIsContinousHit(false), fContinousTime(0.0f)
        , fDelayTime(0.0f), bIsTargetGuided(false), fTargetUpdateTime(0.0f)
        , fTargetUpdateAngle(0.0f), shAttachToTarget(0), bUseExplodeTrap(false)
        , bUseSubWeapon(false), bTargetOnly(false), eRotationType(0)
        , fRotationSpeed(0.0f), bMeshRotation(false), bEffectRotation(false)
        , shExplodeDecision(0), fCreateDelayTime(0.0f), sCollisionRange()
        , fExplodeDelayTime(0.0f), bPenetrate(false), iPenetrateCount(0)
        , shExplodeFaction(0), shExplodeTrapMakeType(0)
        , bApplyEffectWhenPenetrate(false), shRandomShootDirX(0)
        , shRandomShootDirY(0), iTargetPriorityType(0), fTargetFindTime(0.0f)
        , bApplyExplodeOrientation(false), bApplyDisappearOrientation(false)
        , bEffectShowAlways(false) {
        std::memset(szProjParticle, 0, sizeof(szProjParticle));
        std::memset(szProjDisapearParticle, 0, sizeof(szProjDisapearParticle));
        std::memset(szProjExplodeParticle, 0, sizeof(szProjExplodeParticle));
        std::memset(szProjExplodeSound, 0, sizeof(szProjExplodeSound));
        std::memset(szProjAttachSound, 0, sizeof(szProjAttachSound));
        std::memset(szProjStartSound, 0, sizeof(szProjStartSound));
        std::memset(szProjPathAnimName, 0, sizeof(szProjPathAnimName));
    }
};

// tagCONNECTION_INFO - 连接信息结构 (568 bytes)
// PDB LF_FIELDLIST 0x6E7E9: Size 568, LF_STRUCTURE 0x6E7EA
struct tagCONNECTION_INFO {
    std::int32_t iConnectionType;     // offset 0
    float fLifeTime;                  // offset 4
    float fMaxCount;                  // offset 8
    float fDamageMutiple;             // offset 12
    bool bUseLaserEffect;             // offset 16
    bool bRecalcTarget;               // offset 17
    bool bInvisibleWhenNoTarget;      // offset 18
    char strEffect[128];              // offset 19
    char strEffect2[128];             // offset 147
    char strStartBone[128];           // offset 275
    char strEndBone[128];             // offset 403
    tagATTACK_RANGE sAttackRange;     // offset 532

    tagCONNECTION_INFO()
        : iConnectionType(0), fLifeTime(0.0f), fMaxCount(0.0f)
        , fDamageMutiple(0.0f), bUseLaserEffect(false), bRecalcTarget(false)
        , bInvisibleWhenNoTarget(false), sAttackRange() {
        std::memset(strEffect, 0, sizeof(strEffect));
        std::memset(strEffect2, 0, sizeof(strEffect2));
        std::memset(strStartBone, 0, sizeof(strStartBone));
        std::memset(strEndBone, 0, sizeof(strEndBone));
    }
};

// ActionTrigger 已在 VisionEngineTypes.h 中完整定义 (168 bytes)

// ============================================================================
// AttackJudgmentTrigger - 攻击判定触发器 (继承自 ActionTrigger)
// 从 IDA 获取: 总大小 2464 bytes (168 + 2296)
// ============================================================================
// tagCONTINUOUS_MELEE_INFO - 连续近战信息 (136 bytes)
// PDB LF_FIELDLIST 0x6B176: Size 136, LF_STRUCTURE 0x6B179
struct tagCONTINUOUS_MELEE_INFO {
    float fLifeTime;                    // offset 0
    float fIntervalTime;                // offset 4 - 攻击间隔时间
    char szTraceBoneName[128];          // offset 8

    tagCONTINUOUS_MELEE_INFO() : fLifeTime(0.0f), fIntervalTime(0.0f) {
        std::memset(szTraceBoneName, 0, sizeof(szTraceBoneName));
    }
};

// AttackJudgmentTrigger - 攻击判定触发器 (2464 bytes)
// PDB LF_FIELDLIST 0x485B3: Size 2464, 继承 ActionTrigger(168)
// IDA ctor: ??0AttackJudgmentTrigger@@QEAA@XZ @ 0x1406418D0 (Size 2464 = 0x9A0)
//   子结构先默认构造再整体 memset，随后回填 sHitEffect.bHitShaderView=1、sReactionInfo.bCheckCounter=1
struct AttackJudgmentTrigger : public ActionTrigger {
    std::int16_t sAttackRangeType;        // offset 168
    std::int16_t sAttackType;             // offset 170
    std::int16_t sAttackCollision;        // offset 172
    std::int32_t iSpawnObjectID;          // offset 176
    tagATTACK_RANGE sAttackRange;         // offset 180 (36 bytes)
    tagPROJECTILE_INFO sProjInfo;         // offset 216 (1096 bytes)
    tagHIT_EFFECT sHitEffect;             // offset 1312 (48 bytes)
    tagREACTION_INFO sReactionInfo;       // offset 1360 (88 bytes)
    tagCONNECTION_INFO sConnectionInfo;   // offset 1448 (568 bytes)
    tagGRAP_INFO sGrapInfo;               // offset 2016 (164 bytes)
    tagCONTINUOUS_MELEE_INFO sContinuousMeleeInfo; // offset 2180 (136 bytes)
    std::int32_t iChargeLevel;            // offset 2316
    std::int32_t iSkillLevel;             // offset 2320
    std::int16_t sSkillCondition;         // offset 2324
    std::int32_t iCombatType;             // offset 2328
    char szDivergenceValue[128];          // offset 2332
    std::int16_t sWeakAttackStiffenRatio; // offset 2460
    std::int16_t shGroupID;               // offset 2462

    AttackJudgmentTrigger()
        : ActionTrigger()
        , sAttackRangeType(0), sAttackType(0), sAttackCollision(1)
        , iSpawnObjectID(0), sAttackRange(), sProjInfo(), sHitEffect()
        , sReactionInfo(), sConnectionInfo(), sGrapInfo(), sContinuousMeleeInfo()
        , iChargeLevel(-1), iSkillLevel(-1), sSkillCondition(0), iCombatType(-1)
        , sWeakAttackStiffenRatio(0), shGroupID(-1)
    {
        std::memset(&sAttackRange, 0, sizeof(sAttackRange));
        std::memset(&sProjInfo, 0, sizeof(sProjInfo));
        std::memset(&sHitEffect, 0, sizeof(sHitEffect));
        std::memset(&sReactionInfo, 0, sizeof(sReactionInfo));
        std::memset(&sConnectionInfo, 0, sizeof(sConnectionInfo));
        std::memset(&sGrapInfo, 0, sizeof(sGrapInfo));
        std::memset(&sContinuousMeleeInfo, 0, sizeof(sContinuousMeleeInfo));
        std::memset(szDivergenceValue, 0, sizeof(szDivergenceValue));
        sHitEffect.bHitShaderView = true;
        sReactionInfo.bCheckCounter = true;
    }
};

// SGroupID - 分组过滤数据结构
// 用于 ActionDestToEntity 中的随机触发器选择
struct SGroupID {
    std::int16_t iGroupID = 0;      // 分组 ID
    std::int32_t iTotalCount = 0;   // 该分组中触发器的总数
    std::int32_t iRandomIndex = 0;  // 随机选择的索引 (0 到 iTotalCount-1)
    std::int32_t iCurIndex = -1;    // 当前遍历索引
};

// XActionResMgr - 动作资源管理器
// IDA 构造函数: 0x140003660
// IDA 析构函数: 0x140003770
// IDA Clear: 0x1400099d0
// IDA LoadAll: 0x140008ef0
// 继承自 VActionResourceManager
class XActionResMgr : public VActionResourceManager {
public:
    // 构造/析构
    // IDA 0x140003660
    XActionResMgr();
    // IDA 0x140003770
    virtual ~XActionResMgr();

    // 核心方法
    // IDA 0x140003440 - 初始化资源管理器
    bool Initialize();

    // IDA 0x1400099d0 - 清理所有资源
    void Clear();

    // IDA 0x140008ef0 - 加载所有动作资源
    void LoadAll();

    // IDA 0x140003810 - 加载基础动画
    void LoadBaseAnimation(VActionResourceLump* pActionRes, bool bPlayer);

    // 动画注册
    // IDA 0x14000c250 - 注册动画信息
    bool RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, bool bBattlePose);

    // 技能攻击触发器注册
    // IDA 0x14000ce70 - 遍历 TB_SKILL 表，注册攻击触发器
    void RegisterSkillAttackTrigger(VActionResourceLump* pActionRes, std::int8_t byClassID);

    // 获取控制类型
    // IDA 0x14000C790 - 从技能表获取控制类型
    void GetControlTypes(TB_SKILL* pSkillTable, std::vector<unsigned char>& vecControlType);

    // 获取技能动画名称列表
    // IDA 0x14000C890 - 获取技能相关的动画名称
    void GetSkillAnimNames(TB_SKILL* pSkillTable, std::vector<std::string>& vecAnimName);

    // 动画加载辅助函数 (从 LoadAll 调用)
    // IDA 0x140004e60
    void LoadCharacterAnimation(VActionResourceLump* pActionRes, TB_CHARACTER_INFO* pCharInfo);
    // IDA 0x140007020
    void LoadMonsterAnimation(VActionResourceLump* pActionRes, TB_MONSTER* pMobRef);
    // IDA 0x140008aa0
    void LoadNpcAnimation(VActionResourceLump* pActionRes, TB_NPC* pNpcRef);
    // IDA 0x140008c70
    void LoadAkashicAnimation(VActionResourceLump* pActionRes, TB_AKASHIC_RECORDS* pTableRef);
    // IDA 0x140008cc0
    void LoadExtraAnimation();

    // XML 加载函数
    // IDA 0x14000bbf0
    tagHIT_COLLISION_DATA* LoadHitCollisionFromXML(const char* szFilePath);
    // IDA 0x14000bf70
    tagHIT_TRACE_BONE_NAME_DATA* LoadTraceBoneNameFromXML(const char* szFilePath);

    // 文件加载/保存函数
    // IDA 0x14000d120 - 从 XML 文件加载
    bool LoadFromFile(const char* szFilePath);
    // IDA 0x14000d200 - 保存到 XML 文件
    bool SaveToFile(const char* szFilePath);

    // 动作描述获取
    // IDA 0x14000a0c0 - 获取动画信息
    const VAnimationInfo* GetActionDesc(VActionResourceLump* pActionResource, const char* pszAniName);

    // 事件检索
    // IDA 0x14000a180 - 静态函数，检索动作触发器
    static ActionTrigger* RetrieveEvent(std::int16_t actionCode, int iActionIdx, const VAnimationInfo* pActionInfo);

    // 动画回调
    // IDA 0x14000a230 - 更改动作回调
    void ChangeMotionCallback(CMover* pMover, const VAnimationInfo* pInfo);

    // 动作目标到实体
    // IDA 0x14000a280 - 将动作数据应用到实体
    void ActionDestToEntity(CMover* pMover, const VAnimationInfo* pInfo);

    // 分组过滤数据生成
    // IDA 0x14000B6D0 - 创建分组过滤数据
    void MakeGroupFilteringData(CMover* pMover, const VAnimationInfo* pInfo, std::map<int, SGroupID>& mapGroup);

    // 设置碰撞数据到 Actor
    // IDA 0x14000b9b0
    void SetHitCollisionDataToActor(const char* szCodeName, CMover* pMover);

    // 设置骨骼追踪数据到 Actor
    // IDA 0x14000bab0
    void SetTraceBoneNameDataToActor(const char* szCodeName, CMover* pMover);

    // 动画索引获取 (两个重载版本)
    // IDA 0x14000c170 - 通过表ID和动画名查询索引
    std::int32_t GetAnimIndex(std::int32_t dwTableID, const VString& strAnimName);
    // IDA 0x140368a30 - 通过动作类型计算索引 (静态函数)
    static std::int32_t GetAnimIndex(std::int16_t nMotionClass, std::int16_t nSubClass, bool bBattlePose);

    // 设置动画信息到 Actor
    // IDA 0x14000d360
    bool SetAnimInfoToActor(std::uint32_t dwTableID, CMover* pMover);

    // 检查触发器ID是否正确
    // IDA 0x14000d470
    bool IsCorrectTriggerID(std::uint32_t iSkillID, std::uint32_t iEventID);

    // 资源加载 (继承自 VActionResourceManager)
    VManagedResource* Load(const char* szFilePath);
    void RemoveAllResourceLump();

    // 访问器
    VActionResourceLump* GetActionResource() const { return m_pActionResource; }
    VActionResourceLump* GetCommonSkillBoneRes() const { return m_pCommonSkillBoneRes; }

    // IDA 0x1402C8020 - 获取通用技能骨骼资源
    VActionResourceLump* GetCommonBoneRes();

    // 资源获取函数
    // IDA 0x14000d070 - 获取怪物动作资源
    VActionResourceLump* GetMonsterAction(std::uint32_t dwMonsterID);
    // IDA 0x14000d0a0 - 获取 NPC 动作资源
    VActionResourceLump* GetNpcAction(std::uint32_t dwNpcID);
    // IDA 0x14000d0d0 - 获取 Akashic 动作资源
    VActionResourceLump* GetAkashicAction(std::uint32_t dwAkashicID);
    // IDA 0x14000d100 - 获取技能动作资源
    VActionResourceLump* GetSkillAction(std::uint32_t dwSkillID);

    // === Round 7 Phase 1-2 - Resource Loading Functions ===
    VActionResourceLump* LoadAction(std::uint32_t dwActionID);
    void UnloadAction(std::uint32_t dwActionID);
    bool ReloadAction(std::uint32_t dwActionID);
    void UnloadAll();

    // === Round 7 Phase 1-2 - Resource Query Functions ===
    VActionResourceLump* GetAction(std::uint32_t dwActionID);
    bool HasAction(std::uint32_t dwActionID);
    std::size_t GetActionCount() const;
    void GetActionList(std::vector<std::uint32_t>& vecActionIDs);
    std::int32_t FindAction(const char* szActionName);

    // === Round 7 Phase 1-2 - Resource Management Functions ===
    void Cache(std::uint32_t dwActionID);
    void SetCacheSize(std::size_t nMaxSize);
    std::size_t GetCacheSize() const;
    void Optimize();

    // === Round 7 Phase 1-2 - Event Handler Functions ===
    void OnLoad(std::uint32_t dwActionID);
    void OnUnload(std::uint32_t dwActionID);
    void OnError(std::uint32_t dwActionID, const char* szError);
    void RegisterHandler(int nEventType, void* pHandler);
    void UnregisterHandler(int nEventType, void* pHandler);

private:
    // === IDA 确认的成员变量 (从 Clear 和 LoadAll 反编译) ===

    // offset 0x8 (继承自 VActionResourceManager 后的起始): m_dwTableID
    // 当前处理的表 ID (角色/怪物/NPC/Akashic 的 ID)
    // 从 LoadCharacterAnimation/LoadMonsterAnimation/LoadNpcAnimation/LoadAkashicAnimation 设置
    // LoadExtraAnimation 结束时重置为 -1
    std::int32_t m_dwTableID;

    // offset 0x10: m_mapHitCollisionInfo (std::map<VString, tagHIT_COLLISION_DATA*>, 48 bytes)
    // 存储 Hit Collision 数据，键为资源名称 (如 "SW_Erwin")
    std::map<VString, tagHIT_COLLISION_DATA*> m_mapHitCollisionInfo;

    // offset 56: m_mapTraceBoneName (std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*>, 48 bytes)
    // 存储 Trace Bone Name 数据，键为资源名称
    std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*> m_mapTraceBoneName;

    // offset 104: m_mapAnimInfoKey (std::map<int, std::map<VString, std::uint32_t>*>, 48 bytes)
    // 动画信息映射 (Key 版本)
    std::map<std::int32_t, std::map<VString, std::uint32_t>*> m_mapAnimInfoKey;

    // offset 152: m_mapAnimInfoString (std::map<int, std::map<std::uint32_t, VString>*>, 48 bytes)
    // 动画信息映射 (String 版本)
    std::map<std::int32_t, std::map<std::uint32_t, VString>*> m_mapAnimInfoString;

    // offset 200: m_mapSkillAttackTrigger (std::map<int, std::set<unsigned long>*>, 48 bytes)
    // 技能攻击触发器映射
    std::map<std::int32_t, std::set<unsigned long>*> m_mapSkillAttackTrigger;

    // offset 248: m_pActionResource (VActionResourceLump*, 8 bytes)
    // 当前动作资源块
    VActionResourceLump* m_pActionResource;

    // offset 256: m_pCommonSkillBoneRes (VActionResourceLump*, 8 bytes)
    // 通用技能骨骼资源 (从 LoadAll 最后加载)
    VActionResourceLump* m_pCommonSkillBoneRes;

    // === Round 7 Phase 1-2 - Extended Members ===
    std::size_t m_nMaxCacheSize = 1000;
    
    // Event handler types
    typedef void (*ActionLoadHandler)(std::uint32_t);
    typedef void (*ActionUnloadHandler)(std::uint32_t);
    typedef void (*ActionErrorHandler)(std::uint32_t, const char*);
    
    // Event handler lists
    std::vector<ActionLoadHandler> m_vecLoadHandlers;
    std::vector<ActionUnloadHandler> m_vecUnloadHandlers;
    std::vector<ActionErrorHandler> m_vecErrorHandlers;

    // Total size: 264 bytes (verified from IDA)
};
