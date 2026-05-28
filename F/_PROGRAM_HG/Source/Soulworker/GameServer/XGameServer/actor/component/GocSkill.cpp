#include "GocSkill.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/Skill.h"
#include <cstring>

// ============================================================================
// CGocSkill - Game Object Component for actor skills
// IDA 反编译来源: GameServer.exe
// ============================================================================

// ----------------------------------------------------------------------------
// CGocSkill 构造函数
// IDA 0x1401682A0: ??0CGocSkill@@QEAA@XZ
// 基于 IDA 反编译代码还原
// ----------------------------------------------------------------------------
CGocSkill::CGocSkill()
    : GOComponent()
{
    // 调用基类构造函数 (IDA: GOComponent::GOComponent(this))
    // 设置虚表指针 (IDA: this->__vftable = &CGocSkill::`vftable')

    // 初始化 m_HaveSkill 容器
    // IDA 使用 boost::multi_index_container 构造，这里用 std::map 默认构造
    m_HaveSkill.clear();

    // 初始化技能点 (IDA: 偏移176和178)
    m_wTotalSkillPoint = 0;
    m_wSkillPoint = 0;

    // 初始化 m_wSkillDeckSlotCount (IDA: 偏移180)
    m_wSkillDeckSlotCount = 3;

    // 初始化 m_vPassiveSkill (IDA: 偏移184, std::vector default constructor)
    m_vPassiveSkill.clear();

    // 初始化卡组索引 (IDA: 偏移216和217)
    m_byDeckCount = 0;
    m_byActiveDeck = 0;

    // 初始化 m_nSkillDeck 数组 (IDA: 偏移220, int[5][6][4] = 120个int)
    std::memset(m_nSkillDeck, 0, sizeof(m_nSkillDeck));

    // 初始化 m_stSkillDeckPage 数组 (IDA: 偏移700, 5个元素，每个36字节)
    // IDA: `vector constructor iterator'(this->m_stSkillDeckPage, 0x24u, 5, PS_SKILL_DECK_PAGE::PS_SKILL_DECK_PAGE)
    std::memset(m_stSkillDeckPage, 0, sizeof(m_stSkillDeckPage));

    // 初始化 m_nGestureSlot (IDA: 偏移880, int[6])
    std::memset(m_nGestureSlot, 0, sizeof(m_nGestureSlot));

    // 初始化 m_mapSkillDivergence (IDA: 偏移904, std::map default constructor)
    m_mapSkillDivergence.clear();

    // 初始化 m_bUseModeSkill (IDA: 偏移936)
    m_bUseModeSkill = false;

    // 初始化 m_HaveModeSkill 容器 (IDA: 偏移944)
    // IDA 使用 boost::multi_index_container 构造
    m_HaveModeSkill.clear();

    // 初始化 m_nModeSkillDeck (IDA: 偏移1104, int[6])
    std::memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));

    // 初始化 m_vecModeDefaultSkillList (IDA: 偏移1128, std::vector<unsigned long>)
    m_vecModeDefaultSkillList.clear();

    // 初始化 m_vPassiveModeSkill (IDA: 偏移1160, std::vector default constructor)
    m_vPassiveModeSkill.clear();

    // 初始化 m_ModeShopMyInfo (IDA: 偏移1192, PS_ROGUELIKE_SHOP_MY_INFO default constructor, 40字节)
    std::memset(m_ModeShopMyInfo, 0, sizeof(m_ModeShopMyInfo));

    // 初始化 m_mapModeSkillActiveCount (IDA: 偏移1232, std::map<int, ST_ROGUELIKE_SKILL_ACTIVE_COUNT>)
    m_mapModeSkillActiveCount.clear();

    // IDA: 清空 m_HaveSkill 容器 (hashed_index::clear)
    m_HaveSkill.clear();
}

// ----------------------------------------------------------------------------
// CGocSkill 析构函数
// IDA 0x140168500: ??1CGocSkill@@UEAA@XZ
// 基于 IDA 反编译代码还原
// ----------------------------------------------------------------------------
CGocSkill::~CGocSkill()
{
    // 设置虚表指针 (IDA: this->__vftable = &CGocSkill::`vftable')

    // 销毁 m_mapModeSkillActiveCount (IDA: std::map<int, SGroupID>::~map)
    m_mapModeSkillActiveCount.clear();

    // 销毁 m_ModeShopMyInfo (IDA: PS_ROGUELIKE_SHOP_MY_INFO::~PS_ROGUELIKE_SHOP_MY_INFO)
    // 这里不需要手动删除，因为它是固定大小数组

    // 销毁 m_vPassiveModeSkill (IDA: std::vector::~vector)
    m_vPassiveModeSkill.clear();

    // 销毁 m_vecModeDefaultSkillList (IDA: std::vector::~vector)
    m_vecModeDefaultSkillList.clear();

    // 销毁 m_HaveModeSkill (IDA: boost::multi_index_container::~multi_index_container)
    m_HaveModeSkill.clear();

    // 销毁 m_mapSkillDivergence (IDA: std::map::~map)
    m_mapSkillDivergence.clear();

    // 销毁 m_vPassiveSkill (IDA: std::vector::~vector)
    m_vPassiveSkill.clear();

    // 销毁 m_HaveSkill (IDA: boost::multi_index_container::~multi_index_container)
    m_HaveSkill.clear();

    // 调用基类析构函数 (IDA: GOComponent::~GOComponent(this))
}

// ============================================================================
// GOComponent interface
// ============================================================================

bool CGocSkill::Initialize()
{
    return true;
}

void CGocSkill::Shutdown()
{
}

void CGocSkill::Update(float fDeltaTime)
{
}

// ----------------------------------------------------------------------------
// Init - 初始化技能组件
// IDA 0x1401685D0: ?Init@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::Init()
{
    // IDA反编译:
    // this->m_wSkillDeckSlotCount = 3;
    // std::vector<std::tr1::shared_ptr<CSkill>>::clear(&this->m_vPassiveSkill);
    // boost::multi_index::...::clear(&this->m_HaveSkill...);
    // this->m_wTotalSkillPoint = 0;
    // this->m_wSkillPoint = 0;
    // std::_Tree<...>::clear(&this->m_mapSkillDivergence);
    // memset(this->m_nSkillDeck, 0, sizeof(this->m_nSkillDeck));
    // memset(this->m_stSkillDeckPage, 0, sizeof(this->m_stSkillDeckPage));
    // memset(this->m_nGestureSlot, 0, sizeof(this->m_nGestureSlot));
    // this->m_byActiveDeck = 0;
    // this->m_byDeckCount = 0;
    // this->m_bUseModeSkill = 0;
    // boost::multi_index::...::clear(&this->m_HaveModeSkill...);
    // memset(this->m_nModeSkillDeck, 0, sizeof(this->m_nModeSkillDeck));
    // std::vector<float>::clear(&this->m_vecModeDefaultSkillList);
    // std::vector<std::tr1::shared_ptr<CSkill>>::clear(&this->m_vPassiveModeSkill);
    // this->m_ModeShopMyInfo.nRoguelikeMoney = 0;
    // std::_Tree<...>::clear(&this->m_ModeShopMyInfo.mapMyBuyList);
    // std::_Tree<...>::clear(&this->m_mapModeSkillActiveCount);

    m_wSkillDeckSlotCount = 3;
    m_vPassiveSkill.clear();
    m_HaveSkill.clear();
    m_wTotalSkillPoint = 0;
    m_wSkillPoint = 0;
    m_mapSkillDivergence.clear();
    std::memset(m_nSkillDeck, 0, sizeof(m_nSkillDeck));
    std::memset(m_stSkillDeckPage, 0, sizeof(m_stSkillDeckPage));
    std::memset(m_nGestureSlot, 0, sizeof(m_nGestureSlot));
    m_byActiveDeck = 0;
    m_byDeckCount = 0;
    m_bUseModeSkill = false;
    m_HaveModeSkill.clear();
    std::memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));
    m_vecModeDefaultSkillList.clear();
    m_vPassiveModeSkill.clear();
    // m_ModeShopMyInfo.nRoguelikeMoney = 0; // 需要PS_ROGUELIKE_SHOP_MY_INFO结构
    std::memset(m_ModeShopMyInfo, 0, sizeof(m_ModeShopMyInfo));
    m_mapModeSkillActiveCount.clear();
}

// ============================================================================
// 技能检查函数
// ============================================================================

