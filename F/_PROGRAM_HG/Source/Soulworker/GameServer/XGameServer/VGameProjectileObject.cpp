// VGameProjectileObject.cpp
// VGameProjectileObject implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VGameProjectileObject::CreateObject (0x14070e8f0)
// - VGameProjectileObject::GetTypeId (0x14070e960)
// - VGameProjectileObject::Init (0x14070eb70)

#include "VGameProjectileObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>

// Forward declarations
class VType;

// ============================================================================
// VGameProjectileObject::VGameProjectileObject - Constructor
// ============================================================================
VGameProjectileObject::VGameProjectileObject()
{
    // Base class VProjectileBase_cl constructor called automatically
}

// ============================================================================
// VGameProjectileObject::~VGameProjectileObject - Destructor
// ============================================================================
VGameProjectileObject::~VGameProjectileObject()
{
}

// ============================================================================
// VGameProjectileObject::Init - Initialize the projectile object
// IDA @ 0x14070EB70
// ============================================================================
void VGameProjectileObject::Init()
{
    // Call base class Init
    VProjectileBase_cl::Init();

    // TODO: Add more initialization when more functions are decompiled
}

// ============================================================================
// VGameProjectileObject::GetTypeId - Get type ID for RTTI
// IDA @ 0x14070E960
// ============================================================================
VType* VGameProjectileObject::GetTypeId() const
{
    // IDA code:
    // VType *__fastcall VGameProjectileObject::GetTypeId(VGameProjectileObject *this)
    // {
    //   return &VGameProjectileObject::classVGameProjectileObject;
    // }
    return &classVGameProjectileObject;
}

// ============================================================================
// VGameProjectileObject::CreateObject - Static factory method
// IDA @ 0x14070E8F0
// Creates a new VGameProjectileObject instance
// ============================================================================
VGameProjectileObject* VGameProjectileObject::CreateObject()
{
    // IDA code:
    // VGameProjectileObject *__fastcall VGameProjectileObject::CreateObject()
    // {
    //   VGameProjectileObject *v1; // [rsp+28h] [rbp-20h]
    //   v1 = (VGameProjectileObject *)VBaseObject::operator new(0x518u);
    //   if ( v1 )
    //     return VGameProjectileObject::VGameProjectileObject(v1);
    //   else
    //     return nullptr;
    // }

    // Allocate memory for the object (size 0x518 = 1304 bytes)
    void* pMemory = ::operator new(sizeof(VGameProjectileObject));
    if (pMemory)
    {
        return new (pMemory) VGameProjectileObject();
    }
    return nullptr;
}

// Static type info for RTTI
VType VGameProjectileObject::classVGameProjectileObject;
