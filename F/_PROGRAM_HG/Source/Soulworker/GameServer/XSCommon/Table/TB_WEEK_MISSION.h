// 中文说明：
// 1. 本文件承接 TB_WEEK_MISSION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WEEK_MISSION {
    std::uint16_t week_mission_id = 0;
    std::uint8_t Mission_Group_ID = 0;
    std::uint8_t mission_type = 0;
    std::uint16_t target_id = 0;
    std::uint8_t complete_value = 0;
    unsigned int uniReward_Item[3] = {};
    unsigned int uniReward_Count[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_WEEK_MISSION) == 0x1F, "TB_WEEK_MISSION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_WEEK_MISSION* GetTB_WEEK_MISSION(std::uint16_t index) ;
    void SetTB_WEEK_MISSION(std::uint16_t index, const TB_WEEK_MISSION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWeekMissionDB() ;
    std::map<std::uint16_t, TB_WEEK_MISSION> m_mapTB_WEEK_MISSION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_WEEK_MISSION* XResourceMgr::GetTB_WEEK_MISSION(std::uint16_t index) {
        const auto it = m_mapTB_WEEK_MISSION.find(index);
        return it == m_mapTB_WEEK_MISSION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_WEEK_MISSION(std::uint16_t index, const TB_WEEK_MISSION& row) {
        m_mapTB_WEEK_MISSION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBWeekMissionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [week_mission_id], [Mission_Group_ID], [mission_type], [target_id], [complete_value], [reward_item_id_1], [reward_count_1], [reward_item_id_2], [reward_count_2], [reward_item_id_3], [reward_count_3] from tb_Week_Mission")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WEEK_MISSION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WEEK_MISSION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.week_mission_id, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.mission_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.target_id, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.complete_value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 3; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Item[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniReward_Count[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_WEEK_MISSION(row.week_mission_id, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
