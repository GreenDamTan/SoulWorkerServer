// ServerProcess.h - RelayServer
// 对齐 IDA RelayServer.exe: CServerProcess 只有 3 个 sub command

#pragma once

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <memory>
#include <map>

// 前向声明
class XRelayServer;

// 对齐 IDA RelayServer.exe: 服务器同步加载状态枚举
enum E_SERVER_SYNC_LOAD {
    E_SERVER_SYNC_LOAD_USER      = 1,  // 用户信息同步完成
    E_SERVER_SYNC_LOAD_MAZE_INFO = 2,  // 地图信息同步完成
};

// 对齐 IDA RelayServer: CServer 继承自 XClient
// 注意: ST_SYNC_INFO 已在 PSServer.h 中定义
class CServer : public XClient {
public:
    CServer() = default;
    virtual ~CServer() = default;

    // 对齐 IDA 0x1400D1F60: SetServerInfo - 设置服务器信息
    void SetServerInfo(const SS_SERVER_INFO& info);

    // 对齐 IDA 0x14002BF40: GetServerID - 服务器ID
    std::uint32_t GetServerID() const { return m_serverInfo.dwID; }
    void SetServerID(std::uint32_t dwID) { m_serverInfo.dwID = dwID; }

    // 对齐 IDA 0x1400C9670: GetServerType - 返回服务器类型
    int GetServerType() const { return m_serverInfo.nType; }

    // 对齐 IDA 0x1400C8A90: GetServerInfo - 返回服务器信息
    SS_SERVER_INFO* GetServerInfo() { return &m_serverInfo; }
    const SS_SERVER_INFO* GetServerInfo() const { return &m_serverInfo; }

    // 对齐 IDA 0x1400C8A70: GetUserCount - 返回当前用户数
    int GetUserCount() const { return m_serverInfo.nCurUser; }

    // 对齐 IDA 0x1400C89E0: IsSyncLoad - 检查同步加载状态
    // IDA: return this->m_dwSyncLoad == 3;
    bool IsSyncLoad() const { return m_dwSyncLoad == 3; }

    // 对齐 IDA 0x1400C8A20: IsRecvServerInfo - 检查是否已接收服务器信息
    // IDA: return this->m_stSyncInfo.SyncData == 0;
    bool IsRecvServerInfo() const { return m_stSyncInfo.SyncData == 0; }

    // 对齐 IDA 0x1400C9350: RecvUserInfo - 标记用户信息已接收
    // IDA: *(_BYTE *)&this->m_stSyncInfo.SyncData &= ~1u;
    void RecvUserInfo() { m_stSyncInfo.SyncData &= ~1u; }

    // 对齐 IDA 0x1400D1FF0: SetSyncLoad - 设置同步加载状态
    void SetSyncLoad(E_SERVER_SYNC_LOAD eServerSync);

    // TXDBSocket 需要: GetBlockType - 服务器不需要封禁检查，返回 0
    std::uint8_t GetBlockType() const { return 0; }

    // 发送数据 - 使用基类的 SendEx
    // void SendEx(XSendPacket& packet) 继承自 XClient

private:
    // 对齐 IDA: CServer 成员变量布局
    SS_SERVER_INFO m_serverInfo{};      // 服务器信息
    ST_SYNC_INFO m_stSyncInfo{};        // 同步状态信息 (已在 PSServer.h 定义)
    std::uint32_t m_dwSyncLoad = 0;     // 同步加载位标志 (1=USER, 2=MAZE_INFO)
};

// 对齐 IDA RelayServer: CServerProcess 只有 3 个方法
class CServerProcess : public TXProcess<CServer> {
public:
    CServerProcess() = default;
    virtual ~CServerProcess() = default;

    // 对齐 IDA 0x1400CE940: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

private:
    // 对齐 IDA 0x1400CE9C0: ReqCreateServer (sub 0x01)
    bool ReqCreateServer(XPacket& xPacket);

    // 对齐 IDA 0x1400CEA50: ReqUpdateServerInfo (sub 0x03)
    bool ReqUpdateServerInfo(XPacket& xPacket);

    // 对齐 IDA 0x1400CEA90: SyncUsersInfo (sub 0x33)
    bool SyncUsersInfo(XPacket& xPacket);
};