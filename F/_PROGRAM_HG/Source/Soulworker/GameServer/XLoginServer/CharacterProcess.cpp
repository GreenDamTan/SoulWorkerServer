#include "Soulworker/GameServer/XLoginServer/CharacterProcess.h"

#include <cwctype>

namespace {
/**
 * @brief 提取 `ReqSelectCharacter` 发送给 GameDB 的两枚每周任务组 ID。
 *
 * 根据 IDA 中 `ReqSelectCharacter(0x140003f40)`：
 * - 遍历 `m_mapTB_WEEK_GROUP`
 * - `Check_Mission_Type == 1 && Event_Type == 3` -> `byWeeklyMissionGroupID_ComeBack`
 * - `Check_Mission_Type == 2 && Event_Type == 3` -> `byWeeklyMissionGroupID_AccountComeBack`
 */
void ExtractWeeklyMissionGroupIDs(const XResourceMgr& resourceMgr,
                                  std::uint8_t& outComeBack,
                                  std::uint8_t& outAccountComeBack) {
    outComeBack = 0;
    outAccountComeBack = 0;

    for (const auto& entry : resourceMgr.GetTB_WEEK_GROUPRows()) {
        const TB_WEEK_GROUP& weekGroup = entry.second;
        if (weekGroup.Check_Mission_Type == 1 && weekGroup.Event_Type == 3) {
            outComeBack = weekGroup.mission_group;
        }
        if (weekGroup.Check_Mission_Type == 2 && weekGroup.Event_Type == 3) {
            outAccountComeBack = weekGroup.mission_group;
        }
    }
}

/**
 * @brief 向客户端回送角色进入/切服失败结果。
 * @param user 当前客户端。
 * @param resultCode 已确认的错误码。
 */
void SendEnterMapFailure(CUser* user, int resultCode) {
    if (!user) {
        return;
    }

    PS_ENTER_MAP_RES enterMapResult{};
    enterMapResult.nResult = resultCode;

    XSendPacket sendPacket(eCMD_CHARACTER, eSUB_CMD_SELECT_CHARACTER_RES);
    sendPacket << enterMapResult;
    user->BridgeSend(sendPacket);
}
}

/**
 * @brief 按角色阶段子命令分派客户端请求。
 *
 * 根据 IDA 中 `CCharacterProcess::Parse(0x140002250)`，当前已经核实到的角色阶段上行里，
 * `ReqSelectCharacter`、`ReqCharacterChangeServer` 与 `ReqSecondPassword`
 * 已满足继续落工程的证据条件；
 * 其余分支暂时只保留“存在该子命令”的分发表信息。
 */

namespace UtilFunc {
bool CheckValidString(wchar_t* wszString, NATION_TYPE eNationType) {
    if (!wszString) {
        return false;
    }

    bool valid = true;
    const int length = static_cast<int>(std::wcslen(wszString));
    if (eNationType == NATION_TYPE_KOR) {
        for (int index = 0; index < length && valid; ++index) {
            const wchar_t ch = wszString[index];
            valid = (ch >= L'0' && ch <= L'9') ||
                    (ch >= L'A' && ch <= L'Z') ||
                    (ch >= L'a' && ch <= L'z') ||
                    (ch >= 0xAC00 && ch <= 0xD7A3);
        }
    } else if (eNationType == NATION_TYPE_JPN) {
        for (int index = 0; index < length && valid; ++index) {
            const wchar_t ch = wszString[index];
            if ((ch >= L'0' && ch <= L'9') ||
                (ch >= L'A' && ch <= L'Z') ||
                (ch >= L'a' && ch <= L'z') ||
                (ch >= 0x3040 && ch <= 0x309F) ||
                (ch >= 0x30A0 && ch <= 0x30FF) ||
                (ch >= 0x4E00 && ch <= 0x9FFF)) {
                continue;
            }

            switch (ch) {
            case 12539:
            case 65281:
            case 12540:
            case 9734:
            case 9733:
            case 9675:
            case 9679:
            case 9678:
            case 9671:
            case 9670:
            case 9633:
            case 9632:
            case 9651:
            case 9650:
            case 9661:
            case 9660:
            case 9834:
            case 65343:
            case 8213:
            case 8208:
            case 65291:
            case 65293:
            case 8224:
                break;
            default:
                valid = false;
                break;
            }
        }
    }

    return valid;
}

std::wstring ToUpperWide(const wchar_t* value) {
    if (!value) {
        return {};
    }

    std::wstring result(value);
    for (wchar_t& ch : result) {
        ch = static_cast<wchar_t>(std::towupper(ch));
    }
    return result;
}

std::wstring WidenFilterWord(const char* value) {
    if (!value) {
        return {};
    }

    std::wstring result;
    while (*value != '\0') {
        result.push_back(static_cast<unsigned char>(*value));
        ++value;
    }
    return result;
}

bool IsUsableNameFilter(wchar_t* wszString) {
    if (!wszString) {
        return false;
    }

    const std::wstring upperName = ToUpperWide(wszString);
    const auto& rows = TXSingleton<XLoginServer>::Instance()->GetResourceMgr().GetTB_NAMEFILTERRows();
    for (const auto& entry : rows) {
        const TB_NAMEFILTER& row = entry.second;
        const std::wstring upperFilter = ToUpperWide(WidenFilterWord(row.Filter_Word).c_str());
        if (upperFilter.empty()) {
            continue;
        }

        if (row.Filter_Type == 1) {
            if (upperName == upperFilter) {
                return false;
            }
            continue;
        }

        if (upperName.find(upperFilter) != std::wstring::npos) {
            return false;
        }
    }

    return true;
}
}

