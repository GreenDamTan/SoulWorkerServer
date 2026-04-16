// 中文说明：
// 1. 本文件承接 TB_ITEM_TITLE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_TITLE {
    unsigned int Title_Group_ID = 0;
    unsigned int Group_ID01 = 0;
    unsigned int Group_ID02 = 0;
    unsigned int Group_ID03 = 0;
    unsigned int Group_ID04 = 0;
    unsigned int Group_ID05 = 0;
    unsigned int Group_ID06 = 0;
    unsigned int Group_ID07 = 0;
    unsigned int Group_ID08 = 0;
    unsigned int Group_ID09 = 0;
    unsigned int Group_ID10 = 0;
    std::uint16_t Group_rate01 = 0;
    std::uint16_t Group_rate02 = 0;
    std::uint16_t Group_rate03 = 0;
    std::uint16_t Group_rate04 = 0;
    std::uint16_t Group_rate05 = 0;
    std::uint16_t Group_rate06 = 0;
    std::uint16_t Group_rate07 = 0;
    std::uint16_t Group_rate08 = 0;
    std::uint16_t Group_rate09 = 0;
    std::uint16_t Group_rate10 = 0;

    std::array<unsigned int, 10> GetGroupIDs() const {
        return {Group_ID01, Group_ID02, Group_ID03, Group_ID04, Group_ID05,
                Group_ID06, Group_ID07, Group_ID08, Group_ID09, Group_ID10};
    }

    std::array<std::uint16_t, 10> GetGroupRates() const {
        return {Group_rate01, Group_rate02, Group_rate03, Group_rate04, Group_rate05,
                Group_rate06, Group_rate07, Group_rate08, Group_rate09, Group_rate10};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_TITLE) == 0x40, "TB_ITEM_TITLE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_TITLE* GetTB_ITEM_TITLE(unsigned int index) ;
    void SetTB_ITEM_TITLE(unsigned int index, const TB_ITEM_TITLE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemTitleDB() ;
    std::unordered_map<unsigned int, TB_ITEM_TITLE> itemTitleRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_TITLE* XResourceMgr::GetTB_ITEM_TITLE(unsigned int index) {
        auto it = itemTitleRows_.find(index);
        if (it == itemTitleRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_TITLE(unsigned int index, const TB_ITEM_TITLE& row) {
        itemTitleRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemTitleDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Title_Group_ID], [Group_ID01], [Group_rate01], [Group_ID02], [Group_rate02], [Group_ID03], [Group_rate03], [Group_ID04], [Group_rate04], [Group_ID05], [Group_rate05], [Group_ID06], [Group_rate06], [Group_ID07], [Group_rate07], [Group_ID08], [Group_rate08], [Group_ID09], [Group_rate09], [Group_ID10], [Group_rate10] from tb_item_title")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemTitleRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_TITLE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Title_Group_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            auto groupIds = std::array<unsigned int*, 10>{&row.Group_ID01,&row.Group_ID02,&row.Group_ID03,&row.Group_ID04,&row.Group_ID05,&row.Group_ID06,&row.Group_ID07,&row.Group_ID08,&row.Group_ID09,&row.Group_ID10};
            auto groupRates = std::array<std::uint16_t*, 10>{&row.Group_rate01,&row.Group_rate02,&row.Group_rate03,&row.Group_rate04,&row.Group_rate05,&row.Group_rate06,&row.Group_rate07,&row.Group_rate08,&row.Group_rate09,&row.Group_rate10};
            for (int index = 0; index < 10; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, groupIds[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, groupRates[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_TITLE(row.Title_Group_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
