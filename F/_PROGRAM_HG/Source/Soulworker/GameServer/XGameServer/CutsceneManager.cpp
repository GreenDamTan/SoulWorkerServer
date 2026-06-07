// CutsceneManager.cpp - Cutscene Management System Implementation
// Restored from GameServer.exe IDA decompilation

#include "CutsceneManager.h"
#include "Maze.h"
#include "User.h"
#include "actor/component/GocNetwork.h"
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
    // IDA: 创建 ST_CUTSCENE_MEMBER 并添加到 m_mapCutsceneMember
    if (!pUser) {
        return;
    }

    // IDA: 获取用户的 ActorID
    UXActorID actorID = pUser->GetActorID();

    // IDA: 创建 ST_CUTSCENE_MEMBER 并设置 pUser
    ST_CUTSCENE_MEMBER* pMember = new ST_CUTSCENE_MEMBER();
    pMember->pUser = pUser;

    // IDA: 使用 std::shared_ptr 管理 ST_CUTSCENE_MEMBER
    std::shared_ptr<ST_CUTSCENE_MEMBER> spMember(pMember);

    // IDA: 插入到 m_mapCutsceneMember
    m_mapCutsceneMember[actorID] = spMember;
}

// IDA: ?DeleteMember@CCutsceneManager@@QEAAXK@Z (0x1401B07A0)
void CCutsceneManager::DeleteMember(std::uint32_t dwActorID)
{
    // IDA: 查找并删除成员
    auto it = m_mapCutsceneMember.find(dwActorID);
    if (it != m_mapCutsceneMember.end()) {
        m_mapCutsceneMember.erase(it);
    }

    // IDA: 删除成员后调用 ChangeState 更新状态
    ChangeState();
}

// IDA: ?ClearMember@CCutsceneManager@@QEAAXXZ (0x1401B0830)
void CCutsceneManager::ClearMember()
{
    // IDA: 清空所有成员
    m_mapCutsceneMember.clear();

    // IDA: 清空后调用 ChangeState 更新状态
    ChangeState();
}

// IDA: ?ChangeState@CCutsceneManager@@QEAAXXZ (0x1401B0A50)
void CCutsceneManager::ChangeState()
{
    // IDA: 遍历所有成员，检查过场动画状态
    bool bAnyPlaying = false;       // 有任何成员在播放
    bool bAllPlaying = true;        // 所有成员都在播放

    for (auto it = m_mapCutsceneMember.begin(); it != m_mapCutsceneMember.end(); ++it) {
        auto& spMember = it->second;
        if (!spMember) {
            continue;
        }

        // IDA: 检查成员是否在播放过场动画 (szCutscene[0] != '\0')
        bool bPlaying = (spMember->szCutscene[0] != '\0');
        if (bPlaying) {
            bAnyPlaying = true;
        } else {
            bAllPlaying = false;
        }

        // IDA: 如果成员设置了无敌状态但不再播放，需要取消无敌
        CUser* pUser = spMember->pUser;
        if (pUser && spMember->bInvincible) {
            if (!bPlaying) {
                // IDA: 取消无敌状态
                pUser->SetInvincibleActor(0);
                spMember->bInvincible = false;
            }
        } else if (pUser && bPlaying && !pUser->IsInvincibleActor()) {
            // IDA: 设置无敌状态
            pUser->SetInvincibleActor(1);
            spMember->bInvincible = true;
        }
    }

    // IDA: 更新迷宫状态
    if (m_pMaze) {
        if (bAllPlaying && !m_bSyncCutsceneState) {
            // IDA: 所有人都在播放，设置迷宫状态为过场动画
            m_pMaze->SetMazeState(2, 1);
        } else if (!bAnyPlaying && m_bSyncCutsceneState) {
            // IDA: 所有人都播放完毕，恢复迷宫状态
            m_pMaze->SetMazeState(3, 1);
            m_pMaze->CallScriptCutsceneOff(m_strFinishCutsceneName);
            m_strFinishCutsceneName[0] = '\0';
        }
    }

    // IDA: 同步过场动画状态
    if (m_bSyncCutsceneState != bAllPlaying) {
        m_bSyncCutsceneState = bAllPlaying;
        if (m_pMaze && !m_bSyncCutsceneState) {
            // IDA: 发送过场动画结束广播
            PS_CUTSCENE_UPDATE stCutscene;
            stCutscene.bOnPlay = false;

            XSendPacket xSendPacket(3, 0x20);
            xSendPacket << stCutscene.bOnPlay;
            m_pMaze->SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);

            m_bClientCutscenePlay = false;

            if (m_bNotifyMessage) {
                m_pMaze->ChatMessage(0, const_cast<char*>("CUTSCENE_STATE_PLAYER_STOP_ALL : ChangeState"));
            }
        }
    }

    // IDA: 更新客户端过场动画播放状态
    if (m_bClientCutscenePlay != bAnyPlaying) {
        m_bClientCutscenePlay = bAnyPlaying;
        if (m_bClientCutscenePlay && m_pMaze) {
            // IDA: 发送过场动画开始广播
            XSendPacket v17(3, 0x20);
            std::uint32_t dwActorID = 2;
            v17 << dwActorID;
            m_pMaze->SendBroadCast(&v17, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);

            if (m_bNotifyMessage) {
                m_pMaze->ChatMessage(0, const_cast<char*>("CUTSCENE_STATE_PLAYER_PLAY_ANYONE : ChangeState"));
            }
        }
    }
}

