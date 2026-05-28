// SkillProcess.h
// Skill system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

class CSkillProcess : public TXProcess<CUser>
{
public:
    CSkillProcess();
    virtual ~CSkillProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Database response handler
    bool DBParse(XPacket& xPacket);

    // Skill request handlers
    bool ReqPassiveSkill(XPacket& xPacket);
    bool ReqPassiveSkillEnd(XPacket& xPacket);
    bool ReqSkillActive(XPacket& xPacket);
    bool ReqSkillAction(XPacket& xPacket);
    bool ReqCallOutSkillActive(XPacket& xPacket);
    bool ReqActiveBroachEffect(XPacket& xPacket);
    bool ReqAkashicRecord(XPacket& xPacket);
    bool ReqProjectile(XPacket& xPacket);
    bool ReqTrapPosUpdate(XPacket& xPacket);
    bool ReqPreTargetList(XPacket& xPacket);
    bool ReqChain(XPacket& xPacket);
    bool ReqSkillKeypress(XPacket& xPacket);
    bool ReqSkillTargetChange(XPacket& xPacket);
    bool ReqSkillWarpPosition(XPacket& xPacket);
    bool ReqSkillSyncPosition(XPacket& xPacket);
    bool ReqSkillSuboInput(XPacket& xPacket);
    bool ReqSkillChargingStart(XPacket& xPacket);
    bool ReqSkillChargingEnd(XPacket& xPacket);
    bool ReqSkillChargingBothInput(XPacket& xPacket);
    bool ReqSkillLearn(XPacket& xPacket);
    bool ReqSkillReset(XPacket& xPacket);
    bool ReqSkillAddDeckSlot(XPacket& xPacket);
    bool ReqSkillUpdateDeck(XPacket& xPacket);
    bool ReqDivergenceLearn(XPacket& xPacket);
    bool ReqSkillDeckBonus(XPacket& xPacket);
    bool ReqSkillResetTaget(XPacket& xPacket);
    bool ReqDeckPageActive(XPacket& xPacket);
    bool ReqDeckPageName(XPacket& xPacket);
    bool ReqSkillDeckPageOpen(XPacket& xPacket);

    // Database response handlers
    bool ResSkillLoad(XPacket& xPacket);
    bool ResBoosterLoad(XPacket& xPacket);
    bool ResSkillAddDeckSlot(XPacket& xPacket);
    bool ResSkillDeckPageOpen(XPacket& xPacket);
};
