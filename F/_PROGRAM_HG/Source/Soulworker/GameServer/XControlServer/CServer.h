// CServer.h - ControlServer
// 对齐 IDA ControlServer.exe: CServer 类定义

#pragma once

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include <map>
#include <memory>

// 前向声明
class CMazeInfo;
class CMyRoom;
class CServerProcess;
class CUserProcess;
class CGMToolProcess;
class CWorldModeProcess;

// 对齐 IDA ControlServer.exe: 服务器同步加载状态枚举
enum E_SERVER_SYNC_LOAD {
    E_SERVER_SYNC_LOAD_USER      = 1,  // 用户信息同步完成
    E_SERVER_SYNC_LOAD_MAZE_INFO = 2,  // 地图信息同步完成
};

// 注意: ST_MYROOM_USER 和 ST_MYROOM_OWNER_INFO 定义在 GreenDamTan_MyRoomStructs.h

// 对齐 IDA ControlServer.exe: CServer 类
// 继承 XClient (offset 0, size 131512)
// 注意: XClient 已经继承 IXObject，不需要再次继承
// 总大小: 132768 bytes
class CServer : public XClient {
public:
    CServer();
    virtual ~CServer();

    // 对齐 IDA 0x140041320: Init - 初始化服务器
    virtual bool Init() override;

    // ===== 基硌信息访问 =====

    // 对齐 IDA 0x14000A090: GetServerState
    int GetServerState() const { return m_serverInfo.nState; }

    // 对齐 IDA 0x14000A4E0: GetServerInfo
    SS_SERVER_INFO& GetServerInfo() { return m_serverInfo; }
    const SS_SERVER_INFO& GetServerInfo() const { return m_serverInfo; }
    SS_SERVER_INFO* GetServerInfoPtr() { return &m_serverInfo; }

    // 对齐 IDA 0x140041590: SetServerInfo - 设置服务器信息
    void SetServerInfo(const SS_SERVER_INFO& info);

    // 对齐 IDA 0x140041660: UpdateServerInfo - 更新服务器信息
    void UpdateServerInfo(int nState, int nCount);

    // 对齐 IDA 0x14000B2B0: GetServerID
    std::uint32_t GetServerID() const { return m_serverInfo.dwID; }
    void SetServerID(std::uint32_t dwID) { m_serverInfo.dwID = dwID; }

    // 对齐 IDA: GetServerType
    int GetServerType() const { return m_serverInfo.nType; }

    // 对齐 IDA: GetUserCount
    int GetUserCount() const { return m_serverInfo.nCurUser; }

    // 对齐 IDA: SetUserCount - 设置当前用户数 (用于 LoginServer 同步)
    void SetUserCount(int nCount) { m_serverInfo.nCurUser = nCount; }

    // 对齐 IDA: GetMaxUserCount
    int GetMaxUserCount() const { return m_serverInfo.nMaxUser; }

    // 对齐 IDA 0x1400416D0: GetSerial - 获取并递增序列号
    int GetSerial();

    // 对齐 IDA: SetSerial
    void SetSerial(int nSerial) { m_nSerial = nSerial; }

    // 对齐 IDA: SetLogicThreadCount
    void SetLogicThreadCount(int nCount) { m_nLogicThreadCount = nCount; }

    // ===== 迷宫管理 =====

    // 对齐 IDA 0x1400427B0: CanMakeMaze - 检查是否可以创建迷宫
    // IDA: return m_nMaxMazeCount > m_mapMazeInfo.size();
    bool CanMakeMaze() const { return m_nMaxMazeCount > static_cast<int>(m_mapMazeInfo.size()); }

    // 对齐 IDA 0x1400417C0: GetMapID - 生成地图实例ID
    // IDA: 构造 UXMapID(channel, mapID, serial)
    UXMapID GetMapID(WORD wMapID);

    // 对齐 IDA: IsMaze - 检查指定地图是否为本服务器的迷宫
    bool IsMaze(UXMapID uxMapID) const {
        return m_mapMazeInfo.find(uxMapID) != m_mapMazeInfo.end();
    }

