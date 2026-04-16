// 中文说明：
// 1. 本文件承接 TB_TITLE_STRING 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_TITLE_STRING {
    unsigned int Title_ID = 0;
    char Title_Name[511] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_TITLE_STRING) == 0x203, "TB_TITLE_STRING size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_TITLE_STRING* GetTB_TITLE_STRING(unsigned int index) ;
    void SetTB_TITLE_STRING(unsigned int index, const TB_TITLE_STRING& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBTitleStringDB() ;
    std::map<unsigned int, TB_TITLE_STRING> m_mapTB_TITLE_STRING;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_TITLE_STRING* XResourceMgr::GetTB_TITLE_STRING(unsigned int index) {
        const auto it = m_mapTB_TITLE_STRING.find(index);
        return it == m_mapTB_TITLE_STRING.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_TITLE_STRING(unsigned int index, const TB_TITLE_STRING& row) {
        m_mapTB_TITLE_STRING[index] = row;
    }

std::int64_t XResourceMgr::LoadTBTitleStringDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Title_ID], [Title_Name] from tb_Title_String")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_TITLE_STRING.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_TITLE_STRING row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Title_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Title_Name, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_TITLE_STRING(row.Title_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
