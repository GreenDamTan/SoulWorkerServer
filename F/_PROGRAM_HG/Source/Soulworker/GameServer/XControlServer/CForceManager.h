// CForceManager.h
// CForceManager Force/军团管理器类
// 对齐 IDA ControlServer.exe (40 bytes)

#pragma once

#include "CForce.h"
#include <cstdint>
#include <map>
#include <memory>

// 对齐 IDA: CForceManager Force/军团管理器类 (40 bytes)
// 布局: m_bLoad(1) -> padding(7) -> m_mapForce(32)
class CForceManager {
public:
    CForceManager();
    ~CForceManager();

    // 对齐 IDA: IsLoad 检查是否已加载
    bool IsLoad() const {
        return m_bLoad;
    }

    // 对齐 IDA: SetLoad 设置加载状态
    void SetLoad(bool bLoad) {
        m_bLoad = bLoad;
    }

    // 对齐 IDA: GetForce 获取Force
    std::tr1::shared_ptr<CForce> GetForce(int nForceID) {
        auto it = m_mapForce.find(nForceID);
        if (it != m_mapForce.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 对齐 IDA: AddForce 添加Force
    void AddForce(int nForceID, std::tr1::shared_ptr<CForce> pForce) {
        m_mapForce[nForceID] = pForce;
    }

    // 对齐 IDA: RemoveForce 移除Force
    void RemoveForce(int nForceID) {
        m_mapForce.erase(nForceID);
    }

    // 对齐 IDA: GetForceCount 获取Force数量
    size_t GetForceCount() const {
        return m_mapForce.size();
    }

    // 对齐 IDA: Clear 清空所有Force
    void Clear() {
        m_mapForce.clear();
        m_bLoad = false;
    }

    // 对齐 IDA: GetMapForce 获取Force映射
    const std::map<int, std::tr1::shared_ptr<CForce>>& GetMapForce() const {
        return m_mapForce;
    }

    // 对齐 IDA 0x140030CF0: SetMember 设置成员位置
    void SetMember(int nForceID, int nActorID, const UXMapID& uxMapID);

    // 对齐 IDA 0x140030F60: SetMazeID 设置迷宫ID
    void SetMazeID(int nForceID, const UXMapID& uxMapID);

    // 对齐 IDA 0x140039B30: RemoveMember 移除成员
    void RemoveMember(int nForceID, int nActorID);

    // 对齐 IDA 0x140030EE0: IsFull 检查是否已满
    bool IsFull(int nForceID);

    // 对齐 IDA 0x140039CD0: GetMazeID 获取迷宫ID
    // 参数: nForceID, nActorID (IDA中存在但未使用), pMapID (输出)
    bool GetMazeID(int nForceID, int nActorID, UXMapID* pMapID);

private:
    // 对齐 IDA 布局 (总大小 40 bytes):
    bool m_bLoad = true;                                 // +0x00 (0), 1 byte (IDA pattern: true)
    // +0x01 (1) 到 +0x08 (8): 7 bytes padding (对齐到 8 bytes boundary)
    std::map<int, std::tr1::shared_ptr<CForce>> m_mapForce; // +0x08 (8), 32 bytes
};
static_assert(sizeof(CForceManager) == 40, "CForceManager size mismatch with IDA (40 bytes)");