#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"  // PS_CHANGE_NAME
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

struct ST_FRIEND_COMMUNITY {
    std::uint8_t byState = 0;
    wchar_t strMemo[31] = {};
};

// 对齐 IDA: ST_FRIEND_COMMUNITY 序列化
inline XPacket& operator<<(XPacket& packet, const ST_FRIEND_COMMUNITY& value) {
    packet.XParse << value.byState;
    packet.XParse << GreenDamTan_BoundedWideString(value.strMemo);
    return packet;
}

struct ST_FRIEND_INFO {
    wchar_t strName[21] = {};
    std::uint32_t dwID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint32_t dwProfilePhotoID = 0;
    std::uint8_t byType = 0;
    std::uint8_t byState = 0;
    wchar_t strMemo[31] = {};
    std::uint8_t byChannel = 0;
    std::uint16_t wMapID = 0;
    std::int64_t nFriendPoint = 0;
    bool bLogin = false;
    std::int64_t tLogOut = 0;
    std::int64_t tRemain = 0;
};

struct ST_BLOCK_INFO {
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byLevel = 0;
};

inline void operator>>(XPacket& packet, ST_BLOCK_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.dwUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byLevel;
}

struct PS_FRIEND_LIST {
    std::vector<ST_FRIEND_INFO> vecFriends;
};

struct PS_BLOCKLIST_INFO {
    std::vector<ST_BLOCK_INFO> vecBlockList;
};

