// ServerProcess.h
// RelayServer/ControlServer Server Process Stub

#pragma once

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include "Soulworker/GameServer/XControlServer/MazeInfo.h"
#include "Soulworker/GameServer/XControlServer/CMyRoom.h"
#include <memory>
#include <map>
#ifdef _WIN32
#include <intrin.h>
#endif

// 对齐 IDA: std::tr1 命名空间别名 (C++11 后 tr1 已合并到 std)
namespace std { namespace tr1 = std; }
struct PS_ENTER_MAP_REQ;
struct PS_ENTER_MAP_RES;

// 服务器同步加载状态
enum E_SERVER_SYNC_LOAD
{
    E_SERVER_SYNC_LOAD_USER = 0x01,
    E_SERVER_SYNC_LOAD_MAZE_INFO = 0x02,
};

// 对齐 ControlServer.exe IDA
// CServer 继承自 XClient (定义于 GreenDamTan_ClientBase.h)
class CServer : public XClient {
public:
    CServer() = default;
    virtual ~CServer() = default;

    // 同步状态
    DWORD GetSyncLoad() const { return m_dwSyncLoad; }
    void SetSyncLoad(E_SERVER_SYNC_LOAD eServerSync) {
        if ((m_dwSyncLoad & static_cast<std::uint32_t>(eServerSync)) == 0) {
            m_dwSyncLoad |= static_cast<std::uint32_t>(eServerSync);
        }
    }
    void AddSyncLoad(DWORD dwLoad) { m_dwSyncLoad |= dwLoad; }
    void RemoveSyncLoad(DWORD dwLoad) { m_dwSyncLoad &= ~dwLoad; }
    bool IsSyncLoad(DWORD dwLoad) const { return (m_dwSyncLoad & dwLoad) != 0; }
    bool IsSyncLoad() const { return m_dwSyncLoad != 0; }  // 检查是否有任何同步标志

    // 服务器状态
    int GetServerState() const { return m_nServerState; }
    void SetServerState(int nState) { m_nServerState = nState; }

    // 用户计数
    int GetUserCount() const { return m_nUserCount; }
    void SetUserCount(int nCount) { m_nUserCount = nCount; }

    // 对齐 IDA: GetMaxUserCount
    int GetMaxUserCount() const { return m_nMaxUserCount; }
    void SetMaxUserCount(int nCount) { m_nMaxUserCount = nCount; }

    // 对齐 IDA: CanMakeMaze - 判断是否可以创建迷宫
    bool CanMakeMaze() const { return m_bCanMakeMaze; }
    void SetCanMakeMaze(bool bCan) { m_bCanMakeMaze = bCan; }

    // 对齐 IDA: GetMaxMazeCount / SetMaxMazeCount
    int GetMaxMazeCount() const { return m_nMaxMazeCount; }
    void SetMaxMazeCount(int nCount) { m_nMaxMazeCount = nCount; }

    // 对齐 IDA: RecvMapInfo - 接收地图信息 (发送请求)
    void RecvMapInfo() {
        // 对齐 IDA 0x140027CC0: 清除同步标志位，准备接收地图信息
        // TODO: 原始实现清除 m_stSyncInfo.SyncData 的 bit 1
    }

    // 对齐 IDA: IsRecvServerInfo - 判断是否已接收服务器信息
    bool IsRecvServerInfo() const { return m_bRecvServerInfo; }
    void SetRecvServerInfo(bool bRecv) { m_bRecvServerInfo = bRecv; }

    // 对齐 IDA: RecvUserInfo - 标记已接收用户信息
    void RecvUserInfo() { m_bRecvUserInfo = true; }
    bool HasRecvUserInfo() const { return m_bRecvUserInfo; }

    // ServerID (覆盖基类方法)
    std::uint32_t GetServerID() const { return m_dwServerID; }
    void SetServerID(std::uint32_t dwID) { m_dwServerID = dwID; }

    // 对齐 IDA: SetServerInfo - 设置服务器信息
    void SetServerInfo(const SS_SERVER_INFO& info) { m_stServerInfo = info; }

