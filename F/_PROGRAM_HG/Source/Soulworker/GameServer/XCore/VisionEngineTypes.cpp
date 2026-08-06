// VisionEngineTypes.cpp - Vision Engine Type Implementations
// Resource Management System for GameServer
// IDA verified implementations

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"  // AttackJudgmentTrigger (2464 bytes)
#include <cstring>

// Static member initialization
float VManagedResource::g_fGlobalTime = 0.0f;

// ============================================================================
// VManagedResource Implementation
// ============================================================================

VManagedResource::VManagedResource()
    : m_iResourceFlag(0)
    , m_fLastTimeUsed(0.0f)
    , m_szFilename(nullptr) {
}

VManagedResource::~VManagedResource() {
    // Note: filename memory management would be handled by derived classes
}

// IDA: 0x140018750 - IsResourceFlagSet
// 精确还原：检查资源标志位
bool VManagedResource::IsResourceFlagSet(int iMask) const {
    // IDA decompiled:
    // return (static_cast<std::uint16_t>(iMask) & m_iResourceFlag) == iMask;
    return (static_cast<std::uint16_t>(iMask) & m_iResourceFlag) == iMask;
}

// IDA: 0x140018790 - IsLoaded
// 精确还原：检查资源是否已加载
bool VManagedResource::IsLoaded() const {
    // IDA decompiled:
    // return IsResourceFlagSet(1);
    return IsResourceFlagSet(1);
}

// IDA: 0x14072AD90 - EnsureLoaded
// 精确还原：确保资源已加载
void VManagedResource::EnsureLoaded() {
    // IDA decompiled:
    // this->m_fLastTimeUsed = VManagedResource::GetGlobalTime();
    // if ( !VManagedResource::IsLoaded(this) )
    //     this->DoReload(this);
    
    m_fLastTimeUsed = GetGlobalTime();
    if (!IsLoaded()) {
        DoReload();
    }
}

// IDA: 0x14072AD10 - EnsureUnloaded
// 精确还原：确保资源已卸载
void VManagedResource::EnsureUnloaded() {
    // IDA decompiled:
    // if ( VManagedResource::IsLoaded(this) )
    //     this->DoUnload(this);
    
    if (IsLoaded()) {
        DoUnload();
    }
}

// IDA: 0x14072AD80 - GetGlobalTime (static)
// 精确还原：获取全局时间
float VManagedResource::GetGlobalTime() {
    // IDA decompiled:
    // return VManagedResource::g_fGlobalTime;
    return g_fGlobalTime;
}

// IDA: 0x140734DB0 - GetFilename
// 精确还原：获取资源文件名
const char* VManagedResource::GetFilename() const {
    // IDA decompiled:
    // if ( this->m_szFilename && (*this->m_szFilename == 92 || *this->m_szFilename == 47) 
    //      && this->m_szFilename[1] != 92 )
    //     return this->m_szFilename + 1;
    // else
    //     return this->m_szFilename;
    
    if (m_szFilename && (*m_szFilename == '\\' || *m_szFilename == '/') && 
        m_szFilename[1] != '\\') {
        return m_szFilename + 1;
    }
    return m_szFilename;
}

// IDA: 0x1407727A0 - SetResourceFlag
// 精确还原：设置资源标志位
void VManagedResource::SetResourceFlag(int iMask) {
    // IDA decompiled:
    // this->m_iResourceFlag |= iMask;
    m_iResourceFlag |= static_cast<std::uint16_t>(iMask);
}

// Default implementations for virtual functions
void VManagedResource::DoReload() {
    // Base class stub - derived classes implement actual reload logic
    // Mark resource as loaded
    SetResourceFlag(1);
}

void VManagedResource::DoUnload() {
    // Base class stub - derived classes implement actual unload logic
    // Clear loaded flag
    m_iResourceFlag &= ~1;
}

