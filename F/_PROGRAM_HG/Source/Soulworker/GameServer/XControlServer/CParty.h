// CParty.h
// CParty 队伍类
// 对齐 IDA ControlServer.exe (48 bytes)

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>
#include <memory>

// 对齐 IDA: CParty 队伍类 (48 bytes)
// 布局: m_nPartyID(4) -> padding(4) -> m_uxMapID(8) -> m_mapMemberInfo(32)
class CParty {
public:
    CParty();
    explicit CParty(int nPartyID);
    ~CParty();

    // 对齐 IDA: GetPartyID 获取队伍ID
    int GetPartyID() const {
        return m_nPartyID;
    }

    // 对齐 IDA: SetPartyID 设置队伍ID
    void SetPartyID(int nPartyID) {
        m_nPartyID = nPartyID;
    }

    // 对齐 IDA: GetUxMapID 获取地图ID
    UXMapID GetUxMapID() const {
        return m_uxMapID;
    }

    // 对齐 IDA: SetUxMapID 设置地图ID
    void SetUxMapID(const UXMapID& uxMapID) {
        m_uxMapID = uxMapID;
    }

    // 对齐 IDA: GetMemberInfo 获取成员信息
    const std::map<int, UXMapID>& GetMemberInfo() const {
        return m_mapMemberInfo;
    }

    // 对齐 IDA 0x1400399A0: SetMember 设置成员位置
    void SetMember(int nActorID, const UXMapID& uxMapID) {
        m_mapMemberInfo[nActorID] = uxMapID;
    }

    // 对齐 IDA: GetMazeID 获取迷宫ID
    void GetMazeID(UXMapID* pMapID) const {
        if (pMapID) {
            *pMapID = m_uxMapID;
        }
    }

    // 对齐 IDA 0x140039C00: SetMazeID 设置迷宫ID
    void SetMazeID(const UXMapID& uxMapID) {
        m_uxMapID = uxMapID;
    }

    // 对齐 IDA: IsEmpty 检查是否为空
    bool IsEmpty() const {
        return m_mapMemberInfo.empty();
    }

    // 对齐 IDA: AddMember 添加成员
    void AddMember(int nUserID, const UXMapID& uxMapID) {
        m_mapMemberInfo[nUserID] = uxMapID;
    }

    // 对齐 IDA 0x140030B00: RemoveMember 移除成员
    void RemoveMember(int nActorID);

    // 对齐 IDA 0x140030B80: IsFull 检查是否已满 (最大 4 人)
    bool IsFull() const {
        return m_mapMemberInfo.size() >= 4;
    }

    // 对齐 IDA 0x140030BC0: FindSamePlace 查找同位置成员
    // 遍历 m_mapMemberInfo，找到 MapID 匹配且不是 dwActorID 的成员
    UXMapID FindSamePlace(unsigned int dwMapID, unsigned int dwActorID) const;

    // 对齐 IDA: GetMemberCount 获取成员数量
    size_t GetMemberCount() const {
        return m_mapMemberInfo.size();
    }

    // 对齐 IDA: IsMember 检查是否为成员
    bool IsMember(int nUserID) const {
        return m_mapMemberInfo.find(nUserID) != m_mapMemberInfo.end();
    }

    // 对齐 IDA: Clear 清空成员
    void Clear() {
        m_mapMemberInfo.clear();
    }

private:
    // 对齐 IDA 布局 (总大小 48 bytes):
    int m_nPartyID = 0;                     // +0x00 (0), 4 bytes
    // +0x04 (4): 4 bytes padding (对齐到 8 bytes boundary)
    UXMapID m_uxMapID{};                    // +0x08 (8), 8 bytes
    std::map<int, UXMapID> m_mapMemberInfo; // +0x10 (16), 32 bytes
};
static_assert(sizeof(CParty) == 48, "CParty size mismatch with IDA (48 bytes)");