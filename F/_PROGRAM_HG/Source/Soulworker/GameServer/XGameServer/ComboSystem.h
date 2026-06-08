#pragma once

#include <cstdint>

// ============================================================================
// TB_COMBO_BUFF - Combo Buff Table Structure
// IDA: GameServer.exe symbol TB_COMBO_BUFF
// ============================================================================
#pragma pack(push, 1)
struct TB_COMBO_BUFF {
    std::uint16_t wID;                    // Combo buff ID
    std::uint16_t Fixed_Buff_ID;          // Fixed buff ID to apply
    std::uint8_t Get_RBuff_Count;         // Number of random buffs to apply
    std::uint8_t padding1;
    
    // Random buff IDs (8 slots)
    std::uint16_t RBuff_ID_00;
    std::uint16_t RBuff_ID_01;
    std::uint16_t RBuff_ID_02;
    std::uint16_t RBuff_ID_03;
    std::uint16_t RBuff_ID_04;
    std::uint16_t RBuff_ID_05;
    std::uint16_t RBuff_ID_06;
    std::uint16_t RBuff_ID_07;
    
    // Random buff rates (8 slots)
    std::uint32_t RBuff_Rate_00;
    std::uint32_t RBuff_Rate_01;
    std::uint32_t RBuff_Rate_02;
    std::uint32_t RBuff_Rate_03;
    std::uint32_t RBuff_Rate_04;
    std::uint32_t RBuff_Rate_05;
    std::uint32_t RBuff_Rate_06;
    std::uint32_t RBuff_Rate_07;
    
    std::uint32_t SV_Absorb;              // Absorb stat value
    char padding2[32];                     // Additional fields
};
#pragma pack(pop)

// ============================================================================
// SubordinationComboTrigger - Subordination Combo Trigger Class
// IDA: 0x14072D6C0 (constructor), 0x14072D760 (Serialize)
// ============================================================================
class SubordinationComboTrigger {
public:
    SubordinationComboTrigger();
    virtual ~SubordinationComboTrigger();
    
    // Serialize: IDA 0x14072D760
    virtual bool Serialize(class VChunkFile* pFile, int nMode);
    
private:
    char m_data[64];  // Placeholder for trigger data
};
