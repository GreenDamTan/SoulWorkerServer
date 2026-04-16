// 中文说明：
// 1. 本文件承接 TB_BROACHDATA 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BROACHDATA {
    unsigned int Broach_Index = 0;
    unsigned int Broach_ID = 0;
    std::uint8_t Broach_Type_Value = 0;
    std::uint8_t Broach_Rank_Value = 0;
    std::uint8_t Broach_Max_Cnt = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_BROACHDATA) == 0xB, "TB_BROACHDATA size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BROACHDATA* GetTB_BROACHDATA(unsigned int index) ;
    void SetTB_BROACHDATA(unsigned int index, const TB_BROACHDATA& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBroachDataDB() ;
    std::map<unsigned int, TB_BROACHDATA> m_mapTB_BROACHDATA;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BROACHDATA* XResourceMgr::GetTB_BROACHDATA(unsigned int index) {
        const auto it = m_mapTB_BROACHDATA.find(index);
        return it == m_mapTB_BROACHDATA.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BROACHDATA(unsigned int index, const TB_BROACHDATA& row) {
        m_mapTB_BROACHDATA[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBroachDataDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Broach_Index], [Broach_ID], [Broach_Type_Value], [Broach_Rank_Value], [Broach_Max_Cnt] from TB_BroachData")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BROACHDATA.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BROACHDATA row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_Type_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_Rank_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Broach_Max_Cnt, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BROACHDATA(row.Broach_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
