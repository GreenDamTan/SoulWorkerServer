// GameSockets.h
// Game Server Socket Stubs

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include <cstdint>

// CCommunitySocket - Community Socket 存根
class CCommunitySocket : public XRelaySocket {
public:
    CCommunitySocket() = default;
    ~CCommunitySocket() = default;

    void SetMyInfo(XOption* pOption) {}
    bool Init_2(E_POOL_ID ePoolID, const char* szIP, std::int16_t shPort) { return true; }
    static bool IsCachingLoadFinish(CCommunitySocket* pSocket) { return true; }
    static bool IsCanSend(CCommunitySocket* pSocket) { return true; }
};

// CGameControlSocket - Game Control Socket 存根
class CGameControlSocket : public XRelaySocket {
public:
    CGameControlSocket() = default;
    ~CGameControlSocket() = default;

    void SetMyInfo(XOption* pOption) {}
    bool Init_2(E_POOL_ID ePoolID, const char* szIP, std::int16_t shPort) { return true; }
    static bool IsCachingLoadFinish(CGameControlSocket* pSocket) { return true; }
    static bool IsCanSend(CGameControlSocket* pSocket) { return true; }
};
