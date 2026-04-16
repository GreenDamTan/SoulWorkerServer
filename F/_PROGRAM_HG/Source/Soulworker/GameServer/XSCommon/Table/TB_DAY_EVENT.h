// 中文说明：
// 1. 本文件承接 TB_DAY_EVENT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DAY_EVENT {
    unsigned int ID = 0;
    std::uint8_t Event_Count_Min = 0;
    std::uint8_t Event_Count_Max = 0;
    std::uint16_t Maze_Select_1 = 0;
    std::uint16_t Maze_Select_2 = 0;
    std::uint16_t Maze_Select_3 = 0;
    std::uint16_t Maze_Select_4 = 0;
    std::uint16_t Maze_Select_5 = 0;
    std::uint16_t Maze_Select_6 = 0;
    std::uint16_t Maze_Select_7 = 0;
    std::uint16_t Maze_Select_8 = 0;
    std::uint16_t Maze_Select_9 = 0;
    std::uint16_t Maze_Select_10 = 0;
    std::uint16_t Maze_Booster_Group_1 = 0;
    std::uint16_t Maze_Booster_Group_2 = 0;
    std::uint16_t Maze_Booster_Group_3 = 0;
    std::uint16_t Maze_Booster_Group_4 = 0;
    std::uint16_t Maze_Booster_Group_5 = 0;
    std::uint16_t Maze_Booster_Group_6 = 0;
    std::uint16_t Maze_Booster_Group_7 = 0;
    std::uint16_t Maze_Booster_Group_8 = 0;
    std::uint16_t Maze_Booster_Group_9 = 0;
    std::uint16_t Maze_Booster_Group_10 = 0;
    std::uint16_t Fix_Maze_Select_1 = 0;
    std::uint16_t Fix_Maze_Select_2 = 0;
    std::uint16_t Fix_Maze_Select_3 = 0;
    std::uint16_t Fix_Maze_Select_4 = 0;
    std::uint16_t Fix_Maze_Booster_Group_1 = 0;
    std::uint16_t Fix_Maze_Booster_Group_2 = 0;
    std::uint16_t Fix_Maze_Booster_Group_3 = 0;
    std::uint16_t Fix_Maze_Booster_Group_4 = 0;

    std::array<std::uint16_t, 10> GetMazeSelects() const {
        return {Maze_Select_1, Maze_Select_2, Maze_Select_3, Maze_Select_4, Maze_Select_5,
                Maze_Select_6, Maze_Select_7, Maze_Select_8, Maze_Select_9, Maze_Select_10};
    }

    std::array<std::uint16_t, 10> GetMazeBoosterGroups() const {
        return {Maze_Booster_Group_1, Maze_Booster_Group_2, Maze_Booster_Group_3, Maze_Booster_Group_4,
                Maze_Booster_Group_5, Maze_Booster_Group_6, Maze_Booster_Group_7, Maze_Booster_Group_8,
                Maze_Booster_Group_9, Maze_Booster_Group_10};
    }

    std::array<std::uint16_t, 4> GetFixMazeSelects() const {
        return {Fix_Maze_Select_1, Fix_Maze_Select_2, Fix_Maze_Select_3, Fix_Maze_Select_4};
    }

    std::array<std::uint16_t, 4> GetFixMazeBoosterGroups() const {
        return {Fix_Maze_Booster_Group_1, Fix_Maze_Booster_Group_2, Fix_Maze_Booster_Group_3,
                Fix_Maze_Booster_Group_4};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_DAY_EVENT) == 0x3E, "TB_DAY_EVENT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DAY_EVENT* GetTB_DAY_EVENT(unsigned int index) ;
    void SetTB_DAY_EVENT(unsigned int index, const TB_DAY_EVENT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDayEventDB() ;
    std::unordered_map<unsigned int, TB_DAY_EVENT> dayEventRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DAY_EVENT* XResourceMgr::GetTB_DAY_EVENT(unsigned int index) {
        auto it = dayEventRows_.find(index);
        if (it == dayEventRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DAY_EVENT(unsigned int index, const TB_DAY_EVENT& row) {
        dayEventRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDayEventDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Event_Count_Min], [Event_Count_Max], [Maze_Select_1], [Maze_Booster_Group_1], [Maze_Select_2], [Maze_Booster_Group_2], [Maze_Select_3], [Maze_Booster_Group_3], [Maze_Select_4], [Maze_Booster_Group_4], [Maze_Select_5], [Maze_Booster_Group_5], [Maze_Select_6], [Maze_Booster_Group_6], [Maze_Select_7], [Maze_Booster_Group_7], [Maze_Select_8], [Maze_Booster_Group_8], [Maze_Select_9], [Maze_Booster_Group_9], [Maze_Select_10], [Maze_Booster_Group_10], [Fix_Maze_Select_1], [Fix_Maze_Booster_Group_1], [Fix_Maze_Select_2], [Fix_Maze_Booster_Group_2], [Fix_Maze_Select_3], [Fix_Maze_Booster_Group_3], [Fix_Maze_Select_4], [Fix_Maze_Booster_Group_4] from tb_Day_Event")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dayEventRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DAY_EVENT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Count_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Count_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Select_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Booster_Group_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Select_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Booster_Group_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Select_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Booster_Group_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Select_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Booster_Group_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Select_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Maze_Booster_Group_4, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DAY_EVENT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
