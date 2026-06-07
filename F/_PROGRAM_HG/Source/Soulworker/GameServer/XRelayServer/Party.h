#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CPartyMember {
public:
    CPartyMember() = default;
    // 对齐 IDA: ??0CPartyMember@@QEAA@UST_PARTY_MEMBER@@@Z = 按值传递
    explicit CPartyMember(const ST_PARTY_MEMBER& partyMember)
        : m_stPartyMember(partyMember), m_uxEnterMap(partyMember.uxMapID) {
        // 对齐 IDA: 如果 bLogin 为 false，需要设置踢出定时器
        if (!m_stPartyMember.bLogin) {
            Logout();
        }
    }

    std::uint32_t GetMemberID() const { return m_stPartyMember.dwMemberID; }
    void SetMemberInfo(ST_PARTY_MEMBER& partyMember) { m_stPartyMember = partyMember; }  // 对齐 IDA 0x140014640: 非const引用

    // 对齐 IDA: 设置/获取进入地图
    void SetEnterMap(UXMapID uxMapID) { m_uxEnterMap = uxMapID; }
    UXMapID GetEnterMap() const { return m_uxEnterMap; }
    bool GetMemberInfo(ST_PARTY_MEMBER& partyMember) const {
        partyMember = m_stPartyMember;
        return true;
    }

    // 对齐 IDA 0x140095460: Login 只重置踢出定时器
    void Login() { m_dwKickOutTime = 0; }
    // 对齐 IDA: Logout 设置踢出定时器（与 CForceMember 共享偏移）
    void Logout();

    // IDA 0x1401C53D0: GetRecode - 复制迷宫记录数组（10个int）
    void GetRecode(int* pMazeRecode) const {
        if (pMazeRecode) {
            std::memcpy(pMazeRecode, m_nMazeRecode, sizeof(m_nMazeRecode));
        }
    }

private:
    ST_PARTY_MEMBER m_stPartyMember{};
    std::uint64_t m_dwKickOutTime = 0;  // 对齐 IDA: 踢出定时器
    UXMapID m_uxEnterMap{};             // 对齐 IDA: 进入地图记录
    int m_nMazeRecode[10] = {};         // IDA: 迷宫记录数组（0x28字节=10个int）
};

class CParty {
public:
    CParty() = default;  // 对齐 IDA: ??0CParty@@QEAA@XZ = 无参数默认构造
    explicit CParty(PS_REQ_PARTY_CREATE& stCreateParty);

    std::uint32_t GetPartyID() { return m_dwPartyID; }  // 对齐 IDA 0x140014540: 非const方法
    std::uint32_t GetMasterID() { return m_dwMasterID; }  // 对齐 IDA 0x14001BFC0: 非const方法
    void SetMasterID(std::uint32_t dwMasterID) { m_dwMasterID = dwMasterID; }
    std::uint8_t GetUserCount() { return static_cast<std::uint8_t>(m_mapPartyMember.size()); }  // 对齐 IDA 0x14001BFA0: 非const方法

    // GetLavelGap - 0x14010D560 (IDA shows typo "Lavel" instead of "Level")
    // Returns the level gap between max and min level in party
    int GetLavelGap() { return static_cast<int>(m_nMaxLevel - m_nMinLevel); }

    void Clear();  // 对齐 IDA 0x140060830

