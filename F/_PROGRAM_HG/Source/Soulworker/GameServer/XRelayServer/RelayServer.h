#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/ForceMatching.h"
#include "Soulworker/GameServer/XRelayServer/GameDBSocket.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/PartyManager.h"
#include "Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/RelayControlSocket.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/ObserveSocket.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"
#include "Soulworker/GameServer/XRelayServer/UserPartyInfo.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

class CServer;
struct PS_USERS_INFO;

// 对齐 IDA: XSeed::Init 存根 - 随机种子初始化
inline void GreenDamTan_XSeedInit(int nSeed) {
    static_cast<void>(nSeed);
    // 简化实现: 实际应初始化 m_xSeed 成员用于随机数生成
    // 完整实现: XSeed::Init(&m_xSeed, nSeed)
}

struct PS_KICK_USER_INFO_UCID {
    std::uint8_t byKickType = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nParam = 0;
    wchar_t strMsg[513] = {};
};

inline XPacket& operator<<(XPacket& packet, const PS_KICK_USER_INFO_UCID& value) {
    packet.XParse << value.byKickType;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nParam;
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    return packet;
}

class CFriendRecommandManager {
public:
    CFriendRecommandManager() = default;
    ~CFriendRecommandManager() = default;

    // 对齐 IDA 0x140042440: 添加用户到推荐索引
    // eSystemType 由调用者从 XRelayServer::GetOption() 获取传入
    void AddUser(const std::shared_ptr<CUserObject>& pUser, SYSTEM_TYPE eSystemType = SYSTEM_TYPE_REAL) {
        if (!pUser) return;

        const std::uint8_t byLevel = pUser->GetLevel();

        // 对齐 IDA: REAL 服且 GM 权限 > 0，或等级 > 68 不加入推荐
        if ((eSystemType == SYSTEM_TYPE_REAL && pUser->GetGMPower() > 0) || (byLevel > 68)) {
            return;
        }

        const int nLevel = static_cast<int>(byLevel);
        ++m_nIndex[nLevel];
        const int nIndex = m_nIndex[nLevel];
        const std::uint32_t dwKey = static_cast<std::uint32_t>(nIndex + nLevel * 10000);

        pUser->SetRecommandIndex(static_cast<int>(dwKey));

        // 存储 key -> user
        m_mapUserInfos[dwKey] = pUser;

        // 存储 matchingID -> key 映射
        const std::uint32_t dwMatchingID = pUser->GetMatchingID();
        m_mapUserCheck[dwMatchingID] = dwKey;
    }

    // 对齐 IDA 0x140042600: 从推荐索引删除用户
    void DeleteUser(const std::shared_ptr<CUserObject>& pUser) {
        if (!pUser) return;

        const std::uint32_t dwMatchingID = pUser->GetMatchingID();
        auto checkIt = m_mapUserCheck.find(dwMatchingID);
        if (checkIt == m_mapUserCheck.end()) {
            return;
        }

        const std::uint32_t dwKey = checkIt->second;

        // 从主索引删除
        auto it = m_mapUserInfos.find(dwKey);
        if (it != m_mapUserInfos.end()) {
            m_mapUserInfos.erase(it);
        }

        DeleteUserCheck(dwMatchingID);
    }

    // 对齐 IDA 0x1400427C0: 删除用户检查映射
    void DeleteUserCheck(std::uint32_t dwUCID) {
        auto it = m_mapUserCheck.find(dwUCID);
        if (it != m_mapUserCheck.end()) {
            m_mapUserCheck.erase(it);
        }
    }

    // 对齐 IDA 0x140042850: 更新用户等级（重新索引）
    void UpdateLevel(const std::shared_ptr<CUserObject>& pUser, SYSTEM_TYPE eSystemType = SYSTEM_TYPE_REAL) {
        DeleteUser(pUser);
        AddUser(pUser, eSystemType);
    }

