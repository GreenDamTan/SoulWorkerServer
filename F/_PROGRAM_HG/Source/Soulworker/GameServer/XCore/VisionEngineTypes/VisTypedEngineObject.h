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

// ============================================================================
// VVarChangeRes_e - Variable change result enumeration
// ============================================================================
enum class VVarChangeRes_e : int {
    Changed = 0,
    NotChanged = 1,
    Error = 2
};

// ============================================================================
// VSerializationContext - Serialization context (forward declaration)
// ============================================================================
class VSerializationContext;

// ============================================================================
// VTypedObject - Base typed object class
// ============================================================================
class VTypedObject {
public:
    // OnVariableValueChanging - Called when variable value is changing
    // IDA: ?OnVariableValueChanging@VTypedObject@@UEAA?AW4VVarChangeRes_e@@PEAVVisVariable_cl@@PEBD@Z @ 0x1401894E0
    virtual VVarChangeRes_e OnVariableValueChanging(VisVariable_cl* pVar, const char* value) {
        // IDA: return 2; (VVarChangeRes_e::Error or similar default)
        return VVarChangeRes_e::Error;
    }

    // OnDeserializationCallback - Called after deserialization
    // IDA: ?OnDeserializationCallback@VTypedObject@@UEAAXAEAVVSerializationContext@@@Z @ 0x140189500
    virtual void OnDeserializationCallback(VSerializationContext* context) {
        // IDA: this->m_pDeserializationArchive = nullptr;
        m_pDeserializationArchive = nullptr;
    }

    virtual ~VTypedObject() = default;

protected:
    void* m_pDeserializationArchive = nullptr;
};

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
