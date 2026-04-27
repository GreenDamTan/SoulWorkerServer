// 中文说明：
// 1. 本文件承接 TB_LEAGUE_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_LEAGUE_INFO {
    unsigned int League_Info_ID = 0;
    unsigned int League_Level = 0;
    unsigned int Need_Exp = 0;
    unsigned int League_Member = 0;
    unsigned int League_Inventory = 0;
    unsigned int League_Skill_Get_Point = 0;
    unsigned int Day_Exp_Get_Point = 0;
};
static_assert(sizeof(TB_LEAGUE_INFO) == 0x1C, "TB_LEAGUE_INFO size mismatch with IDA");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_LEAGUE_INFO* GetTB_LEAGUE_INFO(unsigned int index) ;
    void SetTB_LEAGUE_INFO(unsigned int index, const TB_LEAGUE_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBLeagueInfoDB() ;
    std::map<unsigned int, TB_LEAGUE_INFO> m_mapTB_LEAGUE_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_LEAGUE_INFO* XResourceMgr::GetTB_LEAGUE_INFO(unsigned int index) {
        const auto it = m_mapTB_LEAGUE_INFO.find(index);
        return it == m_mapTB_LEAGUE_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_LEAGUE_INFO(unsigned int index, const TB_LEAGUE_INFO& row) {
        m_mapTB_LEAGUE_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBLeagueInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [League_Info_ID], [League_Level], [Need_Exp], [League_Member], [League_Inventory], [League_Skill_Get_Point], [Day_Exp_Get_Point] from tb_League_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_LEAGUE_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_LEAGUE_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Info_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Exp, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Member, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Inventory, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Get_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Day_Exp_Get_Point, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_LEAGUE_INFO(row.League_Info_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
