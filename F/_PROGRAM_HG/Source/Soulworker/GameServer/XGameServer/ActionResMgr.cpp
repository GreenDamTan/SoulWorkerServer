// ActionResMgr.cpp - XActionResMgr 实现
// 从 IDA 反编译还原: GameServer.exe
// 构造函数: 0x140003660
// 析构函数: 0x140003770
// Clear: 0x1400099d0
// LoadAll: 0x140008ef0

#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstring>
#include <cstdio>

// 包含表结构体定义
#define GREENDAMTAN_TB_STRUCT_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RECORDS.h"
#undef GREENDAMTAN_TB_STRUCT_SECTION

// ============================================================================
// XActionResMgr::XActionResMgr 构造函数
// IDA 0x140003660
// 还原自 IDA 反编译:
//   VActionResourceManager::VActionResourceManager(this);
//   this->__vftable = (XActionResMgr_vtbl *)&XActionResMgr::`vftable';
//   std::map<...>::map<...>(&this->m_mapHitCollisionInfo);
//   std::map<...>::map<...>(&this->m_mapTraceBoneName);
//   std::map<...>::map<...>(&this->m_mapAnimInfoKey);
//   std::map<...>::map<...>(&this->m_mapAnimInfoString);
//   std::map<...>::map<...>(&this->m_mapSkillAttackTrigger);
//   this->m_pActionResource = nullptr;
// ============================================================================
XActionResMgr::XActionResMgr()
    : VActionResourceManager()
    , m_mapHitCollisionInfo()
    , m_mapTraceBoneName()
    , m_mapAnimInfoKey()
    , m_mapAnimInfoString()
    , m_mapSkillAttackTrigger()
    , m_pActionResource(nullptr)
    , m_pCommonSkillBoneRes(nullptr)
{
    // std::map 默认构造函数会自动调用，无需显式初始化
    // m_pActionResource 和 m_pCommonSkillBoneRes 已在初始化列表中设置为 nullptr
}

// ============================================================================
// XActionResMgr::~XActionResMgr 析构函数
// IDA 0x140003770
// 还原自 IDA 反编译:
//   this->__vftable = (XActionResMgr_vtbl *)&XActionResMgr::`vftable';
//   std::map<int,VCommonPositionBoxInfo *>::~map(&this->m_mapSkillAttackTrigger);
//   std::map<int,VCommonPositionBoxInfo *>::~map(&this->m_mapAnimInfoString);
//   std::map<int,VCommonPositionBoxInfo *>::~map(&this->m_mapAnimInfoKey);
//   std::map<VString,tagHIT_TRACE_BONE_NAME_DATA *>::~map(&this->m_mapTraceBoneName);
//   std::map<VString,tagHIT_TRACE_BONE_NAME_DATA *>::~map(&this->m_mapHitCollisionInfo);
//   VActionResourceManager::~VActionResourceManager(this);
// ============================================================================
XActionResMgr::~XActionResMgr()
{
    // 调用 Clear 清理所有资源
    Clear();

    // 父类析构函数会自动调用
}

