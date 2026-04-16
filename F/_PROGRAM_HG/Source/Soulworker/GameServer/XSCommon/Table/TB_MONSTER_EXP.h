// 中文说明：
// 1. 本文件承接 TB_MONSTER_EXP 的单表还原片段，保持原版 1-byte key + 7 组经验值布局。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏多次包含。
// 3. 不要把 `Level` 或 `uniMExp` 改成其它抽象结构，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MONSTER_EXP {
    std::uint8_t ID = 0;
    std::uint8_t Level = 0;
    int uniMExp[7] = {};
};
#pragma pack(pop)

static_assert(sizeof(TB_MONSTER_EXP) == 0x1E, "TB_MONSTER_EXP size must match decompile layout");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MONSTER_EXP* GetTB_MONSTER_EXP(std::uint8_t index) ;
    void SetTB_MONSTER_EXP(std::uint8_t index, const TB_MONSTER_EXP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMonsterExpDB() ;
    std::map<std::uint8_t, TB_MONSTER_EXP> m_mapTB_MONSTER_EXP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MONSTER_EXP* XResourceMgr::GetTB_MONSTER_EXP(std::uint8_t index) {
        const auto it = m_mapTB_MONSTER_EXP.find(index);
        return it == m_mapTB_MONSTER_EXP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MONSTER_EXP(std::uint8_t index, const TB_MONSTER_EXP& row) {
        m_mapTB_MONSTER_EXP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMonsterExpDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Level], [EXP_Slave], [EXP_Normal], [EXP_Elite], [EXP_Named], [EXP_Boss], [EXP_Raid], [EXP_Summon] from tb_Monster_EXP")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MONSTER_EXP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MONSTER_EXP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Level, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 7; ++index) {
                if (!GreenDamTan_DBGetSigned(xDBBinder, &row.uniMExp[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_MONSTER_EXP(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
