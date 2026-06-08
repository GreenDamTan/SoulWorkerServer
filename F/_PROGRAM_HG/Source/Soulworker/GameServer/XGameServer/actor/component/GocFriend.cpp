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
// IDA decompiled: return byType && byType <= 3u;
// Returns true if type is 1 (Friend), 2 (Special Friend), or 3 (Unlimited)
bool CGocFriend::IsValiedFriendType(std::uint8_t byType) const {
    return byType != 0 && byType <= 3;
}

// IsValiedListCount - IDA: 0x140086760
// Validates list count against type-specific limits
// IDA shows: uses boost::multi_index::count() to count friends of specific type
// - Type 1: count < 100 (0x64)
// - Type 2: count < 20 (0x14)
// - Type 3: always true (unlimited)
// - Type 101: block list size < 50 (0x32)
// Current implementation counts by iterating since we use vector instead of multi_index
bool CGocFriend::IsValiedListCount(std::uint8_t byType) const {
    switch (byType) {
        case 1:  // Friend - max 100 (0x64)
            {
                std::size_t count = 0;
                for (const auto* pFriend : m_vecFriends) {
                    if (pFriend && pFriend->GetType() == 1) {
                        ++count;
                    }
                }
                return count < 100;
            }
        case 2:  // Special friend - max 20 (0x14)
            {
                std::size_t count = 0;
                for (const auto* pFriend : m_vecFriends) {
                    if (pFriend && pFriend->GetType() == 2) {
                        ++count;
                    }
                }
                return count < 20;
            }
        case 3:  // Unlimited
            return true;
        case 101: // Block list - max 50 (0x32)
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
// IDA logic:
// 1. Check IsValiedFriendType first
// 2. Find in m_mapFriend by UCID (hashed index)
// 3. If found, compare GetType() with byType
// Returns true only if found AND type matches
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
// 2. If exists, call UpdateFriend and return true
// 3. If not, create new CFriend (size 0xA0), wrap in shared_ptr, insert into m_mapFriend
// 4. If bSend, send packet (main=0x19, sub=0x14) to client
// 5. Log debug message with UCID
// Returns true always (unless allocation fails)
bool CGocFriend::AddFriend(ST_FRIEND_INFO* stFriendInfo, bool bSend) {
    if (!stFriendInfo) {
        return false;
    }

    // Check if friend already exists (any valid type)
    if (IsFriend(stFriendInfo->dwID, 1) ||
        IsFriend(stFriendInfo->dwID, 2) ||
        IsFriend(stFriendInfo->dwID, 3)) {
        // Friend exists - update info
        UpdateFriend(stFriendInfo);
        return true;
    }

    // Create new CFriend object
    // IDA shows: VBaseObject::operator new(0xA0u) then CFriend::CFriend
    CFriend* pNewFriend = new CFriend(stFriendInfo);
    if (!pNewFriend) {
        return false;
    }

    // Add to friend list
    // IDA uses: boost::multi_index::insert with shared_ptr
    m_vecFriends.push_back(pNewFriend);

    // Send packet if requested
    if (bSend) {
        XSendPacket xSendPacket(0x19, 0x14);
        xSendPacket << stFriendInfo;
        XActor* pActor = GetOwnerActor();
        if (pActor) {
            CGocNetwork::Send(pActor, &xSendPacket);
        }
        LogHelper::LogDebug("game.contents", "<FRIEND> Add Friend ( UCID: %d ) ( %d )", stFriendInfo.dwID, 209);
    }

    return true;
}

// DeleteFriend - IDA: 0x1400877F0
// Removes a friend from the list by UCID
// IDA logic:
// 1. Get hashed index from m_mapFriend
// 2. Find friend by dwFriendID using find()
// 3. If found and not end(), erase from container
// 4. Send PS_FRIEND_DELETE packet (main=0x19, sub=0x15)
// Note: CFriend objects are managed by shared_ptr in original, so no manual delete needed
void CGocFriend::DeleteFriend(std::uint32_t dwFriendID, bool bOnMsg) {
    // Find friend by UCID
    auto it = std::find_if(m_vecFriends.begin(), m_vecFriends.end(),
        [dwFriendID](const CFriend* pFriend) {
            return pFriend && pFriend->GetUCID() == dwFriendID;
        });

    if (it != m_vecFriends.end()) {
        // Found - delete and remove
        // IDA: boost::multi_index::erase() handles removal
        if (*it) {
            delete *it;
        }
        m_vecFriends.erase(it);

        // Send delete packet to client
        PS_FRIEND_DELETE psDelete;
        psDelete.dwFriendID = dwFriendID;
        psDelete.bOnMsg = bOnMsg;
        XSendPacket xSendPacket(0x19, 0x15);
        xSendPacket << psDelete;
        XActor* pActor = GetOwnerActor();
        if (pActor) {
            CGocNetwork::Send(pActor, &xSendPacket);
        }
    }
    (void)bOnMsg;
}

// UpdateFriend - IDA: 0x140089B50
// Updates existing friend information
// IDA logic:
// 1. Find friend by UCID in m_mapFriend using find()
// 2. If found and valid shared_ptr, call CFriend::UpdateInfo
// 3. Get updated info via CFriend::GetInfo
// 4. Send ST_FRIEND_INFO packet (main=0x19, sub=0x31) to client
void CGocFriend::UpdateFriend(ST_FRIEND_INFO* stFriendInfo) {
    if (!stFriendInfo) {
        return;
    }

    // Find friend by ID
    for (auto* pFriend : m_vecFriends) {
        if (pFriend && pFriend->GetUCID() == stFriendInfo->dwID) {
            // Found - update info
            pFriend->UpdateInfo(stFriendInfo);

            // Send update packet to client
    // TODO: Requires full dependency implementation
            // IDA shows:
            // CFriend::GetInfo(v7, &stInfo);
            // XSendPacket::XSendPacket(&xSendPacket, 0x19u, 0x31u);
            // operator<<(&xSendPacket, &stInfo);
            // CGocNetwork::Send(pActor, &xSendPacket);
            return;
        }
    }
}

// GetFriendList - IDA: 0x140089980
// Retrieves friend list, optionally filtered by type
// IDA logic:
// 1. Get begin/end iterators from m_mapFriend (hashed index)
// 2. Iterate through all entries
// 3. For each valid shared_ptr:
//    - If byType == 0: include all
//    - Otherwise: only if GetType() == byType
// 4. Push ST_FRIEND_INFO directly to output vector (uses shared_ptr->operator->)
void CGocFriend::GetFriendList(PS_FRIEND_LIST* stFriendList, std::uint8_t byType) {
    if (!stFriendList) {
        return;
    }

    // Iterate through all friends
    for (const auto* pFriend : m_vecFriends) {
        if (!pFriend) {
            continue;
        }

        // Filter by type if specified
        // IDA: if (!byType || GetType() == byType)
        if (byType == 0 || pFriend->GetType() == byType) {
            // Get friend info and add to output list
            ST_FRIEND_INFO stInfo;
            pFriend->GetInfo(&stInfo);
            stFriendList->vecFriends.push_back(stInfo);
        }
    }
}

// SetFriendList - IDA: 0x140086CB0
// Sets friend list from server data
// IDA logic:
// 1. Iterate through input vector (vecFriends)
// 2. Call AddFriend for each ST_FRIEND_INFO (with bSend=false)
// 3. Get owner as CUser via RTTI dynamic cast
// 4. If CUser valid, set bit 0 of UserDB+1 (friend list loaded flag)
// 5. If m_bReqFriendList is set, call SendFriendList
void CGocFriend::SetFriendList(PS_FRIEND_LIST* stFriendList) {
    if (!stFriendList) {
        return;
    }

    // Add each friend from the list
    for (auto& stInfo : stFriendList->vecFriends) {
        AddFriend(&stInfo, false);
    }

    // Set user DB flag
    // TODO: Requires full dependency implementation
    // IDA shows:
    // v4 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // pUser = (CUser *)_RTDynamicCast_0(v4, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    // if (pUser) {
    //     v8 = (char *)&CUser::stMyCharInfoEx(pUser)->UserDB + 1;
    //     *v8 |= 1u;  // Set bit 0
    // }

    // Send list if requested
    if (m_bReqFriendList) {
        SendFriendList();
    }
}

// SendFriendList - IDA: 0x140086E90
// Sends friend list to client
// IDA logic:
// 1. Get owner as CUser via RTTI dynamic cast
// 2. If CUser valid:
//    - Check if friend list loaded (bit 0 of UserDB+1)
//    - If loaded (flag == 1):
//      * Create PS_FRIEND_LIST, call GetFriendList with type 0
//      * Set m_bReqFriendList = false
//      * Send packet (main=0x19, sub=1) to client
//    - If not loaded:
//      * Set m_bReqFriendList = true
//      * If bit 1 of UserDB+1 is set (community connected):
//        - Get dwReqUCID from user
//        - Send request to community socket (main=0xF5, sub=1)
void CGocFriend::SendFriendList() {
    // Full implementation requires CUser/XGameServer access
    // TODO: Requires full dependency implementation
    // IDA pseudocode:
    // pUser = GetOwnerUser();
    // if (pUser) {
    //     if ((pUser->stMyCharInfoEx()->UserDB[1] & 1) == 1) {
    //         // Friend list loaded - send to client
    //         PS_FRIEND_LIST stFriendList;
    //         GetFriendList(&stFriendList, 0);
    //         m_bReqFriendList = false;
    //         XSendPacket packet(0x19, 1);
    //         packet << stFriendList;
    //         CGocNetwork::Send(pUser, &packet);
    //     } else {
    //         m_bReqFriendList = true;
    //         if ((pUser->stMyCharInfoEx()->UserDB[1] & 2) != 0) {
    //             // Request from community server
    //             XSendPacket packet(0xF5, 1);
    //             packet << pUser->GetReqUCID();
    //             XGameServer::Instance()->m_communitySocket.SendCmd(&packet, pUser, 0x19, 1);
    //         }
    //     }
    // }
    m_bReqFriendList = false;
}

// Get blocked count
int CGocFriend::GetBlockedCount() const {
    return static_cast<int>(m_vecBlockList.size());
}

// IsBlock - IDA: 0x140086970
// Checks if a user is blocked by UCID
// IDA logic:
// 1. Get hashed index from m_mapBlockList
// 2. Find by UCID
// 3. If found (not end()), verify UCID matches
// Returns true if found
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
// 1. Check if already blocked via IsBlock
// 2. If exists, log error "AddBlock error - Duplicate BLOCK" and return false
// 3. Create new CBlockUser (size 0x30), wrap in shared_ptr
// 4. Insert into m_mapBlockList
// 5. If bSend, send packet (main=0x19, sub=0x21) to client
// Returns true on success
bool CGocFriend::AddBlock(ST_BLOCK_INFO* stBlockInfo, bool bSend) {
    if (!stBlockInfo) {
        return false;
    }

    // Check if already blocked
    if (IsBlock(stBlockInfo->dwUCID)) {
        // Log error
    // TODO: Requires full dependency implementation
        // IDA shows:
        // LogHelper::LogError("game.contents",
        //     "AddBlock error - Duplicate BLOCK[ ActorID:%d, BlockActorID:%d ] ( %d )",
        //     actorID, stBlockInfo->dwUCID, 368);
        return false;
    }

    // Create new blocked user
    // IDA shows: VBaseObject::operator new(0x30u) then CBlockUser::CBlockUser
    // Note: CBlockUser from UserObject.h has m_stBlockInfo member directly
    CBlockUser* pNewBlock = new CBlockUser();
    if (!pNewBlock) {
        return false;
    }
    // Copy block info to the new object's member
    if (stBlockInfo) {
        pNewBlock->m_stBlockInfo.dwUCID = stBlockInfo->dwUCID;
        std::wcsncpy(pNewBlock->m_stBlockInfo.strName, stBlockInfo->strName, 21);
        pNewBlock->m_stBlockInfo.byLevel = stBlockInfo->byLevel;
    }

    // Add to block list
    // IDA uses: boost::multi_index::insert with shared_ptr
    m_vecBlockList.push_back(pNewBlock);

    // Send packet if requested
    if (bSend) {
        XSendPacket xSendPacket(0x19, 0x21);
        xSendPacket << *stBlockInfo;
        XActor* pActor = GetOwnerActor();
        if (pActor) {
            CGocNetwork::Send(pActor, &xSendPacket);
        }
    }

    return true;
}

// DeleteBlock - IDA: 0x140087A50
// Removes a blocked user by UCID
// IDA logic:
// 1. Get hashed index from m_mapBlockList
// 2. Find by dwFriendID
// 3. If found, erase from container
// 4. If bSend, copy strName to packet, send ST_BLOCK_DELETE (main=0x19, sub=0x22)
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

        // Send delete packet to client if bSend
        if (bSend) {
            PS_BLOCK_DELETE psDelete;
            psDelete.dwUCID = stBlockInfo->dwUCID;
            XSendPacket xSendPacket(0x19, 0x19);
            xSendPacket << psDelete;
            XActor* pActor = GetOwnerActor();
            if (pActor) {
                CGocNetwork::Send(pActor, &xSendPacket);
            }
        }
}

// GetBlockList - IDA: 0x140089A90
// Retrieves block list
// IDA logic:
// 1. Get begin/end iterators from m_mapBlockList
// 2. For each valid shared_ptr, push ST_BLOCK_INFO to output vector
void CGocFriend::GetBlockList(PS_BLOCKLIST_INFO* stBlockList) {
    if (!stBlockList) {
        return;
    }

    // Iterate through all blocked users
    for (const auto* pBlock : m_vecBlockList) {
        if (pBlock) {
            // CBlockUser from UserObject.h has m_stBlockInfo member directly
            stBlockList->vecBlockList.push_back(pBlock->m_stBlockInfo);
        }
    }
}

// SetBlockList - IDA: 0x140086DA0
// Sets block list from server data
// IDA logic:
// 1. Iterate through input vector
// 2. Call AddBlock for each ST_BLOCK_INFO (with bSend=false)
// 3. Get owner as CUser via RTTI
// 4. If CUser valid, set bit 6 of UserDB (block list loaded flag)
// 5. If m_bReqBlockList is set, call SendBlockList
void CGocFriend::SetBlockList(PS_BLOCKLIST_INFO* stBlockList) {
    if (!stBlockList) {
        return;
    }

    // Add each block from the list
    for (auto& stInfo : stBlockList->vecBlockList) {
        AddBlock(&stInfo, false);
    }

    // Set user DB flag
    // TODO: Requires full dependency implementation
    // IDA shows:
    // pUser = (CUser *)_RTDynamicCast_0(...);
    // if (pUser) {
    //     v10 = &CUser::stMyCharInfoEx(pUser)->___u16;
    //     *(_BYTE *)&v10->UserDB |= 0x40u;  // Set bit 6
    // }

    // Send list if requested
    if (m_bReqBlockList) {
        SendBlockList();
    }
}

// SendBlockList - IDA: 0x1400870F0
// Sends block list to client
// IDA logic (similar to SendFriendList):
// 1. Get owner as CUser via RTTI
// 2. If CUser valid:
//    - Check if block list loaded (bit 6 of UserDB)
//    - If loaded:
//      * Create PS_BLOCKLIST_INFO, call GetBlockList
//      * Set m_bReqBlockList = false
//      * Send packet (main=0x19, sub=2) to client
//    - If not loaded:
//      * Set m_bReqBlockList = true
//      * If bit 1 of UserDB+1 is set (community connected):
//        - Send request to community socket (main=0xF5, sub=2)
void CGocFriend::SendBlockList() {
    // Full implementation requires CUser/XGameServer access
    // TODO: Requires full dependency implementation
    // IDA pseudocode:
    // pUser = GetOwnerUser();
    // if (pUser) {
    //     if ((pUser->stMyCharInfoEx()->UserDB & 0x40) != 0) {
    //         // Block list loaded - send to client
    //         PS_BLOCKLIST_INFO stBlockList;
    //         GetBlockList(&stBlockList);
    //         m_bReqBlockList = false;
    //         XSendPacket packet(0x19, 2);
    //         packet << stBlockList;
    //         CGocNetwork::Send(pUser, &packet);
    //     } else {
    //         m_bReqBlockList = true;
    //         if ((pUser->stMyCharInfoEx()->UserDB[1] & 2) != 0) {
    //             // Request from community server
    //             XSendPacket packet(0xF5, 2);
    //             packet << pUser->GetReqUCID();
    //             XGameServer::Instance()->m_communitySocket.SendCmd(&packet, pUser, 0x19, 2);
    //         }
    //     }
    // }
    m_bReqBlockList = false;
}

// Recommend/Recruit operations

// ResetRecommandTime - IDA: 0x1400860F0
void CGocFriend::ResetRecommandTime() {
    m_tNextRecommandTime.clear();
}

// GetRecommandListReq - IDA: 0x140086140
bool CGocFriend::GetRecommandListReq() {
    return m_bReqRecommandList;
}

// GetRecruitListReq - IDA: 0x140086160
bool CGocFriend::GetRecruitListReq(PS_RECRUIT_LIST* stInfo) {
    (void)stInfo;
    return m_bReqRecruitList;
}

// GetRecruitInfoReq - IDA: 0x140086190
bool CGocFriend::GetRecruitInfoReq() {
    return m_bReqRecruitInfo;
}

// SetRecommandListReq - IDA: 0x1400861B0
void CGocFriend::SetRecommandListReq(bool bReq) {
    m_bReqRecommandList = bReq;
}

// SetRecommandListReq with PS_RECRUIT_LIST - IDA: 0x1400861B0 (overload)
void CGocFriend::SetRecommandListReq(bool bReq, PS_RECRUIT_LIST* pList) {
    m_bReqRecommandList = bReq;
    (void)pList;
}

// SetRecruitListReq - IDA: 0x1400861D0
void CGocFriend::SetRecruitListReq(bool bReq, PS_RECRUIT_LIST* pList) {
    m_bReqRecruitList = bReq;
    (void)pList;
}

// SetRecruitInfoReq - IDA: 0x140086210
void CGocFriend::SetRecruitInfoReq(bool bReq) {
    m_bReqRecruitInfo = bReq;
}

// Helper methods - TODO: Implement when CMover/CUser is available
CMover* CGocFriend::GetOwnerMover() const {
    // TODO: Implement when CMover is available
    // IDA shows: this + offset to owner actor, then dynamic_cast or direct access
    return nullptr;
}

CUser* CGocFriend::GetOwnerUser() const {
    // TODO: Implement when CUser is available
    // IDA shows: _RTDynamicCast_0 with CMover and CUser type descriptors
    return nullptr;
}

// ============================================================================
// Additional IDA-verified implementations
// ============================================================================

// IsBlock (by name) - IDA: 0x140086A70
// Checks if a user is blocked by name
// IDA logic: uses boost::multi_index hashed index on name to find block entry
bool CGocFriend::IsBlockByName(const wchar_t* strName) const {
    if (!strName) {
        return false;
    }

    // Search block list by name
    for (const auto* pBlock : m_vecBlockList) {
        if (pBlock) {
            // Compare name (CBlockUser has GetName() returning std::wstring)
            const wchar_t* blockName = pBlock->m_stBlockInfo.strName;
            if (blockName && wcscmp(blockName, strName) == 0) {
                return true;
            }
        }
    }
    return false;
}

// SetFriendServerLoad - IDA: 0x140086C20
// Sets the friend server loaded flag in user DB
// IDA logic:
// 1. Get owner as CUser via RTTI
// 2. Set bit 1 of UserDB+1 (friend server connected flag)
// 3. If m_bReqFriendList is set, call SendFriendList
void CGocFriend::SetFriendServerLoad() {
    // Requires CUser access
    // TODO: Requires full dependency implementation
    // IDA shows:
    // pUser = (CUser *)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    // if (pUser) {
    //     v4 = (char *)&CUser::stMyCharInfoEx(pUser)->UserDB + 1;
    //     *v4 |= 2u;  // Set bit 1
    // }
    // if (m_bReqFriendList) {
    //     CGocFriend::SendFriendList(this);
    // }

    // Simplified implementation
    if (m_bReqFriendList) {
        SendFriendList();
    }
}

// UpdatePartyBooster - IDA: 0x140087980
// Updates party booster for friend system
// IDA logic:
// 1. Get owner as CUser via RTTI
// 2. Get CGocParty component and call UpdatePartyBooster
// 3. Get CGocForce component and call UpdatePartyBooster
void CGocFriend::UpdatePartyBooster() {
    // Full implementation requires CUser, CGocParty, CGocForce
    // TODO: Requires full dependency implementation
    // IDA shows:
    // pUser = (CUser *)_RTDynamicCast_0(v1, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    // if (pUser) {
    //     CMover::GetGOC<CGocParty>(&pUser->CMoverEx, &pPartyPtr, 0);
    //     CGocParty::UpdatePartyBooster(pPartyPtr.operator->());
    //     CMover::GetGOC<CGocForce>(&pUser->CMoverEx, &pForcePtr, 0);
    //     CGocForce::UpdatePartyBooster(pForcePtr.operator->());
    // }
}

// PrepareFriendInvite - IDA: 0x140087C80
// Prepares and sends friend invite request to community server
void CGocFriend::PrepareFriendInvite(PS_REQ_FRIEND_INVITE& stInvite) {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    CUser* pTarget = pServer->FindNameToUser(stInvite.strName);
    
    if (!pTarget || pTarget->CheckGameOption(eOption_Register_Friend, eGAME_OPTION_REFUSE_ALL)) {
        // Target not found or refusing friend requests - send error to community
        PS_RES_FRIEND_INVITE stResInvite;
        stResInvite.dwReqUCID = GetOwnerUser()->GetUCID();
        std::wcscpy(stResInvite.strReqUserName, GetOwnerUser()->GetName().c_str());
        std::wcscpy(stResInvite.strTargetUserName, stInvite.strName);
        
        XSendPacket xSendPacket(0xF5, 3);
        xSendPacket << stResInvite;
        CCommunitySocket::SendCmd(pServer->m_communitySocket, &xSendPacket, GetOwnerUser(), 0x19, 0x11);
    } else {
        // Target is refusing - send error to client
        LogHelper::LogError("game.contents", "PrepareFriendInvite error - Refuse game option ( %d )", 501);
        
        PS_FRIEND_RESULT stResult;
        std::wcscpy(stResult.strName, stInvite.strName);
        stResult.nResult = 59202;
        
        XSendPacket packet(0x19, 0x11);
        packet << stResult;
        CGocNetwork::Send(GetOwnerActor(), &packet);
    }
}

// PrepareFriendAccept - IDA: 0x1400880B0
// Prepares and sends friend accept response to community server
void CGocFriend::PrepareFriendAccept(PS_REQ_FRIEND_ACCEPT& stAccept) {
    if (!stAccept.dwTargetUCID || std::wcslen(stAccept.strTargetUserName) == 0) {
        // Invalid parameters
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x13, 0xD740);
        return;
    }
    
    if (IsBlock(stAccept.strTargetUserName)) {
        // Target is blocked
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x13, 0xD741);
        return;
    }
    
    if (IsFriend(stAccept.dwTargetUCID, 2)) {
        // Already special friend - send to community for upgrade
        stAccept.dwReqUCID = GetOwnerUser()->GetUCID();
        
        XSendPacket xSendPacket(0xF5, 4);
        xSendPacket << stAccept;
        CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket, 
                                   &xSendPacket, GetOwnerUser(), 0x19, 0x13);
    } else {
        // Not a friend
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x13, 0xD74A);
    }
}

