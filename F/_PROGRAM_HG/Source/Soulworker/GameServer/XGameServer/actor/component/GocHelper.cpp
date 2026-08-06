#include "GocHelper.h"
#include "GocNetwork.h"
#include "GocAttribute.h"
#include "GocNpcAttribute.h"
#include "GocParty.h"
#include "../../User.h"
#include "../../Monster.h"
#include "../../Ai.h"
#include "../../GameServer.h"
#include "../../Maze.h"
#include "../Mover/Mover.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <ctime>

// ============================================================================
// CGocHelper - Game Object Component for helper/assistant system
// ============================================================================

// IDA: ??0CGocHelper@@QEAA@XZ (0x140091E90)
// 对齐 IDA: 构造函数 - 初始化成员并调用 Clear
CGocHelper::CGocHelper()
    : GOComponent()
    , m_tLastSummon(0)
    , m_mapSummonedHelper()
    , m_mapHelper()
    , m_stMySupport()
    , m_bMySupportResister(0)
    , m_byMySupportReward(0)
    , m_byAutoSummonFlag(1)
{
    Clear();
}

// IDA: ??1CGocHelper@@UEAA@XZ (0x140091F80)
// 对齐 IDA: 析构函数 - 清理并释放资源
CGocHelper::~CGocHelper() {
    Clear();
    m_mapHelper.clear();
    m_mapSummonedHelper.clear();
}

// GOComponent interface
bool CGocHelper::Initialize() {
    return true;
}

void CGocHelper::Shutdown() {
    Clear();
}

// IDA: No explicit Update function found - appears to be empty/minimal
void CGocHelper::Update(float fDeltaTime) {
    (void)fDeltaTime;
    // Empty - no update logic needed for helper component
}

// IDA: ?Init@CGocHelper@@QEAAXXZ (0x140091FE0)
void CGocHelper::Init() {
    Clear();
}

// IDA: ?Clear@CGocHelper@@QEAAXXZ (0x140092000)
// 对齐 IDA: 清空所有成员变量
void CGocHelper::Clear() {
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
    m_mapHelper.clear();
    m_bMySupportResister = 0;
    m_byMySupportReward = 0;
    m_byAutoSummonFlag = 1;
    memset(&m_stMySupport, 0, sizeof(m_stMySupport));
}

// IDA: ?Reset@CGocHelper@@QEAAXXZ (0x140092080)
// 对齐 IDA: 重置召唤相关数据
void CGocHelper::Reset() {
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
}

// IDA: ?GetFamilyID@CGocHelper@@SAHXZ (0x140039020)
// 对齐 IDA: 返回组件家族ID
int CGocHelper::GetFamilyID() {
    return 16;
}

// IDA: ?SetHelperList@CGocHelper@@QEAA_NAEAUPS_HELPER_LIST_RES@@@Z (0x1400920C0)
// 对齐 IDA: 设置助手列表并自动召唤
bool CGocHelper::SetHelperList(PS_HELPER_LIST_RES& psList) {
    SetAutoSummonFlag(psList.byAutoSummon);

    for (size_t i = 0; i < psList.vecHelper.size(); ++i) {
        ST_HELPER_INFO stAdd = psList.vecHelper[i];
        AddMyHelper(stAdd, false);
    }

    SendHelperList();
    return true;
}

// IDA: ?GetHelperList@CGocHelper@@QEAA_NAEAV?$vector@UST_HELPER_INFO@@V?$allocator@UST_HELPER_INFO@@@std@@@std@@_N@Z (0x1400922B0)
// 对齐 IDA: 获取助手列表，支持按顺序排序
bool CGocHelper::GetHelperList(std::vector<ST_HELPER_INFO>& vecHelper, bool isOrder) {
    vecHelper.clear();

    if (isOrder) {
        // Sort by order
        std::map<std::uint8_t, ST_HELPER_INFO> mapHelperOrder;

        for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
            mapHelperOrder[it->second.byOrder] = it->second;
        }

        for (auto it = mapHelperOrder.begin(); it != mapHelperOrder.end(); ++it) {
            vecHelper.push_back(it->second);
        }
    } else {
        // No ordering, just iterate
        for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
            vecHelper.push_back(it->second);
        }
    }

    return true;
}

// IDA: ?SendHelperList@CGocHelper@@QEAAXXZ (0x140092560)
// 对齐 IDA: 发送助手列表到客户端
void CGocHelper::SendHelperList() {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 构建 PS_HELPER_LIST_RES
    PS_HELPER_LIST_RES psHelper;
    psHelper.dwUCID = pUser->GetUCID();
    psHelper.byAutoSummon = IsAutoSummon() ? 1 : 0;

    // IDA: 获取助手列表
    if (!GetHelperList(psHelper.vecHelper, false)) {
        return;
    }

    // IDA: 发送包 (Main=0x27, Sub=1)
    XSendPacket xSendPacket(0x27, 1);
    xSendPacket << psHelper;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?IsAutoSummon@CGocHelper@@QEAA_NXZ (0x140091DC0)
// 对齐 IDA: 检查自动召唤标志
bool CGocHelper::IsAutoSummon() const {
    return m_byAutoSummonFlag != 0;
}

// IDA: ?SetHelperSummonTime@CGocHelper@@QEAAXXZ (0x140091DF0)
// 对齐 IDA: 设置最后召唤时间为当前时间
void CGocHelper::SetHelperSummonTime() {
    // IDA: ATL::CTime::GetTickCount(&result)
    m_tLastSummon = std::time(nullptr);
}

// IDA: ?SetAutoSummonFlag@CGocHelper@@QEAAXE@Z (0x140091DA0)
// 对齐 IDA: 设置自动召唤标志
void CGocHelper::SetAutoSummonFlag(std::uint8_t byFlag) {
    m_byAutoSummonFlag = byFlag;
}

// IDA: ?GetHelperInfo@CGocHelper@@QEAA_NKAEAUST_HELPER_INFO@@@Z (0x140092700)
// 对齐 IDA: 获取助手信息
bool CGocHelper::GetHelperInfo(std::uint32_t dwHelperID, ST_HELPER_INFO& stInfo) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it != m_mapHelper.end()) {
        stInfo = it->second;
        return true;
    }
    return false;
}

// IDA: ?FindHelper@CGocHelper@@QEAA_NK@Z (0x1400928A0)
// 对齐 IDA: 查找助手是否存在
bool CGocHelper::FindHelper(std::uint32_t dwHelperID) {
    return m_mapHelper.find(dwHelperID) != m_mapHelper.end();
}

// IDA: ?SetHelperSummonState@CGocHelper@@QEAAXK_N@Z (0x140092C20)
// 对齐 IDA: 设置助手召唤状态
void CGocHelper::SetHelperSummonState(std::uint32_t dwHelperID, bool bSummon) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it != m_mapHelper.end()) {
        it->second.bSummon = bSummon;
    }
}

// IDA: ?GetSummonedHelper@CGocHelper@@QEAAPEAVCMonster@@K@Z (0x140092AD0)
// 对齐 IDA: 获取已召唤的助手怪物
CMonster* CGocHelper::GetSummonedHelper(std::uint32_t dwHelperID) {
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it != m_mapSummonedHelper.end()) {
        return it->second;
    }
    return nullptr;
}

// IDA: ?GetSummonedHelperList@CGocHelper@@QEAAXAEAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z (0x140092B40)
// 对齐 IDA: 获取所有已召唤助手列表
// IDA 反编译: 遍历 m_mapSummonedHelper，获取 CMonster 的 ActorID
void CGocHelper::GetSummonedHelperList(std::map<std::uint32_t, std::uint32_t>& mapSummonHelperList) {
    mapSummonHelperList.clear();

    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        std::uint32_t dwHelperID = it->first;
        CMonster* pMonster = it->second;

        if (pMonster) {
            // IDA: 获取 CMonster 的 ActorID
            std::uint32_t dwActorID = pMonster->GetActorID().GetID();
            mapSummonHelperList[dwHelperID] = dwActorID;
        }
    }
}

// IDA: ?CheckSummonHelper@CGocHelper@@QEAA_NK@Z (0x140092D10)
// 对齐 IDA: 检查是否可以召唤指定助手
// 返回 true 如果助手存在且未召唤
bool CGocHelper::CheckSummonHelper(std::uint32_t dwHelperID) {
    // 检查助手是否存在于 m_mapHelper
    if (m_mapHelper.find(dwHelperID) == m_mapHelper.end()) {
        return false;
    }
    // 检查是否已经召唤（如果已召唤则返回 false）
    return m_mapSummonedHelper.find(dwHelperID) == m_mapSummonedHelper.end();
}

// IDA: ?CheckReleaseHelper@CGocHelper@@QEAA_NK@Z (0x140092DC0)
// 对齐 IDA: 检查是否可以释放指定助手
// 返回 true 如果助手存在且已召唤
bool CGocHelper::CheckReleaseHelper(std::uint32_t dwHelperID) {
    // 检查助手是否存在于 m_mapHelper
    if (m_mapHelper.find(dwHelperID) == m_mapHelper.end()) {
        return false;
    }
    // 检查是否已经召唤（如果已召唤则返回 true）
    return m_mapSummonedHelper.find(dwHelperID) != m_mapSummonedHelper.end();
}

// IDA: ?CheckHelperSummonDelay@CGocHelper@@QEAA_NXZ (0x140092CA0)
// 对齐 IDA: 检查召唤延迟时间
bool CGocHelper::CheckHelperSummonDelay() {
    // IDA: 比较当前时间与 m_tLastSummon
    // 默认延迟 10 秒
    std::time_t now = std::time(nullptr);
    return (now - m_tLastSummon) >= 10;
}

