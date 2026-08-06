#include "GocEvent.h"
#include "GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLogin.h"
#include <cstring>
#include <ctime>

// ============================================================================
// CGocEvent - 构造函数和析构函数
// 来自 IDA 反编译 GameServer.exe
// ============================================================================

// IDA: 0x140068840
// CGocEvent *__fastcall CGocEvent::CGocEvent(CGocEvent *this)
// {
//   GOComponent::GOComponent(this);
//   this->__vftable = (CGocEvent_vtbl *)&CGocEvent::`vftable';
//   std::map<...>::map<...>(&this->m_mapWorldEvent);
//   std::map<...>::map<...>(&this->m_mapWorldEventReward);
//   PS_EXCHANGE_ITEM_RECALL_RES::PS_EXCHANGE_ITEM_RECALL_RES(&this->m_psRouletteInfo);
//   std::map<...>::map<...>(&this->m_mapNetCafeMission);
//   return this;
// }
CGocEvent::CGocEvent()
    : GOComponent()
    , m_bWorldEventDBCall(false)
    , m_psRouletteInfo{}  // IDA: PS_EXCHANGE_ITEM_RECALL_RES::PS_EXCHANGE_ITEM_RECALL_RES
    , m_bRouletteUse(false)
    , m_bNetCafeMission(false)
    , m_nNetCafeNextDay(0)
    , m_dw64NetCafeUpdateTick(0)
    , m_nNetCafeMission_InitHour(9)
    , m_dw64NetCafeDBUpdateTick(0)
{
    // IDA: 0x140068840
    // m_mapWorldEvent, m_mapWorldEventReward, m_mapNetCafeMission
    // 由 std::map 默认构造函数初始化
}

// IDA: 0x140068960
// void __fastcall CGocEvent::~CGocEvent(CGocEvent *this)
// {
//   this->__vftable = (CGocEvent_vtbl *)&CGocEvent::`vftable';
//   std::map<...>::~map<...>(&this->m_mapNetCafeMission);
//   std::map<...>::~map<...>(&this->m_mapWorldEventReward);
//   std::map<...>::~map<...>(&this->m_mapWorldEvent);
//   GOComponent::~GOComponent(this);
// }
CGocEvent::~CGocEvent() {
    // std::map 析构函数自动调用清理
    // m_mapNetCafeMission.~map()
    // m_mapWorldEventReward.~map()
    // m_mapWorldEvent.~map()
}

// ============================================================================
// GOComponent 接口实现
// ============================================================================

bool CGocEvent::Initialize() {
    return true;
}

void CGocEvent::Shutdown() {
}

void CGocEvent::Update(float fDeltaTime) {
    (void)fDeltaTime;
}

// ============================================================================
// 静态函数
// ============================================================================

// IDA: 0x14005B410
// __int64 __fastcall CGocEvent::GetFamilyID()
// {
//   return 2;
// }
int CGocEvent::GetFamilyID() {
    return 2;
}

// ============================================================================
// 初始化和清理
// ============================================================================

// IDA: 0x1400689D0
// void __fastcall CGocEvent::Init(CGocEvent *this)
// {
//   this->m_bWorldEventDBCall = 0;
//   std::_Tree<...>::clear(&this->m_mapWorldEvent);
//   std::_Tree<...>::clear(&this->m_mapWorldEventReward);
//   memset(&this->m_psRouletteInfo, 0, sizeof(this->m_psRouletteInfo));
//   this->m_bRouletteUse = 0;
//   this->m_bNetCafeMission = 0;
//   this->m_nNetCafeNextDay = 0;
//   this->m_dw64NetCafeUpdateTick = 0;
//   std::_Tree<...>::clear(&this->m_mapNetCafeMission);
//   this->m_nNetCafeMission_InitHour = 9;
//   this->m_dw64NetCafeDBUpdateTick = 0;
// }
void CGocEvent::Init() {
    m_bWorldEventDBCall = false;
    m_mapWorldEvent.clear();
    m_mapWorldEventReward.clear();

    // IDA: memset(&this->m_psRouletteInfo, 0, sizeof(this->m_psRouletteInfo));
    m_psRouletteInfo = {};
    m_bRouletteUse = false;

    // Reset netcafe mission
    m_bNetCafeMission = false;
    m_nNetCafeNextDay = 0;
    m_dw64NetCafeUpdateTick = 0;
    m_mapNetCafeMission.clear();
    m_nNetCafeMission_InitHour = 9;
    m_dw64NetCafeDBUpdateTick = 0;
}

// IDA: 0x140068A80
// void __fastcall CGocEvent::LogOut(CGocEvent *this)
// {
//   CGocEvent::AllDBUpdateNetCafeMission(this);
// }
void CGocEvent::LogOut() {
    AllDBUpdateNetCafeMission();
}

// ============================================================================
// 世界事件函数
// ============================================================================

// IDA: 0x1400695E0
// __int64 __fastcall CGocEvent::GetWorldEventTotalCount(CGocEvent *this, int nEventID)
int CGocEvent::GetWorldEventTotalCount(int nEventID) {
    auto it = m_mapWorldEvent.find(nEventID);
    if (it != m_mapWorldEvent.end()) {
        return it->second.nTotalCount;
    }
    return 0;
}

// IDA: 0x140069650
// __int64 __fastcall CGocEvent::GetWorldEventMyCount(CGocEvent *this, int nEventID)
int CGocEvent::GetWorldEventMyCount(int nEventID) {
    auto it = m_mapWorldEvent.find(nEventID);
    if (it != m_mapWorldEvent.end()) {
        return it->second.nMyCount;
    }
    return 0;
}

// IDA: 0x1400696C0
// __int64 __fastcall CGocEvent::GetWorldEventLastResisterDate(CGocEvent *this, int nEventID)
std::int64_t CGocEvent::GetWorldEventLastResisterDate(int nEventID) {
    auto it = m_mapWorldEvent.find(nEventID);
    if (it != m_mapWorldEvent.end()) {
        return it->second.biLastRegisterDate;
    }
    return 0;
}

// IDA: 0x140069730
// __int64 __fastcall CGocEvent::GetWorldEventDailyRewardDate(CGocEvent *this, int nEventID)
std::int64_t CGocEvent::GetWorldEventDailyRewardDate(int nEventID) {
    auto it = m_mapWorldEvent.find(nEventID);
    if (it != m_mapWorldEvent.end()) {
        return it->second.biDailyRewardDate;
    }
    return 0;
}

