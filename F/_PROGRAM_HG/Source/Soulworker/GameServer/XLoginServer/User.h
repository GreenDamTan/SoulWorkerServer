#pragma once

#include "Soulworker/GameServer/XLoginServer/stdafx.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <map>
#include <new>
#include <queue>
#include <shared_mutex>
#include <type_traits>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

/**
 * @brief 登录服内部使用的入服状态机。
 *
 * 这些枚举值来自现有 PDB 命名，主要描述玩家从登录、排队到选择世界阶段的推进状态。
 */
enum ENTER_SERVER_STATE {
    ENTER_SERVER_STATE_NONE = 0,
    ENTER_SERVER_STATE_LOGIN_REQ = 1,
    ENTER_SERVER_STATE_LOGIN_RES = 2,
    ENTER_SERVER_STATE_SELECT_WORLD_REQ = 3,
    ENTER_SERVER_STATE_SELECT_WORLD_RES = 4,
    ENTER_SERVER_STATE_SELECT_GAME_REQ = 5,
    ENTER_SERVER_STATE_SELECT_GAME_RES = 6,
};

class CUser;
class XClient;


/**
 * @brief 登录服中的客户端对象。
 *
 * 该类承担两类职责：
 * 1. 保存账号、排队、二级密码、GM 标志等会话状态。
 * 2. 为还原工程提供最小的发包/错误/踢线记录能力，便于验证链路。
 *
 * 依赖关系：
 * - 被 `XLoginProcess` 驱动处理客户端请求。
 * - 被 `XGameDBSocket` 与 `CLoginControlSocket` 回填数据库/控制服返回值。
 * - 被 `XLoginServer` 存入在线表与等待队列。
 */
class CUser : public XClient {
public:
    struct ErrorRecord {
        std::uint8_t packetSubHeader = 0;
        std::uint16_t errorCode = 0;
    };

    struct AuditPacket {
        std::uint8_t mainCmd = 0;
        std::uint8_t subCmd = 0;
        std::size_t fieldCount = 0;
    };

    explicit CUser(std::uint32_t ipv4 = 0x7F000001);

    bool Init() override;
    bool OnLogOut() override;

    /** @brief 获取当前入服阶段。 */
    ENTER_SERVER_STATE GetEnterServerState() const { return m_eEnterServerState; }
    /** @brief 设置当前入服阶段。 */
    void SetEnterServerState(ENTER_SERVER_STATE state) { m_eEnterServerState = state; }

    /** @brief 获取正式 UAID。 */
    int GetUAID() const { return m_stUserInfo.nUAID; }
    /** @brief 设置正式 UAID。 */
    void SetUAID(int uaid) { m_stUserInfo.nUAID = uaid; }

    /** @brief 获取账号认证 SessionID。 */
    std::uint64_t GetAuthSessionID() const { return static_cast<std::uint64_t>(m_biAuthSessionID); }
    /** @brief 设置账号认证 SessionID。 */
    void SetAuthSessionID(std::uint64_t sessionId) {
        m_biAuthSessionID = static_cast<std::int64_t>(sessionId);
    }

    /** @brief 获取客户端 IPv4。 */
    std::uint32_t GetIPv4() const { return scAddr.sin_addr.s_addr; }
    /** @brief 设置客户端 IPv4。 */
    void SetIPv4(std::uint32_t ipv4) { scAddr.sin_addr.s_addr = ipv4; }

    /** @brief 获取发送 SessionID 校验请求的时间戳。 */
    std::uint64_t GetSendCheckSessionID() const { return m_dw64CheckSessionIDTick; }
    /** @brief 按原始无参接口记录当前 SessionID 校验发送时刻。 */
    void SetSendCheckSessionID();
    /** @brief 设置发送 SessionID 校验请求的时间戳。 */
    void SetSendCheckSessionID(std::uint64_t tick) { m_dw64CheckSessionIDTick = tick; }

