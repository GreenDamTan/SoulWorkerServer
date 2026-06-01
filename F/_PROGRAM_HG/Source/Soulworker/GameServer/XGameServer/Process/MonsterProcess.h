// MonsterProcess.h
// Monster control packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XIOCPServer/TXProcess.h"

class CUser;
class XPacket;

// ============================================================================
// ST_MONSTER_WRONG_POS - Monster wrong position report structure
// IDA confirmed size: 24 bytes
// ============================================================================
struct ST_MONSTER_WRONG_POS {
    std::uint32_t dwMonsterActorID;  // offset 0: Monster actor ID
    std::uint32_t dwAreaID;          // offset 4: Area ID
    std::uint16_t wType;             // offset 8: Type/report category
    // offset 10: padding (2 bytes)
    struct {
        float x, y, z;
    } vClientPos;                    // offset 12: Client-side position (12 bytes)
};

// CMonsterProcess - handles monster control packets (main cmd 0x17)
// Sub commands:
//   0x21 ('!') - ReqTargetChange
//   0x41 ('A') - ReqDefensiveWeaponStart
//   0x43 ('C') - ReqDefensiveWeaponEnd
//   0x45 ('E') - ReqDefensiveWeaponAttack
//   0x52 ('R') - ReqControlMonsterAttack
//   0x55 ('U') - ReqMonsterWrongPos

class CMonsterProcess : public TXProcess<CUser>
{
public:
    CMonsterProcess();
    virtual ~CMonsterProcess();

    // Main packet parser - dispatches based on sub command
    virtual bool Parse(XPacket& xPacket) override;

protected:
    // ReqTargetChange IDA 0x140519060 - Request target change
    bool ReqTargetChange(XPacket& xPacket);

    // ReqDefensiveWeaponStart IDA 0x140519170 - Start defensive weapon mode
    bool ReqDefensiveWeaponStart(XPacket& xPacket);

    // ReqDefensiveWeaponEnd IDA 0x140519550 - End defensive weapon mode
    bool ReqDefensiveWeaponEnd(XPacket& xPacket);

    // ReqDefensiveWeaponAttack IDA 0x1405198F0 - Defensive weapon attack
    bool ReqDefensiveWeaponAttack(XPacket& xPacket);

    // ReqControlMonsterAttack IDA 0x140519B00 - Control monster attack
    bool ReqControlMonsterAttack(XPacket& xPacket);

    // ReqMonsterWrongPos IDA 0x140519DF0 - Report monster wrong position
    bool ReqMonsterWrongPos(XPacket& xPacket);
};