// IDA: ?CheckSummonHelperCount@CGocHelper@@QEAA_NXZ (0x140092E70)
// 对齐 IDA: 检查是否可以召唤更多助手
// IDA 反编译: 检查队伍人数和迷宫助手数量
bool CGocHelper::CheckSummonHelperCount() {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return false;
    }

    // IDA: 获取 XArea (BattleZone)
    XArea* pArea = pOwner->GetArea();
    if (!pArea) {
        return false;
    }

    // IDA: 检查是否可以获取 BattleZone
    // 通过虚函数调用检查 BattleZone 是否有效
    // pArea->size() 检查 (通过 m_pBattleZoneList)
    // 简化实现：直接检查 pArea

    // IDA: RTTI 转换为 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (!pMaze) {
        return false;
    }

    // IDA: 检查迷宫类型是否为 12 (特殊类型不允许召唤)
    if (pMaze->GetMazeType() == 12) {
        return false;
    }

    // IDA: 获取 CGocParty 组件
    std::shared_ptr<CGocParty> pParty = pOwner->GetGOC<CGocParty>(false);

    // IDA: 检查 shared_ptr 是否有效
    if (!pParty) {
        return false;
    }

    // IDA: 检查是否在队伍中
    if (pParty->IsParty()) {
        // IDA: 在队伍中，检查队伍人数 + 迷宫助手数量 < 4
        int nPartyUserCount = pParty->GetForceUserCount();
        int nMazeHelperCount = pMaze->GetHelperCount();

        if (nMazeHelperCount + nPartyUserCount >= 4) {
            return false;
        }
    } else {
        // IDA: 不在队伍中，检查迷宫助手数量 + 1 < 4
        int nMazeHelperCount = pMaze->GetHelperCount();

        if (nMazeHelperCount + 1 >= 4) {
            return false;
        }
    }

    return true;
}

// IDA: ?AddMyHelper@CGocHelper@@QEAA_NAEAUST_HELPER_INFO@@_N@Z (0x140092900)
// 对齐 IDA: 添加助手到列表
bool CGocHelper::AddMyHelper(ST_HELPER_INFO& stHelper, bool bSend) {
    // IDA: 获取 XGameServer 实例
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    // IDA: 检查 TB_HELPER 表是否存在该助手
    if (!XResourceMgr::GetTB_HELPER(&pGameServer->m_xResourceMgr, stHelper.dwHelperID)) {
        return false;
    }

    // IDA: 检查助手是否已存在
    if (FindHelper(stHelper.dwHelperID)) {
        return false;
    }

    // IDA: 计算助手属性
    CalcHelperStatsALL(&stHelper, 1);

    // IDA: 插入到 m_mapHelper
    m_mapHelper.insert(std::make_pair(stHelper.dwHelperID, stHelper));

    // IDA: 如果需要发送，发送给客户端 (Main=0x27, Sub=3)
    if (bSend) {
        CMover* pOwner = GetOwnerGO();

        XSendPacket xSendPacket(0x27, 3);
        xSendPacket << stHelper;
        CGocNetwork::Send(pOwner, &xSendPacket);
    }

    return true;
}

// IDA: ?HelperSummon@CGocHelper@@QEAA_NK@Z (0x140093410)
// 对齐 IDA: 召唤助手
bool CGocHelper::HelperSummon(std::uint32_t dwHelperID) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return false;
    }

    // IDA: 检查 owner 是否有效
    if (!pOwner->GetArea()) {
        return false;
    }

    // IDA: 获取 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pOwner->GetArea());
    if (!pMaze) {
        return false;
    }

    // IDA: 检查迷宫类型 (type 12 不允许)
    if (pMaze->GetMazeType() == 12) {
        return false;
    }

    // IDA: 获取助手信息
    ST_HELPER_INFO stHelperInfo;
    if (!GetHelperInfo(dwHelperID, stHelperInfo)) {
        return false;
    }

    // IDA: 检查 TB_HELPER 表
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetTB_HELPER(&pGameServer->m_xResourceMgr, dwHelperID)) {
        return false;
    }

    // IDA: 检查 TB_MONSTER 表
    if (!XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, dwHelperID)) {
        return false;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return false;
    }

    // IDA: 获取玩家位置
    hkvVec3 vCreatePos = pUser->GetPosition();
    hkvVec3 vDestPos = vCreatePos;

    // IDA: 计算召唤位置 (玩家前方 100 单位)
    float fDirYaw = pUser->GetOrientationYaw();
    hkvMat3 matRot;
    Vision::Math.EulerToMatrix(fDirYaw, 0.0f, 0.0f, matRot);

    hkvVec3 vOffset(0.0f, -100.0f, 0.0f);
    hkvVec3 vRotated = matRot * vOffset;
    vDestPos = vCreatePos + vRotated;

    // IDA: 检查高度
    if (!pUser->GetHeight(&vDestPos, 200.0f)) {
        vDestPos = vCreatePos;
    }

    // IDA: 获取玩家 ActorID 和 SectorID
    UXActorID actorID = pUser->GetActorID();
    float fMovingYaw = pUser->GetMovingYaw();
    int nSectorID = pUser->GetSectorID();
    UXMapID instanceID = pMaze->GetInstanceID();

    // IDA: 创建怪物
    CMonster* pSummonMob = pMaze->CreateMonster(
        instanceID,
        nSectorID,
        dwHelperID,
        &vDestPos,
        fMovingYaw,
        eSendInfoTypeSend,
        0,
        0,
        actorID
    );

    if (!pSummonMob) {
        return false;
    }

    // IDA: 获取 CGocNpcAttribute 组件
    std::shared_ptr<CGocNpcAttribute> pAttr = pSummonMob->GetGOC<CGocNpcAttribute>();
    if (!pAttr) {
        return false;
    }

    // IDA: 更新助手状态
    stHelperInfo.bSummon = 1;
    SetHelperSummonState(stHelperInfo.dwHelperID, true);

    // IDA: 设置召唤类型
    pSummonMob->SetSummonType(1);

    // IDA: 设置 OwnerID
    pSummonMob->SetOwnerID(pUser->GetActorID().GetID());

    // IDA: 设置方向
    pSummonMob->SetDirectionYaw(fMovingYaw, 2);

    // IDA: 设置 AI Warp 启用
    CAi* pAi = pSummonMob->GetAi();
    if (pAi) {
        pAi->SetEnalbeHelperWarp(true);
    }

    // IDA: 初始化属性
    TB_MONSTER* pMonTable = pSummonMob->GetMobTableRef();
    int nLevel = pUser->GetLevel();
    int nClass = pUser->GetClass();
    pAttr->Init(nClass, nLevel, pMonTable);

    // IDA: 计算助手属性
    CalcHelperStatsALL(&stHelperInfo, 0);

    // IDA: 应用附加属性
    for (size_t i = 0; i < stHelperInfo.vecAddditional.size(); ++i) {
        pAttr->UpdateStat(
            stHelperInfo.vecAddditional[i].byIndex,
            stHelperInfo.vecAddditional[i].statValue
        );
    }

    // IDA: 计算属性变化
    pAttr->CalculateChangedStat(0);

    // IDA: 设置 HP
    float fHP = pAttr->GetStat(0x0A);
    pSummonMob->SetHpEx(static_cast<int>(fHP));

    // IDA: 完成初始化
    pAttr->Finalize();

    // IDA: 添加到迷宫助手列表
    pMaze->AddHelper(pSummonMob);

    // IDA: 插入到 m_mapSummonedHelper
    m_mapSummonedHelper.insert(std::make_pair(dwHelperID, pSummonMob));

    // IDA: 设置召唤时间
    SetHelperSummonTime();

    // IDA: 记录日志
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 16;
    stLog._sSubType = 2;
    stLog.nParam0 = dwHelperID;
    stLog.nParam1 = pMaze->GetTBMapID();
    stLog.nParam2 = pUser->GetLevel();
    pGameServer->SendDBLog(&stLog);

    // IDA: 构造响应包
    PS_HELPER_SUMMON_RES psSummon;
    psSummon.dwUCID = pUser->GetActorID().GetID();
    psSummon.dwActorID = pSummonMob->GetActorID().GetID();
    psSummon.bSummon = 1;
    psSummon.stHelper = stHelperInfo;

    // IDA: 发送响应包 (Main=0x27, Sub=2)
    XSendPacket xSendPacket(0x27, 2);
    xSendPacket << psSummon;

    // IDA: 检查队伍并发送
    std::shared_ptr<CGocParty> pParty = pUser->GetGOC<CGocParty>();
    if (!pParty || !pParty->IsParty()) {
        CGocNetwork::Send(pOwner, &xSendPacket);
    } else {
        pParty->Send(&xSendPacket);
    }

    return true;
}

// IDA: ?HelperRelease@CGocHelper@@QEAA_NK@Z (0x140094300)
// 对齐 IDA: 释放召唤的助手
bool CGocHelper::HelperRelease(std::uint32_t dwHelperID) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return false;
    }

    // IDA: 检查 owner 是否有效
    if (!pOwner->GetArea()) {
        return false;
    }

    // IDA: 获取 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pOwner->GetArea());
    if (!pMaze) {
        return false;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return false;
    }

    // IDA: 获取已召唤的助手
    CMonster* pMonster = GetSummonedHelper(dwHelperID);
    if (!pMonster) {
        return false;
    }

    // IDA: 禁用 AI Warp
    CAi* pAi = pMonster->GetAi();
    if (pAi) {
        pAi->SetEnalbeHelperWarp(false);
    }

    // IDA: 获取怪物 ActorID
    std::uint32_t dwMonsterActorID = pMonster->GetActorID().GetID();

    // IDA: 获取助手信息
    ST_HELPER_INFO stHelperInfo;
    if (!GetHelperInfo(dwHelperID, stHelperInfo)) {
        return false;
    }

    // IDA: 更新助手状态
    stHelperInfo.bSummon = 0;
    SetHelperSummonState(stHelperInfo.dwHelperID, false);

    // IDA: 释放支援
    HelperSupportRelease(dwHelperID);

    // IDA: 从迷宫删除助手
    pMaze->DeleteHelper(pMonster);

    // IDA: 从 m_mapSummonedHelper 删除
    m_mapSummonedHelper.erase(dwHelperID);

    // IDA: 记录日志
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 16;
    stLog._sSubType = 3;
    stLog.nParam0 = dwHelperID;
    stLog.nParam1 = pMaze->GetTBMapID();
    stLog.nParam2 = pUser->GetLevel();
    XGameServer::Instance()->SendDBLog(&stLog);

    // IDA: 构造响应包
    PS_HELPER_SUMMON_RES psSummon;
    psSummon.dwUCID = pUser->GetActorID().GetID();
    psSummon.dwActorID = dwMonsterActorID;
    psSummon.bSummon = 0;
    psSummon.stHelper = stHelperInfo;

    // IDA: 发送响应包 (Main=0x27, Sub=2)
    XSendPacket xSendPacket(0x27, 2);
    xSendPacket << psSummon;

    // IDA: 检查队伍并发送
    if (pMaze->IsParty()) {
        pMaze->PartySend(&xSendPacket);
    } else {
        CGocNetwork::Send(pOwner, &xSendPacket);
    }

    return true;
}

