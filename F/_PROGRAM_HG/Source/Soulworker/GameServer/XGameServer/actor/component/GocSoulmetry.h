#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <map>
#include <list>
#include <vector>

// Forward declarations
class CMover;
class CUser;
struct TB_SOUL_METRY;

// PS_SOULMETRY_INFO and PS_SOULMETRY_LIST are defined in PSServerDB.h

/**
 * @brief ST_SOULMETRY - SoulMetry内部状态结构
 * IDA size: 2 bytes
 */
struct ST_SOULMETRY {
    std::int16_t shCompleteBit = 0;

    ST_SOULMETRY() : shCompleteBit(0) {}
};

/**
 * @brief CGocSoulMetry - SoulMetry系统组件
 *
 * 处理SoulMetry(灵魂熔炉)机制，包括任务完成追踪、奖励发放等功能。
 * IDA struct size: 352 bytes
 */
class CGocSoulMetry : public GOComponent {
public:
    // IDA: ??0CGocSoulMetry@@QEAA@XZ (0x140195C40)
    CGocSoulMetry();

    // IDA: ??1CGocSoulMetry@@UEAA@XZ (0x140195CE0)
    virtual ~CGocSoulMetry();

    // GOComponent interface
    // IDA: ?Init@CGocSoulMetry@@QEAA_NXZ (0x140195D40)
    bool Init();

    // IDA: ?Clear@CGocSoulMetry@@QEAAXXZ (0x140195D60)
    void Clear();

    // IDA: ?ResetAll@CGocSoulMetry@@QEAAXXZ (0x140195DF0)
    void ResetAll();

    // IDA: ?SetSoulMetryList@CGocSoulMetry@@QEAAXAEAUPS_SOULMETRY_LIST@@0@Z (0x140196000)
    void SetSoulMetryList(PS_SOULMETRY_LIST& stSoulmetryList, PS_SOULMETRY_LIST& stCompleteSoulmetryList);

    // IDA: ?SendSoulMetryList@CGocSoulMetry@@QEAAXXZ (0x1401961F0)
    void SendSoulMetryList();

    // IDA: ?SendSoulMetryAdd@CGocSoulMetry@@QEAAXXZ (0x1401963A0)
    void SendSoulMetryAdd();

    // IDA: ?UpdateSoulMetry@CGocSoulMetry@@QEAA_NHH@Z (0x1401969B0)
    bool UpdateSoulMetry(int nSoulMetryID, int nObjectID);

    // IDA: ?AddSoulMetry@CGocSoulMetry@@QEAA_NH@Z (0x140196E90)
    bool AddSoulMetry(int nSoulMetryID);

    // IDA: ?CompleteSoulMetry@CGocSoulMetry@@QEAAXH@Z (0x140197280)
    void CompleteSoulMetry(int nSoulMetryID);

    // IDA: ?DeleteSoulMetry@CGocSoulMetry@@QEAAXH@Z (0x140197C50)
    void DeleteSoulMetry(int nSoulMetryID);

    // IDA: ?GetFamilyID@CGocSoulMetry@@SAHXZ (0x140144B40)
    static int GetFamilyID();

    // GOComponent virtual interface stubs
    bool Initialize() override { return Init(); }
    void Shutdown() override { Clear(); }
    void Update(float fDeltaTime) override { (void)fDeltaTime; }

protected:
    // IDA: offset 16, size 32
    std::map<int, ST_SOULMETRY> m_mapSoulMetry;

    // IDA: offset 48, size 257 (char[257])
    char m_szCompleteSoulMetry[257] = {};

    // IDA: offset 305, size 1
    bool m_bLoad = false;

    // IDA: offset 306, size 1
    bool m_bAddSoulMetry = false;

    // IDA: offset 308, size 8 (PS_SOULMETRY_INFO is 8 bytes)
    PS_SOULMETRY_INFO m_stAddSoulMetryInfo = {};

    // IDA: offset 320, size 24 on MSVC (std::list<unsigned long>)
    // Note: std::list is 24 bytes on MSVC but 16 bytes on clang-cl/GCC
    std::list<unsigned long> m_listCompleteSoulmetry;

    // IDA: offset 344, size 1
    bool m_bOpenAlwaysSoulMetry = false;

    // Padding to match IDA size (352 bytes on MSVC)
    // Note: Actual size differs between MSVC (352) and clang-cl (344) due to std::list size
    char _pad0[7] = {};
};

// Size check: MSVC=352, clang-cl may differ due to std::list implementation
static_assert(sizeof(CGocSoulMetry) >= 344, "CGocSoulMetry size check");
