// hkvVec4.h
// hkvVec4 - 4D vector class (Havok Vision Engine), 16 bytes: 4 floats
// PDB LF_CLASS 0xF359 (GameServer.pdb.cvdump.types.txt)

#pragma once

// ============================================================================
// hkvVec4 - 4D vector (16 bytes)
// Used by ShaderChangeTrigger::vecParamValue (offset 552)
// ============================================================================
struct hkvVec4 {
    float x;
    float y;
    float z;
    float w;

    // === Constructors ===
    hkvVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    hkvVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    // === Static Factory ===
    static hkvVec4 ZeroVector() {
        return hkvVec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    // === Methods ===
    void setZero() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
};

static_assert(sizeof(hkvVec4) == 16, "hkvVec4 size mismatch - expected 16 bytes");
