// 中文说明：
// 1. 本文件承接 TB_SOUL_GUAGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SOUL_GUAGE {
    std::uint8_t SG_Index = 0;
    std::uint8_t SG_Regen_Type = 0;
    unsigned int Delay_Time = 0;
    std::uint8_t SG_Reset = 0;
    std::uint8_t Buff_Condition_Type = 0;
    std::uint16_t Buff_Condition = 0;
    std::uint16_t Get_Buff_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SOUL_GUAGE) == 0xC, "TB_SOUL_GUAGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SOUL_GUAGE* GetTB_SOUL_GUAGE(std::uint8_t index) ;
    void SetTB_SOUL_GUAGE(std::uint8_t index, const TB_SOUL_GUAGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSoulGuageDB() ;
    std::map<std::uint8_t, TB_SOUL_GUAGE> m_mapTB_SOUL_GUAGE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SOUL_GUAGE* XResourceMgr::GetTB_SOUL_GUAGE(std::uint8_t index) {
        const auto it = m_mapTB_SOUL_GUAGE.find(index);
        return it == m_mapTB_SOUL_GUAGE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SOUL_GUAGE(std::uint8_t index, const TB_SOUL_GUAGE& row) {
        m_mapTB_SOUL_GUAGE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSoulGuageDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [SG_Index], [SG_Regen_Type], [Delay_Time], [SG_Reset], [Buff_Condition_Type], [Buff_Condition], [Get_Buff_ID] from tb_Soul_Guage")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SOUL_GUAGE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SOUL_GUAGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SG_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SG_Regen_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Delay_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SG_Reset, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Condition_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Get_Buff_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SOUL_GUAGE(row.SG_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
