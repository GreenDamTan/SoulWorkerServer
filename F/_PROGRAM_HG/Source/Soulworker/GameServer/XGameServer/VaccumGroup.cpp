#include "Soulworker/GameServer/XGameServer/VaccumGroup.h"
#include "Soulworker/GameServer/XGameServer/VaccumManager.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/VaccumCube.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_OBJECT.h"
#include "Soulworker/GameServer/XGameServer/InteractionObject.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

// Prioritize 比较器实现
// IDA: priority_queue 比较器用于按 m_nRandomKey 排序
// priority_queue 是最大堆，返回 true 表示 a 的优先级低于 b（a 排在后面）
bool Prioritize::operator()(const CVaccumCube* a, const CVaccumCube* b) const {
    if (!a || !b) {
        return false;
    }
    // IDA: 比较 m_nRandomKey，较小的 key 优先级较高（排在队列前面）
    // 对于 priority_queue，返回 true 表示 a 应该排在 b 后面
    return a->GetRandomKey() > b->GetRandomKey();
}

// Per IDA 0x1401917d0: CVaccumGroup 构造函数
// IDA 反编译精确逻辑:
// 1. std::priority_queue<CVaccumCube*>::priority_queue() - m_queueNonActiveVaccumCube 默认构造
// 2. std::map<int, CVaccumCube*>::map() - m_mapActiveVaccumCube 默认构造
// 3. m_pVaccumManager = pManager
// 4. m_dwNextSpawnTime = 0
// 5. m_bAutoSpawn = bAutoSpawn
CVaccumGroup::CVaccumGroup(CVaccumManager* pManager, bool bAutoSpawn)
    : m_pVaccumManager(pManager)
    , m_pTBInteraction(nullptr)
    , m_dwNextSpawnTime(0)
    , m_bAutoSpawn(bAutoSpawn)
{
    // IDA: 所有 std::map/std::priority_queue 成员由编译器自动调用默认构造函数
}

// CVaccumGroup 析构函数
CVaccumGroup::~CVaccumGroup() {
    // 标准C++析构函数会自动逆序调用成员析构函数
}

