// VisBaseEntity.h
// VisBaseEntity_cl - Vision Engine base entity class
// IDA decompilation from GameServer.exe
//
// Functions:
// - VisBaseEntity_cl::SetFixedSkinningMode (0x140189430)
// - VisBaseEntity_cl::GetFixedSkinningMode (0x140189450)

#pragma once

#include <cstdint>
#include "VisObject3D.h"

// ============================================================================
// VisSkinningMode_e - Skinning mode enumeration
// ============================================================================
enum class VisSkinningMode_e : int {
    None = 0,
    Software = 1,
    Hardware = 2,
    Mixed = 3
};

// ============================================================================
// VisBaseEntity_cl - Vision Engine base entity class
// Base class for all visible 3D entities
// Inherits from VisObject3D_cl
// ============================================================================
class VisBaseEntity_cl : public VisObject3D_cl {
public:
    // === Skinning Mode ===

    // SetFixedSkinningMode - Set fixed skinning mode
    // IDA: ?SetFixedSkinningMode@VisBaseEntity_cl@@UEAAXW4VisSkinningMode_e@@@Z @ 0x140189430
    virtual void SetFixedSkinningMode(VisSkinningMode_e eType) {
        m_eFixedSkinningMode = eType;
    }

    // GetFixedSkinningMode - Get current skinning mode
    // IDA: ?GetFixedSkinningMode@VisBaseEntity_cl@@UEAA?AW4VisSkinningMode_e@@XZ @ 0x140189450
    virtual VisSkinningMode_e GetFixedSkinningMode() {
        return m_eFixedSkinningMode;
    }

    // === Virtual Destructor ===
    virtual ~VisBaseEntity_cl() = default;

protected:
    // Member variables
    VisSkinningMode_e m_eFixedSkinningMode = VisSkinningMode_e::None;
};
