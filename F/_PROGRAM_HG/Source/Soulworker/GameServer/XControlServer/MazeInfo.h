// MazeInfo.h
// CMazeInfo 迷宫信息类定义 (对齐 IDA)

#pragma once

// WIN32 headers - use minimal includes
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdint>
#include <memory>
#include <map>
#include <vector>

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

    // 对齐 IDA 0x140036450: Init 初始化迷宫信息 (ST_CREATE_MODE_MAZE 版本)
    // IDA 签名: void __fastcall CMazeInfo::Init(CMazeInfo *this, ST_CREATE_MODE_MAZE *stServerInfo)
    // ST_CREATE_MODE_MAZE 继承自 ST_MAP_INFO，IDA field_0 即基类
    void Init(ST_CREATE_MODE_MAZE* pCreateMaze) {
        if (pCreateMaze) {
            // 对齐 IDA: 复制整个 ST_MAP_INFO 结构 (600 bytes) - 基类继承，直接赋值
            m_stMazeInfo = *static_cast<ST_MAP_INFO*>(pCreateMaze);
            // 对齐 IDA: 设置用户数和类型
            m_nUserCount = 0;
            m_nType = 2;  // 对齐 IDA: 模式迷宫类型为 2
            GreenDamTan_log(__FILE__, __FUNCTION__, "maze created (ST_CREATE_MODE_MAZE)");
        }
    }

    // 对齐 IDA 0x1400360D0: Init 初始化迷宫信息 (ST_CREATE_MAZE 版本)
    // IDA 签名: void __fastcall CMazeInfo::Init(CMazeInfo *this, ST_CREATE_MAZE *stServerInfo)
    // ST_CREATE_MAZE 继承自 ST_MAP_INFO，IDA field_0 即基类
    void Init(ST_CREATE_MAZE* pCreateMaze) {
        if (!pCreateMaze) return;

        // 对齐 IDA: 复制整个 ST_MAP_INFO 结构 (600 bytes) - 基类继承，直接赋值
        m_stMazeInfo = *static_cast<ST_MAP_INFO*>(pCreateMaze);

        // 对齐 IDA: 复制队伍信息
        m_stPartyInfo = pCreateMaze->stPartyInfo;

        // 对齐 IDA: 复制成员列表
        m_vecEnterMember.clear();
        if (!pCreateMaze->vecEnterMember.empty()) {
            m_vecEnterMember.assign(pCreateMaze->vecEnterMember.begin(), pCreateMaze->vecEnterMember.end());
        }

        // 对齐 IDA: 设置用户数为 0
        m_nUserCount = 0;

        // 对齐 IDA: 清空等待进入用户映射
        m_mapWaitEnterMazeUser.clear();

        // 对齐 IDA: 遍历 vecEnterMember，初始化等待进入用户
        for (size_t i = 0; i < pCreateMaze->vecEnterMember.size(); ++i) {
            ST_MAZE_WAIT_ENTER_USER_INFO stInfo{};
            stInfo.stMemberInfo = pCreateMaze->vecEnterMember[i];
            stInfo.dw64ExitTime = 0;
            stInfo.byState = 10;  // 对齐 IDA: 初始状态为 10
            // 对齐 IDA: 使用 stMemberInfo.dwMember 作为 key
            m_mapWaitEnterMazeUser[stInfo.stMemberInfo.dwMember] = stInfo;
        }

        GreenDamTan_log(__FILE__, __FUNCTION__, "maze created (ST_CREATE_MAZE)");
    }

    // 对齐 IDA 0x140036410: Init 初始化迷宫信息 (PS_CREATE_MAP 版本 - AddMap 使用)
    // IDA: this->m_stMazeInfo.uxMapID = stCreateMap->uxMapID.nMapID; this->m_nUserCount = stCreateMap->nCurUserCount;
    void Init(PS_CREATE_MAP& stMap) {
        m_stMazeInfo.uxMapID = stMap.uxMapID;
        m_nUserCount = stMap.nCurUserCount;  // 对齐 IDA: 添加 nCurUserCount 设置
        GreenDamTan_log(__FILE__, __FUNCTION__, "maze created (PS_CREATE_MAP)");
    }

    // 对齐 IDA: 获取地图ID
    UXMapID GetMapID() const { return m_stMazeInfo.uxMapID; }

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

    // 对齐 IDA 0x140036DA0: IsEnterMember - 检查用户是否为成员
    bool IsEnterMember(std::uint32_t dwActorID) const {
        for (const auto& member : m_vecEnterMember) {
            if (member.dwMember == dwActorID) {
                return true;
            }
        }
        return false;
    }

    // 对齐 IDA 0x140027B50: GetParentMaze 获取父迷宫
    std::shared_ptr<CMazeInfo> GetParentMaze() const {
        return m_pParentMaze;
    }

    // 对齐 IDA: GetMazeInfo 获取迷宫信息 (ST_MAP_INFO 版本)
    ST_MAP_INFO GetMazeInfo() const {
        return m_stMazeInfo;
    }

    // 对齐 IDA 0x1400370A0: GetMazeInfo - 填充 PS_ENTER_MAP_RES
    // IDA: nPortalID is also copied
    void GetMazeInfo(PS_ENTER_MAP_RES* stEnterMap) {
        stEnterMap->dwServerID = m_stMazeInfo.dwServerID;
        stEnterMap->nJumpID = m_stMazeInfo.nJumpID;
        stEnterMap->nPortalID = m_stMazeInfo.nPortalID;  // 对齐 IDA: 添加 nPortalID
        stEnterMap->uxMapID = m_stMazeInfo.uxMapID;
        stEnterMap->uxParentInstanceID = m_stMazeInfo.uxParentInstanceID;
        memcpy(stEnterMap->szIP, m_stMazeInfo.szIP, sizeof(stEnterMap->szIP));
        stEnterMap->sPort = m_stMazeInfo.sPort;
        stEnterMap->stPosInfo = m_stMazeInfo.stPosInfo;
        stEnterMap->byType = m_stMazeInfo.byType;
    }

    // 对齐 IDA 0x140036C10: IsValidEnterMaze 检查是否可以进入
    // IDA 签名: __int64 __fastcall CMazeInfo::IsValidEnterMaze(CMazeInfo *this, unsigned int dwActorID, UXMapID *uxMapID)
    // 返回值: 0=成功, 55043=状态2(非Apocalypse), 55054=状态4, 55022=其他错误状态, 55042=非成员, 55036=断线状态错误
    int IsValidEnterMaze(std::uint32_t dwActorID, UXMapID* puxMapID) {
        // 对齐 IDA: 检查迷宫类型 (从 TB_MAZE_INFO 获取)
        // 特殊类型: Maze_Type == 9, 8, 2 为 ApocalypseRaid
        bool bApocalypsRaid = false;

        // 对齐 IDA: 从 uxMapID 提取 mapID
        // IDA: v8 = uxMapID->nMapID << 16 >> 48; (提取 bits 32-47)
        std::uint16_t wMapID = static_cast<std::uint16_t>((puxMapID->nMapID >> 32) & 0xFFFF);

        // TODO: 需要从 XResourceMgr 获取 TB_MAZE_INFO 确认迷宫类型
        // auto pControlServer = XControlServer::Instance();
        // if (pControlServer) {
        //     auto pTBMazeInfo = XResourceMgr::GetTB_MAZE_INFO(&pControlServer->m_xResourceMgr, wMapID);
        //     if (pTBMazeInfo && (pTBMazeInfo->Maze_Type == 9 || pTBMazeInfo->Maze_Type == 8 || pTBMazeInfo->Maze_Type == 2)) {
        //         bApocalypsRaid = true;
        //     }
        // }

        // 对齐 IDA: 检查迷宫状态
        int nMazeState = m_nState;
        if (nMazeState == 2 && !bApocalypsRaid) {
            return 55043;  // 状态2错误(非Apocalypse)
        }
        if (nMazeState == 4) {
            return 55054;  // 状态4错误
        }
        if (nMazeState != 1) {
            if (!bApocalypsRaid) {
                return 55022;  // 其他错误状态
            }
            if (nMazeState != 2) {
                return 55022;  // Apocalypse非状态2也错误
            }
        }

        // 对齐 IDA: 检查是否为成员
        if (!IsEnterMember(dwActorID)) {
            return 55042;  // 非成员
        }

        // 对齐 IDA: 检查断线用户状态
        std::uint8_t byRealState = 0;
        std::uint8_t nCheckResult = CheckDisconnecUsertState(dwActorID, byRealState);

        // 对齐 IDA: if ( result == 1 || byRealState && byRealState != 3 ) return 0; else return 55036;
        if (nCheckResult == 1 || (byRealState && byRealState != 3)) {
            return 0;  // 成功
        }
        return 55036;  // 断线状态错误
    }

    // 对齐 IDA 0x140029070: SetMazeState - 设置迷宫状态
    void SetMazeState(int nState, unsigned __int64 dwTime) {
        m_nState = nState;
        m_dwStateTime = dwTime;
    }

    // 对齐 IDA: GetMazeState - 获取迷宫状态
    int GetMazeState() const { return m_nState; }

    // 对齐 IDA 0x140036D70: IsDestroy - 检查是否销毁状态
    // IDA: return this->m_nState == 3 && this->m_dwStateTime < GetTickCount64();
    bool IsDestroy() const { return m_nState == 3 && m_dwStateTime < GetTickCount64(); }

    // 对齐 IDA: GetMazeType - 获取迷宫类型
    int GetMazeType() const { return m_nType; }

    // 对齐 IDA 0x140039490: GetUserCount - 获取用户数量
    int GetUserCount() const { return m_nUserCount; }

    // 对齐 IDA 0x140036B60: ResetChildMaze - 重置子迷宫引用
    void ResetChildMaze() {
        m_pChildMaze.reset();
    }

    // 对齐 IDA: GetChildMaze - 获取子迷宫引用
    std::shared_ptr<CMazeInfo> GetChildMaze() const {
        return m_pChildMaze;
    }

    // 对齐 IDA 0x140036B80: ResetParentMaze - 重置父迷宫引用
    void ResetParentMaze(CServer* pServer);

    // 对齐 IDA 0x140036560: UpdateMazeInfo - 更新迷宫信息
    // IDA 签名: void __fastcall CMazeInfo::UpdateMazeInfo(CMazeInfo *this, PS_MAZE_UPDATE_INFO *stMazeInfo)
    void UpdateMazeInfo(PS_MAZE_UPDATE_INFO* stMazeInfo);

    // 对齐 IDA 0x140036820: SyncMazeInfo - 同步迷宫信息
    void SyncMazeInfo(PS_MAZE_UPDATE_INFO_SYNC* stMazeInfo);

    // 对齐 IDA 0x140036E20: SetDisconnectUserState - 设置断线用户状态
    void SetDisconnectUserState(ST_MAZE_WAIT_ENTER_USER_INFO& stDisconnect) {
        // 对齐 IDA: 使用 stMemberInfo.dwMember 作为 key
        auto it = m_mapWaitEnterMazeUser.find(stDisconnect.stMemberInfo.dwMember);
        if (it != m_mapWaitEnterMazeUser.end()) {
            it->second = stDisconnect;
        }
    }

    // 对齐 IDA 0x140036EC0: CheckDisconnecUsertState - 检查断线用户状态
    // 返回值: 0=未找到/无Party/非Apoc state11, 1=找到(普通), 2=Apoc state10, 3=Apoc state12->13
    // 对齐 IDA ApocalypseRaid: state=10返回2, state=11返回0, state=12设为13返回3
    // 实现在 MazeInfo.cpp 中（需要完整 XControlServer 定义）
    std::uint8_t CheckDisconnecUsertState(std::uint32_t dwUCID, std::uint8_t& byState);

