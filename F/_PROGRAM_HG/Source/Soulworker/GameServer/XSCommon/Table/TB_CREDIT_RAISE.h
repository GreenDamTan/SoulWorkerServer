// 中文说明：
// 1. 本文件承接 TB_CREDIT_RAISE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CREDIT_RAISE {
    std::uint16_t Index = 0;
    std::uint16_t Npc_Group_ID = 0;
    std::uint8_t Credit_Raise_Type = 0;
    std::uint8_t Raise_Count_Min = 0;
    std::uint8_t Raise_Count_Max = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CREDIT_RAISE) == 0x7, "TB_CREDIT_RAISE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CREDIT_RAISE* GetTB_CREDIT_RAISE(std::uint16_t index) ;
    void SetTB_CREDIT_RAISE(std::uint16_t index, const TB_CREDIT_RAISE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCreditRaiseDB() ;
    std::unordered_map<std::uint16_t, TB_CREDIT_RAISE> creditRaiseRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CREDIT_RAISE* XResourceMgr::GetTB_CREDIT_RAISE(std::uint16_t index) {
        auto it = creditRaiseRows_.find(index);
        if (it == creditRaiseRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CREDIT_RAISE(std::uint16_t index, const TB_CREDIT_RAISE& row) {
        creditRaiseRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCreditRaiseDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Index], [Npc_Group_ID], [Credit_Raise_Type], [Raise_Count_Min], [Raise_Count_Max] from tb_Credit_Raise")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        creditRaiseRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CREDIT_RAISE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Npc_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Credit_Raise_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Raise_Count_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Raise_Count_Max, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CREDIT_RAISE(row.Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
