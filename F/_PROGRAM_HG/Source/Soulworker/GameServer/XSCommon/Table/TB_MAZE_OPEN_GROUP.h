// 中文说明：
// 1. 本文件承接 TB_MAZE_OPEN_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_MAZE_OPEN_GROUP {
    std::uint16_t ID = 0;
    std::uint16_t OpenCondition_MazeID_01 = 0;
    std::uint16_t OpenCondition_MazeID_02 = 0;
    std::uint16_t OpenCondition_MazeID_03 = 0;
    std::uint16_t OpenCondition_MazeID_04 = 0;
    std::uint16_t OpenCondition_MazeID_05 = 0;
    std::uint16_t OpenCondition_MazeID_06 = 0;
    std::uint16_t OpenCondition_MazeID_07 = 0;
    std::uint16_t OpenCondition_MazeID_08 = 0;
    std::uint16_t OpenCondition_MazeID_09 = 0;
    std::uint16_t OpenCondition_MazeID_10 = 0;
};
static_assert(sizeof(TB_MAZE_OPEN_GROUP) == 0x16, "TB_MAZE_OPEN_GROUP size mismatch with IDA");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZE_OPEN_GROUP* GetTB_MAZE_OPEN_GROUP(std::uint16_t index) ;
    void SetTB_MAZE_OPEN_GROUP(std::uint16_t index, const TB_MAZE_OPEN_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeOpenGroupDB() ;
    std::map<std::uint16_t, TB_MAZE_OPEN_GROUP> m_mapTB_MAZE_OPEN_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZE_OPEN_GROUP* XResourceMgr::GetTB_MAZE_OPEN_GROUP(std::uint16_t index) {
        const auto it = m_mapTB_MAZE_OPEN_GROUP.find(index);
        return it == m_mapTB_MAZE_OPEN_GROUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZE_OPEN_GROUP(std::uint16_t index, const TB_MAZE_OPEN_GROUP& row) {
        m_mapTB_MAZE_OPEN_GROUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeOpenGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [OpenCondition_MazeID_01], [OpenCondition_MazeID_02], [OpenCondition_MazeID_03], [OpenCondition_MazeID_04], [OpenCondition_MazeID_05], [OpenCondition_MazeID_06], [OpenCondition_MazeID_07], [OpenCondition_MazeID_08], [OpenCondition_MazeID_09], [OpenCondition_MazeID_10] from tb_Maze_Open_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZE_OPEN_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZE_OPEN_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OpenCondition_MazeID_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZE_OPEN_GROUP(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
