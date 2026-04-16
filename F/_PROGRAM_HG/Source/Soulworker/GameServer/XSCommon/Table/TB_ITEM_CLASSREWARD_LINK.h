// 中文说明：
// 1. 本文件承接 TB_ITEM_CLASSREWARD_LINK 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_CLASSREWARD_LINK {
    std::uint16_t Box_ID = 0;
    std::uint32_t uniPC_BoxItem[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_CLASSREWARD_LINK) == 0x22, "TB_ITEM_CLASSREWARD_LINK size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_CLASSREWARD_LINK* GetTB_ITEM_CLASSREWARD_LINK(std::uint16_t index) ;
    void SetTB_ITEM_CLASSREWARD_LINK(std::uint16_t index, const TB_ITEM_CLASSREWARD_LINK& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemClassRewardLinkDB() ;
    std::unordered_map<std::uint16_t, TB_ITEM_CLASSREWARD_LINK> itemClassRewardLinkRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_CLASSREWARD_LINK* XResourceMgr::GetTB_ITEM_CLASSREWARD_LINK(std::uint16_t index) {
        auto it = itemClassRewardLinkRows_.find(index);
        if (it == itemClassRewardLinkRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_CLASSREWARD_LINK(std::uint16_t index, const TB_ITEM_CLASSREWARD_LINK& row) {
        itemClassRewardLinkRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemClassRewardLinkDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Box_ID], [PC_A_BoXItem], [PC_B_BoXItem], [PC_C_BoXItem], [PC_D_BoXItem], [PC_E_BoXItem], [PC_F_BoXItem], [PC_G_BoXItem], [PC_H_BoXItem] from tb_Item_Classreward_Link")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemClassRewardLinkRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_CLASSREWARD_LINK row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Box_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 8; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniPC_BoxItem[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_CLASSREWARD_LINK(row.Box_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
