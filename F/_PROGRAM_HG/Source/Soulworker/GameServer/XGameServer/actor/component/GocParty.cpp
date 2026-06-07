// GocParty.cpp
// CGocParty - Game Object Component for Party system
// 从 IDA GameServer.exe 精确还原

#include "GocParty.h"
#include "GocForce.h"  // For GetForceMember - CGocParty and CGocForce share memory layout
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/SendPacket.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Party.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/Actor/Component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/ResourceMgr.h"
#include <tr1/memory>

// ============================================================================
// CGocParty Memory Layout Notes (IDA verified):
// ============================================================================
// CGocParty 和 CGocForce 共享相同的内存布局，允许 CGocForce::GetForceMember
// 接受 CGocParty* 作为 this 指针:
//   - offset 0:  vtable (8 bytes from GOComponent)
//   - offset 16: m_pParty/m_pForce (std::shared_ptr, 16 bytes)
//   - offset 32: m_biMatchingDate (__int64, 8 bytes)
// 这种设计允许队伍和公会系统共享 GetForceMember 实现
// ============================================================================

// ============================================================================
// 静态方法
// ============================================================================

// IDA: ?GetFamilyID@CGocParty@@SAHXZ @ 0x140039000
// Family ID = 8 (Party组件的家族ID)
int CGocParty::GetFamilyID()
{
    return 8;
}

// ============================================================================
// 构造函数/析构函数
// ============================================================================

// IDA: ??0CGocParty@@QEAA@XZ @ 0x14010AFF0
CGocParty::CGocParty()
    : GOComponent()
    , m_pParty()
    , m_biMatchingDate(0)
{
    // GOComponent 构造函数会初始化 vtable
    // m_pParty 默认初始化为空 shared_ptr
}

// IDA: ??1CGocParty@@UEAA@XZ @ 0x14010B080
CGocParty::~CGocParty()
{
    // 设置 vtable 指向 CGocParty 的虚函数表
    // 释放 m_pParty (shared_ptr 析构)
    m_pParty.reset();
}

// ============================================================================
// 初始化/清理
// ============================================================================

// IDA: ?Init@CGocParty@@QEAAXXZ @ 0x14010B0D0
void CGocParty::Init()
{
    m_pParty.reset();
    m_biMatchingDate = 0;
}

// IDA: ?Clear@CGocParty@@QEAAXXZ @ 0x14010B100
void CGocParty::Clear()
{
    if (m_pParty)
    {
        // 获取当前玩家的 ActorID
        CMover* pOwner = GetOwnerGO();
        if (pOwner)
        {
            std::uint32_t dwActorID = pOwner->GetActorID();

            // 从队伍中移除成员
            if (m_pParty)
            {
                m_pParty->RemoveMember(dwActorID);
            }
        }

        m_pParty.reset();
    }
    m_biMatchingDate = 0;
}

// ============================================================================
// 队伍状态查询
// ============================================================================

// IDA: ?IsParty@CGocParty@@QEAA_NXZ @ 0x140091E20
bool CGocParty::IsParty() const
{
    // 检查 shared_ptr 是否有效 (operator bool)
    return m_pParty.operator bool();
}

// IDA: ?GetPartyID@CGocParty@@QEAAHXZ @ 0x14009F760
std::uint32_t CGocParty::GetPartyID() const
{
    if (!m_pParty)
    {
        return 0;
    }
    return m_pParty->GetPartyID();
}

// IDA: ?IsMember@CGocParty@@QEAA_NK@Z @ 0x1400831B0
bool CGocParty::IsMember(std::uint32_t dwActorID) const
{
    if (!m_pParty)
    {
        return false;
    }
    return m_pParty->IsMember(dwActorID);
}

// IDA: ?IsFull@CGocParty@@QEAA_NXZ @ 0x14010D370
bool CGocParty::IsFull() const
{
    if (!m_pParty)
    {
        return false;
    }
    // 队伍最大人数为 4
    return m_pParty->GetUserCount() == 4;
}