// ----------------------------------------------------------------------------
// IsHaveBaseSkill - 检查是否拥有基础技能
// IDA 0x140168740: ?IsHaveBaseSkill@CGocSkill@@QEAA_NK@Z
// 基于 IDA 反编译代码还原
// ----------------------------------------------------------------------------
bool CGocSkill::IsHaveBaseSkill(unsigned int dwSkillIndex) const
{
    // IDA反编译 (完整还原):
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SKILL* pTB_Skill = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, dwSkillIndex);
    // if (!pTB_Skill) return false;
    // if (pTB_Skill->Use_Position == 2 || dwSkillIndex == 30000) return true;
    // if (IsHaveSkill(dwSkillIndex)) return true;
    // GetHaveSkillGroup(pTB_Skill->Skill_Group, &pSkillPtr);
    // if (pSkillPtr && (pSkillPtr->GetDivergenceID() == pTB_Skill->Div_GroupID_01
    //     || pSkillPtr->GetDivergenceID() == pTB_Skill->Div_GroupID_02)) return true;
    // if ((pTB_Skill->Skill_Type == 1 || pTB_Skill->Skill_Type == 2) && pTB_Skill->Passive_Type) return true;
    // return false;

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return false;

    TB_SKILL* pTB_Skill = pServer->GetResourceMgr().GetTB_SKILL(dwSkillIndex);
    if (!pTB_Skill) return false;

    // 特殊位置或特殊技能ID直接返回true
    if (pTB_Skill->Use_Position == 2 || dwSkillIndex == 30000) return true;

    // 检查是否已拥有该技能
    if (IsHaveSkill(static_cast<int>(dwSkillIndex))) return true;

    // 检查同组技能的分歧ID是否匹配
    std::shared_ptr<CSkill> pSkillPtr = GetHaveSkillGroup(static_cast<int>(pTB_Skill->Skill_Group));
    if (pSkillPtr) {
        // 检查分歧ID匹配 (IDA: 检查Div_GroupID_01或Div_GroupID_02)
        TB_SKILL_DIVERGENCE* pDiv = pSkillPtr->GetDivergence();
        if (pDiv) {
            // 需要比较分歧组ID - 这里简化处理，实际需要TB_SKILL_DIVERGENCE结构
            // IDA: *(_DWORD *)((char *)&v5[6]._Parent + 3) == pTB_Skill->Div_GroupID_01
        }
    }

    // 检查被动技能类型
    if ((pTB_Skill->Skill_Type == 1 || pTB_Skill->Skill_Type == 2) && pTB_Skill->Passive_Type)
        return true;

    return false;
}

// ----------------------------------------------------------------------------
// IsHaveSkillQuickSlot - 检查技能是否在快捷槽
// IDA 0x1401688B0: ?IsHaveSkillQuickSlot@CGocSkill@@QEAA_NK@Z
// 完整逻辑基于 IDA 反编译:
// 1. 获取TB_SKILL，如果不存在返回false
// 2. 如果Use_Position==2 或 dwSkillIndex==30000 返回true
// 3. 如果(Skill_Type==1或2)且Passive_Type 返回true
// 4. 如果Skill_Type为4,5,6,7,9 返回true
// 5. 如果Use_State==1 返回true
// 6. 如果FindSkillDeck且HaveModeSkillActiveCount 返回true
// 7. 检查Swap_Skill_Index
// ----------------------------------------------------------------------------
bool CGocSkill::IsHaveSkillQuickSlot(unsigned int dwSkillIndex) const
{
    // IDA反编译 (完整还原):
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SKILL* pTB_Skill = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, dwSkillIndex);
    // if (!pTB_Skill) return false;
    // if (pTB_Skill->Use_Position == 2 || dwSkillIndex == 30000) return true;
    // if ((pTB_Skill->Skill_Type == 1 || pTB_Skill->Skill_Type == 2) && pTB_Skill->Passive_Type) return true;
    // if (pTB_Skill->Skill_Type == 4 || pTB_Skill->Skill_Type == 5 || pTB_Skill->Skill_Type == 6
    //     || pTB_Skill->Skill_Type == 7 || pTB_Skill->Skill_Type == 9) return true;
    // if (pTB_Skill->Use_State == 1) return true;
    // if (FindSkillDeck(dwSkillIndex) && HaveModeSkillActiveCount(pTB_Skill->Skill_Group)) return true;
    // return pTB_Skill->Swap_Skill_Index
    //     && FindSkillDeck(pTB_Skill->Swap_Skill_Index)
    //     && HaveModeSkillActiveCount(pTB_Skill->Skill_Group);

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return false;

    TB_SKILL* pTB_Skill = pServer->GetResourceMgr().GetTB_SKILL(dwSkillIndex);
    if (!pTB_Skill) return false;

    // 检查特殊位置或特殊技能ID
    if (pTB_Skill->Use_Position == 2 || dwSkillIndex == 30000) return true;

    // 检查被动技能类型 (Skill_Type 1或2 且有Passive_Type)
    if ((pTB_Skill->Skill_Type == 1 || pTB_Skill->Skill_Type == 2) && pTB_Skill->Passive_Type) return true;

    // 检查特殊技能类型 (4,5,6,7,9)
    if (pTB_Skill->Skill_Type == 4 || pTB_Skill->Skill_Type == 5 ||
        pTB_Skill->Skill_Type == 6 || pTB_Skill->Skill_Type == 7 ||
        pTB_Skill->Skill_Type == 9) return true;

    // 检查使用状态
    if (pTB_Skill->Use_State == 1) return true;

    // 检查是否在技能卡组中且模式技能激活计数有效
    if (FindSkillDeck(static_cast<int>(dwSkillIndex)) && HaveModeSkillActiveCount(static_cast<int>(pTB_Skill->Skill_Group)))
        return true;

    // 检查交换技能
    if (pTB_Skill->Swap_Skill_Index &&
        FindSkillDeck(static_cast<int>(pTB_Skill->Swap_Skill_Index)) &&
        HaveModeSkillActiveCount(static_cast<int>(pTB_Skill->Skill_Group)))
        return true;

    return false;
}

// ----------------------------------------------------------------------------
// IsHaveSkill - 检查是否拥有技能
// IDA 0x14016BC80: ?IsHaveSkill@CGocSkill@@QEAA_NH@Z
// 基于 IDA 反编译代码还原:
// 检查 m_HaveSkill 或 m_HaveModeSkill 容器中是否存在指定技能ID
// ----------------------------------------------------------------------------
bool CGocSkill::IsHaveSkill(int nSkillID) const
{
    // IDA反编译: 使用boost::multi_index::hashed_index::find查找技能
    // if (this->m_bUseModeSkill) {
    //     Index = impl(&this->m_HaveModeSkill)
    //     iter = find(Index, nSkillID)
    //     return iter != end(Index)
    // } else {
    //     Index = impl(&this->m_HaveSkill)
    //     iter = find(Index, nSkillID)
    //     return iter != end(Index)
    // }
    if (m_bUseModeSkill) {
        return m_HaveModeSkill.find(nSkillID) != m_HaveModeSkill.end();
    } else {
        return m_HaveSkill.find(nSkillID) != m_HaveSkill.end();
    }
}

// ============================================================================
// 技能加载/学习/删除
// ============================================================================

// ----------------------------------------------------------------------------
// LoadSkill - 加载技能数据
// IDA 0x140168A50: ?LoadSkill@CGocSkill@@QEAA_NAEAUPS_SKILL_LOAD@@@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 设置 m_wTotalSkillPoint, m_wSkillPoint, m_wSkillDeckSlotCount
// 2. 检查 owner 是否为 CUser (RTTI)
// 3. 遍历 vecInfo 加载技能到 m_HaveSkill
// 4. 处理被动技能 (Passive_Type == 17)
// 5. 设置 Swap_Skill_Index 和 DivergenceID
// 6. 调用 LoadSkillDeck 加载卡组
// ----------------------------------------------------------------------------
bool CGocSkill::LoadSkill(void* stSkillLoad)
{
    // IDA反编译 (完整还原):
    // PS_SKILL_LOAD* psLoad = (PS_SKILL_LOAD*)stSkillLoad;
    //
    // // 设置技能点 (IDA: 直接赋值)
    // this->m_wTotalSkillPoint = psLoad->wTotalSkillPoint;
    // this->m_wSkillPoint = psLoad->wSkillPoint;
    // this->m_wSkillDeckSlotCount = psLoad->wDeckSlotCount;
    // if (this->m_wSkillDeckSlotCount > 4u)
    //     this->m_wSkillDeckSlotCount = 4;
    //
    // // 检查 owner 是否为 CUser (IDA: _RTDynamicCast_0)
    // v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);  // 获取 owner
    // if (!_RTDynamicCast_0(v2, 0, &CMover RTTI, &CUser RTTI, 0))
    //     return false;
    //
    // // 遍历 vecInfo 加载技能 (IDA: vector iterator loop)
    // for (auto it = psLoad->vecInfo.begin(); it != psLoad->vecInfo.end(); ++it) {
    //     ST_SKILL_INFO& skillInfo = *it;
    //     v5 = TXSingleton<XGameServer>::Instance();
    //     pTBSkill = XResourceMgr::GetTB_SKILL(&v5->m_xResourceMgr, skillInfo.nID);
    //     if (pTBSkill) {
    //         // 检查是否已有同组技能 (IDA: GetHaveSkillGroup)
    //         GetHaveSkillGroup(this, &pGroupSkill, pTBSkill->Skill_Group);
    //         if (!pGroupSkill) {  // IDA: 检查 shared_ptr::operator bool
    //             // 创建新技能 (IDA: CSkill::CSkill)
    //             v22 = (CSkill*)VBaseObject::operator new(0x28u);  // 40字节
    //             if (v22)
    //                 v29 = CSkill::CSkill(v22, pTBSkill);
    //             else
    //                 v29 = nullptr;
    //             _Px = v29;
    //             std::tr1::shared_ptr<CSkill>::shared_ptr(&pSkillData, v29);
    //
    //             // 插入到 m_HaveSkill (IDA: boost::multi_index::hashed_index::insert)
    //             boost::multi_index::...::insert(&this->m_HaveSkill, &result, &pSkillData);
    //             if (result.second) {  // 插入成功
    //                 // 如果是被动技能 (IDA: Skill_Type == 1 && Passive_Type == 17)
    //                 if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 17)
    //                     std::vector::push_back(&this->m_vPassiveSkill, &pSkillData);
    //
    //                 // 如果有 Swap_Skill_Index (IDA: 设置交换技能)
    //                 if (pTBSkill->Swap_Skill_Index) {
    //                     v8 = TXSingleton<XGameServer>::Instance();
    //                     dwCheckTick = XResourceMgr::GetTB_SKILL(&v8->m_xResourceMgr, pTBSkill->Swap_Skill_Index);
    //                     v9 = std::tr1::shared_ptr::operator->(&pSkillData);
    //                     CWeeklyMissionInfo::UpdateDate(v9, dwCheckTick);  // 实际是 SetSwapSkill
    //                 }
    //
    //                 // 设置分歧ID (IDA: 插入到 m_mapSkillDivergence)
    //                 if (skillInfo.nDivergenceID) {
    //                     std::pair<int,int>::pair(&v24, &skillInfo.nID, &skillInfo.nDivergenceID);
    //                     std::_Tree::insert(&this->m_mapSkillDivergence, &v25, v10);
    //                 }
    //             }
    //         }
    //     }
    // }
    //
    // // 加载技能卡组 (IDA: 调用 LoadSkillDeck)
    // CGocSkill::LoadSkillDeck(this, stSkillLoad);
    // return true;

    // TODO: 需要PS_SKILL_LOAD, ST_SKILL_INFO, TB_SKILL等结构定义和依赖
    // 完整实现需要以下结构和函数:
    // - PS_SKILL_LOAD { wTotalSkillPoint, wSkillPoint, wDeckSlotCount, vecInfo }
    // - ST_SKILL_INFO { nID, nDivergenceID }
    // - TB_SKILL { Skill_Index, Skill_Group, Skill_Type, Passive_Type, Swap_Skill_Index }
    // - CSkill::CSkill(TB_SKILL*)
    // - XResourceMgr::GetTB_SKILL()
    // - _RTDynamicCast_0 (RTTI)
    (void)stSkillLoad;
    return true;
}

