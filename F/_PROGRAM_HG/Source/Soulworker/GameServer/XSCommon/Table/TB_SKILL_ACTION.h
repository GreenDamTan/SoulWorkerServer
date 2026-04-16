// 中文说明：
// 1. 本文件承接 TB_SKILL_ACTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SKILL_ACTION {
    std::uint16_t Class_Index = 0;
    unsigned int Skill_ID[9] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_SKILL_ACTION) == 0x26, "TB_SKILL_ACTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SKILL_ACTION* GetTB_SKILL_ACTION(std::uint16_t index) ;
    void SetTB_SKILL_ACTION(std::uint16_t index, const TB_SKILL_ACTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSkillActionDB() ;
    std::map<std::uint16_t, TB_SKILL_ACTION> m_mapTB_SKILL_ACTION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SKILL_ACTION* XResourceMgr::GetTB_SKILL_ACTION(std::uint16_t index) {
        const auto it = m_mapTB_SKILL_ACTION.find(index);
        return it == m_mapTB_SKILL_ACTION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SKILL_ACTION(std::uint16_t index, const TB_SKILL_ACTION& row) {
        m_mapTB_SKILL_ACTION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSkillActionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Class_Index], [Skill_ID_01], [Skill_ID_02], [Skill_ID_03], [Skill_ID_04], [Skill_ID_05], [Skill_ID_06], [Skill_ID_07], [Skill_ID_08], [Skill_ID_09] from tb_Skill_Action")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SKILL_ACTION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SKILL_ACTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Class_Index, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& skillId : row.Skill_ID) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &skillId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_SKILL_ACTION(row.Class_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
