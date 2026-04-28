// CServer.cpp
// CServer 类实现 (对齐 IDA)

#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XControlServer/MazeInfo.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdio>

// 对齐 IDA 0x140042340: SyncMaze - 同步迷宫信息
void CServer::SyncMaze(PS_MAZE_UPDATE_INFO_SYNC* pMazeInfo) {
    if (!pMazeInfo) {
        return;
    }

    // 对齐 IDA: 检查是否为最后一个同步包
    if (pMazeInfo->bLast) {
        // 对齐 IDA: 递增同步计数并检查是否完成
        ++m_nLogicThreadSyncCount;
        if (m_nLogicThreadSyncCount == m_dwLogicThreadCount) {
            SetSyncLoad(E_SERVER_SYNC_LOAD_MAZE_INFO);
        }
        LogHelper::LogInfo("game.relay",
            "<SYNC> SyncMaze Finish [ID:%d] [%d / %d]",
            m_stServerInfo.dwID,
            m_nLogicThreadSyncCount,
            m_dwLogicThreadCount);
        return;
    }

    // 对齐 IDA: 查找或创建迷宫信息
    UXMapID uxMapID = pMazeInfo->psMazeInfo.uxMapID;
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it != m_mapMazeInfo.end()) {
        // 更新现有迷宫信息
        auto& pMaze = it->second;
        if (pMaze) {
            pMaze->SyncMazeInfo(pMazeInfo);
        }
    } else {
        // 对齐 IDA: 创建新迷宫信息
        auto pControlServer = XControlServer::Instance();
        auto pMaze = std::tr1::make_shared<CMazeInfo>();
        if (pMaze) {
            pMaze->SyncMazeInfo(pMazeInfo);
            m_mapMazeInfo[uxMapID] = pMaze;
        }
    }
}

// 对齐 IDA 0x140041930: RemoveMaze - 移除迷宫
void CServer::RemoveMaze(UXMapID uxMapID, bool bResult) {
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it == m_mapMazeInfo.end()) {
        return;
    }

    auto& pMazeInfo = it->second;
    if (pMazeInfo) {
        if (bResult) {
            // 对齐 IDA: 重置父迷宫和子迷宫引用
            pMazeInfo->ResetParentMaze(this);
            pMazeInfo->ResetChildMaze();

            // 对齐 IDA: 处理队伍迷宫ID
            int nPartyID = pMazeInfo->GetPartyID();
            if (nPartyID > 0) {
                auto pControlServer = XControlServer::Instance();
                UXMapID uxEmptyMapID{};  // 设置为 0
                if (pControlServer->GetPartyManager().SetMazeID(nPartyID, uxEmptyMapID, uxMapID)) {
                    // 发送队伍移除包 (0xF2, 0x45)
                    XSendPacket xSendPacket(0xF2, 0x45);
                    xSendPacket.XParse << nPartyID;
                    xSendPacket.XParse << 0;  // bResult = false
                    xSendPacket.XParse << uxMapID.nMapID;
                    pControlServer->SendCommunity(xSendPacket);
                }
            }

            // 对齐 IDA: 处理Force迷宫ID
            int nForceID = pMazeInfo->GetForceID();
            if (nForceID > 0) {
                auto pControlServer = XControlServer::Instance();
                UXMapID uxEmptyMapID{};  // 设置为 0
                if (pControlServer->GetForceManager().SetMazeID(nForceID, uxEmptyMapID, uxMapID)) {
                    // 发送Force移除包 (0xF2, 0x46)
                    XSendPacket xSendPacket(0xF2, 0x46);
                    xSendPacket.XParse << nForceID;
                    xSendPacket.XParse << 0;  // bResult = false
                    xSendPacket.XParse << uxMapID.nMapID;
                    pControlServer->SendCommunity(xSendPacket);
                }
            }
        } else {
            // 对齐 IDA: 如果 bResult 为 false，设置状态为正常
            pMazeInfo->SetMazeState(1, 0);
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveMaze complete");
}

// 对齐 IDA 0x1400421E0: UpdateMaze - 更新迷宫信息
void CServer::UpdateMaze(PS_MAZE_UPDATE_INFO* pMazeInfo) {
    if (!pMazeInfo) {
        return;
    }

    UXMapID uxMapID = pMazeInfo->uxMapID;
    auto it = m_mapMazeInfo.find(uxMapID);

    if (it != m_mapMazeInfo.end()) {
        // 更新现有迷宫
        auto& pMaze = it->second;
        if (pMaze) {
            pMaze->UpdateMazeInfo(*pMazeInfo);
        }
    } else {
        // 对齐 IDA: 使用 ClassFactory 创建新迷宫
        auto pControlServer = XControlServer::Instance();
        auto pMaze = std::tr1::make_shared<CMazeInfo>();
        if (pMaze) {
            pMaze->UpdateMazeInfo(*pMazeInfo);
            m_mapMazeInfo[uxMapID] = pMaze;
        }
    }
}

// 对齐 IDA 0x140042550: OnUpdate - 服务器更新循环
void CServer::OnUpdate(ULONGLONG dwTick) {
    // 对齐 IDA: 遍历迷宫映射
    auto it = m_mapMazeInfo.begin();
    while (it != m_mapMazeInfo.end()) {
        auto& pMazeInfo = it->second;
        if (pMazeInfo && pMazeInfo->IsDestroy()) {
            // 对齐 IDA: 发送移除迷宫包 (0xF2, 0x25)
            XSendPacket xSendPacket(0xF2, 0x25);
            ST_MAP_INFO stMazeInfo = pMazeInfo->GetMazeInfo();
            xSendPacket.XParse << stMazeInfo.uxMapID.nMapID;
            xSendPacket.XParse << pMazeInfo->GetMazeType();
            SendEx(xSendPacket);

            // 对齐 IDA: 调用 RemoveMaze
            RemoveMaze(stMazeInfo.uxMapID, true);

            // 对齐 IDA: 从映射中移除并继续
            it = m_mapMazeInfo.erase(it);
        } else {
            ++it;
        }
    }
}

// 对齐 IDA 0x140041180: RegisterProcess - 注册包处理类
bool CServer::RegisterProcess() {
    // 对齐 IDA: 创建 CServerProcess 并注册到 0xF2
    CServerProcess* pServerProcess = new CServerProcess();
    if (!Register(0xF2, pServerProcess)) {
        return false;
    }

    // 对齐 IDA: 创建 CUserProcess 并注册到 0xF3
    CUserProcess* pUserProcess = new CUserProcess();
    if (!Register(0xF3, pUserProcess)) {
        return false;
    }

    // 对齐 IDA: 创建 CGMToolProcess 并注册到 0xF8
    CGMToolProcess* pGMToolProcess = new CGMToolProcess();
    if (!Register(0xF8, pGMToolProcess)) {
        return false;
    }

    // 对齐 IDA: 创建 CWorldModeProcess 并注册到 0xFB
    CWorldModeProcess* pWorldModeProcess = new CWorldModeProcess();
    if (!Register(0xFB, pWorldModeProcess)) {
        return false;
    }

    return true;
}
