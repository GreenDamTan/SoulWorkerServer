#pragma once

#include <cstdint>
#include <vector>

// 前置声明 - Vision Engine 类型
class XActor;
class XSendPacket;
class XVec3;
class CMover;

// E_BROADCAST_TYPE - Broadcast type enumeration for area broadcast
// IDA: Used by CGocNetwork::SendBroadCast and XArea::SendBroadCast
enum class E_BROADCAST_TYPE : std::uint32_t {
    E_BROADCAST_TYPE_NORMAL = 0,
    E_BROADCAST_TYPE_NEARBY = 1,
    E_BROADCAST_TYPE_ALL = 2,
};

// TODO: 推测结果 - 需要IDA验证
// IXArea 是 XArea 的基类接口
// 来自 IDA struct XArea field_0 (offset 0, size 8)
class IXArea {
public:
    virtual ~IXArea() = default;

    // 纯虚接口
    virtual void OnUpdate(float fDelta) = 0;
    virtual void EnterActor(XActor* pActor) = 0;
    virtual void ExitActor(XActor* pActor) = 0;
    virtual XActor* FindActor(std::uint32_t dwActorID) = 0;

    // Broadcast interface
    // IDA: XArea::SendBroadCast
    virtual void SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) = 0;
    // IDA: XArea::SendBroadCastAfterLoading
    virtual void SendBroadCastAfterLoading(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) = 0;
    // IDA: XArea::ScanGridOrigin
    virtual void ScanGridOrigin(XActor* pActor, int nRange, unsigned int uFlag, std::vector<CMover*>& vecOut) = 0;
};
