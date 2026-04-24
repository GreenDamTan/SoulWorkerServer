#include "Soulworker/GameServer/XLoginServer/GameDBSocket.h"
#include "Soulworker/GameServer/XLoginServer/ServerMain.h"

#include <array>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <set>
#include <thread>

namespace {
constexpr std::uint8_t kCharacterMainCmd = 3;
constexpr std::uint8_t kSecondPasswordResultSubCmd = 0x17;
constexpr std::uint8_t kStatisticsMainCmd = 0xF0;
constexpr std::uint8_t kStatisticsCharacterCreateSubCmd = 0x01;
constexpr std::uint8_t kStatisticsCharacterSaveSubCmd = 0x02;
constexpr std::uint8_t kStatisticsSkillSubCmd = 0x08;

/**
 * @brief 读取登录结果结构。
 * @param packet DB 返回包。
 * @return 反序列化后的登录结果。
 */
PS_LOGIN_RES ReadLoginResult(XPacket& packet) {
    PS_LOGIN_RES result;
    packet >> result;
    return result;
}

/**
 * @brief 把登录结果重新序列化给客户端。
 * @param packet 输出包。
 * @param result 登录结果结构。
 */
void WriteLoginResult(XSendPacket& packet, const PS_LOGIN_RES& result) {
    packet << result;
}

/**
 * @brief 把窄字符 MAC 地址扩展成宽字符，便于写入日志。
 * @param macAddress 原始 MAC 缓冲区。
 * @return 转换后的宽字符串。
 */
std::wstring WidenMacAddress(const char* macAddress) {
    std::wstring widened;
    if (!macAddress) {
        return widened;
    }

    for (std::size_t index = 0; index < 18; ++index) {
        const char ch = macAddress[index];
        if (ch == '\0') {
            break;
        }
        widened.push_back(static_cast<wchar_t>(static_cast<unsigned char>(ch)));
    }
    return widened;
}

/**
 * @brief 把窄字符串按字节扩展为宽字符串。
 * @param value 输入字符串。
 * @return 简单 widening 结果。
 */
std::wstring WidenString(const std::string& value) {
    std::wstring widened;
    widened.reserve(value.size());
    for (char ch : value) {
        widened.push_back(static_cast<wchar_t>(static_cast<unsigned char>(ch)));
    }
    return widened;
}

/**
 * @brief 把网络序 IPv4 数值格式化成点分十进制字符串。
 * @param ipv4 `sin_addr.s_addr` 风格的 32bit IPv4。
 * @return 点分十进制字符串。
 */
std::string IPv4ToString(std::uint32_t ipv4) {
    std::array<unsigned char, 4> octets{};
    std::memcpy(octets.data(), &ipv4, sizeof(ipv4));

    char buffer[16] = {};
    std::snprintf(buffer,
                  sizeof(buffer),
                  "%u.%u.%u.%u",
                  static_cast<unsigned int>(octets[0]),
                  static_cast<unsigned int>(octets[1]),
                  static_cast<unsigned int>(octets[2]),
                  static_cast<unsigned int>(octets[3]));
    return std::string(buffer);
}

/**
 * @brief 向客户端回送二级密码结果包。
 * @param user 目标用户对象。
 * @param result 已解析好的二级密码结果。
 *
 * 依赖关系：
 * - 使用 `CUser::BridgeSend` 记录发送行为。
 * - 协议号遵循 IDA 中观察到的 `main=3, sub=0x17`。
 */
void SendSecondPasswordResult(CUser* user, const PS_SECOND_PW_RES& result) {
    if (!user) {
        return;
    }

    XSendPacket sendPacket(kCharacterMainCmd, kSecondPasswordResultSubCmd);
    sendPacket << result;
    user->BridgeSend(sendPacket);
}

/**
 * @brief 判断角色名是否以 `Bot` 开头。
 *
 * `ResSelectCharacter` 中存在一条特殊分支：当名字前缀是 `Bot`
 * 或某个位标志被置位时，会直接走“进入已有地图”流程。
 * 当前只按 IDA 已确认的三字符前缀比较保留此语义。
 */
bool HasBotNamePrefix(const STBaseCharInfo& baseInfo) {
    return baseInfo.strName[0] == L'B' && baseInfo.strName[1] == L'o' && baseInfo.strName[2] == L't';
}

/**
 * @brief 记录建角成功后生成的物品细日志。
 *
 * 根据 `ResCharacterCreate(0x14000ac20)` 中直接构造 `ST_LOG_GAME` 的分支，
 * 当前已能稳定确认以下字段：
 * - `_sMainType = 4`
 * - `_sSubType = 3`
 * - `_nUAID = CUser::GetUAID()`
 * - `_nUCID = UXActorID::operator unsigned long()` 对应的原始 `dwActorID`
 * - `nParam0 / nParam1 / nParam5 / nParam11 / nParam12` 分别对应物品 ID、数量、序列号与重复数量
 */
void WriteCharacterCreateItemLog(CUser* user, const STMyCharInfoEx& createdCharacter, const STItem& item) {
    if (!user) {
        return;
    }

    ST_LOG_GAME logGame{};
    logGame._nUAID = user->GetUAID();
    logGame._nUCID = static_cast<int>(static_cast<unsigned long>(createdCharacter.uxActorID));
    logGame._sMainType = 4;
    logGame._sSubType = 3;
    logGame.nParam0 = item.nItemID;
    logGame.nParam1 = item.sCount;
    logGame.nParam5 = item.xSerial;
    logGame.nParam11 = item.sCount;
    logGame.nParam12 = item.sCount;
    TXSingleton<XLoginServer>::Instance()->WriteLogDB(logGame);
}
}

// 对齐 IDA: 参数类型 H = int
CUser* XGameDBSocket::FindUser(int xSessionID) {
    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (!loginServer) {
        return nullptr;
    }

    return loginServer->TXServer<CUser>::FindUser(xSessionID);
}

bool XGameDBSocket::DBParse(CUser* pUser, XPacket& xPacket) {
    const std::uint8_t byMainCmd = xPacket.GetMainCmd();
    if (byMainCmd == 2) {
        return DBLoginParse(pUser, xPacket);
    }
    if (byMainCmd == 3) {
        return DBCharacterParse(pUser, xPacket);
    }
    return true;
}

void XGameDBSocket::OnDisConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnDisconnect ( %d ) !!", m_byType);
    XIOCPClient::OnDisConnect();
    if (m_byType != 4) {
        XIOCPClient::Shutdown(0xFFFFFFFFu);
    }

    if (m_byType == 0 && !m_bSafetyShutdown) {
        LogHelper::LogError("game.system", "[DB_SOCKET] OnDisConnect !!");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        SET_SERVICE_STATE(1u, 3u);
        if (auto* loginServer = TXSingleton<XLoginServer>::Instance()) {
            loginServer->Shutdown(0xFFFFFFFFu);
        }
    }
}

void XGameDBSocket::OnNotConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnNotConnect ( %d ) !!", m_byType);
    if (!m_bCloseProcess && m_byType != 4) {
        m_bCloseProcess = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        SET_SERVICE_STATE(1u, 3u);
        if (auto* loginServer = TXSingleton<XLoginServer>::Instance()) {
            loginServer->Shutdown(0xFFFFFFFFu);
        }
    }
}

/**
 * @brief 分派登录阶段的 DB 回包。
 *
 * 对应 IDA 中的 `DBLoginParse`，当前只把已经还原清楚的子命令接入到骨架工程：
 * - `sub=1` 登录结果
 * - `sub=0x11` 入服结果
 * - `sub=0x14` 服务器组角色计数
 * - `sub=0x31` 选项与内容开关加载
 * - `sub=0x34~0x37` 二级密码结果
 * - `sub=0x53/0x58` SG 认证信息加载
 *
 * @param pUser 当前登录连接对应的用户对象。
 * @param xPacket DB 回包。
 * @return 成功分派返回 true。
 */
bool XGameDBSocket::DBLoginParse(CUser* pUser, XPacket& xPacket) {
    const std::uint8_t subCmd = xPacket.GetSubCmd();
    if (subCmd == 0x11) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::DBLoginParse sub=%u dbType=%u session=%d socket=%lld user=%p",
                            static_cast<unsigned int>(subCmd),
                            static_cast<unsigned int>(m_byType),
                            pUser ? pUser->GetSessionID() : -1,
                            pUser ? static_cast<long long>(pUser->Socket) : -1LL,
                            static_cast<void*>(pUser));
    }

    switch (subCmd) {
    case 1:
        return ResLogin(pUser, xPacket);
    case 0x11:
        return ResEnterServer(pUser, xPacket);
    case 0x14:
        return ResLoginCharacterCount(pUser, xPacket);
    case 0x31:
        return ResOptionLoad(pUser, xPacket);
    case 0x34:
        return ResSecondPWContinue(pUser, xPacket);
    case 0x35:
        return ResSecondPWCreate(pUser, xPacket);
    case 0x36:
        return ResSecondPWCheck(pUser, xPacket);
    case 0x37:
        return ResSecondPWStateCheck(pUser, xPacket);
    case 0x53:
    case 0x58:
        return ResSGAuthInfoLoad(pUser, xPacket);
    default:
        return true;
    }
}

