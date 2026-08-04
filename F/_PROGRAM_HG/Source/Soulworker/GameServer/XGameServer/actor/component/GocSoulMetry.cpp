// CGocSoulMetry - SoulMetry系统组件实现
// 还原自 IDA GameServer.exe

#include "GocSoulMetry.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_REWARD.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocInventory.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"
#include <cstring>

// 外部日志辅助
extern const wchar_t* g_wszSoulMetryCompleteComment;
extern const wchar_t* g_wszSoulMetryAddComment;
extern const wchar_t* g_wszSoulMetryUpdateComment;

// ============================================================================
// 构造函数 / 析构函数
// ============================================================================

// IDA: ??0CGocSoulMetry@@QEAA@XZ (0x140195C40)
CGocSoulMetry::CGocSoulMetry()
    : GOComponent()
    , m_mapSoulMetry()
    , m_szCompleteSoulMetry{}
    , m_bLoad(false)
    , m_bAddSoulMetry(false)
    , m_stAddSoulMetryInfo{}
    , m_listCompleteSoulmetry()
    , m_bOpenAlwaysSoulMetry(false)
{
    // IDA 反编译显示：
    // - 调用 GOComponent 基类构造函数
    // - 设置虚表指针
    // - 构造 m_mapSoulMetry (std::map)
    // - 构造 m_listCompleteSoulmetry (std::list)
}

// IDA: ??1CGocSoulMetry@@UEAA@XZ (0x140195CE0)
// Verified: Direct IDA decompilation - destructor
CGocSoulMetry::~CGocSoulMetry()
{
    // IDA: 设置虚表指针
    // IDA: 析构 m_listCompleteSoulmetry
    // IDA: 析构 m_mapSoulMetry
    // IDA: 调用基类析构函数
    m_listCompleteSoulmetry.clear();
    m_mapSoulMetry.clear();
}

// ============================================================================
// 基础操作函数
// ============================================================================

// IDA: ?Init@CGocSoulMetry@@QEAA_NXZ (0x140195D40)
bool CGocSoulMetry::Init()
{
    Clear();
    return true;
}

// IDA: ?Clear@CGocSoulMetry@@QEAAXXZ (0x140195D60)
void CGocSoulMetry::Clear()
{
    // IDA 反编译:
    // memset(this->m_szCompleteSoulMetry, 0, sizeof(this->m_szCompleteSoulMetry));
    // memset(&this->m_stAddSoulMetryInfo, 0, sizeof(this->m_stAddSoulMetryInfo));
    // m_mapSoulMetry.clear();
    // this->m_bLoad = 0;
    // this->m_bAddSoulMetry = 0;
    // m_listCompleteSoulmetry.clear();
    // this->m_bOpenAlwaysSoulMetry = 0;

    std::memset(m_szCompleteSoulMetry, 0, sizeof(m_szCompleteSoulMetry));
    std::memset(&m_stAddSoulMetryInfo, 0, sizeof(m_stAddSoulMetryInfo));
    m_mapSoulMetry.clear();
    m_bLoad = false;
    m_bAddSoulMetry = false;
    m_listCompleteSoulmetry.clear();
    m_bOpenAlwaysSoulMetry = false;
}

// IDA: ?ResetAll@CGocSoulMetry@@QEAAXXZ (0x140195DF0)
// Verified: Direct IDA decompilation - resets all soulmetry data and sends DB/client packets
void CGocSoulMetry::ResetAll()
{
    // IDA: 清空 m_szCompleteSoulMetry
    std::memset(m_szCompleteSoulMetry, 0, sizeof(m_szCompleteSoulMetry));

    // IDA: 清空 m_mapSoulMetry
    m_mapSoulMetry.clear();

    // IDA: 发送 DB 包 (main=0x46, sub=6)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x46, 6);
            xSendDBPacket << pUser->GetQuestID();

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(&xSendDBPacket);
            }
        }
    }

    // IDA: 发送客户端包 (main=0x21, sub=6)
    if (pMover) {
        XSendPacket xSendPacket(0x21, 6);
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            xSendPacket << pUser->GetQuestID();
            pUser->Send(&xSendPacket);
        }
    }
}

