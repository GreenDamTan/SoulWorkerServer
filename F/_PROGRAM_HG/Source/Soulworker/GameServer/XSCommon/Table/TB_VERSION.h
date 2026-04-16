// 中文说明：
// 1. 本文件承接 TB_VERSION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_VERSION {
    unsigned int ID = 0;
    unsigned int MainVer = 0;
    unsigned int SubVer = 0;
    unsigned int DataVer = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_VERSION) == 0x10, "TB_VERSION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_VERSION* GetTB_VERSION(unsigned int index) ;
    void SetTB_VERSION(unsigned int index, const TB_VERSION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBVersionDB() ;
    std::map<unsigned int, TB_VERSION> m_mapTB_VERSION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_VERSION* XResourceMgr::GetTB_VERSION(unsigned int index) {
        const auto it = m_mapTB_VERSION.find(index);
        return it == m_mapTB_VERSION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_VERSION(unsigned int index, const TB_VERSION& row) {
        m_mapTB_VERSION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBVersionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [MainVer], [SubVer], [DataVer] from tb_Version")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_VERSION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_VERSION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MainVer, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubVer, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DataVer, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_VERSION(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