/**
 * @brief 分派角色阶段的 DB 回包。
 *
 * 根据 IDA 中 `DBCharacterParse(0x14000a340)`，角色链路当前已知的子命令分派为：
 * - `sub=1`  : `ResCharacterList`
 * - `sub=2`  : `ResCharacterCreate`
 * - `sub=3`  : `ResCharacterDelete`
 * - `sub=4`  : `ResCharacterCheckName`
 * - `sub=6`  : `ResCharacterChangeSlot`
 * - `sub=7`  : `ResCharacterRepresentativeCheck`
 * - `sub=8`  : `ResCharacterRepresentativeChange`
 * - `sub=0x22` : `ResSelectCharacter`
 * - `sub=0x39` : `ResCharacterUpdateMap`
 * - `sub=0x42` : `ResCharacterChangeServer`
 *
 * 当前骨架已把当前已核实清楚的分支都真正接进来；
 * 仍未核实的分支则继续保留为空。
 */
bool XGameDBSocket::DBCharacterParse(CUser* pUser, XPacket& xPacket) {
    if (xPacket.GetSubCmd() == 1) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::DBCharacterParse sub=1 session=%d user=%p",
                            pUser ? pUser->GetSessionID() : -1,
                            static_cast<void*>(pUser));
    }
    if (xPacket.GetSubCmd() == 2) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::DBCharacterParse sub=2 session=%d user=%p",
                            pUser ? pUser->GetSessionID() : -1,
                            static_cast<void*>(pUser));
    }
    switch (xPacket.GetSubCmd()) {
    case 1:
        return ResCharacterList(pUser, xPacket);
    case 2:
        return ResCharacterCreate(pUser, xPacket);
    case 3:
        return ResCharacterDelete(pUser, xPacket);
    case 4:
        return ResCharacterCheckName(pUser, xPacket);
    case 6:
        return ResCharacterChangeSlot(pUser, xPacket);
    case 0x22:
        return ResSelectCharacter(pUser, xPacket);
    case 0x39:
        return ResCharacterUpdateMap(pUser, xPacket);
    case 0x42:
        return ResCharacterChangeServer(pUser, xPacket);
    case 7:
        return ResCharacterRepresentativeCheck(pUser, xPacket);
    case 8:
        return ResCharacterRepresentativeChange(pUser, xPacket);
    default:
        return true;
    }
}

/**
 * @brief 处理建角回包，并在成功时刷新本地角色列表。
 *
 * 根据 IDA 中 `XGameDBSocket::ResCharacterCreate(0x14000ac20)`，
 * 当前保留的已证实语义为：
 * 1. 先读取 `resultCode / unknownResponseParam / currentCharacterCount / STMyCharInfoEx`。
 * 2. 无论成功失败，都先清除 `WaitCreateCharacterPacketRes` 并记录耗时日志。
 * 3. `resultCode == 2` 时回客户端 `0xC73D`；其他非 0 错误回 `0xC743`。
 * 4. 成功时继续读取：默认背包、武器、6 件建角服装、20 个默认技能、2 个默认统计值。
 * 5. 把新角色写入 `CUser`，同步角色数量与创建时间，并通知 AccountDB `main=2, sub=0x24`。
 * 6. 继续向 LogDB 发送建角物品细日志，并向 StatisticsDB 发送建角统计、物品统计和去重后的技能统计。
 * 7. 最后调用 `SendCharacterList`，让客户端刷新角色列表。
 *
 * 其中 `unknownResponseParam` 的业务含义在当前函数体里只参与日志，
 * 仍待继续结合上游 SQL 结果结构核名。
 */
bool XGameDBSocket::ResCharacterCreate(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    int resultCode = 0;
    int unknownResponseParam = 0;
    std::uint16_t currentCharacterCount = 0;
    STMyCharInfoEx createdCharacter{};

    xPacket.XParse >> resultCode;
    xPacket.XParse >> unknownResponseParam;
    xPacket.XParse >> currentCharacterCount;
    xPacket >> createdCharacter;

    pUser->SetWaitCreateCharacterPacketRes(false);

    const std::uint64_t nowTick = static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count());
    const std::uint64_t elapsedTick = nowTick - pUser->GetTickCreateCharacterPacketRes();
    LogHelper::LogInfo("game.system",
                       "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResCharacterCreate uaid=%d elapsedMs=%llu result=%d",
                       createdCharacter.dwUAID,
                       static_cast<unsigned long long>(elapsedTick),
                       resultCode);

    if (resultCode == 2) {
        pUser->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC73Du);
        return true;
    }
    if (resultCode != 0) {
        pUser->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC743u);
        return true;
    }

    PS_DEFAULT_INVEN_ITEMS defaultItems{};
    STItem weaponItem{};
    std::array<STItem, 6> costumeItems{};
    std::array<int, 20> defaultSkillIds{};
    std::array<int, 2> defaultConsumeCounts{};

    xPacket >> defaultItems;
    xPacket >> weaponItem;
    for (STItem& costumeItem : costumeItems) {
        xPacket >> costumeItem;
    }
    for (int& skillId : defaultSkillIds) {
        xPacket.XParse >> skillId;
    }
    for (int& consumeCount : defaultConsumeCounts) {
        xPacket.XParse >> consumeCount;
    }

    LogHelper::LogDebug(
        "game.contents",
        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResCharacterCreate reply-items uaid=%u weapon=%d costume=[body=%d,hands=%d,foot=%d,stocking=%d,pants=%d,head=%d] defaultConsume=[%d,%d] defaultInven=%u userDB=0x%08X sync=0x%08X world=%d",
        createdCharacter.dwUAID,
        weaponItem.nItemID,
        costumeItems[0].nItemID,
        costumeItems[1].nItemID,
        costumeItems[2].nItemID,
        costumeItems[3].nItemID,
        costumeItems[4].nItemID,
        costumeItems[5].nItemID,
        defaultConsumeCounts[0],
        defaultConsumeCounts[1],
        static_cast<unsigned int>(defaultItems.vecItems.size()),
        static_cast<unsigned int>(createdCharacter.userDBBits.nUserDB),
        static_cast<unsigned int>(createdCharacter.syncUserBits.nSyncUser),
        static_cast<int>(createdCharacter.stPosInfo.sWorldID));

    LogHelper::LogDebug(
        "game.contents",
        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResCharacterCreate preset uaid=%u ucid=%u class=%u slot=%u appearance=[%u,%u,%u,%u] appearanceEx=[%u,%u,%u,%u] shape=[head=%d,hands=%d,body=%d,stocking=%d,foot=%d,pants=%d] weapon=%d photo=%u",
        createdCharacter.dwUAID,
        createdCharacter.uxActorID.dwActorID & 0x1FFFFFFF,
        static_cast<unsigned int>(createdCharacter.stBaseInfo.byClass),
        static_cast<unsigned int>(createdCharacter.byCharSlotPos),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearance.stAppearance.wAppearanceID[0]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearance.stAppearance.wAppearanceID[1]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearance.stAppearance.wAppearanceID[2]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearance.stAppearance.wAppearanceID[3]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[0]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[1]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[2]),
        static_cast<unsigned int>(createdCharacter.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[3]),
        createdCharacter.stShapeEquipItemInfo[1].nItemID,
        createdCharacter.stShapeEquipItemInfo[4].nItemID,
        createdCharacter.stShapeEquipItemInfo[6].nItemID,
        createdCharacter.stShapeEquipItemInfo[8].nItemID,
        createdCharacter.stShapeEquipItemInfo[9].nItemID,
        createdCharacter.stShapeEquipItemInfo[11].nItemID,
        createdCharacter.stSoulWeapon.dwItemID,
        createdCharacter.stBaseInfo.dwProfilePhotoID);

    const unsigned int createdUCID = createdCharacter.uxActorID.dwActorID & 0x1FFFFFFF;
    const std::uint16_t newCharacterCount = static_cast<std::uint16_t>(currentCharacterCount + 1);
    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();

    pUser->AddCharacterInfo(createdCharacter);
    pUser->SetCharacterCount(newCharacterCount);
    pUser->SetCreateDate(createdCharacter.uxActorID.dwActorID, loginServer->GetCurDate());

    XSendDBPacket notifyAccountPacket(static_cast<IXObject*>(pUser), eCMD_LOGIN, 0x24);
    notifyAccountPacket.XParse << pUser->GetUAID();
    notifyAccountPacket.XParse << static_cast<std::uint16_t>(loginServer->GetOption().GetGroupID());
    notifyAccountPacket.XParse << newCharacterCount;
    loginServer->SendDBAccount(notifyAccountPacket);

    ST_STATISTICS_CHARACTER_CREATE createStatistics{};
    createStatistics.dwUAID = static_cast<unsigned int>(pUser->GetUAID());
    createStatistics.dwUCID = createdUCID;
    std::wmemset(createStatistics.strName, 0, std::size(createStatistics.strName));
    std::wmemcpy(createStatistics.strName,
                 createdCharacter.stBaseInfo.strName,
                 std::size(createStatistics.strName) - 1);
    createStatistics.byClass = createdCharacter.stBaseInfo.byClass;
    createStatistics.byCount = static_cast<std::uint8_t>(newCharacterCount);
    createStatistics.dwItem1 = static_cast<unsigned int>(defaultConsumeCounts[0]);
    createStatistics.dwItem2 = static_cast<unsigned int>(defaultConsumeCounts[1]);

    XSendDBPacket createStatisticsPacket(static_cast<IXObject*>(pUser),
                                         kStatisticsMainCmd,
                                         kStatisticsCharacterCreateSubCmd);
    createStatisticsPacket << createStatistics;
    loginServer->SendDBStatistics(createStatisticsPacket);

    pUser->SendCharacterList(createdUCID);

    for (const PS_DEFAULT_INVEN_ITEM& defaultItem : defaultItems.vecItems) {
        if (defaultItem.stItem.nItemID == 0) {
            continue;
        }
        WriteCharacterCreateItemLog(pUser, createdCharacter, defaultItem.stItem);
        SendStatisticsDB_Item(pUser,
                              1,
                              defaultItem.stItem.xSerial,
                              createdUCID,
                              static_cast<unsigned int>(defaultItem.stItem.nItemID),
                              defaultItem.stItem.byUpgrade,
                              defaultItem.stItem.byUpgradeLimit);
    }

    if (weaponItem.xSerial != -1 && weaponItem.nItemID != -1) {
        WriteCharacterCreateItemLog(pUser, createdCharacter, weaponItem);
        SendStatisticsDB_Item(pUser,
                              1,
                              weaponItem.xSerial,
                              createdUCID,
                              static_cast<unsigned int>(weaponItem.nItemID),
                              weaponItem.byUpgrade,
                              weaponItem.byUpgradeLimit);
    }

    for (const STItem& costumeItem : costumeItems) {
        if (costumeItem.xSerial == -1 || costumeItem.nItemID == -1) {
            continue;
        }
        WriteCharacterCreateItemLog(pUser, createdCharacter, costumeItem);
        SendStatisticsDB_Item(pUser,
                              1,
                              costumeItem.xSerial,
                              createdUCID,
                              static_cast<unsigned int>(costumeItem.nItemID),
                              costumeItem.byUpgrade,
                              costumeItem.byUpgradeLimit);
    }

    std::set<unsigned int> emittedSkillIds;
    for (int skillId : defaultSkillIds) {
        if (skillId <= 0) {
            continue;
        }
        if (!emittedSkillIds.insert(static_cast<unsigned int>(skillId)).second) {
            continue;
        }

        ST_STATISTICS_SKILL skillStatistics{};
        skillStatistics.byFlag = 1;
        skillStatistics.dwUCID = createdUCID;
        skillStatistics.dwSkill_New = static_cast<unsigned int>(skillId);

        XSendDBPacket skillStatisticsPacket(static_cast<IXObject*>(pUser),
                                            kStatisticsMainCmd,
                                            kStatisticsSkillSubCmd);
        skillStatisticsPacket << skillStatistics;
        loginServer->SendDBStatistics(skillStatisticsPacket);
    }

    LogHelper::LogInfo("game.contents",
                       "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResCharacterCreate success uaid=%d ucid=%u slot=%u class=%u count=%u unknown=%d",
                       pUser->GetUAID(),
                       createdUCID,
                       static_cast<unsigned int>(createdCharacter.byCharSlotPos),
                       static_cast<unsigned int>(createdCharacter.stBaseInfo.byClass),
                       static_cast<unsigned int>(newCharacterCount),
                       unknownResponseParam);
    return true;
}

