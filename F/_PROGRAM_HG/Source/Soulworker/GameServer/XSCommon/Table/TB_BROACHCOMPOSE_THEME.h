// 中文说明：
// 1. 本文件承接 TB_BROACHCOMPOSE_THEME 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BROACHCOMPOSE_THEME {
    std::uint16_t Broach_Theme_Value = 0;
    std::uint16_t uniBroach_Theme_Result[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_BROACHCOMPOSE_THEME) == 0xA, "TB_BROACHCOMPOSE_THEME size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BROACHCOMPOSE_THEME* GetTB_BROACHCOMPOSE_THEME(std::uint16_t index) ;
    void SetTB_BROACHCOMPOSE_THEME(std::uint16_t index, const TB_BROACHCOMPOSE_THEME& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBroachComposeThemeDB() ;
    std::map<std::uint16_t, TB_BROACHCOMPOSE_THEME> m_mapTB_BROACHCOMPOSE_THEME;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BROACHCOMPOSE_THEME* XResourceMgr::GetTB_BROACHCOMPOSE_THEME(std::uint16_t index) {
        const auto it = m_mapTB_BROACHCOMPOSE_THEME.find(index);
        return it == m_mapTB_BROACHCOMPOSE_THEME.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BROACHCOMPOSE_THEME(std::uint16_t index, const TB_BROACHCOMPOSE_THEME& row) {
        m_mapTB_BROACHCOMPOSE_THEME[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBroachComposeThemeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Broach_Theme_Value], [Result_Theme_01], [Result_Theme_02], [Result_Theme_03], [Result_Theme_04] from TB_BroachCompose_Theme")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BROACHCOMPOSE_THEME.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BROACHCOMPOSE_THEME row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_Theme_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Theme_Result[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Theme_Result[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Theme_Result[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Theme_Result[3], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BROACHCOMPOSE_THEME(row.Broach_Theme_Value, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
