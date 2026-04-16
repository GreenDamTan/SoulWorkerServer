// 中文说明：
// 1. 本文件承接 TB_AKASHIC_COMBINATION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AKASHIC_COMBINATION {
    std::uint8_t Combination_Rank = 0;
    std::uint16_t Combination_Need_Point = 0;
    std::uint16_t Combination_Success_Rate = 0;
    unsigned int Success_Group = 0;
    unsigned int Combination_Need_Zeny = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_AKASHIC_COMBINATION) == 0xD, "TB_AKASHIC_COMBINATION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AKASHIC_COMBINATION* GetTB_AKASHIC_COMBINATION(std::uint8_t index) ;
    void SetTB_AKASHIC_COMBINATION(std::uint8_t index, const TB_AKASHIC_COMBINATION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAkashicCombinationDB() ;
    std::map<std::uint8_t, TB_AKASHIC_COMBINATION> m_mapTB_AKASHIC_COMBINATION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AKASHIC_COMBINATION* XResourceMgr::GetTB_AKASHIC_COMBINATION(std::uint8_t index) {
        const auto it = m_mapTB_AKASHIC_COMBINATION.find(index);
        return it == m_mapTB_AKASHIC_COMBINATION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AKASHIC_COMBINATION(std::uint8_t index, const TB_AKASHIC_COMBINATION& row) {
        m_mapTB_AKASHIC_COMBINATION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAkashicCombinationDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Combination_Rank], [Combination_Need_Point], [Combination_Success_Rate], [Success_Group], [Combination_Need_Zeny] from tb_Akashic_Combination")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AKASHIC_COMBINATION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AKASHIC_COMBINATION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combination_Rank, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combination_Need_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combination_Success_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Success_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combination_Need_Zeny, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AKASHIC_COMBINATION(row.Combination_Rank, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
