// 中文说明：
// 1. 本文件承接 TB_PC_COSTUME 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 `Item_ID` 的语义改写成推测外键包装，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PC_COSTUME {
    unsigned int ID = 0;
    unsigned int Item_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_PC_COSTUME) == 0x8, "TB_PC_COSTUME size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PC_COSTUME* GetTB_PC_COSTUME(unsigned int index) ;
    void SetTB_PC_COSTUME(unsigned int index, const TB_PC_COSTUME& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPCCostumeDB() ;
    std::map<unsigned int, TB_PC_COSTUME> m_mapTB_PC_COSTUME;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PC_COSTUME* XResourceMgr::GetTB_PC_COSTUME(unsigned int index) {
        const auto it = m_mapTB_PC_COSTUME.find(index);
        return it == m_mapTB_PC_COSTUME.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PC_COSTUME(unsigned int index, const TB_PC_COSTUME& row) {
        m_mapTB_PC_COSTUME[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPCCostumeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Item_ID] from tb_PC_Costume")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PC_COSTUME.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PC_COSTUME row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PC_COSTUME(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
