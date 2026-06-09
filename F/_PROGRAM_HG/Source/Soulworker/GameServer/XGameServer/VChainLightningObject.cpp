// VChainLightningObject.cpp
// VChainLightningObject implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VChainLightningObject::Init (0x14070b370)
// - VChainLightningObject::GetTypeId (0x14070b0e0)
// - VChainLightningObject::SetFinish (0x14070e530)
// - VChainLightningObject::CreateObject (0x14070b080)

#include "VChainLightningObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>

// Forward declarations
class VType;

// ============================================================================
// VChainLightningObject::VChainLightningObject - Constructor
// ============================================================================
VChainLightningObject::VChainLightningObject()
    : m_pActionEvent(nullptr)
    , m_iCurChainCount(0)
    , m_bActivated(0)
    , m_pArea(nullptr)
    , m_fChainLife(0.0f)
    , m_pSkillInfo(nullptr)
    , m_nSessionID(0)
{
    // Base class VChainBase_cl constructor called automatically
}

// ============================================================================
// VChainLightningObject::~VChainLightningObject - Destructor
// ============================================================================
VChainLightningObject::~VChainLightningObject()
{
    // Vector destructor called automatically
}

// ============================================================================
// VChainLightningObject::Init - Initialize the chain lightning object
// IDA @ 0x14070B370
// ============================================================================
void VChainLightningObject::Init()
{
    // IDA code:
    // void __fastcall VChainLightningObject::Init(VChainLightningObject *this)
    // {
    //   VChainBase_cl::Init(this);
    //   this->m_pActionEvent = nullptr;
    //   this->m_iCurChainCount = 0;
    //   std::vector<unsigned __int64>::clear((std::vector<tagWARP_POTAL_INFO *> *)&this->m_vecChainEffect);
    //   this->m_bActivated = 0;
    //   this->m_pArea = nullptr;
    // }

    // Call base class Init
    VChainBase_cl::Init();

    // Initialize members
    m_pActionEvent = nullptr;
    m_iCurChainCount = 0;
    m_vecChainEffect.clear();
    m_bActivated = 0;
    m_pArea = nullptr;
    m_pSkillInfo = nullptr;
    m_nSessionID = 0;
}

// ============================================================================
// VChainLightningObject::GetTypeId - Get type ID for RTTI
// IDA @ 0x14070B0E0
// ============================================================================
VType* VChainLightningObject::GetTypeId() const
{
    // IDA code:
    // VType *__fastcall VChainLightningObject::GetTypeId(VChainLightningObject *this)
    // {
    //   return &VChainLightningObject::classVChainLightningObject;
    // }
    return &classVChainLightningObject;
}

// ============================================================================
// VChainLightningObject::SetFinish - Sets the chain lightning to finished state
// IDA @ 0x14070e530
// ============================================================================
void VChainLightningObject::SetFinish()
{
    // IDA: Sets chain life to 0 and deactivates
    m_fChainLife = 0.0f;
    m_bActivated = 0;
}

// ============================================================================
// VChainLightningObject::CreateObject - Static factory method
// IDA @ 0x14070B080
// Creates a new VChainLightningObject instance
// ============================================================================
VChainLightningObject* VChainLightningObject::CreateObject()
{
    // IDA code (from VGameProjectileObject::CreateObject pattern):
    // VChainLightningObject *__fastcall VChainLightningObject::CreateObject()
    // {
    //   VChainLightningObject *v1; // [rsp+28h] [rbp-20h]
    //   v1 = (VChainLightningObject *)VBaseObject::operator new(0x518u);
    //   if ( v1 )
    //     return VChainLightningObject::VChainLightningObject(v1);
    //   else
    //     return nullptr;
    // }

    // Allocate memory for the object (size 0x518 = 1304 bytes)
    void* pMemory = ::operator new(sizeof(VChainLightningObject));
    if (pMemory)
    {
        return new (pMemory) VChainLightningObject();
    }
    return nullptr;
}

// Static type info for RTTI
VType VChainLightningObject::classVChainLightningObject;
