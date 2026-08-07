// MonsterProcess.cpp
// Monster control packet handler for GameServer
// Reconstructed from IDA decompilation

#include "MonsterProcess.h"
#include "../Monster.h"
#include "../User.h"
#include "../GameServer.h"
#include "../actor/component/GocNetwork.h"
#include "../../XCore/XServer/XServer.h"
#include "../../XRelayServer/Thread/LogicThreadProcessor.h"
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
// 精确还原: 通过 DoJob 提交两个作业（lambda0 设置防御武器玩家并广播, lambda192 收尾）
bool CMonsterProcess::ReqDefensiveWeaponStart(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::uint32_t dwTargetActorID = 0;
    xPacket.XParse >> dwTargetActorID;

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    std::int64_t instanceID = pUser->GetMapInsID().nMapID;

    // IDA lambda0: 设置防御武器玩家并广播 (0x17, 0x42)
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser, &dwTargetActorID]() {
        if (pUser && pUser->GetArea()) {
            CMover* pMoverObject = pUser->GetMoverObject(dwTargetActorID);
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverObject);
            if (pMonster) {
                pMonster->SetDefensiveWeaponPlayer(pUser);
            }
            XSendPacket xSendPacket(0x17, 0x42);
            xSendPacket.XParse << pUser->GetUCID();
            xSendPacket.XParse << dwTargetActorID;
            CGocNetwork::SendBroadCast(pUser, xSendPacket, E_BROADCAST_TYPE::eNoneSelf);
        }
    });

    // IDA lambda192: 作业完成回调
    // TODO: 推测结果 - IDA 反编译中 lambda192 为按引用捕获 pUser 的收尾回调
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// IDA 0x140519550 - End defensive weapon mode
// 精确还原: 通过 DoJob 提交两个作业（lambda2 解除防御武器玩家并广播, lambda192 收尾）
bool CMonsterProcess::ReqDefensiveWeaponEnd(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::uint32_t dwTargetActorID = 0;
    xPacket.XParse >> dwTargetActorID;

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    std::int64_t instanceID = pUser->GetMapInsID().nMapID;

    // IDA lambda2: 解除防御武器玩家并广播 (0x17, 0x44)
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser, &dwTargetActorID]() {
        if (pUser && pUser->GetArea()) {
            CMover* pMoverObject = pUser->GetMoverObject(dwTargetActorID);
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverObject);
            if (pMonster) {
                pMonster->SetDefensiveWeaponPlayer(nullptr);
            }
            XSendPacket xSendPacket(0x17, 0x44);
            xSendPacket.XParse << pUser->GetUCID();
            xSendPacket.XParse << dwTargetActorID;
            CGocNetwork::SendBroadCast(pUser, xSendPacket, E_BROADCAST_TYPE::eNoneSelf);
        }
    });

    // IDA lambda192: 作业完成回调
    // TODO: 推测结果 - IDA 反编译中 lambda192 为按引用捕获 pUser 的收尾回调
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// IDA 0x1405198F0 - Defensive weapon attack
// 精确还原: 通过 DoJob 提交两个作业（lambda4 设置技能并攻击, lambda192 收尾）
bool CMonsterProcess::ReqDefensiveWeaponAttack(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::uint32_t dwTargetActorID = 0;
    std::uint8_t byAttackIdx = 0;
    xPacket.XParse >> dwTargetActorID;
    xPacket.XParse >> byAttackIdx;

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    std::int64_t instanceID = pUser->GetMapInsID().nMapID;

    // IDA lambda4: 设置技能并执行攻击
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser, &dwTargetActorID, &byAttackIdx]() {
        if (pUser && pUser->GetArea()) {
            CMover* pMoverObject = pUser->GetMoverObject(dwTargetActorID);
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverObject);
            if (pMonster) {
                TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
                if (pMobRef) {
                    pMonster->SetCurSkillTableIdx(
                        static_cast<int>((&pMobRef->Monster_Skill1_ID)[byAttackIdx]));
                    pMonster->ActionAttack();
                }
            }
        }
    });

    // IDA lambda192: 作业完成回调
    // TODO: 推测结果 - IDA 反编译中 lambda192 为按引用捕获 pUser 的收尾回调
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// IDA 0x140519B00 - Control monster attack
// 精确还原: 通过 DoJob 提交两个作业（lambda6 设置技能并攻击, lambda192 收尾）
bool CMonsterProcess::ReqControlMonsterAttack(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::uint32_t dwTargetActorID = 0;
    std::uint8_t byAttackIdx = 0;
    xPacket.XParse >> dwTargetActorID;
    xPacket.XParse >> byAttackIdx;

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    std::int64_t instanceID = pUser->GetMapInsID().nMapID;

    // IDA lambda6: 与 lambda4 同构 - 设置技能并执行攻击
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser, &dwTargetActorID, &byAttackIdx]() {
        if (pUser && pUser->GetArea()) {
            CMover* pMoverObject = pUser->GetMoverObject(dwTargetActorID);
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverObject);
            if (pMonster) {
                TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
                if (pMobRef) {
                    pMonster->SetCurSkillTableIdx(
                        static_cast<int>((&pMobRef->Monster_Skill1_ID)[byAttackIdx]));
                    pMonster->ActionAttack();
                }
            }
        }
    });

    // IDA lambda192: 作业完成回调
    // TODO: 推测结果 - IDA 反编译中 lambda192 为按引用捕获 pUser 的收尾回调
    CLogicThreadManager::Instance().DoJob(instanceID, [&pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

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
