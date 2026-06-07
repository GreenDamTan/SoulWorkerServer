// hkvMath.h
// hkvMath - Havok Vision Engine math utilities
// IDA decompilation from GameServer.exe
//
// Functions:
// - hkvMath::Deg2Rad (0x14009f470)

#pragma once

// ============================================================================
// hkvMath - Math utility class
// Static utility functions for common math operations
// ============================================================================
class hkvMath {
public:
    // === Angle Conversion ===

    // Deg2Rad - Convert degrees to radians
    // IDA: ?Deg2Rad@hkvMath@@SAMM@Z @ 0x14009F470
    static float Deg2Rad(float f)
    {
        // IDA: return 0.017453292 * f;
        // 0.017453292 = PI / 180
        return f * 0.017453292f;
    }

    // Rad2Deg - Convert radians to degrees
    static float Rad2Deg(float f)
    {
        return f * 57.2957795f;  // 180 / PI
    }

    // === Constants ===
    static constexpr float PI = 3.14159265f;
    static constexpr float DEG_TO_RAD = 0.017453292f;
    static constexpr float RAD_TO_DEG = 57.2957795f;
};