// IDA: ?HelperWarp@CGocHelper@@QEAA_NK@Z (0x140094130)
// 对齐 IDA: 传送助手到玩家位置
bool CGocHelper::HelperWarp(std::uint32_t dwHelperID) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return false;
    }

    // IDA: 检查 owner 是否有效
    if (!pOwner->GetArea()) {
        return false;
    }

    // IDA: 获取 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pOwner->GetArea());
    if (!pMaze) {
        return false;
    }

    // IDA: 检查迷宫类型 (type 12 不允许)
    if (pMaze->GetMazeType() == 12) {
        return false;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return false;
    }

    // IDA: 获取已召唤的助手
    CMonster* pMonster = GetSummonedHelper(dwHelperID);
    if (!pMonster) {
        return false;
    }

    // IDA: 获取 AI 并传送
    CAi* pAi = pMonster->GetAi();
    if (pAi) {
        hkvVec3 vPos = pUser->GetPosition();
        pAi->WarpHelperToUser(&vPos);
    }

    return true;
}

// IDA: ?HelperProcess@CGocHelper@@QEAAXAEAUPS_HELPER_SUMMON_REQ@@@Z (0x140093080)
// 对齐 IDA: 处理助手召唤/释放请求
void CGocHelper::HelperProcess(struct PS_HELPER_SUMMON_REQ& psInfo) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 检查是否死亡
    if (pOwner->IsDie()) {
        return;
    }

    if (psInfo.bSummon) {
        // IDA: 召唤请求
        if (!CheckHelperSummonDelay()) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE295);
            return;
        }

        // IDA: 检查 owner 是否有效
        if (!pOwner->GetArea()) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE296);
            return;
        }

        // IDA: 获取 XMaze
        XMaze* pMaze = dynamic_cast<XMaze*>(pOwner->GetArea());
        if (!pMaze) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE296);
            return;
        }

        // IDA: 获取地图信息
        std::uint16_t TBMapID = pMaze->GetTBMapID();
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        TB_MAZE_INFO* pTB_MAZE_INFO = XResourceMgr::GetTB_MAZE_INFO(&pGameServer->m_xResourceMgr, TBMapID);

        // IDA: 检查是否允许召唤
        if (!pTB_MAZE_INFO || !pTB_MAZE_INFO->Helper_Summon_Check || pTB_MAZE_INFO->Maze_Type == 12) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE297);
            return;
        }

        // IDA: 检查助手是否可召唤
        if (!CheckSummonHelper(psInfo.dwHelperID)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE297);
            return;
        }

        // IDA: 检查召唤数量
        if (!CheckSummonHelperCount()) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE298);
            return;
        }

        // IDA: 召唤助手
        if (!HelperSummon(psInfo.dwHelperID)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE297);
        }
    } else {
        // IDA: 释放请求
        if (!pOwner->GetArea()) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE296);
            return;
        }

        // IDA: 检查是否可释放
        if (!CheckReleaseHelper(psInfo.dwHelperID)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE297);
            return;
        }

        // IDA: 释放助手
        if (!HelperRelease(psInfo.dwHelperID)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 2, 0xE297);
        }
    }
}

        HelperRelease(psInfo.dwHelperID);
    }
}

// IDA: ?CalcHelperStatsALL@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@H@Z (0x140095280)
// 对齐 IDA: 计算助手所有属性
void CGocHelper::CalcHelperStatsALL(ST_HELPER_INFO* stHelper, int bSend) {
    if (!stHelper) {
        return;
    }

    // IDA: 清空原始属性和附加属性
    stHelper->vecOrigin.clear();
    stHelper->vecAddditional.clear();

    // IDA: 计算各项属性
    CalcOriginStats(*stHelper);
    CalcEquipItemStats(*stHelper);
    CalcFriendItemStats(*stHelper);

    // IDA: 如果需要发送，更新召唤助手的属性
    if (bSend) {
        CMonster* pHelper = GetSummonedHelper(stHelper->dwHelperID);
        if (pHelper) {
            std::shared_ptr<CGocNpcAttribute> pAttr = pHelper->GetGOC<CGocNpcAttribute>();
            if (pAttr) {
                pAttr->CalculateChangedStat(1);
            }
        }
    }
}

// IDA: ?CalcOriginStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x140095360)
// 对齐 IDA: 计算助手原始属性
void CGocHelper::CalcOriginStats(ST_HELPER_INFO& stHelper) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 获取 TB_MONSTER 表
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_MONSTER* pTB_Monster = XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, stHelper.dwHelperID);
    if (!pTB_Monster) {
        return;
    }

    // IDA: 获取玩家职业和等级
    std::uint8_t byClass = pUser->GetClass();
    std::uint8_t byLevel = pUser->GetLevel();

    // IDA: 创建临时 CGocNpcAttribute 计算属性
    CGocNpcAttribute gocTemp;
    gocTemp.Init(byClass, byLevel, pTB_Monster);
    gocTemp.GetFinalStats(stHelper.vecOrigin);
}

// IDA: ?CalcEquipItemStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x140095500)
// 对齐 IDA: 计算装备物品属性
void CGocHelper::CalcEquipItemStats(ST_HELPER_INFO& stHelper) {
    // IDA: 创建临时属性列表
    std::vector<ST_HELPER_STAT_INFO> vecAddditional;

    // IDA: 遍历 3 个装备槽位
    for (int i = 0; i < 3; ++i) {
        // IDA: 获取物品表
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        TB_ITEM* pTB_ITEM = pGameServer->GetResourceMgr().GetTB_ITEM(stHelper.stItem[i].nItemID);

        if (pTB_ITEM) {
            // IDA: 添加物理攻击 (Index 21)
            ST_HELPER_STAT_INFO stInfo;
            stInfo.byIndex = 21;
            stInfo.fStatValue = static_cast<float>(pTB_ITEM->Item_physical_Attack);
            vecAddditional.push_back(stInfo);

            // IDA: 添加物理攻击 (Index 20)
            stInfo.byIndex = 20;
            stInfo.fStatValue = static_cast<float>(pTB_ITEM->Item_physical_Attack);
            vecAddditional.push_back(stInfo);

            // IDA: 添加物理防御 (Index 24)
            stInfo.byIndex = 24;
            stInfo.fStatValue = static_cast<float>(pTB_ITEM->Item_physical_Defense);
            vecAddditional.push_back(stInfo);
        }
    }

    // IDA: 获取召唤助手
    CMonster* pHelper = GetSummonedHelper(stHelper.dwHelperID);
    CGocNpcAttribute* pAttr = nullptr;

    if (pHelper) {
        pAttr = pHelper->GetGOC<CGocNpcAttribute>();
    }

    // IDA: 合并属性到 vecAddditional
    for (size_t k = 0; k < vecAddditional.size(); ++k) {
        // IDA: 如果助手已召唤，更新属性组件
        if (pAttr) {
            pAttr->UpdateAddStat(vecAddditional[k].byIndex, vecAddditional[k].fStatValue);
        }

        // IDA: 查找是否已存在相同索引
        bool bFind = false;
        for (size_t j = 0; j < stHelper.vecAddditional.size(); ++j) {
            if (stHelper.vecAddditional[j].byIndex == vecAddditional[k].byIndex) {
                // IDA: 累加属性值
                stHelper.vecAddditional[j].fStatValue += vecAddditional[k].fStatValue;
                bFind = true;
                break;
            }
        }

        // IDA: 如果不存在，添加新属性
        if (!bFind) {
            stHelper.vecAddditional.push_back(vecAddditional[k]);
        }
    }
}

// IDA: ?CalcFriendItemStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x1400958B0)
// 对齐 IDA: 计算好友支援物品属性
void CGocHelper::CalcFriendItemStats(ST_HELPER_INFO& stHelper) {
    // IDA: 检查是否有好友支援
    if (!stHelper.stFriendSupport.dwFriendUCID) {
        return;
    }

    // IDA: 检查属性值是否有效
    if (stHelper.stFriendSupport.fVal <= 0.0f) {
        return;
    }

    // IDA: 检查过期时间
    std::time_t tCurr = std::time(nullptr);
    if (tCurr >= stHelper.stFriendSupport.nDate) {
        return;
    }

    // IDA: 创建临时属性列表
    std::vector<StatInfo> vecAddditional;

    // IDA: 根据支援类型添加属性
    std::uint8_t bySupportType = stHelper.stFriendSupport.bySupportType;
    if (bySupportType == 0) {
        // IDA: 类型0 - 物理攻击 (Index 20, 21)
        StatInfo stInfo;
        stInfo.byIndex = 20;
        stInfo.statValue = stHelper.stFriendSupport.fVal;
        vecAddditional.push_back(stInfo);

        stInfo.byIndex = 21;
        stInfo.statValue = stHelper.stFriendSupport.fVal;
        vecAddditional.push_back(stInfo);
    } else if (bySupportType == 1) {
        // IDA: 类型1 - 物理防御 (Index 24)
        StatInfo stInfo;
        stInfo.byIndex = 24;
        stInfo.statValue = stHelper.stFriendSupport.fVal;
        vecAddditional.push_back(stInfo);
    } else {
        return;
    }

    // IDA: 获取召唤助手
    CMonster* pHelper = GetSummonedHelper(stHelper.dwHelperID);
    std::shared_ptr<CGocNpcAttribute> pAttr;

    if (pHelper) {
        pAttr = pHelper->GetGOC<CGocNpcAttribute>();
    }

    // IDA: 合并属性到 vecAddditional
    for (size_t k = 0; k < vecAddditional.size(); ++k) {
        // IDA: 如果助手已召唤，更新属性组件
        if (pAttr) {
            pAttr->UpdateStat(vecAddditional[k].byIndex, vecAddditional[k].statValue);
        }

        // IDA: 查找是否已存在相同索引
        bool bFind = false;
        for (size_t i = 0; i < stHelper.vecAddditional.size(); ++i) {
            if (stHelper.vecAddditional[i].byIndex == vecAddditional[k].byIndex) {
                // IDA: 累加属性值
                stHelper.vecAddditional[i].statValue += vecAddditional[k].statValue;
                bFind = true;
                break;
            }
        }

        // IDA: 如果不存在，添加新属性
        if (!bFind) {
            stHelper.vecAddditional.push_back(vecAddditional[k]);
        }
    }
}