    /** @brief 获取等待队列票号。 */
    std::int64_t GetTicket_Wait() const { return m_biTicket_Wait; }
    /** @brief 设置等待队列票号。 */
    void SetTicket_Wait(std::int64_t ticket) { m_biTicket_Wait = ticket; }

    /** @brief 获取等待队列中的 UAID。 */
    unsigned int GetUAID_Wait() const { return m_dwUAID_Wait; }
    /** @brief 设置等待队列中的 UAID。 */
    void SetUAID_Wait(unsigned int uaid) { m_dwUAID_Wait = uaid; }

    /** @brief 获取等待队列中的上次服务器索引。 */
    std::uint16_t GetLastServerIndex_Wait() const { return m_wLastServerIndex_Wait; }
    /** @brief 设置等待队列中的上次服务器索引。 */
    void SetLastServerIndex_Wait(std::uint16_t index) { m_wLastServerIndex_Wait = index; }

    /** @brief 获取等待队列中的 SessionID。 */
    std::uint64_t GetAuthSessionID_Wait() const {
        return static_cast<std::uint64_t>(m_biAuthSessionID_Wait);
    }
    /** @brief 设置等待队列中的 SessionID。 */
    void SetAuthSessionID_Wait(std::uint64_t sessionId) {
        m_biAuthSessionID_Wait = static_cast<std::int64_t>(sessionId);
    }

    /** @brief 标记等待队列加入时间。 */
    void SetAddTime_Wait();
    /** @brief 获取等待队列加入时间。 */
    std::int64_t GetAddTime_Wait() const { return m_AddTime_Wait.GetTime(); }

    /** @brief 判断是否已发过等待提示包。 */
    bool IsSendWaitPacket() const { return m_bSendWaitPacket; }
    /** @brief 设置是否已发过等待提示包。 */
    void SetSendWaitPacket(bool enabled) { m_bSendWaitPacket = enabled; }

    /** @brief 判断是否请求取消排队。 */
    bool IsCancel_Wait() const { return m_bCancel_Wait; }
    /** @brief 设置是否取消排队。 */
    void SetCancel_Wait(bool cancel) { m_bCancel_Wait = cancel; }

    /**
     * @brief 设置当前二级密码状态值。
     * @param state PDB 中定义的 `unsigned char` 状态值；常见值为 0/1/2。
     */
    void SetSecondPWState(std::uint8_t state) { m_bySecondPassword = state; }
    /**
     * @brief 获取当前二级密码状态值。
     * @return 返回当前缓存的二级密码状态。
     */
    std::uint8_t GetSecondPWState() const { return m_bySecondPassword; }
    /**
     * @brief 判断当前账号是否已进入“需要继续二级密码流程”的状态。
     * @return 当状态值等于 2 时返回 true。
     */
    bool CheckSecondPasswordState() const { return m_bySecondPassword == 2; }
    /**
     * @brief 设置交易密码状态值。
     * @param state PDB 中定义的 `unsigned char` 状态值。
     */
    void SetTradePWState(std::uint8_t state) { m_byTradePassword = state; }
    /**
     * @brief 获取交易密码状态值。
     * @return 当前缓存的交易密码状态。
     */
    std::uint8_t GetTradePWState() const { return m_byTradePassword; }
    /** @brief 设置封禁/限制类型。 */
    void SetBlockType(std::uint8_t blocked) { m_byBlockType = blocked; }
    /** @brief 设置账号字符串。 */
    void SetAccountID(const std::string& accountId) {
        std::memset(m_szAccountID, 0, sizeof(m_szAccountID));
        const std::size_t copyLength =
            std::min<std::size_t>(accountId.size(), sizeof(m_szAccountID) - 1);
        std::memcpy(m_szAccountID, accountId.data(), copyLength);
    }
    /** @brief 设置 GM 标志。 */
    void SetGM(std::uint8_t isGM) { m_byGM = isGM; }

