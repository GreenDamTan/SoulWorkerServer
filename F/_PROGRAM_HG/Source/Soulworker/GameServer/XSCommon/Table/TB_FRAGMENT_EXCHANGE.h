// 中文说明：
// 1. 本文件承接 TB_FRAGMENT_EXCHANGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_FRAGMENT_EXCHANGE {
    unsigned int S_Exchange_ID = 0;
    unsigned int S_Get_Item = 0;
    unsigned int S_GetItem_Count = 0;
    unsigned int S_Exchange_Count = 0;
    unsigned int uniMaterial[6] = {};
    unsigned int uniCount[6] = {};
    unsigned int uniCost[6] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_FRAGMENT_EXCHANGE) == 0x58, "TB_FRAGMENT_EXCHANGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_FRAGMENT_EXCHANGE* GetTB_FRAGMENT_EXCHANGE(unsigned int index) ;
    void SetTB_FRAGMENT_EXCHANGE(unsigned int index, const TB_FRAGMENT_EXCHANGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBFragmentExchangeDB() ;
    std::unordered_map<unsigned int, TB_FRAGMENT_EXCHANGE> fragmentExchangeRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_FRAGMENT_EXCHANGE* XResourceMgr::GetTB_FRAGMENT_EXCHANGE(unsigned int index) {
        auto it = fragmentExchangeRows_.find(index);
        if (it == fragmentExchangeRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_FRAGMENT_EXCHANGE(unsigned int index, const TB_FRAGMENT_EXCHANGE& row) {
        fragmentExchangeRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBFragmentExchangeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [S_Exchange_ID], [S_Get_Item], [S_GetItem_Count], [S_Exchange_Count], [S_Exchange01_Material], [S_Exchange01_MaterialCount], [S_Exchange01_Gold], [S_Exchange02_Material], [S_Exchange02_MaterialCount], [S_Exchange02_Gold], [S_Exchange03_Material], [S_Exchange03_MaterialCount], [S_Exchange03_Gold], [S_Exchange04_Material], [S_Exchange04_MaterialCount], [S_Exchange04_Gold], [S_Exchange05_Material], [S_Exchange05_MaterialCount], [S_Exchange05_Gold], [S_Exchange06_Material], [S_Exchange06_MaterialCount], [S_Exchange06_Gold] from tb_Fragment_Exchange")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        fragmentExchangeRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_FRAGMENT_EXCHANGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Exchange_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Get_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_GetItem_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Exchange_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCost[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCost[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCost[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCost[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCost[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaterial[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCost[5], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_FRAGMENT_EXCHANGE(row.S_Exchange_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
