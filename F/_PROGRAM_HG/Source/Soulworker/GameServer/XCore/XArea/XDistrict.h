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
class CMover;
class CMonster;
class CNpc;
class CInteractionObject;
class CVaccumCube;
class CSocialItemObject;

// 前置声明 - 模板类
template<typename T> class Range2DScanner;

// 前置声明 - 表结构
struct TB_DISTRICT;

// 前置声明 - 世界模式相关结构
struct ST_WORLD_MODE_INFO;
struct PS_WORLD_MODE_FINISH;

// 前置声明 - 生成相关结构
struct VMonsterSpawnInfo;
enum E_SEND_INFO_TYPE;

// TODO: 推测结果 - 来自 IDA struct XDistrict (416+ bytes)
class XDistrict : public XArea {
public:
    XDistrict();
    virtual ~XDistrict();

    // 访问器
    DohHavokNavMeshInstance* GetNavMeshInstance() const { return m_pNavMeshInstance; }
    int GetNavMeshIndex() const { return m_nNavMeshIndex; }
    bool CanUseActiveAkashic() const { return m_bCanUseActiveAkashic; }

    // IDA 0x1401ACF60 - 获取对象资源
    virtual VEventObjectResource* GetObjectResource();

    // IDA 0x1402C8630 - 创建区域
    bool Create(TB_DISTRICT* pDistrict);

    // IDA 0x1402C8850 - 清理区域
    virtual void Clear();

    // IDA 0x1402C8810 - 初始化区域
    virtual bool Init();

    // IDA 0x1402C88F0 - 进入Actor
    // IDA: ?EnterActor@XDistrict@@UEAAGPEAVXActor@@@Z - returns unsigned short
    virtual std::uint16_t EnterActor(XActor* pActor) override;

    // IDA 0x1402C8890 - 获取扫描器
    Range2DScanner<CMover*>* GetScanner(XActor* pActor);

    // IDA 0x1402CC7D0 - 更新
    void OnUpdate(float fElapsed) override;

    // IDA 0x1402CD500 - 移动Actor
    std::uint16_t MoveActor(XActor* pActor, XVec3& vPos, float fRot, bool bByForce);

    // IDA 0x1402CDB30 - 检查是否在周围区域
    bool IsAroundSector(float fx, float fy, float fx2, float fy2);

    // IDA 0x1402CDBD0 - 检查是否在同一区域
    bool IsSameSector(float fx, float fy, float fx2, float fy2);

    // IDA 0x1402CB960 - 发送对象信息
    virtual bool SendObjectInfo(XActor* pActor);

    // IDA 0x1402CD4C0 - 检查位置是否有效
    virtual bool IsValidPosition(XVec3& vPos);

    // IDA 0x1402CBD60 - 逃离Actor
    virtual bool EscapeActor(XActor* pActor);

    // IDA 0x1402CC270 - 生成对象
    void SpawnGenerate();

