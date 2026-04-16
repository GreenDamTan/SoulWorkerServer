// 中文说明：
// 1. 本文件承接 TB_AKASHIC_RANDOM_GROUP_IN 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AKASHIC_RANDOM_GROUP_IN {
    unsigned int Akashic_Group_ID = 0;
    unsigned int uniAkashic_Record_ID[10] = {};
    std::uint16_t unichance[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_AKASHIC_RANDOM_GROUP_IN) == 0x40, "TB_AKASHIC_RANDOM_GROUP_IN size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AKASHIC_RANDOM_GROUP_IN* GetTB_AKASHIC_RANDOM_GROUP_IN(unsigned int index) ;
    void SetTB_AKASHIC_RANDOM_GROUP_IN(unsigned int index, const TB_AKASHIC_RANDOM_GROUP_IN& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAkashicRandomGroupInDB() ;
    std::map<unsigned int, TB_AKASHIC_RANDOM_GROUP_IN> m_mapTB_AKASHIC_RANDOM_GROUP_IN;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AKASHIC_RANDOM_GROUP_IN* XResourceMgr::GetTB_AKASHIC_RANDOM_GROUP_IN(unsigned int index) {
        const auto it = m_mapTB_AKASHIC_RANDOM_GROUP_IN.find(index);
        return it == m_mapTB_AKASHIC_RANDOM_GROUP_IN.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AKASHIC_RANDOM_GROUP_IN(unsigned int index, const TB_AKASHIC_RANDOM_GROUP_IN& row) {
        m_mapTB_AKASHIC_RANDOM_GROUP_IN[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAkashicRandomGroupInDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Akashic_Group_ID], [Akashic_Record_ID_01], [Chance_01], [Akashic_Record_ID_02], [Chance_02], [Akashic_Record_ID_03], [Chance_03], [Akashic_Record_ID_04], [Chance_04], [Akashic_Record_ID_05], [Chance_05], [Akashic_Record_ID_06], [Chance_06], [Akashic_Record_ID_07], [Chance_07], [Akashic_Record_ID_08], [Chance_08], [Akashic_Record_ID_09], [Chance_09], [Akashic_Record_ID_10], [Chance_10] from tb_Akashic_Random_Group_In")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AKASHIC_RANDOM_GROUP_IN.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AKASHIC_RANDOM_GROUP_IN row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[8], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[8], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Record_ID[9], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unichance[9], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AKASHIC_RANDOM_GROUP_IN(row.Akashic_Group_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