inline XPacket& operator<<(XPacket& packet, const ST_FRIEND_INFO& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.dwID;
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.byType;
    packet.XParse << value.byState;
    packet.XParse << GreenDamTan_BoundedWideString(value.strMemo);
    packet.XParse << value.byChannel;
    packet.XParse << value.wMapID;
    packet.XParse << value.nFriendPoint;
    packet.XParse << value.bLogin;
    packet.XParse << value.tLogOut;
    packet.XParse << value.tRemain;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_BLOCK_INFO& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.byLevel;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_FRIEND_LIST& value) {
    const std::uint8_t count = static_cast<std::uint8_t>(std::min<std::size_t>(value.vecFriends.size(), 0xFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < count; ++index) {
        packet << value.vecFriends[index];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_BLOCKLIST_INFO& value) {
    const std::uint8_t count = static_cast<std::uint8_t>(std::min<std::size_t>(value.vecBlockList.size(), 0xFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < count; ++index) {
        packet << value.vecBlockList[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_BLOCKLIST_INFO& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecBlockList.clear();
    value.vecBlockList.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        ST_BLOCK_INFO blockInfo{};
        packet >> blockInfo;
        value.vecBlockList.push_back(blockInfo);
    }
}

class CUserObject;
class XRelayServer;

template <typename T>
class TXSingleton;

class CFriendMember {
public:
    ST_FRIEND_INFO m_stFriendInfo{};
    std::shared_ptr<CUserObject> m_pFriend;

    std::uint32_t GetUCID() const { return m_stFriendInfo.dwID; }
    std::uint8_t GetType() const { return m_stFriendInfo.byType; }
    std::wstring GetName() const { return FixedWideArrayToWString(m_stFriendInfo.strName); }
};

class CBlockUser {
public:
    ST_BLOCK_INFO m_stBlockInfo{};

    std::uint32_t GetUCID() const { return m_stBlockInfo.dwUCID; }
    std::wstring GetName() const { return FixedWideArrayToWString(m_stBlockInfo.strName); }
};

class CCommunity {
public:
    void Clear() {
        m_vecFriend.clear();
        m_vecBlockList.clear();
        m_bLoadFriendList = false;
        m_bLoadBlockList = false;
        m_bSyncFriendList = false;
        m_bSyncBlockList = false;
        m_nRecommandIndex = 0;
    }

    // 对齐 IDA: QEAA_NKE@Z = 非const方法
    bool IsFriend(std::uint32_t dwUCID, std::uint8_t byType) {
        if (!IsValidCommunityType(byType)) {
            return false;
        }

        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(), [dwUCID](const std::shared_ptr<CFriendMember>& friendMember) {
            return friendMember->GetUCID() == dwUCID;
        });
        return it != m_vecFriend.end() && (*it)->GetType() == byType;
    }

    // 对齐 IDA: QEAA_NPEA_WE@Z = 非const方法，参数 wchar_t*
    bool IsFriend(wchar_t* pName, std::uint8_t byType) {
        if (!pName || !IsValidCommunityType(byType)) {
            return false;
        }

        const std::wstring targetName(pName);
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(), [&targetName](const std::shared_ptr<CFriendMember>& friendMember) {
            return friendMember->GetName() == targetName;
        });
        return it != m_vecFriend.end() && (*it)->GetType() == byType;
    }

    // 对齐 IDA: QEAA_NK@Z = 非const方法
    bool IsBlockList(std::uint32_t dwUCID) {
        return std::any_of(m_vecBlockList.begin(), m_vecBlockList.end(), [dwUCID](const std::shared_ptr<CBlockUser>& blockUser) {
            return blockUser->GetUCID() == dwUCID;
        });
    }

    // 对齐 IDA: QEAA_NPEA_W@Z = 非const方法，参数 wchar_t*
    bool IsBlockList(wchar_t* pName) {
        if (!pName) {
            return false;
        }

        const std::wstring targetName(pName);
        return std::any_of(m_vecBlockList.begin(), m_vecBlockList.end(), [&targetName](const std::shared_ptr<CBlockUser>& blockUser) {
            return blockUser->GetName() == targetName;
        });
    }

    // 对齐 IDA: QEAAKPEA_W@Z = 非const方法
    std::uint32_t GetFriendUCID(wchar_t* pName) {
        if (!pName) {
            return 0;
        }

        const std::wstring targetName(pName);
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(), [&targetName](const std::shared_ptr<CFriendMember>& friendMember) {
            return friendMember->GetName() == targetName;
        });
        return it == m_vecFriend.end() ? 0u : (*it)->GetUCID();
    }

    // 对齐 IDA: QEAAXAEAUPS_FRIEND_LIST@@E@Z = 非const方法
    void GetFriendList(PS_FRIEND_LIST& stFriendList, std::uint8_t byType) {
        stFriendList.vecFriends.clear();
        for (const auto& friendMember : m_vecFriend) {
            if (byType != 0 && friendMember->GetType() != byType) {
                continue;
            }
            stFriendList.vecFriends.push_back(friendMember->m_stFriendInfo);
        }
    }

    // 对齐 IDA: QEAAXAEAV?$vector@V?$shared_ptr@VCFriendMember@@@tr1@std@@...E@Z = 对象列表版本
    void GetFriendList(std::vector<std::shared_ptr<CFriendMember>>& vecFriendList, std::uint8_t byType) {
        vecFriendList.clear();
        for (const auto& friendMember : m_vecFriend) {
            if (byType == 0 || friendMember->GetType() == byType) {
                vecFriendList.push_back(friendMember);
            }
        }
    }

    // 对齐 IDA: QEAAXAEAUPS_BLOCKLIST_INFO@@@Z = 非const方法
    void GetBlcokList(PS_BLOCKLIST_INFO& stBlockList) {
        stBlockList.vecBlockList.clear();
        for (const auto& blockUser : m_vecBlockList) {
            stBlockList.vecBlockList.push_back(blockUser->m_stBlockInfo);
        }
    }

    void SetLoadFriendList(bool bLoad) { m_bLoadFriendList = bLoad; }
    void SetLoadBlockList(bool bLoad) { m_bLoadBlockList = bLoad; }
    // 对齐 IDA: QEAA_NXZ = 非const方法
    bool GetLoadFriendList() { return m_bLoadFriendList; }
    bool GetLoadBlockList() { return m_bLoadBlockList; }
    void SetSyncFriendList(bool bSync) { m_bSyncFriendList = bSync; }
    void SetSyncBlockList(bool bSync) { m_bSyncBlockList = bSync; }

    // 对齐 IDA: 好友/黑名单加载方法
    void AddBlockUser(const std::shared_ptr<CBlockUser>& blockUser) { m_vecBlockList.push_back(blockUser); }
    // 对齐 IDA: QEAA_NXZ = 非const方法
    bool GetSyncBlockList() { return m_bSyncBlockList; }
    bool GetSyncFriendList() { return m_bSyncFriendList; }

    // 对齐 IDA: QEAAEK@Z = 非const方法，返回 uint8_t (E)
    std::uint8_t GetFriendType(std::uint32_t dwUCID) {
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                                     [dwUCID](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == dwUCID; });
        return it != m_vecFriend.end() ? (*it)->GetType() : 0;
    }

    // 对齐 IDA: QEAA_NAEAU@Z = 非const方法，参数为非const引用
    bool IsChangeFriendInfo(ST_FRIEND_INFO& stInfo) {
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                                     [&stInfo](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == stInfo.dwID; });
        if (it == m_vecFriend.end()) return false;
        const auto& cur = (*it)->m_stFriendInfo;
        return cur.byLevel != stInfo.byLevel ||
               cur.byAwaken != stInfo.byAwaken ||
               cur.dwProfilePhotoID != stInfo.dwProfilePhotoID ||
               cur.byChannel != stInfo.byChannel ||
               cur.wMapID != stInfo.wMapID ||
               cur.bLogin != stInfo.bLogin ||
               cur.byState != stInfo.byState;
    }

    // 对齐 IDA: QEAAXAEAU...V...@Z = 非const方法，参数1为非const引用，参数2为shared_ptr按值传递
    // 对齐 IDA 0x140002290: 根据bLogin标志处理m_pFriend，并更新各字段
    void UpdateFriendInfo(ST_FRIEND_INFO& stInfo, std::shared_ptr<CUserObject> pFriend) {
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                               [&stInfo](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == stInfo.dwID; });
        if (it != m_vecFriend.end()) {
            auto& member = *it;
            // 对齐 IDA: bLogin=true 且 pFriend 有效时更新 m_pFriend
            if (stInfo.bLogin) {
                if (pFriend) {
                    member->m_pFriend = pFriend;
                }
            } else {
                // 对齐 IDA: bLogin=false 时清除 m_pFriend
                member->m_pFriend = nullptr;
            }
            // 对齐 IDA: 更新各字段
            member->m_stFriendInfo.bLogin = stInfo.bLogin;
            member->m_stFriendInfo.wMapID = stInfo.wMapID;
            member->m_stFriendInfo.byChannel = stInfo.byChannel;
            member->m_stFriendInfo.byLevel = stInfo.byLevel;
            member->m_stFriendInfo.byAwaken = stInfo.byAwaken;
            member->m_stFriendInfo.dwProfilePhotoID = stInfo.dwProfilePhotoID;
            member->m_stFriendInfo.byState = stInfo.byState;
            // 对齐 IDA: byType 仅在非 0 时更新
            if (stInfo.byType) {
                member->m_stFriendInfo.byType = stInfo.byType;
            }
#ifdef _WIN32
            wcscpy_s(member->m_stFriendInfo.strMemo, stInfo.strMemo);
#else
            std::wcsncpy(member->m_stFriendInfo.strMemo, stInfo.strMemo, 30);
            member->m_stFriendInfo.strMemo[30] = L'\0';
#endif
        }
    }

    // 对齐 IDA: ?AddFriend@CCommunity@@QEAA_NV?$shared_ptr@VCFriendMember@@@tr1@std@@@Z
    // 参数为 shared_ptr<CFriendMember> 按值传递，返回 bool
    bool AddFriend(std::shared_ptr<CFriendMember> pFriendMember) {
        if (!pFriendMember) return false;
        // 检查是否已存在同 UCID
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
            [&pFriendMember](const std::shared_ptr<CFriendMember>& m) {
                return m->GetUCID() == pFriendMember->GetUCID();
            });
        if (it != m_vecFriend.end()) return false;
        m_vecFriend.push_back(std::move(pFriendMember));
        return true;
    }

    // 对齐 IDA: 删除好友（按 UCID）
    void DeleteFriend(std::uint32_t dwUCID) {
        m_vecFriend.erase(
            std::remove_if(m_vecFriend.begin(), m_vecFriend.end(),
                           [dwUCID](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == dwUCID; }),
            m_vecFriend.end());
    }

    // 对齐 IDA: 更新好友信息（bAddIfNotExists=true: 不存在则添加; =false: 仅更新）
    void UpdateFriend(ST_FRIEND_INFO& stInfo, bool bAddIfNotExists) {
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                               [&stInfo](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == stInfo.dwID; });
        if (it != m_vecFriend.end()) {
            (*it)->m_stFriendInfo = stInfo;
        } else if (bAddIfNotExists) {
            auto pMember = std::make_shared<CFriendMember>();
            pMember->m_stFriendInfo = stInfo;
            m_vecFriend.push_back(std::move(pMember));
        }
    }

    // 对齐 IDA: 添加黑名单
    bool AddBlockList(ST_BLOCK_INFO& stBlock);  // 对齐 IDA: 返回 bool

    // 对齐 IDA: ?AddBlock@CCommunity@@QEAA_NV?$shared_ptr@VCBlockUser@@@tr1@std@@@Z
    bool AddBlock(std::shared_ptr<CBlockUser> pBlockUser) {
        if (!pBlockUser) return false;
        auto it = std::find_if(m_vecBlockList.begin(), m_vecBlockList.end(),
            [&pBlockUser](const std::shared_ptr<CBlockUser>& u) {
                return u->GetUCID() == pBlockUser->GetUCID();
            });
        if (it != m_vecBlockList.end()) return false;
        m_vecBlockList.push_back(std::move(pBlockUser));
        return true;
    }

    // 对齐 IDA: 删除黑名单（按 UCID）
    void DeleteBlockList(std::uint32_t dwUCID);  // 对齐 IDA 0x140002570

    // 对齐 IDA: 社区状态/备忘录设置
    void SetCommunityState(std::uint8_t byState) { m_stCommunity.byState = byState; }
    // 对齐 IDA: QEAAXPEA_W@Z = 非const wchar_t* 参数
    void SetMemo(wchar_t* strMemo) {
        if (strMemo) {
#ifdef _WIN32
            wcscpy_s(m_stCommunity.strMemo, strMemo);
#else
            std::wcsncpy(m_stCommunity.strMemo, strMemo, 30);
            m_stCommunity.strMemo[30] = L'\0';
#endif
        }
    }
    // 对齐 IDA: QEAAXAEAUST_FRIEND_COMMUNITY@@@Z = 非const方法，参数为非const引用
    void UpdateCharCommunity(ST_FRIEND_COMMUNITY& stCommunity) {
        m_stCommunity = stCommunity;
    }
    // 对齐 IDA 0x1400D68F0: SetCharCommunity（直接赋值，与 UpdateCharCommunity 相同实现）
    void SetCharCommunity(ST_FRIEND_COMMUNITY& stCommunity) {
        m_stCommunity = stCommunity;
    }
    // 对齐 IDA: QEAAEXZ = 非const方法
    std::uint8_t GetCommunityState() { return m_stCommunity.byState; }
    // 对齐 IDA: QEAA?AV...@Z = 非const方法
    std::wstring GetMemo() { return FixedWideArrayToWString(m_stCommunity.strMemo); }

    // 对齐 IDA: QEAA_NE@Z = 非const方法
    bool IsValidCommunityType(std::uint8_t byType) {
        return byType >= 1 && byType <= 3;
    }

    // 对齐 IDA 0x1400011C0: 好友列表容量检查
    // type 1 (好友): 上限 100, type 2 (推荐好友): 上限 20, type 3 (招募): 无限制
    // type 101 (黑名单): 上限 50
    // 对齐 IDA: QEAA_NE@Z = 非const方法
    bool IsValiedListCount(std::uint8_t byType) {
        switch (byType) {
        case 1: {
            std::size_t count = 0;
            for (const auto& f : m_vecFriend) {
                if (f->GetType() == 1) ++count;
            }
            return count < 100;
        }
        case 2: {
            std::size_t count = 0;
            for (const auto& f : m_vecFriend) {
                if (f->GetType() == 2) ++count;
            }
            return count < 20;
        }
        case 3:
            return true;
        case 101:
            return m_vecBlockList.size() < 50;
        default:
            return false;
        }
    }

    // 对齐 IDA 0x140002610: 好友邀请检查
    // byResult: 0=可邀请, 2=已是好友, 3=已是推荐好友, 4=推荐列表已满, 5=在黑名单中
    // 对齐 IDA: QEAA_NAEAU@Z = 非const方法
    bool CheckFriendInvite(PS_RES_FRIEND_INVITE& stInvite) {
        stInvite.byResult = 0;
        if (IsFriend(stInvite.dwReqUCID, 1u)) {
            stInvite.byResult = 2;
        } else if (IsFriend(stInvite.dwReqUCID, 2u)) {
            stInvite.byResult = 3;
        } else if (IsBlockList(stInvite.dwReqUCID)) {
            stInvite.byResult = 5;
        } else if (!IsValiedListCount(2u)) {
            stInvite.byResult = 4;
            return true;
        }
        return stInvite.byResult == 0;
    }

    // 对齐 IDA 0x1400026E0: 好友接受检查
    // nResult: 0=可接受, 55101=已是好友, 55105=在黑名单中, 55103=好友列表已满
    // 对齐 IDA: QEAA_NAEAU...AEAH@Z = 非const方法，参数2为非const引用
    bool CheckFriendAccept(PS_REQ_FRIEND_ACCEPT& stAccept, int& nResult) {
        nResult = 0;
        if (IsFriend(stAccept.dwTargetUCID, 1u)) {
            nResult = 55101;
        } else if (IsFriend(stAccept.dwTargetUCID, 2u)) {
            if (IsBlockList(stAccept.dwTargetUCID)) {
                nResult = 55105;
            } else if (!IsValiedListCount(1u)) {
                nResult = 55103;
            }
        } else {
            nResult = 55101;
        }
        return nResult == 0;
    }

    // 对齐 IDA 0x1400027B0: 黑名单添加检查
    // nResult: 0=可添加, 55101=已是好友(任何类型), 55105=已在黑名单中, 55106=黑名单已满
    // 对齐 IDA: QEAA_NPEA_WAEAH@Z = 非const方法，参数1为wchar_t*，参数2为int&
    bool CheckBlockAdd(wchar_t* strName, int& nResult) {
        nResult = 0;
        if (IsFriend(strName, 1u) || IsFriend(strName, 2u) || IsFriend(strName, 3u)) {
            nResult = 55101;
            return true;
        }
        if (IsBlockList(strName)) {
            nResult = 55105;
        } else if (!IsValiedListCount(0x65u)) {
            nResult = 55106;
        }
        return nResult == 0;
    }

    // 对齐 IDA 0x1400021B0: 按 UCID 获取好友信息
    // 对齐 IDA: QEAA_NKAEAU@Z = 非const方法
    bool GetFriendInfo(std::uint32_t dwFriendUCID, ST_FRIEND_INFO& stFriendInfo) {
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                                     [dwFriendUCID](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == dwFriendUCID; });
        if (it == m_vecFriend.end()) {
            return false;
        }
        stFriendInfo = (*it)->m_stFriendInfo;
        return true;
    }

    // 对齐 IDA 0x140002AC0: 增加好友积分
    std::int64_t AddFriendPoint(std::uint32_t dwUCID, int nPoint) {
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                               [dwUCID](const std::shared_ptr<CFriendMember>& m) { return m->GetUCID() == dwUCID; });
        if (it == m_vecFriend.end()) {
            return -1;
        }
        if ((*it)->m_stFriendInfo.nFriendPoint == -1) {
            return -1;
        }
        (*it)->m_stFriendInfo.nFriendPoint += nPoint;
        return (*it)->m_stFriendInfo.nFriendPoint;
    }

    // 对齐 IDA 0x140002A20: 检查招募列表冷却时间
    bool CheckRecruitListTime() {
        const auto now = std::chrono::steady_clock::now().time_since_epoch();
        const auto nowSec = static_cast<std::int64_t>(
            std::chrono::duration_cast<std::chrono::seconds>(now).count());
        if (m_tNextRecruitListTime > nowSec) {
            return false;
        }
        m_tNextRecruitListTime = nowSec + 10;  // 10 秒冷却
        return true;
    }

    // 对齐 IDA 0x140002A90: 初始化招募列表时间
    void InitRecruitListTime() {
        m_tNextRecruitListTime = 0;
    }

    // 对齐 IDA: 推荐索引
    int GetRecommandIndex() { return m_nRecommandIndex; }  // 对齐 IDA: 非const，返回 H (int)
    void SetRecommandIndex(int nIndex) { m_nRecommandIndex = nIndex; }

    // 对齐 IDA 0x140002890: 获取最旧等待好友（按 tRemain 排序）
    // 对齐 IDA: QEAA_NAEAK@Z = 非const方法
    bool GetLastFriendWaitList(std::uint32_t& dwUCID) {
        dwUCID = 0;
        bool bFound = false;
        const auto nowSec = static_cast<std::int64_t>(std::time(nullptr));
        const std::int64_t threshold = nowSec + 604800;  // 7 天阈值 (0x93A80)
        std::int64_t tOld = threshold;

        for (const auto& friendMember : m_vecFriend) {
            if (friendMember->GetType() != 4) {  // eFRIEND_TYPE_WAIT = 4
                continue;
            }
            if (tOld > friendMember->m_stFriendInfo.tRemain) {
                tOld = friendMember->m_stFriendInfo.tRemain;
                dwUCID = friendMember->GetUCID();
                bFound = true;
            }
        }
        return bFound;
    }

