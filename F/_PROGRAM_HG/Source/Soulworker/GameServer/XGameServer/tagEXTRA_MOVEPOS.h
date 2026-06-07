// tagEXTRA_MOVEPOS.h
// Extra movement position structure
// IDA decompilation from GameServer.exe
//
// Functions:
// - tagEXTRA_MOVEPOS::Clear (0x140189340)

#pragma once

// ============================================================================
// tagEXTRA_MOVEPOS - Extra movement position data
// Used for additional movement tracking
// ============================================================================
struct tagEXTRA_MOVEPOS {
    float fMovingTime;      // Moving time
    float fRemainTime;      // Remaining time
    float x;                // X position
    float y;                // Y position (or Z in 3D)
    std::uint8_t bySettingType;  // Setting type

    // === Clear - Clear extra move position data
    // IDA: ?Clear@tagEXTRA_MOVEPOS@@QEAAXXZ @ 0x140189340
    void Clear() {
        fMovingTime = 0.0f;
        fRemainTime = 0.0f;
        x = 0.0f;
        y = 0.0f;
        bySettingType = 0;
    }
};