// PrepareDelFriend - IDA: 0x1400882F0
// Prepares and sends friend delete request to community server
bool CGocFriend::PrepareDelFriend(PS_FRIEND_DELETE& stDelete) {
    if (!IsFriend(stDelete.dwFriendID, 1)) {
        return false;
    }
    
    PS_REQ_FRIEND_DELETE stFriendDel;
    stFriendDel.dwReqID = GetOwnerUser()->GetUCID();
    stFriendDel.dwFriendID = stDelete.dwFriendID;
    
    XSendPacket xSendPacket(0xF5, 5);
    xSendPacket << stFriendDel;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, GetOwnerUser(), 0x19, 5);
    return true;
}

// PrepareAddBlock - IDA: 0x140088460
// Prepares and sends block add request to community server
bool CGocFriend::PrepareAddBlock(PS_FRIEND_BLOCK_ADD& stBlockAdd) {
    if (IsBlock(stBlockAdd.strName)) {
        // Already blocked
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x21, 0xD741);
        return false;
    }
    
    if (!IsValiedListCount(0x65)) {
        // Block list is full
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x21, 0xD742);
        return false;
    }
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return false;
    }
    
    PS_REQ_FRIEND_BLOCK_ADD psBlockAdd;
    psBlockAdd.dwReqUAID = pUser->GetUAID();
    std::wcscpy(psBlockAdd.strTargetName, stBlockAdd.strName);
    
    XSendPacket xSendPacket(0xF5, 7);
    xSendPacket << psBlockAdd;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, pUser, 0x19, 0x21);
    return true;
}

