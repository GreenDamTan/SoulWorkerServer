#include "GocEvent.h"
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
    , m_bRouletteUse(false)
    , m_bNetCafeMission(false)
    , m_nNetCafeNextDay(0)
    , m_dw64NetCafeUpdateTick(0)
    , m_nNetCafeMission_InitHour(9)
    , m_dw64NetCafeDBUpdateTick(0)
{
    // m_mapWorldEvent, m_mapWorldEventReward, m_mapNetCafeMission
    // 由 std::map 默认构造函数初始化
    // m_stRouletteInfoData 由默认初始化为零
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

    // Reset roulette info
    m_stRouletteInfoData.nDayCount = 0;
    m_stRouletteInfoData.biRegDate = 0;
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
// char __fastcall CGocEvent::AddWorldEventReward(CGocEvent *this, ST_LEVEL_UP_EVENT_DATA *stInfo)
bool CGocEvent::AddWorldEventReward(ST_LEVEL_UP_EVENT_DATA& stInfo) {
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
    return m_stRouletteInfoData.nDayCount;
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
void CGocEvent::SetRouletteDayCount(int nCount) {
    m_stRouletteInfoData.nDayCount = nCount;
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
    // TODO: 需要访问 CMover/CUser 来检查状态
    // 特殊事件ID 255 检查用户某个状态是否为6
    if (dwEventID == 255) {
        // 需要通过 CMover 获取用户状态进行检查
        // 暂时返回 false 作为占位符
    }
    return false;
}

// ============================================================================
// 世界事件请求/响应函数
// ============================================================================

// IDA: 0x140068AA0 - RequestLoadAccountEvent
// 发送账号事件加载请求到DB
void CGocEvent::RequestLoadAccountEvent() {
    // TODO: 需要 CUser, XSendDBPacket, XGameServer 等类的支持
    // 流程: 获取CUser -> 构造PS_ACCOUNT_EVENT_LIST -> 发送到DB
}

// IDA: 0x140068D00 - LoadAccountEvent
// 加载账号事件列表并处理
void CGocEvent::LoadAccountEvent(PS_ACCOUNT_EVENT_LIST& stEventList) {
    // TODO: 需要 CUser, CGocPost, XSendDBPacket 等类的支持
    // 流程: 遍历事件列表 -> 检查账号事件 -> 发送邮件 -> 更新DB
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
    // 设置默认时间边界 (2000-01-01 09:00:00)
    // ATL::CTime tDefault(2000, 1, 1, 9, 0, 0, -1);
    // std::int64_t nDefaultTime = tDefault.GetTime();

    // // 修正无效日期
    // if (biLastRegisterDate <= nDefaultTime)
    //     biLastRegisterDate = nDefaultTime;
    // if (biDailyRewardDate <= nDefaultTime)
    //     biDailyRewardDate = nDefaultTime;

    // 查找或创建事件条目
    auto it = m_mapWorldEvent.find(nEventID);
    if (it != m_mapWorldEvent.end()) {
        // 更新现有条目
        it->second.nTotalCount = nTotalCount;
        it->second.nMyCount = nMyCount;
        it->second.biLastRegisterDate = biLastRegisterDate;
        it->second.biDailyRewardDate = biDailyRewardDate;
    } else {
        // 插入新条目
        ST_WORLD_EVENT_BOOSTER stInfo;
        stInfo.nEventID = nEventID;
        stInfo.nTotalCount = nTotalCount;
        stInfo.nMyCount = nMyCount;
        stInfo.biLastRegisterDate = biLastRegisterDate;
        stInfo.biDailyRewardDate = biDailyRewardDate;
        m_mapWorldEvent[nEventID] = stInfo;
    }

    // TODO: 需要从 XGameServer 获取当前日期来判断奖励状态
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // std::int64_t biToday = XGameServer::GetBeforeInitDate(pServer);

    // 简化逻辑: 返回基本状态判断
    // if (biDailyRewardDate >= biToday) return 2; // 已领取
    // if (biDailyRewardDate < biToday && biToday <= biLastRegisterDate) return 1; // 可领取
    return 0; // 无奖励
}

// IDA: 0x1400697A0 - ReqWorldEventInfo
// 请求世界事件信息，发送DB请求
int CGocEvent::ReqWorldEventInfo(PS_WORLD_EVENT_INFO_REQ& psReq) {
    // 检查是否正在等待DB响应
    if (m_bWorldEventDBCall) {
        return 59007; // 正在处理中
    }

    // TODO: 获取CUser进行用户验证
    // CUser* pUser = GetOwner<CUser>();
    // if (!pUser) return 59007;

    // TODO: 从XGameServer获取TB_WORLD_EVENT表数据
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(&pServer->m_xResourceMgr, psReq.nEventID);
    // if (!pTB_WORLD_EVENT) return 59002; // 事件不存在
    // if (!pTB_WORLD_EVENT->event_activation) return 59003; // 事件未激活

    // TODO: 检查事件时间范围
    // ATL::CTime tCurr, tStart, tEnd;
    // ATL::CTime::GetTickCount(&tCurr);
    // 解析 event_start_date 和 event_end_date 字符串
    // 如果不在时间范围内，返回 59003

    // 设置DB调用标志
    m_bWorldEventDBCall = true;

    // TODO: 发送DB请求
    // PS_DB_WORLD_EVENT_INFO_REQ psDBReq;
    // psDBReq.dwUAID = pUser->GetUAID();
    // psDBReq.dwUCID = pUser->GetUCID();
    // psDBReq.nEventID = psReq.nEventID;
    // XSendDBPacket xSendDBPacket(pObject, 0x49, 0x27);
    // xSendDBPacket << psDBReq;
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    (void)psReq;
    return 0; // 成功
}

// IDA: 0x140069D90 - ReqWorldEventRegister
// 世界事件注册请求
int CGocEvent::ReqWorldEventRegister(PS_WORLD_EVENT_REGISTER_REQ& psReq) {
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // TODO: 完整实现需要:
    // 1. 验证事件存在和激活状态
    // 2. 验证事件时间范围
    // 3. 获取物品信息 (TB_ITEM, TB_ITEM_CLASSIFY)
    // 4. 获取背包组件 (CGocInventory)
    // 5. 查找并锁定对应物品
    // 6. 发送DB注册请求

    m_bWorldEventDBCall = true;
    (void)psReq;
    return 0;
}

// IDA: 0x14006A7E0 - ReqWorldEventReward
// 世界事件奖励请求
int CGocEvent::ReqWorldEventReward(PS_WORLD_EVENT_REWARD_REQ& psReq) {
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // TODO: 完整实现需要:
    // 1. 验证事件存在和激活状态
    // 2. 验证事件时间范围
    // 3. 获取TB_WORLD_EVENT_REWARD表数据
    // 4. 检查奖励类型和个人/总贡献度
    // 5. 检查是否已领取该奖励 (FindWorldEventReward)
    // 6. 创建物品或发送邮件
    // 7. 发送DB奖励请求

    m_bWorldEventDBCall = true;
    (void)psReq;
    return 0;
}

// IDA: 0x14006B300 - ReqWorldEventDailyReward
// 世界事件每日奖励请求
int CGocEvent::ReqWorldEventDailyReward(PS_WORLD_EVENT_DAILY_REWARD_REQ& psReq) {
    if (m_bWorldEventDBCall) {
        return 59007;
    }

    // TODO: 完整实现需要:
    // 1. 验证用户
    // 2. 获取背包组件
    // 3. 验证事件存在和激活状态
    // 4. 验证事件时间范围
    // 5. 检查每日奖励领取状态 (SetWorldEventInfo返回值)
    // 6. 创建奖励物品
    // 7. 发送DB每日奖励请求

    m_bWorldEventDBCall = true;
    (void)psReq;
    return 0;
}

// IDA: 0x14006BD30 - ResWorldEventInfo
// 处理DB世界事件信息响应
void CGocEvent::ResWorldEventInfo(PS_DB_WORLD_EVENT_INFO_RES& psRes) {
    // 重置DB调用标志
    m_bWorldEventDBCall = false;

    // TODO: 获取CUser
    // CUser* pUser = GetOwner<CUser>();
    // if (!pUser) return;

    // TODO: 获取TB_WORLD_EVENT验证事件
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_WORLD_EVENT* pTB_WORLD_EVENT = XResourceMgr::GetTB_WORLD_EVENT(&pServer->m_xResourceMgr, psRes.psInfo.nEventID);
    // if (!pTB_WORLD_EVENT) {
    //     CUser::SendErrorMessage(pUser, 0x2A, 0x22, 0xE67A);
    //     return;
    // }

    // 限制总计数不超过最大值
    // if (pTB_WORLD_EVENT->event_item_amount_max < psRes.psInfo.nTotalCount)
    //     psRes.psInfo.nTotalCount = pTB_WORLD_EVENT->event_item_amount_max;

    // 设置世界事件信息
    // psRes.byDailyRewardState = SetWorldEventInfo(psRes, psRes.biLastRegisterDate, psRes.biDailyRewardDate);

    // TODO: 发送响应包给客户端
    // XSendPacket xSendPacket(0x2A, 0x22);
    // xSendPacket << psRes;
    // CGocNetwork::Send(&pUser->XActor, &xSendPacket);

    (void)psRes;
}

// IDA: 0x14006BF80 - ResWorldEventRegister
// 处理DB世界事件注册响应
void CGocEvent::ResWorldEventRegister(PS_DB_WORLD_EVENT_REGISTER_RES& psRes) {
    // 重置DB调用标志
    m_bWorldEventDBCall = false;

    // TODO: 获取CUser和CGocInventory
    // CUser* pUser = GetOwner<CUser>();
    // if (!pUser) return;

    // TODO: 获取TB_WORLD_EVENT验证事件
    // TODO: 获取背包组件处理物品锁定/解锁
    // TODO: 如果有错误，发送错误消息并解锁物品

    // 如果成功:
    // 1. 移除使用的物品并发送统计日志
    // 2. 更新世界事件信息
    // 3. 发送注册成功响应给客户端
    // 4. 发送日志到DB

    (void)psRes;
}

// IDA: 0x14006C880 - ResWorldEventReward
// 处理DB世界事件奖励响应
void CGocEvent::ResWorldEventReward(PS_DB_WORLD_EVENT_REWARD& psRes) {
    // 重置DB调用标志
    m_bWorldEventDBCall = false;

    // TODO: 获取CUser
    // if (psRes.nError) {
    //     CUser::SendErrorMessage(pUser, 0x2A, 0x24, 0xE67D);
    //     return;
    // }

    // 根据奖励类型处理:
    // if (psRes.psReq.byRewardType == 1) {
    //     // 直接给物品 - 发送更新/创建物品包
    // } else {
    //     // 通过邮件发送 - 发送ST_ACCOUNT_POST_DATA到DB
    // }

    // 添加奖励到已领取列表
    // ST_LEVEL_UP_EVENT_DATA stInfo;
    // stInfo.nRewardIndex = psRes.psReq.nRewardIndex;
    // stInfo.byRewardType = psRes.psReq.byRewardType;
    // stInfo.byRewardState = 2; // 已领取
    // AddWorldEventReward(stInfo);

    // 发送奖励响应给客户端
    // 发送日志到DB

    (void)psRes;
}

// IDA: 0x14006CF00 - ResWorldEventDailyReward
// 处理DB世界事件每日奖励响应
void CGocEvent::ResWorldEventDailyReward(PS_DB_WORLD_EVENT_DAILY_REWARD& psRes) {
    // 重置DB调用标志
    m_bWorldEventDBCall = false;

    // TODO: 获取CUser
    // if (psRes.nError) {
    //     CUser::SendErrorMessage(pUser, 0x2A, 0x25, 0xCB3A);
    //     return;
    // }

    // 更新世界事件信息
    // std::int64_t biLastRegisterDate = GetWorldEventLastResisterDate(psRes.nEventID);
    // int nMyCount = GetWorldEventMyCount(psRes.nEventID);
    // int nTotalCount = GetWorldEventTotalCount(psRes.nEventID);
    // SetWorldEventInfo(psRes.nEventID, nTotalCount, nMyCount, biLastRegisterDate, psRes.biDailyRewardDate);

    // 发送更新/创建物品包给客户端
    // 发送每日奖励响应给客户端
    // 发送日志到DB

    (void)psRes;
}

// ============================================================================
// 轮盘事件函数
// ============================================================================

// IDA: 0x14006D310 - SendDBRouletteInfo
// 发送轮盘信息到DB
void CGocEvent::SendDBRouletteInfo(std::uint8_t byUseType, int nEventID) {
    // TODO: 获取CUser
    // CUser* pUser = GetOwner<CUser>();
    // if (!pUser) return;

    // PS_DB_ROULETTE_EVENT_INFO psDBRouletteInfo;
    // psDBRouletteInfo.nEventID = nEventID;
    // psDBRouletteInfo.dwUAID = pUser->GetUAID();
    // if (byUseType != 1) {
    //     psDBRouletteInfo.dwUCID = pUser->GetUCID();
    // }

    // XSendDBPacket xSendDBPacket(pObject, 0x49, 0x2B);
    // xSendDBPacket << psDBRouletteInfo;
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    (void)byUseType;
    (void)nEventID;
}

// IDA: 0x14006D4A0 - LoadRouletteEventInfo
// 加载轮盘事件信息并检查是否需要重置每日计数
void CGocEvent::LoadRouletteEventInfo(PS_ROULETTE_INFO& psInfo) {
    // TODO: 需人工审查 - PS_ROULETTE_INFO is incomplete type
    // 复制轮盘信息
    // qmemcpy(&m_psRouletteInfo, &psInfo, sizeof(m_psRouletteInfo));
    // m_stRouletteInfoData.nDayCount = psInfo.nDayCount;
    // m_stRouletteInfoData.biRegDate = psInfo.biRegDate;
    (void)psInfo;

    // 发送轮盘信息给客户端
    SendRouletteEventInfo();
}

// IDA: 0x14006D5B0 - SendRouletteEventInfo
// 发送轮盘事件信息给客户端
void CGocEvent::SendRouletteEventInfo() {
    // TODO: 获取CUser
    // CUser* pUser = GetOwner<CUser>();
    // if (!pUser) return;

    // XSendPacket xSendPacket(0x2A, 0x28);
    // xSendPacket << m_psRouletteInfo;
    // CGocNetwork::Send(&pUser->XActor, &xSendPacket);
}

// IDA: 0x14006D6D0 - IsRouletteEvent
// 执行轮盘事件，消耗货币/物品并随机获取奖励
int CGocEvent::IsRouletteEvent() {
    // TODO: 完整实现需要:
    // 1. 获取CUser
    // 2. 获取CTimeEventMgr获取轮盘事件配置
    // 3. 检查每日使用次数限制
    // 4. 获取CGocInventory
    // 5. 根据消耗类型(物品/金币/以太/BP/点券)扣除
    // 6. 随机计算奖励
    // 7. 获取CGocPost发送奖励邮件
    // 8. 更新每日计数并发送DB更新

    // 错误码定义:
    // 59600 - 无事件/通用错误
    // 59601 - 每日次数已达上限
    // 59602 - 金币不足
    // 59603 - 以太不足
    // 59604 - BP不足
    // 59605 - 点券不足
    // 55602 - 发送邮件失败

    return 59600; // 返回错误码 - 无事件
}

// IDA: 0x14006EA10 - InitRouletteDayCount
// 初始化轮盘每日计数
void CGocEvent::InitRouletteDayCount(std::int64_t biInitTime) {
    // TODO: 获取CUser
    // CUser* pUser = GetOwner<CUser>();
    // if (!pUser) return;

    // 获取轮盘事件配置
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // PS_GM_ROULETTE_EVENT psEventInfo;
    // if (!CTimeEventMgr::GetRouletteEventReward(&pServer->m_TimeEventMgr, &psEventInfo)) {
    //     LogHelper::LogError("game.item", "InitRouletteDayCount error - No event roulette");
    //     return;
    // }

    // 发送DB初始化请求
    // PS_DB_INIT_ROULETTE_INFO psDBInit;
    // psDBInit.nEventID = psEventInfo.nEventID;
    // psDBInit.dwUAID = pUser->GetUAID();
    // psDBInit.biInitDate = biInitTime;
    // if (psEventInfo.byUseType != 1) {
    //     psDBInit.dwUCID = pUser->GetUCID();
    // }

    // XSendDBPacket xSendDBPacket(pObject, 0x49, 0x2E);
    // xSendDBPacket << psDBInit;
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    // 重置本地计数
    m_stRouletteInfoData.nDayCount = 0;
    m_stRouletteInfoData.biRegDate = biInitTime;

    (void)biInitTime;
}

// ============================================================================
// 网吧任务函数
// ============================================================================

// IDA: 0x14006EC60 - SetStartNetCafeMission
// 启动或停止网吧任务
void CGocEvent::SetStartNetCafeMission(bool bStart) {
    // 设置网吧任务标志
    m_bNetCafeMission = bStart;

    // TODO: 获取CUser并发送DB请求加载任务列表
    // 需要检查 XResourceMgr::GetServerContents(E_SERVER_OPTION_NETCAFE)
    // 如果 m_dw64NetCafeUpdateTick 为0，发送 PS_NETCAFE_MISSION_LIST 到DB
    (void)bStart;
}

// IDA: 0x14006EEC0 - LoadNetCafeMission
// 加载网吧任务列表
void CGocEvent::LoadNetCafeMission(PS_NETCAFE_MISSION_LIST& psInfo) {
    // TODO: 获取CUser并验证UAID
    // 计算下一个重置时间 (m_nNetCafeNextDay)
    // 遍历任务列表，获取TB_PC_REWARD_SYSTEM验证
    // 获取任务时间范围，检查是否需要重置
    // 设置更新tick并发送任务信息给客户端
    SendNetCafeMissionInfo(0);
    (void)psInfo;
}

// IDA: 0x14006F480 - GetNetCafeMissionTime
// 获取网吧任务时间范围
bool CGocEvent::GetNetCafeMissionTime(unsigned int dwID, std::time_t& tCurr, std::time_t& tStart, std::time_t& tEnd) {
    // 根据任务ID确定时间范围:
    // dwID == 1: 每日任务 (从今天InitHour到明天InitHour)
    // dwID == 2: 每周任务 (从上周二InitHour到本周二InitHour+7天)
    // dwID == 3: 每月任务 (从本月1日InitHour到下月1日InitHour-1秒)
    (void)dwID;
    (void)tCurr;
    (void)tStart;
    (void)tEnd;
    return false;
}

// IDA: 0x14006FB40 - SendNetCafeMissionInfo
// 发送网吧任务信息给客户端
void CGocEvent::SendNetCafeMissionInfo(unsigned int dwID) {
    // TODO: 获取CUser，构造PS_NETCAFE_MISSION_LIST并发送
    (void)dwID;
}

// IDA: 0x14006FD90 - DBUpdateNetCafeMission
// 发送网吧任务更新到DB
void CGocEvent::DBUpdateNetCafeMission(unsigned int dwID, bool bInit) {
    // 查找任务
    auto it = m_mapNetCafeMission.find(dwID);
    if (it == m_mapNetCafeMission.end()) {
        return;
    }

    // TODO: 获取CUser，构造PS_NETCAFE_MISSION_UPDATE并发送到DB
    // 发送日志 ST_LOG_GAME (mainType=25, subType=bInit?51:52)
    (void)dwID;
    (void)bInit;
}

// IDA: 0x1400700E0 - Cheat_NetCafeMission_PlayTime
// 作弊函数：设置网吧任务游玩时间
void CGocEvent::Cheat_NetCafeMission_PlayTime(unsigned int dwID, bool bClear, int nValue) {
    // 查找任务
    auto it = m_mapNetCafeMission.find(dwID);
    if (it == m_mapNetCafeMission.end()) {
        return;
    }

    if (bClear) {
        // 清零
        it->second.dwValue = 0;
    } else {
        // 增加值
        it->second.dwValue += nValue;
        if (it->second.dwValue < 0) {
            it->second.dwValue = 0;
        }
    }

    // 发送更新到DB和客户端
    DBUpdateNetCafeMission(dwID, bClear);
    SendNetCafeMissionInfo(dwID);
}
