// 中文说明：
// 1. 本文件承接 TB_INFINITE_TOWER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_INFINITE_TOWER {
    std::uint16_t Chapter_ID = 0;
    std::int16_t Restriction_Field_ID = 0;
    std::uint16_t uniMazeID[5] = {};
    unsigned int uniItemID[5] = {};
    std::uint8_t uniItemCnt[5] = {};
    unsigned int uniRewardBP[5] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_INFINITE_TOWER) == 0x3B, "TB_INFINITE_TOWER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_INFINITE_TOWER* GetTB_INFINITE_TOWER(std::uint16_t index) ;
    void SetTB_INFINITE_TOWER(std::uint16_t index, const TB_INFINITE_TOWER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBInfiniteTowerDB() ;
    std::unordered_map<std::uint16_t, TB_INFINITE_TOWER> infiniteTowerRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_INFINITE_TOWER* XResourceMgr::GetTB_INFINITE_TOWER(std::uint16_t index) {
        auto it = infiniteTowerRows_.find(index);
        if (it == infiniteTowerRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_INFINITE_TOWER(std::uint16_t index, const TB_INFINITE_TOWER& row) {
        infiniteTowerRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBInfiniteTowerDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Chapter_ID], [Restriction_Field_ID], [MazeID_Stage_01], [RewardItemID_Stage_01], [RewardItemCount_Stage_01], [RewardBP_Stage_01], [MazeID_Stage_02], [RewardItemID_Stage_02], [RewardItemCount_Stage_02], [RewardBP_Stage_02], [MazeID_Stage_03], [RewardItemID_Stage_03], [RewardItemCount_Stage_03], [RewardBP_Stage_03], [MazeID_Stage_04], [RewardItemID_Stage_04], [RewardItemCount_Stage_04], [RewardBP_Stage_04], [MazeID_Stage_05], [RewardItemID_Stage_05], [RewardItemCount_Stage_05], [RewardBP_Stage_05] from tb_Infinite_Tower")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        infiniteTowerRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_INFINITE_TOWER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Chapter_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Restriction_Field_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMazeID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemCnt[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRewardBP[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMazeID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemCnt[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRewardBP[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMazeID[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemID[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemCnt[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRewardBP[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMazeID[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemID[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemCnt[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRewardBP[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMazeID[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemID[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItemCnt[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRewardBP[4], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_INFINITE_TOWER(row.Chapter_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
