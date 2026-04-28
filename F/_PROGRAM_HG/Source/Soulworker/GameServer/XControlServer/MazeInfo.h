// MazeInfo.h
// CMazeInfo 迷宫信息类定义 (对齐 IDA)

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdint>
#include <memory>
#include <map>
#include <vector>

// 对齐 IDA: std::tr1 命名空间别名 (C++11 后 tr1 已合并到 std)
namespace std { namespace tr1 = std; }

// 前向声明
class CMazeInfo;
class CServer;

// 对齐 IDA: PS_MAZE_UPDATE_INFO 结构体定义 (如果 PSServer.h 中未定义)
#ifndef PS_MAZE_UPDATE_INFO_DEFINED
struct PS_MAZE_UPDATE_INFO {
    UXMapID uxMapID{};
    int nUserCount = 0;
    int nState = 0;
    std::vector<ST_MAZE_WAIT_ENTER_USER_INFO> vecMemberInfo;
};
#define PS_MAZE_UPDATE_INFO_DEFINED

// 对齐 IDA: PS_MAZE_UPDATE_INFO 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_UPDATE_INFO& value) {
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nUserCount;
    packet.XParse >> value.nState;
    int nCount = 0;
    packet.XParse >> nCount;
    value.vecMemberInfo.clear();
    value.vecMemberInfo.reserve(nCount);
    for (int i = 0; i < nCount; ++i) {
        ST_MAZE_WAIT_ENTER_USER_INFO member{};
        packet >> member;
        value.vecMemberInfo.push_back(member);
    }
}

#endif

// 对齐 IDA 0x140035F10 (CMazeInfo::CMazeInfo)
// CMazeInfo 迷宫信息类
class CMazeInfo {
public:
    CMazeInfo();
    virtual ~CMazeInfo() = default;

    // 对齐 IDA: Init 初始化迷宫信息
    void Init(ST_CREATE_MODE_MAZE* pCreateMaze) {
        if (pCreateMaze) {
            m_uxMapID = pCreateMaze->uxMapID;
            m_wReqMapID = pCreateMaze->wReqMapID;
            // 填充 m_stMazeInfo
            m_stMazeInfo.uxMapID = pCreateMaze->uxMapID;
            m_stMazeInfo.dwServerID = pCreateMaze->dwServerID;
            m_stMazeInfo.nJumpID = pCreateMaze->wReqMapID;
            memcpy(m_stMazeInfo.szIP, pCreateMaze->szIP, sizeof(m_stMazeInfo.szIP));
            m_stMazeInfo.sPort = pCreateMaze->sPort;
            GreenDamTan_log(__FILE__, __FUNCTION__, "maze created");
        }
    }

    // 对齐 IDA: Init 初始化迷宫信息 (ST_CREATE_MAZE 版本)
    void Init(ST_CREATE_MAZE& stCreateMaze) {
        m_uxMapID = stCreateMaze.uxMapID;
        m_wReqMapID = stCreateMaze.wReqMapID;
        m_dwServerID = stCreateMaze.dwServerID;
        m_uxParentMazeID = stCreateMaze.uxParentInstanceID;
        memcpy(m_szIP, stCreateMaze.szIP, sizeof(m_szIP));
        m_sPort = stCreateMaze.sPort;
        // 填充 m_stMazeInfo
        m_stMazeInfo.uxMapID = stCreateMaze.uxMapID;
        m_stMazeInfo.dwServerID = stCreateMaze.dwServerID;
        m_stMazeInfo.nJumpID = stCreateMaze.wReqMapID;
        m_stMazeInfo.uxParentInstanceID = stCreateMaze.uxParentInstanceID;
        memcpy(m_stMazeInfo.szIP, stCreateMaze.szIP, sizeof(m_stMazeInfo.szIP));
        m_stMazeInfo.sPort = stCreateMaze.sPort;
        // 复制队伍信息 (ST_PARTY_INFO has byGroupType and nID)
        m_stPartyInfo.byGroupType = stCreateMaze.stPartyInfo.byGroupType;
        m_stPartyInfo.nID = stCreateMaze.stPartyInfo.nID;
        // 复制成员列表
        m_vecEnterMember = stCreateMaze.vecEnterMember;
        GreenDamTan_log(__FILE__, __FUNCTION__, "maze created (ST_CREATE_MAZE)");
    }

    // 对齐 IDA: Init 初始化迷宫信息 (PS_CREATE_MAP 版本 - AddMap 使用)
    void Init(PS_CREATE_MAP& stMap) {
        m_uxMapID = stMap.uxMapID;
        m_stMazeInfo.uxMapID = stMap.uxMapID;
        GreenDamTan_log(__FILE__, __FUNCTION__, "maze created (PS_CREATE_MAP)");
    }

    // 获取地图ID
    UXMapID GetMapID() const { return m_uxMapID; }

    // 对齐 IDA 0x140036500: GetPartyID - 获取队伍ID
    int GetPartyID() const {
        if (m_stPartyInfo.byGroupType == 1) {
            return m_stPartyInfo.nID;
        }
        return 0;
    }

    // 对齐 IDA 0x140036530: GetForceID - 获取Force ID
    int GetForceID() const {
        if (m_stPartyInfo.byGroupType == 2) {
            return m_stPartyInfo.nID;
        }
        return 0;
    }

    // 对齐 IDA 0x140006690: GetMemberVector - 获取成员列表
    std::vector<ST_ENTER_MAZE_MEMBER_INFO>* GetMemberVector() {
        return &m_vecEnterMember;
    }

