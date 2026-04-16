// 中文说明：
// 1. 本文件承接 TB_ITEM_CLASSIFY 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_CLASSIFY {
    unsigned int Item_Classify_Index = 0;
    std::uint8_t GroupID = 0;
    std::uint8_t SubGroupID = 0;
    std::uint8_t CategoryID = 0;
    std::uint8_t SubCategoryID = 0;
    std::uint8_t Item_Gain_Type = 0;
    std::uint8_t Item_Inven_Type = 0;
    std::uint8_t Item_Slot_Type = 0;
    std::uint8_t Item_Repair_Type = 0;
    std::uint8_t Item_Use_State = 0;
    std::uint8_t Item_Use_Type = 0;
    std::uint8_t Consume_Type = 0;
    std::uint16_t Exchange_Apply_Fee = 0;
    std::int16_t Classify_Align_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_CLASSIFY) == 0x13, "TB_ITEM_CLASSIFY size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_CLASSIFY* GetTB_ITEM_CLASSIFY(unsigned int index) ;
    void SetTB_ITEM_CLASSIFY(unsigned int index, const TB_ITEM_CLASSIFY& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemClassifyDB() ;
    bool LoadItemClassifyFile(const std::filesystem::path& basePath) ;
    std::unordered_map<unsigned int, TB_ITEM_CLASSIFY> itemClassifyRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_CLASSIFY* XResourceMgr::GetTB_ITEM_CLASSIFY(unsigned int index) {
        auto it = itemClassifyRows_.find(index);
        if (it == itemClassifyRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_CLASSIFY(unsigned int index, const TB_ITEM_CLASSIFY& row) {
        itemClassifyRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemClassifyDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [Item_Classify_Index], [GroupID], [SubGroupID], [CategoryID], [SubCategoryID], [Item_Gain_Type], [Item_Inven_Type], [Item_Slot_Type], [Item_Repair_Type], [Item_Use_State], [Item_Use_Type], [Consume_Type], [Exchange_Apply_Fee], [Classify_Align_Type] from tb_Item_Classify")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemClassifyRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_CLASSIFY row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Classify_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.GroupID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubGroupID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CategoryID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubCategoryID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Gain_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Inven_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Slot_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Repair_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Use_State, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Use_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Consume_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Exchange_Apply_Fee, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Classify_Align_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            SetTB_ITEM_CLASSIFY(row.Item_Classify_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadItemClassifyFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Item_Classify.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_Item_Classify";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_ITEM_CLASSIFY row{};
            if (!cursor.ReadCheckSumU32(&row.Item_Classify_Index) ||
                !cursor.ReadCheckSumU8(&row.GroupID) ||
                !cursor.ReadCheckSumU8(&row.SubGroupID) ||
                !cursor.ReadCheckSumU8(&row.CategoryID) ||
                !cursor.ReadCheckSumU8(&row.SubCategoryID) ||
                !cursor.ReadCheckSumU8(&row.Item_Gain_Type) ||
                !cursor.ReadCheckSumU8(&row.Item_Inven_Type) ||
                !cursor.ReadCheckSumU8(&row.Item_Slot_Type) ||
                !cursor.ReadCheckSumU8(&row.Item_Repair_Type) ||
                !cursor.ReadCheckSumU8(&row.Item_Use_State) ||
                !cursor.ReadCheckSumU8(&row.Item_Use_Type) ||
                !cursor.ReadCheckSumU8(&row.Consume_Type) ||
                !cursor.ReadCheckSumU16(&row.Exchange_Apply_Fee) ||
                !cursor.ReadCheckSumI16(&row.Classify_Align_Type)) {
                return false;
            }

            SetTB_ITEM_CLASSIFY(row.Item_Classify_Index, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        if (CheckSum(loader)) {
            return true;
        }

        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log DBLoadTable.h::XResourceMgr::LoadItemClassifyFile checksum-bypass rowCount=%d loadedRows=%zu expectedMd5=%s",
                            loader.m_nRowCount,
                            itemClassifyRows_.size(),
                            md5Value.c_str());
        return true;
    }

#endif
