// MazeInfo.cpp
// CMazeInfo 迷宫信息类实现 (对齐 IDA)

#include "MazeInfo.h"
#include "ControlServer.h"
#include "CServer.h"
#include <algorithm>
#include <cstdio>

// 对齐 IDA 0x140035F10 (CMazeInfo::CMazeInfo)
CMazeInfo::CMazeInfo()
    : m_nState(1)
    , m_dwStateTime(0)
    , m_nUserCount(0)
    , m_nType(0)
{
    // 对齐 IDA: 初始化父/子迷宫引用为 nullptr
    m_pParentMaze.reset();
    m_pChildMaze.reset();

    // 对齐 IDA: 清空成员列表
    m_vecEnterMember.clear();

    // 对齐 IDA: 清空迷宫信息
    memset(&m_stMazeInfo, 0, sizeof(m_stMazeInfo));

    // 对齐 IDA: 清空等待进入用户映射
    m_mapWaitEnterMazeUser.clear();
}

// 对齐 IDA 0x140036B80: ResetParentMaze - 重置父迷宫引用
// IDA: if (m_pParentMaze valid) { pParent = m_pParentMaze->; if (pParent->GetUserCount() == 0) SetMazeState(3, time); pParent->ResetChildMaze(); m_pParentMaze.reset(); }
void CMazeInfo::ResetParentMaze(CServer* pServer)
{
    // 对齐 IDA: 检查 m_pParentMaze 是否有效
    if (m_pParentMaze) {
        // 对齐 IDA: 获取父迷宫指针
        auto pParent = m_pParentMaze;
        // 对齐 IDA: 检查父迷宫的用户数
        if (pParent->GetUserCount() == 0) {
            // 对齐 IDA: 设置删除状态，60秒后删除
            ULONGLONG dwTime = GetTickCount64() + 60000;
            pParent->SetMazeState(3, dwTime);
        }
        // 对齐 IDA: pParent->ResetChildMaze() (父迷宫重置其子迷宫引用)
        pParent->ResetChildMaze();
        // 对齐 IDA: m_pParentMaze.reset() (清空当前迷宫的父迷宫引用)
        m_pParentMaze.reset();
    }
}

// 对齐 IDA 0x140036560: UpdateMazeInfo - 更新迷宫信息
// IDA 签名: void __fastcall CMazeInfo::UpdateMazeInfo(CMazeInfo *this, PS_MAZE_UPDATE_INFO *stMazeInfo)
void CMazeInfo::UpdateMazeInfo(PS_MAZE_UPDATE_INFO* stMazeInfo)
{
    if (!stMazeInfo) return;

    // 对齐 IDA: if (m_nState != 3) 才执行更新
    if (m_nState == 3) {
        // 已处于删除状态，跳过更新
        return;
    }

    // 对齐 IDA: 更新地图信息
    m_stMazeInfo.uxMapID = stMazeInfo->uxMapID;
    m_nUserCount = stMazeInfo->nUserCount;
    m_nState = stMazeInfo->nState;

    // 对齐 IDA: 清空成员列表
    m_vecEnterMember.clear();
    m_mapWaitEnterMazeUser.clear();

    // 对齐 IDA: 复制成员信息
    for (size_t i = 0; i < stMazeInfo->vecMemberInfo.size(); ++i) {
        const auto& member = stMazeInfo->vecMemberInfo[i];
        // 对齐 IDA: 转换 ST_MAZE_WAIT_ENTER_USER_INFO 到 ST_ENTER_MAZE_MEMBER_INFO
        // IDA 显示 push_back 接受 ST_MAZE_WAIT_ENTER_USER_INFO* (直接复制)
        ST_ENTER_MAZE_MEMBER_INFO enterMember;
        enterMember.dwMember = member.stMemberInfo.dwMember;
        enterMember.nState = member.stMemberInfo.nState;
        m_vecEnterMember.push_back(enterMember);

        // 对齐 IDA: 使用 stMemberInfo.dwMember 作为 key (UCID/ActorID)
        m_mapWaitEnterMazeUser[member.stMemberInfo.dwMember] = member;
    }

    // 对齐 IDA: 记录日志 (使用 LogHelper::LogDebug)
    LogHelper::LogDebug("game.relay",
        "<UPDATE_MAZE> Update State ( %I64d / %d / %d )",
        stMazeInfo->uxMapID.nMapID,
        stMazeInfo->nState,
        static_cast<int>(stMazeInfo->vecMemberInfo.size()));

    // 对齐 IDA: 如果状态变为删除状态且无子迷宫，开始计时
    // IDA: if ( m_nState == 3 && !GetChildMaze() )
    if (m_nState == 3 && !m_pChildMaze) {
        m_dwStateTime = GetTickCount64();
        LogHelper::LogDebug("game.relay",
            "<UPDATE_MAZE> Wait for deleted maze ( %I64d )",
            stMazeInfo->uxMapID.nMapID);
    }
}

