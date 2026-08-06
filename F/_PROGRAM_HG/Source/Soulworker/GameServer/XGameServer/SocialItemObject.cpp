// SocialItemObject.cpp
// CSocialItemObject implementation
// IDA decompilation from GameServer.exe

#include "SocialItemObject.h"
#include <new>
#include <cstring>

// Static type info for RTTI
VType* CSocialItemObject::classCSocialItemObject = nullptr;

// ============================================================================
// CSocialItemObject::CreateObject - Create new social item object
// IDA @ 0x14018B580
// ============================================================================
CSocialItemObject* CSocialItemObject::CreateObject()
{
    // IDA code:
    // CSocialItemObject *__fastcall CSocialItemObject::CreateObject()
    // {
    //   CSocialItemObject *v1; // [rsp+28h] [rbp-20h]
    //
    //   v1 = (CSocialItemObject *)VBaseObject::operator new(0xED08u);
    //   if ( v1 )
    //     return CSocialItemObject::CSocialItemObject(v1);
    //   else
    //     return nullptr;
    // }

    // Allocate memory for the object (size 0xED08 = 60680 bytes)
    void* pMemory = ::operator new(sizeof(CSocialItemObject));
    if (pMemory)
    {
        return new (pMemory) CSocialItemObject();
    }
    return nullptr;
}

// ============================================================================
// CSocialItemObject::GetTypeId - Get type ID for RTTI
// IDA @ 0x14018B5E0
// ============================================================================
VType* CSocialItemObject::GetTypeId() const
{
    // IDA code:
    // VType *__fastcall CSocialItemObject::GetTypeId(CSocialItemObject *this)
    // {
    //   return &CSocialItemObject::classCSocialItemObject;
    // }
    return classCSocialItemObject;
}

// ============================================================================
// CSocialItemObject::InitComponant - Initialize components
// IDA @ 0x14018BC10
// ============================================================================
void CSocialItemObject::InitComponant()
{
    // IDA code:
    // void __fastcall CSocialItemObject::InitComponant(CVaccumCube *this)
    // {
    //   std::tr1::shared_ptr<CGocAttribute> result; // [rsp+20h] [rbp-18h] BYREF
    //
    //   GOComponent::CreateAndRegister<CGocAttribute>(&result, this);
    //   std::tr1::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>((std::tr1::shared_ptr<CGocNetwork> *)&result);
    // }

    // Create and register CGocAttribute component
    // This is called during object initialization to set up the attribute component
    // The component is automatically destroyed when the shared_ptr goes out of scope
    
    // TODO: Implement when GOComponent/CGocAttribute available
    // std::tr1::shared_ptr<CGocAttribute> attrComponent;
    // GOComponent::CreateAndRegister<CGocAttribute>(&attrComponent, this);
}

// ============================================================================
// CSocialItemObject::BuildInfoPacket - Build info packet
// IDA @ 0x14018BC70
// ============================================================================
void CSocialItemObject::BuildInfoPacket(ST_SOCIAL_ITEM_RES* stInfo)
{
    // IDA code:
    // void __fastcall CSocialItemObject::BuildInfoPacket(CSocialItemObject *this, ST_SOCIAL_ITEM_RES *stInfo)
    // {
    //   stInfo->itemInfo.dwObjectID = this->m_itemInfo.dwObjectID;
    //   stInfo->itemInfo.dwOwnerID = this->m_itemInfo.dwOwnerID;
    //   stInfo->itemInfo.dwItemID = this->m_itemInfo.dwItemID;
    //   stInfo->itemInfo.wSocialItemID = this->m_itemInfo.wSocialItemID;
    //   stInfo->itemInfo.lRemainTime = this->m_itemInfo.lRemainTime;
    //   std::vector<float>::clear((std::vector<float> *)&stInfo->itemInfo.vecUsers);
    //   // ... copy vecUsers
    //   for ( i = 0; i < 4; ++i )
    //     stInfo->itemInfo.stUsedSlot[i] = this->m_itemInfo.stUsedSlot[i];
    //   stInfo->fPosX = this->m_vPosition.x;
    //   stInfo->fPosY = this->m_vPosition.y;
    //   stInfo->fPosZ = this->m_vPosition.z;
    //   stInfo->fRot = this->m_fRot;
    // }

    if (!stInfo)
        return;

    // Copy item info
    stInfo->itemInfo.dwObjectID = m_itemInfo.dwObjectID;
    stInfo->itemInfo.dwOwnerID = m_itemInfo.dwOwnerID;
    stInfo->itemInfo.dwItemID = m_itemInfo.dwItemID;
    stInfo->itemInfo.wSocialItemID = m_itemInfo.wSocialItemID;
    stInfo->itemInfo.lRemainTime = m_itemInfo.lRemainTime;

    // Copy user list
    stInfo->itemInfo.vecUsers.clear();
    stInfo->itemInfo.vecUsers = m_itemInfo.vecUsers;

    // Copy used slots
    for (int i = 0; i < 4; ++i)
    {
        stInfo->itemInfo.stUsedSlot[i] = m_itemInfo.stUsedSlot[i];
    }

    // Copy position and rotation
    stInfo->fPosX = m_vPosition[0];
    stInfo->fPosY = m_vPosition[1];
    stInfo->fPosZ = m_vPosition[2];
    stInfo->fRot = m_fRot;
}

