// CWorldModeMgr - 世界模式管理器
// 对齐 ControlServer.exe IDA
// 反编译来源: CWorldModeMgr::Update @ 0x14004A050, SendModeSync @ 0x14004A630, LoadMode @ 0x1400498A0

#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#endif

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"

// 前向声明
class CWorldMode;
struct ST_WORLD_MODE_INFO_VEC;

// 使用 std::tr1 命名空间 (VS2010 兼容)
namespace std { namespace tr1 = std; }

// 对齐 IDA CWorldModeMgr
// 成员变量布局从 IDA 反编译推断
class CWorldModeMgr {
public:
    CWorldModeMgr();
    ~CWorldModeMgr();

    void Update();
    void SendModeSync();
    void LoadMode(ST_WORLD_MODE_INFO_VEC& stModeInfoVec);
    void GetModeList(ST_WORLD_MODE_INFO_VEC& stModeInfoVec);
    void SetModeStartTime(int nModeID, std::vector<int>& vecParam, __int64 biTime);
    void SetModeFinishTime(int nModeID, std::vector<int>& vecParam, __int64 biTime);
    bool ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete, bool bForce);
    bool IsActiveMode(int nModeID, std::vector<int>& vecParam);
    bool IsTodayModeList(int nModeID, std::vector<int>& vecParam);
    void SendModeList();
    void UpdateMonsterCount(PS_WORLD_MODE_UPDATE stUpdate);
    int IsEnterMode(ST_ENTER_WORLD_MODE_INFO* stEnterInfo, int* nErrorCode);
    void GetActiveMode(int& nModeID, int& nState);
    void InitMode(/*ATL::CTime*/);

private:
    // 对齐 IDA 成员变量 (从 Update/SendModeSync/LoadMode 反编译推断)
    bool m_bLoadReq = false;           // 是否已请求加载
    bool m_bModeOn = true;             // 模式是否开启
    bool m_bLoadDB = false;            // 是否从DB加载完成
    bool m_bSendMode = false;          // 是否已发送模式
    unsigned long long m_dwUpdateTime = 0;  // 更新时间戳
    // ATL::CTime m_tInit;            // 初始化时间 (简化处理)
    CFSRWLock m_rwLock;                // 读写锁

    // m_mapWorldMode: std::map<std::pair<int,int>, std::tr1::shared_ptr<CWorldMode>>
    // pair.first = nModeDateID, pair.second = nModeID
    std::map<std::pair<int, int>, std::tr1::shared_ptr<CWorldMode>> m_mapWorldMode;
};
