// ControlServer Simple Party Manager
// 对齐 ControlServer.exe IDA 结构

#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

// 移除 SS_UPDATE_SERVER_INFO 重定义冲突
#ifdef SS_UPDATE_SERVER_INFO
#undef SS_UPDATE_SERVER_INFO
#endif

// 对齐 IDA CParty: size=48 (0x30)
// struct CParty {
//   m_nPartyID (int, +0x0)
//   m_uxMapID (UXMapID, +0x8)
//   m_mapMemberInfo (std::map<int,UXMapID>, +0x10, size=32)
// }
class CParty {
public:
    CParty() = default;
    explicit CParty(int nPartyID) : m_nPartyID(nPartyID) {}

    int GetPartyID() const { return m_nPartyID; }
    void SetPartyID(int nPartyID) { m_nPartyID = nPartyID; }

    UXMapID GetMazeID() const { return m_uxMapID; }
    void SetMazeID(UXMapID uxMapID) { m_uxMapID = uxMapID; }

    // 对齐 IDA 0x140030B80: size() >= 4
    bool IsFull() const { return m_mapMemberInfo.size() >= 4; }

    // 对齐 IDA 0x140039980: empty()
    bool IsEmpty() const { return m_mapMemberInfo.empty(); }

    // 对齐 IDA CForce::SetMember 0x1400398D0
    void SetMember(int nActorID, UXMapID uxMapID) {
        auto it = m_mapMemberInfo.find(nActorID);
        if (it != m_mapMemberInfo.end()) {
            it->second = uxMapID;
        } else {
            m_mapMemberInfo.emplace(nActorID, uxMapID);
        }
    }

    // 对齐 IDA 0x140030B00: erase by key
    void RemoveMember(int nActorID) {
        m_mapMemberInfo.erase(nActorID);
    }

    // 对齐 IDA 0x140030BC0: FindSamePlace - 返回同地图成员的 UXMapID
    UXMapID FindSamePlace(std::uint16_t wMapID, std::uint32_t dwExcludeActorID) const {
        for (const auto& pair : m_mapMemberInfo) {
            if (pair.first != static_cast<int>(dwExcludeActorID)) {
                // 检查 mapID 匹配 (使用 HIWORD 提取 mapID)
                // HIWORD equivalent: (nMapID >> 16) & 0xFFFF
                std::uint16_t memberMapID = static_cast<std::uint16_t>((pair.second.nMapID >> 16) & 0xFFFF);
                if (memberMapID == wMapID) {
                    return pair.second;
                }
            }
        }
        return UXMapID();  // 返回默认构造的 UXMapID
    }

    std::size_t GetUserCount() const { return m_mapMemberInfo.size(); }

private:
    int m_nPartyID = 0;                              // +0x00
    std::uint8_t _pad0[4] = {};                       // +0x04 (padding)
    UXMapID m_uxMapID{};                              // +0x08
    std::map<int, UXMapID> m_mapMemberInfo;           // +0x10
};

// 对齐 IDA CPartyManager
class CPartyManager {
public:
    void Clear() { m_mapParty.clear(); m_mapPartyUser.clear(); }

    // 对齐 IDA: IsLoad - 判断是否已加载
    bool IsLoad() const { return m_bLoad; }
    void SetLoad(bool bLoad) { m_bLoad = bLoad; }

    std::shared_ptr<CParty> GetParty(std::uint32_t dwPartyID) {
        auto it = m_mapParty.find(dwPartyID);
        return it == m_mapParty.end() ? std::shared_ptr<CParty>{} : it->second;
    }

    bool IsParty(std::uint32_t dwActorID) {
        UXActorID uxActorID{};
        uxActorID.dwActorID = dwActorID;
        return m_mapPartyUser.find(uxActorID) != m_mapPartyUser.end();
    }

    // 对齐 IDA 0x140030EE0: IsFull
    bool IsFull(std::uint32_t dwPartyID) {
        auto it = m_mapParty.find(dwPartyID);
        if (it == m_mapParty.end() || !it->second) {
            return false;
        }
        return it->second->IsFull();
    }

    // 对齐 IDA 0x1400399A0: SetMember
    void SetMember(int nPartyID, int nActorID, UXMapID uxMapID);

    // 对齐 IDA 0x140039B30: RemoveMember
    void RemoveMember(int nPartyID, int nActorID);

    // 对齐 IDA 0x140039C00: SetMazeID (3参数版本)
    bool SetMazeID(int nPartyID, UXMapID uxMapID, UXMapID uxBeforeMapID);

    // 对齐 IDA 0x140030F60: SetMazeID (2参数版本 - ControlServer使用)
    // 注意: IDA中此函数位于CForceManager，但实际被CPartyManager调用
    void SetMazeID(int nPartyID, UXMapID uxMapID);

    // 对齐 IDA CForceManager::GetMazeID 0x140039D00
    bool GetMazeID(int nPartyID, int nActorID, UXMapID* puxMapID);

    // 对齐 IDA 0x140030FE0: FindSamePlace
    UXMapID FindSamePlace(std::uint32_t dwPartyID, std::uint32_t dwActorID, bool bRaid);

private:
    std::shared_ptr<CParty> GetOrCreateParty(std::uint32_t dwPartyID);

protected:
    std::map<std::uint32_t, std::shared_ptr<CParty>> m_mapParty;
    std::map<UXActorID, std::uint32_t> m_mapPartyUser;
    bool m_bLoad = false;
};

// 对齐 IDA CForce: size=48 (同 CParty)
class CForce : public CParty {
public:
    CForce() = default;
    explicit CForce(int nForceID) : CParty(nForceID) {}

    int GetForceID() const { return GetPartyID(); }
    void SetForceID(int nForceID) { SetPartyID(nForceID); }
};

// 对齐 IDA CForceManager (继承 CPartyManager)
class CForceManager : public CPartyManager {
public:
    std::shared_ptr<CForce> GetForce(std::uint32_t dwForceID) {
        auto party = GetParty(dwForceID);
        return std::static_pointer_cast<CForce>(party);
    }

    bool IsForce(std::uint32_t dwForceID) {
        return m_mapParty.find(dwForceID) != m_mapParty.end();
    }

    // 对齐 IDA 0x140030CF0: SetMember
    void SetMember(int nForceID, int nActorID, UXMapID uxMapID);

    // 对齐 IDA 0x140039B30: RemoveMember
    void RemoveMember(int nForceID, int nActorID);

    // 对齐 IDA 0x140030F60: SetMazeID (2参数版本 - ControlServer)
    void SetMazeID(int nForceID, UXMapID uxMapID);
    // 对齐 IDA 0x140039C00: SetMazeID (3参数版本 - 兼容)
    bool SetMazeID(int nForceID, UXMapID uxMapID, UXMapID uxBeforeMapID);

    // 对齐 IDA 0x140039D00: GetMazeID
    bool GetMazeID(int nForceID, int nActorID, UXMapID* puxMapID);

    // 对齐 IDA 0x140030EE0: IsFull
    bool IsFull(int nForceID);

private:
    std::shared_ptr<CForce> GetOrCreateForce(std::uint32_t dwForceID);
};