private:
    ST_FRIEND_COMMUNITY m_stCommunity{};
    std::vector<std::shared_ptr<CFriendMember>> m_vecFriend;
    std::vector<std::shared_ptr<CBlockUser>> m_vecBlockList;
    bool m_bLoadFriendList = false;
    bool m_bLoadBlockList = false;
    bool m_bSyncFriendList = false;
    bool m_bSyncBlockList = false;
    int m_nRecommandIndex = 0;
    std::int64_t m_tNextRecruitListTime = 0;
};

class CUserObject {
public:
    CUserObject() = default;
    CUserObject(CServer* pServer, const STCharInfo& charInfo, UXMapID uxMapID)
        : m_pServer(pServer),
          m_stCharInfo(charInfo),
          m_uxMapID(uxMapID) {}

    std::uint32_t GetCID() const { return m_stCharInfo.uxActorID.dwActorID; }
    std::uint32_t GetUAID() const { return m_stCharInfo.dwUAID; }
    std::wstring GetName() const { return FixedWideArrayToWString(m_stCharInfo.stBaseInfo.strName); }
    std::uint32_t GetServerID() const { return m_dwServerID; }
    std::uint32_t GetIP() const { return m_dwIP; }
    std::uint16_t GetMapID() const { return static_cast<std::uint16_t>(m_uxMapID.nMapID); }
    std::uint16_t GetChannel() const { return 0; }
    int GetHP() const { return 1; }  // IDA 显示非const，保持const以兼容调用者
    int GetMaxHP() const { return 1; }  // IDA 显示非const，保持const以兼容调用者
    std::uint8_t GetClass() const { return m_stCharInfo.stBaseInfo.byClass; }
    std::uint8_t GetLevel() const { return m_stCharInfo.byLevel; }
    std::uint8_t GetAwaken() const { return m_stCharInfo.stBaseInfo.byAwaken; }
    std::uint32_t GetProfilePhoto() const { return m_stCharInfo.stBaseInfo.dwProfilePhotoID; }
    std::uint8_t GetGMPower() const { return m_stCharInfo.byGMPower; }
    UXMapID GetMapIns() const { return m_uxMapID; }  // IDA 显示非const，但调用者从const方法使用，保持const以兼容

