// 中文说明：
// 1. 本文件承接 TB_ROOMRANK_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ROOMRANK_REWARD {
    std::uint16_t ID = 0;
    std::uint16_t Min_Ranking = 0;
    std::uint16_t Max_Ranking = 0;
    unsigned int uniitem[2] = {};
    std::uint8_t uniamount[2] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ROOMRANK_REWARD) == 0x10, "TB_ROOMRANK_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ROOMRANK_REWARD* GetTB_ROOMRANK_REWARD(std::uint16_t index) ;
    void SetTB_ROOMRANK_REWARD(std::uint16_t index, const TB_ROOMRANK_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRoomRankRewardDB() ;
    std::map<std::uint16_t, TB_ROOMRANK_REWARD> m_mapTB_ROOMRANK_REWARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ROOMRANK_REWARD* XResourceMgr::GetTB_ROOMRANK_REWARD(std::uint16_t index) {
        const auto it = m_mapTB_ROOMRANK_REWARD.find(index);
        return it == m_mapTB_ROOMRANK_REWARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_ROOMRANK_REWARD(std::uint16_t index, const TB_ROOMRANK_REWARD& row) {
        m_mapTB_ROOMRANK_REWARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRoomRankRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Min_Ranking], [Max_Ranking], [Ranking_Reward_Item_01], [Item_Amount_01], [Ranking_Reward_Item_02], [Item_Amount_02] from tb_RoomRank_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_ROOMRANK_REWARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ROOMRANK_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Ranking, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Ranking, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniitem[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniamount[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniitem[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniamount[1], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ROOMRANK_REWARD(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
