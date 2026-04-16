// 中文说明：
// 1. 本文件承接 TB_AKASHIC_SLOT_EXTEND 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AKASHIC_SLOT_EXTEND {
    std::uint8_t Extend_Index = 0;
    unsigned int Extend_Akashic_Need_Item_ID = 0;
    std::uint8_t Extend_Akashic_Need_Item_Num = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_AKASHIC_SLOT_EXTEND) == 0x6, "TB_AKASHIC_SLOT_EXTEND size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AKASHIC_SLOT_EXTEND* GetTB_AKASHIC_SLOT_EXTEND(std::uint8_t index) ;
    void SetTB_AKASHIC_SLOT_EXTEND(std::uint8_t index, const TB_AKASHIC_SLOT_EXTEND& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAkashicSlotExtendDB() ;
    std::map<std::uint8_t, TB_AKASHIC_SLOT_EXTEND> m_mapTB_AKASHIC_SLOT_EXTEND;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AKASHIC_SLOT_EXTEND* XResourceMgr::GetTB_AKASHIC_SLOT_EXTEND(std::uint8_t index) {
        const auto it = m_mapTB_AKASHIC_SLOT_EXTEND.find(index);
        return it == m_mapTB_AKASHIC_SLOT_EXTEND.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AKASHIC_SLOT_EXTEND(std::uint8_t index, const TB_AKASHIC_SLOT_EXTEND& row) {
        m_mapTB_AKASHIC_SLOT_EXTEND[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAkashicSlotExtendDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Extend_Index], [Extend_Akashic_Need_Item_ID], [Extend_Akashic_Need_Item_Num] from Tb_Akashic_Slot_Extend")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AKASHIC_SLOT_EXTEND.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AKASHIC_SLOT_EXTEND row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Akashic_Need_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Akashic_Need_Item_Num, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AKASHIC_SLOT_EXTEND(row.Extend_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