private:
    // 对齐 IDA 0x140035F10 (CMazeInfo::CMazeInfo) 成员布局 - 728 bytes, 10 members
    // +0x00 (16 bytes): m_pParentMaze
    std::shared_ptr<CMazeInfo> m_pParentMaze;
    // +0x10 (16 bytes): m_pChildMaze
    std::shared_ptr<CMazeInfo> m_pChildMaze;
    // +0x20 (8 bytes): m_stPartyInfo
    ST_PARTY_INFO m_stPartyInfo{};
    // +0x28 (32 bytes): m_vecEnterMember
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> m_vecEnterMember;
    // +0x48 (4 bytes): m_nState
    int m_nState = 1;           // 对齐 IDA: 初始状态为 1 (正常)
    // +0x50 (8 bytes): m_dwStateTime
    unsigned __int64 m_dwStateTime = 0;
    // +0x58 (600 bytes): m_stMazeInfo
    ST_MAP_INFO m_stMazeInfo{};
    // +0x2B8 (4 bytes): m_nUserCount
    int m_nUserCount = 0;       // 对齐 IDA: 用户数量
    // +0x2BC (4 bytes): m_nType
    int m_nType = 0;            // 对齐 IDA: 迷宫类型
    // +0x2C0 (32 bytes): m_mapWaitEnterMazeUser
    std::map<std::uint32_t, ST_MAZE_WAIT_ENTER_USER_INFO> m_mapWaitEnterMazeUser;
};
static_assert(sizeof(CMazeInfo) == 728, "CMazeInfo size mismatch with IDA");