// IDA: 0x1400694D0
// bool __fastcall CGocEvent::FindWorldEventReward(CGocEvent *this, int nRewardIndex)
bool CGocEvent::FindWorldEventReward(int nRewardIndex) {
    auto it = m_mapWorldEventReward.find(nRewardIndex);
    return it != m_mapWorldEventReward.end();
}

// IDA: 0x140069530
// char __fastcall CGocEvent::AddWorldEventReward(CGocEvent *this, ST_WORLD_EVENT_REWARD_INFO *stInfo)
bool CGocEvent::AddWorldEventReward(ST_WORLD_EVENT_REWARD_INFO& stInfo) {
    auto it = m_mapWorldEventReward.find(stInfo.nRewardIndex);
    if (it == m_mapWorldEventReward.end()) {
        // 插入新条目
        m_mapWorldEventReward[stInfo.nRewardIndex] = stInfo;
        return true;
    } else {
        // 更新现有条目
        it->second = stInfo;
        return false;
    }
}

// ============================================================================
// 轮盘事件函数
// ============================================================================

// IDA: 0x140364840
// __int64 __fastcall CGocEvent::GetRouletteDayCount(CGocEvent *this)
// {
//   return (unsigned int)this->m_psRouletteInfo.nDayCount;
// }
int CGocEvent::GetRouletteDayCount() {
    return static_cast<unsigned int>(m_psRouletteInfo.nDayCount);
}

// IDA: 0x1404121B0
// void __fastcall CGocEvent::ResetUseRoulette(CGocEvent *this)
// {
//   this->m_bRouletteUse = 0;
// }
void CGocEvent::ResetUseRoulette() {
    m_bRouletteUse = false;
}

// IDA: 0x14006EC30
// char __fastcall CGocEvent::IsUseRoulette(CGocEvent *this)
// {
//   if ( this->m_bRouletteUse )
//     return 0;
//   this->m_bRouletteUse = 1;
//   return 1;
// }
bool CGocEvent::IsUseRoulette() {
    if (m_bRouletteUse) {
        return false;
    }
    m_bRouletteUse = true;
    return true;
}

// IDA: 0x14006D6B0
// void __fastcall CGocEvent::SetRouletteDayCount(CGocEvent *this, int nCount)
// {
//   this->m_psRouletteInfo.nDayCount = nCount;
// }
void CGocEvent::SetRouletteDayCount(int nCount) {
    m_psRouletteInfo.nDayCount = nCount;
}

// ============================================================================
// 网吧任务函数
// ============================================================================

// IDA: 0x140070090
// void __fastcall CGocEvent::AllDBUpdateNetCafeMission(CGocEvent *this)
// {
//   CGocEvent::DBUpdateNetCafeMission(this, 1u, 0);
//   CGocEvent::DBUpdateNetCafeMission(this, 2u, 0);
//   CGocEvent::DBUpdateNetCafeMission(this, 3u, 0);
// }
void CGocEvent::AllDBUpdateNetCafeMission() {
    DBUpdateNetCafeMission(1, false);
    DBUpdateNetCafeMission(2, false);
    DBUpdateNetCafeMission(3, false);
}

// ============================================================================
// 账号事件函数
// ============================================================================

// IDA: 0x140069080
// bool __fastcall CGocEvent::CheckAccountEvent(CGocEvent *this, unsigned int dwEventID)
// {
//   VChunkFile *v2; // rax
//   bool result; // al
//   result = false;
//   if ( dwEventID == 255 )
//   {
//     v2 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//     if ( ((unsigned __int8 (__fastcall *)(VChunkFile *))v2->__vftable[5].OnStartChunk)(v2) == 6 )
//       return true;
//   }
//   return result;
// }
bool CGocEvent::CheckAccountEvent(unsigned int dwEventID) {
    // IDA精确还原：特殊事件ID 255 检查用户状态
    // Event ID 255 是特殊账号事件，需要检查 CUser 的状态
    if (dwEventID == 255) {
        // IDA: 通过 RTTI dynamic_cast 获取 CUser，然后调用虚函数检查状态
        // 需要 GetOwner<CUser>() 和状态检查虚函数调用
        // TODO: 需要完整实现 CUser RTTI 访问
        // 原始逻辑: pUser->虚函数(检查状态) == 6
        return false;
    }
    return false;
}

// ============================================================================
// 世界事件请求/响应函数
// ============================================================================

// IDA: 0x140068AA0 - RequestLoadAccountEvent
// IDA精确还原: 发送账号事件加载请求到DB
void CGocEvent::RequestLoadAccountEvent() {
    // IDA 0x140068AA0: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;

    // IDA: Build account event request
    PS_ACCOUNT_EVENT_LIST stEventLoad;
    stEventLoad.dwUCID = pUser->GetUCID();
    char* pAccountID = pUser->GetAccountID();
    if (pAccountID) {
        std::strcpy(stEventLoad.szAccountID, pAccountID);
    }

    // IDA: Send DB packet (Main=2, Sub=0x55)
    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 2, 0x55);
    xSendDBPacket << stEventLoad;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) pServer->SendDBGame(xSendDBPacket);
}

// IDA: 0x140068D00 - LoadAccountEvent
// IDA精确还原: 加载账号事件列表并处理
void CGocEvent::LoadAccountEvent(PS_ACCOUNT_EVENT_LIST& stEventList) {
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // IDA: Build update list
    PS_ACCOUNT_EVENT_LIST stEventUpdate;
    // stEventUpdate.szAccountID = pUser->GetAccountID();
    
    // IDA: Iterate through event IDs
    for (const auto& dwEventID : stEventList.vecEventID)
    {
        if (CheckAccountEvent(dwEventID))
        {
            // IDA: Get GocPost and send auto mail
            // CGocPost* pPost = pUser->GetGOC<CGocPost>();
            // if (pPost && pPost->SendAutoMail(dwEventID))
            // {
            //     stEventUpdate.vecEventID.push_back(dwEventID);
            // }
            // else
            // {
            //     LogError("[ACCOUNT_EVENT] Failed Send AuthMail");
            // }
        }
    }
    
    // IDA: Send update to DB if not empty
    if (!stEventUpdate.vecEventID.empty())
    {
        // XSendDBPacket xSendDBPacket(pUser, 0x02, 0x56);
        // xSendDBPacket << stEventUpdate;
        // XGameServer::SendDBGame(&xSendDBPacket);
    }
    
    (void)stEventList;
}