// ============================================================================
// XActionResMgr::Clear
// IDA 0x1400099d0
// 清理所有资源映射和指针
// 还原自 IDA 反编译的详细逻辑:
//   1. 调用 RemoveAllResourceLump()
//   2. 遍历 m_mapHitCollisionInfo，删除每个 tagHIT_COLLISION_DATA*
//   3. 遍历 m_mapTraceBoneName，删除每个 tagHIT_TRACE_BONE_NAME_DATA*
//   4. 遍历 m_mapAnimInfoKey，删除每个 std::map<VString, unsigned long>*
//   5. 遍历 m_mapAnimInfoString，删除每个 std::map<unsigned long, VString>*
//   6. 遍历 m_mapSkillAttackTrigger，删除每个 std::set<unsigned long>*
// ============================================================================
void XActionResMgr::Clear()
{
    // 清理所有资源块
    RemoveAllResourceLump();

    // 清理 m_mapHitCollisionInfo
    for (auto it = m_mapHitCollisionInfo.begin(); it != m_mapHitCollisionInfo.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapHitCollisionInfo.clear();

    // 清理 m_mapTraceBoneName
    for (auto it = m_mapTraceBoneName.begin(); it != m_mapTraceBoneName.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapTraceBoneName.clear();

    // 清理 m_mapAnimInfoKey
    for (auto it = m_mapAnimInfoKey.begin(); it != m_mapAnimInfoKey.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapAnimInfoKey.clear();

    // 清理 m_mapAnimInfoString
    for (auto it = m_mapAnimInfoString.begin(); it != m_mapAnimInfoString.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapAnimInfoString.clear();

    // 清理 m_mapSkillAttackTrigger
    for (auto it = m_mapSkillAttackTrigger.begin(); it != m_mapSkillAttackTrigger.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapSkillAttackTrigger.clear();
}

// ============================================================================
// XActionResMgr::LoadAll
// IDA 0x140008ef0
// 加载所有动作资源
// 还原自 IDA 反编译的详细逻辑:
//   1. 遍历 TB_CHARACTER_INFO 表，加载角色动画资源
//   2. 遍历 TB_MONSTER 表，加载怪物动画资源
//   3. 遍历 TB_NPC 表，加载 NPC 动画资源
//   4. 遍历 TB_AKASHIC_RECORDS 表，加载 Akashic 动画资源
//   5. 加载通用技能骨骼资源 (Monster ID 0x9896E9 = 9999999)
// ============================================================================
void XActionResMgr::LoadAll()
{
    // TODO: 需要以下依赖才能完整实现:
    // 1. XGameServer::Instance() 单例访问
    // 2. XResourceMgr::GetTB_CHARACTER_INFO() 等表访问器
    // 3. TB_CHARACTER_INFO, TB_MONSTER, TB_NPC, TB_AKASHIC_RECORDS 结构定义
    // 4. VActionResourceLump::AddJumpInfo() 方法
    // 5. g_strCurPath_6 全局路径变量

    char szFilePath[260];

    // ========================================
    // 第一部分: 加载角色动画资源
    // 遍历 TB_CHARACTER_INFO 表
    // ========================================
    // TODO: 从 XGameServer 获取 XResourceMgr
    // auto pGameServer = TXSingleton<XGameServer>::Instance();
    // auto& characterTable = pGameServer->m_xResourceMgr.GetTB_CHARACTER_INFO();
    //
    // for (auto iterCharacter = characterTable.begin(); iterCharacter != characterTable.end(); ++iterCharacter) {
    //     const TB_CHARACTER_INFO& charInfo = iterCharacter->second;
    //     const char* pCodeName = /* 从 charInfo 获取 Code_Name */;
    //
    //     if (strlen(pCodeName) > 1) {
    //         // 加载 .adf 文件
    //         sprintf_s(szFilePath, sizeof(szFilePath), "%s.adf", pCodeName);
    //         VActionResourceLump* pActionRes = (VActionResourceLump*)Load(szFilePath);
    //
    //         if (pActionRes && VManagedResource::IsLoaded(pActionRes)) {
    //             // 加载角色动画
    //             LoadCharacterAnimation(pActionRes, &charInfo);
    //
    //             // 添加跳跃信息
    //             sprintf_s(szFilePath, sizeof(szFilePath), "%s_Jump.jdf", pCodeName);
    //             pActionRes->AddJumpInfo(szFilePath);
    //
    //             // 加载 Hit Collision XML
    //             sprintf_s(szFilePath, sizeof(szFilePath), "%s/ActionData/%s.xml", g_strCurPath_6.c_str(), pCodeName);
    //             tagHIT_COLLISION_DATA* pHitCollision = LoadHitCollisionFromXML(szFilePath);
    //             if (pHitCollision) {
    //                 VString strKey(pCodeName);
    //                 m_mapHitCollisionInfo[strKey] = pHitCollision;
    //             }
    //
    //             // 加载 Trace Bone Name XML
    //             tagHIT_TRACE_BONE_NAME_DATA* pTraceBone = LoadTraceBoneNameFromXML(szFilePath);
    //             if (pTraceBone) {
    //                 VString strKey(pCodeName);
    //                 m_mapTraceBoneName[strKey] = pTraceBone;
    //             }
    //         } else {
    //             LogHelper::LogError("game.contents", "[ %s ] Missing Resource Load Fail", szFilePath);
    //         }
    //     }
    // }

    // ========================================
    // 第二部分: 加载怪物动画资源
    // 遍历 TB_MONSTER 表
    // ========================================
    // TODO: 实现 TB_MONSTER 遍历
    // for (auto iterMonster = monsterTable.begin(); iterMonster != monsterTable.end(); ++iterMonster) {
    //     TB_MONSTER* pMobRef = &iterMonster->second;
    //     const char* pCodeName = pMobRef->Monster_Code_Name;
    //
    //     if (strlen(pCodeName) > 1) {
    //         sprintf_s(szFilePath, sizeof(szFilePath), "%s.adf", pCodeName);
    //         VManagedResource* pResource = Load(szFilePath);
    //
    //         if (pResource && VManagedResource::IsLoaded(pResource)) {
    //             LoadMonsterAnimation((VActionResourceLump*)pResource, pMobRef);
    //             // ... 加载 XML ...
    //         }
    //     }
    // }

    // ========================================
    // 第三部分: 加载 NPC 动画资源
    // 遍历 TB_NPC 表
    // ========================================
    // TODO: 实现 TB_NPC 遍历
    // for (auto iterNPC = npcTable.begin(); iterNPC != npcTable.end(); ++iterNPC) {
    //     TB_NPC* pNpcRef = &iterNPC->second;
    //     const char* pCodeName = /* 从 TB_NPC 获取 */;
    //
    //     if (strlen(pCodeName) > 0) {
    //         std::string strFileName(pCodeName);
    //         if (strFileName != "0") {
    //             strFileName += ".adf";
    //             VActionResourceLump* pActionRes = (VActionResourceLump*)Load(strFileName.c_str());
    //             if (pActionRes) {
    //                 LoadNpcAnimation(pActionRes, pNpcRef);
    //             } else {
    //                 LogHelper::LogError("game.contents", "[ %s ] Error Action Resource Load Fail", strFileName.c_str());
    //             }
    //         }
    //     }
    // }

    // ========================================
    // 第四部分: 加载 Akashic 动画资源
    // 遍历 TB_AKASHIC_RECORDS 表
    // ========================================
    // TODO: 实现 TB_AKASHIC_RECORDS 遍历
    // for (auto iterAka = akaTable.begin(); iterAka != akaTable.end(); ++iterAka) {
    //     TB_AKASHIC_RECORDS* pTableRef = &iterAka->second;
    //
    //     // 只加载特定类型 (Type == 1 || Type == 2)
    //     if (pTableRef->Type == 1 || pTableRef->Type == 2) {
    //         const char* pCodeName = pTableRef->Code_Name;
    //
    //         if (strlen(pCodeName) > 1) {
    //             sprintf_s(szFilePath, sizeof(szFilePath), "%s.adf", pCodeName);
    //             VManagedResource* pResource = Load(szFilePath);
    //
    //             if (pResource && VManagedResource::IsLoaded(pResource)) {
    //                 LoadAkashicAnimation((VActionResourceLump*)pResource, pTableRef);
    //             } else {
    //                 LogHelper::LogError("game.contents", "[ %s ] Missing Resource Load Fail", szFilePath);
    //             }
    //         }
    //     }
    // }

    // ========================================
    // 第五部分: 加载通用技能骨骼资源
    // Monster ID 0x9896E9 = 9999999
    // ========================================
    // TODO: 实现
    // auto pGameServer = TXSingleton<XGameServer>::Instance();
    // TB_MONSTER* pMobRef = XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, 0x9896E9);
    // if (pMobRef) {
    //     sprintf_s(szFilePath, sizeof(szFilePath), "%s.adf", pMobRef->Monster_Code_Name);
    //     m_pCommonSkillBoneRes = (VActionResourceLump*)Load(szFilePath);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadAll - TODO: needs complete dependency types");
}

// ============================================================================
// XActionResMgr::LoadBaseAnimation
// IDA 0x140003810
// 参数:
//   pActionRes - 动画资源块指针
//   bPlayer - 是否为玩家角色 (true=玩家, false=NPC/怪物)
// ============================================================================
void XActionResMgr::LoadBaseAnimation(VActionResourceLump* pActionRes, bool bPlayer)
{
    // IDA 反编译:
    // this->m_pActionResource = pActionRes;
    // 然后调用大量 RegisterAnimInfo 注册动画

    m_pActionResource = pActionRes;

    // 注册通用动画 (Motion Class 0: Spawn)
    RegisterAnimInfo(0, 0, VString("N_Spawn"), 0);

    // 注册通用动画 (Motion Class 2: Idle)
    RegisterAnimInfo(2, 0, VString("N_Idle_01"), 0);

    // 注册 Boss/受击动画 (Type 1)
    RegisterAnimInfo(0, 0, VString("N_Spawn"), 1);
    RegisterAnimInfo(2, 0, VString("B_Idle_01"), 1);

    // 死亡动画 (Motion Class 12-14)
    RegisterAnimInfo(12, 0, VString("B_Death_Stand"), 1);
    RegisterAnimInfo(13, 0, VString("B_Death_Down"), 1);
    RegisterAnimInfo(14, 0, VString("B_Death_Down_Wait"), 1);

    // 受击动画 (Motion Class 15-17)
    RegisterAnimInfo(15, 0, VString("B_DMG_L"), 1);
    RegisterAnimInfo(16, 0, VString("B_DMG_R"), 1);
    RegisterAnimInfo(17, 0, VString("B_KB_Start"), 1);
    RegisterAnimInfo(17, 1, VString("B_KB_End"), 1);

    // 根据 bPlayer 参数注册不同的 KnockDown 动画
    if (bPlayer) {
        // 玩家 KnockDown 动画 (Motion Class 18-21)
        // KnockDown Strength (18)
        RegisterAnimInfo(18, 0, VString("B_KD_Str_Start"), 1);
        RegisterAnimInfo(18, 1, VString("B_KD_Upp_End"), 1);
        RegisterAnimInfo(18, 2, VString("B_KD_Upp_Down"), 1);
        RegisterAnimInfo(18, 3, VString("B_KD_Upp_Down_Hit"), 1);
        RegisterAnimInfo(18, 4, VString("B_KD_Upp_Air_Hit"), 1);
        RegisterAnimInfo(18, 5, VString("B_KD_Upp_Raise"), 1);
        RegisterAnimInfo(18, 6, VString("B_KD_Upp_Loop"), 1);

        // KnockDown Upper (19)
        RegisterAnimInfo(19, 0, VString("B_KD_Upp_Start"), 1);
        RegisterAnimInfo(19, 1, VString("B_KD_Upp_End"), 1);
        RegisterAnimInfo(19, 2, VString("B_KD_Upp_Down"), 1);
        RegisterAnimInfo(19, 3, VString("B_KD_Upp_Down_Hit"), 1);
        RegisterAnimInfo(19, 4, VString("B_KD_Upp_Air_Hit"), 1);
        RegisterAnimInfo(19, 5, VString("B_KD_Upp_Raise"), 1);
        RegisterAnimInfo(19, 6, VString("B_KD_Upp_Loop"), 1);

        // KnockDown Hammer Front (20)
        RegisterAnimInfo(20, 0, VString("B_KD_Ham_F_Start"), 1);
        RegisterAnimInfo(20, 1, VString("B_KD_Upp_End"), 1);
        RegisterAnimInfo(20, 2, VString("B_KD_Upp_Down"), 1);
        RegisterAnimInfo(20, 3, VString("B_KD_Upp_Down_Hit"), 1);
        RegisterAnimInfo(20, 4, VString("B_KD_Upp_Air_Hit"), 1);
        RegisterAnimInfo(20, 5, VString("B_KD_Upp_Raise"), 1);
        RegisterAnimInfo(20, 6, VString("B_KD_Upp_Loop"), 1);

        // KnockDown Hammer Back (21)
        RegisterAnimInfo(21, 0, VString("B_KD_Ham_B_Start"), 1);
        RegisterAnimInfo(21, 1, VString("B_KD_Upp_End"), 1);
        RegisterAnimInfo(21, 2, VString("B_KD_Upp_Down"), 1);
        RegisterAnimInfo(21, 3, VString("B_KD_Upp_Down_Hit"), 1);
        RegisterAnimInfo(21, 4, VString("B_KD_Upp_Air_Hit"), 1);
        RegisterAnimInfo(21, 5, VString("B_KD_Upp_Raise"), 1);
        RegisterAnimInfo(21, 6, VString("B_KD_Upp_Loop"), 1);
    }
    else {
        // NPC/怪物 KnockDown 动画 (Motion Class 18-21)
        // KnockDown Strength (18)
        RegisterAnimInfo(18, 0, VString("B_KD_Str_Start"), 1);
        RegisterAnimInfo(18, 1, VString("B_KD_Str_End"), 1);
        RegisterAnimInfo(18, 2, VString("B_KD_Str_Down"), 1);
        RegisterAnimInfo(18, 3, VString("B_KD_Str_Down_Hit"), 1);
        RegisterAnimInfo(18, 4, VString("B_KD_Str_Air_Hit"), 1);
        RegisterAnimInfo(18, 5, VString("B_KD_Str_Raise"), 1);
        RegisterAnimInfo(18, 6, VString("B_KD_Str_Loop"), 1);

        // KnockDown Upper (19)
        RegisterAnimInfo(19, 0, VString("B_KD_Upp_Start"), 1);
        RegisterAnimInfo(19, 1, VString("B_KD_Upp_End"), 1);
        RegisterAnimInfo(19, 2, VString("B_KD_Upp_Down"), 1);
        RegisterAnimInfo(19, 3, VString("B_KD_Upp_Down_Hit"), 1);
        RegisterAnimInfo(19, 4, VString("B_KD_Upp_Air_Hit"), 1);
        RegisterAnimInfo(19, 5, VString("B_KD_Upp_Raise"), 1);
        RegisterAnimInfo(19, 6, VString("B_KD_Upp_Loop"), 1);

        // KnockDown Hammer Front (20)
        RegisterAnimInfo(20, 0, VString("B_KD_Ham_F_Start"), 1);
        RegisterAnimInfo(20, 1, VString("B_KD_Ham_F_End"), 1);
        RegisterAnimInfo(20, 2, VString("B_KD_Ham_F_Down"), 1);
        RegisterAnimInfo(20, 3, VString("B_KD_Ham_F_Down_Hit"), 1);
        RegisterAnimInfo(20, 4, VString("B_KD_Ham_F_Air_Hit"), 1);
        RegisterAnimInfo(20, 5, VString("B_KD_Ham_F_Raise"), 1);
        RegisterAnimInfo(20, 6, VString("B_KD_Ham_F_Loop"), 1);

        // KnockDown Hammer Back (21)
        RegisterAnimInfo(21, 0, VString("B_KD_Ham_B_Start"), 1);
        RegisterAnimInfo(21, 1, VString("B_KD_Ham_B_End"), 1);
        RegisterAnimInfo(21, 2, VString("B_KD_Ham_B_Down"), 1);
        RegisterAnimInfo(21, 3, VString("B_KD_Ham_B_Down_Hit"), 1);
        RegisterAnimInfo(21, 4, VString("B_KD_Ham_B_Air_Hit"), 1);
        RegisterAnimInfo(21, 5, VString("B_KD_Ham_B_Raise"), 1);
        RegisterAnimInfo(21, 6, VString("B_KD_Ham_B_Loop"), 1);
    }

    // Stun 动画 (Motion Class 22)
    RegisterAnimInfo(22, 0, VString("B_Stun"), 1);

    // Jump Air Hit 动画 (Motion Class 23)
    RegisterAnimInfo(23, 0, VString("B_Jump_Air_Hit"), 1);
}

// ============================================================================
// XActionResMgr::RegisterAnimInfo
// 注册动画信息到管理器
// 参数:
//   nMotionClass - 动作类型 (0=Spawn, 2=Idle, 12-23=各种受击/死亡)
//   nSubClass - 子类型 (用于同一动作的不同阶段)
//   strAnimName - 动画名称
//   nType - 类型 (0=普通, 1=Boss/受击)
// 返回:
//   true 如果注册成功, false 如果动画已存在
// TODO [INCOMPLETE]:
//   - 完整实现需要从 IDA 还原 RegisterAnimInfo 函数
//   - 需要分析 m_mapAnimInfoKey 和 m_mapAnimInfoString 的存储逻辑
//   - 参考 IDA 0x140367AE0 (CMover::IsRegisterAnimInfo) 相关函数
// ============================================================================
bool XActionResMgr::RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, std::int16_t nType)
{
    // 计算动画 Key (MotionClass * 100 + SubClass + Type * 10000)
    std::int32_t nAnimKey = nMotionClass * 100 + nSubClass + nType * 10000;

    // TODO: 完整实现需要存储到 m_mapAnimInfoKey 和 m_mapAnimInfoString
    // 当前仅记录动画名称用于调试

    // 使用 LogHelper 输出调试信息
    LogHelper::LogDebug("ActionResMgr", "RegisterAnimInfo: MotionClass=%d, SubClass=%d, AnimName=%s, Type=%d, Key=%d",
                        nMotionClass, nSubClass, strAnimName.AsChar(), nType, nAnimKey);

    // 返回 true 表示成功 (临时)
    return true;
}

// ============================================================================
// XActionResMgr::LoadCharacterAnimation
// IDA 0x140004e60
// 加载角色动画
// 参数:
//   pActionRes - 动画资源块
//   pCharInfo - 角色信息表记录
// 还原自 IDA 反编译 (0x140004e60 - 0x140007019)
// ============================================================================
void XActionResMgr::LoadCharacterAnimation(VActionResourceLump* pActionRes, TB_CHARACTER_INFO* pCharInfo)
{
    // 保存当前处理的表 ID
    m_dwTableID = pCharInfo->ID;

    // 加载基础动画 (玩家角色 bPlayer=true)
    LoadBaseAnimation(pActionRes, true);

    // 注册技能攻击触发器
    // TODO [DEPENDENCY]: 需要 RegisterSkillAttackTrigger 函数实现
    // RegisterSkillAttackTrigger(pActionRes, pCharInfo->Character_ID);

    // ========================================
    // 注册角色特定动画
    // ========================================

    // Stand 动画 (Motion Class 1)
    RegisterAnimInfo(1, 0, VString("B_Stand"), 1);
    RegisterAnimInfo(1, 0, VString("N_Stand"), 0);

    // Walk 动画 (Motion Class 3)
    RegisterAnimInfo(3, 0, VString("N_Walk_F"), 0);
    RegisterAnimInfo(3, 1, VString("N_Walk_L"), 0);
    RegisterAnimInfo(3, 2, VString("N_Walk_R"), 0);
    RegisterAnimInfo(3, 3, VString("N_Walk_B"), 0);

    // Run 动画 (Motion Class 5)
    RegisterAnimInfo(5, 0, VString("N_Run_F"), 0);
    RegisterAnimInfo(5, 1, VString("N_Run_L"), 0);
    RegisterAnimInfo(5, 2, VString("N_Run_R"), 0);
    RegisterAnimInfo(5, 3, VString("N_Run_B"), 0);

    // Run End 动画 (Motion Class 6)
    RegisterAnimInfo(6, 0, VString("N_Run_F_End"), 0);

    // Dash 动画 (Motion Class 32-34)
    RegisterAnimInfo(32, 0, VString("N_Dash_Start"), 0);
    RegisterAnimInfo(33, 0, VString("N_Dash_Loop"), 0);
    RegisterAnimInfo(34, 0, VString("N_Dash_End"), 0);

    // Boss Walk 动画 (Motion Class 3, Type 1)
    RegisterAnimInfo(3, 0, VString("B_Walk_F"), 1);
    RegisterAnimInfo(3, 1, VString("B_Walk_L"), 1);
    RegisterAnimInfo(3, 2, VString("B_Walk_R"), 1);
    RegisterAnimInfo(3, 3, VString("B_Walk_B"), 1);

    // Boss Run 动画 (Motion Class 5, Type 1)
    RegisterAnimInfo(5, 0, VString("B_Run_F"), 1);
    RegisterAnimInfo(5, 1, VString("B_Run_L"), 1);
    RegisterAnimInfo(5, 2, VString("B_Run_R"), 1);
    RegisterAnimInfo(5, 3, VString("B_Run_B"), 1);
    RegisterAnimInfo(6, 0, VString("B_Run_F_End"), 1);

    // Jump 动画 (Motion Class 9-11)
    // Boss Jump Start (所有方向使用相同动画)
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(9, i, VString("B_Jump_Start_F"), 1);
    }
    // Boss Jump Loop
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(10, i, VString("B_Jump_Loop_F"), 1);
    }
    // Boss Jump Land
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(11, i, VString("B_Jump_Land_C"), 1);
    }
    // Normal Jump Start
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(9, i, VString("N_Jump_Start_F"), 0);
    }
    // Normal Jump Loop
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(10, i, VString("N_Jump_Loop_F"), 0);
    }
    // Normal Jump Land
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(11, i, VString("N_Jump_Land_C"), 0);
    }

    // Mode 切换动画 (Motion Class 27-28)
    RegisterAnimInfo(27, 0, VString("B_N_Mode"), 1);
    RegisterAnimInfo(27, 0, VString("B_N_Mode"), 0);
    RegisterAnimInfo(28, 0, VString("N_B_Mode"), 1);
    RegisterAnimInfo(28, 0, VString("N_B_Mode"), 0);

    // Evade 动画 (Motion Class 30)
    RegisterAnimInfo(30, 0, VString("B_Evade_F"), 1);
    RegisterAnimInfo(30, 1, VString("B_Evade_L"), 1);
    RegisterAnimInfo(30, 2, VString("B_Evade_R"), 1);
    RegisterAnimInfo(30, 3, VString("B_Evade_B"), 1);

    // Walk Lower 动画 (Motion Class 31)
    RegisterAnimInfo(31, 0, VString("B_Walk_Lower_F"), 1);
    RegisterAnimInfo(31, 1, VString("B_Walk_Lower_L"), 1);
    RegisterAnimInfo(31, 2, VString("B_Walk_Lower_R"), 1);
    RegisterAnimInfo(31, 3, VString("B_Walk_Lower_B"), 1);

    // Boss Dash 动画 (Motion Class 32-34)
    RegisterAnimInfo(32, 0, VString("B_Dash_Start"), 1);
    RegisterAnimInfo(33, 0, VString("B_Dash_Loop"), 1);
    RegisterAnimInfo(34, 0, VString("B_Dash_End"), 1);

    // Dash Jump 动画 (Motion Class 35-37)
    RegisterAnimInfo(35, 0, VString("N_Dash_Jump_Start"), 0);
    RegisterAnimInfo(36, 0, VString("N_Dash_Jump_Loop"), 0);
    RegisterAnimInfo(37, 0, VString("N_Dash_Jump_End"), 0);

    RegisterAnimInfo(35, 0, VString("B_Dash_Jump_Start"), 1);
    if (!RegisterAnimInfo(36, 0, VString("B_Dash_Jump_Loop"), 1)) {
        RegisterAnimInfo(36, 0, VString("B_Jump_Loop"), 1);
    }
    if (!RegisterAnimInfo(37, 0, VString("B_Dash_Jump_End"), 1)) {
        RegisterAnimInfo(37, 0, VString("B_Jump_Land_C"), 1);
    }

    // Roll 动画 (Motion Class 38)
    RegisterAnimInfo(38, 0, VString("B_Roll_F"), 1);
    RegisterAnimInfo(38, 1, VString("B_Roll_L"), 1);
    RegisterAnimInfo(38, 2, VString("B_Roll_R"), 1);
    RegisterAnimInfo(38, 3, VString("B_Roll_B"), 1);

    // Looting 动画 (Motion Class 39)
    RegisterAnimInfo(39, 0, VString("B_Looting"), 1);

    // Interaction Object 动画 (Motion Class 40-42)
    RegisterAnimInfo(40, 0, VString("B_Interaction_Object_Start"), 1);
    RegisterAnimInfo(41, 0, VString("B_Interaction_Object_Loop"), 1);
    RegisterAnimInfo(42, 0, VString("B_Interaction_Object_End"), 1);

    // Login Class 动画 (Motion Class 43)
    RegisterAnimInfo(43, 0, VString("Login_Class_N_Stand"), 1);
    RegisterAnimInfo(43, 1, VString("Login_Class_B_Stand"), 1);
    RegisterAnimInfo(43, 2, VString("Login_Class_N_B_Mode"), 1);
    RegisterAnimInfo(43, 3, VString("Login_Class_N_Look_Start"), 1);
    RegisterAnimInfo(43, 4, VString("Login_Class_N_Look_End"), 1);
    RegisterAnimInfo(43, 5, VString("Login_Class_N_Look_Stand"), 1);
    RegisterAnimInfo(43, 6, VString("Login_Class_Select"), 1);
    RegisterAnimInfo(43, 7, VString("Login_Class_Select_Wait"), 1);
    RegisterAnimInfo(43, 8, VString("L_Stand"), 1);
    RegisterAnimInfo(43, 9, VString("L_Stand_Change"), 1);

    // Win 动画 (Motion Class 44-45)
    RegisterAnimInfo(44, 0, VString("B_Win"), 1);
    RegisterAnimInfo(45, 0, VString("B_Win02"), 1);

    // Special Evade 动画 (Motion Class 46)
    RegisterAnimInfo(46, 0, VString("B_Special_Evade_F"), 1);
    RegisterAnimInfo(46, 1, VString("B_Special_Evade_L"), 1);
    RegisterAnimInfo(46, 2, VString("B_Special_Evade_R"), 1);
    RegisterAnimInfo(46, 3, VString("B_Special_Evade_B"), 1);

    // Jump Move 动画 (Motion Class 47)
    for (int i = 0; i < 4; i++) {
        RegisterAnimInfo(47, i, VString("B_Jump_Move_F"), 1);
    }

    // 加载额外动画
    LoadExtraAnimation();
}