// IDA: ?HelperSupportRelease@CGocHelper@@QEAAXXZ (0x1400966D0)
// 对齐 IDA: 释放所有助手支援
void CGocHelper::HelperSupportRelease() {
    // IDA: 遍历所有已召唤助手
    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        HelperSupportRelease(it->first);
    }
}

// IDA: ?HelperSupportRelease@CGocHelper@@QEAAXK@Z (0x140096750)
// 对齐 IDA: 释放指定助手的支援
void CGocHelper::HelperSupportRelease(std::uint32_t dwHelperID) {
    // IDA: 查找助手
    auto it = m_mapHelper.find(dwHelperID);
    if (it == m_mapHelper.end()) {
        return;
    }

    // IDA: 检查是否有好友支援
    ST_HELPER_INFO& stHelper = it->second;
    if (!stHelper.stFriendSupport.dwFriendUCID) {
        return;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 构造数据库请求
    PS_DB_HELPER_SUPPORT_RELEASE psRelease;
    psRelease.dwUCID = pUser->GetUCID();
    psRelease.dwHelperID = dwHelperID;

    // IDA: 发送数据库请求 (Main=0x26, Sub=4)
    XSendDBPacket xSendDBPacket(pUser, 0x26, 4);
    xSendDBPacket << psRelease;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// IDA: ?SyncSummonedInfo@CGocHelper@@QEAAXXZ (0x140096060)
// 对齐 IDA: 同步已召唤助手信息
void CGocHelper::SyncSummonedInfo() {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 获取 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pOwner->GetArea());
    if (!pMaze) {
        return;
    }

    // IDA: 获取助手列表和队伍成员列表
    std::vector<CMonster*> vecHelperList;
    std::vector<CUser*> vecPartyMember;

    pMaze->GetHelperList(&vecHelperList);
    pMaze->GetPartyUserList(&vecPartyMember);

    // IDA: 遍历助手列表
    for (size_t i = 0; i < vecHelperList.size(); ++i) {
        CMonster* pMonster = vecHelperList[i];
        if (!pMonster) {
            continue;
        }

        // IDA: 遍历队伍成员
        for (size_t n = 0; n < vecPartyMember.size(); ++n) {
            CUser* pMember = vecPartyMember[n];
            if (!pMember) {
                continue;
            }

            // IDA: 获取 CGocHelper 组件
            std::shared_ptr<CGocHelper> pHelper = pMember->GetGOC<CGocHelper>();
            if (!pHelper) {
                continue;
            }

            // IDA: 获取助手ID
            std::uint32_t dwHelperID = pMonster->GetTableID();

            // IDA: 获取已召唤的助手
            CMonster* pSummoned = pHelper->GetSummonedHelper(dwHelperID);
            if (!pSummoned) {
                continue;
            }

            // IDA: 比较 ActorID
            UXActorID actorMonster = pMonster->GetActorID();
            UXActorID actorSummoned = pSummoned->GetActorID();
            if (actorSummoned != actorMonster) {
                continue;
            }

            // IDA: 获取助手信息
            ST_HELPER_INFO stHelper;
            if (!pHelper->GetHelperInfo(dwHelperID, stHelper)) {
                continue;
            }

            // IDA: 构造响应包
            PS_HELPER_SUMMON_RES psSummon;
            psSummon.dwUCID = pMonster->GetParentID().GetID();
            psSummon.dwActorID = pMonster->GetActorID().GetID();
            psSummon.bSummon = 1;
            psSummon.stHelper = stHelper;

            // IDA: 发送给客户端 (Main=0x27, Sub=2)
            XSendPacket xSendPacket(0x27, 2);
            xSendPacket << psSummon;
            CGocNetwork::Send(pOwner, &xSendPacket);
        }
    }
}

// IDA: ?CheckAllHelperSummon@CGocHelper@@QEAAXXZ (0x1400948A0)
// 对齐 IDA: 检查并自动召唤所有助手
void CGocHelper::CheckAllHelperSummon() {
    // IDA: 检查自动召唤标志
    if (!IsAutoSummon()) {
        return;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 获取 CGocParty 组件
    std::shared_ptr<CGocParty> pParty = pOwner->GetGOC<CGocParty>();
    if (!pParty) {
        return;
    }

    // IDA: 检查是否在队伍中
    if (pParty->IsParty()) {
        return;
    }

    // IDA: 检查 owner 是否有效
    if (!pOwner->GetArea()) {
        return;
    }

    // IDA: 获取 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pOwner->GetArea());
    if (!pMaze) {
        return;
    }

    // IDA: 获取地图信息
    std::uint16_t TBMapID = pMaze->GetTBMapID();
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_MAZE_INFO* pTB_MAZE_INFO = XResourceMgr::GetTB_MAZE_INFO(&pGameServer->m_xResourceMgr, TBMapID);
    if (!pTB_MAZE_INFO) {
        return;
    }

    // IDA: 检查是否允许召唤
    if (!pTB_MAZE_INFO->Helper_Summon_Check) {
        return;
    }

    // IDA: 检查迷宫类型 (type 12 不允许)
    if (pTB_MAZE_INFO->Maze_Type == 12) {
        return;
    }

    // IDA: 获取助手列表
    std::vector<ST_HELPER_INFO> vecHelper;
    GetHelperList(vecHelper, true);

    // IDA: 遍历并召唤
    for (size_t i = 0; i < vecHelper.size() && CheckSummonHelperCount(); ++i) {
        HelperSummon(vecHelper[i].dwHelperID);
    }
}

// IDA: ?AllHelperRelease@CGocHelper@@QEAAXXZ (0x140094AF0)
// 对齐 IDA: 释放所有已召唤的助手
void CGocHelper::AllHelperRelease() {
    // IDA: 收集所有需要释放的助手ID
    std::vector<std::uint32_t> vecReleaseList;

    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        CMonster* pMonster = it->second;
        if (pMonster) {
            std::uint32_t dwHelperID = pMonster->GetTableID();
            vecReleaseList.push_back(dwHelperID);
        }
    }

    // IDA: 释放所有助手
    for (size_t i = 0; i < vecReleaseList.size(); ++i) {
        HelperRelease(vecReleaseList[i]);
    }
}

// IDA: ?AllHelperWarp@CGocHelper@@QEAAXXZ (0x140094C10)
// 对齐 IDA: 传送所有助手到玩家位置
// IDA 反编译: 先收集所有助手 ID，然后调用 HelperWarp
void CGocHelper::AllHelperWarp() {
    // IDA: 创建助手 ID 列表
    std::vector<std::uint32_t> vecWarpList;

    // IDA: 遍历已召唤助手，收集 ID
    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        CMonster* pMonster = it->second;
        if (pMonster) {
            // IDA: 获取怪物 TableID
            std::uint32_t dwHelperID = pMonster->GetTableID();
            vecWarpList.push_back(dwHelperID);
        }
    }

    // IDA: 遍历列表并传送
    for (size_t i = 0; i < vecWarpList.size(); ++i) {
        HelperWarp(vecWarpList[i]);
    }
}

// IDA: ?OtherHelperClear@CGocHelper@@QEAAXXZ (0x140094D30)
// 对齐 IDA: 清除其他助手 (不是自己的助手)
void CGocHelper::OtherHelperClear() {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 获取 XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pUser->GetArea());
    if (!pMaze) {
        return;
    }

    // IDA: 获取扫描器中的对象
    std::map<std::uint32_t, CUser*>* pVecActor = pMaze->GetScanner(pUser);
    if (!pVecActor) {
        return;
    }

    // IDA: 遍历扫描器中的对象
    for (auto iter = pVecActor->begin(); iter != pVecActor->end(); ++iter) {
        CUser* pMember = dynamic_cast<CUser*>(iter->second);
        if (!pMember) {
            continue;
        }

        // IDA: 获取成员的 CGocHelper 组件
        std::shared_ptr<CGocHelper> pMemberHelper = pMember->GetGOC<CGocHelper>();
        if (!pMemberHelper) {
            continue;
        }

        // IDA: 获取成员的已召唤助手列表
        std::map<std::uint32_t, std::uint32_t> mapSummonHelperList;
        pMemberHelper->GetSummonedHelperList(mapSummonHelperList);

        // IDA: 获取成员 UCID
        std::uint32_t dwUCID = pMember->GetActorID().GetID();

        // IDA: 遍历成员的助手
        for (auto itHelper = mapSummonHelperList.begin(); itHelper != mapSummonHelperList.end(); ++itHelper) {
            ST_HELPER_INFO stHelperInfo;
            if (!pMemberHelper->GetHelperInfo(itHelper->first, stHelperInfo)) {
                continue;
            }

            std::uint32_t dwMonsterActorID = itHelper->second;

            // IDA: 构造响应包
            PS_HELPER_SUMMON_RES psSummon;
            psSummon.dwUCID = dwUCID;
            psSummon.dwActorID = dwMonsterActorID;
            psSummon.bSummon = 0;
            psSummon.stHelper = stHelperInfo;

            // IDA: 发送响应包 (Main=0x27, Sub=2)
            XSendPacket xSendPacket(0x27, 2);
            xSendPacket << psSummon;
            CGocNetwork::Send(pOwner, &xSendPacket);
        }
    }
}

// Support info
// IDA: ?SetMySupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140096660)
// 对齐 IDA: 设置支援信息
void CGocHelper::SetMySupportInfo(const ST_HELPER_SUPPORT_INFO& stInfo) {
    m_stMySupport = stInfo;
}

// IDA: ?SetMySupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140096660)
// 对齐 IDA: 设置支援信息 (从数据库响应)
void CGocHelper::SetMySupportInfo(PS_HELPER_SUPPORT_INFO_RES& stMyInfo) {
    // IDA: qmemcpy(&this->m_stMySupport, v2, sizeof(this->m_stMySupport));
    // IDA: this->m_bMySupportResister = stMyInfo->bRegister;
    // IDA: this->m_byMySupportReward = stMyInfo->byRewardType;
    std::memcpy(&m_stMySupport, &stMyInfo, sizeof(m_stMySupport));
    m_bMySupportResister = stMyInfo.bRegister;
    m_byMySupportReward = stMyInfo.byRewardType;
}