    // 对齐 IDA 0x1400428E0: 获取好友推荐列表
    int GetFriendRecommandList(const std::shared_ptr<CUserObject>& pUser,
                                int nMaxCount,
                                std::vector<ST_RECOMMAND_FRIEND_INFO>& vecFriendInfo) {
        if (!pUser) return 0;

        const std::uint8_t byLevel = pUser->GetLevel();
        const int nMinLevel = std::max(1, static_cast<int>(byLevel) - 3);
        const int nMaxLevel = std::min(68, static_cast<int>(byLevel) + 3);

        const std::uint32_t dwMinKey = static_cast<std::uint32_t>(nMinLevel * 10000);
        const std::uint32_t dwMaxKey = static_cast<std::uint32_t>(nMaxLevel * 10000 + 9999);

        auto kb = m_mapUserInfos.lower_bound(dwMinKey);
        auto ke = m_mapUserInfos.lower_bound(dwMaxKey + 1);

        if (kb == m_mapUserInfos.end()) {
            return 0;
        }

        int nCount = 0;
        const std::uint32_t dwMyMatchingID = pUser->GetMatchingID();

        while (kb != ke) {
            const auto& pObject = kb->second;
            if (pObject) {
                const std::uint32_t dwTargetMatchingID = pObject->GetMatchingID();
                if (dwTargetMatchingID != dwMyMatchingID) {
                    // 排除已是好友
                    if (!pUser->IsFriendList(dwTargetMatchingID, 1u)) {
                        bool bFind = false;
                        for (const auto& info : vecFriendInfo) {
                            if (info.dwID == dwTargetMatchingID) {
                                bFind = true;
                                break;
                            }
                        }

                        if (!bFind) {
                            ST_RECOMMAND_FRIEND_INFO stInfo{};
                            pObject->GetRecommandInfo(stInfo);
                            vecFriendInfo.push_back(stInfo);
                            ++nCount;

                            if (nCount >= nMaxCount) {
                                break;
                            }
                        }
                    }
                }
            }
            ++kb;
        }

        return nCount;
    }

private:
    std::map<std::uint32_t, std::shared_ptr<CUserObject>> m_mapUserInfos;  // key -> user
    std::map<std::uint32_t, std::uint32_t> m_mapUserCheck;  // matchingID -> key
    int m_nIndex[70] = {};  // 每级索引计数器（等级 0-69）
};

class CFriendRecruitManager {
public:
    CFriendRecruitManager() = default;
    ~CFriendRecruitManager() = default;

    // 对齐 IDA 0x140044EC0: 加载招募列表
    void LoadRecruitList(const std::vector<ST_RECRUIT_INFO>* pvecRecruitInfo, std::uint8_t byLast) {
        if (!pvecRecruitInfo) return;

        CFAutoSlimWriteLock autolock(&m_rwLock);
        if (m_bDBLoad) return;

        for (const auto& info : *pvecRecruitInfo) {
            auto it = m_mapRecruit.find(info.dwID);
            if (it == m_mapRecruit.end()) {
                auto pRecruit = std::make_shared<CRecruitUser>();
                pRecruit->SetInfo(info);
                m_mapRecruit[info.dwID] = pRecruit;
            }
        }

        LogHelper::LogInfo("game.contents", "<FRIEND> LOAD FRIEND RECRUIT INFO ALL ( COUNT : %d )",
                           static_cast<int>(m_mapRecruit.size()));

        if (byLast == 1) {
            m_bDBLoad = true;
            // 对齐 IDA: SetCachingLoad 由 XRelayServer 调用
        }
    }

    // 对齐 IDA 0x140045110: 周期更新，清理过期招募
    void OnUpdate();

    // 对齐 IDA 0x140045330: 添加招募
    bool AddRecruit(const ST_RECRUIT_INFO& stAdd) {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        if (m_mapRecruit.find(stAdd.dwID) != m_mapRecruit.end()) {
            return false;
        }

        auto pRecruit = std::make_shared<CRecruitUser>();
        pRecruit->SetInfo(stAdd);
        m_mapRecruit[stAdd.dwID] = pRecruit;
        return true;
    }

    // 对齐 IDA 0x1400454C0: 删除招募
    bool DeleteRecruit(std::uint32_t dwUCID) {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        auto it = m_mapRecruit.find(dwUCID);
        if (it == m_mapRecruit.end()) {
            return false;
        }

        m_mapRecruit.erase(it);
        return true;
    }

