// ControlServer WorldMode Manager Stub
// 对齐 ControlServer.exe IDA

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

// 前向声明
struct ST_WORLD_MODE;
struct ST_WORLD_MODE_INFO_VEC;

// 对齐 IDA CWorldModeMgr
class CWorldModeMgr {
public:
    CWorldModeMgr();
    ~CWorldModeMgr();

    // 对齐 IDA: CWorldModeMgr::Init 静态方法
    static void Init(CWorldModeMgr* pMgr) {
        // TODO: 对齐 IDA 实现 - 初始化世界模式管理器
    }

    void Update();
    void SendModeSync();

    // 对齐 IDA LoadMode
    void LoadMode(ST_WORLD_MODE_INFO_VEC& stModeInfoVec);

    // 对齐 IDA GetModeList
    void GetModeList(ST_WORLD_MODE_INFO_VEC& stModeInfoVec);

    // 对齐 IDA SetModeStartTime/SetModeFinishTime
    void SetModeStartTime(int nModeID, std::vector<int>& vecParam, __int64 biTime);
    void SetModeFinishTime(int nModeID, std::vector<int>& vecParam, __int64 biTime);

    // 对齐 IDA ReqModeComplete
    bool ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete, bool bForce);

    // 对齐 IDA IsActiveMode
    bool IsActiveMode(int nModeID, std::vector<int>& vecParam);

    // 对齐 IDA IsTodayModeList
    bool IsTodayModeList(int nModeID, std::vector<int>& vecParam);

    // 对齐 IDA SendModeList
    void SendModeList();

    // 对齐 IDA UpdateMonsterCount
    void UpdateMonsterCount(PS_WORLD_MODE_UPDATE stUpdate);

    // 对齐 IDA IsEnterMode
    int IsEnterMode(ST_ENTER_WORLD_MODE_INFO* stEnterInfo, int* nErrorCode) {
        // Stub: 返回 0 表示可以进入
        if (nErrorCode) *nErrorCode = 0;
        return 0;
    }

    // 对齐 IDA GetActiveMode
    void GetActiveMode(int& nModeID, int& nState);

    // 对齐 IDA InitMode
    void InitMode(/*ATL::CTime*/);

private:
    std::map<int, std::shared_ptr<ST_WORLD_MODE>> m_mapMode;
    bool m_bLoad = false;
};
