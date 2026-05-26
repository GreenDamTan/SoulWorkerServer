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
    RegisterAnimInfo(0, 0, VString("N_Spawn"), false);

    // 注册通用动画 (Motion Class 2: Idle)
    RegisterAnimInfo(2, 0, VString("N_Idle_01"), false);

    // 注册 Boss/受击动画 (BattlePose = true)
    RegisterAnimInfo(0, 0, VString("N_Spawn"), true);
    RegisterAnimInfo(2, 0, VString("B_Idle_01"), true);

    // 死亡动画 (Motion Class 12-14)
    RegisterAnimInfo(12, 0, VString("B_Death_Stand"), true);
    RegisterAnimInfo(13, 0, VString("B_Death_Down"), true);
    RegisterAnimInfo(14, 0, VString("B_Death_Down_Wait"), true);

    // 受击动画 (Motion Class 15-17)
    RegisterAnimInfo(15, 0, VString("B_DMG_L"), true);
    RegisterAnimInfo(16, 0, VString("B_DMG_R"), true);
    RegisterAnimInfo(17, 0, VString("B_KB_Start"), true);
    RegisterAnimInfo(17, 1, VString("B_KB_End"), true);

    // 根据 bPlayer 参数注册不同的 KnockDown 动画
    if (bPlayer) {
        // 玩家 KnockDown 动画 (Motion Class 18-21)
        // KnockDown Strength (18)
        RegisterAnimInfo(18, 0, VString("B_KD_Str_Start"), true);
        RegisterAnimInfo(18, 1, VString("B_KD_Upp_End"), true);
        RegisterAnimInfo(18, 2, VString("B_KD_Upp_Down"), true);
        RegisterAnimInfo(18, 3, VString("B_KD_Upp_Down_Hit"), true);
        RegisterAnimInfo(18, 4, VString("B_KD_Upp_Air_Hit"), true);
        RegisterAnimInfo(18, 5, VString("B_KD_Upp_Raise"), true);
        RegisterAnimInfo(18, 6, VString("B_KD_Upp_Loop"), true);

        // KnockDown Upper (19)
        RegisterAnimInfo(19, 0, VString("B_KD_Upp_Start"), true);
        RegisterAnimInfo(19, 1, VString("B_KD_Upp_End"), true);
        RegisterAnimInfo(19, 2, VString("B_KD_Upp_Down"), true);
        RegisterAnimInfo(19, 3, VString("B_KD_Upp_Down_Hit"), true);
        RegisterAnimInfo(19, 4, VString("B_KD_Upp_Air_Hit"), true);
        RegisterAnimInfo(19, 5, VString("B_KD_Upp_Raise"), true);
        RegisterAnimInfo(19, 6, VString("B_KD_Upp_Loop"), true);

        // KnockDown Hammer Front (20)
        RegisterAnimInfo(20, 0, VString("B_KD_Ham_F_Start"), true);
        RegisterAnimInfo(20, 1, VString("B_KD_Upp_End"), true);
        RegisterAnimInfo(20, 2, VString("B_KD_Upp_Down"), true);
        RegisterAnimInfo(20, 3, VString("B_KD_Upp_Down_Hit"), true);
        RegisterAnimInfo(20, 4, VString("B_KD_Upp_Air_Hit"), true);
        RegisterAnimInfo(20, 5, VString("B_KD_Upp_Raise"), true);
        RegisterAnimInfo(20, 6, VString("B_KD_Upp_Loop"), true);

        // KnockDown Hammer Back (21)
        RegisterAnimInfo(21, 0, VString("B_KD_Ham_B_Start"), true);
        RegisterAnimInfo(21, 1, VString("B_KD_Upp_End"), true);
        RegisterAnimInfo(21, 2, VString("B_KD_Upp_Down"), true);
        RegisterAnimInfo(21, 3, VString("B_KD_Upp_Down_Hit"), true);
        RegisterAnimInfo(21, 4, VString("B_KD_Upp_Air_Hit"), true);
        RegisterAnimInfo(21, 5, VString("B_KD_Upp_Raise"), true);
        RegisterAnimInfo(21, 6, VString("B_KD_Upp_Loop"), true);
    }
    else {
        // NPC/怪物 KnockDown 动画 (Motion Class 18-21)
        // KnockDown Strength (18)
        RegisterAnimInfo(18, 0, VString("B_KD_Str_Start"), true);
        RegisterAnimInfo(18, 1, VString("B_KD_Str_End"), true);
        RegisterAnimInfo(18, 2, VString("B_KD_Str_Down"), true);
        RegisterAnimInfo(18, 3, VString("B_KD_Str_Down_Hit"), true);
        RegisterAnimInfo(18, 4, VString("B_KD_Str_Air_Hit"), true);
        RegisterAnimInfo(18, 5, VString("B_KD_Str_Raise"), true);
        RegisterAnimInfo(18, 6, VString("B_KD_Str_Loop"), true);

        // KnockDown Upper (19)
        RegisterAnimInfo(19, 0, VString("B_KD_Upp_Start"), true);
        RegisterAnimInfo(19, 1, VString("B_KD_Upp_End"), true);
        RegisterAnimInfo(19, 2, VString("B_KD_Upp_Down"), true);
        RegisterAnimInfo(19, 3, VString("B_KD_Upp_Down_Hit"), true);
        RegisterAnimInfo(19, 4, VString("B_KD_Upp_Air_Hit"), true);
        RegisterAnimInfo(19, 5, VString("B_KD_Upp_Raise"), true);
        RegisterAnimInfo(19, 6, VString("B_KD_Upp_Loop"), true);

        // KnockDown Hammer Front (20)
        RegisterAnimInfo(20, 0, VString("B_KD_Ham_F_Start"), true);
        RegisterAnimInfo(20, 1, VString("B_KD_Ham_F_End"), true);
        RegisterAnimInfo(20, 2, VString("B_KD_Ham_F_Down"), true);
        RegisterAnimInfo(20, 3, VString("B_KD_Ham_F_Down_Hit"), true);
        RegisterAnimInfo(20, 4, VString("B_KD_Ham_F_Air_Hit"), true);
        RegisterAnimInfo(20, 5, VString("B_KD_Ham_F_Raise"), true);
        RegisterAnimInfo(20, 6, VString("B_KD_Ham_F_Loop"), true);

        // KnockDown Hammer Back (21)
        RegisterAnimInfo(21, 0, VString("B_KD_Ham_B_Start"), true);
        RegisterAnimInfo(21, 1, VString("B_KD_Ham_B_End"), true);
        RegisterAnimInfo(21, 2, VString("B_KD_Ham_B_Down"), true);
        RegisterAnimInfo(21, 3, VString("B_KD_Ham_B_Down_Hit"), true);
        RegisterAnimInfo(21, 4, VString("B_KD_Ham_B_Air_Hit"), true);
        RegisterAnimInfo(21, 5, VString("B_KD_Ham_B_Raise"), true);
        RegisterAnimInfo(21, 6, VString("B_KD_Ham_B_Loop"), true);
    }

    // Stun 动画 (Motion Class 22)
    RegisterAnimInfo(22, 0, VString("B_Stun"), true);

    // Jump Air Hit 动画 (Motion Class 23)
    RegisterAnimInfo(23, 0, VString("B_Jump_Air_Hit"), true);
}

