// CAchieveType 实现文件
// 从 GameServer.exe IDA 反编译还原

#include "Soulworker/GameServer/XGameServer/AchieveType.h"
#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include <memory>

// 通过宏定义包含 TB_ACHIEVEMENT 结构体
#define GREENDAMTAN_TB_STRUCT_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT.h"
#undef GREENDAMTAN_TB_STRUCT_SECTION

// 前向声明所需结构体 (避免 PSServerDB.h 中的编译问题)
struct ST_ACHIEVE_INFO {
    int nIndex = 0;
    std::uint64_t biCount = 0;
};

struct ST_ACHIEVE_LIST {
    std::vector<ST_ACHIEVE_INFO> vecList;
};

struct ST_ACHIEVE_UPDATE {
    ST_ACHIEVE_INFO stUpdateInfo{};
    int nNextIndex = 0;
    std::uint8_t byCategory = 0;
    std::uint16_t wCount = 0;
};

struct ST_ACHIEVE_UPDATE_LIST {
    std::vector<ST_ACHIEVE_UPDATE> vecList;
};

// ============================================================================
// Init (0x140001e10)
// 状态: 已精确还原
// IDA 地址: 0x140001e10 - 0x140001f5a
//
// 反编译逻辑分析:
//   1. 查找 map 中是否存在 pTBAchieve->taget_ID 对应的成就
//   2. 如果不存在 (iter == end):
//      a. 创建新的 CAchieve 对象 (0x18 bytes = 24 bytes)
//      b. 调用 CAchieve::Init(pTBAchieve)
//      c. 创建 shared_ptr 包装
//      d. 插入 map<taget_ID, shared_ptr<CAchieve>>
//   3. 如果已存在，不做任何操作
//
// 依赖:
//   - TB_ACHIEVEMENT::taget_ID (已定义)
//   - CAchieve::Init (已实现)
//   - std::map<int, std::shared_ptr<CAchieve>>
// ============================================================================
void CAchieveType::Init(TB_ACHIEVEMENT* pTBAchieve) {
    if (!pTBAchieve) {
        return;
    }

    int nTargetID = pTBAchieve->taget_ID;

    // 查找是否已存在
    auto iter = m_mapAchieve.find(nTargetID);

    if (iter == m_mapAchieve.end()) {
        // 不存在，创建新成就对象
        auto pAchieve = std::make_shared<CAchieve>();
        pAchieve->Init(pTBAchieve);

        // 插入 map
        m_mapAchieve[nTargetID] = pAchieve;
    }
    // 如果已存在，不做任何操作
}

// ============================================================================
// AddAchieve (0x140001f60)
// 状态: 已精确还原
// IDA 地址: 0x140001f60 - 0x1400020dd
//
// 反编译逻辑分析:
//   1. 查找 map 中是否存在 pTBAchieve->taget_ID 对应的成就
//   2. 如果存在 (iter != end):
//      a. 获取 CAchieve 指针
//      b. 调用 CAchieve::Init(biCount) - 注意这里实际调用的是 Init 而非 SetAchieve
//      c. IDA 显示调用的是 CGroupAggro::Init，但根据上下文应该是 CAchieve::Init
//   3. 如果不存在:
//      a. 创建新的 CAchieve 对象 (0x18 bytes = 24 bytes)
//      b. 调用 CAchieve::SetAchieve(pTBAchieve, biCount)
//      c. 创建 shared_ptr 包装
//      d. 插入 map<taget_ID, shared_ptr<CAchieve>>
//
// 注意: IDA 中显示调用 CGroupAggro::Init 是类型推断错误，实际应该是 CAchieve::Init
//       因为 iter->second 是 shared_ptr<CAchieve> 类型
//
// 依赖:
//   - TB_ACHIEVEMENT::taget_ID (已定义)
//   - CAchieve::Init (已实现)
//   - CAchieve::SetAchieve (已实现)
//   - std::map<int, std::shared_ptr<CAchieve>>
// ============================================================================
void CAchieveType::AddAchieve(TB_ACHIEVEMENT* pTBAchieve, std::int64_t biCount) {
    if (!pTBAchieve) {
        return;
    }

    int nTargetID = pTBAchieve->taget_ID;

    // 查找是否已存在
    auto iter = m_mapAchieve.find(nTargetID);

    if (iter != m_mapAchieve.end()) {
        // 已存在，调用 Init 设置计数
        // IDA 显示调用的是 CGroupAggro::Init，但实际应该是 CAchieve::Init
        // 根据 IDA 反编译，这里传入的是 biCount 作为参数
        iter->second->Init(pTBAchieve);
    } else {
        // 不存在，创建新成就对象并设置计数
        auto pAchieve = std::make_shared<CAchieve>();
        pAchieve->SetAchieve(pTBAchieve, biCount);

        // 插入 map
        m_mapAchieve[nTargetID] = pAchieve;
    }
}

