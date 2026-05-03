#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

enum SYSTEM_TYPE : std::int32_t {
    SYSTEM_TYPE_NONE = 0,
    SYSTEM_TYPE_DEV = 1,
    SYSTEM_TYPE_TEST = 2,
    SYSTEM_TYPE_REAL = 3,
};

enum NATION_TYPE : std::int32_t {
    NATION_TYPE_NONE = 0,
    NATION_TYPE_KOR = 1,
    NATION_TYPE_JPN = 2,
    NATION_TYPE_CHN = 3,
    NATION_TYPE_TWN = 4,
};

enum BILLING_TYPE : std::int32_t {
    BILLING_TYPE_TEST = 0,
    BILLING_TYPE_REAL = 1,
};

enum SECURITY_TYPE : std::int32_t {
    SECURITY_OFF = 0,
    SECURITY_ON = 1,
};

enum RES_LOAD_TYPE : std::int32_t {
    RES_LOAD_TYPE_DB = 0,
    RES_LOAD_TYPE_FILE = 1,
};

struct CONTENTS_OPTION_INFO {
    int nOptionFlag = 0;
    bool bContents[E_SERVER_OPTION_MAX] = {};
};

struct ST_SERVER_INFO {
    char szIP[513] = {};
    char szName[21] = {};
    int nPort = 0;
    int nType = 0;
    int nMaxIOPool = 0;
    int nDBAgentCount = 0;
};

struct SERVER_SYSTEM_INFO {
    std::uint32_t dwID = 0;
    int nGroup = 0;
    int nType = 0;
    int nChannel = 0;
    int nChannelFlag = 0;
    int nState = 0;
    int nMaxUser = 0;
    int nIoThread = 0;
    int nLogicThread = 0;
    int nPort = 0;
    char szName[21] = {};
    char szIP[513] = {};
    char szPublicIP[513] = {};
    char szPath[260] = {};
    int nMaxIOPool = 0;
    int nLimitIOPool = 0;
    bool bUserAuth = false;
    char _pad0[3] = {};
    int nSubType = 0;
    std::array<ST_SERVER_INFO, 5> stDBAgentInfo{};
    std::array<ST_SERVER_INFO, 3> stAgentInfo{};
};

struct AGENT_SYSTEM_INFO {
    SERVER_SYSTEM_INFO stServerInfo{};
    int nDBThreadCount = 0;
    std::string strDNS;
};

struct CHANNEL_INFO {
    int nID = 0;
    std::uint16_t wFrom = 0;
    std::uint16_t wTo = 0;
};

struct OBJECT_POOL_INFO {
    int nPCMaxCount = 0;
    int nMonsterMaxCount = 0;
    int nNPCMaxCount = 0;
    int nDistrictMaxCount = 0;
    int nMazeMaxCount = 0;
    int nProjectileMaxCount = 0;
    int nTrapMaxCount = 0;
    int nInteractionMaxCount = 0;
    int nAkashicMaxCount = 0;
};

struct MOBILE_APP_INFO {
    char szMobileIP[15] = {};
    char _pad0 = '\0';
    int nMobilePort = 81;
};

#ifdef _WIN32
static_assert(sizeof(CONTENTS_OPTION_INFO) == 0x14, "CONTENTS_OPTION_INFO size must match PDB");
static_assert(sizeof(ST_SERVER_INFO) == 0x228, "ST_SERVER_INFO size must match PDB");
static_assert(sizeof(SERVER_SYSTEM_INFO) == 0x1694, "SERVER_SYSTEM_INFO size must match PDB");
static_assert(sizeof(CHANNEL_INFO) == 0x8, "CHANNEL_INFO size must match PDB");
static_assert(sizeof(OBJECT_POOL_INFO) == 0x24, "OBJECT_POOL_INFO size must match PDB");
static_assert(sizeof(MOBILE_APP_INFO) == 0x14, "MOBILE_APP_INFO size must match PDB");
#endif

class XOption {
public:
    bool Load(const char* szName);
    void ShowServerInfo();

