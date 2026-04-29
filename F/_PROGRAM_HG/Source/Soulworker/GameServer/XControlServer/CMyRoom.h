// CMyRoom.h
// CMyRoom MyRoom 房间类
// 对齐 IDA ControlServer.exe

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdint>
#include <memory>

class CServer;

// MyRoom 状态枚举 (对齐 IDA)
// 状态机: NONE -> READY(创建成功后) -> DELETE(删除请求后)
enum E_MYROOM_STATE
{
    E_MYROOM_STATE_NONE = 0,        // 初始状态
    E_MYROOM_STATE_READY = 1,       // 就绪 (CreateSucc 设置)
    E_MYROOM_STATE_DELETE = 2,      // 删除中 (DeleteReq 设置)
};

// UXMapID 已在 PSCommon.h 中定义，直接使用

// 对齐 IDA: CMyRoom MyRoom 房间类
// 注意：IDA struct_info 显示无 vtable (m_stOwnerInfo at offset 0)，所以析构函数非 virtual
// 布局: ST_MYROOM_OWNER_INFO (68 bytes) -> padding(4) -> CServer*(8) -> UXMapID(8) -> state(1)
class CMyRoom {
public:
    CMyRoom();
    ~CMyRoom();  // 非 virtual，对齐 IDA struct_info

    // 对齐 IDA 0x140027150: GetMyRoomState 获取房间状态
    std::uint8_t GetMyRoomState() const {
        return m_byState;
    }

    // 对齐 IDA 0x140027140: DeleteReq 删除请求
    void DeleteReq() {
        m_byState = E_MYROOM_STATE_DELETE;
    }

    // 对齐 IDA 0x140039690: RoomEnterUser 用户进入房间
    void RoomEnterUser(ST_MYROOM_USER* pEnterUser, CServer* pSendServer, std::uint32_t dwOwnerUCID);

    // 对齐 IDA 0x1400271E0: CreateSucc 创建成功
    void CreateSucc() {
        m_byState = E_MYROOM_STATE_READY;
    }

    // 对齐 IDA 0x140039820: CreateMyRoom 初始化 MyRoom
    void CreateMyRoom(ST_MYROOM_OWNER_INFO* stOwnerInfo, UXMapID uxMapID, ST_MYROOM_USER* stEnterUser, CServer* pServer);

    // 对齐 IDA 0x140039630: Init 初始化
    void Init();

    // 对齐 IDA 0x1400398B0: EnterSucc 进入成功
    void EnterSucc();

    // 对齐 IDA 0x140027190: GetOwnerInfo 获取所有者信息
    ST_MYROOM_OWNER_INFO GetOwnerInfo() const {
        return m_stOwnerInfo;
    }

    // 对齐 IDA 0x140027160: GetUxMapID 获取地图ID
    UXMapID GetUxMapID() const {
        return m_uxMapID;
    }

    // 设置所有者信息
    void SetOwnerInfo(const ST_MYROOM_OWNER_INFO& stInfo) {
        m_stOwnerInfo = stInfo;
    }

    // 设置地图ID
    void SetUxMapID(UXMapID uxMapID) {
        m_uxMapID = uxMapID;
    }

private:
    // 对齐 IDA 布局 (总大小 96 bytes / 0x60):
    // Init: memset 68 bytes (0x44), mov [rax+48h]=0, UXMapID::operator= at +50h, mov [rax+58h]=0
    ST_MYROOM_OWNER_INFO m_stOwnerInfo{};  // +0x00 (0), 68 bytes
    // +0x44 (68) 到 +0x48 (72): 4 bytes padding (对齐到 8 bytes boundary)
    CServer* m_pServer = nullptr;          // +0x48 (72), 8 bytes
    UXMapID m_uxMapID{};                   // +0x50 (80), 8 bytes
    std::uint8_t m_byState = E_MYROOM_STATE_NONE; // +0x58 (88), 1 byte
    // +0x59 (89) 到 +0x60 (96): 7 bytes padding
};
static_assert(sizeof(CMyRoom) == 96, "CMyRoom size mismatch with IDA (96 bytes)");
