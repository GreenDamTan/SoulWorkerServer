#pragma once

#include <algorithm>
#include <cstdint>
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

private:
    bool IsValidCommunityType(std::uint8_t byType) const {
        return byType >= 1 && byType <= 3;
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
    UXMapID GetMapIns() const { return m_uxMapID; }
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

    void ChangeMap(UXMapID uxMapID) { m_uxMapID = uxMapID; }
    void SetMapIns(UXMapID uxMapID) { m_uxMapID = uxMapID; }
    void SendFriendServerLoad() {}
    void InitRecruitListTime() { m_dwConnectTick = 0; }
    void Logout() {}
    void SetLevel(std::uint8_t byLevel) { m_stCharInfo.byLevel = byLevel; }
    void SetAwaken(std::uint8_t byAwaken) { m_stCharInfo.stBaseInfo.byAwaken = byAwaken; }
    void SetProfilePhoto(std::uint32_t dwProfilePhotoID) {
        m_stCharInfo.stBaseInfo.dwProfilePhotoID = dwProfilePhotoID;
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

    void GetFriendList(PS_FRIEND_LIST& stFriendList, std::uint8_t byType) const {
        m_Community.GetFriendList(stFriendList, byType);
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

    bool IsMaze() const {
        // Primary check: map IDs in range 20000-29999 are maze maps
        // Full implementation would also check XResourceMgr::GetTB_MAZE_INFO(MapID)->Maze_Type != 6
        // But RelayServer doesn't load maze info table, so we use the range check only
        const std::uint16_t wMapID = GetMapID();
        return (wMapID / 10000) == 2;
    }

    void SendPacket(XSendPacket& xPacket) {
        if (m_pServer) {
            m_pServer->SendEx(xPacket);
        }
    }

private:
    std::uint64_t m_dwConnectTick = 0;
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
