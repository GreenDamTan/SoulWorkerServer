// 中文说明：
// 1. 本文件承接 TB_AKASHIC_RECORDS 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AKASHIC_RECORDS {
    unsigned int ID = 0;
    char UI_String[511] = {};
    unsigned int Akashic_Group = 0;
    unsigned int Akashic_Effect_Group = 0;
    std::uint16_t Array_Index = 0;
    std::uint8_t Type = 0;
    std::uint8_t Cash_Type = 0;
    std::uint8_t Rank = 0;
    std::uint8_t Rare_Point = 0;
    std::uint8_t Hidden_Type = 0;
    char Code_Name[511] = {};
    std::uint16_t Action_Group = 0;
    std::uint8_t Combo_Count_Type = 0;
    unsigned int Use_Skill_ID = 0;
    std::uint8_t Invoke_Condition = 0;
    std::uint16_t Invoke_Rate = 0;
    unsigned int Skill_ID = 0;
    unsigned int CoolTime = 0;
    unsigned int CoolTime_Group = 0;
    std::uint8_t CoolTime_Save = 0;
    std::uint16_t CoolTime_Global = 0;
    std::uint8_t Akashic_Limit_Type = 0;
    unsigned int Akashic_Disassemble = 0;
    unsigned int EXP_Add = 0;
    unsigned int Akashic_Combination_Point = 0;
    unsigned int Gold_Add = 0;
    unsigned int Make_Need_Exp = 0;
    std::uint8_t Level_Max = 0;
    unsigned int Lease_Period = 0;
    unsigned int Lease_Cooltime = 0;
    unsigned int Gold_Need = 0;
    unsigned int Gold_Send = 0;
    unsigned int Basic_Exp = 0;
    std::uint8_t Make_Group_Enable_Type = 0;
    unsigned int Make_Group_Enable_Item = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_AKASHIC_RECORDS) == 0x459, "TB_AKASHIC_RECORDS size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AKASHIC_RECORDS* GetTB_AKASHIC_RECORDS(unsigned int index) ;
    void SetTB_AKASHIC_RECORDS(unsigned int index, const TB_AKASHIC_RECORDS& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAkashicRecordsDB() ;
    bool LoadAkashicRecordsFile(const std::filesystem::path& basePath) ;
    std::map<unsigned int, TB_AKASHIC_RECORDS> m_mapTB_AKASHIC_RECORDS;
    std::map<unsigned int, TB_AKASHIC_RECORDS*> m_mapPCAkashic;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AKASHIC_RECORDS* XResourceMgr::GetTB_AKASHIC_RECORDS(unsigned int index) {
        const auto it = m_mapTB_AKASHIC_RECORDS.find(index);
        return it == m_mapTB_AKASHIC_RECORDS.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AKASHIC_RECORDS(unsigned int index, const TB_AKASHIC_RECORDS& row) {
        m_mapTB_AKASHIC_RECORDS[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAkashicRecordsDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [UI_String], [Akashic_Group], [Akashic_Effect_Group], [Array_Index], [Type], [Cash_Type], [Rank], [Rare_Point], [Hidden_Type], [Code_Name], [Action_Group], [Combo_Count_Type], [Use_Skill_ID], [Invoke_Condition], [Invoke_Rate], [Skill_ID], [CoolTime], [CoolTime_Group], [CoolTime_Save], [CoolTime_Global], [Akashic_Limit_Type], [Akashic_Disassemble], [EXP_Add], [Akashic_Combination_Point], [Gold_Add], [Make_Need_Exp], [Level_Max], [Lease_Period], [Lease_Cooltime], [Gold_Need], [Gold_Send], [Basic_Exp], [Make_Group_Enable_Type], [Make_Group_Enable_Item] from tb_Akashic_Records")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AKASHIC_RECORDS.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AKASHIC_RECORDS row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.UI_String, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Effect_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Array_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cash_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rank, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rare_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hidden_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Code_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Action_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combo_Count_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Skill_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Invoke_Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Invoke_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Save, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Global, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Limit_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Disassemble, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EXP_Add, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Combination_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Add, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Need_Exp, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Level_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Lease_Period, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Lease_Cooltime, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Need, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Send, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Basic_Exp, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Group_Enable_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Group_Enable_Item, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AKASHIC_RECORDS(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadAkashicRecordsFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Akashic_Records.res", buffer) ||
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
        loader.m_strTableName = "tb_Akashic_Records";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_AKASHIC_RECORDS row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.UI_String, utf8Text);

            if (!cursor.ReadCheckSumU32(&row.Akashic_Group) ||
                !cursor.ReadCheckSumU32(&row.Akashic_Effect_Group) ||
                !cursor.ReadCheckSumU16(&row.Array_Index) ||
                !cursor.ReadCheckSumU8(&row.Type) ||
                !cursor.ReadCheckSumU8(&row.Cash_Type) ||
                !cursor.ReadCheckSumU8(&row.Rank) ||
                !cursor.ReadCheckSumU8(&row.Rare_Point) ||
                !cursor.ReadCheckSumU8(&row.Hidden_Type) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Code_Name, utf8Text);

            if (!cursor.ReadCheckSumU16(&row.Action_Group) ||
                !cursor.ReadCheckSumU8(&row.Combo_Count_Type) ||
                !cursor.ReadCheckSumU32(&row.Use_Skill_ID) ||
                !cursor.ReadCheckSumU8(&row.Invoke_Condition) ||
                !cursor.ReadCheckSumU16(&row.Invoke_Rate) ||
                !cursor.ReadCheckSumU32(&row.Skill_ID) ||
                !cursor.ReadCheckSumU32(&row.CoolTime) ||
                !cursor.ReadCheckSumU32(&row.CoolTime_Group) ||
                !cursor.ReadCheckSumU8(&row.CoolTime_Save) ||
                !cursor.ReadCheckSumU16(&row.CoolTime_Global) ||
                !cursor.ReadCheckSumU8(&row.Akashic_Limit_Type) ||
                !cursor.ReadCheckSumU32(&row.Akashic_Disassemble) ||
                !cursor.ReadCheckSumU32(&row.EXP_Add) ||
                !cursor.ReadCheckSumU32(&row.Akashic_Combination_Point) ||
                !cursor.ReadCheckSumU32(&row.Gold_Add) ||
                !cursor.ReadCheckSumU32(&row.Make_Need_Exp) ||
                !cursor.ReadCheckSumU8(&row.Level_Max) ||
                !cursor.ReadCheckSumU32(&row.Lease_Period) ||
                !cursor.ReadCheckSumU32(&row.Lease_Cooltime) ||
                !cursor.ReadCheckSumU32(&row.Gold_Need) ||
                !cursor.ReadCheckSumU32(&row.Gold_Send) ||
                !cursor.ReadCheckSumU32(&row.Basic_Exp) ||
                !cursor.ReadCheckSumU8(&row.Make_Group_Enable_Type) ||
                !cursor.ReadCheckSumU32(&row.Make_Group_Enable_Item)) {
                return false;
            }

            SetTB_AKASHIC_RECORDS(row.ID, row);
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
        return CheckSum(loader);
    }

#endif