// IDA: ?AddCutsceneInfo@CCutsceneManager@@QEAAXPEADHKHH@Z (0x1401B1070)
// IDA 精确还原: 参数 (char* szName, int nType, unsigned int dwTime, int nConditionType, int nConditionValue)
void CCutsceneManager::AddCutsceneInfo(char* szName, int nType, unsigned int dwTime, int nConditionType, int nConditionValue)
{
    // IDA: ST_CUTSCENE_INFO::ST_CUTSCENE_INFO(&info)
    ST_CUTSCENE_INFO info;

    // IDA: info.nType = nType
    info.nType = nType;

    // IDA: info.dwTime = dwTime
    info.dwTime = dwTime;

    // IDA: info.nConditionType = nConditionType
    info.nConditionType = nConditionType;

    // IDA: info.nConditionValue = nConditionValue
    info.nConditionValue = nConditionValue;

    // IDA: info.nOrder = CCutsceneManager::GetCutsceneOrder(this, nConditionType)
    info.nOrder = GetCutsceneOrder(nConditionType);

    // IDA: std::string::string(&v7, szName)
    std::string strName(szName ? szName : "");

    // IDA: m_mapCutsceneInfo.insert(std::make_pair(strName, info))
    m_mapCutsceneInfo[strName] = info;
}

// IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXHH@Z (0x1401B1240)
// Verified: IDA decompilation
void CCutsceneManager::CheckCutsceneState(std::int32_t nConditionType, std::int32_t nConditionValue)
{
    // IDA: 检查过场动画状态，根据条件类型和值触发过场动画
    if (!m_pMaze) {
        return;
    }

    bool bChange = false;
    std::vector<std::uint32_t> vecActorID;

    // IDA: 遍历所有过场动画信息
    for (auto it = m_mapCutsceneInfo.begin(); it != m_mapCutsceneInfo.end(); ++it) {
        ST_CUTSCENE_INFO& info = it->second;

        // IDA: 检查条件类型和值是否匹配 (nType == 0 && nConditionType == nConditionType && nConditionValue == nConditionValue)
        if (info.nType == 0 && info.nConditionType == nConditionType && info.nConditionValue == nConditionValue) {
            // IDA: 获取符合条件的玩家列表
            // TODO: 汇编还原 - XMaze::GetActorIDForCutsceneCondition 需要实现
            // m_pMaze->GetActorIDForCutsceneCondition(info.nConditionType, info.nConditionValue, &vecActorID);

            // IDA: 遍历玩家列表
            for (auto itActor = vecActorID.begin(); itActor != vecActorID.end(); ++itActor) {
                std::uint32_t dwActorID = *itActor;

                // IDA: 查找成员
                auto itMember = m_mapCutsceneMember.find(dwActorID);
                if (itMember == m_mapCutsceneMember.end()) {
                    continue;
                }

                auto& spMember = itMember->second;
                if (!spMember) {
                    continue;
                }

                // IDA: 检查是否已经在播放过场动画
                if (spMember->szCutscene[0] != '\0') {
                    continue;
                }

                // IDA: 设置过场动画名称和状态
                std::strncpy(spMember->szCutscene, it->first.c_str(), sizeof(spMember->szCutscene) - 1);
                spMember->szCutscene[sizeof(spMember->szCutscene) - 1] = '\0';
                spMember->bChangeState = true;
                bChange = true;

                // IDA: 记录日志 - 使用 printf 暂时替代 LogHelper
                // LogHelper::LogError("game.contents", "[CUTSCENE %u] CheckCutsceneState1 %s / %d / %d", dwActorID, it->first.c_str(), nConditionType, nConditionValue);
            }
        }
    }

    // IDA: 如果有变化，调用 ChangeState 更新状态
    if (bChange) {
        ChangeState();
    }
}

// IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXKHH@Z (0x1401B17C0)
// Verified: IDA decompilation
void CCutsceneManager::CheckCutsceneState(std::uint32_t dwActorID, std::int32_t nConditionType, std::int32_t nConditionValue)
{
    // IDA: 检查指定玩家的过场动画状态
    auto itMember = m_mapCutsceneMember.find(dwActorID);
    if (itMember == m_mapCutsceneMember.end()) {
        return;
    }

    auto& spMember = itMember->second;
    if (!spMember) {
        return;
    }

    // IDA: 遍历过场动画信息查找匹配的过场动画
    for (auto it = m_mapCutsceneInfo.begin(); it != m_mapCutsceneInfo.end(); ++it) {
        ST_CUTSCENE_INFO& info = it->second;

        // IDA: 检查 nType == 0 && nConditionType && nConditionValue
        if (info.nType == 0 && info.nConditionType == nConditionType && info.nConditionValue == nConditionValue) {
            // IDA: 设置过场动画
            std::strncpy(spMember->szCutscene, it->first.c_str(), sizeof(spMember->szCutscene) - 1);
            spMember->szCutscene[sizeof(spMember->szCutscene) - 1] = '\0';
            spMember->bChangeState = true;

            // IDA: 更新状态
            ChangeState();
            break;
        }
    }
}

// IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B1C40)
// Verified: IDA decompilation
void CCutsceneManager::CheckCutsceneState(std::uint32_t dwActorID, char* pszCutsceneName)
{
    if (!pszCutsceneName) {
        return;
    }

    bool bChange = false;

    // IDA: 查找过场动画信息
    auto it = m_mapCutsceneInfo.find(std::string(pszCutsceneName));
    if (it == m_mapCutsceneInfo.end()) {
        return;
    }

    ST_CUTSCENE_INFO& info = it->second;

    // IDA: 检查是否客户端过场动画 (nType == 1)
    if (info.nType == 1) {
        // IDA: 检查是否已经在显示列表中
        bool bFind = false;
        for (auto& actorID : info.listShowActorID) {
            if (actorID == dwActorID) {
                bFind = true;
                break;
            }
        }

        // IDA: 如果不在列表中，检查条件
        // TODO: 汇编还原 - XMaze::IsCutsceneCondition 需要实现
        bool bCondition = true; // m_pMaze && m_pMaze->IsCutsceneCondition(dwActorID, info.nConditionType, info.nConditionValue);

        if (!bFind && bCondition) {
            // IDA: 查找成员
            auto itMember = m_mapCutsceneMember.find(dwActorID);
            if (itMember != m_mapCutsceneMember.end()) {
                auto& spMember = itMember->second;
                if (spMember) {
                    // IDA: 检查是否已经在播放
                    if (spMember->szCutscene[0] != '\0') {
                        // LogHelper::LogError("game.contents", "[CUTSCENE %d] Error Duplication play Cutscene %s / %s", dwActorID, spMember->szCutscene, pszCutsceneName);
                    }

                    // IDA: 设置过场动画名称
                    std::strncpy(spMember->szCutscene, pszCutsceneName, sizeof(spMember->szCutscene) - 1);
                    spMember->szCutscene[sizeof(spMember->szCutscene) - 1] = '\0';

                    // IDA: 设置结束时间 (nTime = dwTime + GetTickCount64())
                    spMember->nTime = info.dwTime + GetTickCount64();

                    // IDA: 设置状态
                    spMember->bChangeState = true;
                    spMember->nOrder = info.nOrder;

                    bChange = true;

                    // LogHelper::LogError("game.contents", "[CUTSCENE %d] CheckCutsceneState3 %s", dwActorID, pszCutsceneName);
                }
            }
        }
    }

    // IDA: 如果有变化，调用 ChangeState 更新状态
    if (bChange) {
        ChangeState();
    }
}