// ============================================================================
// XActionResMgr::RegisterAnimInfo
// IDA 0x14000c250
// 注册动画信息到管理器
// 参数:
//   nMotionClass - 动作类型 (0=Spawn, 2=Idle, 12-23=各种受击/死亡)
//   nSubClass - 子类型 (用于同一动作的不同阶段)
//   strAnimName - 动画名称
//   bBattlePose - 是否战斗姿态 (Boss/受击动画)
// 返回:
//   true 如果注册成功, false 如果动画不存在于资源中
// 还原自 IDA 反编译:
//   1. 检查动画是否存在于资源中 (FindAnimationInfo)
//   2. 计算动画 Key = nSubClass + 1000 * nMotionClass + (bBattlePose ? 100 : 0)
//   3. 获取或创建表ID对应的 map
//   4. 插入到 m_mapAnimInfoKey 和 m_mapAnimInfoString
// ============================================================================
bool XActionResMgr::RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, bool bBattlePose)
{
    if (!m_pActionResource) {
        return false;
    }

    // 检查动画是否存在于资源中
    // TODO [DEPENDENCY]: 需要 VActionResourceLump::FindAnimationInfo 方法
    // const char* pszAnimName = strAnimName.AsChar();
    // if (!VActionResourceLump::FindAnimationInfo(m_pActionResource, pszAnimName)) {
    //     return false;
    // }

    // 计算动画 Key (使用静态方法)
    std::uint32_t dwKey = static_cast<std::uint32_t>(GetAnimIndex(nMotionClass, nSubClass, bBattlePose));

    // 获取或创建 map
    std::map<VString, unsigned long>* mapAnimKey = nullptr;
    std::map<unsigned long, VString>* mapAnimString = nullptr;

    auto itKey = m_mapAnimInfoKey.find(m_dwTableID);
    auto itString = m_mapAnimInfoString.find(m_dwTableID);

    if (itKey != m_mapAnimInfoKey.end() && itString != m_mapAnimInfoString.end()) {
        mapAnimKey = itKey->second;
        mapAnimString = itString->second;
    }
    else {
        // 创建新的 map
        mapAnimKey = new std::map<VString, unsigned long>();
        mapAnimString = new std::map<unsigned long, VString>();

        m_mapAnimInfoKey[m_dwTableID] = mapAnimKey;
        m_mapAnimInfoString[m_dwTableID] = mapAnimString;
    }

    if (mapAnimKey && mapAnimString) {
        // 检查是否已存在相同 Key
        auto itExisting = mapAnimString->find(dwKey);
        if (itExisting != mapAnimString->end()) {
            // 已存在，检查名称是否相同
            const char* pExistingName = itExisting->second.AsChar();
            const char* pNewName = strAnimName.AsChar();
            if (pExistingName && pNewName && strcmp(pExistingName, pNewName) != 0) {
                // 名称不同，更新映射
                (*mapAnimKey)[strAnimName] = dwKey;
                (*mapAnimString)[dwKey] = strAnimName;
            }
        }
        else {
            // 不存在，插入新映射
            (*mapAnimKey)[strAnimName] = dwKey;
            (*mapAnimString)[dwKey] = strAnimName;
        }
    }

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
    RegisterAnimInfo(1, 0, VString("B_Stand"), true);
    RegisterAnimInfo(1, 0, VString("N_Stand"), false);

    // Walk 动画 (Motion Class 3)
    RegisterAnimInfo(3, 0, VString("N_Walk_F"), false);
    RegisterAnimInfo(3, 1, VString("N_Walk_L"), false);
    RegisterAnimInfo(3, 2, VString("N_Walk_R"), false);
    RegisterAnimInfo(3, 3, VString("N_Walk_B"), false);

    // Run 动画 (Motion Class 5)
    RegisterAnimInfo(5, 0, VString("N_Run_F"), false);
    RegisterAnimInfo(5, 1, VString("N_Run_L"), false);
    RegisterAnimInfo(5, 2, VString("N_Run_R"), false);
    RegisterAnimInfo(5, 3, VString("N_Run_B"), false);

    // Run End 动画 (Motion Class 6)
    RegisterAnimInfo(6, 0, VString("N_Run_F_End"), false);

    // Dash 动画 (Motion Class 32-34)
    RegisterAnimInfo(32, 0, VString("N_Dash_Start"), false);
    RegisterAnimInfo(33, 0, VString("N_Dash_Loop"), false);
    RegisterAnimInfo(34, 0, VString("N_Dash_End"), false);

    // Boss Walk 动画 (Motion Class 3, BattlePose)
    RegisterAnimInfo(3, 0, VString("B_Walk_F"), true);
    RegisterAnimInfo(3, 1, VString("B_Walk_L"), true);
    RegisterAnimInfo(3, 2, VString("B_Walk_R"), true);
    RegisterAnimInfo(3, 3, VString("B_Walk_B"), true);

    // Boss Run 动画 (Motion Class 5, BattlePose)
    RegisterAnimInfo(5, 0, VString("B_Run_F"), true);
    RegisterAnimInfo(5, 1, VString("B_Run_L"), true);
    RegisterAnimInfo(5, 2, VString("B_Run_R"), true);
    RegisterAnimInfo(5, 3, VString("B_Run_B"), true);
    RegisterAnimInfo(6, 0, VString("B_Run_F_End"), true);

    // Jump 动画 (Motion Class 9-11)
    // Boss Jump Start (所有方向使用相同动画)
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(9, i, VString("B_Jump_Start_F"), true);
    }
    // Boss Jump Loop
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(10, i, VString("B_Jump_Loop_F"), true);
    }
    // Boss Jump Land
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(11, i, VString("B_Jump_Land_C"), true);
    }
    // Normal Jump Start
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(9, i, VString("N_Jump_Start_F"), false);
    }
    // Normal Jump Loop
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(10, i, VString("N_Jump_Loop_F"), false);
    }
    // Normal Jump Land
    for (int i = 0; i < 5; i++) {
        RegisterAnimInfo(11, i, VString("N_Jump_Land_C"), false);
    }

    // Mode 切换动画 (Motion Class 27-28)
    RegisterAnimInfo(27, 0, VString("B_N_Mode"), true);
    RegisterAnimInfo(27, 0, VString("B_N_Mode"), false);
    RegisterAnimInfo(28, 0, VString("N_B_Mode"), true);
    RegisterAnimInfo(28, 0, VString("N_B_Mode"), false);

    // Evade 动画 (Motion Class 30)
    RegisterAnimInfo(30, 0, VString("B_Evade_F"), true);
    RegisterAnimInfo(30, 1, VString("B_Evade_L"), true);
    RegisterAnimInfo(30, 2, VString("B_Evade_R"), true);
    RegisterAnimInfo(30, 3, VString("B_Evade_B"), true);

    // Walk Lower 动画 (Motion Class 31)
    RegisterAnimInfo(31, 0, VString("B_Walk_Lower_F"), true);
    RegisterAnimInfo(31, 1, VString("B_Walk_Lower_L"), true);
    RegisterAnimInfo(31, 2, VString("B_Walk_Lower_R"), true);
    RegisterAnimInfo(31, 3, VString("B_Walk_Lower_B"), true);

    // Boss Dash 动画 (Motion Class 32-34)
    RegisterAnimInfo(32, 0, VString("B_Dash_Start"), true);
    RegisterAnimInfo(33, 0, VString("B_Dash_Loop"), true);
    RegisterAnimInfo(34, 0, VString("B_Dash_End"), true);

    // Dash Jump 动画 (Motion Class 35-37)
    RegisterAnimInfo(35, 0, VString("N_Dash_Jump_Start"), false);
    RegisterAnimInfo(36, 0, VString("N_Dash_Jump_Loop"), false);
    RegisterAnimInfo(37, 0, VString("N_Dash_Jump_End"), false);

    RegisterAnimInfo(35, 0, VString("B_Dash_Jump_Start"), true);
    if (!RegisterAnimInfo(36, 0, VString("B_Dash_Jump_Loop"), true)) {
        RegisterAnimInfo(36, 0, VString("B_Jump_Loop"), true);
    }
    if (!RegisterAnimInfo(37, 0, VString("B_Dash_Jump_End"), true)) {
        RegisterAnimInfo(37, 0, VString("B_Jump_Land_C"), true);
    }

    // Roll 动画 (Motion Class 38)
    RegisterAnimInfo(38, 0, VString("B_Roll_F"), true);
    RegisterAnimInfo(38, 1, VString("B_Roll_L"), true);
    RegisterAnimInfo(38, 2, VString("B_Roll_R"), true);
    RegisterAnimInfo(38, 3, VString("B_Roll_B"), true);

    // Looting 动画 (Motion Class 39)
    RegisterAnimInfo(39, 0, VString("B_Looting"), true);

    // Interaction Object 动画 (Motion Class 40-42)
    RegisterAnimInfo(40, 0, VString("B_Interaction_Object_Start"), true);
    RegisterAnimInfo(41, 0, VString("B_Interaction_Object_Loop"), true);
    RegisterAnimInfo(42, 0, VString("B_Interaction_Object_End"), true);

    // Login Class 动画 (Motion Class 43)
    RegisterAnimInfo(43, 0, VString("Login_Class_N_Stand"), true);
    RegisterAnimInfo(43, 1, VString("Login_Class_B_Stand"), true);
    RegisterAnimInfo(43, 2, VString("Login_Class_N_B_Mode"), true);
    RegisterAnimInfo(43, 3, VString("Login_Class_N_Look_Start"), true);
    RegisterAnimInfo(43, 4, VString("Login_Class_N_Look_End"), true);
    RegisterAnimInfo(43, 5, VString("Login_Class_N_Look_Stand"), true);
    RegisterAnimInfo(43, 6, VString("Login_Class_Select"), true);
    RegisterAnimInfo(43, 7, VString("Login_Class_Select_Wait"), true);
    RegisterAnimInfo(43, 8, VString("L_Stand"), true);
    RegisterAnimInfo(43, 9, VString("L_Stand_Change"), true);

    // Win 动画 (Motion Class 44-45)
    RegisterAnimInfo(44, 0, VString("B_Win"), true);
    RegisterAnimInfo(45, 0, VString("B_Win02"), true);

    // Special Evade 动画 (Motion Class 46)
    RegisterAnimInfo(46, 0, VString("B_Special_Evade_F"), true);
    RegisterAnimInfo(46, 1, VString("B_Special_Evade_L"), true);
    RegisterAnimInfo(46, 2, VString("B_Special_Evade_R"), true);
    RegisterAnimInfo(46, 3, VString("B_Special_Evade_B"), true);

    // Jump Move 动画 (Motion Class 47)
    for (int i = 0; i < 4; i++) {
        RegisterAnimInfo(47, i, VString("B_Jump_Move_F"), true);
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
        RegisterAnimInfo(3, i, VString("N_Walk"), false);
    }
    for (int i = 0; i < 4; i++) {
        RegisterAnimInfo(3, i, VString("N_Walk"), true);
    }

    // Gaze 动画 (Motion Class 4)
    RegisterAnimInfo(4, 0, VString("B_Gaze_F"), true);
    RegisterAnimInfo(4, 1, VString("B_Gaze_L"), true);
    RegisterAnimInfo(4, 2, VString("B_Gaze_R"), true);
    RegisterAnimInfo(4, 3, VString("B_Gaze_B"), true);

    // 根据 Monster_BattleMode_Type 注册不同 Stand 和 Mode 动画
    // TODO [DEPENDENCY]: 需要 TB_MONSTER::Monster_BattleMode_Type 字段
    // int battleModeType = pMobRef->Monster_BattleMode_Type;
    int battleModeType = 0; // 临时默认值

    if (battleModeType == 0) {
        // 默认模式
        RegisterAnimInfo(1, 0, VString("B_Stand"), true);
        RegisterAnimInfo(1, 0, VString("N_Stand"), false);
        RegisterAnimInfo(27, 0, VString("B_N_Mode"), true);
        RegisterAnimInfo(27, 0, VString("B_N_Mode"), false);
        RegisterAnimInfo(28, 0, VString("N_B_Mode"), true);
        RegisterAnimInfo(28, 0, VString("N_B_Mode"), false);
    }
    else if (battleModeType == 1) {
        // A 型战斗模式
        RegisterAnimInfo(1, 0, VString("B_Stand_A"), true);
        RegisterAnimInfo(1, 0, VString("N_Stand_A"), false);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_A"), true);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_A"), false);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_A"), true);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_A"), false);
    }
    else if (battleModeType == 2) {
        // B 型战斗模式
        RegisterAnimInfo(1, 0, VString("B_Stand_B"), true);
        RegisterAnimInfo(1, 0, VString("N_Stand_B"), false);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_B"), true);
        RegisterAnimInfo(27, 0, VString("B_N_Mode_B"), false);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_B"), true);
        RegisterAnimInfo(28, 0, VString("N_B_Mode_B"), false);
    }
    else {
        // 其他类型
        RegisterAnimInfo(1, 0, VString("B_Stand"), true);
        RegisterAnimInfo(1, 0, VString("N_Stand"), false);
    }

    // Run 动画 (Motion Class 5) - 带回退逻辑
    if (!RegisterAnimInfo(5, 0, VString("B_Run_F"), true)) {
        RegisterAnimInfo(5, 0, VString("B_Run"), true);
    }
    if (!RegisterAnimInfo(5, 1, VString("B_Run_L"), true)) {
        RegisterAnimInfo(5, 1, VString("B_Run"), true);
    }
    if (!RegisterAnimInfo(5, 2, VString("B_Run_R"), true)) {
        RegisterAnimInfo(5, 2, VString("B_Run"), true);
    }
    if (!RegisterAnimInfo(5, 3, VString("B_Run_B"), true)) {
        RegisterAnimInfo(5, 3, VString("B_Run"), true);
    }

    // Normal Run 动画
    if (RegisterAnimInfo(5, 0, VString("N_Run"), false)) {
        // 如果 N_Run 存在，所有方向使用相同动画
        for (int i = 0; i < 4; i++) {
            RegisterAnimInfo(5, i, VString("N_Run"), false);
        }
    }
    else {
        // 回退到 B_Run 系列
        if (!RegisterAnimInfo(5, 0, VString("B_Run_F"), false)) {
            RegisterAnimInfo(5, 0, VString("B_Run"), false);
        }
        if (!RegisterAnimInfo(5, 1, VString("B_Run_L"), false)) {
            RegisterAnimInfo(5, 1, VString("B_Run"), false);
        }
        if (!RegisterAnimInfo(5, 2, VString("B_Run_R"), false)) {
            RegisterAnimInfo(5, 2, VString("B_Run"), false);
        }
        if (!RegisterAnimInfo(5, 3, VString("B_Run_B"), false)) {
            RegisterAnimInfo(5, 3, VString("B_Run"), false);
        }
    }

    // Turn 动画 (Motion Class 7-8)
    RegisterAnimInfo(7, 0, VString("B_Turn_L"), true);
    RegisterAnimInfo(8, 0, VString("B_Turn_R"), true);
    RegisterAnimInfo(7, 1, VString("B_Big_Turn_L"), true);
    RegisterAnimInfo(8, 1, VString("B_Big_Turn_R"), true);

    // SABreak 动画 (Motion Class 24-26)
    RegisterAnimInfo(24, 0, VString("B_SABreak_Start"), true);
    RegisterAnimInfo(25, 0, VString("B_SABreak_Loop"), true);
    RegisterAnimInfo(26, 0, VString("B_SABreak_End"), true);

    // Death Flat 动画 (Motion Class 13, SubClass 10)
    RegisterAnimInfo(13, 10, VString("B_Death_Flat"), true);

    // Damage 动画 (Motion Class 15-16, BattlePose = false)
    RegisterAnimInfo(15, 0, VString("B_DMG_L"), false);
    RegisterAnimInfo(16, 0, VString("B_DMG_R"), false);

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
    RegisterAnimInfo(1, 0, VString("N_Stand"), false);
    RegisterAnimInfo(1, 0, VString("B_Stand"), true);

    // Idle 动画 (Motion Class 2, SubClass 1)
    RegisterAnimInfo(2, 1, VString("N_Idle_02"), false);
    RegisterAnimInfo(2, 1, VString("N_Idle_02"), true);

    // Walk 动画 (Motion Class 3)
    RegisterAnimInfo(3, 0, VString("N_Walk"), false);

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
// IDA 0x14000c170
// 查询动画索引
// 参数:
//   dwTableID - 表 ID
//   strAnimName - 动画名称
// 返回:
//   动画索引，未找到返回 -1
// 还原自 IDA 反编译:
//   查找 m_mapAnimInfoKey[dwTableID]，然后在该 map 中查找 strAnimName
// ============================================================================
std::int32_t XActionResMgr::GetAnimIndex(std::int32_t dwTableID, const VString& strAnimName)
{
    // 查找表 ID 对应的动画映射
    auto itTable = m_mapAnimInfoKey.find(dwTableID);
    if (itTable == m_mapAnimInfoKey.end() || !itTable->second) {
        return -1;
    }

    // 在动画映射中查找动画名称
    auto* pAnimMap = itTable->second;
    auto itAnim = pAnimMap->find(strAnimName);
    if (itAnim == pAnimMap->end()) {
        return -1;
    }

    return static_cast<std::int32_t>(itAnim->second);
}

