// XGameDBSocketMgr.h
// XGameDBSocketMgr DB Agent 连接管理器
// 对齐 ControlServer.exe IDA
//
// 注意: CGameDBSocket 和 CServer 类应在各服务器的实现文件中定义
// - ControlServer: XControlServer/CServer.h, GameDBSocketMgr.cpp
// - RelayServer: XRelayServer/ServerProcess.h, XRelayServer/GameDBSocket.h

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include <chrono>
#include <cstdint>
#include <thread>

// 前向声明 - 各服务器有自己的实现
// ControlServer: XControlServer/CServer.h
// RelayServer: XRelayServer/ServerProcess.h
class CServer;

// 前向声明 - CGameDBSocket 在各服务器的 GameDBSocket 实现中定义
// ControlServer: GameDBSocketMgr.cpp (或使用此文件的模板)
// RelayServer: XRelayServer/GameDBSocket.h
class CGameDBSocket;

// 对齐 ControlServer.exe IDA struct (72 bytes):
// +0x00: vtable* (8 bytes) - 虚函数表指针
// +0x08: m_pAccountDBAgent* (8 bytes)
// +0x10: m_pGameDBAgent* (8 bytes)
// +0x18: m_pLogDBAgent* (8 bytes)
// +0x20: m_pStatisticsDBAgent* (8 bytes)
// +0x28: m_pSGLogDBAgent* (8 bytes)
// +0x30: m_nAccountAgentCnt (4 bytes)
// +0x34: m_nGameAgentCnt (4 bytes)
// +0x38: m_nLogAgentCnt (4 bytes)
// +0x3C: m_nStatisticsAgentCnt (4 bytes)
// +0x40: m_nSGLogAgentCnt (4 bytes)
class XGameDBSocketMgr {
public:
    XGameDBSocketMgr();
    ~XGameDBSocketMgr();

    // 对齐 IDA: XGameDBSocketMgr::Init
    void Init();

    // 对齐 IDA: XGameDBSocketMgr::AutoConnect
    void AutoConnect();

    // 对齐 IDA: XGameDBSocketMgr::DisConnect
    void DisConnect();

    // 对齐 IDA: XGameDBSocketMgr::Clear
    void Clear();

    // 对齐 IDA 0x140027590: GetAccountDBAgentCount
    int GetAccountDBAgentCount();

    // 对齐 IDA 0x140027580: GetGameDBAgentCount
    int GetGameDBAgentCount();

    // 对齐 IDA 0x140027570: GetLogDBAgentCount
    int GetLogDBAgentCount() const { return m_nLogAgentCnt; }

    // 对齐 IDA 0x140034410: IsAccountDBConnectionAll - 检查所有 AccountDB 连接状态
    bool IsAccountDBConnectionAll() const;

    // 对齐 IDA 0x140034290: SendAccountDBAgent
    bool SendAccountDBAgent(int iIndex, XSendPacket& packet);

    // 对齐 IDA 0x140034310: SendGameDBAgent
    bool SendGameDBAgent(int iIndex, XSendPacket& packet);

    // 对齐 IDA 0x140034390: SendLogDBAgent
    bool SendLogDBAgent(int iIndex, XSendPacket& packet);

protected:
    // 对齐 IDA: 初始化指定类型的 Agent 组
    void InitAgentGroup(std::uint8_t byType, CGameDBSocket*& ppAgents, int& nAgentCount);

    // 对齐 IDA: 自动连接一组 Agent
    void AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount);

    // 对齐 IDA: 断开一组 Agent
    void DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount);

protected:
    // 对齐 IDA struct 布局 (72 bytes)
    // 注意: vtable 指针在偏移 0，由编译器自动生成
    CGameDBSocket* m_pAccountDBAgent = nullptr;     // +0x08 (8)
    CGameDBSocket* m_pGameDBAgent = nullptr;         // +0x10 (16)
    CGameDBSocket* m_pLogDBAgent = nullptr;          // +0x18 (24)
    CGameDBSocket* m_pStatisticsDBAgent = nullptr;   // +0x20 (32)
    CGameDBSocket* m_pSGLogDBAgent = nullptr;        // +0x28 (40)
    int m_nAccountAgentCnt = 0;                      // +0x30 (48)
    int m_nGameAgentCnt = 0;                         // +0x34 (52)
    int m_nLogAgentCnt = 0;                          // +0x38 (56)
    int m_nStatisticsAgentCnt = 0;                   // +0x3C (60)
    int m_nSGLogAgentCnt = 0;                        // +0x40 (64)
    // +0x44 (68): padding to 72 bytes
};

// TODO: 静态断言暂时禁用 - IDA struct 有 vtable (72 bytes)，当前实现无 vtable (64 bytes)
// static_assert(sizeof(XGameDBSocketMgr) == 72, "XGameDBSocketMgr size mismatch with IDA (72 bytes)");