// PrepareDelBlock - IDA: 0x1400886B0
// Prepares and sends block delete request to community server
bool CGocFriend::PrepareDelBlock(PS_FRIEND_BLOCK_DELETE& stBlockDel) {
    if (!IsBlock(stBlockDel.strName)) {
        // Not blocked
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x22, 0xD745);
        return false;
    }
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return false;
    }
    
    PS_REQ_FRIEND_BLOCK_DELETE psBlockDelete;
    psBlockDelete.dwReqUAID = pUser->GetUAID();
    std::wcscpy(psBlockDelete.strTargetName, stBlockDel.strName);
    
    XSendPacket xSendPacket(0xF5, 8);
    xSendPacket << psBlockDelete;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, pUser, 0x19, 0x22);
    return true;
}

// PrepareRecruitList - IDA: 0x1400888C0
// Prepares and sends recruit list request to community server
bool CGocFriend::PrepareRecruitList(PS_RECRUIT_LIST& stRecruit) {
    // Check cooldown
    ATL::CTime currentTime = ATL::CTime::GetTickCount();
    if (m_tNextRecruitTime > currentTime) {
        return false;
    }
    
    // Validate parameters
    if (stRecruit.byClass >= 9) {
        return false;
    }
    if (stRecruit.byLevelMin > stRecruit.byLevelMax) {
        return false;
    }
    
    // Check if already requesting
    PS_RECRUIT_LIST stListTemp;
    if (GetRecruitListReq(&stListTemp)) {
        LogHelper::LogError("game.contents", "PrepareRecruitList error - Already Request ( ucid:%d / %d )", 
                            GetOwnerUser()->GetUCID(), 665);
        return false;
    }
    
    SetRecruitListReq(true, &stRecruit);
    
    CUser* pUser = GetOwnerUser();
    if (!pUser || (pUser->stMyCharInfoEx()->UserDB[1] & 2) == 0) {
        LogHelper::LogError("game.contents", "PrepareRecruitList error - Wait Community Add User ( ucid:%d / %d )", 
                            GetOwnerUser()->GetUCID(), 675);
        return false;
    }
    
    // Set cooldown
    ATL::CTimeSpan span(10);
    m_tNextRecruitTime = currentTime + span;
    
    stRecruit.dwUCID = GetOwnerUser()->GetUCID();
    
    XSendPacket xSendPacket(0xF5, 0x15);
    xSendPacket << stRecruit;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, pUser, 0x19, 0x41);
    return true;
}