// ============================================================================
// VActionResourceLump::CreateTrigger Implementation
// IDA: ?CreateTrigger@VActionResourceLump@@SAPEAVActionTrigger@@PEAVVChunkFile@@@Z @ 0x14072F350
// ============================================================================

ActionTrigger* VActionResourceLump::CreateTrigger(VChunkFile* infile) {
    short templateShort[2];
    if (!infile->ReadShort(templateShort))
        return nullptr;

    ActionTrigger* pTrigger = nullptr;
    switch (templateShort[0]) {
    case 0:
        pTrigger = new CameraShakingTrigger();
        break;
    case 1:
        pTrigger = new SoundPlayTrigger();
        break;
    case 2:
        pTrigger = new ExtraInputTrigger();
        break;
    case 3:
        pTrigger = new AttackJudgmentTrigger();
        break;
    case 4:
        pTrigger = new ChargingInputTrigger();
        break;
    case 5:
        pTrigger = new UserDataTrigger();
        break;
    case 6:
        pTrigger = new TrajectoryTrigger();
        break;
    case 7:
        pTrigger = new ScreenBlurTrigger();
        break;
    case 8:
        pTrigger = new MovingInputTrigger();
        break;
    case 9:
        pTrigger = new AlphaBlendingTrigger();
        break;
    case 10:
        pTrigger = new JumpAttackTrigger();
        break;
    case 11:
        pTrigger = new CreateEffectTrigger();
        break;
    case 12:
        pTrigger = new ShaderChangeTrigger();
        break;
    case 13:
        pTrigger = new DeathTrigger();
        break;
    case 14:
        pTrigger = new InvisibleTrigger();
        break;
    case 15:
        pTrigger = new WarpToPointTrigger();
        break;
    case 16:
        pTrigger = new SummonMonsterTrigger();
        break;
    case 17:
        pTrigger = new LuaFunctionCallTrigger();
        break;
    case 18:
        pTrigger = new AkashicTrigger();
        break;
    case 19:
        pTrigger = new MeshAttachmentTrigger();
        break;
    case 20:
        pTrigger = new CameraZoomTrigger();
        break;
    case 21:
        pTrigger = new SubordinationComboTrigger();
        break;
    case 22:
        pTrigger = new AttachToAttackerTrigger();
        break;
    case 23:
        pTrigger = new AnimSpeedTrigger();
        break;
    case 24:
        pTrigger = new CounterAttackTrigger();
        break;
    case 25:
        pTrigger = new DefenseTypeTrigger();
        break;
    case 26:
        pTrigger = new AttackJunctionTrigger();
        break;
    case 27:
        pTrigger = new InputFlagTrigger();
        break;
    case 28:
        pTrigger = new DeathShaderTrigger();
        break;
    case 29:
        pTrigger = new CharacterCameraLockTrigger();
        break;
    case 30:
        pTrigger = new DetachTrigger();
        break;
    case 31:
        pTrigger = new ShaderEffectTrigger();
        break;
    case 32:
        pTrigger = new CameraAnimTrigger();
        break;
    case 33:
        pTrigger = new CollisionChangeTrigger();
        break;
    case 34:
        pTrigger = new CameraFOVTrigger();
        break;
    case 35:
        pTrigger = new AutoRotationTrigger();
        break;
    case 36:
        pTrigger = new RandomSummonTrigger();
        break;
    case 37:
        pTrigger = new LinkSkillTrigger();
        break;
    case 38:
        pTrigger = new CheckAttackSkillTrigger();
        break;
    case 39:
        pTrigger = new DelSummonMonsterTrigger();
        break;
    case 40:
        pTrigger = new ApplyPassiveSkillTrigger();
        break;
    case 41:
        pTrigger = new MyBuffControlTrigger();
        break;
    default:
        break;
    }

    if (pTrigger) {
        pTrigger->TypeOfTrigger = static_cast<std::uint8_t>(templateShort[0]);
        pTrigger->AddRef();
    }
    return pTrigger;
}