    // 对齐 IDA: GetParentMaze 获取父迷宫
    std::tr1::shared_ptr<CMazeInfo> GetParentMaze() const {
        if (m_uxParentMazeID.nMapID == 0) {
            return nullptr;
        }
        // Stub: 返回 nullptr 表示父迷宫未找到
        return nullptr;
    }

    // 对齐 IDA: GetMazeInfo 获取迷宫信息 (ST_MAP_INFO 版本)
    ST_MAP_INFO GetMazeInfo() const {
        return m_stMazeInfo;
    }

    // 对齐 IDA 0x1400370A0: GetMazeInfo - 填充 PS_ENTER_MAP_RES
    void GetMazeInfo(PS_ENTER_MAP_RES* stEnterMap) {
        stEnterMap->dwServerID = m_stMazeInfo.dwServerID;
        stEnterMap->nJumpID = m_stMazeInfo.nJumpID;
        stEnterMap->uxMapID = m_stMazeInfo.uxMapID;
        stEnterMap->uxParentInstanceID = m_stMazeInfo.uxParentInstanceID;
        memcpy(stEnterMap->szIP, m_stMazeInfo.szIP, sizeof(stEnterMap->szIP));
        stEnterMap->sPort = m_stMazeInfo.sPort;
        stEnterMap->stPosInfo = m_stMazeInfo.stPosInfo;
        stEnterMap->byType = m_stMazeInfo.byType;
    }

    // 对齐 IDA: IsValidEnterMaze 检查是否可以进入
    int IsValidEnterMaze(std::uint32_t dwActorID, UXMapID* puxMapID) const {
        // 当前返回 0 表示可以进入
        return 0;
    }

    // 对齐 IDA 0x140029070: SetMazeState - 设置迷宫状态
    void SetMazeState(int nState, unsigned __int64 dwTime) {
        m_nState = nState;
        m_dwStateTime = dwTime;
    }

    // 对齐 IDA: GetMazeState - 获取迷宫状态
    int GetMazeState() const { return m_nState; }

    // 对齐 IDA: IsDestroy - 检查是否销毁状态
    bool IsDestroy() const { return m_nState == 3; }

    // 对齐 IDA: GetMazeType - 获取迷宫类型
    int GetMazeType() const { return m_nType; }

    // 对齐 IDA 0x140039490: GetUserCount - 获取用户数量
    int GetUserCount() const { return m_nUserCount; }

    // 对齐 IDA 0x140036B60: ResetChildMaze - 重置子迷宫引用
    void ResetChildMaze() {
        m_pChildMaze.reset();
    }

    // 对齐 IDA: GetChildMaze - 获取子迷宫引用
    std::tr1::shared_ptr<CMazeInfo> GetChildMaze() const {
        return m_pChildMaze;
    }

    // 对齐 IDA 0x140036B80: ResetParentMaze - 重置父迷宫引用
    void ResetParentMaze(CServer* pServer);

    // 对齐 IDA 0x140036560: UpdateMazeInfo - 更新迷宫信息
    void UpdateMazeInfo(PS_MAZE_UPDATE_INFO& stMazeInfo);

    // 对齐 IDA 0x140036820: SyncMazeInfo - 同步迷宫信息
    void SyncMazeInfo(PS_MAZE_UPDATE_INFO_SYNC* stMazeInfo);

    // 对齐 IDA 0x140036E20: SetDisconnectUserState - 设置断线用户状态
    void SetDisconnectUserState(ST_MAZE_WAIT_ENTER_USER_INFO& stDisconnect) {
        auto it = m_mapWaitEnterMazeUser.find(stDisconnect.dwUCID);
        if (it != m_mapWaitEnterMazeUser.end()) {
            it->second = stDisconnect;
        }
    }

    // 对齐 IDA 0x140036EC0: CheckDisconnecUsertState - 检查断线用户状态
    std::uint8_t CheckDisconnecUsertState(std::uint32_t dwUCID, std::uint8_t& byState) {
        byState = 0;
        auto it = m_mapWaitEnterMazeUser.find(dwUCID);
        if (it == m_mapWaitEnterMazeUser.end()) {
            return 0;
        }
        return 1;
    }

private:
    // 对齐 IDA 0x140035F10 (CMazeInfo::CMazeInfo) 成员布局
    std::tr1::shared_ptr<CMazeInfo> m_pParentMaze;    // 父迷宫引用
    std::tr1::shared_ptr<CMazeInfo> m_pChildMaze;     // 子迷宫引用
    UXMapID m_uxMapID{};
    UXMapID m_uxParentMazeID{};
    std::uint16_t m_wReqMapID = 0;
    std::uint32_t m_dwServerID = 0;  // 对齐 IDA: 使用 uint32_t 替代 DWORD
    char m_szIP[513] = {};
    std::int16_t m_sPort = 0;
    int m_nState = 1;           // 对齐 IDA: 初始状态为 1 (正常)
    unsigned __int64 m_dwStateTime = 0;
    int m_nUserCount = 0;       // 对齐 IDA: 用户数量
    int m_nType = 0;            // 对齐 IDA: 迷宫类型
    ST_MAP_INFO m_stMazeInfo{};  // 对齐 IDA: 迷宫信息结构体
    ST_PARTY_INFO m_stPartyInfo{};  // 对齐 IDA: 队伍信息 (使用 ST_PARTY_INFO，有 byGroupType 和 nID)
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> m_vecEnterMember;  // 对齐 IDA: 成员列表
    std::map<std::uint32_t, ST_MAZE_WAIT_ENTER_USER_INFO> m_mapWaitEnterMazeUser;
};