    // IDA 0x1402CC390 - 执行生成盒
    void ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType);

    // IDA 0x1402CB0B0 - 退出社交物品对象
    bool ExitSocialItemObject(CUser* pUser, int bLeave);

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

    // === IDA 反编译 - 对象处理函数 ===

    // IDA 0x1402C9B10 - 处理进入对象
    void ProcessEnterObject(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList);

    // IDA 0x1402C9B70 - 发送进入对象给其他人
    void ProcessSendEnterObjectToOthers(std::vector<CMover*>& vecPlayerList, XActor* pEnterActor);

    // IDA 0x1402C9E80 - 发送进入对象列表给玩家
    void ProcessSendEnterObjectListToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList);

    // IDA 0x1402C9F30 - 发送PC信息
    void SendPcInfo(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList);

    // IDA 0x1402CA260 - 发送NPC信息
    void SendNpcInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA 0x1402CA590 - 发送怪物信息
    void SendMonsterInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA 0x1402CA9D0 - 发送交互对象信息
    void SendInteractionInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA 0x1402CAC10 - 发送VaccumCube信息
    void SendVaccumInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA 0x1402CAE50 - 发送社交物品信息
    void SendSocialInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA 0x1402925B0 - 发送传送信息给其他人
    void ProcessSendTranslateInfoToOthers(XActor* pActor, std::vector<CMover*>& vecPlayerList);

    // IDA 0x1402CE420 - 发送传送信息给玩家
    void ProcessSendTranslateInfoToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList);

    // IDA 0x1402CB530 - 退出Actor
    void ExitActor(XActor* pActor) override;

    // IDA 0x1402CC060 - 广播消息
    void SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) override;

    // IDA 0x1402CC180 - 广播消息给所有人
    void SendBroadCastAll(XSendPacket& packet);

    // IDA 0x1402CE7F0 - 生成NPC
    virtual bool SpawnNPC(int nNpcID, XVec3& xPos, float fRot);

    // IDA 0x1402CE940 - 删除NPC
    virtual bool DeleteNPC(int nNpcID);

    // IDA 0x1402CEAB0 - 创建社交物品对象
    CSocialItemObject* CreateSocialItemObject(std::uint32_t dwOwnerID, XVec3& vecPos, float fRot, std::uint16_t wItemID);

    // IDA 0x1402CEB50 - 添加社交物品对象
    bool AddSocialItemObject(CSocialItemObject* pSocialItem);

    // IDA 0x1402CED90 - 检查是否可以创建社交物品
    bool CanCreateSocialItem(std::uint8_t bySocialObjType);

    // IDA 0x1402CEDF0 - 删除社交物品对象
    virtual bool DeleteSocialItemObject(std::uint32_t dwObjectID);

    // IDA 0x1402CF360 - 查找社交物品对象
    CSocialItemObject* FindSocialItemObject(std::uint32_t dwObjectID);

    // IDA 0x1402CF4A0 - 根据所有者查找社交物品对象
    CSocialItemObject* FindSocialItemObjectByOwner(std::uint32_t dwOwnerID);

    // IDA 0x1402CF1F0 - 检查位置是否在社交物品对象范围内
    virtual bool IsInSocialItemObjects(XVec3& vCircleCenter, float fRadius);

    // IDA 0x1402CF5B0 - 进入队伍/势力成员
    bool EnterPartyForceMember(CUser* pUser);

    // IDA 0x1402CFE00 - 创建导航网格
    bool CreateNavMesh(const char* pszFileName);

    // IDA 0x1402CFF60 - 开始世界模式
    virtual void StartWorldMode(ST_WORLD_MODE_INFO& stInfo);

    // IDA 0x1402AD220 - 获取生成位置
    virtual void GetSpawnPos(const VMonsterSpawnInfo* pMonsterSpawn, XVec3& vPos);

    // IDA 0x1402D0EE0 - 出现事件怪物
    virtual void AppearEventMonster(int nModeID, std::int64_t biStartTime, std::int64_t biFinishTime,
                                     int nModeDateID, std::int64_t biModeStartTime, std::int64_t biModeEndTime);

    // IDA 0x140339430 - 清除世界模式
    virtual void ClearWorldMode(ST_WORLD_MODE_INFO& stInfo);

    // IDA 0x140340230 - 获取世界模式信息
    virtual void InfoWorldMode(std::vector<ST_WORLD_MODE_INFO>& stInfos);

    // IDA 0x1405FA3B0 - 检查是否可以使用Active Akashic
    bool IsCanUseActiveAkashic();

    // IDA 0x1402DF5F0 - 是否是区域类型
    virtual bool IsDistirct();

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

    // 生成的NPC列表
    std::list<std::uint32_t> m_listSpawnNPC;

    // 世界模式相关成员 (IDA 反编译发现)
    std::map<int, ST_WORLD_MODE_INFO> m_mapWorldMode;
    std::vector<ST_WORLD_MODE_INFO> m_vecWorldModeList;

    // Total size: 416+ bytes (verified from IDA)
};