// ============================================================================
// XActionResMgr::LoadMonsterAnimation
// IDA 0x140007020
// 加载怪物动画
// 参数:
//   pActionRes - 动画资源块
//   pMobRef - 怪物信息表记录
// 还原自 IDA 反编译 (0x140007020 - 0x140008a91)
// TODO [INCOMPLETE]:
//   - 需要完整定义 TB_MONSTER 结构体字段:
//     - Monster_BattleMode_Type (战斗模式类型: 0=默认, 1=A型, 2=B型)
//     - Monster_Default_Action_Type (默认动作类型)
//     - Monster_Default_Action_Type_01 等 (动作步骤数组)
// ============================================================================
void XActionResMgr::LoadMonsterAnimation(VActionResourceLump* pActionRes, TB_MONSTER* pMobRef)
{
    // 保存当前处理的表 ID
    m_dwTableID = pMobRef->ID;

    // 加载基础动画 (怪物 bPlayer=false)
    LoadBaseAnimation(pActionRes, false);

    // ========================================
    // 注册怪物特定动画
    // ========================================

    // Walk 动画 (Motion Class 3) - 所有方向使用相同动画名
    for (int i = 0; i < 4; i++) {
        RegisterAnimInfo(3, i, VString("N_Walk"), 0);
    }
    for (int i = 0; i < 4; i++) {
        RegisterAnimInfo(3, i, VString("N_Walk"), 1);
    }

    // Gaze 动画 (Motion Class 4)
    RegisterAnimInfo(4, 0, VString("B_Gaze_F"), 1);
    RegisterAnimInfo(4, 1, VString("B_Gaze_L"), 1);
    RegisterAnimInfo(4, 2, VString("B_Gaze_R"), 1);
    RegisterAnimInfo(4, 3, VString("B_Gaze_B"), 1);

    // 根据 Monster_BattleMode_Type 注册不同 Stand 和 Mode 动画
    // TODO [DEPENDENCY]: 需要 TB_MONSTER::Monster_BattleMode_Type 字段
    // int battleModeType = pMobRef->Monster_BattleMode_Type;
    int battleModeType = 0; // 临时默认值

    if (battleModeType == 0) {
        // 默认模式
        RegisterAnimInfo(1, 0, VString("B_Stand"), 1);
        RegisterAnimInfo(1, 0, VString("N_Stand"), 0);
        RegisterAnimInfo(27, 0, VString("B_N_Mode"), 1);
        RegisterAnimInfo(27, 0, VString("B_N_Mode"), 0);
        RegisterAnimInfo(28, 0, VString("N_B_Mode"), 1);
        RegisterAnimInfo(28, 0, VString("N_B_Mode"), 0);
    }
    else if (battleModeType == 1) {
        // A 型战斗模式
        RegisterAnimInfo(1, 0, VString("B_Stand_A"), 1);
        RegisterAnimInfo(1, 0, VString("N_Stand_A"), 0);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_A"), 1);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_A"), 0);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_A"), 1);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_A"), 0);
    }
    else if (battleModeType == 2) {
        // B 型战斗模式
        RegisterAnimInfo(1, 0, VString("B_Stand_B"), 1);
        RegisterAnimInfo(1, 0, VString("N_Stand_B"), 0);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_B"), 1);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_B"), 0);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_B"), 1);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_B"), 0);
    }
    else {
        // 其他类型
        RegisterAnimInfo(1, 0, VString("B_Stand"), 1);
        RegisterAnimInfo(1, 0, VString("N_Stand"), 0);
    }

    // Run 动画 (Motion Class 5) - 带回退逻辑
    if (!RegisterAnimInfo(5, 0, VString("B_Run_F"), 1)) {
        RegisterAnimInfo(5, 0, VString("B_Run"), 1);
    }
    if (!RegisterAnimInfo(5, 1, VString("B_Run_L"), 1)) {
        RegisterAnimInfo(5, 1, VString("B_Run"), 1);
    }
    if (!RegisterAnimInfo(5, 2, VString("B_Run_R"), 1)) {
        RegisterAnimInfo(5, 2, VString("B_Run"), 1);
    }
    if (!RegisterAnimInfo(5, 3, VString("B_Run_B"), 1)) {
        RegisterAnimInfo(5, 3, VString("B_Run"), 1);
    }

    // Normal Run 动画
    if (RegisterAnimInfo(5, 0, VString("N_Run"), 0)) {
        // 如果 N_Run 存在，所有方向使用相同动画
        for (int i = 0; i < 4; i++) {
            RegisterAnimInfo(5, i, VString("N_Run"), 0);
        }
    }
    else {
        // 回退到 B_Run 系列
        if (!RegisterAnimInfo(5, 0, VString("B_Run_F"), 0)) {
            RegisterAnimInfo(5, 0, VString("B_Run"), 0);
        }
        if (!RegisterAnimInfo(5, 1, VString("B_Run_L"), 0)) {
            RegisterAnimInfo(5, 1, VString("B_Run"), 0);
        }
        if (!RegisterAnimInfo(5, 2, VString("B_Run_R"), 0)) {
            RegisterAnimInfo(5, 2, VString("B_Run"), 0);
        }
        if (!RegisterAnimInfo(5, 3, VString("B_Run_B"), 0)) {
            RegisterAnimInfo(5, 3, VString("B_Run"), 0);
        }
    }

    // Turn 动画 (Motion Class 7-8)
    RegisterAnimInfo(7, 0, VString("B_Turn_L"), 1);
    RegisterAnimInfo(8, 0, VString("B_Turn_R"), 1);
    RegisterAnimInfo(7, 1, VString("B_Big_Turn_L"), 1);
    RegisterAnimInfo(8, 1, VString("B_Big_Turn_R"), 1);

    // SABreak 动画 (Motion Class 24-26)
    RegisterAnimInfo(24, 0, VString("B_SABreak_Start"), 1);
    RegisterAnimInfo(25, 0, VString("B_SABreak_Loop"), 1);
    RegisterAnimInfo(26, 0, VString("B_SABreak_End"), 1);

    // Death Flat 动画 (Motion Class 13, SubClass 10)
    RegisterAnimInfo(13, 10, VString("B_Death_Flat"), 1);

    // Damage 动画 (Motion Class 15-16, Type 0)
    RegisterAnimInfo(15, 0, VString("B_DMG_L"), 0);
    RegisterAnimInfo(16, 0, VString("B_DMG_R"), 0);

    // ========================================
    // 根据默认动作步骤注册额外动画
    // TODO [DEPENDENCY]: 需要 TB_MONSTER 中的以下字段:
    //   - Monster_Default_Action_Type (默认动作类型)
    //   - Monster_Default_Action_Type_01, _02, _03 (动作步骤数组)
    // 这些字段用于动态生成 A/B/C/D 型动画名称
    // ========================================
    int iStep[4] = { 0, 0, 0, 0 };
    // TODO: 从 TB_MONSTER 读取默认动作类型
    // unsigned char byDefaultAnimStep = pMobRef->Monster_Default_Action_Type;
    // if (byDefaultAnimStep > 0 && byDefaultAnimStep <= 4) {
    //     iStep[byDefaultAnimStep - 1] = 1;
    // }
    // for (int i = 0; i < 3; i++) {
    //     int iTempVal = *(&pMobRef->Monster_Default_Action_Type_01 + i);
    //     if (iTempVal > 0 && iTempVal <= 4) {
    //         iStep[iTempVal - 1] = 1;
    //     }
    // }

    // 为每个动作步骤注册动画
    for (int ia = 0; ia < 4; ia++) {
        if (iStep[ia]) {
            char szAnimName[64];

            // B_X_Stand
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Stand", 'A' + ia);
            RegisterAnimInfo(1, ia + 1, VString(szAnimName), 1);

            // B_X_Idle_01
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Idle_01", 'A' + ia);
            RegisterAnimInfo(2, ia + 1, VString(szAnimName), 1);

            // B_X_Run
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Run", 'A' + ia);
            RegisterAnimInfo(5, ia + 1, VString(szAnimName), 1);

            // B_X_Turn_L/R
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Turn_L", 'A' + ia);
            RegisterAnimInfo(7, ia + 1, VString(szAnimName), 1);
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Turn_R", 'A' + ia);
            RegisterAnimInfo(8, ia + 1, VString(szAnimName), 1);

            // B_X_Death_Stand/Down
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Death_Stand", 'A' + ia);
            RegisterAnimInfo(12, ia + 1, VString(szAnimName), 1);
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_Death_Down", 'A' + ia);
            RegisterAnimInfo(13, ia + 1, VString(szAnimName), 1);

            // B_X_SABreak_Start/Loop/End
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_SABreak_Start", 'A' + ia);
            RegisterAnimInfo(24, ia + 1, VString(szAnimName), 1);
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_SABreak_Loop", 'A' + ia);
            RegisterAnimInfo(25, ia + 1, VString(szAnimName), 1);
            sprintf_s(szAnimName, sizeof(szAnimName), "B_%c_SABreak_End", 'A' + ia);
            RegisterAnimInfo(26, ia + 1, VString(szAnimName), 1);
        }
    }

    // Death Clear 动画 (Motion Class 29)
    RegisterAnimInfo(29, 0, VString("B_Death_Clear"), 1);

    // 加载额外动画
    LoadExtraAnimation();
}