    /**
     * @brief 向当前账号的角色列表中追加一名角色。
     * @param characterInfo 单个角色的扩展显示信息。
     */
    void AddCharacterInfo(const STMyCharInfoEx& characterInfo);
    /**
     * @brief 获取当前账号缓存的角色列表。
     * @return 角色列表容器引用。
     */
    const std::vector<STMyCharInfoEx>& GetCharacterInfos() const { return m_stUserInfo.vSTCharInfo; }
    /**
     * @brief 根据 `UCID` 查找单个角色信息。
     * @param nUCID 目标角色 ID。
     * @return 找到时返回对应 `STCharInfo` 基类指针，否则返回空。
     */
    STCharInfo* GetCharacterInfo(int nUCID);
    /**
     * @brief 根据 `UCID` 删除本地缓存中的单个角色。
     * @param nUCID 目标角色 ID。
     * @return 删除成功返回 true，否则返回 false。
     */
    bool DeleteCharacterInfo(int nUCID);
    /**
     * @brief 清空当前账号缓存的角色列表。
     */
    void ClearCharacterInfos() { ClearCharacterInfo(); }
    /**
     * @brief 清空账号角色信息，并把角色数量重置为 0。
     *
     * 对应 PDB/IDA 中的 `ClearCharacterInfo`，当前已改回
     * `CFAutoSlimWriteLock(&m_rwLock)` 包裹下的清空流程。
     */
    void ClearCharacterInfo();
    /**
     * @brief 下发当前服务器内容开关。
     *
     * 依赖关系：
     * - 从 `XLoginServer::GetResourceMgr()` 读取 `PS_CONTENTS_INFO`
     * - 最终向客户端发送 `main=1, sub=7`
     */
    void SendServerOption();
    /**
     * @brief 向客户端下发角色列表。
     * @param dwLastUCID 上次选择的角色 UCID。
     */
    void SendCharacterList(unsigned int dwLastUCID);
    /** @brief 设置账号当前角色数量。 */
    void SetCharacterCount(std::uint16_t wCharacterCount) { m_wCharacterCount = wCharacterCount; }
    /** @brief 获取账号当前角色数量。 */
    std::uint16_t GetCharacterCount() const { return m_wCharacterCount; }
    /**
     * @brief 保存角色地图映射列表。
     * @param mapList 数据库返回的角色地图映射列表。
     */
    void SetCharacterMapList(PS_CHARACTER_MAP_LIST mapList);
    /** @brief 获取角色地图映射列表。 */
    const PS_CHARACTER_MAP_LIST& GetCharacterMapList() const { return m_psMapList; }
    /** @brief 设置角色删除相关的过期时间。 */
    void SetDeleteCharListExpireTime(std::int64_t expireTime) { m_nDeleteCharListExpireTime = expireTime; }
    /** @brief 获取角色删除相关的过期时间。 */
    std::int64_t GetDeleteCharListExpireTime() const { return m_nDeleteCharListExpireTime; }
    /** @brief 设置代表角色 UCID。 */
    void SetRepresentativeUCID(unsigned int dwRepresentativeUCID) {
        m_dwRepresentativeUCID = dwRepresentativeUCID;
    }
    /** @brief 获取代表角色 UCID。 */
    unsigned int GetRepresentativeUCID() const { return m_dwRepresentativeUCID; }
    /** @brief 设置最近一次选择的角色 UCID。 */
    void SetLastSelectUCID(unsigned int dwLastUCID);
    /** @brief 获取最近一次选择的角色 UCID。 */
    unsigned int GetLastSelectUCID() const;
    /** @brief 设置最近一次代表角色时间戳。 */
    void SetLastRepresentativeCharTime(std::uint64_t value) {
        m_nLastRepresentativeCharTime = static_cast<std::int64_t>(value);
    }
    /** @brief 获取最近一次代表角色时间戳。 */
    std::uint64_t GetLastRepresentativeCharTime() const {
        return static_cast<std::uint64_t>(m_nLastRepresentativeCharTime);
    }
    /** @brief 设置当前选中的角色 UCID。 */
    void SetSelectUCID(unsigned int nUCID) { m_dwSelectedUCID = nUCID; }
    /** @brief 获取当前选中的角色 UCID。 */
    unsigned int GetSelectUCID() const { return m_dwSelectedUCID; }
    /** @brief 记录最近一次待处理的选角请求 UCID。 */
    void SetPendingSelectUCID(unsigned int ucid) { m_dwPendingSelectUCID = ucid; }
    /** @brief 获取最近一次待处理的选角请求 UCID。 */
    unsigned int GetPendingSelectUCID() const { return m_dwPendingSelectUCID; }
    /** @brief 获取封禁/限制类型。 */
    std::uint8_t GetBlockType() const { return m_byBlockType; }
    /**
     * @brief 用角色地图缓存修正选角请求中的上次地图/复活点。
     * @param selectInfo 待修正的角色选择请求结构。
     */
    void UpdateCharacterMapInfo(PS_CHARACTER_SELECT& selectInfo);
    /**
     * @brief 判断当前账号是否允许进入指定角色。
     *
     * 当前只保留 IDA 中已确认的最小语义：
     * - 若存在删除角色过期时间，仅检查目标角色是否在列表中
     * - 否则还要求 `byCharSlotPos <= 8`
     */
    bool CanEnterGame(unsigned int dwUCID) const;
    /** @brief 记录“等待代表角色回包”状态。 */
    void SetWaitRepresentativePacketRes(bool bRet) { m_bRepresentativePacket = bRet; }
    /** @brief 获取“等待代表角色回包”状态。 */
    bool GetWaitRepresentativePacketRes() const { return m_bRepresentativePacket; }
    /** @brief 获取“等待代表角色回包”状态。 */
    bool IsWaitRepresentativePacketRes() const { return m_bRepresentativePacket; }
    /** @brief 设置代表角色校验是否通过。 */
    void SetRepresentativeCheck(bool bCheck) { m_bRepresentativeCheck = bCheck; }
    /** @brief 获取代表角色校验状态。 */
    bool GetRepresentativeCheck() const { return m_bRepresentativeCheck; }
    /** @brief 设置代表角色校验结果码。 */
    void SetRepresentativeCheckResult(int nResult) { m_nRepresentativeResult = nResult; }
    /** @brief 获取代表角色校验结果码。 */
    int GetRepresentativeCheckResult() const { return m_nRepresentativeResult; }
    /** @brief 设置“等待删角回包”状态。 */
    void SetWaitDeleteCharacterPacketRes(bool bRet) { m_bCreateDeletePacket = bRet; }
    /** @brief 获取“等待删角回包”状态。 */
    bool GetWaitDeleteCharacterPacketRes() const { return m_bCreateDeletePacket; }
    /** @brief 设置“等待建角回包”状态。 */
    void SetWaitCreateCharacterPacketRes(bool bRet);
    /** @brief 获取“等待建角回包”状态。 */
    bool GetWaitCreateCharacterPacketRes() const;
    /** @brief 记录建角请求发送时间戳。 */
    void SetTickCreateCharacterPacketRes();
    /** @brief 获取建角请求发送时间戳。 */
    std::uint64_t GetTickCreateCharacterPacketRes() const;
    /** @brief 判断某个主角色槽是否为空。 */
    bool IsEmptySlot(std::uint8_t bySlot) const;
    /** @brief 设置“等待换槽回包”状态。 */
    void SetWaitChangeSlotPacketRes(bool bRet) { m_bChangeSlotPacket = bRet; }
    /** @brief 获取“等待换槽回包”状态。 */
    bool GetWaitChangeSlotPacketRes() const { return m_bChangeSlotPacket; }
    /**
     * @brief 校验代表角色变更请求是否满足当前账号状态与时间窗口。
     * @param psChange 待补全的代表角色变更结构。
     * @return 允许发往 GameDB 返回 true；否则写入 `nError` 并返回 false。
     */
    bool CheckRepresentativeChange(PS_CHARACTER_REPRESENTATIVE_CHANGE& psChange) const;
    /**
     * @brief 检查目标角色是否已超过删角保护时间。
     * @param dwUCID 目标角色 ID。
     * @param psRes 失败时写回剩余等待秒数。
     */
    bool CheckCreateDate(unsigned int dwUCID, PS_DELETE_CHARACTER_RES& psRes) const;
    /** @brief 判断目标角色是否被标记为联盟会长。 */
    bool IsLeagueMaster(unsigned int dwUCID) const;
    /**
     * @brief 校验换槽请求并在必要时回写源/目标 UCID。
     * @param stSlot 客户端换槽包。
     */
    bool CheckChangeSlot(PS_CHARACTER_CHANGE_SLOT& stSlot);
    /** @brief 在本地角色列表中应用一次换槽结果。 */
    void ChangeCharacterSlot(PS_CHARACTER_CHANGE_SLOT& stSlot);

