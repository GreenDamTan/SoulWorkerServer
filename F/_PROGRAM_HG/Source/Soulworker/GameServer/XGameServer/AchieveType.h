// CAchieveType - 成就类型管理类
// 从 GameServer.exe IDA 反编译还原
// 地址范围: 0x140001e10 - 0x1400022f0

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

// 前置声明
struct TB_ACHIEVEMENT;
struct ST_ACHIEVE_INFO;
struct ST_ACHIEVE_UPDATE_LIST;
struct ST_ACHIEVE_LIST;
class CAchieve;

// CAchieveType - 管理一组成就的集合
// 负责成就的初始化、添加、收集结束和数据加载
// 内部使用 std::map<int, std::tr1::shared_ptr<CAchieve>> 存储成就
class CAchieveType {
public:
    // 构造函数
    CAchieveType() = default;

    // 析构函数
    ~CAchieveType() = default;

    // Init (0x140001e10)
    // 初始化成就 - 如果指定 target_ID 的成就不存在，则创建新成就并初始化
    // 参数: pTBAchieve - 成就表数据指针
    // 逻辑: 查找 map 中是否存在 pTBAchieve->taget_ID 对应的成就
    //       如果不存在，创建新 CAchieve 对象，调用 CAchieve::Init，并插入 map
    void Init(TB_ACHIEVEMENT* pTBAchieve);

    // AddAchieve (0x140001f60)
    // 添加成就 - 根据计数设置成就状态
    // 参数: pTBAchieve - 成就表数据指针
    //       biCount - 成就计数
    // 逻辑: 查找 map 中是否存在 pTBAchieve->taget_ID 对应的成就
    //       如果存在，调用 CAchieve::Init 设置计数
    //       如果不存在，创建新 CAchieve 对象，调用 CAchieve::SetAchieve，并插入 map
    void AddAchieve(TB_ACHIEVEMENT* pTBAchieve, std::int64_t biCount);

    // EndCollect (0x140002180)
    // 结束收集 - 遍历所有成就并结束收集状态
    // 参数: stUpdateList - 成就更新列表，用于收集需要更新的成就信息
    // 逻辑: 遍历 map 中所有 CAchieve，调用 CAchieve::EndCollect
    void EndCollect(ST_ACHIEVE_UPDATE_LIST* stUpdateList);

    // LoadAchieve (0x140002200)
    // 加载成就数据 - 将所有成就信息导出到列表
    // 参数: stLoadList - 成就信息列表，用于存储导出的成就数据
    // 逻辑: 遍历 map 中所有 CAchieve，将成就信息添加到 stLoadList
    void LoadAchieve(ST_ACHIEVE_LIST* stLoadList);

    // 成员变量
    // 使用 std::tr1::shared_ptr 管理成就对象 (原始代码使用 std::tr1::shared_ptr)
    // 键: taget_ID (成就目标ID)
    // 值: CAchieve 智能指针
    std::map<int, std::shared_ptr<CAchieve>> m_mapAchieve;
};
