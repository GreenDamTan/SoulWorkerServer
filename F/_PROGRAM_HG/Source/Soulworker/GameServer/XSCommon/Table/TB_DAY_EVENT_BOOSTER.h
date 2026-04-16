// 中文说明：
// 1. 本文件承接 TB_DAY_EVENT_BOOSTER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DAY_EVENT_BOOSTER {
    std::uint16_t ID = 0;
    std::uint16_t Booster_ID_01 = 0;
    std::uint16_t Booster_ID_02 = 0;
    std::uint16_t Booster_ID_03 = 0;
    std::uint16_t Booster_ID_04 = 0;
    std::uint16_t Booster_ID_05 = 0;
    std::uint16_t Booster_ID_06 = 0;
    std::uint16_t Booster_ID_07 = 0;
    std::uint16_t Booster_ID_08 = 0;
    std::uint16_t Booster_ID_09 = 0;
    std::uint16_t Booster_ID_10 = 0;
    std::uint16_t Booster_Rate_01 = 0;
    std::uint16_t Booster_Rate_02 = 0;
    std::uint16_t Booster_Rate_03 = 0;
    std::uint16_t Booster_Rate_04 = 0;
    std::uint16_t Booster_Rate_05 = 0;
    std::uint16_t Booster_Rate_06 = 0;
    std::uint16_t Booster_Rate_07 = 0;
    std::uint16_t Booster_Rate_08 = 0;
    std::uint16_t Booster_Rate_09 = 0;
    std::uint16_t Booster_Rate_10 = 0;

    std::array<std::uint16_t, 10> GetBoosterIDs() const {
        return {Booster_ID_01, Booster_ID_02, Booster_ID_03, Booster_ID_04, Booster_ID_05,
                Booster_ID_06, Booster_ID_07, Booster_ID_08, Booster_ID_09, Booster_ID_10};
    }

    std::array<std::uint16_t, 10> GetBoosterRates() const {
        return {Booster_Rate_01, Booster_Rate_02, Booster_Rate_03, Booster_Rate_04, Booster_Rate_05,
                Booster_Rate_06, Booster_Rate_07, Booster_Rate_08, Booster_Rate_09, Booster_Rate_10};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_DAY_EVENT_BOOSTER) == 0x2A, "TB_DAY_EVENT_BOOSTER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DAY_EVENT_BOOSTER* GetTB_DAY_EVENT_BOOSTER(std::uint16_t index) ;
    void SetTB_DAY_EVENT_BOOSTER(std::uint16_t index, const TB_DAY_EVENT_BOOSTER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDayEventBoosterDB() ;
    std::unordered_map<std::uint16_t, TB_DAY_EVENT_BOOSTER> dayEventBoosterRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DAY_EVENT_BOOSTER* XResourceMgr::GetTB_DAY_EVENT_BOOSTER(std::uint16_t index) {
        auto it = dayEventBoosterRows_.find(index);
        if (it == dayEventBoosterRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DAY_EVENT_BOOSTER(std::uint16_t index, const TB_DAY_EVENT_BOOSTER& row) {
        dayEventBoosterRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDayEventBoosterDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Booster_ID_01], [Booster_Rate_01], [Booster_ID_02], [Booster_Rate_02], [Booster_ID_03], [Booster_Rate_03], [Booster_ID_04], [Booster_Rate_04], [Booster_ID_05], [Booster_Rate_05], [Booster_ID_06], [Booster_Rate_06], [Booster_ID_07], [Booster_Rate_07], [Booster_ID_08], [Booster_Rate_08], [Booster_ID_09], [Booster_Rate_09], [Booster_ID_10], [Booster_Rate_10] from tb_Day_Event_Booster")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dayEventBoosterRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DAY_EVENT_BOOSTER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Rate_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DAY_EVENT_BOOSTER(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
