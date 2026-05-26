// ActionResMgr.cpp - XActionResMgr 实现
// 从 IDA 反编译还原: GameServer.exe
// 构造函数: 0x140003660
// 析构函数: 0x140003770
// LoadBaseAnimation: 0x140003810

#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstring>
#include <cstdio>

// ============================================================================
// XActionResMgr::XActionResMgr 构造函数
// IDA 0x140003660
// ============================================================================
XActionResMgr::XActionResMgr()
    : VActionResourceManager()
    , m_pActionResource(nullptr)
{
    // IDA 反编译:
    // VActionResourceManager::VActionResourceManager(this);
    // this->__vftable = (XActionResMgr_vtbl *)&XActionResMgr::`vftable';
    // std::map<...>::map<...>(&this->m_mapHitCollisionInfo);
    // std::map<...>::map<...>(&this->m_mapTraceBoneName);
    // std::map<...>::map<...>(&this->m_mapAnimInfoKey);
    // std::map<...>::map<...>(&this->m_mapAnimInfoString);
    // std::map<...>::map<...>(&this->m_mapSkillAttackTrigger);
    // this->m_pActionResource = nullptr;

    // std::map 默认构造函数会自动调用，无需显式初始化
    // m_pActionResource 已在初始化列表中设置为 nullptr
}

// ============================================================================
// XActionResMgr::~XActionResMgr 析构函数
// IDA 0x140003770
// ============================================================================
XActionResMgr::~XActionResMgr()
{
    // IDA 反编译:
    // this->__vftable = (XActionResMgr_vtbl *)&XActionResMgr::`vftable';
    // std::map<int,VCommonPositionBoxInfo *>::~map(&this->m_mapSkillAttackTrigger);
    // std::map<int,VCommonPositionBoxInfo *>::~map(&this->m_mapAnimInfoString);
    // std::map<int,VCommonPositionBoxInfo *>::~map(&this->m_mapAnimInfoKey);
    // std::map<VString,tagHIT_TRACE_BONE_NAME_DATA *>::~map(&this->m_mapTraceBoneName);
    // std::map<VString,tagHIT_TRACE_BONE_NAME_DATA *>::~map(&this->m_mapHitCollisionInfo);
    // VActionResourceManager::~VActionResourceManager(this);

    // 清理 map 中的指针数据
    for (auto& pair : m_mapSkillAttackTrigger) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_mapSkillAttackTrigger.clear();

    for (auto& pair : m_mapAnimInfoString) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_mapAnimInfoString.clear();

    for (auto& pair : m_mapAnimInfoKey) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_mapAnimInfoKey.clear();

    for (auto& pair : m_mapTraceBoneName) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_mapTraceBoneName.clear();

    for (auto& pair : m_mapHitCollisionInfo) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_mapHitCollisionInfo.clear();

    // 父类析构函数会自动调用
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
        // 玩者 KnockDown 动画 (Motion Class 18-21)
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
// ============================================================================
void XActionResMgr::RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, std::int16_t nType)
{
    // GreenDamTan_stub: RegisterAnimInfo 的完整实现需要进一步反编译
    // 当前仅记录动画名称，实际逻辑需要从 IDA 0x140367AE0 (CMover::IsRegisterAnimInfo) 和相关函数还原

    // 计算动画 Key (MotionClass * 100 + SubClass + Type * 10000)
    std::int32_t nAnimKey = nMotionClass * 100 + nSubClass + nType * 10000;

    // 存储到 m_mapAnimInfoKey
    // TODO: 需要从 IDA 还原完整的 RegisterAnimInfo 实现

    // 使用 LogHelper 输出调试信息
    LogHelper::LogDebug("ActionResMgr", "RegisterAnimInfo: MotionClass=%d, SubClass=%d, AnimName=%s, Type=%d, Key=%d",
                        nMotionClass, nSubClass, strAnimName.AsChar(), nType, nAnimKey);
}