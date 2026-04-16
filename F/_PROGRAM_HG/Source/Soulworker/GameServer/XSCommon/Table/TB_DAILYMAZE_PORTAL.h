// 中文说明：
// 1. 本文件承接 TB_DAILYMAZE_PORTAL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DAILYMAZE_PORTAL {
    std::uint16_t ID = 0;
    std::uint16_t Monday_Maze_Type = 0;
    std::uint16_t Monday_Attribute_Desc = 0;
    std::uint16_t Monday_Episode_01 = 0;
    std::uint16_t Monday_Episode_02 = 0;
    std::uint16_t Monday_Episode_03 = 0;
    std::uint16_t Monday_Episode_04 = 0;
    std::uint16_t Tuesday_Maze_Type = 0;
    std::uint16_t Tuesday_Attribute_Desc = 0;
    std::uint16_t Tuesday_Episode_01 = 0;
    std::uint16_t Tuesday_Episode_02 = 0;
    std::uint16_t Tuesday_Episode_03 = 0;
    std::uint16_t Tuesday_Episode_04 = 0;
    std::uint16_t Wednesday_Maze_Type = 0;
    std::uint16_t Wednesday_Attribute_Desc = 0;
    std::uint16_t Wednesday_Episode_01 = 0;
    std::uint16_t Wednesday_Episode_02 = 0;
    std::uint16_t Wednesday_Episode_03 = 0;
    std::uint16_t Wednesday_Episode_04 = 0;
    std::uint16_t Thursday_Maze_Type = 0;
    std::uint16_t Thursday_Attribute_Desc = 0;
    std::uint16_t Thursday_Episode_01 = 0;
    std::uint16_t Thursday_Episode_02 = 0;
    std::uint16_t Thursday_Episode_03 = 0;
    std::uint16_t Thursday_Episode_04 = 0;
    std::uint16_t Friday_Maze_Type = 0;
    std::uint16_t Friday_Attribute_Desc = 0;
    std::uint16_t Friday_Episode_01 = 0;
    std::uint16_t Friday_Episode_02 = 0;
    std::uint16_t Friday_Episode_03 = 0;
    std::uint16_t Friday_Episode_04 = 0;
    std::uint16_t Saturday_Maze_Type = 0;
    std::uint16_t Saturday_Attribute_Desc = 0;
    std::uint16_t Saturday_Episode_01 = 0;
    std::uint16_t Saturday_Episode_02 = 0;
    std::uint16_t Saturday_Episode_03 = 0;
    std::uint16_t Saturday_Episode_04 = 0;
    std::uint16_t Sunday_Maze_Type = 0;
    std::uint16_t Sunday_Attribute_Desc = 0;
    std::uint16_t Sunday_Episode_01 = 0;
    std::uint16_t Sunday_Episode_02 = 0;
    std::uint16_t Sunday_Episode_03 = 0;
    std::uint16_t Sunday_Episode_04 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DAILYMAZE_PORTAL) == 0x56, "TB_DAILYMAZE_PORTAL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DAILYMAZE_PORTAL* GetTB_DAILYMAZE_PORTAL(std::uint16_t index) ;
    void SetTB_DAILYMAZE_PORTAL(std::uint16_t index, const TB_DAILYMAZE_PORTAL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDailyMazePortalDB() ;
    std::unordered_map<std::uint16_t, TB_DAILYMAZE_PORTAL> dailyMazePortalRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DAILYMAZE_PORTAL* XResourceMgr::GetTB_DAILYMAZE_PORTAL(std::uint16_t index) {
        auto it = dailyMazePortalRows_.find(index);
        if (it == dailyMazePortalRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DAILYMAZE_PORTAL(std::uint16_t index, const TB_DAILYMAZE_PORTAL& row) {
        dailyMazePortalRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDailyMazePortalDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Monday_Maze_Type], [Monday_Attribute_Desc], [Monday_Episode_01], [Monday_Episode_02], [Monday_Episode_03], [Monday_Episode_04], [Tuesday_Maze_Type], [Tuesday_Attribute_Desc], [Tuesday_Episode_01], [Tuesday_Episode_02], [Tuesday_Episode_03], [Tuesday_Episode_04], [Wednesday_Maze_Type], [Wednesday_Attribute_Desc], [Wednesday_Episode_01], [Wednesday_Episode_02], [Wednesday_Episode_03], [Wednesday_Episode_04], [Thursday_Maze_Type], [Thursday_Attribute_Desc], [Thursday_Episode_01], [Thursday_Episode_02], [Thursday_Episode_03], [Thursday_Episode_04], [Friday_Maze_Type], [Friday_Attribute_Desc], [Friday_Episode_01], [Friday_Episode_02], [Friday_Episode_03], [Friday_Episode_04], [Saturday_Maze_Type], [Saturday_Attribute_Desc], [Saturday_Episode_01], [Saturday_Episode_02], [Saturday_Episode_03], [Saturday_Episode_04], [Sunday_Maze_Type], [Sunday_Attribute_Desc], [Sunday_Episode_01], [Sunday_Episode_02], [Sunday_Episode_03], [Sunday_Episode_04] from tb_DailyMaze_Portal")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dailyMazePortalRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DAILYMAZE_PORTAL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monday_Episode_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tuesday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tuesday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tuesday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tuesday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tuesday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tuesday_Episode_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wednesday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wednesday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wednesday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wednesday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wednesday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wednesday_Episode_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Thursday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Thursday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Thursday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Thursday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Thursday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Thursday_Episode_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friday_Episode_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Saturday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Saturday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Saturday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Saturday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Saturday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Saturday_Episode_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sunday_Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sunday_Attribute_Desc, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sunday_Episode_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sunday_Episode_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sunday_Episode_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sunday_Episode_04, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DAILYMAZE_PORTAL(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