    // 对齐 IDA 0x140045580: 更新招募状态
    void UpdateRecruit(std::uint32_t dwUCID, bool bLogin) {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        auto it = m_mapRecruit.find(dwUCID);
        if (it != m_mapRecruit.end()) {
            it->second->SetLogin(bLogin);
        }
    }

    // 对齐 IDA 0x140045660: 检查是否在招募列表
    bool IsRecruitList(std::uint32_t dwUCID) const {
        CFAutoSlimReadLock autolock(&m_rwLock);
        return m_mapRecruit.find(dwUCID) != m_mapRecruit.end();
    }

    // 对齐 IDA 0x140045700: 获取招募列表
    void GetFriendRecruitList(PS_REQ_RECRUIT_LIST& stInfo,
                               std::vector<ST_RECRUIT_INFO>& vecRecruitInfo,
                               const std::shared_ptr<CUserObject>& pUser);

    // 对齐 IDA 0x1400460C0: 获取招募添加时间
    std::int64_t GetRecruitAddTime(std::uint32_t dwUCID) const {
        CFAutoSlimReadLock autolock(&m_rwLock);
        auto it = m_mapRecruit.find(dwUCID);
        if (it != m_mapRecruit.end()) {
            return it->second->GetAddTime();
        }
        return 0;
    }

    // 对齐 IDA 0x140064130: 检查是否已加载
    bool IsLoad() const { return m_bDBLoad; }

private:
    // 简化的招募用户类（原版使用 boost::multi_index_container）
    class CRecruitUser {
    public:
        std::uint32_t GetUCID() const { return m_stInfo.dwID; }
        std::uint8_t GetLevel() const { return m_stInfo.byLevel; }
        std::uint8_t GetClass() const { return m_stInfo.byClass; }
        bool GetLogin() const { return m_bLogin; }
        void SetLogin(bool bLogin) { m_bLogin = bLogin; }
        std::int64_t GetAddTime() const { return m_stInfo.tAddTime; }

        void SetInfo(const ST_RECRUIT_INFO& info) {
            m_stInfo = info;
            m_bLogin = false;
        }

        void GetInfo(ST_RECRUIT_INFO& info) const {
            info = m_stInfo;
        }

    private:
        ST_RECRUIT_INFO m_stInfo{};
        bool m_bLogin = false;
    };

    std::map<std::uint32_t, std::shared_ptr<CRecruitUser>> m_mapRecruit;
    mutable CFSRWLock m_rwLock{};
    std::int64_t m_tUpdate = 0;
    bool m_bDBLoad = false;
};

// 对齐 IDA: 交易所价格管理器
class CExchangePriceMgr {
public:
    // 对齐 IDA: 加载价格列表到缓存
    void LoadPriceList(const PS_DB_EXCHANGE_PRICE_HISTORY_RES* psList) {
        if (!psList) return;
        m_mapPriceHistory[psList->stRes.dwItemID] = psList->stRes;
    }

    // 对齐 IDA: 获取价格列表，返回 true 时缓存命中
    bool GetPriceList(std::uint32_t dwItemID, PS_EXCHANGE_PRICE_HISTORY_RES* psResult) {
        auto it = m_mapPriceHistory.find(dwItemID);
        if (it != m_mapPriceHistory.end()) {
            if (psResult) {
                *psResult = it->second;
            }
            return true;
        }
        return false;
    }

    // 对齐 IDA 0x14000D610: 获取 DB 请求日期
    std::int64_t GetDBRequestDate() {
        // 对齐 IDA: 如果日期 <= CTime(2000, 1, 1)，则设置为今天零点
        if (m_n64DBRequestDate <= 946684800) {  // 2000-01-01 00:00:00 UTC
            std::time_t now = std::time(nullptr);
            std::tm* tmNow = std::localtime(&now);
            tmNow->tm_hour = 0;
            tmNow->tm_min = 0;
            tmNow->tm_sec = 0;
            m_n64DBRequestDate = static_cast<std::int64_t>(std::mktime(tmNow));
        }
        return m_n64DBRequestDate;
    }

private:
    std::map<std::uint32_t, PS_EXCHANGE_PRICE_HISTORY_RES> m_mapPriceHistory;  // itemID -> 价格历史
    std::int64_t m_n64DBRequestDate = 0;  // 对齐 IDA: DB 请求日期（分区用）
};

