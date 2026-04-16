// 中文说明：
// 1. 本文件承接 TB_TRANSPORT_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_TRANSPORT_INFO {
    std::uint16_t ID = 0;
    std::uint8_t Transport_Type = 0;
    std::uint8_t Arrival_Type = 0;
    std::uint16_t Arrival_ID = 0;
    std::uint8_t Transport_Area_Type = 0;
    unsigned int Need_Gold = 0;
    std::uint8_t Check_Level = 0;
    unsigned int Check_Quest = 0;
    unsigned int Check_Achivement = 0;
    char Path_File_Name[511] = {};
    std::uint8_t Path_Direction_Type = 0;
    unsigned int Monster_ID = 0;
    std::int16_t Position_Box_ID = 0;
    std::int16_t Transport_Speed = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_TRANSPORT_INFO) == 0x21C, "TB_TRANSPORT_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_TRANSPORT_INFO* GetTB_TRANSPORT_INFO(std::uint16_t index) ;
    void SetTB_TRANSPORT_INFO(std::uint16_t index, const TB_TRANSPORT_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBTransportInfoDB() ;
    std::map<std::uint16_t, TB_TRANSPORT_INFO> m_mapTB_TRANSPORT_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_TRANSPORT_INFO* XResourceMgr::GetTB_TRANSPORT_INFO(std::uint16_t index) {
        const auto it = m_mapTB_TRANSPORT_INFO.find(index);
        return it == m_mapTB_TRANSPORT_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_TRANSPORT_INFO(std::uint16_t index, const TB_TRANSPORT_INFO& row) {
        m_mapTB_TRANSPORT_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBTransportInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Transport_Type], [Arrival_Type], [Arrival_ID], [Transport_Area_Type], [Need_Gold], [Check_Level], [Check_Quest], [Check_Achivement], [Path_File_Name], [Path_Direction_Type], [Monster_ID], [Position_Box_ID], [Transport_Speed] from tb_Transport_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_TRANSPORT_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_TRANSPORT_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Arrival_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Arrival_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Transport_Area_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Quest, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Achivement, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Path_File_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Path_Direction_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Position_Box_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Transport_Speed, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_TRANSPORT_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
