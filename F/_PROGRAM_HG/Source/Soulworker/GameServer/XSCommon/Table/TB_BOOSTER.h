// 中文说明：
// 1. 本文件承接 TB_BOOSTER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BOOSTER {
    std::uint16_t Booster_Index = 0;
    std::uint16_t Booster_Group = 0;
    std::uint8_t uniEffectType[8] = {};
    std::uint8_t uniApplyType[8] = {};
    float uniEffectValue[8] = {};
    std::uint16_t uniEffectString[8] = {};
    char Booster_Info[511] = {};
    std::uint8_t Decrease_Condition = 0;
    unsigned int Booster_Time = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_BOOSTER) == 0x248, "TB_BOOSTER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BOOSTER* GetTB_BOOSTER(std::uint16_t index) ;
    void SetTB_BOOSTER(std::uint16_t index, const TB_BOOSTER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBoosterDB() ;
    std::map<std::uint16_t, TB_BOOSTER> m_mapTB_BOOSTER;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BOOSTER* XResourceMgr::GetTB_BOOSTER(std::uint16_t index) {
        const auto it = m_mapTB_BOOSTER.find(index);
        return it == m_mapTB_BOOSTER.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BOOSTER(std::uint16_t index, const TB_BOOSTER& row) {
        m_mapTB_BOOSTER[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBoosterDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Booster_Index], [Booster_Group], [EffectType_01], [ApplyType_01], [EffectValue_01], [EffectString_01], [EffectType_02], [ApplyType_02], [EffectValue_02], [EffectString_02], [EffectType_03], [ApplyType_03], [EffectValue_03], [EffectString_03], [EffectType_04], [ApplyType_04], [EffectValue_04], [EffectString_04], [EffectType_05], [ApplyType_05], [EffectValue_05], [EffectString_05], [EffectType_06], [ApplyType_06], [EffectValue_06], [EffectString_06], [EffectType_07], [ApplyType_07], [EffectValue_07], [EffectString_07], [EffectType_08], [ApplyType_08], [EffectValue_08], [EffectString_08], [Booster_Info], [Decrease_Condition], [Booster_Time] from tb_Booster")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BOOSTER.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BOOSTER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[0], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[1], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[2], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[3], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[4], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[5], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[6], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectType[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniApplyType[7], &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.uniEffectValue[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEffectString[7], &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Booster_Info, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Decrease_Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Time, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BOOSTER(row.Booster_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