    void AddMember(ST_PARTY_MEMBER& stPartyMember);  // 对齐 IDA 0x140094190: 非const引用 AEAU
    void SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP);  // 对齐 IDA 0x1400136A0
    void SetPartyInfo(PS_PARTY_INFO& partyInfo);  // 对齐 IDA 0x140093F10: 非const引用
    void GetPartyInfo(PS_PARTY_INFO& partyInfo);  // 对齐 IDA 0x1400944A0: 非const方法
    void GetPartyMemberList(ST_PARTY_MEMBER_LIST& stMemberList);  // 对齐 IDA 0x1400946F0: 非const方法

    std::uint32_t FindNewMaster();  // 对齐 IDA 0x1400943E0: 非const方法 QEAAKXZ
    void Kickout(std::uint32_t dwMemberID);  // 对齐 IDA 0x140094360
    bool ChangeMaster(std::uint32_t dwMaster, bool bLeave);  // 对齐 IDA 0x1400942B0: CForce::ChangeMaster

    UXMapID GetMazeID() { return m_uxMazeID; }  // 对齐 IDA 0x14001B8E0: 非const方法
    void SetMazeID(UXMapID uxMazeID) { m_uxMazeID = uxMazeID; }
    void SendNameChange(std::uint32_t dwActorID, const wchar_t* pChangeName);  // 对齐 IDA 0x140094820

    // GreenDamTan_: IDA 中不存在的辅助方法，用于支持现有调用
    void GreenDamTan_SetPartyID(std::uint32_t dwPartyID) { m_dwPartyID = dwPartyID; }
    void SetPartyID(std::uint32_t dwPartyID) { m_dwPartyID = dwPartyID; }  // ControlServer 专用
    void GreenDamTan_SetMemberInfo(ST_PARTY_MEMBER& stPartyMember);
    bool GreenDamTan_GetMemberInfo(std::uint32_t dwMemberID, ST_PARTY_MEMBER* pPartyMember);

    // 对齐 IDA ReqForceMatchingExit lambda10_: 遍历成员ID
    void ForEachMemberID(const std::function<void(std::uint32_t)>& callback) {
        for (const auto& pair : m_mapPartyMember) {
            callback(pair.first);
        }
    }

    // 对齐 IDA: 获取队伍类型
    std::uint8_t GetPartyType() { return m_byPartyType; }  // 对齐 IDA: 非const方法
    void SetPartyType(std::uint8_t byPartyType) { m_byPartyType = byPartyType; }

    // 对齐 IDA: GetForceType/SetForceType（原始二进制中 CParty 可能包含 force type 字段）
    // ReqPartyMazeClear 中检查 GetForceType() == 1 并调用 SetForceType(0)
    std::uint8_t GetForceType() { return m_byForceType; }  // 对齐 IDA: 非const方法
    void SetForceType(std::uint8_t byForceType) { m_byForceType = byForceType; }

    // 对齐 IDA 0x140013DF0: CParty::SetMemberEnterMap
    // 记录成员进入地图，更新 m_uxEnterMap
    void SetMemberEnterMap(std::uint32_t dwMemberID, UXMapID uxMapID) {
        auto it = m_mapPartyMember.find(dwMemberID);
        if (it != m_mapPartyMember.end() && it->second) {
            it->second->SetEnterMap(uxMapID);
        }
    }

    // 对齐 IDA 0x140030B80: CParty::IsFull
    bool IsFull() { return m_mapPartyMember.size() >= 4; }  // 队伍最大4人

    // 对齐 IDA 0x140030B00: CParty::RemoveMember
    void RemoveMember(std::uint32_t dwMemberID) {
        m_mapPartyMember.erase(dwMemberID);
    }

    // 对齐 IDA: CParty::IsEmpty
    bool IsEmpty() { return m_mapPartyMember.empty(); }

    // IDA 0x1403A6110: SendToLocal - 发送数据包给同地图的队伍成员
    void SendToLocal(XSendPacket& xSendPacket, UXMapID uxMapID, std::uint32_t dwExceptID);

    // IDA 0x1403A6DB0: SetMemberMapID - 设置成员地图ID并通知更新
    void SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, UXMapID uxMapID);
    // IDA 0x1403A6E80: SetMemberHP - 设置成员当前HP并广播
    void SetMemberHP(std::uint32_t dwActorID, UXMapID uxMapID, int nHP);
    // IDA 0x1403A6FE0: SetMemberMaxHP - 设置成员最大HP并广播
    void SetMemberMaxHP(std::uint32_t dwActorID, UXMapID uxMapID, int nMaxHP);
    // IDA 0x1403A7140: SetMemberLevel - 设置成员等级
    void SetMemberLevel(std::uint32_t dwActorID, int nLevel);
    // IDA 0x1403A71E0: SetMemberAwaken - 设置成员觉醒状态
    void SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken);
    // IDA 0x1403A7280: SetMemberProfilePhoto - 设置成员头像
    void SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID);
    // IDA 0x1403AA2D0: SendUpdateMemberInfo - 发送成员更新信息
    void SendUpdateMemberInfo(std::uint32_t dwActorID);
    // IDA 0x1403AA360: SetEnterMazeResponse - 设置进入迷宫响应
    bool SetEnterMazeResponse(std::uint32_t dwAgreeActor);
    // IDA 0x1401BD3B0: SetPartyMemberState - 设置队伍成员状态
    void SetPartyMemberState(std::uint8_t byType);
    // IDA 0x1403AA1F0: AgreeEnterMaze - 同意进入迷宫
    void AgreeEnterMaze(std::uint32_t dwActorID);

private:
    std::shared_ptr<CPartyMember> GetOrCreateMember(std::uint32_t dwMemberID);

    std::uint32_t m_dwPartyID = 0;
    std::uint32_t m_dwMasterID = 0;
    UXMapID m_uxMazeID{};
    std::uint8_t m_byPartyType = 0;  // 对齐 IDA: 队伍类型字段
    std::uint8_t m_byForceType = 0;  // 对齐 IDA: force type 字段（ReqPartyMazeClear 使用）
    std::int32_t m_nMaxLevel = 0;    // 对齐 IDA: 最高等级
    std::int32_t m_nMinLevel = 0;    // 对齐 IDA: 最低等级
    std::map<std::uint32_t, std::shared_ptr<CPartyMember>> m_mapPartyMember;
};