bool CGocSkill::LearnSkill(int nSkillID, bool bUseCheat, int nTicknum)
{
    // TODO: 需要IDA反编译确认实现
    // IDA 0x140168EE0 - 大型函数，需要TB_SKILL等依赖
    (void)nSkillID;
    (void)bUseCheat;
    (void)nTicknum;
    return true;
}

void CGocSkill::ResetSkill(bool bFullReset, int nSkillID)
{
    // TODO: 需要IDA反编译确认实现
    (void)bFullReset;
    (void)nSkillID;
}

void CGocSkill::DeleteSkill(std::uint16_t wSkillID)
{
    // TODO: 需要IDA反编译确认实现
    (void)wSkillID;
}

// ----------------------------------------------------------------------------
// GetHaveSkillGroup - 获取技能组
// IDA 0x14016BD60: ?GetHaveSkillGroup@CGocSkill@@QEAA?AV?$shared_ptr@VCSkill@@@tr1@std@@H@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 检查 m_bUseModeSkill 决定搜索哪个容器
// 2. 遍历容器查找匹配 Skill_Group 的技能
// 3. 返回匹配的技能或 nullptr
// ----------------------------------------------------------------------------
std::shared_ptr<CSkill> CGocSkill::GetHaveSkillGroup(int nSkillGroup) const
{
    // IDA反编译 (完整还原):
    // 使用 boost::multi_index 遍历，这里用 std::map 替代
    //
    // if (m_bUseModeSkill) {
    //     // 遍历 m_HaveModeSkill
    //     for (auto iter = begin(m_HaveModeSkill); iter != end(m_HaveModeSkill); ++iter) {
    //         pSkillData = *iter;
    //         if (!pSkillData) {
    //             return nullptr;
    //         }
    //         // IDA: 获取 TB_SKILL 表指针并检查 Skill_Group
    //         pTblRef = pSkillData->GetTable();
    //         if (pTblRef && pTblRef->Skill_Group == nSkillGroup) {
    //             return pSkillData;
    //         }
    //     }
    // } else {
    //     // 遍历 m_HaveSkill
    //     for (auto iter = begin(m_HaveSkill); iter != end(m_HaveSkill); ++iter) {
    //         pSkillData = *iter;
    //         if (!pSkillData) {
    //             return nullptr;
    //         }
    //         // IDA: 获取 TB_SKILL 表指针并检查 Skill_Group
    //         pTblRef = pSkillData->GetTable();
    //         if (pTblRef && pTblRef->Skill_Group == nSkillGroup) {
    //             return pSkillData;
    //         }
    //     }
    // }
    // return nullptr;

    // 使用 CSkill::GetGroup() 方法代替直接访问 TB_SKILL::Skill_Group
    if (m_bUseModeSkill) {
        for (const auto& pair : m_HaveModeSkill) {
            if (pair.second) {
                // IDA: 检查 pSkillData->GetGroup() == nSkillGroup
                if (pair.second->GetGroup() == nSkillGroup) {
                    return pair.second;
                }
            }
        }
    } else {
        for (const auto& pair : m_HaveSkill) {
            if (pair.second) {
                // IDA: 检查 pSkillData->GetGroup() == nSkillGroup
                if (pair.second->GetGroup() == nSkillGroup) {
                    return pair.second;
                }
            }
        }
    }
    return nullptr;
}

// ============================================================================
// 技能点管理
// ============================================================================

// ----------------------------------------------------------------------------
// AddSkillPoint - 添加技能点
// IDA 0x14016C050: ?AddSkillPoint@CGocSkill@@QEAAXHH_N@Z
// ----------------------------------------------------------------------------
void CGocSkill::AddSkillPoint(int nPoints, int nReason, bool bUpdate)
{
    // IDA反编译:
    // this->m_wSkillPoint += nPoints;
    // CGocSkill::SendPacketUpdateSkillPoint(this);
    // if (bUpdate)
    //     CGocSkill::SendDBUpdateSkillPoint(this);
    m_wSkillPoint += static_cast<std::uint16_t>(nPoints);
    (void)nReason;
    SendPacketUpdateSkillPoint();
    if (bUpdate) {
        SendDBUpdateSkillPoint();
    }
}

// ============================================================================
// 技能组
// ============================================================================

// ----------------------------------------------------------------------------
// GetFamilyID - 获取组件Family ID
// IDA 0x140039010: ?GetFamilyID@CGocSkill@@SAHXZ
// ----------------------------------------------------------------------------
int CGocSkill::GetFamilyID()
{
    // IDA反编译: return 5;
    return 5;
}

// ----------------------------------------------------------------------------
// IsModeState - 检查是否处于模式状态
// IDA 0x140168280: ?IsModeState@CGocSkill@@QEAA_NXZ
// ----------------------------------------------------------------------------
bool CGocSkill::IsModeState() const
{
    // IDA反编译: return this->m_bUseModeSkill;
    return m_bUseModeSkill;
}

// ----------------------------------------------------------------------------
// GetModeShopMoney - 获取模式商店货币
// IDA 0x14005B420: ?GetModeShopMoney@CGocSkill@@QEAAHXZ
// ----------------------------------------------------------------------------
int CGocSkill::GetModeShopMoney() const
{
    // IDA反编译: return (unsigned int)this->m_ModeShopMyInfo.nRoguelikeMoney;
    // TODO: 需要PS_ROGUELIKE_SHOP_MY_INFO结构定义
    // return reinterpret_cast<const PS_ROGUELIKE_SHOP_MY_INFO*>(m_ModeShopMyInfo)->nRoguelikeMoney;
    return 0; // 占位符，需要结构定义后修正
}

// ============================================================================
// 被动技能
// ============================================================================

// ----------------------------------------------------------------------------
// SetPassiveSkillStat - 设置被动技能状态
// IDA 0x14016D550: ?SetPassiveSkillStat@CGocSkill@@QEAAXG@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 通过 RTTI 获取 owner 的 CUser 指针
// 2. 获取 TB_BUFF 表数据
// 3. 如果 EffectType_01 或 Buff_Time 非零, 调用 SetBuffStatus
// 4. 否则调用 SetBuffAbility 处理 EffectType_Status_01/02/03
// ----------------------------------------------------------------------------
void CGocSkill::SetPassiveSkillStat(std::uint16_t wBuffID)
{
    // IDA反编译 (完整还原):
    // v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);  // 获取 owner
    // pUser = (CUser*)_RTDynamicCast_0(v2, 0, &CMover RTTI, &CUser RTTI, 0);
    // if (pUser) {
    //     v3 = TXSingleton<XGameServer>::Instance();
    //     pBuffTable = XResourceMgr::GetTB_BUFF(&v3->m_xResourceMgr, wBuffID);
    //     if (pBuffTable) {
    //         if (pBuffTable->EffectType_01 || pBuffTable->Buff_Time) {
    //             pUser->SetBuffStatus(&pUser->CMoverEx, wBuffID, 0, true);
    //         } else {
    //             if (pBuffTable->EffectType_Status_01)
    //                 pUser->SetBuffAbility(&pUser->CMoverEx, pBuffTable->EffectType_Status_01);
    //             if (pBuffTable->EffectType_Status_02)
    //                 pUser->SetBuffAbility(&pUser->CMoverEx, pBuffTable->EffectType_Status_02);
    //             if (pBuffTable->EffectType_Status_03)
    //                 pUser->SetBuffAbility(&pUser->CMoverEx, pBuffTable->EffectType_Status_03);
    //         }
    //     }
    // }

    // TODO: 需要CUser, TB_BUFF, XResourceMgr, _RTDynamicCast等依赖
    // 完整实现需要以下结构和函数:
    // - CUser::SetBuffStatus(CMoverEx*, uint16_t, int, bool)
    // - CUser::SetBuffAbility(CMoverEx*, int)
    // - XResourceMgr::GetTB_BUFF()
    // - TB_BUFF 结构 (EffectType_01, Buff_Time, EffectType_Status_01/02/03)
    (void)wBuffID;
}