// IDA: ?GetSupportTypeRate@CGocHelper@@QEAAME@Z (0x140096500)
// 对齐 IDA: 获取支援类型比率
float CGocHelper::GetSupportTypeRate(std::uint8_t byType) {
    // IDA: if (!byType) return 0.19999999; // 0.2
    // IDA: if (byType == 1) return 0.19999999; // 0.2
    // IDA: return 0.0;
    if (!byType) {
        return 0.2f;
    }
    if (byType == 1) {
        return 0.2f;
    }
    return 0.0f;
}

// IDA: ?GetSupportTypeValue@CGocHelper@@QEAAME@Z (0x140096540)
// 对齐 IDA: 获取支援类型数值
float CGocHelper::GetSupportTypeValue(std::uint8_t byType) {
    // IDA: fRate = CGocHelper::GetSupportTypeRate(this, byType);
    // IDA: if (fRate <= 0.0) return 0.0;
    float fRate = GetSupportTypeRate(byType);
    if (fRate <= 0.0f) {
        return 0.0f;
    }

    float fValue = 0.0f;

    // IDA: 获取 owner CMover 和 CGocAttribute 组件
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return 0.0f;
    }

    // IDA: 获取 CGocAttribute 组件
    std::shared_ptr<CGocAttribute> pAttr = pOwner->GetGOC<CGocAttribute>();
    if (!pAttr) {
        return 0.0f;
    }

    // IDA: 根据类型获取玩家属性
    // byType == 0: 获取 STAT_INDEX 0x15 (攻击力)
    // byType == 1: 获取 STAT_INDEX 0x18 (防御力)
    if (byType == 0) {
        fValue = pAttr->GetStat(0x15); // STAT_INDEX for physical attack
    } else if (byType == 1) {
        fValue = pAttr->GetStat(0x18); // STAT_INDEX for defense
    }

    if (fValue > 0.0f) {
        return fValue * fRate;
    }
    return 0.0f;
}

// IDA: ?GetMyHelperStatsALL@CGocHelper@@QEAAXAEAUPS_HELPER_STAT_UPDATE@@@Z (0x140095170)
// 对齐 IDA: 获取所有助手属性
void CGocHelper::GetMyHelperStatsALL(PS_HELPER_STAT_UPDATE& psStat) {
    // IDA: 遍历 m_mapHelper，将每个助手的 vecAddditional 添加到 psStat.mapHelperStats
    psStat.mapHelperStats.clear();

    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        std::uint32_t dwHelperID = it->first;
        const ST_HELPER_INFO& stInfo = it->second;

        // IDA: 复制 vecAddditional 到 mapHelperStats
        psStat.mapHelperStats[dwHelperID] = stInfo.vecAddditional;
    }
}

// IDA: ?UnEquipHelperItemStats@CGocHelper@@QEAAXKAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x140095c80)
// 对齐 IDA: 卸下助手物品属性
void CGocHelper::UnEquipHelperItemStats(std::uint32_t dwHelperID, std::vector<float>& vecDel) {
    // IDA: 获取已召唤的助手怪物
    CMonster* pHelper = GetSummonedHelper(dwHelperID);
    if (!pHelper) {
        return;
    }

    // IDA: 获取 CGocNpcAttribute 组件
    std::shared_ptr<CGocNpcAttribute> pAttr = pHelper->GetGOC<CGocNpcAttribute>();
    if (!pAttr) {
        return;
    }

    // IDA: 遍历 vecDel 中的物品ID
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    for (size_t i = 0; i < vecDel.size(); ++i) {
        // IDA: vecDel 实际存储的是物品ID (作为 float)
        std::uint32_t dwItemID = static_cast<std::uint32_t>(vecDel[i]);

        // IDA: 获取 TB_ITEM 表
        TB_ITEM* pTB_ITEM = pGameServer->m_xResourceMgr.GetTB_ITEM(dwItemID);
        if (!pTB_ITEM) {
            continue;
        }

        // IDA: 如果物品有物理攻击属性，移除攻击相关属性
        if (pTB_ITEM->Item_physical_Attack) {
            pAttr->RemoveStat(21); // STAT_INDEX for physical attack
            pAttr->RemoveStat(20); // STAT_INDEX for attack related
        }

        // IDA: 如果物品有物理防御属性，移除防御相关属性
        if (pTB_ITEM->Item_physical_Defense) {
            pAttr->RemoveStat(24); // STAT_INDEX for physical defense
        }
    }
}

// IDA: ?UnEquipHelperFriendItemStats@CGocHelper@@QEAAXK@Z (0x140095e80)
// 对齐 IDA: 卸下助手好友支援物品属性
void CGocHelper::UnEquipHelperFriendItemStats(std::uint32_t dwHelperID) {
    // IDA: 查找助手
    auto it = m_mapHelper.find(dwHelperID);
    if (it == m_mapHelper.end()) {
        return;
    }

    // IDA: 检查是否有好友支援物品 (stFriendSupport.fVal > 0)
    ST_HELPER_INFO& stInfo = it->second;
    if (stInfo.stFriendSupport.fVal <= 0.0f) {
        return;
    }

    // IDA: 获取已召唤的助手怪物
    CMonster* pHelper = GetSummonedHelper(dwHelperID);
    if (!pHelper) {
        return;
    }

    // IDA: 获取 CGocNpcAttribute 组件并更新属性
    std::shared_ptr<CGocNpcAttribute> pAttr = pHelper->GetGOC<CGocNpcAttribute>();
    if (!pAttr) {
        return;
    }

    // IDA: 根据 bySupportType 移除不同属性
    // bySupportType == 1: 移除防御属性 (24)
    // bySupportType == 0: 移除攻击属性 (21, 20)
    if (stInfo.stFriendSupport.bySupportType == 1) {
        pAttr->RemoveStat(24); // STAT_INDEX for defense
    } else if (stInfo.stFriendSupport.bySupportType == 0) {
        pAttr->RemoveStat(21); // STAT_INDEX for physical attack
        pAttr->RemoveStat(20); // STAT_INDEX for attack related
    }
}

// IDA: ?ReqHelperSupportInfo@CGocHelper@@QEAAXXZ (0x1400968f0)
// 对齐 IDA: 请求支援信息
void CGocHelper::ReqHelperSupportInfo() {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 获取 UCID
    std::uint32_t dwUCID = pUser->GetUCID();

    // IDA: 发送请求到 RelayServer (Main=0xF5, Sub=0x27)
    XSendPacket xSendPacket(0xF5, 0x27);
    xSendPacket << dwUCID;

    // IDA: 通过 communitySocket 发送
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 5);
    }
}

// IDA: ?ReqHelperSupportRegister@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_REGISTER_REQ@@@Z (0x140096a10)
// 对齐 IDA: 请求注册支援
void CGocHelper::ReqHelperSupportRegister(PS_HELPER_SUPPORT_REGISTER_REQ& psSupport) {
    // IDA: fSupportValue = CGocHelper::GetSupportTypeValue(this, psSupport->bySupportType);
    float fSupportValue = GetSupportTypeValue(psSupport.bySupportType);
    if (fSupportValue < 0.0f) {
        return;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 计算过期时间 (当前时间 + 24小时)
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    ATL::CTimeSpan tDuration(0, 24, 0, 0);
    ATL::CTime tDate = tCurr + tDuration;

    // IDA: 构造并发送 PS_SERVER_HELPER_SUPPORT_REGISTER
    PS_SERVER_HELPER_SUPPORT_REGISTER psSend;
    psSend.stInfo.dwFriendUCID = pUser->GetUCID();
    psSend.stInfo.bySupportType = psSupport.bySupportType;
    psSend.stInfo.fVal = fSupportValue;
    psSend.stInfo.nDate = tDate.GetTime();

    // IDA: 发送请求到 RelayServer (Main=0xF5, Sub=0x28)
    XSendPacket xSendPacket(0xF5, 0x28);
    xSendPacket << psSend;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 6);
    }
}

// IDA: ?ReqHelperSupportReward@CGocHelper@@QEAAXXZ (0x140096be0)
// 对齐 IDA: 请求支援奖励
void CGocHelper::ReqHelperSupportReward() {
    // IDA: 检查是否可以领取奖励 (m_byMySupportReward == 1)
    if (m_byMySupportReward != 1) {
        return;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 获取奖励配置
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    TB_HELPER_REWARD* pTB_HELPER_REWARD = pGameServer->m_xResourceMgr.GetTB_HELPER_REWARD(1);
    if (!pTB_HELPER_REWARD) {
        return;
    }

    // IDA: 构造奖励请求
    PS_SERVER_HELPER_SUPPORT_REWARD psReward;
    psReward.dwUCID = pUser->GetUCID();

    // IDA: 添加奖励物品
    if (pTB_HELPER_REWARD->Reward_Item_ID_01 && pTB_HELPER_REWARD->Reward_Item_Count_01) {
        ST_CREATE_ITEM stItem;
        stItem.nItemID = pTB_HELPER_REWARD->Reward_Item_ID_01;
        stItem.shCount = pTB_HELPER_REWARD->Reward_Item_Count_01;
        psReward.stCreateItems.push_back(stItem);
    }
    if (pTB_HELPER_REWARD->Reward_Item_ID_02 && pTB_HELPER_REWARD->Reward_Item_Count_02) {
        ST_CREATE_ITEM stItem;
        stItem.nItemID = pTB_HELPER_REWARD->Reward_Item_ID_02;
        stItem.shCount = pTB_HELPER_REWARD->Reward_Item_Count_02;
        psReward.stCreateItems.push_back(stItem);
    }
    if (pTB_HELPER_REWARD->Reward_Item_ID_03 && pTB_HELPER_REWARD->Reward_Item_Count_03) {
        ST_CREATE_ITEM stItem;
        stItem.nItemID = pTB_HELPER_REWARD->Reward_Item_ID_03;
        stItem.shCount = pTB_HELPER_REWARD->Reward_Item_Count_03;
        psReward.stCreateItems.push_back(stItem);
    }

    psReward.wFriendPoint = pTB_HELPER_REWARD->Reward_FriendPoint_Value;

    // IDA: 检查背包空间
    std::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();
    if (!pInven) {
        return;
    }

    int nError = pInven->CheckAddItems(psReward.stCreateItems);
    if (nError) {
        // IDA: 发送错误消息
        if (nError == 5) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 7, 0xCB2A);
        } else {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 7, 0xCB24);
        }
        return;
    }

    // IDA: 发送请求到 RelayServer (Main=0xF5, Sub=0x29)
    XSendPacket xSendPacket(0xF5, 0x29);
    xSendPacket << psReward;
    pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 7);
}