// IDA: ?GetFamilyID@CGocSoulMetry@@SAHXZ (0x140144B40)
int CGocSoulMetry::GetFamilyID()
{
    // IDA 反编译: return 12;
    return 12;
}

// ============================================================================
// 数据设置与发送
// ============================================================================

// IDA: ?SetSoulMetryList@CGocSoulMetry@@QEAAXAEAUPS_SOULMETRY_LIST@@0@Z (0x140196000)
void CGocSoulMetry::SetSoulMetryList(PS_SOULMETRY_LIST& stSoulmetryList, PS_SOULMETRY_LIST& stCompleteSoulmetryList)
{
    m_bLoad = true;

    // 遍历 stSoulmetryList 并添加到 m_mapSoulMetry
    for (const auto& info : stSoulmetryList.vecInfo) {
        ST_SOULMETRY stSoulMetryInfo;
        stSoulMetryInfo.shCompleteBit = info.shValue;
        m_mapSoulMetry[info.dwSoulMetryID] = stSoulMetryInfo;
    }

    // IDA 显示对 stCompleteSoulmetryList 的二次处理
    // 但实际逻辑可能是处理完成列表
    for (const auto& info : stCompleteSoulmetryList.vecInfo) {
        ST_SOULMETRY stSoulMetryInfo;
        stSoulMetryInfo.shCompleteBit = info.shValue;
        m_mapSoulMetry[info.dwSoulMetryID] = stSoulMetryInfo;
    }

    // 添加完成列表到 m_listCompleteSoulmetry
    for (const auto& info : stCompleteSoulmetryList.vecInfo) {
        m_listCompleteSoulmetry.push_back(info.dwSoulMetryID);
    }
}

// IDA: ?SendSoulMetryList@CGocSoulMetry@@QEAAXXZ (0x1401961F0)
// Verified: Direct IDA decompilation - sends soulmetry list to client
void CGocSoulMetry::SendSoulMetryList()
{
    // IDA: 构造 PS_SOULMETRY_LIST
    PS_SOULMETRY_LIST stSoulmetryList;

    // IDA: 遍历 m_mapSoulMetry 填充数据
    for (const auto& pair : m_mapSoulMetry) {
        PS_SOULMETRY_INFO info;
        info.dwSoulMetryID = pair.first;
        info.shValue = pair.second.shCompleteBit;
        stSoulmetryList.vecInfo.push_back(info);
    }

    // IDA: 发送包 (main=0x21, sub=1)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(0x21, 1);
        xSendPacket << stSoulmetryList;
        CGocNetwork::Send(pMover, &xSendPacket);
    }
}

// IDA: ?IsCompleteSoulMetry@CGocSoulMetry@@QEAA_NK@Z (0x140198080)
// Verified: Direct IDA decompilation - checks if a soulmetry is complete using bit flags
bool CGocSoulMetry::IsCompleteSoulMetry(unsigned long dwSoulMetryID) const
{
    // IDA: 获取 TB_SOUL_METRY 表
    XGameServer* pServer = XGameServer::Instance();
    TB_SOUL_METRY* pTB_SoulMetry = pServer->GetResourceMgr().GetTB_SOUL_METRY(dwSoulMetryID);

    if (!pTB_SoulMetry) {
        return false;
    }

    // IDA: 计算位索引
    int nIndex = pTB_SoulMetry->Soul_metry_complet_bit / 8;

    // IDA: 检查位是否在范围内
    if (nIndex > 256) {
        return false;
    }

    // IDA: 检查对应位是否设置
    int nBit = pTB_SoulMetry->Soul_metry_complet_bit & 7;
    return (m_szCompleteSoulMetry[nIndex] & (1 << nBit)) != 0;
}