// ============================================================================
// CSocialItemObject::SetInfoPacket - Set info packet for network send
// IDA @ 0x14018BDE0
// ============================================================================
void CSocialItemObject::SetInfoPacket(XSendPacket& xSendPacket)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetInfoPacket(CSocialItemObject *this, XSendPacket *xSendPacket)
    // {
    //   ST_SOCIAL_ITEM_RES stInfo; // [rsp+30h] [rbp-88h] BYREF
    //
    //   ST_SOCIAL_ITEM_RES::ST_SOCIAL_ITEM_RES(&stInfo);
    //   CSocialItemObject::BuildInfoPacket((CSocialItemObject *)((char *)this - 872), &stInfo);
    //   operator<<(xSendPacket, &stInfo);
    //   ST_SOCIAL_ITEM_RES::~ST_SOCIAL_ITEM_RES(&stInfo);
    // }

    ST_SOCIAL_ITEM_RES stInfo;
    BuildInfoPacket(&stInfo);

    // Serialize the info structure to the packet
    // TODO: Implement packet serialization when XSendPacket operator<< available
    // xSendPacket << stInfo;
    
    // Manual serialization for now
    xSendPacket.XParse.SetBytes(
        reinterpret_cast<const char*>(&stInfo), sizeof(ST_SOCIAL_ITEM_RES));
}

// ============================================================================
// CSocialItemObject::IsExistUser - Check if user exists
// IDA @ 0x14018C2A0
// ============================================================================
bool CSocialItemObject::IsExistUser(std::uint32_t dwActorID)
{
    // IDA code:
    // char __fastcall CSocialItemObject::IsExistUser(CSocialItemObject *this, unsigned int dwActorID)
    // {
    //   int i; // [rsp+20h] [rbp-18h]
    //
    //   for ( i = 0; i < std::vector<PS_ITEM_SLOT_INFO>::size((std::vector<float> *)&this->m_itemInfo.vecUsers); ++i )
    //   {
    //     if ( std::vector<ST_CHANNEL_INFO>::operator[]((std::vector<UXActorID> *)&this->m_itemInfo.vecUsers, i)->dwActorID == dwActorID )
    //       return 1;
    //   }
    //   return 0;
    // }

    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        if (m_itemInfo.vecUsers[i] == dwActorID)
        {
            return true;
        }
    }
    return false;
}

// ============================================================================
// CSocialItemObject::AddUser - Add user to social item
// IDA @ 0x14018BEC0
// ============================================================================
bool CSocialItemObject::AddUser(std::uint32_t dwActorID, int* nSlot, int byAniIndex)
{
    // IDA code (complex logic):
    // - Check if user already exists
    // - Handle different social types (1=normal, 2=owner_first, 3=party)
    // - Find available slot
    // - Update play state

    // Check if user already exists
    if (IsExistUser(dwActorID))
    {
        return false;
    }

    // Owner handling for social type 2
    if (m_bySocialType == E_SOCIAL_OBJECT_TYPE_OWNER_FIRST && m_itemInfo.dwOwnerID == dwActorID)
    {
        *nSlot = 0;
        m_itemInfo.vecUsers.push_back(dwActorID);
        return true;
    }

    // Owner handling for other types
    if (m_itemInfo.dwOwnerID == dwActorID)
    {
        m_itemInfo.vecUsers.push_back(dwActorID);
        if (m_bySocialType == E_SOCIAL_OBJECT_TYPE_PARTY)
        {
            m_byPlayState = E_SOCIAL_OBJECT_STATE_WAIT;
            if (m_itemInfo.vecUsers.size() == m_byMaxUserCount)
            {
                m_byPlayState = E_SOCIAL_OBJECT_STATE_READY;
            }
        }
        return true;
    }

    // Check capacity
    bool bOwnerExists = IsExistUser(m_itemInfo.dwOwnerID);
    if (!bOwnerExists || m_bySocialType == E_SOCIAL_OBJECT_TYPE_OWNER_FIRST || m_bySocialType == E_SOCIAL_OBJECT_TYPE_PARTY)
    {
        if (m_itemInfo.vecUsers.size() >= m_byMaxUserCount)
        {
            return false;
        }
    }
    else
    {
        if (m_itemInfo.vecUsers.size() >= static_cast<size_t>(m_byMaxUserCount) + 1)
        {
            return false;
        }
    }

    bool bFind = true;

    // Find available slot based on social type
    switch (m_bySocialType)
    {
    case E_SOCIAL_OBJECT_TYPE_NORMAL:
        bFind = false;
        for (int i = 0; i < m_byMaxUserCount; ++i)
        {
            if (m_itemInfo.stUsedSlot[i].dwUserID == 0)
            {
                m_itemInfo.stUsedSlot[i].dwUserID = dwActorID;
                m_itemInfo.stUsedSlot[i].byAniIndex = static_cast<std::uint8_t>(byAniIndex);
                bFind = true;
                *nSlot = i;
                break;
            }
        }
        break;

    case E_SOCIAL_OBJECT_TYPE_OWNER_FIRST:
        bFind = false;
        for (int j = 1; j < m_byMaxUserCount; ++j)
        {
            if (m_itemInfo.stUsedSlot[j].dwUserID == 0)
            {
                m_itemInfo.stUsedSlot[j].dwUserID = dwActorID;
                m_itemInfo.stUsedSlot[j].byAniIndex = static_cast<std::uint8_t>(byAniIndex);
                bFind = true;
                *nSlot = j;
                break;
            }
        }
        break;

    case E_SOCIAL_OBJECT_TYPE_PARTY:
        // Check if all users are owner or owner's party members
        for (size_t k = 0; k < m_itemInfo.vecUsers.size(); ++k)
        {
            if (m_itemInfo.dwOwnerID != m_itemInfo.vecUsers[k] && m_itemInfo.dwOwnerID != dwActorID)
            {
                return false;
            }
        }
        break;
    }

    if (bFind)
    {
        m_itemInfo.vecUsers.push_back(dwActorID);
        if (m_bySocialType == E_SOCIAL_OBJECT_TYPE_PARTY)
        {
            m_byPlayState = E_SOCIAL_OBJECT_STATE_WAIT;
            if (m_itemInfo.vecUsers.size() == m_byMaxUserCount)
            {
                m_byPlayState = E_SOCIAL_OBJECT_STATE_READY;
            }
        }
        return true;
    }

    return false;
}

