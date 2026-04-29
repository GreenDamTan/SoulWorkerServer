// CPartyManager.h
// CPartyManager 队伍管理器类
// 对齐 IDA ControlServer.exe (40 bytes)

#pragma once

#include "CParty.h"
#include <cstdint>
#include <map>
#include <memory>

// 对齐 IDA: CPartyManager 队伍管理器类 (40 bytes)
// 布局: m_bLoad(1) -> padding(7) -> m_mapParty(32)
class CPartyManager {
public:
    CPartyManager();
    ~CPartyManager();

    // 对齐 IDA: IsLoad 检查是否已加载
    bool IsLoad() const {
        return m_bLoad;
    }

    // 对齐 IDA: SetLoad 设置加载状态
    void SetLoad(bool bLoad) {
        m_bLoad = bLoad;
    }

    // 对齐 IDA: GetParty 获取队伍
    std::tr1::shared_ptr<CParty> GetParty(int nPartyID) {
        auto it = m_mapParty.find(nPartyID);
        if (it != m_mapParty.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 对齐 IDA: AddParty 添加队伍
    void AddParty(int nPartyID, std::tr1::shared_ptr<CParty> pParty) {
        m_mapParty[nPartyID] = pParty;
    }

    // 对齐 IDA: RemoveParty 移除队伍
    void RemoveParty(int nPartyID) {
        m_mapParty.erase(nPartyID);
    }

    // 对齐 IDA: GetPartyCount 获取队伍数量
    size_t GetPartyCount() const {
        return m_mapParty.size();
    }

    // 对齐 IDA: Clear 清空所有队伍
    void Clear() {
        m_mapParty.clear();
        m_bLoad = false;
    }

    // 对齐 IDA: GetMapParty 获取队伍映射
    const std::map<int, std::tr1::shared_ptr<CParty>>& GetMapParty() const {
        return m_mapParty;
    }

    // 对齐 IDA 0x1400399A0: SetMember 设置成员位置
    void SetMember(int nPartyID, int nActorID, const UXMapID& uxMapID);

    // 对齐 IDA 0x140039C00: SetMazeID 设置迷宫ID
    bool SetMazeID(int nPartyID, const UXMapID& uxMapID, const UXMapID& uxBeforeMapID);

    // 对齐 IDA: RemoveMember 移除成员
    void RemoveMember(int nPartyID, int nActorID);

    // 对齐 IDA 0x140030E80: IsParty 检查队伍是否存在
    bool IsParty(int nPartyID);

    // 对齐 IDA 0x140030FE0: FindSamePlace 查找同位置成员
    UXMapID FindSamePlace(int nPartyID, unsigned int dwActorID, unsigned int dwMapID);

private:
    // 对齐 IDA 布局 (总大小 40 bytes):
    bool m_bLoad = true;                                 // +0x00 (0), 1 byte (IDA constructor sets to true)
    // +0x01 (1) 到 +0x08 (8): 7 bytes padding (对齐到 8 bytes boundary)
    std::map<int, std::tr1::shared_ptr<CParty>> m_mapParty; // +0x08 (8), 32 bytes
};
static_assert(sizeof(CPartyManager) == 40, "CPartyManager size mismatch with IDA (40 bytes)");