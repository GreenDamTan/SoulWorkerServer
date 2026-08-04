// hkvMath.h
// hkvMath - Havok Vision Engine math utilities
// IDA decompilation from GameServer.exe
//
// Functions:
// - hkvMath::Deg2Rad (0x14009f470)
// - hkvMath::Rad2Deg (0x1402c7290)
// - hkvMath::isFloatEqual (0x140364350)
// - hkvMath::clamp<float> (0x140375260)

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
    // IDA: ?Rad2Deg@hkvMath@@SAMM@Z @ 0x1402c7290
    static float Rad2Deg(float f)
    {
        // IDA: return 57.295776 * f;
        // 57.295776 = 180 / PI
        return f * 57.295776f;
    }

    // === Float Comparison ===

    // isFloatEqual - Compare two floats with epsilon tolerance
    // IDA: ?isFloatEqual@hkvMath@@SA_NMMM@Z @ 0x140364350
    static bool isFloatEqual(float f1, float f2, float fEpsilon)
    {
        if (fEpsilon == 0.0f)
            return f1 == f2;
        return f1 >= (f2 - fEpsilon) && (f2 + fEpsilon) >= f1;
    }

    // === Clamping ===

    // clamp - Clamp a value to a range
    // IDA: ??$clamp@M@hkvMath@@SAMMMM@Z @ 0x140375260
    template<typename T>
    static T clamp(T val, T tMin, T tMax)
    {
        if (tMin > val)
            return tMin;
        if (val > tMax)
            return tMax;
        return val;
    }

    // === Min/Max ===

    // Max - Return the maximum of two values
    // IDA: ??$Max@H@hkvMath@@SAHHH@Z @ 0x140376fc0
    template<typename T>
    static T Max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    // Min - Return the minimum of two values
    template<typename T>
    static T Min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    // === Constants ===
    static constexpr float PI = 3.14159265f;
    static constexpr float DEG_TO_RAD = 0.017453292f;
    static constexpr float RAD_TO_DEG = 57.2957795f;
};