// ============================================================================
// CSocialItemObject::DeleteUser - Delete user from social item
// IDA @ 0x14018C320
// ============================================================================
bool CSocialItemObject::DeleteUser(std::uint32_t dwActorID)
{
    // IDA: Complex function that removes user from vecUsers and stUsedSlot
    // Returns true if user was found and deleted

    if (m_itemInfo.vecUsers.empty())
    {
        return false;
    }

    bool bRes = false;

    // Find and remove user from vecUsers
    for (auto it = m_itemInfo.vecUsers.begin(); it != m_itemInfo.vecUsers.end(); ++it)
    {
        if (*it == dwActorID)
        {
            m_itemInfo.vecUsers.erase(it);
            bRes = true;
            break;
        }
    }

    // Clear slot for social types 1 and 2
    if (bRes && (m_bySocialType == E_SOCIAL_OBJECT_TYPE_NORMAL || m_bySocialType == E_SOCIAL_OBJECT_TYPE_OWNER_FIRST))
    {
        for (int i = 0; i < m_byMaxUserCount; ++i)
        {
            if (m_itemInfo.stUsedSlot[i].dwUserID == dwActorID)
            {
                m_itemInfo.stUsedSlot[i].dwUserID = 0;
                m_itemInfo.stUsedSlot[i].byAniIndex = 0;
                break;
            }
        }
    }

    // Update play state
    if (m_byPlayState == E_SOCIAL_OBJECT_STATE_READY)
    {
        m_byPlayState = E_SOCIAL_OBJECT_STATE_WAIT;
    }

    return bRes;
}

// ============================================================================
// CSocialItemObject::SetSocialType - Set social type
// IDA @ 0x14018C850
// ============================================================================
void CSocialItemObject::SetSocialType(std::uint8_t bySocialType)
{
    // IDA: this->m_bySocialType = bySocialType;
    m_bySocialType = bySocialType;
}

// ============================================================================
// CSocialItemObject::IsFunniture - Check if this is furniture
// IDA @ 0x14018C870
// ============================================================================
bool CSocialItemObject::IsFunniture()
{
    // IDA: return this->m_bySocialType && this->m_bySocialType != 3;
    return m_bySocialType != 0 && m_bySocialType != E_SOCIAL_OBJECT_TYPE_PARTY;
}

// ============================================================================
// CSocialItemObject::SetInfoLeavePacket - Set leave info packet
// IDA @ 0x14018BE90
// ============================================================================
void CSocialItemObject::SetInfoLeavePacket(XSendPacket& xSendPacket)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetInfoLeavePacket(CSocialItemObject *this, XSendPacket *xSendPacket)
    // {
    //   operator<<(xSendPacket, &this->m_itemInfo);
    // }
    
    // Serialize the item info structure to the packet
    // TODO: Implement packet serialization when XSendPacket operator<< available
    // xSendPacket << m_itemInfo;
    
    // Manual serialization for now
    xSendPacket.XParse.SetBytes(
        reinterpret_cast<const char*>(&m_itemInfo), sizeof(ST_SOCIAL_ITEM_INFO));
}

// ============================================================================
// CSocialItemObject::SetFurnitureInfo - Set furniture max user count
// IDA @ 0x14018C830
// ============================================================================
void CSocialItemObject::SetFurnitureInfo(std::uint8_t nMaxUseNum)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetFurnitureInfo(CSocialItemObject *this, unsigned __int8 nMaxUseNum)
    // {
    //   this->m_byMaxUserCount = nMaxUseNum;
    // }
    m_byMaxUserCount = nMaxUseNum;
}

// ============================================================================
// CSocialItemObject::SendPlayInfo - Send play info to all users
// IDA @ 0x14018C8A0
// ============================================================================
void CSocialItemObject::SendPlayInfo(PS_SOCIALITEM_PLAY* psPlayInfo)
{
    // IDA code: Iterates through all users and sends play info packet
    // for ( i = 0; i < vecUsers.size(); ++i )
    // {
    //   dwActorID = vecUsers[i]->dwActorID;
    //   pUser = XGameServer::FindActorIDToUser(dwActorID);
    //   if ( pUser )
    //   {
    //     XSendPacket xSendPacket(0x2D, 9);
    //     xSendPacket << psPlayInfo;
    //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    //   }
    // }

    if (!psPlayInfo)
        return;

    // Iterate through all users and send play info packet
    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        std::uint32_t dwActorID = m_itemInfo.vecUsers[i];
        
        // TODO: Implement when XGameServer/XSendPacket/CGocNetwork available
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // CUser* pUser = pServer->FindActorIDToUser(dwActorID);
        // if (pUser)
        // {
        //     XSendPacket xSendPacket(0x2D, 9);  // Main=0x2D, Sub=9
        //     xSendPacket << *psPlayInfo;
        //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
        // }
    }
}

// ============================================================================
// CSocialItemObject::GetPlayNextTurn - Get next turn player UCID
// IDA @ 0x14018CA10
// ============================================================================
std::uint32_t CSocialItemObject::GetPlayNextTurn()
{
    // IDA code:
    // unsigned int __fastcall CSocialItemObject::GetPlayNextTurn(CSocialItemObject *this)
    // {
    //   if ( this->m_dwTurnUCID == this->m_itemInfo.dwOwnerID )
    //     return CSocialItemObject::GetPlayGuestID(this);
    //   else
    //     return this->m_itemInfo.dwOwnerID;
    // }
    if (m_dwTurnUCID == m_itemInfo.dwOwnerID)
    {
        return GetPlayGuestID();
    }
    return m_itemInfo.dwOwnerID;
}

