// 中文说明：
// 1. 本文件承接 TB_MODE_BI_CLASS_STARTSKILL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_BI_CLASS_STARTSKILL {
    std::uint8_t ID = 0;
    unsigned int BI_StartSkill_01 = 0;
    unsigned int BI_StartSkill_02 = 0;
    unsigned int BI_StartSkill_03 = 0;
    unsigned int BI_StartSkill_04 = 0;
    unsigned int BI_StartSkill_05 = 0;
    unsigned int BI_StartSkill_06 = 0;
    unsigned int BI_StartSkill_07 = 0;
    unsigned int BI_StartSkill_08 = 0;
    unsigned int BI_StartSkill_09 = 0;
    unsigned int BI_StartSkill_10 = 0;
    unsigned int BI_StartSkill_11 = 0;
    unsigned int BI_StartSkill_12 = 0;
    unsigned int BI_StartSkill_13 = 0;
    unsigned int BI_StartSkill_14 = 0;
    unsigned int BI_StartSkill_15 = 0;
    unsigned int BI_StartSkill_Active = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_BI_CLASS_STARTSKILL) == 0x41, "TB_MODE_BI_CLASS_STARTSKILL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_CLASS_STARTSKILL* GetTB_MODE_BI_CLASS_STARTSKILL(std::uint8_t index) ;
    void SetTB_MODE_BI_CLASS_STARTSKILL(std::uint8_t index, const TB_MODE_BI_CLASS_STARTSKILL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBIClassStartSkillDB() ;
    std::map<std::uint8_t, TB_MODE_BI_CLASS_STARTSKILL> m_mapTB_MODE_BI_CLASS_STARTSKILL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_CLASS_STARTSKILL* XResourceMgr::GetTB_MODE_BI_CLASS_STARTSKILL(std::uint8_t index) {
        const auto it = m_mapTB_MODE_BI_CLASS_STARTSKILL.find(index);
        return it == m_mapTB_MODE_BI_CLASS_STARTSKILL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_CLASS_STARTSKILL(std::uint8_t index, const TB_MODE_BI_CLASS_STARTSKILL& row) {
        m_mapTB_MODE_BI_CLASS_STARTSKILL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBIClassStartSkillDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [BI_StartSkill_01], [BI_StartSkill_02], [BI_StartSkill_03], [BI_StartSkill_04], [BI_StartSkill_05], [BI_StartSkill_06], [BI_StartSkill_07], [BI_StartSkill_08], [BI_StartSkill_09], [BI_StartSkill_10], [BI_StartSkill_11], [BI_StartSkill_12], [BI_StartSkill_13], [BI_StartSkill_14], [BI_StartSkill_15], [BI_StartSkill_Active] from tb_Mode_BI_Class_StartSkill")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_CLASS_STARTSKILL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_CLASS_STARTSKILL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_11, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_12, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_13, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_14, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_15, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_StartSkill_Active, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_BI_CLASS_STARTSKILL(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