    // 对齐 IDA: 填充推荐好友信息 QEAAXAEAU...@Z = 非const方法
    void GetRecommandInfo(ST_RECOMMAND_FRIEND_INFO& stInfo) {
        stInfo = {};
        const std::wstring name = GetName();
        std::wcsncpy(stInfo.strName, name.c_str(), 20);
        stInfo.strName[20] = L'\0';
        stInfo.dwID = GetCID();
        stInfo.byLevel = GetLevel();
        stInfo.byClass = GetClass();
        stInfo.byAwaken = GetAwaken();
        stInfo.dwProfilePhotoID = GetProfilePhoto();
        stInfo.wMapID = GetMapID();
        stInfo.byChannel = GetChannel();
        stInfo.bLogin = true;
    }
    // 对齐 IDA: QEAAXPEAUST_PARTY_MEMBER@@@Z = 非const方法，参数为指针
    void GetPartyMemberInfo(ST_PARTY_MEMBER* pMemberInfo) {
        if (!pMemberInfo) return;
        std::memset(pMemberInfo, 0, sizeof(ST_PARTY_MEMBER));
        pMemberInfo->dwMemberID = GetCID();
        pMemberInfo->nMapID = GetMapID();
        pMemberInfo->byClass = GetClass();
        pMemberInfo->byAwaken = GetAwaken();
        pMemberInfo->dwProfilePhotoID = GetProfilePhoto();
        pMemberInfo->byLevel = GetLevel();
        const std::wstring name = GetName();
        const std::size_t copyLen = std::min<std::size_t>(name.size(), std::size(pMemberInfo->strName) - 1);
        std::wmemcpy(pMemberInfo->strName, name.c_str(), copyLen);
        pMemberInfo->strName[copyLen] = L'\0';
        pMemberInfo->nChannel = GetChannel();
        pMemberInfo->nHP = 1;
        pMemberInfo->nMaxHP = 1;
        pMemberInfo->bLogin = true;
        pMemberInfo->uxMapID = GetMapIns();
    }