// ============================================================================
// CSocialItemObject::IsUsePlaySocialItem - Check if can be used for play
// IDA @ 0x14018DAB0
// ============================================================================
bool CSocialItemObject::IsUsePlaySocialItem()
{
    // IDA code:
    // bool __fastcall CSocialItemObject::IsUsePlaySocialItem(CSocialItemObject *this)
    // {
    //   return this->m_bySocialType != 3 || this->m_byPlayState < (unsigned int)E_SOCIAL_OBJECT_STATE_READY;
    // }
    return m_bySocialType != E_SOCIAL_OBJECT_TYPE_PARTY || m_byPlayState < E_SOCIAL_OBJECT_STATE_READY;
}

// ============================================================================
// CSocialItemObject::GetPlayGuestID - Get guest player UCID
// IDA @ 0x14018DAF0
// ============================================================================
std::uint32_t CSocialItemObject::GetPlayGuestID()
{
    // IDA code:
    // unsigned __int64 __fastcall CSocialItemObject::GetPlayGuestID(CSocialItemObject *this)
    // {
    //   for ( i = 0; i < vecUsers.size(); ++i )
    //   {
    //     if ( this->m_itemInfo.dwOwnerID != vecUsers[i]->dwActorID )
    //       return vecUsers[i]->dwActorID;
    //   }
    //   return 0;
    // }
    if (m_bySocialType != E_SOCIAL_OBJECT_TYPE_PARTY)
        return 0;

    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        if (m_itemInfo.dwOwnerID != m_itemInfo.vecUsers[i])
        {
            return m_itemInfo.vecUsers[i];
        }
    }
    return 0;
}

// ============================================================================
// CSocialItemObject::FinishPlaySocialItemObject - Finish play and send rewards
// IDA @ 0x14018D3B0
// ============================================================================
void CSocialItemObject::FinishPlaySocialItemObject(std::uint32_t dwLeaveUCID)
{
    // IDA: Complex function handling game finish
    // - If play state is START, set to FINISH
    // - Reset play count and clear card info map
    // - For owner who left: send lose reward
    // - For other players: send win reward
    // - Log game result to DB

    if (m_byPlayState != E_SOCIAL_OBJECT_STATE_START)
        return;

    m_byPlayState = E_SOCIAL_OBJECT_STATE_FINISH;
    m_byPlayCount = 0;
    m_byMaxPlayCount = 0;
    m_mpCardInfo.clear();

    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        std::uint32_t dwActorID = m_itemInfo.vecUsers[i];

        if (dwActorID == dwLeaveUCID)
        {
            // Owner who left - send lose reward
            if (dwActorID == m_itemInfo.dwOwnerID)
            {
                // TODO: Implement when XGameServer/CGocPost/XResourceMgr available
                // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                // CUser* pUser = pServer->FindActorIDToUser(dwActorID);
                // if (pUser)
                // {
                //     auto pPostPtr = pUser->GetGOC<CGocPost>();
                //     if (pPostPtr)
                //     {
                //         TB_MODE_CARDMATCH_RULE* pRule = XResourceMgr::GetTB_MODE_CARDMATCH_RULE(m_itemInfo.wSocialItemID);
                //         if (pRule)
                //         {
                //             TB_ITEM* pLoseItem = XResourceMgr::GetTB_ITEM(pRule->Lose_Reward);
                //             if (pLoseItem)
                //             {
                //                 pPostPtr->SystemPostSend(pLoseItem, 1, 5, 9, 0, nullptr);
                //             }
                //         }
                //     }
                // }
            }
        }
        else
        {
            // Other player - send win reward
            // TODO: Implement when XGameServer/CGocPost/XResourceMgr available
            // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            // CUser* pUser = pServer->FindActorIDToUser(dwActorID);
            // if (pUser)
            // {
            //     auto pPostPtr = pUser->GetGOC<CGocPost>();
            //     if (pPostPtr)
            //     {
            //         TB_MODE_CARDMATCH_RULE* pRule = XResourceMgr::GetTB_MODE_CARDMATCH_RULE(m_itemInfo.wSocialItemID);
            //         if (pRule)
            //         {
            //             TB_ITEM* pWinItem = XResourceMgr::GetTB_ITEM(pRule->Win_Reward);
            //             if (pWinItem)
            //             {
            //                 pPostPtr->SystemPostSend(pWinItem, 1, 5, 8, 0, nullptr);
            //                 CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2D, 9, 0xE7B0);
            //             }
            //         }
            //     }
            // }
            break;  // Only one winner
        }
    }

    // TODO: Add text DB log when CTextDBLog available
    // CTextDBLog::AddLog(m_textDBLog, 55, m_itemInfo.dwObjectID, dwLeaveUCID, 0, 0, 0, 0, 0, dwLeaveUCID);
    // CTextDBLog::SendLogDB(m_textDBLog, m_bSendLogDB);
    m_bSendLogDB = false;
}

