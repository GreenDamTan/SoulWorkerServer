// 中文说明：
// 1. 本文件承接 TB_MONSTER_BROKEN_PARTS 的单表还原片段，字段与装载顺序按 PDB / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把破坏部位的三段动画/链接字段随意折叠或改名，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MONSTER_BROKEN_PARTS {
    unsigned int ID = 0;
    char Anim_File[511] = {};
    unsigned int Type = 0;
    unsigned int HP_Pct = 0;
    unsigned int Spawn_Monster_ID = 0;
    char Spawn_Ani[511] = {};
    unsigned int Link_Parts = 0;
    unsigned int Link_Monster_ID = 0;
    char Link_Monster_Parts_Ani[511] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_MONSTER_BROKEN_PARTS) == 0x615, "TB_MONSTER_BROKEN_PARTS size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MONSTER_BROKEN_PARTS* GetTB_MONSTER_BROKEN_PARTS(unsigned int index) ;
    void SetTB_MONSTER_BROKEN_PARTS(unsigned int index, const TB_MONSTER_BROKEN_PARTS& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMonsterBrokenPartsDB() ;
    std::map<unsigned int, TB_MONSTER_BROKEN_PARTS> m_mapTB_MONSTER_BROKEN_PARTS;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MONSTER_BROKEN_PARTS* XResourceMgr::GetTB_MONSTER_BROKEN_PARTS(unsigned int index) {
        const auto it = m_mapTB_MONSTER_BROKEN_PARTS.find(index);
        return it == m_mapTB_MONSTER_BROKEN_PARTS.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MONSTER_BROKEN_PARTS(unsigned int index, const TB_MONSTER_BROKEN_PARTS& row) {
        m_mapTB_MONSTER_BROKEN_PARTS[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMonsterBrokenPartsDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Anim_File], [Type], [HP_Pct], [Spawn_Monster_ID], [Spawn_Ani], [Link_Parts], [Link_Monster_ID], [Link_Monster_Parts_Ani] from tb_Monster_Broken_Parts")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MONSTER_BROKEN_PARTS.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MONSTER_BROKEN_PARTS row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Anim_File, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.HP_Pct, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Spawn_Monster_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Spawn_Ani, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Parts, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Monster_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Link_Monster_Parts_Ani, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MONSTER_BROKEN_PARTS(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