// PrepareRecruitAdd - IDA: 0x140088C00
// Prepares and sends recruit add request to community server
bool CGocFriend::PrepareRecruitAdd() {
    PS_RECRUIT_ADD stRecruit;
    stRecruit.dwUCID = GetOwnerUser()->GetUCID();
    
    XSendPacket xSendPacket(0xF5, 0x16);
    xSendPacket << stRecruit;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, GetOwnerUser(), 0x19, 0x42);
    return true;
}

// PrepareRecruitDelete - IDA: 0x140088D30
// Prepares and sends recruit delete request to community server
bool CGocFriend::PrepareRecruitDelete() {
    PS_RECRUIT_ADD stRecruit;  // Uses same structure as add
    stRecruit.dwUCID = GetOwnerUser()->GetUCID();
    
    XSendPacket xSendPacket(0xF5, 0x17);
    xSendPacket << stRecruit;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, GetOwnerUser(), 0x19, 0x43);
    return true;
}

// PrepareRecruitInfo - IDA: 0x140088E60
// Prepares and sends recruit info request to community server
bool CGocFriend::PrepareRecruitInfo() {
    if (GetRecruitInfoReq()) {
        LogHelper::LogError("game.contents", "PrepareRecruitInfo error - Already Request ( ucid:%d / %d )", 
                            GetOwnerUser()->GetUCID(), 725);
        return false;
    }
    
    SetRecruitInfoReq(true);
    
    CUser* pUser = GetOwnerUser();
    if (!pUser || (pUser->stMyCharInfoEx()->UserDB[1] & 2) == 0) {
        LogHelper::LogError("game.contents", "PrepareRecruitInfo error - Wait Community Add User ( ucid:%d / %d )", 
                            GetOwnerUser()->GetUCID(), 735);
        return false;
    }
    
    std::uint32_t dwUCID = pUser->GetUCID();
    
    XSendPacket xSendPacket(0xF5, 0x18);
    xSendPacket << dwUCID;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, pUser, 0x19, 0x44);
    return true;
}

