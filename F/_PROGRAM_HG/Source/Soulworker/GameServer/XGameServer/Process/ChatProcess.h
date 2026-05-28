// ChatProcess.h
// Chat system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;
struct PS_CHAT_ITEM_LINK_FOR_SERVER;
struct PS_CHAT_WHISPER;

class CChatProcess : public TXProcess<CUser>
{
public:
    CChatProcess();
    virtual ~CChatProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Chat request handlers
    bool ReqNormalChatting(XPacket& xPacket);
    bool ReqChatWhisper(XPacket& xPacket);
    bool ReqChatTrade(XPacket& xPacket);
    bool ReqChatNotice(XPacket& xPacket);
    bool ReqChatMegaPhone(XPacket& xPacket);
    bool ReqGMCommand(XPacket& xPacket);

    // Static helper functions
    static bool ProcessNormal(CUser* pUser, wchar_t* szMessage, PS_CHAT_ITEM_LINK_FOR_SERVER* psItemLink);
    static bool ProcessParty(CUser* pUser, wchar_t* szMessage, PS_CHAT_ITEM_LINK_FOR_SERVER* psItemLink);
    static bool ProcessLeague(CUser* pUser, wchar_t* szMessage, PS_CHAT_ITEM_LINK_FOR_SERVER* psItemLink);

    // Whisper handling
    void SendChatWhisper(CUser* pUser, PS_CHAT_WHISPER& psWhisper, PS_CHAT_ITEM_LINK_FOR_SERVER psItemLink);

    // GM command processing
    bool GMCommandProcess(const CString& strCommand, CUser* pUser, int* pnResult, bool& bSuccess);
    void InitGMCommand();
    void InitCommand_Debug();
};
