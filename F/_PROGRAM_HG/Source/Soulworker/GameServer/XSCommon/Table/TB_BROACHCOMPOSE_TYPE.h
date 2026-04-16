// 中文说明：
// 1. 本文件承接 TB_BROACHCOMPOSE_TYPE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BROACHCOMPOSE_TYPE {
    std::uint16_t Broach_Rank_Value = 0;
    std::uint16_t uniBroach_Type_Result[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_BROACHCOMPOSE_TYPE) == 0x8, "TB_BROACHCOMPOSE_TYPE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BROACHCOMPOSE_TYPE* GetTB_BROACHCOMPOSE_TYPE(std::uint16_t index) ;
    void SetTB_BROACHCOMPOSE_TYPE(std::uint16_t index, const TB_BROACHCOMPOSE_TYPE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBroachComposeTypeDB() ;
    std::map<std::uint16_t, TB_BROACHCOMPOSE_TYPE> m_mapTB_BROACHCOMPOSE_TYPE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BROACHCOMPOSE_TYPE* XResourceMgr::GetTB_BROACHCOMPOSE_TYPE(std::uint16_t index) {
        const auto it = m_mapTB_BROACHCOMPOSE_TYPE.find(index);
        return it == m_mapTB_BROACHCOMPOSE_TYPE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BROACHCOMPOSE_TYPE(std::uint16_t index, const TB_BROACHCOMPOSE_TYPE& row) {
        m_mapTB_BROACHCOMPOSE_TYPE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBroachComposeTypeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Broach_Rank_Value], [Result_Type_Attack], [Result_Type_Defence], [Result_Type_Function] from TB_BroachCompose_Type")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BROACHCOMPOSE_TYPE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BROACHCOMPOSE_TYPE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_Rank_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Type_Result[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Type_Result[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Type_Result[2], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BROACHCOMPOSE_TYPE(row.Broach_Rank_Value, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