// ============================================================================
// XActionResMgr::LoadNpcAnimation
// IDA 0x140008aa0
// 加载 NPC 动画
// 参数:
//   pActionRes - 动画资源块
//   pNpcRef - NPC 信息表记录
// 还原自 IDA 反编译 (0x140008aa0 - 0x140008c67)
// ============================================================================
void XActionResMgr::LoadNpcAnimation(VActionResourceLump* pActionRes, TB_NPC* pNpcRef)
{
    // 保存当前处理的表 ID
    // TODO [DEPENDENCY]: 需要 TB_NPC::NPC_ID 字段定义
    // m_dwTableID = pNpcRef->NPC_ID;

    // 加载基础动画 (NPC bPlayer=false)
    LoadBaseAnimation(pActionRes, false);

    // ========================================
    // 注册 NPC 特定动画
    // ========================================

    // Stand 动画 (Motion Class 1)
    RegisterAnimInfo(1, 0, VString("N_Stand"), 0);
    RegisterAnimInfo(1, 0, VString("B_Stand"), 1);

    // Idle 动画 (Motion Class 2, SubClass 1)
    RegisterAnimInfo(2, 1, VString("N_Idle_02"), 0);
    RegisterAnimInfo(2, 1, VString("N_Idle_02"), 1);

    // Walk 动画 (Motion Class 3)
    RegisterAnimInfo(3, 0, VString("N_Walk"), 0);

    // 加载额外动画
    LoadExtraAnimation();
}

