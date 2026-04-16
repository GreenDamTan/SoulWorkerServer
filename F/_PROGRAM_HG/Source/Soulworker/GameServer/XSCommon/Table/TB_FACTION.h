// 中文说明：
// 1. 本文件承接 TB_FACTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_FACTION {
    unsigned int ID = 0;
    std::uint8_t Local_Faction_Type = 0;
    std::uint8_t Enamy_Faction_Type = 0;
    std::uint8_t Faction_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_FACTION) == 0x7, "TB_FACTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_FACTION* GetTB_FACTION(unsigned int index) ;
    void SetTB_FACTION(unsigned int index, const TB_FACTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBFactionDB() ;
    std::unordered_map<unsigned int, TB_FACTION> factionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_FACTION* XResourceMgr::GetTB_FACTION(unsigned int index) {
        auto it = factionRows_.find(index);
        if (it == factionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_FACTION(unsigned int index, const TB_FACTION& row) {
        factionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBFactionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Local_Faction_Type], [Enamy_Faction_Type], [Faction_Type] from tb_Faction")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        factionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_FACTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Local_Faction_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Enamy_Faction_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Faction_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_FACTION(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
