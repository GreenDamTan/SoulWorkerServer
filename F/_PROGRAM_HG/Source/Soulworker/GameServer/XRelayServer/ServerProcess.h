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
};

// 对齐 ControlServer.exe IDA
// CServer 继承自 XClient (定义于 GreenDamTan_ClientBase.h)
class CServer : public XClient {
public:
    CServer() = default;
    virtual ~CServer() = default;

    // 同步状态
    DWORD GetSyncLoad() const { return m_dwSyncLoad; }
    void SetSyncLoad(DWORD dwLoad) { m_dwSyncLoad = dwLoad; }
    void AddSyncLoad(DWORD dwLoad) { m_dwSyncLoad |= dwLoad; }
    void RemoveSyncLoad(DWORD dwLoad) { m_dwSyncLoad &= ~dwLoad; }
    bool IsSyncLoad(DWORD dwLoad) const { return (m_dwSyncLoad & dwLoad) != 0; }

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

    // 对齐 IDA: IsRecvServerInfo - 判断是否已接收服务器信息
    bool IsRecvServerInfo() const { return m_bRecvServerInfo; }
    void SetRecvServerInfo(bool bRecv) { m_bRecvServerInfo = bRecv; }

    // 对齐 IDA: RecvUserInfo - 标记已接收用户信息
    void RecvUserInfo() { m_bRecvUserInfo = true; }
    bool HasRecvUserInfo() const { return m_bRecvUserInfo; }

    // ServerID (覆盖基类方法)
    std::uint32_t GetServerID() const { return m_dwServerID; }
    void SetServerID(std::uint32_t dwID) { m_dwServerID = dwID; }

    // 对齐 IDA: GetServerInfo - 返回服务器信息
    SS_SERVER_INFO* GetServerInfo() { return &m_stServerInfo; }
    const SS_SERVER_INFO* GetServerInfo() const { return &m_stServerInfo; }

    // 对齐 IDA: OnUpdate (空实现)
    virtual void OnUpdate(ULONGLONG dwTick) {}

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

    // 发送数据 - 使用基类的 SendEx
    // void SendEx(XSendPacket& packet) 继承自 XClient

private:
    std::uint32_t m_dwServerID = 0;
    DWORD m_dwSyncLoad = 0;
    int m_nServerState = 0;
    int m_nUserCount = 0;
    int m_nMaxUserCount = 0;
    bool m_bRecvServerInfo = false;
    bool m_bCanMakeMaze = false;
    bool m_bRecvUserInfo = false;  // 对齐 IDA: 用户信息同步完成标志
    LONG m_nSerial = 0;  // 对齐 IDA: 序列号计数器
    SS_SERVER_INFO m_stServerInfo{};
    std::map<UXMapID, std::tr1::shared_ptr<CMazeInfo>> m_mapMazeInfo;  // 对齐 IDA: 迷宫信息映射
    std::map<DWORD, std::tr1::shared_ptr<CMyRoom>> m_mpMyRoomInfo;  // 对齐 IDA: MyRoom 映射
};
