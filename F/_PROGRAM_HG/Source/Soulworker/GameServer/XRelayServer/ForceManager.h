#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/Force.h"

class CServer;

class CForceManager {
public:
    void Clear() {}
    void ResUpdateMemberInfo(ST_UPDATE_FORCE_MEMBER& stUpdateMember);
    void SendForceMessage(PS_CHAT_PARTY& stChatForce, PS_CHAT_ITEM_LINK_FOR_SERVER& psChatItemLinkInfo);
    void EnterServer(CServer* pServer, PS_REQ_FORCE_ENTER_SERVER& stEnterServer);
    bool CreateForceMatching(const PS_FORCE_INFO& stCreateForce);
    void DeleteForce(std::uint32_t dwForceID);
    std::shared_ptr<CForce> GetForce(std::uint32_t dwForceID);
    std::shared_ptr<CForce> GetForce(UXActorID uxActorID);
    void ReqForceInfo(CServer* pServer, std::uint32_t dwForceID, std::uint32_t dwActorID);
    void ReqJoinMember(CServer* pServer, PS_FORCE_ADDMEMBER& stAddMember, std::uint32_t dwUAID, std::uint8_t byLevel, std::uint32_t dwRecruitID);
    void ReqCreateForce(CServer* pServer, PS_REQ_FORCE_CREATE& stCreateForce);
    void ResJoinMember(PS_FORCE_ADDMEMBER& stAddMember, std::uint32_t dwRecruitID);
    void CreateForce(PS_REQ_FORCE_CREATE& stForceReq);
    void ResChangeMaster(PS_FORCE_CHANGE_MASTER& stChangeMaster);
    void ResForceLeave(PS_FORCE_LEAVE& stForceLeave, std::uint32_t dwNewMaster);
    void ResDeleteForce(PS_FORCE_LEAVE& stForceLeave);
    void ResLoadForceAll(PS_FORCE_INFO_ALL& stForceInfoAll, std::uint8_t byEnd);
    void SetMaze(std::uint32_t dwForceID, UXMapID uxMapID, UXMapID uxBeforeMapID);
    void ReqInviteForce(CServer* pServer, PS_REQ_FORCE_INVITE& stForceInvite, int dwUAID, std::uint8_t byLevel, std::uint32_t dwForceID);
    void SendForceErrorInvite(CServer* pServer, PS_REQ_FORCE_INVITE& stForceInvite, int nErrorCode);
    void ReqAcceptForce(CServer* pServer, PS_RES_FORCE_INVITE& stAcceptForce, std::uint32_t dwUAID, std::uint8_t byLevel);
    void SendForceErrorAccept(CServer* pServer, std::uint32_t dwActorID, int nErrorCode);
    void ReqCancelForce(CServer* pServer, PS_PARTY_REJECT& stForceReject);
    void ReqForceLeave(CServer* pServer, PS_FORCE_LEAVE& stForceLeave, std::uint32_t dwActorID, std::uint32_t dwUAID, std::uint8_t byLevel, std::uint8_t byLeaveLevel);
    void ReqChangeMaster(CServer* pServer, PS_FORCE_CHANGE_MASTER& stChangeMaster);
    bool ReqForceMazeClear(std::uint32_t dwForceID);
    void ReqDeleteForce(CServer* pServer, PS_FORCE_LEAVE& stForceLeave, std::uint32_t dwActorID, std::uint32_t dwUAID, std::uint8_t byLevel);
    void RemoveForceMember(std::uint32_t dwMemberID);

private:
    std::shared_ptr<CForce> GetOrCreateForce(std::uint32_t dwForceID);

    std::map<std::uint32_t, std::shared_ptr<CForce>> m_mapForce;
    std::map<UXActorID, std::uint32_t> m_mapForceUser;
    // 对齐 IDA: 原版为 ClassFactory<CForce, 64>，内部使用 boost::object_pool<CForce>
    // 当前重建使用 std::make_shared 代替，此字段保留作为布局占位
    std::uint8_t m_factoryForce[48] = {};
    int m_nRequestNo = 0;
    bool m_bLoadForce = false;
    std::map<std::uint32_t, ST_FORCE_INVITE_INFO> m_mapForceInvite;
};

