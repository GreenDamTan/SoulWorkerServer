// 中文说明：
// 1. 本文件承接 TB_MODE_BI_CLASS_CORRECTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_BI_CLASS_CORRECTION {
    std::uint8_t ID = 0;
    float BI_Correction_MaxHP = 0.0f;
    float BI_Correction_Attack = 0.0f;
    float BI_Correction_Critical_Rate = 0.0f;
    float BI_Correction_Speed_Attack = 0.0f;
    float BI_Correction_Accurate = 0.0f;
    float BI_Correction_Penetrate = 0.0f;
    float BI_Correction_MaxStamina = 0.0f;
    float BI_Correction_Speed_Move = 0.0f;
    float BI_Correction_Armor = 0.0f;
    float BI_Correction_Avoid = 0.0f;
    float BI_Correction_Cooldown = 0.0f;
    float BI_Correction_Staminaspeed = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_BI_CLASS_CORRECTION) == 0x31, "TB_MODE_BI_CLASS_CORRECTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_CLASS_CORRECTION* GetTB_MODE_BI_CLASS_CORRECTION(std::uint8_t index) ;
    void SetTB_MODE_BI_CLASS_CORRECTION(std::uint8_t index, const TB_MODE_BI_CLASS_CORRECTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBIClassCorrectionDB() ;
    std::map<std::uint8_t, TB_MODE_BI_CLASS_CORRECTION> m_mapTB_MODE_BI_CLASS_CORRECTION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_CLASS_CORRECTION* XResourceMgr::GetTB_MODE_BI_CLASS_CORRECTION(std::uint8_t index) {
        const auto it = m_mapTB_MODE_BI_CLASS_CORRECTION.find(index);
        return it == m_mapTB_MODE_BI_CLASS_CORRECTION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_CLASS_CORRECTION(std::uint8_t index, const TB_MODE_BI_CLASS_CORRECTION& row) {
        m_mapTB_MODE_BI_CLASS_CORRECTION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBIClassCorrectionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [BI_Correction_MaxHP], [BI_Correction_Attack], [BI_Correction_Critical_Rate], [BI_Correction_Speed_Attack], [BI_Correction_Accurate], [BI_Correction_Penetrate], [BI_Correction_MaxStamina], [BI_Correction_Speed_Move], [BI_Correction_Armor], [BI_Correction_Avoid], [BI_Correction_Cooldown], [BI_Correction_Staminaspeed] from tb_Mode_BI_Class_Correction")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_CLASS_CORRECTION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_CLASS_CORRECTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_MaxHP, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Attack, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Critical_Rate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Speed_Attack, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Accurate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Penetrate, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_MaxStamina, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Speed_Move, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Armor, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Avoid, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Cooldown, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.BI_Correction_Staminaspeed, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_BI_CLASS_CORRECTION(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
