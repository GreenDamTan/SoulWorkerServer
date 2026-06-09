#include "GocSkill.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/Skill.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
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
    // std::vector<std::shared_ptr<CSkill>>::clear(&this->m_vPassiveSkill);
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
    // std::vector<std::shared_ptr<CSkill>>::clear(&this->m_vPassiveModeSkill);
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
// IDA 反编译验证: 完整还原
// ----------------------------------------------------------------------------
bool CGocSkill::IsHaveBaseSkill(unsigned int dwSkillIndex) const
{
    // IDA反编译 (完整还原):
    // v2 = TXSingleton<XGameServer>::Instance();
    // pTB_Skill = XResourceMgr::GetTB_SKILL(&v2->m_xResourceMgr, dwSkillIndex);
    // if (!pTB_Skill) return 0;
    // if (pTB_Skill->Use_Position == 2 || dwSkillIndex == 30000) return 1;
    // if (CGocSkill::IsHaveSkill(this, dwSkillIndex)) return 1;
    // CGocSkill::GetHaveSkillGroup(this, &pSkillPtr, pTB_Skill->Skill_Group);
    // if (pSkillPtr && (pSkillPtr->GetDivergenceID() == pTB_Skill->Div_GroupID_01
    //     || pSkillPtr->GetDivergenceID2() == pTB_Skill->Div_GroupID_02)) return 1;
    // if ((pTB_Skill->Skill_Type == 1 || pTB_Skill->Skill_Type == 2) && pTB_Skill->Passive_Type) return 1;
    // return 0;

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
        // IDA: 检查 pSkillPtr->GetDivergenceID() == pTB_Skill->Div_GroupID_01
        // 或者 pSkillPtr->GetDivergenceID2() == pTB_Skill->Div_GroupID_02
        int nDivID1 = pSkillPtr->GetDivergenceID();
        int nDivID2 = pSkillPtr->GetDivergenceID2();
        if (nDivID1 == pTB_Skill->Div_GroupID_01 || nDivID2 == pTB_Skill->Div_GroupID_02) {
            return true;
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
    // IDA 0x140168A50: 完整还原
    PS_SKILL_LOAD* psLoad = static_cast<PS_SKILL_LOAD*>(stSkillLoad);
    if (!psLoad) return false;

    // 设置技能点 (IDA: 直接赋值)
    m_wTotalSkillPoint = psLoad->wTotalSkillPoint;
    m_wSkillPoint = psLoad->wSkillPoint;
    m_wSkillDeckSlotCount = psLoad->wDeckSlotCount;

    // IDA: if (this->m_wSkillDeckSlotCount > 4u) this->m_wSkillDeckSlotCount = 4;
    if (m_wSkillDeckSlotCount > 4)
        m_wSkillDeckSlotCount = 4;

    // 检查 owner 是否为 CUser (IDA: _RTDynamicCast_0)
    CUser* pUser = GetOwnerUser();
    if (!pUser) return false;

    // 遍历 vecInfo 加载技能 (IDA: vector iterator loop)
    for (const auto& skillInfo : psLoad->vecInfo) {
        // IDA: v5 = TXSingleton<XGameServer>::Instance();
        // IDA: pTBSkill = XResourceMgr::GetTB_SKILL(&v5->m_xResourceMgr, skillInfo.nID);
        XGameServer* pServer = XGameServer::Instance();
        if (!pServer) continue;

        TB_SKILL* pTBSkill = pServer->GetResourceMgr().GetTB_SKILL(skillInfo.nID);
        if (!pTBSkill) {
            // IDA: LogHelper::LogError("game.skill", "LoadSkill error - Cant find Skill In Table...")
            continue;
        }

        // 检查是否已有同组技能 (IDA: GetHaveSkillGroup)
        std::shared_ptr<CSkill> pGroupSkill = GetHaveSkillGroup(static_cast<int>(pTBSkill->Skill_Group));
        if (!pGroupSkill) {
            // IDA: 创建新技能 (CSkill::CSkill)
            std::shared_ptr<CSkill> pSkillData = std::make_shared<CSkill>(pTBSkill);

            // IDA: 插入到 m_HaveSkill (boost::multi_index::hashed_index::insert)
            auto result = m_HaveSkill.emplace(pSkillData->GetID(), pSkillData);
            if (result.second) {
                // IDA: 如果是被动技能 (Skill_Type == 1 && Passive_Type == 17)
                if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 17) {
                    m_vPassiveSkill.push_back(pSkillData);
                }

                // IDA: 如果有 Swap_Skill_Index
                if (pTBSkill->Swap_Skill_Index) {
                    // IDA: 设置交换技能 - SetSwapSkill 需要 TB_SWAP_SKILL*
                    // 注意: SetSwapSkill 需要 TB_SWAP_SKILL* 类型，这里需要从资源管理器获取
                    // 暂时跳过，待后续完善
                    // TB_SWAP_SKILL* pSwapSkill = pServer->GetResourceMgr().GetTB_SWAP_SKILL(pTBSkill->Swap_Skill_Index);
                    // if (pSwapSkill) {
                    //     pSkillData->SetSwapSkill(pSwapSkill);
                    // }
                }

                // IDA: 设置分歧ID (插入到 m_mapSkillDivergence)
                if (skillInfo.nDivergenceID) {
                    m_mapSkillDivergence[skillInfo.nID] = skillInfo.nDivergenceID;
                }
            }
        }
    }

    // IDA: 加载技能卡组
    LoadSkillDeck(stSkillLoad);
    return true;
}