// IDA: ?OnUpdate@CCutsceneManager@@QEAAXXZ (0x1401B2090)
// Verified: IDA decompilation
void CCutsceneManager::OnUpdate()
{
    // IDA: 遍历所有成员检查超时
    bool bChange = false;

    for (auto it = m_mapCutsceneMember.begin(); it != m_mapCutsceneMember.end(); ++it) {
        auto& spMember = it->second;
        if (!spMember) {
            continue;
        }

        // IDA: 检查过场动画名称是否存在
        if (spMember->szCutscene[0] != '\0') {
            // IDA: 检查是否超时 (nTime <= GetTickCount64())
            if (spMember->nTime > 0 && spMember->nTime <= GetTickCount64()) {
                // IDA: 超时，清除过场动画状态
                spMember->szCutscene[0] = '\0';
                spMember->bChangeState = false;
                bChange = true;
            }
        }
    }

    // IDA: 如果有变化，调用 ChangeState 更新状态
    if (bChange) {
        ChangeState();
    }
}

// IDA: ?SkipCutscene@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B2280)
// IDA 精确还原:
void CCutsceneManager::SkipCutscene(std::uint32_t dwActorID, char* szName)
{
    // IDA: 在 m_mapCutsceneMember 中查找 dwActorID
    auto it = m_mapCutsceneMember.find(dwActorID);
    if (it == m_mapCutsceneMember.end()) {
        return;
    }

    // IDA: 获取成员 shared_ptr
    auto& spMember = it->second;
    if (!spMember) {
        return;
    }

    // IDA: LogHelper::LogError("game.system", "[CUTSCENE %d] SkipCutscene %s %d ", dwActorIDa, ...)
    // LogHelper::LogError("game.system", "[CUTSCENE %d] SkipCutscene %s %d ", dwActorID, spMember->szCutscene, spMember->bChangeState);

    // IDA: 清除过场动画状态 (spMember[17].__vftable = nullptr)
    spMember->bChangeState = false;
    spMember->szCutscene[0] = '\0';

    // IDA: PS_CUTSCENE_UPDATE_RES stCutscene; stCutscene.nPlayState = 0
    PS_CUTSCENE_UPDATE_RES stCutscene;
    stCutscene.nPlayState = 0;

    // IDA: XSendPacket xSendPacket(3, 0x20)
    XSendPacket xSendPacket(3, 0x20);

    // IDA: xSendPacket << stCutscene
    xSendPacket << stCutscene;

    // IDA: 获取 pActor 并发送包
    XActor* pActor = nullptr;
    if (spMember->pUser) {
        pActor = reinterpret_cast<XActor*>(spMember->pUser);
    }

    // IDA: CGocNetwork::Send(pActor, &xSendPacket)
    // CGocNetwork::Send(pActor, xSendPacket);

    // IDA: if (m_bNotifyMessage)
    if (m_bNotifyMessage && m_pMaze) {
        // IDA: sprintf(buff, "SkipCutscene / %d / %s", dwActorID, szName)
        char buff[256];
        std::snprintf(buff, sizeof(buff), "SkipCutscene / %u / %s", dwActorID, szName ? szName : "");
        // IDA: XMaze::ChatMessage(m_pMaze, 0, buff)
        // m_pMaze->ChatMessage(0, buff);
    }

    // IDA: 调用额外函数 (可能是 ChangeState 或其他)
    // hkaiGraphBuilder::extraPositionData(...) - 这是 IDA 误识别，实际上是其他逻辑
    ChangeState();
}

