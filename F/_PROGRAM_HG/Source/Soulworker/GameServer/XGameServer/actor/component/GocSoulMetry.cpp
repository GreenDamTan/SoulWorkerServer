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
CGocSoulMetry::~CGocSoulMetry()
{
    // IDA 反编译显示：
    // - 设置虚表指针
    // - 析构 m_listCompleteSoulmetry
    // - 析构 m_mapSoulMetry
    // - 调用基类析构函数
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
// TODO: 需要完整实现，依赖 XSendDBPacket, XSendPacket, CQuestCondition 等
void CGocSoulMetry::ResetAll()
{
    // IDA 反编译显示:
    // 1. 清空 m_szCompleteSoulMetry
    // 2. 清空 m_mapSoulMetry
    // 3. 发送 DB 包 (main=0x46, sub=6)
    // 4. 发送客户端包 (main=0x21, sub=6)

    std::memset(m_szCompleteSoulMetry, 0, sizeof(m_szCompleteSoulMetry));
    m_mapSoulMetry.clear();

    // TODO: 需要实现完整的网络包发送逻辑
    // CMover* pMover = GetOwnerGO();
    // if (pMover) {
    //     XSendDBPacket xSendDBPacket(pMover, 0x46, 6);
    //     // ... 发送到 DB
    //     XSendPacket xSendPacket(0x21, 6);
    //     // ... 发送到客户端
    // }
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
void CGocSoulMetry::SendSoulMetryList()
{
    // TODO: 需要完整实现网络包发送
    // IDA 反编译显示:
    // 1. 构造 PS_SOULMETRY_LIST
    // 2. 遍历 m_mapSoulMetry 填充数据
    // 3. 发送包 (main=0x21, sub=1)

    PS_SOULMETRY_LIST stSoulmetryList;

    for (const auto& pair : m_mapSoulMetry) {
        PS_SOULMETRY_INFO info;
        info.dwSoulMetryID = pair.first;
        info.shValue = pair.second.shCompleteBit;
        stSoulmetryList.vecInfo.push_back(info);
    }

    // TODO: 发送网络包
    // CMover* pMover = GetOwnerGO();
    // if (pMover) {
    //     XSendPacket xSendPacket(0x21, 1);
    //     xSendPacket << stSoulmetryList;
    //     CGocNetwork::Send(pMover, &xSendPacket);
    // }
}

// IDA: ?SendSoulMetryAdd@CGocSoulMetry@@QEAAXXZ (0x1401963A0)
void CGocSoulMetry::SendSoulMetryAdd()
{
    if (!m_bAddSoulMetry) {
        return;
    }

    // TODO: 需要完整实现网络包发送
    // IDA 反编译显示:
    // 1. 发送包 (main=0x21, sub=3)
    // 2. 发送 m_stAddSoulMetryInfo
    // 3. 清除 m_bAddSoulMetry 标志

    // CMover* pMover = GetOwnerGO();
    // if (pMover) {
    //     XSendPacket xSendPacket(0x21, 3);
    //     xSendPacket << m_stAddSoulMetryInfo;
    //     CGocNetwork::Send(pMover, &xSendPacket);
    // }

    m_bAddSoulMetry = false;
}

// ============================================================================
// SoulMetry 操作
// ============================================================================

// IDA: ?UpdateSoulMetry@CGocSoulMetry@@QEAA_NHH@Z (0x1401969B0)
bool CGocSoulMetry::UpdateSoulMetry(int nSoulMetryID, int nObjectID)
{
    // 查找 SoulMetry
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter == m_mapSoulMetry.end()) {
        return false;
    }

    // 获取表数据
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    TB_SOUL_METRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(static_cast<std::uint16_t>(nSoulMetryID));
    if (!pTBSoulMetry) {
        LogHelper::LogDebug("game.quest", "<SOULMETRY> Cant find TB_SOUL_METRY ( %d )", nSoulMetryID);
        return false;
    }

    // 查找对应的 Object 和 MazeID
    int nOrder = 0;
    int nMazeID = 0;

    for (int i = 0; i < 5; ++i) {
        if (pTBSoulMetry->uniSoul_metry_object[i] == static_cast<unsigned int>(nObjectID)) {
            nOrder = i;
            nMazeID = pTBSoulMetry->uniSoul_metry_maze_object[i];

            // 检查是否已完成
            if ((iter->second.shCompleteBit & (1 << i)) != 0) {
                LogHelper::LogDebug("game.quest", "<SOULMETRY> Already Complete SoulMetry ( %d )", nObjectID);
                return false;
            }
            break;
        }
    }

    // 设置完成位
    iter->second.shCompleteBit |= static_cast<std::int16_t>(1 << nOrder);

    // 构造响应包
    PS_SOULMETRY_INFO psSoulMetry;
    psSoulMetry.dwSoulMetryID = nSoulMetryID;
    psSoulMetry.shValue = iter->second.shCompleteBit;

    // TODO: 发送 DB 包 (main=0x46, sub=3)
    // TODO: 发送客户端包 (main=0x21, sub=4)
    // TODO: 写入日志

    return true;
}

// IDA: ?AddSoulMetry@CGocSoulMetry@@QEAA_NH@Z (0x140196E90)
bool CGocSoulMetry::AddSoulMetry(int nSoulMetryID)
{
    // 检查是否已存在
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter != m_mapSoulMetry.end()) {
        return false;
    }

    // 获取表数据
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    TB_SOULMETRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(static_cast<std::uint16_t>(nSoulMetryID));
    if (!pTBSoulMetry) {
        LogHelper::LogDebug("game.quest", "<SOULMETRY> Cant find TB_SOUL_METRY ( %d )", nSoulMetryID);
        return false;
    }

    // 创建新的 SoulMetry 条目
    ST_SOULMETRY stSoulMetry;
    for (int i = 0; i < 5; ++i) {
        if (pTBSoulMetry->uniSoul_metry_object[i] != 0) {
            stSoulMetry.shCompleteBit &= static_cast<std::int16_t>(~(1 << i));
        }
    }

    m_mapSoulMetry[nSoulMetryID] = stSoulMetry;

    // 设置添加信息
    m_stAddSoulMetryInfo.dwSoulMetryID = nSoulMetryID;
    m_stAddSoulMetryInfo.shValue = stSoulMetry.shCompleteBit;

    // TODO: 发送 DB 包 (main=0x46, sub=3)
    // TODO: 写入日志

    m_bAddSoulMetry = true;
    SendSoulMetryAdd();

    return true;
}

