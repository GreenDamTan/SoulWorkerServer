// VaccumCubeProcess.cpp
// VaccumCube protocol processor implementation
// IDA decompilation from GameServer.exe

#include "Soulworker/GameServer/XGameServer/VaccumCubeProcess.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XGameServer/ManagerStubs.h"
#include <string>

namespace {
#pragma pack(push, 1)
struct PS_REQ_VACCUM_CLICK_START {
    int nID = 0;
};

struct PS_REQ_VACCUM_CLICK_CANCEL {
    int nID = 0;
};
#pragma pack(pop)

static_assert(sizeof(PS_REQ_VACCUM_CLICK_START) == 4, "PS_REQ_VACCUM_CLICK_START layout mismatch");
static_assert(sizeof(PS_REQ_VACCUM_CLICK_CANCEL) == 4, "PS_REQ_VACCUM_CLICK_CANCEL layout mismatch");

XPacket& operator>>(XPacket& xPacket, PS_REQ_VACCUM_CLICK_START& stReq) {
    xPacket.XParse >> stReq.nID;
    return xPacket;
}

XPacket& operator>>(XPacket& xPacket, PS_REQ_VACCUM_CLICK_CANCEL& stReq) {
    xPacket.XParse >> stReq.nID;
    return xPacket;
}

XActor* GetEmbeddedActor(CUser* pUser) {
    if (!pUser) {
        return nullptr;
    }

    CMover* pMover = static_cast<CMover*>(pUser);
    return reinterpret_cast<XActor*>(reinterpret_cast<std::uint8_t*>(pMover) + 872);
}

std::int64_t GetLogicThreadIndex(XActor* pActor) {
    if (!pActor) {
        return 0;
    }

    const UXMapID uxMapID = pActor->GetMapInsID();
    return uxMapID.nMapID;
}
} // namespace

// Constructor
// IDA @ 0x140622800
CVaccumCubeProcess::CVaccumCubeProcess()
    : TXProcess<CUser>()
{
    // IDA code:
    // CVaccumCubeProcess *__fastcall CVaccumCubeProcess::CVaccumCubeProcess(CVaccumCubeProcess *this)
    // {
    //   std::string v2; // [rsp+20h] [rbp-48h] BYREF
    //   std::string *v3; // [rsp+48h] [rbp-20h]
    //   __int64 v4; // [rsp+50h] [rbp-18h]
    //   std::string *v5; // [rsp+58h] [rbp-10h]
    //
    //   v4 = -2;
    //   TXProcess<CUser>::TXProcess<CUser>(this);
    //   this->__vftable = (CVaccumCubeProcess_vtbl *)&CVaccumCubeProcess::`vftable';
    //   IXComponent::SetCmd(this, 0x25u);
    //   v3 = &v2;
    //   v5 = std::string::string(&v2, "CVaccumCubeProcess");
    //   IXComponent::SetName(this, v5);
    //   return this;
    // }

    // Note: Base class constructor called, vtable set
    // SetCmd(0x25); // eMAIN_CMD_VACCUM_CUBE
    // SetName("CVaccumCubeProcess");
}

// Destructor
// IDA @ 0x1406228C0
CVaccumCubeProcess::~CVaccumCubeProcess()
{
    // IDA code:
    // void __fastcall CVaccumCubeProcess::~CVaccumCubeProcess(CVaccumCubeProcess *this)
    // {
    //   this->__vftable = (CVaccumCubeProcess_vtbl *)&CVaccumCubeProcess::`vftable';
    //   TXProcess<CUser>::~TXProcess<CUser>(this);
    // }
    // Note: vtable restored, base destructor called
}

// Parse - Parse incoming packet
// IDA @ 0x1406228F0
bool CVaccumCubeProcess::Parse(XPacket& xPacket)
{
    // IDA code:
    // char __fastcall CVaccumCubeProcess::Parse(CVaccumCubeProcess *this, XPacket *xPacket)
    // {
    //   unsigned __int8 SubCmd; // [rsp+24h] [rbp-14h]
    //
    //   SubCmd = XPacket::GetSubCmd(xPacket);
    //   if ( SubCmd == 1 )
    //     return CVaccumCubeProcess::ReqVaccumClickStart(this, xPacket);
    //   if ( SubCmd == 2 )
    //     return CVaccumCubeProcess::ReqVaccumClickCancel(this, xPacket);
    //   return 1;
    // }

    std::uint8_t SubCmd = xPacket.GetSubCmd();
    if (SubCmd == 1)
        return ReqVaccumClickStart(xPacket);
    if (SubCmd == 2)
        return ReqVaccumClickCancel(xPacket);
    return true;
}

