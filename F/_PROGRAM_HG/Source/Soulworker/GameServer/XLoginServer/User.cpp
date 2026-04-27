#include "Soulworker/GameServer/XLoginServer/User.h"

#include <cwchar>
#include <ctime>
#include <new>

#include "Soulworker/GameServer/XLoginServer/CharacterProcess.h"
#include "Soulworker/GameServer/XLoginServer/LoginProcess.h"
#include "Soulworker/GameServer/XLoginServer/LoginServer.h"
#include "Soulworker/GameServer/XLoginServer/SystemProcess.h"

namespace {
constexpr std::uint8_t kSystemMainCmd = eCMD_SYSTEM;
constexpr std::uint8_t kServerOptionSubCmd = 7;
constexpr std::uint8_t kCharacterMainCmd = eCMD_CHARACTER;
constexpr std::uint8_t kCharacterKickoutSubCmd = 4;
constexpr std::uint8_t kCharacterListSubCmd = 0x12;

std::uint64_t GetTickCount64Compat() {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count());
}

}

// XClient::SendErrorMessage 已移至共享层 GreenDamTan_XServerRuntime.cpp
// LoginServer 特有的 CUser 诊断逻辑（PushError / GreenDamTan_RecordSentPacket）
// 后续可通过虚函数 hook 或 override 重新接入

CUser::CUser(std::uint32_t ipv4) {
    if (!RegisterProcess()) {
        LogHelper::LogError("game.system", "CUser::RegisterProcess failed");
    }
    Init();
    SetIPv4(ipv4);
}

bool CUser::Init() {
    XClient::Init();

    m_eEnterServerState = ENTER_SERVER_STATE_NONE;
    m_stUserInfo.nUAID = 0;
    m_biAuthSessionID = 0;
    m_dw64CheckSessionIDTick = 0;

    m_dwUAID_Wait = 0;
    m_biTicket_Wait = 0;
    m_wLastServerIndex_Wait = 0;
    m_biAuthSessionID_Wait = 0;
    m_AddTime_Wait = {};
    m_bSendWaitPacket = false;
    m_bCancel_Wait = false;

    m_bySecondPassword = 0;
    m_byTradePassword = 0;
    m_byBlockType = 0;
    m_byGM = 0;
    std::memset(m_szAccountID, 0, sizeof(m_szAccountID));

    m_stSGAuthInfo = {};
    m_stGFAuthInfo = {};
    m_nSGAuthProcessState = 0;
    m_stUserInfo = {};
    m_stUserInfo.vSTCharInfo.clear();
    m_psMapList = {};
    m_psMapList.vecInfo.clear();
    m_wCharacterCount = 0;
    m_nDeleteCharListExpireTime = 0;
    m_dwRepresentativeUCID = 0;
    m_dwLastSelectUCID = 0;
    m_dwSelectedUCID = 0;
    m_nLastRepresentativeCharTime = 0;
    m_bRepresentativePacket = false;
    m_bRepresentativeCheck = false;
    m_nRepresentativeResult = 0;
    m_bCreateCharacterPacket = false;
    m_bCreateDeletePacket = false;
    m_dw64ChracterCreateTick = 0;
    m_bChangeSlotPacket = false;
    m_mapCreateDate.clear();
    ClearLeagueInfo();
    m_dwLeagueMasterUCID.reserve(6);

    m_byEchelonLevel = 0;
    m_nEchelonExp = 0;
    m_bDeleteUserInfo = true;
    kicked_ = false;
    errors_.clear();
    sentPackets_.clear();
    kickHistory_.clear();
    return true;
}

void CUser::SetSendCheckSessionID() {
    m_dw64CheckSessionIDTick = GetTickCount64Compat();
}

void CUser::SetAddTime_Wait() {
    m_AddTime_Wait = ATL::CTime::GetTickCount();
}

bool CUser::RegisterProcess() {
    IXProcess* loginProcess = new (std::nothrow) XLoginProcess();
    if (!Register(eCMD_LOGIN, loginProcess)) {
        delete loginProcess;
        return false;
    }

    IXProcess* characterProcess = new (std::nothrow) CCharacterProcess();
    if (!Register(eCMD_CHARACTER, characterProcess)) {
        delete characterProcess;
        return false;
    }

    IXProcess* systemProcess = new (std::nothrow) CSystemProcess();
    if (!Register(eCMD_SYSTEM, systemProcess)) {
        delete systemProcess;
        return false;
    }

    return true;
}

