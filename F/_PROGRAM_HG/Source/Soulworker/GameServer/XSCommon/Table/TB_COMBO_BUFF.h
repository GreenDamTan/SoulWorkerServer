// 中文说明：
// 1. 本文件承接 TB_COMBO_BUFF 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_COMBO_BUFF {
    std::uint16_t Combo_Index = 0;
    std::uint8_t SV_Absorb = 0;
    std::uint16_t Fixed_Buff_ID = 0;
    std::uint8_t Get_RBuff_Count = 0;
    std::uint16_t RBuff_ID[8] = {};
    std::uint16_t RBuff_Rate[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_COMBO_BUFF) == 0x26, "TB_COMBO_BUFF size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_COMBO_BUFF* GetTB_COMBO_BUFF(std::uint16_t index) ;
    void SetTB_COMBO_BUFF(std::uint16_t index, const TB_COMBO_BUFF& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBComboBuffDB() ;
    std::unordered_map<std::uint16_t, TB_COMBO_BUFF> comboBuffRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_COMBO_BUFF* XResourceMgr::GetTB_COMBO_BUFF(std::uint16_t index) {
        auto it = comboBuffRows_.find(index);
        if (it == comboBuffRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_COMBO_BUFF(std::uint16_t index, const TB_COMBO_BUFF& row) {
        comboBuffRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBComboBuffDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Combo_Index], [SV_Absorb], [Fixed_Buff_ID], [Get_RBuff_Count], [RBuff_ID_00], [RBuff_Rate_00], [RBuff_ID_01], [RBuff_Rate_01], [RBuff_ID_02], [RBuff_Rate_02], [RBuff_ID_03], [RBuff_Rate_03], [RBuff_ID_04], [RBuff_Rate_04], [RBuff_ID_05], [RBuff_Rate_05], [RBuff_ID_06], [RBuff_Rate_06], [RBuff_ID_07], [RBuff_Rate_07] from tb_Combo_Buff")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        comboBuffRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_COMBO_BUFF row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combo_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SV_Absorb, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fixed_Buff_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Get_RBuff_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 8; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.RBuff_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RBuff_Rate[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_COMBO_BUFF(row.Combo_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
