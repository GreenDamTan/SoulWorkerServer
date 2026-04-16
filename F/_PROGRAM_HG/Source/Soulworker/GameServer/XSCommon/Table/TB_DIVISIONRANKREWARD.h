// 中文说明：
// 1. 本文件承接 TB_DIVISIONRANKREWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DIVISIONRANKREWARD {
    unsigned int ID = 0;
    unsigned int GroupID = 0;
    std::uint8_t RewardType = 0;
    std::uint8_t GiveType = 0;
    unsigned int RewardValue_Min = 0;
    unsigned int RewardValue_Max = 0;
    unsigned int Drop_Index = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DIVISIONRANKREWARD) == 0x16, "TB_DIVISIONRANKREWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DIVISIONRANKREWARD* GetTB_DIVISIONRANKREWARD(unsigned int index) ;
    void SetTB_DIVISIONRANKREWARD(unsigned int index, const TB_DIVISIONRANKREWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDivisionRankRewardDB() ;
    std::unordered_map<unsigned int, TB_DIVISIONRANKREWARD> divisionRankRewardRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DIVISIONRANKREWARD* XResourceMgr::GetTB_DIVISIONRANKREWARD(unsigned int index) {
        auto it = divisionRankRewardRows_.find(index);
        if (it == divisionRankRewardRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DIVISIONRANKREWARD(unsigned int index, const TB_DIVISIONRANKREWARD& row) {
        divisionRankRewardRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDivisionRankRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [GroupID], [RewardType], [GiveType], [RewardValue_Min], [RewardValue_Max], [Drop_Index] from tb_DivisionRankReward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        divisionRankRewardRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DIVISIONRANKREWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.GroupID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RewardType, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.GiveType, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RewardValue_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RewardValue_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Drop_Index, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DIVISIONRANKREWARD(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
