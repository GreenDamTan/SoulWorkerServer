// 中文说明：
// 1. 本文件承接 TB_DAILY_MISSION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DAILY_MISSION {
    unsigned int Mission_ID = 0;
    std::uint8_t Mission_Type = 0;
    char Name[511] = {};
    unsigned int District_ID = 0;
    std::uint8_t Finish_Type = 0;
    std::uint8_t Target_Type = 0;
    unsigned int Target_ID = 0;
    std::uint8_t Target_Faction = 0;
    unsigned int Condition_Type = 0;
    unsigned int Condition_Value = 0;
    std::uint8_t Condition_Class = 0;
    unsigned int Condition_Title_01 = 0;
    unsigned int Condition_Title_02 = 0;
    std::uint8_t Condition_Count = 0;
    std::uint8_t Mission_Count = 0;
    char Date_Start[511] = {};
    char Date_End[511] = {};
    unsigned int Reward_Item_Id_01 = 0;
    unsigned int Reward_Item_Id_02 = 0;
    unsigned int Reward_Item_Id_03 = 0;
    unsigned int Reward_Item_Id_04 = 0;
    std::uint8_t Reward_Item_Count_01 = 0;
    std::uint8_t Reward_Item_Count_02 = 0;
    std::uint8_t Reward_Item_Count_03 = 0;
    std::uint8_t Reward_Item_Count_04 = 0;

    std::array<unsigned int, 4> GetRewardItemIDs() const {
        return {Reward_Item_Id_01, Reward_Item_Id_02, Reward_Item_Id_03, Reward_Item_Id_04};
    }

    std::array<std::uint8_t, 4> GetRewardItemCounts() const {
        return {Reward_Item_Count_01, Reward_Item_Count_02, Reward_Item_Count_03, Reward_Item_Count_04};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_DAILY_MISSION) == 0x634, "TB_DAILY_MISSION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DAILY_MISSION* GetTB_DAILY_MISSION(unsigned int index) ;
    void SetTB_DAILY_MISSION(unsigned int index, const TB_DAILY_MISSION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDailyMissionDB() ;
    std::unordered_map<unsigned int, TB_DAILY_MISSION> dailyMissionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DAILY_MISSION* XResourceMgr::GetTB_DAILY_MISSION(unsigned int index) {
        auto it = dailyMissionRows_.find(index);
        if (it == dailyMissionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DAILY_MISSION(unsigned int index, const TB_DAILY_MISSION& row) {
        dailyMissionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDailyMissionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Mission_ID], [Mission_Type], [Name], [District_ID], [Finish_Type], [Target_Type], [Target_ID], [Target_Faction], [Condition_Type], [Condition_Value], [Condition_Class], [Condition_Title_01], [Condition_Title_02], [Condition_Count], [Mission_Count], [Date_Start], [Date_End], [Reward_Item_Id_01], [Reward_Item_Count_01], [Reward_Item_Id_02], [Reward_Item_Count_02], [Reward_Item_Id_03], [Reward_Item_Count_03], [Reward_Item_Id_04], [Reward_Item_Count_04] from tb_Daily_Mission")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dailyMissionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DAILY_MISSION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Finish_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_Faction, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Title_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Title_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_Count, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Date_Start, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Date_End, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Id_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Id_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Id_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Id_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_04, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DAILY_MISSION(row.Mission_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
