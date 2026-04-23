#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/Party.h"

class CServer;

class CPartyManager {
public:
    void Clear() {}
    // 对齐 IDA 0x1400BD5C0 UpdateServerState: 检查 Party 数据是否加载完成
    bool Isload() const { return m_bLoadParty; }

    // 对齐 IDA: 获取队伍信息
    std::shared_ptr<CParty> GetParty(std::uint32_t dwPartyID);
    std::uint32_t GetPartyID(UXActorID uxActorID) {
        const auto it = m_mapPartyUser.find(uxActorID);
        return it == m_mapPartyUser.end() ? 0u : it->second;
    }
    bool IsParty(std::uint32_t dwActorID);
    void AddPartyMember(std::uint32_t dwPartyID, std::uint32_t dwMemberID);
    void DeleteParty(std::uint32_t dwPartyID);
    void ResRecruitAccept(CServer* pServer, PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK& psCheck);
    void ReqJoinMember(CServer* pServer, PS_PARTY_ADDMEMBER& stAddMember, std::uint32_t dwUAID, std::uint8_t byLevel, std::uint32_t dwRecruitID);
    void ReqCreateParty(CServer* pServer, PS_REQ_PARTY_CREATE& stCreateParty);
    void ResJoinMember(PS_PARTY_ADDMEMBER& stAddMember, std::uint32_t dwRecruitID);
    void CreateParty(PS_REQ_PARTY_CREATE& stPartyReq);
    void ResPartyLeave(PS_PARTY_LEAVE& stPartyLeave, std::uint32_t dwNewMaster);
    void ResDeleteParty(PS_PARTY_LEAVE& stPartyLeave);
    void ResUpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stUpdateMember);
    void ResChangeMaster(PS_PARTY_CHANGE_MASTER& stChangeMaster);
    void ResLoadPartyAll(PS_PARTY_INFO_ALL& stPartyInfoAll, std::uint8_t byEnd);
    void CreatePartyMatching(PS_PARTY_INFO& stCreateParty);
    void SetMaze(std::uint32_t dwPartyID, UXMapID uxMapID, UXMapID uxBeforeMapID);
    void ReqInviteParty(CServer* pServer, PS_REQ_PARTY_INVITE& stPartyInvite, int dwUAID, std::uint8_t byLevel, std::uint32_t dwPartyID);
    void SendPartyErrorInvite(CServer* pServer, PS_REQ_PARTY_INVITE& stPartyInvite, int nErrorCode);
    void SendPartyErrorAccept(CServer* pServer, std::uint32_t dwActorID, int nErrorCode);
    void EnterServer(CServer* pServer, PS_REQ_PARTY_ENTER_SERVER& stEnterServer);

    // 对齐 IDA: 新增队伍管理方法
    void ReqLeaveMember(CServer* pServer, const PS_PARTY_LEAVE& stLeave,
                        std::uint32_t dwActorID, std::uint32_t dwUAID,
                        std::uint8_t byLevel, std::uint8_t byLeaverLevel);
    void ReqDeleteParty(CServer* pServer, const PS_PARTY_LEAVE& stLeave,
                        std::uint32_t dwActorID, std::uint32_t dwUAID,
                        std::uint8_t byLevel, std::uint8_t byLeaverLevel);
    void ReqChangeMaster(CServer* pServer, const PS_PARTY_CHANGE_MASTER& stChangeMaster);
    void ReqAcceptParty(CServer* pServer, const PS_RES_PARTY_INVITE& stAccept,
                        std::uint32_t dwUAID, std::uint8_t byLevel);
    void ReqCancelParty(CServer* pServer, PS_PARTY_REJECT* stReject);
    void SendPartyMessage(PS_CHAT_PARTY* pChatParty, PS_CHAT_ITEM_LINK_FOR_SERVER* pItemLink);
    void ReqMazeClear(std::uint32_t dwPartyID, std::uint8_t byClearFail);

private:
    std::shared_ptr<CParty> GetOrCreateParty(std::uint32_t dwPartyID);

    std::map<std::uint32_t, std::shared_ptr<CParty>> m_mapParty;
    std::map<UXActorID, std::uint32_t> m_mapPartyUser;
    // 对齐 IDA: 原版为 ClassFactory<CParty, 64>，内部使用 boost::object_pool<CParty>
    // 当前重建使用 std::make_shared 代替，此字段保留作为布局占位
    std::uint8_t m_factoryParty[48] = {};
    int m_nRequestNo = 0;
    bool m_bLoadParty = false;
    std::map<std::uint32_t, ST_INVITE_INFO> m_mapPartyInvite;
};
