#pragma once

#include <cstdint>
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// 前置声明
struct VInterActionBoxInfo;
struct TB_INTERACTION_ITEM;
struct PS_VACCUM_CUBE_IN;
class XActor;
struct XVec3;

// Per IDA: CVaccumCube - 继承自 CMoverEx
// 真空立方体对象，用于交互物品拾取
class CVaccumCube : public CMoverEx {
public:
    CVaccumCube();
    virtual ~CVaccumCube();

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
