// 中文说明：
// 1. 本文件承接 TB_CULTIVATION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CULTIVATION {
    unsigned int Seed_Item_ID = 0;
    unsigned int Crops_Item_ID = 0;
    unsigned int Crops_Item_Num = 0;
    unsigned int Cultivation_Bonus_Rate = 0;
    unsigned int Crops_Bonus_Item_Num = 0;
    unsigned int Cultivation_Time = 0;
    unsigned int Cultivation_Boost_Time = 0;
    unsigned int Wilt_Item_ID = 0;
    unsigned int Wilt_Item_ID_Num = 0;
    unsigned int Decay_Item_ID = 0;
    unsigned int Decay_Item_ID_Num = 0;
    unsigned int Auto_Usage_Item = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CULTIVATION) == 0x30, "TB_CULTIVATION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CULTIVATION* GetTB_CULTIVATION(unsigned int index) ;
    void SetTB_CULTIVATION(unsigned int index, const TB_CULTIVATION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCultivationDB() ;
    std::unordered_map<unsigned int, TB_CULTIVATION> cultivationRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CULTIVATION* XResourceMgr::GetTB_CULTIVATION(unsigned int index) {
        auto it = cultivationRows_.find(index);
        if (it == cultivationRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CULTIVATION(unsigned int index, const TB_CULTIVATION& row) {
        cultivationRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCultivationDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Seed_Item_ID], [Crops_Item_ID], [Crops_Item_Num], [Cultivation_Bonus_Rate], [Crops_Bonus_Item_Num], [Cultivation_Time], [Cultivation_Boost_Time], [Wilt_Item_ID], [Wilt_Item_ID_Num], [Decay_Item_ID], [Decay_Item_ID_Num], [Auto_Usage_Item] from tb_Cultivation")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        cultivationRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CULTIVATION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Seed_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Crops_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Crops_Item_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cultivation_Bonus_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Crops_Bonus_Item_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cultivation_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cultivation_Boost_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wilt_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Wilt_Item_ID_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Decay_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Decay_Item_ID_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Auto_Usage_Item, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CULTIVATION(row.Seed_Item_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