bool CCharacterProcess::Parse(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::Parse sub=%u session=%d socket=%lld user=%p",
                        static_cast<unsigned int>(xPacket.GetSubCmd()),
                        user ? user->GetSessionID() : -1,
                        user ? static_cast<long long>(user->Socket) : -1LL,
                        static_cast<void*>(user));
    switch (xPacket.GetSubCmd()) {
    case eSUB_CMD_DELETE_CHARACTER_REQ:
        return ReqCharacterDelete(xPacket);
    case eSUB_CMD_CHARACTER_CHANGE_SLOT:
        return ReqCharacterChangeSlot(xPacket);
    case eSUB_CMD_CHARACTER_LIST_REQ:
        return ReqCharacterList(xPacket);
    case eSUB_CMD_CHARACTER_CHECK_NAME:
        return ReqCharacterCheckName(xPacket);
    case eSUB_CMD_CHARACTER_REPRESENTATIVE_CHECK:
        return ReqCharacterRepresentativeCheck(xPacket);
    case eSUB_CMD_CHARACTER_REPRESENTATIVE_CHANGE:
        return ReqCharacterRepresentativeChange(xPacket);
    case eSUB_CMD_SELECT_CHARACTER_REQ:
        return ReqSelectCharacter(xPacket);
    case eSUB_CMD_CHARACTER_CHANGE_SERVER_REQ:
        return ReqCharacterChangeServer(xPacket);
    case eSUB_CMD_SECOND_PASSWORD:
        return ReqSecondPassword(xPacket);
    case eSUB_CMD_CREATE_CHARACTER_REQ:
        return ReqCharacterCreate(xPacket);
    default:
        return true;
    }
}

/**
 * @brief 处理客户端角色列表请求。
 *
 * 根据 IDA 中 `CCharacterProcess::ReqCharacterList(0x140002491)`：
 * 1. 从客户端包里读取 `AuthSessionID`
 * 2. 写回当前用户对象
 * 3. 若控制口已连接，则向控制服发送 `main=0xF3, sub=0x32`
 * 4. 包体顺序为 `UAID -> AuthSessionID`
 * 5. 发送成功后记录一次 `SetSendCheckSessionID()`
 * 6. 未连接时回客户端 `3/0x12/0xC3BB`
 */
bool CCharacterProcess::ReqCharacterList(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    std::uint64_t authSessionId = 0;
    xPacket.XParse >> authSessionId;
    const std::uint64_t previousAuthSessionId = user->GetAuthSessionID();
    user->SetAuthSessionID(authSessionId);

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    const bool controlConnected = loginServer->GetControlSocket().XIOCPClient::IsConnection();
    const bool controlReady = loginServer->GetControlSocket().IsReady();
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterList session=%d uaid=%d auth=%llu prevAuth=%llu controlConnected=%d ready=%d user=%p secondPW=%u tradePW=%u enterState=%d",
                        user->GetSessionID(),
                        user->GetUAID(),
                        static_cast<unsigned long long>(authSessionId),
                        static_cast<unsigned long long>(previousAuthSessionId),
                        controlConnected ? 1 : 0,
                        controlReady ? 1 : 0,
                        static_cast<void*>(user),
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()),
                        static_cast<int>(user->GetEnterServerState()));
    if (!controlConnected) {
        LogHelper::LogError("game.contents", "<LOGIN> ReqCharacterList Check Control Server");
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC3BBu);
        return false;
    }

    XSendPacket sendPacket(0xF3, 0x32);
    sendPacket.XParse << user->GetUAID();
    sendPacket.XParse << user->GetAuthSessionID();
    const bool sendOk = loginServer->GetControlSocket().Send(sendPacket);
    if (sendOk) {
        loginServer->GetControlSocket().GreenDamTan_RecordPendingCheckSession(
            static_cast<unsigned int>(user->GetUAID()),
            user->GetAuthSessionID(),
            user->GetSessionID());
    }
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterList->SendControl main=0xF3 sub=0x32 uaid=%d auth=%llu session=%d sendOk=%d secondPW=%u tradePW=%u",
                        user->GetUAID(),
                        static_cast<unsigned long long>(user->GetAuthSessionID()),
                        user->GetSessionID(),
                        sendOk ? 1 : 0,
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()));
    user->SetSendCheckSessionID();
    return true;
}

/**
 * @brief 处理客户端删角请求。
 *
 * 根据 IDA 中 `ReqCharacterDelete(0x140003af0)`，当前保留以下稳定语义：
 * 1. 先读取目标 `UCID`
 * 2. 调用 `CheckCreateDate` 校验 1 天删角保护
 * 3. 若开启二级密码内容开关且尚未通过二级密码，则回内容开关包
 * 4. 校验入服阶段、等待标记、联盟会长标记和代表角色冲突
 * 5. 成功时向 GameDB 发送 `main=3, sub=3`
 */
bool CCharacterProcess::ReqCharacterDelete(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    int ucid = 0;
    xPacket.XParse >> ucid;

    PS_DELETE_CHARACTER_RES deleteResult{};
    if (!user->CheckCreateDate(static_cast<unsigned int>(ucid), deleteResult)) {
        XSendPacket sendPacket(eCMD_CHARACTER, eSUB_SQL_CHARACTER_DELETE);
        sendPacket << deleteResult;
        user->BridgeSend(sendPacket);
        return true;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (loginServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_SECOND_PW) &&
        !user->CheckSecondPasswordState()) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_SQL_CHARACTER_DELETE, 0xC739u);
        LogHelper::LogDebug("game.contents", "<DELETE_CHAR> Need second password");
        user->SendServerOption();
        return true;
    }

    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC739u);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterDelete> Check Enter Server State  %d/%d",
                            ucid,
                            static_cast<int>(user->GetEnterServerState()));
        return false;
    }

    if (user->GetWaitDeleteCharacterPacketRes()) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC739u);
        LogHelper::LogError("game.contents",
                            "<CREATE_CHAR> GetWaitDeleteCharacterPacketRes  %d",
                            user->GetUAID());
        return false;
    }

    if (user->IsLeagueMaster(static_cast<unsigned int>(ucid))) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC744u);
        LogHelper::LogDebug("game.contents", "<DELETE_CHAR> League master");
        return true;
    }

    if (static_cast<int>(user->GetRepresentativeUCID()) == ucid) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xE8F3u);
        LogHelper::LogDebug("game.contents", "<DELETE_CHAR> Representative character");
        return false;
    }

    user->SetWaitDeleteCharacterPacketRes(true);

    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_DELETE);
    sendPacket.XParse << (ucid & 0x1FFFFFFF);
    sendPacket.XParse << user->GetUAID();
    sendPacket.XParse << true;
    loginServer->SendDBGame(sendPacket);
    return true;
}

/**
 * @brief 处理客户端角色名重复检测请求。
 *
 * 根据 IDA 中 `ReqCharacterCheckName(0x140004570)`，当前仅做：
 * 1. 读取 `PS_REQ_CHECK_NAME`
 * 2. 原样转发到 GameDB `main=3, sub=4`
 */
