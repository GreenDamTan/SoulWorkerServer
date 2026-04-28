// TXMultiPoolServer.h
// 多池服务器模板类定义

#pragma once

#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"

// ============================================================================
// XMultiPoolServer - 多池服务器基类
// ============================================================================
class XMultiPoolServer : public XServer
{
public:
    XMultiPoolServer() {}
    virtual ~XMultiPoolServer() {}

    // 接收线程
    virtual std::uint32_t AcceptThread();
    // 后台线程
    virtual std::uint32_t BackendThreadProc();
};

// ============================================================================
// TXMultiPoolServer - 多池服务器模板类
// 使用 IXObject.h 中已定义的 TXObjectMgr
// ============================================================================
template<typename T>
class TXMultiPoolServer : public XMultiPoolServer
{
public:
    TXMultiPoolServer()
        : XMultiPoolServer()
    {
        m_pIObjectMgr = &m_xObjectMgr;
    }

    virtual ~TXMultiPoolServer()
    {
    }

    // 查找用户
    T* FindUser(int nSessionID)
    {
        return static_cast<T*>(m_xObjectMgr.Find(nSessionID));
    }

protected:
    TXObjectMgr<T> m_xObjectMgr;
};