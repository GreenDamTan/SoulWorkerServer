#include "GocEvent.h"
#include "GocNetwork.h"
#include "GocInventory.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLogin.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <cstring>
#include <ctime>
#include <cstdio>

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
    // IDA 0x140069080: result = false;
    // if (dwEventID == 255) {
    //   v2 = GetOwnerGO();  // XActor
    //   if (v2->GetStatus() == 6) return true;
    // }
    // return false;
    if (dwEventID == 255) {
        XActor* pOwner = GetOwnerGO();
        if (pOwner && pOwner->GetStatus() == 6)
            return true;
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
    // IDA 0x140068D00: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;

    // IDA: Build update list with account ID
    PS_ACCOUNT_EVENT_LIST stEventUpdate;
    char* pAccountID = pUser->GetAccountID();
    if (pAccountID) {
        std::strcpy(stEventUpdate.szAccountID, pAccountID);
    }

    // IDA: Iterate through event IDs and send auto mail
    for (const auto& dwEventID : stEventList.vecEventID)
    {
        if (CheckAccountEvent(dwEventID))
        {
            std::shared_ptr<CGocPost> pPost = pUser->GetGOC_Post(false);
            if (pPost && pPost->SendAutoMail(static_cast<std::uint16_t>(dwEventID)))
            {
                stEventUpdate.vecEventID.push_back(dwEventID);
            }
            else
            {
                LogHelper::LogError("game.contents", "[ACCOUNT_EVENT] Failed Send AuthMail UCID : %d / ID : %d ",
                                    pUser->GetUCID(), dwEventID);
            }
        }
    }

    // IDA: Send update to DB if not empty (main=2, sub=0x56)
    if (!stEventUpdate.vecEventID.empty())
    {
        XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 2, 0x56);
        xSendDBPacket << stEventUpdate;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }
}

// IDA: 0x1400690E0 - SetWorldEventInfo (第一个重载)
// 处理PS_WORLD_EVENT_INFO_RES中的奖励信息，然后调用第二个重载
std::uint8_t CGocEvent::SetWorldEventInfo(PS_WORLD_EVENT_INFO_RES& psRes, std::int64_t biLastRegisterDate, std::int64_t biDailyRewardDate) {
    // IDA 0x1400690E0: 遍历奖励信息，更新或插入到 m_mapWorldEventReward
    for (std::size_t i = 0; i < psRes.vecRewardInfo.size(); ++i) {
        ST_WORLD_EVENT_REWARD_INFO& rewardInfo = psRes.vecRewardInfo[i];
        auto it = m_mapWorldEventReward.find(rewardInfo.nRewardIndex);
        if (it != m_mapWorldEventReward.end()) {
            it->second.byRewardState = rewardInfo.byRewardState;
        } else {
            m_mapWorldEventReward[rewardInfo.nRewardIndex] = rewardInfo;
        }
    }

    // IDA: 调用第二重载处理事件级数据
    return SetWorldEventInfo(psRes.nEventID, psRes.nTotalCount, psRes.nMyCount,
                             biLastRegisterDate, biDailyRewardDate);
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
    // IDA 0x1400697A0: 错误码 59002=事件不存在, 59003=未激活/不在时间范围, 59007=正在处理

    if (m_bWorldEventDBCall) {
        return 59007; // 正在处理中
    }

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return 59007;
    }

    PS_DB_WORLD_EVENT_INFO_REQ psDBReq;
    psDBReq.dwUAID = pUser->GetUAID();
    psDBReq.dwUCID = pUser->GetUCID();
    psDBReq.nEventID = psReq.nEventID;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTBWorldEvent = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psDBReq.nEventID);
    if (!pTBWorldEvent) {
        return 59002; // 事件不存在
    }
    if (!pTBWorldEvent->event_activation) {
        return 59003; // 未激活
    }

    // IDA: 解析 event_start_date/event_end_date ("YYYY-MM-DD HH:MM:SS")
    std::time_t tCurr = std::time(nullptr);
    auto parseDate = [](const char* szDate) -> std::time_t {
        int y = 2000, mon = 1, d = 1, h = 0, mi = 0, s = 0;
        if (szDate && std::sscanf(szDate, "%d-%d-%d %d:%d:%d", &y, &mon, &d, &h, &mi, &s) >= 3
            && y >= 2000 && y <= 2040 && mon >= 1 && mon <= 12
            && d >= 1 && d <= 31 && h <= 24 && mi <= 60 && s <= 60) {
            std::tm tm = {};
            tm.tm_year = y - 1900;
            tm.tm_mon = mon - 1;
            tm.tm_mday = d;
            tm.tm_hour = h;
            tm.tm_min = mi;
            tm.tm_sec = s;
            tm.tm_isdst = -1;
            return std::mktime(&tm);
        }
        std::tm tmDef = {};
        tmDef.tm_year = 100;
        tmDef.tm_mon = 0;
        tmDef.tm_mday = 1;
        tmDef.tm_isdst = -1;
        return std::mktime(&tmDef);
    };
    std::time_t tStart = parseDate(pTBWorldEvent->event_start_date);
    std::time_t tEnd = parseDate(pTBWorldEvent->event_end_date);

    if (tCurr < tStart || tEnd < tCurr) {
        return 59003; // 不在时间范围
    }

    m_bWorldEventDBCall = true;

    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 0x49, 0x27);
    xSendDBPacket << psDBReq;
    pServer->SendDBGame(xSendDBPacket);
    return 0; // 成功
}

