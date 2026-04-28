// WorldManager.h
// ControlServer.exe 世界管理器类定义

#pragma once

// 必须在包含其他头文件前定义 Windows 宏
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#endif

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h" // CFSRWLock defined in GreenDamTan_ClientBase.h
#include <map>
#include <memory>
#include <utility>

// 前向声明
class CServer;
class CWorldMode;
class CChannelOfMap;
class CMapWithChannel;
struct PS_CREATE_MAP_LIST;
struct PS_ENTER_MAP_REQ;
struct ST_PARTY_INFO;

// 使用 std::tr1 命名空间 (VS2010 兼容)
namespace std { namespace tr1 = std; }

// ============================================================================
// ST_CHANNEL_MAP_INFO - 频道地图信息结构
// ============================================================================
struct ST_CHANNEL_MAP_INFO
{
    int nCount;                         // 计数
    std::map<int, UXMapID> mapUXMapID;  // UXMapID映射

    ST_CHANNEL_MAP_INFO();
    ~ST_CHANNEL_MAP_INFO();
};

// ============================================================================
// CChannelOfMap - 地图频道
// ============================================================================
class CChannelOfMap
{
public:
    CChannelOfMap();
    CChannelOfMap(CServer* pServer, UXMapID uxMapID, int nMaxUserCount, int nCurUserCount);
    ~CChannelOfMap();

    // 用户计数
    void AddUserCount(int nUserID, int nAdd);
    void AddSelectUser(int nUserID);

    // 频道状态
    void ChangeChannelState();
    int CalculateChannelState(int nBeforeState, bool* pbChange);
    bool IsOKToEnter() const;
    int GetServerState() const;
    bool IsState() const;

    // 获取信息
    UXMapID* GetMapID(UXMapID* pMapID) const;
    int GetTotalCount();
    int GetMaxUserCount() const { return m_nMaxUserCount; }
    CServer* GetServer() const { return m_pServer; }

    // 初始化
    void Init(CServer* pServer, UXMapID uxMapID, int nMaxUserCount, int nCurUserCount);

private:
    // 成员变量 (总大小: 64 bytes)
    UXMapID m_uxMapID;                          // +0x00: 地图ID (8 bytes)
    CServer* m_pServer;                         // +0x08: 所属服务器指针 (8 bytes)
    int m_nState;                               // +0x10: 频道状态 (4 bytes)
    int m_nUserCount;                           // +0x14: 用户计数 (4 bytes)
    int m_nMaxUserCount;                        // +0x18: 最大用户数 (4 bytes)
    std::map<int, __int64> m_mapSelectUser;     // +0x20: 选择用户映射 (32 bytes)
};

// ============================================================================
// CMapWithChannel - 带频道的地图管理
// ============================================================================
class CMapWithChannel
{
public:
    CMapWithChannel(WORD wMapID);
    virtual ~CMapWithChannel();

    // 添加/删除频道
    bool AddChannel(CServer* pServer, UXMapID uxMapID, int nMaxUserCount, int nCurUserCount);
    void DeleteChannel(CServer* pServer);

    // 查找频道
    std::tr1::shared_ptr<CChannelOfMap> FindChannel(
        DWORD dwServerID, ST_PARTY_INFO* stPartyInfo, DWORD dwActorID, bool bRaid);
    std::tr1::shared_ptr<CChannelOfMap> FindChannel(int nChannel);
    std::tr1::shared_ptr<CChannelOfMap> FindChannelByParty(
        ST_PARTY_INFO* stPartyInfo, DWORD dwActorID, bool bRaid);
    std::tr1::shared_ptr<CChannelOfMap> FindChannelByServerID(
        DWORD dwServerID, DWORD dwActorID, bool bRaid);
    std::tr1::shared_ptr<CChannelOfMap> FindChannelByUserCount(
        DWORD dwActorID, bool bRaid);
    std::tr1::shared_ptr<CChannelOfMap> CheckChannelUserCount(
        int nChannel, ST_CHANNEL_MAP_INFO* pInfo, bool bRaid);
    std::tr1::shared_ptr<CChannelOfMap> FindChannelUserLow(
        ST_CHANNEL_MAP_INFO* pInfo, int nChannel, bool bRaid);
    std::tr1::shared_ptr<CChannelOfMap> FindChannelUserLow(bool bRaid);

    // 获取频道信息
    void GetChannel(PS_CHANNEL_INFO& stChannel);

    // 发送
    void SendDBAddChannelMap(std::tr1::shared_ptr<CChannelOfMap> pChannel);
    void SendServerAddChannelMap(CServer* pServer, UXMapID uxMapID, bool bResult);

    // 获取数量
    int GetCount() const;

private:
    // 成员变量 (总大小: 88 bytes)
    void* __vftable;                                        // +0x00: 虚函数表指针 (8 bytes)
    std::map<int, std::tr1::shared_ptr<CChannelOfMap>> m_mapChannel;    // +0x08: 频道映射 (32 bytes)
    std::map<int, ST_CHANNEL_MAP_INFO*> m_mapChannelForServer;          // +0x28: 服务器频道映射 (32 bytes)
    int m_nTableID;                                         // +0x48: 地图表ID (4 bytes)
    int m_nMaxChannelCount;                                 // +0x4C: 最大频道数 (4 bytes)
    int m_nChannel_Index;                                   // +0x50: 频道索引 (4 bytes)
    int m_nStartChannel;                                    // +0x54: 起始频道 (4 bytes)
};

// ============================================================================
// CWorldManager - 世界管理器
// ============================================================================
class CWorldManager
{
public:
    CWorldManager();
    ~CWorldManager();

    // 添加/删除地图
    void AddMap(CServer* pServer, PS_CREATE_MAP_LIST& stMapList);
    void DeleteMap(CServer* pServer);

    // 进入地图请求
    bool ReqEnterMap(CServer* pServer, PS_ENTER_MAP_REQ& stEnterReq);
    bool ReqEnterMapToOther(CServer* pServer, PS_ENTER_MAP_REQ& stEnterReq,
                            STPosInfo stPosInfo, DWORD dwParam);
    void ReqChangeChannel(CServer* pServer, PS_ENTER_MAP_REQ& stEnterReq);

    // 频道信息
    void SendChannelInfoAll();
    void UpdateUserMap(int dwActorID, UXMapID uxOldMapID, UXMapID uxNewMapID);
    void AddChannelOfMap(UXMapID uxMapID, std::tr1::shared_ptr<CChannelOfMap> pChannel);
    void RemoveChannelOfMap(UXMapID uxMapID);

private:
    // 成员变量 (总大小: 72 bytes)
    CFSRWLock m_rwLock;                                             // +0x00: 读写锁 (8 bytes)
    std::map<int, std::tr1::shared_ptr<CMapWithChannel>> m_mapMapInfo;        // +0x08: 地图信息映射 (32 bytes)
    std::map<UXMapID, std::tr1::shared_ptr<CChannelOfMap>> m_mapChannelOfMap; // +0x28: 频道映射 (32 bytes)
};

// 对齐 IDA: PS_CHANNEL_INFO 包序列化
inline XPacket& operator<<(XPacket& packet, const PS_CHANNEL_INFO& value) {
    packet.XParse << value.wMapID;
    packet.XParse << static_cast<int>(static_cast<int>(value.vecChannel.size()));
    for (const auto& ch : value.vecChannel) {
        packet.XParse << ch.wChannel;
        packet.XParse << ch.byChannelState;
    }
    return packet;
}