// ============================================================================
// XActionResMgr::RegisterSkillAttackTrigger
// IDA 0x14000ce70
// 注册技能攻击触发器
// 参数:
//   pActionRes - 动画资源块
//   byClassID - 角色 Class ID (对应 TB_CHARACTER_INFO 的 Character_ID)
// 还原自 IDA 反编译 (0x14000ce70 - 0x14000d064):
//   1. 遍历 TB_SKILL 表，筛选指定 ClassID 的技能
//   2. 对每个技能调用 GetSkillAnimNames 获取动画名称列表
//   3. 遍历动画名称，查找触发器并添加到 m_mapSkillAttackTrigger
// ============================================================================
void XActionResMgr::RegisterSkillAttackTrigger(VActionResourceLump* pActionRes, std::int8_t byClassID)
{
    // TODO [DEPENDENCY]: 需要以下依赖才能完整实现:
    // 1. TXSingleton<XGameServer>::Instance() 获取 GameServer 实例
    // 2. XGameServer::m_xResourceMgr 访问资源管理器
    // 3. XResourceMgr::m_mapTB_SKILL 技能表
    // 4. GetSkillAnimNames() 函数
    // 5. VActionResourceLump::FindAnimationInfo() 方法

    // IDA 反编译逻辑:
    // std::vector<std::string> vecSkillAnimName;
    // auto pGameServer = TXSingleton<XGameServer>::Instance();
    // auto& skillTable = pGameServer->m_xResourceMgr.m_mapTB_SKILL;
    //
    // for (auto iterSkill = skillTable.begin(); iterSkill != skillTable.end(); ++iterSkill) {
    //     TB_SKILL* pSkillRef = &iterSkill->second;
    //
    //     // 检查技能是否属于当前角色 Class
    //     // 条件: pSkillRef->Use_Class == byClassID && pSkillRef->Skill_Index >= 0xA95F60
    //     if (pSkillRef->Use_Class == byClassID && pSkillRef->Skill_Index >= 0xA95F60) {
    //         // 获取技能动画名称列表
    //         GetSkillAnimNames(pSkillRef, &vecSkillAnimName);
    //
    //         // 遍历每个动画名称，查找攻击触发器
    //         for (size_t i = 0; i < vecSkillAnimName.size(); i++) {
    //             const char* szAnimName = vecSkillAnimName[i].c_str();
    //
    //             // 查找动画信息
    //             const VAnimationInfo* pInfo = GetActionDesc(pActionRes, szAnimName);
    //             if (pInfo && pInfo->arTriggers.GetLength() > 0) {
    //                 // 遍历触发器，找到 AttackJudgmentTrigger
    //                 for (int j = 0; j < pInfo->arTriggers.GetLength(); j++) {
    //                     ActionTrigger* pTrigger = pInfo->arTriggers[j];
    //                     if (pTrigger && pTrigger->TypeOfTrigger == 3) { // AttackJudgmentTrigger
    //                         AttackJudgmentTrigger* pAJTrigger = (AttackJudgmentTrigger*)pTrigger;
    //
    //                         // 添加到 m_mapSkillAttackTrigger
    //                         std::set<unsigned long>* pSet = m_mapSkillAttackTrigger[pSkillRef->Skill_Index];
    //                         if (!pSet) {
    //                             pSet = new std::set<unsigned long>();
    //                             m_mapSkillAttackTrigger[pSkillRef->Skill_Index] = pSet;
    //                         }
    //                         pSet->insert(pAJTrigger->EventID);
    //                     }
    //                 }
    //             }
    //         }
    //
    //         vecSkillAnimName.clear();
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "RegisterSkillAttackTrigger - TODO: needs complete dependency types");
}

