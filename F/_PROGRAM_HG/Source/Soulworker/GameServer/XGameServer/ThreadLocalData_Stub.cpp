// ThreadLocalData_Stub.cpp
// Stub implementation for ThreadLocalData static methods
// This file provides minimal implementations to resolve linker errors

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <windows.h>
#endif

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_TimeCompat.h"
#include "Soulworker/GameServer/XGameServer/VaccumCube.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerGM.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XGameServer/XMonsterMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/LeagueMember.h"
#include "Soulworker/GameServer/XGameServer/XPartyManager.h"
#include "Soulworker/GameServer/XGameServer/User.h"

namespace {
struct GreenDamTan_ThreadLocalSlots {
    void* reserved0 = nullptr;
    VDefaultTimer* timer = nullptr;
    void* reserved2 = nullptr;
    ThreadLocalData* localData = nullptr;
};

thread_local GreenDamTan_ThreadLocalSlots g_threadLocalSlots;
}

// ============================================================================
// ThreadLocalData Constructor - IDA @ 0x1406D0A50
// ============================================================================
ThreadLocalData::ThreadLocalData()
    : m_nMazeCount(0)
    , m_nMonsterCount(0)
    , m_nModeMazeCount(0)
    , m_bInitPool(false)
    , m_bLoadWorld(false)
    , m_bReqSyncWorld(false)
    , m_nLogSynctime(0)
    , m_nReportSynctime(0)
    , m_nThreadCount(0)
    , m_nOwnerThreadIndex(0)
    , m_xPartyMgr(new XPartyManager())
    , m_xForceMgr(nullptr)
    , m_xMonsterMgr(new XMonsterMgr())
    , m_xNpcMgr(nullptr)
    , m_xAkashicMgr(nullptr)
    , m_xProjectileMgr(nullptr)
    , m_xTrapMgr(nullptr)
    , m_xChainLightningMgr(nullptr)
    , m_xInteractionObjectMgr(nullptr)
    , m_xVaccumCubeObjectMgr(nullptr)
    , m_xSocialItemObjectMgr(nullptr)
    , m_xGameModeMgr(nullptr)
    , m_xLuaScript(nullptr)
    , m_xScriptManager(nullptr)
    , m_DohHavokResourceManager(nullptr)
    , m_xDistrictPool(nullptr)
    , m_xBattleZonePool(nullptr)
    , m_xMazePool(nullptr)
    , m_xMyRoomPool(nullptr)
    , m_xModeMazePool(nullptr)
    , m_pDistrictCreator(nullptr)
    , m_pBattleZoneCreator(nullptr)
    , m_pMazeCreator(nullptr)
    , m_pMyRoomCreator(nullptr)
    , m_pModeMazeCreator(nullptr)
{
}

ThreadLocalData::~ThreadLocalData() {
    delete m_xPartyMgr;
    m_xPartyMgr = nullptr;
    delete m_xMonsterMgr;
    m_xMonsterMgr = nullptr;
}

void ThreadLocalData::Initialize() {
    m_nMazeCount = 0;
    m_nMonsterCount = 0;
    m_nModeMazeCount = 0;
    m_bInitPool = false;
    m_bLoadWorld = false;
    m_bReqSyncWorld = false;
    m_nLogSynctime = static_cast<decltype(m_nLogSynctime)>(
        GreenDamTan::GetTickCount64Compat() + 60000);
    m_nReportSynctime = static_cast<decltype(m_nReportSynctime)>(
        GreenDamTan::GetTickCount64Compat() + 10000);
}

void ThreadLocalData::InitPool() {
    m_bInitPool = true;
}

void ThreadLocalData::Clear() {
    if (!m_bInitPool) {
        return;
    }
    m_mapArea.clear();
    m_mapAi.clear();
    if (m_xMonsterMgr) {
        m_xMonsterMgr->ClearAll();
    }
}