// PrepareRecommandList - IDA: 0x1400890C0
// Prepares and sends friend recommendation request to community server
bool CGocFriend::PrepareRecommandList() {
    // Check cooldown
    ATL::CTime currentTime = ATL::CTime::GetTickCount();
    if (m_tNextRecommandTime > currentTime) {
        return false;
    }
    
    if (GetRecommandListReq()) {
        LogHelper::LogError("game.contents", "PrepareRecommandList error - Already Request ( ucid:%d / %d )", 
                            GetOwnerUser()->GetUCID(), 758);
        return false;
    }
    
    SetRecommandListReq(true);
    
    CUser* pUser = GetOwnerUser();
    if (!pUser || (pUser->stMyCharInfoEx()->UserDB[1] & 2) == 0) {
        LogHelper::LogError("game.contents", "PrepareRecommandList error - Wait Community Add User ( ucid:%d / %d )", 
                            GetOwnerUser()->GetUCID(), 768);
        return false;
    }
    
    PS_RES_FRIEND_RECOMMAND stFriendRes;
    stFriendRes.dwUCID = pUser->GetUCID();
    
    XSendPacket xSendPacket(0xF5, 0x11);
    xSendPacket << stFriendRes;
    CCommunitySocket::SendCmd(TXSingleton<XGameServer>::Instance()->m_communitySocket,
                               &xSendPacket, pUser, 0x19, 0x51);
    
    ResetRecommandTime();
    return true;
}