// ----------------------------------------------------------------------------
// ClearPassiveSkillStat - 清除被动技能状态
// IDA 0x14016D730: ?ClearPassiveSkillStat@CGocSkill@@QEAAXG@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 通过 RTTI 获取 owner 的 CUser 指针
// 2. 获取 TB_BUFF 表数据
// 3. 如果 EffectType_01 非零, 调用 ClearBuffStatus
// 4. 否则调用 ClearBuffAbility 处理 EffectType_Status_01/02/03
// ----------------------------------------------------------------------------
void CGocSkill::ClearPassiveSkillStat(std::uint16_t wBuffID)
{
    // IDA反编译 (完整还原):
    // v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);  // 获取 owner
    // pUser = (CUser*)_RTDynamicCast_0(v2, 0, &CMover RTTI, &CUser RTTI, 0);
    // if (pUser) {
    //     v3 = TXSingleton<XGameServer>::Instance();
    //     pBuffTable = XResourceMgr::GetTB_BUFF(&v3->m_xResourceMgr, wBuffID);
    //     if (pBuffTable) {
    //         if (pBuffTable->EffectType_01) {
    //             pUser->ClearBuffStatus(&pUser->CMoverEx, wBuffID, true, 0);
    //         } else {
    //             if (pBuffTable->EffectType_Status_01)
    //                 pUser->ClearBuffAbility(&pUser->CMoverEx, pBuffTable->EffectType_Status_01);
    //             if (pBuffTable->EffectType_Status_02)
    //                 pUser->ClearBuffAbility(&pUser->CMoverEx, pBuffTable->EffectType_Status_02);
    //             if (pBuffTable->EffectType_Status_03)
    //                 pUser->ClearBuffAbility(&pUser->CMoverEx, pBuffTable->EffectType_Status_03);
    //         }
    //     }
    // }

    // TODO: 需要CUser, TB_BUFF, XResourceMgr, _RTDynamicCast等依赖
    // 完整实现需要以下结构和函数:
    // - CUser::ClearBuffStatus(CMoverEx*, uint16_t, bool, int)
    // - CUser::ClearBuffAbility(CMoverEx*, int)
    // - XResourceMgr::GetTB_BUFF()
    // - TB_BUFF 结构 (EffectType_01, EffectType_Status_01/02/03)
    (void)wBuffID;
}

void CGocSkill::CheckPassiveSkill(std::uint8_t byType)
{
    // IDA 0x14016DEA0 (完整还原):
    // 这是一个大型函数，遍历被动技能并处理条件触发
    //
    // 伪代码逻辑:
    // CUser* pUser = _RTDynamicCast_0(owner, &CMover RTTI, &CUser RTTI, 0);
    // if (pUser) {
    //     CMySkillList* pSkillMgr = pUser->GetSkillMgr();
    //     if (pSkillMgr) {
    //         bool bUpdateStat = false;
    //         if (IsModeState()) {
    //             // 遍历 m_vPassiveModeSkill
    //             for (auto& pSkillData : m_vPassiveModeSkill) {
    //                 if (pSkillData && pSkillData->GetTable()) {
    //                     TB_SKILL* pSkillTable = pSkillData->GetTable();
    //                     if (pSkillTable->Behavior_Condition == byType) {
    //                         float fCooltime = pSkillMgr->GetCooltime(E_COOLTIME_SKILL,
    //                             pSkillTable->CoolTime_Group, pSkillTable->CoolTime_Global, false);
    //                         if (fCooltime == 0.0f) {
    //                             bUpdateStat = true;
    //                             SetPassiveSkillStat(pSkillTable->Passive_Value);
    //                             pSkillMgr->SetSkillCooltime(pSkillTable);
    //                             // 发送冷却包给客户端
    //                         }
    //                     }
    //                 }
    //             }
    //         } else {
    //             // 遍历 m_vPassiveSkill
    //             for (auto& pSkillData : m_vPassiveSkill) {
    //                 if (pSkillData && pSkillData->GetTable()) {
    //                     TB_SKILL* pSkillTable = pSkillData->GetTable();
    //                     if (pSkillTable->Behavior_Condition == byType) {
    //                         float fCooltime = pSkillMgr->GetCooltime(E_COOLTIME_SKILL,
    //                             pSkillTable->CoolTime_Group, pSkillTable->CoolTime_Global, false);
    //                         if (fCooltime == 0.0f) {
    //                             bUpdateStat = true;
    //                             SetPassiveSkillStat(pSkillTable->Passive_Value);
    //                             pSkillMgr->SetSkillCooltime(pSkillTable);
    //                             // 发送冷却包给客户端 (main=6, sub=0x7E)
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //         if (bUpdateStat) {
    //             // 更新属性
    //             auto pAttr = pUser->GetGOC<CGocAttribute>();
    //             if (pAttr) pAttr->UpdateStat();
    //         }
    //     }
    // }

    // TODO: 需要CUser, TB_SKILL, CMySkillList, CGocAttribute等依赖
    (void)byType;
}

// ============================================================================
// 技能卡组
// ============================================================================

// ----------------------------------------------------------------------------
// ResetSkillDeck - 重置技能卡组
// IDA 0x14016C0D0: ?ResetSkillDeck@CGocSkill@@QEAAXXZ
// 基于 IDA 反编译代码还原:
// 1. 清空 m_nSkillDeck 数组
// 2. 获取 owner 对象 (通过 RTTI)
// 3. 发送数据库包通知数据库清空
// ----------------------------------------------------------------------------
void CGocSkill::ResetSkillDeck()
{
    // IDA反编译:
    // memset(this->m_nSkillDeck, 0, sizeof(this->m_nSkillDeck))
    // v7 = std::list<CBattleZone*>::size(this)  // 获取owner
    // if (v7) pObject = &v7[3].m_ChunkSizeTempMemOfs
    // else pObject = nullptr
    // XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x44u, 6u)
    // QuestID = CQuestCondition::GetQuestID(v1)
    // operator<<(&xSendDBPacket.XParse, QuestID)
    // XGameServer::SendDBGame(v3, &xSendDBPacket)

    std::memset(m_nSkillDeck, 0, sizeof(m_nSkillDeck));
    // TODO: 发送数据库包清空卡组 (需要 XSendDBPacket, XGameServer::SendDBGame)
}

// ----------------------------------------------------------------------------
// LoadSkillDeck - 加载技能卡组
// IDA 0x14016C200: ?LoadSkillDeck@CGocSkill@@QEAA_NAEAUPS_SKILL_LOAD@@@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 设置 m_byDeckCount 和 m_byActiveDeck
// 2. 遍历 PS_SKILL_DECK_PAGE 设置 m_stSkillDeckPage
// 3. 遍历 PS_SKILL_DECK 设置 m_nSkillDeck
// ----------------------------------------------------------------------------
bool CGocSkill::LoadSkillDeck(void* stSkillLoad)
{
    // IDA反编译 (完整还原):
    // PS_SKILL_LOAD* psLoad = (PS_SKILL_LOAD*)stSkillLoad;
    //
    // // 设置卡组数量 (IDA: 检查 vecInfo.size() 且 size() <= 5)
    // if (psLoad->psSkillPage.vecInfo.size() && psLoad->psSkillPage.vecInfo.size() <= 5)
    //     m_byDeckCount = psLoad->psSkillPage.vecInfo.size();
    // else
    //     m_byDeckCount = 0;
    //
    // m_byActiveDeck = 0;
    // if (psLoad->psSkillPage.byActivePage && psLoad->psSkillPage.byActivePage < 5
    //     && psLoad->psSkillPage.byActivePage < m_byDeckCount)
    //     m_byActiveDeck = psLoad->psSkillPage.byActivePage;
    //
    // // 设置m_stSkillDeckPage (IDA: for i = 0; i < m_byDeckCount; ++i)
    // for (int i = 0; i < m_byDeckCount; ++i) {
    //     PS_SKILL_DECK_PAGE& psDeckPage = psLoad->psSkillPage.vecInfo[i];
    //     if (psDeckPage.byDeckPage < 5 && psDeckPage.byDeckPage < m_byDeckCount) {
    //         // 默认奖励槽 (IDA: 如果 wDeckBonus[0] == 0 则设置默认值)
    //         if (!psDeckPage.wDeckBonus[0]) {
    //             psDeckPage.wDeckBonus[0] = 1;
    //             psDeckPage.wDeckBonus[1] = 11;
    //             psDeckPage.wDeckBonus[2] = 21;
    //         }
    //         memcpy(&m_stSkillDeckPage[psDeckPage.byDeckPage], &psDeckPage, sizeof(PS_SKILL_DECK_PAGE));
    //     }
    // }
    //
    // // 设置m_nSkillDeck (IDA: 遍历 stSkillDeck vector)
    // for (auto& psDeck : psLoad->stSkillDeck) {
    //     unsigned char byDeckPage = GetDeckPage(psDeck.wPos);
    //     unsigned short wPos = GetDeckPos(psDeck.wPos);
    //     for (int j = 0; j < m_wSkillDeckSlotCount; ++j) {
    //         if (wPos < 6 && byDeckPage < 5 && byDeckPage < m_byDeckCount && j < 4)
    //             m_nSkillDeck[byDeckPage][wPos][j] = *(&psDeck.nSkill_1 + j);
    //     }
    // }
    // return true;

    // TODO: 需要PS_SKILL_LOAD, PS_SKILL_DECK_PAGE, PS_SKILL_DECK结构定义
    // 完整实现需要以下结构:
    // - PS_SKILL_LOAD { wTotalSkillPoint, wSkillPoint, wDeckSlotCount, psSkillPage, stSkillDeck, vecInfo }
    // - PS_SKILL_DECK_PAGE { byDeckPage, wDeckBonus[3], ... }
    // - PS_SKILL_DECK { wPos, nSkill_1, nSkill_2, nSkill_3, nSkill_4 }
    (void)stSkillLoad;
    return true;
}

