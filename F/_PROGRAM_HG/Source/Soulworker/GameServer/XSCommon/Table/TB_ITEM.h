// 中文说明：
// 1. 本文件承接 TB_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM {
    unsigned int Item_ID = 0;
    unsigned int Item_Classify_Index = 0;
    std::uint8_t Item_Rank = 0;
    std::uint16_t Item_Lv = 0;
    unsigned int Sell_Price = 0;
    unsigned int Buy_Price = 0;
    unsigned int Recycle_Sell_Price = 0;
    unsigned int Recycle_Buy_Price = 0;
    std::uint16_t Item_Stack_Max = 0;
    std::uint8_t Item_Bind_Type = 0;
    unsigned int Item_Model_ID = 0;
    char Item_NameS[511] = {};
    std::uint16_t Item_Limit_Lv = 0;
    std::uint8_t Item_Limit_Class = 0;
    std::uint8_t Item_Limit_Sell_Type = 0;
    std::uint8_t Item_Sub_Type = 0;
    std::uint8_t Item_CostumeSet = 0;
    unsigned int Item_CostumeSet_ID = 0;
    char Item_Slot_Disable[511] = {};
    std::uint8_t Item_Endurance_Max = 0;
    std::uint8_t Item_Use_Value = 0;
    unsigned int Item_physical_Attack_Min = 0;
    unsigned int Item_physical_Attack = 0;
    unsigned int Item_Magic_Attack = 0;
    unsigned int Item_physical_Defense_Min = 0;
    unsigned int Item_physical_Defense = 0;
    unsigned int Item_Magic_Defense = 0;
    std::uint8_t Option_Class_1 = 0;
    std::uint8_t Option_Class_2 = 0;
    std::uint8_t Option_Class_3 = 0;
    std::uint8_t Option_Class_4 = 0;
    std::uint8_t Option_Class_5 = 0;
    unsigned int S_Option_Type_1 = 0;
    unsigned int S_Option_Type_2 = 0;
    unsigned int S_Option_Type_3 = 0;
    unsigned int S_Option_Type_4 = 0;
    unsigned int S_Option_Type_5 = 0;
    int S_Option_Value_1 = 0;
    int S_Option_Value_2 = 0;
    int S_Option_Value_3 = 0;
    int S_Option_Value_4 = 0;
    int S_Option_Value_5 = 0;
    unsigned int Item_Ex_Option_ID = 0;
    unsigned int Item_Socket_ID = 0;
    unsigned int Item_SetItem_ID = 0;
    std::uint16_t Item_Reinforce_ID = 0;
    unsigned int Item_Reinforce_Option_ID = 0;
    unsigned int Item_Title_ID = 0;
    unsigned int Item_Evolution_ID = 0;
    std::uint16_t Item_Disassemble_ID = 0;
    unsigned int Furniture_ID = 0;
    std::uint16_t Cooltime_Group = 0;
    unsigned int Cooltime_Value = 0;
    std::uint8_t Cooltime_Save = 0;
    std::uint16_t Item_Effect_Type = 0;
    unsigned int Item_Effect_ID = 0;
    std::uint8_t Item_Cash = 0;
    std::uint8_t Item_Use_Period_Type = 0;
    unsigned int Item_Use_Period_Value = 0;
    std::uint8_t Sealing_Cnt = 0;
    std::uint8_t Break_Cnt = 0;
    unsigned int Item_SimilarGroup_ID = 0;
    unsigned int RepackageItem_ID = 0;

    std::array<std::uint8_t, 5> GetOptionClasses() const {
        return {Option_Class_1, Option_Class_2, Option_Class_3, Option_Class_4, Option_Class_5};
    }

    std::array<unsigned int, 5> GetOptionTypes() const {
        return {S_Option_Type_1, S_Option_Type_2, S_Option_Type_3, S_Option_Type_4, S_Option_Type_5};
    }

    std::array<int, 5> GetOptionValues() const {
        return {S_Option_Value_1, S_Option_Value_2, S_Option_Value_3, S_Option_Value_4, S_Option_Value_5};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM) == 0x4AE, "TB_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM* GetTB_ITEM(unsigned int index) ;
    void SetTB_ITEM(unsigned int index, const TB_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemDB() ;
    bool LoadItemFile(const std::filesystem::path& basePath) ;
    std::unordered_map<unsigned int, TB_ITEM> itemRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM* XResourceMgr::GetTB_ITEM(unsigned int index) {
        auto it = itemRows_.find(index);
        if (it == itemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM(unsigned int index, const TB_ITEM& row) {
        itemRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [Item_ID], [Item_Classify_Index], [Item_Rank], [Item_Lv], [Sell_Price], [Buy_Price], [Recycle_Sell_Price], [Recycle_Buy_Price], [Item_Stack_Max], [Item_Bind_Type], [Item_Model_ID], [Item_NameS], [Item_Limit_Lv], [Item_Limit_Class], [Item_Limit_Sell_Type], [Item_Sub_Type], [Item_CostumeSet], [Item_CostumeSet_ID], [Item_Slot_Disable], [Item_Endurance_Max], [Item_Use_Value], [Item_physical_Attack_Min], [Item_physical_Attack], [Item_Magic_Attack], [Item_physical_Defense_Min], [Item_physical_Defense], [Item_Magic_Defense], [Option_Class_1], [S_Option_Type_1], [S_Option_Value_1], [Option_Class_2], [S_Option_Type_2], [S_Option_Value_2], [Option_Class_3], [S_Option_Type_3], [S_Option_Value_3], [Option_Class_4], [S_Option_Type_4], [S_Option_Value_4], [Option_Class_5], [S_Option_Type_5], [S_Option_Value_5], [Item_Ex_Option_ID], [Item_Socket_ID], [Item_SetItem_ID], [Item_Reinforce_ID], [Item_Reinforce_Option_ID], [Item_Title_ID], [Item_Evolution_ID], [Item_Disassemble_ID], [Furniture_ID], [Cooltime_Group], [Cooltime_Value], [Cooltime_Save], [Item_Effect_Type], [Item_Effect_ID], [Item_Cash], [Item_Use_Period_Type], [Item_Use_Period_Value], [Sealing_Cnt], [Break_Cnt], [Item_SimilarGroup_ID], [RepackageItem_ID] from tb_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Classify_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Rank, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Lv, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Price, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buy_Price, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Recycle_Sell_Price, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Recycle_Buy_Price, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Stack_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Bind_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Model_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Item_NameS, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Limit_Lv, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Limit_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Limit_Sell_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Sub_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_CostumeSet, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_CostumeSet_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Item_Slot_Disable, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Endurance_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Use_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_physical_Attack_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_physical_Attack, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Magic_Attack, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_physical_Defense_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_physical_Defense, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Magic_Defense, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Class_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Option_Type_1, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.S_Option_Value_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Class_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Option_Type_2, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.S_Option_Value_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Class_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Option_Type_3, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.S_Option_Value_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Class_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Option_Type_4, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.S_Option_Value_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Class_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.S_Option_Type_5, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.S_Option_Value_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Ex_Option_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Socket_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_SetItem_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Reinforce_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Reinforce_Option_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Title_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Evolution_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Disassemble_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cooltime_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cooltime_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cooltime_Save, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Effect_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Effect_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Cash, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Use_Period_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Use_Period_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sealing_Cnt, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Break_Cnt, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_SimilarGroup_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RepackageItem_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            SetTB_ITEM(row.Item_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadItemFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Item.res", buffer) ||
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
        loader.m_strTableName = "tb_Item";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_ITEM row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU32(&row.Item_ID) ||
                !cursor.ReadCheckSumU32(&row.Item_Classify_Index) ||
                !cursor.ReadCheckSumU8(&row.Item_Rank) ||
                !cursor.ReadCheckSumU16(&row.Item_Lv) ||
                !cursor.ReadCheckSumU32(&row.Sell_Price) ||
                !cursor.ReadCheckSumU32(&row.Buy_Price) ||
                !cursor.ReadCheckSumU32(&row.Recycle_Sell_Price) ||
                !cursor.ReadCheckSumU32(&row.Recycle_Buy_Price) ||
                !cursor.ReadCheckSumU16(&row.Item_Stack_Max) ||
                !cursor.ReadCheckSumU8(&row.Item_Bind_Type) ||
                !cursor.ReadCheckSumU32(&row.Item_Model_ID) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Item_NameS, utf8Text);

            if (!cursor.ReadCheckSumU16(&row.Item_Limit_Lv) ||
                !cursor.ReadCheckSumU8(&row.Item_Limit_Class) ||
                !cursor.ReadCheckSumU8(&row.Item_Limit_Sell_Type) ||
                !cursor.ReadCheckSumU8(&row.Item_Sub_Type) ||
                !cursor.ReadCheckSumU8(&row.Item_CostumeSet) ||
                !cursor.ReadCheckSumU32(&row.Item_CostumeSet_ID) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Item_Slot_Disable, utf8Text);

            if (!cursor.ReadCheckSumU8(&row.Item_Endurance_Max) ||
                !cursor.ReadCheckSumU8(&row.Item_Use_Value) ||
                !cursor.ReadCheckSumU32(&row.Item_physical_Attack_Min) ||
                !cursor.ReadCheckSumU32(&row.Item_physical_Attack) ||
                !cursor.ReadCheckSumU32(&row.Item_Magic_Attack) ||
                !cursor.ReadCheckSumU32(&row.Item_physical_Defense_Min) ||
                !cursor.ReadCheckSumU32(&row.Item_physical_Defense) ||
                !cursor.ReadCheckSumU32(&row.Item_Magic_Defense) ||
                !cursor.ReadCheckSumU8(&row.Option_Class_1) ||
                !cursor.ReadCheckSumU32(&row.S_Option_Type_1) ||
                !cursor.ReadCheckSumI32(&row.S_Option_Value_1) ||
                !cursor.ReadCheckSumU8(&row.Option_Class_2) ||
                !cursor.ReadCheckSumU32(&row.S_Option_Type_2) ||
                !cursor.ReadCheckSumI32(&row.S_Option_Value_2) ||
                !cursor.ReadCheckSumU8(&row.Option_Class_3) ||
                !cursor.ReadCheckSumU32(&row.S_Option_Type_3) ||
                !cursor.ReadCheckSumI32(&row.S_Option_Value_3) ||
                !cursor.ReadCheckSumU8(&row.Option_Class_4) ||
                !cursor.ReadCheckSumU32(&row.S_Option_Type_4) ||
                !cursor.ReadCheckSumI32(&row.S_Option_Value_4) ||
                !cursor.ReadCheckSumU8(&row.Option_Class_5) ||
                !cursor.ReadCheckSumU32(&row.S_Option_Type_5) ||
                !cursor.ReadCheckSumI32(&row.S_Option_Value_5) ||
                !cursor.ReadCheckSumU32(&row.Item_Ex_Option_ID) ||
                !cursor.ReadCheckSumU32(&row.Item_Socket_ID) ||
                !cursor.ReadCheckSumU32(&row.Item_SetItem_ID) ||
                !cursor.ReadCheckSumU16(&row.Item_Reinforce_ID) ||
                !cursor.ReadCheckSumU32(&row.Item_Reinforce_Option_ID) ||
                !cursor.ReadCheckSumU32(&row.Item_Title_ID) ||
                !cursor.ReadCheckSumU32(&row.Item_Evolution_ID) ||
                !cursor.ReadCheckSumU16(&row.Item_Disassemble_ID) ||
                !cursor.ReadCheckSumU32(&row.Furniture_ID) ||
                !cursor.ReadCheckSumU16(&row.Cooltime_Group) ||
                !cursor.ReadCheckSumU32(&row.Cooltime_Value) ||
                !cursor.ReadCheckSumU8(&row.Cooltime_Save) ||
                !cursor.ReadCheckSumU16(&row.Item_Effect_Type) ||
                !cursor.ReadCheckSumU32(&row.Item_Effect_ID) ||
                !cursor.ReadCheckSumU8(&row.Item_Cash) ||
                !cursor.ReadCheckSumU8(&row.Item_Use_Period_Type) ||
                !cursor.ReadCheckSumU32(&row.Item_Use_Period_Value) ||
                !cursor.ReadCheckSumU8(&row.Sealing_Cnt) ||
                !cursor.ReadCheckSumU8(&row.Break_Cnt) ||
                !cursor.ReadCheckSumU32(&row.Item_SimilarGroup_ID) ||
                !cursor.ReadCheckSumU32(&row.RepackageItem_ID)) {
                return false;
            }

            SetTB_ITEM(row.Item_ID, row);
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
                            "GreenDamTan_log DBLoadTable.h::XResourceMgr::LoadItemFile checksum-bypass rowCount=%d loadedRows=%zu expectedMd5=%s",
                            loader.m_nRowCount,
                            itemRows_.size(),
                            md5Value.c_str());
        return true;
    }

#endif
