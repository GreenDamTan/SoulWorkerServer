// VGameTrapObject.cpp
// VGameTrapObject implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VGameTrapObject::CreateObject (0x140712100)
// - VGameTrapObject::GetTypeId (0x140712170)
// - VGameTrapObject::Init (0x140712430)

#include "VGameTrapObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>

// Forward declarations
class VType;

// ============================================================================
// VTrapBase_cl implementation
// ============================================================================

VTrapBase_cl* VTrapBase_cl::CreateObject()
{
    // IDA code:
    // VTrapBase_cl *__fastcall VTrapBase_cl::CreateObject()
    // {
    //   VTrapBase_cl *v0; // rax
    //   v0 = (VTrapBase_cl *)VBaseAlloc_rel(0x428u);
    //   if ( v0 )
    //     return VTrapBase_cl::VTrapBase_cl(v0);
    //   else
    //     return nullptr;
    // }

    void* pMemory = ::operator new(0x428);  // 1064 bytes
    if (pMemory)
    {
        return new (pMemory) VTrapBase_cl();
    }
    return nullptr;
}

// ============================================================================
// VGameTrapObject implementation
// ============================================================================

VGameTrapObject::VGameTrapObject()
{
    // Base class VTrapBase_cl constructor called automatically
}

VGameTrapObject::~VGameTrapObject()
{
}

void VGameTrapObject::Init()
{
    // Call base class Init
    VTrapBase_cl::Init();

    // TODO: Add more initialization when more functions are decompiled
}

VType* VGameTrapObject::GetTypeId() const
{
    return &classVGameTrapObject;
}

// CreateObject - Static factory method
// IDA @ 0x140712100
VGameTrapObject* VGameTrapObject::CreateObject()
{
    // IDA code:
    // VGameTrapObject *__fastcall VGameTrapObject::CreateObject()
    // {
    //   VGameTrapObject *v1; // [rsp+28h] [rbp-20h]
    //   v1 = (VGameTrapObject *)VBaseObject::operator new(0x510u);
    //   if ( v1 )
    //     return VGameTrapObject::VGameTrapObject(v1);
    //   else
    //     return nullptr;
    // }

    // Allocate memory for the object (size 0x510 = 1296 bytes)
    void* pMemory = ::operator new(0x510);
    if (pMemory)
    {
        return new (pMemory) VGameTrapObject();
    }
    return nullptr;
}

// Static type info for RTTI
VType VGameTrapObject::classVGameTrapObject;