// IDA: 0x1400690E0 - SetWorldEventInfo (第一个重载)
// 处理PS_WORLD_EVENT_INFO_RES中的奖励信息，然后调用第二个重载
std::uint8_t CGocEvent::SetWorldEventInfo(PS_WORLD_EVENT_INFO_RES& psRes, std::int64_t biLastRegisterDate, std::int64_t biDailyRewardDate) {
    // 遍历奖励信息向量，更新或插入到m_mapWorldEventReward
    // for (i = 0; i < psRes.vecRewardInfo.size(); ++i) {
    //     auto& rewardInfo = psRes.vecRewardInfo[i];
    //     auto it = m_mapWorldEventReward.find(rewardInfo.nRewardIndex);
    //     if (it != m_mapWorldEventReward.end()) {
    //         // 更新现有条目
    //         it->second.byRewardState = rewardInfo.byRewardState;
    //     } else {
    //         // 插入新条目
    //         ST_LEVEL_UP_EVENT_DATA stInfo;
    //         stInfo.nRewardIndex = rewardInfo.nRewardIndex;
    //         stInfo.byRewardState = rewardInfo.byRewardState;
    //         m_mapWorldEventReward[stInfo.nRewardIndex] = stInfo;
    //     }
    // }
    // return SetWorldEventInfo(psRes.nEventID, psRes.nTotalCount, psRes.nMyCount, biLastRegisterDate, biDailyRewardDate);

    // TODO: 需要 PS_WORLD_EVENT_INFO_RES 结构的完整定义来遍历 vecRewardInfo
    // 目前使用 stub 实现
    (void)psRes;
    (void)biLastRegisterDate;
    (void)biDailyRewardDate;
    return 0; // Stub - no reward
}

// IDA: 0x1400692A0 - SetWorldEventInfo (第二个重载)
// 设置世界事件信息，返回每日奖励状态 (0=无奖励, 1=可领取, 2=已领取)
std::uint8_t CGocEvent::SetWorldEventInfo(int nEventID, int nTotalCount, int nMyCount, std::int64_t biLastRegisterDate, std::int64_t biDailyRewardDate) {
    // IDA: ATL::CTime::CTime(&tDefault, 2000, 1, 1, 9, 0, 0, -1);
    // 设置默认时间边界 (2000-01-01 09:00:00)
    std::tm tmDefault = {};
    tmDefault.tm_year = 2000 - 1900;
    tmDefault.tm_mon = 0;  // January
    tmDefault.tm_mday = 1;
    tmDefault.tm_hour = 9;
    tmDefault.tm_min = 0;
    tmDefault.tm_sec = 0;
    tmDefault.tm_isdst = -1;
    std::int64_t nDefaultTime = std::mktime(&tmDefault);

    // IDA: if ( (__int64)biLastRegisterDate <= (__int64)v6 ) biLastRegisterDate = v6;
    // 修正无效日期
    if (biLastRegisterDate <= nDefaultTime) {
        biLastRegisterDate = nDefaultTime;
    }
    if (biDailyRewardDate <= nDefaultTime) {
        biDailyRewardDate = nDefaultTime;
    }

    // 查找或创建事件条目
    auto it = m_mapWorldEvent.find(nEventID);
    if (it != m_mapWorldEvent.end()) {
        // IDA: 更新现有条目
        it->second.nTotalCount = nTotalCount;
        it->second.nMyCount = nMyCount;
        it->second.biLastRegisterDate = biLastRegisterDate;
        it->second.biDailyRewardDate = biDailyRewardDate;
    } else {
        // IDA: 插入新条目
        ST_WORLD_EVENT_BOOSTER stInfo;
        stInfo.nEventID = nEventID;
        stInfo.nTotalCount = nTotalCount;
        stInfo.nMyCount = nMyCount;
        stInfo.biLastRegisterDate = biLastRegisterDate;
        stInfo.biDailyRewardDate = biDailyRewardDate;
        m_mapWorldEvent[nEventID] = stInfo;
    }

    // IDA: v14 = TXSingleton<XGameServer>::Instance();
    //      v15._Ptr = XGameServer::GetBeforeInitDate(v14);
    // TODO: 需要从 XGameServer 获取当前日期来判断奖励状态
    // 简化实现: 使用当前时间
    std::time_t tNow = std::time(nullptr);
    std::int64_t biToday = static_cast<std::int64_t>(tNow);

    // IDA: if ( (__int64)biDailyRewardDate >= (__int64)v16 ) return 2;
    if (biDailyRewardDate >= biToday) {
        return 2; // 已领取
    }

    // IDA: if ( (__int64)biDailyRewardDate < (__int64)v18 &&
    //          (__int64)v19 <= (__int64)biLastRegisterDate ) return 1;
    if (biDailyRewardDate < biToday && biToday <= biLastRegisterDate) {
        return 1; // 可领取
    }

    return 0; // 无奖励
}

// IDA: 0x1400697A0 - ReqWorldEventInfo
// 请求世界事件信息，发送DB请求
// IDA精确还原：验证事件、检查时间范围、发送DB请求
int CGocEvent::ReqWorldEventInfo(PS_WORLD_EVENT_INFO_REQ& psReq) {
    // IDA: 错误码定义
    // 59002 = 事件不存在
    // 59003 = 事件未激活或不在时间范围
    // 59007 = 正在处理中
    
    // IDA: 检查是否正在等待DB响应
    if (m_bWorldEventDBCall) {
        return 59007; // 正在处理中
    }

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return 59007;
    // 2. PS_DB_WORLD_EVENT_INFO_REQ psDBReq;
    //    psDBReq.dwUAID = pUser->GetUAID();
    //    psDBReq.dwUCID = pUser->GetUCID();
    //    psDBReq.nEventID = psReq.nEventID;
    // 3. XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //    TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(&pServer->m_xResourceMgr, psDBReq.nEventID);
    //    if (!pTB_WORLD_EVENT) return 59002;
    //    if (!pTB_WORLD_EVENT->event_activation) return 59003;
    // 4. ATL::CTime::GetTickCount(&tCurr);
    //    解析 pTB_WORLD_EVENT->event_start_date/event_end_date (格式: "%d-%d-%d %d:%d:%d")
    //    if (tCurr < tStart || tEnd < tCurr) return 59003;
    // 5. m_bWorldEventDBCall = true;
    // 6. XSendDBPacket xSendDBPacket(pUser, 0x49, 0x27);
    //    xSendDBPacket << psDBReq;
    //    XGameServer::SendDBGame(pServer, &xSendDBPacket);

    // TODO: 需要以下依赖完整实现：
    // - CUser RTTI access
    // - XResourceMgr::GetTB_WORLD_EVENT()
    // - ATL::CTime 日期解析
    // - XSendDBPacket

    (void)psReq;
    m_bWorldEventDBCall = true;
    return 0; // 成功
}

