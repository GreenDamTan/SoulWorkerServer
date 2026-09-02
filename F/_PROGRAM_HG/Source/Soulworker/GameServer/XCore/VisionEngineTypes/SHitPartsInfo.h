// SHitPartsInfo.h
// Hit parts info structure for CMoverEx::m_sHitParts
// PDB UDT 0x2B186 (fieldlist 0x2B185), LF_STRUCTURE PACKED, Size = 13:
//   dwTableID(ulong) +0, byPartsID(uchar) +4, iCurHP(int) +5, iMaxHP(int) +9
// (此前恢复期臆造的 m_byHitPartType/m_byHitPartIndex/m_fHitPartHeight/
//  m_fHitPartRadius/m_reserved 布局已按 PDB 删除)

#pragma once

#include <cstdint>

// PDB: PACKED 结构, 禁止编译器填充
#pragma pack(push, 1)
struct SHitPartsInfo {
    unsigned int dwTableID = 0;   // +0
    unsigned char byPartsID = 0;  // +4
    int iCurHP = 0;               // +5
    int iMaxHP = 0;               // +9

    // PDB fieldlist 0x2B185 list[4]: SHitPartsInfo() VANILLA ctor (index 0x2B184)
    SHitPartsInfo() {}
};
#pragma pack(pop)

static_assert(sizeof(SHitPartsInfo) == 13, "SHitPartsInfo size must match PDB (13)");
static_assert(offsetof(SHitPartsInfo, byPartsID) == 4, "SHitPartsInfo.byPartsID offset mismatch");
static_assert(offsetof(SHitPartsInfo, iCurHP) == 5, "SHitPartsInfo.iCurHP offset mismatch");
static_assert(offsetof(SHitPartsInfo, iMaxHP) == 9, "SHitPartsInfo.iMaxHP offset mismatch");
