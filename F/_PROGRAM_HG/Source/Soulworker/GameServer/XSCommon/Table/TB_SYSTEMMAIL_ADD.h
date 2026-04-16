// 中文说明：
// 1. 本文件承接 TB_SYSTEMMAIL_ADD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SYSTEMMAIL_ADD {
    std::uint16_t AutoMail_ADD_ID = 0;
    char Des[511] = {};
    std::uint8_t MailBox_Type = 0;
    std::uint8_t AutoMail_Type_ID = 0;
    std::uint8_t AutoMail_Type_On_Off = 0;
    std::uint16_t AutoMail_Time_Value = 0;
    std::uint8_t Link_SystemMail_ID = 0;
    unsigned int AutoMail_ADD_Gold = 0;
    unsigned int AutoMail_ADD_Item_01 = 0;
    std::uint16_t AutoMail_ADD_Count_01 = 0;
    unsigned int AutoMail_ADD_Item_02 = 0;
    std::uint16_t AutoMail_ADD_Count_02 = 0;
    unsigned int AutoMail_ADD_Item_03 = 0;
    std::uint16_t AutoMail_ADD_Count_03 = 0;
    unsigned int AutoMail_ADD_Item_04 = 0;
    std::uint16_t AutoMail_ADD_Count_04 = 0;
    unsigned int AutoMail_ADD_Item_05 = 0;
    std::uint16_t AutoMail_ADD_Count_05 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SYSTEMMAIL_ADD) == 0x229, "TB_SYSTEMMAIL_ADD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SYSTEMMAIL_ADD* GetTB_SYSTEMMAIL_ADD(std::uint16_t index) ;
    void SetTB_SYSTEMMAIL_ADD(std::uint16_t index, const TB_SYSTEMMAIL_ADD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSystemMailAddDB() ;
    std::unordered_map<std::uint16_t, TB_SYSTEMMAIL_ADD> autoMailRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SYSTEMMAIL_ADD* XResourceMgr::GetTB_SYSTEMMAIL_ADD(std::uint16_t index) {
        auto it = autoMailRows_.find(index);
        if (it == autoMailRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_SYSTEMMAIL_ADD(std::uint16_t index, const TB_SYSTEMMAIL_ADD& row) {
        autoMailRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSystemMailAddDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [AutoMail_ADD_ID], [Des], [MailBox_Type], [AutoMail_Type_ID], [AutoMail_Type_On_Off], [AutoMail_Time_Value], [Link_SystemMail_ID], [AutoMail_ADD_Gold], [AutoMail_ADD_Item_01], [AutoMail_ADD_Count_01], [AutoMail_ADD_Item_02], [AutoMail_ADD_Count_02], [AutoMail_ADD_Item_03], [AutoMail_ADD_Count_03], [AutoMail_ADD_Item_04], [AutoMail_ADD_Count_04], [AutoMail_ADD_Item_05], [AutoMail_ADD_Count_05] from tb_SystemMail_Add")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        autoMailRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SYSTEMMAIL_ADD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Des, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MailBox_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_Type_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_Type_On_Off, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_Time_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_SystemMail_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Item_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Item_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Item_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Item_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Count_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Item_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AutoMail_ADD_Count_05, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SYSTEMMAIL_ADD(row.AutoMail_ADD_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