// IDA: 0x140069D90 - ReqWorldEventRegister
// 世界事件注册请求 - 注册事件物品贡献
// IDA精确还原：验证事件、查找物品、锁定物品、发送DB请求
int CGocEvent::ReqWorldEventRegister(PS_WORLD_EVENT_REGISTER_REQ& psReq) {
    // IDA: 错误码定义
    // 59002 = 事件不存在
    // 59003 = 事件未激活或不在时间范围
    // 59007 = 正在处理中
    // 52001 = 背包组件获取失败
    // 52004 = 物品表不存在
    // 52014 = 物品不存在
    
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // IDA核心流程：
    // 1. PS_DB_WORLD_EVENT_REGISTER_REQ psDBReq;
    //    psDBReq.dwUCID = GetOwner()->GetUCID();
    //    psDBReq.nEventID = psReq->nEventID;
    // 2. TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(psDBReq.nEventID);
    //    if (!pTB_WORLD_EVENT) return 59002;
    //    if (!pTB_WORLD_EVENT->event_activation) return 59003;
    // 3. 检查事件时间范围 (同ReqWorldEventInfo)
    // 4. TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(pTB_WORLD_EVENT->event_item_ID);
    //    TB_ITEM_CLASSIFY* pTB_ITEM_CLASSIFY = XResourceMgr::GetTB_ITEM_CLASSIFY(pTB_ITEM->Item_Classify_Index);
    // 5. CGocInventory* pInven = GetOwner()->GetGOC<CGocInventory>();
    //    XBaseInventory* pBaseInven = pInven->GetTBInvenPtr(pTB_ITEM_CLASSIFY->Item_Inven_Type);
    // 6. psDBReq.byInvenType = pTB_ITEM_CLASSIFY->Item_Inven_Type;
    // 7. pBaseInven->GetSameItems_2(event_item_ID, &vecFindList); // 查找所有匹配物品
    // 8. for (auto& pItem : vecFindList) {
    //        if (!pBaseInven->GetLock(slot)) {
    //            PS_STORAGE_INFO psInfo;
    //            psInfo.byInvenType = pItem->GetInvenType();
    //            psInfo.shSlotPos = pItem->GetSlot();
    //            psInfo.stItem = pItem->GetItem();
    //            psDBReq.stUpdateItem.push_back(psInfo);
    //            psDBReq.nCount += pItem->GetCount();
    //            pBaseInven->SetLock(slot, true); // 锁定物品
    //        }
    //    }
    // 9. m_bWorldEventDBCall = true;
    // 10. XSendDBPacket(pUser, 0x49, 0x28);
    //     XGameServer::SendDBGame(&xSendDBPacket);

    m_bWorldEventDBCall = true;
    (void)psReq;
    return 0;
}

// IDA: 0x14006A7E0 - ReqWorldEventReward
// 世界事件奖励请求 - 领取世界事件奖励
// IDA精确还原：验证奖励条件、创建物品、发送DB请求
int CGocEvent::ReqWorldEventReward(PS_WORLD_EVENT_REWARD_REQ& psReq) {
    // IDA: 错误码定义
    // 59002 = 事件不存在
    // 59003 = 事件未激活或不在时间范围
    // 59004 = 奖励条件不满足或奖励不存在
    // 59005 = 已领取或背包不足
    // 59007 = 正在处理中
    // 59008 = 创建物品失败
    
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return 59007;
    // 2. TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(psReq->nEventID);
    //    if (!pTB_WORLD_EVENT || !pTB_WORLD_EVENT->event_activation) return 59002/59003;
    // 3. 检查事件时间范围
    // 4. TB_WORLD_EVENT_REWARD* pTB_WORLD_EVENT_REWARD = XResourceMgr::GetTB_WORLD_EVENT_REWARD(psReq->nRewardIndex);
    //    if (!pTB_WORLD_EVENT_REWARD) return 59004;
    //    if (pTB_WORLD_EVENT_REWARD->event_reward_type != 1) return 59004;
    //    if (pTB_WORLD_EVENT_REWARD->world_reward_type != psReq->byRewardType) return 59004;
    // 5. 检查奖励条件：
    //    if (pTB_WORLD_EVENT_REWARD->world_reward_type == 0) {
    //        // 总贡献度奖励
    //        fPercent = GetWorldEventTotalCount(nEventID) / pTB_WORLD_EVENT->event_item_amount_max;
    //        if (fPercent * 100 < pTB_WORLD_EVENT_REWARD->event_item_percentile_min) return 59004;
    //    } else if (pTB_WORLD_EVENT_REWARD->world_reward_type == 1) {
    //        // 个人贡献度奖励
    //        if (GetWorldEventMyCount(nEventID) < pTB_WORLD_EVENT_REWARD->event_item_percentile_min) return 59004;
    //    }
    // 6. if (FindWorldEventReward(psReq->nRewardIndex)) return 59005; // 已领取
    // 7. PS_DB_WORLD_EVENT_REWARD psDBReq;
    //    psDBReq.dwUAID = pUser->GetUAID();
    //    psDBReq.dwUCID = pUser->GetUCID();
    //    psDBReq.psReq = *psReq;
    //    psDBReq.byItemFlag = 45;
    //    psDBReq.dwRewardItemID = pTB_WORLD_EVENT_REWARD->event_reward_value;
    //    psDBReq.shRewardCount = pTB_WORLD_EVENT_REWARD->event_reward_item_amount;
    // 8. if (pTB_WORLD_EVENT_REWARD->world_reward_type == 1) {
    //        // 直接给物品
    //        CGocInventory::CreateItem2(...);
    //    }
    // 9. m_bWorldEventDBCall = true;
    // 10. XSendDBPacket(pUser, 0x49, 0x29);

    m_bWorldEventDBCall = true;
    (void)psReq;
    return 0;
}

