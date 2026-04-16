// 中文说明：
// 1. 本文件承接 TB_POLLEN 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 `Pollen_Effect_*` 字段改写成推测脚本对象，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_POLLEN {
    unsigned int Pollen_Index = 0;
    unsigned int Pollen_Extend_Item_ID = 0;
    unsigned int Pollen_Effect_Type = 0;
    unsigned int Pollen_Effect_Value = 0;
    unsigned int Pollen_Help_Ani_Time = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_POLLEN) == 0x14, "TB_POLLEN size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_POLLEN* GetTB_POLLEN(unsigned int index) ;
    void SetTB_POLLEN(unsigned int index, const TB_POLLEN& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPollenDB() ;
    std::map<unsigned int, TB_POLLEN> m_mapTB_POLLEN;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_POLLEN* XResourceMgr::GetTB_POLLEN(unsigned int index) {
        const auto it = m_mapTB_POLLEN.find(index);
        return it == m_mapTB_POLLEN.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_POLLEN(unsigned int index, const TB_POLLEN& row) {
        m_mapTB_POLLEN[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPollenDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Pollen_Index], [Pollen_Extend_Item_ID], [Pollen_Effect_Type], [Pollen_Effect_Value], [Pollen_Help_Ani_Time] from tb_Pollen")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_POLLEN.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_POLLEN row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Pollen_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Pollen_Extend_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Pollen_Effect_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Pollen_Effect_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Pollen_Help_Ani_Time, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_POLLEN(row.Pollen_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
