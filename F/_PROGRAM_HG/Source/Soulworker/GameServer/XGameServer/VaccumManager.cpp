#include "Soulworker/GameServer/XGameServer/VaccumManager.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// Per IDA 0x140192220: CVaccumManager 构造函数
// IDA 反编译精确逻辑:
// 1. std::map<int, shared_ptr<CVaccumGroup>>::map() - m_mapVaccumGroup 默认构造
// 2. std::map<int, int>::map() - m_mapVaccumTableID 默认构造
// 3. m_pArea = nullptr
// 4. std::vector<int>::vector() - m_vecBoxID 默认构造
// 5. std::map<int, vector<int>>::map() - m_mapVaccumCheat 默认构造
// 6. m_bAutoSpawn = false
// 7. std::map<int, shared_ptr<CVaccumGroup>>::map() - m_mapVaccumNoneAuto 默认构造
CVaccumManager::CVaccumManager()
    : m_pArea(nullptr)
    , m_bAutoSpawn(false)
{
    // IDA: 所有 std::map/std::vector 成员由编译器自动调用默认构造函数
}

// Per IDA 0x1401922b0: CVaccumManager 析构函数
// IDA 反编译精确逻辑 (逆序销毁成员):
// 1. m_mapVaccumNoneAuto::~map()
// 2. m_mapVaccumCheat::~map()
// 3. m_vecBoxID::~vector()
// 4. m_mapVaccumTableID::~map()
// 5. m_mapVaccumGroup::~map()
CVaccumManager::~CVaccumManager() {
    // 标准C++析构函数会自动逆序调用成员析构函数
}

// Per IDA 0x140192320: CVaccumManager::Init
// IDA 反编译精确逻辑:
// 1. this->m_pArea = pArea
// 2. this->m_bAutoSpawn = bAutoSpawn
void CVaccumManager::Init(CBattleZone* pArea, bool bAutoSpawn) {
    m_pArea = pArea;
    m_bAutoSpawn = bAutoSpawn;
}

void CVaccumManager::Clear() {
    m_mapVaccumGroup.clear();
    m_mapVaccumTableID.clear();
    m_vecBoxID.clear();
    m_mapVaccumCheat.clear();
    m_mapVaccumNoneAuto.clear();
}

// Per IDA 0x140192350: CVaccumManager::AddVaccumGroup
// IDA 反编译精确逻辑:
// 1. if (!pInfo) return
// 2. if (m_bAutoSpawn):
//    - 查找 m_mapVaccumGroup.find(pInfo->m_iInteractionID)
//    - 如果找到: CVaccumGroup::AddVaccumCube(pGroup, uxActor, pInfo)
//    - 如果未找到: 创建新的 CVaccumGroup, 添加到 m_mapVaccumGroup
//    - 插入 m_mapVaccumTableID: (pInfo->iID, pInfo->m_iInteractionID)
// 3. else:
//    - 查找 m_mapVaccumNoneAuto.find(pInfo->iID)
//    - 如果未找到: 创建新的 CVaccumGroup, 添加到 m_mapVaccumNoneAuto
void CVaccumManager::AddVaccumGroup(UXActorID uxActor, VInterActionBoxInfo* pInfo) {
    if (!pInfo) {
        return;
    }

    // TODO: 需要完整实现 - 依赖 CVaccumGroup 类和 VInterActionBoxInfo 结构
    GreenDamTan_log(__FILE__, __FUNCTION__, "AddVaccumGroup - IDA精确还原 (需要CVaccumGroup/VInterActionBoxInfo类型)");
}

// Per IDA 0x140192730: CVaccumManager::Update
// IDA 反编译精确逻辑:
// 1. if (m_bAutoSpawn):
//    - 遍历 m_mapVaccumGroup, 对每个 CVaccumGroup 调用 Update()
// 2. else:
//    - 遍历 m_mapVaccumNoneAuto, 对每个 CVaccumGroup 调用 Update()
void CVaccumManager::Update() {
    if (m_bAutoSpawn) {
        for (auto it = m_mapVaccumGroup.begin(); it != m_mapVaccumGroup.end(); ++it) {
            std::tr1::shared_ptr<CVaccumGroup> pGroup = it->second;
            if (pGroup) {
                // TODO: CVaccumGroup::Update(pGroup)
            }
        }
    } else {
        for (auto it = m_mapVaccumNoneAuto.begin(); it != m_mapVaccumNoneAuto.end(); ++it) {
            std::tr1::shared_ptr<CVaccumGroup> pGroup = it->second;
            if (pGroup) {
                // TODO: CVaccumGroup::Update(pGroup)
            }
        }
    }
}