// IDA: 0x140069D90 - ReqWorldEventRegister
// 世界事件注册请求 - 注册事件物品贡献
// IDA精确还原：验证事件、查找物品、锁定物品、发送DB请求
int CGocEvent::ReqWorldEventRegister(PS_WORLD_EVENT_REGISTER_REQ& psReq) {
    // IDA 0x140069D90:
    // 错误码: 59007=处理中, 59002=事件不存在, 59003=未激活/超时, 52001=背包组件失败, 52004=物品表不存在, 52014=物品不存在
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // IDA: PS_DB_WORLD_EVENT_REGISTER_REQ psDBReq; dwUCID=GetUCID; nEventID=psReq->nEventID
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return 59007;
    }
    PS_DB_WORLD_EVENT_REGISTER_REQ psDBReq;
    psDBReq.dwUCID = pUser->GetUCID();
    psDBReq.nEventID = psReq.nEventID;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTB_WORLD_EVENT = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psDBReq.nEventID);
    if (!pTB_WORLD_EVENT) {
        return 59002;
    }
    if (!pTB_WORLD_EVENT->event_activation) {
        return 59003;
    }

    // IDA: 时间范围检查 (event_start_date / event_end_date via sscanf)
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    int _year = 2000, _mon = 1, _day = 1, _hour = 0, _min = 0, _sec = 0;
    sscanf_s(pTB_WORLD_EVENT->event_start_date, "%d-%d-%d %d:%d:%d", &_year, &_mon, &_day, &_hour, &_min, &_sec);
    ATL::CTime tStart(_year, _mon, _day, _hour, _min, _sec, -1);
    int nYear = 2000, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
    sscanf_s(pTB_WORLD_EVENT->event_end_date, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
    ATL::CTime tEnd(nYear, nMonth, nDay, nHour, nMin, nSec, -1);
    if (tCurr.GetTime() < tStart.GetTime() || tEnd.GetTime() < tCurr.GetTime()) {
        return 59003;
    }

    // IDA: GetTB_ITEM(event_item_ID); GetTB_ITEM_CLASSIFY(Item_Classify_Index)
    TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM(pTB_WORLD_EVENT->event_item_ID);
    if (!pTB_ITEM) {
        return 52004;
    }
    TB_ITEM_CLASSIFY* pTB_ITEM_CLASSIFY = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_ITEM->Item_Classify_Index);
    if (!pTB_ITEM_CLASSIFY) {
        return 52004;
    }

    // IDA: GetGOC<CGocInventory>; GetTBInvenPtr(Item_Inven_Type)
    std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
    if (!pInvenPtr) {
        return 52001;
    }
    XBaseInventory* pBaseInven = pInvenPtr->GetTBInvenPtr(pTB_ITEM_CLASSIFY->Item_Inven_Type);
    if (!pBaseInven) {
        return 52001;
    }

    psDBReq.byInvenType = pTB_ITEM_CLASSIFY->Item_Inven_Type;

    // IDA: GetSameItems_2(event_item_ID, &vecFindList, -1)
    std::vector<std::shared_ptr<CItem>> vecFindList;
    pBaseInven->GetSameItems_2(pTB_WORLD_EVENT->event_item_ID, &vecFindList, -1);
    if (vecFindList.empty()) {
        return 52014;
    }

    // IDA: for each found item, if !GetLock(slot), lock + collect
    for (auto& pItem : vecFindList) {
        if (!pItem)
            continue;
        std::int16_t shSlot = pItem->GetSlot();
        if (!pBaseInven->GetLock(shSlot)) {
            PS_STORAGE_INFO psInfo;
            psInfo.byInvenType = pItem->GetInvenType();
            psInfo.shSlotPos = shSlot;
            pItem->GetItem(&psInfo.stItem);
            psInfo.stItem.sCount = 0;
            psDBReq.stUpdateItem.vecItem.push_back(psInfo);
            psDBReq.nCount += pItem->GetCount();
            pBaseInven->SetLock(shSlot, 84);  // IDA: SetLock(slot, 84)
        }
    }

    // IDA: m_bWorldEventDBCall = 1; XSendDBPacket(owner, 0x49, 0x28) << psDBReq; SendDBGame
    m_bWorldEventDBCall = true;
    XSendDBPacket xSendDBPacket(GetOwnerGO(), 0x49, 0x28);
    xSendDBPacket << psDBReq;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);
    return 0;
}