bool CCharacterProcess::ReqCharacterCheckName(XPacket& xPacket) {
    PS_REQ_CHECK_NAME checkName{};
    xPacket >> checkName;

    CUser* user = GetClientPtr();
    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_CHECK_NAME);
    sendPacket << checkName;
    TXSingleton<XLoginServer>::Instance()->SendDBGame(sendPacket);
    return true;
}

/**
 * @brief 处理客户端角色换槽请求。
 *
 * 根据 IDA 中 `ReqCharacterChangeSlot(0x140004fc0)`：
 * 1. 校验入服阶段与 `UAID`
 * 2. 防重复提交 `WaitChangeSlotPacketRes`
 * 3. 调用 `CheckChangeSlot` 回填源/目标 `UCID`
 * 4. 成功时发往 GameDB `main=3, sub=6`
 */
bool CCharacterProcess::ReqCharacterChangeSlot(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    PS_CHARACTER_CHANGE_SLOT slotChange{};
    xPacket >> slotChange;

    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_CHANGE_SLOT, 0xC745u);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterChangeSlot> Check Enter Server State  %d/%d",
                            user->GetUAID(),
                            static_cast<int>(user->GetEnterServerState()));
        return false;
    }

    if (user->GetWaitChangeSlotPacketRes()) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_CHANGE_SLOT, 0xC745u);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterChangeSlot> GetWaitChangeSlotPacketRes  %d",
                            user->GetUAID());
        return false;
    }

    if (!user->CheckChangeSlot(slotChange)) {
        return true;
    }

    slotChange.nUAID = user->GetUAID();
    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_CHANGE_SLOT);
    sendPacket << slotChange;
    TXSingleton<XLoginServer>::Instance()->SendDBGame(sendPacket);
    user->SetWaitChangeSlotPacketRes(true);
    return true;
}

/**
 * @brief 处理客户端角色选择请求。
 *
 * 根据 IDA 中 `CCharacterProcess::ReqSelectCharacter(0x140003f40)`，当前已确认的流程为：
 * 1. 解包 `PS_CHARACTER_SELECT`
 * 2. 要求用户处于 `ENTER_SERVER_STATE_SELECT_WORLD_RES` 且 `UAID > 0`
 * 3. 若开启了二级密码内容开关，且用户还未通过二级密码，则回 `50104`
 *    并补发一次 `main=1, sub=7` 的内容开关包
 * 4. 若控制/Relay 未就绪，则回 `50003`
 * 5. 若 `CanEnterGame` 失败，则回 `50104`
 * 6. 成功时推进到 `ENTER_SERVER_STATE_SELECT_GAME_REQ`
 * 7. 回填 `dwUAID` 与角色地图缓存中的 `nPrevMapID / nPrevRevivePoint`
 * 8. 继续把自动邮件时间与两枚 `TB_COMMON` 编码字节一起转发给 GameDB
 */
/**
 * @brief 处理客户端建角请求。
 *
 * 根据 IDA 中 `CCharacterProcess::ReqCharacterCreate(0x140002550)`，
 * 当前保留的已证实语义包括：
 * 1. 校验入服状态、等待标记与角色槽是否合法。
 * 2. 校验角色名长度、名称过滤与国家字符规则。
 * 3. 校验默认头像、外观资源、职业开放状态与默认物品配置。
 * 4. 最终把 `PS_DB_CHARACTER_CREATE + PS_DEFAULT_INVEN_ITEMS` 发送到 GameDB `main=3, sub=2`。
 */
