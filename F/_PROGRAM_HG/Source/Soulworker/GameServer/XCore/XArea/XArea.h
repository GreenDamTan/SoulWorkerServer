#pragma once

#include "Soulworker/GameServer/XCore/XArea/IXArea.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3
#include <cstdint>
#include <list>
#include <map>
#include <vector>

// 前置声明
class XActor;
class CFSRWLock;
struct ST_WORLD_MODE_INFO;
struct ST_WORLD_MODE_INFO_VEC;

// UXMapID 结构
// IDA: 访问 m_uxMapID.nMapID 作为 64 位值，然后 << 16 >> 48 获取高 16 位
union TUXMapID {
    struct {
        std::uint16_t wMapID;
        std::uint16_t wInstanceID;
        std::uint32_t _pad;
    };
    std::uint64_t nMapID;  // IDA 使用此字段进行位移操作

    TUXMapID() : nMapID(0) {}
    TUXMapID(std::uint16_t wMap, std::uint16_t wInstance) : wMapID(wMap), wInstanceID(wInstance), _pad(0) {}

    bool operator<(const TUXMapID& other) const {
        return nMapID < other.nMapID;
    }

    bool operator==(const TUXMapID& other) const {
        return nMapID == other.nMapID;
    }
};

// TXMap 已在 GreenDamTan_ClientBase.h 中定义为 ATL::CAtlMap 包装
// 这里不再重复定义

// TODO: 推测结果 - 来自 IDA struct XArea (192 bytes)
class XArea : public IXArea {
public:
    XArea();
    virtual ~XArea();

    // 实现 IXArea 接口
    void OnUpdate(float fDelta) override;
    // IDA: ?EnterActor@XArea@@UEAAGPEAVXActor@@@Z - returns unsigned short (error code, 0 = success, 50001 = already exists)
    std::uint16_t EnterActor(XActor* pActor) override;
    void ExitActor(XActor* pActor) override;
    XActor* FindActor(std::uint32_t dwActorID) override;

