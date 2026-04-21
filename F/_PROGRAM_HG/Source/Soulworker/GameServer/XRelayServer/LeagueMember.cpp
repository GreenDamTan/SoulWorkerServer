#include "Soulworker/GameServer/XRelayServer/LeagueMember.h"

CLeagueMember::CLeagueMember() {
    m_bEnrollBoard = true;
}

void CLeagueMember::SetLeagueMember(const ST_LEAGUE_MEMBER_EX& stMember) {
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

void CLeagueMember::SetName(const wchar_t* szName) {
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

void CLeagueMember::SetMapInfo(std::uint16_t wMapID, std::uint8_t byChannel) {
    // 存储地图信息
    static_cast<void>(wMapID);
    static_cast<void>(byChannel);
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
