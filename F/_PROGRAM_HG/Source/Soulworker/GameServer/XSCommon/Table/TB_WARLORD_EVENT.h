// 中文说明：
// 1. 本文件承接 TB_WARLORD_EVENT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WARLORD_EVENT {
    std::uint16_t ID = 0;
    char EventFunction_Name[511] = {};
    unsigned int EventDelay_Time = 0;
    std::uint8_t EventDamageDC_Value = 0;
    unsigned int PostRewardItem_ID = 0;
    std::uint16_t BossBox_ID = 0;
    unsigned int uniObjectSpawn[5] = {};
    std::uint16_t uniSpawn[5] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_WARLORD_EVENT) == 0x22A, "TB_WARLORD_EVENT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_WARLORD_EVENT* GetTB_WARLORD_EVENT(std::uint16_t index) ;
    void SetTB_WARLORD_EVENT(std::uint16_t index, const TB_WARLORD_EVENT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWarlordEventDB() ;
    std::map<std::uint16_t, TB_WARLORD_EVENT> m_mapTB_WARLORD_EVENT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_WARLORD_EVENT* XResourceMgr::GetTB_WARLORD_EVENT(std::uint16_t index) {
        const auto it = m_mapTB_WARLORD_EVENT.find(index);
        return it == m_mapTB_WARLORD_EVENT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_WARLORD_EVENT(std::uint16_t index, const TB_WARLORD_EVENT& row) {
        m_mapTB_WARLORD_EVENT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBWarlordEventDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [EventFunction_Name], [EventDelay_Time], [EventDamageDC_Value], [PostRewardItem_ID], [BossBox_ID], [ObjectMonster_ID_01], [ObjectMonster_ID_02], [ObjectMonster_ID_03], [ObjectMonster_ID_04], [ObjectMonster_ID_05], [SpawnGroup_ID_01], [SpawnGroup_ID_02], [SpawnGroup_ID_03], [SpawnGroup_ID_04], [SpawnGroup_ID_05] from tb_Warlord_Event")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WARLORD_EVENT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WARLORD_EVENT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.EventFunction_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EventDelay_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EventDamageDC_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.PostRewardItem_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BossBox_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniObjectSpawn[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSpawn[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_WARLORD_EVENT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
