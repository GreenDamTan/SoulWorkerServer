// 中文说明：
// 1. 本文件承接 TB_MODE_BI_SKILL_EDIT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_MODE_BI_SKILL_EDIT {
    unsigned int ID = 0;
    unsigned int BI_Skill_Cooltime = 0;
};
static_assert(sizeof(TB_MODE_BI_SKILL_EDIT) == 0x8, "TB_MODE_BI_SKILL_EDIT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_SKILL_EDIT* GetTB_MODE_BI_SKILL_EDIT(unsigned int index) ;
    void SetTB_MODE_BI_SKILL_EDIT(unsigned int index, const TB_MODE_BI_SKILL_EDIT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBISkillEditDB() ;
    std::map<unsigned int, TB_MODE_BI_SKILL_EDIT> m_mapTB_MODE_BI_SKILL_EDIT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_SKILL_EDIT* XResourceMgr::GetTB_MODE_BI_SKILL_EDIT(unsigned int index) {
        const auto it = m_mapTB_MODE_BI_SKILL_EDIT.find(index);
        return it == m_mapTB_MODE_BI_SKILL_EDIT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_SKILL_EDIT(unsigned int index, const TB_MODE_BI_SKILL_EDIT& row) {
        m_mapTB_MODE_BI_SKILL_EDIT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBISkillEditDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [BI_Skill_Cooltime] from tb_Mode_BI_Skill_Edit")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_SKILL_EDIT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_SKILL_EDIT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Skill_Cooltime, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_BI_SKILL_EDIT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
