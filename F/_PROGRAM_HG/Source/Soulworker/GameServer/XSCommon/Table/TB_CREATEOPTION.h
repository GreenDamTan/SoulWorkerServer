// 中文说明：
// 1. 本文件承接 TB_CREATEOPTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CREATEOPTION {
    std::uint16_t EffectType_Index = 0;
    std::uint8_t Option_Class = 0;
    std::uint8_t Invoke_Area = 0;
    std::uint8_t Invoke_Target = 0;
    std::uint8_t Invoke_Condition = 0;
    std::uint8_t Situation_Check = 0;
    int Aid_Value = 0;
    std::uint8_t Aid_Clm = 0;
    std::uint8_t Chance_Clm = 0;
    std::uint8_t Apply_Target = 0;
    std::uint8_t Refer_Status = 0;
    unsigned int StatusType = 0;
    std::uint8_t Value_Clm = 0;
    int Value_Clmcrt = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CREATEOPTION) == 0x18, "TB_CREATEOPTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CREATEOPTION* GetTB_CREATEOPTION(std::uint16_t index) ;
    void SetTB_CREATEOPTION(std::uint16_t index, const TB_CREATEOPTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCreateOptionDB() ;
    std::unordered_map<std::uint16_t, TB_CREATEOPTION> createOptionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CREATEOPTION* XResourceMgr::GetTB_CREATEOPTION(std::uint16_t index) {
        auto it = createOptionRows_.find(index);
        if (it == createOptionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CREATEOPTION(std::uint16_t index, const TB_CREATEOPTION& row) {
        createOptionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCreateOptionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [EffectType_Index], [Option_Class], [Invoke_Area], [Invoke_Target], [Invoke_Condition], [Situation_Check], [Aid_Value], [Aid_Clm], [Chance_Clm], [Apply_Target], [Refer_Status], [StatusType], [Value_Clm], [Value_Clmcrt] from tb_CreateOption")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        createOptionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CREATEOPTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.EffectType_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Invoke_Area, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Invoke_Target, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Invoke_Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Situation_Check, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Aid_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Aid_Clm, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Chance_Clm, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Apply_Target, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Refer_Status, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.StatusType, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Value_Clm, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Value_Clmcrt, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CREATEOPTION(row.EffectType_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