// ============================================================================
// XActionResMgr::LoadAkashicAnimation
// IDA 0x140008c70
// 加载 Akashic 动画
// 参数:
//   pActionRes - 动画资源块
//   pTableRef - Akashic 信息表记录
// 还原自 IDA 反编译 (0x140008c70 - 0x140008cb6)
// ============================================================================
void XActionResMgr::LoadAkashicAnimation(VActionResourceLump* pActionRes, TB_AKASHIC_RECORDS* pTableRef)
{
    // 保存当前处理的表 ID
    // TODO [DEPENDENCY]: 需要 TB_AKASHIC_RECORDS::ID 字段定义
    // m_dwTableID = pTableRef->ID;

    // 加载基础动画 (Akashic bPlayer=false)
    LoadBaseAnimation(pActionRes, false);

    // 加载额外动画
    LoadExtraAnimation();
}

// ============================================================================
// XActionResMgr::LoadExtraAnimation
// IDA 0x140008cc0
// 加载额外动画 (从资源中遍历所有动画并注册未注册的)
// 还原自 IDA 反编译 (0x140008cc0 - 0x140008eea)
// TODO [INCOMPLETE]:
//   - 需要 VActionResourceLump 的以下方法:
//     - GetActionLength() - 获取动画数量
//     - GetActionList() - 获取动画信息数组
//   - 需要 VAnimationInfo 结构定义 (包含 szName 字段)
//   - 需要 GetAnimIndex() 函数实现
// ============================================================================
void XActionResMgr::LoadExtraAnimation()
{
    if (!m_pActionResource) {
        return;
    }

    int nAddMotion = 1;

    // TODO [DEPENDENCY]: 需要 VActionResourceLump::GetActionLength 和 GetActionList
    // int nCount = m_pActionResource->GetActionLength();
    // VAnimationInfo* pActionList = m_pActionResource->GetActionList();
    //
    // for (int i = 0; i < nCount; i++) {
    //     const VAnimationInfo* pInfo = &pActionList[i];
    //
    //     // 跳过特殊动画名
    //     if (!pInfo || strcmp(pInfo->szName, "AnimationOfAll") == 0 || strcmp(pInfo->szName, "Destruction") == 0) {
    //         continue;
    //     }
    //
    //     // 检查动画是否已注册
    //     VString strAnimName(pInfo->szName);
    //     if (GetAnimIndex(m_dwTableID, strAnimName) == -1) {
    //         // 注册为 Boss 动画 (Type=1)
    //         RegisterAnimInfo(nAddMotion + 48, 0, strAnimName, 1);
    //         // 注册为普通动画 (Type=0)
    //         RegisterAnimInfo(nAddMotion + 48, 0, strAnimName, 0);
    //         nAddMotion++;
    //     }
    // }

    // 重置状态
    m_dwTableID = -1;
    m_pActionResource = nullptr;
}