bool CCharacterProcess::ReqCharacterCreate(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    PS_DB_CHARACTER_CREATE createRequest{};
    STCharInfo characterInfo{};
    int createClothId = 0;
    xPacket >> characterInfo;
    xPacket.XParse >> createClothId;

    createRequest.nUAID = user->GetUAID();
    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC739u);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterCreate> Check Enter Server State  %d/%d",
                            createRequest.nUAID,
                            static_cast<int>(user->GetEnterServerState()));
        return false;
    }

    if (xPacket.XParse.GetType() != 0) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate parse-error uaid=%d session=%d packetType=%d",
                            createRequest.nUAID,
                            user->GetSessionID(),
                            xPacket.XParse.GetType());
        return true;
    }

    LogHelper::LogDebug(
        "game.contents",
        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate preset uaid=%d class=%u slot=%u createCloth=%d appearance=[%u,%u,%u,%u] appearanceEx=[%u,%u,%u,%u]",
        createRequest.nUAID,
        static_cast<unsigned int>(characterInfo.stBaseInfo.byClass),
        static_cast<unsigned int>(characterInfo.byCharSlotPos),
        createClothId,
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[0]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[1]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[2]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[3]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[0]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[1]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[2]),
        static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[3]));

    if (user->GetWaitCreateCharacterPacketRes()) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC739u);
        LogHelper::LogError("game.contents",
                            "<CREATE_CHAR> GetWaitCreateCharacterPacketRes  %d",
                            createRequest.nUAID);
        return false;
    }

    if (characterInfo.byCharSlotPos == 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC743u);
        LogHelper::LogError("game.contents",
                            "<CREATE_CHAR> Failed Character SlotNum %d, %d ",
                            createRequest.nUAID,
                            static_cast<int>(characterInfo.byCharSlotPos));
        return false;
    }

    if (!user->IsEmptySlot(characterInfo.byCharSlotPos)) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC73Bu);
        LogHelper::LogError("game.contents",
                            "<CREATE_CHAR> Failed Character Count  %d, %d ",
                            createRequest.nUAID,
                            static_cast<unsigned int>(user->GetCharacterCount()));
        return false;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    const NATION_TYPE nationType = loginServer->GetOption().GetNationType();
    std::size_t nameLength = 0;
    while (nameLength < std::size(characterInfo.stBaseInfo.strName) &&
           characterInfo.stBaseInfo.strName[nameLength] != L'\0') {
        ++nameLength;
    }
    if (nameLength < 2 || nameLength > 12) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC73Cu);
        LogHelper::LogError("game.contents", "<CREATE_CHAR> Failed Valid Name  %d ", createRequest.nUAID);
        return false;
    }

    if (!UtilFunc::IsUsableNameFilter(characterInfo.stBaseInfo.strName)) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC73Cu);
        LogHelper::LogError("game.contents",
                            "<CREATE_CHAR> Failed Filter Valid Name %d ",
                            createRequest.nUAID);
        return false;
    }

    if (!UtilFunc::CheckValidString(characterInfo.stBaseInfo.strName, nationType)) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC73Cu);
        LogHelper::LogError("game.contents", "<CREATE_CHAR> Failed Valid Name  %d ", createRequest.nUAID);
        return false;
    }

    // IDA `CCharacterProcess::ReqCharacterCreate(0x140002550)` 明确先对
    // `stDBCreateChar.stCharInfo` 额外调用一轮 `STMyCharInfoEx::Init`，把未覆写槽位收回原版哨兵值。
    createRequest.stCharInfo.Init();
    createRequest.stCharInfo.stBaseInfo = characterInfo.stBaseInfo;
    createRequest.stCharInfo.byCharSlotPos = characterInfo.byCharSlotPos;

    TB_PHOTO_ITEM* defaultPhotoItem =
        loginServer->GetResourceMgr().FindDefaultPhotoItemID(characterInfo.stBaseInfo.byClass, 1);
    if (!defaultPhotoItem) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CREATE_CHARACTER_REQ, 0xC739u);
        LogHelper::LogDebug("game.contents",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate FindDefaultPhotoItemID failed class=%u session=%d",
                            static_cast<unsigned int>(characterInfo.stBaseInfo.byClass),
                            user->GetSessionID());
        return true;
    }
    createRequest.stCharInfo.stBaseInfo.dwProfilePhotoID = defaultPhotoItem->ID;

    for (int index = 0; index < 4; ++index) {
        const unsigned int appearanceId =
            characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[index];
        TB_APPEARANCE* appearanceRow = loginServer->GetResourceMgr().GetTB_APPEARANCE(appearanceId);
        if (!appearanceRow || appearanceRow->Appearance_Classify != 1) {
            user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CREATE_CHARACTER_REQ, 0xC739u);
            return false;
        }
    }

    const unsigned int characterInfoId =
        static_cast<unsigned int>(characterInfo.stBaseInfo.byClass) * 1000u;
    TB_CHARACTER_INFO* characterInfoRow =
        loginServer->GetResourceMgr().GetTB_CHARACTER_INFO(static_cast<std::uint16_t>(characterInfoId));
    if (!characterInfoRow) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CREATE_CHARACTER_REQ, 0xC739u);
        LogHelper::LogDebug("game.contents",
                            "<CREATE_CHAR> TB_CHARACTER_INFO missing %u",
                            characterInfoId);
        return true;
    }

    if (characterInfoRow->Character_On_Type != 1 || characterInfoRow->Character_On != 1) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CREATE_CHARACTER_REQ, 0xC739u);
        LogHelper::LogError("game.contents",
                            "<CREATE_CHAR> TB_CHARACTER_INFO Not Open Char ( %u )",
                            characterInfoId);
        return true;
    }

    LogHelper::LogDebug("game.contents",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate starter-seed uaid=%d class=%u charInfoId=%u weaponSeed=%u createCloth=%d provideSeed=%u",
                        createRequest.nUAID,
                        static_cast<unsigned int>(characterInfo.stBaseInfo.byClass),
                        characterInfoId,
                        characterInfoRow->Item_ID_Weapon_01,
                        createClothId,
                        static_cast<unsigned int>(characterInfoRow->Default_Provide_Item));

    STItem weaponItem{};
    std::array<STItem, 6> clothItems{};
    const bool hasWeaponRow =
        loginServer->GetResourceMgr().GetTB_ITEM(characterInfoRow->Item_ID_Weapon_01) != nullptr;
    if (hasWeaponRow) {
        loginServer->GetItemFactory().CreateItem(
            weaponItem,
            static_cast<int>(characterInfoRow->Item_ID_Weapon_01),
            1,
            false,
            &loginServer->GetResourceMgr(),
            false);
        createRequest.stCharInfo.stSoulWeapon.dwItemID = characterInfoRow->Item_ID_Weapon_01;
    } else {
        LogHelper::LogDebug("game.contents",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate starter-miss uaid=%d kind=weapon itemId=%u",
                            createRequest.nUAID,
                            characterInfoRow->Item_ID_Weapon_01);
    }

    TB_CREATE_CLOTH* createClothRow =
        loginServer->GetResourceMgr().GetTB_CREATE_CLOTH(static_cast<std::uint16_t>(createClothId));
    if (!createClothRow) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CREATE_CHARACTER_REQ, 0xC739u);
        LogHelper::LogDebug("game.contents",
                            "<CREATE_CHAR> TB_CREATE_CLOTH missing %d",
                            createClothId);
        return true;
    }

    const auto createClothItemIds = createClothRow->GetItemIDs();
    LogHelper::LogDebug(
        "game.contents",
        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate cloth-seed uaid=%d head=%u body=%u hands=%u stocking=%u foot=%u pants=%u",
        createRequest.nUAID,
        createClothItemIds[0],
        createClothItemIds[1],
        createClothItemIds[2],
        createClothItemIds[3],
        createClothItemIds[4],
        createClothItemIds[5]);

    struct ClothSpec {
        unsigned int itemId;
        int shapeIndex;
        int arrayIndex;
    };
    const ClothSpec clothSpecs[6] = {
        {createClothItemIds[1], 6, 0},
        {createClothItemIds[2], 4, 1},
        {createClothItemIds[4], 9, 2},
        {createClothItemIds[3], 8, 3},
        {createClothItemIds[5], 11, 4},
        {createClothItemIds[0], 1, 5},
    };

    for (const ClothSpec& clothSpec : clothSpecs) {
        if (!loginServer->GetResourceMgr().GetTB_ITEM(clothSpec.itemId)) {
            LogHelper::LogDebug("game.contents",
                                "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate starter-miss uaid=%d kind=cloth shapeIndex=%d itemId=%u",
                                createRequest.nUAID,
                                clothSpec.shapeIndex,
                                clothSpec.itemId);
            continue;
        }

        loginServer->GetItemFactory().CreateItem(
            clothItems[clothSpec.arrayIndex],
            static_cast<int>(clothSpec.itemId),
            1,
            false,
            &loginServer->GetResourceMgr(),
            false);
        createRequest.stCharInfo.stShapeEquipItemInfo[clothSpec.shapeIndex].biSerial =
            clothItems[clothSpec.arrayIndex].xSerial;
        createRequest.stCharInfo.stShapeEquipItemInfo[clothSpec.shapeIndex].nItemID =
            static_cast<int>(clothSpec.itemId);
    }

    std::uint64_t defaultConsumeCategory1 = 0;
    std::uint64_t defaultConsumeCategory3 = 0;
    PS_DEFAULT_INVEN_ITEMS defaultItems{};
    TB_PROVIDE_ITEM* provideItemRow =
        loginServer->GetResourceMgr().GetTB_PROVIDE_ITEM(characterInfoRow->Default_Provide_Item);
    if (!provideItemRow && characterInfoRow->Default_Provide_Item != 0) {
        LogHelper::LogDebug("game.contents",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate starter-miss uaid=%d kind=provide provideId=%u",
                            createRequest.nUAID,
                            static_cast<unsigned int>(characterInfoRow->Default_Provide_Item));
    }
    if (provideItemRow) {
        const auto provideItemIds = provideItemRow->GetItemIDs();
        const auto provideItemCounts = provideItemRow->GetItemCounts();
        LogHelper::LogDebug(
            "game.contents",
            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate provide-seed uaid=%d firstItems=[%u,%u,%u,%u] firstCounts=[%u,%u,%u,%u]",
            createRequest.nUAID,
            provideItemIds[0],
            provideItemIds[1],
            provideItemIds[2],
            provideItemIds[3],
            static_cast<unsigned int>(provideItemCounts[0]),
            static_cast<unsigned int>(provideItemCounts[1]),
            static_cast<unsigned int>(provideItemCounts[2]),
            static_cast<unsigned int>(provideItemCounts[3]));
        std::uint16_t slotCounters[4] = {};
        for (int index = 0; index < 40 && provideItemIds[index] != 0; ++index) {
            TB_ITEM* itemRow = loginServer->GetResourceMgr().GetTB_ITEM(provideItemIds[index]);
            if (!itemRow) {
                LogHelper::LogDebug("game.contents",
                                    "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate starter-miss uaid=%d kind=provide-item itemId=%u index=%d",
                                    createRequest.nUAID,
                                    provideItemIds[index],
                                    index);
                continue;
            }

            TB_ITEM_CLASSIFY* classifyRow =
                loginServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(itemRow->Item_Classify_Index);
            if (!classifyRow) {
                LogHelper::LogDebug("game.contents",
                                    "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate starter-miss uaid=%d kind=provide-classify classifyId=%u itemId=%u index=%d",
                                    createRequest.nUAID,
                                    itemRow->Item_Classify_Index,
                                    provideItemIds[index],
                                    index);
                continue;
            }

            if (classifyRow->GroupID == 11) {
                if (classifyRow->CategoryID == 1) {
                    defaultConsumeCategory1 += provideItemCounts[index];
                } else if (classifyRow->CategoryID == 3) {
                    defaultConsumeCategory3 += provideItemCounts[index];
                }
                continue;
            }

            PS_DEFAULT_INVEN_ITEM defaultItem{};
            loginServer->GetItemFactory().CreateItem(
                defaultItem.stItem,
                static_cast<int>(provideItemIds[index]),
                static_cast<std::int16_t>(provideItemCounts[index]),
                false,
                &loginServer->GetResourceMgr(),
                false);

            std::uint8_t counterIndex = 0;
            if (classifyRow->Item_Inven_Type == 0) {
                counterIndex = 0;
                defaultItem.byInvenType = 2;
            } else if (classifyRow->Item_Inven_Type == 2) {
                counterIndex = 1;
                defaultItem.byInvenType = 4;
            } else if (classifyRow->Item_Inven_Type == 9) {
                counterIndex = 3;
                defaultItem.byInvenType = 11;
            } else if (classifyRow->Item_Inven_Type == 13) {
                counterIndex = 2;
                defaultItem.byInvenType = 13;
            } else {
                continue;
            }

            defaultItem.sPos = static_cast<std::int16_t>(slotCounters[counterIndex]++);
            defaultItems.vecItems.push_back(defaultItem);
            for (int consumeIndex = 0; consumeIndex < 2; ++consumeIndex) {
                createRequest.nDefaultConsume[consumeIndex] =
                    static_cast<int>(provideItemIds[consumeIndex]);
            }
        }
    }

    const auto defaultSkillIds = characterInfoRow->GetDefaultSkillIDs();
    const auto quickSlotIds = characterInfoRow->GetQuickSlotIDs();
    const auto gestureSlotIds = characterInfoRow->GetGestureSlotIDs();

    std::memset(createRequest.nSkillDeck, 0, sizeof(createRequest.nSkillDeck));
    for (int index = 0; index < 20; ++index) {
        createRequest.nDefulatSkill[index] = static_cast<int>(defaultSkillIds[index]);
        const std::uint8_t quickSlotId = quickSlotIds[index];
        if (quickSlotId == 0) {
            continue;
        }

        const int deckColumn = quickSlotId / 10;
        const int deckRow = (quickSlotId % 10) - 1;
        if (deckColumn < 4 && deckRow < 6 && deckRow >= 0) {
            createRequest.nSkillDeck[deckRow][deckColumn] =
                static_cast<int>(defaultSkillIds[index]);
        }
    }

    for (int index = 0; index < 6; ++index) {
        createRequest.nDefulatGesture[index] =
            static_cast<int>(gestureSlotIds[index]);
    }

    LogHelper::LogDebug(
        "game.contents",
        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate equip uaid=%d weapon=%d shape=[head=%d,hands=%d,body=%d,stocking=%d,foot=%d,pants=%d] photo=%u defaultConsume=[%d,%d] defaultInven=%u",
        createRequest.nUAID,
        createRequest.stCharInfo.stSoulWeapon.dwItemID,
        createRequest.stCharInfo.stShapeEquipItemInfo[1].nItemID,
        createRequest.stCharInfo.stShapeEquipItemInfo[4].nItemID,
        createRequest.stCharInfo.stShapeEquipItemInfo[6].nItemID,
        createRequest.stCharInfo.stShapeEquipItemInfo[8].nItemID,
        createRequest.stCharInfo.stShapeEquipItemInfo[9].nItemID,
        createRequest.stCharInfo.stShapeEquipItemInfo[11].nItemID,
        createRequest.stCharInfo.stBaseInfo.dwProfilePhotoID,
        createRequest.nDefaultConsume[0],
        createRequest.nDefaultConsume[1],
        static_cast<unsigned int>(defaultItems.vecItems.size()));

    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_CREATE);
    sendPacket << createRequest;
    sendPacket << defaultItems;
    sendPacket.XParse << defaultConsumeCategory1;
    sendPacket.XParse << defaultConsumeCategory3;
    sendPacket.XParse << loginServer->GetCurDate();
    sendPacket.XParse << user->GetIPv4();
    sendPacket.XParse << static_cast<std::uint16_t>(user->GetCharacterCount());
    sendPacket.XParse << static_cast<int>(loginServer->GetServerID());
    sendPacket << weaponItem;
    for (const STItem& clothItem : clothItems) {
        sendPacket << clothItem;
    }

    LogHelper::LogDebug(
        "game.contents",
        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterCreate send-db uaid=%d class=%u slot=%u defaultInven=%u defaultConsumeCategory=[%llu,%llu] charCount=%u serverID=%d",
        createRequest.nUAID,
        static_cast<unsigned int>(createRequest.stCharInfo.stBaseInfo.byClass),
        static_cast<unsigned int>(createRequest.stCharInfo.byCharSlotPos),
        static_cast<unsigned int>(defaultItems.vecItems.size()),
        static_cast<unsigned long long>(defaultConsumeCategory1),
        static_cast<unsigned long long>(defaultConsumeCategory3),
        static_cast<unsigned int>(user->GetCharacterCount()),
        static_cast<int>(loginServer->GetServerID()));

    loginServer->SendDBGame(sendPacket);
    user->SetWaitCreateCharacterPacketRes(true);
    user->SetTickCreateCharacterPacketRes();
    return true;
}