bool CUser::OnLogOut() {
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log User.cpp::CUser::OnLogOut session=%d uaid=%d state=0x%X delete=%d selectUCID=%u socket=%lld secondPW=%u tradePW=%u enterState=%d flags[change=%d,wait=%d,goBackAuth=%d,goBackLobby=%d,enterWaitDB=%d]",
                        GetSessionID(),
                        GetUAID(),
                        static_cast<unsigned int>(m_eNetState),
                        GetDeleteUserInfo() ? 1 : 0,
                        GetSelectUCID(),
                        static_cast<long long>(Socket),
                        static_cast<unsigned int>(GetSecondPWState()),
                        static_cast<unsigned int>(GetTradePWState()),
                        static_cast<int>(GetEnterServerState()),
                        IsState(eStateChangeServer) ? 1 : 0,
                        IsState(eStateEnterWait) ? 1 : 0,
                        IsState(eStateGoBackAuth) ? 1 : 0,
                        IsState(eStateGoBackLobby) ? 1 : 0,
                        IsState(eStateEnterWaitDB) ? 1 : 0);

    if (!IsState(eStateChangeServer) &&
        !IsState(eStateEnterWait) &&
        !IsState(eStateGoBackAuth)) {
        const int nIP = static_cast<int>(scAddr.sin_addr.s_addr);
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log User.cpp::CUser::OnLogOut normal-logout-path session=%d uaid=%d ip=%d secondPW=%u tradePW=%u",
                            GetSessionID(),
                            GetUAID(),
                            nIP,
                            static_cast<unsigned int>(GetSecondPWState()),
                            static_cast<unsigned int>(GetTradePWState()));

        XSendDBPacket sendPacket(this, 2, 2);
        sendPacket.XParse << GetUAID();
        sendPacket.XParse << 0;
        sendPacket.XParse << 0;
        sendPacket.XParse << nIP;
        TXSingleton<XLoginServer>::Instance()->SendDBAccount(sendPacket);

        if (GetUAID() != 0) {
            TXSingleton<XLoginServer>::Instance()->WriteLogDB(
                GetUAID(), 0, 1, 2, 0, 0, nIP, 0, 0, 0, static_cast<std::int64_t>(GetAuthSessionID()), L"\\", L"");
        } else {
            LogHelper::LogError("game.system", "CUser::OnLogOut UAID == 0");
        }
    } else {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log User.cpp::CUser::OnLogOut preserve-session-path session=%d uaid=%d secondPW=%u tradePW=%u goBackLobby=%d goBackAuth=%d change=%d wait=%d",
                            GetSessionID(),
                            GetUAID(),
                            static_cast<unsigned int>(GetSecondPWState()),
                            static_cast<unsigned int>(GetTradePWState()),
                            IsState(eStateGoBackLobby) ? 1 : 0,
                            IsState(eStateGoBackAuth) ? 1 : 0,
                            IsState(eStateChangeServer) ? 1 : 0,
                            IsState(eStateEnterWait) ? 1 : 0);
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log User.cpp::CUser::OnLogOut before-remove session=%d uaid=%d selectUCID=%u delete=%d",
                        GetSessionID(),
                        GetUAID(),
                        GetSelectUCID(),
                        GetDeleteUserInfo() ? 1 : 0);
    loginServer->RemoveActor(GetSelectUCID());
    if (GetDeleteUserInfo()) {
        loginServer->ExitUser(this);
    }

    if (IsState(eStateEnterWaitDB)) {
        loginServer->AddSendGameDBUserCount(-1);
    }

    SetState(eStateFinish);
    return true;
}


bool CUser::GetDeleteUserInfo() const {
    return m_bDeleteUserInfo;
}

void CUser::SetDeleteUserInfo(bool bRet) {
    m_bDeleteUserInfo = bRet;
}

bool CUser::BridgeSend(XSendPacket& packet) {
    packet.usTos = 1;
    // TODO: test-only audit path while the real socket send layer is still absent.
    GreenDamTan_RecordSentPacket(packet);
    if (packet.GetMainCmd() == kCharacterMainCmd && packet.GetSubCmd() == kCharacterListSubCmd) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log User.cpp::CUser::BridgeSend character-list payloadSize=%u fieldCount=%zu uaid=%d session=%d",
                            static_cast<unsigned int>(packet.GetPayloadSize()),
                            packet.XParse.Size(),
                            GetUAID(),
                            GetSessionID());
    }
    XClient::SendEx(packet);
    return true;
}

