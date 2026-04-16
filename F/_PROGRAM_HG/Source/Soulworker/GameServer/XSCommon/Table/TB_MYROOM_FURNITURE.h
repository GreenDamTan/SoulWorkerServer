// 中文说明：
// 1. 本文件承接 TB_MYROOM_FURNITURE 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 3 组 Make_Material_* 字段折叠成自定义容器，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MYROOM_FURNITURE {
    unsigned int ID = 0;
    unsigned int Furniture_Item_Object_Type = 0;
    unsigned int Furniture_Item_ID = 0;
    unsigned int Furniture_Item_Dispose_Position = 0;
    unsigned int Furniture_Item_Greed_X_Coordinate = 0;
    unsigned int Furniture_Item_Greed_Y_Coordinate = 0;
    unsigned int Furniture_Item_Special_Use = 0;
    unsigned int Furniture_Item_Category = 0;
    char Make_Material_Icon_1[511] = {};
    unsigned int Make_Material_ID_1 = 0;
    unsigned int Make_Material_Num_1 = 0;
    char Make_Material_Icon_2[511] = {};
    unsigned int Make_Material_ID_2 = 0;
    unsigned int Make_Material_Num_2 = 0;
    char Make_Material_Icon_3[511] = {};
    unsigned int Make_Material_ID_3 = 0;
    unsigned int Make_Material_Num_3 = 0;
    unsigned int Furniture_Item_Make_Price = 0;
    unsigned int Furniture_Item_Special_Effect = 0;
    unsigned int Furniture_Item_Special_Effect_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MYROOM_FURNITURE) == 0x641, "TB_MYROOM_FURNITURE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MYROOM_FURNITURE* GetTB_MYROOM_FURNITURE(unsigned int index) ;
    void SetTB_MYROOM_FURNITURE(unsigned int index, const TB_MYROOM_FURNITURE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMyroomFurnitureDB() ;
    std::map<unsigned int, TB_MYROOM_FURNITURE> m_mapTB_MYROOM_FURNITURE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MYROOM_FURNITURE* XResourceMgr::GetTB_MYROOM_FURNITURE(unsigned int index) {
        const auto it = m_mapTB_MYROOM_FURNITURE.find(index);
        return it == m_mapTB_MYROOM_FURNITURE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MYROOM_FURNITURE(unsigned int index, const TB_MYROOM_FURNITURE& row) {
        m_mapTB_MYROOM_FURNITURE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMyroomFurnitureDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Furniture_Item_Object_Type], [Furniture_Item_ID], [Furniture_Item_Dispose_Position], [Furniture_Item_Greed_X_Coordinate], [Furniture_Item_Greed_Y_Coordinate], [Furniture_Item_Special_Use], [Furniture_Item_Category], [Make_Material_Icon_1], [Make_Material_ID_1], [Make_Material_Num_1], [Make_Material_Icon_2], [Make_Material_ID_2], [Make_Material_Num_2], [Make_Material_Icon_3], [Make_Material_ID_3], [Make_Material_Num_3], [Furniture_Item_Make_Price], [Furniture_Item_Special_Effect], [Furniture_Item_Special_Effect_ID] from tb_Myroom_Furniture")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MYROOM_FURNITURE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MYROOM_FURNITURE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Object_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Dispose_Position, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Greed_X_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Greed_Y_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Special_Use, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Category, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Make_Material_Icon_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Material_ID_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Material_Num_1, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Make_Material_Icon_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Material_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Material_Num_2, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Make_Material_Icon_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Material_ID_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Material_Num_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Make_Price, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Special_Effect, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Item_Special_Effect_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MYROOM_FURNITURE(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