    // 对齐 IDA: GetServerInfo - 返回服务器信息
    SS_SERVER_INFO* GetServerInfo() { return &m_stServerInfo; }
    const SS_SERVER_INFO* GetServerInfo() const { return &m_stServerInfo; }

    // 对齐 IDA: GetServerType - 返回服务器类型
    int GetServerType() const { return m_stServerInfo.nType; }

    // 对齐 IDA: GetBlockType - 返回阻塞类型 (用于 TXDBSocketT)
    int GetBlockType() const { return 0; }  // 默认不阻塞

    // 对齐 IDA 0x140042550: OnUpdate - 服务器更新循环
    virtual void OnUpdate(ULONGLONG dwTick);

    // 对齐 IDA 0x140041180: RegisterProcess - 注册包处理类
    virtual bool RegisterProcess();

    // 对齐 IDA 0x140041800: IsMaze - 判断 uxMapID 是否属于该服务器
    bool IsMaze(UXMapID uxMapID) {
        return m_mapMazeInfo.find(uxMapID) != m_mapMazeInfo.end();
    }

    // 对齐 IDA 0x140041870: GetMazeInfo - 获取迷宫信息
    std::tr1::shared_ptr<CMazeInfo> GetMazeInfo(UXMapID uxMazeID) {
        auto it = m_mapMazeInfo.find(uxMazeID);
        if (it != m_mapMazeInfo.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 对齐 IDA 0x140041410: AddMaze - 添加迷宫到服务器
    void AddMaze(UXMapID uxMapID, std::tr1::shared_ptr<CMazeInfo> pMazeInfo) {
        if (pMazeInfo) {
            m_mapMazeInfo[uxMapID] = pMazeInfo;
        }
    }

    // 对齐 IDA 0x1400416D0: GetSerial - 原子递增序列号
    LONG GetSerial() {
        return _InterlockedIncrement(&m_nSerial);
    }

    // 对齐 IDA 0x140041710: GetMapID - 生成唯一地图ID
    UXMapID GetMapID(WORD wMapID) {
        UXMapID uxMapID{};
        uxMapID.nMapID = 0;
        // 设置 channel 到高位 (bits 48-63)
        uxMapID.nMapID |= ((std::uint64_t)(std::uint16_t)m_stServerInfo.nChannel << 48);
        // 设置 mapID (bits 32-47)
        uxMapID.nMapID |= ((std::uint64_t)wMapID << 32);
        // 清除 bits 24-31
        uxMapID.nMapID &= 0xFFFFFFFF00FFFFFFULL;
        // 设置 serial (bits 0-23)
        LONG nSerial = GetSerial();
        uxMapID.nMapID |= (std::uint64_t)nSerial & 0xFFFFFFULL;
        return uxMapID;
    }

    // 对齐 IDA 0x140041C80: IsValidEnterPartyMemberInMaze - 验证队伍成员是否可进入迷宫
    bool IsValidEnterPartyMemberInMaze(DWORD dwPartyID, UXMapID uxMapID, PS_ENTER_MAP_REQ* stEnterMap, PS_ENTER_MAP_RES* stEnterMapRes) {
        auto iter = m_mapMazeInfo.find(uxMapID);
        if (iter == m_mapMazeInfo.end()) {
            stEnterMapRes->nResult = 1;
            return false;
        }

        auto pMazeInfo = iter->second;
        if (pMazeInfo->GetPartyID() != static_cast<int>(dwPartyID)) {
            stEnterMapRes->nResult = 2;
            return false;
        }

        WORD wMapID = 0;
        // 从 UXMapID 提取 MapID (bits 32-47)
        wMapID = static_cast<WORD>((uxMapID.nMapID >> 32) & 0xFFFF);
        if (wMapID != stEnterMap->wMapID) {
            stEnterMapRes->nResult = 2;
            return false;
        }

        // 遍历成员列表查找请求者
        auto* pMemberVec = pMazeInfo->GetMemberVector();
        if (pMemberVec) {
            for (const auto& member : *pMemberVec) {
                if (stEnterMap->dwActorID == member.dwMember) {
                    // 填充响应结构
                    pMazeInfo->GetMazeInfo(stEnterMapRes);
                    stEnterMapRes->dwUserID = stEnterMap->dwActorID;
                    stEnterMapRes->nResult = 0;
                    stEnterMapRes->nJumpID = stEnterMap->nJumpID;
                    stEnterMapRes->nPortalID = stEnterMap->nPortalID;
                    return true;
                }
            }
        }

        stEnterMapRes->nResult = 3;
        return false;
    }

    // 对齐 IDA 0x140041F30: IsValidEnterForceMemberInMaze - 验证Force成员是否可进入迷宫
    bool IsValidEnterForceMemberInMaze(DWORD dwForceID, UXMapID uxMapID, PS_ENTER_MAP_REQ* stEnterMap, PS_ENTER_MAP_RES* stEnterMapRes) {
        auto iter = m_mapMazeInfo.find(uxMapID);
        if (iter == m_mapMazeInfo.end()) {
            stEnterMapRes->nResult = 1;
            return false;
        }

        auto pMazeInfo = iter->second;
        if (pMazeInfo->GetForceID() != static_cast<int>(dwForceID)) {
            stEnterMapRes->nResult = 2;
            return false;
        }

        WORD wMapID = 0;
        // 从 UXMapID 提取 MapID (bits 32-47)
        wMapID = static_cast<WORD>((uxMapID.nMapID >> 32) & 0xFFFF);
        if (wMapID != stEnterMap->wMapID) {
            stEnterMapRes->nResult = 2;
            return false;
        }

        // 遍历成员列表查找请求者
        auto* pMemberVec = pMazeInfo->GetMemberVector();
        if (pMemberVec) {
            for (const auto& member : *pMemberVec) {
                if (stEnterMap->dwActorID == member.dwMember) {
                    // 填充响应结构
                    pMazeInfo->GetMazeInfo(stEnterMapRes);
                    stEnterMapRes->dwUserID = stEnterMap->dwActorID;
                    stEnterMapRes->nResult = 0;
                    stEnterMapRes->nJumpID = stEnterMap->nJumpID;
                    stEnterMapRes->nPortalID = stEnterMap->nPortalID;
                    return true;
                }
            }
        }

        stEnterMapRes->nResult = 3;
        return false;
    }

    // 对齐 IDA 0x1400427C0: CreateMyRoom - 创建 MyRoom
    void CreateMyRoom(ST_MYROOM_OWNER_INFO* stOwnerInfo, ST_MYROOM_USER* stEnterUser, DWORD dwOwnerUCID) {
        // 创建新的 MyRoom 对象
        auto pMyRoom = std::tr1::make_shared<CMyRoom>();
        if (!pMyRoom) return;

        // 生成地图ID
        UXMapID uxMapID = GetMapID(static_cast<WORD>(stOwnerInfo->dwUAID));

        // 初始化 MyRoom
        pMyRoom->CreateMyRoom(stOwnerInfo, uxMapID, stEnterUser, this);

        // 插入到 MyRoom 映射
        m_mpMyRoomInfo[stOwnerInfo->dwUAID] = pMyRoom;

        // 发送创建包 (0xF2, 0x52)
        XSendPacket xSendPacket(0xF2, 0x52);
        xSendPacket << *stEnterUser;
        xSendPacket << *stOwnerInfo;
        xSendPacket.XParse << uxMapID.nMapID;
        xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
        SendEx(xSendPacket);
    }

    // 对齐 IDA 0x140042A00: DelMyRoom - 删除 MyRoom
    void DelMyRoom(DWORD dwUAID) {
        m_mpMyRoomInfo.erase(dwUAID);
    }

    // 对齐 IDA 0x140042A30: FindMyRoom - 查找 MyRoom
    std::tr1::shared_ptr<CMyRoom> FindMyRoom(DWORD dwUAID) {
        auto it = m_mpMyRoomInfo.find(dwUAID);
        if (it != m_mpMyRoomInfo.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 对齐 IDA 0x140041930: RemoveMaze - 移除迷宫
    void RemoveMaze(UXMapID uxMapID, bool bResult);

    // 对齐 IDA 0x1400421E0: UpdateMaze - 更新迷宫信息
    void UpdateMaze(PS_MAZE_UPDATE_INFO* pMazeInfo);

    // 对齐 IDA 0x140042340: SyncMaze - 同步迷宫信息
    void SyncMaze(PS_MAZE_UPDATE_INFO_SYNC* pMazeInfo);

    // 对齐 IDA: SetLogicThreadCount - 设置逻辑线程数
    void SetLogicThreadCount(DWORD dwCount) { m_dwLogicThreadCount = dwCount; }
    DWORD GetLogicThreadCount() const { return m_dwLogicThreadCount; }

    // 对齐 IDA 0x1400416D0: SetSerial - 设置序列号
    void SetSerial(LONG nSerial) { m_nSerial = nSerial; }

    // 发送数据 - 使用基类的 SendEx
    // void SendEx(XSendPacket& packet) 继承自 XClient

private:
    std::uint32_t m_dwServerID = 0;
    DWORD m_dwSyncLoad = 0;
    DWORD m_dwLogicThreadCount = 0;  // 对齐 IDA: 逻辑线程数
    DWORD m_nLogicThreadSyncCount = 0;  // 对齐 IDA: 逻辑线程同步计数
    int m_nServerState = 0;
    int m_nUserCount = 0;
    int m_nMaxUserCount = 0;
    int m_nMaxMazeCount = 0;  // 对齐 IDA: 最大迷宫数
    bool m_bRecvServerInfo = false;
    bool m_bCanMakeMaze = false;
    bool m_bRecvUserInfo = false;  // 对齐 IDA: 用户信息同步完成标志
    LONG m_nSerial = 0;  // 对齐 IDA: 序列号计数器
    SS_SERVER_INFO m_stServerInfo{};
    std::map<UXMapID, std::tr1::shared_ptr<CMazeInfo>> m_mapMazeInfo;  // 对齐 IDA: 迷宫信息映射
    std::map<DWORD, std::tr1::shared_ptr<CMyRoom>> m_mpMyRoomInfo;  // 对齐 IDA: MyRoom 映射
};

// 对齐 IDA: CServerProcess - 服务器包处理类
// 包含 ControlServer.exe 所有 0xF2 子命令处理函数
class CServerProcess : public TXProcess<CServer> {
public:
    CServerProcess() = default;
    virtual ~CServerProcess() = default;

    // 对齐 IDA 0x14003CAC0: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

private:
    // 对齐 IDA 0x14003D0A0: ReqCreateServer (sub 0x01)
    bool ReqCreateServer(XPacket& xPacket);

    // 对齐 IDA: ReqUpdateServerInfo (sub 0x03)
    bool ReqUpdateServerInfo(XPacket& xPacket);

    // 对齐 IDA: ReqChangeChannel (sub 0x10)
    bool ReqChangeChannel(XPacket& xPacket);

    // 对齐 IDA 0x14003D1F0: ReqCreateMaze (sub 0x21)
    bool ReqCreateMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D360: ResCreateMaze (sub 0x22)
    bool ResCreateMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D4F0: SyncMaze (sub 0x23)
    bool SyncMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D410: SyncRemoveMaze (sub 0x25)
    bool SyncRemoveMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D480: SyncUpdateMaze (sub 0x26)
    bool SyncUpdateMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D2A0: ReqGoBackMaze (sub 0x27)
    bool ReqGoBackMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003D5D0: ReqCreateMap (sub 0x30)
    bool ReqCreateMap(XPacket& xPacket);

    // 对齐 IDA 0x14003D6E0: ReqEnterMap (sub 0x31)
    bool ReqEnterMap(XPacket& xPacket);

    // 对齐 IDA: ReqCheckPartyInMaze (sub 0x32)
    bool ReqCheckPartyInMaze(XPacket& xPacket);

    // 对齐 IDA: SyncUsersInfo (sub 0x33)
    bool SyncUsersInfo(XPacket& xPacket);

    // 对齐 IDA 0x14003D9E0: SyncUserPartyInfo (sub 0x36)
    bool SyncUserPartyInfo(XPacket& xPacket);

    // 对齐 IDA 0x14003DBF0: SyncLogicThreadCount (sub 0x37)
    bool SyncLogicThreadCount(XPacket& xPacket);

    // 对齐 IDA: ReqCheckEnterMaze (sub 0x38)
    bool ReqCheckEnterMaze(XPacket& xPacket);

    // 对齐 IDA 0x14003DCA0: ReqDisconnectUserSync (sub 0x39)
    bool ReqDisconnectUserSync(XPacket& xPacket);

    // 对齐 IDA 0x14003DD10: SyncMaxMazeID (sub 0x40)
    bool SyncMaxMazeID(XPacket& xPacket);

    // 对齐 IDA 0x14003DFF0: ResCreateMatchingMazeFromGame (sub 0x41/0x42)
    bool ResCreateMatchingMazeFromGame(XPacket& xPacket, int eType);

    // 对齐 IDA 0x14003DD60: ReqCreateMatchingMazeFromCommunity (sub 0x43)
    bool ReqCreateMatchingMazeFromCommunity(XPacket& xPacket);

    // 对齐 IDA: ReqCreateMatchingModeMazeFromCommunity (sub 0x49)
    bool ReqCreateMatchingModeMazeFromCommunity(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomEnterReq (sub 0x50)
    bool ReqMyRoomEnterReq(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomEnterRes (sub 0x51)
    bool ReqMyRoomEnterRes(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomCreate (sub 0x52)
    bool ReqMyRoomCreate(XPacket& xPacket);

    // 对齐 IDA: ReqMyRoomDelete (sub 0x53)
    bool ReqMyRoomDelete(XPacket& xPacket);

    // 对齐 IDA 0x14003D750: EnterOtherMap_cheat (sub 0x54)
    bool EnterOtherMap_cheat(XPacket& xPacket);

    // 对齐 IDA: PartyMazeSync (sub 0x55)
    bool PartyMazeSync(XPacket& xPacket);

    // 对齐 IDA: ReqPostSend (sub 0x56)
    bool ReqPostSend(XPacket& xPacket);

    // 对齐 IDA: ReqFindUser (sub 0x58)
    bool ReqFindUser(XPacket& xPacket);

    // 对齐 IDA: ResMyRoomDelete (sub 0x59)
    bool ResMyRoomDelete(XPacket& xPacket);

    // 对齐 IDA: ForceMazeSync (sub 0x63)
    bool ForceMazeSync(XPacket& xPacket);

    // 对齐 IDA: ResCreateModeMaze (sub 0x73)
    bool ResCreateModeMaze(XPacket& xPacket);

    // 对齐 IDA: ReqUpdateRouletteEvent (sub 0x75)
    bool ReqUpdateRouletteEvent(XPacket& xPacket);

    // 对齐 IDA: ReqReEnterMap (sub 0x77)
    bool ReqReEnterMap(XPacket& xPacket);
};

// 对齐 IDA: CUserProcess - 用户包处理类 (main=0xF3)
// 处理用户相关子命令: 选角、登出、踢出、聊天等
class CUserProcess : public TXProcess<CServer> {
public:
    CUserProcess() = default;
    virtual ~CUserProcess() = default;

    // 对齐 IDA 0x1400450E0: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

private:
    // 对齐 IDA 0x140045310: SyncSelectCharacter (sub 0x01)
    bool SyncSelectCharacter(XPacket& xPacket);

    // 对齐 IDA 0x1400454B0: SyncLogoutUser (sub 0x02)
    bool SyncLogoutUser(XPacket& xPacket);

    // 对齐 IDA 0x140045570: SyncUpdateUserMap (sub 0x03)
    bool SyncUpdateUserMap(XPacket& xPacket);

    // 对齐 IDA 0x1400455D0: SyncUserKickout (sub 0x07)
    bool SyncUserKickout(XPacket& xPacket);

    // 对齐 IDA 0x140045640: ReqUserChatNotice (sub 0x14)
    bool ReqUserChatNotice(XPacket& xPacket);

    // 对齐 IDA 0x1400456B0: ReqUserChatMegaPhone (sub 0x15)
    bool ReqUserChatMegaPhone(XPacket& xPacket);

    // 对齐 IDA 0x140045A10: ReqUserChangeServer (sub 0x11)
    bool ReqUserChangeServer(XPacket& xPacket);

    // 对齐 IDA 0x140045A80: SyncUserMoneyLog (sub 0x12)
    bool SyncUserMoneyLog(XPacket& xPacket);

    // 对齐 IDA 0x140045AC0: ReqUserEnterPartyMaze (sub 0x27)
    bool ReqUserEnterPartyMaze(XPacket& xPacket);

    // 对齐 IDA 0x140045B70: ReqUserEnterForceMaze (sub 0x28)
    bool ReqUserEnterForceMaze(XPacket& xPacket);

    // 对齐 IDA 0x140045C30: ReqUserTradePasswordStateSync (sub 0x26)
    bool ReqUserTradePasswordStateSync(XPacket& xPacket);

    // 对齐 IDA 0x140045CB0: ReqUserTradePasswordState (sub 0x24)
    bool ReqUserTradePasswordState(XPacket& xPacket);

    // 对齐 IDA 0x140045D10: ReqNameChange (sub 0x25)
    bool ReqNameChange(XPacket& xPacket);

    // 对齐 IDA 0x140045D70: ReqCheckSessionID (sub 0x32)
    bool ReqCheckSessionID(XPacket& xPacket);

    // 对齐 IDA 0x140045E00: ReqGameServerEnterUser (sub 0x33)
    bool ReqGameServerEnterUser(XPacket& xPacket);

    // 对齐 IDA 0x140045F40: ReqUserUpdateAuthType (sub 0x34)
    bool ReqUserUpdateAuthType(XPacket& xPacket);
};

// 对齐 IDA: CGMToolProcess - GM工具包处理类 (main=0xF8)
// 处理GM命令: 踢人、公告、关服、活动、服务器选项等
class CGMToolProcess : public TXProcess<CServer> {
public:
    CGMToolProcess() = default;
    virtual ~CGMToolProcess() = default;

    // 对齐 IDA 0x140034640: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

private:
    // 对齐 IDA 0x140034730: ReqGMUserKick (sub 0x01)
    bool ReqGMUserKick(XPacket& xPacket);

    // 对齐 IDA 0x1400347C0: ReqGMNotice (sub 0x02)
    bool ReqGMNotice(XPacket& xPacket);

    // 对齐 IDA 0x140034850: ReqGMShutDwon (sub 0x03)
    bool ReqGMShutDwon(XPacket& xPacket);

    // 对齐 IDA 0x1400348B0: ReqGMTimeEvent (sub 0x04)
    bool ReqGMTimeEvent(XPacket& xPacket);

    // 对齐 IDA 0x140034910: ReqGMValueEvent (sub 0x05)
    bool ReqGMValueEvent(XPacket& xPacket);

    // 对齐 IDA 0x1400349A0: ReqGMServerOption (sub 0x06)
    bool ReqGMServerOption(XPacket& xPacket);

    // 对齐 IDA 0x140034A10: ReqGMCashShopBanner (sub 0x07)
    bool ReqGMCashShopBanner(XPacket& xPacket);
};

// 对齐 IDA: CWorldModeProcess - 世界模式包处理类 (main=0xFB)
// 处理世界模式相关命令
class CWorldModeProcess : public TXProcess<CServer> {
public:
    CWorldModeProcess() = default;
    virtual ~CWorldModeProcess() = default;

    // 对齐 IDA 0x14004D5F0: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

private:
    // 对齐 IDA 0x14004D680: ReqWorldModeCommand (sub 0x01)
    bool ReqWorldModeCommand(XPacket& xPacket);

    // 对齐 IDA 0x14004DAF0: ReqWorldModeComplete (sub 0x02)
    bool ReqWorldModeComplete(XPacket& xPacket);

    // 对齐 IDA 0x14004DC20: ReqWorldModeUpdate (sub 0x03)
    bool ReqWorldModeUpdate(XPacket& xPacket);

    // 对齐 IDA 0x14004DC90: ReqWorldModeEnterList (sub 0x04)
    bool ReqWorldModeEnterList(XPacket& xPacket);
};