    // 对齐 IDA 0x1400d5210: QEAAXAEAU...@Z = 非const方法
    void GetLeagueMemberInfo(ST_LEAGUE_MEMBER_EX& stMemberInfo) {
        const std::wstring name = GetName();
        wcscpy_s(stMemberInfo.szName, name.c_str());
        stMemberInfo.dwUCID = GetCID();
        stMemberInfo.shLevel = static_cast<std::int16_t>(GetLevel());
        stMemberInfo.byClass = GetClass();
        stMemberInfo.byAwaken = GetAwaken();
        stMemberInfo.dwProfilePhotoID = GetProfilePhoto();
        stMemberInfo.byChannel = GetChannel();
        stMemberInfo.sWorldID = static_cast<std::int16_t>(GetMapID());
        stMemberInfo.bLogin = true;
    }

    // 对齐 IDA: QEAAXPEAVCServer@@@Z = 非const方法，参数为指针
    // IDA 实现: 设置 m_pServer 并从 CServer::GetServerID() 设置 m_dwServerID
    void SetServer(CServer* pServer) {
        m_pServer = pServer;
        m_dwServerID = pServer ? pServer->GetServerID() : 0u;
    }

    // 辅助方法：批量更新同步信息（IDA 中不存在此方法）
    void GreenDamTan_UpdateFromSync(CServer* pServer,
                        const STCharInfo& charInfo,
                        UXMapID uxMapID) {
        SetServer(pServer);  // 对齐 IDA: SetServer 同时设置 m_dwServerID
        m_stCharInfo = charInfo;
        m_uxMapID = uxMapID;
    }

    // 对齐 IDA 0x1400D36C0: 换地图时更新好友列表中的地图信息
    void ChangeMap(std::uint16_t wMapID);  // 对齐 IDA: 参数 G (unsigned short = uint16_t)
    void SetMapIns(UXMapID uxMapID) { m_uxMapID = uxMapID; }
    void SendFriendServerLoad() { SendFriendServerLoadImpl(); }
    void SendFriendServerLoadImpl();  // 对齐 IDA 0x1400D4B40 实际实现
    void SetLevel(std::uint8_t byLevel) { m_stCharInfo.byLevel = byLevel; }
    void SetAwaken(std::uint8_t byAwaken) { m_stCharInfo.stBaseInfo.byAwaken = byAwaken; }
    void SetProfilePhoto(std::uint32_t dwProfilePhotoID) {
        m_stCharInfo.stBaseInfo.dwProfilePhotoID = dwProfilePhotoID;
    }
    void SetName(const wchar_t* szName) {
        if (szName) {
#ifdef _WIN32
            wcscpy_s(m_stCharInfo.stBaseInfo.strName, szName);
#else
            std::wcsncpy(m_stCharInfo.stBaseInfo.strName, szName, 20);
            m_stCharInfo.stBaseInfo.strName[20] = L'\0';
#endif
        }
    }

    void SetLeagueID(int nLeagueID) { m_stCharInfo.stLeagueInfo.nLeagueID = nLeagueID; }
    std::int32_t GetLeagueID() const { return m_stCharInfo.stLeagueInfo.nLeagueID; }  // IDA 显示非const，但调用者从const方法使用，保持const以兼容
    void SetLockLeague(bool bLock) { m_bLockLeague = bLock; }  // 对齐 IDA: 参数 _N (bool)
    bool IsLockLeague() const { return m_bLockLeague; }  // IDA 显示非const，保持const以兼容调用者

