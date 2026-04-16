// 中文说明：
// 1. 本文件承接 TB_DISTRICT_TRANSPORT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DISTRICT_TRANSPORT {
    std::uint16_t ID = 0;
    std::uint16_t District_ID = 0;
    std::uint16_t Transport_Info_ID_01 = 0;
    std::uint16_t Transport_Info_ID_02 = 0;
    std::uint16_t Transport_Info_ID_03 = 0;
    std::uint16_t Transport_Info_ID_04 = 0;
    std::uint16_t Transport_Info_ID_05 = 0;
    std::uint8_t End_ID_01 = 0;
    std::uint8_t End_ID_02 = 0;
    std::uint8_t End_ID_03 = 0;
    std::uint8_t End_ID_04 = 0;
    std::uint8_t End_ID_05 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DISTRICT_TRANSPORT) == 0x13, "TB_DISTRICT_TRANSPORT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DISTRICT_TRANSPORT* GetTB_DISTRICT_TRANSPORT(std::uint16_t index) ;
    void SetTB_DISTRICT_TRANSPORT(std::uint16_t index, const TB_DISTRICT_TRANSPORT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDistrictTransportDB() ;
    std::unordered_map<std::uint16_t, TB_DISTRICT_TRANSPORT> districtTransportRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DISTRICT_TRANSPORT* XResourceMgr::GetTB_DISTRICT_TRANSPORT(std::uint16_t index) {
        auto it = districtTransportRows_.find(index);
        if (it == districtTransportRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DISTRICT_TRANSPORT(std::uint16_t index, const TB_DISTRICT_TRANSPORT& row) {
        districtTransportRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDistrictTransportDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [District_ID], [Transport_Info_ID_01], [End_ID_01], [Transport_Info_ID_02], [End_ID_02], [Transport_Info_ID_03], [End_ID_03], [Transport_Info_ID_04], [End_ID_04], [Transport_Info_ID_05], [End_ID_05] from tb_District_Transport")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        districtTransportRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DISTRICT_TRANSPORT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Info_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Info_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Info_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Info_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Info_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_ID_05, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DISTRICT_TRANSPORT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
