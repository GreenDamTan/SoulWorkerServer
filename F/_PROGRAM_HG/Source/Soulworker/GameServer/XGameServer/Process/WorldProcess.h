// WorldProcess.h
// CWorldProcess - World(地图/世界) 请求处理器 (main cmd 4)
// 对齐 IDA GameServer.exe；原始归属 process/worldprocess.cpp
// (PDB cvdump MD5 3DD7FD4089D879CFE722CC61A40CA3D7,
//  OBJ: F:\_PROGRAM_HG\Bin\Server_Release\x64\XGameServer\WorldProcess.obj)

#pragma once

#include "../../XCore/XIOCPServer/TXProcess.h"
#include <cstdint>

class CUser;
class XPacket;
struct STPosInfo;
struct PS_ENTER_MAP_REQ;

// 全局地图类型查询 - IDA: ?GetWorldType@@YAHH@Z @ 0x1406281E0
// nMapID >= 1000 时返回 nMapID/10000 - 1，否则返回 0
int GetWorldType(int nMapID);

// ============================================================================
// CWorldProcess - World 请求处理类 (main=4)
//
// 客户端 -> GameServer 的世界/地图请求处理：
// - Parse 按 subcmd 分发到 ReqWorld* 处理器
// - ReqWorldEnterToOther 由 CUser::EnterWorldToOther 调用，
//   处理跨玩家传送（好友追踪/寻找玩家链）
// ============================================================================
class CWorldProcess : public TXProcess<CUser> {
public:
    // IDA: ??0CWorldProcess@@QEAA@XZ @ 0x140628210
    CWorldProcess();

    // IDA: ??1CWorldProcess@@UEAA@XZ @ 0x1406282D0
    virtual ~CWorldProcess();

    // === 主分发 ===
    // IDA: ?Parse@CWorldProcess@@UEAA_NAEAVXPacket@@@Z @ 0x140628300
    virtual bool Parse(XPacket& xPacket) override;

    // === 请求处理器 ===
    // IDA: ?ReqWorldEnter@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x1406284D0
    bool ReqWorldEnter(XPacket& xPacket);
    // IDA: ?ReqWorldEnterToOther@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062CE60
    bool ReqWorldEnterToOther(XPacket& xPacket);
    // IDA: ?ReqWorldVersion@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062F150
    bool ReqWorldVersion(XPacket& xPacket);
    // IDA: ?ReqWorldObjectInfo@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062A250
    bool ReqWorldObjectInfo(XPacket& xPacket);
    // IDA: ?ReqWorldWarp@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062B770
    bool ReqWorldWarp(XPacket& xPacket);
    // IDA: ?ReqWorldCheckSector@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062C440
    bool ReqWorldCheckSector(XPacket& xPacket);
    // IDA: ?ReqWorldWarpOut@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062C1A0
    bool ReqWorldWarpOut(XPacket& xPacket);
    // IDA: ?ReqWorldMoverInfo@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062C8F0
    bool ReqWorldMoverInfo(XPacket& xPacket);
    // IDA: ?ReqWorldEscape@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062EC90
    bool ReqWorldEscape(XPacket& xPacket);
    // IDA: ?ReqGameOut@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062F500
    bool ReqGameOut(XPacket& xPacket);
    // IDA: ?ReqWorldDistrictTransport@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062F910
    bool ReqWorldDistrictTransport(XPacket& xPacket);
    // IDA: ?ReqWorldEnterByForce@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x14062DA90
    bool ReqWorldEnterByForce(XPacket& xPacket);
    // IDA: ?ReqWorldSkipInTutorial02@CWorldProcess@@QEAA_NAEAVXPacket@@@Z @ 0x140632240
    bool ReqWorldSkipInTutorial02(XPacket& xPacket);

    // === 内部支撑 ===
    // IDA: ?CheckAdmissionMember@CWorldProcess@@QEAA_NH_NAEAH@Z @ 0x140632770
    //      (nType: 1=solo 检查, 2=需在队伍, 3=入队检查, 4=拒绝, 5=Force 检查)
    bool CheckAdmissionMember(int nType, bool bBreakInto, int& nErrorCode);
    // IDA: ?IsEnterTheThingEvent@CWorldProcess@@AEAA_NXZ @ 0x140632B10
    bool IsEnterTheThingEvent();
    // IDA: ?GetEventJumpID@CWorldProcess@@AEAAHH@Z @ 0x140632D70 (单参)
    int GetEventJumpID(int nMapID);
};