bool CCharacterProcess::ReqSelectCharacter(XPacket& xPacket) {
    PS_CHARACTER_SELECT selectInfo{};
    xPacket >> selectInfo;

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSelectCharacter request session=%d uaid=%d requestUCID=%u currentSelectUCID=%u lastSelectUCID=%u pendingSelectUCID=%u secondPW=%u tradePW=%u enterState=%d",
                        user->GetSessionID(),
                        user->GetUAID(),
                        static_cast<unsigned int>(selectInfo.dwActorID),
                        user->GetSelectUCID(),
                        user->GetLastSelectUCID(),
                        user->GetPendingSelectUCID(),
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()),
                        static_cast<int>(user->GetEnterServerState()));
    user->SetPendingSelectUCID(static_cast<unsigned int>(selectInfo.dwActorID));

    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        SendEnterMapFailure(user, 50106);
        LogHelper::LogError("game.contents",
                            "<LOGIN> ReqSelectCharacter : duplication %d ",
                            selectInfo.dwActorID);
        return false;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (loginServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_SECOND_PW) &&
        !user->CheckSecondPasswordState()) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSelectCharacter second-pw-gate session=%d uaid=%d requestUCID=%u pendingSelectUCID=%u secondPW=%u",
                            user->GetSessionID(),
                            user->GetUAID(),
                            static_cast<unsigned int>(selectInfo.dwActorID),
                            user->GetPendingSelectUCID(),
                            static_cast<unsigned int>(user->GetSecondPWState()));
        SendEnterMapFailure(user, 50104);

        PS_CONTENTS_INFO serverContents{};
        loginServer->GetResourceMgr().GetServerContents(&serverContents);

        XSendPacket sendPacket(1, 7);
        sendPacket << serverContents;
        user->BridgeSend(sendPacket);
        return true;
    }

    if (!loginServer->GetControlSocket().IsReady()) {
        SendEnterMapFailure(user, 50003);
        LogHelper::LogError("game.contents",
                            "<LOGIN> ReqSelectCharacter : Not Connect %d ",
                            selectInfo.dwActorID);
        return false;
    }

    if (!user->CanEnterGame(selectInfo.dwActorID)) {
        SendEnterMapFailure(user, 50104);
        LogHelper::LogError("game.contents",
                            "<LOGIN> CanEnterGame %d ",
                            selectInfo.dwActorID);
        return false;
    }

    user->SetEnterServerState(ENTER_SERVER_STATE_SELECT_GAME_REQ);
    selectInfo.dwUAID = static_cast<unsigned int>(user->GetUAID());
    user->UpdateCharacterMapInfo(selectInfo);

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSelectCharacter prepared session=%d uaid=%d requestUCID=%u prevMapID=%d prevRevivePoint=%d pendingSelectUCID=%u",
                        user->GetSessionID(),
                        user->GetUAID(),
                        static_cast<unsigned int>(selectInfo.dwActorID),
                        selectInfo.nPrevMapID,
                        selectInfo.nPrevRevivePoint,
                        user->GetPendingSelectUCID());

    std::uint16_t autoMailTimeValue = 0;
    if (TB_SYSTEMMAIL_ADD* autoMailRow = loginServer->GetResourceMgr().GetTB_SYSTEMMAIL_ADD(2);
        autoMailRow && autoMailRow->AutoMail_Type_On_Off != 0) {
        autoMailTimeValue = autoMailRow->AutoMail_Time_Value;
    }

    std::uint8_t weeklyMissionGroupIDComeBack = 0;
    std::uint8_t weeklyMissionGroupIDAccountComeBack = 0;
    ExtractWeeklyMissionGroupIDs(loginServer->GetResourceMgr(),
                                 weeklyMissionGroupIDComeBack,
                                 weeklyMissionGroupIDAccountComeBack);

    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_SELECT);
    sendPacket << selectInfo;
    sendPacket.XParse << autoMailTimeValue;
    sendPacket.XParse << weeklyMissionGroupIDComeBack;
    sendPacket.XParse << weeklyMissionGroupIDAccountComeBack;
    loginServer->SendDBGame(sendPacket);
    return true;
}

