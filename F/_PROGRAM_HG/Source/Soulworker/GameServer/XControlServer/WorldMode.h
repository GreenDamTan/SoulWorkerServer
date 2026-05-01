// WorldMode.h
// CWorldMode 世界模式类定义
// 对齐 IDA ControlServer.exe

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>

// 对齐 IDA: CWorldMode 世界模式类
class CWorldMode {
public:
    CWorldMode();
    virtual ~CWorldMode();

    // 对齐 IDA 0x140046CD0: Init 初始化
    void Init(ST_WORLD_MODE_INFO& stInfo);

    // 对齐 IDA 0x140047920: Clear 清理
    void Clear();

    // 对齐 IDA 0x140047990: Update 更新
    bool Update(bool bModeOn);

    // 对齐 IDA 0x140047AC0: IsFinish 是否完成
    bool IsFinish() const;

    // 对齐 IDA 0x140047AF0: StartMode 启动模式
    void StartMode();

    // 对齐 IDA 0x140047E10: FinishMode 完成模式
    void FinishMode();

    // 对齐 IDA 0x140048350: ClearMode 清除模式
    void ClearMode();

    // 对齐 IDA 0x140048590: GetNextModeID 获取下一个模式ID
    int GetNextModeID() const;

    // 对齐 IDA 0x1400485A0: GetModeInfo 获取模式信息
    void GetModeInfo(ST_WORLD_MODE_INFO& stInfo) const;

    // 对齐 IDA 0x1400485F0: ReqModeComplete 请求完成
    bool ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete);

    // 对齐 IDA 0x140048BD0: AddMonsterCount 增加怪物计数
    bool AddMonsterCount();

    // 对齐 IDA 0x140048CE0: GetModeStartTime 获取模式开始时间
    void GetModeStartTime(std::int64_t& nTime) const;

    // 对齐 IDA 0x140048D00: SetStartTime 设置开始时间
    void SetStartTime(std::int64_t nTime);

    // 对齐 IDA 0x140048D60: InitMode 初始化模式（用于重置已存在的模式）
    void InitMode(std::int64_t nInitTime);

    // 对齐 IDA 0x14004D460: GetState 获取状态
    int GetState() const { return m_stInfo.nState; }

    // 对齐 IDA 0x14004D470: SetClearTime 设置清除时间
    void SetClearTime();

    // 对齐 IDA 0x14004D490: SetFinishTime 设置完成时间
    void SetFinishTime(std::int64_t nTime);

    // 对齐 IDA 0x14004D4B0: IsSuccess 是否成功
    bool IsSuccess() const { return m_bSuccess; }

    // 辅助方法
    // 对齐 IDA 0x1400486E0: SetNextEventTime 设置下一次事件时间
    void SetNextEventTime();
    // 对齐 IDA 0x140048B50: GetEventTime 获取事件时间
    void GetEventTime(int nTime, int* pHour, int* pMin);
    // 对齐 IDA 0x1400489D0: SendLogNextModeTime 发送下一次模式时间日志
    void SendLogNextModeTime(void* pTime);

private:
    // 成员变量 (对齐 IDA CWorldMode - 152 bytes / 0x98)
    // IDA layout: vftable(8) + m_stInfo(56) + m_nTableID(4) + m_nNextModeID(4) + m_nLimitTime(4) + m_nStartType(4) + m_bReserveFinish(1) + padding(7) + m_uxCompleteMapID(8) + m_strKiller(42) + m_nClearWaitTime(4) + m_bSuccess(1) + padding(3) + m_nModeTime(4)
    ST_WORLD_MODE_INFO m_stInfo{};       // offset 8, size 56 - 模式信息结构
    int m_nTableID = 0;                  // offset 64, size 4 - 表ID (IDA constructor: m_nTableID = 0)
    int m_nNextModeID = 0;               // offset 68, size 4 - 下一个模式ID
    int m_nLimitTime = 0;                // offset 72, size 4 - 限制时间 (分钟)
    int m_nStartType = 0;                // offset 76, size 4 - 启动类型 (0: 时间触发, 1: 手动)
    bool m_bReserveFinish = false;       // offset 80, size 1 - 是否预约完成
    char _padding1[7] = {};              // offset 81-87, padding for alignment
    UXMapID m_uxCompleteMapID{};         // offset 88, size 8 - 完成地图ID
    wchar_t m_strKiller[21] = {};        // offset 96, size 42 - 击杀者名称
    int m_nClearWaitTime = 0;            // offset 140, size 4 - 清除等待时间 (秒)
    bool m_bSuccess = false;             // offset 144, size 1 - 是否成功完成
    char _padding2[3] = {};              // offset 145-147, padding for alignment
    int m_nModeTime = 0;                 // offset 148, size 4 - 模式时间 (分钟)
};