// IDA: ?SendSoulMetryAdd@CGocSoulMetry@@QEAAXXZ (0x1401963A0)
// Verified: Direct IDA decompilation - sends soulmetry add notification to client
void CGocSoulMetry::SendSoulMetryAdd()
{
    // IDA: 如果没有添加标志，直接返回
    if (!m_bAddSoulMetry) {
        return;
    }

    // IDA: 发送包 (main=0x21, sub=3)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(0x21, 3);
        xSendPacket << m_stAddSoulMetryInfo;
        CGocNetwork::Send(pMover, &xSendPacket);
    }

    // IDA: 清除添加标志
    m_bAddSoulMetry = false;
}

// IDA: ?SendSoulMetryCompleteList@CGocSoulMetry@@QEAAXXZ (0x140196770)
// Verified: Direct IDA decompilation - sends complete soulmetry list to client
void CGocSoulMetry::SendSoulMetryCompleteList()
{
    // IDA: 发送包 (main=0x21, sub=2)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(0x21, 2);
        xSendPacket.SetBytes(m_szCompleteSoulMetry, 256);
        CGocNetwork::Send(pMover, &xSendPacket);
    }
}

// IDA: ?SendReqSoulMetryList@CGocSoulMetry@@QEAAXXZ (0x140197CD0)
// Verified: Direct IDA decompilation - requests soulmetry list from DB
void CGocSoulMetry::SendReqSoulMetryList()
{
    // IDA: 发送 DB 包 (main=0x46, sub=1)
    CMover* pMover = GetOwnerGO();
    if (!pMover) return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return;

    XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x46, 1);
    xSendDBPacket << pUser->GetQuestID();

    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SendDBGame(&xSendDBPacket);
    }
}

// IDA: ?SetSoulMetryCompleteList@CGocSoulMetry@@QEAAXAEAUPS_SOULMETRY_COMPLETE@@@Z (0x1401964a0)
// Verified: Direct IDA decompilation - sets complete list and updates DB
void CGocSoulMetry::SetSoulMetryCompleteList(PS_SOULMETRY_COMPLETE& stCompleteInfo)
{
    // IDA: 复制完成数据
    std::memcpy(m_szCompleteSoulMetry, stCompleteInfo.szCompleteData, 256);

    bool bChangeComplete = false;

    // IDA: 遍历 m_listCompleteSoulmetry
    for (auto it = m_listCompleteSoulmetry.begin(); it != m_listCompleteSoulmetry.end(); ++it) {
        unsigned int dwSoulmetry = *it;

        // IDA: 检查是否已完成
        if (!IsCompleteSoulMetry(dwSoulmetry)) {
            // IDA: 获取表数据
            XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
            TB_SOUL_METRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(dwSoulmetry);

            if (pTBSoulMetry) {
                int nIndex = pTBSoulMetry->Soul_metry_complet_bit / 8;
                int nPos = pTBSoulMetry->Soul_metry_complet_bit & 7;

                if (nIndex <= 256) {
                    m_szCompleteSoulMetry[nIndex] |= static_cast<char>(1 << nPos);
                    bChangeComplete = true;
                }
            }
        }
    }

    // IDA: 清空完成列表
    m_listCompleteSoulmetry.clear();

    // IDA: 如果有变更，发送 DB 包
    if (bChangeComplete) {
        CMover* pMover = GetOwnerGO();
        if (pMover) {
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser) {
                XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x46, 4);
                xSendDBPacket << pUser->GetQuestID();
                xSendDBPacket << 0;  // nSoulmetry
                xSendDBPacket.SetBytes(m_szCompleteSoulMetry, 256);

                XGameServer* pServer = XGameServer::Instance();
                if (pServer) {
                    pServer->SendDBGame(&xSendDBPacket);
                }
            }
        }
    }
}