/**
 * @brief 处理客户端继续切服请求。
 *
 * 根据重新在 IDA 中核实的 `CCharacterProcess::ReqCharacterChangeServer(0x140004680)`：
 * 1. 解包 `PS_REQ_CHANGE_SERVER`
 * 2. 把 `dwActorID` 清零，再把 `dwUAID` 改写为当前正式账号 UAID
 * 3. 当 `byType != 0` 时直接返回 `false`
 * 4. 当控制/Relay 已就绪时，向其发送 `main=0xF3, sub=0x12`
 * 5. 否则回一条 `0x60 / 0x60` 错误并记录 not connect 日志
 */
bool CCharacterProcess::ReqCharacterChangeServer(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    PS_REQ_CHANGE_SERVER changeServerReq{};
    xPacket >> changeServerReq;
    changeServerReq.dwActorID = 0;
    changeServerReq.dwUAID = static_cast<unsigned int>(user->GetUAID());

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterChangeServer session=%d uaid=%u actor=%u byType=%u ready=%d user=%p",
                        user->GetSessionID(),
                        changeServerReq.dwUAID,
                        changeServerReq.dwActorID,
                        static_cast<unsigned int>(changeServerReq.byType),
                        TXSingleton<XLoginServer>::Instance()->GetControlSocket().IsReady() ? 1 : 0,
                        static_cast<void*>(user));

    if (changeServerReq.byType != 0) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterChangeServer ignore-nonzero-type session=%d uaid=%u byType=%u",
                            user->GetSessionID(),
                            changeServerReq.dwUAID,
                            static_cast<unsigned int>(changeServerReq.byType));
        return false;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (!loginServer->GetControlSocket().IsReady()) {
        SendErrorMessage(0x60, 0x60);
        LogHelper::LogError("game.contents",
                            "<LOGIN> ReqCharacterChangeServer : Not Connect %d ",
                            user->GetUAID());
        return false;
    }

    XSendPacket relayPacket(0xF3, 0x12);
    relayPacket << changeServerReq;
    loginServer->GetControlSocket().GreenDamTan_RecordPendingChangeServer(changeServerReq,
                                                                          user->GetSessionID());
    const bool sendOk = loginServer->GetControlSocket().Send(relayPacket);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqCharacterChangeServer->SendControl session=%d uaid=%u byType=%u sendOk=%d",
                        user->GetSessionID(),
                        changeServerReq.dwUAID,
                        static_cast<unsigned int>(changeServerReq.byType),
                        sendOk ? 1 : 0);
    return true;
}


