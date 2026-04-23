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
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

struct ST_FRIEND_COMMUNITY {
    std::uint8_t byState = 0;
    wchar_t strMemo[31] = {};
};

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

    bool IsFriend(std::uint32_t dwUCID, std::uint8_t byType) const {
        if (!IsValidCommunityType(byType)) {
            return false;
        }

        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(), [dwUCID](const CFriendMember& friendMember) {
            return friendMember.GetUCID() == dwUCID;
        });
        return it != m_vecFriend.end() && it->GetType() == byType;
    }

    bool IsFriend(const wchar_t* pName, std::uint8_t byType) const {
        if (!pName || !IsValidCommunityType(byType)) {
            return false;
        }

        const std::wstring targetName(pName);
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(), [&targetName](const CFriendMember& friendMember) {
            return friendMember.GetName() == targetName;
        });
        return it != m_vecFriend.end() && it->GetType() == byType;
    }

    bool IsBlockList(std::uint32_t dwUCID) const {
        return std::any_of(m_vecBlockList.begin(), m_vecBlockList.end(), [dwUCID](const CBlockUser& blockUser) {
            return blockUser.GetUCID() == dwUCID;
        });
    }

    bool IsBlockList(const wchar_t* pName) const {
        if (!pName) {
            return false;
        }

        const std::wstring targetName(pName);
        return std::any_of(m_vecBlockList.begin(), m_vecBlockList.end(), [&targetName](const CBlockUser& blockUser) {
            return blockUser.GetName() == targetName;
        });
    }

    std::uint32_t GetFriendUCID(const wchar_t* pName) const {
        if (!pName) {
            return 0;
        }

        const std::wstring targetName(pName);
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(), [&targetName](const CFriendMember& friendMember) {
            return friendMember.GetName() == targetName;
        });
        return it == m_vecFriend.end() ? 0u : it->GetUCID();
    }

    void GetFriendList(PS_FRIEND_LIST& stFriendList, std::uint8_t byType) const {
        stFriendList.vecFriends.clear();
        for (const CFriendMember& friendMember : m_vecFriend) {
            if (byType != 0 && friendMember.GetType() != byType) {
                continue;
            }
            stFriendList.vecFriends.push_back(friendMember.m_stFriendInfo);
        }
    }

    void GetBlcokList(PS_BLOCKLIST_INFO& stBlockList) const {
        stBlockList.vecBlockList.clear();
        for (const CBlockUser& blockUser : m_vecBlockList) {
            stBlockList.vecBlockList.push_back(blockUser.m_stBlockInfo);
        }
    }

    void SetLoadFriendList(bool bLoad) { m_bLoadFriendList = bLoad; }
    void SetLoadBlockList(bool bLoad) { m_bLoadBlockList = bLoad; }
    bool GetLoadFriendList() const { return m_bLoadFriendList; }
    bool GetLoadBlockList() const { return m_bLoadBlockList; }
    void SetSyncFriendList(bool bSync) { m_bSyncFriendList = bSync; }
    void SetSyncBlockList(bool bSync) { m_bSyncBlockList = bSync; }

    // 对齐 IDA: 好友/黑名单加载方法
    void AddBlockUser(const CBlockUser& blockUser) { m_vecBlockList.push_back(blockUser); }
    bool GetSyncBlockList() const { return m_bSyncBlockList; }
    bool GetSyncFriendList() const { return m_bSyncFriendList; }

    // 对齐 IDA: 获取好友类型（1=好友, 2=推荐, 3=招募）, 0=不存在
    std::uint8_t GetFriendType(std::uint32_t dwUCID) const {
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                                     [dwUCID](const CFriendMember& m) { return m.GetUCID() == dwUCID; });
        return it != m_vecFriend.end() ? it->GetType() : 0;
    }

    // 对齐 IDA: 检查好友信息是否发生变化
    bool IsChangeFriendInfo(const ST_FRIEND_INFO* pInfo) const {
        if (!pInfo) return false;
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                                     [pInfo](const CFriendMember& m) { return m.GetUCID() == pInfo->dwID; });
        if (it == m_vecFriend.end()) return false;
        const auto& cur = it->m_stFriendInfo;
        return cur.byLevel != pInfo->byLevel ||
               cur.byAwaken != pInfo->byAwaken ||
               cur.dwProfilePhotoID != pInfo->dwProfilePhotoID ||
               cur.byChannel != pInfo->byChannel ||
               cur.wMapID != pInfo->wMapID ||
               cur.bLogin != pInfo->bLogin ||
               cur.byState != pInfo->byState;
    }

    // 对齐 IDA: 更新好友信息（带在线 shared_ptr）
    void UpdateFriendInfo(const ST_FRIEND_INFO* pInfo, const std::shared_ptr<CUserObject>& pFriend) {
        if (!pInfo) return;
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                               [pInfo](const CFriendMember& m) { return m.GetUCID() == pInfo->dwID; });
        if (it != m_vecFriend.end()) {
            it->m_stFriendInfo = *pInfo;
            it->m_pFriend = pFriend;
        }
    }

    // 对齐 IDA: 添加好友（在线好友带 shared_ptr），返回是否成功
    bool AddFriend(const ST_FRIEND_INFO* pInfo, const std::shared_ptr<CUserObject>& pFriend) {
        if (!pInfo) return false;
        CFriendMember member;
        member.m_stFriendInfo = *pInfo;
        member.m_pFriend = pFriend;
        m_vecFriend.push_back(member);
        return true;
    }

    // 对齐 IDA: 删除好友（按 UCID）
    void DeleteFriend(std::uint32_t dwUCID) {
        m_vecFriend.erase(
            std::remove_if(m_vecFriend.begin(), m_vecFriend.end(),
                           [dwUCID](const CFriendMember& m) { return m.GetUCID() == dwUCID; }),
            m_vecFriend.end());
    }

    // 对齐 IDA: 更新好友信息（bAddIfNotExists=1: 不存在则添加; =0: 仅更新）
    void UpdateFriend(const ST_FRIEND_INFO* pInfo, int bAddIfNotExists) {
        if (!pInfo) return;
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                               [pInfo](const CFriendMember& m) { return m.GetUCID() == pInfo->dwID; });
        if (it != m_vecFriend.end()) {
            it->m_stFriendInfo = *pInfo;
        } else if (bAddIfNotExists) {
            CFriendMember member;
            member.m_stFriendInfo = *pInfo;
            m_vecFriend.push_back(member);
        }
    }

    // 对齐 IDA: 添加黑名单
    void AddBlockList(const ST_BLOCK_INFO* pBlock) {
        if (!pBlock) return;
        CBlockUser blockUser;
        blockUser.m_stBlockInfo = *pBlock;
        m_vecBlockList.push_back(blockUser);
    }

    // 对齐 IDA: 删除黑名单（按 UCID）
    void DeleteBlockList(std::uint32_t dwUCID) {
        m_vecBlockList.erase(
            std::remove_if(m_vecBlockList.begin(), m_vecBlockList.end(),
                           [dwUCID](const CBlockUser& u) { return u.GetUCID() == dwUCID; }),
            m_vecBlockList.end());
    }

    // 对齐 IDA: 社区状态/备忘录设置
    void SetCommunityState(std::uint8_t byState) { m_stCommunity.byState = byState; }
    void SetMemo(const wchar_t* strMemo) {
        if (strMemo) {
#ifdef _WIN32
            wcscpy_s(m_stCommunity.strMemo, strMemo);
#else
            std::wcsncpy(m_stCommunity.strMemo, strMemo, 30);
            m_stCommunity.strMemo[30] = L'\0';
#endif
        }
    }
    void UpdateCharCommunity(const ST_FRIEND_COMMUNITY* pCommunity) {
        if (pCommunity) {
            m_stCommunity = *pCommunity;
        }
    }
    std::uint8_t GetCommunityState() const { return m_stCommunity.byState; }
    std::wstring GetMemo() const { return FixedWideArrayToWString(m_stCommunity.strMemo); }

    // 对齐 IDA: 检查好友类型是否有效（1=好友, 2=推荐, 3=招募）
    bool IsValidCommunityType(std::uint8_t byType) const {
        return byType >= 1 && byType <= 3;
    }

    // 对齐 IDA 0x1400011C0: 好友列表容量检查
    // type 1 (好友): 上限 100, type 2 (推荐好友): 上限 20, type 3 (招募): 无限制
    // type 101 (黑名单): 上限 50
    bool IsValiedListCount(std::uint8_t byType) const {
        switch (byType) {
        case 1: {
            std::size_t count = 0;
            for (const auto& f : m_vecFriend) {
                if (f.GetType() == 1) ++count;
            }
            return count < 100;
        }
        case 2: {
            std::size_t count = 0;
            for (const auto& f : m_vecFriend) {
                if (f.GetType() == 2) ++count;
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
    bool CheckFriendInvite(PS_RES_FRIEND_INVITE& stInvite) const {
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
    bool CheckFriendAccept(PS_REQ_FRIEND_ACCEPT& stAccept, int& nResult) const {
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
    bool CheckBlockAdd(const wchar_t* strName, int* pResult) const {
        if (!strName || !pResult) return false;
        *pResult = 0;
        if (IsFriend(strName, 1u) || IsFriend(strName, 2u) || IsFriend(strName, 3u)) {
            *pResult = 55101;
            return true;
        }
        if (IsBlockList(strName)) {
            *pResult = 55105;
        } else if (!IsValiedListCount(0x65u)) {
            *pResult = 55106;
        }
        return *pResult == 0;
    }

    // 对齐 IDA 0x1400021B0: 按 UCID 获取好友信息
    bool GetFriendInfo(std::uint32_t dwFriendUCID, ST_FRIEND_INFO& stFriendInfo) const {
        const auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                                     [dwFriendUCID](const CFriendMember& m) { return m.GetUCID() == dwFriendUCID; });
        if (it == m_vecFriend.end()) {
            return false;
        }
        stFriendInfo = it->m_stFriendInfo;
        return true;
    }

    // 对齐 IDA 0x140002AC0: 增加好友积分
    std::int64_t AddFriendPoint(std::uint32_t dwUCID, int nPoint) {
        auto it = std::find_if(m_vecFriend.begin(), m_vecFriend.end(),
                               [dwUCID](const CFriendMember& m) { return m.GetUCID() == dwUCID; });
        if (it == m_vecFriend.end()) {
            return -1;
        }
        if (it->m_stFriendInfo.nFriendPoint == -1) {
            return -1;
        }
        it->m_stFriendInfo.nFriendPoint += nPoint;
        return it->m_stFriendInfo.nFriendPoint;
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
    int GetRecommandIndex() const { return m_nRecommandIndex; }
    void SetRecommandIndex(int nIndex) { m_nRecommandIndex = nIndex; }

    // 对齐 IDA 0x140002890: 获取最旧等待好友（按 tRemain 排序）
    bool GetLastFriendWaitList(std::uint32_t& dwUCID) const {
        dwUCID = 0;
        bool bFound = false;
        const auto nowSec = static_cast<std::int64_t>(std::time(nullptr));
        const std::int64_t threshold = nowSec + 604800;  // 7 天阈值 (0x93A80)
        std::int64_t tOld = threshold;

        for (const auto& friendMember : m_vecFriend) {
            if (friendMember.GetType() != 4) {  // eFRIEND_TYPE_WAIT = 4
                continue;
            }
            if (tOld > friendMember.m_stFriendInfo.tRemain) {
                tOld = friendMember.m_stFriendInfo.tRemain;
                dwUCID = friendMember.GetUCID();
                bFound = true;
            }
        }
        return bFound;
    }

    // 对齐 IDA: 获取在线好友用户对象（用于 ChangeMap 等）
    void ForEachOnlineFriend(std::uint8_t byType, std::function<void(std::shared_ptr<CUserObject>&)> callback) {
        for (auto& member : m_vecFriend) {
            if ((byType == 0 || member.GetType() == byType) && member.m_pFriend) {
                callback(member.m_pFriend);
            }
        }
    }

private:
    ST_FRIEND_COMMUNITY m_stCommunity{};
    std::vector<CFriendMember> m_vecFriend;
    std::vector<CBlockUser> m_vecBlockList;
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
    int GetHP() const { return 1; }
    int GetMaxHP() const { return 1; }
    std::uint8_t GetClass() const { return m_stCharInfo.stBaseInfo.byClass; }
    std::uint8_t GetLevel() const { return m_stCharInfo.byLevel; }
    std::uint8_t GetAwaken() const { return m_stCharInfo.stBaseInfo.byAwaken; }
    std::uint32_t GetProfilePhoto() const { return m_stCharInfo.stBaseInfo.dwProfilePhotoID; }
    std::uint8_t GetGMPower() const { return m_stCharInfo.byGMPower; }
    UXMapID GetMapIns() const { return m_uxMapID; }

    // 对齐 IDA: 填充推荐好友信息
    void GetRecommandInfo(ST_RECOMMAND_FRIEND_INFO& stInfo) const {
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
    void GetPartyMemberInfo(ST_PARTY_MEMBER& stMemberInfo) const {
        stMemberInfo = {};
        stMemberInfo.dwMemberID = GetCID();
        stMemberInfo.nMapID = GetMapID();
        stMemberInfo.byClass = GetClass();
        stMemberInfo.byAwaken = GetAwaken();
        stMemberInfo.dwProfilePhotoID = GetProfilePhoto();
        stMemberInfo.byLevel = GetLevel();
        const std::wstring name = GetName();
        const std::size_t copyLen = std::min<std::size_t>(name.size(), std::size(stMemberInfo.strName) - 1);
        std::wmemcpy(stMemberInfo.strName, name.c_str(), copyLen);
        stMemberInfo.strName[copyLen] = L'\0';
        stMemberInfo.nChannel = GetChannel();
        stMemberInfo.nHP = 1;
        stMemberInfo.nMaxHP = 1;
        stMemberInfo.bLogin = true;
        stMemberInfo.uxMapID = GetMapIns();
    }

    // 对齐 IDA 0x1400d5210
    void GetLeagueMemberInfo(ST_LEAGUE_MEMBER_EX& stMemberInfo) const {
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

    void SetServer(CServer* pServer, std::uint32_t dwServerID) {
        m_pServer = pServer;
        m_dwServerID = dwServerID;
    }

    void SetServer(CServer* pServer) {
        m_pServer = pServer;
    }

    void UpdateFromSync(CServer* pServer,
                        std::uint32_t dwServerID,
                        const STCharInfo& charInfo,
                        UXMapID uxMapID) {
        m_pServer = pServer;
        m_dwServerID = dwServerID;
        m_stCharInfo = charInfo;
        m_uxMapID = uxMapID;
    }

    // 对齐 IDA 0x1400D36C0: 换地图时更新好友列表中的地图信息
    void ChangeMap(std::uint16_t wMapID);
    void SetMapIns(UXMapID uxMapID) { m_uxMapID = uxMapID; }
    void SendFriendServerLoad() { SendFriendServerLoadImpl(); }
    void SendFriendServerLoadImpl();  // 对齐 IDA 0x1400D4B40 实际实现
    void Logout() {}
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
    std::int32_t GetLeagueID() const { return m_stCharInfo.stLeagueInfo.nLeagueID; }
    void SetLockLeague(std::uint8_t byLock) { m_bLockLeague = (byLock != 0); }
    bool IsLockLeague() const { return m_bLockLeague; }

    void SetGameOption(const ST_GAME_OPTION& stGameOption) { m_stGameOption = stGameOption; }
    void SetGameOption(const ST_GAME_OPTION* pGameOption) {
        if (pGameOption) {
            m_stGameOption = *pGameOption;
        }
    }
    void SetGameOption(const ST_OPTION_BIT* pOptionBit) {
        if (!pOptionBit) {
            return;
        }

        ST_GAME_OPTION gameOption{};
        gameOption.nOption_WhisperMsg = static_cast<unsigned char>(pOptionBit->szOption[0]);
        gameOption.nOption_Register_Friend = static_cast<unsigned char>(pOptionBit->szOption[1]);
        gameOption.nOption_OtherInfo = static_cast<unsigned char>(pOptionBit->szOption[2]);
        m_stGameOption = gameOption;
    }

    bool CheckGameOption(E_OPTION_INDEX eIndex, E_OPTION_STATE eState) const {
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

    std::uint32_t GetFriendUCID(const wchar_t* pName) const {
        return m_Community.GetFriendUCID(pName);
    }

    // 对齐 IDA: 获取社区状态（委托到 CCommunity）
    std::uint8_t GetCommunityState() const { return m_Community.GetCommunityState(); }
    std::wstring GetMemo() const { return m_Community.GetMemo(); }

    void GetFriendList(PS_FRIEND_LIST& stFriendList, std::uint8_t byType) const {
        m_Community.GetFriendList(stFriendList, byType);
    }

    // 对齐 IDA 0x1400D4A80: 获取好友等级
    std::uint8_t GetFriendLevel(std::uint32_t dwFriend) const {
        ST_FRIEND_INFO stFriendInfo{};
        if (m_Community.GetFriendInfo(dwFriend, stFriendInfo)) {
            return stFriendInfo.byLevel;
        }
        return 0;
    }

    void GetBlcokList(PS_BLOCKLIST_INFO& stBlockList) const {
        m_Community.GetBlcokList(stBlockList);
    }

    void SetLoadFriendList(bool bLoad) {
        m_Community.SetLoadFriendList(bLoad);
    }

    void SetLoadBlockList(bool bLoad) {
        m_Community.SetLoadBlockList(bLoad);
    }

    bool GetLoadFriendList() const {
        return m_Community.GetLoadFriendList();
    }

    bool GetLoadBlockList() const {
        return m_Community.GetLoadBlockList();
    }

    void SetSyncFriendList(bool bSync) {
        m_Community.SetSyncFriendList(bSync);
    }

    void SetSyncBlockList(bool bSync) {
        m_Community.SetSyncBlockList(bSync);
    }

    bool IsFriendList(const wchar_t* pName, std::uint8_t byType) const {
        return m_Community.IsFriend(pName, byType);
    }

    bool IsFriendList(std::uint32_t dwUCID, std::uint8_t byType) const {
        return m_Community.IsFriend(dwUCID, byType);
    }

    bool IsBlockList(const wchar_t* pName) const {
        return m_Community.IsBlockList(pName);
    }

    bool IsBlockList(std::uint32_t dwUCID) const {
        return m_Community.IsBlockList(dwUCID);
    }

    // 对齐 IDA: 社区状态/备忘录设置
    void SetCommunityState(std::uint8_t byState) { m_Community.SetCommunityState(byState); }
    void SetMemo(const wchar_t* strMemo) { m_Community.SetMemo(strMemo); }
    void UpdateCharCommunity(const ST_FRIEND_COMMUNITY* pCommunity) {
        m_Community.UpdateCharCommunity(pCommunity);
    }

    // 对齐 IDA: 好友/黑名单加载方法
    void SetLoadFriend() { m_bLoadFriend = true; }
    void LoadBlock(const DB_BLOCK_INFO* pBlockInfo) {
        if (!pBlockInfo) return;
        CBlockUser blockUser;
        blockUser.m_stBlockInfo.dwUCID = pBlockInfo->dwUCID;
        blockUser.m_stBlockInfo.byLevel = pBlockInfo->byLevel;
#ifdef _WIN32
        wcscpy_s(blockUser.m_stBlockInfo.strName, pBlockInfo->strName);
#else
        std::wcsncpy(blockUser.m_stBlockInfo.strName, pBlockInfo->strName, 20);
        blockUser.m_stBlockInfo.strName[20] = L'\0';
#endif
        m_Community.AddBlockUser(blockUser);
    }
    void SendBlockList() { SendBlockListImpl(); }
    void SendBlockListImpl();  // 对齐 IDA 0x1400D4D50 实际实现
    bool GetSyncBlockList() const { return m_Community.GetSyncBlockList(); }
    bool GetSyncFriendList() const { return m_Community.GetSyncFriendList(); }
    void SendFriendList() { SendFriendListImpl(); }
    void SendFriendListImpl();  // 对齐 IDA 0x1400D4BF0 实际实现
    void GetUserInfo(ST_FRIEND_INFO* pInfo) const {
        if (!pInfo) return;
        std::memset(pInfo, 0, sizeof(ST_FRIEND_INFO));
        const std::wstring name = GetName();
        const std::size_t copyLen = std::min<std::size_t>(name.size(), std::size(pInfo->strName) - 1);
        std::wmemcpy(pInfo->strName, name.c_str(), copyLen);
        pInfo->strName[copyLen] = L'\0';
        pInfo->dwID = GetCID();
        pInfo->byLevel = GetLevel();
        pInfo->byClass = GetClass();
        pInfo->byAwaken = GetAwaken();
        pInfo->dwProfilePhotoID = GetProfilePhoto();
        pInfo->bLogin = true;
        pInfo->byChannel = GetChannel();
        pInfo->wMapID = GetMapID();
    }

    // 对齐 IDA: 好友操作委托方法
    void AddFriend(const ST_FRIEND_INFO* pInfo, const std::shared_ptr<CUserObject>& pFriend) {
        m_Community.AddFriend(pInfo, pFriend);
    }
    void DeleteFriend(std::uint32_t dwUCID) {
        m_Community.DeleteFriend(dwUCID);
    }
    void UpdateFriend(const ST_FRIEND_INFO* pInfo, int bAddIfNotExists) {
        m_Community.UpdateFriend(pInfo, bAddIfNotExists);
    }
    void AddBlockList(const ST_BLOCK_INFO* pBlock) {
        m_Community.AddBlockList(pBlock);
    }
    void DeleteBlockList(std::uint32_t dwUCID) {
        m_Community.DeleteBlockList(dwUCID);
    }

    void SetLeagueWithdrawPenalty(std::int64_t biPenalty) { m_biLeagueWithdrawPenalty = biPenalty; }
    void SetLeagueDeletePenalty(std::int64_t biPenalty) { m_biLeagueDeletePenalty = biPenalty; }
    std::int64_t GetLeagueWithdrawPenalty() const { return m_biLeagueWithdrawPenalty; }
    std::int64_t GetLeagueDeletePenalty() const { return m_biLeagueDeletePenalty; }

    // 对齐 IDA: GetMatchingID 返回 ActorID 作为匹配 ID
    std::uint32_t GetMatchingID() const { return GetCID(); }

    // 对齐 IDA: 好友列表容量检查（委托到 CCommunity）
    bool IsValiedListCount(std::uint8_t byType) const {
        return m_Community.IsValiedListCount(byType);
    }
    bool IsValiedFriendListCount(std::uint8_t byType) const {
        return m_Community.IsValiedListCount(byType);
    }

    // 对齐 IDA: 好友邀请检查（委托到 CCommunity）
    bool CheckFriendInvite(PS_RES_FRIEND_INVITE& stInvite) const {
        return m_Community.CheckFriendInvite(stInvite);
    }

    // 对齐 IDA: 好友接受检查（委托到 CCommunity）
    bool CheckFriendAccept(PS_REQ_FRIEND_ACCEPT& stAccept, int& nResult) const {
        return m_Community.CheckFriendAccept(stAccept, nResult);
    }

    // 对齐 IDA: 黑名单添加检查（委托到 CCommunity）
    bool CheckBlockAdd(const wchar_t* strName, int* pResult) const {
        return m_Community.CheckBlockAdd(strName, pResult);
    }

    // 对齐 IDA: 按 UCID 获取好友信息（委托到 CCommunity）
    bool GetFriendInfo(std::uint32_t dwFriendUCID, ST_FRIEND_INFO& stFriendInfo) const {
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

    // 对齐 IDA: 推荐索引（委托到 CCommunity）
    int GetRecommandIndex() const { return m_Community.GetRecommandIndex(); }
    void SetRecommandIndex(int nIndex) { m_Community.SetRecommandIndex(nIndex); }

    // 对齐 IDA: 获取最旧等待好友（委托到 CCommunity）
    bool GetLastFriendWaitList(std::uint32_t& dwUCID) const {
        return m_Community.GetLastFriendWaitList(dwUCID);
    }

    bool IsMaze() const {
        // Primary check: map IDs in range 20000-29999 are maze maps
        // Full implementation would also check XResourceMgr::GetTB_MAZE_INFO(MapID)->Maze_Type != 6
        // But RelayServer doesn't load maze info table, so we use the range check only
        const std::uint16_t wMapID = GetMapID();
        return (wMapID / 10000) == 2;
    }

    // 对齐 IDA 0x1400D27E0: 加载好友信息
    bool LoadFriend(const PS_DB_FRIEND* pFriend, const std::shared_ptr<CUserObject>& pFriendUser, ST_FRIEND_INFO* pFriendRes);

    // 对齐 IDA 0x1400D30E0: 好友上线通知
    void LoginFriend(const ST_FRIEND_INFO& stMyInfo, const std::shared_ptr<CUserObject>& pMyUser);

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
