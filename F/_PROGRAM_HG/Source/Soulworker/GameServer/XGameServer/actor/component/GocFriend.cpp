// CGocFriend - Game Object Component for friend system
// Restored from GameServer.exe IDA decompilation
// IDA addresses verified from port 10004

#include "GocFriend.h"
#include <cstring>
#include <algorithm>

// Static function - returns Family ID for friend system
// IDA: 0x140091E60 - verified
int CGocFriend::GetFamilyID() {
    return 9;
}

// Constructor - IDA: 0x1400864C0
CGocFriend::CGocFriend()
    : GOComponent()
    , m_bReqFriendList(false)
    , m_bReqBlockList(false)
    , m_bReqRecommandList(false)
    , m_bReqRecruitList(false)
    , m_bReqRecruitInfo(false)
    , m_stReqRecruitList(nullptr)
{
}

// Destructor - IDA: 0x140086610
CGocFriend::~CGocFriend() {
    // Clean up friend list
    for (auto* pFriend : m_vecFriends) {
        if (pFriend) {
            delete pFriend;
        }
    }
    m_vecFriends.clear();

    // Clean up block list
    for (auto* pBlock : m_vecBlockList) {
        if (pBlock) {
            delete pBlock;
        }
    }
    m_vecBlockList.clear();
}

bool CGocFriend::Initialize() {
    Init();
    return true;
}

// Init - IDA: 0x140086670
void CGocFriend::Init() {
    Reset();
}

// Reset - IDA: 0x140086690
void CGocFriend::Reset() {
    // Clear friend list
    for (auto* pFriend : m_vecFriends) {
        if (pFriend) {
            delete pFriend;
        }
    }
    m_vecFriends.clear();

    // Clear block list
    for (auto* pBlock : m_vecBlockList) {
        if (pBlock) {
            delete pBlock;
        }
    }
    m_vecBlockList.clear();

    m_bReqFriendList = false;
    m_bReqBlockList = false;
    m_bReqRecommandList = false;
    m_bReqRecruitList = false;
    m_bReqRecruitInfo = false;

    m_tNextRecommandTime.clear();
    m_tNextRecruitTime.clear();

    m_stReqRecruitList = nullptr;
}

void CGocFriend::Shutdown() {
    Reset();
}

void CGocFriend::Update(float fDeltaTime) {
    (void)fDeltaTime;
}

// IsValiedFriendType - IDA: 0x140086730
// Returns true if type is 1 (Friend), 2 (Special Friend), or 3 (Unlimited)
bool CGocFriend::IsValiedFriendType(std::uint8_t byType) const {
    return byType >= 1 && byType <= 3;
}

// IsValiedListCount - IDA: 0x140086760
// Validates list count against type-specific limits
bool CGocFriend::IsValiedListCount(std::uint8_t byType) const {
    switch (byType) {
        case 1:  // Friend - max 100
            return m_vecFriends.size() < 100;
        case 2:  // Special friend - max 20
            return m_vecFriends.size() < 20;
        case 3:  // Unlimited
            return true;
        case 101: // Block list - max 50
            return m_vecBlockList.size() < 50;
        default:
            return false;
    }
}

// Get friend count
int CGocFriend::GetFriendCount() const {
    return static_cast<int>(m_vecFriends.size());
}

// IsFriend - IDA: 0x140086810
// Checks if a user with given UCID is a friend of specified type
// IDA shows: searches m_mapFriend by UCID, validates type match
bool CGocFriend::IsFriend(std::uint32_t dwUCID, std::uint8_t byType) const {
    if (!IsValiedFriendType(byType)) {
        return false;
    }

    // Search friend list by UCID
    for (const auto* pFriend : m_vecFriends) {
        if (pFriend && pFriend->GetUCID() == dwUCID) {
            // Found by UCID, check type match
            return pFriend->GetType() == byType;
        }
    }

    return false;
}

// AddFriend - IDA: 0x140087350
// Adds a friend to the list or updates if already exists
// IDA logic:
// 1. Check if friend already exists (type 1, 2, or 3)
// 2. If exists, call UpdateFriend
// 3. If not, create new CFriend and insert into m_mapFriend
// 4. If bSend, send packet to client
bool CGocFriend::AddFriend(DB_FRIEND_INFO* stFriendInfo, bool bSend) {
    if (!stFriendInfo) {
        return false;
    }

    // Check if friend already exists (any type)
    if (IsFriend(stFriendInfo->dwID, 1) ||
        IsFriend(stFriendInfo->dwID, 2) ||
        IsFriend(stFriendInfo->dwID, 3)) {
        // Friend exists - update info
        UpdateFriend(stFriendInfo);
        return true;
    }

    // Create new CFriend object (IDA shows CFriend::CFriend at v12)
    CFriend* pNewFriend = new CFriend(stFriendInfo);
    if (!pNewFriend) {
        return false;
    }

    // Add to friend list
    m_vecFriends.push_back(pNewFriend);

    // TODO: 汇编还原 - Send packet if requested
    // IDA shows: XSendPacket with main=0x19, sub=0x14
    // if (bSend) {
    //     XSendPacket packet(0x19, 0x14);
    //     packet << stFriendInfo;
    //     CGocNetwork::Send(GetOwnerMover(), &packet);
    // }
    (void)bSend;

    return true;
}