// ============================================================================
// XActionResMgr::GetActionDesc
// IDA 0x14000a0c0
// 获取动作描述 (动画信息)
// 参数:
//   pActionResource - 动画资源块
//   pszAniName - 动画名称
// 返回:
//   VAnimationInfo 指针，未找到返回 nullptr
// 还原自 IDA 反编译:
// ```cpp
// const VAnimationInfo *__fastcall XActionResMgr::GetActionDesc(
//         XActionResMgr *this,
//         VActionResourceLump *pActionResource,
//         const char *pszAniName)
// {
//   const VAnimationInfo *pInfo;
//   int i;
//   if ( !VActionResourceLump::GetActionList(pActionResource) )
//     return nullptr;
//   pInfo = VActionResourceLump::GetActionList(pActionResource);
//   for ( i = 0;
//         i < (int)VActionResourceLump::GetActionLength(pActionResource);
//         pInfo = &VActionResourceLump::GetActionList(pActionResource)[i] )
//   {
//     if ( pInfo && !strcmp(pInfo->szName, pszAniName) )
//       return pInfo;
//     ++i;
//   }
//   return nullptr;
// }
// ```
// ============================================================================
const VAnimationInfo* XActionResMgr::GetActionDesc(VActionResourceLump* pActionResource, const char* pszAniName)
{
    if (!pActionResource || !pszAniName) {
        return nullptr;
    }

    // TODO: 需要 VActionResourceLump::GetActionList 和 GetActionLength 方法
    // 当前返回 nullptr 作为 stub
    return nullptr;
}