// ============================================================================
// XActionResMgr::GetAnimIndex
// 查询动画索引
// 参数:
//   dwTableID - 表 ID
//   strAnimName - 动画名称
// 返回:
//   动画索引，未找到返回 -1
// TODO [INCOMPLETE]:
//   - 需要从 IDA 反编译还原完整实现
//   - 需要分析 m_mapAnimInfoKey 和 m_mapAnimInfoString 的查询逻辑
// ============================================================================
std::int32_t XActionResMgr::GetAnimIndex(std::int32_t dwTableID, const VString& strAnimName)
{
    // TODO: 从 m_mapAnimInfoKey 和 m_mapAnimInfoString 查询
    return -1;
}

// ============================================================================
// XActionResMgr::RegisterSkillAttackTrigger
// 注册技能攻击触发器
// 参数:
//   pActionRes - 动画资源块
//   nCharacterID - 角色 ID
// TODO [INCOMPLETE]:
//   - 需要从 IDA 反编译还原完整实现
//   - 需要分析 m_mapSkillAttackTrigger 的存储逻辑
// ============================================================================
void XActionResMgr::RegisterSkillAttackTrigger(VActionResourceLump* pActionRes, std::int32_t nCharacterID)
{
    // TODO: 实现技能攻击触发器注册
}