void ThreadLocalData::Update(float fDeltaTime) {
    if (!m_bInitPool) {
        return;
    }
    for (auto& pair : m_mapArea) {
        if (pair.second) {
            pair.second->OnUpdate(fDeltaTime);
        }
    }
}

ThreadLocalData* ThreadLocalData::CreateInstance(
    int threadCount,
    int ownerThreadIndex) {
    VDefaultTimer* timer = new VDefaultTimer(false);
    g_threadLocalSlots.timer = timer;
    timer->Init();

    ThreadLocalData* instance = new ThreadLocalData();
    g_threadLocalSlots.localData = instance;
    instance->m_nThreadCount = threadCount;
    instance->m_nOwnerThreadIndex = ownerThreadIndex;
    return instance;
}

ThreadLocalData* ThreadLocalData::GetInstance() {
    return g_threadLocalSlots.localData;
}

VDefaultTimer* ThreadLocalData::GetTimer() {
    return g_threadLocalSlots.timer;
}

// IDA: ?IsThreadArea@ThreadLocalData@@QEAA_NTUXMapID@@@Z @ 0x1406D5E10
bool ThreadLocalData::IsThreadArea(UXMapID uxMapID) {
    // IDA: Check if area belongs to this thread
    return FindArea(uxMapID) != nullptr;
}

void ThreadLocalData::DestroyInstance() {
    VDefaultTimer* timer = g_threadLocalSlots.timer;
    if (timer) {
        timer->DeleteThis();
        g_threadLocalSlots.timer = nullptr;
    }

    ThreadLocalData* instance = g_threadLocalSlots.localData;
    if (instance) {
        delete instance;
        g_threadLocalSlots.localData = nullptr;
    }
}

CVaccumCube* ThreadLocalData::CreateVaccumCubeObject(XVec3 vPos) {
    CVaccumCube* pVaccum = CVaccumCube::CreateObject();
    if (pVaccum) {
        pVaccum->SetPosition(hkvVec3(vPos.x, vPos.y, vPos.z));
    }
    return pVaccum;
}

void ThreadLocalData::DeleteVaccumCubeObject(CVaccumCube* pVaccumCube) {
    delete pVaccumCube;
}

// ============================================================================
// CreateMonster - Stub implementation for linker
// IDA: ?CreateMonster@ThreadLocalData@@QEAAPEAVCMonster@@PEAVXArea@@TUXMapID@@HUXVec3@@M@Z
// ============================================================================
CMonster* ThreadLocalData::CreateMonster(XArea* pArea, UXMapID uxMapID, int nMonsterID, XVec3 vPos, float fRot) {
    // IDA: Create monster using monster manager
    CMonster* pMonster = nullptr;
    if (m_xMonsterMgr) {
        pMonster = m_xMonsterMgr->Create(uxMapID, nMonsterID, &vPos, fRot);
    }

    if (pMonster) {
        // Increment monster count
        ++m_nMonsterCount;

        // Set area for the monster
        // pMonster->SetArea(pArea);

        // Generate event object
        // pMonster->GenerateEventObject();

        // Check height and adjust position
        // hkvVec3 vCheckPos(vPos.x, vPos.y, vPos.z);
        // if (pMonster->GetHeight(&vCheckPos, 200.0f)) {
        //     vPos.z = vCheckPos.z;
        //     pMonster->Move(&vPos);
        // }

        // Set initial yaw
        // pMonster->SetInitYaw(fRot);
    }

    return pMonster;
}

// ============================================================================
// GetScriptManager - Stub implementation for linker
// IDA: ?GetScriptManager@ThreadLocalData@@QEAAAEAVVScriptResourceManager@@XZ
// ============================================================================
VScriptResourceManager& ThreadLocalData::GetScriptManager() {
    static VScriptResourceManager s_instance;
    return s_instance;
}

