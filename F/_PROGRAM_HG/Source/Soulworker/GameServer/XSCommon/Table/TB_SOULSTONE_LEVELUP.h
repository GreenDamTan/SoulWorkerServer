// 中文说明：
// 1. 本文件承接 TB_SOULSTONE_LEVELUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SOULSTONE_LEVELUP {
    unsigned int SoulStoneLvUp_Index = 0;
    std::uint16_t Evolution_Rate = 0;
    unsigned int Evolution_TargetItemID = 0;
    std::uint16_t HiddenSoulStone_Rate = 0;
    unsigned int HiddenSoulStone_TargetItemID = 0;
    unsigned int uniMaterial[5] = {};
    unsigned int uniCount[5] = {};
    unsigned int Need_Gold = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SOULSTONE_LEVELUP) == 0x3C, "TB_SOULSTONE_LEVELUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SOULSTONE_LEVELUP* GetTB_SOULSTONE_LEVELUP(unsigned int index) ;
    void SetTB_SOULSTONE_LEVELUP(unsigned int index, const TB_SOULSTONE_LEVELUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSoulStoneLevelUpDB() ;
    std::map<unsigned int, TB_SOULSTONE_LEVELUP> m_mapTB_SOULSTONE_LEVELUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SOULSTONE_LEVELUP* XResourceMgr::GetTB_SOULSTONE_LEVELUP(unsigned int index) {
        const auto it = m_mapTB_SOULSTONE_LEVELUP.find(index);
        return it == m_mapTB_SOULSTONE_LEVELUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SOULSTONE_LEVELUP(unsigned int index, const TB_SOULSTONE_LEVELUP& row) {
        m_mapTB_SOULSTONE_LEVELUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSoulStoneLevelUpDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [SoulStoneLvUp_Index], [Evolution_Rate], [Evolution_TargetItemID], [HiddenSoulStone_Rate], [HiddenSoulStone_TargetItemID], [Material_01_ID], [Material_01_Count], [Material_02_ID], [Material_02_Count], [Material_03_ID], [Material_03_Count], [Material_04_ID], [Material_04_Count], [Material_05_ID], [Material_05_Count], [Need_Gold] from tb_SoulStone_LevelUp")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SOULSTONE_LEVELUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SOULSTONE_LEVELUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SoulStoneLvUp_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Evolution_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Evolution_TargetItemID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.HiddenSoulStone_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.HiddenSoulStone_TargetItemID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gold, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SOULSTONE_LEVELUP(row.SoulStoneLvUp_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