/**
 * @brief 处理客户端代表角色资格检查请求。
 *
 * 根据 IDA 中 `ReqCharacterRepresentativeCheck(0x140005294)`：
 * 1. 要求当前用户已处于 `ENTER_SERVER_STATE_SELECT_WORLD_RES` 且 `UAID > 0`
 * 2. 若仍在等待上一条代表角色回包，则直接回 `0xE8EE`
 * 3. 若本地已经缓存过检查通过结果，则直接回客户端 `main=3, sub=0x0D`
 * 4. 否则向 GameDB 发送 `main=3, sub=7`，包体只含 `UAID`
 */
bool CCharacterProcess::ReqCharacterRepresentativeCheck(XPacket& xPacket) {
    (void)xPacket;

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_REPRESENTATIVE_CHECK, 0xE8EEu);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterRepresentativeCheck> Check Enter Server State  %d/%d",
                            user->GetUAID(),
                            static_cast<int>(user->GetEnterServerState()));
        return false;
    }

    if (user->GetWaitRepresentativePacketRes()) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_REPRESENTATIVE_CHECK, 0xE8EEu);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterRepresentativeCheck> GetWaitRepresentativePacketRes  %d",
                            user->GetUAID());
        return false;
    }

    if (user->GetRepresentativeCheck()) {
        XSendPacket sendPacket(eCMD_CHARACTER, eSUB_CMD_CHARACTER_REPRESENTATIVE_CHECK);
        sendPacket.XParse << user->GetRepresentativeCheckResult();
        user->BridgeSend(sendPacket);
        return false;
    }

    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_REPRESENTATIVE_CHECK);
    sendPacket.XParse << user->GetUAID();
    TXSingleton<XLoginServer>::Instance()->SendDBGame(sendPacket);
    user->SetWaitRepresentativePacketRes(true);
    return true;
}

/**
 * @brief 处理客户端代表角色变更请求。
 *
 * 根据 IDA 中 `ReqCharacterRepresentativeChange(0x140005540)`：
 * 1. 先解包 `PS_CHARACTER_REPRESENTATIVE_CHANGE`
 * 2. 校验入服阶段与 `UAID`
 * 3. 若仍在等待上一条代表角色回包，则回 `0xE8EE`
 * 4. 调用 `CUser::CheckRepresentativeChange` 填充/校验结构
 * 5. 成功时把完整结构发往 GameDB `main=3, sub=8`
 */
bool CCharacterProcess::ReqCharacterRepresentativeChange(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    PS_CHARACTER_REPRESENTATIVE_CHANGE changeInfo{};
    xPacket >> changeInfo;

    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_REPRESENTATIVE_CHANGE, 0xE8EEu);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterRepresentativeChange> Check Enter Server State  %d/%d",
                            user->GetUAID(),
                            static_cast<int>(user->GetEnterServerState()));
        return false;
    }

    if (user->GetWaitRepresentativePacketRes()) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_REPRESENTATIVE_CHANGE, 0xE8EEu);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterRepresentativeChange> GetWaitRepresentativePacketRes  %d",
                            user->GetUAID());
        return false;
    }

    if (!user->CheckRepresentativeChange(changeInfo)) {
        user->SendErrorMessage(eCMD_CHARACTER,
                               eSUB_CMD_CHARACTER_REPRESENTATIVE_CHANGE,
                               static_cast<std::uint16_t>(changeInfo.nError));
        LogHelper::LogError("game.contents",
                            "<ReqCharacterRepresentativeChange> GetRepresentativeCheck  %d",
                            user->GetUAID());
        return false;
    }

    XSendDBPacket sendPacket(user, eCMD_CHARACTER, eSUB_SQL_CHARACTER_REPRESENTATIVE_CHANGE);
    sendPacket << changeInfo;
    TXSingleton<XLoginServer>::Instance()->SendDBGame(sendPacket);
    user->SetWaitRepresentativePacketRes(true);
    return true;
}

/**
 * @brief 处理客户端二级密码请求。
 *
 * 根据重新核实的 `CCharacterProcess::ReqSecondPassword(0x1400047c0)`：
 * 1. 仅当用户已处于 `ENTER_SERVER_STATE_SELECT_WORLD_RES` 且 `UAID > 0` 时才继续
 * 2. `byCheckType == 1` 时先做格式校验，成功后发 `sub=0x35`
 * 3. `byCheckType == 2` 时要求密码为 6 位数字，成功后发 `sub=0x36`
 * 4. `byCheckType == 3` 时发 `sub=0x34`
 * 5. `byCheckType == 4` 时发 `sub=0x37`
 * 6. 任一前置校验失败时，都立即回客户端 `main=3, sub=0x17`
 */
