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

// 对齐 IDA: E_SERVER_CACHING_LOAD 枚举
enum class E_SERVER_CACHING_LOAD : std::uint32_t {
    NONE = 0u,
    PARTY = 1u,
    LEAGUE = 2u,
    FRINED = 4u,  // 对齐 IDA 拼写（FRINED 而非 FRIEND）
    USER = 8u,
};

constexpr E_SERVER_CACHING_LOAD operator|(E_SERVER_CACHING_LOAD a, E_SERVER_CACHING_LOAD b) {
    return static_cast<E_SERVER_CACHING_LOAD>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
}

constexpr std::uint32_t operator&(E_SERVER_CACHING_LOAD a, E_SERVER_CACHING_LOAD b) {
    return static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b);
}

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
    // IDA: void AddUser(shared_ptr<CUserObject>) - 按值传递，无 SYSTEM_TYPE 参数
    // IDA 内部通过 XRelayServer::GetOption()->GetSystemType() 获取
    void AddUser(std::shared_ptr<CUserObject> pUser);

    // 对齐 IDA 0x140042600: 从推荐索引删除用户
    // IDA: void DeleteUser(shared_ptr<CUserObject>) - 按值传递
    void DeleteUser(std::shared_ptr<CUserObject> pUser);

    // 对齐 IDA 0x1400427C0: 删除用户检查映射
    void DeleteUserCheck(std::uint32_t dwUCID) {
        auto it = m_mapUserCheck.find(dwUCID);
        if (it != m_mapUserCheck.end()) {
            m_mapUserCheck.erase(it);
        }
    }

    // 对齐 IDA 0x140042850: 更新用户等级（重新索引）
    // IDA: void UpdateLevel(shared_ptr<CUserObject>) - 按值传递，无 SYSTEM_TYPE 参数
    void UpdateLevel(std::shared_ptr<CUserObject> pUser);

    // 对齐 IDA 0x1400428E0: 获取好友推荐列表 (V = shared_ptr 按值传递)
    int GetFriendRecommandList(std::shared_ptr<CUserObject> pUser,
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
    // IDA: void LoadRecruitList(vector<ST_RECRUIT_INFO>&, unsigned char) - 非 const 引用
    void LoadRecruitList(std::vector<ST_RECRUIT_INFO>& vecRecruitInfo, std::uint8_t byLast);

    // 对齐 IDA 0x140045110: 周期更新，清理过期招募
    void OnUpdate();

    // 对齐 IDA 0x140045330: 添加招募
    // IDA: bool AddRecruit(ST_RECRUIT_INFO) - 按值传递
    bool AddRecruit(ST_RECRUIT_INFO stAdd) {
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
    // IDA: QEAA_NK = 非const方法
    bool IsRecruitList(std::uint32_t dwUCID) {
        CFAutoSlimReadLock autolock(&m_rwLock);
        return m_mapRecruit.find(dwUCID) != m_mapRecruit.end();
    }

    // 对齐 IDA 0x140045700: 获取招募列表 (V = shared_ptr 按值传递)
    void GetFriendRecruitList(PS_RECRUIT_LIST& stInfo,
                               std::vector<ST_RECRUIT_INFO>& vecRecruitInfo,
                               std::shared_ptr<CUserObject> pUser);

    // 对齐 IDA 0x1400460C0: 获取招募添加时间 (QEAA = 非const, 返回 ATL::CTime 功能等价 int64_t)
    std::int64_t GetRecruitAddTime(std::uint32_t dwUCID) {
        CFAutoSlimReadLock autolock(&m_rwLock);
        auto it = m_mapRecruit.find(dwUCID);
        if (it != m_mapRecruit.end()) {
            return it->second->GetAddTime();
        }
        return 0;
    }

    // 对齐 IDA 0x1400462C0: 改名更新
    void ChangeName(PS_CHANGE_NAME stChangeName) {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        auto it = m_mapRecruit.find(stChangeName.dwActorID);
        if (it != m_mapRecruit.end()) {
            it->second->ChangeName(stChangeName.szChangeName);
        }
    }

    // 对齐 IDA 0x1400461C0: 更新社区状态
    void UpdateCharCommunity(std::uint32_t dwUCID, ST_CHAR_COMMUNITY& stCommunity) {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        auto it = m_mapRecruit.find(dwUCID);
        if (it != m_mapRecruit.end()) {
            it->second->UpdateCommunity(stCommunity);
        }
    }

    // 对齐 IDA 0x140064130: 检查是否已加载
    // IDA: QEAA_NXZ = 非const方法
    bool IsLoad() { return m_bDBLoad; }

private:
    // 简化的招募用户类（原版使用 boost::multi_index_container）
    class CRecruitUser {
    public:
        CRecruitUser() = default;  // 对齐 IDA: ??0CRecruitUser@@QEAA@XZ
        // 对齐 IDA: ??0CRecruitUser@@QEAA@AEAUST_RECRUIT_INFO@@@Z = 非const引用
        explicit CRecruitUser(ST_RECRUIT_INFO& info) : m_stInfo(info), m_bLogin(false) {}

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

        // 对齐 IDA: QEAAXAEAUST_RECRUIT_INFO@@@Z = 非const方法
        void GetInfo(ST_RECRUIT_INFO& info) {
            info = m_stInfo;
        }

        // 对齐 IDA 0x1400462C0: 改名
        void ChangeName(const wchar_t* szName) {
            wcscpy_s(m_stInfo.strName, szName);
        }

        // 对齐 IDA 0x1400461C0: 更新社区信息
        void UpdateCommunity(const ST_CHAR_COMMUNITY& stCommunity) {
            m_stInfo.byState = stCommunity.byState;
            wcscpy_s(m_stInfo.strMemo, stCommunity.szMemo);
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
    // 对齐 IDA: 嵌套结构 ST_EXCHANGE_PRICE_HISTORY_INFO
    struct ST_EXCHANGE_PRICE_HISTORY_INFO {
        std::uint32_t dwItemID = 0;
        std::int64_t n64Price_High = 0;
        std::int64_t n64Price_Low = 0;
        int nTotalCount = 0;
        std::int64_t n64TotalPrice = 0;
        std::map<std::int64_t, std::vector<ST_EXCHANGE_PRICE_INFO>> mapTimeList;

        ST_EXCHANGE_PRICE_HISTORY_INFO() = default;
        // 对齐 IDA 0x140012DF0: 构造函数
        explicit ST_EXCHANGE_PRICE_HISTORY_INFO(std::uint32_t dwID);
    };

    // 对齐 IDA 0x14000CD10: 加载价格列表到缓存
    bool LoadPriceList(PS_DB_EXCHANGE_PRICE_HISTORY_RES& psList);

    // 对齐 IDA 0x14000CAF0: 获取价格列表，返回 true 时缓存命中
    bool GetPriceList(std::uint32_t dwItemID, PS_EXCHANGE_PRICE_HISTORY_RES& psResult);

    // 对齐 IDA 0x14000D610: 获取 DB 请求日期
    std::int64_t GetDBRequestDate();

    // 对齐 IDA 0x14000CDC0: SetPriceInfo(uint32, int64, int64, int, int64)
    void SetPriceInfo(std::uint32_t dwItemID, std::int64_t n64Price_High,
                      std::int64_t n64Price_Low, int nTotalCount, std::int64_t n64TotalPrice);

    // 对齐 IDA 0x14000CE90: 添加价格列表
    bool AddPriceList(ST_EXCHANGE_PRICE_INFO& stInfo, bool bAddPrice);

    // 对齐 IDA 0x14000D3B0: 删除旧的价格列表
    bool DeletePriceList_Old(std::uint32_t dwItemID);

    // 对齐 IDA 0x14000D4E0: 获取价格列表计数
    int GetPriceListCount(std::uint32_t dwItemID);

private:
    std::map<std::uint32_t, ST_EXCHANGE_PRICE_HISTORY_INFO> m_mapPriceHistory;  // itemID -> 价格历史
    std::int64_t m_n64DBRequestDate = 0;  // 对齐 IDA: DB 请求日期（分区用）
    mutable CFSRWLock m_rwLock{};  // 对齐 IDA: 读写锁
};

// 对齐 IDA: 助战支持对象
class CHelperSupport {
public:
    CHelperSupport() = default;
    ~CHelperSupport() = default;

    // 对齐 IDA 0x14002E8F0: 初始化助战信息 (AEAV = 非const引用)
    void Init(ST_HELPER_SUPPORT_INFO& stInfo) {
        m_stInfo = stInfo;
        m_byRewardState = 1;  // 可领取状态
        m_setRecvList.clear();
    }

    // 对齐 IDA 0x14002EA40: 检查是否在有效期内 (QEAA_NXZ = 非const)
    bool CheckVaildTime() {
        return m_stInfo.nDate >= static_cast<std::int64_t>(std::time(nullptr));
    }

    // 对齐 IDA 0x140032900: 获取奖励状态 (QEAAEXZ = 非const)
    std::uint8_t GetRewardState() { return m_byRewardState; }

    // 对齐 IDA 0x14002EDB0: 设置奖励状态
    void SetMatchingState(std::uint8_t byState) { m_byRewardState = byState; }

    // 对齐 IDA 0x1400329F0: 获取助战信息 (QEAAXAEAV = 非const方法+非const引用)
    void GetSupportInfo(ST_HELPER_SUPPORT_INFO& stInfo) { stInfo = m_stInfo; }

    // 对齐 IDA 0x14002ED90: 获取已领取人数 (QEAAHXZ = 非const, H=int)
    int GetRecvCount() { return static_cast<int>(m_setRecvList.size()); }

    // 对齐 IDA 0x14002E950: 检查是否已领取 (QEAA_NK = 非const)
    bool CheckReceived(std::uint32_t dwUCID) {
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

    // 对齐 IDA 0x14002EF70: 查找助战 (QEAA = 非const)
    std::shared_ptr<CHelperSupport> FindSupport(std::uint32_t dwUCID) {
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
        if (it == m_mapSupport.end()) {
            // 对齐 IDA: 不存在，创建新的并添加时间槽
            auto pSupport = std::make_shared<CHelperSupport>();
            pSupport->Init(psSupport.stInfo);
            m_mapSupport[psSupport.stInfo.dwFriendUCID] = pSupport;
            AddSupportTimeInternal(psSupport.stInfo.nDate, psSupport.stInfo.dwFriendUCID);
            return true;
        }

        // 对齐 IDA: 已存在，检查是否有效
        if (it->second->CheckVaildTime()) {
            return false;  // 有效期内，不能重复注册
        }

        // 对齐 IDA: 已过期，先删除旧时间槽，更新信息，添加新时间槽
        ST_HELPER_SUPPORT_INFO stOld{};
        it->second->GetSupportInfo(stOld);
        DeleteSupportTimeInternal(stOld.nDate, stOld.dwFriendUCID);
        it->second->Init(psSupport.stInfo);
        AddSupportTimeInternal(psSupport.stInfo.nDate, psSupport.stInfo.dwFriendUCID);
        return true;
    }

    // 对齐 IDA 0x14002F330: 删除助战
    bool DeleteSupport(std::uint32_t dwUCID) {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        auto it = m_mapSupport.find(dwUCID);
        if (it != m_mapSupport.end()) {
            // 对齐 IDA: 先获取信息，删除时间槽，再从map移除
            ST_HELPER_SUPPORT_INFO stInfo{};
            it->second->GetSupportInfo(stInfo);
            DeleteSupportTimeInternal(stInfo.nDate, dwUCID);
            m_mapSupport.erase(it);
            return true;
        }
        return false;
    }

    // 对齐 IDA 0x14002F860: 获取助战奖励状态 (QEAAEK = 非const, E=uint8_t返回, K=uint32_t参数)
    std::uint8_t GetSupportReward(std::uint32_t dwUCID) {
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

    // 对齐 IDA 0x14002F450: AddSupportTime(_J, K) → _N
    // 将用户添加到指定时间槽的列表（ATL::CTime 日期归一化后存储）
    bool AddSupportTime(std::int64_t nDate, std::uint32_t dwUCID) {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        return AddSupportTimeInternal(nDate, dwUCID);
    }

    // 对齐 IDA 0x14002F680: DeleteSupportTime(_J, K) → _N
    // 从指定时间槽的列表中移除用户
    bool DeleteSupportTime(std::int64_t nDate, std::uint32_t dwUCID) {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        return DeleteSupportTimeInternal(nDate, dwUCID);
    }

private:
    // 对齐 IDA: 内部版本，不获取锁（供 AddSupport 在已持有锁时调用）
    bool AddSupportTimeInternal(std::int64_t nDate, std::uint32_t dwUCID) {
        // 对齐 IDA: 将日期归一化为整分钟（秒数清零）
        std::tm* tmDate = std::localtime(reinterpret_cast<const std::time_t*>(&nDate));
        if (!tmDate) return false;
        tmDate->tm_sec = 0;
        const std::int64_t normalizedDate = static_cast<std::int64_t>(std::mktime(tmDate));

        auto it = m_mapSupportTime.find(normalizedDate);
        if (it == m_mapSupportTime.end()) {
            std::list<std::uint32_t> newList;
            newList.push_back(dwUCID);
            m_mapSupportTime[normalizedDate] = std::move(newList);
        } else {
            it->second.push_back(dwUCID);
        }
        return true;
    }

    bool DeleteSupportTimeInternal(std::int64_t nDate, std::uint32_t dwUCID) {
        std::tm* tmDate = std::localtime(reinterpret_cast<const std::time_t*>(&nDate));
        if (!tmDate) return false;
        tmDate->tm_sec = 0;
        const std::int64_t normalizedDate = static_cast<std::int64_t>(std::mktime(tmDate));

        auto it = m_mapSupportTime.find(normalizedDate);
        if (it == m_mapSupportTime.end()) {
            return false;
        }
        auto& list = it->second;
        for (auto listIt = list.begin(); listIt != list.end(); ++listIt) {
            if (*listIt == dwUCID) {
                list.erase(listIt);
                return true;
            }
        }
        return false;
    }

public:

private:
    std::map<std::uint32_t, std::shared_ptr<CHelperSupport>> m_mapSupport;
    // 对齐 IDA: 助战时间槽映射 (归一化时间 → 用户 ID 列表)
    std::map<std::int64_t, std::list<std::uint32_t>> m_mapSupportTime;
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
    CModeMazeMatchingMgr& GetModeMazeMatchingMgr() { return m_ModeMazeMatchingMgr; }
    XResourceMgr& GetResourceMgr() { return resourceMgr_; }
    CFriendRecruitManager& GetRecruitManager() { return m_RecruitManager; }
    void AddServerInfo(CServer* pServer);
    void AddGameServerInfo(CServer* pServer);
    void RemoveGameServerInfo(SS_SERVER_INFO& serverInfo);  // 对齐 IDA 0x1400B29A0
    void RemoveServerInfo(CServer* pServer, SS_SERVER_INFO& serverInfo);  // 对齐 IDA 0x1400B2BA0
    bool LoadDataReq(int nIndex);  // 对齐 IDA 0x1400B2C10
    void ClearUserState(std::uint32_t dwServerID);  // 对齐 IDA 0x1400B3160
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
    bool IsFriendBlock(std::uint32_t dwUCID, wchar_t* strTargetName);  // 对齐 IDA: 非 const
    void SetUsersInfo(CServer* pServer, PS_USERS_INFO& stUsersInfo);  // 对齐 IDA 0x1400BA510
    void UpdateUserMap(CServer* pServer, PS_UPDATE_USER_MAP_INFO& updateInfo);  // 对齐 IDA 0x1400B2030
    void RemoveUser(std::uint32_t dwActorID, int nAccountState, bool bKickAlreadyLogin);
    void RemovePartyUser(std::uint32_t dwActorID, std::uint32_t dwUAID);
    void UpdateUserLevelUp(std::uint32_t dwActorID, std::uint8_t byLevel);
    void UpdateUserAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken);
    void UpdateUserProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwProfilePhotoID);
    void ReqExchangePriceList(CServer* pServer, PS_EXCHANGE_PRICE_HISTORY_REQ& stReq);  // 对齐 IDA 0x1400BB590
    void ReqExchangePriceUpdate(CServer* pServer, PS_EXCHANGE_PRICE_HISTORY_UPDATE& stUpdate);  // 对齐 IDA 0x1400BB9A0
    void CharacterNameChange(std::uint32_t dwActorID, wchar_t* szChangeName);  // 对齐 IDA 0x1400BD740: 非 const 指针
    bool ChangeFriendName(PS_CHANGE_NAME stChangeName);  // 对齐 IDA 0x1400BDE10 (by value)
    void SendMyRoomPollenUpdate(std::uint32_t dwUAID, int nPollenIndex,
                                PS_MYROOM_POLLEN_HELP_USER psHelpUser, std::int64_t biHarvestDate);  // 对齐 IDA 0x1400BDC80
    void SendChatNotice(PS_CHAT_NOTICE& stChatNotice);  // 对齐 IDA 0x1400BA3C0
    void SendChatWhisper(std::uint32_t dwActorID,
                         PS_CHAT_WHISPER& stChatWhisper,
                         PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfo);  // 对齐 IDA 0x1400B9DF0
    void SendChatMegaPhone(PS_CHAT_MEGAPHONE& stMegaPhone,
                           PS_CHAT_ITEM_LINK_FOR_SERVER psItemLinkInfo);  // 对齐 IDA 0x1400BA450
    void SendCachingLoad();
    void SetCachingLoad(E_SERVER_CACHING_LOAD loadMask);  // 对齐 IDA 0x1400BD8E0
    void UnSetCachingLoad(E_SERVER_CACHING_LOAD loadMask);  // 对齐 IDA 0x1400BD9B0
    bool SendDBGame(XSendDBPacket& packet);  // 对齐 IDA 0x1400BD530: 非 const 引用
    bool SendDBAccount(XSendDBPacket& packet);  // 对齐 IDA 0x1400BD4A0: 非 const 引用
    void SendDBLog(int uaid,
                   int ucid,
                   std::int16_t mainType,  // 对齐 IDA: __int16
                   std::int16_t subType,   // 对齐 IDA: __int16
                   int param0,
                   int param1,
                   int param2,
                   int param3,
                   int param4,
                   std::int64_t param5,
                   std::int64_t param6,
                   wchar_t* comment = nullptr);  // 对齐 IDA: 非 const 指针
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
                       wchar_t* comment = nullptr);  // 对齐 IDA: 非 const 指针
    bool LoadForceDataReq();
    CServer* GetServer(std::uint32_t dwServerID);
    void KickOutUser(std::uint32_t dwUCID, std::uint8_t byType);  // 对齐 IDA 0x1400B25F0
    bool SendPacketAll(XSendPacket& xSendPacket);  // 对齐 IDA 0x1400B2870
    bool SendPacket(std::uint32_t dwServerID, XSendPacket& xSendPacket);  // 对齐 IDA 0x1400B26D0
    bool SendPacketToGameServer(XSendPacket& xSendPacket, CServer* pExceptSession);  // 对齐 IDA 0x1400B27A0
    bool PrepareDeleteRecruit(PS_RECRUIT_DELETE& stDelete);  // 对齐 IDA 0x1400B8930
    void SendRecruitDelete(std::uint32_t dwUCID);
    bool PrepareAddRecruit(PS_RECRUIT_ADD& stAdd);  // 对齐 IDA 0x1400B8480
    bool AddRecruit(PS_RES_RECRUIT_ADD& stInfo);  // 对齐 IDA 0x1400B8760
    bool RecruitList(PS_RECRUIT_LIST& stInfo);  // 对齐 IDA 0x1400B8E90 (IDA: PS_RECRUIT_LIST)
    bool DeleteRecruit(PS_RES_RECRUIT_DELETE& stDelete);  // 对齐 IDA 0x1400B8D00
    bool PrepareFriendInvite(PS_RES_FRIEND_INVITE& stInvite);  // 对齐 IDA 0x1400B4000
    bool PrepareFriendAccept(PS_REQ_FRIEND_ACCEPT& stAccept);  // 对齐 IDA 0x1400B6150
    bool PrepareDeleteFriend(PS_REQ_FRIEND_DELETE& stDelete);  // 对齐 IDA 0x1400B6F10
    bool PrepareBlockListAdd(PS_REQ_FRIEND_BLOCK_ADD& stBlock);  // 对齐 IDA 0x1400B77B0
    bool PrepareBlockListDel(PS_REQ_FRIEND_BLOCK_DELETE& stBlock);  // 对齐 IDA 0x1400B7E90
    bool RecommandFriend(PS_RES_FRIEND_RECOMMAND& stRecommand);  // 对齐 IDA 0x1400B9AA0
    bool UpdateFriendCommunity(std::uint32_t dwActorID, ST_CHAR_COMMUNITY& stCommunity);  // 对齐 IDA 0x1400B3CF0
    bool ReqFriendFind(PS_REQ_FRIEND_FIND& stFind);  // 对齐 IDA 0x1400B9440
    void DailyMissionFriendReq(PS_DAILY_MISSION_FRIEND_REQ& psMission);  // 对齐 IDA 0x1400BB1E0
    void DailyMissionFriendRes(PS_DAILY_MISSION_FRIEND_RES& psMission);  // 对齐 IDA 0x1400BB3E0
    void HelperSupportInfo(std::uint32_t dwUCID);  // 对齐 IDA 0x1400BBC10
    void HelperSupportRegister(PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport);  // 对齐 IDA 0x1400BBE90
    void HelperSupportReward(PS_SERVER_HELPER_SUPPORT_REWARD& psReward);  // 对齐 IDA 0x1400BC0D0
    void HelperSupportList(std::uint32_t dwUCID);  // 对齐 IDA 0x1400BC320
    void HelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_REQ& psEquip);  // 对齐 IDA 0x1400BC7F0
    bool ReqLeagueInvite(CServer* pServer, ST_REQ_LEAGUE_INVITE& stInvite, ST_REQ_LEAGUE_INVITE& stInviteRes);  // 对齐 IDA 0x1400BAE60
    // DB 响应处理
    void ResExchangePriceList(PS_DB_EXCHANGE_PRICE_HISTORY_RES& psHistory);  // 对齐 IDA 0x1400BB770
    void ResHelperSupportEquip(PS_DB_HELPER_SUPPORT_EQUIP& psEquip);  // 对齐 IDA 0x1400BCDC0
    void SendRecruitList(CServer* pServer, PS_RECRUIT_LIST& stList);
    void SendRecruitAdd(ST_RECRUIT_INFO& stAdd);
    bool PrepareRecruitInfo(std::uint32_t dwUCID);  // 对齐 IDA 0x1400B8B60

    // 对齐 IDA: 好友 DB 响应处理方法
    bool SetCharCommunity(std::uint32_t dwActorID, ST_CHAR_COMMUNITY& stCharCommunity);  // 对齐 IDA 0x1400B3990
    bool SetBlockLoad(std::uint32_t dwActorID, PS_BLOCKLIST_INFO& stBlockList);  // 对齐 IDA 0x1400B3770
    bool SetFriendLoad(PS_DB_FRIEND_LIST& stFriendList);  // 对齐 IDA 0x1400B3400
    bool SendFriendServerLoad(std::uint32_t dwUCID);  // 对齐 IDA 0x1400B3310
    bool InviteFriend(PS_RES_DB_FRIEND_INVITE& stInvite);  // 对齐 IDA 0x1400B4BA0
    bool InviteCheckFriend(PS_RES_FRIEND_INVITE& stInvite, std::uint32_t dwDelUCID);  // 对齐 IDA 0x1400B5860
    bool AcceptFriend(PS_DB_FRIEND_ACCEPT_RES& stAccept);  // 对齐 IDA 0x1400B6850
    bool DeleteFriend(PS_DB_FRIEND_DELETE& stDelete);  // 对齐 IDA 0x1400B7330
    bool AddBlockList(PS_RES_DB_FRIEND_BLOCK& stBlock);  // 对齐 IDA 0x1400B7B30
    bool DeleteBlockList(PS_RES_BLOCKLIST_DELETE& stDelete);  // 对齐 IDA 0x1400B8190
    bool SetRecruitList(ST_RECRUIT_LIST& stList, std::uint8_t byLast);  // 对齐 IDA 0x1400B8440
    void SendServerInfoAll(CServer* pReqServer);  // 对齐 IDA 0x1400BD1E0
    void SendOperationTimeInfo(PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO& psInfo);  // 对齐 IDA 0x1400BD410
    bool ResFriendFind(PS_DB_FRIEND_FIND& stFind);  // 对齐 IDA 0x1400B94E0

    std::int64_t GetCurDateSec();  // 对齐 IDA 0x1400BD8B0: 非const (QEAA_JXZ)
    void UpdateServerState();  // 对齐 IDA 0x1400BD5C0
    static int ConsolCtrlHandler(unsigned int dwOPCode);

protected:
    void SetName() override;
    bool InitServer() override;
    bool Clear() override;  // 对齐 IDA 0x1400B0950: MEAA_NXZ 无参数
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
    E_SERVER_CACHING_LOAD m_dwCachingLoad = E_SERVER_CACHING_LOAD::NONE;
    std::uint64_t m_dwConnectTick = 0;
    std::uint64_t m_dw64FPSTick = 0;
    std::uint32_t m_dwFrame = 0;
};
