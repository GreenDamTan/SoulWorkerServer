// 中文说明：
// 1. 本文件承接 TB_HELPER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_HELPER {
    unsigned int ID = 0;
    std::uint8_t Helper_Number = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_HELPER) == 0x5, "TB_HELPER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_HELPER* GetTB_HELPER(unsigned int index) ;
    void SetTB_HELPER(unsigned int index, const TB_HELPER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBHelperDB() ;
    std::unordered_map<unsigned int, TB_HELPER> helperRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_HELPER* XResourceMgr::GetTB_HELPER(unsigned int index) {
        auto it = helperRows_.find(index);
        if (it == helperRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_HELPER(unsigned int index, const TB_HELPER& row) {
        helperRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBHelperDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Helper_Number] from tb_Helper")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        helperRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_HELPER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Helper_Number, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_HELPER(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