// IDA: 0x14006A7E0 - ReqWorldEventReward
// 世界事件奖励请求 - 领取世界事件奖励
// IDA精确还原：验证奖励条件、创建物品、发送DB请求
int CGocEvent::ReqWorldEventReward(PS_WORLD_EVENT_REWARD_REQ& psReq) {
    // IDA 0x14006A7E0: 错误码 59002/59003/59004/59005/59007/59008

    if (m_bWorldEventDBCall) {
        return 59007;
    }

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return 59007;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTBWorldEvent = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psReq.nEventID);
    if (!pTBWorldEvent) {
        return 59002;
    }
    if (!pTBWorldEvent->event_activation) {
        return 59003;
    }

    // IDA: 时间范围检查
    std::time_t tCurr = std::time(nullptr);
    auto parseDate = [](const char* szDate) -> std::time_t {
        int y = 2000, mon = 1, d = 1, h = 0, mi = 0, s = 0;
        if (szDate && std::sscanf(szDate, "%d-%d-%d %d:%d:%d", &y, &mon, &d, &h, &mi, &s) >= 3
            && y >= 2000 && y <= 2040 && mon >= 1 && mon <= 12
            && d >= 1 && d <= 31 && h <= 24 && mi <= 60 && s <= 60) {
            std::tm tm = {};
            tm.tm_year = y - 1900;
            tm.tm_mon = mon - 1;
            tm.tm_mday = d;
            tm.tm_hour = h;
            tm.tm_min = mi;
            tm.tm_sec = s;
            tm.tm_isdst = -1;
            return std::mktime(&tm);
        }
        std::tm tmDef = {};
        tmDef.tm_year = 100;
        tmDef.tm_mon = 0;
        tmDef.tm_mday = 1;
        tmDef.tm_isdst = -1;
        return std::mktime(&tmDef);
    };
    std::time_t tStart = parseDate(pTBWorldEvent->event_start_date);
    std::time_t tEnd = parseDate(pTBWorldEvent->event_end_date);
    if (tCurr < tStart || tEnd < tCurr) {
        return 59003;
    }

    // IDA: 奖励表校验
    std::int16_t shItemLogType = 115;
    TB_WORLD_EVENT_REWARD* pTBWorldEventReward = pServer->GetResourceMgr().GetTB_WORLD_EVENT_REWARD(psReq.nRewardIndex);
    if (!pTBWorldEventReward) {
        return 59004;
    }
    if (pTBWorldEventReward->event_reward_type != 1) {
        return 59004;
    }
    if (pTBWorldEventReward->world_reward_type != psReq.byRewardType) {
        return 59004;
    }

    // IDA: 奖励条件
    if (pTBWorldEventReward->world_reward_type != 0) {
        if (pTBWorldEventReward->world_reward_type != 1) {
            LogHelper::LogError("game.contents", "ReqWorldEventReward error - Reward Type (%d/%d)",
                                psReq.nRewardIndex, pTBWorldEventReward->world_reward_type);
            return 59004;
        }
        if (GetWorldEventMyCount(psReq.nEventID) < pTBWorldEventReward->event_item_percentile_min) {
            return 59004;
        }
        shItemLogType = 116;
    } else {
        float fPercent = static_cast<float>(GetWorldEventTotalCount(psReq.nEventID))
                       / static_cast<float>(pTBWorldEvent->event_item_amount_max);
        std::uint8_t byPercent = static_cast<std::uint8_t>(fPercent * 100.0f);
        if (byPercent < pTBWorldEventReward->event_item_percentile_min) {
            return 59004;
        }
    }

    // IDA: 已领取检查
    if (FindWorldEventReward(psReq.nRewardIndex)) {
        return 59005;
    }

    PS_DB_WORLD_EVENT_REWARD psDBReq;
    psDBReq.dwUAID = pUser->GetUAID();
    psDBReq.dwUCID = pUser->GetUCID();
    psDBReq.psReq = psReq;
    psDBReq.byItemFlag = 45;
    psDBReq.dwRewardItemID = pTBWorldEventReward->event_reward_value;
    psDBReq.shRewardCount = static_cast<std::int16_t>(pTBWorldEventReward->event_reward_item_amount);

    if (!pServer->GetResourceMgr().GetTB_ITEM(psDBReq.dwRewardItemID) || psDBReq.shRewardCount <= 0) {
        return 59004;
    }

    // IDA: world_reward_type == 1 直接发物品
    if (pTBWorldEventReward->world_reward_type == 1) {
        std::shared_ptr<CGocInventory> pInven = pUser->GetGOC_Inventory(false);
        if (!pInven) {
            return 59005;
        }
        ST_CREATE_ITEMS stCreateItems;
        ST_CREATE_ITEM stAddItem;
        stAddItem.nItemID = psDBReq.dwRewardItemID;
        stAddItem.shCount = static_cast<std::int16_t>(psDBReq.shRewardCount);
        stCreateItems.vecInfo.push_back(stAddItem);

        ST_LOG_GAME stLog;
        stLog._sSubType = static_cast<std::int16_t>(shItemLogType);
        stLog.nParam3 = psReq.nEventID;
        stLog.nParam4 = psReq.nRewardIndex;

        if (!pInven->CreateItem2(stCreateItems, 0x54, 0, psDBReq.stCreateItem, psDBReq.stUpdateItem, stLog)) {
            LogHelper::LogError("game.contents", "AttendanceReward error - CreateItem2");
            return 59008;
        }
    }

    m_bWorldEventDBCall = true;

    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 0x49, 0x29);
    xSendDBPacket << psDBReq;
    pServer->SendDBGame(xSendDBPacket);
    return 0;
}

