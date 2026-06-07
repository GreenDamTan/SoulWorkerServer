// ChatProcess.h
// Chat system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"
#include <map>
#include <string>

class CUser;
class XPacket;
struct PS_CHAT_ITEM_LINK_FOR_SERVER;
struct PS_CHAT_WHISPER;

// GM command handler function type
typedef unsigned char (*GMCommandHandler)(CUser* pUser, wchar_t* szParam1, wchar_t* szParam2,
    wchar_t* szParam3, wchar_t* szParam4, int* pnLogParam, __int64& n64LogParam);

// CString wrapper for map key (using std::wstring for portability)
using GMCommandMap = std::map<std::wstring, GMCommandHandler>;

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
    // 对齐 IDA: ?GMCommandProcess@CChatProcess@@IEAAEV?$CStringT@_WV?$StrTraitATL@_WV?$ChTraitsCRT@_W@ATL@@@ATL@@@ATL@@PEAVCUser@@PEAHAEA_J@Z @ 0x1403E7BE0
    bool GMCommandProcess(const CString& strCommand, CUser* pUser, int* pnResult, __int64& n64LogParam);
    void InitGMCommand();
    void InitCommand_Debug();

private:
    // GM command map - 对齐 IDA: m_mapGMCommand
    GMCommandMap m_mapGMCommand;
};