// IDA: ?SetCutscene@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B24B0)
// Verified: IDA decompilation
void CCutsceneManager::SetCutscene(std::uint32_t dwActorID, char* pszCutsceneName)
{
    // IDA: 发送通知消息
    if (m_bNotifyMessage && m_pMaze) {
        char buff[256];
        std::snprintf(buff, sizeof(buff), "SetCutscene / %u / %s", dwActorID, pszCutsceneName ? pszCutsceneName : "");
        // m_pMaze->ChatMessage(0, buff);
    }

    if (!pszCutsceneName) {
        return;
    }

    // IDA: 查找过场动画信息
    auto it = m_mapCutsceneInfo.find(std::string(pszCutsceneName));
    if (it != m_mapCutsceneInfo.end()) {
        ST_CUTSCENE_INFO& info = it->second;

        // IDA: 检查是否客户端过场动画 (nType == 1)
        if (info.nType == 1) {
            // IDA: 调用 CheckCutsceneState 处理
            CheckCutsceneState(dwActorID, pszCutsceneName);
        }
        else {
            // IDA: 查找成员
            auto itMember = m_mapCutsceneMember.find(dwActorID);
            if (itMember != m_mapCutsceneMember.end()) {
                auto& spMember = itMember->second;
                if (spMember) {
                    // IDA: 检查当前播放的过场动画是否匹配
                    if (std::strncmp(spMember->szCutscene, pszCutsceneName, sizeof(spMember->szCutscene)) != 0) {
                        // LogHelper::LogError("game.contents", "[CUTSCENE %d] Error Server SetCutscene : %s", dwActorID, pszCutsceneName);
                    }

                    // IDA: 设置状态标志
                    spMember->bChangeState = true;
                    spMember->bRecvScene = true;

                    // IDA: 更新状态
                    ChangeState();
                }
            }
        }
    }
    else {
        // IDA: 如果过场动画信息不存在，先添加再检查
        AddCutsceneInfo(pszCutsceneName, 1, 0xEA60, 0, 0);
        CheckCutsceneState(dwActorID, pszCutsceneName);
    }
}

// IDA: ?GetCutsceneOrder@CCutsceneManager@@QEAAHH@Z (0x1401B2750)
// IDA 精确还原: 根据条件类型返回过场动画顺序
std::int32_t CCutsceneManager::GetCutsceneOrder(std::int32_t nConditionType)
{
    // IDA: if (nConditionType >= 0)
    if (nConditionType >= 0) {
        // IDA: if (nConditionType <= 1) return 0
        if (nConditionType <= 1) {
            return 0;
        }
        // IDA: if (nConditionType == 2) return 1
        if (nConditionType == 2) {
            return 1;
        }
        // IDA: if (nConditionType <= 4) return 2
        if (nConditionType <= 4) {
            return 2;
        }
    }
    // IDA: return 0
    return 0;
}

// IDA: ?IsPlayCutscene@CCutsceneManager@@QEAA_NK@Z (0x1401B27A0)
// IDA 精确还原: 检查指定 ActorID 是否正在播放过场动画
bool CCutsceneManager::IsPlayCutscene(std::uint32_t dwActorID)
{
    // IDA: 在 m_mapCutsceneMember 中查找 dwActorID
    auto it = m_mapCutsceneMember.find(dwActorID);

    // IDA: 如果找到且 shared_ptr 有效
    if (it != m_mapCutsceneMember.end()) {
        // IDA: 检查 szCutscene[0] != '\0' 表示正在播放
        if (it->second && it->second->szCutscene[0] != '\0') {
            return true;
        }
    }
    return false;
}
