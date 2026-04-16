// 中文说明：
// 1. 本文件承接 TB_ITEM_ENDURANCE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_ENDURANCE {
    unsigned int Endurance_ID = 0;
    std::uint16_t WP_Attack_Rate = 0;
    float WP_Attack_Value = 0.0f;
    std::uint16_t WP_Death_Rate = 0;
    float WP_Death_Value = 0.0f;
    std::uint16_t SubWP_Attack_Rate = 0;
    float SubWP_Attack_Value = 0.0f;
    std::uint16_t SubWP_Death_Rate = 0;
    float SubWP_Death_Value = 0.0f;
    std::uint16_t Gear_Defence_Rate = 0;
    float Gear_Defence_Value = 0.0f;
    std::uint16_t Gear_Death_Rate = 0;
    float Gear_Death_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_ENDURANCE) == 0x28, "TB_ITEM_ENDURANCE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_ENDURANCE* GetTB_ITEM_ENDURANCE(unsigned int index) ;
    void SetTB_ITEM_ENDURANCE(unsigned int index, const TB_ITEM_ENDURANCE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemEnduranceDB() ;
    std::unordered_map<unsigned int, TB_ITEM_ENDURANCE> itemEnduranceRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_ENDURANCE* XResourceMgr::GetTB_ITEM_ENDURANCE(unsigned int index) {
        auto it = itemEnduranceRows_.find(index);
        if (it == itemEnduranceRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_ENDURANCE(unsigned int index, const TB_ITEM_ENDURANCE& row) {
        itemEnduranceRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemEnduranceDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Endurance_ID], [WP_Attack_Rate], [WP_Attack_Value], [WP_Death_Rate], [WP_Death_Value], [SubWP_Attack_Rate], [SubWP_Attack_Value], [SubWP_Death_Rate], [SubWP_Death_Value], [Gear_Defence_Rate], [Gear_Defence_Value], [Gear_Death_Rate], [Gear_Death_Value] from tb_Item_Endurance")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemEnduranceRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_ENDURANCE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Endurance_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.WP_Attack_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.WP_Attack_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.WP_Death_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.WP_Death_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubWP_Attack_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.SubWP_Attack_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubWP_Death_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.SubWP_Death_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gear_Defence_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_Defence_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gear_Death_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_Death_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ITEM_ENDURANCE(row.Endurance_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