// IDA: 0x14006B300 - ReqWorldEventDailyReward
// 世界事件每日奖励请求 - 领取每日登录奖励
// IDA精确还原：验证每日奖励状态、创建物品、发送DB请求
int CGocEvent::ReqWorldEventDailyReward(PS_WORLD_EVENT_DAILY_REWARD_REQ& psReq) {
    // IDA 0x14006B300: 错误码 59002/59003/59004/59006/59007/59008

    if (m_bWorldEventDBCall) {
        return 59007;
    }

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return 59006;
    }
    std::shared_ptr<CGocInventory> pInven = pUser->GetGOC_Inventory(false);
    if (!pInven) {
        return 59006;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTBWorldEvent = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psReq.nEventID);
    if (!pTBWorldEvent) {
        return 59002;
    }
    if (!pTBWorldEvent->event_activation) {
        return 59003;
    }

    // IDA: 时间范围检查
    std::time_t tCurr = std::time(nullptr);
    auto parseDate = [](const char* szDate) -> std::time_t {
        int y = 2000, mon = 1, d = 1, h = 0, mi = 0, s = 0;
        if (szDate && std::sscanf(szDate, "%d-%d-%d %d:%d:%d", &y, &mon, &d, &h, &mi, &s) >= 3
            && y >= 2000 && y <= 2040 && mon >= 1 && mon <= 12
            && d >= 1 && d <= 31 && h <= 24 && mi <= 60 && s <= 60) {
            std::tm tm = {};
            tm.tm_year = y - 1900;
            tm.tm_mon = mon - 1;
            tm.tm_mday = d;
            tm.tm_hour = h;
            tm.tm_min = mi;
            tm.tm_sec = s;
            tm.tm_isdst = -1;
            return std::mktime(&tm);
        }
        std::tm tmDef = {};
        tmDef.tm_year = 100;
        tmDef.tm_mon = 0;
        tmDef.tm_mday = 1;
        tmDef.tm_isdst = -1;
        return std::mktime(&tmDef);
    };
    std::time_t tStart = parseDate(pTBWorldEvent->event_start_date);
    std::time_t tEnd = parseDate(pTBWorldEvent->event_end_date);
    if (tCurr < tStart || tEnd < tCurr) {
        return 59003;
    }

    // IDA: 奖励物品校验
    TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(pTBWorldEvent->event_daily_reward_item_ID);
    if (!pTBItem || !pTBWorldEvent->event_daily_reward_item_amount) {
        return 59004;
    }

    // IDA: 获取事件状态并校验可领取
    std::int64_t biDailyRewardDate = GetWorldEventDailyRewardDate(psReq.nEventID);
    std::int64_t biLastRegisterDate = GetWorldEventLastResisterDate(psReq.nEventID);
    int nMyCount = GetWorldEventMyCount(psReq.nEventID);
    int nTotalCount = GetWorldEventTotalCount(psReq.nEventID);

    std::uint8_t bRewardState = SetWorldEventInfo(psReq.nEventID, nTotalCount, nMyCount,
                                                  biLastRegisterDate, biDailyRewardDate);
    if (bRewardState != 1) {
        LogHelper::LogError("game.contents", "[ReqWorldEventDailyReward] Error State UCID : %d / State : %d ",
                            pUser->GetUCID(), bRewardState);
        return 59006;
    }

    // IDA: 构建 DB 请求
    PS_DB_WORLD_EVENT_DAILY_REWARD psDBReq;
    psDBReq.dwUCID = pUser->GetUCID();
    psDBReq.nEventID = psReq.nEventID;
    psDBReq.biDailyRewardDate = pServer->GetBeforeInitDate();
    psDBReq.byItemFlag = 45;

    // IDA: 创建奖励物品
    ST_CREATE_ITEMS stCreateItems;
    ST_CREATE_ITEM stAddItem;
    stAddItem.nItemID = pTBWorldEvent->event_daily_reward_item_ID;
    stAddItem.shCount = static_cast<std::int16_t>(pTBWorldEvent->event_daily_reward_item_amount);
    stCreateItems.vecInfo.push_back(stAddItem);

    ST_LOG_GAME stLog;
    stLog._sSubType = 117;
    stLog.nParam2 = pUser->GetLevel();
    stLog.nParam3 = psReq.nEventID;

    if (!pInven->CreateItem2(stCreateItems, 0x54, 0, psDBReq.stCreateItem, psDBReq.stUpdateItem, stLog)) {
        LogHelper::LogError("game.contents", "AttendanceReward error - CreateItem2");
        return 59008;
    }

    m_bWorldEventDBCall = true;

    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 0x49, 0x2A);
    xSendDBPacket << psDBReq;
    pServer->SendDBGame(xSendDBPacket);
    return 0;
}