void CUser::Kickout(const PS_KICK_USER_INFO& kickInfo) {
    XSendPacket sendPacket(kCharacterMainCmd, kCharacterKickoutSubCmd);
    sendPacket << kickInfo;
    BridgeSend(sendPacket);

    kickHistory_.push_back(kickInfo);
    kicked_ = true;

    TXSingleton<XLoginServer>::Instance()->WriteLogDB(GetUAID(),
                                                      0,
                                                      2,
                                                      3,
                                                      kickInfo.byKickType,
                                                      kickInfo.nParam,
                                                      0,
                                                      0,
                                                      0,
                                                      0,
                                                      0,
                                                      L"\\",
                                                      L"");
    SetState(eStateKickOut);

    LogHelper::LogError("game.contents",
                        "<KICKOUT> Kickout ( %d / %d / %d ).",
                        GetUAID(),
                        static_cast<int>(kickInfo.byKickType),
                        kickInfo.nParam);
}

void CUser::SetLastSelectUCID(unsigned int dwLastUCID) {
    m_dwLastSelectUCID = dwLastUCID;
}

unsigned int CUser::GetLastSelectUCID() const {
    return m_dwLastSelectUCID;
}

void CUser::SetCreateDate(unsigned int dwUCID, std::int64_t biCreateDate) {
    m_mapCreateDate[dwUCID] = biCreateDate;
}

/**
 * @brief 向账号缓存中追加一名角色信息。
 * @param characterInfo 单个角色的展示结构。
 *
 * 当前骨架不实现原始工程中的读写锁，
 * 直接使用 `std::vector` 维护角色列表，方便后续继续扩展角色服/角色列表链路。
 */
void CUser::AddCharacterInfo(const STMyCharInfoEx& characterInfo) {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    m_stUserInfo.vSTCharInfo.push_back(characterInfo);
}

/**
 * @brief 按 `UCID` 查找当前账号缓存中的角色。
 *
 * 根据 IDA 中 `CUser::GetCharacterInfo(0x14002e760)`，
 * 当前只保留最稳定的语义：遍历 `m_stUserInfo.vSTCharInfo`，
 * 以 `uxActorID.dwActorID & 0x1FFFFFFF` 与传入 `UCID` 比较。
 */
STCharInfo* CUser::GetCharacterInfo(int nUCID) {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    for (STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        if (static_cast<int>(characterInfo.uxActorID.dwActorID & 0x1FFFFFFF) == nUCID) {
            return &characterInfo;
        }
    }
    return nullptr;
}

/**
 * @brief 按 `UCID` 删除本地角色缓存中的单个条目。
 *
 * 根据 IDA 中 `CUser::DeleteCharacterInfo(0x14002e650)`，
 * 这里只保留向量遍历与按 `UCID` 擦除的核心语义。
 */