// IDA: ?IsMaster@CGocParty@@QEAA_NXZ @ 0x1403104B0
bool CGocParty::IsMaster() const
{
    if (!m_pParty)
    {
        return false;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return false;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    std::uint32_t dwMasterID = m_pParty->GetMasterID();

    return dwActorID == dwMasterID;
}

// IDA: ?GetParty@CGocParty@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@XZ @ 0x1402D3AF0
std::shared_ptr<CParty> CGocParty::GetParty() const
{
    return m_pParty;
}

// ============================================================================
// 队伍数据同步
// ============================================================================

// IDA: ?Send@CGocParty@@QEAAXAEAVXSendPacket@@@Z @ 0x14010B1A0
void CGocParty::Send(XSendPacket& sendPacket)
{
    if (m_pParty)
    {
        m_pParty->Send(sendPacket, nullptr);
    }
}

// IDA: ?SendPartyInfo@CGocParty@@QEAAXE@Z @ 0x14010B1F0
void CGocParty::SendPartyInfo(std::uint8_t byUpdateType)
{
    if (!m_pParty)
    {
        return;
    }

    PS_PARTY_INFO stPartyInfo;
    m_pParty->GetPartyInfo(&stPartyInfo);
    stPartyInfo.byUpdateType = byUpdateType;
    stPartyInfo.byPartyType = m_pParty->GetForceType();

    XSendPacket xSendPacket(0x12, 9);
    xSendPacket << stPartyInfo;

    CMover* pOwner = GetOwnerGO();
    if (pOwner)
    {
        CGocNetwork* pNetwork = pOwner->GetGOC<CGocNetwork>();
        if (pNetwork)
        {
            pNetwork->Send(xSendPacket);
        }
    }
}

// IDA: ?ShowMyPartyInfo@CGocParty@@QEAAXXZ @ 0x14010BB30
void CGocParty::ShowMyPartyInfo()
{
    if (!m_pParty)
    {
        CMover* pOwner = GetOwnerGO();
        if (pOwner)
        {
            GreenDamTan_log("game.party", "<PARTY_INFO> Dont Have And Party (%d)",
                pOwner->GetActorID());
        }
        return;
    }

    m_pParty->ShowPartyInfo();
}

// ============================================================================
// 队伍成员状态设置
// ============================================================================

// IDA: ?SetHP@CGocParty@@QEAAXH@Z @ 0x14010B540
void CGocParty::SetHP(int nHP)
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    UXMapID uxMapID;
    pOwner->GetMapInsID(&uxMapID);

    m_pParty->SetMemberHP(dwActorID, uxMapID, nHP);
}

// IDA: ?SetMaxHP@CGocParty@@QEAAXH@Z @ 0x14010B600
void CGocParty::SetMaxHP(int nMaxHP)
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    UXMapID uxMapID;
    pOwner->GetMapInsID(&uxMapID);

    m_pParty->SetMemberMaxHP(dwActorID, uxMapID, nMaxHP);
}

// IDA: ?SetLevel@CGocParty@@QEAAXH@Z @ 0x14010B6C0
void CGocParty::SetLevel(int nLevel)
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    m_pParty->SetMemberLevel(dwActorID, nLevel);
}

// IDA: ?SetAwaken@CGocParty@@QEAAXE@Z @ 0x14010B740
void CGocParty::SetAwaken(std::uint8_t byAwaken)
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    m_pParty->SetMemberAwaken(dwActorID, byAwaken);
}

// IDA: ?SetProfilePhoto@CGocParty@@QEAAXK@Z @ 0x14010B7C0
void CGocParty::SetProfilePhoto(std::uint32_t dwPhotoID)
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    m_pParty->SetMemberProfilePhoto(dwActorID, dwPhotoID);
}

// IDA: ?SetMapID@CGocParty@@QEAAXHHTUXMapID@@@Z @ 0x14010BC20
void CGocParty::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID)
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    m_pParty->SetMemberMapID(dwActorID, nMapID, nChannel, uxMapID);
}

// ============================================================================
// 匹配系统
// ============================================================================

// IDA: ?IsMatchingDate@CGocParty@@QEAA_NXZ @ 0x14010D3C0
bool CGocParty::IsMatchingDate() const
{
    if (!m_biMatchingDate)
    {
        return false;
    }

    // 匹配有效期 60 秒
    std::int64_t biExpireTime = m_biMatchingDate + 60;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    std::int64_t biCurDate = pServer->GetCurDate();

    return biExpireTime >= biCurDate;
}

// IDA: ?SetMatchingDate@CGocParty@@QEAAX_J@Z @ 0x140085030
void CGocParty::SetMatchingDate(std::int64_t biDate)
{
    m_biMatchingDate = biDate;

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pOwner);
    if (!pUser)
    {
        return;
    }

    if (biDate <= 0)
    {
        // 取消匹配状态
        pUser->SetMatchingState(0);

        // 清除迷宫需求道具
        std::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();
        if (pInven)
        {
            pInven->SetMazeNeedItemID(0);
        }
    }
    else
    {
        // 设置匹配状态
        pUser->SetMatchingState(1);
    }
}

// ============================================================================
// 队伍操作
// ============================================================================

