// 中文说明：
// 1. 本文件承接 TB_SOCKET 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SOCKET {
    unsigned int Socket_ID = 0;
    std::uint8_t Socket_Type = 0;
    std::uint8_t Max_Socket = 0;
    std::uint8_t Fix_Socket = 0;
    std::uint8_t Random_Socket = 0;
    std::uint16_t R1_Chance = 0;
    std::uint16_t R2_Chance = 0;
    std::uint16_t R3_Chance = 0;
    unsigned int Euqip_Cost = 0;
    unsigned int Euqip_Item = 0;
    std::uint16_t Euqip_Count = 0;
    unsigned int Remove_Cost = 0;
    unsigned int Remove_Item = 0;
    std::uint16_t Remove_Count = 0;
    unsigned int Add_Cost = 0;
    unsigned int Add_Item = 0;
    std::uint16_t Add_Count = 0;
    unsigned int Extraction_Item = 0;
    std::uint16_t Extraction_Item_Count = 0;

    std::array<std::uint16_t, 3> GetRandomChances() const {
        return {R1_Chance, R2_Chance, R3_Chance};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_SOCKET) == 0x32, "TB_SOCKET size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SOCKET* GetTB_SOCKET(unsigned int index) ;
    void SetTB_SOCKET(unsigned int index, const TB_SOCKET& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSocketDB() ;
    std::unordered_map<unsigned int, TB_SOCKET> socketRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SOCKET* XResourceMgr::GetTB_SOCKET(unsigned int index) {
        auto it = socketRows_.find(index);
        if (it == socketRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_SOCKET(unsigned int index, const TB_SOCKET& row) {
        socketRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSocketDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Socket_ID], [Socket_Type], [Max_Socket], [Fix_Socket], [Random_Socket], [R1_Chance], [R2_Chance], [R3_Chance], [Euqip_Cost], [Euqip_Item], [Euqip_Count], [Remove_Cost], [Remove_Item], [Remove_Count], [Add_Cost], [Add_Item], [Add_Count], [Extraction_Item], [Extraction_Item_Count] from tb_Socket")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        socketRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SOCKET row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Socket_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Socket_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Socket, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Socket, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Random_Socket, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.R1_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.R2_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.R3_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Euqip_Cost, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Euqip_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Euqip_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Cost, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Cost, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_Item_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SOCKET(row.Socket_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