// ============================================================================
// XActionResMgr::RetrieveEvent
// IDA 0x14000a180
// 检索事件 (动作触发器)
// 参数:
//   actionCode - 动作代码
//   iActionIdx - 动作索引
//   pActionInfo - 动画信息
// 返回:
//   ActionTrigger 指针，未找到返回 nullptr
// 还原自 IDA 反编译:
// ```cpp
// ActionTrigger *__fastcall XActionResMgr::RetrieveEvent(
//         __int16 actionCode,
//         int iActionIdx,
//         const VAnimationInfo *pActionInfo)
// {
//   ActionTrigger **v4;
//   int iMatchActionCount;
//   int i;
//   if ( !pActionInfo )
//     return nullptr;
//   iMatchActionCount = 0;
//   for ( i = 0; i < VArray<ActionTrigger *>::GetLength(&pActionInfo->arTriggers); ++i )
//   {
//     v4 = VArray<ActionTrigger *>::ElementAt(&pActionInfo->arTriggers, i);
//     if ( (*v4)->TypeOfTrigger == actionCode )
//     {
//       if ( iMatchActionCount == iActionIdx )
//         return *v4;
//       ++iMatchActionCount;
//     }
//   }
//   return nullptr;
// }
// ============================================================================
ActionTrigger* XActionResMgr::RetrieveEvent(std::int16_t actionCode, int iActionIdx, const VAnimationInfo* pActionInfo)
{
    if (!pActionInfo) {
        return nullptr;
    }

    // TODO: 需要 VAnimationInfo::arTriggers 成员和 VArray 模板方法
    // 当前返回 nullptr 作为 stub
    return nullptr;
}