bool CUser::DeleteCharacterInfo(int nUCID) {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    for (auto it = m_stUserInfo.vSTCharInfo.begin(); it != m_stUserInfo.vSTCharInfo.end(); ++it) {
        if (static_cast<int>(it->uxActorID.dwActorID & 0x1FFFFFFF) != nUCID) {
            continue;
        }
        m_stUserInfo.vSTCharInfo.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief 清空当前账号已缓存的角色列表信息。
 *
 * 依据 IDA 中 `CUser::ClearCharacterInfo` 的行为，
 * 当前至少需要：
 * 1. 清空 `vSTCharInfo`
 * 2. 把角色数量重置为 0
 */
void CUser::ClearCharacterInfo() {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    m_stUserInfo.vSTCharInfo.clear();
    SetCharacterCount(0);
}

/**
 * @brief 向客户端同步当前服务器内容开关。
 *
 * 对应 IDA 中的 `CUser::SendServerOption`：
 * 1. 从 `XLoginServer` 资源管理器中取得 `PS_CONTENTS_INFO`
 * 2. 构造 `main=1, sub=7` 的包
 * 3. 发给客户端
 */
void CUser::SendServerOption() {
    PS_CONTENTS_INFO serverOption{};
    TXSingleton<XLoginServer>::Instance()->GetResourceMgr().GetServerContents(&serverOption);

    XSendPacket sendPacket(kSystemMainCmd, kServerOptionSubCmd);
    sendPacket << serverOption;
    BridgeSend(sendPacket);
}

/**
 * @brief 向客户端下发角色列表与二级密码相关状态。
 * @param dwLastUCID 上次选择的角色 UCID。
 *
 * 依据 IDA 中 `CUser::SendCharacterList` 可确认的流程：
 * 1. 发送角色数量
 * 2. 逐个发送 `STMyCharInfoEx`
 * 3. 追加最近选择角色、二级密码/交易密码状态
 * 4. 追加删除角色过期时间、代表角色 UCID、最近代表角色时间
 * 5. 随后继续发送服务器内容开关
 */
void CUser::SendCharacterList(unsigned int dwLastUCID) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    XSendPacket sendPacket(kCharacterMainCmd, kCharacterListSubCmd);

    const std::uint8_t characterCount = static_cast<std::uint8_t>(std::min<std::size_t>(m_stUserInfo.vSTCharInfo.size(), 0xFF));
    sendPacket.XParse << characterCount;

    for (const STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        sendPacket << static_cast<const STCharInfo&>(characterInfo);
    }

    sendPacket.XParse << dwLastUCID;
    sendPacket.XParse << GetSecondPWState();
    sendPacket.XParse << GetTradePWState();
    sendPacket.XParse << GetDeleteCharListExpireTime();
    sendPacket.XParse << GetRepresentativeUCID();
    sendPacket.XParse << GetLastRepresentativeCharTime();
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log User.cpp::CUser::SendCharacterList summary lastUCID=%u vectorSize=%zu secondPW=%u tradePW=%u deleteExpire=%lld representativeUCID=%u representativeTime=%llu",
                        dwLastUCID,
                        m_stUserInfo.vSTCharInfo.size(),
                        static_cast<unsigned int>(GetSecondPWState()),
                        static_cast<unsigned int>(GetTradePWState()),
                        static_cast<long long>(GetDeleteCharListExpireTime()),
                        GetRepresentativeUCID(),
                        static_cast<unsigned long long>(GetLastRepresentativeCharTime()));

    BridgeSend(sendPacket);
    SendServerOption();
    SetLastSelectUCID(dwLastUCID);

    LogHelper::LogInfo("game.contents",
                       "SendCharacterList lastUCID=%u characterCount=%u",
                       dwLastUCID,
                       static_cast<unsigned int>(characterCount));

    if (characterCount <= 8) {
        for (const STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
            LogHelper::LogDebug(
                "game.contents",
                "SendCharacterList preset ucid=%u class=%u slot=%u appearance=[%u,%u,%u,%u] appearanceEx=[%u,%u,%u,%u] shape=[head=%d,hands=%d,body=%d,stocking=%d,foot=%d,pants=%d] weapon=%d photo=%u",
                characterInfo.uxActorID.dwActorID & 0x1FFFFFFF,
                static_cast<unsigned int>(characterInfo.stBaseInfo.byClass),
                static_cast<unsigned int>(characterInfo.byCharSlotPos),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[0]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[1]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[2]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearance.stAppearance.wAppearanceID[3]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[0]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[1]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[2]),
                static_cast<unsigned int>(characterInfo.stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[3]),
                characterInfo.stShapeEquipItemInfo[1].nItemID,
                characterInfo.stShapeEquipItemInfo[4].nItemID,
                characterInfo.stShapeEquipItemInfo[6].nItemID,
                characterInfo.stShapeEquipItemInfo[8].nItemID,
                characterInfo.stShapeEquipItemInfo[9].nItemID,
                characterInfo.stShapeEquipItemInfo[11].nItemID,
                characterInfo.stSoulWeapon.dwItemID,
                characterInfo.stBaseInfo.dwProfilePhotoID);
        }
    }

    if (characterCount > 8) {
        for (std::size_t index = 0; index < m_stUserInfo.vSTCharInfo.size(); ++index) {
            LogHelper::LogInfo("game.contents",
                               "Character List Check %u %u",
                               static_cast<unsigned int>(index),
                               m_stUserInfo.vSTCharInfo[index].uxActorID.dwActorID);
        }
    }
}