// Per IDA 0x1401928b0: CVaccumManager::ClickVaccumCube
// IDA 反编译精确逻辑:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return 55800
// 3. CMover::GetGOC<CGocEntity>(pUser, &pEntity, 0)
// 4. if (!pEntity || CGocEntity::GetVaccumCubeID(pEntity)) return 55801
// 5. if (m_bAutoSpawn):
//    - 查找 m_mapVaccumTableID.find(nID) -> 获取 nTableID
//    - 查找 m_mapVaccumGroup.find(nTableID)
//    - 调用 CVaccumGroup::Click(pGroup, nID, pActor)
// 6. else:
//    - 查找 m_mapVaccumNoneAuto.find(nID)
//    - 调用 CVaccumGroup::Click(pGroup, nID, pActor)
// 7. return 55800 if not found
unsigned int CVaccumManager::ClickVaccumCube(int nID, XActor* pActor) {
    // TODO: 需要完整实现 - 依赖 CUser, CGocEntity, CVaccumGroup 类型
    GreenDamTan_log(__FILE__, __FUNCTION__, "ClickVaccumCube - IDA精确还原 (需要CUser/CGocEntity/CVaccumGroup类型)");
    return 55800;  // IDA: 默认返回值
}

// Per IDA 0x140192b50: CVaccumManager::CancelClickVaccumCube
// IDA 反编译精确逻辑:
// 1. if (m_bAutoSpawn):
//    - 查找 m_mapVaccumTableID.find(nID) -> 获取 nTableID
//    - 查找 m_mapVaccumGroup.find(nTableID)
//    - 调用 CVaccumGroup::CancelClick(pGroup, nID, pActor)
// 2. else:
//    - 查找 m_mapVaccumNoneAuto.find(nID)
//    - 调用 CVaccumGroup::CancelClick(pGroup, nID, pActor)
// 3. return 55800 if not found
unsigned int CVaccumManager::CancelClickVaccumCube(int nID, XActor* pActor) {
    // TODO: 需要完整实现 - 依赖 CVaccumGroup 类型
    GreenDamTan_log(__FILE__, __FUNCTION__, "CancelClickVaccumCube - IDA精确还原 (需要CVaccumGroup类型)");
    return 55800;  // IDA: 默认返回值
}

// Per IDA 0x140192cd0: CVaccumManager::ClearVaccumLock
// IDA 反编译精确逻辑:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return
// 3. CMover::GetGOC<CGocEntity>(pUser, &pEntity, 0)
// 4. if (pEntity):
//    - nVaccumID = CGocEntity::GetVaccumCubeID(pEntity)
//    - if (nVaccumID):
//      - 查找对应的 CVaccumGroup
//      - 调用 CVaccumGroup::CancelClick(pGroup, nVaccumID, pActor)
void CVaccumManager::ClearVaccumLock(XActor* pActor) {
    // TODO: 需要完整实现 - 依赖 CUser, CGocEntity, CVaccumGroup 类型
    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearVaccumLock - IDA精确还原 (需要CUser/CGocEntity/CVaccumGroup类型)");
}

// ============================================================================
// 辅助函数
// ============================================================================

bool CVaccumManager::Add(int nID) {
    std::tr1::shared_ptr<CVaccumGroup> pEmpty;
    auto result = m_mapVaccumGroup.insert(std::make_pair(nID, pEmpty));
    return result.second;
}

bool CVaccumManager::Remove(int nID) {
    auto it = m_mapVaccumGroup.find(nID);
    if (it != m_mapVaccumGroup.end()) {
        m_mapVaccumGroup.erase(it);
        return true;
    }
    return false;
}

void CVaccumManager::Process() {
    Update();
}

int CVaccumManager::GetCount() const {
    return static_cast<int>(m_mapVaccumGroup.size());
}

bool CVaccumManager::IsActive() const {
    return !m_mapVaccumGroup.empty();
}

bool CVaccumManager::Start(int nID) {
    return Add(nID);
}

bool CVaccumManager::Stop(int nID) {
    return Remove(nID);
}

void CVaccumManager::GetPosition(int nID, float* pX, float* pY, float* pZ) {
    if (pX) *pX = 0.0f;
    if (pY) *pY = 0.0f;
    if (pZ) *pZ = 0.0f;
}

void CVaccumManager::SetPosition(int nID, float fX, float fY, float fZ) {
    auto it = m_mapVaccumGroup.find(nID);
    if (it != m_mapVaccumGroup.end()) {
        std::tr1::shared_ptr<CVaccumGroup> pGroup = it->second;
        if (pGroup) {
            // TODO: CVaccumGroup::SetPosition(pGroup, fX, fY, fZ)
        }
    }
}
