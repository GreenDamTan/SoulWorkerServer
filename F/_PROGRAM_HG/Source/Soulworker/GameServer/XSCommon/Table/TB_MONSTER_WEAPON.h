// 中文说明：
// 1. 本文件承接 TB_MONSTER_WEAPON 的单表还原片段，字段顺序按原版 SQL / file-mode 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏多次包含。
// 3. 不要擅自更改 Weapon_Mesh_Change_Name 长度或 5 组 HP 值字段顺序。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MONSTER_WEAPON {
    std::uint16_t ID = 0;
    char Weapon_Mesh_Change_Name[511] = {};
    unsigned int Weapon_HP = 0;
    unsigned int Weapon_HP_Value_01 = 0;
    unsigned int Weapon_HP_Value_02 = 0;
    unsigned int Weapon_HP_Value_03 = 0;
    unsigned int Weapon_HP_Value_04 = 0;
    unsigned int Weapon_HP_Value_05 = 0;
};
#pragma pack(pop)

static_assert(sizeof(TB_MONSTER_WEAPON) == 0x219, "TB_MONSTER_WEAPON size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MONSTER_WEAPON* GetTB_MONSTER_WEAPON(std::uint16_t index) ;
    void SetTB_MONSTER_WEAPON(std::uint16_t index, const TB_MONSTER_WEAPON& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMonsterWeaponDB() ;
    std::map<std::uint16_t, TB_MONSTER_WEAPON> m_mapTB_MONSTER_WEAPON;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MONSTER_WEAPON* XResourceMgr::GetTB_MONSTER_WEAPON(std::uint16_t index) {
        const auto it = m_mapTB_MONSTER_WEAPON.find(index);
        return it == m_mapTB_MONSTER_WEAPON.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MONSTER_WEAPON(std::uint16_t index, const TB_MONSTER_WEAPON& row) {
        m_mapTB_MONSTER_WEAPON[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMonsterWeaponDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Weapon_Mesh_Change_Name], [Weapon_HP], [Weapon_HP_Value_01], [Weapon_HP_Value_02], [Weapon_HP_Value_03], [Weapon_HP_Value_04], [Weapon_HP_Value_05] from tb_Monster_Weapon")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MONSTER_WEAPON.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MONSTER_WEAPON row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Weapon_Mesh_Change_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_HP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_HP_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_HP_Value_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_HP_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_HP_Value_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_HP_Value_05, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MONSTER_WEAPON(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
