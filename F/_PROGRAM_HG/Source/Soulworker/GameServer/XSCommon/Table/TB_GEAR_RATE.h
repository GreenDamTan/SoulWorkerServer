// 中文说明：
// 1. 本文件承接 TB_GEAR_RATE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_GEAR_RATE {
    std::uint8_t Gear_Mob_Levelinterval = 0;
    float Gear_Mob_Applyrate = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_GEAR_RATE) == 0x5, "TB_GEAR_RATE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_GEAR_RATE* GetTB_GEAR_RATE(std::uint8_t index) ;
    void SetTB_GEAR_RATE(std::uint8_t index, const TB_GEAR_RATE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBGearRateDB() ;
    std::unordered_map<std::uint8_t, TB_GEAR_RATE> gearRateRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_GEAR_RATE* XResourceMgr::GetTB_GEAR_RATE(std::uint8_t index) {
        auto it = gearRateRows_.find(index);
        if (it == gearRateRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_GEAR_RATE(std::uint8_t index, const TB_GEAR_RATE& row) {
        gearRateRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBGearRateDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Gear_Mob_Levelinterval], [Gear_Mob_Applyrate] from tb_Gear_Rate")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        gearRateRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_GEAR_RATE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gear_Mob_Levelinterval, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Gear_Mob_Applyrate, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_GEAR_RATE(row.Gear_Mob_Levelinterval, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
