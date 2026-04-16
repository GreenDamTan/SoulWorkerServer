// 中文说明：
// 1. 本文件承接 TB_SERVERINFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SERVERINFO {
    unsigned int Server_Index = 0;
    char Server_Name[511] = {};
    std::uint8_t Server_Type = 0;
    std::uint8_t PVP_Enable = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SERVERINFO) == 0x205, "TB_SERVERINFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SERVERINFO* GetTB_SERVERINFO(unsigned int index) ;
    void SetTB_SERVERINFO(unsigned int index, const TB_SERVERINFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBServerInfoDB() ;
    std::map<unsigned int, TB_SERVERINFO> m_mapTB_SERVERINFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SERVERINFO* XResourceMgr::GetTB_SERVERINFO(unsigned int index) {
        const auto it = m_mapTB_SERVERINFO.find(index);
        return it == m_mapTB_SERVERINFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SERVERINFO(unsigned int index, const TB_SERVERINFO& row) {
        m_mapTB_SERVERINFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBServerInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Server_Index], [Server_Name], [Server_Type], [PVP_Enable] from tb_ServerInfo")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SERVERINFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SERVERINFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Server_Index, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Server_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Server_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.PVP_Enable, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SERVERINFO(row.Server_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
