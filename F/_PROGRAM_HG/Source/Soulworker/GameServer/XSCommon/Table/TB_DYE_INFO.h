// 中文说明：
// 1. 本文件承接 TB_DYE_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DYE_INFO {
    std::uint16_t Dye_Info_ID = 0;
    unsigned int S_Dye_Need_Item_ID = 0;
    unsigned int S_Dye_Need_Item_CNT = 0;
    std::uint16_t S_Dye_1 = 0;
    std::uint16_t S_Dye_2 = 0;
    std::uint16_t S_Dye_3 = 0;
    std::uint16_t S_Dye_4 = 0;
    std::uint16_t S_Dye_5 = 0;
    std::uint16_t S_Dye_6 = 0;
    std::uint16_t S_Dye_7 = 0;
    std::uint16_t S_Dye_8 = 0;
    std::uint16_t S_Dye_9 = 0;
    std::uint16_t S_Dye_10 = 0;
    unsigned int R_Dye_Need_Item_ID = 0;
    std::uint8_t R_Dye_Need_Item_CNT = 0;
    std::uint16_t Dye_Rate_ID_1 = 0;
    std::uint16_t Dye_Rate_ID_2 = 0;
    std::uint16_t Dye_Rate_ID_3 = 0;
    std::uint16_t Dye_Rate_ID_4 = 0;
    std::uint16_t Dye_Rate_ID_5 = 0;
    std::uint16_t Dye_Rate_ID_6 = 0;
    std::uint16_t Dye_Rate_ID_7 = 0;
    std::uint16_t Dye_Rate_ID_8 = 0;
    std::uint16_t Dye_Rate_ID_9 = 0;
    std::uint16_t Dye_Rate_ID_10 = 0;
    std::uint16_t Dye_Rate_1 = 0;
    std::uint16_t Dye_Rate_2 = 0;
    std::uint16_t Dye_Rate_3 = 0;
    std::uint16_t Dye_Rate_4 = 0;
    std::uint16_t Dye_Rate_5 = 0;
    std::uint16_t Dye_Rate_6 = 0;
    std::uint16_t Dye_Rate_7 = 0;
    std::uint16_t Dye_Rate_8 = 0;
    std::uint16_t Dye_Rate_9 = 0;
    std::uint16_t Dye_Rate_10 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DYE_INFO) == 0x4B, "TB_DYE_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DYE_INFO* GetTB_DYE_INFO(std::uint16_t index) ;
    void SetTB_DYE_INFO(std::uint16_t index, const TB_DYE_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDyeInfoDB() ;
    std::unordered_map<std::uint16_t, TB_DYE_INFO> dyeInfoRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DYE_INFO* XResourceMgr::GetTB_DYE_INFO(std::uint16_t index) {
        auto it = dyeInfoRows_.find(index);
        if (it == dyeInfoRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DYE_INFO(std::uint16_t index, const TB_DYE_INFO& row) {
        dyeInfoRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDyeInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Dye_Info_ID], [S_Dye_Need_Item_ID], [S_Dye_Need_Item_CNT], [S_Dye_1], [S_Dye_2], [S_Dye_3], [S_Dye_4], [S_Dye_5], [S_Dye_6], [S_Dye_7], [S_Dye_8], [S_Dye_9], [S_Dye_10], [R_Dye_Need_Item_ID], [R_Dye_Need_Item_CNT], [Dye_Rate_ID_1], [Dye_Rate_1], [Dye_Rate_ID_2], [Dye_Rate_2], [Dye_Rate_ID_3], [Dye_Rate_3], [Dye_Rate_ID_4], [Dye_Rate_4], [Dye_Rate_ID_5], [Dye_Rate_5], [Dye_Rate_ID_6], [Dye_Rate_6], [Dye_Rate_ID_7], [Dye_Rate_7], [Dye_Rate_ID_8], [Dye_Rate_8], [Dye_Rate_ID_9], [Dye_Rate_9], [Dye_Rate_ID_10], [Dye_Rate_10] from tb_Dye_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dyeInfoRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DYE_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Info_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_Need_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_Need_Item_CNT, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Dye_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.R_Dye_Need_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.R_Dye_Need_Item_CNT, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_Rate_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DYE_INFO(row.Dye_Info_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
