// CForce.h
// CForce Force/军团类
// 对齐 IDA ControlServer.exe (48 bytes)

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>
#include <memory>

// 对齐 IDA: CForce Force/军团类 (48 bytes)
// 布局: m_nForceID(4) -> padding(4) -> m_uxMapID(8) -> m_mapMemberInfo(32)
class CForce {
public:
    CForce();
    explicit CForce(int nForceID);
    ~CForce();

    // 对齐 IDA: GetForceID 获取Force ID
    int GetForceID() const {
        return m_nForceID;
    }

    // 对齐 IDA: SetForceID 设置Force ID
    void SetForceID(int nForceID) {
        m_nForceID = nForceID;
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

    // 对齐 IDA 0x1400398D0: SetMember 设置成员位置
    void SetMember(int nActorID, const UXMapID& uxMapID);

    // 对齐 IDA 0x140039CD0: GetMazeID 获取迷宫ID (仅返回 m_uxMapID)
    void GetMazeID(UXMapID* pMapID) const {
        if (pMapID) {
            *pMapID = m_uxMapID;
        }
    }

    // 对齐 IDA 0x14003A420: SetMazeID 设置迷宫ID
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

    // 对齐 IDA 0x140039B30: RemoveMember 移除成员
    void RemoveMember(int nUserID) {
        m_mapMemberInfo.erase(nUserID);
    }

    // 对齐 IDA: IsFull 检查是否已满 (最大成员数，实际由上层控制)
    bool IsFull() const {
        // Force 没有 IsFull 检查，由 ForceManager 控制
        return false;
    }

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
    int m_nForceID = 0;                     // +0x00 (0), 4 bytes
    // +0x04 (4): 4 bytes padding (对齐到 8 bytes boundary)
    UXMapID m_uxMapID{};                    // +0x08 (8), 8 bytes
    std::map<int, UXMapID> m_mapMemberInfo; // +0x10 (16), 32 bytes
};
static_assert(sizeof(CForce) == 48, "CForce size mismatch with IDA (48 bytes)");