// DeleteFriend - IDA: 0x1400877F0
// Removes a friend from the list by UCID
// IDA logic:
// 1. Get UCID index from m_mapFriend
// 2. Find friend by dwFriendID
// 3. If found, erase from container
// 4. Send PS_FRIEND_DELETE packet (main=0x19, sub=0x15)
void CGocFriend::DeleteFriend(std::uint32_t dwFriendID, bool bOnMsg) {
    // Find friend by UCID
    auto it = std::find_if(m_vecFriends.begin(), m_vecFriends.end(),
        [dwFriendID](const CFriend* pFriend) {
            return pFriend && pFriend->GetUCID() == dwFriendID;
        });

    if (it != m_vecFriends.end()) {
        // Found - delete and remove
        if (*it) {
            delete *it;
        }
        m_vecFriends.erase(it);

        // TODO: 汇编还原 - Send delete packet to client
        // IDA shows: PS_FRIEND_DELETE packet with main=0x19, sub=0x15
    }
    (void)bOnMsg;
}

// UpdateFriend - IDA: 0x140089B50
// Updates existing friend information
// IDA logic:
// 1. Find friend by UCID in m_mapFriend
// 2. Call CFriend::UpdateInfo
// 3. Send updated info to client (main=0x19, sub=0x31)
void CGocFriend::UpdateFriend(DB_FRIEND_INFO* stFriendInfo) {
    if (!stFriendInfo) {
        return;
    }

    // Find friend by ID
    for (auto* pFriend : m_vecFriends) {
        if (pFriend && pFriend->GetUCID() == stFriendInfo->dwID) {
            // Found - update info
            pFriend->UpdateInfo(stFriendInfo);
            return;
        }
    }
}

// GetFriendList - IDA: 0x140089980
// Retrieves friend list, optionally filtered by type
// IDA logic:
// 1. Iterate through m_mapFriend
// 2. If byType is 0, include all; otherwise filter by type
// 3. Push each friend's info to stFriendList->vecFriend
void CGocFriend::GetFriendList(PS_DB_FRIEND_LIST* stFriendList, std::uint8_t byType) {
    if (!stFriendList) {
        return;
    }

    // Iterate through all friends
    for (const auto* pFriend : m_vecFriends) {
        if (!pFriend) {
            continue;
        }

        // Filter by type if specified
        if (byType == 0 || pFriend->GetType() == byType) {
            // Add friend info to output list
            DB_FRIEND_INFO stInfo;
            pFriend->GetInfo(&stInfo);
            stFriendList->vecFriend.push_back(stInfo);
        }
    }
}

// SetFriendList - IDA: 0x140086CB0
// Sets friend list from server data
// IDA logic:
// 1. Iterate through input vector
// 2. Call AddFriend for each entry (with bSend=false)
// 3. Update user DB flag (bit 0 of UserDB+1)
// 4. If m_bReqFriendList is set, call SendFriendList
void CGocFriend::SetFriendList(PS_DB_FRIEND_LIST* stFriendList) {
    if (!stFriendList) {
        return;
    }

    // Add each friend from the list
    for (auto& stInfo : stFriendList->vecFriend) {
        AddFriend(&stInfo, false);
    }

    // Send list if requested
    if (m_bReqFriendList) {
        SendFriendList();
    }
}

// SendFriendList - IDA: 0x140086E90
// Sends friend list to client
// IDA logic:
// 1. Get owner as CUser
// 2. Check if friend list loaded (bit 0 of UserDB+1)
// 3. If loaded: send PS_FRIEND_LIST packet (main=0x19, sub=1)
// 4. If not loaded: set m_bReqFriendList=true, request from community server
void CGocFriend::SendFriendList() {
    // TODO: 汇编还原 - Full implementation requires CUser access
    m_bReqFriendList = false;
}

// Get blocked count
int CGocFriend::GetBlockedCount() const {
    return static_cast<int>(m_vecBlockList.size());
}

// IsBlock - IDA: 0x140086970
// Checks if a user is blocked by UCID
// IDA logic: searches m_mapBlockList by UCID
bool CGocFriend::IsBlock(std::uint32_t dwUCID) const {
    // Search block list by UCID
    for (const auto* pBlock : m_vecBlockList) {
        if (pBlock && pBlock->GetUCID() == dwUCID) {
            return true;
        }
    }
    return false;
}

