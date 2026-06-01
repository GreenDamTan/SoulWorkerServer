// MonsterProcess.cpp
// Monster control packet handler for GameServer
// Reconstructed from IDA decompilation

#include "MonsterProcess.h"
#include "../Monster.h"
#include "../User.h"
#include "../GameServer.h"
#include "../../XCore/XServer/XServer.h"
#include "../../../Common/XNet/XCommon/PSCommon.h"
#include "../../../Common/XNet/XCommon/PSServer.h"

// ============================================================================
// Constructor / Destructor
// ============================================================================

// IDA 0x140518E50 - Constructor
CMonsterProcess::CMonsterProcess()
    : TXProcess<CUser>()
{
    SetCmd(0x17);  // Main command for monster packets
    SetName("CMonsterProcess");
}

// IDA 0x140518F10 - Destructor
CMonsterProcess::~CMonsterProcess()
{
    // Base class destructor handles cleanup
}

// ============================================================================
// Main Packet Parser
// ============================================================================

// IDA 0x140518F40 - Main packet parser
bool CMonsterProcess::Parse(XPacket& xPacket)
{
    // Get sub command and dispatch
    std::uint8_t bySubCmd = xPacket.GetSubCmd();

    switch (bySubCmd)
    {
    case 0x21:  // '!' - Target change
        ReqTargetChange(xPacket);
        break;

    case 0x41:  // 'A' - Defensive weapon start
        ReqDefensiveWeaponStart(xPacket);
        break;

    case 0x43:  // 'C' - Defensive weapon end
        ReqDefensiveWeaponEnd(xPacket);
        break;

    case 0x45:  // 'E' - Defensive weapon attack
        ReqDefensiveWeaponAttack(xPacket);
        break;

    case 0x52:  // 'R' - Control monster attack
        ReqControlMonsterAttack(xPacket);
        break;

    case 0x55:  // 'U' - Monster wrong position
        ReqMonsterWrongPos(xPacket);
        break;

    default:
        return true;  // Unknown sub command, ignore
    }

    return true;
}

// ============================================================================
// Packet Handlers
// ============================================================================

// IDA 0x140519060 - Request target change
bool CMonsterProcess::ReqTargetChange(XPacket& xPacket)
{
    // Get client (user)
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Parse packet data
    std::uint32_t dwActorID = 0;
    std::uint32_t dwTargetActorID = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwTargetActorID;

    // Create response packet (main=0x17, sub=0x22)
    XSendPacket xSendPacket(0x17, 0x22);

    // TODO: Broadcast to nearby players
    // CGocNetwork::SendBroadCast(pUser, &xSendPacket, 1);

    return true;
}

// IDA 0x140519170 - Start defensive weapon mode
bool CMonsterProcess::ReqDefensiveWeaponStart(XPacket& xPacket)
{
    // Get client (user)
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Parse target actor ID
    std::uint32_t dwTargetActorID = 0;
    xPacket.XParse >> dwTargetActorID;

    // TODO: Implement defensive weapon logic with lambda callbacks
    // This requires:
    // 1. XActor::GetArea check
    // 2. XClient::IncrementJobCount
    // 3. CLogicThreadManager::DoJob with lambda

    return true;
}

// IDA 0x140519550 - End defensive weapon mode
bool CMonsterProcess::ReqDefensiveWeaponEnd(XPacket& xPacket)
{
    // Get client (user)
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Parse target actor ID
    std::uint32_t dwTargetActorID = 0;
    xPacket.XParse >> dwTargetActorID;

    // TODO: Implement defensive weapon end logic with lambda callbacks

    return true;
}

// IDA 0x1405198F0 - Defensive weapon attack
bool CMonsterProcess::ReqDefensiveWeaponAttack(XPacket& xPacket)
{
    // Get client (user)
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Parse packet data
    std::uint32_t dwTargetActorID = 0;
    std::uint8_t byAttackIdx = 0;

    xPacket.XParse >> dwTargetActorID;
    xPacket.XParse >> byAttackIdx;

    // TODO: Implement defensive weapon attack logic

    return true;
}

// IDA 0x140519B00 - Control monster attack
bool CMonsterProcess::ReqControlMonsterAttack(XPacket& xPacket)
{
    // Get client (user)
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Parse packet data
    std::uint32_t dwTargetActorID = 0;
    std::uint8_t byAttackIdx = 0;

    xPacket.XParse >> dwTargetActorID;
    xPacket.XParse >> byAttackIdx;

    // TODO: Implement control monster attack logic

    return true;
}

// IDA 0x140519DF0 - Report monster wrong position
bool CMonsterProcess::ReqMonsterWrongPos(XPacket& xPacket)
{
    // Get client (user)
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    // Parse wrong position report
    ST_MONSTER_WRONG_POS stWrongPos;
    xPacket.XParse >> stWrongPos.dwMonsterActorID;
    xPacket.XParse >> stWrongPos.dwAreaID;
    xPacket.XParse >> stWrongPos.wType;
    xPacket.XParse >> stWrongPos.vClientPos.x;
    xPacket.XParse >> stWrongPos.vClientPos.y;
    xPacket.XParse >> stWrongPos.vClientPos.z;

    // Get monster by actor ID - CUser inherits from CMoverEx which inherits from CMover
    CMover* pMover = static_cast<CMover*>(pUser)->GetMoverObject(stWrongPos.dwMonsterActorID);
    if (!pMover)
        return false;

    // Cast to CMonster
    CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    if (!pMonster)
        return false;

    // Store wrong position info for debugging/anti-cheat
    hkvVec3 vPos(stWrongPos.vClientPos.x,
                 stWrongPos.vClientPos.y,
                 stWrongPos.vClientPos.z);
    pMonster->StoreWrongPosInfo(vPos, stWrongPos.wType);

    return true;
}