// IDA: ?KickOut@CGocParty@@QEAA_NKPEAVCUser@@@Z @ 0x14010BCC0
bool CGocParty::KickOut(std::uint32_t dwActorID, CUser* pUser)
{
    CMover* pOwner = GetOwnerGO();

    // 检查是否在队伍中
    if (!IsParty())
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 4, 0xCF0E);
        }
        return false;
    }

    // 获取自己的 ActorID
    std::uint32_t dwMyActorID = pOwner ? pOwner->GetActorID() : 0;

    // 不能踢出自己
    if (dwMyActorID == dwActorID)
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 4, 0xCF0B);
        }
        return false;
    }

    // 检查是否为队长
    if (!IsMaster())
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 4, 0xCF10);
        }
        return false;
    }

    // 检查目标是否为队伍成员
    if (!m_pParty->IsMember(dwActorID))
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 4, 0xCF0E);
        }
        return false;
    }

    // 获取目标成员信息
    CPartyMember* pMember = m_pParty->GetMember(dwActorID);
    if (!pMember)
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 4, 0xCF13);
        }
        return false;
    }

    // 检查迷宫类型（某些迷宫不能踢人）
    std::uint16_t wMapID = pMember->GetMapID();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MAZE_INFO* pTBmazeInfo = pServer->GetXResourceMgr().GetTB_MAZE_INFO(wMapID);

    if (pTBmazeInfo &&
        (pTBmazeInfo->Maze_Type == 0 ||
         pTBmazeInfo->Maze_Type == 2 ||
         pTBmazeInfo->Maze_Type == 8 ||
         pTBmazeInfo->Maze_Type == 9))
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 4, 0xCF28);
        }
        return false;
    }

    // 发送踢出请求到 CommunitySocket
    PS_PARTY_LEAVE stPartyLeave;
    stPartyLeave.dwPartyID = GetPartyID();
    stPartyLeave.dwLeaveMember = dwActorID;
    stPartyLeave.bKickout = 1;

    XSendPacket xSendPacket(0xF4, 3);
    xSendPacket << stPartyLeave;

    if (pUser)
    {
        UXActorID uxActorID;
        pUser->GetActorID(&uxActorID);
        xSendPacket << uxActorID.dwActorID;
        xSendPacket << pUser->GetUAID();
        xSendPacket << pUser->GetLevel();
        xSendPacket << pMember->GetChannel();
    }

    CCommunitySocket::SendCmd(&pServer->m_communitySocket, &xSendPacket, pUser, 0x12, 4);

    return true;
}

// IDA: ?Leave@CGocParty@@QEAAXXZ @ 0x14010C250
void CGocParty::Leave()
{
    if (!IsParty())
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    CUser* pUser = pOwner ? dynamic_cast<CUser*>(pOwner) : nullptr;
    if (!pUser)
    {
        return;
    }

    // 检查是否在迷宫中
    XArea* pArea = pUser->GetArea();
    if (!pArea)
    {
        return;
    }

    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (pMaze && pMaze->GetMazeState() == 2)
    {
        // 迷宫进行中不能离开队伍
        CUser::SendErrorMessage(pUser, 0x12, 5, 0xCF25);
        return;
    }

    // 发送离开请求
    PS_PARTY_LEAVE stPartyLeave;
    stPartyLeave.dwPartyID = GetPartyID();
    stPartyLeave.dwLeaveMember = pOwner->GetActorID();
    stPartyLeave.bKickout = 0;

    XSendPacket xSendPacket(0xF4, 3);
    xSendPacket << stPartyLeave;

    UXActorID uxActorID;
    pUser->GetActorID(&uxActorID);
    xSendPacket << uxActorID.dwActorID;
    xSendPacket << pUser->GetUAID();
    xSendPacket << pUser->GetLevel();
    xSendPacket << pUser->GetLevel();

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    CCommunitySocket::SendCmd(&pServer->m_communitySocket, &xSendPacket, pUser, 0x12, 5);
}

// IDA: ?ChangeMaster@CGocParty@@QEAAXK@Z @ 0x14010C540
void CGocParty::ChangeMaster(std::uint32_t dwMaster)
{
    if (!IsParty())
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();

    // 检查是否为队长
    if (!IsMaster())
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 3, 0xCF0A);
        }
        return;
    }

    // 检查目标是否为队伍成员
    if (!m_pParty->IsMember(dwMaster))
    {
        if (pOwner)
        {
            CGocNetwork::SendErrorMessage(pOwner, 0x12, 3, 0xCF0E);
        }
        return;
    }

    // 发送更改队长请求
    PS_PARTY_CHANGE_MASTER stChangeMaster;
    stChangeMaster.dwReqActorID = pOwner ? pOwner->GetActorID() : 0;
    stChangeMaster.dwNewMasterID = dwMaster;
    stChangeMaster.dwPartyID = GetPartyID();
    stChangeMaster.nErrorCode = 0;

    XSendPacket xSendPacket(0xF4, 4);
    xSendPacket << stChangeMaster;

    CUser* pUser = pOwner ? dynamic_cast<CUser*>(pOwner) : nullptr;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    CCommunitySocket::SendCmd(&pServer->m_communitySocket, &xSendPacket, pUser, 0x12, 3);
}

