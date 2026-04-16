// 中文说明：
// 1. 本文件承接 TB_COLOR 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_COLOR {
    unsigned int Color_Index = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_COLOR) == 0x4, "TB_COLOR size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_COLOR* GetTB_COLOR(unsigned int index) ;
    void SetTB_COLOR(unsigned int index, const TB_COLOR& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBColorDB() ;
    std::unordered_map<unsigned int, TB_COLOR> colorRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_COLOR* XResourceMgr::GetTB_COLOR(unsigned int index) {
        auto it = colorRows_.find(index);
        if (it == colorRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_COLOR(unsigned int index, const TB_COLOR& row) {
        colorRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBColorDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [Color_Index] from tb_Color")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        colorRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_COLOR row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Color_Index, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_COLOR(row.Color_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