// ============================================================================
// XActionResMgr::SetHitCollisionDataToActor
// IDA 0x14000b9b0
// 设置碰撞数据到 Actor
// 参数:
//   szCodeName - 代码名称
//   pMover - Mover 对象
// 还原自 IDA 反编译:
//   1. 构造 VString key = szCodeName
//   2. 在 m_mapHitCollisionInfo 中查找 key
//   3. 如果找到，设置 HitCylinder 和 HitCollisionData
// ============================================================================
void XActionResMgr::SetHitCollisionDataToActor(const char* szCodeName, CMover* pMover)
{
    if (!szCodeName || !pMover) {
        return;
    }

    // 构造查找键
    VString strKey(szCodeName);

    // 在 m_mapHitCollisionInfo 中查找
    auto it = m_mapHitCollisionInfo.find(strKey);
    if (it == m_mapHitCollisionInfo.end() || !it->second) {
        return;
    }

    tagHIT_COLLISION_DATA* pData = it->second;

    // 设置碰撞圆柱体 (如果有效)
    // TODO: 需要 tagHIT_COLLISION_DATA::fCylinderRadius 和 fCylinderHeight 字段
    // if (pData->fCylinderRadius > 0.0f && pData->fCylinderHeight > 0.0f) {
    //     pMover->SetHitCylinder(pData->fCylinderRadius, pData->fCylinderHeight);
    // }

    // 设置碰撞数据
    pMover->SetHitCollisionData(pData);
}