// AddBlock - IDA: 0x1400875C0
// Adds a blocked user to the list
// IDA logic:
// 1. Check if already blocked
// 2. If exists, log error and return false
// 3. Create new CBlockUser and insert into m_mapBlockList
// 4. If bSend, send packet (main=0x19, sub=0x21)
bool CGocFriend::AddBlock(DB_BLOCK_INFO* stBlockInfo, bool bSend) {
    if (!stBlockInfo) {
        return false;
    }

    // Check if already blocked
    if (IsBlock(stBlockInfo->dwUCID)) {
        return false;
    }

    // Create new blocked user
    CBlockUser* pNewBlock = new CBlockUser(stBlockInfo);
    if (!pNewBlock) {
        return false;
    }

    // Add to block list
    m_vecBlockList.push_back(pNewBlock);

    // TODO: 汇编还原 - Send packet if requested
    (void)bSend;

    return true;
}

// DeleteBlock - IDA: 0x140087A50
// Removes a blocked user by UCID
// IDA logic:
// 1. Find in m_mapBlockList by UCID
// 2. If found, erase from container
// 3. If bSend, send ST_BLOCK_DELETE packet (main=0x19, sub=0x22)
void CGocFriend::DeleteBlock(std::uint32_t dwFriendID, const wchar_t* strName, bool bSend) {
    // Find block by UCID
    auto it = std::find_if(m_vecBlockList.begin(), m_vecBlockList.end(),
        [dwFriendID](const CBlockUser* pBlock) {
            return pBlock && pBlock->GetUCID() == dwFriendID;
        });

    if (it != m_vecBlockList.end()) {
        // Found - delete and remove
        if (*it) {
            delete *it;
        }
        m_vecBlockList.erase(it);
    }
    (void)strName;
    (void)bSend;
}

// GetBlockList - IDA: 0x140089A90
// Retrieves block list
// IDA logic: iterate m_mapBlockList and add each to output vector
void CGocFriend::GetBlockList(DB_BLOCKLIST_INFO* stBlockList) {
    if (!stBlockList) {
        return;
    }

    // Iterate through all blocked users
    for (const auto* pBlock : m_vecBlockList) {
        if (pBlock) {
            DB_BLOCK_INFO stInfo;
            pBlock->GetInfo(&stInfo);
            stBlockList->vecBlockList.push_back(stInfo);
        }
    }
}

// SetBlockList - IDA: 0x140086DA0
// Sets block list from server data
// IDA logic:
// 1. Iterate through input vector
// 2. Call AddBlock for each entry (with bSend=false)
// 3. Update user DB flag (bit 6 of UserDB)
// 4. If m_bReqBlockList is set, call SendBlockList
void CGocFriend::SetBlockList(DB_BLOCKLIST_INFO* stBlockList) {
    if (!stBlockList) {
        return;
    }

    // Add each block from the list
    for (auto& stInfo : stBlockList->vecBlockList) {
        AddBlock(&stInfo, false);
    }

    // Send list if requested
    if (m_bReqBlockList) {
        SendBlockList();
    }
}

// SendBlockList - IDA: 0x1400870F0
// Sends block list to client
// IDA logic similar to SendFriendList
void CGocFriend::SendBlockList() {
    // TODO: 汇编还原 - Full implementation requires CUser access
    m_bReqBlockList = false;
}

// Recommend/Recruit operations - IDA: 0x1400860F0
void CGocFriend::ResetRecommandTime() {
    m_tNextRecommandTime.clear();
}

// IDA: 0x140086140
bool CGocFriend::GetRecommandListReq() {
    return m_bReqRecommandList;
}

// IDA: 0x140086160
bool CGocFriend::GetRecruitListReq(ST_RECRUIT_LIST* pList) {
    (void)pList;
    return m_bReqRecruitList;
}

// IDA: 0x140086190
bool CGocFriend::GetRecruitInfoReq() {
    return m_bReqRecruitInfo;
}

// IDA: 0x1400861B0
void CGocFriend::SetRecommandListReq(bool bReq, ST_RECRUIT_LIST* pList) {
    m_bReqRecommandList = bReq;
    (void)pList;
}

// IDA: 0x1400861D0
void CGocFriend::SetRecruitListReq(bool bReq, ST_RECRUIT_LIST* pList) {
    m_bReqRecruitList = bReq;
    (void)pList;
}

// IDA: 0x140086210
void CGocFriend::SetRecruitInfoReq(bool bReq) {
    m_bReqRecruitInfo = bReq;
}

// Helper methods - TODO: Implement when CMover/CUser is available
CMover* CGocFriend::GetOwnerMover() const {
    // TODO: Implement when CMover is available
    return nullptr;
}

CUser* CGocFriend::GetOwnerUser() const {
    // TODO: Implement when CUser is available
    return nullptr;
}