/**
 * @brief 处理删角回包，并在成功后同步角色数量与统计链。
 *
 * 根据 IDA 中 `XGameDBSocket::ResCharacterDelete(0x14000b920)`，
 * 当前保留的已证实语义为：
 * 1. 先读取 `resultCode`，并立刻清除 `WaitDeleteCharacterPacketRes`。
 * 2. 若 `resultCode != 0`，则向客户端回 `0xC739`。
 * 3. 成功时继续读取 `nUCID / dwLastUCID / bNeedRefreshList`。
 * 4. 若本地仍能找到该角色，则先写一条删角审计日志。
 * 5. 角色数量减 1，并向 AccountDB 发送 `main=2, sub=0x24` 同步最新数量。
 * 6. 构造 `ST_STATISTICS_CHARACTER_SAVE`，并发送到 StatisticsDB `main=0xF0, sub=2`。
 * 7. 从本地角色列表删除该角色；若 `bNeedRefreshList` 为真，则重新下发角色列表。
 */
bool XGameDBSocket::ResCharacterDelete(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    int resultCode = 0;
    xPacket.XParse >> resultCode;
    pUser->SetWaitDeleteCharacterPacketRes(false);
    if (resultCode != 0) {
        pUser->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_LIST_RES, 0xC739u);
        return true;
    }

    int deletedUCID = -1;
    unsigned int lastUCID = 0;
    bool needRefreshList = false;
    xPacket.XParse >> deletedUCID;
    xPacket.XParse >> lastUCID;
    xPacket.XParse >> needRefreshList;

    if (STCharInfo* characterInfo = pUser->GetCharacterInfo(deletedUCID); characterInfo) {
        TXSingleton<XLoginServer>::Instance()->WriteLogDB(
            pUser->GetUAID(),
            deletedUCID,
            2,
            2,
            static_cast<int>(characterInfo->stBaseInfo.byClass),
            0,
            static_cast<int>(characterInfo->byLevel),
            0,
            0,
            0,
            0,
            std::wstring(characterInfo->stBaseInfo.strName),
            L"");
    }

    const std::uint16_t newCharacterCount = static_cast<std::uint16_t>(pUser->GetCharacterCount() - 1);
    pUser->SetCharacterCount(newCharacterCount);

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    XSendDBPacket notifyAccountPacket(static_cast<IXObject*>(pUser), eCMD_LOGIN, 0x24);
    notifyAccountPacket.XParse << pUser->GetUAID();
    notifyAccountPacket.XParse << static_cast<std::uint16_t>(loginServer->GetOption().GetGroupID());
    notifyAccountPacket.XParse << newCharacterCount;
    loginServer->SendDBAccount(notifyAccountPacket);

    ST_STATISTICS_CHARACTER_SAVE saveStatistics{};
    saveStatistics.byFlag = 3;
    saveStatistics.dwUCID = static_cast<unsigned int>(deletedUCID);

    XSendDBPacket saveStatisticsPacket(static_cast<IXObject*>(pUser),
                                       kStatisticsMainCmd,
                                       kStatisticsCharacterSaveSubCmd);
    saveStatisticsPacket << saveStatistics;
    loginServer->SendDBStatistics(saveStatisticsPacket);

    pUser->DeleteCharacterInfo(deletedUCID);
    if (needRefreshList) {
        pUser->SendCharacterList(lastUCID);
    }
    return true;
}

/**
 * @brief 处理角色换槽回包。
 *
 * 根据 IDA 中 `XGameDBSocket::ResCharacterChangeSlot(0x14000bd00)`：
 * 1. 解包 `PS_CHARACTER_CHANGE_SLOT`
 * 2. 清除 `WaitChangeSlotPacketRes`
 * 3. 失败时回客户端 `0xC745` 并记录错误日志
 * 4. 成功时应用本地换槽结果，重发角色列表，并追加一条换槽审计日志
 */
bool XGameDBSocket::ResCharacterChangeSlot(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_CHARACTER_CHANGE_SLOT slotChange{};
    xPacket >> slotChange;
    pUser->SetWaitChangeSlotPacketRes(false);

    if (slotChange.nErrorCode > 0) {
        pUser->SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_CHANGE_SLOT, 0xC745u);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterChangeSlot> if( stSlot.nErrorCode > 0 ) %d / %d",
                            pUser->GetUAID(),
                            slotChange.nErrorCode);
        return false;
    }

    pUser->ChangeCharacterSlot(slotChange);
    unsigned int lastUCID = pUser->GetLastSelectUCID();
    if (slotChange.byDestSlot > 8 || slotChange.bySrcSlot > 8) {
        lastUCID = static_cast<unsigned int>(slotChange.nSrcUCID);
    }
    pUser->SendCharacterList(lastUCID);

    TXSingleton<XLoginServer>::Instance()->WriteLogDB(
        pUser->GetUAID(),
        0,
        2,
        6,
        slotChange.nSrcUCID,
        static_cast<int>(slotChange.bySrcSlot),
        slotChange.nDestUCID,
        static_cast<int>(slotChange.byDestSlot),
        0,
        0,
        0,
        L"",
        L"");
    return true;
}