// 对齐 IDA: 助战支持对象
class CHelperSupport {
public:
    CHelperSupport() = default;
    ~CHelperSupport() = default;

    // 对齐 IDA 0x14002E8F0: 初始化助战信息
    void Init(const ST_HELPER_SUPPORT_INFO& stInfo) {
        m_stInfo = stInfo;
        m_byRewardState = 1;  // 可领取状态
        m_setRecvList.clear();
    }

    // 对齐 IDA 0x14002EA40: 检查是否在有效期内
    bool CheckVaildTime() const {
        return m_stInfo.nDate >= static_cast<std::int64_t>(std::time(nullptr));
    }

    // 对齐 IDA 0x140032900: 获取奖励状态
    std::uint8_t GetRewardState() const { return m_byRewardState; }

    // 对齐 IDA 0x14002EDB0: 设置奖励状态
    void SetMatchingState(std::uint8_t byState) { m_byRewardState = byState; }

    // 对齐 IDA 0x1400329F0: 获取助战信息
    void GetSupportInfo(ST_HELPER_SUPPORT_INFO& stInfo) const { stInfo = m_stInfo; }

    // 对齐 IDA 0x14002ED90: 获取已领取人数
    int GetRecvCount() const { return static_cast<int>(m_setRecvList.size()); }

    // 对齐 IDA 0x14002E950: 检查是否已领取
    bool CheckReceived(std::uint32_t dwUCID) const {
        if (GetRecvCount() <= 0) return false;
        return m_setRecvList.find(dwUCID) != m_setRecvList.end();
    }

    // 对齐 IDA 0x14002E9C0: 添加已领取记录
    bool AddReceived(std::uint32_t dwUCID) {
        return m_setRecvList.insert(dwUCID).second;
    }

private:
    ST_HELPER_SUPPORT_INFO m_stInfo{};
    std::uint8_t m_byRewardState = 0;  // 0=无, 1=可领取, 2=已领取
    std::set<std::uint32_t> m_setRecvList;  // 已领取助战的用户列表
};

// 对齐 IDA: 助战支持管理器
class CHelperSupportMgr {
public:
    CHelperSupportMgr() = default;
    ~CHelperSupportMgr() = default;