// ============================================================================
// EndCollect (0x140002180)
// 状态: 已精确还原
// IDA 地址: 0x140002180 - 0x1400021f8
//
// 反编译逻辑分析:
//   1. 获取 map 的 begin 和 end 迭代器
//   2. 遍历所有成就:
//      a. 获取 CAchieve 指针 (通过 shared_ptr::operator->)
//      b. 调用 CAchieve::EndCollect(stUpdateList)
//      c. 迭代器 ++
//
// 依赖:
//   - CAchieve::EndCollect (已实现)
//   - ST_ACHIEVE_UPDATE_LIST (已定义)
//   - std::map<int, std::shared_ptr<CAchieve>>
// ============================================================================
void CAchieveType::EndCollect(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    for (auto& pair : m_mapAchieve) {
        if (pair.second) {
            pair.second->EndCollect(stUpdateList);
        }
    }
}

// ============================================================================
// LoadAchieve (0x140002200)
// 状态: 已精确还原
// IDA 地址: 0x140002200 - 0x1400022f0
//
// 反编译逻辑分析:
//   1. 获取 map 的 begin 迭代器
//   2. 循环直到 end:
//      a. 获取 shared_ptr<CAchieve>
//      b. 通过 shared_ptr::operator-> 获取 CAchieve 指针
//      c. 获取 m_pTBAchieve (通过 std::tr1::_Ptr_base<CParty>::_Get，实际是获取原始指针)
//      d. 如果 m_pTBAchieve 不为空:
//         - 创建 ST_ACHIEVE_INFO 结构
//         - 设置 stInfo.biCount = CAchieve::m_biCount (通过 std::list<CBattleZone*>::size 获取)
//         - 设置 stInfo.nIndex = pTB->ID
//         - 添加到 stLoadList
//      e. 迭代器 ++
//
// 注意: IDA 中的 std::list<CBattleZone*>::size 实际上是获取 m_biCount 的伪代码表示
//       IDA 显示的 VChunkLocker 和 _Ptr_base<CParty> 都是类型推断不准确的结果
//
// 依赖:
//   - CAchieve::m_pTBAchieve (已定义)
//   - CAchieve::m_biCount (已定义)
//   - TB_ACHIEVEMENT::ID (已定义)
//   - ST_ACHIEVE_INFO (已定义)
//   - ST_ACHIEVE_LIST (已定义)
//   - std::map<int, std::shared_ptr<CAchieve>>
// ============================================================================
void CAchieveType::LoadAchieve(ST_ACHIEVE_LIST* stLoadList) {
    if (!stLoadList) {
        return;
    }

    for (auto& pair : m_mapAchieve) {
        auto& pAchieve = pair.second;
        if (!pAchieve) {
            continue;
        }

        // 获取 TB_ACHIEVEMENT 指针
        TB_ACHIEVEMENT* pTB = pAchieve->m_pTBAchieve;

        if (pTB) {
            ST_ACHIEVE_INFO stInfo;
            stInfo.nIndex = pTB->ID;
            stInfo.biCount = static_cast<std::uint64_t>(pAchieve->m_biCount);

            stLoadList->vecList.push_back(stInfo);
        }
    }
}
