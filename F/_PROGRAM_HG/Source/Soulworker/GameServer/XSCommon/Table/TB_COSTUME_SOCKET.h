// 中文说明：
// 1. 本文件承接 TB_COSTUME_SOCKET 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_COSTUME_SOCKET {
    unsigned int Socket_ID = 0;
    std::uint8_t Socket_Piece_EA = 0;
    std::uint8_t Fix_Socket1 = 0;
    std::uint8_t Fix_Socket2 = 0;
    std::uint8_t Fix_Socket3 = 0;
    std::uint8_t Fix_Socket4 = 0;
    std::uint8_t Fix_Socket5 = 0;
    std::uint8_t Extend_Socket1 = 0;
    std::uint8_t Extend_Socket2 = 0;
    std::uint8_t Extend_Socket3 = 0;
    std::uint8_t Extend_Socket4 = 0;
    std::uint8_t Extend_Socket5 = 0;
    unsigned int Add_Item = 0;
    std::uint16_t Add_Count = 0;

    std::array<std::uint8_t, 5> GetFixSocketCounts() const {
        return {Fix_Socket1, Fix_Socket2, Fix_Socket3, Fix_Socket4, Fix_Socket5};
    }

    std::array<std::uint8_t, 5> GetExtendSocketCounts() const {
        return {Extend_Socket1, Extend_Socket2, Extend_Socket3, Extend_Socket4, Extend_Socket5};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_COSTUME_SOCKET) == 0x15, "TB_COSTUME_SOCKET size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_COSTUME_SOCKET* GetTB_COSTUME_SOCKET(unsigned int index) ;
    void SetTB_COSTUME_SOCKET(unsigned int index, const TB_COSTUME_SOCKET& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCostumeSocketDB() ;
    std::unordered_map<unsigned int, TB_COSTUME_SOCKET> costumeSocketRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_COSTUME_SOCKET* XResourceMgr::GetTB_COSTUME_SOCKET(unsigned int index) {
        auto it = costumeSocketRows_.find(index);
        if (it == costumeSocketRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_COSTUME_SOCKET(unsigned int index, const TB_COSTUME_SOCKET& row) {
        costumeSocketRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCostumeSocketDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Socket_ID], [Socket_Piece_EA], [Fix_Socket1], [Fix_Socket2], [Fix_Socket3], [Fix_Socket4], [Fix_Socket5], [Extend_Socket1], [Extend_Socket2], [Extend_Socket3], [Extend_Socket4], [Extend_Socket5], [Add_Item], [Add_Count] from tb_Costume_Socket")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        costumeSocketRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_COSTUME_SOCKET row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Socket_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Socket_Piece_EA, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Socket1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Socket2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Socket3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Socket4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fix_Socket5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Socket1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Socket2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Socket3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Socket4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Socket5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_COSTUME_SOCKET(row.Socket_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
