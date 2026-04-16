// 中文说明：
// 1. 本文件承接 TB_PROVIDE_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_PROVIDE_ITEM {
    std::uint16_t ID = 0;
    unsigned int uniItem_ID[40] = {};
    unsigned int uniItem_Count[40] = {};

    std::array<unsigned int, 40> GetItemIDs() const {
        std::array<unsigned int, 40> result{};
        std::memcpy(result.data(), uniItem_ID, sizeof(uniItem_ID));
        return result;
    }

    std::array<unsigned int, 40> GetItemCounts() const {
        std::array<unsigned int, 40> result{};
        std::memcpy(result.data(), uniItem_Count, sizeof(uniItem_Count));
        return result;
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_PROVIDE_ITEM) == 0x142, "TB_PROVIDE_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PROVIDE_ITEM* GetTB_PROVIDE_ITEM(std::uint16_t index) ;
    void SetTB_PROVIDE_ITEM(std::uint16_t index, const TB_PROVIDE_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBProvideItemDB() ;
    bool LoadProvideItemFile(const std::filesystem::path& basePath) ;
    std::unordered_map<std::uint16_t, TB_PROVIDE_ITEM> provideItemRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PROVIDE_ITEM* XResourceMgr::GetTB_PROVIDE_ITEM(std::uint16_t index) {
        auto it = provideItemRows_.find(index);
        if (it == provideItemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_PROVIDE_ITEM(std::uint16_t index, const TB_PROVIDE_ITEM& row) {
        provideItemRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBProvideItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [ID], [Item_ID_01], [Item_Count_01], [Item_ID_02], [Item_Count_02], [Item_ID_03], [Item_Count_03], [Item_ID_04], [Item_Count_04], [Item_ID_05], [Item_Count_05], [Item_ID_06], [Item_Count_06], [Item_ID_07], [Item_Count_07], [Item_ID_08], [Item_Count_08], [Item_ID_09], [Item_Count_09], [Item_ID_10], [Item_Count_10], [Item_ID_11], [Item_Count_11], [Item_ID_12], [Item_Count_12], [Item_ID_13], [Item_Count_13], [Item_ID_14], [Item_Count_14], [Item_ID_15], [Item_Count_15], [Item_ID_16], [Item_Count_16], [Item_ID_17], [Item_Count_17], [Item_ID_18], [Item_Count_18], [Item_ID_19], [Item_Count_19], [Item_ID_20], [Item_Count_20], [Item_ID_21], [Item_Count_21], [Item_ID_22], [Item_Count_22], [Item_ID_23], [Item_Count_23], [Item_ID_24], [Item_Count_24], [Item_ID_25], [Item_Count_25], [Item_ID_26], [Item_Count_26], [Item_ID_27], [Item_Count_27], [Item_ID_28], [Item_Count_28], [Item_ID_29], [Item_Count_29], [Item_ID_30], [Item_Count_30], [Item_ID_31], [Item_Count_31], [Item_ID_32], [Item_Count_32], [Item_ID_33], [Item_Count_33], [Item_ID_34], [Item_Count_34], [Item_ID_35], [Item_Count_35], [Item_ID_36], [Item_Count_36], [Item_ID_37], [Item_Count_37], [Item_ID_38], [Item_Count_38], [Item_ID_39], [Item_Count_39], [Item_ID_40], [Item_Count_40] from tb_Provide_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        provideItemRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PROVIDE_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            for (int index = 0; index < 40; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_Count[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }

            SetTB_PROVIDE_ITEM(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadProvideItemFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Provide_Item.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        unsigned int rowCount = 0;
        std::memcpy(&rowCount, buffer.data(), sizeof(rowCount));
        const std::size_t rowOffset = sizeof(rowCount);
        const std::size_t rowBytes = static_cast<std::size_t>(rowCount) * sizeof(TB_PROVIDE_ITEM);
        if (buffer.size() < rowOffset + rowBytes) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_PROVIDE_ITEM row{};
            std::memcpy(&row,
                        buffer.data() + rowOffset + static_cast<std::size_t>(index) * sizeof(TB_PROVIDE_ITEM),
                        sizeof(TB_PROVIDE_ITEM));
            SetTB_PROVIDE_ITEM(row.ID, row);
        }
        return true;
    }

#endif