    // 对齐 IDA 0x14002EF70: 查找助战
    std::shared_ptr<CHelperSupport> FindSupport(std::uint32_t dwUCID) const {
        CFAutoSlimReadLock autolock(&m_rwLock);
        auto it = m_mapSupport.find(dwUCID);
        if (it != m_mapSupport.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 对齐 IDA 0x14002F050: 添加助战
    bool AddSupport(PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport) {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        auto it = m_mapSupport.find(psSupport.stInfo.dwFriendUCID);
        if (it != m_mapSupport.end()) {
            // 已存在，检查是否有效
            if (it->second->CheckVaildTime()) {
                return false;  // 有效期内，不能重复注册
            }
            // 已过期，更新信息
            it->second->Init(psSupport.stInfo);
        } else {
            // 不存在，创建新的
            auto pSupport = std::make_shared<CHelperSupport>();
            pSupport->Init(psSupport.stInfo);
            m_mapSupport[psSupport.stInfo.dwFriendUCID] = pSupport;
        }
        return true;
    }

    // 对齐 IDA 0x14002F330: 删除助战
    bool DeleteSupport(std::uint32_t dwUCID) {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        auto it = m_mapSupport.find(dwUCID);
        if (it != m_mapSupport.end()) {
            m_mapSupport.erase(it);
            return true;
        }
        return false;
    }

    // 对齐 IDA 0x14002F860: 获取助战奖励状态
    std::uint8_t GetSupportReward(std::uint32_t dwUCID) const {
        auto pSupport = FindSupport(dwUCID);
        if (!pSupport) {
            return 0;
        }
        return pSupport->GetRewardState();
    }

    // 对齐 IDA 0x14002F8E0: 添加助战已领取记录
    bool AddSupportReceived(std::uint32_t dwFriendUCID, std::uint32_t dwUCID) {
        auto pSupport = FindSupport(dwFriendUCID);
        if (!pSupport) {
            return false;
        }
        return pSupport->AddReceived(dwUCID);
    }

private:
    std::map<std::uint32_t, std::shared_ptr<CHelperSupport>> m_mapSupport;
    mutable CFSRWLock m_rwLock{};
};

class CRelayPartyMatchingConfig {
public:
    bool Init(const char* commonDNS);
    int GetMatchingWaitMs() const { return m_nMatchingWaitMs; }

private:
    XDBManager m_xCommonDBMgr;
    XDBStmt m_xDBStmt;
    int m_nMatchingWaitMs = 0;
};

class CRelayMazeOpenControl {
public:
    bool Init(const char* commonDNS);
    bool CheckMazeOpenTime(std::uint32_t dwMazeID);
    void SetCheckMazeOpenTime(bool bOn) { m_bCheckMazeOpenTime = bOn; }

private:
    bool LoadRows();

    XDBManager m_xCommonDBMgr;
    XDBStmt m_xDBStmt;
    std::map<std::uint32_t, TB_MAZE_OPENCONTROL> m_mapMazeOpenControl;
    bool m_bCheckMazeOpenTime = true;
};

class CRelayDistrictControl {
public:
    bool Init(const char* commonDNS);
    TB_DISTRICT* GetTB_DISTRICT(std::int16_t nDistrictID);

private:
    bool LoadRows();

    XDBManager m_xCommonDBMgr;
    XDBStmt m_xDBStmt;
    std::unordered_map<std::int16_t, TB_DISTRICT> m_mapDistrict;
};

class XRelayServer : public XServer {
public:
    XRelayServer();

    CRelayMazeOpenControl& GetMazeOpenControl() { return m_MazeOpenControl; }
    CRelayDistrictControl& GetDistrictControl() { return m_DistrictControl; }
    CRelayPartyMatchingConfig& GetPartyMatchingConfig() { return m_PartyMatchingConfig; }
    CRelayControlSocket& GetControlSocket() { return m_scControlSocket; }
    CForceManager& GetForceManager() { return m_ForceManager; }
    CForceMatchingMgr& GetForceMatchingMgr() { return m_ForceMatchingMgr; }
    CPartyManager& GetPartyManager() { return m_partyManager; }
    CPartyMatchingMgr& GetPartyMatchingMgr() { return m_PartyMatchingMgr; }
    CLeagueManager& GetLeagueManager() { return m_LeagueManger; }
    XResourceMgr& GetResourceMgr() { return resourceMgr_; }
    CFriendRecruitManager& GetRecruitManager() { return m_RecruitManager; }
    void AddServerInfo(CServer* pServer);
    void AddGameServerInfo(CServer* pServer);
    bool AddUser(CServer* pServer, STCharInfo& stInfo, UXMapID uxMapID, ST_GAME_OPTION& stGameOption);
    bool AddPartyUser(CServer* pServer, std::uint32_t dwActorID);
    bool AddLeagueUser(CServer* pServer, std::uint32_t dwUCID, int nLeagueID);
    std::shared_ptr<CUserObject> GetUser(std::uint32_t dwActorID);
    std::shared_ptr<CUserObject> GetUserByUAID(std::uint32_t dwUAID);
    std::shared_ptr<CUserObject> GetUser(const wchar_t* pName);
    std::shared_ptr<CUserPartyInfo> GetPartyUser(std::uint32_t dwActorID);
    bool SendFriendList(std::uint32_t dwActorID);
    bool SendBlockList(std::uint32_t dwActorID);
    bool IsFriendBlock(std::uint32_t dwUCID, std::uint32_t dwCheckUCID);
    bool IsFriendBlock(std::uint32_t dwUCID, const wchar_t* strTargetName);
    void SetUsersInfo(CServer* pServer, const PS_USERS_INFO* pUsersInfo);
    void UpdateUserMap(CServer* pServer, const PS_UPDATE_USER_MAP_INFO& updateInfo);
    void RemoveUser(std::uint32_t dwActorID, int nAccountState, bool bKickAlreadyLogin);
    void RemovePartyUser(std::uint32_t dwActorID, std::uint32_t dwUAID);
    void UpdateUserLevelUp(std::uint32_t dwActorID, std::uint8_t byLevel);
    void UpdateUserAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken);
    void UpdateUserProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwProfilePhotoID);
    void ReqExchangePriceList(CServer* pServer, const PS_EXCHANGE_PRICE_HISTORY_REQ& stReq);
    void ReqExchangePriceUpdate(CServer* pServer, const PS_EXCHANGE_PRICE_HISTORY_UPDATE* stUpdate);
    void CharacterNameChange(std::uint32_t dwActorID, const wchar_t* szChangeName);
    void ChangeFriendName(const PS_CHANGE_NAME& stChangeName);
    void SendMyRoomPollenUpdate(std::uint32_t dwUAID, int nPollenIndex,
                                const PS_MYROOM_POLLEN_HELP_USER* psHelpUser, std::uint64_t biHarvestDate);
    void SendChatNotice(const PS_CHAT_NOTICE& stChatNotice);
    void SendChatWhisper(std::uint32_t dwActorID,
                         const PS_CHAT_WHISPER& stChatWhisper,
                         const PS_CHAT_ITEM_LINK_FOR_SERVER& psItemLinkInfo);
    void SendChatMegaPhone(const PS_CHAT_MEGAPHONE& stMegaPhone,
                           const PS_CHAT_ITEM_LINK_FOR_SERVER& psItemLinkInfo);
    void SendCachingLoad();
    void SetCachingLoad(std::uint32_t loadMask);
    void UnSetCachingLoad(std::uint32_t loadMask);
    bool SendDBGame(const XSendDBPacket& packet);
    bool SendDBAccount(const XSendDBPacket& packet);
    void SendDBLog(int uaid,
                   int ucid,
                   std::uint16_t mainType,
                   std::uint16_t subType,
                   int param0,
                   int param1,
                   int param2,
                   int param3,
                   int param4,
                   std::int64_t param5,
                   std::int64_t param6,
                   const wchar_t* comment = L"");
    void SendDBChatLog(int uaid,
                       int ucid,
                       std::int16_t sType,
                       int param0,
                       int param1,
                       int param2,
                       int param3,
                       int param4,
                       std::int64_t param5,
                       std::int64_t param6,
                       const wchar_t* comment = L"");
    bool LoadForceDataReq();
    CServer* GetServer(std::uint32_t dwServerID);
    void KickOutUser(std::uint32_t dwUCID, std::uint8_t byType);
    void SendPacketAll(XSendPacket& xSendPacket);
    void SendPacket(std::uint32_t dwServerID, XSendPacket& xSendPacket);
    bool PrepareDeleteRecruit(const PS_RECRUIT_DELETE& stDelete);
    void SendRecruitDelete(std::uint32_t dwUCID);
    bool DeleteRecruit(const PS_RES_RECRUIT_DELETE& stDelete);
    void PrepareFriendInvite(const PS_RES_FRIEND_INVITE& stInvite);
    void PrepareFriendAccept(const PS_REQ_FRIEND_ACCEPT& stAccept);
    void PrepareDeleteFriend(const PS_REQ_FRIEND_DELETE& stDelete);
    void PrepareBlockListAdd(const PS_REQ_FRIEND_BLOCK_ADD& stBlock);
    void PrepareBlockListDel(const PS_REQ_FRIEND_BLOCK_DELETE& stBlock);
    void RecommandFriend(const PS_RES_FRIEND_RECOMMAND& stRecommand);
    void UpdateFriendCommunity(std::uint32_t dwActorID, const ST_CHAR_COMMUNITY& stCommunity);
    void ReqFriendFind(const PS_REQ_FRIEND_FIND& stFind);
    void DailyMissionFriendReq(const PS_DAILY_MISSION_FRIEND_REQ& psMission);
    void DailyMissionFriendRes(const PS_DAILY_MISSION_FRIEND_RES& psMission);
    void HelperSupportInfo(std::uint32_t dwUCID);
    void HelperSupportRegister(const PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport);
    void HelperSupportReward(const PS_SERVER_HELPER_SUPPORT_REWARD& psReward);
    void HelperSupportList(std::uint32_t dwUCID);
    void HelperSupportEquip(const PS_HELPER_SUPPORT_EQUIP_REQ& psEquip);
    // DB 响应处理
    void ResExchangePriceList(const PS_DB_EXCHANGE_PRICE_HISTORY_RES& psHistory);
    void ResHelperSupportEquipDB(const PS_DB_HELPER_SUPPORT_EQUIP& psEquip);
    void SendRecruitList(CServer* pServer, const PS_REQ_RECRUIT_LIST& stList);
    void SendRecruitAdd(const ST_RECRUIT_INFO& stAdd);
    void PrepareRecruitInfo(std::uint32_t dwUCID);

