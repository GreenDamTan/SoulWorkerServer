#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CGameDBSocket : public TXDBSocketT<CServer> {
public:
    bool OnParse(XPacket& xPacket) override;
    CServer* FindUser(unsigned int xSessionID) override;
    bool DBParse(CServer* pServer, XPacket& xPacket) override;
    bool DBPartyParse(XPacket& xPacket);
    bool DBFriendParse(XPacket& xPacket);
    bool DBForceParse(XPacket& xPacket);
    bool ResRecruitDelete(XPacket& xPacket);
    bool ResForceMatchingCreate(XPacket& xPacket);
    bool ResPartyJoin(XPacket& xPacket);
    bool ResForceJoin(XPacket& xPacket);
    bool ResPartyCreate(XPacket& xPacket);
    bool ResForceCreate(XPacket& xPacket);
    bool ResPartyLeave(XPacket& xPacket);
    bool ResPartyDelete(XPacket& xPacket);
    bool ResPartyUpdateMemberInfo(XPacket& xPacket);
    bool ResPartyChangeMaster(XPacket& xPacket);
    bool ResForceUpdateMemberInfo(XPacket& xPacket);
    bool ResForceChangeMaster(XPacket& xPacket);
    bool ResForceLeave(XPacket& xPacket);
    bool ResForceDelete(XPacket& xPacket);
    bool ResPartyLoadAll(XPacket& xPacket);
    bool ResPartyMatchingCreate(XPacket& xPacket);
    bool ResForceLoadAll(XPacket& xPacket);
};

class XGameDBSocketMgr {
public:
    XGameDBSocketMgr() = default;
    ~XGameDBSocketMgr();

    void Init();
    void AutoConnect();
    void DisConnect();
    bool SendAccountDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    bool SendGameDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    int GetGameDBAgentCount() const;
    int GetAccountDBAgentCount() const;

private:
    void InitAgentGroup(std::uint8_t byType, CGameDBSocket*& ppAgents, int& nAgentCount);
    void AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount);
    void DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount);
    void Clear();

    CGameDBSocket* m_pGameDBAgent = nullptr;
    int m_nGameAgentCnt = 0;
    CGameDBSocket* m_pAccountDBAgent = nullptr;
    int m_nAccountAgentCnt = 0;
};