// IDA: ?FindNewSoulMetry@CGocSoulMetry@@QEAAXXZ (0x140196850)
// Verified: Direct IDA decompilation - finds and adds new soulmetry from maze clear info
void CGocSoulMetry::FindNewSoulMetry()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) return;

    // IDA: 获取 CGocRecode 组件
    std::shared_ptr<CGocRecode> pRecode;
    pMover->GetGOC<CGocRecode>(pRecode);
    if (!pRecode) return;

    // IDA: 获取迷宫清理信息
    std::map<int, ST_MAZE_CLEAR_INFO>* mapClearInfo = pRecode->GetMazeClearInfo();
    if (!mapClearInfo || mapClearInfo->empty()) return;

    // IDA: 遍历清理信息
    for (auto it = mapClearInfo->begin(); it != mapClearInfo->end(); ++it) {
        int nGroup = it->first;

        // IDA: 检查是否已完成
        if (!IsCompleteSoulMetry(nGroup)) {
            // IDA: 检查是否已在 map 中
            auto findIt = m_mapSoulMetry.find(nGroup);
            if (findIt == m_mapSoulMetry.end()) {
                // IDA: 添加新的 SoulMetry
                AddSoulMetry(nGroup);
            }
        }
    }
}

// ============================================================================
// SoulMetry 操作
// ============================================================================

// IDA: ?UpdateSoulMetry@CGocSoulMetry@@QEAA_NHH@Z (0x1401969B0)
// Verified: Direct IDA decompilation - updates soulmetry progress
bool CGocSoulMetry::UpdateSoulMetry(int nSoulMetryID, int nObjectID)
{
    // IDA: 查找 SoulMetry
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter == m_mapSoulMetry.end()) {
        return false;
    }

    // IDA: 获取表数据
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    TB_SOUL_METRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(static_cast<std::uint16_t>(nSoulMetryID));
    if (!pTBSoulMetry) {
        LogHelper::LogDebug("game.quest", "<SOULMETRY> Cant find TB_SOUL_METRY ( %d )", nSoulMetryID);
        return false;
    }

    // IDA: 查找对应的 Object 和 MazeID
    int nOrder = 0;
    int nMazeID = 0;

    for (int i = 0; i < 5; ++i) {
        if (pTBSoulMetry->uniSoul_metry_object[i] == static_cast<unsigned int>(nObjectID)) {
            nOrder = i;
            nMazeID = pTBSoulMetry->uniSoul_metry_maze_object[i];

            // IDA: 检查是否已完成
            if ((iter->second.shCompleteBit & (1 << i)) != 0) {
                LogHelper::LogDebug("game.quest", "<SOULMETRY> Already Complete SoulMetry ( %d )", nObjectID);
                return false;
            }
            break;
        }
    }

    // IDA: 设置完成位
    iter->second.shCompleteBit |= static_cast<std::int16_t>(1 << nOrder);

    // IDA: 构造响应包
    PS_SOULMETRY_INFO psSoulMetry;
    psSoulMetry.dwSoulMetryID = nSoulMetryID;
    psSoulMetry.shValue = iter->second.shCompleteBit;

    // IDA: 发送 DB 包 (main=0x46, sub=3)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x46, 3);
            xSendDBPacket << pUser->GetQuestID();
            xSendDBPacket << psSoulMetry;
            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(&xSendDBPacket);
            }
        }
    }

    // IDA: 发送客户端包 (main=0x21, sub=4)
    if (pMover) {
        XSendPacket xSendPacket(0x21, 4);
        xSendPacket << psSoulMetry;
        CGocNetwork::Send(pMover, &xSendPacket);
    }

    // IDA: 写入日志
    if (pMover) {
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            ST_LOG_GAME stLog;
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetQuestID();
            stLog._sMainType = 17;
            stLog._sSubType = 2;
            stLog.nParam0 = nSoulMetryID;
            stLog.nParam1 = nObjectID;
            stLog.nParam2 = nMazeID;
            wcscpy_s(stLog.szComment, g_wszSoulMetryUpdateComment);

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBLog(&stLog);
            }
        }
    }

    return true;
}

