#pragma once

#include <cstdint>

#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"

/**
 * @brief 联赛成员类
 *
 * 管理单个联赛成员的信息。
 */
class CLeagueMember {
public:
    CLeagueMember();
    ~CLeagueMember() = default;

    // 禁止拷贝
    CLeagueMember(const CLeagueMember&) = delete;
    CLeagueMember& operator=(const CLeagueMember&) = delete;

    // IDA: ?Clear@CLeagueMember@@QEAAXXZ (0x14028a600)
    void Clear();

    // 成员信息
    void SetLeagueMember(ST_LEAGUE_MEMBER_EX stMember);  // 对齐 IDA: 按值传递
    void GetLeagueMember(ST_LEAGUE_MEMBER_EX& stMember);

    // 登录状态
    void Login();
    void LogOut();

    // 属性设置
    void SetPosition(std::uint8_t byPosition);
    void SetEnrollBoardDate(std::int64_t biDate);
    void SetPlayDate(std::int64_t biDate);
    void SetName(wchar_t* szName);  // 对齐 IDA: 非const指针 PEA_W
    void SetLevel(std::uint8_t byLevel);
    void SetAwaken(std::uint8_t byAwaken);
    void SetProfilePhoto(std::uint32_t dwProfilePhotoID);
    void SetMapInfo(std::int16_t wMapID, std::uint8_t byChannel);  // 对齐 IDA: GE = (short, unsigned char)

    // 属性获取
    void GetPosition(std::uint8_t& byPosition);
    void GetName(wchar_t* szName);

    // 经验
    void ResetExp();
    void AddExp(std::int16_t shExp);
    std::int64_t GetDailyExpLimit() const { return m_stMember.stMember.biLeagueExp; }

private:
    ST_LEAGUE_MEMBER_EX m_stMember{};  // 成员信息 (0x70 bytes)
    bool m_bEnrollBoard = true;        // 是否登记板报
};
