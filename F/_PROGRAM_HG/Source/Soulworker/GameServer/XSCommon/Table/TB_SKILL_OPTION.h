// 中文说明：
// 1. 本文件承接 TB_SKILL_OPTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SKILL_OPTION {
    unsigned int Skill_Option_ID = 0;
    unsigned int Skill_Group_ID = 0;
    std::int16_t Skill_Option_Flag = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SKILL_OPTION) == 0xA, "TB_SKILL_OPTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SKILL_OPTION* GetTB_SKILL_OPTION(unsigned int index) ;
    void SetTB_SKILL_OPTION(unsigned int index, const TB_SKILL_OPTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSkillOptionDB() ;
    std::map<unsigned int, TB_SKILL_OPTION> m_mapTB_SKILL_OPTION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SKILL_OPTION* XResourceMgr::GetTB_SKILL_OPTION(unsigned int index) {
        const auto it = m_mapTB_SKILL_OPTION.find(index);
        return it == m_mapTB_SKILL_OPTION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SKILL_OPTION(unsigned int index, const TB_SKILL_OPTION& row) {
        m_mapTB_SKILL_OPTION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSkillOptionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Skill_Option_ID], [Skill_Group_ID], [Skill_Option_Flag] from tb_Skill_Option")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SKILL_OPTION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SKILL_OPTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Option_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Skill_Option_Flag, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SKILL_OPTION(row.Skill_Option_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
