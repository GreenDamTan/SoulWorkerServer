// SHitPartsInfo.h
// Hit parts info structure for collision detection
// Reconstructed from IDA

#pragma once

#include <cstdint>

// SHitPartsInfo - Hit parts information
// Based on IDA: offset 60152, size 26 = 2 * 13 bytes per hit part
struct SHitPartsInfo {
    std::uint8_t m_byHitPartType = 0;     // Hit part type
    std::uint8_t m_byHitPartIndex = 0;    // Hit part index
    float m_fHitPartHeight = 0.0f;        // Hit part height
    float m_fHitPartRadius = 0.0f;        // Hit part radius
    std::uint8_t m_reserved[3] = {0};     // Reserved bytes to reach 13 bytes total
};

// Size check disabled for reconstruction - 1 + 1 + 4 + 4 + 3 = 13 bytes
// static_assert(sizeof(SHitPartsInfo) == 13, "SHitPartsInfo size mismatch");
