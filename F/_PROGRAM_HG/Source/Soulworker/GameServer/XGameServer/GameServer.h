#pragma once

#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
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
class XResourceMgr;
class XItemFactory;
class XActionResMgr;
class XAkashicResMgr;

// TODO: 推测结果 - 需要IDA验证成员布局和类型
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

    // 用户管理
    void EnterUser(CUser* pUser);
    void ExitUser(CUser* pUser);
    CUser* FindNameToUser(wchar_t* pName);
    CUser* FindActorIDToUser(TUXActorID uxActorID);
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
    // 成员变量 - 布局需要IDA验证
    XSeed m_xSeed;
    XGameDBSocketMgr* m_xDBAgentMgr;
    XResourceMgr m_xResourceMgr;
    // ... 大量其他成员需要从IDA构造函数验证
};

// 确认单例模板特化存在
template<>
class TXSingleton<XGameServer>;