    /**
     * @brief 缓存 Soul Gauge / 第三方认证信息。
     * @param info 来自认证链路的认证信息结构。
     */
    void SetSGAuthInfo(const ST_SG_AUTH_INFO& info) { m_stSGAuthInfo = info; }
    /** @brief 缓存 GF 认证信息。 */
    void SetGFAuthInfo(const ST_GF_AUTH_INFO& info) { m_stGFAuthInfo = info; }
    /**
     * @brief 获取当前认证类型。
     * @return `m_stSGAuthInfo.byAuthType` 的当前值。
     */
    std::uint8_t GetSGAuthType() const { return m_stSGAuthInfo.byAuthType; }

    /** @brief 设置段位等级缓存。 */
    void SetEchelonLevel(int value) { m_byEchelonLevel = static_cast<std::uint8_t>(value); }
    /** @brief 获取段位等级缓存。 */
    std::uint8_t GetEchelonLevel() const { return m_byEchelonLevel; }
    /** @brief 设置段位经验缓存。 */
    void SetEchelonExp(int value) { m_nEchelonExp = value; }
    /** @brief 获取段位经验缓存。 */
    int GetEchelonExp() const { return m_nEchelonExp; }
    /** @brief 清空联盟信息缓存。 */
    void ClearLeagueInfo() { m_dwLeagueMasterUCID.clear(); }
    /** @brief 保留角色创建日期索引，供后续角色删除/排序链路使用。 */
    void SetCreateDate(unsigned int dwUCID, std::int64_t biCreateDate);
    /** @brief 角色列表排序/过期时间计算的最小还原。 */
    bool SortCharacterList(std::int64_t nDeleteCharListExpireTime);
    /**
     * @brief 记录一次联盟会长检查输入。
     *
     * 当前 IDA 仅能确认该函数把 `UCID` 压入内部容器；
     * 更深层的联盟判断逻辑仍待后续结合上游调用继续还原。
     */
    void CheckLeagueMaster(unsigned int dwUCID);

