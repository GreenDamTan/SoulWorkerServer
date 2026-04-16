// 中文说明：
// 1. 本文件承接 TB_REINFORCE_EXCHANGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_REINFORCE_EXCHANGE {
    unsigned int R_Exchange_ID = 0;
    std::uint8_t NPC_Type = 0;
    unsigned int M_Item = 0;
    std::uint8_t M_Count = 0;
    unsigned int Ex_Item = 0;
    std::uint8_t Ex_Count = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_REINFORCE_EXCHANGE) == 0xF, "TB_REINFORCE_EXCHANGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_REINFORCE_EXCHANGE* GetTB_REINFORCE_EXCHANGE(unsigned int index) ;
    void SetTB_REINFORCE_EXCHANGE(unsigned int index, const TB_REINFORCE_EXCHANGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBReinforceExchangeDB() ;
    std::map<unsigned int, TB_REINFORCE_EXCHANGE> m_mapTB_REINFORCE_EXCHANGE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_REINFORCE_EXCHANGE* XResourceMgr::GetTB_REINFORCE_EXCHANGE(unsigned int index) {
        const auto it = m_mapTB_REINFORCE_EXCHANGE.find(index);
        return it == m_mapTB_REINFORCE_EXCHANGE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_REINFORCE_EXCHANGE(unsigned int index, const TB_REINFORCE_EXCHANGE& row) {
        m_mapTB_REINFORCE_EXCHANGE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBReinforceExchangeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [R_Exchange_ID], [NPC_Type], [M_Item], [M_Count], [Ex_Item], [Ex_Count] from tb_Reinforce_Exchange")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_REINFORCE_EXCHANGE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_REINFORCE_EXCHANGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.R_Exchange_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ex_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ex_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_REINFORCE_EXCHANGE(row.R_Exchange_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