// ============================================================================
// DeleteMonster - Stub implementation for linker
// IDA: ?DeleteMonster@ThreadLocalData@@QEAAXPEAVCMonster@@@Z
// ============================================================================
void ThreadLocalData::DeleteMonster(CMonster* pMonster) {
    // IDA: Delete monster using monster manager
    if (!pMonster) return;
    if (m_xMonsterMgr) {
        m_xMonsterMgr->Delete(pMonster);
    }
}

// ============================================================================
// CreateNpc - Stub implementation for linker
// IDA: ?CreateNpc@ThreadLocalData@@QEAAPEAVCNpc@@PEAVXArea@@TUXMapID@@KHUXVec3@@MH@Z
// ============================================================================
CNpc* ThreadLocalData::CreateNpc(XArea* pArea, UXMapID uxMapID, std::uint32_t nSectorID, int nNpcID, XVec3* vPos, float fRot, int nUnused) {
    // IDA: Create NPC - currently stubbed
    // TODO: Implement using XNpcMgr when available
    (void)pArea;
    (void)uxMapID;
    (void)nSectorID;
    (void)nNpcID;
    (void)vPos;
    (void)fRot;
    (void)nUnused;
    return nullptr;
}

// ============================================================================
// DeleteNpc - Stub implementation for linker
// ============================================================================
void ThreadLocalData::DeleteNpc(CNpc* pNpc) {
    // IDA: Delete NPC using NPC manager
    if (!pNpc) return;
    // TODO: Implement using XNpcMgr when available
    delete pNpc;
}

// ============================================================================
// CreateAkashicObject
// IDA: ?CreateAkashicObject@ThreadLocalData@@QEAAPEAVCAkashicObject@@PEAVXArea@@TUXMapID@@HUXVec3@@MK@Z (0x1406D8FB0)
// 状态: 部分还原 - 原始逻辑 XAkashicObjectMgr::Create(&m_xAkashicMgr, uxMapID,
//   nAkashicID, vPos, fRot, dwParentID) -> 非空 vftable SetArea(pArea)
// TODO: 需人工审查 - XAkashicObjectMgr.h 半成品 (自造 TXObjectMgr 特化与
//   IXObject.h 主模板冲突), AkashicMgr 批次按 XMonsterMgr 自足类模式重写后
//   恢复完整链
// ============================================================================
CAkashicObject* ThreadLocalData::CreateAkashicObject(XArea* pArea, UXMapID uxMapID,
                                                    int nAkashicID, XVec3* vPos,
                                                    float fRot, unsigned int dwParentID) {
    (void)pArea;
    (void)uxMapID;
    (void)nAkashicID;
    (void)vPos;
    (void)fRot;
    (void)dwParentID;
    return nullptr;
}

// ============================================================================
// DeleteAkashicObject
// IDA: ?DeleteAkashicObject@ThreadLocalData@@QEAAXPEAVCAkashicObject@@@Z (0x1406D9070)
// 状态: STUB - 原始逻辑 XMonsterMgr::Delete(&this->m_xAkashicMgr, pAkashic)
// TODO: 需人工审查 - 同 CreateAkashicObject, AkashicMgr 批次后恢复
// ============================================================================
void ThreadLocalData::DeleteAkashicObject(CAkashicObject* pAkashic) {
    (void)pAkashic;
}

void ThreadLocalData::SendWorldEventBooster(
    unsigned long dwBuffID,
    __int64 biEndDate) {
    for (auto& pair : m_mapArea) {
        XArea* pArea = pair.second;
        if (pArea) {
            pArea->SendWorldEventBooster(dwBuffID, biEndDate);
        }
    }
}

// ============================================================================
// SendBroadcast - IDA @ 0x1406D6100
// ============================================================================
void ThreadLocalData::SendBroadcast(XSendPacket& xSendPacket) {
    for (auto& pair : m_mapArea) {
        XArea* pArea = pair.second;
        if (pArea) {
            pArea->SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eNearby);
        }
    }
}