// Per IDA 0x140191840: CVaccumGroup::AddVaccumCube
// IDA 反编译精确逻辑:
// 1. if (!m_pVaccumManager) return false
// 2. if (!CVaccumManager::GetArea(m_pVaccumManager)) return false
// 3. 获取位置 vecPos (通过 BattleZone 虚函数调用)
// 4. m_pTBInteraction = XResourceMgr::GetTB_INTERACTION_OBJECT(pInfo->m_iInteractionID)
// 5. if (!m_pTBInteraction) return false
// 6. pVaccum = ThreadLocalData::CreateVaccumCubeObject(vecPos)
// 7. if (pVaccum):
//    - CVaccumCube::Init(pVaccum, uxActor, pInfo, vecPos, random, ...)
//    - m_queueNonActiveVaccumCube.push(pVaccum)
//    - if (m_bAutoSpawn && nMaxCount > nActiveCount):
//      - m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + GetTickCount64()
//    - return true
// 8. else: LogError, return false
bool CVaccumGroup::AddVaccumCube(UXActorID uxActor, VInterActionBoxInfo* pInfo) {
    if (!m_pVaccumManager) {
        return false;
    }

    CBattleZone* pArea = m_pVaccumManager->GetArea();
    if (!pArea) {
        return false;
    }

    XVec3 vecPos = {};
    if (pInfo) {
        vecPos.x = (pInfo->PosTopLeft.x + pInfo->PosBottomRight.x) / 2.0f;
        vecPos.y = (pInfo->PosTopLeft.y + pInfo->PosBottomRight.y) / 2.0f;
        vecPos.z = pInfo->PosTopLeft.z;
    }

    // IDA: v4 = TXSingleton<XGameServer>::Instance();
    // this->m_pTBInteraction = XResourceMgr::GetTB_INTERACTION_OBJECT(&v4->m_xResourceMgr, pInfo->m_iInteractionID);
    XGameServer* pGameServer = XGameServer::Instance();
    m_pTBInteraction = pGameServer->GetResourceMgr().GetTB_INTERACTION_OBJECT(pInfo->GetInteractionID());

    if (!m_pTBInteraction) {
        return false;
    }

    ThreadLocalData* pThreadLocal = ThreadLocalData::GetInstance();
    CVaccumCube* pVaccum = pThreadLocal ? pThreadLocal->CreateVaccumCubeObject(vecPos) : nullptr;

    if (pVaccum) {
        // IDA: dwTablePickupTime = this->m_pTBInteraction->Act_Delay_time;
        std::uint64_t dwTablePickupTime = m_pTBInteraction->Act_Delay_time;

        // IDA: nRandom = CVaccumGroup::GetRandomValue(this);
        int nRandom = GetRandomValue();

        // IDA: CVaccumCube::Init(pVaccum, uxActor, pInfo, &vecPos, nRandom, m_pTBInteraction->Add_Random_Item_ID, dwTablePickupTime);
        pVaccum->Init(uxActor, pInfo, vecPos, nRandom, m_pTBInteraction->Add_Random_Item_ID, dwTablePickupTime);

        // IDA: std::priority_queue<CVaccumCube *,std::vector<CVaccumCube *>,Prioritize>::push(&this->m_queueNonActiveVaccumCube, &pVaccum);
        m_queueNonActiveVaccumCube.push(pVaccum);

        // IDA: nMaxCount = this->m_pTBInteraction->Respawn_Value_Max;
        // nActiveCount = std::_Tree<...>::size((std::_Tree<...> *)&this->m_mapActiveVaccumCube);
        int nMaxCount = m_pTBInteraction->Respawn_Value_Max;
        int nActiveCount = static_cast<int>(m_mapActiveVaccumCube.size());

        // IDA: this->m_dwNextSpawnTime = 0;
        // if ( this->m_bAutoSpawn && nMaxCount > nActiveCount )
        //   this->m_dwNextSpawnTime = this->m_pTBInteraction->Respawn_Delay_Time + GetTickCount64();
        m_dwNextSpawnTime = 0;
        if (m_bAutoSpawn && nMaxCount > nActiveCount) {
#ifdef _WIN32
            m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + GetTickCount64();
#else
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + (std::uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
        }

        return true;
    }
    else {
        // IDA: LogHelper::LogError("game.contents", "<VACCUM> Failed Create Vaccum %d!", pInfo->iID);
        GreenDamTan_log(__FILE__, __FUNCTION__, "<VACCUM> Failed Create Vaccum %d!", pInfo->iID);
        return false;
    }
}

// Per IDA 0x140191a90: CVaccumGroup::GetRandomValue
// IDA 反编译精确逻辑:
// v1 = TXSingleton<XWorldManager>::Instance()
// return XWorldManager::nRand(v1, 1, 10000)
int CVaccumGroup::GetRandomValue() {
    // IDA: 返回 1-10000 范围内的随机值
    XWorldManager* pWorldMgr = XWorldManager::Instance();
    return pWorldMgr->nRand(1, 10000);
}

// Per IDA 0x140191ac0: CVaccumGroup::Update
// IDA 反编译精确逻辑:
// 1. if (!m_pVaccumManager || !m_pTBInteraction || !CVaccumManager::GetArea(m_pVaccumManager)) return
// 2. 遍历 m_mapActiveVaccumCube:
//    - if (pVaccumCube && CVaccumCube::IsPickup(pVaccumCube)):
//      - CVaccumCube::Pickup(pVaccumCube)
//      - if (!CVaccumCube::GetCount(pVaccumCube)):
//        - 移除对象，添加到 m_queueNonActiveVaccumCube
// 3. if (m_dwNextSpawnTime && GetTickCount64() >= m_dwNextSpawnTime && nMaxCount > nActiveCount):
//    - 从 m_queueNonActiveVaccumCube 取出对象
//    - CVaccumCube::Spawn(pVaccumCube, nCount)
//    - 添加到 m_mapActiveVaccumCube
//    - m_dwNextSpawnTime = 0 或重新计算
void CVaccumGroup::Update() {
    if (!m_pVaccumManager || !m_pTBInteraction) {
        return;
    }

    CBattleZone* pArea = m_pVaccumManager->GetArea();
    if (!pArea) {
        return;
    }

    // IDA: 遍历 m_mapActiveVaccumCube，处理已拾取的真空立方体
    int nCountTemp = 0;
    auto it = m_mapActiveVaccumCube.begin();
    while (it != m_mapActiveVaccumCube.end()) {
        CVaccumCube* pVaccumCube = it->second;

        if (pVaccumCube && pVaccumCube->IsPickup()) {
            pVaccumCube->Pickup();

            if (pVaccumCube->GetCount() <= 0) {
                // IDA: 移动到非激活队列
                // CVaccumCube::SetRandomKey(pVaccumCube, RandomValue);
                int nRandomValue = GetRandomValue();
                pVaccumCube->SetRandomKey(nRandomValue);

                // IDA: if ( this->m_bAutoSpawn && !this->m_dwNextSpawnTime )
                //   this->m_dwNextSpawnTime = this->m_pTBInteraction->Respawn_Delay_Time + GetTickCount64();
                if (m_bAutoSpawn && !m_dwNextSpawnTime) {
#ifdef _WIN32
                    m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + GetTickCount64();
#else
                    struct timespec ts;
                    clock_gettime(CLOCK_MONOTONIC, &ts);
                    m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + (std::uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
                }

                // IDA: std::priority_queue::push(&this->m_queueNonActiveVaccumCube, &pVaccumCube);
                m_queueNonActiveVaccumCube.push(pVaccumCube);

                // IDA: erase from m_mapActiveVaccumCube
                it = m_mapActiveVaccumCube.erase(it);
                ++nCountTemp;
                continue;
            }
        }

        ++it;
        ++nCountTemp;
    }

    // IDA: 检查是否需要生成新的真空立方体
    int nMaxCount = m_pTBInteraction->Respawn_Value_Max;
    int nActiveCount = static_cast<int>(m_mapActiveVaccumCube.size());

    if (m_dwNextSpawnTime) {
#ifdef _WIN32
        std::uint64_t dwCurrentTime = GetTickCount64();
#else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        std::uint64_t dwCurrentTime = (std::uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif

        if (dwCurrentTime >= m_dwNextSpawnTime && nMaxCount > nActiveCount) {
            if (m_queueNonActiveVaccumCube.empty()) {
                m_dwNextSpawnTime = 0;
            }
            else {
                // IDA: v10 = *std::priority_queue::top(&this->m_queueNonActiveVaccumCube);
                CVaccumCube* pVaccumCube = m_queueNonActiveVaccumCube.top();
                m_queueNonActiveVaccumCube.pop();

                if (pVaccumCube) {
                    // IDA: nMax = this->m_pTBInteraction->Interaction_Count_Max;
                    // nMin = this->m_pTBInteraction->Interaction_Count_Min;
                    // v3 = TXSingleton<XWorldManager>::Instance();
                    // nCount = XWorldManager::nRand(v3, nMin, nMax);
                    int nMax = m_pTBInteraction->Interaction_Count_Max;
                    int nMin = m_pTBInteraction->Interaction_Count_Min;
                    XWorldManager* pWorldMgr = XWorldManager::Instance();
                    int nCount = pWorldMgr->nRand(nMin, nMax);

                    // IDA: CVaccumCube::Spawn(v11, nCount);
                    pVaccumCube->Spawn(nCount);

                    // IDA: _Val1 = v26->GetID(&v11->XActor);
                    // insert into m_mapActiveVaccumCube
                    int nID = static_cast<int>(pVaccumCube->GetID());
                    m_mapActiveVaccumCube[nID] = pVaccumCube;

                    m_dwNextSpawnTime = 0;

                    // IDA: if ( this->m_bAutoSpawn && nMaxCount > nActiveCount + 1 )
                    //   this->m_dwNextSpawnTime = this->m_pTBInteraction->Respawn_Delay_Time + GetTickCount64();
                    if (m_bAutoSpawn && nMaxCount > nActiveCount + 1) {
#ifdef _WIN32
                        m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + GetTickCount64();
#else
                        clock_gettime(CLOCK_MONOTONIC, &ts);
                        m_dwNextSpawnTime = m_pTBInteraction->Respawn_Delay_Time + (std::uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
                    }
                }
            }
        }
    }
}

// Per IDA 0x140191f60: CVaccumGroup::Click
// IDA 反编译精确逻辑:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return 55800
// 3. 查找 m_mapActiveVaccumCube.find(nID)
// 4. if (!found) return 55800
// 5. pVaccum = it->second
// 6. if (!pVaccum || CVaccumCube::IsLock(pVaccum)) return 55801
// 7. CVaccumCube::TakeVaccum(pVaccum, pActor)
// 8. CMover::GetGOC<CGocEntity>(pUser, &pEntity, 0)
// 9. if (pEntity): CGocEntity::SetVaccumCubeID(pEntity, nID)
// 10. return 0
unsigned int CVaccumGroup::Click(int nID, XActor* pActor) {
    // IDA: pUser = (CUser *)_RTDynamicCast_0(pActor, 0, &XActor `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) {
        return 55800;  // IDA: 默认错误码
    }

    // IDA: std::_Tree<...>::find((std::_Tree<...> *)&this->m_mapActiveVaccumCube, ... &nIDa);
    auto it = m_mapActiveVaccumCube.find(nID);
    if (it == m_mapActiveVaccumCube.end()) {
        return 55800;  // IDA: 未找到
    }

    // IDA: pVaccum = (CVaccumCube *)std::_Tree_iterator<...>::operator->(...)->second.__vftable;
    CVaccumCube* pVaccum = it->second;

    // IDA: if ( !pVaccum || CVaccumCube::IsLock(pVaccum) ) return 55801;
    if (!pVaccum || pVaccum->IsLock()) {
        return 55801;  // IDA: 已锁定
    }

    // IDA: CVaccumCube::TakeVaccum(pVaccum, pActora);
    pVaccum->TakeVaccum(pActor);

    // IDA: CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0);
    // if ( (unsigned int)std::tr1::shared_ptr<...>::operator int std::_Bool_struct::*(...) != -1 )
    // {
    //   v5 = (CGocEntity *)std::tr1::shared_ptr<...>::operator->(...);
    //   CGocEntity::SetVaccumCubeID(v5, nIDa);
    // }
    CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
    if (pEntity) {
        pEntity->SetVaccumCubeID(nID);
    }

    return 0;  // IDA: v11 = 0; return v11;
}

// Per IDA 0x1401920b0: CVaccumGroup::CancelClick
// IDA 反编译精确逻辑:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return 55800
// 3. 查找 m_mapActiveVaccumCube.find(nID)
// 4. if (!found) return 55800
// 5. pVaccum = it->second
// 6. if (!pVaccum) return 55800
// 7. if (!CVaccumCube::IsLock(pVaccum) || !CVaccumCube::IsTakeUser(pVaccum, pActor)) return 55802
// 8. CVaccumCube::ClearTakeVaccum(pVaccum)
// 9. CMover::GetGOC<CGocEntity>(pUser, &pEntity, 0)
// 10. if (pEntity): CGocEntity::SetVaccumCubeID(pEntity, 0)
// 11. return 0
unsigned int CVaccumGroup::CancelClick(int nID, XActor* pActor) {
    // IDA: pUser = (CUser *)_RTDynamicCast_0(pActor, 0, &XActor `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) {
        return 55800;  // IDA: 默认错误码
    }

    // IDA: std::_Tree<...>::find((std::_Tree<...> *)&this->m_mapActiveVaccumCube, ... &nIDa);
    auto it = m_mapActiveVaccumCube.find(nID);
    if (it == m_mapActiveVaccumCube.end()) {
        return 55800;  // IDA: 未找到
    }

    // IDA: pVaccum = (CVaccumCube *)std::_Tree_iterator<...>::operator->(...)->second.__vftable;
    CVaccumCube* pVaccum = it->second;

    // IDA: if ( !pVaccum ) return 55800;
    if (!pVaccum) {
        return 55800;
    }

    // IDA: if ( !CVaccumCube::IsLock(pVaccum) || !CVaccumCube::IsTakeUser(pVaccum, pActora) ) return 55802;
    if (!pVaccum->IsLock() || !pVaccum->IsTakeUser(pActor)) {
        return 55802;  // IDA: 非占用者或未锁定
    }

    // IDA: CVaccumCube::ClearTakeVaccum(pVaccum);
    pVaccum->ClearTakeVaccum();

    // IDA: CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0);
    // if ( (unsigned int)std::tr1::shared_ptr<...>::operator int std::_Bool_struct::*(...) != -1 )
    // {
    //   v5 = (CGocEntity *)std::tr1::shared_ptr<...>::operator->(...);
    //   CGocEntity::SetVaccumCubeID(v5, 0);
    // }
    CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
    if (pEntity) {
        pEntity->SetVaccumCubeID(0);
    }

    return 0;  // IDA: v11 = 0; return v11;
}

// Per IDA 0x140192f10: CVaccumGroup::ActiveVaccumCube
// IDA 反编译精确逻辑:
// 激活真空立方体
void CVaccumGroup::ActiveVaccumCube() {
    // IDA: if ( this->m_pTBInteraction && this->m_pVaccumManager && CVaccumManager::GetArea((CAi *)this->m_pVaccumManager) )
    if (!m_pTBInteraction || !m_pVaccumManager) {
        return;
    }

    CBattleZone* pArea = m_pVaccumManager->GetArea();
    if (!pArea) {
        return;
    }

    // IDA: pVaccumCube = *std::priority_queue::top(&this->m_queueNonActiveVaccumCube);
    if (m_queueNonActiveVaccumCube.empty()) {
        return;
    }

    CVaccumCube* pVaccumCube = m_queueNonActiveVaccumCube.top();
    if (!pVaccumCube) {
        return;
    }

    // IDA: std::priority_queue::pop(&this->m_queueNonActiveVaccumCube);
    m_queueNonActiveVaccumCube.pop();

    // IDA: nMax = this->m_pTBInteraction->Interaction_Count_Max;
    // nMin = this->m_pTBInteraction->Interaction_Count_Min;
    // v1 = TXSingleton<XWorldManager>::Instance();
    // nCount = XWorldManager::nRand(v1, nMin, nMax);
    int nMax = m_pTBInteraction->Interaction_Count_Max;
    int nMin = m_pTBInteraction->Interaction_Count_Min;
    XWorldManager* pWorldMgr = XWorldManager::Instance();
    int nCount = pWorldMgr->nRand(nMin, nMax);

    // IDA: CVaccumCube::Spawn(pVaccumCube, nCount);
    pVaccumCube->Spawn(nCount);

    // IDA: _Val1 = v12->GetID(&pVaccumCube->XActor);
    // insert into m_mapActiveVaccumCube
    int nID = static_cast<int>(pVaccumCube->GetID());
    m_mapActiveVaccumCube[nID] = pVaccumCube;
}

int CVaccumGroup::GetFirstVaccumCubeID() const {
    auto it = m_mapActiveVaccumCube.begin();
    if (it == m_mapActiveVaccumCube.end() || !it->second) {
        return 0;
    }

    return static_cast<int>(it->second->GetID());
}