/**
 * @brief 处理角色名检测回包。
 *
 * 根据 IDA 中 `XGameDBSocket::ResCharacterCheckName(0x14000cea0)`：
 * 1. 解包 `PS_RES_CHECK_NAME`
 * 2. 构造客户端 `main=3, sub=0x57`
 * 3. 原样把检测结果回给客户端
 */
bool XGameDBSocket::ResCharacterCheckName(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_RES_CHECK_NAME checkNameResult{};
    xPacket >> checkNameResult;

    XSendPacket sendPacket(eCMD_CHARACTER, eSUB_CMD_CHARACTER_CHECK_NAME);
    sendPacket << checkNameResult;
    pUser->BridgeSend(sendPacket);
    return true;
}

/**
 * @brief 处理 AccountDB 返回的登录结果。
 *
 * 关键步骤：
 * 1. 反序列化 `PS_LOGIN_RES`。
 * 2. 根据错误码推进用户状态或执行顶号踢线。
 * 3. 写入登录审计日志。
 * 4. 把结果重新封装回客户端。
 *
 * @param pUser 当前登录连接对应的用户对象。
 * @param xPacket AccountDB 返回的登录结果包。
 * @return 有效用户返回 true，否则返回 false。
 */
bool XGameDBSocket::ResLogin(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    PS_LOGIN_RES result = ReadLoginResult(xPacket);
    result.bClearTutorial = true;

    switch (result.nErrorCode) {
    case 0:
        // 登录成功：写回 UAID、SessionID，并登记在线映射。
        pUser->SetAuthSessionID(result.biAuthSessionID);
        pUser->SetUAID(result.nUAID);
        loginServer->EnterUser(pUser);
        LogHelper::LogDebug("game.contents", "<LOGIN> Successed Login : %d ", result.nUAID);
        pUser->SetEnterServerState(ENTER_SERVER_STATE_LOGIN_RES);
        break;
    case 1:
        LogHelper::LogError("game.contents", "<LOGIN> Failed Login - Wrong Password!");
        pUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        break;
    case 2: {
        // 顶号分支：通知 Relay，并在本地在线表里踢掉旧会话。
        pUser->SetUAID(result.nUAID);
        PS_KICK_USER_INFO kickInfo{};
        kickInfo.dwUAID = static_cast<unsigned int>(result.nUAID);
        kickInfo.byKickType = 1;
        loginServer->GetControlSocket().SendUserKickout(kickInfo);

        if (CUser* existingUser = loginServer->FindUIDToUser(result.nUAID)) {
            existingUser->Kickout(kickInfo);
            existingUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        }

        LogHelper::LogError("game.contents", "<LOGIN> Failed Login - User in Game Already! ( %d ) ", result.nUAID);
        break;
    }
    case 3:
        LogHelper::LogError("game.contents", "<LOGIN> Failed Login - Block AccountID!");
        pUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        break;
    case 4:
        LogHelper::LogError("game.contents", "<LOGIN> Failed Login - Block IP!");
        pUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        break;
    case 5:
        LogHelper::LogError("game.contents", "<LOGIN> Failed Login - Block MAC!");
        pUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        break;
    case 6:
        LogHelper::LogError("game.contents", "<LOGIN> Failed Login - Wrong MAC!");
        pUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        break;
    case 7:
        LogHelper::LogError("game.contents", "<LOGIN> Server System Check!");
        pUser->SetEnterServerState(ENTER_SERVER_STATE_NONE);
        break;
    default:
        LogHelper::LogError("game.contents", "<LOGIN> Failed Login (%d) - UAID : %d", result.nErrorCode, result.nUAID);
        break;
    }

    if (result.nUAID != 0) {
        loginServer->WriteLogDB(result.nUAID,
                                0,
                                1,
                                result.byLoginType,
                                0,
                                result.nErrorCode,
                                pUser->GetIPv4(),
                                0,
                                0,
                                0,
                                static_cast<std::int64_t>(result.biAuthSessionID),
                                WidenMacAddress(result.szMacAddress),
                                std::wstring(result.szAuthID));
    } else {
        LogHelper::LogError("game.contents", "<LOGIN> Log Failed ResUAID : %d, UserUAID : %d", result.nUAID, pUser->GetUAID());
    }

    XSendPacket sendPacket(eCMD_LOGIN, eSUB_CMD_LOGIN_RES);
    WriteLoginResult(sendPacket, result);
    pUser->BridgeSend(sendPacket);
    return true;
}

/**
 * @brief 处理 AccountDB 返回的入服结果。
 *
 * 关键步骤：
 * 1. 读取是否成功、UAID、二级密码/交易密码开关、SessionID、封禁标记、账号/MAC、GM 标志等字段。
 * 2. 无论成功与否，都先清理 `eStateEnterWaitDB` 状态并回包 `main=2, sub=0x14`。
 * 3. 成功时写回用户对象，登记在线表，并继续下发当前世界时间包
 *    `main=eCMD_WORLD, sub=eSUB_CMD_WORLD_CUR_DATE`。
 * 4. 失败时记录日志并在必要时踢线。
 * 5. 最后减少一次 `SendGameDBUserCount`，表示该等待用户已完成 DB 阶段。
 *
 * @param pUser 当前连接对应的用户对象。
 * @param xPacket AccountDB 返回的入服结果包。
 * @return 有效用户返回 true，否则返回 false。
 */
bool XGameDBSocket::ResEnterServer(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    int resultCode = 0;
    int uaid = 0;
    std::uint8_t secondPWEnabled = 0;
    std::uint8_t tradePWEnabled = 0;
    std::uint64_t authSessionId = 0;
    std::uint8_t blockType = 0;
    char accountIdBuffer[24] = {};
    char macBuffer[24] = {};
    short stringLen = 0;
    int logResultParam = 0;
    std::uint8_t isGM = 0;

    xPacket.XParse >> resultCode;
    xPacket.XParse >> uaid;
    xPacket.XParse >> secondPWEnabled;
    xPacket.XParse >> tradePWEnabled;
    xPacket.XParse >> authSessionId;
    xPacket.XParse >> blockType;
    xPacket.XParse.GetString(accountIdBuffer, 21, &stringLen);
    xPacket.XParse.GetString(macBuffer, 18, &stringLen);
    xPacket.XParse >> logResultParam;
    xPacket.XParse >> isGM;

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResEnterServer session=%d socket=%lld result=%d uaid=%d secondPW=%d tradePW=%d auth=%llu block=%d logParam=%d gm=%d user=%p enterState=%d preSecondPW=%u preTradePW=%u preAuth=%llu",
                        pUser ? pUser->GetSessionID() : -1,
                        pUser ? static_cast<long long>(pUser->Socket) : -1LL,
                        resultCode,
                        uaid,
                        static_cast<unsigned int>(secondPWEnabled),
                        static_cast<unsigned int>(tradePWEnabled),
                        static_cast<unsigned long long>(authSessionId),
                        static_cast<unsigned int>(blockType),
                        logResultParam,
                        static_cast<unsigned int>(isGM),
                        static_cast<void*>(pUser),
                        pUser ? static_cast<int>(pUser->GetEnterServerState()) : -1,
                        pUser ? static_cast<unsigned int>(pUser->GetSecondPWState()) : 0u,
                        pUser ? static_cast<unsigned int>(pUser->GetTradePWState()) : 0u,
                        pUser ? static_cast<unsigned long long>(pUser->GetAuthSessionID()) : 0ull);

    pUser->ClearState(eStateEnterWaitDB);

    XSendPacket enterResultPacket(eCMD_LOGIN, 0x14);
    if (resultCode != 0) {
        LogHelper::LogError("game.contents", "[Error]ResEnterServer [Code:%d, UAID:%d]", resultCode, uaid);
        enterResultPacket.XParse << true;
    } else {
        const std::string accountId(accountIdBuffer);
        const std::string macAddress(macBuffer);

        TXSingleton<XLoginServer>::Instance()->WriteLogDB(uaid,
                                                          0,
                                                          1,
                                                          10,
                                                          0,
                                                          logResultParam,
                                                          pUser->GetIPv4(),
                                                          0,
                                                          0,
                                                          0,
                                                          0,
                                                          WidenString(macAddress),
                                                          WidenString(accountId));

        pUser->SetUAID(uaid);
        pUser->SetSecondPWState(secondPWEnabled);
        pUser->SetTradePWState(tradePWEnabled);
        pUser->SetBlockType(blockType);
        pUser->SetAuthSessionID(authSessionId);
        pUser->SetAccountID(accountId);
        pUser->SetGM(isGM);

        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResEnterServer applied-account-state session=%d uaid=%d secondPW=%u tradePW=%u auth=%llu block=%u gm=%u",
                            pUser->GetSessionID(),
                            pUser->GetUAID(),
                            static_cast<unsigned int>(pUser->GetSecondPWState()),
                            static_cast<unsigned int>(pUser->GetTradePWState()),
                            static_cast<unsigned long long>(pUser->GetAuthSessionID()),
                            static_cast<unsigned int>(blockType),
                            static_cast<unsigned int>(isGM));

        if (TXSingleton<XLoginServer>::Instance()->EnterUser(pUser)) {
            enterResultPacket.XParse << false;
        } else {
            enterResultPacket.XParse << true;
            PS_KICK_USER_INFO kickInfo{};
            kickInfo.dwUAID = static_cast<unsigned int>(uaid);
            kickInfo.byKickType = 1;
            TXSingleton<XLoginServer>::Instance()->GetControlSocket().SendUserKickout(kickInfo);
            pUser->Kickout(kickInfo);
        }
    }

    enterResultPacket.XParse << uaid;
    pUser->BridgeSend(enterResultPacket);
    pUser->SetEnterServerState(ENTER_SERVER_STATE_SELECT_WORLD_RES);

    ST_WORLD_CUR_DATE worldDate{};
    TXSingleton<XLoginServer>::Instance()->GetCurDate(worldDate);
    XSendPacket worldDatePacket(eCMD_WORLD, eSUB_CMD_WORLD_CUR_DATE);
    worldDatePacket << worldDate;
    pUser->BridgeSend(worldDatePacket);

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResEnterServer post-send session=%d uaid=%d result=%d enterState=%d secondPW=%u tradePW=%u auth=%llu",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        resultCode,
                        static_cast<int>(pUser->GetEnterServerState()),
                        static_cast<unsigned int>(pUser->GetSecondPWState()),
                        static_cast<unsigned int>(pUser->GetTradePWState()),
                        static_cast<unsigned long long>(pUser->GetAuthSessionID()));

    if (resultCode != 0) {
        PS_KICK_USER_INFO kickInfo{};
        kickInfo.dwUAID = static_cast<unsigned int>(uaid);
        kickInfo.byKickType = 1;
        pUser->Kickout(kickInfo);
    }

    TXSingleton<XLoginServer>::Instance()->AddSendGameDBUserCount(-1);
    return true;
}

