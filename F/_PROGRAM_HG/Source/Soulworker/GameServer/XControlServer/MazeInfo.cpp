// MazeInfo.cpp
// CMazeInfo 迷宫信息类实现 (对齐 IDA)

#include "MazeInfo.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
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
void CMazeInfo::ResetParentMaze(CServer* pServer)
{
    if (m_pParentMaze) {
        auto pParent = m_pParentMaze;
        if (pParent->GetUserCount() == 0) {
            // 对齐 IDA: 设置删除状态，60秒后删除
            ULONGLONG dwTime = GetTickCount64() + 60000;
            pParent->SetMazeState(3, dwTime);
        }
        pParent->ResetChildMaze();
        m_pParentMaze.reset();
    }
}

// 对齐 IDA 0x140036560: UpdateMazeInfo - 更新迷宫信息
void CMazeInfo::UpdateMazeInfo(PS_MAZE_UPDATE_INFO& stMazeInfo)
{
    if (m_nState == 3) {
        // 已处于删除状态，跳过更新
        return;
    }

    // 对齐 IDA: 更新地图信息
    m_stMazeInfo.uxMapID = stMazeInfo.uxMapID;
    m_nUserCount = stMazeInfo.nUserCount;
    m_nState = stMazeInfo.nState;

    // 对齐 IDA: 清空成员列表
    m_vecEnterMember.clear();
    m_mapWaitEnterMazeUser.clear();

    // 对齐 IDA: 复制成员信息
    for (size_t i = 0; i < stMazeInfo.vecMemberInfo.size(); ++i) {
        const auto& member = stMazeInfo.vecMemberInfo[i];
        // 对齐 IDA: 转换 ST_MAZE_WAIT_ENTER_USER_INFO 到 ST_ENTER_MAZE_MEMBER_INFO
        ST_ENTER_MAZE_MEMBER_INFO enterMember;
        enterMember.dwMember = member.dwActorID;  // 使用 dwActorID
        enterMember.nState = member.byState;
        m_vecEnterMember.push_back(enterMember);

        // 对齐 IDA: 使用 dwUCID 作为 key
        m_mapWaitEnterMazeUser[member.dwUCID] = member;
    }

    // 对齐 IDA: 记录日志
    char szLog[256];
    snprintf(szLog, sizeof(szLog), "UpdateMazeInfo: uxMapID=0x%llx, nState=%d, nUserCount=%d",
        stMazeInfo.uxMapID.nMapID, stMazeInfo.nState, m_nUserCount);
    GreenDamTan_log(__FILE__, __FUNCTION__, szLog);

    // 对齐 IDA: 如果状态变为删除状态且无子迷宫，开始计时
    if (m_nState == 3 && !m_pChildMaze) {
        m_dwStateTime = GetTickCount64();
        char szLog2[256];
        snprintf(szLog2, sizeof(szLog2), "Wait for deleted maze, uxMapID=0x%llx", stMazeInfo.uxMapID.nMapID);
        GreenDamTan_log(__FILE__, __FUNCTION__, szLog2);
    }
}

// 对齐 IDA 0x140036820: SyncMazeInfo - 同步迷宫信息
void CMazeInfo::SyncMazeInfo(PS_MAZE_UPDATE_INFO_SYNC* stMazeInfo) {
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

    // 对齐 IDA: 设置地图ID
    m_stMazeInfo.uxMapID = stMazeInfo->psMazeInfo.uxMapID;
    m_uxMapID = stMazeInfo->psMazeInfo.uxMapID;
    m_nUserCount = stMazeInfo->psMazeInfo.nUserCount;
    m_nState = stMazeInfo->psMazeInfo.nState;

    // 对齐 IDA: 清空成员列表
    m_vecEnterMember.clear();
    m_mapWaitEnterMazeUser.clear();

    // 对齐 IDA: 复制成员信息
    for (size_t i = 0; i < stMazeInfo->psMazeInfo.vecMemberInfo.size(); ++i) {
        const auto& member = stMazeInfo->psMazeInfo.vecMemberInfo[i];

        // 对齐 IDA: 添加到成员向量
        ST_ENTER_MAZE_MEMBER_INFO enterMember;
        enterMember.dwMember = member.dwActorID;
        enterMember.nState = member.byState;
        m_vecEnterMember.push_back(enterMember);

        // 对齐 IDA: 添加到等待进入映射 (转换为 ST_MAZE_WAIT_ENTER_USER_INFO)
        ST_MAZE_WAIT_ENTER_USER_INFO waitUserInfo{};
        waitUserInfo.dwActorID = member.dwActorID;
        waitUserInfo.dwUCID = member.dwUCID;
        waitUserInfo.byState = member.byState;
        m_mapWaitEnterMazeUser[member.dwUCID] = waitUserInfo;
    }

    // 对齐 IDA: 记录同步日志
    char szLog[256];
    snprintf(szLog, sizeof(szLog), "SyncMazeInfo: uxMapID=0x%llx, nState=%d, nMemberCount=%zu",
        stMazeInfo->psMazeInfo.uxMapID.nMapID, m_nState, stMazeInfo->psMazeInfo.vecMemberInfo.size());
    GreenDamTan_log(__FILE__, __FUNCTION__, szLog);

    // 对齐 IDA: 如果状态为删除且无子迷宫，开始计时
    if (m_nState == 3 && !m_pChildMaze) {
        m_dwStateTime = GetTickCount64();
        char szLog2[256];
        snprintf(szLog2, sizeof(szLog2), "Wait for deleted maze (sync), uxMapID=0x%llx", stMazeInfo->psMazeInfo.uxMapID.nMapID);
        GreenDamTan_log(__FILE__, __FUNCTION__, szLog2);
    }
}