    /**
     * @brief 返回断线时是否需要真正从在线表删除当前账号。
     *
     * 对应 PDB 中的 `m_bDeleteUserInfo`。当新连接顶掉旧连接时，
     * `EnterUser` 会先把旧连接改成 `false`，避免旧连接 `OnLogOut`
     * 误把新连接从在线表移除。
     */
    bool GetDeleteUserInfo() const;

    /** @brief 设置断线时是否删除在线表条目。 */
    void SetDeleteUserInfo(bool bRet);

    /**
     * @brief 记录发送给客户端的错误码。
     * @param packetSubHeader 登录协议子包号。
     * @param errorCode 服务器返回的错误码。
     */
    void PushError(std::uint8_t packetSubHeader, std::uint16_t errorCode) {
        errors_.push_back({packetSubHeader, errorCode});
    }

    /**
     * @brief 记录一次发送给客户端的包。
     * @param packet 已构造好的发送包。
     */
    void GreenDamTan_RecordSentPacket(const XPacket& packet) {
        sentPackets_.push_back({packet.GetMainCmd(), packet.GetSubCmd(), packet.XParse.Size()});
    }

    /**
     * @brief 按原版语义桥接客户端发送。
     *
     * 当前仍保留审计记录，便于在没有真实套接字层时验证还原结果。
     */
    bool BridgeSend(XSendPacket& packet);