/**
 * @brief 角色列表排序/删除过期时间计算的最小还原。
 * @param nDeleteCharListExpireTime 数据库返回的删除角色过期时间。
 * @return 当前仍返回 false，表示沿用原始流程继续直接发送角色列表。
 *
 * 原始代码这里还会结合当前时间判断该过期值是否仍有效；
 * 当前骨架只保留这一最小语义，不擅自补入未完全确认的复杂排序逻辑。
 */
bool CUser::SortCharacterList(std::int64_t nDeleteCharListExpireTime) {
    SetDeleteCharListExpireTime(nDeleteCharListExpireTime);

    const ATL::CTime currentTime = ATL::CTime::GetTickCount();
    if (currentTime.GetTime() >= nDeleteCharListExpireTime) {
        SetDeleteCharListExpireTime(0);
    }

    return false;
}

/**
 * @brief 记录一次联盟会长检查的角色 UCID。
 * @param ucid 当前参与检查的角色 ID。
 *
 * 根据 IDA 中 `CUser::CheckLeagueMaster` 的函数体，
 * 当前已确认行为仅为把传入的 `UCID` 压入内部容器。
 * 真实的联盟主判定/筛选逻辑尚未在登录链路里恢复出来，
 * 因此这里不额外推导布尔结果，只保留数据落点。
 */
void CUser::CheckLeagueMaster(unsigned int dwUCID) {
    m_dwLeagueMasterUCID.push_back(dwUCID);
}

/**
 * @brief 用角色地图缓存回填选角请求里的上次地图和复活点。
 *
 * 根据 IDA 中 `CUser::UpdateCharacterMapInfo(0x14002f0f0)`，
 * 当前只在 `nMapID == 30031` 的记录命中时，把：
 * - `nPrevMapID`
 * - `nPrevRevivePoint`
 *
 * 写回到 `PS_CHARACTER_SELECT`。
 */
void CUser::UpdateCharacterMapInfo(PS_CHARACTER_SELECT& selectInfo) {
    for (const ST_CHARACTER_MAP_INFO& mapInfo : m_psMapList.vecInfo) {
        if (static_cast<unsigned int>(mapInfo.nUCID) == selectInfo.dwActorID && mapInfo.nMapID == 30031) {
            selectInfo.nPrevMapID = mapInfo.nPrevMapID;
            selectInfo.nPrevRevivePoint = mapInfo.nPrevRevivePoint;
        }
    }
}

/**
 * @brief 判断当前账号是否允许选择指定角色进入游戏。
 *
 * 根据 IDA 中 `CUser::CanEnterGame(0x14002f8b0)`：
 * - 若 `m_nDeleteCharListExpireTime != 0`，只要角色存在于列表中即可
 * - 否则除“角色存在”外，还要求 `byCharSlotPos <= 8`
 */
bool CUser::CanEnterGame(unsigned int dwUCID) const {
    for (const STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        if ((characterInfo.uxActorID.dwActorID & 0x1FFFFFFF) != dwUCID) {
            continue;
        }

        if (m_nDeleteCharListExpireTime != 0) {
            return true;
        }

        return characterInfo.byCharSlotPos <= 8;
    }

    return false;
}

/**
 * @brief 检查目标角色是否已超过删角保护时间。
 *
 * 该实现按 IDA 中 `CheckCreateDate(0x14002efb0)` 的已确认语义保留：
 * - 保护窗口固定为 1 天
 * - 若找不到创建时间记录，则按 1 天剩余时间处理
 * - 若尚未到期，则把剩余秒数写回 `nRemainTime`
 */
bool CUser::CheckCreateDate(unsigned int dwUCID, PS_DELETE_CHARACTER_RES& psRes) const {
    const ATL::CTime currentTime = ATL::CTime::GetTickCount();
    const ATL::CTimeSpan deleteProtectTime(1, 0, 0, 0);
    const std::int64_t deleteProtectSeconds = deleteProtectTime.GetTotalSeconds();

    const auto it = m_mapCreateDate.find(dwUCID);
    if (it == m_mapCreateDate.end()) {
        psRes.nRemainTime = static_cast<int>(deleteProtectSeconds);
        return false;
    }

    const std::int64_t deleteAvailableTime = it->second + deleteProtectSeconds;
    if (currentTime.GetTime() < deleteAvailableTime) {
        psRes.nRemainTime = static_cast<int>(deleteAvailableTime - currentTime.GetTime());
        return false;
    }

    return true;
}

