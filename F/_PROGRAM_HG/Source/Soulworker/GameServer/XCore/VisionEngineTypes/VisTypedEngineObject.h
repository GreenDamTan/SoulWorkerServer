// VisTypedEngineObject.h
// VisTypedEngineObject_cl - Vision Engine typed object class
// IDA decompilation from GameServer.exe
//
// Functions:
// - VisTypedEngineObject_cl::TriggerScriptEvent (0x1401894a0)

#pragma once

#include <cstdint>

// Forward declarations
class VisVariable_cl;
class VType;
class VArchive;
struct VVariableAttributeInfo;

// ============================================================================
// VVarChangeRes_e - Variable change result enumeration
// ============================================================================
enum class VVarChangeRes_e : int {
    VCHANGE_IS_REDUNDANT = 0,
    VCHANGE_IS_CANCELLED = 1,
    VCHANGE_IS_ALLOWED = 2
};

enum VObjectFlags_e : int {
    VObjectFlag_None = 0,
    VObjectFlag_BusySerializing = 1,
    VObjectFlag_IsNetworkReplica = 2,
    VObjectFlag_NetworkObjectExistsRemotely = 4,
    VObjectFlag_InsideSerializationSession = 16,
    VObjectFlag_AutoDispose = 32,
    VObjectFlag_Disposed = 64,
    VObjectFlag_Disposing = 128,
    VObjectFlag_UserBit0 = 65536,
    VObjectFlag_UserBit1 = 131072,
    VObjectFlag_UserBit2 = 262144,
    VObjectFlag_UserBit3 = 524288,
    VObjectFlag_UserBit4 = 1048576,
    VObjectFlag_UserBit5 = 2097152,
    VObjectFlag_UserBit6 = 4194304,
    VObjectFlag_UserBit7 = 8388608
};

// ============================================================================
// VSerializationContext - Serialization context (forward declaration)
// ============================================================================
class VSerializationContext;

// ============================================================================
// VTypedObject - Base typed object class
// ============================================================================
// TODO: 需人工审查：原始虚函数槽位尚未完整恢复，不能据此确认派生类的虚调用 ABI。
class VTypedObject {
public:
    // Member variables (public for compatibility)
    std::int32_t m_eObjectFlags = 0;        // object flags
    VType* m_pRegisteredAtType = nullptr;    // VType*
    VArchive* m_pDeserializationArchive = nullptr;  // VArchive*

    bool IsObjectFlagSet(VObjectFlags_e eFlag) const {
        return (eFlag & m_eObjectFlags) > 0;
    }

    bool IsDisposed() const {
        return IsObjectFlagSet(VObjectFlag_Disposed);
    }

    // OnDeserializationCallback - Called after deserialization
    // IDA: ?OnDeserializationCallback@VTypedObject@@UEAAXAEAVVSerializationContext@@@Z @ 0x140189500
    virtual void OnDeserializationCallback(VSerializationContext& context) {
        // IDA: this->m_pDeserializationArchive = nullptr;
        m_pDeserializationArchive = nullptr;
    }

    virtual int WantsDeserializationCallback(VSerializationContext& context) {
        return 0;
    }

    virtual ~VTypedObject() = default;

    // OnVariableValueChanging - Called when variable value is changing
    // IDA: ?OnVariableValueChanging@VTypedObject@@UEAA?AW4VVarChangeRes_e@@PEAVVisVariable_cl@@PEBD@Z @ 0x1401894E0
    virtual VVarChangeRes_e OnVariableValueChanging(VisVariable_cl* pVar, const char* value) {
        // IDA: return 2; (VCHANGE_IS_ALLOWED)
        return VVarChangeRes_e::VCHANGE_IS_ALLOWED;
    }

    // TODO: 需人工审查：原函数与其他符号共用空函数体，完整虚调用 ABI 仍待核对。
    virtual void OnVariableValueChanged(VisVariable_cl* pVar, const char* value) {
    }

    // TODO: 需人工审查：与其他符号共用空函数体，原始虚槽顺序仍待恢复。
    virtual void GetVariableAttributes(VisVariable_cl* pVariable, VVariableAttributeInfo& destInfo) {
    }
};

template <typename T>
class DynArray_cl;

// TODO: 需人工审查：该实例的构造、析构及其他虚槽尚未恢复，不能据此实例化原始数组。
template <>
class DynArray_cl<class IVObjectComponent*> {
public:
    DynArray_cl();
    virtual ~DynArray_cl();

    class IVObjectComponent** GetDataPtr() const {
        return data;
    }

protected:
    std::uint32_t size;
    class IVObjectComponent** data;
    class IVObjectComponent* defaultValue;
};

// TODO: 需人工审查：构造、析构与元素所有权尚无可执行函数证据，不能用于原始派生类构造链。
class VObjectComponentCollection {
public:
    VObjectComponentCollection();
    ~VObjectComponentCollection();

    int Count() const {
        return m_iElementCount;
    }

    class IVObjectComponent** GetPtrs() const {
        if (m_iElementCount <= 1)
            return const_cast<class IVObjectComponent**>(m_ElementsNoAlloc);
        else
            return m_Elements.GetDataPtr();
    }

private:
    class IVObjectComponent* m_pLastAccessedComp;
    VType* m_pLastAccessedType;
    std::int32_t m_iElementCount;
    DynArray_cl<class IVObjectComponent*> m_Elements;
    class IVObjectComponent* m_ElementsNoAlloc[1];
};

static_assert(sizeof(DynArray_cl<class IVObjectComponent*>) == 32);
static_assert(sizeof(VObjectComponentCollection) == 64);

// ============================================================================
// VisTypedEngineObject_cl - Vision Engine typed engine object
// Inherits from VTypedObject
// ============================================================================
class VisTypedEngineObject_cl : public VTypedObject {
public:
    // TriggerScriptEvent - Trigger a script event
    // IDA: ?TriggerScriptEvent@VisTypedEngineObject_cl@@UEAAXPEBD@Z @ 0x1401894A0
    virtual void TriggerScriptEvent(const char* pszEvent) {
        // IDA: this->TriggerScriptEvent_2(this, pszEvent, "*");
        TriggerScriptEvent_2(pszEvent, "*");
    }

protected:
    // Internal trigger function
    void TriggerScriptEvent_2(const char* pszEvent, const char* pszParam);
};
