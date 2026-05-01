// CForceManager.cpp
// CForceManager Force/军团管理器实现
// 对齐 IDA ControlServer.exe

#include "CForceManager.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// 对齐 IDA: 构造函数 - m_bLoad 初始化为 true
CForceManager::CForceManager()
    : m_bLoad(true)
{
}

CForceManager::~CForceManager()
{
    Clear();
}

// 对齐 IDA 0x140030CF0: SetMember 设置成员位置
void CForceManager::SetMember(int nForceID, int nActorID, const UXMapID& uxMapID)
{
    auto it = m_mapForce.find(nForceID);
    if (it != m_mapForce.end()) {
        // Force 已存在，直接设置成员
        std::tr1::shared_ptr<CForce> pForce = it->second;
        if (pForce) {
            pForce->SetMember(nActorID, uxMapID);
        }
    } else {
        // Force 不存在，创建新 Force
        std::tr1::shared_ptr<CForce> pForce(new CForce(nForceID));
        if (pForce) {
            pForce->SetMember(nActorID, uxMapID);
            m_mapForce[nForceID] = pForce;
        }
    }
    GreenDamTan_log(__FILE__, __FUNCTION__, "ForceManager::SetMember force=%d actor=%d", nForceID, nActorID);
}

// 对齐 IDA 0x140030F60: SetMazeID 设置迷宫ID
void CForceManager::SetMazeID(int nForceID, const UXMapID& uxMapID)
{
    auto it = m_mapForce.find(nForceID);
    if (it == m_mapForce.end()) {
        return;
    }

    std::tr1::shared_ptr<CForce> pForce = it->second;
    if (!pForce) {
        return;
    }

    pForce->SetMazeID(uxMapID);
}

// 对齐 IDA 0x140039B30: RemoveMember 移除成员
void CForceManager::RemoveMember(int nForceID, int nActorID)
{
    auto it = m_mapForce.find(nForceID);
    if (it == m_mapForce.end()) {
        return;
    }

    std::tr1::shared_ptr<CForce> pForce = it->second;
    if (!pForce) {
        return;
    }

    pForce->RemoveMember(nActorID);

    // 如果成员为空，删除整个 Force
    if (pForce->IsEmpty()) {
        m_mapForce.erase(it);
    }
}

// 对齐 IDA 0x140030EE0: IsFull 检查是否已满
// IDA 显示调用 CParty::IsFull，实际是检查 Force 成员数 >= 4
bool CForceManager::IsFull(int nForceID)
{
    auto it = m_mapForce.find(nForceID);
    if (it == m_mapForce.end()) {
        return false;
    }

    std::tr1::shared_ptr<CForce> pForce = it->second;
    if (!pForce) {
        return false;
    }

    // 对齐 IDA: 调用 CParty::IsFull 方法检查成员数 >= 4
    return pForce->IsFull();
}

// 对齐 IDA 0x140039CD0: GetMazeID 获取迷宫ID
// 参数: nForceID, nActorID (IDA中存在但实际未使用), pMapID (输出)
bool CForceManager::GetMazeID(int nForceID, int nActorID, UXMapID* pMapID)
{
    if (!pMapID) {
        return false;
    }

    auto it = m_mapForce.find(nForceID);
    if (it == m_mapForce.end()) {
        return false;
    }

    std::tr1::shared_ptr<CForce> pForce = it->second;
    if (!pForce) {
        return false;
    }

    pForce->GetMazeID(pMapID);
    return true;
}