// ============================================================================
// XActionResMgr::SetTraceBoneNameDataToActor
// IDA 0x14000bab0
// 设置骨骼追踪数据到 Actor
// 参数:
//   szCodeName - 代码名称
//   pMover - Mover 对象
// ============================================================================
void XActionResMgr::SetTraceBoneNameDataToActor(const char* szCodeName, CMover* pMover)
{
    if (!szCodeName || !pMover) {
        return;
    }

    // 构造查找键
    VString strKey(szCodeName);

    // 在 m_mapTraceBoneName 中查找
    auto it = m_mapTraceBoneName.find(strKey);
    if (it == m_mapTraceBoneName.end() || !it->second) {
        return;
    }

    // TODO: 设置骨骼追踪数据到 Mover
    // tagHIT_TRACE_BONE_NAME_DATA* pData = it->second;
    // pMover->SetTraceBoneNameData(pData);
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
// XActionResMgr::GetAnimIndex (通过动作类型计算索引)
// IDA 0x140368a30
// 静态函数 - 计算动画索引
// 参数:
//   nMotionClass - 动作类型
//   nSubClass - 子类型
//   bBattlePose - 是否战斗姿态
// 返回:
//   动画索引 (Key)
// ============================================================================
std::int32_t XActionResMgr::GetAnimIndex(std::int16_t nMotionClass, std::int16_t nSubClass, bool bBattlePose)
{
    // IDA 反编译:
    // dwKey = nSubClass + 1000 * nMotionClass;
    // if ( bBattlePose )
    //     dwKey += 100;
    // return dwKey;

    std::uint32_t dwKey = static_cast<std::uint32_t>(nSubClass) + 1000 * static_cast<std::uint32_t>(nMotionClass);
    if (bBattlePose) {
        dwKey += 100;
    }
    return static_cast<std::int32_t>(dwKey);
}

// ============================================================================
// XActionResMgr::ChangeMotionCallback
// IDA 0x14000a230
// 更改动作回调
// 参数:
//   pMover - Mover 对象
//   pInfo - 动画信息
// ============================================================================
void XActionResMgr::ChangeMotionCallback(CMover* pMover, const VAnimationInfo* pInfo)
{
    // IDA 反编译:
    // if ( pInfo->eActionBufferBehavior == RESETBEFORE )
    //     pMover->DeleteActionBuffer(pMover);
    // XActionResMgr::ActionDestToEntity(this, pMover, pInfo);

    if (!pMover || !pInfo) {
        return;
    }

    // TODO: 需要 VAnimationInfo::eActionBufferBehavior 字段
    // if (pInfo->eActionBufferBehavior == ACTION_BUFFER_BEHAVIOR_RESETBEFORE) {
    //     pMover->DeleteActionBuffer();
    // }

    ActionDestToEntity(pMover, pInfo);
}

// ============================================================================
// XActionResMgr::ActionDestToEntity
// IDA 0x14000a280
// 将动作数据应用到实体
// 这是一个非常复杂的大型函数，处理多种触发器类型:
//   - AttackJudgmentTrigger (攻击判定, type 3)
//   - ChargingInputTrigger (蓄力输入, type 4)
//   - UserDataTrigger (用户数据, type 5)
//   - MovingInputTrigger (移动输入, type 8)
//   - JumpAttackTrigger (跳跃攻击, type 10)
//   - DeathTrigger (死亡, type 13)
//   - InvisibleTrigger (隐身, type 14)
//   - WarpToPointTrigger (传送, type 15)
//   - SummonMonsterTrigger (召唤怪物, type 16)
//   - LuaFunctionCallTrigger (Lua函数调用, type 17)
//   - AkashicTrigger (Akashic, type 18)
//   - SubordinationComboTrigger (从属连击, type 21)
//   - AttachToAttackerTrigger (附加到攻击者, type 22)
//   - AnimSpeedTrigger (动画速度, type 23)
//   - CounterAttackTrigger (反击, type 24)
//   - DefenseTypeTrigger (防御类型, type 25)
//   - DetachTrigger (分离, type 30)
//   - CollisionChangeTrigger (碰撞变更, type 33)
//   - AutoRotationTrigger (自动旋转, type 35)
//   - RandomSummonTrigger (随机召唤, type 36)
//   - LinkSkillTrigger (链接技能, type 37)
//   - CheckAttackSkillTrigger (检查攻击技能, type 38)
//   - DelSummonMonsterTrigger (删除召唤怪物, type 39)
//   - ApplyPassiveSkillTrigger (应用被动技能, type 40)
//   - MyBuffControlTrigger (我的Buff控制, type 41)
// 参数:
//   pMover - Mover 对象
//   pInfo - 动画信息
// 还原自 IDA 反编译 (0x14000a280 - 0x14000b6c8):
//   1. 获取 Mover 的各种状态 (PvpCondition, ActionCondition, Divergence, CombatType, SkillChargeStep, SkillLevel)
//   2. 调用 ActionTrigger::SetFiltering 设置过滤数据
//   3. 调用 MakeGroupFilteringData 创建分组过滤数据
//   4. 遍历 pInfo->arTriggers 触发器数组
//   5. 对每个触发器进行过滤检查和分组检查
//   6. 根据触发器类型执行相应操作 (通过 AddActionBuffer 添加到 Mover)
// ============================================================================
void XActionResMgr::ActionDestToEntity(CMover* pMover, const VAnimationInfo* pInfo)
{
    if (!pMover || !pInfo) {
        return;
    }

    // TODO [DEPENDENCY]: 这是一个非常复杂的函数，需要以下依赖:
    // 1. CMover::GetPvpCondition() - 获取 PVP 条件
    // 2. CMover::GetActionCondition() - 获取动作条件
    // 3. CMover::GetDivergenceValue() - 获取分歧值
    // 4. CMover::GetCombatType() - 获取战斗类型
    // 5. CMover::GetSkillChargeStep() - 获取技能蓄力步骤
    // 6. CMover::GetSkillLevel() - 获取技能等级
    // 7. ActionTrigger::SetFiltering() - 设置过滤条件
    // 8. ActionTrigger::IsFiltering() - 检查过滤条件
    // 9. MakeGroupFilteringData() - 创建分组过滤数据
    // 10. 各种 Trigger 类型的 RTTI 动态转换 (AttackJudgmentTrigger, ChargingInputTrigger, 等)
    // 11. tagACTION_BUFFER 结构和 CMover::AddActionBuffer()
    // 12. CMover::IsSendProjectilePacket() - 发送投射物包
    // 13. XActor::GetType() - 获取 Actor 类型
    // 14. CMonster::GetMobTableRef() - 获取怪物表引用 (类型 ID 31305905 特殊处理)
    // 15. CMoverEx::GetSkillLoopTime() - 获取技能循环时间

    // IDA 反编译的核心逻辑:
    // int iPvpCondition = pMover->GetPvpCondition();
    // int iActionCondition = pMover->GetActionCondition();
    // int iDivergence = pMover->GetDivergenceValue();
    // int iCombatType = pMover->GetCombatType();
    // int iChargeLevel = pMover->GetSkillChargeStep();
    // int iSkillLevel = pMover->GetSkillLevel();
    //
    // int nCurFilterData1, nCurFilterData2, nCurFilterData3;
    // ActionTrigger::SetFiltering(iSkillLevel, iChargeLevel, iCombatType, iDivergence, iActionCondition, iPvpCondition,
    //                             &nCurFilterData1, &nCurFilterData2, &nCurFilterData3);
    //
    // std::map<int, SGroupID> mapGroup;
    // MakeGroupFilteringData(pMover, pInfo, &mapGroup);
    //
    // int iLength = pInfo->arTriggers.GetLength();
    // for (int i = 0; i < iLength; i++) {
    //     ActionTrigger* pTrigger = pInfo->arTriggers[i];
    //     if (!pTrigger) continue;
    //
    //     // 分组检查 (针对 AttackJudgmentTrigger)
    //     if (pTrigger->TypeOfTrigger == 3) {
    //         AttackJudgmentTrigger* pAJTrigger = (AttackJudgmentTrigger*)pTrigger;
    //         auto itGroup = mapGroup.find(pAJTrigger->shGroupID);
    //         if (itGroup == mapGroup.end()) goto process_trigger;
    //         SGroupID* pTempGroup = &itGroup->second;
    //         pTempGroup->iCurIndex++;
    //         if (pTempGroup->iCurIndex == pTempGroup->iRandomIndex) goto process_trigger;
    //         continue;
    //     }
    //
    // process_trigger:
    //     // 过滤检查
    //     if (!ActionTrigger::IsFiltering(nCurFilterData1, nCurFilterData2, nCurFilterData3,
    //                                      pTrigger->dwFilterInfo1, pTrigger->dwFilterInfo2, pTrigger->dwFilterInfo3)) {
    //         continue;
    //     }
    //
    //     // 根据触发器类型处理
    //     switch (pTrigger->TypeOfTrigger - 3) {
    //         case 0: // AttackJudgmentTrigger (type 3)
    //             // 特殊处理: 检查 ActorType, 攻击类型 (1,2,4,5,6), 调用 IsSendProjectilePacket
    //             // 创建 tagACTION_BUFFER(1, StartTime), 添加 EventID, 0, 1, 1, fYaw=-1000.0
    //             break;
    //         case 1: // ChargingInputTrigger (type 4)
    //             // 创建 tagACTION_BUFFER(0x23, StartTime)
    //             break;
    //         case 2: // UserDataTrigger (type 5)
    //             // 创建 tagACTION_BUFFER(0x12, StartTime)
    //             break;
    //         case 5: // MovingInputTrigger (type 8)
    //             // 创建 tagACTION_BUFFER(0x10, StartTime)
    //             break;
    //         case 7: // JumpAttackTrigger (type 10)
    //             // 创建 tagACTION_BUFFER(0x13, StartTime)
    //             break;
    //         case 10: // DeathTrigger (type 13)
    //             // 创建 tagACTION_BUFFER(0x17, StartTime)
    //             break;
    //         case 11: // InvisibleTrigger (type 14)
    //             // 创建 tagACTION_BUFFER(0x18, StartTime)
    //             break;
    //         case 12: // WarpToPointTrigger (type 15)
    //             // 创建 tagACTION_BUFFER(0x19, StartTime)
    //             break;
    //         case 13: // SummonMonsterTrigger (type 16)
    //             // 根据 SummonType 处理 (0=普通, 3=爆炸召唤)
    //             // 检查 SummonChance 概率
    //             // 创建 tagACTION_BUFFER(0x1A, StartTime)
    //             break;
    //         case 14: // LuaFunctionCallTrigger (type 17)
    //             // 创建 tagACTION_BUFFER(0x1B, StartTime)
    //             break;
    //         case 15: // AkashicTrigger (type 18)
    //             // 创建 tagACTION_BUFFER(0x1C, StartTime)
    //             break;
    //         case 18: // SubordinationComboTrigger (type 21)
    //             // 设置 pMover->SetWaitSuboInputActionProcess(1)
    //             // 创建 tagACTION_BUFFER(0x1F, StartTime)
    //             break;
    //         case 19: // AttachToAttackerTrigger (type 22)
    //             // 创建 tagACTION_BUFFER(0x20, StartTime)
    //             break;
    //         case 20: // AnimSpeedTrigger (type 23)
    //             // 创建 tagACTION_BUFFER(0x21, StartTime)
    //             break;
    //         case 21: // CounterAttackTrigger (type 24)
    //             // 创建 tagACTION_BUFFER(0x22, StartTime)
    //             break;
    //         case 22: // DefenseTypeTrigger (type 25)
    //             // 创建 tagACTION_BUFFER(0x24, StartTime)
    //             break;
    //         case 27: // DetachTrigger (type 30)
    //             // 创建 tagACTION_BUFFER(0x29, StartTime)
    //             break;
    //         case 30: // CollisionChangeTrigger (type 33)
    //             // 创建 tagACTION_BUFFER(0x2C, StartTime)
    //             break;
    //         case 32: // AutoRotationTrigger (type 35)
    //             // 创建 tagACTION_BUFFER(0x2E, StartTime)
    //             break;
    //         case 33: // RandomSummonTrigger (type 36)
    //             // 创建 tagACTION_BUFFER(0x2F, StartTime)
    //             break;
    //         case 34: // LinkSkillTrigger (type 37)
    //             // 创建 tagACTION_BUFFER(0x30, StartTime)
    //             break;
    //         case 35: // CheckAttackSkillTrigger (type 38)
    //             // 创建 tagACTION_BUFFER(0x31, StartTime)
    //             break;
    //         case 36: // DelSummonMonsterTrigger (type 39)
    //             // 检查 MonsterID != 0
    //             // 创建 tagACTION_BUFFER(0x32, StartTime)
    //             break;
    //         case 37: // ApplyPassiveSkillTrigger (type 40)
    //             // 检查 iSkillGroupID != 0
    //             // 创建 tagACTION_BUFFER(0x33, StartTime)
    //             break;
    //         case 38: // MyBuffControlTrigger (type 41)
    //             // 创建 tagACTION_BUFFER(0x34, StartTime)
    //             break;
    //         default:
    //             continue;
    //     }
    //     // 所有 case 最终都调用 CMover::AddActionBuffer(pMover, &xAction)
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ActionDestToEntity - TODO: needs complete dependency types");
}

// ============================================================================
// XActionResMgr::SetAnimInfoToActor
// IDA 0x14000d360
// 设置动画信息到 Actor
// 参数:
//   dwTableID - 表 ID
//   pMover - Mover 对象
// 返回:
//   true 如果成功设置至少一个映射
// ============================================================================
bool XActionResMgr::SetAnimInfoToActor(std::uint32_t dwTableID, CMover* pMover)
{
    // IDA 反编译:
    // bRet = 0;
    // it = this->m_mapAnimInfoKey.find(dwTableID);
    // if ( it != end ) {
    //     CMover::SetAnimInfoKey(pMover, it->second);
    //     bRet = 1;
    // }
    // it_str = this->m_mapAnimInfoString.find(dwTableID);
    // if ( it_str != end ) {
    //     CMover::SetAnimInfoString(pMover, it_str->second);
    //     return 1;
    // }
    // return bRet;

    if (!pMover) {
        return false;
    }

    bool bRet = false;

    // 设置 AnimInfoKey
    auto itKey = m_mapAnimInfoKey.find(static_cast<std::int32_t>(dwTableID));
    if (itKey != m_mapAnimInfoKey.end() && itKey->second) {
        // TODO [DEPENDENCY]: 需要 CMover::SetAnimInfoKey 方法
        // pMover->SetAnimInfoKey(itKey->second);
        bRet = true;
    }

    // 设置 AnimInfoString
    auto itString = m_mapAnimInfoString.find(static_cast<std::int32_t>(dwTableID));
    if (itString != m_mapAnimInfoString.end() && itString->second) {
        // TODO [DEPENDENCY]: 需要 CMover::SetAnimInfoString 方法
        // pMover->SetAnimInfoString(itString->second);
        return true;
    }

    return bRet;
}

// ============================================================================
// XActionResMgr::IsCorrectTriggerID
// IDA 0x14000d470
// 检查触发器 ID 是否正确
// 参数:
//   iSkillID - 技能 ID
//   iEventID - 事件 ID
// 返回:
//   true 如果触发器存在
// ============================================================================
bool XActionResMgr::IsCorrectTriggerID(std::uint32_t iSkillID, std::uint32_t iEventID)
{
    // IDA 反编译:
    // pSetTrigger = this->m_mapSkillAttackTrigger[iSkillID];
    // if ( !pSetTrigger )
    //     return 0;
    // it = pSetTrigger->find(iEventID);
    // return it != pSetTrigger->end();

    auto it = m_mapSkillAttackTrigger.find(static_cast<std::int32_t>(iSkillID));
    if (it == m_mapSkillAttackTrigger.end() || !it->second) {
        return false;
    }

    std::set<unsigned long>* pSetTrigger = it->second;
    return pSetTrigger->find(iEventID) != pSetTrigger->end();
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