    void SetGameOption(ST_GAME_OPTION& stGameOption) { m_stGameOption = stGameOption; }  // 对齐 IDA: 非const引用 AEAU
    void GreenDamTan_SetGameOption(const ST_GAME_OPTION& stGameOption) { m_stGameOption = stGameOption; }  // const引用版本供调用者使用
    void SetGameOption(ST_OPTION_BIT stOptionBit) {  // 对齐 IDA: 按值传递 U
        ST_GAME_OPTION gameOption{};
        gameOption.nOption_WhisperMsg = static_cast<unsigned char>(stOptionBit.szOption[0]);
        gameOption.nOption_Register_Friend = static_cast<unsigned char>(stOptionBit.szOption[1]);
        gameOption.nOption_OtherInfo = static_cast<unsigned char>(stOptionBit.szOption[2]);
        m_stGameOption = gameOption;
    }

    // 对齐 IDA: QEAA_NW4E_OPTION_INDEX@@W4E_OPTION_STATE@@@Z = 非const方法
    bool CheckGameOption(E_OPTION_INDEX eIndex, E_OPTION_STATE eState) {
        int nState = 0;
        switch (eIndex) {
        case eOption_OtherInfo:
            nState = m_stGameOption.nOption_OtherInfo;
            break;
        case eOption_Register_Friend:
            nState = m_stGameOption.nOption_Register_Friend;
            if (nState == 1) {
                nState = 0;
                eState = eGAME_OPTION_ALLOW_ALL;
            }
            break;
        case eOption_WhisperMsg:
            nState = m_stGameOption.nOption_WhisperMsg;
            break;
        default:
            break;
        }
        return nState == eState;
    }

    // 对齐 IDA: QEAAKPEA_W@Z = 非const方法
    std::uint32_t GetFriendUCID(wchar_t* pName) {
        return m_Community.GetFriendUCID(pName);
    }

    // 对齐 IDA: 获取社区状态（委托到 CCommunity） QEAAEXZ = 非const方法
    std::uint8_t GetCommunityState() { return m_Community.GetCommunityState(); }
    // 对齐 IDA: QEAA?AV...@Z = 非const方法
    std::wstring GetMemo() { return m_Community.GetMemo(); }

    // 对齐 IDA: QEAAXAEAUPS_FRIEND_LIST@@E@Z = 非const方法
    void GetFriendList(PS_FRIEND_LIST& stFriendList, std::uint8_t byType) {
        m_Community.GetFriendList(stFriendList, byType);
    }

    // 对齐 IDA 0x1400D4A80: 获取好友等级
    // 对齐 IDA: 非const（因为CCommunity::GetFriendInfo是非const）
    std::uint8_t GetFriendLevel(std::uint32_t dwFriend) {
        ST_FRIEND_INFO stFriendInfo{};
        if (m_Community.GetFriendInfo(dwFriend, stFriendInfo)) {
            return stFriendInfo.byLevel;
        }
        return 0;
    }

    // 对齐 IDA: QEAAXAEAUPS_BLOCKLIST_INFO@@@Z = 非const方法
    void GetBlcokList(PS_BLOCKLIST_INFO& stBlockList) {
        m_Community.GetBlcokList(stBlockList);
    }

    void SetLoadFriendList(bool bLoad) {
        m_Community.SetLoadFriendList(bLoad);
    }

    void SetLoadBlockList(bool bLoad) {
        m_Community.SetLoadBlockList(bLoad);
    }

    // 对齐 IDA: QEAA_NXZ = 非const方法
    bool GetLoadFriendList() {
        return m_Community.GetLoadFriendList();
    }

    bool GetLoadBlockList() {
        return m_Community.GetLoadBlockList();
    }

    void SetSyncFriendList(bool bSync) {
        m_Community.SetSyncFriendList(bSync);
    }

    void SetSyncBlockList(bool bSync) {
        m_Community.SetSyncBlockList(bSync);
    }

    // 对齐 IDA: QEAA_NPEA_WE@Z = 非const方法
    bool IsFriendList(wchar_t* pName, std::uint8_t byType) {
        return m_Community.IsFriend(pName, byType);
    }

    // 对齐 IDA: QEAA_NKE@Z = 非const方法
    bool IsFriendList(std::uint32_t dwUCID, std::uint8_t byType) {
        return m_Community.IsFriend(dwUCID, byType);
    }

    // 对齐 IDA: QEAA_NPEA_W@Z = 非const方法
    bool IsBlockList(wchar_t* pName) {
        return m_Community.IsBlockList(pName);
    }

    // 对齐 IDA: QEAA_NK@Z = 非const方法
    bool IsBlockList(std::uint32_t dwUCID) {
        return m_Community.IsBlockList(dwUCID);
    }

    // 对齐 IDA: 社区状态/备忘录设置
    void SetCommunityState(std::uint8_t byState) { m_Community.SetCommunityState(byState); }
    // 对齐 IDA: 非const wchar_t* 参数
    void SetMemo(wchar_t* strMemo) { m_Community.SetMemo(strMemo); }
    // 对齐 IDA 0x1400D2FB0: QEAAXAEAUST_FRIEND_COMMUNITY@@@Z = 非const方法
    // 检查社区状态/签名是否变化，变化则更新并通知好友
    void UpdateCharCommunity(ST_FRIEND_COMMUNITY& stCommunity) {
        // 对齐 IDA: 获取当前 memo 和 state，比较是否有变化
        const std::wstring currentMemo = m_Community.GetMemo();
        const bool bChanged = (currentMemo.compare(stCommunity.strMemo) != 0) ||
                              (m_Community.GetCommunityState() != stCommunity.byState);
        if (bChanged) {
            m_Community.SetCharCommunity(stCommunity);
            SendUpdateCommunity();
        }
    }

