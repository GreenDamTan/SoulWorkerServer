#pragma once

#include <cstdint>

// Forward declarations
class CUser;
struct PS_XIGNCODE_UPDATE;

/**
 * @brief IXigncodeServer 接口 - Xigncode SDK 服务器接口
 *
 * 该接口由 zwave_sdk_helper_x64.dll 提供，用于反作弊功能。
 * 方法签名从 IDA 反编译结果推导。
 */
struct IXigncodeServer {
    virtual ~IXigncodeServer() = default;

    // 初始化/关闭
    virtual bool OnBegin(IXigncodeServer* pThis, int nBufferSize) = 0;
    virtual void OnEnd(IXigncodeServer* pThis) = 0;
    virtual void Release(IXigncodeServer* pThis) = 0;

    // 用户连接管理
    virtual void OnAccept(IXigncodeServer* pThis, void* pUser, void* pMeta) = 0;
    virtual void OnDisconnect(IXigncodeServer* pThis, void* pUser) = 0;
    virtual void SetUserInformationA(IXigncodeServer* pThis, void* pUser, unsigned int nAddr, void* pMeta) = 0;

    // 数据接收
    virtual void OnReceive(IXigncodeServer* pThis, void* pUser, const char* pData, unsigned int dwSize) = 0;
};

/**
 * @brief CXigncode - Xigncode 反作弊适配器
 *
 * 对应 IDA 地址:
 * - Init: 0x140001000
 * - Release: 0x140001080
 * - ConnectUser: 0x1400010c0
 * - DisconnectUser: 0x140001150
 * - SendProc: 0x1400011A0
 * - DisconectionProc: 0x140001300
 * - RecvXigncode: 0x140001400
 */
class CXigncode {
public:
    CXigncode() = default;
    ~CXigncode() = default;

    // Initialization and cleanup
    bool Init();
    void Release();

    // User management
    void ConnectUser(int sessionID, std::uint32_t nAddr);
    void DisconnectUser(int sessionID);

    // Xigncode data receive
    void RecvXigncode(int sessionID, const PS_XIGNCODE_UPDATE& stXigncode);

    // Callbacks (static) - SDK 回调函数
    static int __fastcall SendProc(void* uid, void* meta, const char* buf, unsigned int size);
    static void __fastcall DisconectionProc(void* uid, void* meta, int code, const char* report);

    // Xigncode server instance (static, 对应 IDA 地址 0x140D711B0)
    static IXigncodeServer* m_pXigncodeServer;
};