    // 对齐 IDA: 好友 DB 响应处理方法
    void SetCharCommunity(std::uint32_t dwActorID, const ST_CHAR_COMMUNITY* pCommunity);
    bool SetBlockLoad(std::uint32_t dwActorID, const DB_BLOCKLIST_INFO* pBlockList);  // 对齐 IDA 0x1400B3770
    void SetFriendLoad(const PS_DB_FRIEND_LIST* pFriendList);
    void SendFriendServerLoad(std::uint32_t dwActorID);
    void InviteFriend(const PS_RES_DB_FRIEND_INVITE* pInvite);
    void InviteCheckFriend(const PS_RES_FRIEND_INVITE* pInvite, std::uint32_t dwDelUCID);
    void AcceptFriend(const PS_DB_FRIEND_ACCEPT_RES* pAccept);
    void DeleteFriend(const PS_DB_FRIEND_DELETE* pDelete);
    void AddBlockList(const PS_RES_DB_FRIEND_BLOCK* pBlock);
    void DeleteBlockList(const PS_RES_BLOCKLIST_DELETE* pDelete);
    void SetRecruitList(const ST_RECRUIT_LIST* pList, std::uint8_t byLast);
    void SendServerInfoAll(CServer* pReqServer);  // 对齐 IDA 0x1400BD1E0
    void SendOperationTimeInfo(const PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO& psInfo);  // 对齐 IDA 0x1400BD410
    void ResFriendFind(const PS_DB_FRIEND_FIND* pFind);

