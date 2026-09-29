// SocialItemObject.cpp
// CSocialItemObject implementation
// IDA decompilation from GameServer.exe

#include "SocialItemObject.h"
#include "GameServer.h"
#include "User.h"
#include "actor/component/GocNetwork.h"
#include "Log/GreenDamTan_TextDBLog.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
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
void CSocialItemObject::BuildInfoPacket(ST_SOCIAL_ITEM_RES& stInfo)
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

    // Copy item info
    stInfo.itemInfo.dwObjectID = m_itemInfo.dwObjectID;
    stInfo.itemInfo.dwOwnerID = m_itemInfo.dwOwnerID;
    stInfo.itemInfo.dwItemID = m_itemInfo.dwItemID;
    stInfo.itemInfo.wSocialItemID = m_itemInfo.wSocialItemID;
    stInfo.itemInfo.lRemainTime = m_itemInfo.lRemainTime;

    // Copy user list
    stInfo.itemInfo.vecUsers.clear();
    stInfo.itemInfo.vecUsers = m_itemInfo.vecUsers;

    // Copy used slots
    for (int i = 0; i < 4; ++i)
    {
        stInfo.itemInfo.stUsedSlot[i] = m_itemInfo.stUsedSlot[i];
    }

    // Copy position and rotation
    stInfo.fPosX = m_vPosition[0];
    stInfo.fPosY = m_vPosition[1];
    stInfo.fPosZ = m_vPosition[2];
    stInfo.fRot = m_fRot;
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
    BuildInfoPacket(stInfo);
    xSendPacket << stInfo;
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
bool CSocialItemObject::AddUser(GreenDamTan_SocialItemActorID dwActorID, int& nSlot, int byAniIndex)
{
    // IDA code (complex logic):
    // - Check if user already exists
    // - Handle different social types (1=BATCH, 2=FUNITURE, 3=PLAY)
    // - Find available slot
    // - Update play state

    // Check if user already exists
    if (IsExistUser(dwActorID))
    {
        return false;
    }

    // Owner handling for social type 2
    if (m_bySocialType == E_SOCIAL_OBJECT_TYPE_FUNITURE && m_itemInfo.dwOwnerID == dwActorID)
    {
        nSlot = 0;
        m_itemInfo.vecUsers.push_back(dwActorID);
        return true;
    }

    // Owner handling for other types
    if (m_itemInfo.dwOwnerID == dwActorID)
    {
        m_itemInfo.vecUsers.push_back(dwActorID);
        if (m_bySocialType == E_SOCIAL_OBJECT_TYPE_PLAY)
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
    if (!bOwnerExists || m_bySocialType == E_SOCIAL_OBJECT_TYPE_FUNITURE || m_bySocialType == E_SOCIAL_OBJECT_TYPE_PLAY)
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
    case E_SOCIAL_OBJECT_TYPE_BATCH:
        bFind = false;
        for (int i = 0; i < m_byMaxUserCount; ++i)
        {
            if (m_itemInfo.stUsedSlot[i].dwUserID == 0)
            {
                m_itemInfo.stUsedSlot[i].dwUserID = dwActorID;
                m_itemInfo.stUsedSlot[i].byAniIndex = static_cast<std::uint8_t>(byAniIndex);
                bFind = true;
                nSlot = i;
                break;
            }
        }
        break;

    case E_SOCIAL_OBJECT_TYPE_FUNITURE:
        bFind = false;
        for (int j = 1; j < m_byMaxUserCount; ++j)
        {
            if (m_itemInfo.stUsedSlot[j].dwUserID == 0)
            {
                m_itemInfo.stUsedSlot[j].dwUserID = dwActorID;
                m_itemInfo.stUsedSlot[j].byAniIndex = static_cast<std::uint8_t>(byAniIndex);
                bFind = true;
                nSlot = j;
                break;
            }
        }
        break;

    case E_SOCIAL_OBJECT_TYPE_PLAY:
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
        if (m_bySocialType == E_SOCIAL_OBJECT_TYPE_PLAY)
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
// 状态: 部分还原 - 已恢复用户记录与离开通知链，社交对象基类 ABI 尚未恢复。
// TODO: 需人工审查
//   1. 在恢复 CMoverEx/XActor 继承后核查该对象的生命周期与虚调用。
//   2. 对照原始客户端协议验证 0x2D/0x10 实际封包与发送行为。
// 依赖: CSocialItemObject 原始继承布局及客户端封包回归验证。
// ============================================================================
bool CSocialItemObject::DeleteUser(GreenDamTan_SocialItemActorID dwActorID)
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
    if (bRes && (m_bySocialType == E_SOCIAL_OBJECT_TYPE_BATCH || m_bySocialType == E_SOCIAL_OBJECT_TYPE_FUNITURE))
    {
        for (int i = 0; i < m_byMaxUserCount; ++i)
        {
            if (m_itemInfo.stUsedSlot[i].dwUserID == dwActorID)
            {
                m_itemInfo.stUsedSlot[i].reset();
                break;
            }
        }
    }

    for (auto it = m_vecPlayerInfo.begin(); it != m_vecPlayerInfo.end(); ++it)
    {
        if (it->dwUCID != dwActorID)
            continue;

        PS_SOCIALITEM_USER psDelUserInfo = *it;
        psDelUserInfo.bLeave = true;
        std::uint32_t dwUCID = GetOtherInfo(dwActorID);
        CUser* pUser = XGameServer::Instance()->FindActorIDToUser(UXActorID(dwUCID));
        if (pUser)
        {
            XSendPacket xSendPacket(0x2D, 0x10);
            xSendPacket << psDelUserInfo;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
        }

        if (m_byPlayState == E_SOCIAL_OBJECT_STATE_READY)
            m_byPlayState = E_SOCIAL_OBJECT_STATE_WAIT;

        m_vecPlayerInfo.erase(it);
        return true;
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
    return m_bySocialType != 0 && m_bySocialType != E_SOCIAL_OBJECT_TYPE_PLAY;
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
    
    xSendPacket << m_itemInfo;
}

// ============================================================================
// CSocialItemObject::SetFurnitureInfo - Set furniture max user count
// IDA @ 0x14018C830
// ============================================================================
void CSocialItemObject::SetFurnitureInfo(int nMaxUseNum)
{
    // PDB 记录参数为 int；原始指令仅将其低字节写入成员。
    m_byMaxUserCount = static_cast<std::uint8_t>(nMaxUseNum);
}

// ============================================================================
// CSocialItemObject::SendPlayInfo - Send play info to all users
// IDA @ 0x14018C8A0
// 状态: 部分还原 - 发送与序列化顺序已恢复，真实会话尚未核对。
// TODO: 需人工审查
//   1. 在有效双人社交道具会话中验证 0x2D/9 封包及两名用户的接收结果。
//   2. 恢复 CMoverEx/XActor 继承后核对原始对象虚调用及生命周期。
// 依赖: 原始社交对象继承布局和实际客户端卡牌会话。
// ============================================================================
void CSocialItemObject::SendPlayInfo(PS_SOCIALITEM_PLAY psPlayInfo)
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

    for (int i = 0; i < static_cast<int>(m_itemInfo.vecUsers.size()); ++i)
    {
        std::uint32_t dwActorID = m_itemInfo.vecUsers[static_cast<std::size_t>(i)];
        CUser* pUser = XGameServer::Instance()->FindActorIDToUser(UXActorID(dwActorID));
        if (pUser)
        {
            XSendPacket xSendPacket(0x2D, 9);
            xSendPacket << psPlayInfo;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
        }
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
    return m_bySocialType != E_SOCIAL_OBJECT_TYPE_PLAY || m_byPlayState < E_SOCIAL_OBJECT_STATE_READY;
}

// ============================================================================
// CSocialItemObject::GetPlayGuestID - Get guest player UCID
// IDA @ 0x14018DAF0
// ============================================================================
GreenDamTan_SocialItemActorID CSocialItemObject::GetPlayGuestID()
{
    // IDA code:
    // unsigned __int64 __fastcall CSocialItemObject::GetPlayGuestID(CSocialItemObject *this)
    // {
    //   if ( this->m_bySocialType != 3 )
    //     return 0;
    //   for ( i = 0; i < vecUsers.size(); ++i )
    //   {
    //     if ( this->m_itemInfo.dwOwnerID != vecUsers[i]->dwActorID )
    //       return vecUsers[i]->dwActorID;
    //   }
    //   return vecUsers.size();
    // }
    if (m_bySocialType != E_SOCIAL_OBJECT_TYPE_PLAY)
        return 0;

    for (size_t i = 0; i < m_itemInfo.vecUsers.size(); ++i)
    {
        if (m_itemInfo.dwOwnerID != m_itemInfo.vecUsers[i])
        {
            return m_itemInfo.vecUsers[i];
        }
    }
    return static_cast<GreenDamTan_SocialItemActorID>(m_itemInfo.vecUsers.size());
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
    // TODO: 需人工审查 - IDA 伪代码给出 32 位转换，但汇编使用 cvtsi2ss 和 cvttss2si 的 64 位形式。
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
UXActorID CSocialItemObject::GetActorID()
{
    // TODO: 需人工审查 - 当前类尚未恢复 CMoverEx/XActor 继承及虚调用链。
    // IDA 从 XActor 子对象 +59520 取值，PDB 确认其对应完整对象 +60392 的 m_itemInfo.dwObjectID。
    return UXActorID(m_itemInfo.dwObjectID);
}

// ============================================================================
// CSocialItemObject::StartPlaySocialItem - Start play social item game
// IDA @ 0x14018DF00
// 状态: 部分还原 - 卡牌数量、资源检查及插入顺序已对照 IDA，还缺原始对象布局与实际会话验证。
// TODO: 需人工审查
//   1. 恢复 CMoverEx/XActor 基类后核对成员偏移及卡牌 map 生命周期。
//   2. 在有效卡牌会话中验证资源缺失和重复卡牌的返回路径。
// 依赖: CSocialItemObject 原始继承布局和实际客户端卡牌会话。
// ============================================================================
bool CSocialItemObject::StartPlaySocialItem(PS_SOCIAL_ITEM_PLAY_START psStart)
{
    m_byMaxPlayCount = 1;

    // IDA 对卡牌数先做整数除以 2；奇数 37 也会通过此判断。
    if (psStart.vecCardInfo.size() / 2 != 18)
    {
        LogHelper::LogError("game.contents", "StartPlaySocialItem error - CardSize[%d]",
                            static_cast<int>(psStart.vecCardInfo.size()));
        return false;
    }

    m_byPlayState = E_SOCIAL_OBJECT_STATE_START;
    m_dwTurnUCID = m_itemInfo.dwOwnerID;
    m_byTurnCnt = 0;
    m_dwCheckCardID = 0;
    m_byBonus = 0;
    m_byReverseCount = 0;

    XResourceMgr& resourceMgr = XGameServer::Instance()->GetResourceMgr();
    for (int i = 0; i < static_cast<int>(psStart.vecCardInfo.size()); ++i)
    {
        ST_SOCIALITEM_CARD stCard = psStart.vecCardInfo[static_cast<std::size_t>(i)];
        if (!resourceMgr.GetTB_MODE_CARDMATCH_RULE(m_itemInfo.wSocialItemID))
        {
            LogHelper::LogError("game.contents", "StartPlaySocialItem error - No TB_MODE_CARDMATCH_ROUL[%d]",
                                m_itemInfo.wSocialItemID);
            return false;
        }
        if (!resourceMgr.GetTB_MODE_CARDMATCH_CARD(stCard.dwCardID))
        {
            LogHelper::LogError("game.contents", "StartPlaySocialItem error - No TB_MODE_CARDMATCH_CARD[%d]",
                                stCard.dwCardID);
            return false;
        }
        m_mpCardInfo.insert({stCard.dwCardID, 0});
    }
    return true;
}

// ============================================================================
// CSocialItemObject::SendStartInfo - Send start info to all users
// IDA @ 0x14018E1C0
// 状态: 部分还原 - 发送与数据库日志链已落地，内嵌文本日志和原始基类仍待恢复。
// TODO: 需人工审查
//   1. 将当前空指针文本日志恢复为 PDB 所示内嵌 CTextDBLog，并核对 53 类日志实际发送。
//   2. 在有效双人会话中核对 0x2D/8 封包及两名用户的 DB 日志。
// 依赖: CSocialItemObject 原始继承与日志布局、有效客户端会话。
// ============================================================================
void CSocialItemObject::SendStartInfo(PS_SOCIAL_ITEM_PLAY_START psStartInfo)
{
    // IDA: Sends start info packet to all users in vecUsers
    // - Packet main=0x2D, sub=8
    // - Logs game start with ST_LOG_GAME (main=20, sub=3)

    for (int i = 0; i < static_cast<int>(m_itemInfo.vecUsers.size()); ++i)
    {
        std::uint32_t dwActorID = m_itemInfo.vecUsers[static_cast<std::size_t>(i)];
        XGameServer* pServer = XGameServer::Instance();
        CUser* pUser = pServer->FindActorIDToUser(UXActorID(dwActorID));
        if (pUser)
        {
            XSendPacket xSendPacket(0x2D, 8);
            xSendPacket << psStartInfo;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);

            // TODO: 需人工审查 - 原版日志内嵌对象尚未恢复，当前指针为空时不得解引用。
            if (m_textDBLog)
                m_textDBLog->AddLog(53, m_itemInfo.dwObjectID, dwActorID, 0, 0, 0, 0, 0, 0);

            ST_LOG_GAME stLog;
            stLog._nUCID = pUser->GetActorID().dwActorID;
            stLog._nUAID = pUser->GetUAID();
            stLog._sMainType = 20;
            stLog._sSubType = 3;
            stLog.nParam0 = m_itemInfo.dwItemID;
            stLog.nParam1 = m_itemInfo.dwOwnerID;
            stLog.nParam2 = GetOtherInfo(dwActorID);
            stLog.nParam5 = m_itemInfo.dwObjectID;
            pServer->SendDBLog(stLog);
        }
    }
}

// ============================================================================
// CSocialItemObject::IsPlayGame - Check and process play game
// IDA @ 0x14018CA50
// 状态: 部分还原 - 结果 5/6 的后续结算路径已对照 IDA，日志与完整对象布局仍未恢复。
// TODO: 需人工审查
//   1. 恢复内嵌 CTextDBLog 后核对结果 1-4、换手、结算及常规出牌的日志参数与发送。
//   2. 在有效会话中核对回合切换、结算与实际封包交互。
//   3. 核对原版完成时清空卡牌 map 后仍使用旧迭代器的未定义行为。
// 依赖: 原始社交对象继承布局、可用的卡牌会话及内嵌文本日志链。
// ============================================================================
int CSocialItemObject::IsPlayGame(GreenDamTan_SocialItemActorID dwUCID, PS_SOCIALITEM_PLAY psPlayInfo)
{
    PS_SOCIALITEM_PLAY* psPlay = &psPlayInfo;
    int nResult = 0;
    // IDA: Complex game logic with multiple result codes:
    // - nResult = 0: Success
    // - nResult = 1: Not in START state
    // - nResult = 2: Not this player's turn
    // - nResult = 3: Card not found in m_mpCardInfo
    // - nResult = 4: Card already used
    // - nResult = 5: Game finish but not all cards matched
    // - nResult = 6: Too many cards matched
    // - nResult = 0: Turn change (nType=1), logged with result code 10

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
        return 0;
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
            nResult = 5;
            if (m_textDBLog)
                m_textDBLog->AddLog(55, m_itemInfo.dwObjectID, dwUCID,
                                    psPlay->dwCardID, psPlay->psOwnerInfo.nTotalPoint,
                                    psPlay->psGuestInfo.nTotalPoint, 5, m_byReverseCount, 0);
        }
    }
    else if (m_byReverseCount > 18)
    {
        m_bSendLogDB = true;
        nResult = 6;
        if (m_textDBLog)
            m_textDBLog->AddLog(55, m_itemInfo.dwObjectID, dwUCID,
                                psPlay->dwCardID, psPlay->psOwnerInfo.nTotalPoint,
                                psPlay->psGuestInfo.nTotalPoint, 6, m_byReverseCount, 0);
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
        
        // TODO: 需人工审查 - 原版为内嵌日志对象，当前仅能在指针有效时发送。
        if (m_textDBLog)
        {
            m_textDBLog->AddLog(55, m_itemInfo.dwObjectID, dwUCID,
                                psPlay->dwCardID, psPlay->psOwnerInfo.nTotalPoint,
                                psPlay->psGuestInfo.nTotalPoint, nResult, nWinnerUCID, 0);
            m_textDBLog->SendLogDB(m_bSendLogDB);
        }
        m_bSendLogDB = false;
    }

    // Update card state
    if (nTurnCheck)
    {
        // First card of the pair
        m_dwCheckCardID = psPlay->dwCardID;
    }
    else if (bSuccess && !psPlay->bFinish)
    {
        // TODO: 需人工审查 - 原版完成时清空 map 后仍写旧迭代器，此处跳过失效写入。
        it->second = 2;
    }

    return 0;  // Success
}

