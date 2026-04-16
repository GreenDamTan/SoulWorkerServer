// 中文说明：
// 1. 本文件承接 TB_MODE_BI_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_BI_REWARD {
    std::uint8_t ID = 0;
    std::uint16_t BI_Reward_Coin_Value = 0;
    unsigned int BI_Reward_Item = 0;
    unsigned int BI_Reward_Item_Value = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_BI_REWARD) == 0xB, "TB_MODE_BI_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_REWARD* GetTB_MODE_BI_REWARD(std::uint8_t index) ;
    void SetTB_MODE_BI_REWARD(std::uint8_t index, const TB_MODE_BI_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBIRewardDB() ;
    std::map<std::uint8_t, TB_MODE_BI_REWARD> m_mapTB_MODE_BI_REWARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_REWARD* XResourceMgr::GetTB_MODE_BI_REWARD(std::uint8_t index) {
        const auto it = m_mapTB_MODE_BI_REWARD.find(index);
        return it == m_mapTB_MODE_BI_REWARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_REWARD(std::uint8_t index, const TB_MODE_BI_REWARD& row) {
        m_mapTB_MODE_BI_REWARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBIRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [BI_Reward_Coin_Value], [BI_Reward_Item], [BI_Reward_Item_Value] from tb_Mode_BI_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_REWARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Reward_Coin_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Reward_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Reward_Item_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_BI_REWARD(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
