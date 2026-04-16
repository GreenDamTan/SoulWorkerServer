// 中文说明：
// 1. 本文件承接 TB_RANK_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANK_INFO {
    std::uint16_t Ranking_Index = 0;
    std::uint8_t Ranking_Category = 0;
    std::uint8_t Ranking_Total_Type = 0;
    std::uint8_t Ranking_Total_Class_Type = 0;
    std::uint16_t Ranking_Visible = 0;
    std::uint8_t Ranking_Reward_Receive = 0;
    unsigned int Ranking_Reward = 0;
    unsigned int Ranking_Maze = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_RANK_INFO) == 0x10, "TB_RANK_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANK_INFO* GetTB_RANK_INFO(std::uint16_t index) ;
    void SetTB_RANK_INFO(std::uint16_t index, const TB_RANK_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRankInfoDB() ;
    std::map<std::uint16_t, TB_RANK_INFO> m_mapTB_RANK_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANK_INFO* XResourceMgr::GetTB_RANK_INFO(std::uint16_t index) {
        const auto it = m_mapTB_RANK_INFO.find(index);
        return it == m_mapTB_RANK_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANK_INFO(std::uint16_t index, const TB_RANK_INFO& row) {
        m_mapTB_RANK_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRankInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Ranking_Index], [Ranking_Category], [Ranking_Total_Type], [Ranking_Total_Class_Type], [Ranking_Visible], [Ranking_Reward_Receive], [Ranking_Reward], [Ranking_Maze] from tb_Rank_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANK_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANK_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Category, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Total_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Total_Class_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Visible, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Reward_Receive, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Reward, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Maze, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_RANK_INFO(row.Ranking_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