// ============================================================================
// CSocialItemObject::GetOtherInfo - Get other player's UCID
// IDA @ 0x14018DE70
// ============================================================================
std::uint32_t CSocialItemObject::GetOtherInfo(std::uint32_t dwUCID)
{
    // IDA code:
    // __int64 __fastcall CSocialItemObject::GetOtherInfo(CSocialItemObject *this, unsigned int dwUCID)
    // {
    //   for ( i = 0; i < vecUsers.size(); ++i )
    //   {
    //     if ( dwUCID != vecUsers[i]->dwActorID )
    //       return vecUsers[i]->dwActorID;
    //   }
    //   return 0;
    // }
    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        if (dwUCID != m_itemInfo.vecUsers[i])
        {
            return m_itemInfo.vecUsers[i];
        }
    }
    return 0;
}

// ============================================================================
// CSocialItemObject::CheckRemainTime - Check and update remaining time
// IDA @ 0x14018C7B0
// ============================================================================
bool CSocialItemObject::CheckRemainTime(float fElapsedTime)
{
    // IDA code:
    // char __fastcall CSocialItemObject::CheckRemainTime(CSocialItemObject *this, float fElapsedTime)
    // {
    //   if ( this->m_itemInfo.lRemainTime <= 0 )
    //     return 0;
    //   this->m_itemInfo.lRemainTime = (unsigned int)(int)(float)((float)(int)this->m_itemInfo.lRemainTime - (float)(fElapsedTime * 1000.0));
    //   if ( this->m_itemInfo.lRemainTime > 0 )
    //     return 0;
    //   this->m_itemInfo.lRemainTime = 0;
    //   return 1;
    // }
    if (m_itemInfo.lRemainTime <= 0)
        return false;

    m_itemInfo.lRemainTime = static_cast<std::int64_t>(
        static_cast<float>(m_itemInfo.lRemainTime) - (fElapsedTime * 1000.0f)
    );

    if (m_itemInfo.lRemainTime > 0)
        return false;

    m_itemInfo.lRemainTime = 0;
    return true;
}

// ============================================================================
// CSocialItemObject::GetOwnerID - Get owner ID
// IDA @ 0x14018FC40
// ============================================================================
std::uint32_t CSocialItemObject::GetOwnerID()
{
    // IDA code:
    // __int64 __fastcall CSocialItemObject::GetOwnerID(CSocialItemObject *this)
    // {
    //   return this->m_itemInfo.dwOwnerID;
    // }
    return m_itemInfo.dwOwnerID;
}

// ============================================================================
// CSocialItemObject::GetRadius - Get radius
// IDA @ 0x1402D36C0
// ============================================================================
float CSocialItemObject::GetRadius()
{
    // IDA code:
    // float __fastcall CSocialItemObject::GetRadius(CSocialItemObject *this)
    // {
    //   return this->m_fRadius;
    // }
    return m_fRadius;
}

// ============================================================================
// CSocialItemObject::SetMaxCount - Set max user count
// IDA @ 0x1402D3730
// ============================================================================
void CSocialItemObject::SetMaxCount(std::uint8_t byCount)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetMaxCount(CSocialItemObject *this, unsigned __int8 byCount)
    // {
    //   this->m_byMaxUserCount = byCount;
    // }
    m_byMaxUserCount = byCount;
}

// ============================================================================
// CSocialItemObject::SetRadius - Set radius
// IDA @ 0x1402D3750
// ============================================================================
void CSocialItemObject::SetRadius(float fRadius)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetRadius(CSocialItemObject *this, float fRadius)
    // {
    //   this->m_fRadius = fRadius;
    // }
    m_fRadius = fRadius;
}

// ============================================================================
// CSocialItemObject::GetSocialItemID - Get social item ID
// IDA @ 0x140601C40
// ============================================================================
std::uint16_t CSocialItemObject::GetSocialItemID()
{
    // IDA code:
    // __int64 __fastcall CSocialItemObject::GetSocialItemID(CSocialItemObject *this)
    // {
    //   return this->m_itemInfo.wSocialItemID;
    // }
    return m_itemInfo.wSocialItemID;
}

// ============================================================================
// CSocialItemObject::SetRemainTime - Set remaining time
// IDA @ 0x140601CB0
// ============================================================================
void CSocialItemObject::SetRemainTime(std::uint32_t dwDuration)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetRemainTime(CSocialItemObject *this, unsigned int dwDuration)
    // {
    //   this->m_itemInfo.lRemainTime = dwDuration;
    // }
    m_itemInfo.lRemainTime = static_cast<std::int64_t>(dwDuration);
}

// ============================================================================
// CSocialItemObject::SetItemSerialID - Set item serial ID
// IDA @ 0x140601CD0
// ============================================================================
void CSocialItemObject::SetItemSerialID(std::int64_t xSerial)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetItemSerialID(CSocialItemObject *this, __int64 xSerial)
    // {
    //   this->m_xItemSerial = xSerial;
    // }
    m_xItemSerial = xSerial;
}

// ============================================================================
// CSocialItemObject::SetItemID - Set item ID
// IDA @ 0x140601CF0
// ============================================================================
void CSocialItemObject::SetItemID(std::uint32_t dwItemID)
{
    // IDA code:
    // void __fastcall CSocialItemObject::SetItemID(CSocialItemObject *this, unsigned int dwItemID)
    // {
    //   this->m_itemInfo.dwItemID = dwItemID;
    // }
    m_itemInfo.dwItemID = dwItemID;
}

// ============================================================================
// CSocialItemObject::GetSocialPlayState - Get social play state
// IDA @ 0x1402D3A60
// ============================================================================
E_SOCIAL_OBJECT_STATE CSocialItemObject::GetSocialPlayState()
{
    // IDA code:
    // __int64 __fastcall CSocialItemObject::GetSocialPlayState(CSocialItemObject *this)
    // {
    //   return (unsigned int)this->m_byPlayState;
    // }
    return static_cast<E_SOCIAL_OBJECT_STATE>(m_byPlayState);
}

