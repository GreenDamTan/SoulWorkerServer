// CMyRoom.cpp
// CMyRoom MyRoom 房间类实现
// 对齐 IDA ControlServer.exe

#include "CMyRoom.h"
#include "ControlServer.h"  // 包含完整结构体定义

// 对齐 IDA 0x1400395F0: CMyRoom 构造函数
CMyRoom::CMyRoom()
{
    // ST_MYROOM_OWNER_INFO 默认构造
    memset(&m_stOwnerInfo, 0, sizeof(m_stOwnerInfo));
    // 初始化成员
    Init();
}

// 对齐 IDA: CMyRoom 析构函数
CMyRoom::~CMyRoom()
{
    // 默认析构
}

// 对齐 IDA 0x140039630: Init 初始化
// IDA: memset(this, 0, 0x44u); m_pServer=nullptr; UXMapID::operator=(&m_uxMapID, nullptr); m_byState=0
void CMyRoom::Init()
{
    // 清空前 68 bytes (m_stOwnerInfo 在 offset 0)
    memset(&m_stOwnerInfo, 0, sizeof(m_stOwnerInfo));  // IDA: memset(this, 0, 0x44u)
    m_pServer = nullptr;                                // IDA: mov [rax+48h], 0
    m_uxMapID.nMapID = 0;                               // IDA: UXMapID::operator=
    m_byState = E_MYROOM_STATE_NONE;                    // IDA: mov [rax+58h], 0
}

// 对齐 IDA 0x1400398B0: EnterSucc 进入成功
// IDA: if (this->m_byState == 2) this->m_byState = 1;
void CMyRoom::EnterSucc()
{
    // 对齐 IDA: 只有在 DELETE 状态(2)下才转换为 READY(1)
    if (m_byState == E_MYROOM_STATE_DELETE)
    {
        m_byState = E_MYROOM_STATE_READY;
    }
}

// 对齐 IDA 0x140039690: RoomEnterUser 用户进入房间
// IDA 签名: void __fastcall CMyRoom::RoomEnterUser(CMyRoom *this, ST_MYROOM_USER *stEnterUser, CServer *pServer, int dwOwnerUCID)
void CMyRoom::RoomEnterUser(ST_MYROOM_USER* pEnterUser, CServer* pSendServer, std::uint32_t dwOwnerUCID)
{
    if (m_byState == E_MYROOM_STATE_READY)
    {
        // 对齐 IDA: 房间就绪，发送进入成功响应 (main=0xF2, sub=0x50)
        XSendPacket xSendPacket(0xF2, 0x50);
        xSendPacket.XParse << m_uxMapID.nMapID;  // IDA: UXMapID::operator<< (完整结构)
        xSendPacket << *pEnterUser;
        xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
        // 对齐 IDA: 使用 m_pServer (this->m_pServer)
        if (m_pServer)
        {
            m_pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 对齐 IDA: 房间未就绪，发送进入失败响应 (main=0xF2, sub=0x51)
    PS_ENTER_MAP_RES stEnterMapRes{};
    ST_MYROOM_OWNER_INFO stOwnerInfo{};

    XSendPacket xSendPacket(0xF2, 0x51);
    xSendPacket.XParse << 100;  // 错误码 (IDA: XParse::operator<<(100))
    xSendPacket << stEnterMapRes;
    xSendPacket << stOwnerInfo;
    xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
    // 对齐 IDA: 使用 pServer 参数 (pSendServer)
    if (pSendServer)
    {
        pSendServer->SendEx(xSendPacket);
    }
}

// 对齐 IDA 0x140039820: CreateMyRoom 初始化 MyRoom
void CMyRoom::CreateMyRoom(ST_MYROOM_OWNER_INFO* stOwnerInfo, UXMapID uxMapID, ST_MYROOM_USER* stEnterUser, CServer* pServer)
{
    (void)stEnterUser;  // 参数保留但未使用 (IDA 显示)

    if (stOwnerInfo)
    {
        // 复制所有者信息 (68 bytes)
        m_stOwnerInfo = *stOwnerInfo;
    }
    m_uxMapID = uxMapID;
    m_pServer = pServer;
    m_byState = E_MYROOM_STATE_NONE;
}
