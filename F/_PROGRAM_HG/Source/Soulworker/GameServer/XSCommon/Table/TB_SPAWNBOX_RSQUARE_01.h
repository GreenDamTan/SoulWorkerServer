// 中文说明：
// 1. 本文件承接 TB_SPAWNBOX_RSQUARE_01 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SPAWNBOX_RSQUARE_01 {
    std::uint8_t ID = 0;
    std::uint16_t SpawnBox_ID = 0;
    unsigned int Monster_ID = 0;
    std::uint8_t Monster_Count = 0;
    unsigned int NextSpawn_Deley = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SPAWNBOX_RSQUARE_01) == 0xC, "TB_SPAWNBOX_RSQUARE_01 size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SPAWNBOX_RSQUARE_01* GetTB_SPAWNBOX_RSQUARE_01(std::uint8_t index) ;
    void SetTB_SPAWNBOX_RSQUARE_01(std::uint8_t index, const TB_SPAWNBOX_RSQUARE_01& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSpawnBoxRSquare01DB() ;
    std::map<std::uint8_t, TB_SPAWNBOX_RSQUARE_01> m_mapTB_SPAWNBOX_RSQUARE_01;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SPAWNBOX_RSQUARE_01* XResourceMgr::GetTB_SPAWNBOX_RSQUARE_01(std::uint8_t index) {
        const auto it = m_mapTB_SPAWNBOX_RSQUARE_01.find(index);
        return it == m_mapTB_SPAWNBOX_RSQUARE_01.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SPAWNBOX_RSQUARE_01(std::uint8_t index, const TB_SPAWNBOX_RSQUARE_01& row) {
        m_mapTB_SPAWNBOX_RSQUARE_01[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSpawnBoxRSquare01DB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [SpawnBox_ID], [Monster_ID], [Monster_Count], [NextSpawn_Deley] from tb_SpawnBox_RSquare_01")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SPAWNBOX_RSQUARE_01.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SPAWNBOX_RSQUARE_01 row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SpawnBox_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NextSpawn_Deley, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SPAWNBOX_RSQUARE_01(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
