// 中文说明：
// 1. 本文件承接 TB_DS_POINT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DS_POINT {
    std::uint16_t ID = 0;
    std::uint8_t Share_Type = 0;
    std::uint16_t DS_Point_ID = 0;
    std::uint8_t Gauge_Type = 0;
    std::uint8_t Clear_Point_1 = 0;
    std::uint8_t Clear_Point_2 = 0;
    std::uint8_t Clear_Point_3 = 0;
    std::uint8_t Clear_Point_4 = 0;
    std::uint8_t Clear_Point_5 = 0;
    std::uint16_t Reward_Value_01 = 0;
    std::uint16_t Reward_Value_02 = 0;
    std::uint16_t Reward_Value_03 = 0;
    std::uint16_t Reward_Value_04 = 0;
    std::uint16_t Reward_Value_05 = 0;
    std::uint8_t Reward_ItemType_01 = 0;
    std::uint8_t Reward_ItemType_02 = 0;
    std::uint8_t Reward_ItemType_03 = 0;
    std::uint8_t Reward_ItemType_04 = 0;
    std::uint8_t Reward_ItemType_05 = 0;
    unsigned int Reward_ItemID_01 = 0;
    unsigned int Reward_ItemID_02 = 0;
    unsigned int Reward_ItemID_03 = 0;
    unsigned int Reward_ItemID_04 = 0;
    unsigned int Reward_ItemID_05 = 0;
    std::uint8_t Item_Count_01 = 0;
    std::uint8_t Item_Count_02 = 0;
    std::uint8_t Item_Count_03 = 0;
    std::uint8_t Item_Count_04 = 0;
    std::uint8_t Item_Count_05 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DS_POINT) == 0x33, "TB_DS_POINT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DS_POINT* GetTB_DS_POINT(std::uint16_t index) ;
    void SetTB_DS_POINT(std::uint16_t index, const TB_DS_POINT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDSPointDB() ;
    std::unordered_map<std::uint16_t, TB_DS_POINT> dsPointRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DS_POINT* XResourceMgr::GetTB_DS_POINT(std::uint16_t index) {
        auto it = dsPointRows_.find(index);
        if (it == dsPointRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DS_POINT(std::uint16_t index, const TB_DS_POINT& row) {
        dsPointRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDSPointDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Share_Type], [DS_Point_ID], [Gauge_Type], [Clear_Point_1], [Clear_Point_2], [Clear_Point_3], [Clear_Point_4], [Clear_Point_5], [Reward_Value_01], [Reward_ItemType_01], [Reward_ItemID_01], [Item_Count_01], [Reward_Value_02], [Reward_ItemType_02], [Reward_ItemID_02], [Item_Count_02], [Reward_Value_03], [Reward_ItemType_03], [Reward_ItemID_03], [Item_Count_03], [Reward_Value_04], [Reward_ItemType_04], [Reward_ItemID_04], [Item_Count_04], [Reward_Value_05], [Reward_ItemType_05], [Reward_ItemID_05], [Item_Count_05] from tb_DS_Point")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dsPointRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DS_POINT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Share_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DS_Point_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gauge_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Point_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Point_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Point_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Point_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Point_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemType_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Value_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemType_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemType_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Value_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemType_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Value_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemType_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_ItemID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count_05, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DS_POINT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
