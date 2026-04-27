// 中文说明：
// 1. 本文件承接 TB_MAZE_ENTER_COUNT_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_MAZE_ENTER_COUNT_GROUP {
    std::uint16_t Group_ID = 0;
    std::uint16_t Group_Maze_01 = 0;
    std::uint16_t Group_Maze_02 = 0;
    std::uint16_t Group_Maze_03 = 0;
    std::uint16_t Group_Maze_04 = 0;
    std::uint16_t Group_Maze_05 = 0;
    std::uint16_t Group_Maze_06 = 0;
    std::uint16_t Group_Maze_07 = 0;
    std::uint16_t Group_Maze_08 = 0;
    std::uint16_t Group_Maze_09 = 0;
    std::uint16_t Group_Maze_10 = 0;
};
static_assert(sizeof(TB_MAZE_ENTER_COUNT_GROUP) == 0x16, "TB_MAZE_ENTER_COUNT_GROUP size mismatch with IDA");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZE_ENTER_COUNT_GROUP* GetTB_MAZE_ENTER_COUNT_GROUP(std::uint16_t index) ;
    void SetTB_MAZE_ENTER_COUNT_GROUP(std::uint16_t index, const TB_MAZE_ENTER_COUNT_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeEnterCountGroupDB() ;
    std::map<std::uint16_t, TB_MAZE_ENTER_COUNT_GROUP> m_mapTB_MAZE_ENTER_COUNT_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZE_ENTER_COUNT_GROUP* XResourceMgr::GetTB_MAZE_ENTER_COUNT_GROUP(std::uint16_t index) {
        const auto it = m_mapTB_MAZE_ENTER_COUNT_GROUP.find(index);
        return it == m_mapTB_MAZE_ENTER_COUNT_GROUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZE_ENTER_COUNT_GROUP(std::uint16_t index, const TB_MAZE_ENTER_COUNT_GROUP& row) {
        m_mapTB_MAZE_ENTER_COUNT_GROUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeEnterCountGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Group_ID], [Group_Maze_01], [Group_Maze_02], [Group_Maze_03], [Group_Maze_04], [Group_Maze_05], [Group_Maze_06], [Group_Maze_07], [Group_Maze_08], [Group_Maze_09], [Group_Maze_10] from tb_Maze_Enter_Count_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZE_ENTER_COUNT_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZE_ENTER_COUNT_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Maze_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZE_ENTER_COUNT_GROUP(row.Group_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