// ============================================================================
// XActionResMgr::LoadHitCollisionFromXML
// 从 XML 加载 Hit Collision 数据
// 参数:
//   szFilePath - XML 文件路径
// 返回:
//   tagHIT_COLLISION_DATA 指针，失败返回 nullptr
// ============================================================================
tagHIT_COLLISION_DATA* XActionResMgr::LoadHitCollisionFromXML(const char* szFilePath)
{
    // TODO: 从 IDA 反编译还原
    // 需要分析 LoadHitCollisionFromXML 函数的完整逻辑
    // 涉及 XML 解析和碰撞数据构造
    return nullptr;
}

// ============================================================================
// XActionResMgr::LoadTraceBoneNameFromXML
// 从 XML 加载 Trace Bone Name 数据
// 参数:
//   szFilePath - XML 文件路径
// 返回:
//   tagHIT_TRACE_BONE_NAME_DATA 指针，失败返回 nullptr
// ============================================================================
tagHIT_TRACE_BONE_NAME_DATA* XActionResMgr::LoadTraceBoneNameFromXML(const char* szFilePath)
{
    // TODO: 从 IDA 反编译还原
    // 需要分析 LoadTraceBoneNameFromXML 函数的完整逻辑
    // 涉及 XML 解析和骨骼追踪数据构造
    return nullptr;
}

// ============================================================================
// XActionResMgr::Load
// 加载资源文件
// 参数:
//   szFilePath - 资源文件路径
// 返回:
//   VManagedResource 指针
// ============================================================================
VManagedResource* XActionResMgr::Load(const char* szFilePath)
{
    // TODO: 继承自 VActionResourceManager 的虚函数
    // 需要从基类实现或 IDA 反编译还原
    return nullptr;
}

// ============================================================================
// XActionResMgr::RemoveAllResourceLump
// 移除所有资源块
// ============================================================================
void XActionResMgr::RemoveAllResourceLump()
{
    // TODO: 继承自 VActionResourceManager 的虚函数
    // 需要从基类实现或 IDA 反编译还原
}
