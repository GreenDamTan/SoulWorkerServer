// CMyRoom.cpp
// CMyRoom MyRoom 房间类实现

#include "CMyRoom.h"
#include "ControlServer.h"  // 包含完整结构体定义

// 对齐 IDA 0x140039690: RoomEnterUser 用户进入房间
void CMyRoom::RoomEnterUser(ST_MYROOM_USER* pEnterUser, CServer* pSendServer, std::uint32_t dwOwnerUCID)
{
    if (m_byState == E_MYROOM_STATE_READY)
    {
        XSendPacket xSendPacket(0xF2, 0x50);
        xSendPacket.XParse << m_uxMapID.nMapID;
        xSendPacket << *pEnterUser;
        xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
        if (m_pServer)
        {
            m_pServer->SendEx(xSendPacket);
        }
        return;
    }

    PS_ENTER_MAP_RES stEnterMapRes{};
    ST_MYROOM_OWNER_INFO stOwnerInfo{};

    XSendPacket xSendPacket(0xF2, 0x51);
    xSendPacket.XParse << 100;
    xSendPacket << stEnterMapRes;
    xSendPacket << stOwnerInfo;
    xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
    if (pSendServer)
    {
        pSendServer->SendEx(xSendPacket);
    }
}

// 对齐 IDA 0x140039820: CreateMyRoom 初始化 MyRoom
void CMyRoom::CreateMyRoom(ST_MYROOM_OWNER_INFO* stOwnerInfo, UXMapID uxMapID, ST_MYROOM_USER* stEnterUser, CServer* pServer)
{
    (void)stEnterUser;

    if (stOwnerInfo)
    {
        memcpy(&m_stOwnerInfo, stOwnerInfo, sizeof(ST_MYROOM_OWNER_INFO));
    }
    m_pServer = pServer;
    m_uxMapID = uxMapID;
    m_byState = E_MYROOM_STATE_NONE;
}
