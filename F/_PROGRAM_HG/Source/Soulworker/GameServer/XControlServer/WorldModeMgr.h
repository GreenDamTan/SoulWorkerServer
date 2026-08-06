// WorldModeMgr.h
// CWorldModeMgr 世界模式管理类定义
// 对齐 IDA ControlServer.exe

#pragma once

// 减少 Windows 头文件冲突
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "WorldMode.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include <list>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_StdCompat.h"

// 前向声明
class CFAutoSlimWriteLock;

// 对齐 IDA: CWorldModeMgr 世界模式管理类
class CWorldModeMgr {
public:
    CWorldModeMgr();
    ~CWorldModeMgr();

    // 对齐 IDA 0x140049840: Init 初始化
    void Init();

    // 对齐 IDA 0x1400498A0: LoadMode 加载模式
    void LoadMode(ST_WORLD_MODE_INFO_VEC& stModeVec);

    // 对齐 IDA 0x14004A050: Update 更新
    void Update();

    // 对齐 IDA 0x14004A630: SendModeSync 发送模式同步
    void SendModeSync();

    // 对齐 IDA 0x14004A9A0: SetModeStartTime 设置模式开始时间
    void SetModeStartTime(int nModeDateID, std::vector<int>& vecModeID, std::int64_t nTime);

    // 对齐 IDA 0x14004AAB0: SetModeFinishTime 设置模式完成时间
    void SetModeFinishTime(int nModeDateID, std::vector<int>& vecModeID, std::int64_t nTime);

    // 对齐 IDA 0x14004ABC0: ReqModeComplete 请求完成
    bool ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete, bool bClear);

    // 对齐 IDA 0x14004ACC0: IsActiveMode 是否激活模式
    bool IsActiveMode(int nModeDateID, std::vector<int>& vecModeID);

    // 对齐 IDA 0x14004AE00: GetModeList 获取模式列表
    void GetModeList(ST_WORLD_MODE_INFO_VEC& stModeVec);

    // 对齐 IDA 0x14004AFC0: IsTodayModeList 是否今日模式列表
    bool IsTodayModeList(int nModeDateID, std::vector<int>& vecModeID);

    // 对齐 IDA 0x14004B140: InitMode 初始化模式
    void InitMode(std::int64_t nTime);

    // 对齐 IDA 0x14004B490: SendModeList 发送模式列表
    void SendModeList();

    // 对齐 IDA 0x14004B730: UpdateMonsterCount 更新怪物计数
    void UpdateMonsterCount(PS_WORLD_MODE_UPDATE& stUpdate);

    // 对齐 IDA 0x14004B8A0: IsEnterMode 是否进入模式
    // IDA 签名: ?IsEnterMode@CWorldModeMgr@@QEAAHUST_ENTER_WORLD_MODE_INFO@@AEAH@Z
    // 实际参数是 std::vector<ST_WORLD_MODE>
    int IsEnterMode(std::vector<ST_WORLD_MODE>& stList, int& nActiveModeID);

    // 对齐 IDA 0x14004BB40: GetActiveMode 获取激活模式
    void GetActiveMode(int& nActiveModeDateID, int& nModeID);

private:
    // 成员变量 (对齐 IDA CWorldModeMgr - 72 bytes)
    // IDA layout: m_rwLock(8) + m_bLoadReq(1) + m_bLoadDB(1) + padding(6) + m_mapWorldMode(32) + m_tInit(8) + m_dwUpdateTime(8) + m_bModeOn(1) + m_bSendMode(1) + padding(6)
    CFSRWLock m_rwLock;               // offset 0, size 8 - 读写锁
    bool m_bLoadReq = false;          // offset 8, size 1 - 是否已发送DB请求
    bool m_bLoadDB = false;           // offset 9, size 1 - 是否已从DB加载
    char _padding1[6] = {};           // offset 10-15, padding for alignment
    std::map<std::pair<int, int>, std::tr1::shared_ptr<CWorldMode>> m_mapWorldMode;  // offset 16, size 32 - 世界模式映射
    std::int64_t m_tInit = 0;         // offset 48, size 8 - 初始化时间 (ATL::CTime 内部表示)
    unsigned long m_dwUpdateTime = 0; // offset 56, size 8 - 更新时间 (毫秒)
    bool m_bModeOn = false;           // offset 64, size 1 - 是否开启模式
    bool m_bSendMode = false;         // offset 65, size 1 - 是否已发送模式
    int m_nActiveModeDateID = 0;      // offset 66 (未对齐 IDA，临时成员)
    int m_nActiveModeID = 0;          // offset 70 (未对齐 IDA，临时成员)
};