// 中文说明：
// 1. 本文件承接 TB_CHANGE_MOB_ID 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CHANGE_MOB_ID {
    unsigned int MOB_ID_Difficulty01 = 0;
    unsigned int MOB_ID_Difficulty02 = 0;
    unsigned int MOB_ID_Difficulty03 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CHANGE_MOB_ID) == 0xC, "TB_CHANGE_MOB_ID size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CHANGE_MOB_ID* GetTB_CHANGE_MOB_ID(unsigned int index) ;
    void SetTB_CHANGE_MOB_ID(unsigned int index, const TB_CHANGE_MOB_ID& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBChangeMobIdDB() ;
    std::map<unsigned int, TB_CHANGE_MOB_ID> m_mapTB_CHANGE_MOB_ID;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CHANGE_MOB_ID* XResourceMgr::GetTB_CHANGE_MOB_ID(unsigned int index) {
        const auto it = m_mapTB_CHANGE_MOB_ID.find(index);
        return it == m_mapTB_CHANGE_MOB_ID.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_CHANGE_MOB_ID(unsigned int index, const TB_CHANGE_MOB_ID& row) {
        m_mapTB_CHANGE_MOB_ID[index] = row;
    }

std::int64_t XResourceMgr::LoadTBChangeMobIdDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [MOB_ID_Difficulty01], [MOB_ID_Difficulty02], [MOB_ID_Difficulty03] from tb_Change_Mob_ID")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_CHANGE_MOB_ID.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CHANGE_MOB_ID row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.MOB_ID_Difficulty01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MOB_ID_Difficulty02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MOB_ID_Difficulty03, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CHANGE_MOB_ID(row.MOB_ID_Difficulty01, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
