// VRefCounter.h
// Reference counter base class
// IDA decompilation from GameServer.exe
//
// Functions:
// - VRefCounter::DeleteThis (0x140188d60)

#pragma once

// Forward declarations
class VBaseObject;

// ============================================================================
// VRefCounter - Reference counter base class
// Provides reference counting and deletion functionality
// ============================================================================
class VRefCounter {
public:
    // === Virtual Destructor ===
    virtual ~VRefCounter() = default;

    // === DeleteThis - Delete this object
    // IDA: ?DeleteThis@VRefCounter@@UEAAXXZ @ 0x140188D60
    virtual void DeleteThis();

protected:
    // Virtual function pointer for destructor
    // IDA shows: this->dtr_VBaseObject(this, 1)
    // Points to the virtual destructor chain
};