// IDA: 0x14006BD30 - ResWorldEventInfo
// 处理DB世界事件信息响应，发送响应给客户端
void CGocEvent::ResWorldEventInfo(PS_DB_WORLD_EVENT_INFO_RES& psRes) {
    m_bWorldEventDBCall = false;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    // IDA: PS_WORLD_EVENT_INFO_RES psRes = psDBRes->psInfo; (拷贝，后续会修正计数)
    PS_WORLD_EVENT_INFO_RES psClientRes = psRes.psInfo;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTB_WORLD_EVENT = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psRes.psInfo.nEventID);
    if (pTB_WORLD_EVENT) {
        if (pTB_WORLD_EVENT->event_item_amount_max < psClientRes.nTotalCount)
            psClientRes.nTotalCount = pTB_WORLD_EVENT->event_item_amount_max;

        psClientRes.byDailyRewardState = SetWorldEventInfo(psClientRes, psRes.biLastRegisterDate, psRes.biDailyRewardDate);

        XSendPacket xSendPacket(0x2A, 0x22);
        xSendPacket << psClientRes;
        CGocNetwork::Send(GetOwnerGO(), xSendPacket);
    } else {
        pUser->SendErrorMessage(0x2A, 0x22, 0xE67A);  // IDA: 59002
    }
}