// ============================================================================
// 成员操作
// ============================================================================

// IDA: ?GetMemberInMap@CGocParty@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z @ 0x14010D130
void CGocParty::GetMemberInMap(std::vector<CMover*>& vecMover)
{
    if (!IsParty())
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    XArea* pArea = pOwner->GetArea();
    if (!pArea)
    {
        return;
    }

    std::uint16_t wTBMapID = pArea->GetTBMapID();

    // 遍历队伍成员
    auto& memberMap = m_pParty->GetMemberMap();
    for (auto it = memberMap.begin(); it != memberMap.end(); ++it)
    {
        CPartyMember* pMember = it->second;
        if (!pMember)
        {
            continue;
        }

        // 检查成员是否在线且在同一地图
        if (pMember->IsOnline() && pMember->GetMapID() == wTBMapID)
        {
            CUser* pMemberUser = pMember->GetUser();
            if (pMemberUser)
            {
                UXMapID uxMapID;
                pMemberUser->GetMapInsID(&uxMapID);

                ThreadLocalData* pTLD = ThreadLocalData::GetInstance();
                if (pTLD->IsThreadArea(uxMapID))
                {
                    vecMover.push_back(&pMemberUser->CMoverEx);
                }
            }
        }
    }
}

// IDA: ?ChangePartyMemberName@CGocParty@@QEAAXAEAUPS_CHANGE_NAME@@@Z @ 0x14010D460
void CGocParty::ChangePartyMemberName(PS_CHANGE_NAME& stChangeName)
{
    if (!IsParty())
    {
        return;
    }

    m_pParty->ChangeMemberName(&stChangeName);
}

// ============================================================================
// 工具函数
// ============================================================================

// IDA: ?IsExistDeadUser@CGocParty@@QEAA_NXZ @ 0x140083200
bool CGocParty::IsExistDeadUser() const
{
    if (!m_pParty)
    {
        return false;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return false;
    }

    UXMapID myMapInsID;
    pOwner->GetMapInsID(&myMapInsID);

    // 遍历队伍成员检查是否有死亡成员在同一地图
    auto& memberMap = m_pParty->GetMemberMap();
    for (auto it = memberMap.begin(); it != memberMap.end(); ++it)
    {
        CPartyMember* pMember = it->second;
        if (!pMember)
        {
            continue;
        }

        CUser* pMemberUser = pMember->GetUser();
        if (!pMemberUser)
        {
            continue;
        }

        UXMapID uxMapID;
        pMemberUser->GetMapInsID(&uxMapID);

        // 检查是否在同一地图且死亡
        if (uxMapID == myMapInsID && pMemberUser->IsDie())
        {
            return true;
        }
    }

    return false;
}

// IDA: ?Logout@CGocParty@@QEAAXXZ @ 0x14010B840
void CGocParty::Logout()
{
    if (IsParty())
    {
        CMover* pOwner = GetOwnerGO();
        std::uint32_t dwActorID = pOwner ? pOwner->GetActorID() : 0;

        CPartyMember* pMember = m_pParty->GetMember(dwActorID);
        if (pMember)
        {
            // 准备登出数据
            ST_UPDATE_PARTY_MEMBER stPartyMember;
            stPartyMember.dwPartyID = GetPartyID();
            pMember->Logout(&stPartyMember);

            // 发送到 CommunitySocket
            XSendPacket xSendPacket(0xF4, 5);
            xSendPacket << stPartyMember;

            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            CCommunitySocket::SendCheck(&pServer->m_communitySocket, &xSendPacket);
        }
    }
    else
    {
        // 没有队伍，发送单独的登出消息
        CMover* pOwner = GetOwnerGO();
        CUser* pUser = pOwner ? dynamic_cast<CUser*>(pOwner) : nullptr;
        if (pUser)
        {
            XSendPacket xSendPacket(0xF4, 0x21);
            xSendPacket << pOwner->GetActorID();
            xSendPacket << 2;
            xSendPacket << pUser->GetUAID();

            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            CCommunitySocket::SendCheck(&pServer->m_communitySocket, &xSendPacket);
        }
    }

    GreenDamTan_log("game.party", "<PARTY> Logout Req (%d)",
        GetOwnerGO() ? GetOwnerGO()->GetActorID() : 0);

    Clear();
}