/**
 * @brief 判断指定角色是否被标记为联盟会长。
 *
 * 对应 IDA 中 `CUser::IsLeagueMaster(0x14002EDA0)`:
 * - 使用 CFAutoSlimReadLock(&m_rwLock) 保护遍历
 */
bool CUser::IsLeagueMaster(unsigned int dwUCID) const {
    CFAutoSlimReadLock autolock(&m_rwLock);
    for (unsigned int trackedUCID : m_dwLeagueMasterUCID) {
        if (trackedUCID == dwUCID) {
            return true;
        }
    }
    return false;
}

/**
 * @brief 校验角色换槽请求并回填源/目标 UCID。
 *
 * 根据 IDA 中 `CheckChangeSlot(0x14002f630)`：
 * 1. 先按角色槽位匹配本地角色列表，回填 `nSrcUCID / nDestUCID`
 * 2. 两边都匹配不到时，立即回 `0xC745`
 * 3. 若存在删角状态，则直接允许继续
 * 4. 否则按 `<= 8` 的主槽位限制校验源/目标槽组合
 */
bool CUser::CheckChangeSlot(PS_CHARACTER_CHANGE_SLOT& stSlot) {
    stSlot.nSrcUCID = 0;
    stSlot.nDestUCID = 0;

    for (const STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        if (characterInfo.byCharSlotPos == stSlot.bySrcSlot) {
            stSlot.nSrcUCID = static_cast<int>(characterInfo.uxActorID.dwActorID & 0x1FFFFFFF);
        }
        if (characterInfo.byCharSlotPos == stSlot.byDestSlot) {
            stSlot.nDestUCID = static_cast<int>(characterInfo.uxActorID.dwActorID & 0x1FFFFFFF);
        }
    }

    if (stSlot.nSrcUCID == 0 && stSlot.nDestUCID == 0) {
        SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_CHANGE_SLOT, 0xC745u);
        LogHelper::LogError("game.contents",
                            "<ReqCharacterChangeSlot> nSrcUCID == 0 && nDestUCID == 0 %d",
                            GetUAID());
        return false;
    }

    if (m_nDeleteCharListExpireTime == 0) {
        if (stSlot.bySrcSlot <= 8) {
            if (stSlot.byDestSlot > 8 && stSlot.nSrcUCID > 0) {
                SendErrorMessage(eCMD_CHARACTER,
                                 eSUB_CMD_CHARACTER_CHANGE_SLOT,
                                 0xC745u);
                LogHelper::LogError(
                    "game.contents",
                    "<ReqCharacterChangeSlot> stSlot.bySrcSlot > MAX_CHARACTER_COUNT || stSlot.nSrcUCID > 0 %d",
                    GetUAID());
                return false;
            }
        } else if (stSlot.byDestSlot > 8 || stSlot.nDestUCID > 0) {
            SendErrorMessage(eCMD_CHARACTER, eSUB_CMD_CHARACTER_CHANGE_SLOT, 0xC745u);
            LogHelper::LogError("game.contents",
                                "<ReqCharacterChangeSlot> stSlot.byDestSlot > MAX_CHARACTER_COUNT || stSlot.nDestUCID > 0 %d",
                                GetUAID());
            return false;
        }
    }

    return true;
}

/**
 * @brief 在本地角色列表中应用一次换槽结果。
 */
void CUser::ChangeCharacterSlot(PS_CHARACTER_CHANGE_SLOT& stSlot) {
    for (STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        const unsigned int characterUCID = characterInfo.uxActorID.dwActorID & 0x1FFFFFFF;
        if (stSlot.nSrcUCID == static_cast<int>(characterUCID)) {
            characterInfo.byCharSlotPos = stSlot.byDestSlot;
        }
        if (stSlot.nDestUCID == static_cast<int>(characterUCID)) {
            characterInfo.byCharSlotPos = stSlot.bySrcSlot;
        }
    }
}

void CUser::SetCharacterMapList(PS_CHARACTER_MAP_LIST mapList) {
    m_psMapList = mapList;
}

