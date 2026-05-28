#pragma once

#include "Soulworker/GameServer/XCore/XArea/IXArea.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include <cstdint>
#include <list>
#include <map>
#include <vector>

// 前置声明
class XActor;
class CFSRWLock;
struct ST_WORLD_MODE_INFO;
struct ST_WORLD_MODE_INFO_VEC;

// UXMapID 结构
struct TUXMapID {
    std::uint16_t wMapID;
    std::uint16_t wInstanceID;

    TUXMapID() : wMapID(0), wInstanceID(0) {}
    TUXMapID(std::uint16_t wMap, std::uint16_t wInstance) : wMapID(wMap), wInstanceID(wInstance) {}

    bool operator<(const TUXMapID& other) const {
        if (wMapID != other.wMapID) return wMapID < other.wMapID;
        return wInstanceID < other.wInstanceID;
    }

    bool operator==(const TUXMapID& other) const {
        return wMapID == other.wMapID && wInstanceID == other.wInstanceID;
    }
};

// TXMap 已在 GreenDamTan_ClientBase.h 中定义为 ATL::CAtlMap 包装
// 这里不再重复定义

// TODO: 推测结果 - 来自 IDA struct XArea (192 bytes)
class XArea : public IXArea {
public:
    XArea();
    virtual ~XArea();

    // 实现 IXArea 接口
    void OnUpdate(float fDelta) override;
    void EnterActor(XActor* pActor) override;
    void ExitActor(XActor* pActor) override;
    XActor* FindActor(std::uint32_t dwActorID) override;

    // Broadcast interface
    // IDA: XArea::SendBroadCast
    void SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) override;
    // IDA: XArea::SendBroadCastAfterLoading
    void SendBroadCastAfterLoading(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) override;
    // IDA: XArea::ScanGridOrigin
    void ScanGridOrigin(XActor* pActor, int nRange, unsigned int uFlag, std::vector<CMover*>& vecOut) override;

    // 访问器
    TUXMapID GetMapID() const { return m_uxMapID; }
    void SetMapID(TUXMapID uxMapID) { m_uxMapID = uxMapID; }
    int GetMaxUserCount() const { return m_nMaxUserCount; }

    // 获取世界类型 (0=未知, 1=迷宫, 2=战场等)
    virtual int GetWorldType() { return 0; }

protected:
    // === IDA 确认的成员变量 (offset from struct start) ===
    // offset 0: IXArea vtable (继承)

    // offset 8: m_mapActor (TXMap<unsigned long, XActor*>, 72 bytes)
    // TODO: 推测目录归属 - TXMap 内部布局需要验证
    std::map<std::uint32_t, XActor*> m_mapActor;

    // offset 80: m_rwLock (CFSRWLock, 8 bytes)
    CFSRWLock m_rwLock;

    // offset 88: m_uxMapID (UXMapID, 8 bytes)
    TUXMapID m_uxMapID;

    // offset 96: m_nMaxUserCount (int)
    int m_nMaxUserCount;

    // offset 104: m_listSpawnNPC (std::list<unsigned long>, 24 bytes)
    std::list<std::uint32_t> m_listSpawnNPC;

    // offset 128: m_mapWorldMode (std::map<int, ST_WORLD_MODE_INFO>, 32 bytes)
    std::map<int, void*> m_mapWorldMode;  // TODO: 需人工审查 - 类型待确认

    // offset 160: m_vecWorldModeList (ST_WORLD_MODE_INFO_VEC, 32 bytes)
    // TODO: 需人工审查 - 需要 ST_WORLD_MODE_INFO_VEC 定义
    std::vector<std::uint8_t> m_vecWorldModeList_dummy;  // 临时占位

    // Total size: 192 bytes (verified from IDA)
};
