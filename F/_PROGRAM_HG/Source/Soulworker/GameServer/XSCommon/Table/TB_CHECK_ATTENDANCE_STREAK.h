// 中文说明：
// 1. 本文件承接 TB_CHECK_ATTENDANCE_STREAK 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CHECK_ATTENDANCE_STREAK {
    unsigned int ID = 0;
    unsigned int Check_Attendance_Type = 0;
    unsigned int Attendance_Streak_Reward[3] = {};
    unsigned int Attendance_Streak_Reward_Value[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_CHECK_ATTENDANCE_STREAK) == 0x20, "TB_CHECK_ATTENDANCE_STREAK size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CHECK_ATTENDANCE_STREAK* GetTB_CHECK_ATTENDANCE_STREAK(unsigned int index) ;
    void SetTB_CHECK_ATTENDANCE_STREAK(unsigned int index, const TB_CHECK_ATTENDANCE_STREAK& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCheckAttendanceStreakDB() ;
    // IDA-verified member variable name from XResourceMgr
    std::map<unsigned int, TB_CHECK_ATTENDANCE_STREAK> m_mapTB_CHECK_ATTENDANCE_STREAK;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CHECK_ATTENDANCE_STREAK* XResourceMgr::GetTB_CHECK_ATTENDANCE_STREAK(unsigned int index) {
        auto it = m_mapTB_CHECK_ATTENDANCE_STREAK.find(index);
        if (it == m_mapTB_CHECK_ATTENDANCE_STREAK.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CHECK_ATTENDANCE_STREAK(unsigned int index, const TB_CHECK_ATTENDANCE_STREAK& row) {
        m_mapTB_CHECK_ATTENDANCE_STREAK[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCheckAttendanceStreakDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Check_Attendance_Type], [Attendance_Streak_Reward_1day], [Attendance_Streak_Reward_1day_Value], [Attendance_Streak_Reward_2day], [Attendance_Streak_Reward_2day_Value], [Attendance_Streak_Reward_3day], [Attendance_Streak_Reward_3day_Value] from tb_Check_Attendance_Streak")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_CHECK_ATTENDANCE_STREAK.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CHECK_ATTENDANCE_STREAK row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Attendance_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 3; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attendance_Streak_Reward[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attendance_Streak_Reward_Value[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_CHECK_ATTENDANCE_STREAK(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
