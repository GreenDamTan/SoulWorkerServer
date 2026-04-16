// 中文说明：
// 1. 本文件承接 TB_WEEK_DAY 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WEEK_DAY {
    std::uint16_t daily_id = 0;
    std::uint8_t mission_group = 0;
    std::uint16_t unimission_id[11] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_WEEK_DAY) == 0x19, "TB_WEEK_DAY size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_WEEK_DAY* GetTB_WEEK_DAY(std::uint16_t index) ;
    void SetTB_WEEK_DAY(std::uint16_t index, const TB_WEEK_DAY& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWeekDayDB() ;
    std::map<std::uint16_t, TB_WEEK_DAY> m_mapTB_WEEK_DAY;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_WEEK_DAY* XResourceMgr::GetTB_WEEK_DAY(std::uint16_t index) {
        const auto it = m_mapTB_WEEK_DAY.find(index);
        return it == m_mapTB_WEEK_DAY.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_WEEK_DAY(std::uint16_t index, const TB_WEEK_DAY& row) {
        m_mapTB_WEEK_DAY[index] = row;
    }

std::int64_t XResourceMgr::LoadTBWeekDayDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [daily_id], [mission_group], [mission_id_1], [mission_id_2], [mission_id_3], [mission_id_4], [mission_id_5], [mission_id_6], [mission_id_7], [mission_id_8], [mission_id_9], [mission_id_10], [mission_id_11] from tb_Week_Day")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WEEK_DAY.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WEEK_DAY row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.daily_id, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.mission_group, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& missionId : row.unimission_id) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &missionId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_WEEK_DAY(row.daily_id, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
