// 中文说明：
// 1. 本文件承接 TB_DROPRATE_MOB 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DROPRATE_MOB {
    std::uint8_t DropRate_Mob_LevelInterval = 0;
    float DropRate_MobInterval_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_DROPRATE_MOB) == 0x5, "TB_DROPRATE_MOB size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DROPRATE_MOB* GetTB_DROPRATE_MOB(std::uint8_t index) ;
    void SetTB_DROPRATE_MOB(std::uint8_t index, const TB_DROPRATE_MOB& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDropRateMobDB() ;
    std::unordered_map<std::uint8_t, TB_DROPRATE_MOB> dropRateMobRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DROPRATE_MOB* XResourceMgr::GetTB_DROPRATE_MOB(std::uint8_t index) {
        auto it = dropRateMobRows_.find(index);
        if (it == dropRateMobRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DROPRATE_MOB(std::uint8_t index, const TB_DROPRATE_MOB& row) {
        dropRateMobRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDropRateMobDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [DropRate_Mob_LevelInterval], [DropRate_MobInterval_Value] from tb_DropRate_Mob")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dropRateMobRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DROPRATE_MOB row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.DropRate_Mob_LevelInterval, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.DropRate_MobInterval_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DROPRATE_MOB(row.DropRate_Mob_LevelInterval, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