/**
 * @brief 处理服务器组角色计数回包。
 *
 * 数据库会先回显请求里的 `UAID`，再返回 `ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC`。
 * 当前根据反编译结果，真正的业务动作是把角色计数列表交给
 * `XLoginServer::SendServerGroupList`，由后者统一打包成客户端所需的服务器列表包。
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的服务器组角色计数包。
 * @return 成功处理返回 true。
 */
bool XGameDBSocket::ResLoginCharacterCount(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    int echoedUAID = 0;
    ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC serverCharacterCounts{};
    xPacket.XParse >> echoedUAID;
    xPacket >> serverCharacterCounts;

    LogHelper::LogDebug("game.system",
                        "ResLoginCharacterCount uaid=%u echoedUAID=%d lastServer=%d itemCount=%zu",
                        pUser->GetUAID(),
                        echoedUAID,
                        serverCharacterCounts.nLastServerID,
                        serverCharacterCounts.vecInfo.size());
    if (echoedUAID != pUser->GetUAID()) {
        LogHelper::LogError("game.system",
                            "ResLoginCharacterCount uaid mismatch expected=%u echoed=%d",
                            pUser->GetUAID(),
                            echoedUAID);
    }
    for (const ST_USER_CHARACTER_COUNT_FOR_SERVER& info : serverCharacterCounts.vecInfo) {
        LogHelper::LogDebug("game.system",
                            "ResLoginCharacterCount item group=%d count=%u",
                            info.nServerGroupID,
                            static_cast<unsigned int>(info.wCount));
    }

    TXSingleton<XLoginServer>::Instance()->SendServerGroupList(pUser, serverCharacterCounts);
    return true;
}

/**
 * @brief 处理客户端选项/按键配置回包。
 *
 * 原始逻辑会：
 * 1. 读取 `ST_OPTION_BIT`
 * 2. 读取 `ST_USER_KEY_OPTION`
 * 3. 从资源管理器取当前服务器内容开关 `PS_CONTENTS_INFO`
 * 4. 拼成 `main=2, sub=0x31` 回给客户端
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的选项回包。
 * @return 成功处理返回 true。
 */
bool XGameDBSocket::ResOptionLoad(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    ST_OPTION_BIT optionBits{};
    ST_USER_KEY_OPTION userKeyOption{};
    PS_CONTENTS_INFO serverContents{};

    xPacket >> optionBits;
    xPacket >> userKeyOption;
    TXSingleton<XLoginServer>::Instance()->GetResourceMgr().GetServerContents(&serverContents);

    XSendPacket sendPacket(eCMD_LOGIN, 0x31);
    sendPacket << optionBits;
    sendPacket << serverContents;
    sendPacket << userKeyOption;
    pUser->BridgeSend(sendPacket);
    return true;
}

/**
 * @brief 处理“继续二级密码流程”回包。
 *
 * 根据 IDA 的 `ResSecondPWContinue`：
 * 1. 先读取一份 `PS_SECOND_PW_RES`
 * 2. 若 DB 返回“当前没有激活中的二级密码状态”，则把本地用户状态推进到 `2`
 * 3. 构造一个新的成功响应包回给客户端，状态值使用用户对象当前缓存值
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的二级密码继续流程回包。
 * @return 成功处理返回 true；用户为空返回 false。
 */
bool XGameDBSocket::ResSecondPWContinue(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_SECOND_PW_RES dbResult{};
    xPacket >> dbResult;
    const std::uint8_t previousState = pUser->GetSecondPWState();

    if (dbResult.bySecondPWState == 0) {
        pUser->SetSecondPWState(2);
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSecondPWContinue session=%d uaid=%d prevState=%u dbState=%u finalState=%u tradePW=%u",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        static_cast<unsigned int>(previousState),
                        static_cast<unsigned int>(dbResult.bySecondPWState),
                        static_cast<unsigned int>(pUser->GetSecondPWState()),
                        static_cast<unsigned int>(pUser->GetTradePWState()));

    PS_SECOND_PW_RES clientResult{};
    clientResult.nErrorID = 0;
    clientResult.bySecondPWState = pUser->GetSecondPWState();
    SendSecondPasswordResult(pUser, clientResult);
    return true;
}

/**
 * @brief 处理“创建二级密码”回包。
 *
 * 关键逻辑：
 * 1. 读取 `PS_SECOND_PW_RES`
 * 2. 把 DB 返回的状态值写回 `CUser`
 * 3. 直接向客户端回送同结构结果
 * 4. 若状态值为 `2`，则补记一条登录审计日志，保持与 IDA 中 `WriteLogDB(..., 11, ...)` 一致
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的创建二级密码结果包。
 * @return 成功处理返回 true；用户为空返回 false。
 */
bool XGameDBSocket::ResSecondPWCreate(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_SECOND_PW_RES result{};
    xPacket >> result;
    const std::uint8_t previousState = pUser->GetSecondPWState();
    pUser->SetSecondPWState(result.bySecondPWState);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSecondPWCreate session=%d uaid=%d prevState=%u newState=%u error=%d tradePW=%u",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        static_cast<unsigned int>(previousState),
                        static_cast<unsigned int>(result.bySecondPWState),
                        result.nErrorID,
                        static_cast<unsigned int>(pUser->GetTradePWState()));
    SendSecondPasswordResult(pUser, result);

    if (result.bySecondPWState == 2) {
        TXSingleton<XLoginServer>::Instance()->WriteLogDB(
            pUser->GetUAID(), 0, 1, 11, 0, 0, pUser->GetIPv4(), 0, 0, 0, 0, L"", L"");
    }
    return true;
}

/**
 * @brief 处理“校验二级密码”回包。
 *
 * 关键逻辑：
 * 1. 读取 `PS_SECOND_PW_RES`
 * 2. 把校验后的二级密码状态写回 `CUser`
 * 3. 转发同结构结果给客户端
 * 4. 追加一条 `loginType=13` 的审计日志，并把状态值作为错误/结果参数保留
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的校验二级密码结果包。
 * @return 成功处理返回 true；用户为空返回 false。
 */
bool XGameDBSocket::ResSecondPWCheck(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_SECOND_PW_RES result{};
    xPacket >> result;
    const std::uint8_t previousState = pUser->GetSecondPWState();
    pUser->SetSecondPWState(result.bySecondPWState);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSecondPWCheck session=%d uaid=%d prevState=%u newState=%u error=%d tradePW=%u",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        static_cast<unsigned int>(previousState),
                        static_cast<unsigned int>(result.bySecondPWState),
                        result.nErrorID,
                        static_cast<unsigned int>(pUser->GetTradePWState()));
    SendSecondPasswordResult(pUser, result);

    TXSingleton<XLoginServer>::Instance()->WriteLogDB(
        pUser->GetUAID(),
        0,
        1,
        13,
        0,
        static_cast<int>(result.bySecondPWState),
        pUser->GetIPv4(),
        0,
        0,
        0,
        0,
        L"",
        L"");
    return true;
}

/**
 * @brief 处理“查询二级密码状态”回包。
 *
 * 与创建/校验回包类似，但该分支只负责：
 * 1. 同步 `CUser` 中的二级密码状态
 * 2. 将 DB 返回包原样转发给客户端
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的二级密码状态查询包。
 * @return 成功处理返回 true；用户为空返回 false。
 */
