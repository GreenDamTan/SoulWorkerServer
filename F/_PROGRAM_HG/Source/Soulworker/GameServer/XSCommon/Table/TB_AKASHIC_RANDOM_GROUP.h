// 中文说明：
// 1. 本文件承接 TB_AKASHIC_RANDOM_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AKASHIC_RANDOM_GROUP {
    unsigned int Akashic_Random_Group_ID = 0;
    unsigned int uniAkashic_Group[10] = {};
    std::uint16_t uniChance[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_AKASHIC_RANDOM_GROUP) == 0x40, "TB_AKASHIC_RANDOM_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AKASHIC_RANDOM_GROUP* GetTB_AKASHIC_RANDOM_GROUP(unsigned int index) ;
    void SetTB_AKASHIC_RANDOM_GROUP(unsigned int index, const TB_AKASHIC_RANDOM_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAkashicRandomGroupDB() ;
    std::map<unsigned int, TB_AKASHIC_RANDOM_GROUP> m_mapTB_AKASHIC_RANDOM_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AKASHIC_RANDOM_GROUP* XResourceMgr::GetTB_AKASHIC_RANDOM_GROUP(unsigned int index) {
        const auto it = m_mapTB_AKASHIC_RANDOM_GROUP.find(index);
        return it == m_mapTB_AKASHIC_RANDOM_GROUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AKASHIC_RANDOM_GROUP(unsigned int index, const TB_AKASHIC_RANDOM_GROUP& row) {
        m_mapTB_AKASHIC_RANDOM_GROUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAkashicRandomGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Akashic_Random_Group_ID], [Akashic_Group_01], [Chance_01], [Akashic_Group_02], [Chance_02], [Akashic_Group_03], [Chance_03], [Akashic_Group_04], [Chance_04], [Akashic_Group_05], [Chance_05], [Akashic_Group_06], [Chance_06], [Akashic_Group_07], [Chance_07], [Akashic_Group_08], [Chance_08], [Akashic_Group_09], [Chance_09], [Akashic_Group_10], [Chance_10] from tb_Akashic_Random_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AKASHIC_RANDOM_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AKASHIC_RANDOM_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Random_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[8], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[8], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAkashic_Group[9], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniChance[9], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AKASHIC_RANDOM_GROUP(row.Akashic_Random_Group_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