// ============================================================================
// SendTimeEvent - IDA @ 0x1406D85A0
// ============================================================================
void ThreadLocalData::SendTimeEvent(ST_GM_TIME_EVENT_INFO& stInfo) {
    for (auto& pair : m_mapArea) {
        XArea* pArea = pair.second;
        if (pArea) {
            pArea->SendTimeEvent(stInfo);
        }
    }
}

// ============================================================================
// UpdateChannelAll - IDA @ 0x1406D67D0
// ============================================================================
void ThreadLocalData::UpdateChannelAll(PS_CHANNEL_INFO& stChannel) {
    auto it = m_mapChannelInfo.find(stChannel.wMapID);
    if (it != m_mapChannelInfo.end()) {
        m_mapChannelInfo.erase(it);
    }
    m_mapChannelInfo.insert(std::make_pair(stChannel.wMapID, stChannel));
}

// ============================================================================
// UpdateChannel - IDA @ 0x1406D6920
// ============================================================================
void ThreadLocalData::UpdateChannel(std::uint16_t wMapID, ST_CHANNEL_INFO& stChannel) {
    auto it = m_mapChannelInfo.find(wMapID);
    if (it == m_mapChannelInfo.end()) {
        return;
    }
    for (std::size_t i = 0; i < it->second.vecChannel.size(); ++i) {
        if (it->second.vecChannel[i].wChannel == stChannel.wChannel) {
            it->second.vecChannel[i].byChannelState = stChannel.byChannelState;
            return;
        }
    }
}

// ============================================================================
// CreateMatchingMaze (PS_FORCE_INFO variant) - IDA @ 0x1406D2960
// 状态: STUB - 活动边界占位
// TODO: 需人工审查 - 完整实现依赖:
//   1. m_xMazePool (TXPool<XMaze>) 池化实例 - 完整 ThreadLocalData 0x1450 布局组件
//   2. XMaze::Create / SetPartyInfo / SetForce
//   3. 建立成功后 lambda1 DoJobAllThread 广播 + ST_LOG_GAME(5,1) L"메이즈 생성" 日志
//   4. (0xF2,0x42) {dwMatchingID, stCreateMaze, stForceInfo} 回 ControlServer
//   5. 失败码 50006 (无池) / 50007 (Create 失败)
// 依赖: ThreadLocalData 完整布局批次（0x1450 布局重建是独立已规划任务）
void ThreadLocalData::CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_FORCE_INFO& stForceInfo, unsigned long dwMatchingID) {
    // TODO: 需人工审查 - m_xMazePool 池不可用（活动层无池化迷宫组件）
    stCreateMaze.nResult = 50006;
    LogHelper::LogError("game.contents",
        "CreateMatchingMaze error - Failed create maze no pool[ MapID:%I64d ] ( %d )",
        stCreateMaze.uxMapID.nMapID, 409);
}

// ============================================================================
// AddLeagueMember - IDA @ 0x1406D70A0
// 状态: 部分还原 - m_mapLeagueMember 在活动层为 void* 占位映射
// （原始 0x1450 布局中为 std::map<unsigned long, CLeagueMember*>）。
// TODO: 需人工审查 - 完整布局批次应把成员改回强类型 CLeagueMember* 映射；
// 当前按 IDA 逻辑落地: 按 nLeagueID 查找 -> 已存在则 AddLeagueMember +
// CompareSyncCount；不存在则 new CLeagueMember + AddLeagueMember + 插入 +
// SendSyncLeagueInfo。
void ThreadLocalData::AddLeagueMember(CUser* pUser) {
    if (!pUser) {
        return;
    }

    const int nLeagueID = pUser->GetLeagueID();
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(nLeagueID));
    if (it != m_mapLeagueMember.end()) {
        CLeagueMember* pLeagueMember = static_cast<CLeagueMember*>(it->second);
        if (pLeagueMember) {
            pLeagueMember->AddLeagueMember(pUser);
            pLeagueMember->CompareSyncCount(pUser->GetActorID().dwActorID);
        }
        return;
    }

    CLeagueMember* pLeagueMember = new CLeagueMember();
    if (!pLeagueMember) {
        return;
    }
    pLeagueMember->AddLeagueMember(pUser);
    m_mapLeagueMember[static_cast<unsigned int>(nLeagueID)] = pLeagueMember;
    pLeagueMember->SendSyncLeagueInfo(pUser);
}

