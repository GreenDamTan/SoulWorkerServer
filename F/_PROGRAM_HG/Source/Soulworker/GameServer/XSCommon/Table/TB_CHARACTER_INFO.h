// 中文说明：
// 1. 本文件承接 TB_CHARACTER_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CHARACTER_INFO {
    std::uint16_t ID = 0;
    std::uint8_t Character_ID = 0;
    std::uint16_t Character_Group_ID = 0;
    char PC_Code_Name[511] = {};
    unsigned int Item_ID_Weapon_01 = 0;
    unsigned int Default_Cloth_ID_01 = 0;
    unsigned int Default_Cloth_ID_02 = 0;
    unsigned int Default_Cloth_ID_03 = 0;
    unsigned int Default_Cloth_ID_04 = 0;
    std::uint16_t Default_Provide_Item = 0;
    std::uint16_t District_ID = 0;
    std::uint16_t District_Position_X = 0;
    std::uint16_t District_Position_Y = 0;
    std::uint16_t District_Position_Z = 0;
    std::uint16_t Maze_ID = 0;
    unsigned int Maze_Spawnbox_ID = 0;
    unsigned int Character_Start_Quest_ID = 0;
    unsigned int Character_Skip_Quest_ID = 0;
    std::uint8_t Character_Status_Type = 0;
    unsigned int Character_Status = 0;
    std::uint16_t Char_Height_Value = 0;
    std::uint16_t Nomal_Walk_Speed = 0;
    std::uint16_t Nomal_Run_Speed = 0;
    std::uint16_t Nomal_JumpMove_Speed = 0;
    std::uint16_t Battle_Run_Speed = 0;
    std::uint16_t Battle_JumpMove_Speed = 0;
    std::uint16_t Battle_Dash_Speed = 0;
    float Battle_BackMove_Ratio = 0.0f;
    std::uint8_t PC_Faction = 0;
    std::uint16_t Char_CollisionRadius = 0;
    std::uint16_t Char_CollisionHeight = 0;
    std::uint16_t Char_RealHeight = 0;
    unsigned int Default_Skill_ID_01 = 0;
    unsigned int Default_Skill_ID_02 = 0;
    unsigned int Default_Skill_ID_03 = 0;
    unsigned int Default_Skill_ID_04 = 0;
    unsigned int Default_Skill_ID_05 = 0;
    unsigned int Default_Skill_ID_06 = 0;
    unsigned int Default_Skill_ID_07 = 0;
    unsigned int Default_Skill_ID_08 = 0;
    unsigned int Default_Skill_ID_09 = 0;
    unsigned int Default_Skill_ID_10 = 0;
    unsigned int Default_Skill_ID_11 = 0;
    unsigned int Default_Skill_ID_12 = 0;
    unsigned int Default_Skill_ID_13 = 0;
    unsigned int Default_Skill_ID_14 = 0;
    unsigned int Default_Skill_ID_15 = 0;
    unsigned int Default_Skill_ID_16 = 0;
    unsigned int Default_Skill_ID_17 = 0;
    unsigned int Default_Skill_ID_18 = 0;
    unsigned int Default_Skill_ID_19 = 0;
    unsigned int Default_Skill_ID_20 = 0;
    std::uint8_t Set_Quick_Slot_ID_01 = 0;
    std::uint8_t Set_Quick_Slot_ID_02 = 0;
    std::uint8_t Set_Quick_Slot_ID_03 = 0;
    std::uint8_t Set_Quick_Slot_ID_04 = 0;
    std::uint8_t Set_Quick_Slot_ID_05 = 0;
    std::uint8_t Set_Quick_Slot_ID_06 = 0;
    std::uint8_t Set_Quick_Slot_ID_07 = 0;
    std::uint8_t Set_Quick_Slot_ID_08 = 0;
    std::uint8_t Set_Quick_Slot_ID_09 = 0;
    std::uint8_t Set_Quick_Slot_ID_10 = 0;
    std::uint8_t Set_Quick_Slot_ID_11 = 0;
    std::uint8_t Set_Quick_Slot_ID_12 = 0;
    std::uint8_t Set_Quick_Slot_ID_13 = 0;
    std::uint8_t Set_Quick_Slot_ID_14 = 0;
    std::uint8_t Set_Quick_Slot_ID_15 = 0;
    std::uint8_t Set_Quick_Slot_ID_16 = 0;
    std::uint8_t Set_Quick_Slot_ID_17 = 0;
    std::uint8_t Set_Quick_Slot_ID_18 = 0;
    std::uint8_t Set_Quick_Slot_ID_19 = 0;
    std::uint8_t Set_Quick_Slot_ID_20 = 0;
    std::uint16_t Set_Gesture_Slot_ID_01 = 0;
    std::uint16_t Set_Gesture_Slot_ID_02 = 0;
    std::uint16_t Set_Gesture_Slot_ID_03 = 0;
    std::uint16_t Set_Gesture_Slot_ID_04 = 0;
    std::uint16_t Set_Gesture_Slot_ID_05 = 0;
    std::uint16_t Set_Gesture_Slot_ID_06 = 0;
    float Akashic_Active_Time = 0.0f;
    std::uint8_t Character_On_Type = 0;
    std::uint8_t Character_On = 0;

    std::array<unsigned int, 20> GetDefaultSkillIDs() const {
        return {Default_Skill_ID_01, Default_Skill_ID_02, Default_Skill_ID_03, Default_Skill_ID_04,
                Default_Skill_ID_05, Default_Skill_ID_06, Default_Skill_ID_07, Default_Skill_ID_08,
                Default_Skill_ID_09, Default_Skill_ID_10, Default_Skill_ID_11, Default_Skill_ID_12,
                Default_Skill_ID_13, Default_Skill_ID_14, Default_Skill_ID_15, Default_Skill_ID_16,
                Default_Skill_ID_17, Default_Skill_ID_18, Default_Skill_ID_19, Default_Skill_ID_20};
    }

    std::array<std::uint8_t, 20> GetQuickSlotIDs() const {
        return {Set_Quick_Slot_ID_01, Set_Quick_Slot_ID_02, Set_Quick_Slot_ID_03, Set_Quick_Slot_ID_04,
                Set_Quick_Slot_ID_05, Set_Quick_Slot_ID_06, Set_Quick_Slot_ID_07, Set_Quick_Slot_ID_08,
                Set_Quick_Slot_ID_09, Set_Quick_Slot_ID_10, Set_Quick_Slot_ID_11, Set_Quick_Slot_ID_12,
                Set_Quick_Slot_ID_13, Set_Quick_Slot_ID_14, Set_Quick_Slot_ID_15, Set_Quick_Slot_ID_16,
                Set_Quick_Slot_ID_17, Set_Quick_Slot_ID_18, Set_Quick_Slot_ID_19, Set_Quick_Slot_ID_20};
    }

    std::array<std::uint16_t, 6> GetGestureSlotIDs() const {
        return {Set_Gesture_Slot_ID_01, Set_Gesture_Slot_ID_02, Set_Gesture_Slot_ID_03,
                Set_Gesture_Slot_ID_04, Set_Gesture_Slot_ID_05, Set_Gesture_Slot_ID_06};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_CHARACTER_INFO) == 0x2C4, "TB_CHARACTER_INFO size must match PDB");
