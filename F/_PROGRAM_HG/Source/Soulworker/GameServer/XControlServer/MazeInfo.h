// MazeInfo.h
// CMazeInfo 迷宫信息类定义 (Stub)

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

// 对齐 IDA: CMazeInfo 迷宫信息类
class CMazeInfo {
public:
    CMazeInfo() = default;
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
        m_nMaxUserCount = stMap.nMaxUserCount;
        m_nCurUserCount = stMap.nCurUserCount;
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
    int IsValidEnterMaze(DWORD dwActorID, UXMapID* puxMapID) const {
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
    UXMapID m_uxMapID{};
    UXMapID m_uxParentMazeID{};
    std::uint16_t m_wReqMapID = 0;
    DWORD m_dwServerID = 0;
    char m_szIP[513] = {};
    std::int16_t m_sPort = 0;
    int m_nState = 0;
    unsigned __int64 m_dwStateTime = 0;
    int m_nMaxUserCount = 0;
    int m_nCurUserCount = 0;
    ST_MAP_INFO m_stMazeInfo{};  // 对齐 IDA: 迷宫信息结构体
    ST_PARTY_INFO m_stPartyInfo{};  // 对齐 IDA: 队伍信息 (使用 ST_PARTY_INFO，有 byGroupType 和 nID)
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> m_vecEnterMember;  // 对齐 IDA: 成员列表
    std::map<std::uint32_t, ST_MAZE_WAIT_ENTER_USER_INFO> m_mapWaitEnterMazeUser;
};