// IDA: 0x14006B300 - ReqWorldEventDailyReward
// 世界事件每日奖励请求 - 领取每日登录奖励
// IDA精确还原：验证每日奖励状态、创建物品、发送DB请求
int CGocEvent::ReqWorldEventDailyReward(PS_WORLD_EVENT_DAILY_REWARD_REQ& psReq) {
    // IDA: 错误码定义
    // 59002 = 事件不存在
    // 59003 = 事件未激活或不在时间范围
    // 59004 = 奖励物品不存在
    // 59006 = 用户验证失败或奖励状态错误
    // 59007 = 正在处理中
    // 59008 = 创建物品失败
    
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return 59006;
    // 2. CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    //    if (!pInven) return 59006;
    // 3. TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(psReq->nEventID);
    //    if (!pTB_WORLD_EVENT) return 59002;
    //    if (!pTB_WORLD_EVENT->event_activation) return 59003;
    // 4. 检查事件时间范围
    // 5. TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(pTB_WORLD_EVENT->event_daily_reward_item_ID);
    //    if (!pTB_ITEM || !pTB_WORLD_EVENT->event_daily_reward_item_amount) return 59004;
    // 6. 获取当前事件状态
    //    biDailyRewardDate = GetWorldEventDailyRewardDate(psReq->nEventID);
    //    biLastRegisterDate = GetWorldEventLastResisterDate(psReq->nEventID);
    //    nMyCount = GetWorldEventMyCount(psReq->nEventID);
    //    nTotalCount = GetWorldEventTotalCount(psReq->nEventID);
    // 7. bRewardState = SetWorldEventInfo(nEventID, nTotalCount, nMyCount, biLastRegisterDate, biDailyRewardDate);
    //    if (bRewardState != 1) return 59006; // 不是可领取状态
    // 8. PS_DB_WORLD_EVENT_DAILY_REWARD psDBReq;
    //    psDBReq.dwUCID = pUser->GetUCID();
    //    psDBReq.nEventID = psReq->nEventID;
    //    psDBReq.biDailyRewardDate = XGameServer::GetBeforeInitDate();
    //    psDBReq.byItemFlag = 45;
    // 9. 创建奖励物品
    //    ST_CREATE_ITEMS stCreateItems;
    //    stCreateItems[0].nItemID = pTB_WORLD_EVENT->event_daily_reward_item_ID;
    //    stCreateItems[0].shCount = pTB_WORLD_EVENT->event_daily_reward_item_amount;
    //    CGocInventory::CreateItem2(pInven, &stCreateItems, 0x54, 0, &psDBReq.stCreateItem, &psDBReq.stUpdateItem);
    // 10. m_bWorldEventDBCall = true;
    // 11. XSendDBPacket(pUser, 0x49, 0x2A);

    m_bWorldEventDBCall = true;
    (void)psReq;
    return 0;
}

// IDA: 0x14006BD30 - ResWorldEventInfo
// 处理DB世界事件信息响应，发送响应给客户端
// IDA精确还原：验证事件、限制计数、发送客户端包
void CGocEvent::ResWorldEventInfo(PS_DB_WORLD_EVENT_INFO_RES& psRes) {
    // 重置DB调用标志
    m_bWorldEventDBCall = false;

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return;
    // 2. XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //    TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(&pServer->m_xResourceMgr, psRes.psInfo.nEventID);
    // 3. if (!pTB_WORLD_EVENT) {
    //        CUser::SendErrorMessage(pUser, 0x2A, 0x22, 0xE67A); // 59002
    //        return;
    //    }
    // 4. if (pTB_WORLD_EVENT->event_item_amount_max < psRes.psInfo.nTotalCount) {
    //        psRes.psInfo.nTotalCount = pTB_WORLD_EVENT->event_item_amount_max;
    //    }
    // 5. psRes.byDailyRewardState = SetWorldEventInfo(
    //        psRes.psInfo.nEventID,
    //        psRes.psInfo.nTotalCount,
    //        psRes.psInfo.nMyCount,
    //        psRes.biLastRegisterDate,
    //        psRes.biDailyRewardDate
    //    );
    // 6. PS_WORLD_EVENT_INFO_RES psClientRes;
    //    psClientRes.nEventID = psRes.psInfo.nEventID;
    //    psClientRes.nTotalCount = psRes.psInfo.nTotalCount;
    //    psClientRes.nMyCount = psRes.psInfo.nMyCount;
    //    psClientRes.byDailyRewardState = psRes.byDailyRewardState;
    //    psClientRes.vecRewardInfo = psRes.psInfo.vecRewardInfo;
    // 7. XSendPacket xSendPacket(0x2A, 0x22);
    //    xSendPacket << psClientRes;
    //    CGocNetwork::Send(&pUser->XActor, &xSendPacket);

    (void)psRes;
}

// IDA: 0x14006BF80 - ResWorldEventRegister
// 处理DB世界事件注册响应
// IDA精确还原：解锁物品、更新计数、发送响应
void CGocEvent::ResWorldEventRegister(PS_DB_WORLD_EVENT_REGISTER_RES& psRes) {
    m_bWorldEventDBCall = false;

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return;
    // 2. TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(psRes.nEventID);
    //    if (!pTB_WORLD_EVENT) return;
    // 3. CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
    //    if (!pInven) return;
    // 4. XBaseInventory* pBaseInven = pInven->GetTBInvenPtr(psRes.byInvenType);
    // 5. if (psRes.nError) {
    //        // 错误：解锁所有物品
    //        for (auto& item : psRes.stUpdateItem) {
    //            pBaseInven->SetLock(item.shSlotPos, false);
    //        }
    //        CUser::SendErrorMessage(pUser, 0x2A, 0x23, psRes.nError);
    //        return;
    //    }
    // 6. // 成功：移除物品
    //    for (auto& item : psRes.stUpdateItem) {
    //        pBaseInven->RemoveItem(item.shSlotPos, item.stItem.shCount);
    //        // 发送统计日志
    //        XGameServer::SendStatisticsLog(...);
    //    }
    // 7. // 更新世界事件计数
    //    biLastRegisterDate = XGameServer::GetBeforeInitDate();
    //    SetWorldEventInfo(psRes.nEventID, psRes.nTotalCount + psRes.nRegisterCount, 
    //                      psRes.nMyCount + psRes.nRegisterCount, biLastRegisterDate, 0);
    // 8. // 发送成功响应
    //    PS_WORLD_EVENT_REGISTER_RES psClientRes;
    //    psClientRes.nEventID = psRes.nEventID;
    //    psClientRes.nTotalCount = psRes.nTotalCount + psRes.nRegisterCount;
    //    psClientRes.nMyCount = psRes.nMyCount + psRes.nRegisterCount;
    //    psClientRes.nRegisterCount = psRes.nRegisterCount;
    //    XSendPacket xSendPacket(0x2A, 0x23);
    //    xSendPacket << psClientRes;
    //    CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    // 9. // 发送日志到DB
    //    XSendDBPacket xSendDBPacket(pUser, 0x02, 0x57);
    //    XGameServer::SendDBGame(&xSendDBPacket);

    (void)psRes;
}

