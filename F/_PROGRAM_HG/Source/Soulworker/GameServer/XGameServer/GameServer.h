#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <cstdint>
#include <string>
#include <map>

// 前置声明
class CUser;
class CDailyMissionMgr;
class CTimeEventMgr;
class CDayEventMgr;
class CWorldEventMgr;
class CRankingMgr;
class CGameCurlWrapper;
class CFSRWLock;
class CCommunitySocket;
class CGameControlSocket;
class CObserveSocket;
class XGameDBSocketMgr;
class XItemFactory;
class XActionResMgr;
class XAkashicResMgr;
class XWorldResMgr;
class CChattingCommandMgr;
class CXigncode;
struct ST_LOG_GAME;
struct ST_CHAT_LOG_GAME;
struct ST_STAT_LOG_GAME;
struct ST_LOG_TEXT;
struct ST_LOG_SYSTEM;
struct TB_SHOP;
struct STCashItem;

// boost multi_index 前置声明（简化）
namespace boost { namespace multi_index {
    template<typename T, typename... Args> class multi_index_container;
}}

// TODO: 推测结果 - 需要IDA验证成员布局和类型
// IDA 0x140001450 显示 XGameServer 实例大小为 0x42438 (271416 字节)
// 构造函数使用 VBaseObject::operator new 分配内存（虚基类支持）
// NOTE: TXServer<CUser> 已继承自 XServer，避免菱形继承
class XGameServer : public TXServer<CUser> {
public:
    // 构造/析构
    XGameServer();
    virtual ~XGameServer();

    // 单例访问
    static XGameServer* Instance();

    // 核心生命周期
    bool InitServer() override;
    bool Clear() override;
    void OnUpdate(std::uint64_t dwTick) override;
    int SetConsoleHandler(int add) override;

    // 用户管理
    void EnterUser(CUser* pUser);
    void ExitUser(CUser* pUser);
    CUser* FindNameToUser(wchar_t* pName);
    CUser* FindActorIDToUser(UXActorID uxActorID);
    CUser* FindUIDToUser(std::uint32_t dwUID);

    // 网络
    bool OnAccect(XClient* pClient) override;

    // 日志
    void WriteLog(char* szFormat, ...);
    bool SendDBLog(ST_LOG_GAME& stLog);
    bool SendDBChatLog(ST_CHAT_LOG_GAME& stLog);
    bool SendDBStatLog(ST_STAT_LOG_GAME& stLog);
    bool SendDBTextLog(ST_LOG_TEXT& stLog);
    bool SendDBSystemLog(ST_LOG_SYSTEM& stLog);

    // 其他
    int nRand(int nMin, int nMax);
    float fRand(float fMin, float fMax);
    bool Shutdown(std::uint32_t dwTick);

private:
    // =====================================================
    // 成员变量 - 从 IDA GameServer.exe 反编译验证 (结构体大小: 0x42438 = 271416 字节)
    // =====================================================

    // [0x0000 - 0xB4B7] 基类 TXServer<CUser> (46264 字节)
    // TXServer<CUser> 继承自 XServer

    // [0xB4B8] gap (1 字节填充)
    std::uint8_t m_paddingB4B8;

    // [0xB4B9 - 0xB4BF] TXSingleton<XGameServer> 基类 (7 字节 + 填充)
    // 继承自 TXSingleton

    // [0xB4C0 - 0xB567] XSeed 成员 (168 字节)
    XSeed m_xSeed;                                      // offset: 46272 (0xB4C0)

    // [0xB568] SG 保活标志
    bool m_bSGKeepAlive;                                // offset: 46440 (0xB568)

    // [0xB570 - 0xB5B7] 数据库代理管理器 (72 字节)
    XGameDBSocketMgr* m_xDBAgentMgr;                    // offset: 46448 (0xB570) - 注意: IDA显示类型为XGameDBSocketMgr而非指针

    // [0xB5B8 - 0xF51F] 资源管理器 (16216 字节)
    XResourceMgr m_xResourceMgr;                        // offset: 46520 (0xB5B8)

    // [0xF520 - 0xF53F] 世界资源管理器 (32 字节)
    XWorldResMgr m_xWorldResMgr;                        // offset: 62736 (0xF520)

    // [0xF540 - 0xF64F] 物品工厂 (272 字节)
    XItemFactory m_xItemFactory;                        // offset: 62768 (0xF540)

    // [0xF650 - 0x1FFCF] 社区套接字 (Relay连接) (68352 字节)
    CCommunitySocket m_communitySocket;                 // offset: 63040 (0xF650)

    // [0x1FFD0 - 0x30AD7] 控制套接字 (Control连接) (68344 字节)
    CGameControlSocket m_controlSocket;                 // offset: 131392 (0x20170)

    // [0x30AD8 - 0x41B57] 观察套接字 (Observe连接) (69720 字节)
    CObserveSocket m_scObserveSocket;                   // offset: 199736 (0x30AD8)

    // [0x41B58 - 0x41B6F] 计时器变量
    std::uint64_t m_dwControlConnectTick;               // offset: 269456 (0x41B58)
    std::uint64_t m_dwUpdateServerInfoTick;             // offset: 269464 (0x41B60)
    std::uint64_t m_dwCommunityConnectTick;             // offset: 269472 (0x41B68)

