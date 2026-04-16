// 中文说明：
// 1. 本文件承接 TB_CUSTOMER_GRADE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CUSTOMER_GRADE {
    std::uint16_t Index = 0;
    std::uint16_t Npc_Group_ID = 0;
    std::uint8_t Customer_Grade = 0;
    std::uint16_t Need_Credit = 0;
    std::uint16_t Credit_Accrue = 0;
    std::uint8_t Day_Credit_Limit = 0;
    unsigned int Reward_Item = 0;
    std::uint8_t Reward_Item_Count = 0;
    unsigned int Benefit_ID_01 = 0;
    unsigned int Benefit_ID_02 = 0;
    unsigned int Benefit_ID_03 = 0;
    unsigned int Benefit_ID_04 = 0;
    unsigned int Benefit_ID_05 = 0;
    unsigned int Benefit_ID_06 = 0;

    std::array<unsigned int, 6> GetBenefitIDs() const {
        return {Benefit_ID_01, Benefit_ID_02, Benefit_ID_03, Benefit_ID_04, Benefit_ID_05, Benefit_ID_06};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_CUSTOMER_GRADE) == 0x27, "TB_CUSTOMER_GRADE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CUSTOMER_GRADE* GetTB_CUSTOMER_GRADE(std::uint16_t index) ;
    void SetTB_CUSTOMER_GRADE(std::uint16_t index, const TB_CUSTOMER_GRADE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCustomerGradeDB() ;
    std::unordered_map<std::uint16_t, TB_CUSTOMER_GRADE> customerGradeRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CUSTOMER_GRADE* XResourceMgr::GetTB_CUSTOMER_GRADE(std::uint16_t index) {
        auto it = customerGradeRows_.find(index);
        if (it == customerGradeRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CUSTOMER_GRADE(std::uint16_t index, const TB_CUSTOMER_GRADE& row) {
        customerGradeRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCustomerGradeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Index], [Npc_Group_ID], [Customer_Grade], [Need_Credit], [Credit_Accrue], [Day_Credit_Limit], [Reward_Item], [Reward_Item_Count], [Benefit_ID_01], [Benefit_ID_02], [Benefit_ID_03], [Benefit_ID_04], [Benefit_ID_05], [Benefit_ID_06] from tb_Customer_Grade")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        customerGradeRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CUSTOMER_GRADE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Npc_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Customer_Grade, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Credit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Credit_Accrue, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Day_Credit_Limit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Benefit_ID_06, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CUSTOMER_GRADE(row.Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
