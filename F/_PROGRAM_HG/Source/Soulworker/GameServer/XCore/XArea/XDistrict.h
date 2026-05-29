#pragma once

#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include <cstdint>
#include <map>
#include <set>
#include <list>
#include <vector>

// 前置声明
class CUser;
class VEventObjectResource;
struct AREA_OBJECT;
class DohHavokNavMeshInstance;

// 前置声明 - 世界模式相关结构
struct ST_WORLD_MODE_INFO;
struct PS_WORLD_MODE_FINISH;

// TODO: 推测结果 - 来自 IDA struct XDistrict (416+ bytes)
class XDistrict : public XArea {
public:
    XDistrict();
    virtual ~XDistrict();

    // 访问器
    DohHavokNavMeshInstance* GetNavMeshInstance() const { return m_pNavMeshInstance; }
    int GetNavMeshIndex() const { return m_nNavMeshIndex; }
    bool CanUseActiveAkashic() const { return m_bCanUseActiveAkashic; }

    void AddWaitForRecvInfo(CUser* pUser);
    void RemoveWaitForRecvInfo(CUser* pUser);

    // IDA 0x1402D1330 - 获取区域类型
    virtual std::uint8_t GetDistrictType();

    // IDA 0x1402D1010 - 加载完成处理
    virtual void LoadComplete(XActor* pActor);

    // IDA 0x1402D11E0 - 发送世界模式信息
    void SendWorldModeInfo(XActor* pActor);

    // IDA 0x1402D0930 - 发送玩家进入信息
    void SendEnterPlayerInfo(CUser* pUser);

    // IDA 0x1402D0A50 - 发送玩家退出信息
    void SendExitPlayerInfo(CUser* pUser);

    // IDA 0x1402D0B60 - 发送所有玩家信息
    void SendPlayerInfoAll(CUser* pUser);

    // IDA 0x1402D0190 - 完成世界模式
    virtual void FinishWorldMode(PS_WORLD_MODE_FINISH* stInfo);

    // IDA 0x1402D0590 - 同步世界模式
    virtual void SyncWorldMode(std::vector<ST_WORLD_MODE_INFO>* stInfoVec);

    // IDA 0x1402D0FE0 - 设置对象信息请求
    void SetObjectInfoReq(CUser* pUser);

protected:
    // === IDA 确认的成员变量 (offset from XArea end, 192+) ===

    // offset 192: m_pObjectResource (VEventObjectResource*, 8 bytes)
    VEventObjectResource* m_pObjectResource;

    // offset 200: m_objectScanner (AREA_OBJECT, 24 bytes)
    // TODO: 需人工审查 - AREA_OBJECT 定义待确认
    std::uint8_t m_objectScanner_dummy[24];  // 临时占位

    // offset 224: m_pcCount (int)
    int m_nPcCount;

    // offset 232: m_pNavMeshInstance (DohHavokNavMeshInstance*, 8 bytes)
    DohHavokNavMeshInstance* m_pNavMeshInstance;

    // offset 240: m_nNavMeshIndex (int)
    int m_nNavMeshIndex;

    // offset 248: m_setSocialObjectKeys (std::set<unsigned long>, 32 bytes)
    std::set<std::uint32_t> m_setSocialObjectKeys;

    // offset 280: m_mapSocialOwnerKeys (std::map<unsigned long, unsigned long>, 32 bytes)
    std::map<std::uint32_t, std::uint32_t> m_mapSocialOwnerKeys;

    // offset 312: m_setSocialObjectFuniture (std::set<unsigned long>, 32 bytes)
    std::set<std::uint32_t> m_setSocialObjectFuniture;

    // offset 344: m_setSocialObjects (std::set<unsigned long>, 32 bytes)
    std::set<std::uint32_t> m_setSocialObjects;

    // offset 376: m_bCanUseActiveAkashic (bool)
    bool m_bCanUseActiveAkashic;

    // offset 384: m_nCheckUserLog (__int64, 8 bytes)
    std::int64_t m_nCheckUserLog;

    // offset 392: m_listWaitForRecvInfo (std::list<CUser*>, 24 bytes)
    std::list<CUser*> m_listWaitForRecvInfo;

    // 世界模式相关成员 (IDA 反编译发现)
    std::map<int, ST_WORLD_MODE_INFO> m_mapWorldMode;
    std::vector<ST_WORLD_MODE_INFO> m_vecWorldModeList;

    // Total size: 416+ bytes (verified from IDA)
};
