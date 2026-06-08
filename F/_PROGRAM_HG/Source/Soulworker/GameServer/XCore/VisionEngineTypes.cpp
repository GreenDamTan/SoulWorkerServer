// VisionEngineTypes.cpp - Vision Engine Type Implementations
// Resource Management System for GameServer
// IDA verified implementations

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
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
