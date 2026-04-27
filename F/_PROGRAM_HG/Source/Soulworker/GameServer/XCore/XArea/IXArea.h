#pragma once

// 前置声明 - Vision Engine 类型
class XActor;
class XVec3;

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
};