// IDA: ?ReqHelperSupportList@CGocHelper@@QEAAXXZ (0x140096f70)
// 对齐 IDA: 请求支援列表
void CGocHelper::ReqHelperSupportList() {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 获取 UCID
    std::uint32_t dwUCID = pUser->GetUCID();

    // IDA: 发送请求到 RelayServer (Main=0xF5, Sub=0x30)
    XSendPacket xSendPacket(0xF5, 0x30);
    xSendPacket << dwUCID;

    // IDA: 通过 communitySocket 发送
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 8);
    }
}

// IDA: ?GetLastOrderNumber@CGocHelper@@QEAAEXZ (0x14009a780)
// 对齐 IDA: 获取最后一个顺序号
std::uint8_t CGocHelper::GetLastOrderNumber() {
    std::uint8_t byOrder = 0;

    // IDA: 遍历 m_mapHelper，找到最大的 byOrder
    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        const ST_HELPER_INFO& stInfo = it->second;
        if (byOrder < stInfo.byOrder) {
            byOrder = stInfo.byOrder;
        }
    }

    return byOrder;
}

// IDA: ?ReqHelperEquip@CGocHelper@@QEAAXAEAUPS_HELPER_EQUIP_REQ@@@Z (0x140097200)
// 对齐 IDA: 请求装备助手物品
void CGocHelper::ReqHelperEquip(PS_HELPER_EQUIP_REQ& psEquip) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 构造数据库请求
    PS_DB_HELPER_EQUIP_REQ psDB;
    memset(&psDB, 0, sizeof(psDB));
    psDB.dwUCID = pUser->GetUCID();
    psDB.psEquip = psEquip;

    // IDA: 查找助手
    auto it = m_mapHelper.find(psEquip.dwHelperID);
    auto itEnd = m_mapHelper.end();

    if (it == itEnd) {
        CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
        return;
    }

    ST_HELPER_INFO& stHelper = it->second;

    // IDA: 获取 TB_HELPER 表
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_HELPER* pTB_HELPER = XResourceMgr::GetTB_HELPER(&pGameServer->m_xResourceMgr, stHelper.dwHelperID);
    if (!pTB_HELPER) {
        CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
        return;
    }

    // IDA: 获取 CGocInventory 组件
    std::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();
    if (!pInven) {
        CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
        return;
    }

    // IDA: 处理三种情况
    if (psEquip.xInvenSerial && psEquip.xHelperSerial) {
        // IDA: 情况1: 交换背包和助手物品
        std::shared_ptr<CItem> pInvenItem;
        pInven->GetItemPtr(pInvenItem, psEquip.xInvenSerial);

        if (!pInvenItem) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        TB_ITEM_CLASSIFY* pClassifyTable = pInvenItem->GetClassifyTable();
        TB_ITEM* pItemTable = pInvenItem->GetItemTable();

        if (!pClassifyTable || !pItemTable) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        std::uint32_t dwItemID = pInvenItem->GetCurID();

        if (!pInven->IsHelperItem(dwItemID)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 检查物品限制类是否匹配
        if (pItemTable->Item_Limit_Class != pTB_HELPER->Helper_Number) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 检查助手槽位
        if (psEquip.shHelperSlotPos > 2) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        STItem* stHelperItem = &stHelper.stItem[psEquip.shHelperSlotPos];
        if (stHelperItem->xSerial != psEquip.xHelperSerial) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 获取助手物品表信息
        TB_ITEM* pHelperItemTable = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, stHelperItem->nItemID);
        if (!pHelperItemTable) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        TB_ITEM_CLASSIFY* pHelperClassifyTable = XResourceMgr::GetTB_ITEM_CLASSIFY(&pGameServer->m_xResourceMgr, pHelperItemTable->Item_Classify_Index);
        if (!pHelperClassifyTable) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 检查槽位类型是否匹配
        if (pClassifyTable->Item_Slot_Type != pHelperClassifyTable->Item_Slot_Type) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        psDB.psEquip.byInvenType = pInvenItem->GetInvenType();
    }
    else if (psEquip.xInvenSerial) {
        // IDA: 情况2: 从背包装备到助手空槽
        std::shared_ptr<CItem> pInvenItem;
        pInven->GetItemPtr(pInvenItem, psEquip.xInvenSerial);

        if (!pInvenItem) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        TB_ITEM_CLASSIFY* pClassifyTable = pInvenItem->GetClassifyTable();
        TB_ITEM* pItemTable = pInvenItem->GetItemTable();

        if (!pClassifyTable || !pItemTable) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        std::uint32_t dwItemID = pInvenItem->GetCurID();

        if (!pInven->IsHelperItem(dwItemID)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 检查物品限制类是否匹配
        if (pItemTable->Item_Limit_Class != pTB_HELPER->Helper_Number) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 获取助手槽位
        std::int16_t shHelperSlot = pInven->ConvertHelperInvenSlot(pClassifyTable->Item_Slot_Type);
        if (shHelperSlot > 2 || shHelperSlot < 0) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 检查槽位是否为空
        STItem* stHelperItem = &stHelper.stItem[shHelperSlot];
        if (stHelperItem->nItemID > 0) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        psDB.psEquip.shHelperSlotPos = shHelperSlot;
    }
    else if (psEquip.xHelperSerial) {
        // IDA: 情况3: 从助手卸下到背包空槽
        if (psEquip.shHelperSlotPos > 2) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        STItem* stHelperItem = &stHelper.stItem[psEquip.shHelperSlotPos];

        TB_ITEM* pHelperItemTable = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, stHelperItem->nItemID);
        if (!pHelperItemTable) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        TB_ITEM_CLASSIFY* pHelperClassifyTable = XResourceMgr::GetTB_ITEM_CLASSIFY(&pGameServer->m_xResourceMgr, pHelperItemTable->Item_Classify_Index);
        if (!pHelperClassifyTable) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        // IDA: 获取目标背包
        XBaseInventory* pBaseInven = pInven->GetTBInvenPtr(pHelperClassifyTable->Item_Inven_Type);
        if (!pBaseInven) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xE294);
            return;
        }

        psDB.psEquip.byInvenType = pBaseInven->GetInvenType();
        psDB.psEquip.shInvenSlotPos = pBaseInven->GetEmptySlot_2();

        if (psDB.psEquip.shInvenSlotPos == -1) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 0x10, 0xCB2A);
            return;
        }
    }

    // IDA: 发送数据库请求 (Main=0x26, Sub=5)
    XSendDBPacket xSendDBPacket(pUser, 0x26, 5);
    xSendDBPacket << psDB;
    pGameServer->SendDBGame(&xSendDBPacket);
}

// IDA: ?ReqHelperChangeOrder@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_ORDER@@@Z (0x140097eb0)
// 对齐 IDA: 请求更改助手顺序
void CGocHelper::ReqHelperChangeOrder(PS_HELPER_CHANGE_ORDER& psOrder) {
    // IDA: 查找两个助手
    auto it1 = m_mapHelper.find(psOrder.dwHelperID_1);
    auto it2 = m_mapHelper.find(psOrder.dwHelperID_2);

    // IDA: 如果两个助手都存在
    if (it1 == m_mapHelper.end() || it2 == m_mapHelper.end()) {
        return;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 设置 UCID 和交换顺序
    psOrder.dwUCID = pUser->GetUCID();
    psOrder.byOrder_1 = it2->second.byOrder;
    psOrder.byOrder_2 = it1->second.byOrder;

    // IDA: 发送数据库请求 (Main=0x26, Sub=6)
    XSendDBPacket xSendDBPacket(pUser, 0x26, 6);
    xSendDBPacket << psOrder;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// IDA: ?ReqHelperChangeAutoSummon@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_AUTO_SUMMON@@@Z (0x1400980c0)
// 对齐 IDA: 请求更改自动召唤设置
void CGocHelper::ReqHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& psFlag) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 检查是否与当前设置相同
    bool bNewFlag = (psFlag.bySelect != 0);
    if (IsAutoSummon() == bNewFlag) {
        // IDA: 如果相同，直接发送响应给客户端 (Main=0x27, Sub=0x15)
        XSendPacket xSendPacket(0x27, 0x15);
        xSendPacket << psFlag;
        CGocNetwork::Send(pOwner, &xSendPacket);
        return;
    }

    // IDA: 发送数据库请求 (Main=0x26, Sub=7)
    XSendDBPacket xSendDBPacket(pOwner, 0x26, 7);
    xSendDBPacket << psFlag;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// IDA: ?ReqHelperSupportEquip@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_REQ@@@Z (0x140097080)
// 对齐 IDA: 请求支援装备
void CGocHelper::ReqHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_REQ& psEquip) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 设置 UCID
    psEquip.dwUCID = pUser->GetUCID();

    // IDA: 查找助手
    auto it = m_mapHelper.find(psEquip.nPoint);
    auto itEnd = m_mapHelper.end();

    if (it != itEnd) {
        // IDA: 发送请求到 RelayServer (Main=0xF5, Sub=0x31)
        XSendPacket xSendPacket(0xF5, 0x31);
        xSendPacket << psEquip;

        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (pGameServer) {
            pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 9);
        }
    }
}

