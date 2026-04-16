// 中文说明：
// 1. 本文件承接 TB_BROACHCOMPOSE_RANK 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BROACHCOMPOSE_RANK {
    std::uint16_t Compose_Value = 0;
    std::uint16_t uniBroach_Rank_Rate[5] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_BROACHCOMPOSE_RANK) == 0xC, "TB_BROACHCOMPOSE_RANK size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BROACHCOMPOSE_RANK* GetTB_BROACHCOMPOSE_RANK(std::uint16_t index) ;
    void SetTB_BROACHCOMPOSE_RANK(std::uint16_t index, const TB_BROACHCOMPOSE_RANK& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBroachComposeRankDB() ;
    std::map<std::uint16_t, TB_BROACHCOMPOSE_RANK> m_mapTB_BROACHCOMPOSE_RANK;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BROACHCOMPOSE_RANK* XResourceMgr::GetTB_BROACHCOMPOSE_RANK(std::uint16_t index) {
        const auto it = m_mapTB_BROACHCOMPOSE_RANK.find(index);
        return it == m_mapTB_BROACHCOMPOSE_RANK.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BROACHCOMPOSE_RANK(std::uint16_t index, const TB_BROACHCOMPOSE_RANK& row) {
        m_mapTB_BROACHCOMPOSE_RANK[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBroachComposeRankDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Compose_Value], [Bit_Broach_Rate], [Kilo_Broach_Rate], [Mega_Broach_Rate], [Giga_Broach_Rate], [Tera_Broach_Rate] from TB_BroachCompose_Rank")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BROACHCOMPOSE_RANK.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BROACHCOMPOSE_RANK row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Compose_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Rank_Rate[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Rank_Rate[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Rank_Rate[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Rank_Rate[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBroach_Rank_Rate[4], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BROACHCOMPOSE_RANK(row.Compose_Value, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