// IDA: ?CompleteSoulMetry@CGocSoulMetry@@QEAAXH@Z (0x140197280)
void CGocSoulMetry::CompleteSoulMetry(int nSoulMetryID)
{
    // TODO: 需要完整实现
    // IDA 反编译显示复杂的逻辑:
    // 1. 检查 UserDB 同步标志
    // 2. 查找 SoulMetry
    // 3. 检查完成位是否等于 1023 (所有位完成)
    // 4. 检查是否已领取奖励
    // 5. 发放奖励物品
    // 6. 更新完成标志
    // 7. 删除 SoulMetry 条目
    // 8. 发送完成包 (main=0x21, sub=5)
    // 9. 写入日志

    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // 检查 DB 同步标志
    // if (!CUser::CheckDBSync(pMover)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x21, 5, 0xC35B);
    //     return;
    // }

    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter == m_mapSoulMetry.end()) {
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    TB_SOULMETRY* pTBSoulMetry = pGameServer->GetResourceMgr().GetTB_SOUL_METRY(static_cast<std::uint16_t>(nSoulMetryID));
    if (!pTBSoulMetry) {
        return;
    }

    // 检查是否全部完成 (1023 = 0x3FF = 所有 10 位都设置)
    if (iter->second.shCompleteBit != 1023) {
        return;
    }

    // 计算完成位索引
    int nIndex = pTBSoulMetry->Soul_metry_complet_bit / 8;
    int nPos = pTBSoulMetry->Soul_metry_complet_bit & 7;

    if (nIndex > 256) {
        return;
    }

    // 检查是否已领取
    if ((m_szCompleteSoulMetry[nIndex] & (1 << nPos)) != 0) {
        return;
    }

    // TODO: 发放奖励物品
    // ST_CREATE_ITEMS stCreateItems;
    // ... 填充奖励物品

    // 设置完成标志
    m_szCompleteSoulMetry[nIndex] |= static_cast<char>(1 << nPos);

    // TODO: 发送 DB 包更新完成标志 (main=0x46, sub=4)
    // TODO: 删除 SoulMetry 条目
    // DeleteSoulMetry(nSoulMetryID);

    // TODO: 发送完成包
    // TODO: 写入日志
}

// IDA: ?DeleteSoulMetry@CGocSoulMetry@@QEAAXH@Z (0x140197C50)
void CGocSoulMetry::DeleteSoulMetry(int nSoulMetryID)
{
    auto iter = m_mapSoulMetry.find(nSoulMetryID);
    if (iter != m_mapSoulMetry.end()) {
        m_mapSoulMetry.erase(iter);
    }
}
