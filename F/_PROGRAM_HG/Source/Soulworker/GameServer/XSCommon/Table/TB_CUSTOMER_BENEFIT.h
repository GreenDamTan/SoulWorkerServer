// 中文说明：
// 1. 本文件承接 TB_CUSTOMER_BENEFIT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CUSTOMER_BENEFIT {
    unsigned int Benefit_ID = 0;
    std::uint8_t Benefit_Type = 0;
    std::uint16_t Benefit_Value_Rate = 0;
    std::uint16_t Benefit_Value_Int = 0;
    unsigned int Benefit_Item_ID_01 = 0;
    unsigned int Benefit_Item_ID_02 = 0;
    unsigned int Benefit_Item_ID_03 = 0;
    unsigned int Helper_Quest_ID = 0;

    std::array<unsigned int, 3> GetBenefitItemIDs() const {
        return {Benefit_Item_ID_01, Benefit_Item_ID_02, Benefit_Item_ID_03};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_CUSTOMER_BENEFIT) == 0x19, "TB_CUSTOMER_BENEFIT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CUSTOMER_BENEFIT* GetTB_CUSTOMER_BENEFIT(unsigned int index) ;
    void SetTB_CUSTOMER_BENEFIT(unsigned int index, const TB_CUSTOMER_BENEFIT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCustomerBenefitDB() ;
    std::unordered_map<unsigned int, TB_CUSTOMER_BENEFIT> customerBenefitRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CUSTOMER_BENEFIT* XResourceMgr::GetTB_CUSTOMER_BENEFIT(unsigned int index) {
        auto it = customerBenefitRows_.find(index);
        if (it == customerBenefitRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CUSTOMER_BENEFIT(unsigned int index, const TB_CUSTOMER_BENEFIT& row) {
        customerBenefitRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCustomerBenefitDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Benefit_ID], [Benefit_Type], [Benefit_Value_Rate], [Benefit_Value_Int], [Benefit_Item_ID_01], [Benefit_Item_ID_02], [Benefit_Item_ID_03], [Helper_Quest_ID] from tb_Customer_Benefit")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        customerBenefitRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CUSTOMER_BENEFIT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_Value_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_Value_Int, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_Item_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_Item_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_Item_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Helper_Quest_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CUSTOMER_BENEFIT(row.Benefit_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
