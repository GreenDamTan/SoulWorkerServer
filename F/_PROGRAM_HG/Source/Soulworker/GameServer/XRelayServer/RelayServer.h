#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/ForceMatching.h"
#include "Soulworker/GameServer/XRelayServer/GameDBSocket.h"
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
    void DeleteUser(const std::shared_ptr<CUserObject>& userInfo) {
        static_cast<void>(userInfo);
    }

    void AddUser(const std::shared_ptr<CUserObject>& userInfo) {
        static_cast<void>(userInfo);
    }
};

class CFriendRecruitManager {
public:
    void UpdateRecruit(std::uint32_t dwActorID, int state);
    bool IsRecruitList(std::uint32_t dwActorID) const;
    void DeleteRecruit(std::uint32_t dwActorID);
    void OnUpdate();

private:
    struct RecruitInfo {
        bool bRecruit = false;
        bool bLogin = false;
        std::int64_t tExpire = 0;
    };

    std::map<std::uint32_t, RecruitInfo> m_mapRecruit;
    mutable CFSRWLock m_rwLock{};
    std::int64_t m_tUpdate = 0;
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
    void ReqExchangePriceList(CServer* pServer, const void* stReq);
    void ReqExchangePriceUpdate(CServer* pServer, const void* stUpdate);
    void CharacterNameChange(std::uint32_t dwActorID, const wchar_t* szChangeName);
    void ChangeFriendName(const void* stChangeName);
    void SendMyRoomPollenUpdate(std::uint32_t dwUAID, int nPollenIndex,
                                const void* psHelpUser, std::uint64_t biHarvestDate);
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
    bool LoadForceDataReq();
    CServer* GetServer(std::uint32_t dwServerID);
    void KickOutUser(std::uint32_t dwUCID, std::uint8_t byType);
    void SendPacketAll(XSendPacket& xSendPacket);
    void SendPacket(std::uint32_t dwServerID, XSendPacket& xSendPacket);
    bool PrepareDeleteRecruit(const PS_RECRUIT_DELETE& stDelete);
    void SendRecruitDelete(std::uint32_t dwUCID);
    bool DeleteRecruit(const PS_RES_RECRUIT_DELETE& stDelete);
    void PrepareFriendInvite(const void* stInvite);
    void PrepareFriendAccept(const void* stAccept);
    void PrepareDeleteFriend(const void* stDelete);
    void PrepareBlockListAdd(const void* stBlock);
    void PrepareBlockListDel(const void* stBlock);
    void RecommandFriend(const void* stRecommand);
    void UpdateFriendCommunity(std::uint32_t dwActorID, const void* stCommunity);
    void ReqFriendFind(const void* stFind);
    void DailyMissionFriendReq(const void* psMission);
    void DailyMissionFriendRes(const void* psMission);
    void HelperSupportInfo(std::uint32_t dwUCID);
    void HelperSupportRegister(const void* psSupport);
    void HelperSupportReward(const void* psReward);
    void HelperSupportList(std::uint32_t dwUCID);
    void HelperSupportEquip(const void* psEquip);
    void SendRecruitList(CServer* pServer, const void* stList);
    void SendRecruitAdd(const void* stAdd);
    void SendRecruitInfo(std::uint32_t dwUCID);
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