// IDA: ?AddSoulMetry@CGocSoulMetry@@QEAA_NH@Z (0x140196E90)
// Verified: Direct IDA decompilation - adds new soulmetry entry
bool CGocSoulMetry::AddSoulMetry(int nSoulMetryID)
{
    // IDA: 检查是否已存在
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter != m_mapSoulMetry.end()) {
        return false;
    }

    // IDA: 获取表数据
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    TB_SOUL_METRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(static_cast<std::uint16_t>(nSoulMetryID));
    if (!pTBSoulMetry) {
        LogHelper::LogDebug("game.quest", "<SOULMETRY> Cant find TB_SOUL_METRY ( %d )", nSoulMetryID);
        return false;
    }

    // IDA: 创建新的 SoulMetry 条目
    ST_SOULMETRY stSoulMetry;
    for (int i = 0; i < 5; ++i) {
        if (pTBSoulMetry->uniSoul_metry_object[i] != 0) {
            stSoulMetry.shCompleteBit &= static_cast<std::int16_t>(~(1 << i));
        }
    }

    m_mapSoulMetry[nSoulMetryID] = stSoulMetry;

    // IDA: 设置添加信息
    m_stAddSoulMetryInfo.dwSoulMetryID = nSoulMetryID;
    m_stAddSoulMetryInfo.shValue = stSoulMetry.shCompleteBit;

    // IDA: 发送 DB 包 (main=0x46, sub=3)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x46, 3);
            xSendDBPacket << pUser->GetQuestID();
            xSendDBPacket << m_stAddSoulMetryInfo;
            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBGame(&xSendDBPacket);
            }
        }
    }

    m_bAddSoulMetry = true;
    SendSoulMetryAdd();

    // IDA: 写入日志
    if (pMover) {
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            ST_LOG_GAME stLog;
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetQuestID();
            stLog._sMainType = 17;
            stLog._sSubType = 1;
            stLog.nParam0 = nSoulMetryID;
            wcscpy_s(stLog.szComment, g_wszSoulMetryAddComment);

            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                pServer->SendDBLog(&stLog);
            }
        }
    }

    return true;
}

