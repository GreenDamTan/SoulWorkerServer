// 中文说明：
// 1. 本文件承接 TB_MYROOM_GREED 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把门窗坐标字段擅自改写成推测结构体，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MYROOM_GREED {
    unsigned int ID = 0;
    unsigned int Greed_X_Axis_Num = 0;
    unsigned int Greed_Y_Axis_Num = 0;
    unsigned int Greed_Z_Axis_Num = 0;
    unsigned int Door_Index = 0;
    unsigned int Door_Position = 0;
    unsigned int Door_X_Coordinate = 0;
    unsigned int Door_Y_Coordinate = 0;
    unsigned int LeftWindow_Index = 0;
    unsigned int LeftWindow_Position = 0;
    unsigned int LeftWindow_X_Coordinate = 0;
    unsigned int LeftWindow_Y_Coordinate = 0;
    unsigned int SmallDoor_Index = 0;
    unsigned int SmallDoor_Position = 0;
    unsigned int SmallDoor_X_Coordinate = 0;
    unsigned int SmallDoor_Y_Coordinate = 0;
    unsigned int RightWindow_Index = 0;
    unsigned int RightWindow_Position = 0;
    unsigned int RightWindow_X_Coordinate = 0;
    unsigned int RightWindow_Y_Coordinate = 0;
    unsigned int Common_Position_Box_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MYROOM_GREED) == 0x54, "TB_MYROOM_GREED size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MYROOM_GREED* GetTB_MYROOM_GREED(unsigned int index) ;
    void SetTB_MYROOM_GREED(unsigned int index, const TB_MYROOM_GREED& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMyroomGreedDB() ;
    std::map<unsigned int, TB_MYROOM_GREED> m_mapTB_MYROOM_GREED;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MYROOM_GREED* XResourceMgr::GetTB_MYROOM_GREED(unsigned int index) {
        const auto it = m_mapTB_MYROOM_GREED.find(index);
        return it == m_mapTB_MYROOM_GREED.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MYROOM_GREED(unsigned int index, const TB_MYROOM_GREED& row) {
        m_mapTB_MYROOM_GREED[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMyroomGreedDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Greed_X_Axis_Num], [Greed_Y_Axis_Num], [Greed_Z_Axis_Num], [Door_Index], [Door_Position], [Door_X_Coordinate], [Door_Y_Coordinate], [LeftWindow_Index], [LeftWindow_Position], [LeftWindow_X_Coordinate], [LeftWindow_Y_Coordinate], [SmallDoor_Index], [SmallDoor_Position], [SmallDoor_X_Coordinate], [SmallDoor_Y_Coordinate], [RightWindow_Index], [RightWindow_Position], [RightWindow_X_Coordinate], [RightWindow_Y_Coordinate], [Common_Position_Box_ID] from tb_Myroom_Greed")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MYROOM_GREED.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MYROOM_GREED row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Greed_X_Axis_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Greed_Y_Axis_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Greed_Z_Axis_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Door_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Door_Position, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Door_X_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Door_Y_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LeftWindow_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LeftWindow_Position, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LeftWindow_X_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LeftWindow_Y_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SmallDoor_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SmallDoor_Position, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SmallDoor_X_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SmallDoor_Y_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RightWindow_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RightWindow_Position, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RightWindow_X_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RightWindow_Y_Coordinate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Common_Position_Box_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MYROOM_GREED(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