// IDA: ?SendDBAddHelper@CGocHelper@@QEAA_NK@Z (0x14009a290)
// 对齐 IDA: 发送添加助手到数据库
bool CGocHelper::SendDBAddHelper(std::uint32_t dwHelperID) {
    // IDA: 检查参数
    if (!dwHelperID) {
        return false;
    }

    // IDA: 检查是否已存在
    if (FindHelper(dwHelperID)) {
        return false;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return false;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return false;
    }

    // IDA: 构造请求
    PS_HELPER_ADD_REQ psHelper;
    memset(&psHelper, 0, sizeof(psHelper));
    psHelper.dwUCID = pUser->GetUCID();
    psHelper.dwHelperID = dwHelperID;
    psHelper.byOrder = GetLastOrderNumber() + 1;

    // IDA: 发送数据库请求 (Main=0x26, Sub=2)
    XSendDBPacket xSendDBPacket(pUser, 0x26, 2);
    xSendDBPacket << psHelper;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);

    // IDA: 记录日志
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 16;
    stLog._sSubType = 1;
    stLog.nParam0 = dwHelperID;
    stLog.nParam2 = pUser->GetLevel();
    // IDA: wcscpy_s(stLog.szComment, &word_140B69570);
    XGameServer::Instance()->SendDBLog(&stLog);

    // IDA: 记录统计
    ST_STATISTICS_HELPER stStatistics;
    memset(&stStatistics, 0, sizeof(stStatistics));
    stStatistics.byFlag = 1;
    stStatistics.dwUCID = pUser->GetUCID();
    stStatistics.dwHelperID = dwHelperID;

    XSendDBPacket xSendDBStatistics(pUser, 0xF0, 5);
    xSendDBStatistics << stStatistics;
    XGameServer::Instance()->SendDBStatistics(&xSendDBStatistics);

    return true;
}

// ============================================================================
// Response Handlers (from DB/Server)
// ============================================================================

// IDA: ?ResHelperSupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140098280)
// 对齐 IDA: 处理支援信息响应
void CGocHelper::ResHelperSupportInfo(PS_HELPER_SUPPORT_INFO_RES& psInfo) {
    // IDA: CGocHelper::SetMySupportInfo(this, psInfo);
    SetMySupportInfo(psInfo);

    // IDA: 发送响应给客户端 (Main=0x27, Sub=5)
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    XSendPacket xSendPacket(0x27, 5);
    xSendPacket << psInfo;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportRegister@CGocHelper@@QEAAXAEAUPS_SERVER_HELPER_SUPPORT_REGISTER@@@Z (0x140098370)
// 对齐 IDA: 处理支援注册响应
void CGocHelper::ResHelperSupportRegister(PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);

    // IDA: 如果成功，更新支援信息
    if (!psSupport.nResult) {
        // IDA: 构造支援信息
        ST_HELPER_SUPPORT_INFO stMyInfo = psSupport.stInfo;
        stMyInfo.bRegister = 1;
        stMyInfo.byRewardType = 1;
        SetMySupportInfo(stMyInfo);

        // IDA: 记录日志
        if (pUser) {
            ST_LOG_GAME stLog;
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetUCID();
            stLog._sMainType = 16;
            stLog._sSubType = 8;
            stLog.nParam2 = pUser->GetLevel();
            stLog.nParam3 = stMyInfo.stInfo.bySupportType;
            XGameServer::Instance()->SendDBLog(&stLog);
        }
    }

    // IDA: 发送响应给客户端 (Main=0x27, Sub=6)
    XSendPacket xSendPacket(0x27, 6);
    xSendPacket << psSupport;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportReward@CGocHelper@@QEAAXAEAUPS_SERVER_HELPER_SUPPORT_REWARD@@@Z (0x140098600)
// 对齐 IDA: 处理支援奖励响应
void CGocHelper::ResHelperSupportReward(PS_SERVER_HELPER_SUPPORT_REWARD& psReward) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    std::int32_t nResult = psReward.nResult;

    // IDA: if (psReward->nResult) { m_byMySupportReward = 0; }
    if (psReward.nResult) {
        m_byMySupportReward = 0;
    } else {
        // IDA: 获取 CGocInventory 组件
        std::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();
        if (!pInven) {
            return;
        }

        // IDA: 创建物品
        ST_LOG_GAME stLogData;
        if (!pInven->CreateItemReq(
                psReward.stCreateItems,
                false,
                E_ITEM_CREATE_TYPE_HELPER_SUPPORT,
                stLogData)) {
            CGocNetwork::SendErrorMessage(pOwner, 0x27, 7, 0xCB2A);
            return;
        }

        // IDA: 设置奖励状态并添加好友点数
        m_byMySupportReward = 2;
        pInven->AddTotalFriendPoint(psReward.wFriendPoint, 1);
    }

    // IDA: 发送响应给客户端 (Main=0x27, Sub=7)
    XSendPacket xSendPacket(0x27, 7);
    xSendPacket << nResult;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportList@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_LIST_RES@@@Z (0x140098850)
// 对齐 IDA: 处理支援列表响应
void CGocHelper::ResHelperSupportList(PS_HELPER_SUPPORT_LIST_RES& psList) {
    // IDA: 发送响应给客户端 (Main=0x27, Sub=8)
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    XSendPacket xSendPacket(0x27, 8);
    xSendPacket << psList;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportEquip@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_RES@@@Z (0x140098920)
// 对齐 IDA: 处理支援装备响应
void CGocHelper::ResHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_RES& psEquip) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);

    // IDA: 构造响应
    PS_HELPER_SUPPORT_EQUIP_RES psResult;
    psResult.nResult = psEquip.nResult;

    // IDA: 如果成功，更新助手的好友支援信息
    if (!psEquip.nResult) {
        auto it = m_mapHelper.find(psEquip.dwHelperID);
        if (it != m_mapHelper.end()) {
            ST_HELPER_INFO& stHelper = it->second;
            // IDA: qmemcpy(&stHelper->stFriendSupport, &psEquip->stHelper.stFriendSupport, sizeof(stHelper->stFriendSupport));
            std::memcpy(&stHelper.stFriendSupport, &psEquip.stHelper.stFriendSupport, sizeof(stHelper.stFriendSupport));
            CalcHelperStatsALL(&stHelper, 1);

            psResult.dwHelperID = psEquip.dwHelperID;
            psResult.stHelper = stHelper;

            // IDA: 记录日志
            if (pUser) {
                ST_LOG_GAME stLog;
                stLog._nUAID = pUser->GetUAID();
                stLog._nUCID = pUser->GetUCID();
                stLog._sMainType = 16;
                stLog._sSubType = 9;
                stLog.nParam0 = psEquip.dwHelperID;
                stLog.nParam1 = psEquip.stHelper.stFriendSupport.dwFriendUCID;
                stLog.nParam2 = pUser->GetLevel();
                stLog.nParam3 = psEquip.stHelper.stFriendSupport.bySupportType;
                XGameServer::Instance()->SendDBLog(&stLog);
            }
        }
    }

    // IDA: 发送响应给客户端 (Main=0x27, Sub=9)
    XSendPacket xSendPacket(0x27, 9);
    xSendPacket << psResult;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportEquipReward@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_REWARD_RES@@@Z (0x140098ca0)
// 对齐 IDA: 处理支援装备奖励响应
void CGocHelper::ResHelperSupportEquipReward(PS_HELPER_SUPPORT_EQUIP_REWARD_RES& psReward) {
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 获取 CGocFriend 组件
    std::shared_ptr<CGocFriend> pFriend = pOwner->GetGOC<CGocFriend>();
    if (!pFriend) {
        return;
    }

    // IDA: 添加好友点数
    pFriend->AddFriendPoint(psReward.dwFriendUCID, psReward.wFriendPointReward);
}

