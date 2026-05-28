#include "GocSkill.h"
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
// ----------------------------------------------------------------------------
bool CGocSkill::IsHaveBaseSkill(unsigned int dwSkillIndex) const
{
    // 获取技能表
    // IDA: v2 = TXSingleton<XGameServer>::Instance()
    //      pTB_Skill = XResourceMgr::GetTB_SKILL(&v2->m_xResourceMgr, dwSkillIndex)
    // TODO: 需要XResourceMgr和TB_SKILL结构
    (void)dwSkillIndex;
    return false;
}

// ----------------------------------------------------------------------------
// IsHaveSkillQuickSlot - 检查技能是否在快捷槽
// IDA 0x1401688B0: ?IsHaveSkillQuickSlot@CGocSkill@@QEAA_NK@Z
// 逻辑:
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
    // IDA反编译逻辑:
    // if (!pTB_Skill) return 0;
    // if (pTB_Skill->Use_Position == 2 || dwSkillIndex == 30000) return 1;
    // if ((pTB_Skill->Skill_Type == 1 || pTB_Skill->Skill_Type == 2) && pTB_Skill->Passive_Type) return 1;
    // if (pTB_Skill->Skill_Type == 4 || pTB_Skill->Skill_Type == 5 || pTB_Skill->Skill_Type == 6
    //     || pTB_Skill->Skill_Type == 7 || pTB_Skill->Skill_Type == 9) return 1;
    // if (pTB_Skill->Use_State == 1) return 1;
    // if (FindSkillDeck(dwSkillIndex) && HaveModeSkillActiveCount(pTB_Skill->Skill_Group)) return 1;
    // return pTB_Skill->Swap_Skill_Index && FindSkillDeck(pTB_Skill->Swap_Skill_Index)
    //        && HaveModeSkillActiveCount(pTB_Skill->Skill_Group);

    // TODO: 需要完整的TB_SKILL结构和XResourceMgr
    (void)dwSkillIndex;
    return false;
}