// ============================================================================
// CSocialItemObject::GetItemInfo - Get item info reference
// IDA @ 0x1402D3A80
// ============================================================================
ST_SOCIAL_ITEM_INFO& CSocialItemObject::GetItemInfo()
{
    // IDA code:
    // ST_SOCIAL_ITEM_INFO *__fastcall CSocialItemObject::GetItemInfo(CSocialItemObject *this)
    // {
    //   return &this->m_itemInfo;
    // }
    return m_itemInfo;
}

// ============================================================================
// CSocialItemObject::GetItemSerialID - Get item serial ID
// IDA @ 0x14048CF90
// ============================================================================
std::int64_t CSocialItemObject::GetItemSerialID()
{
    // IDA code:
    // __int64 __fastcall CSocialItemObject::GetItemSerialID(CSocialItemObject *this)
    // {
    //   return this->m_xItemSerial;
    // }
    return m_xItemSerial;
}

// ============================================================================
// CSocialItemObject::Init - Initialize social item with area, position, owner
// IDA @ 0x14018B890
// ============================================================================
void CSocialItemObject::Init(XArea* pArea, XVec3* vecPos, float fRot, std::uint32_t dwOwnerID, std::uint16_t wItemID)
{
    // IDA: Complex initialization logic
    // - Set position via SetPosInfo
    // - Set map instance ID from area
    // - Set world ID from area
    // - Set position via VisObject3D_cl::SetPosition
    // - Set actor type to eActorSocialItemObject
    // - Initialize m_itemInfo with owner and item ID
    // - Reset all play state variables

    // TODO: Implement when XArea/XVec3/XActor/VisObject3D_cl available
    // XActor::SetPosInfo(this, vecPos);
    // XActor::SetMapInsID(this, pArea->GetInstanceID());
    // XActor::SetWorldID(this, pArea->GetTBMapID());
    // VisObject3D_cl::SetPosition(this, vecPos->x, vecPos->y, vecPos->z);
    // SetDirectionYaw(fRot);
    // m_eActorType = eActorSocialItemObject;

    // Store position
    if (vecPos)
    {
        m_vPosition[0] = vecPos->x;
        m_vPosition[1] = vecPos->y;
        m_vPosition[2] = vecPos->z;
    }
    m_fRot = fRot;

    // Initialize item info
    // TODO: Generate proper actor ID when XActor available
    // m_itemInfo.dwObjectID = GenerateActorID();
    m_itemInfo.dwOwnerID = dwOwnerID;
    m_itemInfo.wSocialItemID = wItemID;
    m_xItemSerial = 0;
    m_bySocialType = 0;
    m_byPlayState = E_SOCIAL_OBJECT_STATE_NONE;
    m_byPlayCount = 0;
    m_byMaxPlayCount = 0;
    m_mpCardInfo.clear();
    m_dwTurnUCID = 0;
    m_byTurnCnt = 0;
    m_dwCheckCardID = 0;
    m_vecPlayerInfo.clear();
    m_byBonus = 0;
    m_bSendLogDB = false;
    m_byReverseCount = 0;
    
    // TODO: Initialize text DB log when CTextDBLog available
    // CTextDBLog::Init(m_textDBLog, pArea->GetInstanceID());
}

// ============================================================================
// CSocialItemObject::EndProcess - End process and cleanup users
// IDA @ 0x14018C6D0
// ============================================================================
void CSocialItemObject::EndProcess()
{
    // IDA code: Iterates through all users and cleans up
    // for (auto& user : m_itemInfo.vecUsers)
    // {
    //     CUser* pUser = XGameServer::FindActorIDToUser(user);
    //     if (pUser)
    //     {
    //         CMoverEx::RemoveAuraSkill(&pUser->CMoverEx, 1);
    //         CUser::SetSocialUseID(pUser, 0);
    //     }
    // }

    // Iterate through all users and clean up
    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        std::uint32_t dwActorID = m_itemInfo.vecUsers[i];
        
        // TODO: Implement when XGameServer/CUser/CMoverEx available
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // CUser* pUser = pServer->FindActorIDToUser(dwActorID);
        // if (pUser)
        // {
        //     pUser->CMoverEx.RemoveAuraSkill(1);
        //     pUser->SetSocialUseID(0);
        // }
    }
}

// ============================================================================
// CSocialItemObject::GetActorID - Get actor ID
// IDA @ 0x14018BC40
// ============================================================================
std::uint32_t CSocialItemObject::GetActorID()
{
    // IDA code:
    // VBitmask *__fastcall CSocialItemObject::GetActorID(CSocialItemObject *this, VBitmask *result)
    // {
    //   ATL::CAtlMap<int,IXObject *,ATL::CElementTraits<int>,ATL::CElementTraits<IXObject *>>::CPair::CPair(
    //     result,
    //     LODWORD(this->m_fSkillSkipCoolTime));
    //   return result;
    // }
    // Note: Returns actor ID from m_itemInfo.dwObjectID
    return m_itemInfo.dwObjectID;
}

