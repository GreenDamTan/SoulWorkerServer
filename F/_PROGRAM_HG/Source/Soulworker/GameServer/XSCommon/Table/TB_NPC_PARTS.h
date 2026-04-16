// 中文说明：
// 1. 本文件承接 TB_NPC_PARTS 的单表还原片段，保持原版最小 `ID` 布局。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要因为字段极少就并回别的表文件；它是原版独立表。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_NPC_PARTS {
    unsigned int ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_NPC_PARTS) == 0x4, "TB_NPC_PARTS size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_NPC_PARTS* GetTB_NPC_PARTS(unsigned int index) ;
    void SetTB_NPC_PARTS(unsigned int index, const TB_NPC_PARTS& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBNpcPartsDB() ;
    std::map<unsigned int, TB_NPC_PARTS> m_mapTB_NPC_PARTS;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_NPC_PARTS* XResourceMgr::GetTB_NPC_PARTS(unsigned int index) {
        const auto it = m_mapTB_NPC_PARTS.find(index);
        return it == m_mapTB_NPC_PARTS.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_NPC_PARTS(unsigned int index, const TB_NPC_PARTS& row) {
        m_mapTB_NPC_PARTS[index] = row;
    }

std::int64_t XResourceMgr::LoadTBNpcPartsDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID] from tb_NPC_Parts")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_NPC_PARTS.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_NPC_PARTS row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_NPC_PARTS(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
