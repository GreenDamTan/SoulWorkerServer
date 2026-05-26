// CXigncode 实现 - Xigncode 反作弊适配器
// 从 GameServer.exe IDA 反编译还原

#include "Soulworker/GameServer/XGameServer/Xigncode.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <windows.h>
#include <cstring>
#include <algorithm>

// Static member initialization
IXigncodeServer* CXigncode::m_pXigncodeServer = nullptr;

/**
 * @brief 加载 Xigncode SDK DLL 并获取创建函数
 *
 * 对应 IDA 地址: 0x1400016F0
 * 该函数加载 zwave_sdk_helper_x64.dll 并返回 ICreateXigncodeServerA 函数指针
 */
typedef int (__fastcall *CXProc_t)(IXigncodeServer**, int (__fastcall*)(void*, void*, const char*, unsigned int), void (__fastcall*)(void*, void*, int, const char*));

static CXProc_t GreenDamTan_LoadHelperDllA(const char* pszPath) {
    char szT[272];
    char Base[272];
    HMODULE h;

    if (pszPath) {
        strcpy_s(szT, sizeof(szT), pszPath);
    } else {
        // 获取当前模块路径
        GetModuleFileNameA(nullptr, szT, sizeof(szT));
        char* _p = strrchr(szT, '\\');
        if (_p) {
            *_p = 0;
        }
        strcat_s(szT, sizeof(szT), "\\xigncode");
    }

    strcpy_s(Base, sizeof(Base), szT);

    if (szT[strlen(szT) - 1] != '\\') {
        strcat_s(szT, sizeof(szT), "\\");
    }
    strcat_s(szT, sizeof(szT), "zwave_sdk_helper_x64.dll");

    h = LoadLibraryA(szT);
    if (!h) {
        return nullptr;
    }

    // 调用 PushPathA 设置路径（如果存在）
    auto fnPPA = reinterpret_cast<void (__fastcall *)(const char*)>(GetProcAddress(h, "PushPathA"));
    if (fnPPA) {
        fnPPA(Base);
    }

    // 获取创建函数
    auto f = reinterpret_cast<CXProc_t>(GetProcAddress(h, "ICreateXigncodeServerA"));
    return f ? f : nullptr;
}

/**
 * @brief 初始化 Xigncode SDK
 *
 * IDA 反编译 (0x140001000):
 * - 调用 LoadHelperDllA(nullptr) 获取创建函数
 * - 调用创建函数初始化 m_pXigncodeServer，传入回调函数
 * - 调用 OnBegin(512) 启动服务
 */
bool CXigncode::Init() {
    CXProc_t CXProc = GreenDamTan_LoadHelperDllA(nullptr);
    if (!CXProc) {
        return false;
    }

    if (!CXProc(&m_pXigncodeServer,
                reinterpret_cast<int (__fastcall*)(void*, void*, const char*, unsigned int)>(SendProc),
                DisconectionProc)) {
        return false;
    }

    if (m_pXigncodeServer->OnBegin(m_pXigncodeServer, 512)) {
        return true;
    }

    GetLastError();
    return false;
}

/**
 * @brief 释放 Xigncode SDK
 *
 * IDA 反编译 (0x140001080):
 * - 如果 m_pXigncodeServer 存在，调用 OnEnd 和 Release
 */
void CXigncode::Release() {
    if (m_pXigncodeServer) {
        m_pXigncodeServer->OnEnd(m_pXigncodeServer);
        m_pXigncodeServer->Release(m_pXigncodeServer);
    }
}

/**
 * @brief 用户连接时通知 Xigncode
 *
 * IDA 反编译 (0x1400010c0):
 * - 调用 OnAccept 和 SetUserInformationA
 */
void CXigncode::ConnectUser(int sessionID, std::uint32_t nAddr) {
    m_pXigncodeServer->OnAccept(m_pXigncodeServer, reinterpret_cast<void*>(sessionID), nullptr);
    m_pXigncodeServer->SetUserInformationA(m_pXigncodeServer, reinterpret_cast<void*>(sessionID), nAddr, nullptr);
    LogHelper::LogDebug("game.system", "<XIGNCODE> Connect User ( %d ) ", sessionID);
}

/**
 * @brief 用户断开时通知 Xigncode
 *
 * IDA 反编译 (0x140001150):
 * - 调用 OnDisconnect
 */
void CXigncode::DisconnectUser(int sessionID) {
    m_pXigncodeServer->OnDisconnect(m_pXigncodeServer, reinterpret_cast<void*>(sessionID));
    LogHelper::LogDebug("game.system", "<XIGNCODE> Disconnect User ( %d ) ", sessionID);
}

/**
 * @brief SDK 发送回调 - 向客户端发送 Xigncode 数据
 *
 * IDA 反编译 (0x1400011A0):
 * - 查找用户，如果存在且状态正确，构造 PS_XIGNCODE_UPDATE 包发送
 * - 主命令: 1, 子命令: 3
 */
int __fastcall CXigncode::SendProc(void* uid, void* meta, const char* buf, unsigned int size) {
    (void)meta;

    int session = static_cast<int>(reinterpret_cast<std::intptr_t>(uid));
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer->FindUIDToUser(static_cast<std::uint32_t>(session));

    if (pUser) {
        if (pUser->IsState(XClient::eStateConnect)) {
            PS_XIGNCODE_UPDATE stXigncode;
            stXigncode.session = session;
            std::memcpy(stXigncode.szBuff, buf, std::min(size, static_cast<unsigned int>(sizeof(stXigncode.szBuff))));

            XSendPacket xSendPacket(1, 3);
            xSendPacket << stXigncode;
            pUser->SendEx(xSendPacket);
        }
        LogHelper::LogDebug("game.system", "<XIGNCODE> Call SendProc %d / %d ", session, size);
    }
    return 1;
}

/**
 * @brief SDK 断开回调 - 用户被 Xigncode 踢出
 *
 * IDA 反编译 (0x140001300):
 * - 调用 OnDisconnect
 * - 查找用户，如果存在且状态正确，踢出用户
 * - 踢出类型: 5 (Xigncode 踢出)
 */
void __fastcall CXigncode::DisconectionProc(void* uid, void* meta, int code, const char* report) {
    (void)meta;
    (void)code;
    (void)report;

    int uida = static_cast<int>(reinterpret_cast<std::intptr_t>(uid));

    m_pXigncodeServer->OnDisconnect(m_pXigncodeServer, uid);

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer->FindUIDToUser(static_cast<std::uint32_t>(uida));

    if (pUser && pUser->IsState(XClient::eStateConnect)) {
        PS_KICK_USER_INFO psKick{};
        // psKick.dwUAID = pUser->GetUAID();  // TODO: 需要实现 GetUAID
        psKick.byKickType = 5;  // Xigncode kick type
        // pUser->Kickout(&psKick, 0);  // TODO: 需要实现 Kickout
        LogHelper::LogError("game.system", "<XIGNCODE> User kicked by Xigncode: session=%d", uida);
    }

    LogHelper::LogError("game.system", "<XIGNCODE> Call DisconectionProc %d ", uida);
}

/**
 * @brief 接收客户端 Xigncode 数据并转发给 SDK
 *
 * IDA 反编译 (0x140001400):
 * - 调用 OnReceive 将数据传递给 SDK
 */
void CXigncode::RecvXigncode(int sessionID, const PS_XIGNCODE_UPDATE& stXigncode) {
    m_pXigncodeServer->OnReceive(m_pXigncodeServer, reinterpret_cast<void*>(sessionID), stXigncode.szBuff, 512);
}
