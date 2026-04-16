// 中文说明：
// 1. 本文件承接 TB_MAZE_OPENCONTROL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_MAZE_OPENCONTROL {
    unsigned int ID = 0;
    std::uint16_t Open_Time = 0;
    std::uint16_t Remain_Time = 0;
};
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZE_OPENCONTROL* GetTB_MAZE_OPENCONTROL(unsigned int index) ;
    void SetTB_MAZE_OPENCONTROL(unsigned int index, const TB_MAZE_OPENCONTROL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeOpenControlDB() ;
    std::map<unsigned int, TB_MAZE_OPENCONTROL> m_mapTB_MAZE_OPENCONTROL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZE_OPENCONTROL* XResourceMgr::GetTB_MAZE_OPENCONTROL(unsigned int index) {
        const auto it = m_mapTB_MAZE_OPENCONTROL.find(index);
        return it == m_mapTB_MAZE_OPENCONTROL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZE_OPENCONTROL(unsigned int index, const TB_MAZE_OPENCONTROL& row) {
        m_mapTB_MAZE_OPENCONTROL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeOpenControlDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Open_Time], [Remain_Time] from tb_Maze_OpenControl")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZE_OPENCONTROL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZE_OPENCONTROL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Open_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remain_Time, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZE_OPENCONTROL(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