// FriendInvite - IDA: 0x140089390
// Handles friend invite result from community server
void CGocFriend::FriendInvite(PS_FRIEND_RESULT& stResult) {
    switch (stResult.nResult) {
        case 0:  // Success
            {
                XSendPacket packet(0x19, 0x11);
                packet << stResult;
                CGocNetwork::Send(GetOwnerActor(), &packet);
            }
            break;
        case 1:
            CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x11, 0xD745);
            break;
        case 2:
            CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x11, 0xD73D);
            break;
        case 3:
            CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x11, 0xD747);
            break;
        case 5:
            stResult.nResult = 55107;
            {
                XSendPacket packet(0x19, 0x13);
                packet << stResult;
                CGocNetwork::Send(GetOwnerActor(), &packet);
            }
            break;
        case 6:
            stResult.nResult = 55103;
            {
                XSendPacket packet(0x19, 0x11);
                packet << stResult;
                CGocNetwork::Send(GetOwnerActor(), &packet);
            }
            break;
        case 9:
            stResult.nResult = 59202;
            {
                XSendPacket packet(0x19, 0x11);
                packet << stResult;
                CGocNetwork::Send(GetOwnerActor(), &packet);
            }
            break;
        default:
            break;
    }
}

// FriendAccept - IDA: 0x140089720
// Handles friend accept response from community server
void CGocFriend::FriendAccept(PS_RES_FRIEND_ACCEPT& stAccept) {
    if (stAccept.nResult != 55105) {
        PS_FRIEND_RESULT stResult;
        stResult.nResult = stAccept.nResult;
        std::wcscpy(stResult.strName, stAccept.stFriend.strName);
        
        XSendPacket packet(0x19, 0x13);
        packet << stResult;
        CGocNetwork::Send(GetOwnerActor(), &packet);
    }
}

