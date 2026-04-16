// 中文说明：
// 1. 本文件承接 TB_WORLD_EVENT_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WORLD_EVENT_REWARD {
    unsigned int world_reward_ID = 0;
    std::uint8_t world_reward_type = 0;
    std::uint16_t event_item_percentile_min = 0;
    std::uint16_t event_item_percentile_max = 0;
    std::uint8_t event_reward_type = 0;
    unsigned int event_reward_value = 0;
    std::uint16_t event_reward_item_amount = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_WORLD_EVENT_REWARD) == 0x10, "TB_WORLD_EVENT_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_WORLD_EVENT_REWARD* GetTB_WORLD_EVENT_REWARD(unsigned int index) ;
    void SetTB_WORLD_EVENT_REWARD(unsigned int index, const TB_WORLD_EVENT_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWorldEventRewardDB() ;
    std::map<unsigned int, TB_WORLD_EVENT_REWARD> m_mapTB_WORLD_EVENT_REWARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_WORLD_EVENT_REWARD* XResourceMgr::GetTB_WORLD_EVENT_REWARD(unsigned int index) {
        const auto it = m_mapTB_WORLD_EVENT_REWARD.find(index);
        return it == m_mapTB_WORLD_EVENT_REWARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_WORLD_EVENT_REWARD(unsigned int index, const TB_WORLD_EVENT_REWARD& row) {
        m_mapTB_WORLD_EVENT_REWARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBWorldEventRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [world_reward_ID], [world_reward_type], [event_item_percentile_min], [event_item_percentile_max], [event_reward_type], [event_reward_value], [event_reward_item_amount] from tb_world_event_reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WORLD_EVENT_REWARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WORLD_EVENT_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.world_reward_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.world_reward_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_item_percentile_min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_item_percentile_max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_reward_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_reward_value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_reward_item_amount, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_WORLD_EVENT_REWARD(row.world_reward_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
