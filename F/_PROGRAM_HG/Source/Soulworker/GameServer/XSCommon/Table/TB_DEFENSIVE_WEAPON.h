// 中文说明：
// 1. 本文件承接 TB_DEFENSIVE_WEAPON 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DEFENSIVE_WEAPON {
    unsigned int ID = 0;
    unsigned int Monster_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DEFENSIVE_WEAPON) == 0x8, "TB_DEFENSIVE_WEAPON size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DEFENSIVE_WEAPON* GetTB_DEFENSIVE_WEAPON(unsigned int index) ;
    void SetTB_DEFENSIVE_WEAPON(unsigned int index, const TB_DEFENSIVE_WEAPON& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDefensiveWeaponDB() ;
    std::unordered_map<unsigned int, TB_DEFENSIVE_WEAPON> defensiveWeaponRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DEFENSIVE_WEAPON* XResourceMgr::GetTB_DEFENSIVE_WEAPON(unsigned int index) {
        auto it = defensiveWeaponRows_.find(index);
        if (it == defensiveWeaponRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DEFENSIVE_WEAPON(unsigned int index, const TB_DEFENSIVE_WEAPON& row) {
        defensiveWeaponRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDefensiveWeaponDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Monster_ID] from tb_Defensive_Weapon")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        defensiveWeaponRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DEFENSIVE_WEAPON row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DEFENSIVE_WEAPON(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
