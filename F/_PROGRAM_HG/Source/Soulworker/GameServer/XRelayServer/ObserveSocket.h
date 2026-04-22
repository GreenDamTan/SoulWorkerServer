#pragma once

#include <cstdint>
#include "Soulworker/GameServer/XRelayServer/RelayControlSocket.h"

class XOption;

/// 对齐 IDA RelayServer.exe: CObserveSocket 继承 XRelaySocket
/// 定期向 ObserveAgent 报告服务器状态
class CObserveSocket : public XRelaySocket {
public:
    bool StartUp(XOption* pOption);
    bool OnUpdate(std::uint64_t currentTick,
                  const char* ip,
                  int port,
                  int userCount,
                  bool controlConnect,
                  bool communityConnect,
                  int maxThreadCount,
                  bool netCafe);

private:
    void OnConnect() override;
    void OnDisConnect() override;
    void OnNotConnect() override;
    void SetMyInfo(const XOption* option) override;

    SS_SERVER_INFO m_observeInfo{};
    bool m_bActivate = false;
};