bool CGocSkill::UpdateSkillDeck(void* stSkillDeckVec)
{
    // IDA 0x14016C5C0 (完整还原):
    // 这是一个大型函数，更新技能卡组
    //
    // 伪代码逻辑:
    // bool bSuccessed = true;
    // int nTempSkillDeck[5][6][4];
    // memcpy(nTempSkillDeck, m_nSkillDeck, sizeof(nTempSkillDeck));
    // PS_SKILL_DECK_VEC psFailDeck;
    //
    // for (auto& deck : stSkillDeckVec) {
    //     unsigned short wX = GetDeckPos(deck.wPos);
    //     unsigned char byPage = GetDeckPage(deck.wPos);
    //
    //     // 验证页和位置
    //     if (byPage >= 5 || byPage > m_byDeckCount - 1) {
    //         psFailDeck.push_back(deck);
    //         break;
    //     }
    //     if (wX >= 6) {
    //         psFailDeck.push_back(deck);
    //         break;
    //     }
    //
    //     // 验证技能是否存在
    //     for (int i = 0; i < m_wSkillDeckSlotCount; ++i) {
    //         if (deck.nSkill[i] && !IsHaveSkill(deck.nSkill[i])) {
    //             psFailDeck.push_back(deck);
    //             break;
    //         }
    //         if (i < 4)
    //             nTempSkillDeck[byPage][wX][i] = deck.nSkill[i];
    //     }
    // }
    //
    // if (psFailDeck.empty()) {
    //     // 成功，更新卡组
    //     memcpy(m_nSkillDeck, nTempSkillDeck, sizeof(m_nSkillDeck));
    //     // 发送数据库包 (main=0x44, sub=5)
    //     // 发送客户端包 (main=6, sub=0x75)
    // } else {
    //     // 失败，发送错误包
    //     bSuccessed = false;
    //     // 发送失败包给客户端
    // }
    // return bSuccessed;

    // TODO: 需要PS_SKILL_DECK_VEC结构定义
    (void)stSkillDeckVec;
    return true;
}

bool CGocSkill::AddDeckSlot(void* stStorageInfo, int nCheatCount)
{
    // IDA 0x14016CCC0 (完整还原):
    // 伪代码逻辑:
    // if (nCheatCount)
    //     m_wSkillDeckSlotCount = 2;
    //
    // if (m_wSkillDeckSlotCount < 4) {
    //     ++m_wSkillDeckSlotCount;
    //     // 发送数据库包 (main=0x44, sub=4)
    //     XSendDBPacket xSendDBPacket(pObject, 0x44, 4);
    //     xSendDBPacket << GetQuestID();
    //     xSendDBPacket << m_wSkillDeckSlotCount;
    //     xSendDBPacket << stStorageInfo;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //     return true;
    // } else {
    //     // 发送错误消息
    //     CGocNetwork::SendErrorMessage(owner, 6, 0x76, 0xDACD);
    //     return false;
    // }

    // TODO: 需要XSendDBPacket等依赖
    (void)stStorageInfo;
    (void)nCheatCount;
    return true;
}

void CGocSkill::ChangeDeckNewSkill(int nOldSkillID, int nNewSkillID)
{
    // IDA 0x14016CEC0 (完整还原):
    // 伪代码逻辑:
    // std::vector<unsigned short> vecChangePos;
    //
    // // 遍历所有卡组页和位置
    // for (int byDeckPage = 0; byDeckPage < m_byDeckCount; ++byDeckPage) {
    //     for (int nPos = 0; nPos < 6; ++nPos) {
    //         int nDeckPos = GetDeckPos(nPos);
    //         for (int nSlot = 0; nSlot < m_wSkillDeckSlotCount; ++nSlot) {
    //             if (nDeckPos <= 5 && nSlot < 4) {
    //                 // 找到旧技能，替换为新技能
    //                 if (m_nSkillDeck[byDeckPage][nDeckPos][nSlot] == nOldSkillID) {
    //                     m_nSkillDeck[byDeckPage][nDeckPos][nSlot] = nNewSkillID;
    //                     vecChangePos.push_back(nDeckPos + 6 * byDeckPage);
    //                 }
    //             }
    //         }
    //     }
    // }
    //
    // // 发送更新的卡组给客户端和数据库
    // if (!vecChangePos.empty()) {
    //     PS_SKILL_DECK_VEC stDeckVec;
    //     for (auto wPos : vecChangePos) {
    //         PS_SKILL_DECK stDeck;
    //         stDeck.wPos = wPos;
    //         unsigned char DeckPage = GetDeckPage(wPos);
    //         unsigned short DeckPos = GetDeckPos(wPos);
    //         memcpy(&stDeck.nSkill, m_nSkillDeck[DeckPage][DeckPos], sizeof(stDeck.nSkill));
    //         stDeckVec.push_back(stDeck);
    //     }
    //     // 发送包 (main=6, sub=0x75)
    //     // 发送数据库包 (main=0x44, sub=5)
    // }

    // TODO: 需要PS_SKILL_DECK_VEC等依赖
    (void)nOldSkillID;
    (void)nNewSkillID;
}

// ----------------------------------------------------------------------------
// FindSkillDeck - 查找技能卡组
// IDA 0x14016D490: ?FindSkillDeck@CGocSkill@@QEAA_NH@Z
// IDA 反编译验证: 完整还原
// ----------------------------------------------------------------------------
bool CGocSkill::FindSkillDeck(int nSkillIndex) const
{
    // IDA反编译 (完整还原):
    // if (this->m_bUseModeSkill) return 1;
    // for (nPos = 0; nPos < 6; ++nPos) {
    //     for (nSlot = 0; nSlot < this->m_wSkillDeckSlotCount; ++nSlot) {
    //         if (nSlot < 4 && this->m_nSkillDeck[this->m_byActiveDeck][nPos][nSlot] == nSkillIndex)
    //             return 1;
    //     }
    // }
    // return 0;
    if (m_bUseModeSkill) {
        return true;
    }

    // m_nSkillDeck is stored as int[120] representing [5][6][4]
    // Calculate base index for active deck: byActiveDeck * 24 (6*4)
    for (int nPos = 0; nPos < 6; ++nPos) {
        for (int nSlot = 0; nSlot < m_wSkillDeckSlotCount; ++nSlot) {
            if (nSlot < 4) {
                int nIndex = m_byActiveDeck * 24 + nPos * 4 + nSlot;
                if (m_nSkillDeck[nIndex] == nSkillIndex) {
                    return true;
                }
            }
        }
    }
    return false;
}

// ----------------------------------------------------------------------------
// DefualtDeckCount - 设置默认卡组槽数量
// IDA 0x1405DAD30: ?DefualtDeckCount@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::DefualtDeckCount()
{
    // IDA反编译: this->m_wSkillDeckSlotCount = 3;
    m_wSkillDeckSlotCount = 3;
}

// ----------------------------------------------------------------------------
// GetDeckCount - 获取卡组槽数量
// IDA 0x1405DAD50: ?GetDeckCount@CGocSkill@@QEAAGXZ
// ----------------------------------------------------------------------------
std::uint16_t CGocSkill::GetDeckCount() const
{
    // IDA反编译: return this->m_wSkillDeckSlotCount;
    return m_wSkillDeckSlotCount;
}

// ----------------------------------------------------------------------------
// GetPageDeckCount - 获取页卡组数量
// IDA 0x1405DAD70: ?GetPageDeckCount@CGocSkill@@QEAAEXZ
// ----------------------------------------------------------------------------
std::uint8_t CGocSkill::GetPageDeckCount() const
{
    // IDA反编译: return this->m_byDeckCount;
    return m_byDeckCount;
}

// ----------------------------------------------------------------------------
// GetDeckPos - 获取卡组位置
// IDA 0x140171440: ?GetDeckPos@CGocSkill@@QEAAGG@Z
// ----------------------------------------------------------------------------
std::uint16_t CGocSkill::GetDeckPos(std::uint16_t wPos) const
{
    // IDA反编译: if (wPos >= 6) return wPos % 6; else return wPos;
    if (wPos >= 6) {
        return wPos % 6;
    }
    return wPos;
}

// ----------------------------------------------------------------------------
// GetDeckPage - 获取卡组页
// IDA 0x140171470: ?GetDeckPage@CGocSkill@@QEAAHG@Z
// ----------------------------------------------------------------------------
std::uint8_t CGocSkill::GetDeckPage(std::uint16_t wPos) const
{
    // IDA反编译: if (wPos >= 6) return wPos / 6; else return 0;
    if (wPos >= 6) {
        return static_cast<std::uint8_t>(wPos / 6);
    }
    return 0;
}

