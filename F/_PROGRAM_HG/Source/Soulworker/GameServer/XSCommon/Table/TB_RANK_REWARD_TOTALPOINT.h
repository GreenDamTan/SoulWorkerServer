// 中文说明：
// 1. 本文件承接 TB_RANK_REWARD_TOTALPOINT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANK_REWARD_TOTALPOINT {
    std::uint16_t Reward_Index = 0;
    std::uint8_t Ranking_Output_ID = 0;
    std::uint8_t Ranking_Reward_Division = 0;
    std::uint8_t Max_Ranking_Type = 0;
    std::uint16_t Max_Ranking = 0;
    unsigned int Ranking_Point_Value = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_RANK_REWARD_TOTALPOINT) == 0xB, "TB_RANK_REWARD_TOTALPOINT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANK_REWARD_TOTALPOINT* GetTB_RANK_REWARD_TOTALPOINT(std::uint16_t index) ;
    void SetTB_RANK_REWARD_TOTALPOINT(std::uint16_t index, const TB_RANK_REWARD_TOTALPOINT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRankRewardTotalPointDB() ;
    std::map<std::uint16_t, TB_RANK_REWARD_TOTALPOINT> m_mapTB_RANK_REWARD_TOTALPOINT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANK_REWARD_TOTALPOINT* XResourceMgr::GetTB_RANK_REWARD_TOTALPOINT(std::uint16_t index) {
        const auto it = m_mapTB_RANK_REWARD_TOTALPOINT.find(index);
        return it == m_mapTB_RANK_REWARD_TOTALPOINT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANK_REWARD_TOTALPOINT(std::uint16_t index, const TB_RANK_REWARD_TOTALPOINT& row) {
        m_mapTB_RANK_REWARD_TOTALPOINT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRankRewardTotalPointDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Reward_Index], [Ranking_Output_ID], [Ranking_Reward_Division], [Max_Ranking_Type], [Max_Ranking], [Ranking_Point_Value] from tb_Rank_Reward_Totalpoint")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANK_REWARD_TOTALPOINT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANK_REWARD_TOTALPOINT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Output_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Reward_Division, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Ranking_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Ranking, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Point_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_RANK_REWARD_TOTALPOINT(row.Reward_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
