// 中文说明：
// 1. 本文件承接 TB_WEAPON_RATE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WEAPON_RATE {
    std::uint8_t Weapon_Mob_Levelinterval = 0;
    float Weapon_Mob_Applyrate = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_WEAPON_RATE) == 0x5, "TB_WEAPON_RATE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_WEAPON_RATE* GetTB_WEAPON_RATE(std::uint8_t index) ;
    void SetTB_WEAPON_RATE(std::uint8_t index, const TB_WEAPON_RATE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWeaponRateDB() ;
    std::map<std::uint8_t, TB_WEAPON_RATE> m_mapTB_WEAPON_RATE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_WEAPON_RATE* XResourceMgr::GetTB_WEAPON_RATE(std::uint8_t index) {
        const auto it = m_mapTB_WEAPON_RATE.find(index);
        return it == m_mapTB_WEAPON_RATE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_WEAPON_RATE(std::uint8_t index, const TB_WEAPON_RATE& row) {
        m_mapTB_WEAPON_RATE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBWeaponRateDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Weapon_Mob_Levelinterval], [Weapon_Mob_Applyrate] from tb_Weapon_Rate")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WEAPON_RATE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WEAPON_RATE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Weapon_Mob_Levelinterval, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            float applyRate = 0.0f;
            if (!GreenDamTan_DBGetFloat(xDBBinder, &applyRate, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            std::memcpy(&row.Weapon_Mob_Applyrate, &applyRate, sizeof(applyRate));
            SetTB_WEAPON_RATE(row.Weapon_Mob_Levelinterval, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
