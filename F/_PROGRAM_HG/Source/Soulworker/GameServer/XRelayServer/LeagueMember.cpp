#include "Soulworker/GameServer/XRelayServer/LeagueMember.h"

CLeagueMember::CLeagueMember() {
    m_bEnrollBoard = true;
}

// IDA: ?Clear@CLeagueMember@@QEAAXXZ (0x14028a600)
// Verified: Direct IDA decompilation
void CLeagueMember::Clear() {
    // IDA decompiled:
    // void __fastcall CLeagueMember::Clear(CLeagueMember *this)
    // {
    //   std::_Tree<...>::clear(...);
    //   this->m_nInventorySyncCount = 0;
    //   this->m_nSyncCount = 0;
    // }

    // Reset member info
    m_stMember = ST_LEAGUE_MEMBER_EX{};
    m_bEnrollBoard = true;
    // TODO: m_nInventorySyncCount and m_nSyncCount not defined yet
}

void CLeagueMember::SetLeagueMember(ST_LEAGUE_MEMBER_EX stMember) {
    m_stMember = stMember;
}

void CLeagueMember::GetLeagueMember(ST_LEAGUE_MEMBER_EX& stMember) {
    stMember = m_stMember;
}

void CLeagueMember::Login() {
    m_stMember.bLogin = true;
}

void CLeagueMember::LogOut() {
    m_stMember.bLogin = false;
}

void CLeagueMember::SetPosition(std::uint8_t byPosition) {
    m_stMember.stMember.byPosition = byPosition;
}

void CLeagueMember::SetEnrollBoardDate(std::int64_t biDate) {
    m_stMember.biBoardLimitTime = biDate;
}

void CLeagueMember::SetPlayDate(std::int64_t biDate) {
    m_stMember.biPlayDate = biDate;
}

void CLeagueMember::SetName(wchar_t* szName) {  // 对齐 IDA: 非const指针
    if (szName) {
        wcscpy_s(m_stMember.szName, 21, szName);
    }
}

void CLeagueMember::SetLevel(std::uint8_t byLevel) {
    m_stMember.shLevel = static_cast<std::int16_t>(byLevel);
}

void CLeagueMember::SetAwaken(std::uint8_t byAwaken) {
    m_stMember.byAwaken = byAwaken;
}

void CLeagueMember::SetProfilePhoto(std::uint32_t dwProfilePhotoID) {
    m_stMember.dwProfilePhotoID = dwProfilePhotoID;
}

void CLeagueMember::SetMapInfo(std::int16_t wMapID, std::uint8_t byChannel) {  // 对齐 IDA: GE
    // 对齐 IDA 0x140073270: sWorldID + byChannel 字段赋值
    m_stMember.sWorldID = wMapID;
    m_stMember.byChannel = byChannel;
}

void CLeagueMember::GetPosition(std::uint8_t& byPosition) {
    byPosition = m_stMember.stMember.byPosition;
}

void CLeagueMember::GetName(wchar_t* szName) {
    if (szName) {
        wcscpy_s(szName, 21, m_stMember.szName);
    }
}

void CLeagueMember::ResetExp() {
    m_stMember.stMember.biLeagueExp = 0;
}

void CLeagueMember::AddExp(std::int16_t shExp) {
    // 对齐 IDA 0x140064490
    m_stMember.stMember.biLeagueExp += shExp;
    if (m_stMember.stMember.biLeagueExp > 200) {
        m_stMember.stMember.biLeagueExp = 200;
    }
}