// ============================================================================
// CSocialItemObject::AddPlayUserInfo - Add play user info
// IDA @ 0x14018DBA0
// 状态: 部分还原 - 发送顺序与玩家记录已恢复，实际封包仍待运行核对。
// TODO: 需人工审查
//   1. 在有效用户会话中核对两个方向的 0x2D/0x10 实际封包。
//   2. 核对恢复后的 CSocialItemObject 基类与用户对象虚调用。
// 依赖: 有效 GameServer 会话和 CSocialItemObject 的 CMoverEx/XActor 继承布局。
// ============================================================================
void CSocialItemObject::AddPlayUserInfo(PS_SOCIALITEM_USER psInfo)
{
    // IDA: Sends user info to all existing players, then sends all existing
    // player info to new player, finally adds new player to m_vecPlayerInfo

    // Send new player info to all existing players
    for (size_t i = 0; i < m_vecPlayerInfo.size(); ++i)
    {
        std::uint32_t dwActorID = m_vecPlayerInfo[i].dwUCID;
        CUser* pUser = XGameServer::Instance()->FindActorIDToUser(UXActorID(dwActorID));
        if (pUser)
        {
            XSendPacket xSendPacket(0x2D, 0x10);
            xSendPacket << psInfo;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
        }
    }

    // Send all existing player info to new player
    for (size_t j = 0; j < m_vecPlayerInfo.size(); ++j)
    {
        CUser* pNewUser = XGameServer::Instance()->FindActorIDToUser(UXActorID(psInfo.dwUCID));
        if (pNewUser)
        {
            XSendPacket packet(0x2D, 0x10);
            packet << m_vecPlayerInfo[j];
            CGocNetwork::Send(static_cast<XActor*>(pNewUser), packet);
        }
    }

    // Add new player to list
    m_vecPlayerInfo.push_back(psInfo);
}

// ============================================================================
// CSocialItemObject constructor
// ============================================================================
CSocialItemObject::CSocialItemObject()
{
    // TODO: 需人工审查 - 基类、位置和组件构造链仍待按 PDB 完整核准。
    m_itemInfo.dwObjectID = 0;
    m_itemInfo.dwOwnerID = 0;
    m_itemInfo.dwItemID = 0;
    m_itemInfo.wSocialItemID = 0;
    m_itemInfo.lRemainTime = 0;
    m_itemInfo.vecUsers.clear();
    for (auto& slot : m_itemInfo.stUsedSlot)
        slot = {};
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
