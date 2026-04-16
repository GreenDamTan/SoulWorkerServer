// 中文说明：
// 1. 本文件承接 TB_REINFORCE_OPTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_REINFORCE_OPTION {
    unsigned int Reinforce_Option_ID = 0;
    std::uint8_t Reinforce_Count = 0;
    std::uint8_t Phase_R01 = 0;
    std::uint8_t Phase_R02 = 0;
    std::uint8_t Phase_R03 = 0;
    std::uint8_t Phase_R04 = 0;
    std::uint8_t Phase_R05 = 0;
    std::uint8_t OptionClass_R01 = 0;
    std::uint8_t OptionClass_R02 = 0;
    std::uint8_t OptionClass_R03 = 0;
    std::uint8_t OptionClass_R04 = 0;
    std::uint8_t OptionClass_R05 = 0;
    std::uint16_t OptionID_R01 = 0;
    std::uint16_t OptionID_R02 = 0;
    std::uint16_t OptionID_R03 = 0;
    std::uint16_t OptionID_R04 = 0;
    std::uint16_t OptionID_R05 = 0;
    unsigned int OptionValue_R01 = 0;
    unsigned int OptionValue_R02 = 0;
    unsigned int OptionValue_R03 = 0;
    unsigned int OptionValue_R04 = 0;
    unsigned int OptionValue_R05 = 0;

    std::array<std::uint8_t, 5> GetPhases() const {
        return {Phase_R01, Phase_R02, Phase_R03, Phase_R04, Phase_R05};
    }

    std::array<std::uint8_t, 5> GetOptionClasses() const {
        return {OptionClass_R01, OptionClass_R02, OptionClass_R03, OptionClass_R04, OptionClass_R05};
    }

    std::array<std::uint16_t, 5> GetOptionIDs() const {
        return {OptionID_R01, OptionID_R02, OptionID_R03, OptionID_R04, OptionID_R05};
    }

    std::array<unsigned int, 5> GetOptionValues() const {
        return {OptionValue_R01, OptionValue_R02, OptionValue_R03, OptionValue_R04, OptionValue_R05};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_REINFORCE_OPTION) == 0x2D, "TB_REINFORCE_OPTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_REINFORCE_OPTION* GetTB_REINFORCE_OPTION(unsigned int index) ;
    void SetTB_REINFORCE_OPTION(unsigned int index, const TB_REINFORCE_OPTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBReinforceOptionDB() ;
    std::unordered_map<unsigned int, TB_REINFORCE_OPTION> reinforceOptionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_REINFORCE_OPTION* XResourceMgr::GetTB_REINFORCE_OPTION(unsigned int index) {
        auto it = reinforceOptionRows_.find(index);
        if (it == reinforceOptionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_REINFORCE_OPTION(unsigned int index, const TB_REINFORCE_OPTION& row) {
        reinforceOptionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBReinforceOptionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Reinforce_Option_ID], [Reinforce_Count], [Phase_R01], [OptionClass_R01], [OptionID_R01], [OptionValue_R01], [Phase_R02], [OptionClass_R02], [OptionID_R02], [OptionValue_R02], [Phase_R03], [OptionClass_R03], [OptionID_R03], [OptionValue_R03], [Phase_R04], [OptionClass_R04], [OptionID_R04], [OptionValue_R04], [Phase_R05], [OptionClass_R05], [OptionID_R05], [OptionValue_R05] from tb_Reinforce_Option")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        reinforceOptionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_REINFORCE_OPTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reinforce_Option_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reinforce_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            auto phases = std::array<std::uint8_t*, 5>{&row.Phase_R01,&row.Phase_R02,&row.Phase_R03,&row.Phase_R04,&row.Phase_R05};
            auto optionClasses = std::array<std::uint8_t*, 5>{&row.OptionClass_R01,&row.OptionClass_R02,&row.OptionClass_R03,&row.OptionClass_R04,&row.OptionClass_R05};
            auto optionIds = std::array<std::uint16_t*, 5>{&row.OptionID_R01,&row.OptionID_R02,&row.OptionID_R03,&row.OptionID_R04,&row.OptionID_R05};
            auto optionValues = std::array<unsigned int*, 5>{&row.OptionValue_R01,&row.OptionValue_R02,&row.OptionValue_R03,&row.OptionValue_R04,&row.OptionValue_R05};
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, phases[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, optionClasses[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, optionIds[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, optionValues[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_REINFORCE_OPTION(row.Reinforce_Option_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
