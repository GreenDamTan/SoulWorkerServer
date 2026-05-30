// CutsceneManager.cpp - Cutscene Management System Implementation
// Restored from GameServer.exe IDA decompilation

#include "CutsceneManager.h"
#include "Maze.h"
#include "User.h"
#include <cstring>

// IDA: ??0CCutsceneManager@@QEAA@XZ (0x1401B0540)
// Verified: Direct IDA decompilation
CCutsceneManager::CCutsceneManager()
    : m_pMaze(nullptr)
{
    // IDA: 构造函数初始化 m_pMaze 为 nullptr
    // maps 使用默认构造函数
}

// IDA: ??1CCutsceneManager@@QEAA@XZ (0x1401B05B0)
// Verified: Direct IDA decompilation
CCutsceneManager::~CCutsceneManager()
{
    // IDA: 析构函数调用 map 析构函数，自动清理
    // maps 会在析构时自动清理
}

// IDA: ?Init@CCutsceneManager@@QEAAXPEAVXMaze@@@Z (0x1401B05F0)
// Verified: Direct IDA decompilation
void CCutsceneManager::Init(XMaze* pMaze)
{
    // IDA decompiled:
    // this->m_pMaze = pMaze;
    // this->m_bSyncCutsceneState = 0;
    // this->m_strFinishCutsceneName[0] = 0;
    // this->m_bClientCutscenePlay = 0;
    // this->m_bNotifyMessage = 0;

    m_pMaze = pMaze;
    m_bSyncCutsceneState = false;
    m_strFinishCutsceneName[0] = '\0';
    m_bClientCutscenePlay = false;
    m_bNotifyMessage = false;
}

// IDA: ?Clear@CCutsceneManager@@QEAAXXZ (0x1401B0630)
// Verified: Direct IDA decompilation
void CCutsceneManager::Clear()
{
    // IDA decompiled:
    // std::_Tree<std::_Tmap_traits<...>>::clear(&this->m_mapCutsceneMember);
    // std::_Tree<std::_Tmap_traits<...>>::clear(&this->m_mapCutsceneInfo);
    // this->m_strFinishCutsceneName[0] = 0;
    // this->m_bClientCutscenePlay = 0;
    // this->m_bNotifyMessage = 0;

    m_mapCutsceneMember.clear();
    m_mapCutsceneInfo.clear();
    m_strFinishCutsceneName[0] = '\0';
    m_bClientCutscenePlay = false;
    m_bNotifyMessage = false;
}

// IDA: ?AddMember@CCutsceneManager@@QEAAXPEAVCUser@@@Z (0x1401B0680)
void CCutsceneManager::AddMember(CUser* pUser)
{
    // TODO: 汇编还原 - 需要完整实现
    // IDA: 将用户添加到 m_mapCutsceneMember
    if (!pUser) {
        return;
    }
    // m_mapCutsceneMember[dwActorID] = pUser;
}

// IDA: ?DeleteMember@CCutsceneManager@@QEAAXK@Z (0x1401B07A0)
void CCutsceneManager::DeleteMember(std::uint32_t dwActorID)
{
    // TODO: 汇编还原 - 需要完整实现
    m_mapCutsceneMember.erase(dwActorID);
}

// IDA: ?ClearMember@CCutsceneManager@@QEAAXXZ (0x1401B0830)
void CCutsceneManager::ClearMember()
{
    // TODO: 汇编还原 - 需要完整实现
    m_mapCutsceneMember.clear();
}

// IDA: ?ChangeState@CCutsceneManager@@QEAAXXZ (0x1401B0A50)
void CCutsceneManager::ChangeState()
{
    // TODO: 汇编还原 - 复杂的状态切换逻辑
}

// IDA: ?AddCutsceneInfo@CCutsceneManager@@QEAAXPEADHKHH@Z (0x1401B1070)
void CCutsceneManager::AddCutsceneInfo(char* pszCutsceneName, std::uint32_t dwCutsceneID,
                                        std::int32_t nOrder, std::int32_t nSectorID,
                                        std::int32_t nClientCutscene)
{
    // TODO: 汇编还原 - 需要完整实现
    if (!pszCutsceneName) {
        return;
    }

    ST_CUTSCENE_INFO info;
    info.dwCutsceneID = dwCutsceneID;
    info.nOrder = nOrder;
    info.nSectorID = nSectorID;
    info.bClientCutscene = (nClientCutscene != 0);

    m_mapCutsceneInfo[std::string(pszCutsceneName)] = info;
}

// IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXHH@Z (0x1401B1240)
void CCutsceneManager::CheckCutsceneState(std::int32_t nSectorID, std::int32_t nState)
{
    // TODO: 汇编还原 - 需要 XMaze 和 CUser 类型
}

// IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXKHH@Z (0x1401B17C0)
void CCutsceneManager::CheckCutsceneState(std::uint32_t dwActorID, std::int32_t nSectorID, std::int32_t nState)
{
    // TODO: 汇编还原 - 需要 XMaze 和 CUser 类型
}

// IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B1C40)
void CCutsceneManager::CheckCutsceneState(std::uint32_t dwActorID, char* pszCutsceneName)
{
    // TODO: 汇编还原 - 需要 XMaze 和 CUser 类型
}

// IDA: ?OnUpdate@CCutsceneManager@@QEAAXXZ (0x1401B2090)
void CCutsceneManager::OnUpdate()
{
    // TODO: 汇编还原 - 更新逻辑
}

// IDA: ?SkipCutscene@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B2280)
void CCutsceneManager::SkipCutscene(std::uint32_t dwActorID, char* pszCutsceneName)
{
    // TODO: 汇编还原 - 跳过过场动画逻辑
}

// IDA: ?SetCutscene@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B24B0)
void CCutsceneManager::SetCutscene(std::uint32_t dwActorID, char* pszCutsceneName)
{
    // TODO: 汇编还原 - 设置过场动画逻辑
}

// IDA: ?GetCutsceneOrder@CCutsceneManager@@QEAAHH@Z (0x1401B2750)
std::int32_t CCutsceneManager::GetCutsceneOrder(std::int32_t nSectorID)
{
    // TODO: 汇编还原 - 获取过场动画顺序
    for (const auto& pair : m_mapCutsceneInfo) {
        if (pair.second.nSectorID == nSectorID) {
            return pair.second.nOrder;
        }
    }
    return 0;
}

// IDA: ?IsPlayCutscene@CCutsceneManager@@QEAA_NK@Z (0x1401B27A0)
bool CCutsceneManager::IsPlayCutscene(std::uint32_t dwCutsceneID)
{
    // TODO: 汇编还原 - 检查是否正在播放
    for (const auto& pair : m_mapCutsceneInfo) {
        if (pair.second.dwCutsceneID == dwCutsceneID && pair.second.nState == 1) {
            return true;
        }
    }
    return false;
}
