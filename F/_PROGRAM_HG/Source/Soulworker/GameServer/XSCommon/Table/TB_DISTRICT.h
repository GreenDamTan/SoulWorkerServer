// 中文说明：
// 1. 本文件承接 TB_DISTRICT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DISTRICT {
    std::int16_t District_ID = 0;
    unsigned int NeedQuest_ID = 0;
    char District_FileName[255] = {};
    char District_BatName[255] = {};
    char District_ServerName[255] = {};
    std::uint16_t District_Time = 0;
    int District_Start_X = 0;
    int District_Start_Y = 0;
    unsigned int District_Size_X = 0;
    unsigned int District_Size_Y = 0;
    std::uint8_t District_Type = 0;
    std::uint8_t District_Ability = 0;
    std::uint8_t District_Max_People = 0;
    std::uint8_t District_Channel_Count = 0;
    std::uint8_t Arkashic_Use = 0;
    std::uint8_t Force_Use = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DISTRICT) == 0x31B, "TB_DISTRICT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DISTRICT* GetTB_DISTRICT(std::int16_t index) ;
    void SetTB_DISTRICT(std::int16_t index, const TB_DISTRICT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDistrictDB() ;
    std::unordered_map<std::int16_t, TB_DISTRICT> districtRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DISTRICT* XResourceMgr::GetTB_DISTRICT(std::int16_t index) {
        auto it = districtRows_.find(index);
        if (it == districtRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DISTRICT(std::int16_t index, const TB_DISTRICT& row) {
        districtRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDistrictDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [District_ID], [NeedQuest_ID], [District_FileName], [District_BatName], [District_ServerName], [District_Time], [District_Start_X], [District_Start_Y], [District_Size_X], [District_Size_Y], [District_Type], [District_Ability], [District_Max_People], [District_Channel_Count], [Arkashic_Use], [Force_Use] from tb_district")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        districtRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DISTRICT row{};
            if (!GreenDamTan_DBGetSigned(xDBBinder, &row.District_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NeedQuest_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.District_FileName, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.District_BatName, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.District_ServerName, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Time, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.District_Start_X, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.District_Start_Y, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Size_X, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Size_Y, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Ability, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Max_People, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Channel_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Arkashic_Use, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Force_Use, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DISTRICT(row.District_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