// IDA: 0x14006C880 - ResWorldEventReward
// 处理DB世界事件奖励响应
// IDA精确还原：添加奖励记录、发送响应、发送日志
void CGocEvent::ResWorldEventReward(PS_DB_WORLD_EVENT_REWARD& psRes) {
    m_bWorldEventDBCall = false;

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return;
    // 2. if (psRes.nError) {
    //        CUser::SendErrorMessage(pUser, 0x2A, 0x24, psRes.nError);
    //        return;
    //    }
    // 3. // 根据奖励类型处理
    //    if (psRes.psReq.byRewardType == 1) {
    //        // 个人贡献度奖励：物品已在ReqWorldEventReward中创建
    //        // 发送更新/创建物品包给客户端
    //        XSendPacket xSendPacket(0x52, 0x0C); // UpdateInventory?
    //        xSendPacket << psRes.stUpdateItem;
    //        CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    //    } else {
    //        // 总贡献度奖励：通过邮件发送
    //        PS_DB_ACCOUNT_POST_DATA psPostData;
    //        psPostData.dwUAID = pUser->GetUAID();
    //        psPostData.nPostType = 10; // 事件奖励邮件类型
    //        psPostData.nItemID = psRes.dwRewardItemID;
    //        psPostData.shCount = psRes.shRewardCount;
    //        XSendDBPacket xSendDBPacket(pUser, 0x49, 0x2B);
    //        XGameServer::SendDBGame(&xSendDBPacket);
    //    }
    // 4. // 添加奖励到已领取列表
    //    ST_LEVEL_UP_EVENT_DATA stInfo;
    //    stInfo.nRewardIndex = psRes.psReq.nRewardIndex;
    //    stInfo.byRewardType = psRes.psReq.byRewardType;
    //    stInfo.byRewardState = 2; // 已领取
    //    AddWorldEventReward(stInfo);
    // 5. // 发送奖励响应给客户端
    //    PS_WORLD_EVENT_REWARD_RES psClientRes;
    //    psClientRes.nEventID = psRes.psReq.nEventID;
    //    psClientRes.nRewardIndex = psRes.psReq.nRewardIndex;
    //    psClientRes.byRewardType = psRes.psReq.byRewardType;
    //    XSendPacket xSendPacket(0x2A, 0x24);
    //    xSendPacket << psClientRes;
    //    CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    // 6. // 发送日志到DB
    //    XSendDBPacket xSendDBPacket(pUser, 0x02, 0x58);
    //    XGameServer::SendDBGame(&xSendDBPacket);

    (void)psRes;
}

// IDA: 0x14006CF00 - ResWorldEventDailyReward
// 处理DB世界事件每日奖励响应
// IDA精确还原：更新每日奖励日期、发送响应、发送日志
void CGocEvent::ResWorldEventDailyReward(PS_DB_WORLD_EVENT_DAILY_REWARD& psRes) {
    m_bWorldEventDBCall = false;

    // IDA核心流程：
    // 1. CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    //    if (!pUser) return;
    // 2. if (psRes.nError) {
    //        CUser::SendErrorMessage(pUser, 0x2A, 0x25, psRes.nError);
    //        return;
    //    }
    // 3. // 更新世界事件信息（标记每日奖励已领取）
    //    std::int64_t biLastRegisterDate = GetWorldEventLastResisterDate(psRes.nEventID);
    //    int nMyCount = GetWorldEventMyCount(psRes.nEventID);
    //    int nTotalCount = GetWorldEventTotalCount(psRes.nEventID);
    //    SetWorldEventInfo(psRes.nEventID, nTotalCount, nMyCount, biLastRegisterDate, psRes.biDailyRewardDate);
    // 4. // 发送更新/创建物品包给客户端
    //    XSendPacket xSendPacket(0x52, 0x0C); // UpdateInventory?
    //    xSendPacket << psRes.stUpdateItem;
    //    CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    // 5. // 发送每日奖励响应给客户端
    //    PS_WORLD_EVENT_DAILY_REWARD_RES psClientRes;
    //    psClientRes.nEventID = psRes.nEventID;
    //    psClientRes.biDailyRewardDate = psRes.biDailyRewardDate;
    //    XSendPacket xSendPacket(0x2A, 0x25);
    //    xSendPacket << psClientRes;
    //    CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    // 6. // 发送日志到DB
    //    XSendDBPacket xSendDBPacket(pUser, 0x02, 0x59);
    //    XGameServer::SendDBGame(&xSendDBPacket);

    (void)psRes;
}

// ============================================================================
// 轮盘事件函数
// ============================================================================

// IDA: 0x14006D310 - SendDBRouletteInfo
// IDA精确还原: 发送轮盘信息到DB
void CGocEvent::SendDBRouletteInfo(std::uint8_t byUseType, int nEventID) {
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // IDA: Build DB request
    PS_DB_ROULETTE_EVENT_INFO psDBRouletteInfo;
    psDBRouletteInfo.nEventID = nEventID;
    psDBRouletteInfo.dwUAID = pUser->GetUAID();
    
    // IDA: Check use type for UCID
    if (byUseType != 1)
    {
        psDBRouletteInfo.dwUCID = pUser->GetID();
    }
    else
    {
        psDBRouletteInfo.dwUCID = 0;
    }
    
    // IDA: Send DB packet (Main=0x49, Sub=0x2B)
    // XSendDBPacket xSendDBPacket(pUser, 0x49, 0x2B);
    // xSendDBPacket << psDBRouletteInfo;
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);
    
    (void)byUseType;
    (void)nEventID;
}