void CUser::SetWaitCreateCharacterPacketRes(bool bRet) {
    m_bCreateCharacterPacket = bRet;
}

bool CUser::GetWaitCreateCharacterPacketRes() const {
    return m_bCreateCharacterPacket;
}

void CUser::SetTickCreateCharacterPacketRes() {
    m_dw64ChracterCreateTick = GetTickCount64Compat();
}

std::uint64_t CUser::GetTickCreateCharacterPacketRes() const {
    return m_dw64ChracterCreateTick;
}

bool CUser::IsEmptySlot(std::uint8_t bySlot) const {
    if (bySlot > 8) {
        return false;
    }

    for (const STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        if (characterInfo.byCharSlotPos == bySlot) {
            return false;
        }
    }

    return true;
}

/**
 * @brief 校验并补齐代表角色变更请求。
 *
 * 该实现直接跟随 IDA 中 `CUser::CheckRepresentativeChange(0x14002fc60)`：
 * 1. 必须已经通过过代表角色检查
 * 2. 不允许把当前代表角色再次设为代表角色
 * 3. 读取 `TB_COMMON[0x11171]` 作为代表角色切换冷却小时数
 * 4. 仅在 `CheckRankingTime()` 允许时继续
 * 5. 命中角色列表后，回填 `dwUAID / byClass / byLevel / strName / dwProfilePhotoID / nRepresentativeDate`
 */
bool CUser::CheckRepresentativeChange(PS_CHARACTER_REPRESENTATIVE_CHANGE& psChange) const {
    constexpr unsigned int kRepresentativeCooldownTableId = 0x11171;
    constexpr int kRepresentativeCommonError = 59630;
    constexpr int kRepresentativeRankingWindowError = 59634;
    constexpr int kRepresentativeCooldownError = 59636;

    if (!GetRepresentativeCheck()) {
        psChange.nError = kRepresentativeCommonError;
        return false;
    }

    if (GetRepresentativeCheckResult() != 0) {
        psChange.nError = kRepresentativeCommonError;
        return false;
    }

    if (psChange.dwUCID == GetRepresentativeUCID()) {
        psChange.nError = kRepresentativeCommonError;
        return false;
    }

    XResourceMgr& resourceMgr = TXSingleton<XLoginServer>::Instance()->GetResourceMgr();
    TB_COMMON* cooldownRow = resourceMgr.GetTB_COMMON(kRepresentativeCooldownTableId);
    if (!cooldownRow) {
        psChange.nError = kRepresentativeCommonError;
        return false;
    }

    const ATL::CTime currentTime = ATL::CTime::GetTickCount();
    const std::int64_t cooldownSeconds =
        static_cast<std::int64_t>(cooldownRow->Value * 60.0f * 60.0f);
    if (static_cast<std::int64_t>(GetLastRepresentativeCharTime()) + cooldownSeconds >
        currentTime.GetTime()) {
        psChange.nError = kRepresentativeCooldownError;
        return false;
    }

    if (!resourceMgr.CheckRankingTime()) {
        psChange.nError = kRepresentativeRankingWindowError;
        return false;
    }

    CFAutoSlimReadLock autolock(&m_rwLock);
    for (const STMyCharInfoEx& characterInfo : m_stUserInfo.vSTCharInfo) {
        if ((characterInfo.uxActorID.dwActorID & 0x1FFFFFFF) != psChange.dwUCID) {
            continue;
        }

        psChange.dwUAID = static_cast<unsigned int>(GetUAID());
        psChange.byClass = characterInfo.stBaseInfo.byClass;
        psChange.byLevel = characterInfo.byLevel;
        std::wmemset(psChange.strName, 0, std::size(psChange.strName));
#ifdef _WIN32
        wcsncpy_s(psChange.strName,
                  std::size(psChange.strName),
                  characterInfo.stBaseInfo.strName,
                  _TRUNCATE);
#else
        std::wcsncpy(psChange.strName,
                     characterInfo.stBaseInfo.strName,
                     std::size(psChange.strName) - 1);
#endif
        psChange.dwProfilePhotoID = characterInfo.stBaseInfo.dwProfilePhotoID;
        psChange.nRepresentativeDate = currentTime.GetTime();
        psChange.nError = 0;
        return true;
    }

    psChange.nError = kRepresentativeCommonError;
    return false;
}
