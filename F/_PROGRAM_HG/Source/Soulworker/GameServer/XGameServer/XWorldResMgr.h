// XWorldResMgr.h
// World Resource Manager Stub

#pragma once

#include <cstdint>

// 前置声明（PDB: VEventObjectResource 2320B，VMap 子系统未还原）
class VEventObjectResource;
struct STPosInfo;

// XWorldResMgr - 世界资源管理器存根
class XWorldResMgr {
public:
    XWorldResMgr() = default;
    ~XWorldResMgr() = default;

    void Clear() {}
    static void LoadAll(XWorldResMgr* pMgr) {
        // TODO: 对齐 IDA 实现
    }

    // TODO: GetResource needs proper implementation
    static void* GetResource(XWorldResMgr* pMgr, std::uint16_t wMapID) {
        // TODO: 对齐 IDA 实现
        return nullptr;
    }

    // IDA: ?GetPortalPos@XWorldResMgr@@QEAA_NHHPEAUSTPosInfo@@@Z @ 0x1407203D0
    // 查找指定地图/跳跃点的传送门位置。依赖 VEventObjectResource/VMap 子系统未还原，
    // 活跃层暂返回 false，由后续 VEventObjectResource 子系统批次精确还原。
    // TODO: 需人工审查 - 依赖 VEventObjectResource::SearchFromID / VMap 未还原
    bool GetPortalPos(int nMapID, int nJumpID, STPosInfo* posInfo) {
        (void)nMapID; (void)nJumpID; (void)posInfo;
        return false;
    }

    // IDA: ?GetStartPortalPos@XWorldResMgr@@QEAA_NHPEAUSTPosInfo@@@Z @ 0x140720530
    // 查找地图起始传送门位置。依赖 VEventObjectResource/VMap 子系统未还原，
    // 活跃层暂返回 false。
    // TODO: 需人工审查 - 依赖 VEventObjectResource::GetMap / VMap 未还原
    bool GetStartPortalPos(int nMapID, STPosInfo* posInfo) {
        (void)nMapID; (void)posInfo;
        return false;
    }
};
