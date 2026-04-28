// CMyRoom.h
// CMyRoom MyRoom 房间类

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

// 对齐 IDA: CMyRoom MyRoom 房间类
// 布局: 前 68 字节为 ST_MYROOM_OWNER_INFO，然后是状态和地图ID
class CMyRoom {
public:
    CMyRoom() = default;
    virtual ~CMyRoom() = default;

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

    // 设置所有者信息
    void SetOwnerInfo(const ST_MYROOM_OWNER_INFO& stInfo) {
        memcpy(&m_stOwnerInfo, &stInfo, sizeof(ST_MYROOM_OWNER_INFO));
    }

    // 对齐 IDA 0x140027190: GetOwnerInfo 获取所有者信息
    ST_MYROOM_OWNER_INFO GetOwnerInfo() const {
        return m_stOwnerInfo;
    }

    // 对齐 IDA 0x140027160: GetUxMapID 获取地图ID
    UXMapID GetUxMapID() const {
        return m_uxMapID;
    }

    // 设置地图ID
    void SetUxMapID(UXMapID uxMapID) {
        m_uxMapID = uxMapID;
    }

    // 对齐 IDA 0x1400398B0: EnterSucc 进入成功
    void EnterSucc() {
        m_byState = E_MYROOM_STATE_READY;
    }

private:
    // 对齐 IDA: 前 68 字节为 ST_MYROOM_OWNER_INFO
    ST_MYROOM_OWNER_INFO m_stOwnerInfo{};  // +0x00, 68 bytes (dwUAID + dwUCID + szName[21])
    std::uint8_t m_byState = E_MYROOM_STATE_NONE;  // 对齐 IDA: 初始状态为 NONE
    UXMapID m_uxMapID{};  // 地图实例ID
};
