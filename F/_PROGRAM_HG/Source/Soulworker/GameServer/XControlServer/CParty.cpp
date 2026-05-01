// CParty.cpp
// CParty 队伍类实现
// 对齐 IDA ControlServer.exe

#include "CParty.h"

CParty::CParty()
    : m_nPartyID(0)
    , m_uxMapID{}
{
}

CParty::CParty(int nPartyID)
    : m_nPartyID(nPartyID)
    , m_uxMapID{}
{
}

CParty::~CParty()
{
    m_mapMemberInfo.clear();
}

// 对齐 IDA 0x140030B00: RemoveMember 移除成员
void CParty::RemoveMember(int nActorID)
{
    auto it = m_mapMemberInfo.find(nActorID);
    if (it != m_mapMemberInfo.end()) {
        m_mapMemberInfo.erase(it);
    }
}

// 对齐 IDA 0x140030BC0: FindSamePlace 查找同位置成员
// 遍历 m_mapMemberInfo，找到 MapID 匹配且不是 dwActorID 的成员
UXMapID CParty::FindSamePlace(unsigned int dwMapID, unsigned int dwActorID) const
{
    for (auto it = m_mapMemberInfo.begin(); it != m_mapMemberInfo.end(); ++it) {
        // 对齐 IDA: dwActorID != it->first 时才检查
        if (static_cast<unsigned int>(it->first) == dwActorID) {
            continue;
        }
        // 对齐 IDA: SWORD2(uxMemberMapID.nMapID) == wMapID
        // SWORD2 提取 bits 16-31 (signed short)
        short memberMapID = static_cast<short>((it->second.nMapID >> 16) & 0xFFFF);
        if (static_cast<unsigned short>(memberMapID) == static_cast<unsigned short>(dwMapID)) {
            return it->second;
        }
    }
    return UXMapID{};
}