// ============================================================================
// CompareLeagueInventoryCount - IDA @ 0x1406DA820
// 已精确还原 - 按 nLeagueID 查 m_mapLeagueMember；
// 未命中返回 false；命中且非空则交给 CLeagueMember::CompareInventorySyncCount。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
bool ThreadLocalData::CompareLeagueInventoryCount(unsigned int nLeagueID, unsigned int dwUCID) {
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it == m_mapLeagueMember.end()) {
        return false;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (pMember) {
        return pMember->CompareInventorySyncCount(dwUCID);
    }
    return false;
}

// ============================================================================
// SendLeagueApply - IDA @ 0x1406D7540
// 已精确还原 - 按 stApplicant.nLeagueID 查 m_mapLeagueMember；
// 命中且非空则把 ST_LEAGUE_APPLICANT 按值副本交给
// CLeagueMember::SendLeagueApply。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueApply(ST_LEAGUE_APPLICANT& stApplicant) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(stApplicant.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (pMember) {
        pMember->SendLeagueApply(stApplicant);
    }
}

// ============================================================================
// SendLeagueRecruitNoticeToMember - IDA @ 0x1406D9600
// 已精确还原 - 按 stRecruitNoticeInfo.nLeagueID 查 m_mapLeagueMember；
// 命中且非空则把 ST_LEAGUE_RECRUIT_NOTICE 按值副本连同剩余时间交给
// CLeagueMember::SendLeagueRecruitNotice。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueRecruitNoticeToMember(ST_LEAGUE_RECRUIT_NOTICE& stRecruitNoticeInfo, __int64 biRemainTime) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(stRecruitNoticeInfo.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (pMember) {
        pMember->SendLeagueRecruitNotice(stRecruitNoticeInfo, biRemainTime);
    }
}

// ============================================================================
// SendLeagueRecordUpdate - IDA @ 0x1406D9890
// 已精确还原 - 按 stRecordInfo.nLeagueID 查 m_mapLeagueMember；
// 命中且非空则把 ST_LEAGUE_RECORD 按值副本交给 CLeagueMember::Record。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueRecordUpdate(ST_LEAGUE_RECORD& stRecordInfo) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(stRecordInfo.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (pMember) {
        pMember->Record(stRecordInfo);
    }
}

// ============================================================================
// DeleteLeague - IDA @ 0x1406D74A0
// 已精确还原 - 按 nLeagueID 查 m_mapLeagueMember；命中则先对成员管理器执行
// CLeagueMember::DeleteLeagueMember(dwUCID)，再从映射中移除该条目。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::DeleteLeague(unsigned int nLeagueID, unsigned int dwUCID) {
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (pMember) {
        pMember->DeleteLeagueMember(dwUCID);
    }
    m_mapLeagueMember.erase(it);
}

// ============================================================================
// SendLeagueKickout - IDA @ 0x1406D97C0
// 已精确还原 - 按 nLeagueID 查 m_mapLeagueMember；命中则
// CLeagueMember::UpdateSyncCount(nSyncCount) + KickoutLeagueMember
// (dwReqUCID, dwKickoutUCID, stUpdateInfo 按值副本)。
void ThreadLocalData::SendLeagueKickout(unsigned int nLeagueID, int dwReqUCID,
                                        unsigned int dwKickoutUCID,
                                        ST_LEAGUE_INFO_UPDATE& stUpdateInfo,
                                        int nSyncCount) {
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    ST_LEAGUE_INFO_UPDATE stUpdate = stUpdateInfo;
    pMember->KickoutLeagueMember(dwReqUCID, dwKickoutUCID, stUpdate);
}

