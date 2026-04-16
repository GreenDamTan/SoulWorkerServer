// 中文说明：
// 1. 本文件承接 TB_SYSTEMMAIL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SYSTEMMAIL {
    std::uint8_t SysMail_ID = 0;
    std::uint16_t SysMail_TypeID = 0;
    std::uint16_t SysMail_TypeTargetID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SYSTEMMAIL) == 0x5, "TB_SYSTEMMAIL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SYSTEMMAIL* GetTB_SYSTEMMAIL(std::uint8_t index) ;
    void SetTB_SYSTEMMAIL(std::uint8_t index, const TB_SYSTEMMAIL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSystemMailDB() ;
    std::map<std::uint8_t, TB_SYSTEMMAIL> m_mapTB_SYSTEMMAIL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SYSTEMMAIL* XResourceMgr::GetTB_SYSTEMMAIL(std::uint8_t index) {
        const auto it = m_mapTB_SYSTEMMAIL.find(index);
        return it == m_mapTB_SYSTEMMAIL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SYSTEMMAIL(std::uint8_t index, const TB_SYSTEMMAIL& row) {
        m_mapTB_SYSTEMMAIL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSystemMailDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [SysMail_ID], [SysMail_TypeID], [SysMail_TypeTargetID] from tb_SystemMail")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SYSTEMMAIL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SYSTEMMAIL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SysMail_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SysMail_TypeID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SysMail_TypeTargetID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SYSTEMMAIL(row.SysMail_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