    SYSTEM_TYPE GetSystemType() const { return m_eSystemType; }
    NATION_TYPE GetNationType() const { return m_eNationType; }
    BILLING_TYPE GetBillingType() const { return m_eBillingType; }
    SECURITY_TYPE GetSecurityType() const { return m_eSecurityType; }
    RES_LOAD_TYPE GetResLoadType() const { return m_eResLoadType; }

    void SetInitXignCode(bool init) { m_bInitXignCode = init; }
    bool IsInitXignCode() const { return m_bInitXignCode; }

    void SetName(const char* name) {
        std::memset(m_serverInfo.szName, 0, sizeof(m_serverInfo.szName));
        if (name) {
#ifdef _WIN32
            strncpy_s(m_serverInfo.szName, sizeof(m_serverInfo.szName), name, _TRUNCATE);
#else
            std::strncpy(m_serverInfo.szName, name, sizeof(m_serverInfo.szName) - 1);
#endif
        }
    }
    const char* GetName() const { return m_serverInfo.szName; }

    std::uint32_t GetServerID() const { return m_serverInfo.dwID; }
    void SetServerID(std::uint32_t serverId) { m_serverInfo.dwID = serverId; }

    std::uint16_t GetGroupID() const { return static_cast<std::uint16_t>(m_serverInfo.nGroup); }
    int GetType() const { return m_serverInfo.nType; }
    int GetSubType() const { return m_serverInfo.nSubType; }
    int GetChannel() const { return m_serverInfo.nChannel; }
    std::int16_t GetPort() const { return static_cast<std::int16_t>(m_serverInfo.nPort); }
    std::int16_t GetLogicThread() const { return static_cast<std::int16_t>(m_serverInfo.nLogicThread); }
    std::int16_t GetMaxConnect() const { return static_cast<std::int16_t>(m_serverInfo.nMaxUser); }
    int GetMaxIOPool() const { return m_serverInfo.nMaxIOPool; }
    OBJECT_POOL_INFO& GetPoolInfo();
    const OBJECT_POOL_INFO& GetPoolInfo() const;
    const char* GetIP() const { return m_serverInfo.szIP; }
    const char* GetPublicIP() const { return m_serverInfo.szPublicIP; }
    const char* GetResFilePath() const { return m_szResFilePath; }

    CONTENTS_OPTION_INFO* GetContentsOption() { return &m_ContentsOption; }
    const CONTENTS_OPTION_INFO* GetContentsOption() const { return &m_ContentsOption; }

    SERVER_SYSTEM_INFO& GetServerInfo() { return m_serverInfo; }
    const SERVER_SYSTEM_INFO& GetServerInfo() const { return m_serverInfo; }

    AGENT_SYSTEM_INFO& GetAgentInfo(std::size_t index) { return m_agentInfo[index]; }
    const AGENT_SYSTEM_INFO& GetAgentInfo(std::size_t index) const { return m_agentInfo[index]; }

    const char* GetDNS(int type) const {
        if (type < 0 || type >= static_cast<int>(m_strDNS.size())) {
            return "";
        }
        return m_strDNS[static_cast<std::size_t>(type)].c_str();
    }

    bool GetDBAgentInfo(std::uint16_t type,
                        char* ip,
                        std::int16_t* port,
                        char* name,
                        int* maxIOPoolCount,
                        int* dbAgentCount) const;
    bool GetServerPrivateIPAndPort(char* szServerName, char* szIP, std::uint16_t& sPort) const;
    bool GetAgentPrivateIPAndPort(int nType, char* szIP, std::uint16_t& sPort) const;
    bool GreenDamTan_GetServerSystemInfo(const char* szServerName, SERVER_SYSTEM_INFO* outInfo) const;

    // 对齐 IDA: IsUserAuth - 判断是否启用用户认证
    bool IsUserAuth() const { return m_serverInfo.bUserAuth; }

    // 对齐 IDA: IsWriteLog - 判断是否启用日志写入
    bool IsWriteLog() const { return m_bWriteLog; }
    // 对齐 IDA: IsWriteStatistics - 判断是否启用统计写入
    bool IsWriteStatistics() const { return m_bWriteStatistics; }
    // 对齐 IDA: IsWriteSGNetCafe - 判断是否启用SG网吧日志
    bool IsWriteSGNetCafe() const { return m_bWriteSGNetcafe; }

