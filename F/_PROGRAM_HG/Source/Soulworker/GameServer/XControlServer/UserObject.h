// UserObject.h
// CUserObject 用户对象类定义

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
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <string>
#include <memory>

// 前向声明
class CServer;
struct ST_ENTER_WORLD_MODE_INFO;
struct ST_WORLD_MODE;

// 使用 std::tr1 命名空间 (VS2010 兼容)
namespace std { namespace tr1 = std; }

// ============================================================================
// CUserObject - 用户对象类
// 结构大小: 888 bytes (0x378)
// ============================================================================
class CUserObject
{
public:
    // 构造函数
    CUserObject(CServer* pServer, STCharInfo& stInfo, UXMapID uxMapID);
    ~CUserObject();

    // boost::multi_index 需要的 getter 方法
    const DWORD GetCID() const { return m_stCharInfo.uxActorID.dwActorID; }
    const DWORD GetUAID() const { return m_stCharInfo.dwUAID; }
    const DWORD GetServerID() const { return m_dwServerID; }
    const std::wstring& GetName() const;  // 从 m_stCharInfo.stBaseInfo 获取

    // Setter 方法
    void SetWorldState(int nState, ULONGLONG dwWaitTick, UXMapID uxMapID);
    void SetPartyInfo(ST_PARTY_INFO& stPartyInfo);
    void SetBeforeServerID(int nServerID);
    void SetServer(CServer* pServer);
    void SetMapIns(UXMapID uxMapID);
    void SetAuthType(BYTE byAuthType);
    void SetGroupIDTick(ULONGLONG dwTick);
    void SetBlockType(BYTE byBlockType);
    void SetAuthSessionID(__int64 biSessionID);
    void SetTradePWState(BYTE byState);
    void SetIP(DWORD dwIP);
    void SetName(const wchar_t* szName);
    void SetEnterWorldModeInfo(ST_WORLD_MODE stMode);
    void SetConnectTick(ULONGLONG dwTick) { m_dwConnectTick = dwTick; }

    // Getter 方法
    int GetBeforeServerID() { return m_nBeforeServerID; }
    BYTE GetAuthType() { return m_byAuthType; }
    ST_PARTY_INFO GetPartyInfo() { return m_stPartyInfo; }
    void GetPartyInfo(ST_PARTY_INFO* pPartyInfo) { if (pPartyInfo) *pPartyInfo = m_stPartyInfo; }
    UXMapID GetMapIns() { return m_uxMapID; }
    void GetMapIns(UXMapID* pMapID) { if (pMapID) *pMapID = m_uxMapID; }
    BYTE GetTradePWState() const { return m_byTradePasswordState; }
    BYTE GetBlockType() { return m_byBlockType; }
    __int64 GetAuthSessionID() { return m_biAuthSessionID; }
    ULONGLONG GetGroupIDTick() { return m_dw64GroupIDTick; }
    void GetEnterWorldModeInfo(ST_ENTER_WORLD_MODE_INFO& stInfo);
    ULONGLONG GetConnectTick() const { return m_dwConnectTick; }

    // 对齐 IDA: CChannelOfMap::IsState 返回值 (用户在线状态)
    bool IsState() const { return m_nWorldState == 1; }

    // 功能方法
    void SendPacket(XSendPacket& packet);
    bool CanChangeWorld();
    void ResetWorldState(UXMapID uxMapID);
    void LoadEnterWorldModeInfo(ST_ENTER_WORLD_MODE_INFO stInfo);

private:
    // 成员变量 (总大小: 888 bytes = 0x378)
    // 布局严格匹配 IDA 反编译结果

    ULONGLONG m_dwConnectTick;                    // +0x000: 连接时间戳 (8 bytes)
    CServer* m_pServer;                           // +0x008: 所属服务器 (8 bytes)
    DWORD m_dwIP;                                 // +0x010: IP地址 (4 bytes)
    // +0x014-0x017: padding (4 bytes)
    STCharInfo m_stCharInfo;                      // +0x018: 角色信息 (752 bytes)
    UXMapID m_uxMapID;                            // +0x308: 当前地图 (8 bytes)
    DWORD m_dwServerID;                           // +0x310: 服务器ID (4 bytes)
    ST_PARTY_INFO m_stPartyInfo;                  // +0x314: 队伍信息 (8 bytes)
    BYTE m_byTradePasswordState;                  // +0x31C: 交易密码状态 (1 byte)
    // +0x31D-0x31F: padding (3 bytes)
    __int64 m_biAuthSessionID;                    // +0x320: 认证会话ID (8 bytes)
    BYTE m_byBlockType;                           // +0x328: 封锁类型 (1 byte)
    // +0x329-0x32F: padding (7 bytes)
    ULONGLONG m_dw64GroupIDTick;                  // +0x330: 组ID滴答 (8 bytes)
    int m_nWorldState;                            // +0x338: 世界状态 (4 bytes)
    int m_nBeforeServerID;                        // +0x33C: 前一服务器ID (4 bytes)
    ULONGLONG m_dwWorldChangeWaitTick;            // +0x340: 世界变更等待时间 (8 bytes)
    BYTE m_byAuthType;                            // +0x348: 认证类型 (1 byte)
    // +0x349-0x34F: padding (7 bytes)
    UXMapID m_uxTransMapID;                       // +0x350: 过渡地图ID (8 bytes)
    ST_ENTER_WORLD_MODE_INFO m_stEnterWorldModeInfo; // +0x358: 进入世界模式信息 (32 bytes)
};
