// WorldModeProcess.h
// CWorldModeProcess 世界模式处理类定义
// 对齐 IDA ControlServer.exe

#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "CServer.h"
#include <cstdint>

// 前向声明
class CServer;
class XPacket;
class CWorldModeMgr;

// 对齐 IDA: CWorldModeProcess 世界模式处理类
// 继承自 TXProcess<CServer>
class CWorldModeProcess : public TXProcess<CServer> {
public:
    // 对齐 IDA 0x14004D500: 构造函数
    CWorldModeProcess();

    // 对齐 IDA 0x14004D5C0: 析构函数
    virtual ~CWorldModeProcess();

    // 对齐 IDA 0x14004D5F0: Parse 解析包
    virtual bool Parse(XPacket& xPacket) override;

private:
    // 对齐 IDA 0x14004D680: ReqWorldModeCommand 请求世界模式命令
    // SubCmd = 6
    bool ReqWorldModeCommand(XPacket& xPacket);

    // 对齐 IDA 0x14004DAF0: ReqWorldModeComplete 请求世界模式完成
    // SubCmd = 7
    bool ReqWorldModeComplete(XPacket& xPacket);

    // 对齐 IDA 0x14004DC20: ReqWorldModeUpdate 请求世界模式更新
    // SubCmd = 2
    bool ReqWorldModeUpdate(XPacket& xPacket);

    // 对齐 IDA 0x14004DC90: ReqWorldModeEnterList 请求世界模式进入列表
    // SubCmd = 9
    bool ReqWorldModeEnterList(XPacket& xPacket);
};