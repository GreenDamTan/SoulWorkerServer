// 中文说明：
// 1. 本文件承接 TB_HELPER_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_HELPER_REWARD {
    std::uint8_t ID = 0;
    std::uint8_t Reward_Type = 0;
    std::uint16_t Reward_FriendPoint_Value = 0;
    unsigned int uniReward_Item[3] = {};
    std::uint8_t uniReward_Count[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_HELPER_REWARD) == 0x13, "TB_HELPER_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_HELPER_REWARD* GetTB_HELPER_REWARD(std::uint8_t index) ;
    void SetTB_HELPER_REWARD(std::uint8_t index, const TB_HELPER_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBHelperRewardDB() ;
    std::unordered_map<std::uint8_t, TB_HELPER_REWARD> helperRewardRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_HELPER_REWARD* XResourceMgr::GetTB_HELPER_REWARD(std::uint8_t index) {
        auto it = helperRewardRows_.find(index);
        if (it == helperRewardRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_HELPER_REWARD(std::uint8_t index, const TB_HELPER_REWARD& row) {
        helperRewardRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBHelperRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Reward_Type], [Reward_FriendPoint_Value], [Reward_Item_ID_01], [Reward_Item_Count_01], [Reward_Item_ID_02], [Reward_Item_Count_02], [Reward_Item_ID_03], [Reward_Item_Count_03] from tb_Helper_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        helperRewardRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_HELPER_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_FriendPoint_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Item[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Count[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Item[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Count[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Item[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Count[2], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_HELPER_REWARD(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