bool CCharacterProcess::ReqSecondPassword(XPacket& xPacket) {
    PS_SECOND_PW_REQ request{};
    xPacket >> request;

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSecondPassword session=%d uaid=%d checkType=%u enterState=%d secondPW=%u tradePW=%u",
                        user->GetSessionID(),
                        user->GetUAID(),
                        static_cast<unsigned int>(request.byCheckType),
                        static_cast<int>(user->GetEnterServerState()),
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()));

    if (user->GetEnterServerState() != ENTER_SERVER_STATE_SELECT_WORLD_RES || user->GetUAID() <= 0) {
        user->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_SECOND_PASSWORD, 0xC739u);
        LogHelper::LogError("game.contents",
                            "<ReqSecondPassword> Check Enter Server State  %d/%d",
                            user->GetUAID(),
                            static_cast<int>(user->GetEnterServerState()));
        return false;
    }

    const auto sendSecondPasswordFailure = [&](int errorId) -> bool {
        PS_SECOND_PW_RES result{};
        result.nErrorID = errorId;
        result.bySecondPWState = user->GetSecondPWState();

        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSecondPassword failure session=%d uaid=%d checkType=%u error=%d secondPW=%u tradePW=%u",
                            user->GetSessionID(),
                            user->GetUAID(),
                            static_cast<unsigned int>(request.byCheckType),
                            errorId,
                            static_cast<unsigned int>(user->GetSecondPWState()),
                            static_cast<unsigned int>(user->GetTradePWState()));

        XSendPacket sendPacket(eCMD_CHARACTER, eSUB_CMD_SECOND_PASSWORD);
        sendPacket << result;
        user->BridgeSend(sendPacket);
        return false;
    };

    switch (request.byCheckType) {
    case 1: {
        int errorId = 59501;
        if (!IsValidSecondPassword(request.strPassword, errorId)) {
            return sendSecondPasswordFailure(errorId);
        }

        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSecondPassword->SendDBAccount create session=%d uaid=%d secondPW=%u tradePW=%u",
                            user->GetSessionID(),
                            user->GetUAID(),
                            static_cast<unsigned int>(user->GetSecondPWState()),
                            static_cast<unsigned int>(user->GetTradePWState()));
        XSendDBPacket sendPacket(user, eCMD_LOGIN, eSUB_SQL_SECOND_PW_CREATE);
        sendPacket.XParse << user->GetUAID();
        sendPacket << request;
        TXSingleton<XLoginServer>::Instance()->SendDBAccount(sendPacket);
        return true;
    }
    case 2:
        if (std::strlen(request.strPassword) != 6) {
            return sendSecondPasswordFailure(59501);
        }

        for (int index = 0; index < 6; ++index) {
            if (request.strPassword[index] < '0' || request.strPassword[index] > '9') {
                return sendSecondPasswordFailure(59501);
            }
        }

        {
            LogHelper::LogDebug("game.system",
                                "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSecondPassword->SendDBAccount check session=%d uaid=%d secondPW=%u tradePW=%u",
                                user->GetSessionID(),
                                user->GetUAID(),
                                static_cast<unsigned int>(user->GetSecondPWState()),
                                static_cast<unsigned int>(user->GetTradePWState()));
            XSendDBPacket sendPacket(user, eCMD_LOGIN, eSUB_SQL_SECOND_PW_CHECK);
            sendPacket.XParse << user->GetUAID();
            sendPacket << request;
            TXSingleton<XLoginServer>::Instance()->SendDBAccount(sendPacket);
            return true;
        }
    case 3: {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSecondPassword->SendDBAccount continue session=%d uaid=%d secondPW=%u tradePW=%u",
                            user->GetSessionID(),
                            user->GetUAID(),
                            static_cast<unsigned int>(user->GetSecondPWState()),
                            static_cast<unsigned int>(user->GetTradePWState()));
        XSendDBPacket sendPacket(user, eCMD_LOGIN, eSUB_SQL_SECOND_PW_CONTINUE);
        sendPacket.XParse << user->GetUAID();
        TXSingleton<XLoginServer>::Instance()->SendDBAccount(sendPacket);
        return true;
    }
    case 4: {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log CharacterProcess.cpp::CCharacterProcess::ReqSecondPassword->SendDBAccount state-check session=%d uaid=%d secondPW=%u tradePW=%u",
                            user->GetSessionID(),
                            user->GetUAID(),
                            static_cast<unsigned int>(user->GetSecondPWState()),
                            static_cast<unsigned int>(user->GetTradePWState()));
        XSendDBPacket sendPacket(user, eCMD_LOGIN, eSUB_SQL_SECOND_PW_STATE_CHECK);
        sendPacket.XParse << user->GetUAID();
        TXSingleton<XLoginServer>::Instance()->SendDBAccount(sendPacket);
        return true;
    }
    default:
        return false;
    }
}

/**
 * @brief 校验二级密码格式。
 *
 * 该实现直接按 IDA 中 `IsValidSecondPassword(0x140004d80)` 的判定顺序保留：
 * - 必须是 6 位
 * - 必须全为数字
 * - 不允许连续 3 位相同
 * - 不允许连续 3 位递增或递减
 *
 * 错误码同样沿用反编译中已经明确出现的：
 * - `59501`：长度或字符非法
 * - `59502`：连续 3 位相同
 * - `59503`：连续 3 位递增或递减
 */
bool CCharacterProcess::IsValidSecondPassword(char* strPassword, int& nErrorID) {
    if (!strPassword) {
        return false;
    }

    if (std::strlen(strPassword) != 6) {
        nErrorID = 59501;
        return false;
    }

    int sameCount = 0;
    char sameValue = 0;
    int ascendingCount = 0;
    char ascendingValue = 0;
    int descendingCount = 0;
    char descendingValue = 0;

    for (int index = 0; index < 6; ++index) {
        const char current = strPassword[index];
        if (current < '0' || current > '9') {
            return false;
        }

        if (index == 0) {
            sameValue = current;
            sameCount = 1;
            ascendingValue = current;
            ascendingCount = 1;
            descendingValue = current;
            descendingCount = 1;
        } else {
            if (sameValue == current) {
                ++sameCount;
            } else {
                sameValue = current;
                sameCount = 1;
            }

            if (ascendingValue + 1 == current) {
                ascendingValue = current;
                ++ascendingCount;
            } else {
                ascendingValue = current;
                ascendingCount = 1;
            }

            if (descendingValue - 1 == current) {
                descendingValue = current;
                ++descendingCount;
            } else {
                descendingValue = current;
                descendingCount = 1;
            }
        }

        if (sameCount >= 3) {
            nErrorID = 59502;
            return false;
        }

        if (ascendingCount >= 3 || descendingCount >= 3) {
            nErrorID = 59503;
            return false;
        }
    }

    return true;
}