// IDA: ?ResHelperEquip@CGocHelper@@QEAAXAEAUPS_DB_HELPER_EQUIP_RES@@@Z (0x140098d20)
// 对齐 IDA: 处理装备响应
void CGocHelper::ResHelperEquip(PS_DB_HELPER_EQUIP_RES& psRes) {
    // IDA: 如果有错误，直接返回
    if (psRes.nError) {
        return;
    }

    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);

    // IDA: 创建删除列表
    std::vector<std::uint32_t> vecDelHelperInven;

    // IDA: 查找助手
    auto it = m_mapHelper.find(psRes.psEquip.dwHelperID);
    auto itEnd = m_mapHelper.end();

    if (it == itEnd) {
        return;
    }

    ST_HELPER_INFO& stHelper = it->second;

    // IDA: 获取 CGocInventory 组件
    std::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();

    // IDA: 处理三种情况
    if (psRes.psEquip.xInvenSerial && psRes.psEquip.xHelperSerial) {
        // IDA: 情况1: 交换背包和助手物品
        STItem stIvenItem;
        STItem stHelperItem;
        memset(&stIvenItem, 0, sizeof(stIvenItem));
        memset(&stHelperItem, 0, sizeof(stHelperItem));

        // IDA: 复制助手物品
        if (psRes.psEquip.shHelperSlotPos <= 2) {
            stHelperItem = stHelper.stItem[psRes.psEquip.shHelperSlotPos];
            stHelper.stItem[psRes.psEquip.shHelperSlotPos].Init();
            vecDelHelperInven.push_back(stHelperItem.nItemID);
        }

        // IDA: 获取背包物品
        if (pInven) {
            std::shared_ptr<CItem> pInvenItem;
            pInven->GetItemPtr(pInvenItem, psRes.psEquip.xInvenSerial);

            if (pInvenItem) {
                pInvenItem->GetItem(&stIvenItem);
                std::int32_t nSlot = pInvenItem->GetSlot();
                std::uint8_t byInvenType = pInvenItem->GetInvenType();
                pInven->RemoveItem(byInvenType, nSlot);
                pInven->SendBreakItem(byInvenType, nSlot);
            }

            // IDA: 如果两个物品都有效，进行交换
            if (stIvenItem.nItemID > 0 && stHelperItem.nItemID > 0) {
                // IDA: 装备到助手
                stHelper.stItem[psRes.psEquip.shHelperSlotPos] = stIvenItem;

                // IDA: 添加到背包
                STItem stAddItem = stHelperItem;
                pInven->AddItem(psRes.psEquip.byInvenType,
                                psRes.psEquip.shInvenSlotPos,
                                stAddItem, false);

                // IDA: 发送创建物品消息
                PS_RES_STORAGE_INFO stItemList;
                PS_STORAGE_INFO stInfo;
                stInfo.byInvenType = psRes.psEquip.byInvenType;
                stInfo.shSlotPos = psRes.psEquip.shInvenSlotPos;
                stInfo.stItem = stHelperItem;
                stItemList.vecItem.push_back(stInfo);
                pInven->SendCreateItem(stItemList);

                // IDA: 记录日志
                if (pUser) {
                    // IDA: 日志1 - 装备
                    ST_LOG_GAME stLog;
                    stLog._nUAID = pUser->GetUAID();
                    stLog._nUCID = pUser->GetUCID();
                    stLog._sMainType = 16;
                    stLog._sSubType = 7;
                    stLog.nParam0 = psRes.psEquip.dwHelperID;
                    stLog.nParam2 = pUser->GetLevel();
                    stLog.nParam3 = stHelperItem.nItemID;
                    stLog.nParam4 = psRes.psEquip.shHelperSlotPos;
                    stLog.nParam6 = stHelperItem.xSerial;
                    XGameServer::Instance()->SendDBLog(&stLog);

                    // IDA: 日志2 - 卸下
                    ST_LOG_GAME stLogGame;
                    stLogGame._nUAID = pUser->GetUAID();
                    stLogGame._nUCID = pUser->GetUCID();
                    stLogGame._sMainType = 16;
                    stLogGame._sSubType = 6;
                    stLogGame.nParam0 = psRes.psEquip.dwHelperID;
                    stLogGame.nParam2 = pUser->GetLevel();
                    stLogGame.nParam3 = stIvenItem.nItemID;
                    stLogGame.nParam4 = psRes.psEquip.shHelperSlotPos;
                    stLogGame.nParam6 = stIvenItem.xSerial;
                    XGameServer::Instance()->SendDBLog(&stLogGame);
                }
            }
        }
    }
    else if (psRes.psEquip.xInvenSerial) {
        // IDA: 情况2: 从背包装备到助手空槽
        STItem stInvenItem;
        memset(&stInvenItem, 0, sizeof(stInvenItem));

        if (pInven) {
            std::shared_ptr<CItem> pInvenItem;
            pInven->GetItemPtr(pInvenItem, psRes.psEquip.xInvenSerial);

            if (pInvenItem) {
                pInvenItem->GetItem(&stInvenItem);
                std::int32_t nSlot = pInvenItem->GetSlot();
                std::uint8_t byInvenType = pInvenItem->GetInvenType();
                pInven->RemoveItem(byInvenType, nSlot);
                pInven->SendBreakItem(byInvenType, nSlot);
            }

            // IDA: 如果物品有效，装备到助手
            if (stInvenItem.nItemID > 0 && psRes.psEquip.shHelperSlotPos <= 2) {
                stHelper.stItem[psRes.psEquip.shHelperSlotPos] = stInvenItem;

                // IDA: 记录日志
                if (pUser) {
                    ST_LOG_GAME stLog;
                    stLog._nUAID = pUser->GetUAID();
                    stLog._nUCID = pUser->GetUCID();
                    stLog._sMainType = 16;
                    stLog._sSubType = 6;
                    stLog.nParam0 = psRes.psEquip.dwHelperID;
                    stLog.nParam2 = pUser->GetLevel();
                    stLog.nParam3 = stInvenItem.nItemID;
                    stLog.nParam4 = psRes.psEquip.shHelperSlotPos;
                    stLog.nParam6 = stInvenItem.xSerial;
                    XGameServer::Instance()->SendDBLog(&stLog);
                }
            }
        }
    }
    else if (psRes.psEquip.xHelperSerial) {
        // IDA: 情况3: 从助手卸下到背包空槽
        STItem stHelperItem;
        memset(&stHelperItem, 0, sizeof(stHelperItem));

        if (psRes.psEquip.shHelperSlotPos <= 2) {
            stHelperItem = stHelper.stItem[psRes.psEquip.shHelperSlotPos];
            stHelper.stItem[psRes.psEquip.shHelperSlotPos].Init();
            vecDelHelperInven.push_back(stHelperItem.nItemID);
        }

        // IDA: 如果物品有效，添加到背包
        if (pInven && stHelperItem.nItemID > 0) {
            STItem stAddItem = stHelperItem;
            pInven->AddItem(psRes.psEquip.byInvenType,
                            psRes.psEquip.shInvenSlotPos,
                            stAddItem, false);

            // IDA: 发送创建物品消息
            PS_RES_STORAGE_INFO stItemList;
            PS_STORAGE_INFO stInfo;
            stInfo.byInvenType = psRes.psEquip.byInvenType;
            stInfo.shSlotPos = psRes.psEquip.shInvenSlotPos;
            stInfo.stItem = stHelperItem;
            stItemList.vecItem.push_back(stInfo);
            pInven->SendCreateItem(stItemList);

            // IDA: 记录日志
            if (pUser) {
                ST_LOG_GAME stLog;
                stLog._nUAID = pUser->GetUAID();
                stLog._nUCID = pUser->GetUCID();
                stLog._sMainType = 16;
                stLog._sSubType = 7;
                stLog.nParam0 = psRes.psEquip.dwHelperID;
                stLog.nParam2 = pUser->GetLevel();
                stLog.nParam3 = stHelperItem.nItemID;
                stLog.nParam4 = psRes.psEquip.shHelperSlotPos;
                stLog.nParam6 = stHelperItem.xSerial;
                XGameServer::Instance()->SendDBLog(&stLog);
            }
        }
    }

    // IDA: 更新助手属性
    UnEquipHelperItemStats(psRes.psEquip.dwHelperID, vecDelHelperInven);
    CalcHelperStatsALL(&stHelper, 1);

    // IDA: 发送响应给客户端 (Main=0x27, Sub=0x10)
    PS_HELPER_EQUIP_RES psResult;
    psResult.nResult = psRes.nError;
    psResult.stHelper = stHelper;

    XSendPacket xSendPacket(0x27, 0x10);
    xSendPacket << psResult;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportRelease@CGocHelper@@QEAAXAEAUPS_DB_HELPER_SUPPORT_RELEASE@@@Z (0x140099d60)
// 对齐 IDA: 处理支援释放响应
void CGocHelper::ResHelperSupportRelease(PS_DB_HELPER_SUPPORT_RELEASE& psRelease) {
    // IDA: 如果成功，清除好友支援信息
    if (psRelease.nResult) {
        return;
    }

    auto it = m_mapHelper.find(psRelease.dwHelperID);
    if (it == m_mapHelper.end()) {
        return;
    }

    ST_HELPER_INFO& stHelper = it->second;

    // IDA: 清除 stFriendSupport
    memset(&stHelper.stFriendSupport, 0, sizeof(stHelper.stFriendSupport));

    // IDA: 卸下好友支援物品属性
    UnEquipHelperFriendItemStats(psRelease.dwHelperID);

    // IDA: 重新计算属性
    CalcHelperStatsALL(&stHelper, 1);

    // IDA: 发送响应给客户端 (Main=0x27, Sub=0x11)
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    XSendPacket xSendPacket(0x27, 0x11);
    xSendPacket << stHelper;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperChangeOrder@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_ORDER@@@Z (0x140099f60)
// 对齐 IDA: 处理顺序变更响应
void CGocHelper::ResHelperChangeOrder(PS_HELPER_CHANGE_ORDER& psOrder) {
    // IDA: 如果成功，更新顺序
    if (!psOrder.nError) {
        auto it1 = m_mapHelper.find(psOrder.dwHelperID_1);
        auto it2 = m_mapHelper.find(psOrder.dwHelperID_2);

        if (it1 != m_mapHelper.end() && it2 != m_mapHelper.end()) {
            // IDA: 交换顺序
            it1->second.byOrder = psOrder.byOrder_1;
            it2->second.byOrder = psOrder.byOrder_2;
        }
    }

    // IDA: 发送响应给客户端 (Main=0x27, Sub=0x12)
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    XSendPacket xSendPacket(0x27, 0x12);
    xSendPacket << psOrder;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperChangeAutoSummon@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_AUTO_SUMMON@@@Z (0x14009a140)
// 对齐 IDA: 处理自动召唤变更响应
void CGocHelper::ResHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& psFlag) {
    // IDA: 如果 bySelect 是 0 或 1，更新标志
    if (!psFlag.bySelect || psFlag.bySelect == 1) {
        SetAutoSummonFlag(psFlag.bySelect);
    }

    // IDA: psFlag->bySelect = IsAutoSummon();
    psFlag.bySelect = IsAutoSummon() ? 1 : 0;

    // IDA: 发送响应给客户端 (Main=0x27, Sub=0x15)
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    XSendPacket xSendPacket(0x27, 0x15);
    xSendPacket << psFlag;
    CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?SendDBHelperList@CGocHelper@@QEAAXXZ (0x14009A670)
// 对齐 IDA: 发送助手列表请求到数据库
// IDA 反编译:
// void __fastcall CGocHelper::SendDBHelperList(CGocHelper *this)
// {
//   VBitmask *v1; // rax
//   int QuestID; // eax
//   XGameServer *v3; // rax
//   XSendDBPacket xSendDBPacket; // [rsp+30h] [rbp-10058h] BYREF
//   _BYTE v5[8]; // [rsp+10050h] [rbp-38h] BYREF
//   __int64 v6; // [rsp+10058h] [rbp-30h]
//   VChunkFile *v7; // [rsp+10060h] [rbp-28h]
//   IXObject *pObject; // [rsp+10068h] [rbp-20h]
//   DynArray_cl<int> *p_m_ChunkSizeTempMemOfs; // [rsp+10070h] [rbp-18h]
//
//   v6 = -2;
//   v7 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//   if ( v7 )
//     pObject = (IXObject *)&v7[3].m_ChunkSizeTempMemOfs;
//   else
//     pObject = nullptr;
//   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x26u, 1u);
//   p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size((VChunkLocker *)this)[3].m_ChunkSizeTempMemOfs;
//   v1 = (VBitmask *)((__int64 (__fastcall *)(DynArray_cl<int> *, _BYTE *))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(p_m_ChunkSizeTempMemOfs, v5);
//   QuestID = CQuestCondition::GetQuestID(v1);
//   XParse::operator<<(&xSendDBPacket.XParse, QuestID);
//   v3 = TXSingleton<XGameServer>::Instance();
//   XGameServer::SendDBGame(v3, &xSendDBPacket);
//   XSendDBPacket::~XSendDBPacket((VGameProjectileObject *)&xSendDBPacket);
// }
void CGocHelper::SendDBHelperList()
{
    // IDA: 获取 owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // IDA: 动态转换为 CUser
    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser) {
        return;
    }

    // IDA: 获取 UCID
    std::uint32_t dwUCID = pUser->GetUCID();

    // IDA: 发送数据库请求 (Main=0x26, Sub=1)
    XSendDBPacket xSendDBPacket(pUser, 0x26, 1);
    xSendDBPacket << dwUCID;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}
