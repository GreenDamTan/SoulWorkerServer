// 中文说明：
// 1. 本文件承接 TB_SPECIALMAZE_OPEN 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SPECIALMAZE_OPEN {
    std::uint16_t ID = 0;
    std::uint16_t uniMazeGroup[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_SPECIALMAZE_OPEN) == 0x16, "TB_SPECIALMAZE_OPEN size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SPECIALMAZE_OPEN* GetTB_SPECIALMAZE_OPEN(std::uint16_t index) ;
    void SetTB_SPECIALMAZE_OPEN(std::uint16_t index, const TB_SPECIALMAZE_OPEN& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSpecialMazeOpenDB() ;
    std::map<std::uint16_t, TB_SPECIALMAZE_OPEN> m_mapTB_SPECIALMAZE_OPEN;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SPECIALMAZE_OPEN* XResourceMgr::GetTB_SPECIALMAZE_OPEN(std::uint16_t index) {
        const auto it = m_mapTB_SPECIALMAZE_OPEN.find(index);
        return it == m_mapTB_SPECIALMAZE_OPEN.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SPECIALMAZE_OPEN(std::uint16_t index, const TB_SPECIALMAZE_OPEN& row) {
        m_mapTB_SPECIALMAZE_OPEN[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSpecialMazeOpenDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [OpenCondition_MazeGroupID_01], [OpenCondition_MazeGroupID_02], [OpenCondition_MazeGroupID_03], [OpenCondition_MazeGroupID_04], [OpenCondition_MazeGroupID_05], [OpenCondition_MazeGroupID_06], [OpenCondition_MazeGroupID_07], [OpenCondition_MazeGroupID_08], [OpenCondition_MazeGroupID_09], [OpenCondition_MazeGroupID_10] from tb_SpecialMaze_Open")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SPECIALMAZE_OPEN.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SPECIALMAZE_OPEN row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& value : row.uniMazeGroup) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &value, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_SPECIALMAZE_OPEN(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