// ReqVaccumClickStart - Handle vaccum click start request (subcmd 1)
// IDA @ 0x140622950
bool CVaccumCubeProcess::ReqVaccumClickStart(XPacket& xPacket)
{
    // IDA code:
    // char __fastcall CVaccumCubeProcess::ReqVaccumClickStart(CVaccumCubeProcess *this, XPacket *xPacket)
    // {
    //   ... complex logic with lambda functions and thread management ...
    //   operator>>(xPacket, &stVaccumReq);
    //   pUser = TXProcess<CUser>::GetClientPtr(this);
    //   if ( !pUser )
    //     return 0;
    //   if ( !v19->GetArea(&pUser->XActor) )
    //     return 0;
    //   XClient::IncrementJobCount(pUser);
    //   ... lambda dispatch to logic thread ...
    //   return 1;
    // }

    PS_REQ_VACCUM_CLICK_START stVaccumReq;
    xPacket >> stVaccumReq;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    XActor* pActor = GetEmbeddedActor(pUser);
    XArea* pArea = pActor ? pActor->GetArea() : nullptr;
    if (!pArea)
        return false;

    pUser->IncrementJobCount();

    std::function<void()> clickJob = [pUser, stVaccumReq]() {
        XActor* pJobActor = GetEmbeddedActor(pUser);
        XArea* pJobArea = pJobActor ? pJobActor->GetArea() : nullptr;
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pJobArea);
        if (pBattleZone) {
            CVaccumManager* pVaccumManager = pBattleZone->GetVaccumManager();
            if (pVaccumManager) {
                pVaccumManager->ClickVaccumCube(stVaccumReq.nID, pJobActor);
            }
        }
    };

    const std::int64_t nIndex = GetLogicThreadIndex(pActor);
    CLogicThreadManager::DoJob(TXSingleton<CLogicThreadManager>::Instance(), nIndex, &clickJob);

    std::function<void()> decrementJob = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::DoJob(TXSingleton<CLogicThreadManager>::Instance(), nIndex, &decrementJob);

    return true;
}

// ReqVaccumClickCancel - Handle vaccum click cancel request (subcmd 2)
// IDA @ 0x140622E40
bool CVaccumCubeProcess::ReqVaccumClickCancel(XPacket& xPacket)
{
    // IDA code:
    // char __fastcall CVaccumCubeProcess::ReqVaccumClickCancel(CVaccumCubeProcess *this, XPacket *xPacket)
    // {
    //   ... complex logic with lambda functions and thread management ...
    //   operator>>(xPacket, &stVaccumReq);
    //   pUser = TXProcess<CUser>::GetClientPtr(this);
    //   if ( !pUser )
    //     return 0;
    //   if ( !v19->GetArea(&pUser->XActor) )
    //     return 0;
    //   XClient::IncrementJobCount(pUser);
    //   ... lambda dispatch to logic thread ...
    //   return 1;
    // }

    PS_REQ_VACCUM_CLICK_CANCEL stVaccumReq;
    xPacket >> stVaccumReq;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    XActor* pActor = GetEmbeddedActor(pUser);
    XArea* pArea = pActor ? pActor->GetArea() : nullptr;
    if (!pArea)
        return false;

    pUser->IncrementJobCount();

    std::function<void()> cancelJob = [pUser, stVaccumReq]() {
        XActor* pJobActor = GetEmbeddedActor(pUser);
        XArea* pJobArea = pJobActor ? pJobActor->GetArea() : nullptr;
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pJobArea);
        if (pBattleZone) {
            CVaccumManager* pVaccumManager = pBattleZone->GetVaccumManager();
            if (pVaccumManager) {
                pVaccumManager->CancelClickVaccumCube(stVaccumReq.nID, pJobActor);
            }
        }
    };

    const std::int64_t nIndex = GetLogicThreadIndex(pActor);
    CLogicThreadManager::DoJob(TXSingleton<CLogicThreadManager>::Instance(), nIndex, &cancelJob);

    std::function<void()> decrementJob = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::DoJob(TXSingleton<CLogicThreadManager>::Instance(), nIndex, &decrementJob);

    return true;
}
