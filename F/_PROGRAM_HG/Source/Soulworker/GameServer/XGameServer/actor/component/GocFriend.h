#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>

// Forward declarations
class CFriend;
class CBlockUser;
class CMover;
class CUser;
struct ST_FRIEND_INFO;
struct ST_BLOCK_INFO;
struct PS_FRIEND_LIST;
struct PS_BLOCKLIST_INFO;
struct PS_RECRUIT_LIST;

/**
 * @brief CGocFriend - Game Object Component for friend/social system
 *
 * Handles friend list, block list, social interactions, and friend-related functionality.
 *
 * IDA Analysis from GameServer.exe (port 10004):
 * - Uses boost::multi_index_container for friend and block lists
 * - Friend types: 1=Friend(max 100), 2=Special Friend(max 20), 3=Unlimited, 101=Block(max 50)
 * - GetFamilyID() returns 9
 */
class CGocFriend : public GOComponent {
public:
    CGocFriend();
    virtual ~CGocFriend();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static family ID
    static int GetFamilyID();

    // Initialization
    void Init();
    void Reset();

    // Friend type validation
    bool IsValiedFriendType(std::uint8_t byType) const;
    bool IsValiedListCount(std::uint8_t byType) const;

    // Friend list operations
    int GetFriendCount() const;
    bool IsFriend(std::uint32_t dwUCID, std::uint8_t byType) const;

    // Friend management
    bool AddFriend(ST_FRIEND_INFO* stFriendInfo, bool bSend);
    void DeleteFriend(std::uint32_t dwFriendID, bool bOnMsg);
    void UpdateFriend(ST_FRIEND_INFO* stFriendInfo);

    // Friend list access
    void GetFriendList(PS_FRIEND_LIST* stFriendList, std::uint8_t byType);
    void SetFriendList(PS_FRIEND_LIST* stFriendList);
    void SendFriendList();

    // Block list operations
    int GetBlockedCount() const;
    bool IsBlock(std::uint32_t dwUCID) const;

    // Block management
    bool AddBlock(ST_BLOCK_INFO* stBlockInfo, bool bSend);
    void DeleteBlock(std::uint32_t dwFriendID, const wchar_t* strName, bool bSend);

    // Block list access
    void GetBlockList(PS_BLOCKLIST_INFO* stBlockList);
    void SetBlockList(PS_BLOCKLIST_INFO* stBlockList);
    void SendBlockList();

    // Recruit/Recommend operations
    void ResetRecommandTime();
    bool GetRecommandListReq();
    bool GetRecruitListReq(PS_RECRUIT_LIST* pList);
    bool GetRecruitInfoReq();
    void SetRecommandListReq(bool bReq, PS_RECRUIT_LIST* pList);
    void SetRecruitListReq(bool bReq, PS_RECRUIT_LIST* pList);
    void SetRecruitInfoReq(bool bReq);

protected:
    // Friend list container (boost::multi_index in original)
    // Indices: UCID (unique), Type, Name
    std::vector<CFriend*> m_vecFriends;

    // Block list container (boost::multi_index in original)
    // Indices: UCID (unique), Name
    std::vector<CBlockUser*> m_vecBlockList;

    // Request flags
    bool m_bReqFriendList;
    bool m_bReqBlockList;
    bool m_bReqRecommandList;
    bool m_bReqRecruitList;
    bool m_bReqRecruitInfo;

    // Timers for recommend/recruit
    std::map<int, std::uint32_t> m_tNextRecommandTime;
    std::map<int, std::uint32_t> m_tNextRecruitTime;

    // Recruit list request data
    PS_RECRUIT_LIST* m_stReqRecruitList;

private:
    // Helper to get owner as CMover
    CMover* GetOwnerMover() const;
    // Helper to get owner as CUser
    CUser* GetOwnerUser() const;
};
