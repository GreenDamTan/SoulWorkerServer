// 中文说明：
// 1. 本文件承接 TB_RANK_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANK_REWARD {
    unsigned int Reward_Index = 0;
    std::uint16_t Ranking_Output_ID = 0;
    unsigned int Ranking_Reward_Division = 0;
    std::uint8_t Max_Ranking_Type = 0;
    std::uint16_t Max_Ranking = 0;
    unsigned int uniitem[2] = {};
    std::uint16_t uniamount[2] = {};
    unsigned int uniClassReward[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_RANK_REWARD) == 0x39, "TB_RANK_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANK_REWARD* GetTB_RANK_REWARD(unsigned int index) ;
    void SetTB_RANK_REWARD(unsigned int index, const TB_RANK_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRankRewardDB() ;
    std::map<unsigned int, TB_RANK_REWARD> m_mapTB_RANK_REWARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANK_REWARD* XResourceMgr::GetTB_RANK_REWARD(unsigned int index) {
        const auto it = m_mapTB_RANK_REWARD.find(index);
        return it == m_mapTB_RANK_REWARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANK_REWARD(unsigned int index, const TB_RANK_REWARD& row) {
        m_mapTB_RANK_REWARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRankRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Reward_Index], [Ranking_Output_ID], [Ranking_Reward_Division], [Max_Ranking_Type], [Max_Ranking], [Ranking_Reward_Item_01], [Item_Amount_01], [Ranking_Reward_Item_02], [Item_Amount_02], [Ranking_Class_Reward_Item_01], [Ranking_Class_Reward_Item_02], [Ranking_Class_Reward_Item_03], [Ranking_Class_Reward_Item_04], [Ranking_Class_Reward_Item_05], [Ranking_Class_Reward_Item_06], [Ranking_Class_Reward_Item_07], [Ranking_Class_Reward_Item_08] from tb_Rank_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANK_REWARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANK_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Output_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Reward_Division, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Ranking_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Ranking, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniitem[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniamount[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniitem[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniamount[1], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& classReward : row.uniClassReward) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &classReward, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_RANK_REWARD(row.Reward_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
