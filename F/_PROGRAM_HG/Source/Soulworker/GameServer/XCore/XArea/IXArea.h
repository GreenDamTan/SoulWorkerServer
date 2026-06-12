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
    eNoneSelf = 0,              // 不发送给自己
    eAll = 1,                   // 发送给所有
    eNearby = 2,                // 发送给附近
    // Legacy names for backward compatibility
    E_BROADCAST_TYPE_NORMAL = eNoneSelf,
    E_BROADCAST_TYPE_ALL = eAll,
    E_BROADCAST_TYPE_NEARBY = eNearby,
};

// TODO: 推测结果 - 需要IDA验证
// IXArea 是 XArea 的基类接口
// 来自 IDA struct XArea field_0 (offset 0, size 8)
class IXArea {
public:
    virtual ~IXArea() = default;

    // 纯虚接口
    // IDA: ?EnterActor@XArea@@UEAAGPEAVXActor@@@Z - returns unsigned short (error code, 0 = success)
    virtual void OnUpdate(float fDelta) = 0;
    virtual std::uint16_t EnterActor(XActor* pActor) = 0;
    virtual void ExitActor(XActor* pActor) = 0;
    virtual XActor* FindActor(std::uint32_t dwActorID) = 0;

    // Broadcast interface
    // IDA: XArea::SendBroadCast
    virtual void SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) = 0;
    // IDA: XArea::SendBroadCastAfterLoading
    virtual void SendBroadCastAfterLoading(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) = 0;
    // IDA: ?ScanGridOrigin@XDistrict@@UEAAXMMEHKAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
    // Virtual function signature: (float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut)
    virtual void ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) = 0;
};