// ============================================================================
// LeagueInfoChange - IDA @ 0x1406D7750
// 已精确还原 - 按 stInfo.nLeagueID 查 m_mapLeagueMember；命中则把
// ST_LEAGUE_INFO 按值副本交给 CLeagueMember::SendLeagueInfo 广播。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::LeagueInfoChange(ST_LEAGUE_INFO stInfo) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(stInfo.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    ST_LEAGUE_INFO stCopy = stInfo;
    pMember->SendLeagueInfo(stCopy);
}

// ============================================================================
// UpdateLeagueMember - IDA @ 0x1406D7B10
// 已精确还原 - 按 stUpdate.nLeagueID 查 m_mapLeagueMember；命中且非空则把
// ST_LEAGUE_MEMBER_UPDATE 按值副本交给 CLeagueMember::UpdateLeagueMember
// 向未同步成员广播 (0x22,0x22)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::UpdateLeagueMember(ST_LEAGUE_MEMBER_UPDATE stUpdate) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(stUpdate.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    ST_LEAGUE_MEMBER_UPDATE stCopy = stUpdate;
    pMember->UpdateLeagueMember(stCopy);
}

// ============================================================================
// SendLeagueNoticeChangeToMember - IDA @ 0x1406D9540
// 已精确还原 - 按 stNotice.nLeagueID 查 m_mapLeagueMember；命中且非空则把
// ST_LEAGUE_NOTICE 按值副本交给 CLeagueMember::SendLeagueNotice(stNotice,
// dwReqUCID) 向未同步且非 dwReqUCID 本人的成员广播 (0x22,0x29)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueNoticeChangeToMember(ST_LEAGUE_NOTICE stNotice, unsigned int dwReqUCID) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(stNotice.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    ST_LEAGUE_NOTICE stCopy = stNotice;
    pMember->SendLeagueNotice(stCopy, dwReqUCID);
}

// ============================================================================
// ChangeLeagueAuth - IDA @ 0x1406D7800
// 已精确还原 - 按 nLeagueID 查 m_mapLeagueMember；命中且非空则先
// CLeagueMember::UpdateSyncCount(nSyncCount)，再把 ST_LEAGUE_AUTH_CHANGE
// 按值副本交给 CLeagueMember::ChangeLeagueAuth 广播。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::ChangeLeagueAuth(ST_LEAGUE_AUTH_CHANGE stChange, int nLeagueID, int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    ST_LEAGUE_AUTH_CHANGE stCopy = stChange;
    pMember->ChangeLeagueAuth(stCopy);
}

// ============================================================================
// SendLeagueMsg - IDA @ 0x1406D7BC0
// 已精确还原 - 按 stChatInfo.dwLeagueID 查 m_mapLeagueMember；命中且非空则把
// PS_CHAT_LEAGUE 与 PS_CHAT_ITEM_LINK_FOR_SERVER 按值副本交给
// CLeagueMember::SendLeagueMsg 广播 (7,1) 聊天包。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueMsg(PS_CHAT_LEAGUE psChatInfo, PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(psChatInfo.dwLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    PS_CHAT_LEAGUE stChatCopy = psChatInfo;
    PS_CHAT_ITEM_LINK_FOR_SERVER psLinkCopy = psLinkItemInfo;
    pMember->SendLeagueMsg(stChatCopy, psLinkCopy);
}

// ============================================================================
// SendLeagueDelegate - IDA @ 0x1406DA1B0
// 已精确还原 - 按 psDelegateRes.nLeagueID 查 m_mapLeagueMember；命中且非空则先
// CLeagueMember::UpdateSyncCount(nSyncCount)，再把 PS_RES_LEAGUE_DELEGATE
// 按值副本交给 CLeagueMember::Delegate(stCopy, dwDelegatedUCID)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueDelegate(PS_RES_LEAGUE_DELEGATE& psDelegateRes,
                                         unsigned int dwDelegatedUCID,
                                         int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(
        psDelegateRes.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    PS_RES_LEAGUE_DELEGATE psCopy = psDelegateRes;
    pMember->Delegate(psCopy, dwDelegatedUCID);
}