bool XGameDBSocket::ResSecondPWStateCheck(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_SECOND_PW_RES result{};
    xPacket >> result;
    const std::uint8_t previousState = pUser->GetSecondPWState();
    pUser->SetSecondPWState(result.bySecondPWState);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSecondPWStateCheck session=%d uaid=%d prevState=%u newState=%u error=%d tradePW=%u",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        static_cast<unsigned int>(previousState),
                        static_cast<unsigned int>(result.bySecondPWState),
                        result.nErrorID,
                        static_cast<unsigned int>(pUser->GetTradePWState()));
    SendSecondPasswordResult(pUser, result);
    return true;
}

/**
 * @brief 处理 SG / 外部认证信息加载回包。
 *
 * 第 131 轮继续按 `DBLoginParse(0x14000A160)` 与共享地址 `0x14000E4F0` 复核后，
 * 当前二进制在 `sub=0x53 / 0x58` 上都会落到同一枚共享 `return true` stub；
 * PDB 虽然同时存在 `ResSGAuthInfoLoad / ResGFAuthInfoLoad` 名称，
 * 但登录服本地当前仍没有展开额外 payload 落地逻辑。
 * 为了与现有 IDA 行为保持一致，这里保留为显式 no-op；
 * 后续若认证链路被进一步还原，可在此处接入 `ST_SG_AUTH_INFO / ST_GF_AUTH_INFO` 的落盘逻辑。
 *
 * @param pUser 当前客户端对象；当前实现未直接使用。
 * @param xPacket DB 返回的认证信息包；当前实现未直接使用。
 * @return 始终返回 true。
 */
bool XGameDBSocket::ResSGAuthInfoLoad(CUser* pUser, XPacket& xPacket) {
    (void)pUser;
    (void)xPacket;
    return true;
}

/**
 * @brief 处理角色列表回包。
 *
 * 根据 IDA 中 `ResCharacterList(0x14000a500)`，当前已确认的关键流程为：
 * 1. 读取角色地图列表、段位等级/经验、删除角色过期时间、角色数量、代表角色信息。
 * 2. 清空并重建 `CUser` 上缓存的角色列表前置状态。
 * 3. 循环读取每个 `STMyCharInfoEx`、其联盟会长 `UCID`、创建时间与 `PS_BROACH_SHAPE_LIST`。
 * 4. 把账号级段位信息写回到每个角色条目，再落入 `CUser` 缓存。
 * 5. 记录最近选择角色，并在 `SortCharacterList` 不拦截时直接下发角色列表。
 * 6. 第 134 轮继续对照 `0x14000a500` 后，当前 `ResCharacterList` 的 GameDB 回包体
 *    已覆盖 `PS_CHARACTER_MAP_LIST -> Echelon -> 角色循环 -> dwLastUCID` 主方向，
 *    尚未发现会阻断 `SendCharacterList` 的额外缺失字段。
 *
 * 注意：
 * - `BroachEffect` 在原始程序里依赖 `TB_ITEM / TB_BROACH_SET` 资源表；
 *   当前工程尚未恢复这部分表驱动逻辑，因此这里只保留调用入口，不臆造 buff 计算。
 * - 反编译把角色数量解成了 `bool/char` 局部变量；结合循环、`SetCharacterCount`
 *   与发送逻辑，当前骨架按“一字节数量值”处理。
 */
bool XGameDBSocket::ResCharacterList(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_CHARACTER_MAP_LIST characterMapList{};
    std::uint8_t echelonLevel = 0;
    int echelonExp = 0;
    std::int64_t deleteCharListExpireTime = 0;
    std::uint8_t characterCount = 0;
    unsigned int representativeUCID = 0;
    std::uint64_t lastRepresentativeCharTime = 0;

    xPacket >> characterMapList;
    xPacket.XParse >> echelonLevel;
    xPacket.XParse >> echelonExp;
    xPacket.XParse >> deleteCharListExpireTime;
    xPacket.XParse >> characterCount;
    xPacket.XParse >> representativeUCID;
    xPacket.XParse >> lastRepresentativeCharTime;
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResCharacterList head session=%d uaid=%d count=%u lastUCID_pre=%u representativeUCID=%u deleteExpire=%lld echelon=[%u,%d]",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        static_cast<unsigned int>(characterCount),
                        0u,
                        representativeUCID,
                        static_cast<long long>(deleteCharListExpireTime),
                        static_cast<unsigned int>(echelonLevel),
                        echelonExp);


    pUser->ClearCharacterInfo();
    pUser->SetCharacterMapList(characterMapList);
    pUser->SetRepresentativeUCID(representativeUCID);
    pUser->SetLastRepresentativeCharTime(lastRepresentativeCharTime);
    pUser->SetEchelonLevel(echelonLevel);
    pUser->SetEchelonExp(echelonExp);

    for (std::uint8_t index = 0; index < characterCount; ++index) {
        STMyCharInfoEx characterInfo{};
        unsigned int leagueMasterUCID = 0;
        std::int64_t createDate = 0;
        PS_BROACH_SHAPE_LIST broachShapeList{};

        xPacket >> characterInfo;
        xPacket.XParse >> leagueMasterUCID;
        xPacket.XParse >> createDate;
        xPacket >> broachShapeList;

        characterInfo.byEchelonLevel = pUser->GetEchelonLevel();
        characterInfo.nEchelonExp = pUser->GetEchelonExp();

        for (const PS_BROACH_SHAPE& broachShape : broachShapeList.vecInfo) {
            BroachEffect(broachShape, &characterInfo);
        }

        pUser->AddCharacterInfo(characterInfo);
        pUser->CheckLeagueMaster(leagueMasterUCID);
        pUser->SetCreateDate(characterInfo.uxActorID.dwActorID, createDate);
    }

    unsigned int lastUCID = 0;
    xPacket.XParse >> lastUCID;
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResCharacterList body session=%d uaid=%d vectorSize=%zu lastUCID=%u characterCount=%u representativeUCID=%u deleteExpire=%lld",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        pUser->GetCharacterCount(),
                        lastUCID,
                        static_cast<unsigned int>(characterCount),
                        representativeUCID,
                        static_cast<long long>(deleteCharListExpireTime));
    pUser->SetCharacterCount(characterCount);

    if (!pUser->SortCharacterList(deleteCharListExpireTime)) {
        pUser->SendCharacterList(lastUCID);
    }

    TXSingleton<XLoginServer>::Instance()->WriteLogDB(
        pUser->GetUAID(), 0, 2, 11, static_cast<int>(characterCount), 0, 0, 0, 0, 0, 0, L"", L"");
    return true;
}

/**
 * @brief 处理选择角色后的切服/入图结果。
 *
 * 根据 IDA 中 `ResSelectCharacter(0x14000c1c0)`，当前已确认的主流程为：
 * 1. 读取结果码、`STCharInfo`、`nMapID / nJumpID / nState / dwPartyID / dwForceID`
 *    和一个额外布尔位。
 * 2. 失败时给客户端回 `main=3, sub=0x14`，错误码固定为 `51001`。
 * 3. 控制/Relay 未就绪时，回 `50003` 并返回 false。
 * 4. 成功时先把角色信息、客户端 IP、交易密码状态、SessionID、BlockType
 *    转发给控制/Relay 层的 `main=0xF3, sub=1`。
 * 5. 写回 `SelectUCID`、登记角色映射并设置 `eStateChangeServer`。
 * 6. 若位标志最低位被置位，或角色名前缀为 `Bot`，则走 `PS_ENTER_MAP_REQ` 直连分支。
 * 7. 否则构造 `ST_CREATE_MAZE`，并按原版顺序再次补一轮 `SetSelectUCID / AddActor` 后请求控制/Relay 层创建迷宫。
 * 8. 最后统一补一条角色进入阶段日志；`comment` 为角色名，`comment2` 为客户端 IP 宽字符串。
 *
 * 仍未臆造的部分：
 * - `nJumpID / bClearTutorial` 在当前登录服函数体内没有继续参与控制流，
 *   因此先按“已读未消费字段”保留，不额外臆造用途。
 * - `PS_ENTER_MAP_RES` / `ST_CREATE_MAZE` 仍是最小字段骨架，只覆盖当前函数已经明确消费的成员。
 */