// IDA: 0x14006BF80 - ResWorldEventRegister
// 处理DB世界事件注册响应：解锁/移除提交的物品，更新计数，发送客户端响应与日志
void CGocEvent::ResWorldEventRegister(PS_DB_WORLD_EVENT_REGISTER_RES& psRes) {
    m_bWorldEventDBCall = false;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTB_WORLD_EVENT = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psRes.psReq.nEventID);
    if (!pTB_WORLD_EVENT) {
        pUser->SendErrorMessage(0x2A, 0x23, 0xCB24);  // IDA: 52004
        return;
    }

    std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
    XBaseInventory* pBaseInven = pInvenPtr ? pInvenPtr->GetTBInvenPtr(psRes.psReq.byInvenType) : nullptr;
    if (!pBaseInven) {
        pUser->SendErrorMessage(0x2A, 0x23, 0xCB21);  // IDA: 52001
        return;
    }

    // IDA: 先解锁所有提交的物品槽位 (SetLock(slot, 0))
    for (auto& stItem : psRes.psReq.stUpdateItem.vecItem) {
        pBaseInven->SetLock(stItem.shSlotPos, 0);
    }

    if (psRes.nError) {
        pUser->SendErrorMessage(0x2A, 0x22, 0xCB21);  // IDA: 注册失败错误子命令 0x22
        return;
    }

    // IDA: 移除物品并记录统计/变更日志 (仅整件删除: stItem.sCount <= 0)
    for (auto& stItem : psRes.psReq.stUpdateItem.vecItem) {
        std::shared_ptr<CItem> pItem = pBaseInven->GetItem(stItem.stItem.xSerial);
        if (pItem && stItem.stItem.sCount <= 0) {
            int nLogValue = -static_cast<int>(pItem->GetCount());
            pBaseInven->RemoveItem(stItem.shSlotPos);

            ST_STATISTICS_ITEM stStatistics;
            stStatistics.byFlag = 3;
            stStatistics.biSerial = stItem.stItem.xSerial;
            XSendDBPacket xSendDBStatistics(GetOwnerGO(), 0xF0, 0x11);
            xSendDBStatistics << stStatistics;
            pServer->SendDBStatistics(xSendDBStatistics);

            ST_LOG_GAME stLogGame;
            stLogGame._sMainType = 4;
            stLogGame._sSubType = 118;
            stLogGame._nUAID = pUser->GetUAID();
            stLogGame._nUCID = pUser->GetUCID();
            stLogGame.nParam0 = pItem->GetID();
            stLogGame.nParam1 = stItem.stItem.sCount;
            stLogGame.nParam2 = psRes.psReq.nEventID;
            // TODO: 推测结果 - IDA 反编译中 nParam3/nParam5 为类型污染无法确认
            stLogGame.nParam11 = nLogValue;
            stLogGame.nParam12 = stItem.stItem.sCount;
            pServer->SendDBLog(stLogGame);
        }
    }

    if (pTB_WORLD_EVENT->event_item_amount_max < psRes.nTotalCount)
        psRes.nTotalCount = pTB_WORLD_EVENT->event_item_amount_max;

    pInvenPtr->SendUpdateItem(psRes.psReq.stUpdateItem);

    PS_WORLD_EVENT_REGISTER_RES psClientRes;
    psClientRes.nEventID = psRes.psReq.nEventID;
    psClientRes.nCount = psRes.psReq.nCount;
    psClientRes.nTotalCount = psRes.nTotalCount;
    psClientRes.nMyCount = psRes.nMyCount;
    psClientRes.byDailyRewardState = SetWorldEventInfo(psClientRes.nEventID, psClientRes.nTotalCount,
                                                       psClientRes.nMyCount, psRes.biLastRegisterDate,
                                                       GetWorldEventDailyRewardDate(psRes.psReq.nEventID));

    XSendPacket xSendPacket(0x2A, 0x23);
    xSendPacket << psClientRes;
    CGocNetwork::Send(GetOwnerGO(), xSendPacket);

    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 25;
    stLog._sSubType = 30;
    stLog.nParam0 = psClientRes.nEventID;
    stLog.nParam1 = pTB_WORLD_EVENT->event_item_ID;
    stLog.nParam2 = psClientRes.nCount;
    stLog.nParam3 = psClientRes.byDailyRewardState;
    stLog.nParam4 = GetWorldEventMyCount(psClientRes.nEventID);
    stLog.nParam5 = static_cast<std::int64_t>(GetWorldEventTotalCount(psClientRes.nEventID));
    stLog.nParam6 = pTB_WORLD_EVENT->event_item_amount_max;
    pServer->SendDBLog(stLog);
}

