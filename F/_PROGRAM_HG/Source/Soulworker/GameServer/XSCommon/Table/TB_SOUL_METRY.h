// 中文说明：
// 1. 本文件承接 TB_SOUL_METRY 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SOUL_METRY {
    std::uint16_t Soul_metry_ID = 0;
    std::uint16_t Soul_metry_complet_bit = 0;
    std::uint16_t uniSoul_metry_maze_object[5] = {};
    unsigned int uniSoul_metry_object[5] = {};
    std::uint16_t uniSoul_metry_object_rate[5] = {};
    std::uint8_t uniSoul_metry_reward_item_type[2] = {};
    unsigned int unireward_item[2] = {};
    std::uint8_t unireward_item_count[2] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_SOUL_METRY) == 0x38, "TB_SOUL_METRY size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SOUL_METRY* GetTB_SOUL_METRY(std::uint16_t index) ;
    void SetTB_SOUL_METRY(std::uint16_t index, const TB_SOUL_METRY& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSoulMetryDB() ;
    std::map<std::uint16_t, TB_SOUL_METRY> m_mapTB_SOUL_METRY;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SOUL_METRY* XResourceMgr::GetTB_SOUL_METRY(std::uint16_t index) {
        const auto it = m_mapTB_SOUL_METRY.find(index);
        return it == m_mapTB_SOUL_METRY.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SOUL_METRY(std::uint16_t index, const TB_SOUL_METRY& row) {
        m_mapTB_SOUL_METRY[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSoulMetryDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Soul_metry_ID], [Soul_metry_complet_bit], [Soul_metry_maze_object_1], [Soul_metry_object_1], [Object_1_spawn_rate], [Soul_metry_maze_object_2], [Soul_metry_object_2], [Object_2_spawn_rate], [Soul_metry_maze_object_3], [Soul_metry_object_3], [Object_3_spawn_rate], [Soul_metry_maze_object_4], [Soul_metry_object_4], [Object_4_spawn_rate], [Soul_metry_maze_object_5], [Soul_metry_object_5], [Object_5_spawn_rate], [Soul_metry_reward_item_type_1], [Soul_metry_reward_item_1], [Soul_metry_reward_item_1_count], [Soul_metry_reward_item_type_2], [Soul_metry_reward_item_2], [Soul_metry_reward_item_2_count] from tb_soul_metry")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SOUL_METRY.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SOUL_METRY row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Soul_metry_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Soul_metry_complet_bit, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSoul_metry_maze_object[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSoul_metry_object[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSoul_metry_object_rate[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            for (int index = 0; index < 2; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSoul_metry_reward_item_type[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unireward_item[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.unireward_item_count[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_SOUL_METRY(row.Soul_metry_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
