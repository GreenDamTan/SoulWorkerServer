// QuestProcess.h
// Quest system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

class CQuestProcess : public TXProcess<CUser>
{
public:
    CQuestProcess();
    virtual ~CQuestProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Database response parser
    bool DBQuestParse(XPacket& xPacket);

    // Quest request handlers
    bool ReqQuestAccept(XPacket& xPacket);
    bool ReqQuestComplete(XPacket& xPacket);
    bool ReqQuestGiveUp(XPacket& xPacket);
    bool ReqQuestEventUpdate(XPacket& xPacket);
    bool ReqQuestHelper(XPacket& xPacket);
    bool ReqQuestFail(XPacket& xPacket);

    // Database response handlers
    bool ResLoadEpisode(XPacket& xPacket);
    bool ResQuestComplete(XPacket& xPacket);
    bool ResQuestDelete(XPacket& xPacket);
};