    std::int64_t GetCurDateSec() const;
    void UpdateServerState();  // 对齐 IDA 0x1400BD5C0
    static int ConsolCtrlHandler(unsigned int dwOPCode);

protected:
    void SetName() override;
    bool InitServer() override;
    bool Clear(std::uint32_t maxWait) override;
    void OnUpdate(std::uint64_t currentTick) override;
    int SetConsoleHandler(int add) override;

private:
    XResourceMgr resourceMgr_;  // 资源管理器
    ST_SERVER_GROUP_INFO m_stServerGroupInfo{};
    bool m_bRegisterAuth = false;  // 对齐 IDA: 注册认证标志
    CRelayControlSocket m_scControlSocket;
    CObserveSocket m_scObserveSocket;  // 对齐 IDA: CObserveSocket 成员
    CRelayPartyMatchingConfig m_PartyMatchingConfig;
    CRelayMazeOpenControl m_MazeOpenControl;
    CRelayDistrictControl m_DistrictControl;
    XGameDBSocketMgr m_xDBAgentMgr;
    CFriendRecommandManager m_RecommandManager;
    CFriendRecruitManager m_RecruitManager;
    CLeagueManager m_LeagueManger;
    CPartyManager m_partyManager;
    CPartyMatchingMgr m_PartyMatchingMgr;
    CForceManager m_ForceManager;
    CForceMatchingMgr m_ForceMatchingMgr;
    CExchangePriceMgr m_ExchangePriceMgr;  // 对齐 IDA 构造函数
    CHelperSupportMgr m_HelperSupportMgr;  // 对齐 IDA 构造函数
    CModeMazeMatchingMgr m_ModeMazeMatchingMgr;  // 对齐 IDA 构造函数
    CFSRWLock m_rwLock{};
    CFSRWLock m_rwServerLock{};
    std::map<std::uint32_t, CServer*> m_mapGameServer;
    std::map<std::uint32_t, std::shared_ptr<CUserPartyInfo>> m_mapUserPartyInfos;
    std::map<std::uint32_t, std::shared_ptr<CUserObject>> m_mapUserInfos;
    std::uint32_t m_dwCachingLoad = 0;
    std::uint64_t m_dwConnectTick = 0;
    std::uint64_t m_dw64FPSTick = 0;
    std::uint32_t m_dwFrame = 0;
};