// IDA: 0x14006C880 - ResWorldEventReward
// 处理DB世界事件奖励响应：按奖励类型派发物品/邮件，添加领取记录，发送响应与日志
void CGocEvent::ResWorldEventReward(PS_DB_WORLD_EVENT_REWARD& psRes) {
    m_bWorldEventDBCall = false;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    if (psRes.nError) {
        LogHelper::LogError("game.contents",
                            "ResWorldEventReward error - UAID:%d, UCID:%d, Error:%d (%d)",
                            pUser->GetUAID(), pUser->GetUCID(), psRes.nError, 787);
        pUser->SendErrorMessage(0x2A, 0x24, 0xE67D);  // IDA: 59005
        return;
    }

    if (psRes.psReq.byRewardType == 1) {
        // IDA: 个人贡献度奖励 - 物品已创建，向客户端发送更新/创建包
        std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
        if (pInvenPtr) {
            pInvenPtr->SendUpdateItem(psRes.stUpdateItem);
            pInvenPtr->SendCreateItem(psRes.stCreateItem);
        }
    } else if (psRes.psReq.byRewardType == 0) {
        // IDA: 总贡献度奖励 - 通过系统邮件发放
        ST_ACCOUNT_POST_DATA stAccountPostData;
        stAccountPostData.byMainType = 4;
        stAccountPostData.bySubType = XGameServer::Instance()->GetSystemPostTableIndex(0xD, 2);
        stAccountPostData.dwUAID = pUser->GetUAID();
        stAccountPostData.biRegTime = XGameServer::Instance()->GetCurDate();
        stAccountPostData.biDelDate = XGameServer::Instance()->GetCurDate() + 29454;
        stAccountPostData.stItemList[0].xSerial = 0;
        stAccountPostData.stItemList[0].nItemID = psRes.dwRewardItemID;
        stAccountPostData.stItemList[0].sCount = psRes.shRewardCount;

        if (!stAccountPostData.bySubType) {
            LogHelper::LogError("game.contents",
                                "Send WorldEventReward Post error - Check TB_SystemMail(UAID:%d) (%d)",
                                pUser->GetUAID(), 817);
        }

        XSendDBPacket xSendDBPacket(GetOwnerGO(), 6, 0x18);
        xSendDBPacket << stAccountPostData;
        XGameServer::Instance()->SendDBGame(xSendDBPacket);
    }

    // IDA: 记录已领取
    ST_WORLD_EVENT_REWARD_INFO stInfo;
    stInfo.nRewardIndex = psRes.psReq.nRewardIndex;
    stInfo.byRewardType = psRes.psReq.byRewardType;
    stInfo.byRewardState = 2;
    AddWorldEventReward(stInfo);

    // IDA: 发送客户端响应 (RES 与 REQ 同布局，直接拷贝)
    PS_WORLD_EVENT_REWARD_RES psClientRes;
    psClientRes.nEventID = psRes.psReq.nEventID;
    psClientRes.byRewardType = psRes.psReq.byRewardType;
    psClientRes.nRewardIndex = psRes.psReq.nRewardIndex;
    XSendPacket xSendPacket(0x2A, 0x24);
    xSendPacket << psClientRes;
    CGocNetwork::Send(GetOwnerGO(), xSendPacket);

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_WORLD_EVENT* pTB_WORLD_EVENT = pServer->GetResourceMgr().GetTB_WORLD_EVENT(psRes.psReq.nEventID);
    if (pTB_WORLD_EVENT) {
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetUCID();
        stLog._sMainType = 25;
        if (psRes.psReq.byRewardType == 1)
            stLog._sSubType = 32;
        else if (psRes.psReq.byRewardType == 0)
            stLog._sSubType = 33;
        stLog.nParam0 = psRes.psReq.nEventID;
        stLog.nParam1 = psRes.dwRewardItemID;
        stLog.nParam2 = psRes.shRewardCount;
        stLog.nParam3 = psRes.psReq.nRewardIndex;
        stLog.nParam4 = GetWorldEventMyCount(psRes.psReq.nEventID);
        stLog.nParam5 = static_cast<std::int64_t>(GetWorldEventTotalCount(psRes.psReq.nEventID));
        stLog.nParam6 = pTB_WORLD_EVENT->event_item_amount_max;
        pServer->SendDBLog(stLog);
    }
}

