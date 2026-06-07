// STEquipBase.h
// STEquipBase - Equipment base structure
// IDA decompilation from GameServer.exe
//
// Functions:
// - STEquipBase::Init (0x1400f9f30)

#pragma once

#include <cstdint>

// ============================================================================
// STEquipBase - Base equipment structure
// Used for item equipment data
// ============================================================================
struct STEquipBase {
    std::uint32_t dwItemID = static_cast<std::uint32_t>(-1);  // Item ID (-1 = invalid)
    std::uint8_t byUpgrade = 0;                                 // Upgrade level

    // === Initialization ===

    // Init - Initialize equipment base data
    // IDA: ?Init@STEquipBase@@QEAAXXZ @ 0x1400F9F30
    void Init()
    {
        // IDA code:
        // void __fastcall STEquipBase::Init(STEquipBase *this)
        // {
        //   this->dwItemID = -1;
        //   this->byUpgrade = 0;
        // }
        dwItemID = static_cast<std::uint32_t>(-1);
        byUpgrade = 0;
    }
};