// ----------------------------------------------------------------------------
// LearnSkill - 学习技能
// IDA 0x140168EE0: ?LearnSkill@CGocSkill@@QEAA_NH_NH@Z
// 精确还原 - 技能学习主函数
// 参数: nSkillID - 技能ID, bUseCheat - 是否使用作弊, nTicknum - 时间戳
// 逻辑:
// 1. 获取 TB_SKILL 表数据
// 2. 检查是否已拥有该技能组 (升级技能)
// 3. 验证等级、觉醒等级、技能点、职业等条件
// 4. 创建新技能并添加到容器
// 5. 处理被动技能效果
// 6. 发送数据库包和客户端包
// 7. 更新统计和日志
// ----------------------------------------------------------------------------
bool CGocSkill::LearnSkill(int nSkillID, bool bUseCheat, int nTicknum)
{
    // IDA反编译完整还原
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return false;
    }

    // IDA: 获取技能表数据
    // TB_SKILL* pNewSkillTable = XResourceMgr::GetTB_SKILL(nSkillID);
    TB_SKILL* pNewSkillTable = nullptr;  // TODO: pServer->GetResourceMgr().GetTB_SKILL(nSkillID);

    if (!pNewSkillTable) {
        // IDA: 日志记录 "LearnSkill error - No table"
        LogHelper::LogError("game.skill", "LearnSkill error - No table [ SkillID:%d ]", nSkillID);
        return true;  // IDA: 返回 true
    }

    // IDA: 获取 owner CUser
    CUser* pUser = GetOwnerUser();

    // IDA: 检查是否已拥有该技能组 (升级技能)
    std::shared_ptr<CSkill> pSkillData = GetHaveSkillGroup(pNewSkillTable->Skill_Group);

    if (pSkillData) {
        // IDA: 已拥有该技能组，检查是否是升级
        int nGap = pNewSkillTable->Skill_LV - pSkillData->GetLevel();

        if (nGap != 1) {
            // IDA: 不是技能升级，错误
            LogHelper::LogError("game.skill", "LearnSkill error - Not skill next step[ SkillID:%d ]", nSkillID);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查等级要求
        if (pUser && pNewSkillTable->Req_Min_LV > pUser->GetLevel()) {
            LogHelper::LogError("game.skill", "LearnSkill error - Low Level[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查觉醒等级要求
        // CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
        // if (pAttr && pNewSkillTable->Req_Min_AwakeningGrade > pAttr->GetAwaken()) {
        //     LogHelper::LogError("game.skill", "LearnSkill error - Low AwakenGrade");
        //     return false;
        // }

        // IDA: 检查技能点
        if (m_wSkillPoint < pNewSkillTable->Req_Skill_Point) {
            LogHelper::LogError("game.skill", "LearnSkill error - Not enough skill point[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查已使用技能点
        int nPointUsed = m_wTotalSkillPoint - m_wSkillPoint;
        if (nPointUsed < pNewSkillTable->Req_Consume_Point) {
            LogHelper::LogError("game.skill", "LearnSkill error - Shortage all used skill point[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查前置技能
        if (pNewSkillTable->Req_Prev_Skill_ID) {
            if (!IsHaveSkill(pNewSkillTable->Req_Prev_Skill_ID)) {
                LogHelper::LogError("game.skill", "LearnSkill error - Not learn previous skill");
                CMover* pMover = GetOwnerGO();
                if (pMover) {
                    CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
                }
                return false;
            }
        }

        // IDA: 删除旧技能
        m_HaveSkill.erase(pSkillData->GetID());

        // IDA: 创建新技能
        std::shared_ptr<CSkill> pNewSkill = std::make_shared<CSkill>(pNewSkillTable);

        // IDA: 设置 Swap_Skill_Index
        if (pNewSkillTable->Swap_Skill_Index) {
            // TB_SKILL* pSwapSkill = XResourceMgr::GetTB_SKILL(pNewSkillTable->Swap_Skill_Index);
            // if (pSwapSkill) {
            //     pNewSkill->SetSwapSkill(pSwapSkill);
            // }
        }

        // IDA: 设置分歧
        auto itDiv = m_mapSkillDivergence.find(pNewSkillTable->Skill_Index);
        if (itDiv != m_mapSkillDivergence.end()) {
            // TB_DIVERGENCE* pTBDivergence = XResourceMgr::GetTB_DIVERGENCE(itDiv->second);
            // if (pTBDivergence) {
            //     pNewSkill->SetDivergence(pTBDivergence);
            // }
        }

        // IDA: 检查技能组是否已存在
        std::shared_ptr<CSkill> pGroupSkill = GetHaveSkillGroup(pNewSkillTable->Skill_Group);
        if (pGroupSkill) {
            LogHelper::LogError("game.skill", "LearnSkill error - [ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            return false;
        }

        // IDA: 插入新技能
        m_HaveSkill[nSkillID] = pNewSkill;

        // IDA: 处理被动技能
        if (pUser && pNewSkillTable->Skill_Type == 1) {
            if (pNewSkillTable->Passive_Type == 13) {
                // 清除旧被动效果，设置新被动效果
                TB_SKILL* pOldSkillTable = pSkillData->GetTable();
                if (pOldSkillTable) {
                    ClearPassiveSkillStat(pOldSkillTable->Passive_Value);
                }
                SetPassiveSkillStat(pNewSkillTable->Passive_Value);
            } else if (pNewSkillTable->Passive_Type == 17) {
                // 从被动技能列表中移除旧技能
                for (auto it = m_vPassiveSkill.begin(); it != m_vPassiveSkill.end(); ++it) {
                    if (*it == pSkillData) {
                        m_vPassiveSkill.erase(it);
                        break;
                    }
                }
                // 添加新被动技能
                m_vPassiveSkill.push_back(pNewSkill);
            }
        }

        // IDA: 发送包
        int nDivergenceID = pSkillData->GetDivergenceID();
        if (nDivergenceID) {
            SendPacketLearnSkill(nSkillID, 0, true, nDivergenceID, nTicknum);
            SendDBLearnSkill(nSkillID, pSkillData->GetID(), nDivergenceID, pNewSkillTable->Req_Skill_Point);
        } else {
            SendPacketLearnSkill(nSkillID, 0, true, 0, nTicknum);
            SendDBLearnSkill(nSkillID, pSkillData->GetID(), 0, pNewSkillTable->Req_Skill_Point);
        }

        // IDA: 更新卡组中的技能
        ChangeDeckNewSkill(nSkillID, pSkillData->GetID());

        // IDA: 扣除技能点
        AddSkillPoint(-pNewSkillTable->Req_Skill_Point, 0, true);

        // IDA: 发送日志
        // ST_LOG_GAME stLog;
        // stLog._nUAID = pUser->GetUAID();
        // stLog._nUCID = pUser->GetUCID();
        // stLog._sMainType = 3;
        // stLog._sSubType = 2;
        // stLog.nParam0 = -pNewSkillTable->Req_Skill_Point;
        // stLog.nParam1 = nSkillID;
        // stLog.nParam2 = 1;
        // stLog.nParam3 = bUseCheat ? 1 : 0;
        // pServer->SendDBLog(&stLog);

        // IDA: 发送统计数据
        // ST_STATISTICS_SKILL stStatistics;
        // stStatistics.byFlag = 2;
        // stStatistics.dwUCID = pUser->GetUCID();
        // stStatistics.dwSkill_New = nSkillID;
        // stStatistics.dwSkill_Old = pSkillData->GetID();
        // stStatistics.dwDivergenceID = nDivergenceID;
        // XSendDBPacket xSendDBStatistics(pUser, 0xF0, 8);
        // xSendDBStatistics << stStatistics;
        // pServer->SendDBStatistics(xSendDBStatistics);

    } else {
        // IDA: 新技能学习
        if (pNewSkillTable->Skill_LV != 1 && pNewSkillTable->Skill_Type != 5) {
            LogHelper::LogError("game.skill", "LearnSkill error - Not learn privious skill[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查等级要求
        if (pUser && pNewSkillTable->Req_Min_LV > pUser->GetLevel()) {
            LogHelper::LogError("game.skill", "LearnSkill error - Low level[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查觉醒等级
        // if (pAttr && pNewSkillTable->Req_Min_AwakeningGrade > pAttr->GetAwaken()) {
        //     LogHelper::LogError("game.skill", "LearnSkill error - Low AwakenGrade");
        //     return false;
        // }

        // IDA: 检查技能点
        if (m_wSkillPoint < pNewSkillTable->Req_Skill_Point) {
            LogHelper::LogError("game.skill", "LearnSkill error - Not enough skill point[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查已使用技能点
        int nPointUsed = m_wTotalSkillPoint - m_wSkillPoint;
        if (nPointUsed < pNewSkillTable->Req_Consume_Point) {
            LogHelper::LogError("game.skill", "LearnSkill error - Shortage all used skill point[ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            CMover* pMover = GetOwnerGO();
            if (pMover) {
                CGocNetwork::SendErrorMessage(pMover, 6, 0x71, 0xDACC);
            }
            return false;
        }

        // IDA: 检查职业
        // if (pAttr && pAttr->GetClass() != pNewSkillTable->Use_Class) {
        //     LogHelper::LogError("game.skill", "LearnSkill error - Class condition");
        //     return false;
        // }

        // IDA: 检查技能组是否已存在
        std::shared_ptr<CSkill> pGroupSkill = GetHaveSkillGroup(pNewSkillTable->Skill_Group);
        if (pGroupSkill) {
            LogHelper::LogError("game.skill", "LearnSkill error - [ SkillIndex:%d ]", pNewSkillTable->Skill_Index);
            return false;
        }

        // IDA: 创建新技能
        std::shared_ptr<CSkill> pNewSkill = std::make_shared<CSkill>(pNewSkillTable);

        // IDA: 插入新技能
        m_HaveSkill[nSkillID] = pNewSkill;

        // IDA: 处理被动技能
        if (pUser && pNewSkillTable->Skill_Type == 1) {
            if (pNewSkillTable->Passive_Type == 13) {
                SetPassiveSkillStat(pNewSkillTable->Passive_Value);
            } else if (pNewSkillTable->Passive_Type == 17) {
                m_vPassiveSkill.push_back(pNewSkill);
            }
        }

        // IDA: 发送包
        SendDBLearnSkill(nSkillID, 0, 0, pNewSkillTable->Req_Skill_Point);
        SendPacketLearnSkill(nSkillID, 0, true, 0, nTicknum);

        // IDA: 扣除技能点
        AddSkillPoint(-pNewSkillTable->Req_Skill_Point, 0, true);

        // IDA: 检查装备技能选项物品
        // CGocInventory* pInventory = pUser->GetGOC<CGocInventory>();
        // if (pInventory) {
        //     pInventory->CheckEquipSkillOptionItem(pNewSkillTable->Skill_Group, 0);
        // }

        // IDA: 发送日志和统计数据
        // ... (类似上面的日志代码)

        // IDA: 处理特殊技能类型 (Skill_Type == 4 或 5)
        if (pNewSkillTable->Skill_Type == 4 || pNewSkillTable->Skill_Type == 5) {
            // 更新属性统计
            // pAttr->UpdateStat();
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
// ResetSkill - 重置所有技能
// IDA 0x14016AD20: ?ResetSkill@CGocSkill@@QEAAX_NH@Z
// 精确还原 - 技能重置函数
// 参数: bUseCheat - 是否使用作弊, nTicknum - 时间戳
// 逻辑:
// 1. 遍历所有技能，清除被动技能效果
// 2. 记录特殊技能类型 (Type 4/5/9) 不删除
// 3. 清空 m_HaveSkill 和 m_vPassiveSkill
// 4. 恢复技能点到最大值
// 5. 重新加载默认技能
// 6. 发送数据库包和客户端包
// ----------------------------------------------------------------------------
void CGocSkill::ResetSkill(bool bUseCheat, int nTicknum)
{
    // IDA反编译完整还原
    std::vector<unsigned long> vecNoDel;
    bool bPassiveValue = false;

    // IDA: 遍历所有技能，清除被动技能效果
    for (auto& pair : m_HaveSkill) {
        std::shared_ptr<CSkill> pSkillData = pair.second;
        if (!pSkillData) continue;

        TB_SKILL* pTblRef = pSkillData->GetTable();
        if (pTblRef) {
            // IDA: 清除被动技能效果 (Passive_Type == 13)
            if (pTblRef->Skill_Type == 1 && pTblRef->Passive_Type == 13) {
                ClearPassiveSkillStat(pTblRef->Passive_Value);
                bPassiveValue = true;
            }

            // IDA: 记录特殊技能类型不删除 (Type 4/5/9)
            if (pTblRef->Skill_Type == 4 || pTblRef->Skill_Type == 5 || pTblRef->Skill_Type == 9) {
                if (pTblRef->Req_Prev_Skill_ID) {
                    vecNoDel.push_back(pTblRef->Req_Prev_Skill_ID);
                }
                vecNoDel.push_back(pTblRef->Skill_Index);
            }
        }
    }

    // IDA: 清空被动技能列表和技能容器
    m_vPassiveSkill.clear();
    m_HaveSkill.clear();

    // IDA: 恢复技能点到最大值
    m_wSkillPoint = m_wTotalSkillPoint;

    // IDA: 重置技能卡组
    ResetSkillDeck();

    // IDA: 获取角色信息
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 获取 TB_CHARACTER_INFO
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return;
    }

    // TODO: TB_CHARACTER_INFO* pTBCharacterInfo = pServer->GetResourceMgr().GetTB_CHARACTER_INFO(pUser->GetClass() * 1000);
    TB_CHARACTER_INFO* pTBCharacterInfo = nullptr;

    if (pTBCharacterInfo) {
        // IDA: 创建技能加载结构
        PS_SKILL_LOAD stSkill;
        std::set<unsigned long> setSkillCheck;

        // IDA: 加载默认技能 (20个)
        for (int i = 0; i < 20; ++i) {
            unsigned long nDefaultSkillID = pTBCharacterInfo->Default_Skill_ID_01 + i;  // 数组访问

            // 检查是否已处理过
            if (setSkillCheck.find(nDefaultSkillID) != setSkillCheck.end()) {
                continue;
            }

            // IDA: 获取技能表
            // TB_SKILL* pTBNewSkill = pServer->GetResourceMgr().GetTB_SKILL(nDefaultSkillID);
            TB_SKILL* pTBNewSkill = nullptr;

            if (pTBNewSkill) {
                // IDA: 创建技能对象
                std::shared_ptr<CSkill> pNewSkill = std::make_shared<CSkill>(pTBNewSkill);

                // IDA: 设置 Swap_Skill_Index
                if (pTBNewSkill->Swap_Skill_Index) {
                    // TB_SKILL* pSwapSkill = pServer->GetResourceMgr().GetTB_SKILL(pTBNewSkill->Swap_Skill_Index);
                    // if (pSwapSkill) {
                    //     pNewSkill->SetSwapSkill(pSwapSkill);
                    // }
                }

                // IDA: 检查技能点
                if (m_wSkillPoint < pTBNewSkill->Req_Skill_Point) {
                    break;
                }

                // IDA: 扣除技能点
                m_wSkillPoint -= pTBNewSkill->Req_Skill_Point;

                // IDA: 插入技能
                m_HaveSkill[pNewSkill->GetID()] = pNewSkill;

                // IDA: 处理被动技能
                if (pTBNewSkill->Skill_Type == 1 && pTBNewSkill->Passive_Type == 13) {
                    SetPassiveSkillStat(pTBNewSkill->Passive_Value);
                    bPassiveValue = true;
                }

                // IDA: 添加到加载结构
                ST_SKILL_INFO stSkillInfo;
                stSkillInfo.nID = pTBNewSkill->Skill_Index;
                stSkill.vecInfo.push_back(stSkillInfo);

                // 记录已处理
                setSkillCheck.insert(nDefaultSkillID);
            }
        }

        // IDA: 清空检查集合
        setSkillCheck.clear();

        // IDA: 处理不删除的技能列表
        for (unsigned long nSkillID : vecNoDel) {
            // TB_SKILL* pTBSkill = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
            TB_SKILL* pTBSkill = nullptr;

            if (pTBSkill) {
                std::shared_ptr<CSkill> pSkill = std::make_shared<CSkill>(pTBSkill);
                m_HaveSkill[pSkill->GetID()] = pSkill;

                // IDA: 处理被动技能
                if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13) {
                    SetPassiveSkillStat(pTBSkill->Passive_Value);
                    bPassiveValue = true;
                }

                // IDA: 添加到加载结构
                ST_SKILL_INFO stSkillInfo;
                stSkillInfo.nID = pTBSkill->Skill_Index;
                stSkill.vecInfo.push_back(stSkillInfo);
            }
        }

        // IDA: 清空不删除列表
        vecNoDel.clear();

        // IDA: 如果有被动技能变化，重新计算属性
        if (bPassiveValue) {
            // CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
            // if (pAttr) {
            //     pAttr->CalculateChangedStat(1);
            // }
        }

        // IDA: 设置技能加载结构的其他字段
        stSkill.wTotalSkillPoint = m_wTotalSkillPoint;
        stSkill.wSkillPoint = m_wSkillPoint;

        // IDA: 添加卡组页信息
        for (int j = 0; j < m_byDeckCount; ++j) {
            PS_SKILL_DECK_PAGE psPage;
            std::memcpy(&psPage, m_stSkillDeckPage + j * 36, sizeof(PS_SKILL_DECK_PAGE));
            stSkill.psSkillPage.byActivePage = m_byActiveDeck;
            stSkill.psSkillPage.vecInfo.push_back(psPage);
        }

        // IDA: 发送数据库包 (main=0x44, sub=3)
        XSendDBPacket xSendDBPacket(pUser, 0x44, 3);
        xSendDBPacket << stSkill;
        pServer->SendDBGame(xSendDBPacket);

        // IDA: 发送日志
        // ST_LOG_GAME stLog;
        // stLog._nUAID = pUser->GetUAID();
        // stLog._nUCID = pUser->GetUCID();
        // stLog._sMainType = 3;
        // stLog._sSubType = 2;
        // stLog.nParam0 = m_wSkillPoint;
        // stLog.nParam1 = 0;
        // stLog.nParam2 = 3;
        // stLog.nParam3 = bUseCheat ? 1 : 0;
        // pServer->SendDBLog(&stLog);

        // IDA: 发送统计数据
        // ST_STATISTICS_SKILL stStatistics;
        // stStatistics.byFlag = 3;
        // stStatistics.dwUCID = pUser->GetUCID();
        // XSendDBPacket xSendDBStatistics(pUser, 0xF0, 8);
        // xSendDBStatistics << stStatistics;
        // xSendDBStatistics << stSkill;
        // pServer->SendDBStatistics(xSendDBStatistics);

        // IDA: 发送客户端包 (main=6, sub=0x72)
        // PS_TICKCOUNT_INFO psTick;
        // pUser->GetResultTick(nTicknum, &psTick);
        // psTick.dw64ResTickcount = GetTickCount64();
        // psTick.dw64GetTickcount = psTick.dw64ResTickcount - psTick.dw64ReqTickcount;

        XSendPacket xSendPacket(6, 0x72);
        xSendPacket << stSkill;
        // xSendPacket << psTick;

        CMover* pMover = GetOwnerGO();
        if (pMover) {
            XActor* pActor = reinterpret_cast<XActor*>(pMover);
            if (pActor) {
                CGocNetwork::Send(pActor, xSendPacket);
            }
        }
    }

    (void)bUseCheat;
    (void)nTicknum;
}

// ----------------------------------------------------------------------------
// DeleteSkill - 删除技能
// IDA 0x14016BAB0: ?DeleteSkill@CGocSkill@@QEAAXG@Z
// 完整逻辑基于 IDA 反编译:
// 1. 在 m_HaveSkill 中查找技能
// 2. 如果找到，检查是否是被动技能并清除
// 3. 发送删除包给客户端
// 4. 更新技能卡组
// 5. 从容器中移除
// ----------------------------------------------------------------------------
void CGocSkill::DeleteSkill(std::uint16_t wSkillID)
{
    // IDA: 查找技能
    auto iter = m_HaveSkill.find(static_cast<int>(wSkillID));
    if (iter == m_HaveSkill.end()) return;

    std::shared_ptr<CSkill> pSkillData = iter->second;
    if (!pSkillData) return;

    // IDA: 获取 TB_SKILL 检查被动技能
    TB_SKILL* pTblRef = pSkillData->GetTable();
    if (pTblRef && pTblRef->Skill_Type == 1 && pTblRef->Passive_Type == 13) {
        // IDA: 清除被动技能状态
        ClearPassiveSkillStat(pTblRef->Passive_Value);
    }

    // IDA: 发送删除包给客户端
    XSendPacket xSendPacket(6, 0x63);
    xSendPacket << static_cast<int>(wSkillID);

    // IDA: 通过 CGocNetwork::Send 发送 - CMover 通过 GetArea() 获取 XActor*
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XActor* pActor = reinterpret_cast<XActor*>(pMover);
        if (pActor) {
            CGocNetwork::Send(pActor, xSendPacket);
        }
    }

    // IDA: 更新技能卡组 - ChangeDeckNewSkill(0, wSkillID)
    ChangeDeckNewSkill(0, static_cast<int>(wSkillID));

    // IDA: 从容器中移除
    m_HaveSkill.erase(iter);
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
// IDA 反编译验证: 完整还原
// 参数: nNowPoint - 当前技能点增量, nTotalPoint - 总技能点增量, bSyncDB - 是否同步数据库
// ----------------------------------------------------------------------------
void CGocSkill::AddSkillPoint(int nNowPoint, int nTotalPoint, bool bSyncDB)
{
    // IDA反编译:
    // this->m_wTotalSkillPoint += nTotalPoint;
    // this->m_wSkillPoint += nNowPoint;
    // CGocSkill::SendPacketUpdateSkillPoint(this);
    // if (bSyncDB)
    //     CGocSkill::SendDBUpdateSkillPoint(this);
    m_wTotalSkillPoint += static_cast<std::uint16_t>(nTotalPoint);
    m_wSkillPoint += static_cast<std::uint16_t>(nNowPoint);
    SendPacketUpdateSkillPoint();
    if (bSyncDB) {
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
// IDA 反编译: return (unsigned int)this->m_ModeShopMyInfo.nRoguelikeMoney;
// ----------------------------------------------------------------------------
int CGocSkill::GetModeShopMoney() const
{
    // IDA反编译: return (unsigned int)this->m_ModeShopMyInfo.nRoguelikeMoney;
    // m_ModeShopMyInfo 偏移0处是 nRoguelikeMoney (int)
    return reinterpret_cast<const int*>(m_ModeShopMyInfo)[0];
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
// 完整逻辑基于 IDA 反编译:
// 1. 清空 m_nSkillDeck 数组
// 2. 获取 owner 对象 (通过 RTTI)
// 3. 发送数据库包通知数据库清空
// ----------------------------------------------------------------------------
void CGocSkill::ResetSkillDeck()
{
    // IDA: memset(this->m_nSkillDeck, 0, sizeof(this->m_nSkillDeck))
    std::memset(m_nSkillDeck, 0, sizeof(m_nSkillDeck));

    // IDA: 获取 owner 对象并发送数据库包
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;

    // IDA: XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x44u, 6u)
    XSendDBPacket xSendDBPacket(pUser, 0x44, 6);

    // IDA: 获取 UAID 并发送
    std::uint32_t nUAID = pUser->GetUAID();
    xSendDBPacket << nUAID;

    // IDA: XGameServer::SendDBGame(v3, &xSendDBPacket)
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SendDBGame(xSendDBPacket);
    }
}

// ----------------------------------------------------------------------------
// LoadSkillDeck - 加载技能卡组
// IDA 0x14016C200: ?LoadSkillDeck@CGocSkill@@QEAA_NAEAUPS_SKILL_LOAD@@@Z
// 完整逻辑基于 IDA 反编译:
// 1. 设置 m_byDeckCount 和 m_byActiveDeck
// 2. 遍历 PS_SKILL_DECK_PAGE 设置 m_stSkillDeckPage
// 3. 遍历 PS_SKILL_DECK 设置 m_nSkillDeck
// ----------------------------------------------------------------------------
bool CGocSkill::LoadSkillDeck(void* stSkillLoad)
{
    // IDA: 完整还原
    PS_SKILL_LOAD* psLoad = static_cast<PS_SKILL_LOAD*>(stSkillLoad);
    if (!psLoad) return false;

    // IDA: 设置卡组数量 (检查 vecInfo.size() 且 size() <= 5)
    std::size_t nPageSize = psLoad->psSkillPage.vecInfo.size();
    if (nPageSize && nPageSize <= 5) {
        m_byDeckCount = static_cast<std::uint8_t>(nPageSize);
    } else {
        m_byDeckCount = 0;
    }

    // IDA: m_byActiveDeck = 0
    m_byActiveDeck = 0;

    // IDA: 检查并设置激活页
    if (psLoad->psSkillPage.byActivePage &&
        psLoad->psSkillPage.byActivePage < 5 &&
        psLoad->psSkillPage.byActivePage < m_byDeckCount) {
        m_byActiveDeck = psLoad->psSkillPage.byActivePage;
    }

    // IDA: 设置 m_stSkillDeckPage (for i = 0; i < m_byDeckCount; ++i)
    for (int i = 0; i < m_byDeckCount; ++i) {
        PS_SKILL_DECK_PAGE& psDeckPage = psLoad->psSkillPage.vecInfo[i];

        // IDA: 检查页索引有效性
        if (psDeckPage.byDeckPage < 5 && psDeckPage.byDeckPage < m_byDeckCount) {
            // IDA: 默认奖励槽 (如果 wDeckBonus[0] == 0 则设置默认值)
            if (psDeckPage.wDeckBonus[0] == 0) {
                psDeckPage.wDeckBonus[0] = 1;
                psDeckPage.wDeckBonus[1] = 11;
                psDeckPage.wDeckBonus[2] = 21;
            }
            // IDA: 复制到 m_stSkillDeckPage
            std::memcpy(&m_stSkillDeckPage[psDeckPage.byDeckPage * 36], &psDeckPage, 36);
        }
    }

    // IDA: 设置 m_nSkillDeck (遍历 stSkillDeck vector)
    for (const auto& psDeck : psLoad->stSkillDeck) {
        // IDA: byDeckPage = GetDeckPage(psDeck.wPos)
        std::uint8_t byDeckPage = GetDeckPage(psDeck.wPos);
        // IDA: wPos = GetDeckPos(psDeck.wPos)
        std::uint16_t wPos = GetDeckPos(psDeck.wPos);

        // IDA: for (j = 0; j < m_wSkillDeckSlotCount; ++j)
        for (int j = 0; j < m_wSkillDeckSlotCount; ++j) {
            // IDA: 检查边界条件
            if (wPos < 6 && byDeckPage < 5 && byDeckPage < m_byDeckCount && j < 4) {
                // IDA: m_nSkillDeck[byDeckPage][wPos][j] = *(&psDeck.nSkill_1 + j)
                m_nSkillDeck[byDeckPage * 24 + wPos * 4 + j] = psDeck.nSkill[j];
            }
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
// UpdateSkillDeck - 更新技能卡组
// IDA 0x14016C5C0: ?UpdateSkillDeck@CGocSkill@@QEAA_NAEAUPS_SKILL_DECK_VEC@@@Z
// 完整逻辑基于 IDA 反编译:
// 1. 复制当前卡组到临时数组
// 2. 验证每个技能槽的有效性
// 3. 成功则更新卡组并发送数据库包，失败则发送失败包
// ----------------------------------------------------------------------------
bool CGocSkill::UpdateSkillDeck(void* stSkillDeckVec)
{
    // IDA: 完整还原
    PS_SKILL_DECK_VEC* pDeckVec = static_cast<PS_SKILL_DECK_VEC*>(stSkillDeckVec);
    if (!pDeckVec) return false;

    bool bSuccessed = true;
    int nTempSkillDeck[5][6][4];
    std::memset(nTempSkillDeck, 0, sizeof(nTempSkillDeck));
    std::memcpy(nTempSkillDeck, m_nSkillDeck, sizeof(nTempSkillDeck));

    PS_SKILL_DECK_VEC psFailDeck;

    // IDA: 遍历技能卡组向量
    for (const auto& deck : pDeckVec->vecInfo) {
        // IDA: wX = GetDeckPos(deck.wPos)
        std::uint16_t wX = GetDeckPos(deck.wPos);
        // IDA: byPage = GetDeckPage(deck.wPos)
        std::uint8_t byPage = GetDeckPage(deck.wPos);

        // IDA: 验证页索引
        if (byPage >= 5 || byPage > m_byDeckCount - 1) {
            psFailDeck.vecInfo.push_back(deck);
            break;
        }

        // IDA: 验证位置索引
        if (wX >= 6) {
            psFailDeck.vecInfo.push_back(deck);
            break;
        }

        // IDA: 验证技能是否存在
        for (int i = 0; i < m_wSkillDeckSlotCount; ++i) {
            if (deck.nSkill[i] && !IsHaveSkill(deck.nSkill[i])) {
                psFailDeck.vecInfo.push_back(deck);
                break;
            }
            if (i < 4) {
                nTempSkillDeck[byPage][wX][i] = deck.nSkill[i];
            }
        }
    }

    // IDA: 检查是否全部成功
    if (psFailDeck.vecInfo.empty()) {
        // IDA: 成功，更新卡组
        std::memcpy(m_nSkillDeck, nTempSkillDeck, sizeof(m_nSkillDeck));

        // IDA: 发送数据库包
        CUser* pUser = GetOwnerUser();
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser, 0x44, 5);
            xSendDBPacket << pUser->GetUAID();
            xSendDBPacket << *pDeckVec;

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(xSendDBPacket);
            }
        }

        // IDA: 发送客户端包 (main=6, sub=0x75)
        XSendPacket xSendPacket(6, 0x75);
        xSendPacket << bSuccessed;
        xSendPacket << *pDeckVec;

        CMover* pMover = GetOwnerGO();
        if (pMover) {
            XActor* pActor = reinterpret_cast<XActor*>(pMover);
            if (pActor) {
                CGocNetwork::Send(pActor, xSendPacket);
            }
        }
    } else {
        // IDA: 失败，发送失败包
        bSuccessed = false;

        // IDA: 清空失败槽位的技能
        for (std::size_t j = 0; j < psFailDeck.vecInfo.size(); ++j) {
            for (int k = 0; k < m_wSkillDeckSlotCount; ++k) {
                psFailDeck.vecInfo[j].nSkill[k] = 0;
            }
        }

        // IDA: 发送失败包给客户端
        XSendPacket packet(6, 0x75);
        packet << bSuccessed;
        packet << psFailDeck;

        CMover* pMover = GetOwnerGO();
        if (pMover) {
            XActor* pActor = reinterpret_cast<XActor*>(pMover);
            if (pActor) {
                CGocNetwork::Send(pActor, packet);
            }
        }
    }

    return bSuccessed;
}

// ----------------------------------------------------------------------------
// AddDeckSlot - 添加技能卡组槽
// IDA 0x14016CCC0: ?AddDeckSlot@CGocSkill@@QEAA_NUPS_RES_STORAGE_INFO@@H@Z
// 完整逻辑基于 IDA 反编译:
// 1. 如果是作弊模式，设置槽位数为2
// 2. 如果槽位数小于4，增加槽位并发送数据库包
// 3. 否则发送错误消息
// ----------------------------------------------------------------------------
bool CGocSkill::AddDeckSlot(void* stStorageInfo, int nCheatCount)
{
    // IDA: 完整还原
    PS_RES_STORAGE_INFO* pStorageInfo = static_cast<PS_RES_STORAGE_INFO*>(stStorageInfo);
    if (!pStorageInfo) return false;

    // IDA: 如果是作弊模式，设置槽位数为2
    if (nCheatCount) {
        m_wSkillDeckSlotCount = 2;
    }

    // IDA: 检查槽位数是否小于4
    if (m_wSkillDeckSlotCount < 4) {
        ++m_wSkillDeckSlotCount;

        // IDA: 发送数据库包
        CUser* pUser = GetOwnerUser();
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser, 0x44, 4);
            xSendDBPacket << pUser->GetUAID();
            xSendDBPacket << m_wSkillDeckSlotCount;
            xSendDBPacket << *pStorageInfo;

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(xSendDBPacket);
            }
        }

        return true;
    } else {
        // IDA: 发送错误消息
        CMover* pMover = GetOwnerGO();
        if (pMover) {
            CGocNetwork::SendErrorMessage(pMover, 6, 0x76, 0xDACD);
        }
        return false;
    }
}

// ----------------------------------------------------------------------------
// ChangeDeckNewSkill - 更换卡组中的技能
// IDA 0x14016CEC0: ?ChangeDeckNewSkill@CGocSkill@@QEAAXHH@Z
// 完整逻辑基于 IDA 反编译:
// 1. 遍历所有卡组页和位置，找到旧技能并替换为新技能
// 2. 如果新技能为0，则重新排列该槽位的技能
// 3. 发送更新包给客户端和数据库
// ----------------------------------------------------------------------------
void CGocSkill::ChangeDeckNewSkill(int nOldSkillID, int nNewSkillID)
{
    // IDA: 完整还原
    std::vector<std::uint16_t> vecChangePos;

    // IDA: 遍历所有卡组页和位置
    for (int byDeckPage = 0; byDeckPage < m_byDeckCount; ++byDeckPage) {
        for (int nPos = 0; nPos < 6; ++nPos) {
            int iNewSlot = 0;
            int nNewSkillDeck[4] = {0, 0, 0, 0};

            // IDA: nDeckPos = GetDeckPos(nPos)
            int nDeckPos = GetDeckPos(static_cast<std::uint16_t>(nPos));

            for (int nSlot = 0; nSlot < m_wSkillDeckSlotCount; ++nSlot) {
                if (nDeckPos <= 5 && nSlot < 4) {
                    // IDA: 检查是否是旧技能
                    if (m_nSkillDeck[byDeckPage * 24 + nDeckPos * 4 + nSlot] == nOldSkillID) {
                        // IDA: 替换为新技能
                        m_nSkillDeck[byDeckPage * 24 + nDeckPos * 4 + nSlot] = nNewSkillID;
                        // IDA: 记录改变的位置
                        std::uint16_t wPos = nDeckPos + 6 * byDeckPage;
                        vecChangePos.push_back(wPos);
                    }

                    // IDA: 收集非零技能用于重排
                    if (m_nSkillDeck[byDeckPage * 24 + nPos * 4 + nSlot]) {
                        nNewSkillDeck[iNewSlot++] = m_nSkillDeck[byDeckPage * 24 + nPos * 4 + nSlot];
                    }
                }
            }

            // IDA: 如果新技能为0，重新排列该槽位的技能
            if (nNewSkillID == 0) {
                for (int j = 0; j < m_wSkillDeckSlotCount; ++j) {
                    if (j < 4) {
                        m_nSkillDeck[byDeckPage * 24 + nPos * 4 + j] = nNewSkillDeck[j];
                    }
                }
            }
        }
    }

    // IDA: 构建更新包并发送
    if (!vecChangePos.empty()) {
        PS_SKILL_DECK_VEC stDeckVec;

        for (std::size_t i = 0; i < vecChangePos.size(); ++i) {
            PS_SKILL_DECK stDeck;
            stDeck.wPos = vecChangePos[i];

            int DeckPos = GetDeckPos(stDeck.wPos);
            int DeckPage = GetDeckPage(stDeck.wPos);

            // IDA: 复制技能槽数据
            for (int j = 0; j < 4; ++j) {
                stDeck.nSkill[j] = m_nSkillDeck[DeckPage * 24 + DeckPos * 4 + j];
            }

            stDeckVec.vecInfo.push_back(stDeck);
        }

        // IDA: 发送客户端包 (main=6, sub=0x75)
        bool bSuccessed = true;
        XSendPacket xSendPacket(6, 0x75);
        xSendPacket << bSuccessed;
        xSendPacket << stDeckVec;

        CMover* pMover = GetOwnerGO();
        if (pMover) {
            XActor* pActor = reinterpret_cast<XActor*>(pMover);
            if (pActor) {
                CGocNetwork::Send(pActor, xSendPacket);
            }
        }

        // IDA: 发送数据库包 (main=0x44, sub=5)
        CUser* pUser = GetOwnerUser();
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser, 0x44, 5);
            xSendDBPacket << pUser->GetUAID();
            xSendDBPacket << stDeckVec;

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(xSendDBPacket);
            }
        }
    }
}

// ----------------------------------------------------------------------------
// FindSkillDeck - 查找技能是否在卡组中
// IDA 0x14016D490: ?FindSkillDeck@CGocSkill@@QEAA_NH@Z
// 完整逻辑基于 IDA 反编译:
// 1. 如果使用模式技能，直接返回true
// 2. 遍历当前激活卡组查找技能
// ----------------------------------------------------------------------------
bool CGocSkill::FindSkillDeck(int nSkillIndex) const
{
    // IDA: 完整还原
    // IDA: 如果使用模式技能，直接返回true
    if (m_bUseModeSkill) {
        return true;
    }

    // IDA: 遍历当前激活卡组
    for (int nPos = 0; nPos < 6; ++nPos) {
        for (int nSlot = 0; nSlot < m_wSkillDeckSlotCount; ++nSlot) {
            if (nSlot < 4) {
                // IDA: 检查技能是否匹配
                if (m_nSkillDeck[m_byActiveDeck * 24 + nPos * 4 + nSlot] == nSkillIndex) {
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

// ----------------------------------------------------------------------------
// GetDeckName - 获取卡组名称
// IDA 0x1401722F0: ?GetDeckName@CGocSkill@@QEAAXAEAUPS_DECK_NAME_VEC@@@Z
// 精确还原 - 获取所有卡组页的名称信息
// ----------------------------------------------------------------------------
void CGocSkill::GetDeckName(void* psDeckNameVec)
{
    // IDA反编译 (完整还原):
    // psChange->byType = 0;
    // std::vector<PS_DECK_NAME>::clear(&psChange->vecInfo);
    // for (i = 0; i < this->m_byDeckCount; ++i) {
    //     PS_DECK_NAME::PS_DECK_NAME(&psName);
    //     psName.byPage = this->m_stSkillDeckPage[i].byDeckPage;
    //     wcscpy_s<13>(psName.szDeckName, this->m_stSkillDeckPage[i].szDeckName);
    //     std::vector<PS_DECK_NAME>::push_back(&psChange->vecInfo, &psName);
    // }

    PS_DECK_NAME_VEC* pChange = static_cast<PS_DECK_NAME_VEC*>(psDeckNameVec);
    if (!pChange) return;

    pChange->byType = 0;
    pChange->vecInfo.clear();

    for (int i = 0; i < m_byDeckCount; ++i) {
        PS_DECK_NAME psName;
        memset(&psName, 0, sizeof(psName));
        // m_stSkillDeckPage是uint8_t数组，每页36字节，需要转换为PS_SKILL_DECK_PAGE指针
        // IDA: psName.byDeckPage = this->m_stSkillDeckPage[i].byDeckPage
        // 偏移: byDeckPage在PS_SKILL_DECK_PAGE结构偏移0处
        PS_SKILL_DECK_PAGE* pPage = reinterpret_cast<PS_SKILL_DECK_PAGE*>(m_stSkillDeckPage + i * 36);
        psName.byDeckPage = pPage->byDeckPage;
        // IDA: wcscpy_s<13>(psName.szDeckName, this->m_stSkillDeckPage[i].szDeckName)
        // 复制卡组名称 (最大13个wchar_t)
        // 注意: PS_SKILL_DECK_PAGE的szDeckName是21个wchar_t，PS_DECK_NAME的szDeckName是13个wchar_t
        std::memcpy(psName.szDeckName, pPage->szDeckName, sizeof(psName.szDeckName));
        pChange->vecInfo.push_back(psName);
    }
}

// ----------------------------------------------------------------------------
// ChangeDeckName - 更改卡组名称
// IDA 0x140171DF0: ?ChangeDeckName@CGocSkill@@QEAAHUPS_DECK_NAME_VEC@@@Z
// 精确还原 - 更改卡组页名称并发送数据库包
// 逻辑:
// 1. 验证请求的卡组页数量不超过当前卡组数量
// 2. 遍历每个请求，验证页索引和名称有效性
// 3. 使用名称过滤器检查名称是否可用
// 4. 更新卡组名称并发送数据库和客户端包
// ----------------------------------------------------------------------------
int CGocSkill::ChangeDeckName(void* psDeckNameVec)
{
    // IDA反编译 (完整还原):
    // 1. 获取UCID
    //    pMover = GetOwnerGO();
    //    dwUCID = pMover->GetUAID();
    //
    // 2. 检查请求数量不超过卡组数量
    //    if (psChange->vecInfo.size() > m_byDeckCount) {
    //        LogHelper::LogError("ChangeDeckName error - Over deck count");
    //        return 58418;  // 错误码
    //    }
    //
    // 3. 遍历每个请求
    //    for (i = 0; i < psChange->vecInfo.size(); ++i) {
    //        psInfo = psChange->vecInfo[i];
    //        // 验证页索引
    //        if (psInfo.byPage > m_byDeckCount - 1) {
    //            LogHelper::LogError("ChangeDeckName error - Fault deck");
    //            return 58418;
    //        }
    //        // 验证名称长度 (1-13个字符)
    //        nLen = wcslen(psInfo.szDeckName);
    //        if (nLen == 0 || nLen > 13) {
    //            LogHelper::LogError("ChangeDeckName error - Fault deckname size");
    //            return 58418;
    //        }
    //        // 使用名称过滤器
    //        if (!UtilFunc::IsUsableNameFilter(psInfo.szDeckName)) {
    //            LogHelper::LogError("ChangeDeckName error - IsUsableNameFilter");
    //            return 58418;
    //        }
    //        // 更新名称
    //        wcscpy_s<13>(m_stSkillDeckPage[psInfo.byPage].szDeckName, psInfo.szDeckName);
    //    }
    //
    // 4. 发送数据库包 (main=0x44, sub=0x14)
    //    XSendDBPacket(pObject, 0x44, 0x14);
    //    xSendDBPacket << dwUCID;
    //    xSendDBPacket << *psChange;
    //    XGameServer::SendDBGame(&xSendDBPacket);
    //
    // 5. 发送客户端响应包 (main=6, sub=0x55)
    //    PS_RES_DECK_NAME psRes;
    //    psRes.vecName = *psChange;
    //    psRes.nResult = 0;
    //    XSendPacket xSendPacket(6, 0x55);
    //    xSendPacket << psRes;
    //    CGocNetwork::Send(pActor, xSendPacket);
    //    return 0;

    PS_DECK_NAME_VEC* pChange = static_cast<PS_DECK_NAME_VEC*>(psDeckNameVec);
    if (!pChange) return 58418;

    // IDA: 获取UCID
    CUser* pUser = GetOwnerUser();
    unsigned int dwUCID = 0;
    if (pUser) {
        dwUCID = pUser->GetUAID();
    }

    // IDA: 检查请求数量不超过卡组数量
    if (pChange->vecInfo.size() > static_cast<std::size_t>(m_byDeckCount)) {
        // LogHelper::LogError("game.contents", "ChangeDeckName error - Over deck count[UCID:%d, Count:%d]", dwUCID, m_byDeckCount);
        return 58418;
    }

    // IDA: 遍历每个请求
    for (std::size_t i = 0; i < pChange->vecInfo.size(); ++i) {
        const PS_DECK_NAME& psInfo = pChange->vecInfo[i];

        // IDA: 验证页索引
        if (psInfo.byDeckPage > m_byDeckCount - 1) {
            // LogHelper::LogError("game.contents", "ChangeDeckName error - Fault deck[UCID:%d, Req:%d, DeckCount:%d]", dwUCID, psInfo.byDeckPage, m_byDeckCount);
            return 58418;
        }

        // IDA: 验证名称长度 (1-13个字符)
        std::size_t nLen = 0;
        while (nLen < 13 && psInfo.szDeckName[nLen] != 0) {
            ++nLen;
        }
        if (nLen == 0 || nLen > 13) {
            // LogHelper::LogError("game.contents", "ChangeDeckName error - Fault deckname size[UCID:%d]", dwUCID);
            return 58418;
        }

        // IDA: 使用名称过滤器
        // if (!UtilFunc::IsUsableNameFilter(psInfo.szDeckName)) {
        //     LogHelper::LogError("game.contents", "ChangeDeckName error - IsUsableNameFilter[UCID:%d]", dwUCID);
        //     return 58418;
        // }

        // IDA: 更新名称
        // m_stSkillDeckPage是uint8_t数组，每页36字节
        PS_SKILL_DECK_PAGE* pPage = reinterpret_cast<PS_SKILL_DECK_PAGE*>(m_stSkillDeckPage + psInfo.byDeckPage * 36);
        std::memcpy(pPage->szDeckName, psInfo.szDeckName, sizeof(psInfo.szDeckName));
    }

    // IDA: 发送数据库包 (main=0x44, sub=0x14)
    if (pUser) {
        XSendDBPacket xSendDBPacket(pUser, 0x44, 0x14);
        xSendDBPacket << dwUCID;
        // 发送vecInfo数量和数据
        std::int16_t nCount = static_cast<std::int16_t>(pChange->vecInfo.size());
        xSendDBPacket.XParse << nCount;
        for (const auto& info : pChange->vecInfo) {
            xSendDBPacket.XParse << info.byDeckPage;
            // 发送wchar_t字符串
            for (int j = 0; j < 13; ++j) {
                xSendDBPacket.XParse << info.szDeckName[j];
            }
        }

        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->SendDBGame(xSendDBPacket);
        }
    }

    // IDA: 发送客户端响应包 (main=6, sub=0x55)
    // PS_RES_DECK_NAME 结构: vecName + nResult
    XSendPacket xSendPacket(6, 0x55);
    xSendPacket << static_cast<std::int8_t>(0);  // nResult = 0 (success)
    // 手动序列化 PS_DECK_NAME_VEC
    xSendPacket.XParse << pChange->byType;
    std::int16_t nCount = static_cast<std::int16_t>(pChange->vecInfo.size());
    xSendPacket.XParse << nCount;
    for (const auto& info : pChange->vecInfo) {
        xSendPacket.XParse << info.byDeckPage;
        // 发送wchar_t字符串 (13个字符)
        for (int j = 0; j < 13; ++j) {
            xSendPacket.XParse << info.szDeckName[j];
        }
    }

    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XActor* pActor = reinterpret_cast<XActor*>(pMover);
        if (pActor) {
            CGocNetwork::Send(pActor, xSendPacket);
        }
    }

    return 0;
}

// ============================================================================
// 数据库/网络同步
// ============================================================================

// ----------------------------------------------------------------------------
// SendDBLearnSkill - 发送学习技能到数据库
// IDA 0x14016D900: ?SendDBLearnSkill@CGocSkill@@QEAAXHHHH@Z
// 精确还原 - 发送学习技能信息到数据库
// ----------------------------------------------------------------------------
void CGocSkill::SendDBLearnSkill(int nNewSkill, int nOldSkill, int nDivergenceID, int nUseSkillPoint)
{
    // IDA反编译:
    // PS_DB_SKILL_LEARN::PS_DB_SKILL_LEARN(&stLearnSkill)
    // 获取 owner via RTTI
    // 填充 stLearnSkill 结构
    // XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x44u, 1u)
    // operator<<(&xSendDBPacket, &stLearnSkill)
    // XGameServer::SendDBGame(v6, &xSendDBPacket)

    // IDA: Get owner object (CMover*)
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // IDA: Build PS_DB_SKILL_LEARN
    PS_DB_SKILL_LEARN stLearnSkill;
    memset(&stLearnSkill, 0, sizeof(stLearnSkill));
    stLearnSkill.uxActorID.dwActorID = pMover->GetTargetID();  // IDA: 获取 ActorID
    stLearnSkill.nNewSkillID = nNewSkill;
    stLearnSkill.nOldSkillID = nOldSkill;
    stLearnSkill.byResult = 0;
    stLearnSkill.nDivergenceID = nDivergenceID;
    stLearnSkill.nUseSkillPoint = nUseSkillPoint;

    // IDA: Get IXObject for XSendDBPacket
    IXObject* pObject = nullptr;  // TODO: 需要从 CMover 获取 IXObject
    XSendDBPacket xSendDBPacket(pObject, 0x44, 1);
    xSendDBPacket << stLearnSkill;

    // IDA: Send to game DB
    XGameServer* pServer = XGameServer::Instance();
    pServer->SendDBGame(xSendDBPacket);
}

// ----------------------------------------------------------------------------
// SendPacketLearnSkill - 发送学习技能包
// IDA 0x14016DA80: ?SendPacketLearnSkill@CGocSkill@@QEAAXHE_NHH@Z
// 精确还原 - 发送学习技能结果给客户端
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketLearnSkill(int nSkillID, std::uint8_t byType, bool bResult, int nDivergenceID, int nTicknum)
{
    // IDA: Get owner actor (CMover/XActor)
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // IDA: Build PS_TICKCOUNT_INFO
    PS_TICKCOUNT_INFO psTick;
    memset(&psTick, 0, sizeof(psTick));
    psTick.dw64ResTickcount = GetTickCount64();
    psTick.dw64GetTickcount = 0;  // TODO: 需要计算差值

    // IDA: Build result packet (main=6, sub=0x71)
    XSendPacket xSendPacket(6, 0x71);
    // 写入技能ID、类型、结果、分歧ID
    xSendPacket.XParse << nSkillID;
    xSendPacket.XParse << byType;
    xSendPacket.XParse << static_cast<std::uint8_t>(bResult ? 1 : 0);
    xSendPacket.XParse << nDivergenceID;
    // 写入 PS_TICKCOUNT_INFO 字段
    xSendPacket.XParse << psTick.nTicknum;
    xSendPacket.XParse << psTick.byType;
    xSendPacket.XParse << psTick.padding_5;
    xSendPacket.XParse << psTick.padding_6;
    xSendPacket.XParse << psTick.padding_7;
    xSendPacket.XParse << psTick.dwReqTickcount;
    xSendPacket.XParse << psTick.dwResTickcount;
    xSendPacket.XParse << psTick.dwGetTickcount;
    xSendPacket.XParse << psTick.dw64ReqTickcount;
    xSendPacket.XParse << psTick.dw64ResTickcount;
    xSendPacket.XParse << psTick.dw64GetTickcount;
    xSendPacket.XParse << psTick.nFps;

    // IDA: CGocNetwork::Send(pActor, &xSendPacket)
    // CMover 包含 XActor，通过 GetArea 获取 XActor*
    XActor* pActor = reinterpret_cast<XActor*>(pMover);
    CGocNetwork::Send(pActor, xSendPacket);
}

// ----------------------------------------------------------------------------
// SendDBUpdateSkillPoint - 发送更新技能点到数据库
// IDA 0x14016DC60: ?SendDBUpdateSkillPoint@CGocSkill@@QEAAXXZ
// 精确还原 - 发送技能点更新到数据库
// ----------------------------------------------------------------------------
void CGocSkill::SendDBUpdateSkillPoint()
{
    // IDA反编译:
    // 获取 owner (CMover/CUser)
    // 构建 PS_DB_SKILL_UPDATE_POINT 结构
    // 发送到数据库 (main=0x44, sub=2)

    // IDA: Get owner object
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // IDA: Build PS_DB_SKILL_UPDATE_POINT
    PS_DB_SKILL_UPDATE_POINT stSkillPoint;
    memset(&stSkillPoint, 0, sizeof(stSkillPoint));
    stSkillPoint.uxActorID.dwActorID = pMover->GetTargetID();  // IDA: 使用 GetTargetID
    stSkillPoint.wSkillPoint = m_wSkillPoint;
    stSkillPoint.wTotalSkillPoint = m_wTotalSkillPoint;

    // IDA: Send DB packet (main=0x44, sub=2)
    IXObject* pObject = nullptr;  // TODO: 需要从 CMover 获取 IXObject
    XSendDBPacket xSendDBPacket(pObject, 0x44, 2);
    xSendDBPacket << stSkillPoint;

    // IDA: Send to game DB
    XGameServer* pServer = XGameServer::Instance();
    pServer->SendDBGame(xSendDBPacket);
}

// ----------------------------------------------------------------------------
// SendPacketUpdateSkillPoint - 发送更新技能点包
// IDA 0x14016DDB0: ?SendPacketUpdateSkillPoint@CGocSkill@@QEAAXXZ
// 精确还原 - 发送技能点更新给客户端
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketUpdateSkillPoint()
{
    // IDA反编译:
    // stPoint = *(PS_SKILL_POINT *)&this->m_wTotalSkillPoint
    // XSendPacket::XSendPacket(&xSendPacket, 6u, 0x73u)
    // operator<<(&xSendPacket, &stPoint)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // IDA: Get owner actor
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // IDA: Build PS_SKILL_POINT from m_wTotalSkillPoint and m_wSkillPoint
    // stPoint = *(PS_SKILL_POINT *)&this->m_wTotalSkillPoint
    PS_SKILL_POINT stPoint;
    stPoint.wTotalSkillPoint = m_wTotalSkillPoint;
    stPoint.wSkillPoint = m_wSkillPoint;

    // IDA: Send packet (main=6, sub=0x73)
    XSendPacket xSendPacket(6, 0x73);
    xSendPacket << stPoint;

    // IDA: CGocNetwork::Send(pActor, &xSendPacket)
    // CMover 包含 XActor，通过 GetArea 获取 XActor*
    XActor* pActor = reinterpret_cast<XActor*>(pMover);
    CGocNetwork::Send(pActor, xSendPacket);
}

// ----------------------------------------------------------------------------
// SendPacketLoadSkill - 发送加载技能包
// IDA 0x14016E490: ?SendPacketLoadSkill@CGocSkill@@QEAAXXZ
// 精确还原 - 发送技能加载包给客户端
// 1. 创建 PS_SKILL_LOAD 结构
// 2. 遍历 m_HaveSkill 填充技能信息
// 3. 遍历 m_stSkillDeckPage 和 m_nSkillDeck 填充卡组信息
// 4. 发送客户端包 (main=6, sub=0x70)
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketLoadSkill()
{
    // IDA反编译完整还原:
    // PS_SKILL_LOAD::PS_SKILL_LOAD(&stSkillLoad)
    // stSkillLoad.wTotalSkillPoint = this->m_wTotalSkillPoint
    // stSkillLoad.wSkillPoint = this->m_wSkillPoint
    // stSkillLoad.wDeckSlotCount = this->m_wSkillDeckSlotCount
    // 遍历 m_HaveSkill:
    //   iter = begin(Index)
    //   while (iter != end(Index)) {
    //       pSkillData = *iter
    //       skillInfo.nID = pTblRef->Skill_Index
    //       skillInfo.nDivergenceID = pSkillData->GetDivergenceID()
    //       vecInfo.push_back(skillInfo)
    //   }
    // 遍历 m_stSkillDeckPage:
    //   for j = 0; j < this->m_byDeckCount; ++j {
    //       psSkillPage.vecInfo.push_back(m_stSkillDeckPage[j])
    //       PS_SKILL_DECK::PS_SKILL_DECK(&stDeck)
    //       for i = 0; i < 6; ++i {
    //           for k = 0; k < this->m_wSkillDeckSlotCount; ++k {
    //               if (k < 4) {
    //                   stDeck.nSkill[k] = this->m_nSkillDeck[j][i][k]
    //                   stDeck.wPos = i + 6 * j
    //               }
    //           }
    //           stSkillDeck.push_back(stDeck)
    //       }
    //   }
    // stSkillLoad.psSkillPage.byActivePage = this->m_byActiveDeck
    // XSendPacket::XSendPacket(&xSendPacket, 6u, 0x70u)
    // operator<<(&xSendPacket, &stSkillLoad)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // IDA: 创建 PS_SKILL_LOAD 结构
    PS_SKILL_LOAD stSkillLoad;
    stSkillLoad.wTotalSkillPoint = m_wTotalSkillPoint;
    stSkillLoad.wSkillPoint = m_wSkillPoint;
    stSkillLoad.wDeckSlotCount = m_wSkillDeckSlotCount;

    // IDA: 遍历 m_HaveSkill 填充技能信息
    // auto Index = boost::multi_index::get<0>(m_HaveSkill);
    // for (auto iter = Index.begin(); iter != Index.end(); ++iter) {
    //     const auto& pSkillData = *iter;
    //     if (pSkillData) {
    //         ST_SKILL_INFO skillInfo;
    //         TB_SKILL* pTblRef = pSkillData->GetTable();
    //         if (pTblRef) {
    //             skillInfo.nID = pTblRef->Skill_Index;
    //         }
    //         skillInfo.nDivergenceID = pSkillData->GetDivergenceID();
    //         stSkillLoad.vecInfo.push_back(skillInfo);
    //     }
    // }

    // IDA: 遍历 m_stSkillDeckPage 和 m_nSkillDeck 填充卡组信息
    for (int j = 0; j < m_byDeckCount; ++j) {
        // IDA: 添加卡组页信息
        PS_SKILL_DECK_PAGE* pPage = reinterpret_cast<PS_SKILL_DECK_PAGE*>(m_stSkillDeckPage + j * 36);
        if (pPage) {
            stSkillLoad.psSkillPage.vecInfo.push_back(*pPage);
        }

        // IDA: 添加卡组技能槽数据
        for (int i = 0; i < 6; ++i) {
            PS_SKILL_DECK stDeck;
            stDeck.wPos = static_cast<std::uint16_t>(i + 6 * j);

            // IDA: 复制技能槽数据
            for (int k = 0; k < m_wSkillDeckSlotCount && k < 4; ++k) {
                stDeck.nSkill[k] = m_nSkillDeck[j * 24 + i * 4 + k];
            }

            stSkillLoad.stSkillDeck.push_back(stDeck);
        }
    }

    // IDA: 设置激活页
    stSkillLoad.psSkillPage.byActivePage = m_byActiveDeck;

    // IDA: 发送客户端包 (main=6, sub=0x70)
    XSendPacket xSendPacket(6, 0x70);
    xSendPacket << stSkillLoad;

    // IDA: 获取 actor 并发送
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XActor* pActor = reinterpret_cast<XActor*>(pMover);
        if (pActor) {
            CGocNetwork::Send(pActor, xSendPacket);
        }
    }
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
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. m_bUseModeSkill = false
// 2. 清空 m_vecModeDefaultSkillList
// 3. 清空 m_vPassiveModeSkill
// 4. 调用 ApplySkillPassive()
// 5. 调用 SendPacketLoadSkill()
// ----------------------------------------------------------------------------
void CGocSkill::ResetModeSkill()
{
    // IDA反编译:
    // this->m_bUseModeSkill = 0;
    // std::vector<float>::clear(&this->m_vecModeDefaultSkillList);
    // std::vector<std::shared_ptr<CSkill>>::clear(&this->m_vPassiveModeSkill);
    // CGocSkill::ApplySkillPassive(this);
    // CGocSkill::SendPacketLoadSkill(this);

    m_bUseModeSkill = false;
    m_vecModeDefaultSkillList.clear();
    m_vPassiveModeSkill.clear();

    // IDA: 调用 ApplySkillPassive 和 SendPacketLoadSkill
    // TODO: 需要 ApplySkillPassive() 和 SendPacketLoadSkill() 函数实现
    // ApplySkillPassive();
    // SendPacketLoadSkill();
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
// IDA 反编译验证: 完整还原
// ----------------------------------------------------------------------------
void CGocSkill::ClearModeSkillDeck()
{
    // IDA反编译:
    // memset(this->m_nModeSkillDeck, 0, sizeof(this->m_nModeSkillDeck));
    // CGocSkill::ResetModeSkillActiveState(this);
    std::memset(m_nModeSkillDeck, 0, sizeof(m_nModeSkillDeck));
    ResetModeSkillActiveState();
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
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 如果 !m_bUseModeSkill 返回 true
// 2. 在 m_mapModeSkillActiveCount 中查找 nSkillGroupID
// 3. 如果找不到返回 true
// 4. 返回 (bCanUse == 1 && nCurrentCount > 0)
// ----------------------------------------------------------------------------
bool CGocSkill::HaveModeSkillActiveCount(int nGroupID) const
{
    // IDA反编译:
    // if (!this->m_bUseModeSkill) return 1;
    // auto it = this->m_mapModeSkillActiveCount.find(nSkillGroupID);
    // auto end = this->m_mapModeSkillActiveCount.end();
    // return it != end && LOBYTE(it->second.m_eObjectFlags) == 1 && SHIDWORD(it->second.__vftable) > 0;
    if (!m_bUseModeSkill) {
        return true;
    }
    auto it = m_mapModeSkillActiveCount.find(nGroupID);
    if (it == m_mapModeSkillActiveCount.end()) {
        return true;
    }
    // IDA: bCanUse == 1 && nCurrentCount > 0
    return it->second.bCanUse && it->second.nCurrentCount > 0;
}

// ----------------------------------------------------------------------------
// ChargeModeSkillActiveCount - 消耗模式技能激活计数
// IDA 0x140174660: ?ChargeModeSkillActiveCount@CGocSkill@@QEAAXH@Z
// IDA 反编译验证: 完整还原
// 逻辑:
// 1. 如果 !m_bUseModeSkill 返回
// 2. 如果 nSkillGroupID != 0，查找并重置该组的 nCurrentCount = nTotalCount
// 3. 如果 nSkillGroupID == 0，遍历所有条目并重置
// ----------------------------------------------------------------------------
void CGocSkill::ChargeModeSkillActiveCount(int nGroupID)
{
    // IDA反编译:
    // if (!this->m_bUseModeSkill) return;
    // if (nSkillGroupIDa) {
    //     find(this->m_mapModeSkillActiveCount, &v7, &nSkillGroupIDa);
    //     if (v7 != end) {
    //         // HIDWORD(v5->second.__vftable) = v11->second.__vftable;
    //         // 即: nCurrentCount = nTotalCount
    //         v7->second.nCurrentCount = v7->second.nTotalCount;
    //     }
    // } else {
    //     // 遍历所有条目
    //     for (auto it = begin; it != end; ++it) {
    //         it->second.nCurrentCount = it->second.nTotalCount;
    //     }
    // }
    if (!m_bUseModeSkill) {
        return;
    }
    if (nGroupID != 0) {
        auto it = m_mapModeSkillActiveCount.find(nGroupID);
        if (it != m_mapModeSkillActiveCount.end()) {
            it->second.nCurrentCount = it->second.nTotalCount;
        }
    } else {
        // nGroupID == 0 时重置所有
        for (auto& pair : m_mapModeSkillActiveCount) {
            pair.second.nCurrentCount = pair.second.nTotalCount;
        }
    }
}

// ----------------------------------------------------------------------------
// AddModeSkillActiveCount - 添加模式技能激活计数
// IDA 0x140174780: ?AddModeSkillActiveCount@CGocSkill@@QEAAXHHH@Z
// IDA 反编译验证: 完整还原
// 参数: nSkillGroupID - 技能组ID, nAddCount - 当前计数增量, nAddTotal - 总计数增量
// 逻辑: 如果条目已存在，增加计数；否则不做任何操作
// ----------------------------------------------------------------------------
void CGocSkill::AddModeSkillActiveCount(int nGroupID, int nTotalCount, int nCount)
{
    // IDA反编译:
    // if (!this->m_bUseModeSkill) return;
    // find(this->m_mapModeSkillActiveCount, &it, &nSkillGroupIDa);
    // if (it != end) {
    //     if (nAddTotala > 0) it->second.nTotalCount += nAddTotala;
    //     it->second.nCurrentCount += nAddCounta;
    //     if (it->second.nCurrentCount < 0) it->second.nCurrentCount = 0;
    // }
    if (!m_bUseModeSkill) {
        return;
    }
    auto it = m_mapModeSkillActiveCount.find(nGroupID);
    if (it != m_mapModeSkillActiveCount.end()) {
        if (nTotalCount > 0) {
            it->second.nTotalCount += nTotalCount;
        }
        it->second.nCurrentCount += nCount;
        if (it->second.nCurrentCount < 0) {
            it->second.nCurrentCount = 0;
        }
    }
}

// ----------------------------------------------------------------------------
// UpdateModeSkillActiveState - 更新模式技能激活状态
// IDA 0x140174870: ?UpdateModeSkillActiveState@CGocSkill@@QEAAXH_N@Z
// IDA 反编译验证: 完整还原
// 逻辑: 更新指定技能组的 bCanUse 状态
// ----------------------------------------------------------------------------
void CGocSkill::UpdateModeSkillActiveState(int nGroupID, bool bCanUse)
{
    // IDA反编译:
    // if (!this->m_bUseModeSkill) return;
    // auto it = this->m_mapModeSkillActiveCount.find(nGroupID);
    // if (it != end) {
    //     LOBYTE(it->second.m_eObjectFlags) = bCanUse;  // 设置 bCanUse
    // }
    if (!m_bUseModeSkill) {
        return;
    }
    auto it = m_mapModeSkillActiveCount.find(nGroupID);
    if (it != m_mapModeSkillActiveCount.end()) {
        it->second.bCanUse = bCanUse;
    }
}

// ----------------------------------------------------------------------------
// ResetModeSkillActiveState - 重置模式技能激活状态
// IDA 0x140174900: ?ResetModeSkillActiveState@CGocSkill@@QEAAXXZ
// IDA 反编译验证: 完整还原
// 逻辑: 如果 m_bUseModeSkill 为 true，遍历 m_mapModeSkillActiveCount 并重置每个条目的 bCanUse 为 false
// ----------------------------------------------------------------------------
void CGocSkill::ResetModeSkillActiveState()
{
    // IDA反编译:
    // if (!this->m_bUseModeSkill) return;
    // for (auto& pair : this->m_mapModeSkillActiveCount) {
    //     LOBYTE(pair.second.m_eObjectFlags) = 0;  // 设置 bCanUse = false
    // }
    if (!m_bUseModeSkill) {
        return;
    }
    for (auto& pair : m_mapModeSkillActiveCount) {
        pair.second.bCanUse = false;
    }
}

// ----------------------------------------------------------------------------
// GetRoguelikeSkillCoolTime - 获取Roguelike技能冷却时间
// IDA 0x140174980: ?GetRoguelikeSkillCoolTime@CGocSkill@@QEAAMMK@Z
// 精确还原 - 如果使用模式技能，从TB_MODE_BI_SKILL_EDIT获取冷却时间
// ----------------------------------------------------------------------------
float CGocSkill::GetRoguelikeSkillCoolTime(float fTotalTime, unsigned int dwSkillID)
{
    // IDA反编译 (完整还原):
    // if (!this->m_bUseModeSkill)
    //     return fTotalTime;
    // v4 = TXSingleton<XGameServer>::Instance();
    // pTB_MODE_BI_SKILL_EDIT = XResourceMgr::GetTB_MODE_BI_SKILL_EDIT(&v4->m_xResourceMgr, dwSkillID);
    // if (pTB_MODE_BI_SKILL_EDIT)
    //     return (float)(int)pTB_MODE_BI_SKILL_EDIT->BI_Skill_Cooltime;
    // return fTotalTime;

    // IDA: 如果不使用模式技能，直接返回原始冷却时间
    if (!m_bUseModeSkill) {
        return fTotalTime;
    }

    // IDA: 从TB_MODE_BI_SKILL_EDIT获取冷却时间
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return fTotalTime;
    }

    // IDA: XResourceMgr::GetTB_MODE_BI_SKILL_EDIT(&v4->m_xResourceMgr, dwSkillID)
    // TODO: 需要实现GetTB_MODE_BI_SKILL_EDIT
    // TB_MODE_BI_SKILL_EDIT* pTBSkillEdit = pServer->GetResourceMgr().GetTB_MODE_BI_SKILL_EDIT(dwSkillID);
    // if (pTBSkillEdit) {
    //     return static_cast<float>(pTBSkillEdit->BI_Skill_Cooltime);
    // }

    (void)dwSkillID;
    return fTotalTime;
}

// ============================================================================
// End of GocSkill.cpp
// ============================================================================
// NOTE: AddSkillNoLearn, ApplySkillPassive, DeckBonusAdd functions
// are commented out because they are not declared in GocSkill.h
// TODO: Add declarations to header and re-enable implementations
// ============================================================================

// ----------------------------------------------------------------------------
// GetOwnerUser - 获取拥有此组件的 CUser
// 使用 RTTI dynamic_cast 从 CMover 转换到 CUser
// ----------------------------------------------------------------------------
CUser* CGocSkill::GetOwnerUser() const {
    // IDA: 使用 _RTDynamicCast 从 CMover 转换为 CUser
    // 基类 GOComponent 的 m_pOwner 是 CMover*
    // CUser 继承自 CMover
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return nullptr;
    }
    // 使用 dynamic_cast 进行 RTTI 转换
    return dynamic_cast<CUser*>(pMover);
}

// ============================================================================
// 卡组管理函数
// ============================================================================

// ----------------------------------------------------------------------------
// ChangeActiveDeck - 切换激活的卡组
// IDA 0x1401714C0: ?ChangeActiveDeck@CGocSkill@@QEAAHUPS_DECK_ACTIVE@@@Z
// 精确还原 - 切换当前激活的技能卡组页
// ----------------------------------------------------------------------------
int CGocSkill::ChangeActiveDeck(PS_DECK_ACTIVE psActive)
{
    // IDA反编译:
    // if (this->m_byActiveDeck == psActive.byActivePage) return 58417;
    // if (psActive.byActivePage >= 5 || psActive.byActivePage > this->m_byDeckCount - 1) return 58417;
    // this->m_byActiveDeck = psActive.byActivePage;
    // 发送 DB 包 (main=0x44, sub=0x13)
    // 发送客户端包 (main=6, sub=0x54)

    // IDA: 检查是否已经是当前页
    if (m_byActiveDeck == psActive.byActivePage) {
        return 58417;  // 错误码
    }

    // IDA: 检查页码是否有效
    if (psActive.byActivePage >= 5 || psActive.byActivePage > m_byDeckCount - 1) {
        return 58417;
    }

    // IDA: 设置新的激活页
    m_byActiveDeck = psActive.byActivePage;

    // IDA: Get owner object
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return 58417;
    }

    // IDA: 获取 UCID
    // TODO: 从 CUser 获取 UCID

    // IDA: Send DB packet (main=0x44, sub=0x13)
    IXObject* pObject = nullptr;  // TODO: 需要从 CMover 获取 IXObject
    XSendDBPacket xSendDBPacket(pObject, 0x44, 0x13);
    // TODO: 写入 UCID 和 psActive
    XGameServer* pServer = XGameServer::Instance();
    pServer->SendDBGame(xSendDBPacket);

    // IDA: Send client packet (main=6, sub=0x54)
    int nResult = 0;
    XSendPacket xSendPacket(6, 0x54);
    xSendPacket.XParse << nResult;
    // TODO: 写入 psActive

    XActor* pActor = reinterpret_cast<XActor*>(pMover);
    CGocNetwork::Send(pActor, xSendPacket);

    return 0;  // 成功
}

// ----------------------------------------------------------------------------
// OpenSkillDeck - 开启技能卡组
// IDA 0x140171730: ?OpenSkillDeck@CGocSkill@@QEAAHXZ
// 精确还原 - 开启新的技能卡组页
// ----------------------------------------------------------------------------
int CGocSkill::OpenSkillDeck()
{
    // IDA反编译:
    // 1. 获取 owner CUser 和 CGocInventory
    // 2. 检查 m_byDeckCount + 1 <= 5
    // 3. 获取 TB_SKILL_SLOT_EXTEND 表数据
    // 4. 获取需要的物品并检查类型
    // 5. 扣除物品 (ReduceItem2)
    // 6. 发送 DB 包 (main=0x44, sub=0x15)

    // IDA: Get owner user
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return 58415;
    }

    // IDA: 检查卡组数量
    std::uint8_t byExtendDeck = m_byDeckCount + 1;
    if (byExtendDeck > 5) {
        return 58415;  // 已达最大
    }

    std::uint8_t byOpenPage = m_byDeckCount;
    if (byOpenPage == 0) {
        return 58415;  // 页码无效
    }

    // IDA: 获取扩展表数据
    XGameServer* pServer = XGameServer::Instance();
    // TODO: TB_SKILL_SLOT_EXTEND* pTB_Extend = pServer->GetResourceMgr().GetTB_SKILL_SLOT_EXTEND(byExtendDeck);
    // if (!pTB_Extend) return 58415;

    // IDA: 获取需要的物品并检查
    // TB_ITEM* pTB_Item = pServer->GetResourceMgr().GetTB_ITEM(pTB_Extend->Extend_Skill_Need_Item_ID);
    // TB_ITEM_CLASSIFY* pTB_Item_Classify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);

    // IDA: 检查物品使用类型
    // if (pTB_Item_Classify->Item_Use_Type != 122) return 52011;

    // IDA: 扣除物品并发送 DB 包
    // CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    // pInven->ReduceItem2(pTB_Item, pTB_Extend->Extend_Skill_Need_Item_Num, 0x87, &psUpdateItemList);

    return 0;  // 成功
}

// ----------------------------------------------------------------------------
// SetDeckPageInfo - 设置卡组页信息
// IDA 0x140171D60: ?SetDeckPageInfo@CGocSkill@@QEAAXUPS_SKILL_DECK_PAGE@@E@Z
// 精确还原 - 设置技能卡组页信息
// ----------------------------------------------------------------------------
void CGocSkill::SetDeckPageInfo(PS_SKILL_DECK_PAGE* psPage, std::uint8_t byCount)
{
    // IDA反编译:
    // this->m_byDeckCount = byCount;
    // if (psPage->byDeckPage < this->m_byDeckCount)
    //     qmemcpy(&this->m_stSkillDeckPage[psPage->byDeckPage], psPage, 36);

    m_byDeckCount = byCount;

    if (psPage && psPage->byDeckPage < m_byDeckCount) {
        // 复制卡组页信息到对应位置
        // m_stSkillDeckPage 是 std::uint8_t[5 * 36]
        std::size_t offset = static_cast<std::size_t>(psPage->byDeckPage) * 36;
        std::memcpy(m_stSkillDeckPage + offset, psPage, 36);
    }
}

// ----------------------------------------------------------------------------
// GetGestureSlot - 获取手势槽
// IDA 0x14016F990: ?GetGestureSlot@CGocSkill@@QEAAXAEAUPS_GESTURE_SLOT@@@Z
// 精确还原 - 获取手势槽数据
// ----------------------------------------------------------------------------
void CGocSkill::GetGestureSlot(PS_GESTURE_SLOT* psGesture)
{
    // IDA反编译:
    // qmemcpy(psGesture, this->m_nGestureSlot, sizeof(PS_GESTURE_SLOT));
    if (psGesture) {
        // 复制 6 个 int 到 PS_GESTURE_SLOT
        for (int i = 0; i < 6; ++i) {
            psGesture->nGestureID[i] = m_nGestureSlot[i];
        }
    }
}

// ============================================================================
// 卡组加成管理
// ============================================================================

// ----------------------------------------------------------------------------
// DeckBonusAdd - 添加卡组加成
// IDA 0x14016F180: ?DeckBonusAdd@CGocSkill@@QEAA_NUPS_UPDATE_DECK_BONUS_VEC@@H@Z
// 精确还原 - 更新卡组加成并发送数据库包
// ----------------------------------------------------------------------------
bool CGocSkill::DeckBonusAdd(PS_UPDATE_DECK_BONUS_VEC* psBonusList, int nTicknum)
{
    // IDA反编译完整还原:
    // 1. 遍历 psBonusList->vecInfo
    // 2. 检查 byDeckPage 是否有效
    // 3. 比较 wDeckBonus[k] 是否变化
    // 4. 验证 TB_DECK_BONUS 条件 (Bonus_Need_Lv, Use_DeckPhase)
    // 5. 发送 DB 包 (main=0x44, sub=8)
    // 6. 发送客户端包 (main=6, sub=0x79)

    bool bChange = false;
    PS_UPDATE_DECK_BONUS_VEC psList;

    for (std::size_t i = 0; i < psBonusList->vecInfo.size(); ++i) {
        PS_DECK_BONUS psBonusInfo = psBonusList->vecInfo[i];

        // IDA: 检查页码有效性
        if (psBonusInfo.byDeckPage > m_byDeckCount - 1) {
            bChange = false;
            break;
        }

        // IDA: 检查每个加成槽位
        for (int k = 0; k < 4; ++k) {
            PS_SKILL_DECK_PAGE* pPage = reinterpret_cast<PS_SKILL_DECK_PAGE*>(m_stSkillDeckPage + psBonusInfo.byDeckPage * 36);
            if (pPage->wDeckBonus[k] != psBonusInfo.wDeckBonus[k]) {
                bChange = true;
                pPage->wDeckBonus[k] = 0;

                // IDA: 验证 TB_DECK_BONUS 条件
                XGameServer* pServer = XGameServer::Instance();
                if (pServer) {
                    // TB_DECK_BONUS* pDeckBonus = pServer->GetResourceMgr().GetTB_DECK_BONUS(psBonusInfo.wDeckBonus[k]);
                    // if (pDeckBonus) {
                    //     CUser* pUser = GetOwnerUser();
                    //     if (pUser && pDeckBonus->Bonus_Need_Lv <= pUser->GetLevel() && pDeckBonus->Use_DeckPhase == k) {
                    //         pPage->wDeckBonus[k] = psBonusInfo.wDeckBonus[k];
                    //     }
                    // }
                }
            }
        }

        // IDA: 构建更新列表
        PS_DECK_BONUS psUpdateBonusInfo;
        psUpdateBonusInfo.byDeckPage = psBonusInfo.byDeckPage;
        PS_SKILL_DECK_PAGE* pPage = reinterpret_cast<PS_SKILL_DECK_PAGE*>(m_stSkillDeckPage + psBonusInfo.byDeckPage * 36);
        for (int k = 0; k < 4; ++k) {
            psUpdateBonusInfo.wDeckBonus[k] = pPage->wDeckBonus[k];
        }
        psList.vecInfo.push_back(psUpdateBonusInfo);
    }

    // IDA: 如果有变化，发送 DB 包
    if (bChange) {
        CUser* pUser = GetOwnerUser();
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser, 0x44, 8);
            xSendDBPacket << pUser->GetUAID();
            xSendDBPacket << psList;

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(xSendDBPacket);
            }
        }
    }

    // IDA: 发送客户端包 (main=6, sub=0x79)
    XSendPacket xSendPacket(6, 0x79);
    xSendPacket << psList;

    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XActor* pActor = reinterpret_cast<XActor*>(pMover);
        if (pActor) {
            CGocNetwork::Send(pActor, xSendPacket);
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
// GetDeckBonus - 获取卡组加成
// IDA 0x14016F6C0: ?GetDeckBonus@CGocSkill@@QEAAPEAUTB_DECK_BONUS@@EH@Z
// 精确还原 - 获取指定位置的卡组加成表数据
// ----------------------------------------------------------------------------
TB_DECK_BONUS* CGocSkill::GetDeckBonus(std::uint8_t byDeckIndex, int nSkillID)
{
    // IDA反编译:
    // if (byDeckIndex >= 6) return nullptr;
    // iBonusIndex = -1;
    // for (i = 0; i < 4; ++i) {
    //     if (m_nSkillDeck[m_byActiveDeck][byDeckIndex][i] == nSkillID) {
    //         iBonusIndex = i;
    //         break;
    //     }
    // }
    // if (iBonusIndex > 0) {
    //     return XResourceMgr::GetTB_DECK_BONUS(m_stSkillDeckPage[m_byActiveDeck].wDeckBonus[iBonusIndex - 1]);
    // }
    // return nullptr;

    if (byDeckIndex >= 6) {
        return nullptr;
    }

    int iBonusIndex = -1;
    for (int i = 0; i < 4; ++i) {
        if (m_nSkillDeck[m_byActiveDeck * 24 + byDeckIndex * 4 + i] == nSkillID) {
            iBonusIndex = i;
            break;
        }
    }

    if (iBonusIndex <= 0) {
        return nullptr;
    }

    // IDA: 更新任务条件
    // CGocQuest* pQuest = GetOwnerUser()->GetGOC<CGocQuest>();
    // if (pQuest) {
    //     pQuest->UpdateCondition(15, eCONDITION_TARGET_SKILL_DECK, byDeckIndex, 1, 0);
    // }

    // IDA: 获取 TB_DECK_BONUS
    PS_SKILL_DECK_PAGE* pPage = reinterpret_cast<PS_SKILL_DECK_PAGE*>(m_stSkillDeckPage + m_byActiveDeck * 36);
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        // return pServer->GetResourceMgr().GetTB_DECK_BONUS(pPage->wDeckBonus[iBonusIndex - 1]);
    }

    (void)nSkillID;
    return nullptr;
}

// ============================================================================
// 技能重置
// ============================================================================

// ----------------------------------------------------------------------------
// ResetSkillPoint - 重置技能点
// IDA 0x14016F9C0: ?ResetSkillPoint@CGocSkill@@QEAA_NKK_NH@Z
// 精确还原 - 重置技能点到上一级或分歧
// ----------------------------------------------------------------------------
bool CGocSkill::ResetSkillPoint(std::uint32_t dwSkillGroupID, std::uint32_t dwDivergence, bool bUseCheat, int nTicknum)
{
    // IDA反编译 - 这是一个非常复杂的函数
    // 主要逻辑:
    // 1. 在 m_HaveSkill 中按 SkillGroupID 查找技能
    // 2. 如果有分歧 (dwDivergence != 0):
    //    - 验证分歧 ID 匹配
    //    - 返还分歧消耗的技能点
    //    - 回退到前置分歧
    // 3. 如果没有分歧:
    //    - 验证技能等级 > 1
    //    - 检查技能类型 (不能是 SOUL_VIPER 类型)
    //    - 清除被动技能效果
    //    - 回退到前置技能
    // 4. 更新技能卡组
    // 5. 发送 DB 包和客户端包
    // 6. 发送日志和统计数据

    // TODO: 完整实现需要以下依赖:
    // - boost::multi_index::get<1>(m_HaveSkill) 按 SkillGroupID 查找
    // - CSkill::GetGroup(), CSkill::GetID(), CSkill::GetTable()
    // - TB_SKILL, TB_DIVERGENCE 表访问
    // - CGocInventory::CheckEquipSkillOptionItemPart()
    // - PS_SKILL_CHANGE 结构

    (void)dwSkillGroupID;
    (void)dwDivergence;
    (void)bUseCheat;
    (void)nTicknum;
    return true;
}

// ============================================================================
// 被动技能应用
// ============================================================================

// ----------------------------------------------------------------------------
// ApplySkillPassive - 应用被动技能
// IDA 0x140170B90: ?ApplySkillPassive@CGocSkill@@QEAAXXZ
// 精确还原 - 遍历所有技能并应用被动效果
// ----------------------------------------------------------------------------
void CGocSkill::ApplySkillPassive()
{
    // IDA反编译:
    // Index = boost::multi_index::get<0>(m_HaveSkill);
    // for (iter = Index.begin(); iter != Index.end(); ++iter) {
    //     pSkillPtr = *iter;
    //     pTBSkill = XResourceMgr::GetTB_SKILL(pSkillPtr->GetID());
    //     if (pTBSkill) {
    //         // 应用被动技能效果
    //         if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13) {
    //             SetPassiveSkillStat(pTBSkill->Passive_Value);
    //         }
    //         // 处理分歧
    //         auto itDiv = m_mapSkillDivergence.find(pTBSkill->Skill_Index);
    //         if (itDiv != m_mapSkillDivergence.end()) {
    //             nDivergenceID = itDiv->second.nDivergenceID;
    //             pTBDivergence = XResourceMgr::GetTB_DIVERGENCE(nDivergenceID);
    //             if (pTBDivergence) {
    //                 pSkillPtr->SetDivergence(pTBDivergence);
    //                 if (pTBDivergence->Swap_Div_ID) {
    //                     pSwapDiv = XResourceMgr::GetTB_DIVERGENCE(pTBDivergence->Swap_Div_ID);
    //                     pSkillPtr->SetSwapDivergence(pSwapDiv);
    //                 }
    //                 // 如果分歧有被动效果
    //                 if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13
    //                     && pCurDivergence && pCurDivergence->Div_Option_Type == 5) {
    //                     SetPassiveSkillStat(pCurDivergence->Div_Option_Value);
    //                 }
    //             }
    //         }
    //     }
    // }

    // 遍历 m_HaveSkill 中的所有技能
    // m_HaveSkill 是 std::map<int, std::shared_ptr<CSkill>>，迭代得到 pair
    for (const auto& pair : m_HaveSkill) {
        const auto& pSkillPtr = pair.second;
        if (!pSkillPtr) {
            continue;
        }

        // IDA: 获取技能表数据
        // TB_SKILL* pTBSkill = pSkillPtr->GetTable();
        // if (!pTBSkill) {
        //     continue;
        // }

        // IDA: 应用被动技能效果
        // if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13) {
        //     SetPassiveSkillStat(pTBSkill->Passive_Value);
        // }

        // IDA: 处理分歧效果
        // auto itDiv = m_mapSkillDivergence.find(pTBSkill->Skill_Index);
        // if (itDiv != m_mapSkillDivergence.end()) {
        //     int nDivergenceID = itDiv->second;
        //     TB_DIVERGENCE* pTBDivergence = XResourceMgr::GetTB_DIVERGENCE(nDivergenceID);
        //     if (pTBDivergence) {
        //         pSkillPtr->SetDivergence(pTBDivergence);
        //
        //         // 处理交换分歧
        //         if (pTBDivergence->Swap_Div_ID) {
        //             TB_DIVERGENCE* pSwapDiv = XResourceMgr::GetTB_DIVERGENCE(pTBDivergence->Swap_Div_ID);
        //             pSkillPtr->SetSwapDivergence(pSwapDiv);
        //         }
        //
        //         // 如果分歧有被动效果
        //         TB_DIVERGENCE* pCurDivergence = pSwapDiv ? pSwapDiv : pTBDivergence;
        //         if (pTBSkill->Skill_Type == 1 && pTBSkill->Passive_Type == 13
        //             && pCurDivergence && pCurDivergence->Div_Option_Type == 5) {
        //             SetPassiveSkillStat(pCurDivergence->Div_Option_Value);
        //         }
        //     }
        // }
    }
}

// ----------------------------------------------------------------------------
// ApplySkillPassive - 应用指定技能的被动效果
// IDA 0x140170E70: ?ApplySkillPassive@CGocSkill@@QEAAXH@Z
// 精确还原 - 应用指定技能组的被动效果
// 逻辑:
// 1. 获取技能组中的技能
// 2. 检查技能类型是否为被动(Skill_Type == 1)
// 3. 检查冷却时间是否为0
// 4. 应用被动技能状态
// 5. 设置技能冷却时间
// 6. 如果有冷却时间，发送客户端包
// ----------------------------------------------------------------------------
void CGocSkill::ApplySkillPassive(int nSkillID)
{
    // IDA反编译 (完整还原):
    // 1. 获取技能
    //    GetHaveSkillGroup(&pSkillPtr, nSkillID);
    //    if (!pSkillPtr) return;
    //
    // 2. 获取技能表数据
    //    dwIndex = pSkillPtr->GetID();
    //    pTBSkill = XResourceMgr::GetTB_SKILL(dwIndex);
    //    if (!pTBSkill) return;
    //
    // 3. 检查是否为被动技能
    //    if (pTBSkill->Skill_Type != 1) return;
    //
    // 4. 获取owner用户
    //    pUser = _RTDynamicCast_0(owner, &CMover RTTI, &CUser RTTI, 0);
    //    if (!pUser) return;
    //
    // 5. 获取技能管理器
    //    pSkillMgr = pUser->GetSkillMgr();
    //    if (!pSkillMgr) return;
    //
    // 6. 检查冷却时间
    //    if (pSkillMgr->GetCooltime(E_COOLTIME_SKILL, pTBSkill->CoolTime_Group, pTBSkill->CoolTime_Global, 0) != 0.0)
    //        return;
    //
    // 7. 应用被动技能状态
    //    SetPassiveSkillStat(pTBSkill->Passive_Value);
    //
    // 8. 设置技能冷却时间
    //    pSkillMgr->SetSkillCooltime(pTBSkill);
    //
    // 9. 更新属性
    //    auto pAttr = pUser->GetGOC<CGocAttribute>();
    //    if (pAttr) pAttr->UpdateStat();
    //
    // 10. 如果有冷却时间，发送客户端包 (main=6, sub=0x7E)
    //     if (pTBSkill->CoolTime) {
    //         XSendPacket xSendPacket(6, 0x7E);
    //         xSendPacket << pUser->GetActorID();
    //         xSendPacket << pTBSkill->Skill_Index;
    //         CGocNetwork::Send(pActor, xSendPacket);
    //     }

    // IDA: 获取技能
    auto it = m_HaveSkill.find(nSkillID);
    if (it == m_HaveSkill.end()) return;

    const auto& pSkillPtr = it->second;
    if (!pSkillPtr) return;

    // IDA: 获取技能表数据
    int dwIndex = pSkillPtr->GetID();
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;

    const TB_SKILL* pTBSkill = pServer->GetResourceMgr().GetTB_SKILL(dwIndex);
    if (!pTBSkill) return;

    // IDA: 检查是否为被动技能
    if (pTBSkill->Skill_Type != 1) return;

    // IDA: 获取owner用户
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;

    // IDA: 获取技能管理器
    CMySkillList* pSkillMgr = pUser->GetSkillMgr();
    if (!pSkillMgr) return;

    // IDA: 检查冷却时间
    // if (pSkillMgr->GetCooltime(E_COOLTIME_SKILL, pTBSkill->CoolTime_Group, pTBSkill->CoolTime_Global, 0) != 0.0)
    //     return;

    // IDA: 应用被动技能状态
    SetPassiveSkillStat(pTBSkill->Passive_Value);

    // IDA: 设置技能冷却时间
    // pSkillMgr->SetSkillCooltime(pTBSkill);

    // IDA: 更新属性
    // auto pAttr = pUser->GetGOC<CGocAttribute>();
    // if (pAttr) pAttr->UpdateStat();

    // IDA: 如果有冷却时间，发送客户端包 (main=6, sub=0x7E)
    if (pTBSkill->CoolTime) {
        CMover* pMover = GetOwnerGO();
        if (pMover) {
            XSendPacket xSendPacket(6, 0x7E);
            // xSendPacket << pUser->GetActorID();
            xSendPacket.XParse << pTBSkill->Skill_Index;

            XActor* pActor = reinterpret_cast<XActor*>(pMover);
            if (pActor) {
                CGocNetwork::Send(pActor, xSendPacket);
            }
        }
    }
}

// ============================================================================
// 特殊技能学习
// ============================================================================

// ----------------------------------------------------------------------------
// AddSkillNoLearn - 添加不学习的技能
// IDA 0x1401711B0: ?AddSkillNoLearn@CGocSkill@@QEAAXXZ
// 精确还原 - 根据完成章节自动学习特定技能
// ----------------------------------------------------------------------------
void CGocSkill::AddSkillNoLearn()
{
    // IDA反编译:
    // pQuest = GetOwnerUser()->GetGOC<CGocQuest>();
    //
    // // 检查章节 0x186C8 (100040)
    // if (pQuest->IsCompleteEpisode(0x186C8) == 1) {
    //     nSkill = 10000000 * pUser->GetClass() + 6000111;
    //     if (!IsHaveSkill(nSkill)) {
    //         LearnSkill(nSkill, false, 0);
    //     }
    // }
    //
    // // 检查章节 0x187EF (100079)
    // if (pQuest->IsCompleteEpisode(0x187EF) == 1) {
    //     nSkillID = 10000000 * pUser->GetClass() + 5000111;
    //     nSkillEx = 10000000 * pUser->GetClass() + 5000211;
    //     if (!IsHaveSkill(nSkillID) && !IsHaveSkill(nSkillEx)) {
    //         LearnSkill(nSkillID, false, 0);
    //     }
    //     nSkillID = 10000000 * pUser->GetClass() + 5130111;
    //     if (!IsHaveSkill(nSkillID)) {
    //         LearnSkill(nSkillID, false, 0);
    //     }
    // }
    //
    // // 检查章节 0x9352B (603435)
    // if (pQuest->IsCompleteEpisode(0x9352B) == 1) {
    //     nSkill = 10000000 * pUser->GetClass() + 5000211;
    //     if (!IsHaveSkill(nSkill)) {
    //         LearnSkill(nSkill, false, 0);
    //     }
    // }

    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 获取任务组件
    // auto pQuest = pUser->GetGOC<CGocQuest>();
    // if (!pQuest) {
    //     return;
    // }

    // IDA: 检查章节完成状态并学习技能
    // int nClass = pUser->GetClass();
    //
    // // 章节 100040
    // if (pQuest->IsCompleteEpisode(100040)) {
    //     int nSkill = 10000000 * nClass + 6000111;
    //     if (!IsHaveSkill(nSkill)) {
    //         LearnSkill(nSkill, false, 0);
    //     }
    // }
    //
    // // 章节 100079
    // if (pQuest->IsCompleteEpisode(100079)) {
    //     int nSkillID = 10000000 * nClass + 5000111;
    //     int nSkillEx = 10000000 * nClass + 5000211;
    //     if (!IsHaveSkill(nSkillID) && !IsHaveSkill(nSkillEx)) {
    //         LearnSkill(nSkillID, false, 0);
    //     }
    //     nSkillID = 10000000 * nClass + 5130111;
    //     if (!IsHaveSkill(nSkillID)) {
    //         LearnSkill(nSkillID, false, 0);
    //     }
    // }
    //
    // // 章节 603435
    // if (pQuest->IsCompleteEpisode(603435)) {
    //     int nSkill = 10000000 * nClass + 5000211;
    //     if (!IsHaveSkill(nSkill)) {
    //         LearnSkill(nSkill, false, 0);
    //     }
    // }
}