// ============================================================================
// CSocialItemObject::StartPlaySocialItem - Start play social item game
// IDA @ 0x14018DF00
// ============================================================================
bool CSocialItemObject::StartPlaySocialItem(PS_SOCIAL_ITEM_PLAY_START* psStart)
{
    // IDA: Complex game start logic
    // - Check card size (must be 18 pairs = 36 cards)
    // - Initialize play state, turn, bonus, reverse count
    // - Load card info from TB_MODE_CARDMATCH_RULE and TB_MODE_CARDMATCH_CARD
    // - Insert cards into m_mpCardInfo map

    if (!psStart)
        return false;

    m_byMaxPlayCount = 1;

    // Check if card count is correct (18 pairs = 36 cards)
    // TODO: Check actual vector size from psStart when structure defined
    // size_t cardSize = psStart->vecMission.size();
    // if (cardSize != 36)
    // {
    //     LogHelper::LogError("game.contents", "StartPlaySocialItem error - CardSize[%d]", cardSize);
    //     return false;
    // }

    m_byPlayState = E_SOCIAL_OBJECT_STATE_START;
    m_dwTurnUCID = m_itemInfo.dwOwnerID;
    m_byTurnCnt = 0;
    m_dwCheckCardID = 0;
    m_byBonus = 0;
    m_byReverseCount = 0;

    // Process cards from psStart and insert into m_mpCardInfo
    // TODO: Implement when PS_SOCIAL_ITEM_PLAY_START structure defined
    // for (size_t i = 0; i < psStart->vecMission.size(); ++i)
    // {
    //     ST_SOCIALITEM_CARD& stCard = psStart->vecMission[i];
    //     
    //     // Verify TB_MODE_CARDMATCH_RULE exists
    //     TB_MODE_CARDMATCH_RULE* pRule = XResourceMgr::GetTB_MODE_CARDMATCH_RULE(m_itemInfo.wSocialItemID);
    //     if (!pRule)
    //     {
    //         LogHelper::LogError("game.contents", "StartPlaySocialItem error - No TB_MODE_CARDMATCH_RULE[%d]", 
    //             m_itemInfo.wSocialItemID);
    //         return false;
    //     }
    //     
    //     // Verify TB_MODE_CARDMATCH_CARD exists
    //     TB_MODE_CARDMATCH_CARD* pCard = XResourceMgr::GetTB_MODE_CARDMATCH_CARD(stCard.dwCardID);
    //     if (!pCard)
    //     {
    //         LogHelper::LogError("game.contents", "StartPlaySocialItem error - No TB_MODE_CARDMATCH_CARD[%d]", 
    //             stCard.dwCardID);
    //         return false;
    //     }
    //     
    //     // Insert into m_mpCardInfo with use count 0
    //     m_mpCardInfo[stCard.dwCardID] = 0;
    // }

    return true;
}

// ============================================================================
// CSocialItemObject::SendStartInfo - Send start info to all users
// IDA @ 0x14018E1C0
// ============================================================================
void CSocialItemObject::SendStartInfo(PS_SOCIAL_ITEM_PLAY_START* psStartInfo)
{
    // IDA: Sends start info packet to all users in vecUsers
    // - Packet main=0x2D, sub=8
    // - Logs game start with ST_LOG_GAME (main=20, sub=3)

    if (!psStartInfo)
        return;

    // Iterate through all users and send start info packet
    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        std::uint32_t dwActorID = m_itemInfo.vecUsers[i];
        
        // TODO: Implement when XGameServer/XSendPacket/CGocNetwork available
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // CUser* pUser = pServer->FindActorIDToUser(dwActorID);
        // if (pUser)
        // {
        //     XSendPacket xSendPacket(0x2D, 8);
        //     xSendPacket << *psStartInfo;
        //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
        //     
        //     // Log game start
        //     CTextDBLog::AddLog(m_textDBLog, 53, m_itemInfo.dwObjectID, dwActorID, 0, 0, 0, 0, 0, 0);
        //     
        //     // Send DB log
        //     ST_LOG_GAME stLog;
        //     stLog._nUCID = pUser->GetActorID();
        //     stLog._nUAID = pUser->GetUAID();
        //     stLog._sMainType = 20;
        //     stLog._sSubType = 3;
        //     stLog.nParam0 = m_itemInfo.dwItemID;
        //     stLog.nParam1 = m_itemInfo.dwOwnerID;
        //     stLog.nParam2 = GetOtherInfo(dwActorID);
        //     stLog.nParam5 = m_itemInfo.dwObjectID;
        //     pServer->SendDBLog(&stLog);
        // }
    }
}

