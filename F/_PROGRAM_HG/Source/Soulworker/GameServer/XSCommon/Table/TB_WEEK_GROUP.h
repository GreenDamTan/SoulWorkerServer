// 中文说明：
// 1. 本文件承接 TB_WEEK_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WEEK_GROUP {
    std::uint8_t mission_group = 0;
    std::uint8_t Check_Mission_Type = 0;
    std::uint8_t Event_Type = 0;
    std::uint16_t Event_Start_Year = 0;
    std::uint8_t Event_Start_Month = 0;
    std::uint8_t Event_Start_Day = 0;
    std::uint16_t Event_End_Year = 0;
    std::uint8_t Event_End_Month = 0;
    std::uint8_t Event_End_Day = 0;
    std::uint8_t grop_count = 0;
    unsigned int reward_item_id = 0;
    std::uint16_t reward_count = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_WEEK_GROUP) == 0x12, "TB_WEEK_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    const std::map<std::uint8_t, TB_WEEK_GROUP>& GetTB_WEEK_GROUPRows() const ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWeekGroupDB() ;
    bool LoadWeekGroupFile(const std::filesystem::path& basePath) ;
    std::map<std::uint8_t, TB_WEEK_GROUP> m_mapTB_WEEK_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
const std::map<std::uint8_t, TB_WEEK_GROUP>& XResourceMgr::GetTB_WEEK_GROUPRows() const {
        return m_mapTB_WEEK_GROUP;
    }

std::int64_t XResourceMgr::LoadTBWeekGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [mission_group], [Check_Mission_Type], [Event_Type], [Event_Start_Year], [Event_Start_Month], [Event_Start_Day], [Event_End_Year], [Event_End_Month], [Event_End_Day], [grop_count], [reward_item_id], [reward_count] from tb_Week_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WEEK_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WEEK_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.mission_group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Mission_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Start_Year, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Start_Month, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Start_Day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_End_Year, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_End_Month, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_End_Day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.grop_count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.reward_item_id, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.reward_count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            m_mapTB_WEEK_GROUP[row.mission_group] = row;
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadWeekGroupFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Week_Group.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        unsigned int rowCount = 0;
        std::memcpy(&rowCount, buffer.data(), sizeof(rowCount));
        const std::size_t rowOffset = sizeof(rowCount);
        const std::size_t rowBytes = static_cast<std::size_t>(rowCount) * sizeof(TB_WEEK_GROUP);
        if (buffer.size() < rowOffset + rowBytes) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_WEEK_GROUP row{};
            std::memcpy(&row,
                        buffer.data() + rowOffset + static_cast<std::size_t>(index) * sizeof(TB_WEEK_GROUP),
                        sizeof(TB_WEEK_GROUP));
            m_mapTB_WEEK_GROUP[row.mission_group] = row;
        }
        return true;
    }

#endif