static_assert(sizeof(TB_CHARACTER_INFO) == 0x2C4, "TB_CHARACTER_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CHARACTER_INFO* GetTB_CHARACTER_INFO(std::uint16_t index) ;
    void SetTB_CHARACTER_INFO(std::uint16_t index, const TB_CHARACTER_INFO& row) ;
    // Public iterator access for table traversal
    auto& GetCharacterInfoRows() { return characterInfoRows_; }
    const auto& GetCharacterInfoRows() const { return characterInfoRows_; }
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCharacterInfoDB() ;
    bool LoadCharacterInfoFile(const std::filesystem::path& basePath) ;
    std::unordered_map<std::uint16_t, TB_CHARACTER_INFO> characterInfoRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CHARACTER_INFO* XResourceMgr::GetTB_CHARACTER_INFO(std::uint16_t index) {
        auto it = characterInfoRows_.find(index);
        if (it == characterInfoRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CHARACTER_INFO(std::uint16_t index, const TB_CHARACTER_INFO& row) {
        characterInfoRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCharacterInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [ID], [Character_ID], [Character_Group_ID], [PC_Code_Name], [Item_ID_Weapon_01], [Default_Cloth_ID_01], [Default_Cloth_ID_02], [Default_Cloth_ID_03], [Default_Cloth_ID_04], [Default_Provide_Item], [District_ID], [District_Position_X], [District_Position_Y], [District_Position_Z], [Maze_ID], [Maze_Spawnbox_ID], [Character_Start_Quest_ID], [Character_Skip_Quest_ID], [Character_Status_Type], [Character_Status], [Char_Height_Value], [Nomal_Walk_Speed], [Nomal_Run_Speed], [Nomal_JumpMove_Speed], [Battle_Run_Speed], [Battle_JumpMove_Speed], [Battle_Dash_Speed], [Battle_BackMove_Ratio], [PC_Faction], [Char_CollisionRadius], [Char_CollisionHeight], [Char_RealHeight], [Default_Skill_ID_01], [Set_Quick_Slot_ID_01], [Default_Skill_ID_02], [Set_Quick_Slot_ID_02], [Default_Skill_ID_03], [Set_Quick_Slot_ID_03], [Default_Skill_ID_04], [Set_Quick_Slot_ID_04], [Default_Skill_ID_05], [Set_Quick_Slot_ID_05], [Default_Skill_ID_06], [Set_Quick_Slot_ID_06], [Default_Skill_ID_07], [Set_Quick_Slot_ID_07], [Default_Skill_ID_08], [Set_Quick_Slot_ID_08], [Default_Skill_ID_09], [Set_Quick_Slot_ID_09], [Default_Skill_ID_10], [Set_Quick_Slot_ID_10], [Default_Skill_ID_11], [Set_Quick_Slot_ID_11], [Default_Skill_ID_12], [Set_Quick_Slot_ID_12], [Default_Skill_ID_13], [Set_Quick_Slot_ID_13], [Default_Skill_ID_14], [Set_Quick_Slot_ID_14], [Default_Skill_ID_15], [Set_Quick_Slot_ID_15], [Default_Skill_ID_16], [Set_Quick_Slot_ID_16], [Default_Skill_ID_17], [Set_Quick_Slot_ID_17], [Default_Skill_ID_18], [Set_Quick_Slot_ID_18], [Default_Skill_ID_19], [Set_Quick_Slot_ID_19], [Default_Skill_ID_20], [Set_Quick_Slot_ID_20], [Set_Gesture_Slot_ID_01], [Set_Gesture_Slot_ID_02], [Set_Gesture_Slot_ID_03], [Set_Gesture_Slot_ID_04], [Set_Gesture_Slot_ID_05], [Set_Gesture_Slot_ID_06], [Akashic_Active_Time], [Character_On_Type], [Character_On] from tb_Character_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        characterInfoRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CHARACTER_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.PC_Code_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Weapon_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Default_Cloth_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Default_Cloth_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Default_Cloth_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Default_Cloth_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Default_Provide_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Position_X, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Position_Y, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.District_Position_Z, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Spawnbox_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_Start_Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_Skip_Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_Status_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_Status, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Char_Height_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Walk_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_Run_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Nomal_JumpMove_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Battle_Run_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Battle_JumpMove_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Battle_Dash_Speed, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Battle_BackMove_Ratio, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.PC_Faction, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Char_CollisionRadius, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Char_CollisionHeight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Char_RealHeight, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            unsigned int* skillPtr = &row.Default_Skill_ID_01;
            std::uint8_t* quickPtr = &row.Set_Quick_Slot_ID_01;
            for (int index = 0; index < 20; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &skillPtr[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &quickPtr[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }

            std::uint16_t* gesturePtr = &row.Set_Gesture_Slot_ID_01;
            for (int index = 0; index < 6; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &gesturePtr[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }

            if (!GreenDamTan_DBGetFloat(xDBBinder, &row.Akashic_Active_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_On_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_On, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            SetTB_CHARACTER_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadCharacterInfoFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Character_Info.res", buffer) ||
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
        loader.m_strTableName = "tb_Character_Info";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_CHARACTER_INFO row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU16(&row.ID) ||
                !cursor.ReadCheckSumU8(&row.Character_ID) ||
                !cursor.ReadCheckSumU16(&row.Character_Group_ID) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.PC_Code_Name, utf8Text);

            if (!cursor.ReadCheckSumU32(&row.Item_ID_Weapon_01)) {
                return false;
            }

            std::array<unsigned int, 4> defaultClothIds = {};
            for (int clothIndex = 0; clothIndex < 4; ++clothIndex) {
                if (!cursor.ReadCheckSumU32(&defaultClothIds[clothIndex])) {
                    return false;
                }
            }
            row.Default_Cloth_ID_01 = defaultClothIds[0];
            row.Default_Cloth_ID_02 = defaultClothIds[1];
            row.Default_Cloth_ID_03 = defaultClothIds[2];
            row.Default_Cloth_ID_04 = defaultClothIds[3];

            if (!cursor.ReadCheckSumU16(&row.Default_Provide_Item) ||
                !cursor.ReadCheckSumU16(&row.District_ID) ||
                !cursor.ReadCheckSumU16(&row.District_Position_X) ||
                !cursor.ReadCheckSumU16(&row.District_Position_Y) ||
                !cursor.ReadCheckSumU16(&row.District_Position_Z) ||
                !cursor.ReadCheckSumU16(&row.Maze_ID) ||
                !cursor.ReadCheckSumU32(&row.Maze_Spawnbox_ID) ||
                !cursor.ReadCheckSumU32(&row.Character_Start_Quest_ID) ||
                !cursor.ReadCheckSumU32(&row.Character_Skip_Quest_ID) ||
                !cursor.ReadCheckSumU8(&row.Character_Status_Type) ||
                !cursor.ReadCheckSumU32(&row.Character_Status) ||
                !cursor.ReadCheckSumU16(&row.Char_Height_Value) ||
                !cursor.ReadCheckSumU16(&row.Nomal_Walk_Speed) ||
                !cursor.ReadCheckSumU16(&row.Nomal_Run_Speed) ||
                !cursor.ReadCheckSumU16(&row.Nomal_JumpMove_Speed) ||
                !cursor.ReadCheckSumU16(&row.Battle_Run_Speed) ||
                !cursor.ReadCheckSumU16(&row.Battle_JumpMove_Speed) ||
                !cursor.ReadCheckSumU16(&row.Battle_Dash_Speed) ||
                !cursor.ReadCheckSumF32(&row.Battle_BackMove_Ratio) ||
                !cursor.ReadCheckSumU8(&row.PC_Faction) ||
                !cursor.ReadCheckSumU16(&row.Char_CollisionRadius) ||
                !cursor.ReadCheckSumU16(&row.Char_CollisionHeight) ||
                !cursor.ReadCheckSumU16(&row.Char_RealHeight)) {
                return false;
            }

            std::array<unsigned int, 20> defaultSkillIds = {};
            for (int skillIndex = 0; skillIndex < 20; ++skillIndex) {
                if (!cursor.ReadCheckSumU32(&defaultSkillIds[skillIndex])) {
                    return false;
                }
            }
            std::memcpy(&row.Default_Skill_ID_01, defaultSkillIds.data(),
                        sizeof(unsigned int) * defaultSkillIds.size());

            std::array<std::uint8_t, 20> quickSlotIds = {};
            for (int quickIndex = 0; quickIndex < 20; ++quickIndex) {
                if (!cursor.ReadCheckSumU8(&quickSlotIds[quickIndex])) {
                    return false;
                }
            }
            std::memcpy(&row.Set_Quick_Slot_ID_01, quickSlotIds.data(),
                        sizeof(std::uint8_t) * quickSlotIds.size());

            std::array<std::uint16_t, 6> gestureSlotIds = {};
            for (int gestureIndex = 0; gestureIndex < 6; ++gestureIndex) {
                if (!cursor.ReadCheckSumU16(&gestureSlotIds[gestureIndex])) {
                    return false;
                }
            }
            std::memcpy(&row.Set_Gesture_Slot_ID_01, gestureSlotIds.data(),
                        sizeof(std::uint16_t) * gestureSlotIds.size());

            if (!cursor.ReadCheckSumF32(&row.Akashic_Active_Time) ||
                !cursor.ReadCheckSumU8(&row.Character_On_Type) ||
                !cursor.ReadCheckSumU8(&row.Character_On)) {
                return false;
            }

            SetTB_CHARACTER_INFO(row.ID, row);
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