    // 对齐 IDA: 好友/黑名单加载方法
    void SetLoadFriend() { m_bLoadFriend = true; }
    // 对齐 IDA 0x1400D2D30: QEAA_NAEAUST_BLOCK_INFO@@@Z = 非const方法，返回bool，参数为非const引用
    bool LoadBlock(ST_BLOCK_INFO& stBlockInfo);
    void SendBlockList() { SendBlockListImpl(); }
    void SendBlockListImpl();  // 对齐 IDA 0x1400D4D50 实际实现
    // 对齐 IDA: QEAA_NXZ = 非const方法
    bool GetSyncBlockList() { return m_Community.GetSyncBlockList(); }
    bool GetSyncFriendList() { return m_Community.GetSyncFriendList(); }
    void SendFriendList() { SendFriendListImpl(); }
    void SendFriendListImpl();  // 对齐 IDA 0x1400D4BF0 实际实现
    // 对齐 IDA: QEAAXAEAUST_FRIEND_INFO@@@Z = 非const方法，参数为非const引用
    void GetUserInfo(ST_FRIEND_INFO& stInfo) {
        std::memset(&stInfo, 0, sizeof(ST_FRIEND_INFO));
        const std::wstring name = GetName();
        const std::size_t copyLen = std::min<std::size_t>(name.size(), std::size(stInfo.strName) - 1);
        std::wmemcpy(stInfo.strName, name.c_str(), copyLen);
        stInfo.strName[copyLen] = L'\0';
        stInfo.dwID = GetCID();
        stInfo.byLevel = GetLevel();
        stInfo.byClass = GetClass();
        stInfo.byAwaken = GetAwaken();
        stInfo.dwProfilePhotoID = GetProfilePhoto();
        stInfo.bLogin = true;
        stInfo.byChannel = GetChannel();
        stInfo.wMapID = GetMapID();
    }

    // 对齐 IDA: QEAA_NAEAUST_FRIEND_INFO@@V?$shared_ptr@VCUserObject@@@tr1@std@@@Z = 非const引用 + 按值传递shared_ptr，返回bool
    bool AddFriend(ST_FRIEND_INFO& stInfo, std::shared_ptr<CUserObject> pFriend) {
        auto pFriendMember = std::make_shared<CFriendMember>();
        pFriendMember->m_stFriendInfo = stInfo;
        pFriendMember->m_pFriend = pFriend;
        return m_Community.AddFriend(std::move(pFriendMember));
    }
    void DeleteFriend(std::uint32_t dwUCID) {
        m_Community.DeleteFriend(dwUCID);
    }
    // 对齐 IDA 0x1400D41E0: ?UpdateFriend@CUserObject@@QEAAXAEAUST_FRIEND_INFO@@_N@Z
    // 参数2: bSend=true 时发送好友更新包给客户端
    void UpdateFriend(ST_FRIEND_INFO& stInfo, bool bSend) {
        // 对齐 IDA: 先更新社区中的好友信息
        m_Community.UpdateFriendInfo(stInfo, nullptr);
        // 对齐 IDA: 如果需要发送，获取好友信息并发包
        if (bSend) {
            ST_FRIEND_INFO stFriend{};
            if (m_Community.GetFriendInfo(stInfo.dwID, stFriend)) {
                XSendPacket xSendPacket(0xF5, 0x20);
                xSendPacket.XParse << GetMatchingID();
                xSendPacket << stInfo;
                SendPacket(xSendPacket);
            }
        }
    }
    // 对齐 IDA: QEAA_NAEAUST_BLOCK_INFO@@@Z = 返回 bool，参数为非const引用
    bool AddBlockList(ST_BLOCK_INFO& stBlock) {
        return m_Community.AddBlockList(stBlock);
    }
    void DeleteBlockList(std::uint32_t dwUCID) {
        m_Community.DeleteBlockList(dwUCID);
    }

    void SetLeagueWithdrawPenalty(std::int64_t biPenalty) { m_biLeagueWithdrawPenalty = biPenalty; }
    void SetLeagueDeletePenalty(std::int64_t biPenalty) { m_biLeagueDeletePenalty = biPenalty; }
    std::int64_t GetLeagueWithdrawPenalty() const { return m_biLeagueWithdrawPenalty; }  // IDA 显示非const，保持const以兼容调用者
    std::int64_t GetLeagueDeletePenalty() const { return m_biLeagueDeletePenalty; }

    // 对齐 IDA: GetMatchingID 返回 ActorID 作为匹配 ID
    std::uint32_t GetMatchingID() const { return GetCID(); }

    // 对齐 IDA: 好友列表容量检查（委托到 CCommunity）QEAA_NE@Z = 非const方法
    bool IsValiedListCount(std::uint8_t byType) {
        return m_Community.IsValiedListCount(byType);
    }
    bool IsValiedFriendListCount(std::uint8_t byType) {
        return m_Community.IsValiedListCount(byType);
    }

    // 对齐 IDA: 好友邀请检查（委托到 CCommunity）QEAA_NAEAU@Z = 非const方法
    bool CheckFriendInvite(PS_RES_FRIEND_INVITE& stInvite) {
        return m_Community.CheckFriendInvite(stInvite);
    }

    // 对齐 IDA: 好友接受检查（委托到 CCommunity）QEAA_NAEAU...AEAH@Z = 非const方法
    bool CheckFriendAccept(PS_REQ_FRIEND_ACCEPT& stAccept, int& nResult) {
        return m_Community.CheckFriendAccept(stAccept, nResult);
    }

    // 对齐 IDA: 黑名单添加检查（委托到 CCommunity）QEAA_NPEA_WAEAH@Z = 非const方法
    bool CheckBlockAdd(wchar_t* strName, int& nResult) {
        return m_Community.CheckBlockAdd(strName, nResult);
    }

    // 对齐 IDA: 按 UCID 获取好友信息（委托到 CCommunity）QEAA_NKAEAU@Z = 非const方法
    bool GetFriendInfo(std::uint32_t dwFriendUCID, ST_FRIEND_INFO& stFriendInfo) {
        return m_Community.GetFriendInfo(dwFriendUCID, stFriendInfo);
    }