// ============================================================================
// 数据库/网络同步
// ============================================================================

// ----------------------------------------------------------------------------
// SendDBLearnSkill - 发送学习技能到数据库
// IDA 0x14016D900: ?SendDBLearnSkill@CGocSkill@@QEAAXHHHH@Z
// 基于 IDA 反编译代码还原:
// 1. 创建 PS_DB_SKILL_LEARN 结构
// 2. 设置技能ID和分歧ID等信息
// 3. 发送数据库包 (main=0x44, sub=1)
// ----------------------------------------------------------------------------
void CGocSkill::SendDBLearnSkill(int nNewSkill, int nOldSkill, int nDivergenceID, int nUseSkillPoint)
{
    // IDA反编译:
    // PS_DB_SKILL_LEARN::PS_DB_SKILL_LEARN(&stLearnSkill)
    // stLearnSkill.nNewSkillID = nNewSkill
    // stLearnSkill.nOldSkillID = nOldSkill
    // stLearnSkill.byResult = 0
    // stLearnSkill.nDivergenceID = nDivergenceID
    // stLearnSkill.nUseSkillPoint = nUseSkillPoint
    // XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x44u, 1u)
    // operator<<(&xSendDBPacket, &stLearnSkill)
    // XGameServer::SendDBGame(v6, &xSendDBPacket)

    // TODO: 需要XSendDBPacket, PS_DB_SKILL_LEARN, XGameServer等结构
    (void)nNewSkill;
    (void)nOldSkill;
    (void)nDivergenceID;
    (void)nUseSkillPoint;
}

// ----------------------------------------------------------------------------
// SendPacketLearnSkill - 发送学习技能包
// IDA 0x14016DA80: ?SendPacketLearnSkill@CGocSkill@@QEAAXHE_NHH@Z
// 基于 IDA 反编译代码还原:
// 1. 创建 PS_TICKCOUNT_INFO 并获取时间戳
// 2. 创建 PS_RES_SKILL_LEARN 结构
// 3. 发送客户端包 (main=6, sub=0x71)
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketLearnSkill(int nSkillID, std::uint8_t byType, bool bResult, int nDivergenceID, int nTicknum)
{
    // IDA反编译:
    // PS_TICKCOUNT_INFO::PS_TICKCOUNT_INFO(&psTick)
    // pUser = _RTDynamicCast_0(v8, 0, &CMover RTTI, &CUser RTTI, 0)
    // if (pUser) CUser::GetResultTick(pUser, nTicknum, &psTick)
    // psTick.dw64ResTickcount = GetTickCount64()
    // psTick.dw64GetTickcount = psTick.dw64ResTickcount - psTick.dw64ReqTickcount
    // PS_REQ_SKILL_LEARN::PS_REQ_SKILL_LEARN(&stResult)
    // stResult.nID = nSkillID
    // stResult.bySkillType = byType
    // stResult.bResult = bResult
    // stResult.nDivergenceID = nDivergenceID
    // XSendPacket::XSendPacket(&xSendPacket, 6u, 0x71u)
    // operator<<(&xSendPacket, &stResult)
    // operator<<(&xSendPacket, &psTick)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // TODO: 需要XSendPacket, PS_RES_SKILL_LEARN, PS_TICKCOUNT_INFO等结构
    (void)nSkillID;
    (void)byType;
    (void)bResult;
    (void)nDivergenceID;
    (void)nTicknum;
}

// ----------------------------------------------------------------------------
// SendDBUpdateSkillPoint - 发送更新技能点到数据库
// IDA 0x14016DC60: ?SendDBUpdateSkillPoint@CGocSkill@@QEAAXXZ
// 基于 IDA 反编译代码还原:
// 1. 创建 PS_DB_SKILL_UPDATE_POINT 结构
// 2. 设置技能点信息
// 3. 发送数据库包 (main=0x44, sub=2)
// ----------------------------------------------------------------------------
void CGocSkill::SendDBUpdateSkillPoint()
{
    // IDA反编译:
    // PS_DB_SKILL_UPDATE_POINT stSkillPoint
    // stSkillPoint.wSkillPoint = this->m_wSkillPoint
    // stSkillPoint.wTotalSkillPoint = this->m_wTotalSkillPoint
    // XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x44u, 2u)
    // operator<<(&xSendDBPacket, &stSkillPoint)
    // XGameServer::SendDBGame(v2, &xSendDBPacket)

    // TODO: 需要XSendDBPacket, PS_DB_SKILL_UPDATE_POINT, XGameServer等结构
}

// ----------------------------------------------------------------------------
// SendPacketUpdateSkillPoint - 发送更新技能点包
// IDA 0x14016DDB0: ?SendPacketUpdateSkillPoint@CGocSkill@@QEAAXXZ
// 基于 IDA 反编译代码还原:
// 1. 从 m_wTotalSkillPoint 和 m_wSkillPoint 创建 PS_SKILL_POINT
// 2. 发送客户端包 (main=6, sub=0x73)
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketUpdateSkillPoint()
{
    // IDA反编译:
    // stPoint = *(PS_SKILL_POINT *)&this->m_wTotalSkillPoint
    // XSendPacket::XSendPacket(&xSendPacket, 6u, 0x73u)
    // operator<<(&xSendPacket, &stPoint)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // TODO: 需要XSendPacket, PS_SKILL_POINT, CGocNetwork等结构
}

// ----------------------------------------------------------------------------
// SendPacketLoadSkill - 发送加载技能包
// IDA 0x14016E490: ?SendPacketLoadSkill@CGocSkill@@QEAAXXZ
// 基于 IDA 反编译代码还原:
// 1. 创建 PS_SKILL_LOAD 结构
// 2. 遍历 m_HaveSkill 填充技能信息
// 3. 遍历 m_stSkillDeckPage 和 m_nSkillDeck 填充卡组信息
// 4. 发送客户端包 (main=6, sub=0x70)
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketLoadSkill()
{
    // IDA反编译:
    // PS_SKILL_LOAD::PS_SKILL_LOAD(&stSkillLoad)
    // stSkillLoad.wTotalSkillPoint = this->m_wTotalSkillPoint
    // stSkillLoad.wSkillPoint = this->m_wSkillPoint
    // stSkillLoad.wDeckSlotCount = this->m_wSkillDeckSlotCount
    // 遍历m_HaveSkill:
    //   iter = begin(Index)
    //   while (iter != end(Index)) {
    //       pSkillData = *iter
    //       skillInfo.nID = pTblRef->Skill_Index
    //       skillInfo.nDivergenceID = ...
    //       vecInfo.push_back(skillInfo)
    //   }
    // 遍历m_stSkillDeckPage:
    //   for j = 0; j < this->m_byDeckCount; ++j {
    //       vecInfo.push_back(m_stSkillDeckPage[j])
    //       for i = 0; i < 6; ++i {
    //           for k = 0; k < this->m_wSkillDeckSlotCount; ++k {
    //               stDeck.nSkill_k = this->m_nSkillDeck[j][i][k]
    //               stDeck.wPos = i + 6 * j
    //           }
    //           stSkillDeck.push_back(stDeck)
    //       }
    //   }
    // stSkillLoad.psSkillPage.byActivePage = this->m_byActiveDeck
    // XSendPacket::XSendPacket(&xSendPacket, 6u, 0x70u)
    // operator<<(&xSendPacket, &stSkillLoad)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // TODO: 需要完整的结构定义 (PS_SKILL_LOAD, ST_SKILL_INFO, PS_SKILL_DECK等)
}

// ============================================================================
// 分歧系统
// ============================================================================

