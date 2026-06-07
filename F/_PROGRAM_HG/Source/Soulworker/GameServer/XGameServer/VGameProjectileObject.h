// VGameProjectileObject.h
// VGameProjectileObject - Game projectile object for skills
// IDA decompilation from GameServer.exe
//
// Functions:
// - VGameProjectileObject::CreateObject (0x14070e8f0)
// - VGameProjectileObject::GetTypeId (0x14070e960)
// - VGameProjectileObject::Init (0x14070eb70)

#pragma once

#include <cstdint>

// Forward declarations
class VType;

// VProjectileBase_cl - Base class for projectile objects (Vision Engine)
class VProjectileBase_cl {
public:
    virtual ~VProjectileBase_cl() = default;
    virtual void Init() {}
};

// VGameProjectileObject - Vision Engine game projectile object
// Used for skill projectiles
// IDA: object size = 0x518 bytes (from CreateObject)
class VGameProjectileObject : public VProjectileBase_cl {
public:
    // === Static Factory ===

    // CreateObject - Static factory method to create new instance
    // IDA: ?CreateObject@VGameProjectileObject@@SAPEAVVTypedObject@@XZ @ 0x14070E8F0
    static VGameProjectileObject* CreateObject();

    // === Constructor ===
    VGameProjectileObject();

    // === Destructor ===
    virtual ~VGameProjectileObject();

    // === Virtual Methods ===

    // GetTypeId - Get type ID for RTTI
    // IDA: ?GetTypeId@VGameProjectileObject@@UEBAPEAUVType@@XZ @ 0x14070E960
    virtual VType* GetTypeId() const;

    // Init - Initialize the projectile object
    // IDA: ?Init@VGameProjectileObject@@UEAAXXZ @ 0x14070EB70
    virtual void Init() override;

protected:
    // === Member Variables ===
    // TODO: Add members when more functions are decompiled

private:
    // Static type info for RTTI
    static VType classVGameProjectileObject;
};
