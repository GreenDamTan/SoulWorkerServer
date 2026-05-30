#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <memory>

// 前置声明
class CBattleZone;
class CVaccumGroup;
class XActor;

// UXActorID 定义在 PSCommon.h 中，这里使用前置声明
union UXActorID;
struct VInterActionBoxInfo;

// 使用 std::tr1 命名空间 (VS2010 兼容)
namespace std { namespace tr1 = std; }

// Per IDA: CVaccumManager - 176 bytes
class CVaccumManager {
public:
    CVaccumManager();
    ~CVaccumManager();

    // Per IDA 0x140192320: 初始化
    void Init(CBattleZone* pArea, bool bAutoSpawn);

    // Per IDA 0x140192350: 添加真空组
    void AddVaccumGroup(UXActorID uxActor, VInterActionBoxInfo* pInfo);

    // Per IDA 0x140192730: 更新
    void Update();

    // Per IDA 0x1401928b0: 点击真空立方体
    unsigned int ClickVaccumCube(int nID, XActor* pActor);

    // Per IDA 0x140192b50: 取消点击真空立方体
    unsigned int CancelClickVaccumCube(int nID, XActor* pActor);

    // Per IDA 0x140192cd0: 清除真空锁定
    void ClearVaccumLock(XActor* pActor);

    // Per IDA 0x1403545a0: 获取区域
    CBattleZone* GetArea() const { return m_pArea; }

    void Clear();

    // === 辅助函数 ===
    bool Add(int nID);
    bool Remove(int nID);
    void Process();
    int GetCount() const;
    bool IsActive() const;
    bool Start(int nID);
    bool Stop(int nID);
    void GetPosition(int nID, float* pX, float* pY, float* pZ);
    void SetPosition(int nID, float fX, float fY, float fZ);

private:
    // === IDA 确认的成员变量 (offset from struct start) ===
    // offset 0: m_mapVaccumGroup (std::map<int, shared_ptr<CVaccumGroup>>, 32 bytes)
    std::map<int, std::tr1::shared_ptr<CVaccumGroup>> m_mapVaccumGroup;

    // offset 32: m_mapVaccumTableID (std::map<int, int>, 32 bytes)
    std::map<int, int> m_mapVaccumTableID;

    // offset 64: m_pArea (CBattleZone*, 8 bytes)
    CBattleZone* m_pArea;

    // offset 72: m_vecBoxID (std::vector<int>, 32 bytes)
    std::vector<int> m_vecBoxID;

    // offset 104: m_mapVaccumCheat (std::map<int, std::vector<int>>, 32 bytes)
    std::map<int, std::vector<int>> m_mapVaccumCheat;

    // offset 136: m_bAutoSpawn (bool)
    bool m_bAutoSpawn;

    // offset 144: m_mapVaccumNoneAuto (std::map<int, shared_ptr<CVaccumGroup>>, 32 bytes)
    std::map<int, std::tr1::shared_ptr<CVaccumGroup>> m_mapVaccumNoneAuto;

    // Total size: 176 bytes (verified from IDA)
};
