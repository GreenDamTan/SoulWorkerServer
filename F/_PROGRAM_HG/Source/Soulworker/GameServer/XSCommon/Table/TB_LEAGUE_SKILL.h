// 中文说明：
// 1. 本文件承接 TB_LEAGUE_SKILL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_LEAGUE_SKILL {
    unsigned int League_Skill_Index = 0;
    unsigned int League_Skill_Type = 0;
    unsigned int League_Skill_Level = 0;
    unsigned int League_Skill_Level_Max = 0;
    unsigned int League_Skill_Group = 0;
    unsigned int League_Next_Skill = 0;
    unsigned int League_Get_Skill_Level = 0;
    unsigned int League_Skill_Effect_Type = 0;
    unsigned int League_Skill_Apply_Type = 0;
    unsigned int League_Skill_Apply_Value = 0;
    unsigned int League_Skill_Need_Point = 0;
    unsigned int League_Skill_Need_Gold = 0;
};
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_LEAGUE_SKILL* GetTB_LEAGUE_SKILL(unsigned int index) ;
    void SetTB_LEAGUE_SKILL(unsigned int index, const TB_LEAGUE_SKILL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBLeagueSkillDB() ;
    std::map<unsigned int, TB_LEAGUE_SKILL> m_mapTB_LEAGUE_SKILL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_LEAGUE_SKILL* XResourceMgr::GetTB_LEAGUE_SKILL(unsigned int index) {
        const auto it = m_mapTB_LEAGUE_SKILL.find(index);
        return it == m_mapTB_LEAGUE_SKILL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_LEAGUE_SKILL(unsigned int index, const TB_LEAGUE_SKILL& row) {
        m_mapTB_LEAGUE_SKILL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBLeagueSkillDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [League_Skill_Index], [League_Skill_Type], [League_Skill_Level], [League_Skill_Level_Max], [League_Skill_Group], [League_Next_Skill], [League_Get_Skill_Level], [League_Skill_Effect_Type], [League_Skill_Apply_Type], [League_Skill_Apply_Value], [League_Skill_Need_Point], [League_Skill_Need_Gold] from tb_League_Skill")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_LEAGUE_SKILL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_LEAGUE_SKILL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Level_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Next_Skill, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Get_Skill_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Effect_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Apply_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Apply_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Need_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.League_Skill_Need_Gold, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_LEAGUE_SKILL(row.League_Skill_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