    // [0x41B70 - 0x41D17] 动作资源管理器 (424 字节)
    XActionResMgr m_xActionManager;                     // offset: 269480 (0x41B70)

    // [0x41D18 - 0x41E07] Akashic资源管理器 (240 字节)
    XAkashicResMgr m_xAkashicManager;                   // offset: 269904 (0x41D18)

    // [0x41E08 - 0x41E27] 日常任务管理器 (32 字节)
    CDailyMissionMgr m_DailyMissionMgr;                 // offset: 270144 (0x41E08)

    // [0x41E28 - 0x41E47] 聊天命令管理器 (32 字节)
    CChattingCommandMgr m_ChattingCommandMgr;           // offset: 270176 (0x41E20)

    // [0x41E48 - 0x41E67] 系统公告表 (32 字节)
    std::map<std::uint16_t, std::map<std::uint16_t, std::uint8_t>> m_mapSystemPostTalbe;  // offset: 270208 (0x41E40)

    // [0x41E68 - 0x41F47] 用户信息容器 (224 字节)
    // boost::multi_index::multi_index_container<CUser*, ...>
    // 简化为指针，实际类型需要 boost multi_index 支持
    void* m_UserInfos;                                  // offset: 270240 (0x41E60)

    // [0x41F48 - 0x41F4F] Xigncode反作弊 (8 字节)
    CXigncode m_xignCode;                               // offset: 270464 (0x41F40)

    // [0x41F50 - 0x420FF] 时间事件管理器 (432 字节)
    CTimeEventMgr m_TimeEventMgr;                       // offset: 270472 (0x41F48)

    // [0x42100 - 0x4212F] 天事件管理器 (48 字节)
    CDayEventMgr m_DayEventMgr;                         // offset: 270904 (0x42108)

    // [0x42130 - 0x4215F] 世界事件管理器 (48 字节)
    CWorldEventMgr m_WorldEventMgr;                     // offset: 270952 (0x42130)

    // [0x42160 - 0x42207] 排名管理器 (168 字节)
    CRankingMgr m_RankingMgr;                           // offset: 271000 (0x42168)

    // [0x42208 - 0x4220F] Curl包装器 (8 字节)
    CGameCurlWrapper* m_curlWrapper;                    // offset: 271168 (0x42220)

    // [0x42210 - 0x42217] 读写锁 - 地图锁 (8 字节)
    CFSRWLock m_rwMapLock;                              // offset: 271176 (0x42228)

    // [0x42218 - 0x4221B] 状态标志
    bool m_bClose;                                      // offset: 271184 (0x42230)
    bool m_bAcceptClose;                                // offset: 271185 (0x42231)
    // padding: 2 bytes

    // [0x4221C - 0x4221F] 预留用户数
    std::int32_t m_nReserveUser;                        // offset: 271188 (0x42234)

    // [0x42220 - 0x42227] Vision事件句柄
    void* m_hVisionEvent;                               // offset: 271192 (0x42238)

    // [0x42228 - 0x4222B] 房间索引
    std::int32_t m_nRoomIndex;                          // offset: 271200 (0x42240)

    // [0x42230 - 0x4223F] 读写锁
    CFSRWLock m_rwLock;                                 // offset: 271208 (0x42248)
    CFSRWLock m_rwCinderellaLock;                       // offset: 271216 (0x42250)

    // [0x42240 - 0x4224F] 性能检测
    std::uint32_t m_dwCheckPerformance;                 // offset: 271224 (0x42258)
    std::uint64_t m_dw64PerformanceTick;                // offset: 271232 (0x42260)
    bool m_bPerformanceState;                           // offset: 271240 (0x42268)
    bool m_bNeedHavokInit;                              // offset: 271241 (0x42269)
    // padding: 6 bytes

    // [0x42250 - 0x42277] 日志相关
    std::map<std::uint16_t, std::int32_t> m_mapLog;     // offset: 271248 (0x42270)
    std::uint32_t m_dwWriteTime;                        // offset: 271280 (0x42290)
    // padding: 4 bytes

    // [0x42288 - 0x4229F] 货币供应
    std::int64_t m_biMoneySupply;                       // offset: 271288 (0x42298)
    std::uint64_t m_dw64MoneyTick;                      // offset: 271296 (0x422A0)

    // [0x422A8 - 0x422CF] 商店信息
    std::map<std::uint32_t, std::map<std::uint32_t, TB_SHOP>*> m_mapShopInfo;  // offset: 271304 (0x422A8)

    // [0x422D0 - 0x422DF] 初始化日期
    std::int64_t m_biInitDateBefore;                    // offset: 271336 (0x422D0)
    std::int64_t m_biInitDateAfter;                     // offset: 271344 (0x422D8)
    std::uint64_t m_dw64WaitTick;                       // offset: 271352 (0x422E0)

    // [0x422E8 - 0x4230F] 商城相关
    std::map<std::uint32_t, STCashItem> m_mapCashshopList;  // offset: 271360 (0x422E8)
    std::uint64_t m_dw64CashshopTick;                   // offset: 271392 (0x42308)

    // [0x42310 - 0x42317] 商城锁
    CFSRWLock m_rwCashshopLock;                         // offset: 271400 (0x42318)

    // [0x42318] 重置用户连接信息标志
    bool m_bResetUserConnectInfo;                       // offset: 271408 (0x42320)
    // padding: 7 bytes to reach total size 0x42438
};
