// 中文说明：
// 1. 本文件承接 TB_STATUS 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#ifndef GREENDAMTAN_TB_STATUS_DECLARED
#define GREENDAMTAN_TB_STATUS_DECLARED
#pragma pack(push, 1)
struct TB_STATUS {
    std::uint16_t ID = 0;
    float STR_First_Value = 0.0f;
    float STR_UP_Value = 0.0f;
    float AGI_First_Value = 0.0f;
    float AGI_UP_Value = 0.0f;
    float INT_First_Value = 0.0f;
    float INT_UP_Value = 0.0f;
    float BAL_First_Value = 0.0f;
    float BAL_UP_Value = 0.0f;
    float VIT_First_Value = 0.0f;
    float VIT_UP_Value = 0.0f;
    float LUC_First_Value = 0.0f;
    float LUC_UP_Value = 0.0f;
    float Con_HP_Grow = 0.0f;
    float Con_ST_Reg = 0.0f;
    float Con_SG_Reg = 0.0f;
    float Con_MSR = 0.0f;
    float Con_ASR = 0.0f;
    float Con_PATK_Max = 0.0f;
    float Con_MATK_Max = 0.0f;
    float Con_PDEF = 0.0f;
    float Con_MDEF = 0.0f;
    float PDPR_First_Value = 0.0f;
    float MDPR_First_Value = 0.0f;
    float Con_PAR = 0.0f;
    float Con_MAR = 0.0f;
    float Con_PARP = 0.0f;
    float Con_MARP = 0.0f;
    float Con_PCP = 0.0f;
    float Con_MCP = 0.0f;
    float Con_PCRP = 0.0f;
    float Con_MCRP = 0.0f;
    float Con_PCA = 0.0f;
    float Con_MCA = 0.0f;
    float Con_PDSR = 0.0f;
    float Con_MDSR = 0.0f;
    std::uint8_t Res_Burn_Rate = 0;
    std::uint8_t Res_Poision_Rate = 0;
    std::uint8_t Res_Shcok_Rate = 0;
    std::uint8_t Res_Bleed_Rate = 0;
    std::uint8_t Res_Stun_Rate = 0;
    std::uint8_t Res_Paralysis_Rate = 0;
    std::uint8_t Res_Sleep_Rate = 0;
    std::uint8_t Res_Freeze_Rate = 0;
    std::uint8_t Res_Charm_Rate = 0;
    std::uint8_t Res_Confusion_Rate = 0;
    std::uint8_t Res_Silence_Rate = 0;
    std::uint8_t Res_Weak_Rate = 0;
    std::uint16_t SG_First_Value = 0;
    float Con_PvP_Reduce = 0.0f;
    float Con_PVP_PDPR_Rate = 0.0f;
    float Con_SADamage_Value = 0.0f;
    float Con_SG_Absorb_Rate = 0.0f;
    float Con_HP_First_Value = 0.0f;
    float Con_ST_First_Value = 0.0f;
    float Con_PAR_First_Value = 0.0f;
    float Con_MAR_First_Value = 0.0f;
    float Con_ADR_Value = 0.0f;
    float Check_Stat_SA = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_STATUS) == 0xC4, "TB_STATUS size must match PDB");
#endif
#endif

#ifndef GREENDAMTAN_FIRST_STATUS_TABLE_DECLARED
#define GREENDAMTAN_FIRST_STATUS_TABLE_DECLARED
struct FIRST_STATUS_TABLE {
    float fSTR = 0.0f;
    float fAGI = 0.0f;
    float fBAL = 0.0f;
    float fINT = 0.0f;
    float fVIT = 0.0f;
    float fLUC = 0.0f;
};
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_STATUS* GetTB_STATUS(std::uint16_t index) ;
    void SetTB_STATUS(std::uint16_t index, const TB_STATUS& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBStatusDB() ;
    std::map<std::uint16_t, TB_STATUS> m_mapTB_STATUS;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_STATUS* XResourceMgr::GetTB_STATUS(std::uint16_t index) {
        const auto it = m_mapTB_STATUS.find(index);
        return it == m_mapTB_STATUS.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_STATUS(std::uint16_t index, const TB_STATUS& row) {
        m_mapTB_STATUS[index] = row;
    }

std::int64_t XResourceMgr::LoadTBStatusDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [STR_First_Value], [STR_UP_Value], [AGI_First_Value], [AGI_UP_Value], [INT_First_Value], [INT_UP_Value], [BAL_First_Value], [BAL_UP_Value], [VIT_First_Value], [VIT_UP_Value], [LUC_First_Value], [LUC_UP_Value], [Con_HP_Grow], [Con_ST_Reg], [Con_SG_Reg], [Con_MSR], [Con_ASR], [Con_PATK_Max], [Con_MATK_Max], [Con_PDEF], [Con_MDEF], [PDPR_First_Value], [MDPR_First_Value], [Con_PAR], [Con_MAR], [Con_PARP], [Con_MARP], [Con_PCP], [Con_MCP], [Con_PCRP], [Con_MCRP], [Con_PCA], [Con_MCA], [Con_PDSR], [Con_MDSR], [Res_Burn_Rate], [Res_Poision_Rate], [Res_Shcok_Rate], [Res_Bleed_Rate], [Res_Stun_Rate], [Res_Paralysis_Rate], [Res_Sleep_Rate], [Res_Freeze_Rate], [Res_Charm_Rate], [Res_Confusion_Rate], [Res_Silence_Rate], [Res_Weak_Rate], [SG_First_Value], [Con_PvP_Reduce], [Con_PVP_PDPR_Rate], [Con_SADamage_Value], [Con_SG_Absorb_Rate], [Con_HP_First_Value], [Con_ST_First_Value], [Con_PAR_First_Value], [Con_MAR_First_Value], [Con_ADR_Value], [Check_Stat_SA] from tb_Status")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_STATUS.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_STATUS row{};
            const auto loadPackedFloat = [&](std::size_t offset) {
                float value = 0.0f;
                if (!GreenDamTan_DBGetFloat(xDBBinder, &value, &executeResult)) {
                    return false;
                }
                std::memcpy(reinterpret_cast<char*>(&row) + offset, &value, sizeof(value));
                return true;
            };
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !loadPackedFloat(offsetof(TB_STATUS, STR_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, STR_UP_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, AGI_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, AGI_UP_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, INT_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, INT_UP_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, BAL_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, BAL_UP_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, VIT_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, VIT_UP_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, LUC_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, LUC_UP_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_HP_Grow)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_ST_Reg)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_SG_Reg)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MSR)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_ASR)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PATK_Max)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MATK_Max)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PDEF)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MDEF)) ||
                !loadPackedFloat(offsetof(TB_STATUS, PDPR_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, MDPR_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PAR)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MAR)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PARP)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MARP)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PCP)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MCP)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PCRP)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MCRP)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PCA)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MCA)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PDSR)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MDSR)) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Burn_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Poision_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Shcok_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Bleed_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Stun_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Paralysis_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Sleep_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Freeze_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Charm_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Confusion_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Silence_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Weak_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SG_First_Value, &executeResult) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PvP_Reduce)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PVP_PDPR_Rate)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_SADamage_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_SG_Absorb_Rate)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_HP_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_ST_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_PAR_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_MAR_First_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Con_ADR_Value)) ||
                !loadPackedFloat(offsetof(TB_STATUS, Check_Stat_SA))) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_STATUS(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