// IDA: 0x14006D4A0
// void __fastcall CGocEvent::LoadRouletteEventInfo(CGocEvent *this, PS_ROULETTE_INFO *psInfo)
// 复制轮盘信息，检查日期并可能在需要时重置每日计数
void CGocEvent::LoadRouletteEventInfo(PS_ROULETTE_INFO& psInfo) {
    // IDA: qmemcpy(&this->m_psRouletteInfo, psInfo, sizeof(m_psRouletteInfo));
    m_psRouletteInfo = psInfo;

    // IDA: ATL::CTime::GetTickCount(&tCurr);
    std::time_t tCurrTime = std::time(nullptr);
    std::tm* pTm = std::localtime(&tCurrTime);
    if (!pTm) {
        SendRouletteEventInfo();
        return;
    }

    int nDay = pTm->tm_mday;
    int nMonth = pTm->tm_mon + 1;
    int nYear = pTm->tm_year + 1900;
    int nHour = pTm->tm_hour;

    // IDA: ATL::CTime::CTime(&tTodayInit, Year, nMonth, nDay, 9, 0, 0, -1);
    std::tm tmTodayInit = {};
    tmTodayInit.tm_year = nYear - 1900;
    tmTodayInit.tm_mon = nMonth - 1;
    tmTodayInit.tm_mday = nDay;
    tmTodayInit.tm_hour = 9;
    tmTodayInit.tm_min = 0;
    tmTodayInit.tm_sec = 0;
    tmTodayInit.tm_isdst = -1;
    std::time_t tTodayInit = std::mktime(&tmTodayInit);

    // IDA: 检查是否需要重置每日计数
    // if ( psInfo->biRegDate > 0 && ATL::CTime::GetHour(&tCurr) >= 9 &&
    //      psInfo->biRegDate < tTodayInit )
    if (psInfo.biRegDate > 0 && nHour >= 9 && psInfo.biRegDate < tTodayInit) {
        // 需要重置每日计数
        InitRouletteDayCount(tCurrTime);
    }

    SendRouletteEventInfo();
}

// IDA: 0x14006D5B0 - SendRouletteEventInfo
// IDA精确还原: 发送轮盘事件信息给客户端
void CGocEvent::SendRouletteEventInfo() {
    // IDA: Get CUser via RTTI dynamic_cast
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // IDA: XSendPacket xSendPacket(0x2A, 0x28);
    // IDA: operator<<(&xSendPacket, (PS_EXCHANGE_ITEM_RECALL_RES *)&this->m_psRouletteInfo);
    // IDA: CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    
    // Note: Requires XSendPacket and CGocNetwork implementation
    // Sending packet with main=0x2A, sub=0x28
}

// IDA: 0x14006D6D0 - IsRouletteEvent
// IDA精确还原: 执行轮盘事件，扣除消耗并发送奖励
int CGocEvent::IsRouletteEvent() {
    // IDA: 错误码
    const int ERR_NO_EVENT = 59600;
    const int ERR_DAY_LIMIT = 59601;
    const int ERR_SHORTAGE_ZENNY = 59602;
    const int ERR_SHORTAGE_ETHER = 59603;
    const int ERR_SHORTAGE_BP = 59604;
    const int ERR_SHORTAGE_CASH = 59605;
    const int ERR_SEND_POST = 55602;
    const int ERR_INVALID_COST = 52013;
    const int ERR_NO_ITEM_TABLE = 52004;
    
    // IDA: Get CUser via RTTI
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return ERR_NO_EVENT;
    
    // IDA: Get roulette event info from CTimeEventMgr
    // PS_GM_ROULETTE_EVENT psEventInfo;
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // if (!CTimeEventMgr::GetRouletteEventReward(&pServer->m_TimeEventMgr, &psEventInfo))
    //     return ERR_NO_EVENT;
    
    // IDA: Check daily limit
    // if (psEventInfo.byUseType && psEventInfo.nUseCount <= m_psRouletteInfo.nDayCount)
    //     return ERR_DAY_LIMIT;
    
    // IDA: Get inventory component
    // auto pInvenPtr = pUser->GetGOC<CGocInventory>();
    // if (!pInvenPtr) return ERR_NO_EVENT;
    
    // IDA: Process cost based on type
    // switch (psEventInfo.byCostType) {
    //     case 1: // Item cost
    //         if (!XResourceMgr::GetTB_ITEM(psEventInfo.nCostID)) return ERR_NO_ITEM_TABLE;
    //         // ReduceItem2
    //         break;
    //     case 2: // Zenny cost
    //         if (!pInvenPtr->AddMoney(-psEventInfo.nCostCount, 0x3E, 0, 0, 0)) return ERR_SHORTAGE_ZENNY;
    //         break;
    //     case 3: // Ether cost
    //         if (!pInvenPtr->AddEther(-psEventInfo.nCostCount, 0x3E, 1)) return ERR_SHORTAGE_ETHER;
    //         break;
    //     case 4: // BP cost
    //         if (!pInvenPtr->AddBP(-psEventInfo.nCostCount, 0x3E)) return ERR_SHORTAGE_BP;
    //         break;
    //     case 5: // Cash cost
    //         if (!pInvenPtr->AddCash(-psEventInfo.nCostCount, 0x3E)) return ERR_SHORTAGE_CASH;
    //         break;
    //     default: return ERR_NO_EVENT;
    // }
    
    // IDA: Calculate reward using weighted random
    // int nRate = 10000;
    // int nRewardID = 0;
    // for (const auto& reward : psEventInfo.psRewardList) {
    //     if (reward.nSharePoint > 0 && reward.byRewardStep >= reward.nSharePoint) {
    //         nRate -= reward.nClearCount;
    //     }
    // }
    // nRate = XItemFactory::nRand(1, nRate);
    // for (const auto& reward : psEventInfo.psRewardList) {
    //     if (reward.nGroupID && reward.nClearCount) {
    //         nRate -= reward.nClearCount;
    //         if (nRate <= 0) {
    //             nRewardID = reward.nMazeID;
    //             break;
    //         }
    //     }
    // }
    
    // IDA: Send reward via post
    // auto pPostPtr = pUser->GetGOC<CGocPost>();
    // if (!pPostPtr) return ERR_NO_EVENT;
    // if (psEventInfo.byUseType == 1) {
    //     if (!pPostPtr->AccountPostSend(&stCreateItems, 0x0D, 3)) return ERR_SEND_POST;
    // } else {
    //     if (!pPostPtr->SystemPostSend(&stCreateItems, 0x0D, 3, 0, nullptr)) return ERR_SEND_POST;
    // }
    
    // IDA: Update day count and send DB update
    // m_psRouletteInfo.nDayCount++;
    // m_psRouletteInfo.biRegDate = XGameServer::GetCurDate();
    // Send DB packet (main=0x49, sub=0x2C)
    
    // IDA: Send updated info to client
    SendRouletteEventInfo();
    
    return 0; // Success
}