// ----------------------------------------------------------------------------
// IsHaveSkill - 检查是否拥有技能
// IDA 0x14016BC80: ?IsHaveSkill@CGocSkill@@QEAA_NH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::IsHaveSkill(int nSkillID) const
{
    // IDA反编译: 检查技能是否在m_HaveSkill或m_HaveModeSkill中
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
// ----------------------------------------------------------------------------
bool CGocSkill::LoadSkill(void* stSkillLoad)
{
    // IDA反编译显示复杂逻辑:
    // 1. 设置m_wTotalSkillPoint, m_wSkillPoint, m_wSkillDeckSlotCount
    // 2. 遍历vecInfo, 为每个技能创建CSkill对象
    // 3. 插入到m_HaveSkill容器
    // 4. 处理Passive技能
    // 5. 处理Divergence
    // 6. 调用LoadSkillDeck
    // TODO: 需要PS_SKILL_LOAD结构定义
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
// ----------------------------------------------------------------------------
std::shared_ptr<CSkill> CGocSkill::GetHaveSkillGroup(int nSkillGroup)
{
    // IDA反编译: 遍历m_HaveSkill或m_HaveModeSkill, 查找匹配Skill_Group的技能
    if (m_bUseModeSkill) {
        for (const auto& pair : m_HaveModeSkill) {
            // TODO: 需要CSkill::GetGroup()方法
            // if (pair.second && pair.second->GetGroup() == nSkillGroup)
            //     return pair.second;
            (void)nSkillGroup;
        }
    } else {
        for (const auto& pair : m_HaveSkill) {
            // TODO: 需要CSkill::GetGroup()方法
            // if (pair.second && pair.second->GetGroup() == nSkillGroup)
            //     return pair.second;
            (void)nSkillGroup;
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
// 参数是wBuffID而非wSkillGroup
// ----------------------------------------------------------------------------
void CGocSkill::SetPassiveSkillStat(std::uint16_t wBuffID)
{
    // IDA反编译:
    // pUser = _RTDynamicCast(this, 0, &CMover RTTI, &CUser RTTI, 0)
    // if (pUser) {
    //     pBuffTable = XResourceMgr::GetTB_BUFF(..., wBuffID)
    //     if (pBuffTable) {
    //         if (pBuffTable->EffectType_01 || pBuffTable->Buff_Time)
    //             pUser->SetBuffStatus(&pUser->CMoverEx, wBuffID, 0, 1)
    //         else {
    //             if (pBuffTable->EffectType_Status_01)
    //                 pUser->SetBuffAbility(..., pBuffTable->EffectType_Status_01)
    //             if (pBuffTable->EffectType_Status_02)
    //                 pUser->SetBuffAbility(..., pBuffTable->EffectType_Status_02)
    //             if (pBuffTable->EffectType_Status_03)
    //                 pUser->SetBuffAbility(..., pBuffTable->EffectType_Status_03)
    //         }
    //     }
    // }
    // TODO: 需要CUser, TB_BUFF, XResourceMgr等依赖
    (void)wBuffID;
}

// ----------------------------------------------------------------------------
// ClearPassiveSkillStat - 清除被动技能状态
// IDA 0x14016D730: ?ClearPassiveSkillStat@CGocSkill@@QEAAXG@Z
// ----------------------------------------------------------------------------
void CGocSkill::ClearPassiveSkillStat(std::uint16_t wBuffID)
{
    // IDA反编译: 类似SetPassiveSkillStat但调用ClearBuffStatus/ClearBuffAbility
    // pUser = _RTDynamicCast(...)
    // if (pUser) {
    //     pBuffTable = XResourceMgr::GetTB_BUFF(..., wBuffID)
    //     if (pBuffTable) {
    //         if (pBuffTable->EffectType_01)
    //             pUser->ClearBuffStatus(..., wBuffID, 1, 0)
    //         else {
    //             if (pBuffTable->EffectType_Status_01)
    //                 pUser->ClearBuffAbility(..., pBuffTable->EffectType_Status_01)
    //             // ... 类似处理Status_02, Status_03
    //         }
    //     }
    // }
    // TODO: 需要CUser, TB_BUFF, XResourceMgr等依赖
    (void)wBuffID;
}

void CGocSkill::CheckPassiveSkill(std::uint8_t byType)
{
    // IDA 0x14016DEA0 - 大型函数，遍历被动技能并处理
    // TODO: 需要IDA反编译确认实现
    (void)byType;
}

// ============================================================================
// 技能卡组
// ============================================================================

// ----------------------------------------------------------------------------
// ResetSkillDeck - 重置技能卡组
// IDA 0x14016C0D0: ?ResetSkillDeck@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::ResetSkillDeck()
{
    // IDA反编译:
    // memset(this->m_nSkillDeck, 0, sizeof(this->m_nSkillDeck))
    // v7 = std::list<CBattleZone*>::size(this)  // 获取owner
    // if (v7) pObject = &v7[3].m_ChunkSizeTempMemOfs
    // else pObject = nullptr
    // XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x44u, 6u)
    // ...发送DB包...
    // XGameServer::SendDBGame(v3, &xSendDBPacket)
    std::memset(m_nSkillDeck, 0, sizeof(m_nSkillDeck));
    // TODO: 发送数据库包清空卡组
}

bool CGocSkill::LoadSkillDeck(void* stSkillLoad)
{
    // IDA 0x14016C200: ?LoadSkillDeck@CGocSkill@@QEAA_NAEAUPS_SKILL_LOAD@@@Z
    // TODO: 需要IDA反编译确认实现
    (void)stSkillLoad;
    return true;
}

bool CGocSkill::UpdateSkillDeck(void* stSkillDeckVec)
{
    // IDA 0x14016C5C0: ?UpdateSkillDeck@CGocSkill@@QEAA_NAEAUPS_SKILL_DECK_VEC@@@Z
    // TODO: 需要IDA反编译确认实现
    (void)stSkillDeckVec;
    return true;
}

bool CGocSkill::AddDeckSlot(void* stStorageInfo, int nIndex)
{
    // IDA 0x14016CCC0: ?AddDeckSlot@CGocSkill@@QEAA_NUPS_RES_STORAGE_INFO@@H@Z
    // TODO: 需要IDA反编译确认实现
    (void)stStorageInfo;
    (void)nIndex;
    return true;
}

void CGocSkill::ChangeDeckNewSkill(int nOldSkillID, int nNewSkillID)
{
    // IDA 0x14016CEC0: ?ChangeDeckNewSkill@CGocSkill@@QEAAXHH@Z
    // TODO: 需要IDA反编译确认实现
    (void)nOldSkillID;
    (void)nNewSkillID;
}

// ----------------------------------------------------------------------------
// FindSkillDeck - 查找技能卡组
// IDA 0x14016D490: ?FindSkillDeck@CGocSkill@@QEAA_NH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::FindSkillDeck(int nSkillIndex)
{
    // IDA反编译:
    // if (this->m_bUseModeSkill) return 1
    // for (nPos = 0; nPos < 6; ++nPos) {
    //     for (nSlot = 0; nSlot < this->m_wSkillDeckSlotCount; ++nSlot) {
    //         if (nSlot < 4 && this->m_nSkillDeck[this->m_byActiveDeck][nPos][nSlot] == nSkillIndex)
    //             return 1
    //     }
    // }
    // return 0
    if (m_bUseModeSkill) {
        return true;
    }
    for (int nPos = 0; nPos < 6; ++nPos) {
        for (int nSlot = 0; nSlot < m_wSkillDeckSlotCount; ++nSlot) {
            if (nSlot < 4 && m_nSkillDeck[m_byActiveDeck * 24 + nPos * 4 + nSlot] == nSkillIndex) {
                return true;
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
// 参数: nNewSkill, nOldSkill, nDivergenceID, nUseSkillPoint
// ----------------------------------------------------------------------------
void CGocSkill::SendDBLearnSkill(int nNewSkill, int nOldSkill, int nDivergenceID, int nUseSkillPoint)
{
    // IDA反编译: 发送PS_DB_SKILL_LEARN到数据库
    // stLearnSkill.nNewSkillID = nNewSkill
    // stLearnSkill.nOldSkillID = nOldSkill
    // stLearnSkill.byResult = 0
    // stLearnSkill.nDivergenceID = nDivergenceID
    // stLearnSkill.nUseSkillPoint = nUseSkillPoint
    // XSendDBPacket(main=0x44, sub=1)
    // TODO: 需要XSendDBPacket, PS_DB_SKILL_LEARN等结构
    (void)nNewSkill;
    (void)nOldSkill;
    (void)nDivergenceID;
    (void)nUseSkillPoint;
}

// ----------------------------------------------------------------------------
// SendPacketLearnSkill - 发送学习技能包
// IDA 0x14016DA80: ?SendPacketLearnSkill@CGocSkill@@QEAAXHE_NHH@Z
// 参数: nSkillID, byType, bResult, nDivergenceID, nTicknum
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketLearnSkill(int nSkillID, std::uint8_t byType, bool bResult, int nDivergenceID, int nTicknum)
{
    // IDA反编译: 发送PS_RES_SKILL_LEARN给客户端
    // stResult.nID = nSkillID
    // stResult.bySkillType = byType
    // stResult.bResult = bResult
    // stResult.nDivergenceID = nDivergenceID
    // XSendPacket(main=6, sub=0x71)
    // TODO: 需要XSendPacket, PS_RES_SKILL_LEARN等结构
    (void)nSkillID;
    (void)byType;
    (void)bResult;
    (void)nDivergenceID;
    (void)nTicknum;
}

// ----------------------------------------------------------------------------
// SendDBUpdateSkillPoint - 发送更新技能点到数据库
// IDA 0x14016DC60: ?SendDBUpdateSkillPoint@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::SendDBUpdateSkillPoint()
{
    // IDA反编译: 发送PS_DB_SKILL_UPDATE_POINT到数据库
    // stSkillPoint.wSkillPoint = this->m_wSkillPoint
    // stSkillPoint.wTotalSkillPoint = this->m_wTotalSkillPoint
    // XSendDBPacket(main=0x44, sub=2)
    // TODO: 需要XSendDBPacket, PS_DB_SKILL_UPDATE_POINT等结构
}

// ----------------------------------------------------------------------------
// SendPacketUpdateSkillPoint - 发送更新技能点包
// IDA 0x14016DDB0: ?SendPacketUpdateSkillPoint@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketUpdateSkillPoint()
{
    // IDA反编译: 发送PS_SKILL_POINT给客户端
    // stPoint包含m_wTotalSkillPoint和m_wSkillPoint
    // XSendPacket(main=6, sub=0x73)
    // TODO: 需要XSendPacket, PS_SKILL_POINT等结构
}

// ----------------------------------------------------------------------------
// SendPacketLoadSkill - 发送加载技能包
// IDA 0x14016E490: ?SendPacketLoadSkill@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::SendPacketLoadSkill()
{
    // IDA反编译: 大型函数，构建PS_SKILL_LOAD结构并发送
    // 遍历m_HaveSkill，构建技能信息列表
    // 遍历m_stSkillDeckPage和m_nSkillDeck，构建卡组信息
    // XSendPacket(main=6, sub=0x70)
    // TODO: 需要完整的结构定义
}

// ============================================================================
// 分歧系统
// ============================================================================

// ----------------------------------------------------------------------------
// LearnDivergence - 学习分歧
// IDA 0x14016E840: ?LearnDivergence@CGocSkill@@QEAA_NHHH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::LearnDivergence(int nSkillID, int nDivergenceID, int nReason)
{
    // IDA反编译: 大型函数，处理技能分歧学习
    // TODO: 需要IDA反编译确认实现
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
    // IDA反编译: 超大型函数，初始化roguelike模式技能
    // 1. 获取角色职业信息
    // 2. 设置m_bUseModeSkill = 1
    // 3. 清空m_HaveModeSkill
    // 4. 从TB_MODE_BI_CLASS_STARTSKILL加载初始技能
    // 5. 从TB_MODE_BI_UPGRADE加载升级技能
    // 6. 发送技能加载包
    // TODO: 需要大量依赖结构
}

// ----------------------------------------------------------------------------
// ResetModeSkill - 重置模式技能
// IDA 0x140172DB0: ?ResetModeSkill@CGocSkill@@QEAAXXZ
// ----------------------------------------------------------------------------
void CGocSkill::ResetModeSkill()
{
    // IDA反编译: 简单函数，清空模式技能相关数据
    // TODO: 需要IDA反编译确认实现
}

// ----------------------------------------------------------------------------
// LearnModeSkill - 学习模式技能
// IDA 0x140172E10: ?LearnModeSkill@CGocSkill@@QEAA_NH@Z
// ----------------------------------------------------------------------------
bool CGocSkill::LearnModeSkill(int nSkillID)
{
    // IDA反编译: 大型函数，处理roguelike技能学习
    // TODO: 需要IDA反编译确认实现
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
bool CGocSkill::HaveModeSkillActiveCount(int nGroupID)
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
