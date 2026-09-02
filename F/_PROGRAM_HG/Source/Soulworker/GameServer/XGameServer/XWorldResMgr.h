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

    // IDA: ?GetResource@XWorldResMgr@@QEAAPEAVVEventObjectResource@@F@Z (0x140720370)
    // PDB publics RVA 0x71F370; 非静态成员函数，参数为 int16 (short)，返回 VEventObjectResource*
    // 依赖 Vision VMap 子系统未还原，活跃层暂返回 nullptr，由后续子系统批次精确还原。
    // TODO: 需人工审查 - 依赖 VEventObjectResource / VMap 未还原
    VEventObjectResource* GetResource(std::int16_t nMapID) {
        (void)nMapID;
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

    // IDA: ?GetStartPortalID@XWorldResMgr@@QEAA_NHAEAHPEAUSTPosInfo@@@Z @ 0x140720360
    // 查找地图起始传送门 ID 与位置（XForceProcess 匹配链依赖）。
    // 依赖 VEventObjectResource/VMap 子系统未还原，活跃层暂返回 false，
    // 由匹配进入链路按 55003 错误码拒绝。
    // TODO: 需人工审查 - 依赖 VEventObjectResource / VMap 未还原
    bool GetStartPortalID(int nMapID, int* pJumpID, STPosInfo* posInfo) {
        (void)nMapID; (void)pJumpID; (void)posInfo;
        return false;
    }

    // IDA: ?CanEnterPortal@XWorldResMgr@@QEAA_NHHPEAVCUser@@@Z @ 0x14071FA90
    // 校验当前地图能否通过指定传送门进入（XForceProcess 匹配链依赖）。
    // 依赖 VEventObjectResource/VMap 子系统未还原，活跃层暂返回 false，
    // 由匹配进入链路按 55007 错误码拒绝。
    // TODO: 需人工审查 - 依赖 VEventObjectResource / VMap 未还原
    bool CanEnterPortal(int nMapID, int nJumpID, CUser* pUser) {
        (void)nMapID; (void)nJumpID; (void)pUser;
        return false;
    }

    // IDA: ?CheckEnterMapPortalPos@XWorldResMgr@@QEAA_NPEAVCUser@@HH@Z @ 0x140720C60
    // 校验用户能否通过目标地图的传送门进入:
    // 30031 直接放行; Maze_Type==1 放行; 其余在资源中 SearchFromID(nPortalID)
    // 找到 Box 型(eEventObjectType_Box, BoxType==5) 时按 2000 单位距离校验。
    // 依赖 VEventObjectResource/VMap 子系统未还原，活跃层暂返回 false，
    // 由进图链路按对应错误码拒绝。
    // TODO: 需人工审查 - 依赖 VEventObjectResource / VMap 未还原
    bool CheckEnterMapPortalPos(CUser* pUser, int nEnterMapID, int nPortalID) {
        (void)pUser; (void)nEnterMapID; (void)nPortalID;
        return false;
    }

    // IDA: ?GetJumpID@XWorldResMgr@@QEAAHHH@Z @ 0x140721A70
    // 遍历地图资源的 Box 型事件对象 (BoxType==5, iID==nPortalID)，
    // 返回对应 VPortalBoxInfo::m_iJump，找不到返回 0。
    // 依赖 VEventObjectResource/VMap 子系统未还原，活跃层暂返回 0。
    // TODO: 需人工审查 - 依赖 VEventObjectResource / VMap 未还原
    int GetJumpID(std::int16_t nMapID, int nPortalID) {
        (void)nMapID; (void)nPortalID;
        return 0;
    }
};