    // 对齐 IDA: GetMazeInfo - 获取迷宫信息
    std::shared_ptr<CMazeInfo> GetMazeInfo(UXMapID uxMapID) {
        auto it = m_mapMazeInfo.find(uxMapID);
        if (it != m_mapMazeInfo.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 对齐 IDA: AddMaze - 添加迷宫
    void AddMaze(UXMapID uxMapID, std::shared_ptr<CMazeInfo> pMazeInfo) {
        m_mapMazeInfo[uxMapID] = pMazeInfo;
    }

    // 对齐 IDA: RecvMapInfo - 接收地图信息
    void RecvMapInfo();

    // ===== 同步状态 =====

    // 对齐 IDA: IsSyncLoad - 检查同步加载状态
    // IDA: return this->m_dwSyncLoad == 3;
    bool IsSyncLoad() const { return m_dwSyncLoad == 3; }

    // 对齐 IDA: IsRecvServerInfo - 检查是否已接收服务器信息
    // IDA: return this->m_stSyncInfo.SyncData == 0;
    bool IsRecvServerInfo() const { return m_stSyncInfo.SyncData == 0; }

    // 对齐 IDA: RecvUserInfo - 标记用户信息已接收
    // IDA: *(_BYTE *)&this->m_stSyncInfo.SyncData &= ~1u;
    void RecvUserInfo() { m_stSyncInfo.SyncData &= ~1u; }

    // 对齐 IDA: SetSyncLoad - 设置同步加载状态
    void SetSyncLoad(E_SERVER_SYNC_LOAD eServerSync);

    // ===== 迷宫管理 =====

    // 对齐 IDA: GetMaxMazeCount
    int GetMaxMazeCount() const { return m_nMaxMazeCount; }

    // 对齐 IDA: SetMaxMazeCount
    void SetMaxMazeCount(int nCount) { m_nMaxMazeCount = nCount; }

    // 对齐 IDA 0x140042340: SyncMaze - 同步迷宫信息
    bool SyncMaze(PS_MAZE_UPDATE_INFO_SYNC* pMazeInfo);

    // 对齐 IDA 0x140041930: RemoveMaze - 移除迷宫
    void RemoveMaze(UXMapID uxMapID, bool bResult);

    // 对齐 IDA 0x1400421E0: UpdateMaze - 更新迷宫信息
    bool UpdateMaze(PS_MAZE_UPDATE_INFO* pMazeInfo);

    // 对齐 IDA 0x140042550: OnUpdate - 服务器更新循环
    void OnUpdate(ULONGLONG dwTick);

    // ===== MyRoom 管理 =====

    // 对齐 IDA 0x1400427C0: CreateMyRoom - 创建 MyRoom (发送 0xF2,0x52)
    void CreateMyRoom(ST_MYROOM_OWNER_INFO stOwnerInfo, ST_MYROOM_USER stEnterUser, std::uint32_t dwOwnerUCID);

    // 对齐 IDA: DelMyRoom
    void DelMyRoom(std::uint32_t dwOwnerID);

    // 对齐 IDA: FindMyRoom
    std::shared_ptr<CMyRoom> FindMyRoom(std::uint32_t dwOwnerID) const;

    // ===== 迷宫成员验证 =====

    // 对齐 IDA 0x140041C80: IsValidEnterPartyMemberInMaze - 验证队伍成员进入迷宫
    bool IsValidEnterPartyMemberInMaze(DWORD dwPartyID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap, PS_ENTER_MAP_RES& stEnterMapRes);

    // 对齐 IDA 0x140041F30: IsValidEnterForceMemberInMaze - 验证Force成员进入迷宫
    bool IsValidEnterForceMemberInMaze(DWORD dwForceID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap, PS_ENTER_MAP_RES& stEnterMapRes);

    // ===== 包处理注册 =====

    // 对齐 IDA 0x140041180: RegisterProcess - 注册包处理类
    bool RegisterProcess();

    // ===== TXDBSocket 兼容 =====

    // TXDBSocket 需要: GetBlockType - 服务器不需要封禁检查，返回 0
    std::uint8_t GetBlockType() const { return 0; }

    // ===== 成员变量 (对齐 IDA 布局) =====

protected:
    // 对齐 IDA: CServer 成员变量布局
    SS_SERVER_INFO m_serverInfo{};      // offset 131576, size 1088
    int m_nSerial = 0;                  // offset 132664, size 4
    std::map<UXMapID, std::shared_ptr<CMazeInfo>> m_mapMazeInfo;  // offset 132672, size 32
    int m_nMaxMazeCount = 0;            // offset 132704, size 4
    ST_SYNC_INFO m_stSyncInfo{};        // offset 132708, size 4 (已在 PSServer.h 定义)
    int m_nSyncWorldForThread = 0;      // offset 132712, size 4
    std::map<std::uint32_t, std::shared_ptr<CMyRoom>> m_mpMyRoomInfo;  // offset 132720, size 32
    std::uint32_t m_dwSyncLoad = 0;     // offset 132752, size 4
    int m_nLogicThreadCount = 0;        // offset 132756, size 4
    int m_nLogicThreadSyncCount = 0;    // offset 132760, size 4
};