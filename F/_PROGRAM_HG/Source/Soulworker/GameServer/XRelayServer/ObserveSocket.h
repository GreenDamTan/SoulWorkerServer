#pragma once

#include <cstdint>
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XRelayServer/RelayControlSocket.h"

class XOption;

/// 对齐 IDA RelayServer.exe: CObserveSocket 继承 XRelaySocket
/// 定期向 ObserveAgent 报告服务器状态
class CObserveSocket : public XRelaySocket {
public:
    CObserveSocket();

    bool StartUp(XOption* pOption);
    bool OnUpdate(std::uint64_t currentTick,
                  char* ip,  // 对齐 IDA: 非 const 指针
                  int port,
                  int userCount,
                  bool controlConnect,
                  bool communityConnect,  // 对齐 IDA: _N = bool
                  int maxThreadCount,
                  bool netCafe);  // 对齐 IDA: _N = bool

    // 对齐 IDA SendReportServerStatus: HH_N_NHPEADH_N = (int, int, bool, bool, int, char*, int, bool)
    void SendReportServerStatus(std::int32_t nServerType,
                                std::int32_t nUserCount,
                                bool bControlConnect,
                                bool bCommunityConnect,  // 对齐 IDA: _N = bool
                                std::int32_t nMaxThreadCount,
                                char* szIP,  // 对齐 IDA: 非 const 指针
                                std::int32_t nPort,
                                bool bNetCafe);  // 对齐 IDA: _N = bool

    // 对齐 IDA: 计算线程状态
    void CalculateThreadStatus(wchar_t* szLogicThread, std::int32_t nMaxThreadCount);

private:
    void OnConnect() override;
    void OnDisConnect() override;
    void OnNotConnect() override;
    // 对齐 IDA: SetMyInfo(PEAVXOption) = 非const指针
    void SetMyInfo(XOption* option) override;

    SS_SERVER_INFO m_observeInfo{};          // ObserveAgent 服务器信息
    bool m_bActivate = false;                // 是否激活
    std::uint32_t m_dwProcessID = 0;         // 进程 ID

    // 对齐 IDA: 线程 FPS 计时器（每个逻辑线程）
    mutable CFSRWLock m_rwThreadTickLock;                    // 保护线程 FPS tick 的读写锁
    std::uint64_t m_dwThreadFpsTick_Last[20] = {};   // 上次 FPS tick
    std::uint64_t m_dwThreadFpsTick_Now[20] = {};    // 当前 FPS tick
};
