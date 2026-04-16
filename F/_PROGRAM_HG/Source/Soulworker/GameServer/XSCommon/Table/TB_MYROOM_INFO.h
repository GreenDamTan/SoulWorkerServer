// 中文说明：
// 1. 本文件承接 TB_MYROOM_INFO 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 Nomal_* Item / Pollen 字段擅自抽象成自定义容器，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MYROOM_INFO {
    unsigned int ID = 0;
    unsigned int My_Room_Field_ID = 0;
    unsigned int My_Room_Max_Access_Member = 0;
    unsigned int My_Room_Create_Lv = 0;
    unsigned int My_Room_Create_Gold = 0;
    unsigned int Nomal_WallPaper_Item = 0;
    unsigned int Nomal_Floor_Item = 0;
    unsigned int Nomal_TerraceDoor_Item = 0;
    unsigned int Nomal_Window_Item = 0;
    unsigned int Nomal_Door_Item = 0;
    unsigned int Nomal_Ceiling_Item = 0;
    unsigned int Nomal_Pollen_Index_01 = 0;
    unsigned int Nomal_Pollen_Index_02 = 0;
    unsigned int Nomal_Pollen_Index_03 = 0;
    unsigned int Nomal_Pollen_Index_04 = 0;
    unsigned int Nomal_Pollen_Index_05 = 0;
    unsigned int Nomal_Pollen_Index_06 = 0;
    unsigned int Nomal_Pollen_Index_07 = 0;
    unsigned int Nomal_Pollen_Index_08 = 0;
    unsigned int Nomal_Pollen_Index_09 = 0;
    unsigned int Nomal_Pollen_Index_10 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MYROOM_INFO) == 0x54, "TB_MYROOM_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MYROOM_INFO* GetTB_MYROOM_INFO(unsigned int index) ;
    void SetTB_MYROOM_INFO(unsigned int index, const TB_MYROOM_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMyroomInfoDB() ;
    std::map<unsigned int, TB_MYROOM_INFO> m_mapTB_MYROOM_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MYROOM_INFO* XResourceMgr::GetTB_MYROOM_INFO(unsigned int index) {
        const auto it = m_mapTB_MYROOM_INFO.find(index);
        return it == m_mapTB_MYROOM_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MYROOM_INFO(unsigned int index, const TB_MYROOM_INFO& row) {
        m_mapTB_MYROOM_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMyroomInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [My_Room_Field_ID], [My_Room_Max_Access_Member], [My_Room_Create_Lv], [My_Room_Create_Gold], [Nomal_WallPaper_Item], [Nomal_Floor_Item], [Nomal_TerraceDoor_Item], [Nomal_Window_Item], [Nomal_Door_Item], [Nomal_Ceiling_Item], [Nomal_Pollen_Index_01], [Nomal_Pollen_Index_02], [Nomal_Pollen_Index_03], [Nomal_Pollen_Index_04], [Nomal_Pollen_Index_05], [Nomal_Pollen_Index_06], [Nomal_Pollen_Index_07], [Nomal_Pollen_Index_08], [Nomal_Pollen_Index_09], [Nomal_Pollen_Index_10] from tb_Myroom_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MYROOM_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MYROOM_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.My_Room_Field_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.My_Room_Max_Access_Member, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.My_Room_Create_Lv, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.My_Room_Create_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_WallPaper_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Floor_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_TerraceDoor_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Window_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Door_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Ceiling_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Pollen_Index_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MYROOM_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