// 对齐 IDA 0x140036820: SyncMazeInfo - 同步迷宫信息
// IDA 签名: void __fastcall CMazeInfo::SyncMazeInfo(CMazeInfo *this, PS_MAZE_UPDATE_INFO_SYNC *stMazeInfo)
void CMazeInfo::SyncMazeInfo(PS_MAZE_UPDATE_INFO_SYNC* stMazeInfo)
{
    if (!stMazeInfo) {
        return;
    }

    // 对齐 IDA: 复制队伍信息
    m_stPartyInfo = stMazeInfo->stPartyInfo;

    // 对齐 IDA: 复制服务器信息
    m_stMazeInfo.dwServerID = stMazeInfo->dwServerID;
    m_stMazeInfo.sPort = stMazeInfo->sPort;
    m_stMazeInfo.nJumpID = stMazeInfo->nJumpID;
    strcpy_s(m_stMazeInfo.szIP, sizeof(m_stMazeInfo.szIP), stMazeInfo->szIP);

    // 对齐 IDA: 设置地图ID (从 psMazeInfo)
    m_stMazeInfo.uxMapID = stMazeInfo->psMazeInfo.uxMapID;
    m_nUserCount = stMazeInfo->psMazeInfo.nUserCount;
    m_nState = stMazeInfo->psMazeInfo.nState;

    // 对齐 IDA: 清空成员列表
    m_vecEnterMember.clear();
    m_mapWaitEnterMazeUser.clear();

    // 对齐 IDA: 复制成员信息 (psMazeInfo.vecMemberInfo)
    for (size_t i = 0; i < stMazeInfo->psMazeInfo.vecMemberInfo.size(); ++i) {
        const auto& member = stMazeInfo->psMazeInfo.vecMemberInfo[i];

        // 对齐 IDA: push_back 使用 ST_MAZE_WAIT_ENTER_USER_INFO* (直接复制 stMemberInfo)
        ST_ENTER_MAZE_MEMBER_INFO enterMember;
        enterMember.dwMember = member.dwActorID;  // 对齐 IDA: ST_MAZE_MEMBER_INFO_SYNC 直接字段
        enterMember.nState = static_cast<int>(member.byState);
        m_vecEnterMember.push_back(enterMember);

        // 对齐 IDA: 使用 dwActorID 作为 key (不是 stMemberInfo.dwMember)
        // 将 ST_MAZE_MEMBER_INFO_SYNC 转换为 ST_MAZE_WAIT_ENTER_USER_INFO
        ST_MAZE_WAIT_ENTER_USER_INFO waitUserInfo{};
        waitUserInfo.stMemberInfo.dwMember = member.dwActorID;
        waitUserInfo.stMemberInfo.nState = static_cast<int>(member.byState);
        waitUserInfo.byState = member.byState;
        m_mapWaitEnterMazeUser[member.dwActorID] = waitUserInfo;
    }

    // 对齐 IDA: 记录同步日志 (使用 LogHelper::LogDebug)
    LogHelper::LogDebug("game.relay",
        "<SYNCK_MAZE> Update State ( %I64d / %d / %d )",
        stMazeInfo->psMazeInfo.uxMapID.nMapID,
        stMazeInfo->psMazeInfo.nState,
        static_cast<int>(stMazeInfo->psMazeInfo.vecMemberInfo.size()));

    // 对齐 IDA: 如果状态为删除且无子迷宫，开始计时
    if (m_nState == 3 && !m_pChildMaze) {
        m_dwStateTime = GetTickCount64();
        LogHelper::LogDebug("game.relay",
            "<SYNCK_MAZE> Wait for deleted maze ( %I64d )",
            stMazeInfo->psMazeInfo.uxMapID.nMapID);
    }
}