    /**
     * @brief 记录一次踢线操作。
     * @param kickInfo 踢线原因与参数。
     */
    void Kickout(const PS_KICK_USER_INFO& kickInfo);

    /** @brief 设置位状态。 */
    void SetState(CLIENT_STATE state) { XClient::SetState(state); }
    /** @brief 清除位状态。 */
    void ClearState(CLIENT_STATE state) { XClient::ClearState(state); }
    /** @brief 判断是否拥有某个状态位。 */
    bool IsState(CLIENT_STATE state) const { return XClient::IsState(state); }

protected:
    bool RegisterProcess();

private:
    mutable CFSRWLock m_rwLock{};
    STUserInfo m_stUserInfo{};
    std::uint16_t m_wCharacterCount = 0;
    unsigned int m_dwSelectedUCID = 0;
    ENTER_SERVER_STATE m_eEnterServerState = ENTER_SERVER_STATE_NONE;
    std::uint8_t m_byEchelonLevel = 0;
    int m_nEchelonExp = 0;
    std::uint8_t m_bySecondPassword = 0;
    std::uint8_t m_byTradePassword = 0;
    std::vector<unsigned int> m_dwLeagueMasterUCID;
    std::int64_t m_biAuthSessionID = 0;
    unsigned int m_dwUAID_Wait = 0;
    std::int64_t m_biTicket_Wait = 0;
    std::uint16_t m_wLastServerIndex_Wait = 0;
    std::int64_t m_biAuthSessionID_Wait = 0;
    ATL::CTime m_AddTime_Wait{};
    bool m_bCancel_Wait = false;
    bool m_bSendWaitPacket = false;
    bool m_bDeleteUserInfo = true;
    std::uint8_t m_byBlockType = 0;
    std::uint8_t m_byGM = 0;
    std::map<unsigned int, std::int64_t> m_mapCreateDate;
    char m_szAccountID[21] = {};
    bool m_bCreateCharacterPacket = false;
    bool m_bCreateDeletePacket = false;
    std::uint64_t m_dw64ChracterCreateTick = 0;
    std::uint64_t m_dw64CheckSessionIDTick = 0;
    bool m_bChangeSlotPacket = false;
    ST_SG_AUTH_INFO m_stSGAuthInfo{};
    int m_nSGAuthProcessState = 0;
    PS_CHARACTER_MAP_LIST m_psMapList{};
    std::int64_t m_nDeleteCharListExpireTime = 0;
    ST_GF_AUTH_INFO m_stGFAuthInfo{};
    unsigned int m_dwRepresentativeUCID = 0;
    std::int64_t m_nLastRepresentativeCharTime = 0;
    bool m_bRepresentativeCheck = false;
    int m_nRepresentativeResult = 0;
    bool m_bRepresentativePacket = false;
    unsigned int m_dwLastSelectUCID = 0;
    unsigned int m_dwPendingSelectUCID = 0;
    bool kicked_ = false;
    std::vector<ErrorRecord> errors_;
    std::vector<AuditPacket> sentPackets_;
    std::vector<PS_KICK_USER_INFO> kickHistory_;
};

