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
    , m_fContinousHitTime(0.0f)
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
    m_fChainLife = 0.0f;
    m_fContinousHitTime = 0.0f;
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
// VChainLightningObject::ReleaseAllChainEffect - Releases all chain effects
// IDA @ 0x14070E3B0
// ============================================================================
void VChainLightningObject::ReleaseAllChainEffect()
{
    // IDA code:
    // void __fastcall VChainLightningObject::ReleaseAllChainEffect(VChainLightningObject *this)
    // {
    //   std::vector<std::tr1::shared_ptr<GOComponent>>::begin(
    //     (std::vector<VAnimationInfo> *)&this->m_vecChainEffect,
    //     (std::_Vector_iterator<std::_Vector_val<VAnimationInfo> > *)&iter);
    //   while ( 1 )
    //   {
    //     v1 = std::_Tree<...>::end((std::vector<VAnimationInfo> *)&this->m_vecChainEffect, &v5);
    //     if ( !std::_Vector_const_iterator<...>::operator!=(&iter, v1) )
    //       break;
    //     pChainEffect = *(SChainEffect **)std::_Vector_iterator<...>::operator*(&iter)->szName;
    //     if ( pChainEffect )
    //     {
    //       ptr = pChainEffect;
    //       operator delete(pChainEffect);
    //       pChainEffect = nullptr;
    //     }
    //     std::_Vector_iterator<...>::operator++(&iter, &result, 0);
    //   }
    //   std::vector<unsigned __int64>::clear((std::vector<tagWARP_POTAL_INFO *> *)&this->m_vecChainEffect);
    //   this->m_fContinousHitTime = 0.0;
    //   this->m_fChainLife = 0.0;
    // }

    // Iterate through all chain effects and delete them
    for (auto it = m_vecChainEffect.begin(); it != m_vecChainEffect.end(); ++it)
    {
        // Each element is a pointer to SChainEffect (stored as uint64_t)
        void* pChainEffect = reinterpret_cast<void*>(*it);
        if (pChainEffect)
        {
            operator delete(pChainEffect);
        }
    }

    // Clear the vector
    m_vecChainEffect.clear();

    // Reset timing values
    m_fContinousHitTime = 0.0f;
    m_fChainLife = 0.0f;
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
