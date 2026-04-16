// 中文说明：
// 1. 本文件承接 TB_NAMEFILTER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_NAMEFILTER {
    unsigned int Filter_Index = 0;
    std::uint8_t Filter_Type = 0;
    char Filter_Word[511] = {};
};
static_assert(sizeof(TB_NAMEFILTER) == 0x204, "TB_NAMEFILTER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    const std::unordered_map<unsigned int, TB_NAMEFILTER>& GetTB_NAMEFILTERRows() const ;
    void SetTB_NAMEFILTER(unsigned int index, const TB_NAMEFILTER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBNameFilterDB() ;
    std::unordered_map<unsigned int, TB_NAMEFILTER> nameFilterRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
const std::unordered_map<unsigned int, TB_NAMEFILTER>& XResourceMgr::GetTB_NAMEFILTERRows() const {
        return nameFilterRows_;
    }

void XResourceMgr::SetTB_NAMEFILTER(unsigned int index, const TB_NAMEFILTER& row) {
        nameFilterRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBNameFilterDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Filter_Index], [Filter_Type], [Filter_Word] from tb_NameFilter")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        nameFilterRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_NAMEFILTER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Filter_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Filter_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Filter_Word, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_NAMEFILTER(row.Filter_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