bool XGameDBSocket::ResSelectCharacter(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    int resultCode = 0;
    STCharInfo selectedCharacter{};
    int targetMapID = 0;
    int targetJumpID = 0;
    int enterStateFlags = 0;
    unsigned int partyGroupID = 0;
    unsigned int forceGroupID = 0;
    bool clearTutorialFlag = false;

    xPacket.XParse >> resultCode;
    xPacket >> selectedCharacter;
    xPacket.XParse >> targetMapID;
    xPacket.XParse >> targetJumpID;
    xPacket.XParse >> enterStateFlags;
    xPacket.XParse >> partyGroupID;
    xPacket.XParse >> forceGroupID;
    xPacket.XParse >> clearTutorialFlag;
    (void)targetJumpID;
    (void)clearTutorialFlag;

    const unsigned int requestedUCID = pUser->GetPendingSelectUCID();
    const unsigned int selectedUCID =
        static_cast<unsigned int>(static_cast<unsigned long>(selectedCharacter.uxActorID));
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSelectCharacter session=%d uaid=%d result=%d requestedUCID=%u selectedUCID=%u currentSelectUCID=%u lastSelectUCID=%u targetMapID=%d flags=%d secondPW=%u tradePW=%u",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        resultCode,
                        requestedUCID,
                        selectedUCID,
                        pUser->GetSelectUCID(),
                        pUser->GetLastSelectUCID(),
                        targetMapID,
                        enterStateFlags,
                        static_cast<unsigned int>(pUser->GetSecondPWState()),
                        static_cast<unsigned int>(pUser->GetTradePWState()));

    if (resultCode != 0) {
        PS_ENTER_MAP_RES enterMapResult{};
        enterMapResult.nResult = 51001;

        XSendPacket sendPacket(kCharacterMainCmd, 0x14);
        sendPacket << enterMapResult;
        pUser->BridgeSend(sendPacket);
        return true;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    XRelaySocket& controlSocket = loginServer->GetControlSocket();
    if (!controlSocket.IsReady()) {
        PS_ENTER_MAP_RES enterMapResult{};
        enterMapResult.nResult = 50003;

        XSendPacket sendPacket(kCharacterMainCmd, 0x14);
        sendPacket << enterMapResult;
        pUser->BridgeSend(sendPacket);
        return false;
    }

    if (requestedUCID != 0 && requestedUCID != selectedUCID) {
        LogHelper::LogError("game.system",
                            "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSelectCharacter mismatched-ucid session=%d uaid=%d requestedUCID=%u selectedUCID=%u",
                            pUser->GetSessionID(),
                            pUser->GetUAID(),
                            requestedUCID,
                            selectedUCID);
    }

    XSendPacket relayPacket(0xF3, 1);
    relayPacket << selectedCharacter;
    relayPacket.XParse << pUser->GetIPv4();
    relayPacket.XParse << pUser->GetTradePWState();
    relayPacket.XParse << pUser->GetAuthSessionID();
    relayPacket.XParse << pUser->GetBlockType();
    controlSocket.Send(relayPacket);

    pUser->SetSelectUCID(selectedUCID);
    loginServer->AddActor(selectedUCID, pUser);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSelectCharacter actor-bound session=%d uaid=%d requestedUCID=%u selectedUCID=%u storedSelectUCID=%u",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        requestedUCID,
                        selectedUCID,
                        pUser->GetSelectUCID());
    pUser->SetState(eStateChangeServer);

    if ((enterStateFlags & 1) != 0 || HasBotNamePrefix(selectedCharacter.stBaseInfo)) {
        ST_PARTY_INFO partyInfo{};
        if (partyGroupID != 0) {
            partyInfo.byGroupType = 1;
            partyInfo.nID = static_cast<int>(partyGroupID);
        } else if (forceGroupID != 0) {
            partyInfo.byGroupType = 2;
            partyInfo.nID = static_cast<int>(forceGroupID);
        }

        if (partyGroupID != 0 && forceGroupID != 0) {
            LogHelper::LogError("game.contents",
                                "<LOGIN> ResSelectCharacter Party Force : %d / %d / %d ",
                                selectedUCID,
                                static_cast<int>(partyGroupID),
                                static_cast<int>(forceGroupID));
        }

        PS_ENTER_MAP_REQ enterMapRequest{};
        enterMapRequest.wMapID = static_cast<std::uint16_t>(targetMapID);
        enterMapRequest.nJumpID = 0;
        enterMapRequest.dwActorID = selectedUCID;
        enterMapRequest.dwUAID = selectedCharacter.dwUAID;
        enterMapRequest.stPartyInfo = partyInfo;
        enterMapRequest.byChangeType = 6;

        XSendPacket sendPacket(0xF2, 0x31);
        sendPacket << enterMapRequest;
        controlSocket.Send(sendPacket);

        LogHelper::LogDebug("game.contents",
                            "<LOGIN> eSUB_CMD_SERVER_USER_ENTER_SERVER : %d / %d / %d ",
                            selectedUCID,
                            static_cast<int>(partyInfo.byGroupType),
                            partyInfo.nID);
    } else {
        ST_CREATE_MAZE createMaze{};
        createMaze.wReqMapID = 21111;
        createMaze.nPortalID = 2111101;
        createMaze.nJumpID = 2111101;
        createMaze.dwUserID = selectedUCID;
        createMaze.dwUAID = selectedCharacter.dwUAID;

        const std::uint16_t characterInfoId =
            static_cast<std::uint16_t>(1000 * selectedCharacter.stBaseInfo.byClass);
        if (TB_CHARACTER_INFO* characterInfo =
                loginServer->GetResourceMgr().GetTB_CHARACTER_INFO(characterInfoId)) {
            createMaze.wReqMapID = characterInfo->Maze_ID;
            createMaze.nPortalID = static_cast<int>(characterInfo->Maze_Spawnbox_ID);
            createMaze.nJumpID = static_cast<int>(characterInfo->Maze_Spawnbox_ID);
        }

        ST_ENTER_MAZE_MEMBER_INFO enterMember{};
        enterMember.dwMember = selectedUCID;
        createMaze.vecEnterMember.push_back(enterMember);

        pUser->SetSelectUCID(selectedUCID);
        loginServer->AddActor(selectedUCID, pUser);
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log GameDBSocket.cpp::XGameDBSocket::ResSelectCharacter maze-actor-bound session=%d uaid=%d requestedUCID=%u selectedUCID=%u storedSelectUCID=%u",
                            pUser->GetSessionID(),
                            pUser->GetUAID(),
                            requestedUCID,
                            selectedUCID,
                            pUser->GetSelectUCID());
        controlSocket.SendCreateMazeReq(createMaze);

        LogHelper::LogDebug("game.contents",
                            "<LOGIN> eSUB_CMD_SERVER_CREATE_MAZE_REQ : %d / %d / %d / %d ",
                            createMaze.dwUserID,
                            static_cast<int>(createMaze.wReqMapID),
                            createMaze.nJumpID,
                            createMaze.nPortalID);
    }

    const std::wstring logComment2 = WidenString(IPv4ToString(pUser->GetIPv4()));
    loginServer->WriteLogDB(selectedCharacter.dwUAID,
                            static_cast<int>(selectedUCID),
                            2,
                            4,
                            selectedCharacter.stBaseInfo.byClass,
                            selectedCharacter.byLevel,
                            pUser->GetIPv4(),
                            targetMapID,
                            0,
                            0,
                            0,
                            std::wstring(selectedCharacter.stBaseInfo.strName),
                            logComment2);
    return true;
}

/**
 * @brief 处理角色切服结果回包。
 *
 * 根据 IDA 中 `ResCharacterChangeServer(0x14000dd70)`，当前可稳定确认的语义为：
 * 1. 读取一份 `PS_ENTER_MAP_RES`。
 * 2. `byChangeType == CHANGE_SERVER_TYPE_ENTER_MAP` 时，回客户端 `main=3, sub=0x15`。
 * 3. `byChangeType == CHANGE_SERVER_TYPE_ENTER_DISTRICT` 时，回客户端 `main=3, sub=0x14`。
 * 4. 其他 change type 当前函数不再额外处理。
 * 5. 最后统一设置 `eStateChangeServer`。
 *
 * 这里刻意不臆造其他 `CHANGE_SERVER_TYPE_*` 的后续行为，只保留当前反编译中
 * 已经明确出现的两个分支。
 */
bool XGameDBSocket::ResCharacterChangeServer(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_ENTER_MAP_RES enterMapResult{};
    xPacket >> enterMapResult;

    if (enterMapResult.byChangeType == CHANGE_SERVER_TYPE_ENTER_MAP) {
        XSendPacket sendPacket(kCharacterMainCmd, 0x15);
        sendPacket << enterMapResult;
        pUser->BridgeSend(sendPacket);
        LogHelper::LogDebug("game.contents", "<CHANGE_SERVER> CHANGE_SERVER_TYPE_ENTER_MAP");
    } else if (enterMapResult.byChangeType == CHANGE_SERVER_TYPE_ENTER_DISTRICT) {
        XSendPacket sendPacket(kCharacterMainCmd, 0x14);
        sendPacket << enterMapResult;
        pUser->BridgeSend(sendPacket);
        LogHelper::LogDebug("game.contents", "<CHANGE_SERVER> CHANGE_SERVER_TYPE_ENTER_DISTRICT");
    }

    pUser->SetState(eStateChangeServer);
    return true;
}