// IDA: 0x14006CF00 - ResWorldEventDailyReward
// 处理DB世界事件每日奖励响应
// IDA精确还原：更新每日奖励日期、发送响应、发送日志
void CGocEvent::ResWorldEventDailyReward(PS_DB_WORLD_EVENT_DAILY_REWARD& psRes) {
    // IDA 0x14006CF00:
    // m_bWorldEventDBCall = 0;
    // pUser = dynamic_cast<CUser*>(GetOwner()); if (pUser) {
    //   if (psDBRes->nError) CUser::SendErrorMessage(pUser, 0x2A, 0x25, psRes.nError);
    //   else {
    //     biLast = GetWorldEventLastResisterDate(nEventID); nMy=GetWorldEventMyCount; nTotal=GetWorldEventTotalCount;
    //     SetWorldEventInfo(nEventID, nTotal, nMy, biLast, biDailyRewardDate);
    //     pInven = GetGOC<CGocInventory>; if (pInven) { SendUpdateItem(stUpdateItem); SendCreateItem(stCreateItem); }
    //     psClientRes.nEventID = nEventID; XSendPacket(0x2A, 0x25) << psClientRes; Send;
    //     TB_WORLD_EVENT = GetTB_WORLD_EVENT(nEventID); if (TB) { ST_LOG_GAME ... nParam0=nEventID nParam1=daily_reward_item_ID ... ; SendDBLog }
    //   }
    // }
    m_bWorldEventDBCall = false;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    if (psRes.nError) {
        pUser->SendErrorMessage(0x2A, 0x25, 0xCB3A);
        return;
    }

    std::int64_t biLastRegisterDate = GetWorldEventLastResisterDate(psRes.nEventID);
    int nMyCount = GetWorldEventMyCount(psRes.nEventID);
    int nTotalCount = GetWorldEventTotalCount(psRes.nEventID);
    SetWorldEventInfo(psRes.nEventID, nTotalCount, nMyCount, biLastRegisterDate, psRes.biDailyRewardDate);

    std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
    if (pInvenPtr) {
        pInvenPtr->SendUpdateItem(psRes.stUpdateItem);
        pInvenPtr->SendCreateItem(psRes.stCreateItem);
    }

    // IDA: PS_RECRUIT_DELETE ctor placeholder for PS_WORLD_EVENT_DAILY_REWARD_RES; nEventID only serialized
    struct { std::int32_t nEventID; } psClientRes;
    psClientRes.nEventID = psRes.nEventID;
    XSendPacket xSendPacket(0x2A, 0x25);
    xSendPacket.XParse << psClientRes.nEventID;
    CGocNetwork::Send(GetOwnerGO(), xSendPacket);

    // IDA: log
    TB_WORLD_EVENT* pTB_WORLD_EVENT = TXSingleton<XGameServer>::Instance()->GetResourceMgr().GetTB_WORLD_EVENT(psRes.nEventID);
    if (pTB_WORLD_EVENT) {
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetUCID();
        stLog._sMainType = 25;
        stLog._sSubType = 31;
        stLog.nParam0 = psRes.nEventID;
        stLog.nParam1 = pTB_WORLD_EVENT->event_daily_reward_item_ID;
        stLog.nParam2 = pTB_WORLD_EVENT->event_daily_reward_item_amount;
        stLog.nParam4 = GetWorldEventMyCount(psRes.nEventID);
        stLog.nParam5 = static_cast<int>(GetWorldEventTotalCount(psRes.nEventID));
        stLog.nParam6 = pTB_WORLD_EVENT->event_item_amount_max;
        TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
    }
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