    // Broadcast interface
    // IDA: XArea::SendBroadCast
    void SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) override;
    // IDA: XArea::SendBroadCastAfterLoading
    void SendBroadCastAfterLoading(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) override;
    // IDA: ?ScanGridOrigin@XArea@@UEAAXMMEHKAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
    // Virtual function implementation (base class stub - does nothing)
    void ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) override;

    // IDA 0x1408EF080 - Static helper that dispatches to virtual ScanGridOrigin
    // Mangled: ?ScanGridOrigin@XArea@@SA_NPEAVXActor@@HKAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
    // Returns bool, takes XActor*, int, unsigned int, vector
    static bool ScanGridOrigin(XActor* pActor, int nRange, unsigned int uFlag, std::vector<CMover*>& vecOut);

    // IDA 0x1408F0C70 - Clear
    virtual void Clear();

    // IDA 0x1408EF570 - GetActorCount
    int GetActorCount(int eType);

    // IDA 0x1408EF100 - IsPvPZone
    bool IsPvPZone();

    // IDA: XArea::IsMaze - virtual function (base returns false)
    virtual bool IsMaze() { return false; }
    // IDA: XArea::IsRevive - virtual function (base returns false, XMaze overrides)
    virtual bool IsRevive() { return false; }

    // IDA 0x1406E04B0 - SetMaxUserCount
    void SetMaxUserCount(int nMaxUserCount);

    // IDA 0x1408EF9A0 - SendInInfo
    virtual void SendInInfo(XSendPacket& packet, XActor* pActor);

    // IDA 0x1408F0D50 - SendOutInfo
    virtual void SendOutInfo(XSendPacket& packet, XActor* pActor);

    // IDA 0x1408F06F0 - SendOtherInfos
    virtual void SendOtherInfos(XSendPacket& xSendPacket, XActor* pActor, int eActorType);

    // IDA: ?SendWorldEventBooster@XArea@@QEAAXK_J@Z (0x1408F0250)
    void SendWorldEventBooster(unsigned long dwBuffID, std::int64_t biEndDate);

    // 访问器
    TUXMapID GetMapID() const { return m_uxMapID; }
    void SetMapID(TUXMapID uxMapID) { m_uxMapID = uxMapID; }
    int GetMaxUserCount() const { return m_nMaxUserCount; }

    // IDA: ?GetInstanceID@XArea@@QEAA?ATUXMapID@@XZ (0x140068120)
    // Returns the map instance ID (same as GetMapID)
    TUXMapID GetInstanceID() const { return m_uxMapID; }

    // IDA: ?SetInstanceID@XArea@@QEAAXTUXMapID@@@Z (0x1406c5bc0)
    // Sets the map instance ID
    void SetInstanceID(TUXMapID nIns) { m_uxMapID = nIns; }

    // IDA: ?IsValidPosition@XArea@@UEAA_NAEAUXVec3@@@Z (0x1405052e0)
    // Check if position is valid (default returns true)
    virtual bool IsValidPosition(XVec3& vPos) {
        (void)vPos;
        return true;
    }

    // IDA: ?GetNavMeshInstance@XArea@@UEAAPEAVDohHavokNavMeshInstance@@XZ
    // Virtual - returns nullptr in base class, overridden by XDistrict
    virtual class DohHavokNavMeshInstance* GetNavMeshInstance() { return nullptr; }

    // IDA: ?GetTBMapID@XArea@@QEAAGXZ (0x1400492D0)
    // 返回表格地图ID (从 64 位 nMapID 中提取高 16 位)
    std::uint16_t GetTBMapID() const {
        // IDA: return this->m_uxMapID.nMapID << 16 >> 48;
        // 即取高 16 位 (bits 48-63)
        return static_cast<std::uint16_t>((m_uxMapID.nMapID << 16) >> 48);
    }

    // IDA: ?GetChannel@XArea@@QEAAFXZ (0x140085580)
    // 返回频道ID (从 64 位 nMapID 中提取 bits 56-63)
    std::int16_t GetChannel() const {
        // IDA: return this->m_uxMapID.nMapID << 32 >> 56;
        return static_cast<std::int16_t>((m_uxMapID.nMapID << 32) >> 56);
    }

    // 获取世界类型 (0=未知, 1=迷宫, 2=战场等)
    virtual int GetWorldType() { return 0; }

    // IDA: ?GetExitDistrictID@XArea@@UEAA_NKAEAGAEAHAEAUXVec3@@@Z (0x1401ACF20)
    // Returns exit district ID (stub - returns false)
    virtual bool GetExitDistrictID(std::uint32_t dwActorID, std::uint16_t& wMapID, int& nJumpID, XVec3& vPos) {
        (void)dwActorID; (void)wMapID; (void)nJumpID; (void)vPos;
        return false;
    }

    // IDA: ?ProcessExp@XArea@@UEAAXPEAVXActor@@MH@Z (0x1401ACF80)
    // Process experience (stub - empty function)
    virtual void ProcessExp(XActor* pAtk, float fExp, int nMonsterLv) {
        (void)pAtk; (void)fExp; (void)nMonsterLv;
    }

    // IDA: ?MoveActor@XArea@@UEAAGTUXActorID@@AEAUXVec3@@M@Z (0x1401ACFB0)
    // Move actor in area (stub - returns 0)
    virtual std::uint16_t MoveActor(UXActorID uxActorID, XVec3& vPos, float fRot) {
        (void)uxActorID; (void)vPos; (void)fRot;
        return 0;
    }

    // IDA: ?MoveActor@XMaze@@UEAAGPEAVXActor@@AEAUXVec3@@M_N@Z (0x140315750)
    // Move actor with XActor pointer - virtual, overridden by XMaze/XDistrict
    virtual std::uint16_t MoveActor(XActor* pActor, hkvVec3* vNextPos, float fRot = 0.0f) {
        (void)pActor; (void)vNextPos; (void)fRot;
        return 0;
    }

protected:
    // === IDA 确认的成员变量 (offset from struct start) ===
    // offset 0: IXArea vtable (继承)

    // offset 8: m_mapActor (TXMap<unsigned long, XActor*>, 72 bytes)
    // TODO: 推测目录归属 - TXMap 内部布局需要验证
    std::map<std::uint32_t, XActor*> m_mapActor;

    // offset 80: m_rwLock (CFSRWLock, 8 bytes)
    CFSRWLock m_rwLock;

    // offset 88: m_uxMapID (UXMapID, 8 bytes)
    TUXMapID m_uxMapID;

    // offset 96: m_nMaxUserCount (int)
    int m_nMaxUserCount;

    // offset 104: m_listSpawnNPC (std::list<unsigned long>, 24 bytes)
    std::list<std::uint32_t> m_listSpawnNPC;

    // offset 128: m_mapWorldMode (std::map<int, ST_WORLD_MODE_INFO>, 32 bytes)
    std::map<int, void*> m_mapWorldMode;  // TODO: 需人工审查 - 类型待确认

    // offset 160: m_vecWorldModeList (ST_WORLD_MODE_INFO_VEC, 32 bytes)
    // TODO: 需人工审查 - 需要 ST_WORLD_MODE_INFO_VEC 定义
    std::vector<std::uint8_t> m_vecWorldModeList_dummy;  // 临时占位

    // Total size: 192 bytes (verified from IDA)
};