    // 对齐 IDA: 增加好友积分（委托到 CCommunity）
    std::int64_t AddFriendPoint(std::uint32_t dwUCID, int nPoint) {
        return m_Community.AddFriendPoint(dwUCID, nPoint);
    }

    // 对齐 IDA: 检查招募列表冷却时间（委托到 CCommunity）
    bool CheckRecruitListTime() {
        return m_Community.CheckRecruitListTime();
    }

    // 对齐 IDA: 初始化招募列表时间（委托到 CCommunity）
    void InitRecruitListTime() {
        m_Community.InitRecruitListTime();
    }

    // 对齐 IDA 0x1400B8480: 获取用户招募信息
    // 对齐 IDA: QEAAXAEAUST_RECRUIT_INFO@@@Z = 非const方法，参数为非const引用
    void GetUserRecruitInfo(ST_RECRUIT_INFO& stInfo) {
        std::memset(&stInfo, 0, sizeof(ST_RECRUIT_INFO));
        const std::wstring name = GetName();
        std::wcsncpy(stInfo.strName, name.c_str(), 20);
        stInfo.strName[20] = L'\0';
        stInfo.dwID = GetMatchingID();
        stInfo.byLevel = GetLevel();
        stInfo.byClass = GetClass();
        stInfo.byAwaken = GetAwaken();
        stInfo.dwProfilePhotoID = GetProfilePhoto();
        stInfo.byState = GetCommunityState();
        const std::wstring memo = GetMemo();
        std::wcsncpy(stInfo.strMemo, memo.c_str(), 30);
        stInfo.strMemo[30] = L'\0';
        stInfo.byChannel = static_cast<std::uint8_t>(GetChannel());
        stInfo.wMapID = GetMapID();
        stInfo.bLogin = true;
    }

    // 对齐 IDA: 推荐索引（委托到 CCommunity）
    std::uint32_t GetRecommandIndex() { return static_cast<std::uint32_t>(m_Community.GetRecommandIndex()); }  // 对齐 IDA: 非const，返回 K (uint32_t)
    void SetRecommandIndex(std::uint32_t nIndex) { m_Community.SetRecommandIndex(static_cast<int>(nIndex)); }  // 对齐 IDA: 参数 K (uint32_t)

    // 对齐 IDA: 获取最旧等待好友（委托到 CCommunity）QEAA_NAEAK@Z = 非const方法
    bool GetLastFriendWaitList(std::uint32_t& dwUCID) {
        return m_Community.GetLastFriendWaitList(dwUCID);
    }

    // 对齐 IDA: QEAA_NXZ = 非const方法
    bool IsMaze() {
        // Primary check: map IDs in range 20000-29999 are maze maps
        // Full implementation would also check XResourceMgr::GetTB_MAZE_INFO(MapID)->Maze_Type != 6
        // But RelayServer doesn't load maze info table, so we use the range check only
        const std::uint16_t wMapID = GetMapID();
        return (wMapID / 10000) == 2;
    }

    // 对齐 IDA 0x1400D27E0: 加载好友信息
    // 对齐 IDA: QEAA_NAEAUPS_DB_FRIEND@@V...AEAUST_FRIEND_INFO@@@Z = 非const方法，参数1为引用，参数2为shared_ptr按值，参数3为引用
    bool LoadFriend(PS_DB_FRIEND& stDbFriend, std::shared_ptr<CUserObject> pFriendUser, ST_FRIEND_INFO& stFriendRes);

    // 对齐 IDA 0x1400D30E0: 好友上线通知
    // IDA: QEAAXAEAUST_FRIEND_INFO@@V?$shared_ptr@VCUserObject@@@tr1@std@@@Z = 非const引用 + 按值传递shared_ptr
    void LoginFriend(ST_FRIEND_INFO& stMyInfo, std::shared_ptr<CUserObject> pMyUser);

    // 对齐 IDA 0x1400D3AF0: 升级通知好友
    // IDA: QEAAXE@Z = void(uint8_t)
    void Levelup(std::uint8_t byLevel);

    // 对齐 IDA 0x1400D3EC0: 更新头像通知好友
    // IDA: QEAAXK@Z = void(uint32_t)
    void UpdateProfilePhoto(std::uint32_t dwPhotoID);

    // 对齐 IDA 0x1400D4EA0: 发送社区状态更新给好友
    // IDA: QEAAXXZ = void()
    void SendUpdateCommunity();

    // 对齐 IDA 0x1400D5310: 更名通知好友列表
    // IDA: QEAAXUPS_CHANGE_NAME@@@Z = void(PS_CHANGE_NAME by value)
    void ChangeFriendName(PS_CHANGE_NAME stChangeName);

    // 对齐 IDA 0x1400D3270: 登出处理
    void Logout();  // 对齐 IDA: void()

    void SendPacket(XSendPacket& xPacket) {
        if (m_pServer) {
            m_pServer->SendEx(xPacket);
        }
    }

    // 对齐 IDA: 设置登录 tick
    void SetConnectTick(std::uint64_t tick) { m_dwConnectTick = tick; }
    std::uint64_t GetConnectTick() const { return m_dwConnectTick; }

private:
    std::uint64_t m_dwConnectTick = 0;   // 对齐 IDA: space.data_.align_.t4 - 登录 tick (GetTickCount64)
    CServer* m_pServer = nullptr;
    std::uint32_t m_dwIP = 0;
    STCharInfo m_stCharInfo{};
    UXMapID m_uxMapID{};
    std::uint32_t m_dwServerID = 0;
    bool m_bLoadFriend = false;
    std::int64_t m_biLeagueWithdrawPenalty = 0;
    std::int64_t m_biLeagueDeletePenalty = 0;
    bool m_bLockLeague = false;
    std::uint8_t m_byTradePasswordState = 0;
    ST_GAME_OPTION m_stGameOption{};
    CCommunity m_Community{};
};