// AddBlockList - IDA: 0x1400898C0
// Handles block list add response from community server
void CGocFriend::AddBlockList(PS_RES_BLOCKLIST_ADD& stBlock) {
    if (stBlock.stResult.nResult != 0) {
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x21, stBlock.stResult.nResult);
    } else {
        AddBlock(&stBlock.stBlock, true);
    }
}

// DeleteBlockList - IDA: 0x140089920
// Handles block list delete response from community server
void CGocFriend::DeleteBlockList(PS_RES_BLOCKLIST_DELETE& stBlock) {
    if (stBlock.nResult != 0) {
        CGocNetwork::SendErrorMessage(GetOwnerMover(), 0x19, 0x22, stBlock.nResult);
    } else {
        DeleteBlock(stBlock.dwTargetUCID, stBlock.strTargetName, true);
    }
}

// UpdateFriendCommunity - IDA: 0x140089D20
// Updates friend community info and sends update to client
void CGocFriend::UpdateFriendCommunity(std::uint32_t dwUCID, ST_FRIEND_COMMUNITY& stCommunity) {
    // Find friend by UCID
    for (auto* pFriend : m_vecFriends) {
        if (pFriend && pFriend->GetUCID() == dwUCID) {
            // Found - update community info
            pFriend->UpdateInfo(&stCommunity);
            
            // Get updated info and send to client
            ST_FRIEND_INFO stInfo;
            pFriend->GetInfo(&stInfo);
            
            XSendPacket packet(0x19, 0x31);
            packet << stInfo;
            CGocNetwork::Send(GetOwnerActor(), &packet);
            return;
        }
    }
}

// AddFriendPoint - IDA: 0x140089EE0
// Adds friend points to a friend and updates total
void CGocFriend::AddFriendPoint(std::uint32_t dwUCID, std::int64_t nPoint) {
    // Find friend by UCID
    for (auto* pFriend : m_vecFriends) {
        if (pFriend && pFriend->GetUCID() == dwUCID) {
            // Found - get current info and add points
            ST_FRIEND_INFO stInfo;
            pFriend->GetInfo(&stInfo);
            stInfo.nFriendPoint += nPoint;
            pFriend->UpdateInfo(&stInfo);
            
            // Add to owner's total friend points
            CUser* pUser = GetOwnerUser();
            if (pUser) {
                CGocInventory* pInventory = pUser->GetGOC<CGocInventory>();
                if (pInventory) {
                    pInventory->AddTotalFriendPoint(nPoint, false);
                }
            }
            
            // Send updated info to client
            XSendPacket packet(0x19, 0x31);
            packet << stInfo;
            CGocNetwork::Send(GetOwnerActor(), &packet);
            return;
        }
    }
}
