#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/InteractionObject.h"  // For VInterActionBoxInfo

// 前置声明
struct TB_INTERACTION_ITEM;
class XActor;
struct XVec3;

// IDA: Price types for currency operations
enum E_PRICE_TYPE {
    E_PRICE_TYPE_GOLD = 0,
    E_PRICE_TYPE_BP = 1,
    E_PRICE_TYPE_ETHER = 2,
};

// IDA: Item create type enum
enum E_ITEM_CREATE_TYPE {
    E_ITEM_CREATE_TYPE_VACCUM_CUBE = 32,
};

// IDA 反编译还原: PS_VACCUM_PICK_UP - 真空立方体拾取结果
struct PS_VACCUM_PICK_UP {
    std::uint32_t dwActorID = 0;      // IDA: actor ID
    std::int32_t nID = 0;             // IDA: interaction box ID
    std::uint16_t wRemainCount = 0;   // IDA: remaining count
    std::int32_t nErrorCode = 0;      // IDA: error code
};

// IDA 反编译还原: PS_RES_VACCUM_CLICK_START - 真空立方体点击开始响应
struct PS_RES_VACCUM_CLICK_START {
    std::int32_t nID = 0;             // IDA: vaccum cube ID
    std::int32_t nErrorCode = 0;      // IDA: error code (55800 = default)
};

// IDA 反编译还原: PS_VACCUM_CUBE_IN - 真空立方体进入信息
struct PS_VACCUM_CUBE_IN {
    std::int32_t nID = 0;          // IDA offset 0: pInterActionBoxInfo->iID
    std::int32_t nTableID = 0;     // IDA offset 4: pInterActionBoxInfo->m_iInteractionID
    std::uint8_t byInType = 0;     // IDA offset 8: stInfo->byInType = 1
    std::uint8_t _pad0[3] = {};    // 对齐填充
};

// IDA 反编译还原: PS_VACCUM_CUBE_OUT - 真空立方体离开信息
struct PS_VACCUM_CUBE_OUT {
    std::int32_t nID = 0;          // IDA offset 0: pInterActionBoxInfo->iID
    std::uint8_t byOutType = 0;    // IDA offset 4: stInfo->byOutType = !bDestroy
    std::uint8_t _pad0[3] = {};    // 对齐填充
};

// Per IDA: CVaccumCube - 继承自 CMoverEx
// 真空立方体对象，用于交互物品拾取
class CVaccumCube : public CMoverEx {
public:
    // Static factory - Create a new CVaccumCube object
    // IDA: ?CreateObject@CVaccumCube@@SAPEAVVTypedObject@@XZ @ 0x140190840
    static CVaccumCube* CreateObject();

    CVaccumCube();
    virtual ~CVaccumCube();

    // Virtual method - Get type ID for RTTI
    // IDA: ?GetTypeId@CVaccumCube@@UEBAPEAUVType@@XZ @ 0x1401908A0
    virtual VType* GetTypeId() const;

    // Per IDA 0x140190b10: 重置
    virtual void Reset();

    // Per IDA 0x140190a10: 初始化
    void Init(UXActorID uxActorID, VInterActionBoxInfo* pInfo, XVec3& vecPos, int nRandom, int nItemID, std::uint64_t dwTablePickupTime);

    // Per IDA 0x140190b40: 生成
    void Spawn(int nCount);

    // Per IDA 0x140190b60: 占用真空立方体
    void TakeVaccum(XActor* pActor);

    // Per IDA 0x140190bb0: 是否可以拾取
    bool IsPickup();

    // Per IDA 0x190190bf0: 拾取
    void Pickup();

    // Per IDA 0x140191450: 清除占用
    void ClearTakeVaccum();

    // Per IDA 0x1401915c0: 是否是指定用户占用
    bool IsTakeUser(XActor* pActor);

    // Per IDA 0x140191660: 构建信息包
    void BuildInfoPacket(PS_VACCUM_CUBE_IN& stInfo);

    // Per IDA 0x1401916F0: 设置信息包
    void SetInfoPacket(XSendPacket& xSendPacket);

    // Per IDA 0x140191750: 设置离开信息包
    void SetInfoLeavePacket(XSendPacket& xSendPacket, bool bDestroy);

    // Per IDA 0x1401945d0: 是否锁定
    bool IsLock() const { return m_bLock; }

    // Per IDA 0x140194610: 获取数量
    int GetCount() const { return m_nCount; }

    // Per IDA 0x1401947d0: 获取ID
    virtual std::uint64_t GetID();

    // Per IDA 0x1401947f0: 获取ActorID
    virtual UXActorID GetActorID() const;

    // Per IDA 0x140194950: 获取交互ID
    virtual std::uint64_t GetInteractionID();

    // 设置随机key
    void SetRandomKey(int nKey) { m_nRandomKey = nKey; }

    // 获取随机key
    int GetRandomKey() const { return m_nRandomKey; }

private:
    // === IDA 确认的成员变量 ===
    // 继承自 CMoverEx 的基础成员

    // 交互盒子信息
    VInterActionBoxInfo* m_pInterActionBoxInfo;

    // Actor ID (UXActorID, 8 bytes)
    UXActorID m_uxActorID;

    // 锁定状态
    bool m_bLock;

    // 占用用户
    XActor* m_pTakeUser;

    // 完成拾取时间
    std::uint64_t m_dwCompletePickupTime;

    // 表格拾取时间
    std::uint64_t m_dwTablePickupTime;

    // 数量
    int m_nCount;

    // 随机key
    int m_nRandomKey;

    // 随机物品ID
    int m_nRandomItemID;

    // 位置信息
    STPosInfo m_posInfo;
};

// ============================================================================
// VaccumCubeObjectMgr - VaccumCube object manager
// ============================================================================
// TODO: Should inherit from TXObjectMgr<CVaccumCube> but stub CMover doesn't
// inherit from XActor/IXObject. Using simple map-based implementation for now.
class VaccumCubeObjectMgr {
public:
    // Constructor
    // IDA: ??0VaccumCubeObjectMgr@@QEAA@XZ @ 0x1401903C0
    VaccumCubeObjectMgr();

    // Destructor
    // IDA: ??1VaccumCubeObjectMgr@@UEAA@XZ @ 0x140190430
    virtual ~VaccumCubeObjectMgr();

    // Create - Create a new vaccum cube at position
    // IDA: ?Create@VaccumCubeObjectMgr@@QEAAPEAVCVaccumCube@@UXVec3@@@Z @ 0x14018FF50
    CVaccumCube* Create(XVec3* vPos);

    // Init - Initialize the object manager with max size
    // IDA: ?Init@VaccumCubeObjectMgr@@UEAA_NH@Z @ 0x140190460
    bool Init(int nMaxSize);

    // ClearAll - Clear all vaccum cubes
    // IDA: ?ClearAll@VaccumCubeObjectMgr@@QEAAXXZ @ 0x1401904F0
    void ClearAll();

private:
    int m_nMaxSize = 0;
    std::map<int, CVaccumCube*> m_xObjectMap;
    int m_nNextSessionID = 1000;
};