// ============================================================================
// SendLeagueInventoryMove - IDA @ 0x1406DA3C0
// 已精确还原 - 先做 psResItemMoveInfo 副本并把 psStorageInfo.stItem 覆盖
// 副本的 stItem；按 psReqItemMoveInfo.nLeagueID 查 m_mapLeagueMember；
// 命中且非空则先 CLeagueMember::UpdateInventorySyncCount(nInventorySync)，
// 再把 PS_RES_ITEM_MOVE_LEAGUE_INVEN 按值副本交给
// CLeagueMember::InventoryMove(dwReqUCID, stCopy)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueInventoryMove(
        unsigned int dwReqUCID,
        PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& psItemMoveForServer) {
    PS_RES_ITEM_MOVE_LEAGUE_INVEN psItemMoveInfo = psItemMoveForServer.psResItemMoveInfo;
    psItemMoveInfo.stItem = psItemMoveForServer.psStorageInfo.stItem;

    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(
        psItemMoveForServer.psReqItemMoveInfo.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateInventorySyncCount(psItemMoveForServer.nInventorySync);
    PS_RES_ITEM_MOVE_LEAGUE_INVEN psCopy = psItemMoveInfo;
    pMember->InventoryMove(dwReqUCID, psCopy);
}

// ============================================================================
// SendLeagueJoinUser_Invite - IDA @ 0x1406D96C0
// 已精确还原 - 按 stMemberEx.stMember.nLeagueID 查 m_mapLeagueMember；
// 命中且非空则先 CLeagueMember::UpdateSyncCount(nSyncCount)，再把
// ST_LEAGUE_MEMBER_EX 与 ST_LEAGUE_INFO_UPDATE 按值副本连同 byApplyState
// 交给 CLeagueMember::JoinLeagueUser(stCopy, stUpdateCopy, byApplyState)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueJoinUser_Invite(ST_LEAGUE_MEMBER_EX stMemberEx,
                                               ST_LEAGUE_INFO_UPDATE stInfoUpdate,
                                               unsigned char byApplyState,
                                               int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(
        stMemberEx.stMember.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    ST_LEAGUE_MEMBER_EX stMemberCopy = stMemberEx;
    ST_LEAGUE_INFO_UPDATE stUpdateCopy = stInfoUpdate;
    pMember->JoinLeagueUser(stMemberCopy, stUpdateCopy, byApplyState);
}

// ============================================================================
// SendLeagueJoinUser_Apply - IDA @ 0x1406D7300
// 已精确还原 - 按 stMemberEx.stMember.nLeagueID 查 m_mapLeagueMember；
// 命中且非空则先 CLeagueMember::UpdateSyncCount(nSyncCount)，再把
// ST_LEAGUE_MEMBER_EX 与 ST_LEAGUE_INFO_UPDATE 按值副本交给
// CLeagueMember::JoinLeagueUser(stCopy, stUpdateCopy, 1)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueJoinUser_Apply(ST_LEAGUE_MEMBER_EX stMemberEx,
                                               ST_LEAGUE_INFO_UPDATE stInfoUpdate,
                                               int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(
        stMemberEx.stMember.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    ST_LEAGUE_MEMBER_EX stMemberCopy = stMemberEx;
    ST_LEAGUE_INFO_UPDATE stUpdateCopy = stInfoUpdate;
    pMember->JoinLeagueUser(stMemberCopy, stUpdateCopy, 1);
}

// ============================================================================
// LeagueApplicantUpdate - IDA @ 0x1406D7CC0
// 已精确还原 - 遍历 stUpdateList.vecInfo，逐项按 dwActorID 查
// m_mapLeagueMember；命中且非空则 CLeagueMember::UpdateApplicantList(dwUCID)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::LeagueApplicantUpdate(ST_LEAGUE_APPLICANT_CHECK_LIST& stUpdateList,
                                            unsigned int dwUCID) {
    for (std::size_t i = 0; i < stUpdateList.vecInfo.size(); ++i) {
        auto it = m_mapLeagueMember.find(stUpdateList.vecInfo[i]);
        if (it == m_mapLeagueMember.end()) {
            continue;
        }
        CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
        if (pMember) {
            pMember->UpdateApplicantList(dwUCID);
        }
    }
}