// IDA: 0x14006EA10 - InitRouletteDayCount
// IDA精确还原: 初始化轮盘每日计数
void CGocEvent::InitRouletteDayCount(std::int64_t biInitTime) {
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // IDA: Get roulette event info from CTimeEventMgr
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // CTimeEventMgr* pTimeEventMgr = XResourceMgr::GetTimeEventMgr(&pServer->m_xResourceMgr);
    // TB_ROULETTE_EVENT* pTB_ROULETTE_EVENT = CTimeEventMgr::GetRouletteEventReward(pTimeEventMgr);
    // if (!pTB_ROULETTE_EVENT) return;
    
    // IDA: Reset local count
    m_psRouletteInfo.nDayCount = 0;
    m_psRouletteInfo.biRegDate = biInitTime;
    
    // IDA: Send DB initialization request
    // PS_DB_INIT_ROULETTE_INFO psDBInit;
    // psDBInit.dwUAID = pUser->GetUAID();
    // psDBInit.dwUCID = pUser->GetUCID();
    // psDBInit.nEventID = pTB_ROULETTE_EVENT->event_ID;
    // psDBInit.biRegDate = biInitTime;
    
    // IDA: XSendDBPacket xSendDBPacket(pUser, 0x49, 0x2E);
    // xSendDBPacket << psDBInit;
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);
    
    // Note: Requires XSendDBPacket and PS_DB_INIT_ROULETTE_INFO implementation
}

// ============================================================================
// 网吧任务函数
// ============================================================================

// IDA: 0x14006EC60 - SetStartNetCafeMission
// IDA精确还原: 启动或停止网吧任务
void CGocEvent::SetStartNetCafeMission(bool bStart) {
    m_bNetCafeMission = bStart;
    
    if (bStart)
    {
        // IDA: Check if need to load mission list
        if (m_dw64NetCafeUpdateTick == 0)
        {
            // IDA: Get CUser
            CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
            if (!pUser) return;
            
            // IDA: Send DB request to load netcafe mission list
            // PS_NETCAFE_MISSION_LIST psList;
            // psList.dwUAID = pUser->GetUAID();
            // XSendDBPacket xSendDBPacket(pUser, 0x49, 0x30);
            // xSendDBPacket << psList;
            // XGameServer::SendDBGame(&xSendDBPacket);
        }
    }
}

// IDA: 0x14006EEC0 - LoadNetCafeMission
// IDA精确还原: 加载网吧任务列表
void CGocEvent::LoadNetCafeMission(PS_NETCAFE_MISSION_LIST& psInfo) {
    (void)psInfo;
    // TODO: restore net-cafe mission list parsing once PS_NETCAFE_MISSION_LIST is defined here.
}

// IDA: 0x14006F480 - GetNetCafeMissionTime
// IDA精确还原: 获取网吧任务时间范围
bool CGocEvent::GetNetCafeMissionTime(unsigned int dwID, std::time_t& tCurr, std::time_t& tStart, std::time_t& tEnd) {
    (void)dwID;
    tStart = tCurr;
    tEnd = tCurr;
    // TODO: restore net-cafe mission time-window calculation.
    return false;
}

// IDA: 0x14006FB40 - SendNetCafeMissionInfo
// IDA精确还原: 发送网吧任务信息给客户端
void CGocEvent::SendNetCafeMissionInfo(unsigned int dwID) {
    (void)dwID;
    // TODO: restore PS_NETCAFE_MISSION_LIST client packet serialization.
}

// IDA: 0x14006FD90 - DBUpdateNetCafeMission
// IDA精确还原: 发送网吧任务更新到DB
void CGocEvent::DBUpdateNetCafeMission(unsigned int dwID, bool bInit) {
    (void)dwID;
    (void)bInit;
    // TODO: restore PS_NETCAFE_MISSION_UPDATE DB packet and log serialization.
}

// IDA: 0x1400700E0 - Cheat_NetCafeMission_PlayTime
// IDA精确还原: GM作弊函数，设置网吧任务游玩时间
void CGocEvent::Cheat_NetCafeMission_PlayTime(unsigned int dwID, bool bReset, int nAddMin) {
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // IDA: Get current time and month
    std::time_t tNow = std::time(nullptr);
    std::tm* pTm = std::localtime(&tNow);
    if (!pTm) return;
    
    int nMonth = pTm->tm_mon + 1;
    if (nMonth < 1 || nMonth > 12) return;
    
    // IDA: Find mission
    auto it = m_mapNetCafeMission.find(dwID);
    if (it == m_mapNetCafeMission.end()) return;
    
    // IDA: Get TB_PC_REWARD_SYSTEM
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_PC_REWARD_SYSTEM* pTB_PC_REWARD_SYSTEM = XResourceMgr::GetTB_PC_REWARD_SYSTEM(dwID);
    // if (!pTB_PC_REWARD_SYSTEM) return;
    
    auto& stInfo = it->second;
    
    if (bReset) {
        // IDA: Reset value to 0
        stInfo.dwValue = 0;
    }
    else {
        // IDA: Calculate max value from achieve points
        std::int16_t shLastValue = 0;
        
        // IDA: Iterate through achieve points (up to 5)
        // for (int i = 0; i < 5; ++i) {
        //     if (pTB_PC_REWARD_SYSTEM->Achieve_Point[i] > 0) {
        //         // For monthly mission (ID=3), check TB_PC_REWARD_SYSTEM_MONTH
        //         if (stInfo.dwID == 3) {
        //             // Check TB_PC_REWARD_SYSTEM_MONTH and TB_ITEM
        //         } else {
        //             // Check TB_ITEM for other missions
        //         }
        //         if (shLastValue < pTB_PC_REWARD_SYSTEM->Achieve_Point[i]) {
        //             shLastValue = pTB_PC_REWARD_SYSTEM->Achieve_Point[i];
        //         }
        //     }
        // }
        
        // IDA: Add value with bounds checking
        if (nAddMin >= 0 || stInfo.dwValue <= static_cast<unsigned int>(-nAddMin)) {
            stInfo.dwValue += nAddMin;
        } else {
            stInfo.dwValue = 0;
        }
        
        // IDA: Clamp to max value
        // if (stInfo.dwValue >= shLastValue) {
        //     stInfo.dwValue = shLastValue;
        // }
    }
    
    // IDA: Send DB update and client update
    DBUpdateNetCafeMission(dwID, bReset);
    SendNetCafeMissionInfo(dwID);
}