// IDA: ?SaveRecode@CGocParty@@QEAAXXZ @ 0x14010B340
void CGocParty::SaveRecode()
{
    if (!m_pParty)
    {
        return;
    }

    CMover* pOwner = GetOwnerGO();
    if (!pOwner)
    {
        return;
    }

    // 获取 GocRecode 组件
    std::shared_ptr<CGocRecode> pRecode = pOwner->GetGOC<CGocRecode>();
    if (!pRecode)
    {
        return;
    }

    std::uint32_t dwActorID = pOwner->GetActorID();
    int* pMazeRecode = pRecode->GetMazeRecode();

    m_pParty->SetMazeRecode(dwActorID, pMazeRecode);
}

// IDA: ?UpdatePartyBooster@CGocParty@@QEAAXXZ @ 0x14010C8F0
void CGocParty::UpdatePartyBooster()
{
    if (m_pParty)
    {
        m_pParty->UpdatePartyBooster(0);
    }
}

// IDA: ?SetExp@CGocParty@@QEAAXPEAVCUser@@MH@Z @ 0x14010CC20
void CGocParty::SetExp(CUser* pKiller, float fExp, int nMonsterLv)
{
    if (!m_pParty)
    {
        return;
    }

    // 获取等级差距
    std::uint8_t nLevelGap = m_pParty->GetLavelGap();

    // 获取经验加成配置
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_PARTYEXP_LEVEL* pTBPartyExpLevel = pServer->GetXResourceMgr().GetTB_PARTYEXP_LEVEL(nLevelGap);
    if (!pTBPartyExpLevel)
    {
        return;
    }

    float fTotalExp = fExp * pTBPartyExpLevel->LevelInterval_Value;

    // 获取怪物等级差距配置
    std::int8_t cMonsterInterval = m_pParty->GetCommonThreadIndex() - nMonsterLv;
    TB_PARTYEXP_MOB* pTBPartyExpMob = pServer->GetXResourceMgr().GetTB_PARTYEXP_MOB(cMonsterInterval);
    if (!pTBPartyExpMob)
    {
        return;
    }

    fTotalExp *= pTBPartyExpMob->Mob_IntervalValue;

    // 获取队伍成员列表
    std::vector<CPartyMember*> vecExpMember;
    GetForceMember(pKiller, vecExpMember, false);

    float fPartyExp_Ori = fTotalExp;

    // 遍历成员分配经验
    for (auto it = vecExpMember.begin(); it != vecExpMember.end(); ++it)
    {
        CPartyMember* pMember = *it;
        CUser* pMemberUser = pMember ? pMember->GetUser() : nullptr;
        if (!pMemberUser)
        {
            continue;
        }

        float fPartyExp = fPartyExp_Ori;

        // 获取属性组件计算经验加成
        std::shared_ptr<CGocAttribute> pAttr = pMemberUser->GetGOC<CGocAttribute>();
        if (pAttr)
        {
            float fRate = pAttr->GetSpecialEffect(EFFECT_SPECIAL_EXP_ADD_RAT) * 0.01f;

            // 获取 Booster 加成
            std::shared_ptr<CGocBooster> pBooster = pMemberUser->GetGOC<CGocBooster>();
            if (pBooster)
            {
                float fBoosterRate = pBooster->GetTotalRate(eBooster_Effect_IncExp);
                fRate += fBoosterRate;
            }

            // 联盟技能加成
            float fLeagueRate = 0.0f;
            if (pMemberUser->IsLeagueSkill(4))
            {
                fLeagueRate = pMemberUser->GetLeagueSkillEffectValue(4) * 0.01f;
            }

            float fBonus = fPartyExp * (fRate + fLeagueRate);
            fPartyExp += fBonus;

            GreenDamTan_log("game.contents",
                "[SetExp] CGocParty::SetExp() Before [UCID:%d][Exp:%d][AddExp:%f][AddBonus:%f]",
                pMemberUser->GetActorID(),
                pAttr->GetExp(),
                fPartyExp,
                fBonus);

            pAttr->SetExp(fPartyExp, fBonus, 1);

            GreenDamTan_log("game.contents",
                "[SetExp] CGocParty::SetExp() After [UCID:%d][Exp:%d]",
                pMemberUser->GetActorID(),
                pAttr->GetExp());
        }
    }
}