// ============================================================================
// UpdateMemberPosition - IDA @ 0x1406D7980
// 已精确还原 - 按 nLeagueID 查 m_mapLeagueMember；命中且非空则先
// CLeagueMember::UpdateSyncCount(nSyncCount)，再把 ST_LEAGUE_MEMBER_POSITION
// (0xC 字节) 按值副本交给 CLeagueMember::ChangeLeagueMemberPosition
// (stCopy, dwActorID) 广播。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::UpdateMemberPosition(ST_LEAGUE_MEMBER_POSITION stPosition,
                                          unsigned int nLeagueID,
                                          unsigned int dwActorID, int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    ST_LEAGUE_MEMBER_POSITION stCopy = stPosition;
    pMember->ChangeLeagueMemberPosition(stCopy, dwActorID);
}

// ============================================================================
// ChangePositionName - IDA @ 0x1406D78D0
// 已精确还原 - 按 nLeagueID 查 m_mapLeagueMember；命中且非空则把
// ST_LEAGUE_POSITION_NAME_CHANGE 按值副本交给 CLeagueMember::ChangeLeaguePositionName
// 向未同步成员广播 (0x22,0x33)。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::ChangePositionName(ST_LEAGUE_POSITION_NAME_CHANGE stChange, int nLeagueID) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    ST_LEAGUE_POSITION_NAME_CHANGE stCopy = stChange;
    pMember->ChangeLeaguePositionName(stCopy);
}

// ============================================================================
// SendLeagueCardChange - IDA @ 0x1406D9940
// 已精确还原 - 按 psCardInfo.nLeagueID 查 m_mapLeagueMember；命中且非空则先
// CLeagueMember::UpdateSyncCount(nSyncCount)，再把 PS_REQ_LEAGUE_CARD (0x10 字节)
// 按值副本交给 CLeagueMember::CardChange 广播。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueCardChange(PS_REQ_LEAGUE_CARD psCardInfo, int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(psCardInfo.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    PS_REQ_LEAGUE_CARD psCopy = psCardInfo;
    pMember->CardChange(psCopy);
}

// ============================================================================
// SendLeagueChangeName - IDA @ 0x1406DA280
// 已精确还原 - 按 psResChangeInfo.nLeagueID 查 m_mapLeagueMember；命中且非空则先
// CLeagueMember::UpdateSyncCount(nSyncCount)，再把 PS_RES_LEAGUE_NAME_CHANGE
// (0x1C 字节) 按值副本交给 CLeagueMember::ChangeName 广播。
// 说明: m_mapLeagueMember 在活动层为 void* 占位映射（同 AddLeagueMember）。
void ThreadLocalData::SendLeagueChangeName(PS_RES_LEAGUE_NAME_CHANGE& psResChangeInfo, int nSyncCount) {
    auto it = m_mapLeagueMember.find(static_cast<unsigned int>(psResChangeInfo.nLeagueID));
    if (it == m_mapLeagueMember.end()) {
        return;
    }
    CLeagueMember* pMember = static_cast<CLeagueMember*>(it->second);
    if (!pMember) {
        return;
    }
    pMember->UpdateSyncCount(nSyncCount);
    PS_RES_LEAGUE_NAME_CHANGE psCopy = psResChangeInfo;
    pMember->ChangeName(psCopy);
}
