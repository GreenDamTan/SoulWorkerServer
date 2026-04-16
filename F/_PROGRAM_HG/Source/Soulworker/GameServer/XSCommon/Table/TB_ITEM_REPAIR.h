// 中文说明：
// 1. 本文件承接 TB_ITEM_REPAIR 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_REPAIR {
    unsigned int Index = 0;
    float Weapon_NPC_Cost_Lv = 0.0f;
    float Weapon_NPC_Cost_Dur = 0.0f;
    float Weapon_NPC_Cost_Grade = 0.0f;
    float Weapon_Item_Cost_Lv = 0.0f;
    float Weapon_Item_Cost_Dur = 0.0f;
    float Weapon_Item_Cost_Grade = 0.0f;
    float Sub_Weapon_NPC_Cost_Lv = 0.0f;
    float Sub_Weapon_NPC_Cost_Dur = 0.0f;
    float Sub_Weapon_NPC_Cost_Grade = 0.0f;
    float Sub_Weapon_Item_Cost_Lv = 0.0f;
    float Sub_Weapon_Item_Cost_Dur = 0.0f;
    float Sub_Weapon_Item_Cost_Grade = 0.0f;
    float Gear_NPC_Cost_Lv = 0.0f;
    float Gear_NPC_Cost_Dur = 0.0f;
    float Gear_NPC_Cost_Grade = 0.0f;
    float Gear_Item_Cost_Lv = 0.0f;
    float Gear_Item_Cost_Dur = 0.0f;
    float Gear_Item_Cost_Grade = 0.0f;
    float Dur_Penalty = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_REPAIR) == 0x50, "TB_ITEM_REPAIR size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_REPAIR* GetTB_ITEM_REPAIR(unsigned int index) ;
    void SetTB_ITEM_REPAIR(unsigned int index, const TB_ITEM_REPAIR& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemRepairDB() ;
    std::unordered_map<unsigned int, TB_ITEM_REPAIR> itemRepairRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_REPAIR* XResourceMgr::GetTB_ITEM_REPAIR(unsigned int index) {
        auto it = itemRepairRows_.find(index);
        if (it == itemRepairRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_REPAIR(unsigned int index, const TB_ITEM_REPAIR& row) {
        itemRepairRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemRepairDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Index], [Weapon_NPC_Cost_Lv], [Weapon_NPC_Cost_Dur], [Weapon_NPC_Cost_Grade], [Weapon_Item_Cost_Lv], [Weapon_Item_Cost_Dur], [Weapon_Item_Cost_Grade], [Sub_Weapon_NPC_Cost_Lv], [Sub_Weapon_NPC_Cost_Dur], [Sub_Weapon_NPC_Cost_Grade], [Sub_Weapon_Item_Cost_Lv], [Sub_Weapon_Item_Cost_Dur], [Sub_Weapon_Item_Cost_Grade], [Gear_NPC_Cost_Lv], [Gear_NPC_Cost_Dur], [Gear_NPC_Cost_Grade], [Gear_Item_Cost_Lv], [Gear_Item_Cost_Dur], [Gear_Item_Cost_Grade], [Dur_Penalty] from tb_Item_Repair")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemRepairRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_REPAIR row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Index, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Weapon_NPC_Cost_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Weapon_NPC_Cost_Dur, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Weapon_NPC_Cost_Grade, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Weapon_Item_Cost_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Weapon_Item_Cost_Dur, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Weapon_Item_Cost_Grade, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Sub_Weapon_NPC_Cost_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Sub_Weapon_NPC_Cost_Dur, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Sub_Weapon_NPC_Cost_Grade, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Sub_Weapon_Item_Cost_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Sub_Weapon_Item_Cost_Dur, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Sub_Weapon_Item_Cost_Grade, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_NPC_Cost_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_NPC_Cost_Dur, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_NPC_Cost_Grade, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_Item_Cost_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_Item_Cost_Dur, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_Item_Cost_Grade, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Dur_Penalty, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ITEM_REPAIR(row.Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