    // 对齐 IDA: GetServerPublicIPAndPort - 获取指定服务器类型的公网 IP 和端口
    bool GetServerPublicIPAndPort(const char* szServerName, char* szIP, std::int16_t* sPort) const {
        // TODO: 需从 m_mapSystemInfo 中查找指定服务器
        // 目前简化实现，返回 false
        if (!szServerName || !szIP || !sPort) return false;
        // 查找 AUTH 服务器 (简化版)
        if (strcmp(szServerName, "AUTH") == 0) {
            // TODO: 从配置中获取真实 AUTH 服务器信息
            return false;
        }
        return false;
    }

private:
    static SYSTEM_TYPE GetSystemType(const char* szType);
    static NATION_TYPE GetNationType(const char* szType);
    static int GetServerType(const char* szType);
    void AddChannelInfo(const CHANNEL_INFO& stInfo);
    void AddChannelDistrict6Info(const CHANNEL_INFO& stInfo);

    SERVER_SYSTEM_INFO m_serverInfo{};
    CONTENTS_OPTION_INFO m_ContentsOption{};
    std::map<std::string, SERVER_SYSTEM_INFO> m_mapSystemInfo;
    char m_szRelayIP[513] = {};
    std::uint16_t m_sRelayPort = 0;
    int m_nDBThreadCount = 0;
    std::array<std::string, 9> m_strDNS{};
    char m_szSystemGroup[21] = {};
    OBJECT_POOL_INFO m_stObjectPoolInfo{};
    std::map<int, std::vector<CHANNEL_INFO>> m_mapChannelInfo;
    std::map<int, std::vector<CHANNEL_INFO>> m_mapChannelDistrict6Info;
    bool m_bWriteLog = false;
    bool m_bWriteStatistics = false;
    bool m_bWriteSGNetcafe = false;
    SYSTEM_TYPE m_eSystemType = SYSTEM_TYPE_NONE;
    NATION_TYPE m_eNationType = NATION_TYPE_NONE;
    BILLING_TYPE m_eBillingType = BILLING_TYPE_TEST;
    bool m_bDev = false;
    bool m_bTestServer = false;
    bool m_bRealServer = false;
    std::array<AGENT_SYSTEM_INFO, 3> m_agentInfo{};
    bool m_bInitXignCode = false;
    bool m_bInitHanBilling = false;
    bool m_bInitHanNetCafe = false;
    MOBILE_APP_INFO m_stMobileInfo{};
    char m_szSGAuthLink[1024] = {};
    char m_szSGLogLink[1024] = {};
    char m_szSGLogEnvironment[21] = {};
    char m_szSGBillingLink[1024] = {};
    char m_szSGGameCode[21] = {};
    char m_szSGCinderellaLink[1024] = {};
    std::uint16_t m_wSGCinderellaPort = 0;
    bool m_bSGSinderellaUse = false;
    char m_szWMGameCodeID[1024] = {};
    char m_szWMAuthApiUrl[1024] = {};
    int m_nWMGameServiceID = 0;
    int m_nWMGameProductID = 0;
    char m_szWMBillingApiUrl[1024] = {};
    char m_szSGPCCafeLink[1024] = {};
    std::uint16_t m_wSGPCCafePort = 0;
    bool m_bSGPCCafeUse = false;
    bool m_bSGPCCafeLog = false;
    char m_szSGCharLink[1024] = {};
    char m_szSGERSLink[1024] = {};
    char m_szSGKeepAliveLink[1024] = {};
    char m_szGFClientID[1024] = {};
    char m_szGFClientSecret[1024] = {};
    char m_szGFRedirectUrl[1024] = {};
    char m_szGFPlatformApiUrl[1024] = {};
    char m_szGFOAuthApiUrl[1024] = {};
    char m_szCHNGKey[1024] = {};
    char m_szCHNLink[1024] = {};
    char m_szCHNAuthLink[1024] = {};
    char m_SzCHNLoginKey[1024] = {};
    SECURITY_TYPE m_eSecurityType = SECURITY_OFF;
    RES_LOAD_TYPE m_eResLoadType = RES_LOAD_TYPE_DB;
    char m_szResFilePath[512] = {};
};