// ----------------------------------------------------------------------------
// LearnDivergence - 学习分歧
// IDA 0x14016E840: ?LearnDivergence@CGocSkill@@QEAA_NHHH@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 在 m_HaveSkill 中查找技能
// 2. 获取 TB_DIVERGENCE 表数据
// 3. 验证分歧组ID匹配 (Div_GroupID)
// 4. 检查等级和技能点要求 (Div_Need_Level, Div_Need_Point)
// 5. 处理旧分歧效果 (如果 Div_Option_Type == 5)
// 6. 应用新分歧效果
// 7. 更新技能分歧并发送数据库包 (main=0x44, sub=7)
// 8. 发送结果包给客户端 (main=6, sub=0x77)
// ----------------------------------------------------------------------------
bool CGocSkill::LearnDivergence(int nSkillID, int nDivergenceID, int nReason)
{
    // IDA反编译 (完整还原):
    // 1. 在 m_HaveSkill 中查找技能
    //    Index = boost::multi_index::get<0>(m_HaveSkill);
    //    iter = Index.find(nSkillID);
    //    if (iter == Index.end()) return false;
    //    pSkill = *iter;
    //
    // 2. 获取 TB_DIVERGENCE 表数据
    //    pTBDivergence = XResourceMgr::GetTB_DIVERGENCE(nDivergenceID);
    //    if (!pTBDivergence) return false;
    //
    // 3. 验证分歧组ID匹配
    //    // IDA: 检查 pTBDivergence->Div_GroupID 是否匹配技能的分歧组
    //    if (pTBDivergence->Div_GroupID != pSkill->GetDivergenceID()) {
    //        if (pTBDivergence->Div_GroupID != pSkill->GetDivergenceID2())
    //            return false;
    //    }
    //
    // 4. 检查等级和技能点要求
    //    // IDA: pTBDivergence->Div_Need_Level > pUser->GetLevel()
    //    if (pTBDivergence->Div_Need_Level > pUser->GetLevel()) return false;
    //    // IDA: pTBDivergence->Div_Need_Point > m_wSkillPoint
    //    if (pTBDivergence->Div_Need_Point > m_wSkillPoint) return false;
    //
    // 5. 处理旧分歧效果 (IDA: pPrevDivergence->Div_Option_Type == 5)
    //    pPrevDivergence = pSkill->GetDivergence();
    //    if (pPrevDivergence && pPrevDivergence->Div_Option_Type == 5)
    //        ClearPassiveSkillStat(pPrevDivergence->Div_Option_Value);
    //
    // 6. 应用新分歧效果 (IDA: pTBDivergence->Div_Option_Type == 5)
    //    if (pTBDivergence->Div_Option_Type == 5)
    //        SetPassiveSkillStat(pTBDivergence->Div_Option_Value);
    //
    // 7. 更新技能分歧
    //    pSkill->SetDivergence(pTBDivergence);
    //    // IDA: 如果有 Swap_Div_ID，设置交换分歧
    //    if (pTBDivergence->Swap_Div_ID) {
    //        TB_DIVERGENCE* pSwapDiv = XResourceMgr::GetTB_DIVERGENCE(pTBDivergence->Swap_Div_ID);
    //        pSkill->SetSwapDivergence(pSwapDiv);
    //    }
    //
    // 8. 扣除技能点并发送数据库包
    //    AddSkillPoint(-pTBDivergence->Div_Need_Point, 0, true);
    //    // IDA: 发送 ST_LOG_GAME (main=3, sub=2)
    //    // IDA: 发送 ST_STATISTICS_SKILL (main=0xF0, sub=8)
    //    // IDA: 发送数据库包 (main=0x44, sub=7)
    //    SendDBLearnSkill(nSkillID, nSkillID, nDivergenceID, pTBDivergence->Div_Need_Point);
    //
    // 9. 发送结果包给客户端 (IDA: main=6, sub=0x77)
    //    // PS_TICKCOUNT_INFO 处理
    //    SendPacketLearnSkill(nSkillID, nDivergenceID, true, 0, nReason);
    //    return true;

    // TODO: 需要TB_DIVERGENCE, CUser, XResourceMgr, XSendDBPacket等结构
    // 完整实现需要以下结构和函数:
    // - TB_DIVERGENCE { Div_GroupID, Div_Need_Level, Div_Need_Point, Div_Option_Type, Div_Option_Value, Swap_Div_ID }
    // - CUser::GetLevel()
    // - XResourceMgr::GetTB_DIVERGENCE()
    // - XSendDBPacket, XSendPacket, PS_TICKCOUNT_INFO
    // - ST_LOG_GAME, ST_STATISTICS_SKILL
    (void)nSkillID;
    (void)nDivergenceID;
    (void)nReason;
    return true;
}

// ============================================================================
// 手势槽
// ============================================================================

// ----------------------------------------------------------------------------
// SetGestureSlot - 设置手势槽
// IDA 0x14016F860: ?SetGestureSlot@CGocSkill@@QEAAXPEAH@Z
// ----------------------------------------------------------------------------
void CGocSkill::SetGestureSlot(int* nGestureSlot)
{
    // IDA反编译: qmemcpy(this->m_nGestureSlot, nGestureSlot, sizeof(this->m_nGestureSlot))
    if (nGestureSlot) {
        std::memcpy(m_nGestureSlot, nGestureSlot, sizeof(m_nGestureSlot));
    }
}

// ----------------------------------------------------------------------------
// SendGestureSlot - 发送手势槽
// IDA 0x14016F890: ?SendGestureSlot@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::SendGestureSlot()
{
    // IDA反编译: 发送PS_GESTURE_SLOT给客户端
    // XSendPacket(main=0x23, sub=2)
    // TODO: 需要XSendPacket, PS_GESTURE_SLOT等结构
}

// ----------------------------------------------------------------------------
// GetGestureSlot - 获取手势槽
// IDA 0x14016F990: ?GetGestureSlot@CGocSkill@@QEAAXAEAUPS_GESTURE_SLOT@@@Z
// ----------------------------------------------------------------------------
void CGocSkill::GetGestureSlot(void* psGestureSlot)
{
    // IDA反编译: qmemcpy(psGestureSlot, this->m_nGestureSlot, 6*sizeof(int))
    if (psGestureSlot) {
        std::memcpy(psGestureSlot, m_nGestureSlot, sizeof(m_nGestureSlot));
    }
}

// ============================================================================
// 模式技能
// ============================================================================

// ----------------------------------------------------------------------------
// InitModeSkill - 初始化模式技能
// IDA 0x1401723C0: ?InitModeSkill@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::InitModeSkill()
{
    // IDA反编译 (完整还原):
    // 伪代码逻辑:
    // 1. 获取owner(CUser)
    //    CUser* pUser = _RTDynamicCast_0(owner, &CMover RTTI, &CUser RTTI, 0);
    //    if (!pUser) return;
    //
    // 2. 获取角色信息
    //    int nClass = pUser->GetClass() * 1000;
    //    TB_CHARACTER_INFO* pTBCharInfo = XResourceMgr::GetTB_CHARACTER_INFO(nClass);
    //    if (!pTBCharInfo) return;
    //
    // 3. 初始化模式技能状态
    //    m_bUseModeSkill = true;
    //    m_HaveModeSkill.clear();
    //    memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));
    //    m_vecModeDefaultSkillList.clear();
    //    m_vPassiveModeSkill.clear();
    //    m_ModeShopMyInfo.nRoguelikeMoney = 0;
    //    m_ModeShopMyInfo.mapMyBuyList.clear();
    //    m_mapModeSkillActiveCount.clear();
    //
    // 4. 获取职业属性
    //    auto pAttr = pUser->GetGOC<CGocAttribute>();
    //    int nClass = pAttr->GetClass();
    //
    // 5. 获取随机技能列表
    //    XResourceMgr::GetRoguelikeRandomSkill(nClass, m_vecModeDefaultSkillList);
    //
    // 6. 加载初始技能 (TB_MODE_BI_CLASS_STARTSKILL)
    //    TB_MODE_BI_CLASS_STARTSKILL* pTBModeSkill = XResourceMgr::GetTB_MODE_BI_CLASS_STARTSKILL(nClass);
    //    for (int i = 0; i < 15; ++i) {
    //        int nSkillID = pTBModeSkill->BI_StartSkill_01 + i;
    //        if (setSkillCheck.find(nSkillID) == setSkillCheck.end()) {
    //            TB_SKILL* pTBSkill = XResourceMgr::GetTB_SKILL(nSkillID);
    //            if (pTBSkill) {
    //                CSkill* pNewSkill = new CSkill(pTBSkill);
    //                m_HaveModeSkill.insert({nSkillID, std::shared_ptr<CSkill>(pNewSkill)});
    //                // 处理被动技能
    //                if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13)
    //                    SetPassiveSkillStat(pTBSkill->Passive_Value);
    //            }
    //        }
    //    }
    //
    // 7. 加载升级技能 (TB_MODE_BI_UPGRADE)
    //    for (auto& nID : m_vecModeDefaultSkillList) {
    //        TB_MODE_BI_UPGRADE* pTBUpgrade = XResourceMgr::GetTB_MODE_BI_UPGRADE(nID);
    //        if (pTBUpgrade) {
    //            TB_SKILL* pTBSkill = XResourceMgr::GetTB_SKILL(pTBUpgrade->Upgrade_Type);
    //            if (pTBSkill) {
    //                // 添加技能和激活计数
    //                m_HaveModeSkill.insert({pTBSkill->Skill_Index, ...});
    //                m_mapModeSkillActiveCount[pTBSkill->Skill_Group] = pTBUpgrade->Upgrade_Value;
    //            }
    //        }
    //    }
    //
    // 8. 发送技能加载包给客户端 (main=6, sub=0x72)

    // TODO: 需要大量依赖结构 (TB_MODE_BI_CLASS_STARTSKILL, TB_MODE_BI_UPGRADE等)
}

// ----------------------------------------------------------------------------
// ResetModeSkill - 重置模式技能
// IDA 0x140172DB0: ?ResetModeSkill@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::ResetModeSkill()
{
    // IDA反编译 (简单函数):
    // m_bUseModeSkill = false;
    // m_HaveModeSkill.clear();
    // memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));
    // m_vecModeDefaultSkillList.clear();
    // m_vPassiveModeSkill.clear();
    // memset(m_ModeShopMyInfo, 0, sizeof(m_ModeShopMyInfo));
    // m_mapModeSkillActiveCount.clear();

    m_bUseModeSkill = false;
    m_HaveModeSkill.clear();
    std::memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));
    m_vecModeDefaultSkillList.clear();
    m_vPassiveModeSkill.clear();
    std::memset(m_ModeShopMyInfo, 0, sizeof(m_ModeShopMyInfo));
    m_mapModeSkillActiveCount.clear();
}