// IDA: ?CompleteSoulMetry@CGocSoulMetry@@QEAAXH@Z (0x140197280)
// Verified: Direct IDA decompilation - completes soulmetry and gives rewards
void CGocSoulMetry::CompleteSoulMetry(int nSoulMetryID)
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) {
        return;
    }

    // IDA: 检查 DB 同步标志
    if ((pUser->GetUserDB() & 8) == 0) {
        CGocNetwork::SendErrorMessage(pMover, 0x21, 5, 0xC35B);
        LogHelper::LogError("game.quest",
            "<SOULMETRY> Failed CompleteSoulMetry - Not Sync DB ( %d / %d )",
            nSoulMetryID, pUser->GetQuestID());
        return;
    }

    // IDA: 查找 SoulMetry
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter == m_mapSoulMetry.end()) {
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    TB_SOUL_METRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(static_cast<std::uint16_t>(nSoulMetryID));
    if (!pTBSoulMetry) {
        LogHelper::LogError("game.quest",
            "<SOULMETRY> Failed CompleteSoulMetry - Cant find TB_SOUL_METRY ( %d / %d )",
            nSoulMetryID, pUser->GetQuestID());
        return;
    }

    // IDA: 检查是否全部完成 (1023 = 0x3FF = 所有 10 位都设置)
    if (iter->second.shCompleteBit != 1023) {
        LogHelper::LogError("game.quest",
            "<SOULMETRY> Failed CompleteSoulMetry - Not Complete ( %d / %d )",
            nSoulMetryID, pUser->GetQuestID());
        return;
    }

    // IDA: 计算完成位索引
    int nIndex = pTBSoulMetry->Soul_metry_complet_bit / 8;
    int nPos = pTBSoulMetry->Soul_metry_complet_bit & 7;

    if (nIndex > 256) {
        LogHelper::LogError("game.quest",
            "<SOULMETRY> Failed CompleteSoulMetry - MAX_QUEST_COMPLETE_SIZE Over ( %d / %d / %d )",
            nSoulMetryID, pUser->GetQuestID(), nIndex);
        return;
    }

    // IDA: 检查是否已领取
    if ((m_szCompleteSoulMetry[nIndex] & (1 << nPos)) != 0) {
        LogHelper::LogError("game.quest",
            "<SOULMETRY> Failed CompleteSoulMetry - Alread Complete ( %d / %d )",
            nSoulMetryID, pUser->GetQuestID());
        return;
    }

    // IDA: 构造奖励物品列表
    ST_CREATE_ITEMS stCreateItems;
    for (int i = 0; i < 2; ++i) {
        int nItemID = pTBSoulMetry->uniSoul_metry_reward_item[i];
        if (nItemID > 0) {
            int nItemType = pTBSoulMetry->uniSoul_metry_reward_item_type[i];
            if (nItemType == 1) {
                // IDA: 直接物品奖励
                TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM(nItemID);
                if (pTBItem) {
                    ST_CREATE_ITEM stItem;
                    stItem.nItemID = nItemID;
                    stItem.shCount = pTBSoulMetry->uniSoul_metry_reward_item_count[i];
                    stCreateItems.vecInfo.push_back(stItem);
                }
            } else if (nItemType == 2) {
                // IDA: 任务奖励表
                TB_QUEST_REWARD* pTBReward = pGameServer->GetResourceMgr().GetTB_QUEST_REWARD(nItemID);
                if (pTBReward) {
                    int nClass = pUser->GetClass();
                    unsigned int dwIndex = pTBReward->GetRewardIndex(nClass);
                    TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM(dwIndex);
                    if (pTBItem) {
                        ST_CREATE_ITEM stItem;
                        stItem.nItemID = pTBItem->Item_ID;
                        stItem.shCount = pTBSoulMetry->uniSoul_metry_reward_item_count[i];
                        stCreateItems.vecInfo.push_back(stItem);
                    }
                }
            }
        }
    }

    // IDA: 获取背包组件并发放奖励
    std::shared_ptr<CGocInventory> pInven;
    pMover->GetGOC<CGocInventory>(pInven);
    if (!pInven) {
        LogHelper::LogError("game.quest", "<SOULMETRY> Failed Create Reward Item ( %d )", nSoulMetryID);
        return;
    }

    ST_LOG_GAME stLogItem;
    stLogItem.nParam3 = nSoulMetryID;

    if (!pInven->CreateItemReq(
            stCreateItems,
            false,
            E_ITEM_CREATE_TYPE_SOUL_METRY,
            stLogItem)) {
        LogHelper::LogError("game.quest", "<SOULMETRY> Failed Create Reward Item ( %d )", nSoulMetryID);
        return;
    }

    // IDA: 设置完成标志
    m_szCompleteSoulMetry[nIndex] |= static_cast<char>(1 << nPos);

    // IDA: 发送 DB 包更新完成标志 (main=0x46, sub=4)
    {
        XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x46, 4);
        xSendDBPacket << pUser->GetQuestID();
        xSendDBPacket << nSoulMetryID;
        xSendDBPacket.SetBytes(m_szCompleteSoulMetry, 256);
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->SendDBGame(&xSendDBPacket);
        }
    }

    // IDA: 删除 SoulMetry 条目
    DeleteSoulMetry(nSoulMetryID);

    // IDA: 发送完成包 (main=0x21, sub=5)
    {
        XSendPacket xSendPacket(0x21, 5);
        xSendPacket << nSoulMetryID;
        xSendPacket << stCreateItems;
        CGocNetwork::Send(pMover, &xSendPacket);
    }

    // IDA: 写入日志
    {
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetQuestID();
        stLog._sMainType = 17;
        stLog._sSubType = 3;
        stLog.nParam0 = nSoulMetryID;
        wcscpy_s(stLog.szComment, g_wszSoulMetryCompleteComment);

        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->SendDBLog(&stLog);
        }
    }
}

// IDA: ?DeleteSoulMetry@CGocSoulMetry@@QEAAXH@Z (0x140197C50)
// Verified: Direct IDA decompilation - deletes soulmetry entry from map
void CGocSoulMetry::DeleteSoulMetry(int nSoulMetryID)
{
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter != m_mapSoulMetry.end()) {
        m_mapSoulMetry.erase(iter);
    }
}