// ============================================================================
// CSocialItemObject::IsPlayGame - Check and process play game
// IDA @ 0x14018CA50
// ============================================================================
int CSocialItemObject::IsPlayGame(std::uint32_t dwUCID, PS_SOCIALITEM_PLAY* psPlay)
{
    // IDA: Complex game logic with multiple result codes:
    // - nResult = 0: Success
    // - nResult = 1: Not in START state
    // - nResult = 2: Not this player's turn
    // - nResult = 3: Card not found in m_mpCardInfo
    // - nResult = 4: Card already used
    // - nResult = 5: Game finish but not all cards matched
    // - nResult = 6: Too many cards matched
    // - nResult = 10: Turn change (nType=1)

    if (!psPlay)
        return 1;

    // Check if in START state
    if (m_byPlayState != E_SOCIAL_OBJECT_STATE_START)
    {
        m_bSendLogDB = true;
        return 1;
    }

    // Type 1 = turn change request
    if (psPlay->nType == 1)
    {
        // Change turn
        std::uint32_t nOldTurn = m_dwTurnUCID;
        if (psPlay->psOwnerInfo.bTurn)
            m_dwTurnUCID = m_itemInfo.dwOwnerID;
        else
            m_dwTurnUCID = GetPlayGuestID();
        
        m_byBonus = 0;
        m_byTurnCnt = 0;
        m_dwCheckCardID = 0;
        
        // TODO: Add log when CTextDBLog available
        // CTextDBLog::AddLog(m_textDBLog, 54, m_itemInfo.dwObjectID, dwUCID, 
        //     psPlay->dwCardID, psPlay->psOwnerInfo.nTotalPoint, psPlay->psGuestInfo.nTotalPoint, 
        //     10, nOldTurn, m_dwTurnUCID);
        return 10;
    }

    // Check if this player's turn
    if (dwUCID != m_dwTurnUCID)
    {
        m_bSendLogDB = true;
        return 2;
    }

    // Find card in m_mpCardInfo
    auto it = m_mpCardInfo.find(psPlay->dwCardID);
    if (it == m_mpCardInfo.end())
    {
        m_bSendLogDB = true;
        return 3;  // Card not found
    }

    // Check if card already used (useCount >= 2)
    if (it->second >= 2)
    {
        m_bSendLogDB = true;
        return 4;  // Card already used
    }

    // Process game logic
    bool bSuccess = false;
    if (m_dwCheckCardID != 0)
    {
        bSuccess = (m_dwCheckCardID == psPlay->dwCardID);
    }

    int nTurnCheck = (++m_byTurnCnt) & 1;  // Toggle between 0 and 1
    
    if (nTurnCheck == 0)
    {
        // Second card of the pair
        m_dwCheckCardID = 0;
        m_byTurnCnt = 0;
        
        if (bSuccess)
        {
            // Match found!
            ++m_byBonus;
            ++m_byReverseCount;
            
            if (m_byBonus == 2)
            {
                // Two consecutive matches - bonus turn
                m_dwTurnUCID = GetPlayNextTurn();
                m_byBonus = 0;
            }
        }
        else
        {
            // No match - switch turn
            m_dwTurnUCID = GetPlayNextTurn();
            m_byBonus = 0;
        }
    }

    // Check if game finished
    if (psPlay->bFinish)
    {
        ++m_byPlayCount;
        m_mpCardInfo.clear();
        
        if (m_byReverseCount != 18)
        {
            m_bSendLogDB = true;
            return 5;  // Not all cards matched
        }
    }
    else if (m_byReverseCount > 18)
    {
        m_bSendLogDB = true;
        return 6;  // Too many cards matched
    }

    // Check if game round complete
    if (m_byPlayCount >= static_cast<int>(m_byMaxPlayCount))
    {
        m_byPlayState = E_SOCIAL_OBJECT_STATE_FINISH;
        
        // Determine winner
        std::uint32_t nWinnerUCID = GetPlayGuestID();
        if (psPlay->psOwnerInfo.nTotalPoint >= psPlay->psGuestInfo.nTotalPoint)
        {
            nWinnerUCID = GetOwnerID();
        }
        
        // TODO: Send DB log when CTextDBLog available
        // CTextDBLog::AddLog(m_textDBLog, 55, m_itemInfo.dwObjectID, dwUCID,
        //     psPlay->dwCardID, psPlay->psOwnerInfo.nTotalPoint, psPlay->psGuestInfo.nTotalPoint,
        //     0, nWinnerUCID, 0);
        // CTextDBLog::SendLogDB(m_textDBLog, m_bSendLogDB);
        m_bSendLogDB = false;
    }

    // Update card state
    if (nTurnCheck)
    {
        // First card of the pair
        m_dwCheckCardID = psPlay->dwCardID;
    }
    else if (bSuccess)
    {
        // Mark card as used (both cards in pair)
        it->second = 2;
    }

    return 0;  // Success
}

// ============================================================================
// CSocialItemObject::AddPlayUserInfo - Add play user info
// IDA @ 0x14018DBA0
// ============================================================================
void CSocialItemObject::AddPlayUserInfo(PS_SOCIALITEM_USER* psInfo)
{
    // IDA: Sends user info to all existing players, then sends all existing
    // player info to new player, finally adds new player to m_vecPlayerInfo

    if (!psInfo)
        return;

    // Send new player info to all existing players
    for (size_t i = 0; i < m_vecPlayerInfo.size(); ++i)
    {
        std::uint32_t dwActorID = m_vecPlayerInfo[i].dwUCID;
        
        // TODO: Implement when XGameServer/XSendPacket/CGocNetwork available
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // CUser* pUser = pServer->FindActorIDToUser(dwActorID);
        // if (pUser)
        // {
        //     XSendPacket xSendPacket(0x2D, 0x10);
        //     xSendPacket << *psInfo;
        //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
        // }
    }

    // Send all existing player info to new player
    for (size_t j = 0; j < m_vecPlayerInfo.size(); ++j)
    {
        // TODO: Implement when XGameServer/XSendPacket/CGocNetwork available
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // CUser* pNewUser = pServer->FindActorIDToUser(psInfo->dwUCID);
        // if (pNewUser)
        // {
        //     XSendPacket packet(0x2D, 0x10);
        //     packet << m_vecPlayerInfo[j];
        //     CGocNetwork::Send(&pNewUser->XActor, &packet);
        // }
    }

    // Add new player to list
    m_vecPlayerInfo.push_back(*psInfo);
}

// ============================================================================
// CSocialItemObject constructor
// ============================================================================
CSocialItemObject::CSocialItemObject()
{
    // Initialize members
    std::memset(&m_itemInfo, 0, sizeof(m_itemInfo));
    m_vPosition[0] = 0.0f;
    m_vPosition[1] = 0.0f;
    m_vPosition[2] = 0.0f;
    m_fRot = 0.0f;
    m_bySocialType = E_SOCIAL_OBJECT_TYPE_NORMAL;
    m_byMaxUserCount = 1;
    m_byPlayState = E_SOCIAL_OBJECT_STATE_NONE;
    m_byPlayCount = 0;
    m_byMaxPlayCount = 0;
    m_byReverseCount = 0;
    m_dwTurnUCID = 0;
    m_bSendLogDB = false;

    // Note: Total size is 0xED08 bytes due to inheritance chain
}