/**
 * @brief 处理角色地图更新结果回包。
 *
 * 根据 IDA 中 `ResCharacterUpdateMap(0x14000dee0)`，当前仅确认：
 * 1. 读取 `PS_ENTER_MAP_RES`
 * 2. 当 `byChangeType == CHANGE_SERVER_TYPE_LOGIN` 时，回客户端 `main=3, sub=0x14`
 * 3. 该分支日志文本仍然是 `CHANGE_SERVER_TYPE_ENTER_DISTRICT`
 * 4. 最后统一设置 `eStateChangeServer`
 *
 * 第 3 点看起来有些反直觉，但这是直接按反编译保留的原始表现，没有自行修正。
 */
bool XGameDBSocket::ResCharacterUpdateMap(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_ENTER_MAP_RES enterMapResult{};
    xPacket >> enterMapResult;

    if (enterMapResult.byChangeType == CHANGE_SERVER_TYPE_LOGIN) {
        XSendPacket sendPacket(kCharacterMainCmd, 0x14);
        sendPacket << enterMapResult;
        pUser->BridgeSend(sendPacket);
        LogHelper::LogDebug("game.contents", "<CHANGE_SERVER> CHANGE_SERVER_TYPE_ENTER_DISTRICT");
    }

    pUser->SetState(eStateChangeServer);
    return true;
}

/**
 * @brief 发送单个物品统计包到 StatisticsDB。
 *
 * 对应 IDA 中 `XGameDBSocket::SendStatisticsDB_Item(0x14000e510)`：
 * 这里只保留已证实的 `ST_STATISTICS_ITEM` 组包与转发方向。
 */
void XGameDBSocket::SendStatisticsDB_Item(CUser* pUser,
                                          std::uint8_t byFlag,
                                          std::int64_t biSerial,
                                          unsigned int dwUCID,
                                          unsigned int dwItemID,
                                          std::uint8_t byUpgrade,
                                          std::uint8_t byUpgradeLimit) {
    if (!pUser) {
        return;
    }

    ST_STATISTICS_ITEM statisticsItem{};
    statisticsItem.byFlag = byFlag;
    statisticsItem.biSerial = biSerial;
    statisticsItem.dwUCID = dwUCID;
    statisticsItem.dwItemID = dwItemID;
    statisticsItem.byUpgrade = byUpgrade;
    statisticsItem.byUpgradeLimit = byUpgradeLimit;

    XSendDBPacket statisticsPacket(static_cast<IXObject*>(pUser), kStatisticsMainCmd, 0x11);
    statisticsPacket << statisticsItem;
    TXSingleton<XLoginServer>::Instance()->SendDBStatistics(statisticsPacket);
}

/**
 * @brief 处理单个布罗奇外观项对角色显示信息的影响。
 * @param broachShape 单个三槽布罗奇组合。
 * @param characterInfo 待修饰的角色显示结构。
 *
 * 根据 IDA 中 `XGameDBSocket::BroachEffect(0x14000e640)`：
 * 1. 把 `PS_BROACH_SHAPE::dwItemID` 按 5 组、每组 3 件来检查。
 * 2. 每件物品都要求能命中 `TB_ITEM`，且 `Item_Effect_Type == 6`。
 * 3. 同一组三件的 `Item_Effect_ID` 必须一致，并按 `Item_Rank` 组合成 `nSetCode`。
 * 4. 再用 `10000 * effectID + 1000 * (groupIndex + 1) + nSetCode` 查 `TB_BROACH_SET`。
 * 5. 命中后向 `vecBuffInfo` 追加一条 `STBuffInfo`。
 */
void XGameDBSocket::BroachEffect(const PS_BROACH_SHAPE& broachShape, STMyCharInfoEx* characterInfo) {
    if (!characterInfo) {
        return;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (!loginServer) {
        return;
    }

    constexpr int kBroachGroupCount = 5;
    constexpr int kBroachItemsPerGroup = 3;
    constexpr std::uint16_t kItemEffectTypeBroachSet = 6;

    for (int groupIndex = 0; groupIndex < kBroachGroupCount; ++groupIndex) {
        int effectID = 0;
        int setCode = 0;
        int itemCount = 0;

        while (itemCount < kBroachItemsPerGroup) {
            const int itemID = broachShape.dwItemID[groupIndex * kBroachItemsPerGroup + itemCount];
            if (itemID < 1) {
                break;
            }

            TB_ITEM* itemRow = loginServer->GetResourceMgr().GetTB_ITEM(static_cast<unsigned int>(itemID));
            if (!itemRow || itemRow->Item_Effect_Type != kItemEffectTypeBroachSet) {
                break;
            }

            const int itemEffectID = static_cast<int>(itemRow->Item_Effect_ID);
            if (effectID != 0 && effectID != itemEffectID) {
                break;
            }
            if (effectID == 0) {
                effectID = itemEffectID;
            }

            const int weight = (itemCount == 0) ? 100 : ((itemCount == 1) ? 10 : 1);
            setCode += static_cast<int>(itemRow->Item_Rank) * weight;
            ++itemCount;
        }

        if (itemCount != kBroachItemsPerGroup || effectID == 0) {
            continue;
        }

        const std::uint16_t broachSetIndex = static_cast<std::uint16_t>(
            10000 * effectID + 1000 * (groupIndex + 1) + setCode);
        TB_BROACH_SET* broachSetRow = loginServer->GetResourceMgr().GetTB_BROACH_SET(broachSetIndex);
        if (!broachSetRow) {
            continue;
        }

        STBuffInfo buffInfo{};
        buffInfo.nBuffID = broachSetRow->Broach_Set_Buff;
        buffInfo.dwOwnerID = characterInfo->uxActorID.dwActorID;
        buffInfo.fTime = 0.0f;
        buffInfo.byCount = 1;
        buffInfo.bShow =
            (characterInfo->dwActiveBroachEffect == static_cast<unsigned int>(buffInfo.nBuffID));
        characterInfo->vecBuffInfo.push_back(buffInfo);
    }
}

/**
 * @brief 处理代表角色资格校验回包。
 *
 * 根据 IDA 中 `ResCharacterRepresentativeCheck` 的行为：
 * 1. 读取一个整型结果值
 * 2. 清空“等待代表角色结果”标记
 * 3. 结果为 0 时视为校验通过；否则记录错误码
 * 4. 向客户端回送 `main=3, sub=0x0D`
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的代表角色校验回包。
 * @return 成功处理返回 true；用户为空返回 false。
 */
bool XGameDBSocket::ResCharacterRepresentativeCheck(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    int resultCode = 0;
    xPacket.XParse >> resultCode;

    pUser->SetWaitRepresentativePacketRes(false);
    if (resultCode != 0) {
        pUser->SetRepresentativeCheck(false);
        pUser->SetRepresentativeCheckResult(resultCode);
    } else {
        pUser->SetRepresentativeCheck(true);
        pUser->SetRepresentativeCheckResult(0);
    }

    XSendPacket sendPacket(kCharacterMainCmd, 0x0D);
    sendPacket.XParse << resultCode;
    pUser->BridgeSend(sendPacket);
    return true;
}

/**
 * @brief 处理代表角色切换回包。
 *
 * 根据 IDA 中 `ResCharacterRepresentativeChange`：
 * 1. 读取 `PS_CHARACTER_REPRESENTATIVE_CHANGE`
 * 2. 清空等待标记
 * 3. 若 `nError > 0`，则给客户端发送错误码并记日志
 * 4. 否则更新用户对象上的代表角色 UCID/时间，并回送 `main=3, sub=0x0F`
 * 5. 最后写一条代表角色切换日志
 *
 * @param pUser 当前客户端对象。
 * @param xPacket DB 返回的代表角色切换回包。
 * @return 成功处理返回 true；用户为空返回 false。
 */
bool XGameDBSocket::ResCharacterRepresentativeChange(CUser* pUser, XPacket& xPacket) {
    if (!pUser) {
        return false;
    }

    PS_CHARACTER_REPRESENTATIVE_CHANGE result{};
    xPacket >> result;
    pUser->SetWaitRepresentativePacketRes(false);

    if (result.nError > 0) {
        pUser->SendErrorMessage(kCharacterMainCmd, 0x0F, 0xE8F0u);
        LogHelper::LogError("game.contents",
                            "<ResCharacterRepresentativeChange> if( nErrorCode > 0 ) %d / %d",
                            pUser->GetUAID(),
                            result.nError);
        return true;
    }

    const int previousRepresentativeUCID = static_cast<int>(pUser->GetRepresentativeUCID());
    pUser->SetRepresentativeUCID(result.dwUCID);
    pUser->SetLastRepresentativeCharTime(static_cast<std::uint64_t>(result.nRepresentativeDate));

    XSendPacket sendPacket(kCharacterMainCmd, 0x0F);
    sendPacket << result;
    pUser->BridgeSend(sendPacket);

    TXSingleton<XLoginServer>::Instance()->WriteLogDB(
        pUser->GetUAID(),
        0,
        2,
        7,
        previousRepresentativeUCID,
        static_cast<int>(result.dwUCID),
        0,
        0,
        0,
        0,
        0,
        L"",
        L"");
    return true;
}