// 对齐 IDA 0x140036EC0: CheckDisconnecUsertState - 检查断线用户状态
// 返回值: 0=未找到/无Party/非Apoc state11, 1=找到(普通), 2=Apoc state10, 3=Apoc state12->13
// 对齐 IDA ApocalypseRaid: state=10返回2, state=11返回0, state=12设为13返回3
std::uint8_t CMazeInfo::CheckDisconnecUsertState(std::uint32_t dwUCID, std::uint8_t& byState) {
    byState = 0;

    // 对齐 IDA: 提取 mapID 用于获取 TB_MAZE_INFO
    // IDA: v11 = this->m_stMazeInfo.uxMapID.nMapID << 16 >> 48 (提取 bits 32-47)
    std::int64_t nMapIDKey = m_stMazeInfo.uxMapID.nMapID << 16 >> 48;

    // 对齐 IDA: 从 XResourceMgr 获取 TB_MAZE_INFO
    auto pControlServer = XControlServer::Instance();
    if (!pControlServer) {
        return 0;
    }

    // TODO: 需要 XResourceMgr::GetTB_MAZE_INFO 实现
    // auto pTBMazeInfo = XResourceMgr::GetTB_MAZE_INFO(&pControlServer->m_xResourceMgr, nMapIDKey);
    TB_MAZE_INFO* pTBMazeInfo = nullptr;  // 临时 null

    if (!pTBMazeInfo) {
        // 对齐 IDA: 记录错误日志 (257)
        LogHelper::LogError("game.contents",
            "CheckDisconnecUsertState - m_pTBMazeInfo NULL [ ActorID:%d, InstanceID:%I64d ] ( %d )",
            dwUCID, m_stMazeInfo.uxMapID.nMapID, 257);
        return 0;
    }

    // 对齐 IDA: 检查 Party ID
    if (m_stPartyInfo.nID <= 0) {
        LogHelper::LogError("game.contents",
            "CheckDisconnecUsertState - m_pParty NULL [ ActorID:%d, InstanceID:%I64d ] ( %d )",
            dwUCID, m_stMazeInfo.uxMapID.nMapID, 263);
        return 0;
    }

    // 对齐 IDA: 查找用户
    auto it = m_mapWaitEnterMazeUser.find(dwUCID);
    if (it == m_mapWaitEnterMazeUser.end()) {
        return 0;  // 未找到
    }

    // 对齐 IDA: 获取用户状态
    byState = it->second.byState;

    // 对齐 IDA: 检查迷宫类型 - ApocalypseRaid (Maze_Type 2, 8, 9)
    if (pTBMazeInfo->Maze_Type == 2 || pTBMazeInfo->Maze_Type == 8 || pTBMazeInfo->Maze_Type == 9) {
        // 对齐 IDA: ApocalypseRaid 特殊处理
        std::uint8_t v12 = byState;
        if (v12 == 10) {
            return 2;  // state=10 返回 2
        }
        if (v12 == 11) {
            return 0;  // state=11 返回 0
        }
        if (v12 == 12) {
            // 对齐 IDA: state=12 -> 设置为 13，返回 3
            it->second.byState = 13;
            byState = 13;
            return 3;
        }
        return 0;  // 其他状态返回 0
    }
    else {
        // 对齐 IDA: 非 ApocalypseRaid 返回 1
        return 1;
    }
}
