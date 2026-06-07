// VGameTrapObject.h
// VGameTrapObject - Game trap object for skills
// IDA decompilation from GameServer.exe
//
// Functions:
// - VGameTrapObject::CreateObject (0x140712100)
// - VGameTrapObject::GetTypeId (0x140712170)
// - VGameTrapObject::Init (0x140712430)

#pragma once

#include <cstdint>

// Forward declarations
class VType;

// VTrapBase_cl - Base class for trap objects (Vision Engine)
class VTrapBase_cl {
public:
    virtual ~VTrapBase_cl() = default;
    virtual void Init() {}

    // CreateObject - Static factory method
    // IDA: ?CreateObject@VTrapBase_cl@@SAPEAVVTypedObject@@XZ @ 0x14078B300
    static VTrapBase_cl* CreateObject();
};

// VGameTrapObject - Vision Engine game trap object
// Used for skill traps
// IDA: object size = 0x510 bytes (from CreateObject)
class VGameTrapObject : public VTrapBase_cl {
public:
    // === Static Factory ===

    // CreateObject - Static factory method to create new instance
    // IDA: ?CreateObject@VGameTrapObject@@SAPEAVVTypedObject@@XZ @ 0x140712100
    static VGameTrapObject* CreateObject();

    // === Constructor ===
    VGameTrapObject();

    // === Destructor ===
    virtual ~VGameTrapObject();

    // === Virtual Methods ===

    // GetTypeId - Get type ID for RTTI
    // IDA: ?GetTypeId@VGameTrapObject@@UEBAPEAUVType@@XZ @ 0x140712170
    virtual VType* GetTypeId() const;

    // Init - Initialize the trap object
    // IDA: ?Init@VGameTrapObject@@UEAAXXZ @ 0x140712430
    virtual void Init() override;

protected:
    // === Member Variables ===
    // TODO: Add members when more functions are decompiled

private:
    // Static type info for RTTI
    static VType classVGameTrapObject;
};