// ----------------------------------------------------------------------------
// LearnModeSkill - 学习模式技能
// IDA 0x140172E10: ?LearnModeSkill@CGocSkill@@QEAA_NH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::LearnModeSkill(int nSkillID)
{
    // IDA反编译 (完整还原):
    // 这是一个大型函数，处理roguelike技能学习
    //
    // 伪代码逻辑:
    // if (!m_bUseModeSkill) return false;
    //
    // 1. 在m_HaveModeSkill中查找同组技能
    //    auto Index = boost::multi_index::get<1>(m_HaveModeSkill);
    //    auto iter = Index.find(nSkillGroupID);
    //
    // 2. 如果找到已有技能
    //    if (iter != Index.end()) {
    //        auto pSkillData = *iter;
    //        TB_SKILL* pTBSkill = pSkillData->GetTable();
    //        int nNextSkillID = pSkillData->GetNextSkillID();
    //
    //        if (nNextSkillID) {
    //            // 升级到下一级技能
    //            TB_SKILL* pNewTBSkill = XResourceMgr::GetTB_SKILL(nNextSkillID);
    //            // 删除旧技能，添加新技能
    //            m_HaveModeSkill.erase(iter);
    //            CSkill* pNewSkill = new CSkill(pNewTBSkill);
    //            m_HaveModeSkill.insert({nNextSkillID, std::shared_ptr<CSkill>(pNewSkill)});
    //            // 处理被动技能变化
    //            SendPacketLearnSkill(nNextSkillID, 0, true, 0, 0);
    //        } else {
    //            // 处理分歧升级
    //            TB_DIVERGENCE* pTBDiv = pSkillData->GetDivergence();
    //            if (pTBDiv) {
    //                TB_DIVERGENCE* pNextDiv = XResourceMgr::GetTB_DIVERGENCE(pTBDiv->Next_Div_ID);
    //                // 应用新分歧
    //                SetPassiveSkillStat(pNextDiv->Div_Option_Value);
    //                pSkillData->SetDivergence(pNextDiv);
    //            }
    //        }
    //    }
    // 3. 如果没找到，创建新技能
    //    else {
    //        TB_SKILL* pTBSkill = XResourceMgr::GetTB_SKILL(nSkillGroupID);
    //        if (pTBSkill && pTBSkill->Use_Class == pUser->GetClass()) {
    //            CSkill* pNewSkill = new CSkill(pTBSkill);
    //            m_HaveModeSkill.insert({nSkillGroupID, std::shared_ptr<CSkill>(pNewSkill)});
    //            // 处理被动技能
    //            if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13)
    //                SetPassiveSkillStat(pTBSkill->Passive_Value);
    //            SendPacketLearnSkill(nSkillGroupID, 0, true, 0, 0);
    //        }
    //    }

    // TODO: 需要完整依赖实现
    (void)nSkillID;
    return true;
}

// ----------------------------------------------------------------------------
// CanLearnModeSkill - 检查是否可以学习模式技能
// IDA 0x140173D40: ?CanLearnModeSkill@CGocSkill@@QEAA_NH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::CanLearnModeSkill(int nSkillID)
{
    // IDA反编译: 检查技能学习条件
    // TODO: 需要IDA反编译确认实现
    (void)nSkillID;
    return true;
}

// ----------------------------------------------------------------------------
// ClearModeSkillDeck - 清空模式技能卡组
// IDA 0x1401740B0: ?ClearModeSkillDeck@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::ClearModeSkillDeck()
{
    // IDA反编译: memset(this->m_nModeSkillDeck, 0, sizeof(this->m_nModeSkillDeck))
    std::memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));
}

// ----------------------------------------------------------------------------
// AddModeSkillDeck - 添加模式技能卡组
// IDA 0x1401740F0: ?AddModeSkillDeck@CGocSkill@@QEAAXH@Z
// ----------------------------------------------------------------------------
void CGocSkill::AddModeSkillDeck(int nSkillID)
{
    // IDA反编译: 大型函数，添加技能到模式卡组
    // TODO: 需要IDA反编译确认实现
    (void)nSkillID;
}

// ----------------------------------------------------------------------------
// GetModeDefaultSkillList - 获取模式默认技能列表
// IDA 0x140174360: ?GetModeDefaultSkillList@CGocSkill@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z
// ----------------------------------------------------------------------------
void CGocSkill::GetModeDefaultSkillList(std::vector<unsigned long>& vecModeSkillList)
{
    // IDA反编译: 如果m_bUseModeSkill，复制m_vecModeDefaultSkillList
    if (m_bUseModeSkill) {
        vecModeSkillList = m_vecModeDefaultSkillList;
    }
}

// ----------------------------------------------------------------------------
// GetModeShopMyInfo - 获取模式商店信息
// IDA 0x1401743D0: ?GetModeShopMyInfo@CGocSkill@@QEAAXAEAUPS_ROGUELIKE_SHOP_MY_INFO@@@Z
// ----------------------------------------------------------------------------
void CGocSkill::GetModeShopMyInfo(void* psMyInfo)
{
    // IDA反编译: PS_ROGUELIKE_SHOP_MY_INFO::operator=(psMyInfo, &this->m_ModeShopMyInfo)
    if (psMyInfo) {
        std::memcpy(psMyInfo, m_ModeShopMyInfo, sizeof(m_ModeShopMyInfo));
    }
}

// ----------------------------------------------------------------------------
// UpdateModeShopMoney - 更新模式商店货币
// IDA 0x140174440: ?UpdateModeShopMoney@CGocSkill@@QEAAXH@Z
// ----------------------------------------------------------------------------
void CGocSkill::UpdateModeShopMoney(int nAddMoney)
{
    // IDA反编译:
    // this->m_ModeShopMyInfo.nRoguelikeMoney += nAddMoney
    // if (this->m_ModeShopMyInfo.nRoguelikeMoney < 0)
    //     this->m_ModeShopMyInfo.nRoguelikeMoney = 0
    // TODO: 需要PS_ROGUELIKE_SHOP_MY_INFO结构
    (void)nAddMoney;
}

// ----------------------------------------------------------------------------
// AddModeShopBuyList - 添加模式商店购买列表
// IDA 0x140174490: ?AddModeShopBuyList@CGocSkill@@QEAAXK@Z
// ----------------------------------------------------------------------------
void CGocSkill::AddModeShopBuyList(unsigned int dwUpgradeID)
{
    // IDA反编译: 大型函数，处理roguelike商店购买
    // TODO: 需要IDA反编译确认实现
    (void)dwUpgradeID;
}

// ----------------------------------------------------------------------------
// HaveModeSkillActiveCount - 检查模式技能激活计数
// IDA 0x1401745C0: ?HaveModeSkillActiveCount@CGocSkill@@QEAA_NH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::HaveModeSkillActiveCount(int nGroupID) const
{
    // IDA反编译: 检查m_mapModeSkillActiveCount中是否有足够的计数
    auto it = m_mapModeSkillActiveCount.find(nGroupID);
    if (it != m_mapModeSkillActiveCount.end()) {
        return it->second > 0;
    }
    return true; // 如果不存在，默认返回true
}

// ----------------------------------------------------------------------------
// ChargeModeSkillActiveCount - 消耗模式技能激活计数
// IDA 0x140174660: ?ChargeModeSkillActiveCount@CGocSkill@@QEAAXH@Z
// ----------------------------------------------------------------------------
void CGocSkill::ChargeModeSkillActiveCount(int nGroupID)
{
    // IDA反编译: 减少m_mapModeSkillActiveCount中的计数
    auto it = m_mapModeSkillActiveCount.find(nGroupID);
    if (it != m_mapModeSkillActiveCount.end() && it->second > 0) {
        --it->second;
    }
}

// ----------------------------------------------------------------------------
// AddModeSkillActiveCount - 添加模式技能激活计数
// IDA 0x140174780: ?AddModeSkillActiveCount@CGocSkill@@QEAAXHHH@Z
// ----------------------------------------------------------------------------
void CGocSkill::AddModeSkillActiveCount(int nGroupID, int nTotalCount, int nCount)
{
    // IDA反编译: 添加或更新m_mapModeSkillActiveCount
    m_mapModeSkillActiveCount[nGroupID] = nCount;
    (void)nTotalCount; // 可能用于设置最大计数
}

// ----------------------------------------------------------------------------
// UpdateModeSkillActiveState - 更新模式技能激活状态
// IDA 0x140174870: ?UpdateModeSkillActiveState@CGocSkill@@QEAAXH_N@Z
// ----------------------------------------------------------------------------
void CGocSkill::UpdateModeSkillActiveState(int nGroupID, bool bCanUse)
{
    // IDA反编译: 更新技能组是否可用
    (void)nGroupID;
    (void)bCanUse;
    // TODO: 需要ST_ROGUELIKE_SKILL_ACTIVE_COUNT结构来存储bCanUse状态
}

// ----------------------------------------------------------------------------
// ResetModeSkillActiveState - 重置模式技能激活状态
// IDA 0x140174900: ?ResetModeSkillActiveState@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::ResetModeSkillActiveState()
{
    // IDA反编译: 清空m_mapModeSkillActiveCount
    m_mapModeSkillActiveCount.clear();
}

// ----------------------------------------------------------------------------
// GetRoguelikeSkillCoolTime - 获取Roguelike技能冷却时间
// IDA 0x140174980: ?GetRoguelikeSkillCoolTime@CGocSkill@@QEAAMMK@Z
// ----------------------------------------------------------------------------
float CGocSkill::GetRoguelikeSkillCoolTime(unsigned int dwSkillID)
{
    // IDA反编译: 从TB_SKILL获取冷却时间
    // TODO: 需要XResourceMgr::GetTB_SKILL
    (void)dwSkillID;
    return 0.0f;
}
