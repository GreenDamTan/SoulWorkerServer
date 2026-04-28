// CMyRoom.cpp
// CMyRoom MyRoom 房间类实现

#include "CMyRoom.h"
#include "ControlServer.h"  // 包含完整结构体定义

// 对齐 IDA 0x140039690: RoomEnterUser 用户进入房间
void CMyRoom::RoomEnterUser(ST_MYROOM_USER* pEnterUser, CServer* pSendServer, std::uint32_t dwOwnerUCID)
{
    // 对齐 IDA: 用户进入房间处理
    GreenDamTan_log(__FILE__, __FUNCTION__, "RoomEnterUser called");
}

// 对齐 IDA 0x140039820: CreateMyRoom 初始化 MyRoom
void CMyRoom::CreateMyRoom(ST_MYROOM_OWNER_INFO* stOwnerInfo, UXMapID uxMapID, ST_MYROOM_USER* stEnterUser, CServer* pServer)
{
    // 对齐 IDA: 初始化 MyRoom
    if (stOwnerInfo) {
        memcpy(&m_stOwnerInfo, stOwnerInfo, sizeof(ST_MYROOM_OWNER_INFO));
    }
    m_uxMapID = uxMapID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMyRoom called");
}
