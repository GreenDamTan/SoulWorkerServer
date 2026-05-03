#include "Soulworker/GameServer/XDBAgent/SQLProcessImpl.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"  // For ST_BLOCK_INFO
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"  // For ST_LEAGUE_INFO, ST_LEAGUE_MEMBER_LIST etc.
#include "Soulworker/GameServer/XDBAgent/DBAgent.h"  // For XDBAgent, XOption

#include <cwchar>
#include <cstring>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

// XSQLSystemPorcess - 注意保留原始拼写错误
XSQLSystemPorcess::XSQLSystemPorcess() : TXSQLProcessBase<0x01>("XSQLSystemPorcess") {}

XSQLSystemPorcess::~XSQLSystemPorcess() = default;

std::int32_t XSQLSystemPorcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    const char SubCmd = xPacket.GetSubCmd();
    switch (SubCmd) {
    case 1:
        return ReqConnectHanNetCafeList(pDBStmt, xPacket, xReturnSessionID);
    case 2:
        return ReqConnectHanNetCafeUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 3:
        return ReqWorldEventInfoSync(pDBStmt, xPacket, xReturnSessionID);
    case 4:
        return ReqGetDBTime(pDBStmt, xPacket, xReturnSessionID);
    default:
        return -1;
    }
}

std::int32_t XSQLSystemPorcess::ReqConnectHanNetCafeList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C3EE0: SP_NETCAFE_CONNECT_USER_LOAD（网吧连接用户列表）
    std::int32_t nServerID = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> nServerID;

    PS_DB_HAN_NET_CAFE_LIST psNetCafeList{};
    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nServerID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_NETCAFE_CONNECT_USER_LOAD( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_NETCAFE_CONNECT_USER_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 81);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            PS_DB_HAN_NET_CAFE stNetCafe{};
            xDBBinder.GetData(&stNetCafe.dwUAID);
            xDBBinder.GetString(stNetCafe.szAccountID, 21);

            psNetCafeList.vecList.push_back(stNetCafe);

            if (psNetCafeList.vecList.size() >= 50) {  // 0x32 = 50
                psNetCafeList.bLast = false;
                XSendDBPacket xSendDBPacket(xReturnSessionID, 1, 1);
                xSendDBPacket << psNetCafeList;
                Send(xSendDBPacket);
                psNetCafeList.vecList.clear();
            }
        }
    }

    xDBBinder.Close();

    psNetCafeList.bLast = true;
    XSendDBPacket xSendDBPacket(xReturnSessionID, 1, 1);
    xSendDBPacket << psNetCafeList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLSystemPorcess::ReqConnectHanNetCafeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C4190: SP_NETCAFE_CONNECT_USER_INSERT（网吧连接用户插入）
    PS_DB_HAN_NET_CAFE stNetCafe{};
    xPacket >> stNetCafe;

    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stNetCafe.dwUAID, 1);
    xDBBinder.SetString(stNetCafe.szAccountID, 21, &cbTID, 1);
    xDBBinder.SetData(&stNetCafe.nServerID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_NETCAFE_CONNECT_USER_INSERT( ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_NETCAFE_CONNECT_USER_INSERT ] [ %d error ] - Failed query( %d )", sqlReturn, 117);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLSystemPorcess::ReqWorldEventInfoSync(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C4310: SP_WORLD_EVENT_INFO（世界事件同步）
    PS_DB_WORLD_EVENT_INFO_REQ psEventReq{};
    xPacket >> psEventReq;

    PS_DB_WORLD_EVENT_INFO_RES psEventRes{};
    psEventRes.psInfo.nEventID = psEventReq.nEventID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psEventReq.dwUAID, 1);
    xDBBinder.SetData(&psEventReq.dwUCID, 1);
    xDBBinder.SetData(&psEventRes.psInfo.nEventID, 1);
    xDBBinder.SetData(&psEventRes.psInfo.nTotalCount, 4);
    xDBBinder.SetData(&psEventRes.psInfo.nMyCount, 4);
    xDBBinder.SetData(&psEventRes.biLastRegisterDate, 4);
    xDBBinder.SetData(&psEventRes.biDailyRewardDate, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_WORLD_EVENT_INFO( ?, ?, ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_WORLD_EVENT_INFO sync ] [%d error] UCID:%d - Failed query( %d )",
                            sqlReturn, psEventReq.dwUCID, 163);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        ST_WORLD_EVENT_REWARD_INFO stRewardInfo{};
        stRewardInfo.byRewardState = 2;
        xDBBinder.GetData(&stRewardInfo.byRewardType);
        xDBBinder.GetData(&stRewardInfo.nRewardIndex);  // 实际读取 int

        psEventRes.psInfo.vecRewardInfo.push_back(stRewardInfo);
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 1, 3);
    xSendDBPacket << psEventRes.psInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLSystemPorcess::ReqGetDBTime(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C4610: 调用 SP_GET_DB_TIME 存储过程
    std::int16_t sqlReturn = 0;
    PS_CHAT_NOTICE psInfo{};
    XDBBinder xDBBinder(pDBStmt);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_GET_DB_TIME }")));
    if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        sqlReturn = xDBBinder.GetWString(psInfo.strMsg, 512);
    }
    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 1, 4);
    xSendDBPacket << psInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

// XSQLLoginProcess
XSQLLoginProcess::XSQLLoginProcess() : TXSQLProcessBase<0x02>("XSQLLoginProcess") {}

XSQLLoginProcess::~XSQLLoginProcess() = default;

std::int32_t XSQLLoginProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA: 50+ SubCmd handlers
    const std::uint8_t SubCmd = static_cast<std::uint8_t>(xPacket.GetSubCmd());
    switch (SubCmd) {
    case 0x01: return ReqUserLogin(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqUserLogout(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqUserLoginForNHN(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqUserLoginForSG(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqUserLoginForGF(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqUserLoginForTWN(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqUserLoginForWM(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqUserLoginForCHN(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqEnterServer(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqClearUserState(pDBStmt, xPacket, xReturnSessionID);
    case 0x13: return ReqUpdateUserState(pDBStmt, xPacket, xReturnSessionID);
    case 0x14: return ReqLoginCharacterCount(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ReqLoadServerGroupInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqAddServerGroupInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x22: return ReqUpdateServerGroupInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqDeleteServerGroupInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqUpdateCharacterCountForServer(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqLoginUpdateUserState(pDBStmt, xPacket, xReturnSessionID);
    case 0x30: return ReqOptionKeyUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x31: return ReqOptionLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x32: return ReqOptionUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x33: return ReqOptionLoad_UCID(pDBStmt, xPacket, xReturnSessionID);
    case 0x34: return ReqContinueSecondPW(pDBStmt, xPacket, xReturnSessionID);
    case 0x35: return ReqCreateSecondPW(pDBStmt, xPacket, xReturnSessionID);
    case 0x36: return ReqCheckSecondPW(pDBStmt, xPacket, xReturnSessionID);
    case 0x37: return ReqSecondPWStateCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x38: return ReqCreateTradePW(pDBStmt, xPacket, xReturnSessionID);
    case 0x39: return ReqCheckTradePW(pDBStmt, xPacket, xReturnSessionID);
    case 0x3A: return ReqTradePWStateCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x3B: return ReqTradePWContinue(pDBStmt, xPacket, xReturnSessionID);
    case 0x40: return ReqUserLoadCash(pDBStmt, xPacket, xReturnSessionID);
    case 0x41: return ReqUserAddCash(pDBStmt, xPacket, xReturnSessionID);
    case 0x42: return ReqHanBillingOrderNo(pDBStmt, xPacket, xReturnSessionID);
    case 0x43: return ReqHanBillingUpdateInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x44: return ReqHanBillingOrderNo2(pDBStmt, xPacket, xReturnSessionID);
    case 0x45: return ReqAutoMailRecv(pDBStmt, xPacket, xReturnSessionID);
    case 0x46: return ReqIndulgenceLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x47: return ReqIndulgenceUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x48: return ReqSGBillingOrderNo(pDBStmt, xPacket, xReturnSessionID);
    case 0x49: return ReqUseCoupon(pDBStmt, xPacket, xReturnSessionID);
    case 0x50: return ReqCheckServerGroupID(pDBStmt, xPacket, xReturnSessionID);
    case 0x51: return ReqUserSetCash(pDBStmt, xPacket, xReturnSessionID);
    case 0x52: return ReqSGAuthInfoUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x53: return ReqSGAuthInfoLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x54: return ReqAccountSessionIDUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x55: return ReqAccountEventLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x56: return ReqAccountEventUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x57: return ReqGFAuthInfoUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x58: return ReqGFAuthInfoLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x59: return ReqAutoBlockAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x60: return ReqTWNBillingOrderNo(pDBStmt, xPacket, xReturnSessionID);
    case 0x61: return ReqTWNBillingUpdateInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x62: return ReqWMAuthInfoUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x63: return ReqWMAuthInfoLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x64: return ReqWMBillingOrderNo(pDBStmt, xPacket, xReturnSessionID);
    case 0x65: return ReqWMBillingUpdateInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x66: return ReqAccountDateInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x67: return ReqAccountCashMileageInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x68: return ReqAccountCashMileageUpdate(pDBStmt, xPacket, xReturnSessionID);
    default:
        return -1;
    }
}

std::int32_t XSQLLoginProcess::ReqUserLogin(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140078050: 用户登录处理
    std::int16_t sqlReturn = -1;
    std::int16_t sIDLen = 0;
    std::int16_t sPWLen = 0;
    std::int16_t sMACLen = 0;
    int nIP = 0;
    int nClientPacketVersion = 0;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;
    std::uint8_t byChannelType = 1;

    // 读取登录信息
    std::wstring szID(21, L'\0');
    std::wstring szPW(21, L'\0');
    std::wstring szMacAddress(18, L'\0');
    wchar_t szLoginResultMsg[1032] = {};

    xPacket.XParse.GetWString(&szID[0], 21, sIDLen);
    if (xPacket.XParse.GetLastError()) {
        LogHelper::LogError("game.contents", "ID Length Error");
        SendErrorMessage(1, 0xC3B6);
        return 0;
    }

    xPacket.XParse.GetWString(&szPW[0], 21, sPWLen);
    if (xPacket.XParse.GetLastError()) {
        LogHelper::LogError("game.contents", "PW Length Error");
        return 0;
    }

    xPacket.XParse.GetWString(&szMacAddress[0], 18, sMACLen);
    if (xPacket.XParse.GetLastError()) {
        LogHelper::LogError("game.contents", "Mac Length Error");
        return 0;
    }

    xPacket.XParse >> nClientPacketVersion;
    xPacket.XParse >> nIP;
    xPacket.XParse >> nLoginEventSec;

    // 检查服务器状态
    if (CheckServerState(pDBStmt, &szID[0], nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, &szID[0], &dwUAID, szLoginResultMsg)) {
        // 检查封禁IP
        std::uint8_t byIP1 = nIP & 0xFF;
        std::uint8_t byIP2 = (nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC
            if (CheckBlockMac(pDBStmt, &szMacAddress[0], szLoginResultMsg)) {
                // 执行登录存储过程
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbTMac = -3;

                xDBBinder.SetWString(&szID[0], 0x15, &cbTID, 1);
                xDBBinder.SetWString(&szPW[0], 0x15, &cbTPW, 1);
                xDBBinder.SetData(&nIP, 1);
                xDBBinder.SetWString(&szMacAddress[0], 0x12, &cbTMac, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&byChannelType, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN ] [%d error] - Failed query( %d )", sqlReturn, 357);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                // 转换MAC地址
                std::wcstombs(stLoginRes.szMacAddress, szMacAddress.c_str(), 18);
                std::wcscpy(stLoginRes.szAuthID, szID.c_str());

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqUserLogout(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007C670: 用户登出处理
    std::int16_t sqlReturn = -1;
    int nUAID = -1;
    std::int16_t shLastServerIndex = 0;
    int nAccountState = 0;
    int nIP = 0;
    int nErrorCode = 0;

    xPacket.XParse >> nUAID;
    xPacket.XParse >> shLastServerIndex;
    xPacket.XParse >> nAccountState;
    xPacket.XParse >> nIP;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&shLastServerIndex, 1);
    xDBBinder.SetData(&nAccountState, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGOUT(?,?,?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_LOGOUT ] error - Failed query[ UAID:%d ] ( %d )", nUAID, 1335);
    } else {
        xDBBinder.Close();
        LogHelper::LogDebug("game.contents", "<LOGOUT> Succeeded! UID( %d / %d ) ", nUAID, nAccountState);
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUserLoginForNHN(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140078A70: NHN平台用户登录处理
    std::int16_t sqlReturn = -1;
    std::int16_t sIDLen = 0;
    int nIP = 0;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;
    std::uint8_t byChannelType = 1;

    // 读取登录信息 (NHN使用窄字符串)
    std::string szID(21, '\0');
    std::string szPW(21, '\0');
    std::string szMac(18, '\0');
    wchar_t szLoginResultMsg[1032] = {};
    wchar_t wszID[21] = {};

    // 设置默认密码 "1234"
    std::strcpy(&szPW[0], "1234");

    xPacket.XParse.GetString(&szID[0], 21, &sIDLen);
    xPacket.XParse.GetString(&szMac[0], 18, &sIDLen);
    xPacket.XParse >> nIP;
    xPacket.XParse >> nLoginEventSec;

    // 转换为宽字符串用于检查函数
    MultiByteToWideChar(0, 0, szID.c_str(), -1, wszID, 21);

    // 检查服务器状态
    if (CheckServerState(pDBStmt, wszID, nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, wszID, &dwUAID, szLoginResultMsg)) {
        std::uint8_t byIP1 = nIP & 0xFF;
        std::uint8_t byIP2 = (nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC (使用窄字符版本)
            if (CheckBlockMac(pDBStmt, &szMac[0], szLoginResultMsg)) {
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbMAC = -3;

                xDBBinder.SetString(&szID[0], 0x15, &cbTID, 1);
                xDBBinder.SetString(&szPW[0], 0x15, &cbTPW, 1);
                xDBBinder.SetData(&nIP, 1);
                xDBBinder.SetString(&szMac[0], 0x12, &cbMAC, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&byChannelType, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN_FOR_NHN(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN_FOR_NHN ] [%d error] - Failed query( %d )", sqlReturn, 496);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                std::strcpy(stLoginRes.szMacAddress, szMac.c_str());
                std::wcscpy(stLoginRes.szAuthID, wszID);

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqUserLoginForSG(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400793E0: SG平台用户登录处理
    std::int16_t sqlReturn = -1;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;

    // 读取请求
    PS_LOGIN_REQ_FOR_SG stLoginReq;
    xPacket >> stLoginReq;
    xPacket.XParse >> nLoginEventSec;

    // 设置默认密码 "1234" (需要转换为宽字符)
    wchar_t szPW[] = L"1234";
    wchar_t wszID[21] = {};
    wchar_t szLoginResultMsg[1032] = {};

    // 转换为宽字符串
    MultiByteToWideChar(0, 0, stLoginReq.szAccountID, -1, wszID, 21);

    // 检查服务器状态
    if (CheckServerState(pDBStmt, wszID, stLoginReq.nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, wszID, &dwUAID, szLoginResultMsg)) {
        std::uint8_t byIP1 = stLoginReq.nIP & 0xFF;
        std::uint8_t byIP2 = (stLoginReq.nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (stLoginReq.nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (stLoginReq.nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, stLoginReq.nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC
            if (CheckBlockMac(pDBStmt, stLoginReq.szMacAddress, szLoginResultMsg)) {
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbTMac = -3;

                xDBBinder.SetString(stLoginReq.szAccountID, 0x15, &cbTID, 1);
                xDBBinder.SetWString(szPW, 0x15, &cbTPW, 1);
                xDBBinder.SetData(&stLoginReq.nIP, 1);
                xDBBinder.SetString(stLoginReq.szMacAddress, 0x12, &cbTMac, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&stLoginReq.byAuthType, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN ] [%d error] - Failed query( %d )", sqlReturn, 630);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                std::strcpy(stLoginRes.szMacAddress, stLoginReq.szMacAddress);
                MultiByteToWideChar(0, 0, stLoginReq.szAccountID, -1, stLoginRes.szAuthID, 21);

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqUserLoginForTWN(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007A610: TWN平台用户登录处理
    std::int16_t sqlReturn = -1;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;
    int nAuthType = 0;

    // 读取请求
    PS_LOGIN_REQ_FOR_TWN stLoginReq;
    xPacket >> stLoginReq;
    xPacket.XParse >> nLoginEventSec;

    // 设置默认密码 "1234" (需要转换为宽字符)
    wchar_t szPW[] = L"1234";
    wchar_t wszID[21] = {};
    wchar_t szLoginResultMsg[1032] = {};

    // 转换为宽字符串
    MultiByteToWideChar(0, 0, stLoginReq.szAccountID, -1, wszID, 21);

    // 检查服务器状态
    if (CheckServerState(pDBStmt, wszID, stLoginReq.nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, wszID, &dwUAID, szLoginResultMsg)) {
        std::uint8_t byIP1 = stLoginReq.nIP & 0xFF;
        std::uint8_t byIP2 = (stLoginReq.nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (stLoginReq.nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (stLoginReq.nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, stLoginReq.nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC
            if (CheckBlockMac(pDBStmt, stLoginReq.szMacAddress, szLoginResultMsg)) {
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbTMac = -3;

                xDBBinder.SetString(stLoginReq.szAccountID, 0x15, &cbTID, 1);
                xDBBinder.SetWString(szPW, 0x15, &cbTPW, 1);
                xDBBinder.SetData(&stLoginReq.nIP, 1);
                xDBBinder.SetString(stLoginReq.szMacAddress, 0x12, &cbTMac, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&nAuthType, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN_FOR_TWN ] [%d error] - Failed query( %d )", sqlReturn, 909);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                std::strcpy(stLoginRes.szMacAddress, stLoginReq.szMacAddress);
                std::wcscpy(stLoginRes.szAuthID, wszID);

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqUserLoginForCHN(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007AF00: CHN平台用户登录处理
    std::int16_t sqlReturn = -1;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;
    int nAuthType = 0;

    // 读取请求
    PS_LOGIN_REQ_FOR_CHN stLoginReq;
    xPacket >> stLoginReq;
    xPacket.XParse >> nLoginEventSec;

    // 设置默认密码 "1234" (需要转换为宽字符)
    wchar_t szPW[] = L"1234";
    wchar_t wszID[21] = {};
    wchar_t szLoginResultMsg[1032] = {};

    // 转换为宽字符串
    MultiByteToWideChar(0, 0, stLoginReq.szAccountID, -1, wszID, 21);

    // 检查服务器状态
    if (CheckServerState(pDBStmt, wszID, stLoginReq.nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, wszID, &dwUAID, szLoginResultMsg)) {
        std::uint8_t byIP1 = stLoginReq.nIP & 0xFF;
        std::uint8_t byIP2 = (stLoginReq.nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (stLoginReq.nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (stLoginReq.nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, stLoginReq.nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC
            if (CheckBlockMac(pDBStmt, stLoginReq.szMacAddress, szLoginResultMsg)) {
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbTMac = -3;

                xDBBinder.SetString(stLoginReq.szAccountID, 0x15, &cbTID, 1);
                xDBBinder.SetWString(szPW, 0x15, &cbTPW, 1);
                xDBBinder.SetData(&stLoginReq.nIP, 1);
                xDBBinder.SetString(stLoginReq.szMacAddress, 0x12, &cbTMac, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&nAuthType, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN_FOR_CHN ] [%d error] - Failed query( %d )", sqlReturn, 1048);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                std::strcpy(stLoginRes.szMacAddress, stLoginReq.szMacAddress);
                std::wcscpy(stLoginRes.szAuthID, wszID);

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqUserLoginForGF(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140079CC0: GF平台用户登录处理
    std::int16_t sqlReturn = -1;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;
    std::uint8_t byChannelType = 1;

    // 读取请求
    PS_LOGIN_REQ_FOR_GF stLoginReq;
    ST_GF_AUTH_INFO stAuthInfo;
    xPacket >> stLoginReq;
    xPacket >> stAuthInfo;
    xPacket.XParse >> nLoginEventSec;

    // 设置默认密码 "1234"
    char szPW[] = "1234";
    wchar_t wszID[21] = {};
    wchar_t szLoginResultMsg[1032] = {};

    // 使用DisplayName转换为宽字符串
    MultiByteToWideChar(0, 0, stAuthInfo.szDisplayName, -1, wszID, 21);

    // 检查服务器状态
    if (CheckServerState(pDBStmt, wszID, stLoginReq.nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, wszID, &dwUAID, szLoginResultMsg)) {
        std::uint8_t byIP1 = stLoginReq.nIP & 0xFF;
        std::uint8_t byIP2 = (stLoginReq.nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (stLoginReq.nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (stLoginReq.nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, stLoginReq.nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC
            if (CheckBlockMac(pDBStmt, stLoginReq.szMacAddress, szLoginResultMsg)) {
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbMAC = -3;

                xDBBinder.SetString(stAuthInfo.szDisplayName, 0x15, &cbTID, 1);
                xDBBinder.SetString(szPW, 0x15, &cbTPW, 1);
                xDBBinder.SetData(&stLoginReq.nIP, 1);
                xDBBinder.SetString(stLoginReq.szMacAddress, 0x12, &cbMAC, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&byChannelType, 1);
                xDBBinder.SetString(stAuthInfo.szGFAccountID, 0x100, &cbTID, 1);
                xDBBinder.SetString(stAuthInfo.szGFClientID, 0x100, &cbTID, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN_FOR_GF(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN_FOR_GF ] [%d error] - Failed query( %d )", sqlReturn, 770);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                std::strcpy(stLoginRes.szMacAddress, stLoginReq.szMacAddress);
                std::wcscpy(stLoginRes.szAuthID, wszID);

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqUserLoginForWM(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007B7F0: WM平台用户登录处理
    std::int16_t sqlReturn = -1;
    int nLoginEventSec = 0;
    unsigned int dwUAID = 0;
    int nServerState = 0;
    std::int16_t wState = 0;

    // 读取请求
    PS_LOGIN_REQ_FOR_WM stLoginReq;
    ST_WM_AUTH_INFO stAuthInfo;
    xPacket >> stLoginReq;
    xPacket >> stAuthInfo;
    xPacket.XParse >> nLoginEventSec;

    // 设置默认密码 "1234"
    char szPW[] = "1234";
    wchar_t wszID[21] = {};
    wchar_t szLoginResultMsg[1032] = {};

    // 使用szHID转换为宽字符串
    MultiByteToWideChar(0, 0, stLoginReq.szHID, -1, wszID, 21);

    // 检查服务器状态
    if (CheckServerState(pDBStmt, wszID, stLoginReq.nIP, &nServerState, szLoginResultMsg) && nServerState) {
        PS_LOGIN_RES st{};
        st.nErrorCode = 7;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // 检查封禁账号
    if (CheckBlockAccount(pDBStmt, wszID, &dwUAID, szLoginResultMsg)) {
        std::uint8_t byIP1 = stLoginReq.nIP & 0xFF;
        std::uint8_t byIP2 = (stLoginReq.nIP >> 8) & 0xFF;
        std::uint8_t byIP3 = (stLoginReq.nIP >> 16) & 0xFF;
        std::uint8_t byIP4 = (stLoginReq.nIP >> 24) & 0xFF;

        if (CheckBlockIP(pDBStmt, stLoginReq.nIP, byIP2, byIP3, byIP4, szLoginResultMsg)) {
            // 检查封禁MAC
            if (CheckBlockMac(pDBStmt, stLoginReq.szMacAddress, szLoginResultMsg)) {
                XDBBinder xDBBinder(pDBStmt);
                PS_LOGIN_RES stLoginRes{};

                std::int64_t cbTID = -3;
                std::int64_t cbTPW = -3;
                std::int64_t cbTMac = -3;

                xDBBinder.SetString(stLoginReq.szHID, 0x15, &cbTID, 1);
                xDBBinder.SetString(szPW, 0x15, &cbTPW, 1);
                xDBBinder.SetData(&stLoginReq.nIP, 1);
                xDBBinder.SetString(stLoginReq.szMacAddress, 0x12, &cbTMac, 1);
                xDBBinder.SetData(&nLoginEventSec, 1);
                xDBBinder.SetData(&stAuthInfo.byChannelType, 1);
                xDBBinder.SetData(&stAuthInfo.byUserType, 1);
                xDBBinder.SetData(&stLoginRes.nUAID, 4);
                xDBBinder.SetData(&wState, 4);
                xDBBinder.SetData(&stLoginRes.byLoginType, 4);
                xDBBinder.SetData(&stLoginRes.biAuthSessionID, 4);
                xDBBinder.SetData(&stLoginRes.byGMPower, 4);
                xDBBinder.SetData(&stLoginRes.nErrorCode, 4);

                sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN_FOR_WM(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

                if ((sqlReturn & 0xFFFFFFFE) != 0) {
                    if (sqlReturn != 100) {
                        xDBBinder.Close();
                    }
                    stLoginRes.nErrorCode = -1;
                    LogHelper::LogError("game.contents", "[ SP_LOGIN_FOR_WM ] [%d error] - Failed query( %d )", sqlReturn, 1186);
                } else {
                    xDBBinder.Close();
                    stLoginRes.bClearTutorial = wState != 0;
                }

                std::strcpy(stLoginRes.szMacAddress, stLoginReq.szMacAddress);
                std::wcscpy(stLoginRes.szAuthID, wszID);

                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << stLoginRes;
                Send(xSendDBPacket);

                return sqlReturn;
            } else {
                PS_LOGIN_RES st{};
                st.nUAID = dwUAID;
                st.nErrorCode = 5;
                std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
                XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
                xSendDBPacket << st;
                Send(xSendDBPacket);
                return sqlReturn;
            }
        } else {
            PS_LOGIN_RES st{};
            st.nUAID = dwUAID;
            st.nErrorCode = 4;
            std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
            XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
            xSendDBPacket << st;
            Send(xSendDBPacket);
            return sqlReturn;
        }
    } else {
        PS_LOGIN_RES st{};
        st.nUAID = dwUAID;
        st.nErrorCode = 3;
        std::wcscpy(st.szLoginResultMsg, szLoginResultMsg);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 1);
        xSendDBPacket << st;
        Send(xSendDBPacket);
        return sqlReturn;
    }
}
std::int32_t XSQLLoginProcess::ReqEnterServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007C0F0: 进入服务器处理
    std::int16_t sqlReturn = -1;
    int nUAID = -1;
    std::uint16_t wGroupID = 0;
    std::int64_t nAuthSessionID = 0;
    int nErrorCode = 0;
    int byGobackLobby = 0;
    int nAccountState = 0;
    std::uint8_t bySecondPassword = 0;
    std::uint8_t byTradePassword = 0;
    std::uint8_t byBlockType = 0;

    xPacket.XParse >> nUAID;
    xPacket.XParse >> wGroupID;
    xPacket.XParse >> nAuthSessionID;
    xPacket.XParse >> byGobackLobby;

    if (!nErrorCode) {
        std::uint8_t bySecondPWState = GetSecondePasswordState(pDBStmt, nUAID, &nAccountState);
        if (nAccountState > 1 && bySecondPWState == 1) {
            bySecondPassword = 2;
        } else if (bySecondPWState && bySecondPWState != 1) {
            bySecondPassword = 3;
        } else {
            bySecondPassword = bySecondPWState;
        }

        std::uint8_t byTradePWState = GetTradePasswordState(pDBStmt, nUAID, &nAccountState);
        if (nAccountState > 1 && byTradePWState == 1) {
            byTradePassword = 1;
        } else if (byTradePWState && byTradePWState != 1) {
            byTradePassword = 3;
        } else {
            byTradePassword = byTradePWState;
        }
    }

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&wGroupID, 1);
    xDBBinder.SetData(&byBlockType, 4);
    xDBBinder.SetData(&nAuthSessionID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ENTERSERVER(?,?,?,?) }")));

    std::string szAccountID(21, '\0');
    std::string szMac(18, '\0');
    int nState = 0;
    std::uint8_t byGM = 0;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_ENTERSERVER ] [%d error] - Failed query( %d )", sqlReturn, 1276);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&nErrorCode);
            xDBBinder.GetString(&szAccountID[0], 21);
            xDBBinder.GetString(&szMac[0], 18);
            xDBBinder.GetData(&nState);
            xDBBinder.GetData(&byGM);
        }
        xDBBinder.Close();
    }

    if (!nErrorCode && byGobackLobby == 1 && nState != 2) {
        nErrorCode = 100;
        LogHelper::LogError("game.contents", "[ SP_ENTERSERVER ] StateCheck [nState : %d] - Failed query( %d )", nState, 1283);
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x11);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << nUAID;
    xSendDBPacket.XParse << bySecondPassword;
    xSendDBPacket.XParse << byTradePassword;
    xSendDBPacket.XParse << nAuthSessionID;
    xSendDBPacket.XParse << byBlockType;
    xSendDBPacket.XParse << szAccountID;
    xSendDBPacket.XParse << szMac;
    xSendDBPacket.XParse << nState;
    xSendDBPacket.XParse << byGM;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqClearUserState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007C830: 清除用户状态
    std::int16_t sqlReturn = -1;

    PS_REQ_CLEAR_USER_STATE stClearState;
    xPacket >> stClearState;

    XDBBinder xDBBinder(pDBStmt);

    for (const auto& nUAID : stClearState.vecUserID) {
        int nErrorCode = 0;
        unsigned int dwUAID = nUAID;

        xDBBinder.SetData(&dwUAID, 1);
        xDBBinder.SetData(&nErrorCode, 4);
        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_USER_STATE_CLEAR( ?, ? ) }")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            if (sqlReturn != 100) {
                xDBBinder.Close();
            }
            nErrorCode = -1;
            LogHelper::LogError("game.contents", "[ SP_USER_STATE_CLEAR ] [%d error] - Failed query( %d )", sqlReturn, 1379);
            return nErrorCode;
        }

        xDBBinder.Close();

        if (nErrorCode) {
            LogHelper::LogDebug("game.contents", "<USER_STATE> Failed User State! UID( %d ) ", nUAID);
        }
    }

    return 0;
}
std::int32_t XSQLLoginProcess::ReqUpdateUserState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007CA30: 更新用户状态
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nState = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nState;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&nState, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_UPDATE( ?, ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1415);
    } else {
        xDBBinder.Close();
        LogHelper::LogDebug("game.contents", "<Update_Account> Successed State! UID( %d ) state : %d ", dwUAID, nState);
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqLoginCharacterCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007CB90: 查询登录角色数量
    std::int16_t sqlReturn = -1;
    int nUAID = -1;
    int nErrorCode = 0;

    xPacket.XParse >> nUAID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOGIN_CHARACTER_COUNT(?)}")));

    ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC stInfos{};

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_LOGIN_CHARACTER_COUNT ] [%d error] - Failed query( %d )", sqlReturn, 1460);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_USER_CHARACTER_COUNT_FOR_SERVER info{};
            xDBBinder.GetData(&info.nServerGroupID);
            xDBBinder.GetData(&info.wCount);
            stInfos.vecInfo.push_back(info);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x14);
    xSendDBPacket.XParse << nUAID;
    xSendDBPacket << stInfos;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqLoadServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007CF80: 加载服务器组信息
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    nErrorCode = 0;

    ST_SERVER_GROUP_INFO_VEC stServerInfos{};

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVERINFO_SELECT}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_SERVERINFO_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 1533);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_SERVER_GROUP_INFO stServerInfo{};
            xDBBinder.GetData(&stServerInfo.wID);
            xDBBinder.GetString(stServerInfo.szName, 21);
            xDBBinder.GetString(stServerInfo.szPublicIP, 513);
            xDBBinder.GetData(&stServerInfo.sPort);
            xDBBinder.GetData(&stServerInfo.nState);
            xDBBinder.GetData(&stServerInfo.nUserCount);
            stServerInfos.vecServerInfos.push_back(stServerInfo);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x20);
    xSendDBPacket << stServerInfos;
    Send(xSendDBPacket);

    return sqlReturn;
}
std::int32_t XSQLLoginProcess::ReqAddServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007D230: 添加服务器组信息
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    ST_SERVER_GROUP_INFO stServerGroupInfo{};

    xPacket >> stServerGroupInfo;

    XDBBinder xDBBinder(pDBStmt);
    std::int64_t cb = -3;  // SQL_NTS

    xDBBinder.SetData(&stServerGroupInfo.wID, 1);
    xDBBinder.SetString(stServerGroupInfo.szPublicIP, 0x201, &cb, 1);
    xDBBinder.SetData(&stServerGroupInfo.sPort, 1);
    xDBBinder.SetString(stServerGroupInfo.szName, 0x15, &cb, 1);
    xDBBinder.SetData(&stServerGroupInfo.nState, 1);
    xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVERINFO_ADD(?,?,?,?,?,?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_SERVERINFO_ADD ] [%d error] - Failed query( %d )", sqlReturn, 1580);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&nErrorCode);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x21);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUpdateServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007D4F0: 更新服务器组信息
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    ST_SERVER_GROUP_INFO stServerGroupInfo{};

    xPacket >> stServerGroupInfo;

    XDBBinder xDBBinder(pDBStmt);
    std::int64_t cb = -3;  // SQL_NTS

    xDBBinder.SetData(&stServerGroupInfo.wID, 1);
    xDBBinder.SetString(stServerGroupInfo.szPublicIP, 0x201, &cb, 1);
    xDBBinder.SetData(&stServerGroupInfo.sPort, 1);
    xDBBinder.SetString(stServerGroupInfo.szName, 0x15, &cb, 1);
    xDBBinder.SetData(&stServerGroupInfo.nState, 1);
    xDBBinder.SetData(&stServerGroupInfo.nUserCount, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVERINFO_UPDATE(?,?,?,?,?,?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_SERVERINFO_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1628);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&nErrorCode);
        }
        xDBBinder.Close();
    }

    // 继续查询用户数量
    XDBBinder xDBBinder2(pDBStmt);
    std::int16_t shStatus = 0;
    int nUserCount = 0;
    int nMaxUserCount = 0;

    xDBBinder2.SetData(&stServerGroupInfo.wID, 1);
    sqlReturn = xDBBinder2.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVERINFO_USER_COUNT_SELECT(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder2.Close();
        }
        sqlReturn = -1;
        LogHelper::LogError("game.contents", "[ SP_SERVERINFO_USER_COUNT_SELECT ] [%d error] - Failed query( %d )", -1, 1661);
    } else {
        if ((xDBBinder2.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder2.GetData(&shStatus);
            xDBBinder2.GetData(&nUserCount);
            xDBBinder2.GetData(&nMaxUserCount);
        }
        xDBBinder2.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x22);
    xSendDBPacket.XParse << shStatus;
    xSendDBPacket.XParse << nUserCount;
    xSendDBPacket.XParse << nMaxUserCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqDeleteServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007D950: 删除服务器组信息
    std::int16_t sqlReturn = -1;
    std::uint16_t wID = 0;
    int nErrorCode = 0;

    xPacket.XParse >> wID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&wID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVERINFO_DELETE(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_SERVERINFO_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 1705);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&nErrorCode);
        }
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUpdateCharacterCountForServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007DA90: 更新服务器角色数量
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    std::uint16_t wGroup = 0;
    std::uint16_t wCount = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> wGroup;
    xPacket.XParse >> wCount;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&wGroup, 1);
    xDBBinder.SetData(&wCount, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_COUNT_FOR_SERVER_UPDATE( ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_COUNT_FOR_SERVER_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1731);
    }
    xDBBinder.Close();

    return 0;
}

std::int32_t XSQLLoginProcess::ReqLoginUpdateUserState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007CE30: 更新用户状态（清除教程）
    std::int16_t sqlReturn = -1;
    int nUAID = -1;
    std::uint16_t wState = 0;

    xPacket.XParse >> nUAID;
    xPacket.XParse >> wState;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&wState, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_USER_CLEAR_TUTORIAL(?,?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_USER_CLEAR_TUTORIAL ] [%d error] - Failed query( %d )", sqlReturn, 1490);
    }
    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqOptionLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007DD20: 加载选项设置
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nOption = 0;
    ST_OPTION_BIT stOption{};
    ST_USER_KEY_OPTION stKeyOption{};

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nOption;
    stKeyOption.dwUAID = dwUAID;

    // 查询选项
    XDBBinder xDBBinder1(pDBStmt);
    xDBBinder1.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder1.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_OPTION_SELECT( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_OPTION_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 1783);
    } else if ((xDBBinder1.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder1.GetString(stOption.szOption, 64);
    }
    xDBBinder1.Close();

    if (nOption == 1) {
        // 只返回选项
        XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x79);
        xSendDBPacket << stOption;
        Send(xSendDBPacket);
    } else {
        // 还需要加载按键设置
        XDBBinder xDBBinder2(pDBStmt);
        xDBBinder2.SetData(&dwUAID, 1);
        sqlReturn = xDBBinder2.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_USER_KEY_SETTING_LOAD( ? )}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_USER_KEY_SETTING_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 1807);
        } else if ((xDBBinder2.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder2.GetString(stKeyOption.szKeyOption, 2048);
        }
        xDBBinder2.Close();

        XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x31);
        xSendDBPacket << stOption;
        xSendDBPacket << stKeyOption;
        Send(xSendDBPacket);
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqOptionLoad_UCID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007E1E0: 通过 UCID 加载选项设置
    std::int16_t sqlReturn = -1;
    ST_OPTION_BIT stOption{};
    PS_DB_CHARACTER_INFO_OTHER_REQ psReq{};

    xPacket >> psReq;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psReq.dwTargetUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_OPTION_SELECT_UCID( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_OPTION_SELECT_UCID ] [%d error] - Failed query( %d )", sqlReturn, 1864);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetString(stOption.szOption, 64);
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x20);
    xSendDBPacket << psReq;
    xSendDBPacket << stOption;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqOptionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007E060: 更新选项设置
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    ST_OPTION_BIT stOption{};

    xPacket.XParse >> dwUAID;
    xPacket >> stOption;

    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetString(stOption.szOption, 0x40, &cbTID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_OPTION_UPDATE( ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_OPTION_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1839);
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqOptionKeyUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007DBD0: 更新按键设置
    std::int16_t sqlReturn = -1;
    ST_USER_KEY_OPTION stKeyOption{};

    xPacket >> stKeyOption;

    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stKeyOption.dwUAID, 1);
    xDBBinder.SetString(stKeyOption.szKeyOption, 0x800, &cbTID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_USER_KEY_SETTING_UPDATE( ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_USER_KEY_SETTING_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1754);
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqCreateSecondPW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007EBB0: 创建二级密码
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nError = 0;
    int nAccountState = 0;
    PS_SECOND_PW_REQ psSecondPW{};

    xPacket.XParse >> dwUAID;
    xPacket >> psSecondPW;

    // 检查是否已有二级密码
    if (GetSecondePasswordState(pDBStmt, dwUAID, &nAccountState)) {
        nError = 59501;
        LogHelper::LogError("game.contents", "<Second PW Create> Failed! Already (UAID:%d) - Failed query( %d )", dwUAID, 2054);
    } else {
        std::int64_t cbTPW = -3;  // SQL_NTS

        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&dwUAID, 1);
        xDBBinder.SetString(psSecondPW.strPassword, 7, &cbTPW, 1);
        xDBBinder.SetData(&nError, 4);
        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SECOND_PASSWORD_CREATE( ?, ?, ? )}")));
        xDBBinder.Close();

        if ((sqlReturn & 0xFFFFFFFE) != 0 || nError) {
            LogHelper::LogDebug("game.contents", "<Second PW Create> Failed! UAID( %d / %s ) ", dwUAID, psSecondPW.strPassword);
            LogHelper::LogError("game.contents", "[ SP_SECOND_PASSWORD_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 2048);
        }
    }

    PS_SECOND_PW_RES psRes{};
    psRes.nErrorID = nError;
    psRes.bySecondPWState = nError ? 0 : 2;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x35);
    xSendDBPacket << psRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqCheckSecondPW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007EE90: 检查二级密码
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nError = 0;
    PS_SECOND_PW_REQ psSecondPW{};

    xPacket.XParse >> dwUAID;
    xPacket >> psSecondPW;

    std::int64_t cbTPW = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetString(psSecondPW.strPassword, 7, &cbTPW, 1);
    xDBBinder.SetData(&nError, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SECOND_PASSWORD_CHECK( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nError) {
        LogHelper::LogDebug("game.contents", "<Second PW Check> Failed! UAID( %d / %s ) ", dwUAID, psSecondPW.strPassword);
        LogHelper::LogError("game.contents", "[ SP_SECOND_PASSWORD_CHECK ] [%d error] - Failed query( %d )", sqlReturn, 2091);
    }

    PS_SECOND_PW_RES psRes{};
    psRes.nErrorID = 0;
    psRes.bySecondPWState = 2;

    if (nError == 1) {
        psRes.nErrorID = 50105;
        psRes.bySecondPWState = 1;
    } else if (nError == 2 || nError == 3) {
        psRes.nErrorID = 52003;
        psRes.bySecondPWState = 3;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x36);
    xSendDBPacket << psRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqSecondPWStateCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007F150: 检查二级密码状态
    std::int16_t sqlReturn = 0;
    unsigned int dwUAID = 0;
    int nAccountState = 0;

    xPacket.XParse >> dwUAID;

    PS_SECOND_PW_RES psRes{};
    psRes.bySecondPWState = GetSecondePasswordState(pDBStmt, dwUAID, &nAccountState);

    if (psRes.bySecondPWState) {
        if (psRes.bySecondPWState == 1) {
            psRes.bySecondPWState = 1;
        } else if (psRes.bySecondPWState == 2) {
            psRes.nErrorID = 52003;
            psRes.bySecondPWState = 3;
        } else {
            return -1;
        }
    } else {
        psRes.bySecondPWState = 0;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x37);
    xSendDBPacket << psRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqContinueSecondPW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007EA30: 继续二级密码检查
    std::int16_t sqlReturn = 0;
    unsigned int dwUAID = 0;
    int nAccountState = 0;

    xPacket.XParse >> dwUAID;

    PS_SECOND_PW_RES psRes{};
    psRes.bySecondPWState = GetSecondePasswordState(pDBStmt, dwUAID, &nAccountState);

    if (psRes.bySecondPWState) {
        if (psRes.bySecondPWState == 1) {
            psRes.bySecondPWState = 1;
        } else if (psRes.bySecondPWState == 2) {
            psRes.nErrorID = 52003;
            psRes.bySecondPWState = 3;
        } else {
            return -1;
        }
    } else {
        psRes.bySecondPWState = 0;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 2, 0x34);
    xSendDBPacket << psRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqCreateTradePW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007F2D0: 创建交易密码
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nError = 0;
    PS_TRADE_PW_REQ psTradePW{};

    xPacket.XParse >> dwUAID;
    xPacket >> psTradePW;

    std::int64_t cbTPW = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetString(psTradePW.strPassword, 5, &cbTPW, 1);
    xDBBinder.SetData(&nError, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TRADE_PASSWORD_CREATE( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nError) {
        LogHelper::LogDebug("game.contents", "<Trade PW Create> Failed! UAID( %d / %s ) ", dwUAID, psTradePW.strPassword);
        LogHelper::LogError("game.contents", "[ SP_TRADE_PASSWORD_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 2184);
    }

    PS_TRADE_PW_RES psRes{};
    psRes.nErrorID = nError;
    psRes.byTradePWState = nError ? 0 : 2;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x45);
    xSendDBPacket << psRes;
    Send(xSendDBPacket);

    return sqlReturn;
}
std::int32_t XSQLLoginProcess::ReqCheckTradePW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x14007F550: 检查交易密�?
    std::int16_t sqlReturn = -1;
    int nError = 0;
    unsigned int dwUAID = 0;
    PS_TRADE_PW_REQ psTradePW;

    xPacket.XParse >> dwUAID;
    xPacket >> psTradePW;

    XDBBinder xDBBinder(pDBStmt);

    std::int64_t cbTPW = -3;  // SQL_NTS

    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetString(psTradePW.strPassword, 5, &cbTPW);
    xDBBinder.SetData(&nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TRADE_PASSWORD_CHECK( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nError) {
        LogHelper::LogDebug("game.contents", "<Trade PW Check> Failed! UAID( %d / %s ) ", dwUAID, psTradePW.strPassword);
        LogHelper::LogError("game.contents", "[ SP_TRADE_PASSWORD_CHECK ] [%d error] - Failed query( %d )", sqlReturn, 2221);
    }

    PS_TRADE_PW_RES psRes;
    psRes.nErrorID = 0;
    psRes.byTradePWState = 2;

    if (nError == 1) {
        psRes.nErrorID = 50105;
        psRes.byTradePWState = 1;
    } else if (nError == 2) {
        psRes.nErrorID = 52003;
        psRes.byTradePWState = 3;
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x03, 0x46);
    xSendPacket << psRes;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqTradePWStateCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x14007F7F0: 检查交易密码状�?
    std::int16_t sqlReturn = 0;
    int nAccountState = 0;
    unsigned int dwUAID = 0;

    xPacket.XParse >> dwUAID;

    PS_TRADE_PW_RES psRes;
    psRes.byTradePWState = GetTradePasswordState(pDBStmt, dwUAID, &nAccountState);

    if (psRes.byTradePWState != 0) {
        if (psRes.byTradePWState == 2) {
            psRes.nErrorID = 52003;
            psRes.byTradePWState = 3;
        }
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x3A);
    xSendPacket << psRes;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqTradePWContinue(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007F960: 继续交易密码验证流程
    std::int16_t sqlReturn = -1;
    int nError = 0;
    unsigned int dwUAID = 0;
    PS_TRADE_PW_REQ psTradePW;

    xPacket.XParse >> dwUAID;
    xPacket >> psTradePW;

    XDBBinder xDBBinder(pDBStmt);

    std::int64_t cbTPW = -3;  // SQL_NTS

    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetString(psTradePW.strPassword, 5, &cbTPW);
    xDBBinder.SetData(&nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TRADE_PASSWORD_CHECK( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nError) {
        LogHelper::LogDebug("game.contents", "<Trade PW Continue> Failed! UAID( %d / %s ) ", dwUAID, psTradePW.strPassword);
        LogHelper::LogError("game.contents", "[ SP_TRADE_PASSWORD_CHECK ] [%d error] - Failed query( %d )", sqlReturn, 2253);
    }

    PS_TRADE_PW_RES psRes;
    psRes.nErrorID = 0;
    psRes.byTradePWState = 2;

    if (nError == 1) {
        psRes.nErrorID = 50105;
        psRes.byTradePWState = 1;
    } else if (nError == 2) {
        psRes.nErrorID = 52003;
        psRes.byTradePWState = 3;
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x3B);
    xSendPacket << psRes;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUserLoadCash(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007E3B0: 加载用户 Cash
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nCash = 0;

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    sqlReturn = xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LOADCASH(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LOADCASH ] [%d error] - Failed query( %d )", sqlReturn, 1897);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&nCash);
    }

    xDBBinder.Close();

    XSendDBPacket xSendPacket(xReturnSessionID, 0x03, 0x13);
    xSendPacket.XParse << nCash;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUserAddCash(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007E580: 增加 Cash
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nCash = 0;
    int nResultCash = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nCash;

    XDBBinder xDBBinder(pDBStmt);

    sqlReturn = xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.SetData(&nCash, 1);
    sqlReturn = xDBBinder.SetData(&nResultCash, 4);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ADDCASH(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_ADDCASH ] [%d error] - Failed query( %d )", sqlReturn, 1937);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x03, 0x36);
    xSendPacket.XParse << nErrorCode;
    xSendPacket.XParse << nResultCash;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUserSetCash(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14007E7F0: 设置 Cash
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nCash = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nCash;

    XDBBinder xDBBinder(pDBStmt);

    sqlReturn = xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.SetData(&nCash, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SET_CASH(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_SET_CASH ] [%d error] - Failed query( %d )", sqlReturn, dwUAID);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x51);
    xSendPacket.XParse << nErrorCode;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqCheckServerGroupID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140081BE0: 检查服务器组ID
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    int nServerGroupID = 0;
    int nLastServerGroupID = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nServerGroupID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_GET_LASTSERVER_ID( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_GET_LASTSERVER_ID ] [%d error] - (UAID:%d, GroupID:%d, Last:%d), ", sqlReturn, dwUAID, nServerGroupID, nLastServerGroupID);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&nLastServerGroupID);
    }

    xDBBinder.Close();

    // 如果当前服务器组ID与上次不同，发送通知
    if (nServerGroupID != nLastServerGroupID) {
        LogHelper::LogError("game.contents", "Already Exist Connect Server GroupID - (UAID:%d, GroupID:%d, Last:%d)", dwUAID, nServerGroupID, nLastServerGroupID);

        XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x50);
        xSendPacket.XParse << dwUAID;
        xSendPacket.XParse << nServerGroupID;
        xSendPacket.XParse << nLastServerGroupID;
        Send(xSendPacket);
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqSGAuthInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140081F00: 加载SG认证信息
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;

    xPacket.XParse >> dwUAID;

    ST_SG_AUTH_INFO stInfo;
    stInfo.nUAID = dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ACCOUNT_TOKEN_INFO_LOAD( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_TOKEN_INFO_LOAD ] [%d error] - (UAID:%d ) ", sqlReturn, dwUAID);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&stInfo.byAuthType);
        xDBBinder.GetString(stInfo.szToken, 1025);
        xDBBinder.GetString(stInfo.szRefreshToken, 1025);
        xDBBinder.GetData(&stInfo.nExpireTime);
    }

    xDBBinder.Close();

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x53);
    xSendPacket << stInfo;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqSGAuthInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140081C00: 更新SG认证信息
    std::int16_t sqlReturn = -1;
    ST_SG_AUTH_INFO stInfo;

    xPacket >> stInfo;

    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&stInfo.nUAID, 1);
    xDBBinder.SetData(&stInfo.byAuthType, 1);
    xDBBinder.SetString(stInfo.szToken, 0x401, &cbTID, 1);
    xDBBinder.SetString(stInfo.szRefreshToken, 0x401, &cbTID, 1);
    xDBBinder.SetData(&stInfo.nExpireTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_TOKEN_INFO_SETTING( ?,?,?,?,? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_TOKEN_INFO_SETTING ] [%d error] - Failed query( %d )", sqlReturn, stInfo.nUAID);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqIndulgenceLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140081540: 加载防沉迷信�?
    std::int16_t sqlReturn = -1;
    PS_INDULGENCE_INFO stLoad;

    xPacket >> stLoad;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&stLoad.dwUAID, 1);
    xDBBinder.SetData(&stLoad.nIndulgenceConnectTerm, 4);
    xDBBinder.SetData(&stLoad.nIndulgenceDisconnectTerm, 4);
    xDBBinder.SetData(&stLoad.nLastDisConnectTime, 4);
    xDBBinder.SetData(&stLoad.nLeftAlertTime, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_CONNECT_OFFLINE_TIME_SEARCH( ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_CONNECT_OFFLINE_TIME_SEARCH ] [%d error] - Failed query( %d )", sqlReturn, stLoad.dwUAID);
    }

    xDBBinder.Close();

    // 如果是登录状态，清除剩余警告时间
    if (stLoad.bLogin) {
        stLoad.nLeftAlertTime = 0;
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x46);
    xSendPacket << stLoad;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqIndulgenceUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x1400813F0: 更新防沉迷信�?
    std::int16_t sqlReturn = -1;
    PS_INDULGENCE_INFO stUpdate;

    xPacket >> stUpdate;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&stUpdate.dwUAID, 1);
    xDBBinder.SetData(&stUpdate.nIndulgenceConnectTerm, 1);
    xDBBinder.SetData(&stUpdate.nIndulgenceDisconnectTerm, 1);
    xDBBinder.SetData(&stUpdate.nLastDisConnectTime, 1);
    xDBBinder.SetData(&stUpdate.nLeftAlertTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_CONNECT_OFFLINE_TIME_SETTING( ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_CONNECT_OFFLINE_TIME_SETTING ] [%d error] - Failed query( %d )", sqlReturn, stUpdate.dwUAID);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAutoMailRecv(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140081320: 自动领取邮件奖励
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID;

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_USER_GIFT_REWARD_UPDATE( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_USER_GIFT_REWARD_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 2770);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAccountSessionIDUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400821B0: 更新账户会话ID
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID;

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ACCOUNT_SESSION_ID_UPDATE( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_SESSION_ID_UPDATE ] [%d error] - Failed query( UAID %d )", sqlReturn, dwUAID);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAccountEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140082280: 加载账户事件列表
    std::int16_t sqlReturn = -1;
    PS_ACCOUNT_EVENT_LIST stEventList;

    xPacket >> stEventList;

    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetString(stEventList.szAccountID, 21, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_WORLD_ONE_REWARD_EVENT_LOAD(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ ReqAccountEventLoad ] [%d error] [ %d ] - Failed query( %d )", sqlReturn, stEventList.dwUCID, 3061);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            std::uint32_t dwEventID = 0;
            xDBBinder.GetData(&dwEventID);
            stEventList.vecEventID.push_back(dwEventID);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x55);
    xSendPacket << stEventList;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAccountEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140082500: 更新账户事件列表
    PS_ACCOUNT_EVENT_LIST stEventList;

    xPacket >> stEventList;

    std::int64_t cbTID = -3;  // SQL_NTS

    for (const auto& dwEventID : stEventList.vecEventID) {
        XDBBinder xDBBinder(pDBStmt);

        int nErrorCode = 0;
        std::uint32_t nEventID = dwEventID;  // 复制到非 const 变量
        xDBBinder.SetString(stEventList.szAccountID, 21, &cbTID, 1);
        xDBBinder.SetData(&nEventID, 1);
        xDBBinder.SetData(&nErrorCode, 1);

        std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_WORLD_ONE_REWARD_EVENT_UPDATE( ?,?,? )}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_WORLD_ONE_REWARD_EVENT_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, stEventList.dwUCID);
        }
        if (nErrorCode) {
            LogHelper::LogError("game.contents", "[ SP_WORLD_ONE_REWARD_EVENT_UPDATE ] [%d error] - ( %d )", nErrorCode, stEventList.dwUCID);
        }

        xDBBinder.Close();
    }

    return 0;
}

std::int32_t XSQLLoginProcess::ReqAutoBlockAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140081FD0: 自动封禁添加
    std::int16_t sqlReturn = -1;
    PS_DB_CHECK_AUTO_BLOCK_INFO psBlock;

    xPacket >> psBlock;

    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetString(psBlock.szAccountID, 21, &cbTID, 1);
    xDBBinder.SetData(&psBlock.byTypeDB, 1);
    xDBBinder.SetString(psBlock.szEndDate, 24, &cbTID, 1);
    xDBBinder.SetWString(psBlock.strComment, 101, &cbTID, 1);
    xDBBinder.SetWString(psBlock.strUserComment, 101, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_GMT_BLOCK_USER_INSERT( ?,?,?,?,? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_GMT_BLOCK_USER_INSERT ] [%d error] - Failed query( %s(%d) )", sqlReturn, psBlock.szAccountID, psBlock.byTypeDB);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqWMAuthInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140083190: WM平台认证信息更新
    std::int16_t sqlReturn = -1;
    ST_WM_AUTH_INFO stWMAuthInfo;

    xPacket >> stWMAuthInfo;

    std::int64_t cbTID = -3;  // SQL_NTS
    std::int64_t nTime = 0;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&stWMAuthInfo.nUAID, 1);
    xDBBinder.SetData(&stWMAuthInfo.byUserType, 1);
    xDBBinder.SetString(stWMAuthInfo.szToken, 256, &cbTID, 1);
    xDBBinder.SetString(stWMAuthInfo.szWMAccountID, 256, &cbTID, 1);
    xDBBinder.SetData(&nTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_TOKEN_INFO_SETTING( ?,?,?,?,? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_TOKEN_INFO_SETTING ] [%d error] - Failed query( %d )", sqlReturn, stWMAuthInfo.nUAID);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqWMAuthInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140083350: 加载WM平台认证信息
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID;
    ST_WM_AUTH_INFO stWMAuthInfo;

    xPacket.XParse >> dwUAID;
    stWMAuthInfo.nUAID = dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ACCOUNT_TOKEN_INFO_LOAD( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_TOKEN_INFO_LOAD ] [%d error] - (UAID:%d ) ", sqlReturn, dwUAID);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&stWMAuthInfo.byUserType);
        xDBBinder.GetString(stWMAuthInfo.szToken, 256);
        xDBBinder.GetString(stWMAuthInfo.szWMAccountID, 256);
        std::int64_t nTime = 0;
        xDBBinder.GetData(&nTime);
    }

    xDBBinder.Close();

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x63);
    xSendPacket << stWMAuthInfo;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqGFAuthInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140082720: 更新GF平台认证信息
    std::int16_t sqlReturn = -1;
    ST_GF_AUTH_INFO stInfo;

    xPacket >> stInfo;

    std::int64_t cbTID = -3;  // SQL_NTS
    int nAuthType = 1;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&stInfo.nUAID, 1);
    xDBBinder.SetData(&nAuthType, 1);
    xDBBinder.SetString(stInfo.szToken, 1025, &cbTID, 1);
    xDBBinder.SetString(stInfo.szRefreshToken, 1025, &cbTID, 1);
    xDBBinder.SetData(&stInfo.nExpireTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_TOKEN_INFO_SETTING( ?,?,?,?,? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_TOKEN_INFO_SETTING ] [%d error] - Failed query( %d )", sqlReturn, stInfo.nUAID);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqGFAuthInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400828E0: 加载GF平台认证信息
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID;
    ST_GF_AUTH_INFO stInfo;

    xPacket.XParse >> dwUAID;
    stInfo.nUAID = dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ACCOUNT_TOKEN_INFO_LOAD( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_TOKEN_INFO_LOAD ] [%d error] - (UAID:%d ) ", sqlReturn, dwUAID);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        int nAuthType = 0;
        xDBBinder.GetData(&nAuthType);
        xDBBinder.GetString(stInfo.szToken, 1025);
        xDBBinder.GetString(stInfo.szRefreshToken, 1025);
        xDBBinder.GetData(&stInfo.nExpireTime);
    }

    xDBBinder.Close();

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x58);
    xSendPacket << stInfo;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqUseCoupon(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140081720: 使用优惠�?
    std::int16_t sqlReturn = -1;
    PS_DB_USE_COUPON_REQ psReq;

    xPacket >> psReq;

    PS_DB_USE_COUPON_RES psRes;
    psRes.dwUAID = psReq.dwUAID;
    psRes.dwUCID = psReq.dwUCID;
    std::strncpy(psRes.szCouponCode, psReq.szCouponCode, sizeof(psRes.szCouponCode) - 1);
    psRes.nError = 1;  // 默认错误

    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&psReq.dwUAID, 1);
    xDBBinder.SetData(&psReq.dwUCID, 1);
    xDBBinder.SetString(psReq.szCouponCode, 0x15, &cbTID, 1);
    xDBBinder.SetData(&psRes.nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_COUPON_USE( ?,?,?,? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_COUPON_USE ] [%d error] - (UAID:%d, UCID:%d, Code:%s), ", sqlReturn, psReq.dwUAID, psReq.dwUCID, psReq.szCouponCode);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&psRes.nItem);
        xDBBinder.GetData(&psRes.nItemCount);
        xDBBinder.GetData(&psRes.nEventID);
        xDBBinder.GetData(&psRes.byCouponType);
    }

    xDBBinder.Close();

    XSendDBPacket xSendPacket(xReturnSessionID, 0x49, 0x23);
    xSendPacket << psRes;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAccountDateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140083C20: 加载账户登录日期信息
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID = 0;
    std::int64_t dwComebackTime = 0;
    char szCreateDate[32] = {};

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_LOGIN_DATE_LOAD( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_LOGIN_DATE_LOAD ] [%d error] - (UAID:%d ) ", sqlReturn, dwUAID);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&dwComebackTime);
        xDBBinder.GetString(szCreateDate, 24);
    }

    xDBBinder.Close();

    // 解析创建日期字符串，转换�?time64_t
    std::int64_t tCreateDate = 0;
    int _year = 2000, _mon = 1, _day = 1, _hour = 0, _min = 0, _sec = 0;

#ifdef _WIN32
    sscanf_s(szCreateDate, "%d-%d-%d %d:%d:%d", &_year, &_mon, &_day, &_hour, &_min, &_sec);
#else
    sscanf(szCreateDate, "%d-%d-%d %d:%d:%d", &_year, &_mon, &_day, &_hour, &_min, &_sec);
#endif

    // 验证日期范围
    if (_year >= 2000 && _year <= 2040 && _mon >= 1 && _mon <= 12 &&
        _day >= 1 && _day <= 31 && _hour >= 0 && _hour <= 24 &&
        _min >= 0 && _min <= 60 && _sec >= 0 && _sec <= 60) {
        // 使用 tm 结构体转换为 time_t
        struct tm tm_date = {};
        tm_date.tm_year = _year - 1900;
        tm_date.tm_mon = _mon - 1;
        tm_date.tm_mday = _day;
        tm_date.tm_hour = _hour;
        tm_date.tm_min = _min;
        tm_date.tm_sec = _sec;
        tm_date.tm_isdst = -1;
#ifdef _WIN32
        tCreateDate = _mkgmtime64(&tm_date);
#else
        tCreateDate = timegm(&tm_date);
#endif
    } else {
        // 默认使用 2000-01-01 00:00:00
        struct tm tm_default = {};
        tm_default.tm_year = 100;  // 2000 - 1900
        tm_default.tm_mon = 0;
        tm_default.tm_mday = 1;
#ifdef _WIN32
        tCreateDate = _mkgmtime64(&tm_default);
#else
        tCreateDate = timegm(&tm_default);
#endif
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x66);
    PS_DB_ACCOUNT_DATE_RES psDateRes;
    psDateRes.dwComebackTime = dwComebackTime;
    psDateRes.tCreateDate = tCreateDate;
    xSendPacket << psDateRes;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::LoadCashMileageInfo(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_DB_CASH_MILEAGE_INFO* psDBRes) {
    // Per IDA 0x1400841B0: 加载现金里程信息
    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ACCOUNT_CASH_MILEAGE_LOAD( ?, ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nErrorCode) {
        sqlReturn = -1;
        LogHelper::LogError("game.contents", "Failed query[ SP_ACCOUNT_CASH_MILEAGE_LOAD ] - UAID:%d, Error:%d", dwUAID, nErrorCode);
    } else if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&psDBRes->nCashMileage[0]);
        xDBBinder.GetData(&psDBRes->nCashMileage[1]);
        xDBBinder.GetData(&psDBRes->nCashMileage[2]);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAccountCashMileageInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140084060: 加载账户现金里程信息
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID = 0;

    xPacket.XParse >> dwUAID;

    PS_DB_CASH_MILEAGE_INFO psDBRes;
    psDBRes.byType = 0;

    sqlReturn = LoadCashMileageInfo(pDBStmt, dwUAID, &psDBRes);

    if (sqlReturn) {
        LogHelper::LogError("game.contents", "Failed query[ ReqAccountCashMileageInfo ] - UAID:%d", dwUAID);
        psDBRes.nErrorCode = 1;
    }

    // Per IDA: MainCmd=3, SubCmd=0x90 (144)
    XSendDBPacket xSendPacket(xReturnSessionID, 0x03, 0x90);
    xSendPacket << psDBRes;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqAccountCashMileageUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140084310: 更新账户现金里程
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID = 0;
    std::int32_t nCashMileage[3] = {0, 0, 0};

    xPacket.XParse >> dwUAID;
    for (int i = 0; i < 3; ++i) {
        xPacket.XParse >> nCashMileage[i];
    }

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUAID, 1);
    for (int i = 0; i < 3; ++i) {
        xDBBinder.SetData(&nCashMileage[i], 1);
    }

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ACCOUNT_CASH_MILEAGE_UPDATE( ?, ?, ?, ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_CASH_MILEAGE_UPDATE ] [%d error] - UAID:%d", sqlReturn, dwUAID);
    }

    xDBBinder.Close();

    return sqlReturn;
}

// XSQLCharacterProcess 实现
XSQLCharacterProcess::XSQLCharacterProcess() : TXSQLProcessBase<0x03>("XSQLCharacterProcess") {}
XSQLCharacterProcess::~XSQLCharacterProcess() = default;

std::int32_t XSQLCharacterProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    switch (xPacket.GetSubCmd()) {
    case 0x01:
        return ReqCharacterList(pDBStmt, xPacket, xReturnSessionID);
    case 0x02:
        return ReqCharacterCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x03:
        return ReqCharacterDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x04:
        return ReqCharacterCheckName(pDBStmt, xPacket, xReturnSessionID);
    case 0x05:
        return ReqPacketStepCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x06:
        return ReqCharacterChangeSlot(pDBStmt, xPacket, xReturnSessionID);
    case 0x07:
        return ReqCharacterRepresentativeCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x08:
        return ReqCharacterRepresentativeChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x11:
        return ReqCharacterLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x12:
        return ReqCharacterSave(pDBStmt, xPacket, xReturnSessionID);
    case 0x14:
        return ReqLoadTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x15:
        return ReqAddTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x16:
        return ReqSelectTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x17:
        return ReqClearTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x18:
        return ReqDeleteTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x19:
        return ReqOpenTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x21:
        return ReqEnterGameServer(pDBStmt, xPacket, xReturnSessionID);
    case 0x22:
        return ReqSelectCharacter(pDBStmt, xPacket, xReturnSessionID);
    case 0x23:
        return ReqCharacterChangeServerNoReturn(pDBStmt, xPacket, xReturnSessionID);
    case 0x24:
        return ReqFavoriteTitle(pDBStmt, xPacket, xReturnSessionID);
    case 0x25:
        return ReqCharacterProfilePhotoLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x26:
        return ReqCharacterProfilePhotoAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x27:
        return ReqCharacterProfilePhotoUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x28:
        return ReqCharacterProfilePhotoChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x31:
        return ReqCharacterAddMoney(pDBStmt, xPacket, xReturnSessionID);
    case 0x32:
        return ReqCharacterAddBP(pDBStmt, xPacket, xReturnSessionID);
    case 0x33:
        return ReqCharacterDeleteConnection(pDBStmt, xPacket, xReturnSessionID);
    case 0x34:
        return ReqCharacterAddEther(pDBStmt, xPacket, xReturnSessionID);
    case 0x35:
        return ReqCharacterPvPKillCount(pDBStmt, xPacket, xReturnSessionID);
    case 0x37:
        return ReqCharacterExpSave(pDBStmt, xPacket, xReturnSessionID);
    case 0x38:
        return ReqCharacterLevelUp(pDBStmt, xPacket, xReturnSessionID);
    case 0x39:
        return ReqCharacterUpdateMapID(pDBStmt, xPacket, xReturnSessionID);
    case 0x42:
        return ReqCharacterChangeServer(pDBStmt, xPacket, xReturnSessionID);
    case 0x43:
        return ReqExitServer(pDBStmt, xPacket, xReturnSessionID);
    case 0x44:
        return ReqCharacterAddFriendPoint(pDBStmt, xPacket, xReturnSessionID);
    case 0x48:
        return ReqCharacterResetWorldState(pDBStmt, xPacket, xReturnSessionID);
    case 0x49:
        return ReqCharacterLoadWorldState(pDBStmt, xPacket, xReturnSessionID);
    case 0x4A:
        return ReqCharacterUpdateWorldState(pDBStmt, xPacket, xReturnSessionID);
    case 0x4C:
        return ReqCharacterLoadMazeEnterCount(pDBStmt, xPacket, xReturnSessionID);
    case 0x50:
        return ReqCharacterUpdateDistrictState(pDBStmt, xPacket, xReturnSessionID);
    case 0x51:
        return ReqCharacterUpdateMazeState(pDBStmt, xPacket, xReturnSessionID);
    case 0x53:
        return ReqCharacterUpdateInfiniteTowerLimitTime(pDBStmt, xPacket, xReturnSessionID);
    case 0x54:
        return ReqCharacterUpdateInfiniteTowerClearChapter(pDBStmt, xPacket, xReturnSessionID);
    case 0x55:
        return ReqCharacterUpdateKilledUser(pDBStmt, xPacket, xReturnSessionID);
    case 0x56:
        return ReqCharacterClearKilledUser(pDBStmt, xPacket, xReturnSessionID);
    case 0x57:
        return ReqEchelonUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x58:
        return ReqOtherCharacterInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x59:
        return ReqUpdateUserInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x61:
        return ReqAchieveSelect(pDBStmt, xPacket, xReturnSessionID);
    case 0x62:
        return ReqAchieveUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x63:
        return ReqAchieveReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x65:
        return ReqAchieveCollect(pDBStmt, xPacket, xReturnSessionID);
    case 0x66:
        return ReqAchieveReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x67:
        return ReqClassSceneUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x68:
        return ReqCharacterCommunitySelect(pDBStmt, xPacket, xReturnSessionID);
    case 0x69:
        return ReqCharacterCommunityUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x70:
        return ReqCharacterUpdatePos(pDBStmt, xPacket, xReturnSessionID);
    case 0x71:
        return ReqCharacterFPUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x72:
        return ReqCharacterFPInit(pDBStmt, xPacket, xReturnSessionID);
    case 0x73:
        return ReqCharacterAddState(pDBStmt, xPacket, xReturnSessionID);
    case 0x74:
        return ReqCharacterRemoveState(pDBStmt, xPacket, xReturnSessionID);
    case 0x75:
        return ReqCharacterFPBoosterAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x76:
        return ReqCharacterStatusUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x77:
        return ReqCharacterLocationClearByServerID(pDBStmt, xPacket, xReturnSessionID);
    case 0x78:
        return ReqCharacterCheckLocation(pDBStmt, xPacket, xReturnSessionID);
    case 0x80:
        return ReqCharacterUpdateLeagueWithdrawPenalty(pDBStmt, xPacket, xReturnSessionID);
    case 0x81:
        return ReqCharacterUpdateLeagueDeletePenalty(pDBStmt, xPacket, xReturnSessionID);
    case 0x82:
        return ReqCharacterAddRecycle(pDBStmt, xPacket, xReturnSessionID);
    case 0x83:
        return ReqCharacterUpdateAwaken(pDBStmt, xPacket, xReturnSessionID);
    case 0x84:
        return ReqCharacterBroachActiveEffect(pDBStmt, xPacket, xReturnSessionID);
    case 0x85:
        return ReqCharacterUpdateFreeReviveCount(pDBStmt, xPacket, xReturnSessionID);
    case 0x87:
        return ReqCharacterCheatCountUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x89:
        return ReqCharacterEqualizerUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x91:
        return ReqAddTitleAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x92:
        return ReqCharacterRenovatePointUpdate(pDBStmt, xPacket, xReturnSessionID);
    default:
        return -1;
    }
}

// XSQLCharacterProcess Title handlers - Per IDA implementations
std::int32_t XSQLCharacterProcess::ReqSelectTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140021BA0: SP_TITLE_SELECT (MainCmd=3, SubCmd=0x16)
    std::uint32_t dwActor = 0;
    PS_REQ_TITLE_UPDATE stSelectTitle;
    xPacket.XParse >> dwActor;
    xPacket >> stSelectTitle;

    std::int32_t nErrorCode = 0;
    XDBBinder xDBBinder(pDBStmt);
    std::int16_t sqlReturn = -1;

    xDBBinder.SetData(&dwActor, 1);
    xDBBinder.SetData(&stSelectTitle.stInsideTitle.dwTitleID, 1);
    xDBBinder.SetData(&stSelectTitle.stInsideTitle.dwSuffix, 1);
    xDBBinder.SetData(&stSelectTitle.stOutsideTitle.dwTitleID, 1);
    xDBBinder.SetData(&stSelectTitle.stOutsideTitle.dwSuffix, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_SELECT(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_TITLE_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 1993);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x16);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stSelectTitle;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqClearTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140021E60: SP_TITLE_CLEAR (MainCmd=3, SubCmd=0x17)
    std::uint32_t dwActor = 0;
    xPacket.XParse >> dwActor;

    std::int32_t nErrorCode = 0;
    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActor, 1);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_CLEAR( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_TITLE_CLEAR ] [%d error] - Failed query( %d )", sqlReturn, 2029);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x17);
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqDeleteTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140022020: SP_TITLE_DELETE (MainCmd=3, SubCmd=0x18)
    std::uint32_t dwActor = 0;
    std::int32_t nTitle = 0;
    xPacket.XParse >> dwActor;
    xPacket.XParse >> nTitle;

    std::int32_t nErrorCode = 0;
    XDBBinder xDBBinder(pDBStmt);
    std::int16_t sqlReturn = -1;

    xDBBinder.SetData(&dwActor, 1);
    xDBBinder.SetData(&nTitle, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_DELETE( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_TITLE_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 2065);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x18);
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqOpenTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140022210: SP_TITLE_OPEN (MainCmd=3, SubCmd=0x19) - 无响应包
    std::uint32_t dwActor = 0;
    std::uint32_t dwTitle = 0;
    xPacket.XParse >> dwActor;
    xPacket.XParse >> dwTitle;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActor, 1);
    xDBBinder.SetData(&dwTitle, 1);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_OPEN( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_TITLE_OPEN ] [%d error] - Failed query( %d )", sqlReturn, 2099);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqEnterGameServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001FD80: 空实现，直接返回0
    return 0;
}

std::int32_t XSQLCharacterProcess::ReqSelectCharacter(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001F820: SP_CHARACTER_SELECT (MainCmd=3, SubCmd=0x22)
    // TODO: 完整实现需要 STCharInfo 和 UpdateLastUCID
    return -1;  // Stub - requires complex types
}

std::int32_t XSQLCharacterProcess::ReqCharacterChangeServerNoReturn(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140020FF0: SP_CHARACTER_CHANGE_MAP - 无响应包
    PS_ENTER_MAP_RES stEnterMapRes;
    std::int32_t nPrevMapID = 0;
    std::int32_t nPrevRevivePoint = 0;

    xPacket >> stEnterMapRes;
    xPacket.XParse >> nPrevMapID;
    xPacket.XParse >> nPrevRevivePoint;

    std::int32_t nErrorCode = 0;
    std::int32_t nUCID = static_cast<std::int32_t>(stEnterMapRes.dwUserID);

    XDBBinder xDBBinder(pDBStmt);
    std::int16_t sqlReturn = -1;

    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&stEnterMapRes.uxMapID.nMapID, 1);
    xDBBinder.SetData(&stEnterMapRes.uxMapID, 1);
    xDBBinder.SetData(&stEnterMapRes.stPosInfo.vPos.x, 1);
    xDBBinder.SetData(&stEnterMapRes.stPosInfo.vPos.y, 1);
    xDBBinder.SetData(&stEnterMapRes.stPosInfo.vPos.z, 1);
    xDBBinder.SetData(&stEnterMapRes.stPosInfo.fRot, 1);
    xDBBinder.SetData(&stEnterMapRes.nJumpID, 1);
    xDBBinder.SetData(&nErrorCode, 4);
    xDBBinder.SetData(&stEnterMapRes.dwServerID, 1);
    xDBBinder.SetData(&nPrevMapID, 1);
    xDBBinder.SetData(&nPrevRevivePoint, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_CHANGE_MAP(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_CHANGE_MAP ] [%d error] - Failed query( %d )", sqlReturn, 1775);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

// ============================================================================
// XSQLCharacterProcess - stub implementations for missing functions
// ============================================================================

std::int32_t XSQLCharacterProcess::ReqCharacterList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001ABC0: SP_CHARACTER_LOAD_LIST (MainCmd=3, SubCmd=0x01)
    // TODO: Full implementation requires STCharInfo, STMyCharInfoEx, PS_CHARACTER_MAP_LIST
    std::int32_t nUAID = -1;
    xPacket.XParse >> nUAID;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    XDBBinder xDBBinder(pDBStmt);

    std::uint8_t byEchelonLevel = 0;
    std::int32_t nEchelonExp = 0;
    std::int64_t nDeleteCharListExpireTime = 0;
    std::uint32_t dwRepresentativeUCID = 0;
    std::int64_t nLastRepresentativeCharTime = 0;

    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&byEchelonLevel, 4);
    xDBBinder.SetData(&nEchelonExp, 4);
    xDBBinder.SetData(&nDeleteCharListExpireTime, 4);
    xDBBinder.SetData(&dwRepresentativeUCID, 4);
    xDBBinder.SetData(&nLastRepresentativeCharTime, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_LOAD_LIST(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_LOAD_LIST ] [%d error] - Failed query( %d )", sqlReturn, 382);
    } else {
        xDBBinder.Close();
    }

    // TODO: Complete response packet with character list
    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x01);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001B580: SP_CHARACTER_CREATE (MainCmd=3, SubCmd=0x02)
    // TODO: Full implementation requires PS_DB_CHARACTER_CREATE and related types
    std::int32_t nErrorCode = -1;

    LogHelper::LogError("game.contents", "[ SP_CHARACTER_CREATE ] Not implemented - UAID error", 496);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x02);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return -1;
}

std::int32_t XSQLCharacterProcess::ReqCharacterDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001C7C0: SP_CHARACTER_DELETE (MainCmd=3, SubCmd=0x03)
    std::int32_t nUCID = 0;
    std::int32_t nUAID = 0;
    int bSend = 0;  // XParse only supports int type

    xPacket.XParse >> nUCID;
    xPacket.XParse >> nUAID;
    xPacket.XParse >> bSend;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    std::int32_t nLastUCID = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&nLastUCID, 4);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_DELETE(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 658);
    } else {
        xDBBinder.Close();
    }

    if (nErrorCode) {
        LogHelper::LogDebug("game.contents", "<CHAR> no have character ( %d / %d ) [%d error] ", nUCID, nUAID, nErrorCode);
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x03);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << nUCID;
    xSendDBPacket.XParse << nLastUCID;
    xSendDBPacket.XParse << bSend;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterCheckName(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001D1A0: SP_CHECKCHARACTERNAME (MainCmd=3, SubCmd=0x04)
    PS_REQ_CHECK_NAME stCheckName;
    xPacket >> stCheckName;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    std::uint32_t dwUCID = 0;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetWString(reinterpret_cast<wchar_t*>(&stCheckName), 21, &cbTID, 1);
    xDBBinder.SetData(&dwUCID, 4);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call  SP_CHECKCHARACTERNAME(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHECKCHARACTERNAME ] [%d error] - Failed query( %d )", sqlReturn, 823);
    } else {
        xDBBinder.Close();
    }

    PS_RES_CHECK_NAME stCheckNameRes;
    stCheckNameRes.byResult = static_cast<std::uint8_t>(nErrorCode);
    // wcscpy_s(stCheckNameRes.strName, 21, stCheckName.strName);  // TODO: safe copy

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x04);
    xSendDBPacket << stCheckNameRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqPacketStepCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027CD0: Simple echo (MainCmd=3, SubCmd=0x05)
    std::uint32_t dwPacketID = 0;
    xPacket.XParse >> dwPacketID;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x05);
    xSendDBPacket.XParse << dwPacketID;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLCharacterProcess::ReqCharacterChangeSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001CAA0: SP_CHARACTER_SLOT_CHANGE (MainCmd=3, SubCmd=0x06)
    PS_CHARACTER_CHANGE_SLOT stSlot;
    xPacket >> stSlot;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stSlot.nUAID, 1);
    xDBBinder.SetData(&stSlot.nSrcUCID, 1);
    xDBBinder.SetData(&stSlot.nDestUCID, 1);
    xDBBinder.SetData(&stSlot.bySrcSlot, 1);
    xDBBinder.SetData(&stSlot.byDestSlot, 1);
    xDBBinder.SetData(&stSlot.nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_SLOT_CHANGE(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        stSlot.nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_SLOT_CHANGE ][%d error] - Failed query( %d / %d )", sqlReturn, stSlot.nErrorCode, stSlot.nUAID);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x06);
    xSendDBPacket << stSlot;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterRepresentativeCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001CD00: SP_REPRESENTATIVE_UPDATE_CHECK (MainCmd=3, SubCmd=0x07)
    std::uint32_t dwUAID = 0;
    xPacket.XParse >> dwUAID;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_REPRESENTATIVE_UPDATE_CHECK( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_REPRESENTATIVE_UPDATE_CHECK ][%d error] - Failed query( %d / %d )", sqlReturn, dwUAID, -1);
    } else {
        xDBBinder.Close();
    }

    if (nErrorCode == 1) {
        nErrorCode = 59633;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x07);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterRepresentativeChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001CF10: SP_REPRESENTATIVE_UPDATE (MainCmd=3, SubCmd=0x08)
    PS_CHARACTER_REPRESENTATIVE_CHANGE psInfo;
    xPacket >> psInfo;

    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psInfo.dwUAID, 1);
    xDBBinder.SetData(&psInfo.dwUCID, 1);
    xDBBinder.SetData(&psInfo.byClass, 1);
    xDBBinder.SetData(&psInfo.byLevel, 1);
    xDBBinder.SetWString(psInfo.strName, 21, &cbTID, 1);
    xDBBinder.SetData(&psInfo.dwProfilePhotoID, 1);
    xDBBinder.SetData(&psInfo.nRepresentativeDate, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_REPRESENTATIVE_UPDATE( ?, ?, ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        psInfo.nError = -1;
        LogHelper::LogError("game.contents", "[ SP_REPRESENTATIVE_UPDATE ][%d error] - Failed query( %d )", sqlReturn, psInfo.dwUAID);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x08);
    xSendDBPacket << psInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001D410: SP_CHARACTER_LOAD (MainCmd=3, SubCmd=0x11)
    // TODO: Full implementation requires ST_ENTER_SERVER, STMyCharInfoEx, many types
    std::int32_t nResult = 0;
    LogHelper::LogError("game.contents", "[ SP_CHARACTER_LOAD ] Not fully implemented", 0);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x11);
    xSendDBPacket.XParse << nResult;
    Send(xSendDBPacket);

    return -1;
}

std::int32_t XSQLCharacterProcess::ReqCharacterSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001F420: SP_CHARACTER_SAVE (MainCmd=3, SubCmd=0x12)
    // TODO: Full implementation requires STMyCharInfoEx
    std::int32_t nErrorCode = 0;
    LogHelper::LogError("game.contents", "[ SP_CHARACTER_SAVE ] Not fully implemented", 1289);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x12);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLCharacterProcess::ReqLoadTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140021640: SP_TITLE_LOAD (MainCmd=3, SubCmd=0x14)
    std::uint32_t nUCID = 0;
    xPacket.XParse >> nUCID;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_LOAD(?)}")));

    // TODO: Parse and collect title results

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_TITLE_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 1909);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x14);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqAddTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140021960: SP_TITLE_ADD (MainCmd=3, SubCmd=0x15)
    std::uint32_t dwActor = 0;
    PS_TITLE_ADD stAddTitle;
    xPacket.XParse >> dwActor;
    xPacket >> stAddTitle;

    std::int32_t nErrorCode = 0;
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActor, 1);
    xDBBinder.SetData(reinterpret_cast<std::uint32_t*>(&stAddTitle), 1);
    xDBBinder.SetData(&stAddTitle.nLogType, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_ADD( ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_TITLE_ADD ] [%d error] - Failed query( %d )", sqlReturn, 1949);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x15);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << stAddTitle.dwTitleID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqFavoriteTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140022350: SP_TITLE_FAVORITE (MainCmd=3, SubCmd=0x24)
    PS_TITLE_FAVORITE psFavorite;
    xPacket >> psFavorite;

    PS_DB_TITLE_FAVORITE psFavoriteDB;
    psFavoriteDB.psInfo.dwUCID = psFavorite.dwUCID;
    psFavoriteDB.psInfo.dwTitleID = psFavorite.dwTitleID;
    psFavoriteDB.psInfo.bFavorite = psFavorite.bFavorite;
    psFavoriteDB.nError = 0;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psFavoriteDB.psInfo.dwUCID, 1);
    xDBBinder.SetData(&psFavoriteDB.psInfo.dwTitleID, 1);
    xDBBinder.SetData(reinterpret_cast<std::uint8_t*>(&psFavoriteDB.psInfo.bFavorite), 1);
    xDBBinder.SetData(&psFavoriteDB.nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TITLE_FAVORITE( ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_TITLE_FAVORITE ] [%d error] - Failed query( %d )", sqlReturn, 2130);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x24);
    xSendDBPacket << psFavoriteDB;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterProfilePhotoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140028640: SP_CHARACTER_PHOTO_LOAD (MainCmd=3, SubCmd=0x25)
    // TODO: Full implementation requires PS_PROFILE_PHOTO_LOAD type
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_PHOTO_LOAD( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_PHOTO_LOAD ] [%d error] ActorID %d ", sqlReturn, dwUCID);
    } else {
        xDBBinder.Close();
    }

    // TODO: Complete response packet with photo list
    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x25);
    xSendDBPacket.XParse << static_cast<std::int32_t>(0);  // nErrorCode
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterProfilePhotoAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400288D0: (MainCmd=3, SubCmd=0x26)
    // TODO: Full implementation requires PS_DB_PROFILE_PHOTO_ADD type
    std::int32_t nErrorCode = 0;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x26);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLCharacterProcess::ReqCharacterProfilePhotoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140028B70: (MainCmd=3, SubCmd=0x27)
    // TODO: Full implementation requires PS_DB_PROFILE_PHOTO_UPDATE type
    std::int32_t nErrorCode = 0;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x27);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLCharacterProcess::ReqCharacterProfilePhotoChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140028C70: SP_CHARACTER_PHOTO_CHANGE (MainCmd=3, SubCmd=0x28)
    // TODO: Full implementation requires PS_DB_PROFILE_PHOTO_CHANGE type
    std::uint32_t dwUCID = 0;
    std::uint32_t dwPhotoID = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwPhotoID;

    std::int16_t sqlReturn = -1;
    std::int32_t nError = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&dwPhotoID, 1);
    xDBBinder.SetData(&nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_PHOTO_CHANGE( ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_PHOTO_CHANGE ] [ %d error ] - UCID %d PHOTO %d ERROR %d", sqlReturn, dwUCID, dwPhotoID, nError);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x28);
    xSendDBPacket.XParse << nError;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << dwPhotoID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterAddMoney(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001FDA0: SP_ADDMONEY (MainCmd=3, SubCmd=0x31)
    PS_DB_GOLD_UPDATE psGold;
    xPacket >> psGold;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    std::int64_t nResultMoney = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psGold.dwActorID, 1);
    xDBBinder.SetData(&psGold.nAddGold, 1);
    xDBBinder.SetData(&nResultMoney, 4);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ADDMONEY(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_ADDMONEY ] [%d error] - Failed query( %d )", sqlReturn, 1454);
    } else {
        xDBBinder.Close();
    }

    if (psGold.nTotalGold != nResultMoney) {
        LogHelper::LogError("game.contents", "[ SP_ADDMONEY ] error - Missmath money (UCID:%d, Total:%d, Result:%d) ( %d )", psGold.dwActorID, static_cast<std::int32_t>(psGold.nTotalGold), nResultMoney, 1459);
        nErrorCode = 2;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x31);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << psGold;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterAddBP(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140020060: SP_ADDBP (MainCmd=3, SubCmd=0x32)
    PS_DB_BP_UPDATE stUpdateBP;
    xPacket >> stUpdateBP;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    std::int64_t biResultBP = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stUpdateBP.psBP.dwActorID, 1);
    xDBBinder.SetData(&stUpdateBP.psBP.biBP, 1);
    xDBBinder.SetData(&biResultBP, 4);
    xDBBinder.SetData(&nErrorCode, 4);
    xDBBinder.SetData(&stUpdateBP.nLimitMonsterBP, 1);
    xDBBinder.SetData(&stUpdateBP.nLimitPVPBP, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ADDBP(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_ADDBP ] [%d error] - Failed query( %d )", sqlReturn, 1507);
    } else {
        stUpdateBP.psBP.biBP = biResultBP;
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x32);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stUpdateBP.psBP;
    Send(xSendDBPacket);

    return sqlReturn;
}

// ============================================================================
// XSQLCharacterProcess - Remaining stub implementations
// ============================================================================

std::int32_t XSQLCharacterProcess::ReqCharacterDeleteConnection(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400207F0: 角色断开连接处理
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    int nUCID = -1;
    int nUAID[5] = {0};

    xPacket.XParse >> nUAID[0];
    xPacket.XParse >> nUCID;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&nUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_EXIT_SERVER( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_EXIT_SERVER ] [%d error] - Failed query( %d )", sqlReturn, 1630);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x33);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterAddEther(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140020330: 添加以太
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    std::int64_t biResultEther = 0;

    int nUCID = -1;
    std::int64_t biEther = 0;

    xPacket.XParse >> nUCID;
    xPacket.XParse >> biEther;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&nUCID, 1);
    sqlReturn = xDBBinder.SetData(&biEther, 1);
    sqlReturn = xDBBinder.SetData(&biResultEther, 4);  // output
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ADDETHER(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_ADDETHER ] [%d error] - Failed query( %d )", sqlReturn, 1548);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x34);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biResultEther;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterPvPKillCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400205C0: PvP击杀计数更新
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    unsigned int dwKillCount = 0;
    unsigned int dwResultCount = 0;

    int nUCID = -1;
    xPacket.XParse >> nUCID;
    xPacket.XParse >> dwKillCount;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&nUCID, 1);
    sqlReturn = xDBBinder.SetData(&dwKillCount, 1);
    sqlReturn = xDBBinder.SetData(&dwResultCount, 4);  // output
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ADDPVPKILLCOUNT(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_ADDPVPKILLCOUNT ] [%d error] - Failed query( %d )", sqlReturn, 1589);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterExpSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001F300: 保存角色经验值
    std::int16_t sqlReturn = -1;
    unsigned int dwUCID = 0;
    std::int64_t nExp = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nExp;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&nExp, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_EXP_SAVE(?, ? )}")));
    xDBBinder.Close();

    if (sqlReturn == -1) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_EXP_SAVE ] [%d error] - Failed query( %d )", sqlReturn, 1242);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterLevelUp(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140021300: 角色升级处理
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    unsigned int dwUAID = 0;
    unsigned int dwUCID = 0;
    int nLevel = 0;
    int nAdd = 0;
    int nUseCheat = 0;
    std::int64_t biLevelupDate = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nLevel;
    xPacket.XParse >> nAdd;
    xPacket.XParse >> nUseCheat;
    xPacket.XParse >> biLevelupDate;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&nLevel, 1);
    sqlReturn = xDBBinder.SetData(&biLevelupDate, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_CHARACTER_LEVELUP(?, ?, ?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_LEVELUP ] [%d error] - Failed query( %d )", sqlReturn, 1828);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateMapID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140020A00: 更新角色地图ID
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    PS_ENTER_MAP_RES stEnterMapRes;
    xPacket >> stEnterMapRes;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.dwUserID, 1);
    sqlReturn = xDBBinder.SetData(reinterpret_cast<std::int64_t*>(&stEnterMapRes.uxMapID), 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.dwServerID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call  SP_CHARACTER_UPDATE_MAP(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_UPDATE_MAP ] [%d error] - Failed query( %d )", sqlReturn, 1670);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x39);
    xSendDBPacket << stEnterMapRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterChangeServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140020C40: 角色换服处理
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    int nPrevMapID = 0;
    int nPrevRevivePoint = 0;

    PS_ENTER_MAP_RES stEnterMapRes;
    xPacket >> stEnterMapRes;
    xPacket.XParse >> nPrevMapID;
    xPacket.XParse >> nPrevRevivePoint;

    XDBBinder xDBBinder(pDBStmt);
    int nUCID = stEnterMapRes.dwUserID;

    sqlReturn = xDBBinder.SetData(&nUCID, 1);
    // IDA中的wMapID是uxMapID.nMapID的高16位提取
    std::int64_t wMapID = static_cast<std::int64_t>(stEnterMapRes.uxMapID.nMapID << 16) >> 48;
    sqlReturn = xDBBinder.SetData(&wMapID, 1);
    sqlReturn = xDBBinder.SetData(reinterpret_cast<std::int64_t*>(&stEnterMapRes.uxMapID), 1);
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.stPosInfo.vPos.x, 1);
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.stPosInfo.vPos.y, 1);
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.stPosInfo.vPos.z, 1);
    sqlReturn = xDBBinder.SetData(reinterpret_cast<int*>(&stEnterMapRes.stPosInfo.fRot), 1);
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.nJumpID, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.SetData(&stEnterMapRes.dwServerID, 1);
    sqlReturn = xDBBinder.SetData(&nPrevMapID, 1);
    sqlReturn = xDBBinder.SetData(&nPrevRevivePoint, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call  SP_CHARACTER_CHANGE_MAP(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_CHANGE_MAP ] [%d error] - Failed query( %d )", sqlReturn, 1722);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x42);
    xSendDBPacket << stEnterMapRes;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqExitServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140021520: 退出服务器处理
    std::int16_t sqlReturn = -1;
    unsigned int dwActor = 0;

    xPacket.XParse >> dwActor;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActor, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_EXIT_SERVER( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_EXIT_SERVER ] [%d error] - Failed query( %d )", sqlReturn, 1860);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterAddFriendPoint(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400278E0: 添加好友点数
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    std::int64_t biResultFriendPoint = 0;

    unsigned int dwUCID = 0;
    std::int64_t biFriendPoint = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biFriendPoint;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&biFriendPoint, 1);
    sqlReturn = xDBBinder.SetData(&biResultFriendPoint, 4);  // output
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_POINT_ADD( ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FRIEND_POINT_ADD ] [%d error] - Failed query( %d )", sqlReturn, 3750);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x44);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biResultFriendPoint;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterResetWorldState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140022760: 重置角色世界状态
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    unsigned int dwUCID = 0;

    xPacket.XParse >> dwUCID;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_DISTRICT_STATE_CLEAR( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_DISTRICT_STATE_CLEAR ] [%d error] - Failed query( %d )", sqlReturn, 2216);
    } else {
        xDBBinder.Close();
    }

    return static_cast<std::uint16_t>(nErrorCode);
}

std::int32_t XSQLCharacterProcess::ReqCharacterLoadWorldState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140022870: 加载角色世界状态
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    unsigned int dwUCID = 0;
    xPacket.XParse >> dwUCID;

    ST_MAZE_STATE_LIST stMazeStateList;
    ST_DISTRICT_STATE_LIST stDistrictList;

    // 加载迷宫状态
    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_MAZE_LOAD_STATE( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_MAZE_LOAD_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2266);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_MAZE_STATE stMazeState;
            xDBBinder.GetData(&stMazeState.nMazeID);
            xDBBinder.GetData(&stMazeState.nClearCount);
            xDBBinder.GetData(&stMazeState.nSharePoint);
            std::int16_t shRewardStep = 0;
            xDBBinder.GetData(&shRewardStep);
            xDBBinder.GetData(&stMazeState.nGroupID);
            xDBBinder.GetData(&stMazeState.nDistrictID);
            stMazeState.byRewardStep = static_cast<std::uint8_t>(shRewardStep);
            stMazeStateList.vecList.push_back(stMazeState);
        }
        xDBBinder.Close();
    }

    // 加载区域状态
    XDBBinder xDBBinderDistrict(pDBStmt);
    sqlReturn = xDBBinderDistrict.SetData(&dwUCID, 1);
    sqlReturn = xDBBinderDistrict.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_DISTRICT_LOAD_STATE( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinderDistrict.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_DISTRICT_LOAD_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2299);
    } else {
        while ((xDBBinderDistrict.Fetch() & 0xFFFFFFFE) == 0) {
            ST_DISTRICT_STATE stState;
            xDBBinderDistrict.GetData(&stState.nDistrictID);
            xDBBinderDistrict.GetData(&stState.nDSPoint);
            std::int16_t shReward = 0;
            xDBBinderDistrict.GetData(&shReward);
            stState.bReward = (shReward != 0);
            stDistrictList.vecList.push_back(stState);
        }
        xDBBinderDistrict.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x49);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stDistrictList;
    xSendDBPacket << stMazeStateList;
    Send(xSendDBPacket);

    return nErrorCode;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateWorldState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140022D20: 更新角色世界状态
    std::int16_t sqlReturn = -1;
    int nMazeErrorCode = 0;
    int nDistrictErrorCode = 0;

    unsigned int dwUCID = 0;
    ST_DISTRICT_STATE stDistrictState;
    ST_MAZE_STATE stMazeState;

    xPacket.XParse >> dwUCID;
    xPacket >> stDistrictState;
    xPacket >> stMazeState;

    // 更新迷宫状态
    if (stMazeState.nMazeID > 0) {
        XDBBinder xDBBinder(pDBStmt);
        sqlReturn = xDBBinder.SetData(&dwUCID, 1);
        sqlReturn = xDBBinder.SetData(&stMazeState.nMazeID, 1);
        sqlReturn = xDBBinder.SetData(&stMazeState.nSharePoint, 1);
        sqlReturn = xDBBinder.SetData(&stMazeState.nGroupID, 1);
        std::int16_t shStep = stMazeState.byRewardStep;
        sqlReturn = xDBBinder.SetData(&shStep, 1);
        sqlReturn = xDBBinder.SetData(&stMazeState.nDistrictID, 1);
        sqlReturn = xDBBinder.SetData(&nMazeErrorCode, 4);  // output

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_MAZE_UPDATE_STATE( ?, ?, ?, ?, ?, ?, ? )}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            nMazeErrorCode = sqlReturn;
            LogHelper::LogError("game.contents", "[ SP_MAZE_UPDATE_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2342);
        }
        xDBBinder.Close();
    }

    // 更新区域状态
    if (stDistrictState.nDistrictID > 0) {
        XDBBinder xDBBinder(pDBStmt);
        sqlReturn = xDBBinder.SetData(&dwUCID, 1);
        sqlReturn = xDBBinder.SetData(&stDistrictState.nDistrictID, 1);
        sqlReturn = xDBBinder.SetData(&stDistrictState.nDSPoint, 1);
        std::int16_t in_value = stDistrictState.bReward ? 1 : 0;
        sqlReturn = xDBBinder.SetData(&in_value, 1);
        sqlReturn = xDBBinder.SetData(&nDistrictErrorCode, 4);  // output

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_DISTRICT_UPDATE_STATE( ?, ?, ?, ?, ? )}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            nDistrictErrorCode = sqlReturn;
            LogHelper::LogError("game.contents", "[ SP_DISTRICT_UPDATE_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2365);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x4A);
    xSendDBPacket.XParse << nDistrictErrorCode;
    xSendDBPacket.XParse << nMazeErrorCode;
    Send(xSendDBPacket);

    return nMazeErrorCode;
}

std::int32_t XSQLCharacterProcess::ReqCharacterLoadMazeEnterCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 PS_MAZE_ENTER_LIMIT_COUNT_LIST / PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST 类型
    // Per IDA 0x1400231D0: 加载迷宫进入次数
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateDistrictState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400238C0: 更新区块状态
    std::int16_t sqlReturn = -1;
    unsigned int dwUCID = 0;
    ST_DISTRICT_STATE stDistrictState;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> stDistrictState;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&stDistrictState.nDistrictID, 1);
    sqlReturn = xDBBinder.SetData(&stDistrictState.nDSPoint, 1);
    std::int16_t sReward = stDistrictState.bReward;
    sqlReturn = xDBBinder.SetData(&sReward, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_DISTRICT_UPDATE_STATE( ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        nErrorCode = sqlReturn;
        LogHelper::LogError("game.contents", "[ SP_DISTRICT_UPDATE_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2514);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateMazeState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140023A70: 更新迷宫状态
    std::int16_t sqlReturn = -1;
    unsigned int dwUCID = 0;
    ST_MAZE_STATE stMazeState;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> stMazeState;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&stMazeState.nMazeID, 1);
    sqlReturn = xDBBinder.SetData(&stMazeState.nSharePoint, 1);
    sqlReturn = xDBBinder.SetData(&stMazeState.nGroupID, 1);
    std::int16_t sStep = stMazeState.byRewardStep;
    sqlReturn = xDBBinder.SetData(&sStep, 1);
    sqlReturn = xDBBinder.SetData(&stMazeState.nDistrictID, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_MAZE_UPDATE_STATE( ?, ?, ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        nErrorCode = sqlReturn;
        LogHelper::LogError("game.contents", "[ SP_MAZE_UPDATE_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2548);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateInfiniteTowerLimitTime(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026BB0: 更新无限之塔限制时间
    std::int16_t sqlReturn = -1;
    int nPcLimitCount = 0;
    std::int16_t sLimitCount = 0;
    PS_UPDATE_INFINITETOWER_LIMIT_TIME stInfiteTime;

    xPacket >> stInfiteTime;
    xPacket.XParse >> nPcLimitCount;
    xPacket.XParse >> sLimitCount;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stInfiteTime.dwActorID, 1);
    xDBBinder.SetData(&stInfiteTime.nLimitTime, 1);
    xDBBinder.SetData(&sLimitCount, 1);
    xDBBinder.SetData(&nPcLimitCount, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_UPDATE_INFINITETOWER_LIMIT_TIME( ?, ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_UPDATE_INFINITETOWER_LIMIT_TIME ] [%d error] - Failed query( %d )", sqlReturn, 3495);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateInfiniteTowerClearChapter(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026D20: 更新无限之塔清除章节
    std::int16_t sqlReturn = -1;
    PS_UPDATE_INFINITETOWER_CLEAR_CHAPTER stInfiteClear;

    xPacket >> stInfiteClear;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(reinterpret_cast<unsigned int*>(&stInfiteClear), 1);  // dwActorID at offset 0
    xDBBinder.SetData(&stInfiteClear.sClearChapter, 1);
    xDBBinder.SetData(&stInfiteClear.sClearStage, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_UPDATE_INFINITETOWER_CLEAR_CHAPTER( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_UPDATE_INFINITETOWER_CLEAR_CHAPTER ] [%d error] - Failed query( %d )", sqlReturn, 3515);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateKilledUser(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026E20: 更新击杀用户数
    std::int16_t sqlReturn = -1;
    unsigned int dwActorID = 0;
    unsigned int dwKilledUser = 0;
    int nCount = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwKilledUser;
    xPacket.XParse >> nCount;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&dwKilledUser, 1);
    xDBBinder.SetData(&nCount, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_KILLED_USER_UPDATE( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_KILLED_USER_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 3539);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterClearKilledUser(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027080: 清除击杀用户
    std::int16_t sqlReturn = -1;
    unsigned int dwActorID = 0;
    std::int64_t nDeleteTime = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nDeleteTime;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&nDeleteTime, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_KILLED_USER_CLEAR( ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_KILLED_USER_CLEAR ] [%d error] - Failed query( %d )", sqlReturn, 3590);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqEchelonUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027DA0: 更新梯队信息
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    PT_ECHELON_INFO psInfo;

    xPacket.XParse >> dwUAID;
    xPacket >> psInfo;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.SetData(&psInfo.byEchelonLevel, 1);
    sqlReturn = xDBBinder.SetData(&psInfo.nEchelonExp, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ECHELON_INFO_UPDATE( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogDebug("game.contents", "<Echelon Update> Failed! UAID( %d / %d / %d ) ", dwUAID, psInfo.byEchelonLevel, psInfo.nEchelonExp);
        LogHelper::LogError("game.contents", "[ SP_ECHELON_INFO_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 3832);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqOtherCharacterInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 PS_DB_CHARACTER_INFO_OTHER_REQ / PS_DB_CHARACTER_INFO_OTHER_RES 类型
    // Per IDA 0x140025420: 查询其他角色信息
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqUpdateUserInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027F10: 更新用户信息
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    bool bNetCafe = false;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> bNetCafe;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.SetData(&bNetCafe, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_USERINFO_UPDATE( ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_USERINFO_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 3858);
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqAchieveSelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 ST_ACHIEVE_LIST / ST_ACHIEVE_BIT / ST_ACHIEVE_CATEGORY 类型
    // Per IDA 0x140023F40: 加载成就数据
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqAchieveUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 ST_ACHIEVE_UPDATE_LIST / ST_ACHIEVE_BIT / ST_ACHIEVE_CATEGORY 类型
    // Per IDA 0x1400240D0: 更新成就
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqAchieveReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 ST_ACHIEVE_INFO / ST_ACHIEVE_BIT 类型
    // Per IDA 0x1400242C0: 成就奖励
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqAchieveCollect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 ST_ACHIEVE_UPDATE_LIST 类型
    // Per IDA 0x140024550: 成就收集
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqAchieveReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140024450: 成绩重置
    unsigned int dwUCID = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    sqlReturn = AchieveClear(pDBStmt, dwUCID);
    AchieveCreateBit(pDBStmt, dwUCID);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x66);
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqClassSceneUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

std::int32_t XSQLCharacterProcess::ReqCharacterCommunitySelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

std::int32_t XSQLCharacterProcess::ReqCharacterCommunityUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

// Achievement helper methods implementation

std::int32_t XSQLCharacterProcess::AchieveClear(XDBStmt* pDBStmt, unsigned int dwUCID) {
    // TODO: 完整实现需要 SP_ACHIEVE_CLEAR 存储过程
    std::int16_t sqlReturn = -1;
    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACHIEVE_CLEAR( ? )}")));
    xDBBinder.Close();
    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACHIEVE_CLEAR ] [%d error] - Failed query( %d )", sqlReturn, 0);
    }
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::AchieveCreateBit(XDBStmt* pDBStmt, unsigned int dwUCID) {
    // TODO: 完整实现需要 SP_ACHIEVE_CREATE_BIT 存储过程
    std::int16_t sqlReturn = -1;
    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACHIEVE_CREATE_BIT( ? )}")));
    xDBBinder.Close();
    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACHIEVE_CREATE_BIT ] [%d error] - Failed query( %d )", sqlReturn, 0);
    }
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::AchieveUpdate(XDBStmt* pDBStmt, unsigned int dwUCID, ST_ACHIEVE_UPDATE* pstUpdate) {
    // TODO: 完整实现需要 ST_ACHIEVE_UPDATE 类型和 SP_ACHIEVE_UPDATE 存储过程
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::AchieveReward(XDBStmt* pDBStmt, unsigned int dwUCID, ST_ACHIEVE_BIT* pstBit) {
    // TODO: 完整实现需要 ST_ACHIEVE_BIT 类型和 SP_ACHIEVE_REWARD 存储过程
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::LoadAchieve(XDBStmt* pDBStmt, unsigned int dwUCID, ST_ACHIEVE_LIST* pstList) {
    // TODO: 完整实现需要 ST_ACHIEVE_LIST 类型和 SP_ACHIEVE_LOAD 存储过程
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::LoadAchieveBit(XDBStmt* pDBStmt, unsigned int dwUCID, ST_ACHIEVE_BIT* pstBit, ST_ACHIEVE_CATEGORY* pstCategory) {
    // TODO: 完整实现需要 ST_ACHIEVE_BIT / ST_ACHIEVE_CATEGORY 类型和 SP_ACHIEVE_BIT_LOAD 存储过程
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdatePos(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026090: 更新角色位置
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    PS_CHARACTER_UPDATE_POS stUpdatePos{};

    xPacket >> stUpdatePos;

    unsigned int nUCID = stUpdatePos.dwActorID;
    std::uint16_t wMapID = static_cast<std::uint16_t>(stUpdatePos.uxMapID.nMapID);

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&nUCID, 1);
    sqlReturn = xDBBinder.SetData(&wMapID, 1);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.uxMapID.nMapID, 1);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.vPos.x, 1);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.vPos.y, 1);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.vPos.z, 1);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.nRot, 1);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.nJumpID, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.SetData(&stUpdatePos.dwServerID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call  SP_CHARACTER_UPDATE_POS(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_UPDATE_POS ] [%d error] - Failed query( %d )", sqlReturn, 3296);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterFPUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026300: 更新角色FP值
    std::int16_t sqlReturn = -1;
    unsigned int dwActorID = 0;
    std::int16_t shFP = 0;
    std::int16_t shBonusFP = 0;
    std::int16_t shPCBangFP = 0;
    std::uint8_t byBonusEffect = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> shFP;
    xPacket.XParse >> shBonusFP;
    xPacket.XParse >> shPCBangFP;
    xPacket.XParse >> byBonusEffect;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&shFP, 1);
    xDBBinder.SetData(&shBonusFP, 1);
    xDBBinder.SetData(&shPCBangFP, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_FP_UPDATE( ?, ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_FP_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 3325);
    }

    // Send response
    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x71);
    xSendDBPacket.XParse << shFP;
    xSendDBPacket.XParse << shBonusFP;
    xSendDBPacket.XParse << shPCBangFP;
    xSendDBPacket.XParse << byBonusEffect;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterFPInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026570: 初始化FP值
    std::int16_t sqlReturn = -1;
    unsigned int dwActorID = 0;
    std::int16_t shFP = 0;
    std::int16_t shBonusFP = 0;
    std::int16_t shPCBangFP = 0;
    std::int64_t biInitDate = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> shFP;
    xPacket.XParse >> shBonusFP;
    xPacket.XParse >> shPCBangFP;
    xPacket.XParse >> biInitDate;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&shFP, 1);
    xDBBinder.SetData(&shBonusFP, 1);
    xDBBinder.SetData(&shPCBangFP, 1);
    xDBBinder.SetData(&biInitDate, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_FP_INIT( ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_FP_INIT ] [%d error] - Failed query( %d )", sqlReturn, 3360);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::AddCharacterState(XDBStmt* pDBStmt, unsigned int dwActorID, int nState) {
    // Per IDA 0x140025F70: 添加角色状态
    std::int16_t sqlReturn = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&nState, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_ADD_STATE( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_ADD_STATE ] [%d error] - Failed query( %d )", sqlReturn, 3219);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterAddState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400268B0: 添加角色状态
    unsigned int dwActorID = 0;
    int nState = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nState;

    return AddCharacterState(pDBStmt, dwActorID, nState);
}

std::int32_t XSQLCharacterProcess::ReqCharacterRemoveState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026920: 移除角色状态
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;
    unsigned int dwActorID = 0;
    int nState = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nState;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwActorID, 1);
    sqlReturn = xDBBinder.SetData(&nState, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_REMOVE_STATE( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_REMOVE_STATE ] [%d error] - Failed query( %d )", sqlReturn, dwActorID);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterFPBoosterAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140026700: 添加FP Booster
    std::int16_t sqlReturn = -1;
    unsigned int dwUAID = 0;
    unsigned int dwUCID = 0;
    std::int16_t shFP = 0;
    std::int16_t shBonusFP = 0;
    std::int16_t shPCBangFP = 0;
    std::int16_t shPCBangFP_Other = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> shPCBangFP;
    xPacket.XParse >> shPCBangFP_Other;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&shFP, 1);
    xDBBinder.SetData(&shBonusFP, 1);
    xDBBinder.SetData(&shPCBangFP, 1);
    xDBBinder.SetData(&shPCBangFP_Other, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_FP_ADD_BOOSTER( ?, ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_FP_ADD_BOOSTER ] [%d error] - Failed query( %d )", sqlReturn, 3390);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterStatusUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14001A9B0: 更新角色状态
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    unsigned int dwUCID = 0;
    unsigned int dwStatus = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwStatus;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&dwStatus, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_STATUS_UPDATE(?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_STATUS_UPDATE ][%d error] - Failed query( %d )", sqlReturn, 297);
    }

    xDBBinder.Close();

    // Send response
    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x76);
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterLocationClearByServerID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027520: 根据服务器ID清除角色位置
    std::int16_t sqlReturn = -1;
    unsigned int dwServerID = 0;

    xPacket.XParse >> dwServerID;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwServerID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_LOCATION_CLEAR(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_LOCATION_CLEAR ] [%d error] - Failed query( %d )", sqlReturn, 3670);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterCheckLocation(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 PS_DB_CHECK_LOCATION 结构体
    // Per IDA 0x140027630: 检查角色位置
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateLeagueWithdrawPenalty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027180: 更新联盟退出惩罚
    std::int16_t sqlReturn = -1;
    unsigned int dwActorID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> biPenalty;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&biPenalty, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_WITHDRAW_PENALTY( ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_WITHDRAW_PENALTY ] [%d error] - Failed query( %d )", sqlReturn, 3612);
    }

    // Send response
    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x80);
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << biPenalty;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateLeagueDeletePenalty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140027350: 更新联盟删除惩罚
    std::int16_t sqlReturn = -1;
    unsigned int dwActorID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> biPenalty;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&biPenalty, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_DELETE_PENALTY( ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_DELETE_PENALTY ] [%d error] - Failed query( %d )", sqlReturn, 3639);
    }

    // Send response
    XSendDBPacket xSendDBPacket(xReturnSessionID, 3, 0x81);
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << biPenalty;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterAddRecycle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 完整实现需要 PS_DB_RECYCLE_UPDATE 结构体
    // Per IDA 0x140028030: 添加回收记录
    return -1;  // TODO: Implement - 需要复杂类型支持
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateAwaken(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140028290: 更新觉醒信息
    std::int16_t sqlReturn = -1;
    unsigned int dwUCID = 0;
    std::uint8_t byGrade = 0;
    unsigned int dwProfilePhotoID = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byGrade;
    xPacket.XParse >> dwProfilePhotoID;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&byGrade, 1);
    sqlReturn = xDBBinder.SetData(&dwProfilePhotoID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_AWAKEN_UPDATE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_AWAKEN_UPDATE ] [ %d error ] - UCID %d GRADE %d PHOTO %d", sqlReturn, dwUCID, byGrade, dwProfilePhotoID);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterBroachActiveEffect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400283D0: 更新钻孔激活效果
    std::int16_t sqlReturn = -1;
    unsigned int dwUCID = 0;
    unsigned int dwActiveEffect = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwActiveEffect;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&dwActiveEffect, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_BROACH_EFFECT_UPDATE(?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_BROACH_EFFECT_UPDATE ] [ %d error ] - UCID %d effect %d", sqlReturn, dwUCID, dwActiveEffect);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterUpdateFreeReviveCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140028520: 更新免费复活次数
    std::int16_t sqlReturn = -1;
    unsigned int dwUCID = 0;
    int nFreeReviveCount = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nFreeReviveCount;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&nFreeReviveCount, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_CHARACTER_FREE_REVIVAL_UPDATE(?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_FREE_REVIVAL_UPDATE ] [ %d error ] - UCID %d", sqlReturn, dwUCID);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLCharacterProcess::ReqCharacterCheatCountUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

std::int32_t XSQLCharacterProcess::ReqCharacterEqualizerUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

std::int32_t XSQLCharacterProcess::ReqAddTitleAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

std::int32_t XSQLCharacterProcess::ReqCharacterRenovatePointUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    return -1;  // TODO: Implement
}

// ============================================================================
// XSQLPartyProcess Implementation
// ============================================================================

XSQLPartyProcess::XSQLPartyProcess() : TXSQLProcessBase<0x04>("XSQLPartyProcess") {}
XSQLPartyProcess::~XSQLPartyProcess() = default;

std::int32_t XSQLPartyProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    switch (xPacket.GetSubCmd()) {
    case 0x01:
        return ReqCreateParty(pDBStmt, xPacket, xReturnSessionID);
    case 0x02:
        return ReqJoinParty(pDBStmt, xPacket, xReturnSessionID);
    case 0x03:
        return ReqLeaveParty(pDBStmt, xPacket, xReturnSessionID);
    case 0x04:
        return ReqUpdateMemberInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x05:
        return ReqPartyChangeMaster(pDBStmt, xPacket, xReturnSessionID);
    case 0x06:
        return ReqPartyDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x08:
        return ReqPartyUpdateInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x09:
        return ReqPartyLoginMember(pDBStmt, xPacket, xReturnSessionID);
    case 0x11:
        return ReqPartyLoadAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x13:
        return ReqPartyMatchingCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x16:
        return ReqPartyTypeUpdate(pDBStmt, xPacket, xReturnSessionID);
    default:
        return -1;
    }
}

std::int32_t XSQLPartyProcess::ReqCreateParty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14008F280: SP_PARTY_CREATE (有响应包 MainCmd=4, SubCmd=0x01)
    std::int16_t sqlReturn = -1;

    PS_REQ_PARTY_CREATE stPartyReq;
    xPacket >> stPartyReq;

    XDBBinder xDBBinder(pDBStmt);
    stPartyReq.dwPartyID = 0;
    int nErrorCode = 0;

    sqlReturn = xDBBinder.SetData(&stPartyReq.masterInfo.dwMemberID, 1);
    sqlReturn = xDBBinder.SetData(&stPartyReq.memberInfo.dwMemberID, 1);
    sqlReturn = xDBBinder.SetData(&stPartyReq.dwPartyID, 4);  // output
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_CREATE(?, ?,  ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 102);
    } else {
        xDBBinder.Close();
    }

    stPartyReq.nErrorCode = nErrorCode;

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x01);
    xSendDBPacket << stPartyReq;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqJoinParty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14008F4D0: SP_PARTY_JOIN (有响应包 MainCmd=4, SubCmd=0x02)
    std::int16_t sqlReturn = -1;
    bool bResult = false;
    int nErrorCode = 0;

    PS_PARTY_ADDMEMBER psAddMember;
    xPacket >> psAddMember;

    unsigned int dwRecruitID = 0;
    xPacket.XParse >> dwRecruitID;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&psAddMember.dwPartyID, 1);
    sqlReturn = xDBBinder.SetData(&psAddMember.stMember.dwMemberID, 1);
    sqlReturn = xDBBinder.SetData(&psAddMember.stMember.nChannel, 4);  // output
    sqlReturn = xDBBinder.SetData(&psAddMember.nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_JOIN(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_JOIN ] [%d error] - Failed query( %d )", sqlReturn, 149);
    } else {
        xDBBinder.Close();
        bResult = true;
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x02);
    xSendDBPacket << psAddMember;
    xSendDBPacket.XParse << dwRecruitID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqLeaveParty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14008F770: SP_PARTY_LEAVE (有响应包 MainCmd=4, SubCmd=0x03)
    std::int16_t sqlReturn = -1;

    PS_PARTY_LEAVE stPartyLeave;
    xPacket >> stPartyLeave;

    unsigned int dwNewMaster = 0;
    xPacket.XParse >> dwNewMaster;

    XDBBinder xDBBinder(pDBStmt);
    int nErrorCode = 0;

    sqlReturn = xDBBinder.SetData(&stPartyLeave.dwPartyID, 1);
    sqlReturn = xDBBinder.SetData(&stPartyLeave.dwLeaveMember, 1);
    sqlReturn = xDBBinder.SetData(&dwNewMaster, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_LEAVE(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_LEAVE ] [%d error] - Failed query( %d )", sqlReturn, 192);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x03);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stPartyLeave;
    xSendDBPacket.XParse << dwNewMaster;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqUpdateMemberInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14008FCA0: SP_PARTY_UPDATEMEMBER (有响应包 MainCmd=4, SubCmd=0x04)
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    ST_UPDATE_PARTY_MEMBER stInfo;
    xPacket >> stInfo;

    XDBBinder xDBBinder(pDBStmt);
    std::int16_t sStatus = static_cast<std::int16_t>(stInfo.stPartyMember.bLogin);

    sqlReturn = xDBBinder.SetData(&stInfo.stPartyMember.dwMemberID, 1);
    sqlReturn = xDBBinder.SetData(&sStatus, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_UPDATEMEMBER(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_PARTY_UPDATEMEMBER ] [%d error] - Failed query( %d )", sqlReturn, 291);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x04);
    xSendDBPacket << stInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyChangeMaster(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14008FEE0: SP_PARTY_CHANGEMASTER (有响应包 MainCmd=4, SubCmd=0x05)
    std::int16_t sqlReturn = -1;

    PS_PARTY_CHANGE_MASTER stChangeMaster;
    xPacket >> stChangeMaster;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stChangeMaster.dwPartyID, 1);
    xDBBinder.SetData(&stChangeMaster.dwNewMasterID, 1);
    xDBBinder.SetData(&stChangeMaster.nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_CHANGEMASTER(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        stChangeMaster.nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_CHANGEMASTER ] [%d error] - Failed query( %d )", sqlReturn, 326);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x05);
    xSendDBPacket << stChangeMaster;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400900D0: 调用 PartyDelete 辅助方法
    PS_PARTY_LEAVE stPartyLeave;
    xPacket >> stPartyLeave;
    return PartyDelete(pDBStmt, xReturnSessionID, &stPartyLeave);
}

std::int32_t XSQLPartyProcess::PartyDelete(XDBStmt* pDBStmt, int xReturnSessionID, PS_PARTY_LEAVE* stPartyLeave) {
    // Per IDA 0x14008FB00: SP_PARTY_DELETE (有响应包 MainCmd=4, SubCmd=0x06)
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stPartyLeave->dwPartyID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_DELETE(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 254);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x06);
    xSendDBPacket << *stPartyLeave;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140090140: SP_PARTY_UPDATEINFO (有响应包 MainCmd=4, SubCmd=0x08)
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    int nPartyID = 0;
    std::int64_t nInsID = 0;
    xPacket.XParse >> nPartyID;
    xPacket.XParse >> nInsID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nPartyID, 1);
    sqlReturn = xDBBinder.SetData(&nInsID, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_UPDATEINFO( ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_UPDATEINFO ] [%d error] - Failed query( %d )", sqlReturn, 378);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x08);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << nPartyID;
    xSendDBPacket.XParse << nInsID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyLoginMember(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140090390: SP_PARTY_LOGIN_MEMBER (有响应包 MainCmd=4, SubCmd=0x09)
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    unsigned int dwServerID = 0;
    xPacket.XParse >> dwServerID;

    PS_REQ_PARTY_LOGIN_MEMBER stReqLogin;
    xPacket >> stReqLogin;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&stReqLogin.dwMemberID, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);  // output
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_LOGIN_MEMBER( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_LOGIN_MEMBER ] [%d error] - Failed query( %d )", sqlReturn, 423);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x09);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stReqLogin;
    xSendDBPacket.XParse << dwServerID;
    Send(xSendDBPacket);

    LogHelper::LogDebug("game.contents", "<%d PARTY> Login %d Member", stReqLogin.dwPartyID, stReqLogin.dwMemberID);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyLoadAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140090620: 加载所有队伍信息及其成员
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    PS_PARTY_INFO_ALL stPartyInfoAll;
    PS_PARTY_INFO_ALL stPartySend;

    // Step 1: 调用 SP_PARTY_LOAD_ALL 获取所有队伍基本信息
    XDBBinder xDBBinder(pDBStmt);
    PS_PARTY_INFO stPartyInfo;

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_PARTY_LOAD_ALL }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_PARTY_LOAD_ALL ] [%d error] - Failed query( %d )", sqlReturn, 469);
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x11);
        xSendDBPacket.XParse << nErrorCode;
        xSendDBPacket << stPartyInfo;
        Send(xSendDBPacket);
        return nErrorCode;
    }

    // 读取所有队伍基本信息
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&stPartyInfo.dwPartyID);
        xDBBinder.GetData(&stPartyInfo.dwMaster);
        xDBBinder.GetData(reinterpret_cast<std::int64_t*>(&stPartyInfo.uxMazeID));
        xDBBinder.GetData(&stPartyInfo.byPartyType);
        stPartyInfoAll.vecPartyInfo.push_back(stPartyInfo);
    }
    xDBBinder.Close();

    // Step 2: 对每个队伍加载成员信息
    for (auto& partyInfo : stPartyInfoAll.vecPartyInfo) {
        XDBBinder xDBBinderMember(pDBStmt);
        sqlReturn = xDBBinderMember.SetData(&partyInfo.dwPartyID, 1);
        sqlReturn = xDBBinderMember.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_PARTY_LOAD_ALL_WITH_MEMBER ( ? ) }")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_PARTY_LOAD_ALL_WITH_MEMBER ] [%d error] - Failed query( %d )", sqlReturn, 527);
            if (sqlReturn != 100) {
                xDBBinderMember.Close();
            }
            nErrorCode = -1;
            XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x11);
            xSendDBPacket.XParse << nErrorCode;
            xSendDBPacket << partyInfo;
            Send(xSendDBPacket);
            return nErrorCode;
        }

        // 读取成员信息
        std::int16_t sStatus = 0;
        while ((xDBBinderMember.Fetch() & 0xFFFFFFFE) == 0) {
            ST_PARTY_MEMBER stMemberInfo;
            xDBBinderMember.GetData(&stMemberInfo.dwMemberID);
            xDBBinderMember.GetWString(stMemberInfo.strName, 42);
            xDBBinderMember.GetData(&stMemberInfo.byLevel);
            xDBBinderMember.GetData(&stMemberInfo.byClass);
            xDBBinderMember.GetData(&stMemberInfo.nMapID);
            xDBBinderMember.GetData(&stMemberInfo.nChannel);
            xDBBinderMember.GetData(&sStatus);
            xDBBinderMember.GetData(&stMemberInfo.byAwaken);
            xDBBinderMember.GetData(&stMemberInfo.dwProfilePhotoID);
            partyInfo.vecPartyMember.push_back(stMemberInfo);
        }
        xDBBinderMember.Close();

        // 添加到发送列表
        stPartySend.vecPartyInfo.push_back(partyInfo);

        // 每100个队伍发送一次
        if (stPartySend.vecPartyInfo.size() == 100) {
            XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x11);
            xSendDBPacket.XParse << nErrorCode;
            xSendDBPacket.XParse << static_cast<std::int8_t>(0);  // bLast = false
            xSendDBPacket << stPartySend;
            Send(xSendDBPacket);
            stPartySend.vecPartyInfo.clear();
        }
    }

    // Step 3: 发送剩余的队伍（最后一批）
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x11);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << static_cast<std::int8_t>(1);  // bLast = true
    xSendDBPacket << stPartySend;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyMatchingCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140090E20: SP_PARTY_MATCHING_CREATE (有响应包 MainCmd=4, SubCmd=0x13)
    std::int16_t sqlReturn = -1;

    unsigned int dwMatchingID = 0;
    unsigned int dwLeaderID = 0;
    unsigned int dwMember[4] = {0, 0, 0, 0};

    xPacket.XParse >> dwMatchingID;
    xPacket.XParse >> dwLeaderID;
    for (int i = 0; i < 4; ++i) {
        xPacket.XParse >> dwMember[i];
    }

    unsigned int dwPartyID = 0;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwLeaderID, 1);
    for (int j = 0; j < 4; ++j) {
        xDBBinder.SetData(&dwMember[j], 1);
    }
    xDBBinder.SetData(&dwPartyID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_MATCHING_CREATE(?,?,?,?,? ,?,?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_PARTY_MATCHING_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 587);
        nErrorCode = -1;
    }

    xDBBinder.Close();

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 4, 0x13);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << dwMatchingID;
    xSendDBPacket.XParse << dwPartyID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPartyProcess::ReqPartyTypeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400910D0: SP_PARTY_TYPE_UPDATE (无响应包)
    std::int16_t sqlReturn = -1;
    unsigned int dwPartyID = 0;
    xPacket.XParse >> dwPartyID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwPartyID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_TYPE_UPDATE( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_PARTY_TYPE_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 616);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLPartyProcess::LoadPartyID(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t& dwPartyID) {
    // Per IDA 0x14008F9D0: SP_PARTY_LOAD_ID (无响应包，辅助方�?
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&dwPartyID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_LOAD_ID( ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_PARTY_LOAD_ID ] [%d error] - Failed query( %d )", sqlReturn, 229);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

XSQLFriendProcess::XSQLFriendProcess() : TXSQLProcessBase<0x05>("XSQLFriendProcess") {}
XSQLFriendProcess::~XSQLFriendProcess() = default;

std::int32_t XSQLFriendProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    switch (xPacket.GetSubCmd()) {
    case 0x01:
        return ReqFriendLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x02:
        return ReqInviteFriend(pDBStmt, xPacket, xReturnSessionID);
    case 0x03:
        return ReqInviteCheckFriend(pDBStmt, xPacket, xReturnSessionID);
    case 0x04:
        return ReqDeleteFriend(pDBStmt, xPacket, xReturnSessionID);
    case 0x05:
        return ReqAcceptFriend(pDBStmt, xPacket, xReturnSessionID);
    case 0x06:
        return ReqAddBlockList(pDBStmt, xPacket, xReturnSessionID);
    case 0x07:
        return ReqDelBlockList(pDBStmt, xPacket, xReturnSessionID);
    case 0x08:
        return ReqRecruitList(pDBStmt, xPacket, xReturnSessionID);
    case 0x09:
        return ReqRecruitAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x10:
        return ReqRecruitDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x11:
        return ReqFriendFind(pDBStmt, xPacket, xReturnSessionID);
    default:
        return -1;
    }
}

std::int32_t XSQLFriendProcess::ReqFriendLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400446B0: SP_FRIEND_LOAD (有响应包 MainCmd=5, SubCmd=1)
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

            // 好友数量已满，需要选择一个好友删除
            PS_DB_FRIEND_LIST stFriendList;
    PS_BLOCKLIST_INFO stBlockList;
    std::vector<PS_DB_FRIEND> vecDelete;

    stFriendList.dwActorID = dwUCID;

    sqlReturn = LoadFriend(pDBStmt, dwUCID, stFriendList, vecDelete);
    if (sqlReturn && sqlReturn != 1) {
        nErrorCode = sqlReturn;
    }

    if (!vecDelete.empty()) {
        DeleteFriend(pDBStmt, stFriendList.dwActorID, vecDelete);
    }

    sqlReturn = LoadFriendBlock(pDBStmt, dwUAID, stBlockList);
    if (sqlReturn && sqlReturn != 1) {
        nErrorCode = sqlReturn;
    }

    ST_CHAR_COMMUNITY stCharCommunity;
    sqlReturn = LoadCharCommunity(pDBStmt, dwUCID, stCharCommunity);
    if (sqlReturn && sqlReturn != 1) {
        nErrorCode = sqlReturn;
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 1);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stFriendList;
    xSendDBPacket << stBlockList;
    xSendDBPacket << stCharCommunity;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqInviteFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400449C0: SP_FRIEND_ADD (有响应包 MainCmd=5, SubCmd=2)
    PS_DB_FRIEND_INVITE stInvite;
    xPacket >> stInvite;

    std::int16_t sqlReturn = 0;

    // 如果有删除请求，先删除好友
    if (stInvite.stDeleteReq.dwFriendID != 0) {
        std::uint32_t dwReqUAID = 0;
        std::uint32_t dwFriendUAID = 0;
        DeleteFriend(pDBStmt, stInvite.stDeleteReq.dwReqID, stInvite.stDeleteReq.dwFriendID,
                     dwReqUAID, dwFriendUAID);
    }

    if (stInvite.stDeleteTarget.dwFriendID != 0) {
        std::uint32_t dwUAID = 0;
        std::uint32_t dwFriendUAID = 0;
        DeleteFriend(pDBStmt, stInvite.stDeleteTarget.dwReqID, stInvite.stDeleteTarget.dwFriendID,
                     dwUAID, dwFriendUAID);
    }

    // 添加好友
    ST_FRIEND_INFO stFriend;
    sqlReturn = AddFriend(pDBStmt, stInvite.stReq.dwUCID, stInvite.stTarget.dwUCID,
                          stInvite.stReq.byType, stInvite.stTarget.byType,
                          stFriend, stInvite.stTarget.dwUAID);

    if (sqlReturn != -1) {
        PS_RES_DB_FRIEND_INVITE stRes;
        stRes.stInvite = stInvite;
        stRes.stTargetInfo.dwID = stFriend.dwID;
        wcscpy_s(stRes.stTargetInfo.strName, 21, stFriend.strName);
        stRes.stTargetInfo.byLevel = stFriend.byLevel;
        stRes.stTargetInfo.byClass = stFriend.byClass;
        stRes.stTargetInfo.byAwaken = stFriend.byAwaken;
        stRes.stTargetInfo.dwProfilePhotoID = stFriend.dwProfilePhotoID;
        stRes.stTargetInfo.byState = stFriend.byState;

        XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 2);
        xSendDBPacket << stRes;
        Send(xSendDBPacket);
    }

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqInviteCheckFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140044CA0: SP_CHECKCHARACTERNAME/SP_FRIEND_TYPE_COUNT (有响应包 MainCmd=5, SubCmd=3)
    PS_RES_FRIEND_INVITE stInvite;
    xPacket >> stInvite;

    std::int16_t sqlReturn = 0;
    std::uint32_t dwDelUCID = 0;

    if (CheckFriendName(pDBStmt, stInvite.strTargetUserName, stInvite.dwTargetUCID)) {
        if (!CheckFriendCount(pDBStmt, stInvite.dwTargetUCID, 2, 20)) {
            // 好友数量已满，需要选择一个好友删除
            PS_DB_FRIEND_LIST stFriendList;
            std::vector<PS_DB_FRIEND> vecDelete;
            stFriendList.dwActorID = stInvite.dwTargetUCID;

            LoadFriend(pDBStmt, stInvite.dwTargetUCID, stFriendList, vecDelete);

            if (vecDelete.empty()) {
                // 从现有好友中选择
                std::int64_t nMinFriendPoint = 0;
                for (size_t j = 0; j < stFriendList.vecFriend.size(); ++j) {
                    const PS_DB_FRIEND& stFriend = stFriendList.vecFriend[j];
                    if (dwDelUCID == 0) {
                        dwDelUCID = stFriend.dwUCID;
                        nMinFriendPoint = stFriend.nFriendPoint;
                    } else {
                        if (nMinFriendPoint > stFriend.nFriendPoint) {
                            dwDelUCID = stFriend.dwUCID;
                            nMinFriendPoint = stFriend.nFriendPoint;
                        }
                    }
                }
            } else {
                // 从待删除列表中选择
                std::int64_t nMinRemain = 0;
                for (size_t i = 0; i < vecDelete.size(); ++i) {
                    const PS_DB_FRIEND& stFriend = vecDelete[i];
                    if (dwDelUCID == 0) {
                        dwDelUCID = stFriend.dwUCID;
                        nMinRemain = stFriend.tRemain;
                    } else {
                        if (nMinRemain > stFriend.tRemain) {
                            dwDelUCID = stFriend.dwUCID;
                            nMinRemain = stFriend.tRemain;
                        }
                    }
                }
            }
        }
        stInvite.byResult = 0;
    } else {
        stInvite.byResult = 1;  // 用户名不存在
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 3);
    xSendDBPacket << stInvite;
    xSendDBPacket.XParse << dwDelUCID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqDeleteFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140045080: SP_FRIEND_DELETE (有响应包 MainCmd=5, SubCmd=4)
    PS_REQ_FRIEND_DELETE stDelete;
    xPacket >> stDelete;

    PS_DB_FRIEND_DELETE stResult;
    stResult.dwReqUCID = stDelete.dwReqID;
    stResult.dwFriendUCID = stDelete.dwFriendID;
    stResult.nResult = 0;

    if (DeleteFriend(pDBStmt, stDelete.dwReqID, stDelete.dwFriendID,
                     stResult.dwReqUAID, stResult.dwFriendUAID) != 0) {
        stResult.nResult = 55109;  // 错误码
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 4);
    xSendDBPacket << stResult;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLFriendProcess::DeleteFriend(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint32_t dwFriendID,
                                              std::uint32_t& dwUAID, std::uint32_t& dwFriendUAID) {
    // Per IDA 0x140047120: SP_FRIEND_DELETE 辅助方法
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwID, 1);
    xDBBinder.SetData(&dwFriendID, 1);
    xDBBinder.SetData(&dwUAID, 4);  // output
    xDBBinder.SetData(&dwFriendUAID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_DELETE(?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FRIEND_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 733);
    } else {
        xDBBinder.Close();
    }

    return nErrorCode;
}

std::int32_t XSQLFriendProcess::DeleteFriend(XDBStmt* pDBStmt, std::uint32_t dwID, std::vector<PS_DB_FRIEND>& vecDelete) {
    // Per IDA 0x140047060: 批量删除好友
    std::int16_t sqlReturn = 0;

    for (size_t i = 0; i < vecDelete.size(); ++i) {
        std::uint32_t dwReqUAID = 0;
        std::uint32_t dwFriendUAID = 0;

        if (DeleteFriend(pDBStmt, dwID, vecDelete[i].dwUCID, dwReqUAID, dwFriendUAID) == -1) {
            sqlReturn = -1;
        }
    }

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqAcceptFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400451E0: SP_FRIEND_ADD_CHECK/SP_FRIEND_UPDATE (有响应包 MainCmd=5, SubCmd=5)
    PS_DB_FRIEND_ACCEPT_REQ stAccept;
    xPacket >> stAccept;

    PS_DB_FRIEND_ACCEPT_RES stResult;
    stResult.stReq = stAccept.stReq;
    stResult.stTarget = stAccept.stTarget;
    stResult.nResult = 0;

    std::int16_t sqlReturn = 0;

    if (CheckAddFriend(pDBStmt, stResult.stTarget.dwUCID, stResult.stReq.dwUCID, stResult)) {
        stResult.nResult = UpdateFriend(pDBStmt, stResult.stReq.dwUCID, stResult.stTargetInfo.dwID,
                                        stResult.stTarget.byType, stResult.stReq.byType,
                                        stResult.stTarget.dwUAID);
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 5);
    xSendDBPacket << stResult;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqAddBlockList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400453E0: SP_BLOCKLIST_ADD (有响应包 MainCmd=5, SubCmd=6)
    PS_REQ_FRIEND_BLOCK_ADD stBlock;
    PS_REQ_FRIEND_DELETE stDelete;  // 可选的好友删除请求
    xPacket >> stBlock;
    xPacket >> stDelete;

    // 如果有好友ID，先删除好友
    if (stDelete.dwFriendID != 0) {
        PS_DB_FRIEND_DELETE psDelFriend;
        psDelFriend.nResult = 0;
        psDelFriend.dwReqUCID = stDelete.dwReqID;
        psDelFriend.dwFriendUCID = stDelete.dwFriendID;

        if (DeleteFriend(pDBStmt, stDelete.dwReqID, stDelete.dwFriendID,
                         psDelFriend.dwReqUAID, psDelFriend.dwFriendUAID) == 0) {
            // 发送好友删除响应包
            XSendDBPacket v12(xReturnSessionID, 5, 4);
            v12 << psDelFriend;
            Send(v12);
        }
    }

    // 添加黑名�
    PS_RES_DB_FRIEND_BLOCK stResult;
    stResult.dwReqUAID = stBlock.dwReqUCID;  // 使用 dwReqUCID 作为 UAID

    int nError = AddBlockList(pDBStmt, &stBlock, &stResult.stBlock);
    if (nError) {
        stResult.nResult = 55109;
        if (nError == 2) {
            stResult.nResult = 55117;
        }
    } else {
        stResult.nResult = 0;
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 6);
    xSendDBPacket << stResult;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLFriendProcess::ReqDelBlockList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140045630: SP_BLOCKLIST_DELETE (有响应包 MainCmd=5, SubCmd=7)
    PS_REQ_FRIEND_BLOCK_DELETE stBlock;
    xPacket >> stBlock;

    PS_RES_BLOCKLIST_DELETE psResult;
    psResult.nResult = 0;
    psResult.dwReqUAID = stBlock.dwReqUCID;  // 使用 dwReqUCID 作为 UAID
    wcscpy_s(psResult.strTargetName, 21, stBlock.strTargetName);

    if (DeleteBlockList(pDBStmt, &stBlock, psResult.dwTargetUCID)) {
        psResult.nResult = 0;
    } else {
        psResult.nResult = 55109;
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 7);
    xSendDBPacket << psResult;
    Send(xSendDBPacket);

    return 0;
}

// Helper methods
std::int32_t XSQLFriendProcess::AddBlockList(XDBStmt* pDBStmt, PS_REQ_FRIEND_BLOCK_ADD* stBlock, DB_BLOCK_INFO* stBlockInfo) {
    // Per IDA 0x140047FB0: SP_BLOCKLIST_ADD 辅助方法
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;
    int nError = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stBlock->dwReqUCID, 1);
    xDBBinder.SetWString(stBlock->strTargetName, 21, &cbTID, 1);
    xDBBinder.SetData(&nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_BLOCKLIST_ADD(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_BLOCKLIST_ADD ] [%d error] - Failed query( %d )", sqlReturn, 945);
        xDBBinder.Close();
        return 0;
    } else {
        // 读取结果�
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&stBlockInfo->dwUCID);
            xDBBinder.GetData(&stBlockInfo->byLevel);
            short outLen = 0;
            xDBBinder.GetWString(stBlockInfo->strName, 21, outLen);
        }
        xDBBinder.Close();
        if (nError) {
            LogHelper::LogError("game.contents", "[ SP_BLOCKLIST_ADD ] [%d error] - Add Failed( %d )", nError, 952);
        }
        return nError;
    }
}

bool XSQLFriendProcess::DeleteBlockList(XDBStmt* pDBStmt, PS_REQ_FRIEND_BLOCK_DELETE* stBlock, std::uint32_t& dwTargetUCID) {
    // Per IDA 0x140048190: SP_BLOCKLIST_DELETE 辅助方法
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;
    dwTargetUCID = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stBlock->dwReqUCID, 1);
    xDBBinder.SetWString(stBlock->strTargetName, 21, &cbTID, 1);
    xDBBinder.SetData(&dwTargetUCID, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_BLOCKLIST_DELETE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_BLOCKLIST_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 981);
        return false;
    } else {
        xDBBinder.Close();
        return dwTargetUCID != 0;
    }
}

std::int32_t XSQLFriendProcess::LoadCharCommunity(XDBStmt* pDBStmt, std::uint32_t dwActorID, ST_CHAR_COMMUNITY& stCharCommunity) {
    // Per IDA 0x140046F00: SP_CHARACTER_COMMUNITY_SELECT
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHARACTER_COMMUNITY_SELECT( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_COMMUNITY_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 683);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&stCharCommunity.byState);
            short outLen = 0;
            xDBBinder.GetWString(stCharCommunity.szComment, 51, outLen);
            xDBBinder.GetWString(stCharCommunity.szMemo, 31, outLen);
        }
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::LoadFriend(XDBStmt* pDBStmt, std::uint32_t dwActorID, PS_DB_FRIEND_LIST& stFriendList,
                                            std::vector<PS_DB_FRIEND>& vecDelete) {
    // Per IDA 0x140046640: SP_FRIEND_LOAD
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_LOAD( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_FRIEND_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 617);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            PS_DB_FRIEND stFriend;
            char szLogOut[25] = {0};
            char szRemain[25] = {0};
            std::int32_t nFriendPoint = 0;

            xDBBinder.GetData(&stFriend.dwUCID);
            short outLen = 0;
            xDBBinder.GetWString(stFriend.strName, 21, outLen);
            xDBBinder.GetData(&stFriend.byLevel);
            xDBBinder.GetData(&stFriend.byClass);
            xDBBinder.GetData(&stFriend.byState);
            xDBBinder.GetWString(stFriend.strMemo, 31, outLen);
            xDBBinder.GetString(szLogOut, 24);
            xDBBinder.GetData(&stFriend.byType);
            xDBBinder.GetString(szRemain, 24);
            xDBBinder.GetData(&nFriendPoint);
            stFriend.nFriendPoint = nFriendPoint;
            xDBBinder.GetData(&stFriend.byAwaken);
            xDBBinder.GetData(&stFriend.dwProfilePhotoID);

            // TODO: 时间字符串解�
            stFriend.tLogOut = 0;
            stFriend.tRemain = 0;

            if (stFriend.dwUCID) {
                // TODO: 检查是否需要删除过期好�
                stFriendList.vecFriend.push_back(stFriend);
            }
        }
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::LoadFriendBlock(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_BLOCKLIST_INFO& stBlockList) {
    // Per IDA 0x140046D70: SP_BLOCKLIST_LOAD
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_BLOCKLIST_LOAD( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_BLOCKLIST_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 671);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_BLOCK_INFO stBlock;
            xDBBinder.GetData(&stBlock.dwUCID);
            short outLen = 0;
            xDBBinder.GetWString(stBlock.strName, 21, outLen);
            xDBBinder.GetData(&stBlock.byLevel);
            stBlockList.vecBlockList.push_back(stBlock);
        }
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::AddFriend(XDBStmt* pDBStmt, std::uint32_t dwReqID, std::uint32_t dwTargetID,
                                          std::uint8_t byReqType, std::uint8_t byTargetType,
                                          ST_FRIEND_INFO& stFriend, std::uint32_t& dwTargetUAID) {
    // Per IDA 0x140047910: SP_FRIEND_ADD 辅助方法
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;
    std::int64_t cbTID = -3;
    char szRemainTime[25] = "2000-01-01 00:00:00";  // 默认时间

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwReqID, 1);
    xDBBinder.SetData(&byReqType, 1);
    xDBBinder.SetData(&dwTargetID, 1);
    xDBBinder.SetData(&byTargetType, 1);
    xDBBinder.SetString(szRemainTime, 24, &cbTID, 1);
    xDBBinder.SetData(&dwTargetUAID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_ADD(?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FRIEND_ADD ] [%d error] - Failed query( %d )", sqlReturn, 897);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            char szLogOut[25] = {0};
            xDBBinder.GetData(&stFriend.dwID);
            short outLen = 0;
            xDBBinder.GetWString(stFriend.strName, 21, outLen);
            xDBBinder.GetData(&stFriend.byLevel);
            xDBBinder.GetData(&stFriend.byClass);
            xDBBinder.GetData(&stFriend.byState);
            xDBBinder.GetWString(stFriend.strMemo, 31, outLen);
            xDBBinder.GetString(szLogOut, 24);
            xDBBinder.GetData(&stFriend.byAwaken);
            xDBBinder.GetData(&stFriend.dwProfilePhotoID);
            stFriend.byType = byReqType;
            stFriend.tLogOut = 0;  // TODO: 时间解析
            stFriend.tRemain = 0;
        }
        xDBBinder.Close();
    }

    if (nErrorCode) {
        sqlReturn = -1;
        LogHelper::LogError("game.contents", "[ SP_FRIEND_ADD ] [%d error] - Failed query( %d )", -1, 903);
    }

    return sqlReturn;
}

bool XSQLFriendProcess::CheckFriendName(XDBStmt* pDBStmt, wchar_t* strName, std::uint32_t& dwUCID) {
    // Per IDA 0x140048660: SP_CHECKCHARACTERNAME 辅助方法
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetWString(strName, 21, &cbTID, 1);
    xDBBinder.SetData(&dwUCID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call  SP_CHECKCHARACTERNAME(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHECKCHARACTERNAME ] [%d error] - Failed query( %d )", sqlReturn, 1084);
    } else {
        xDBBinder.Close();
    }

    return nErrorCode == 1;
}

bool XSQLFriendProcess::CheckFriendCount(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int16_t shType, int iMax) {
    // Per IDA 0x1400487D0: SP_FRIEND_TYPE_COUNT 辅助方法
    std::int16_t sqlReturn = 0;
    int iCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&shType, 1);
    xDBBinder.SetData(&iCount, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_TYPE_COUNT(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        iCount = iMax;  // 出错时设置为大值，阻止添加
        LogHelper::LogError("game.contents", "[ SP_FRIEND_TYPE_COUNT ] [%d error] - Failed query( %d )", sqlReturn, 1117);
    } else {
        xDBBinder.Close();
    }

    return iMax > iCount;
}

bool XSQLFriendProcess::CheckAddFriend(XDBStmt* pDBStmt, std::uint32_t dwTargetID, std::uint32_t dwReqID, PS_DB_FRIEND_ACCEPT_RES& stResult) {
    // Per IDA 0x140047280: SP_FRIEND_ADD_CHECK 辅助方法
    std::int16_t sqlReturn = 0;
    int nFriendCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    stResult.nResult = 0;
    xDBBinder.SetData(&dwTargetID, 1);
    xDBBinder.SetData(&dwReqID, 1);
    xDBBinder.SetData(&nFriendCount, 4);  // output
    xDBBinder.SetData(&stResult.nResult, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_ADD_CHECK(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        stResult.nResult = -1;
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_FRIEND_ADD_CHECK ] [%d error] - Failed query( %d )", sqlReturn, 800);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            char szLogOut[25] = {0};
            short outLen = 0;
            xDBBinder.GetWString(stResult.stTargetInfo.strName, 21, outLen);
            xDBBinder.GetData(&stResult.stTargetInfo.byLevel);
            xDBBinder.GetData(&stResult.stTargetInfo.byClass);
            xDBBinder.GetData(&stResult.stTargetInfo.byState);
            xDBBinder.GetWString(stResult.stTargetInfo.strMemo, 31, outLen);
            xDBBinder.GetString(szLogOut, 24);
            xDBBinder.GetData(&stResult.stTargetInfo.byAwaken);
            xDBBinder.GetData(&stResult.stTargetInfo.dwProfilePhotoID);
            stResult.stTargetInfo.tLogOut = 0;  // TODO: 时间解析
            stResult.stTargetInfo.dwID = dwTargetID;
            stResult.stTargetInfo.byType = 0;
            stResult.stTargetInfo.tRemain = 0;
        }
        xDBBinder.Close();

        if (stResult.nResult) {
            stResult.nResult = 55105;
        } else if (nFriendCount >= 100) {
            stResult.nResult = 55103;
        }
    }

    return stResult.nResult == 0;
}

std::int32_t XSQLFriendProcess::UpdateFriend(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint32_t dwFriendID,
                                              std::uint8_t byReqType, std::uint8_t byTargetType, std::uint32_t& dwFriendUAID) {
    // Per IDA 0x140047790: SP_FRIEND_UPDATE 辅助方法
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwID, 1);
    xDBBinder.SetData(&byReqType, 1);
    xDBBinder.SetData(&dwFriendID, 1);
    xDBBinder.SetData(&byTargetType, 1);
    xDBBinder.SetData(&dwFriendUAID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_UPDATE(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FRIEND_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 837);
    } else {
        xDBBinder.Close();
    }

    return nErrorCode;
}

bool XSQLFriendProcess::AddRecruit(XDBStmt* pDBStmt, std::uint32_t dwID, std::int64_t tAddTime) {
    // Per IDA 0x140045490: SP_RECRUIT_ADD helper
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwID, 1);
    xDBBinder.SetData(&tAddTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_RECRUIT_ADD( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_RECRUIT_ADD ] [%d error] - Failed query( %d )", sqlReturn, 1001);
        return false;
    } else {
        xDBBinder.Close();
        return true;
    }
}

bool XSQLFriendProcess::DeleteRecruit(XDBStmt* pDBStmt, std::uint32_t dwID) {
    // Per IDA 0x1400456E0: SP_RECRUIT_DELETE helper
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_RECRUIT_DELETE( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_RECRUIT_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 1013);
        return false;
    } else {
        xDBBinder.Close();
        return true;
    }
}

std::int32_t XSQLFriendProcess::ReqRecruitList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140045780: SP_RECRUIT_LOAD (有响应包 MainCmd=5, SubCmd=8)
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_RECRUIT_LOAD( ? )}")));

    ST_RECRUIT_LIST stList;
    std::vector<std::uint32_t> vecOldRecruit;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_RECRUIT_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 413);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_RECRUIT_INFO stInfo;
            char szLogOut[32] = {0};
            char szAddTime[32] = {0};

            short outLen = 0;
            xDBBinder.GetWString(stInfo.strName, 21, outLen);
            xDBBinder.GetData(&stInfo.dwID);
            xDBBinder.GetData(&stInfo.byLevel);
            xDBBinder.GetData(&stInfo.byClass);
            xDBBinder.GetData(&stInfo.byState);
            xDBBinder.GetWString(stInfo.strMemo, 31, outLen);
            xDBBinder.GetString(szLogOut, 24);
            xDBBinder.GetString(szAddTime, 24);
            xDBBinder.GetData(&stInfo.byAwaken);
            xDBBinder.GetData(&stInfo.dwProfilePhotoID);

            // 解析登出时间字符�
            int _year = 2000, _mon = 1, _day = 1, _hour = 0, _min = 0, _sec = 0;
            sscanf_s(szLogOut, "%d-%d-%d %d:%d:%d", &_year, &_mon, &_day, &_hour, &_min, &_sec);
            if (_year >= 2000 && _year <= 2040 && _mon >= 1 && _mon <= 12 && _day >= 1 && _day <= 31 &&
                _hour >= 0 && _hour <= 23 && _min >= 0 && _min <= 59 && _sec >= 0 && _sec <= 59) {
                // 使用简化的 time_t 计算
                struct tm t = {};
                t.tm_year = _year - 1900;
                t.tm_mon = _mon - 1;
                t.tm_mday = _day;
                t.tm_hour = _hour;
                t.tm_min = _min;
                t.tm_sec = _sec;
                stInfo.tLogOut = static_cast<std::int64_t>(std::mktime(&t));
            } else {
                stInfo.tLogOut = 0;
            }

            // 解析添加时间字符�
            int nYear = 2000, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
            sscanf_s(szAddTime, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
            if (nYear >= 2000 && nYear <= 2040 && nMonth >= 1 && nMonth <= 12 && nDay >= 1 && nDay <= 31 &&
                nHour >= 0 && nHour <= 23 && nMin >= 0 && nMin <= 59 && nSec >= 0 && nSec <= 59) {
                struct tm t = {};
                t.tm_year = nYear - 1900;
                t.tm_mon = nMonth - 1;
                t.tm_mday = nDay;
                t.tm_hour = nHour;
                t.tm_min = nMin;
                t.tm_sec = nSec;
                stInfo.tAddTime = static_cast<std::int64_t>(std::mktime(&t));
            } else {
                stInfo.tAddTime = 0;
            }

            // 获取当前时间
            std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));

            // 检查是否超�?小时 (3600�?
            if (stInfo.tAddTime + 3600 > tCurr) {
                // 仍在有效期内
                stList.vecRecruit.push_back(stInfo);
            } else {
                // 已过期，加入待删除列�
                vecOldRecruit.push_back(stInfo.dwID);
            }
        }
        xDBBinder.Close();
    }

    // 删除过期的招募记�
    for (std::size_t i = 0; i < vecOldRecruit.size(); ++i) {
        DeleteRecruit(pDBStmt, vecOldRecruit[i]);
    }

    // 分批发送响应包 (每批最�?0�?
    ST_RECRUIT_LIST stResult;
    std::uint8_t byLast = 0;

    for (std::size_t j = 0; j < stList.vecRecruit.size(); ++j) {
        if (stResult.vecRecruit.size() >= 50) {
            // 发送当前批�
            XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 8);
            xSendDBPacket.XParse << byLast;
            xSendDBPacket << stResult;
            Send(xSendDBPacket);
            stResult.vecRecruit.clear();
        }
        stResult.vecRecruit.push_back(stList.vecRecruit[j]);
    }

    // 发送最后一�
    byLast = 1;
    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 8);
    xSendDBPacket.XParse << byLast;
    xSendDBPacket << stResult;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqRecruitAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400460C0: SP_RECRUIT_ADD (有响应包 MainCmd=5, SubCmd=9)
    std::int16_t sqlReturn = 0;
    ST_RECRUIT_INFO stInfo;

    xPacket >> stInfo;

    PS_RES_RECRUIT_ADD stResult;
    stResult.stAdd = stInfo;

    if (AddRecruit(pDBStmt, stInfo.dwID, stInfo.tAddTime)) {
        stResult.nResult = 0;
    } else {
        stResult.nResult = 55109;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 9);
    xSendDBPacket << stResult;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqRecruitDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400462C0: SP_RECRUIT_DELETE (有响应包 MainCmd=5, SubCmd=16)
    std::int16_t sqlReturn = 0;
    PS_RECRUIT_DELETE stDelete;

    xPacket >> stDelete;

    PS_RES_RECRUIT_DELETE stResult;
    stResult.dwUCID = stDelete.dwUCID;

    if (DeleteRecruit(pDBStmt, stDelete.dwUCID)) {
        stResult.nResult = 0;
    } else {
        stResult.nResult = 55109;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 0x10);
    xSendDBPacket << stResult;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLFriendProcess::ReqFriendFind(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140046350: SP_FRIEND_FIND (有响应包 MainCmd=5, SubCmd=17)
    std::int16_t sqlReturn = -1;
    PS_REQ_FRIEND_FIND stFind;

    xPacket >> stFind;

    PS_DB_FRIEND_FIND stInfo;
    stInfo.dwReqUCID = stFind.dwReqUCID;
    stInfo.nResult = 0;
    stInfo.psList.vecList.clear();
    stInfo.psList.bLast = 1;

    XDBBinder xDBBinder(pDBStmt);
    std::int64_t cbTID = -3;  // SQL_NTS
    int nMaxCount = 21;

    xDBBinder.SetWString(stFind.strName, 21, &cbTID, 1);
    xDBBinder.SetData(&nMaxCount, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FRIEND_FIND( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_FRIEND_FIND ] [%d error] - Failed query( %d )", sqlReturn, 536);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_FIND_FRIEND stFriend;
            short outLen = 0;

            xDBBinder.GetData(&stFriend.dwUCID);
            xDBBinder.GetWString(stFriend.strName, 21, outLen);
            xDBBinder.GetData(&stFriend.byLevel);

            // 最多返回20条记录
            if (stInfo.psList.vecList.size() >= 20) {
                stInfo.psList.bLast = 0;
                break;
            }

            stInfo.psList.vecList.push_back(stFriend);
            stInfo.psList.bLast = 1;
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 5, 0x11);
    xSendDBPacket << stInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}


// XSQLPostProcess 实现
XSQLPostProcess::XSQLPostProcess() : TXSQLProcessBase<0x06>("XSQLPostProcess") {}
XSQLPostProcess::~XSQLPostProcess() = default;

std::int32_t XSQLPostProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    switch (xPacket.GetSubCmd()) {
    case 0x00:
        return ReqPostSendList(pDBStmt, xPacket, xReturnSessionID);
    case 0x01:
        return ReqPostRecvList(pDBStmt, xPacket, xReturnSessionID);
    case 0x02:
        return ReqPostSend(pDBStmt, xPacket, xReturnSessionID);
    case 0x03:
        return ReqPostRead(pDBStmt, xPacket, xReturnSessionID);
    case 0x04:
        return ReqPostReceipt(pDBStmt, xPacket, xReturnSessionID);
    case 0x05:
        return ReqPostSendDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x06:
        return ReqPostRecvDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x07:
        return ReqPostSendBack(pDBStmt, xPacket, xReturnSessionID);
    case 0x08:
        return ReqPostSendNameCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x09:
        return ReqPostSystemSend(pDBStmt, xPacket, xReturnSessionID);
    case 0x10:
        return ReqGMTSendPostList(pDBStmt, xPacket, xReturnSessionID);
    case 0x11:
        return ReqGMTSendPostSend(pDBStmt, xPacket, xReturnSessionID);
    case 0x12:
        return ReqPostSave(pDBStmt, xPacket, xReturnSessionID);
    case 0x13:
        return ReqPostSaveList(pDBStmt, xPacket, xReturnSessionID);
    case 0x14:
        return ReqPostAccountList(pDBStmt, xPacket, xReturnSessionID);
    case 0x15:
        return ReqPostAccountRead(pDBStmt, xPacket, xReturnSessionID);
    case 0x16:
        return ReqPostAccountReceipt(pDBStmt, xPacket, xReturnSessionID);
    case 0x17:
        return ReqPostAccountDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x18:
        return ReqPostAccountSend(pDBStmt, xPacket, xReturnSessionID);
    case 0x19:
        return ReqPostDeleteAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x20:
        return ReqPostReceiptAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x21:
        return ReqPostLevelUpEventLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x22:
        return ReqPostLevelUpEventUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x23:
        return ReqPostLevelUpEventReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x24:
        return ReqPostRestoreLoad(pDBStmt, xPacket, xReturnSessionID);
    default:
        return -1;
    }
}

std::int32_t XSQLPostProcess::ReqPostSendList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400930A0: 获取已发送邮件列表
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    unsigned int dwUCID = 0;
    xPacket.XParse >> dwUCID;

    // ST_POST_LIST 包含 vecData 成员
    std::vector<ST_POST_DATA> stSendList;
    std::vector<ST_POST_DATA> stDevideSendList;

    bool bLoadSendList = false;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_SENDLIST( ? ) }   ")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_SENDLIST ] [%d error] - Failed query( %d )", sqlReturn, 142);
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_POST_DATA stData;
            xDBBinder.GetData(&stData.biSerial);
            xDBBinder.GetData(&stData.stCharInfo.dwUCID);
            xDBBinder.GetData(reinterpret_cast<std::uint16_t*>(&stData.stCharInfo.byClass));
            xDBBinder.GetWString(stData.stCharInfo.strName, 42);
            xDBBinder.GetData(&stData.stCharInfo.byAwaken);
            xDBBinder.GetData(&stData.stCharInfo.dwProfilePhotoID);
            xDBBinder.GetData(&stData.byPostType);
            xDBBinder.GetData(&stData.byPostSubType);
            xDBBinder.GetWString(stData.strTitle, 82);
            xDBBinder.GetWString(stData.strMsg, 802);
            xDBBinder.GetData(&stData.biMoney);
            for (int i = 0; i < 5; ++i) {
                xDBBinder.GetData(&stData.stItemList[i].xSerial);
                xDBBinder.GetData(&stData.stItemList[i].nItemID);
                xDBBinder.GetData(&stData.stItemList[i].sCount);
            }
            xDBBinder.GetData(&stData.nRegTime);
            xDBBinder.GetData(&stData.nRemainTime);
            stSendList.push_back(stData);
        }
    }
    xDBBinder.Close();

    // 获取ItemProcess用于物品序列查询
    auto* pClientPtr = GetClientPtr();
    if (!pClientPtr) {
        return -1;
    }

    // TODO: 完整实现需要 XSQLItemProcess::SelectItemSerial / SelectSocketItem / SelectBroachItem / SelectPackageItem
    // 当前简化实现，仅发送列表

    nErrorCode = sqlReturn;
    stDevideSendList.reserve(10);

    for (std::size_t k = 0; k < stSendList.size(); ++k) {
        stDevideSendList.push_back(stSendList[k]);
        if (stDevideSendList.size() >= 10) {
            XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0);
            xSendDBPacket.XParse << nErrorCode;
            xSendDBPacket.XParse << bLoadSendList;
            // 发送列表数据
            xSendDBPacket.XParse << static_cast<std::uint16_t>(stDevideSendList.size());
            for (const auto& post : stDevideSendList) {
                xSendDBPacket << post;
            }
            Send(xSendDBPacket);
            stDevideSendList.clear();
        }
    }

    // 发送剩余数据（最后一批）
    bLoadSendList = true;
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << bLoadSendList;
    xSendDBPacket.XParse << static_cast<std::uint16_t>(stDevideSendList.size());
    for (const auto& post : stDevideSendList) {
        xSendDBPacket << post;
    }
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostRecvList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140093830: 获取已接收邮件列表
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    int nSavePostCount = 0;

    unsigned int dwUCID = 0;
    std::int64_t biCurTime = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biCurTime;

    std::vector<ST_POST_DATA> stRecvList;
    std::vector<ST_POST_DATA> stDevideList;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biCurTime, 1);
    xDBBinder.SetData(&nSavePostCount, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_RECVLIST( ?, ?, ? ) }   ")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_RECVLIST ] [%d error] - Failed query( %d )", sqlReturn, 261);
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_POST_DATA stData;
            xDBBinder.GetData(&stData.biSerial);
            xDBBinder.GetData(&stData.stCharInfo.dwUCID);
            xDBBinder.GetData(reinterpret_cast<std::uint16_t*>(&stData.stCharInfo.byClass));
            xDBBinder.GetWString(stData.stCharInfo.strName, 42);
            xDBBinder.GetData(&stData.stCharInfo.byAwaken);
            xDBBinder.GetData(&stData.stCharInfo.dwProfilePhotoID);
            xDBBinder.GetData(reinterpret_cast<std::uint16_t*>(&stData.byFlag));
            xDBBinder.GetData(&stData.nRemainTime);
            xDBBinder.GetWString(stData.strTitle, 82);
            xDBBinder.GetWString(stData.strMsg, 802);
            xDBBinder.GetData(&stData.biMoney);
            xDBBinder.GetData(&stData.byPostType);
            xDBBinder.GetData(&stData.byPostSubType);
            for (int i = 0; i < 5; ++i) {
                xDBBinder.GetData(&stData.stItemList[i].xSerial);
                xDBBinder.GetData(&stData.stItemList[i].nItemID);
                xDBBinder.GetData(&stData.stItemList[i].sCount);
            }
            xDBBinder.GetData(&stData.nRegTime);
            xDBBinder.GetData(reinterpret_cast<std::int64_t*>(&stData.biEventID));  // IDA中的___u14对应biEventID
            stRecvList.push_back(stData);
        }
    }
    xDBBinder.Close();

    stDevideList.reserve(10);

    auto* pClientPtr = GetClientPtr();
    if (!pClientPtr) {
        return -1;
    }

    // TODO: 完整实现需要 XSQLItemProcess::SelectItemSerial / SelectSocketItem / SelectBroachItem / SelectPackageItem

    for (std::size_t k = 0; k < stRecvList.size(); ++k) {
        stDevideList.push_back(stRecvList[k]);
        if (stDevideList.size() >= 10) {
            XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 1);
            xSendDBPacket.XParse << static_cast<std::int8_t>(0);  // bLast = false
            xSendDBPacket.XParse << static_cast<std::uint16_t>(stDevideList.size());
            for (const auto& post : stDevideList) {
                xSendDBPacket << post;
            }
            xSendDBPacket.XParse << nErrorCode;
            xSendDBPacket.XParse << nSavePostCount;
            Send(xSendDBPacket);
            stDevideList.clear();
        }
    }

    // 发送剩余数据（最后一批）
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 1);
    xSendDBPacket.XParse << static_cast<std::int8_t>(1);  // bLast = true
    xSendDBPacket.XParse << static_cast<std::uint16_t>(stDevideList.size());
    for (const auto& post : stDevideList) {
        xSendDBPacket << post;
    }
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << nSavePostCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140094340: 发送邮件
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    ST_POST_DATA stData;
    ST_POST_CHAR stSendCharData;
    std::int64_t biDecMoney = 0;
    PS_RES_STORAGE_INFO stUpdateList;
    std::int64_t biPostDelSerial = 0;
    std::uint16_t wRecvCount = 0;
    std::int64_t biSendDelete = 0;

    xPacket >> stData;
    xPacket >> stSendCharData;
    xPacket.XParse >> biDecMoney;
    xPacket >> stUpdateList;
    xPacket.XParse >> biPostDelSerial;

    biSendDelete = stData.nRegTime + 2592000;  // 30天后自动删除

    XDBBinder xDBBinder(pDBStmt);
    std::int64_t cbTID = -3;

    xDBBinder.SetData(&stData.biSerial, 1);
    xDBBinder.SetData(&stSendCharData.dwUCID, 1);
    xDBBinder.SetData(&stSendCharData.byClass, 1);
    xDBBinder.SetWString(stSendCharData.strName, 21, &cbTID, 1);
    xDBBinder.SetData(&stSendCharData.byAwaken, 1);
    xDBBinder.SetData(&stSendCharData.dwProfilePhotoID, 1);
    xDBBinder.SetData(&stData.stCharInfo.dwUCID, 1);
    xDBBinder.SetData(&stData.stCharInfo.byClass, 1);
    xDBBinder.SetWString(stData.stCharInfo.strName, 21, &cbTID, 1);
    xDBBinder.SetData(&stData.stCharInfo.byAwaken, 1);
    xDBBinder.SetData(&stData.stCharInfo.dwProfilePhotoID, 1);
    xDBBinder.SetData(&stData.byFlag, 1);
    xDBBinder.SetWString(stData.strTitle, 41, &cbTID, 1);
    xDBBinder.SetWString(stData.strMsg, 401, &cbTID, 1);
    xDBBinder.SetData(&stData.biMoney, 1);
    xDBBinder.SetData(&stData.byPostType, 1);
    xDBBinder.SetData(&stData.byPostSubType, 1);

    for (int i = 0; i < 5; ++i) {
        xDBBinder.SetData(&stData.stItemList[i].xSerial, 1);
        xDBBinder.SetData(&stData.stItemList[i].nItemID, 1);
        xDBBinder.SetData(&stData.stItemList[i].sCount, 1);
        xDBBinder.SetData(&stData.stItemList[i].nAttack, 1);
        xDBBinder.SetData(&stData.stItemList[i].nDefense, 1);
        xDBBinder.SetData(&stData.stItemList[i].nTitleID, 1);
        xDBBinder.SetData(&stData.stItemList[i].nDyeID, 1);
    }

    xDBBinder.SetData(&biPostDelSerial, 1);
    xDBBinder.SetData(&stData.nRegTime, 1);
    xDBBinder.SetData(&wRecvCount, 4);  // output
    xDBBinder.SetData(&stData.nRemainTime, 1);
    xDBBinder.SetData(&biSendDelete, 1);
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
        "{call SP_POST_SEND( ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,? ) }   ")));

    xDBBinder.Close();

    if (sqlReturn != 0 || nErrorCode != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_SEND ] [%d return %d error] - Failed query( %d )", sqlReturn, nErrorCode, 396);
        XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 2);
        xSendDBPacket.XParse << nErrorCode;
        xSendDBPacket << stUpdateList;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    // TODO: 需要实现 UpdateSendItem 函数
    // sqlReturn = UpdateSendItem(pDBStmt, stSendCharData.dwUCID, stUpdateList);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 2);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stUpdateList;
    xSendDBPacket << stData;
    xSendDBPacket << stSendCharData;
    xSendDBPacket.XParse << biDecMoney;
    xSendDBPacket.XParse << biPostDelSerial;
    xSendDBPacket.XParse << wRecvCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostRead(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140094BA0: 阅读邮件
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    std::uint16_t wPostCount = 0;

    unsigned int dwUCID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t byPostFlag = 0;
    bool bDel = false;
    std::int64_t biRemainTime = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> byPostFlag;
    xPacket.XParse >> bDel;
    xPacket.XParse >> biRemainTime;

    sqlReturn = UpdateRead(pDBStmt, dwUCID, biSerial, byPostFlag, bDel, biRemainTime, &wPostCount, &nErrorCode);

    if (nErrorCode != 0 || sqlReturn != 0) {
        nErrorCode = 1;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 3);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket.XParse << byPostFlag;
    xSendDBPacket.XParse << biRemainTime;
    xSendDBPacket.XParse << wPostCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

// UpdateRead helper function
std::int16_t XSQLPostProcess::UpdateRead(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                          std::uint8_t byPostFlag, bool bDel, std::int64_t biRemainTime,
                                          std::uint16_t* wPostCount, int* nErrorCode) {
    std::int16_t sqlReturn = -1;
    *nErrorCode = 0;

    if (bDel) {
        // 删除模式：调用 SP_POST_DEL_TIME
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&dwUCID, 1);
        xDBBinder.SetData(&biSerial, 1);
        xDBBinder.SetData(&byPostFlag, 1);
        xDBBinder.SetData(&biRemainTime, 1);
        xDBBinder.SetData(wPostCount, 4);  // output
        xDBBinder.SetData(nErrorCode, 4);  // output

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_DEL_TIME( ?, ?, ?, ?, ?, ? ) }   ")));
        xDBBinder.Close();

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_POST_DEL_TIME ] [%d error] - Failed query( %d )", sqlReturn, 1430);
        }
    } else {
        // 标记已读模式：调用 SP_POST_CHANGE_FLAG
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&dwUCID, 1);
        xDBBinder.SetData(&biSerial, 1);
        xDBBinder.SetData(&byPostFlag, 1);
        xDBBinder.SetData(wPostCount, 4);  // output
        xDBBinder.SetData(nErrorCode, 4);  // output

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_CHANGE_FLAG( ?, ?, ?, ?, ? ) }   ")));
        xDBBinder.Close();

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_POST_CHANGE_FLAG ] [%d error] - Failed query( %d )", sqlReturn, 1447);
        }
    }

    return sqlReturn;
}

// UpdateReceipt helper function
std::int16_t XSQLPostProcess::UpdateReceipt(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                            std::uint8_t byPostFlag, std::int64_t biRemainTime, int* nError) {
    std::int16_t sqlReturn = -1;
    *nError = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byPostFlag, 1);
    xDBBinder.SetData(&biRemainTime, 1);
    xDBBinder.SetData(nError, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_RECEIPT( ?, ?, ?, ?, ? ) }   ")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_RECEIPT ] [%d error] - Failed query( %d )", sqlReturn, 1471);
    }

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140094E00: 接收邮件（领取附件）
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    std::uint16_t wPostCount = 0;

    unsigned int dwUCID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t byPostFlag = 0;
    std::int64_t biRemainTime = 0;
    std::uint8_t byFlag = 0;
    std::int64_t biMoney = 0;

    PS_RES_STORAGE_INFO stCreateList;
    PS_RES_STORAGE_INFO stUpdateList;
    PS_RES_STORAGE_INFO stUpdateSerial;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> byPostFlag;
    xPacket >> stCreateList;
    xPacket >> stUpdateList;
    xPacket >> stUpdateSerial;
    xPacket.XParse >> biRemainTime;
    // TODO: 需要定义 ST_APPEARANCE_LIST 类型
    // xPacket >> stAppearanceList;
    xPacket.XParse >> byFlag;
    xPacket.XParse >> biMoney;

    sqlReturn = UpdateReceipt(pDBStmt, dwUCID, biSerial, byPostFlag, biRemainTime, &nErrorCode);

    if (sqlReturn != 0 || nErrorCode != 0) {
        nErrorCode = 1;
    } else {
        // TODO: 完整实现需要：
        // - XSQLItemProcess::CheckCreateItem
        // - CreatePostItem
        // - PostRecvItemUpdate
        // - XSQLItemProcess::AppearanceUpdate
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 4);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket.XParse << byPostFlag;
    xSendDBPacket << stCreateList;
    xSendDBPacket << stUpdateList;
    xSendDBPacket << stUpdateSerial;
    xSendDBPacket.XParse << biRemainTime;
    xSendDBPacket.XParse << wPostCount;
    // TODO: stAppearanceList 序列化
    xSendDBPacket.XParse << byFlag;
    xSendDBPacket.XParse << biMoney;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSendDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400955D0: SP_POST_SEND_DELETE（删除已发送邮件）
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    std::int64_t biDeleteTime = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> biDeleteTime;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&nErrorCode, 4);
    xDBBinder.SetData(&biDeleteTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_SEND_DELETE( ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_SEND_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 568);
    }
    nErrorCode = sqlReturn;

    // 发送响应包 MainCmd=6, SubCmd=5
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 5);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biSerial;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostRecvDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140095810: SP_POST_RECV_DELETE（删除已接收邮件）
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t bDec = 0;
    std::int64_t biDelTime = 0;
    std::uint8_t byState = 0;
    std::uint16_t wPostCount = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> bDec;
    xPacket.XParse >> biDelTime;
    xPacket.XParse >> byState;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&bDec, 1);
    xDBBinder.SetData(&wPostCount, 4);
    xDBBinder.SetData(&biDelTime, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_RECV_DELETE( ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_RECV_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 610);
    }

    // 发送响应包 MainCmd=6, SubCmd=6
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 6);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket.XParse << bDec;
    xSendDBPacket.XParse << wPostCount;
    xSendDBPacket.XParse << byState;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSendBack(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140095AD0: SP_POST_SENDBACK（退回邮件）
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    ST_POST_DATA stRecvData;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket >> stRecvData;

    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    std::uint16_t wPostCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&stRecvData.byFlag, 1);
    xDBBinder.SetData(&stRecvData.byPostType, 1);
    xDBBinder.SetData(&stRecvData.byPostSubType, 1);
    xDBBinder.SetData(&stRecvData.nRemainTime, 1);
    xDBBinder.SetData(&wPostCount, 4);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_SENDBACK( ?, ?, ?, ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_SENDBACK ] [%d error] - Failed query( %d )", sqlReturn, 654);
        nErrorCode = 1;
    }

    // 发送响应包 MainCmd=6, SubCmd=7
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 7);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket << stRecvData;
    xSendDBPacket.XParse << wPostCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSendNameCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140095DC0: SP_POST_NAMECHECK（邮件发送名称检查）
    std::uint32_t dwUCID = 0;
    ST_POST_CHAR stReqSendData;

    xPacket.XParse >> dwUCID;
    xPacket >> stReqSendData;

    std::int16_t sqlReturn = -1;
    int nResult = 0;
    std::uint32_t dwRecvUCID = 0;
    std::uint8_t byRecvClass = 0;
    std::uint8_t byRecvAwaken = 0;
    std::uint32_t dwProfilePhotoID = 0;

    XDBBinder xDBBinder(pDBStmt);
    std::int64_t cbTID = -3;
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetWString(reinterpret_cast<wchar_t*>(&stReqSendData), 21, &cbTID, 1);
    xDBBinder.SetData(&dwRecvUCID, 4);
    xDBBinder.SetData(&byRecvClass, 4);
    xDBBinder.SetData(&byRecvAwaken, 4);
    xDBBinder.SetData(&dwProfilePhotoID, 4);
    xDBBinder.SetData(&nResult, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_NAMECHECK( ?, ?, ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_NAMECHECK ] [%d error] - Failed query( %d )", sqlReturn, 699);
    }

    // 发送响应包 MainCmd=6, SubCmd=8
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 8);
    xSendDBPacket.XParse << nResult;
    xSendDBPacket.XParse << dwRecvUCID;
    xSendDBPacket.XParse << byRecvClass;
    xSendDBPacket.XParse << byRecvAwaken;
    xSendDBPacket.XParse << dwProfilePhotoID;
    xSendDBPacket << stReqSendData;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSystemSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400960C0: 发送系统邮件
    std::int16_t sqlReturn = -1;
    std::uint32_t dwRecvUCID = 0;
    std::int64_t biPostSerial = 0;
    ST_SYSTEM_POST stSystemPost{};
    std::int64_t biRegTime = 0;
    std::uint16_t wPostCount = 0;
    int nPostErrorCode = 0;

    xPacket.XParse >> dwRecvUCID;
    xPacket.XParse >> biPostSerial;
    xPacket >> stSystemPost;

    // 获取当前时间作为 biDelDate
    std::int64_t biDelDate = 0;
#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    // 转换为 Unix 时间戳（简化处理）
    biDelDate = static_cast<std::int64_t>(time(nullptr)) + 32400;  // UTC+9 时区偏移
#else
    biDelDate = static_cast<std::int64_t>(std::time(nullptr)) + 32400;
#endif

    sqlReturn = SendPostSystemSend(pDBStmt, dwRecvUCID, biPostSerial, &stSystemPost,
                                    &biRegTime, &wPostCount, biDelDate, 0, &nPostErrorCode);

    if (sqlReturn || nPostErrorCode) {
        return sqlReturn;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 9);
    xSendDBPacket.XParse << dwRecvUCID;
    xSendDBPacket.XParse << biPostSerial;
    xSendDBPacket << stSystemPost;
    xSendDBPacket.XParse << biRegTime;
    xSendDBPacket.XParse << wPostCount;
    xSendDBPacket.XParse << biDelDate;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqGMTSendPostList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140096340: GMT邮件列表查询
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUCID = 0;
    int nRefreshPostType = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nRefreshPostType;

    PS_GMT_POST_LIST ptSendList{};
    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_GMT_SEND_POST_SELECT( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_GMT_SEND_POST_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 823);
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_GMT_POST_INFO stPost{};
            char szDate[25] = {};
            std::uint8_t bySendPost = 0;
            int nUCID = 0;
            int iGold = 0;
            std::int16_t shType = 0;

            xDBBinder.GetData(&stPost.biNo);
            xDBBinder.GetData(&nUCID);
            stPost.dwUCID = (nUCID > 0) ? static_cast<std::uint32_t>(nUCID) : 0;
            xDBBinder.GetData(&shType);
            stPost.byPostSubType = static_cast<std::uint8_t>(shType);
            xDBBinder.GetWString(stPost.strTitle, 41);
            xDBBinder.GetWString(stPost.strMsg, 401);

            for (int i = 0; i < 5; ++i) {
                xDBBinder.GetData(&stPost.stSysItem[i].nItemID);
                xDBBinder.GetData(&stPost.stSysItem[i].shCount);
            }

            xDBBinder.GetString(szDate, 24);
            xDBBinder.GetData(&bySendPost);
            xDBBinder.GetData(&iGold);
            stPost.biGold = iGold;

            for (int j = 0; j < 5; ++j) {
                xDBBinder.GetData(&stPost.biItemSerial[j]);
            }

            xDBBinder.GetWString(stPost.strName, 21);
            xDBBinder.GetData(&stPost.biDelDate);

            for (int m = 0; m < 5; ++m) {
                xDBBinder.GetData(&stPost.stSysItem[m].nAttack);
                xDBBinder.GetData(&stPost.stSysItem[m].nDefense);
                xDBBinder.GetData(&stPost.stSysItem[m].nTitleID);
                xDBBinder.GetData(&stPost.stSysItem[m].nDyeID);
            }

            for (int n = 0; n < 3; ++n) {
                xDBBinder.GetData(&stPost.stCondition[n].byConditionType);
                xDBBinder.GetData(&stPost.stCondition[n].nMin);
                xDBBinder.GetData(&stPost.stCondition[n].nMax);
            }

            stPost.biPostSerial = 0;
            stPost.bSendPost = (bySendPost != 0);
            ptSendList.vecPostList.push_back(stPost);
        }
    }

    xDBBinder.Close();

    // 加载物品详情（需要 XSQLItemProcess）
    // TODO: 完整实现需要 XSQLItemProcess::SelectPostItemSerial

    if (ptSendList.vecPostList.empty()) {
        PS_GMT_POST_LIST st{};
        st.nRefreshPostType = nRefreshPostType;
        st.bLast = true;
        XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x10);
        xSendDBPacket << st;
        Send(xSendDBPacket);
    } else {
        PS_GMT_POST_LIST ptNewSendList{};
        ptNewSendList.nRefreshPostType = nRefreshPostType;
        int nGMTPostCount = 0;

        for (const auto& post : ptSendList.vecPostList) {
            ptNewSendList.vecPostList.push_back(post);
            if (++nGMTPostCount >= 10) {
                ptNewSendList.bLast = false;
                XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x10);
                xSendDBPacket << ptNewSendList;
                Send(xSendDBPacket);
                ptNewSendList.vecPostList.clear();
                nGMTPostCount = 0;
            }
        }

        ptNewSendList.bLast = true;
        XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x10);
        xSendDBPacket << ptNewSendList;
        Send(xSendDBPacket);
    }

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqGMTSendPostSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140096C20: GMT邮件发送
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;

    PS_GMT_POST_LIST ptList{};
    xPacket >> ptList;

    PS_GMT_POST_SEND_LIST ptSend{};
    std::vector<std::int64_t> vecNoSendPostList;

    for (std::size_t i = 0; i < ptList.vecPostList.size(); ++i) {
        const auto& postInfo = ptList.vecPostList[i];
        std::int32_t byPostSubType = postInfo.byPostSubType - 2;

        switch (byPostSubType) {
        case 0:  // 系统邮件 (byPostSubType = 2)
            {
                ST_POST_DATA stSystemPost{};
                std::uint16_t wPostCount = 0;
                int nPostErrorCode = 0;

                stSystemPost.byPostType = 1;
                stSystemPost.byPostSubType = postInfo.bySystemMailSubType;
                stSystemPost.biSerial = postInfo.biPostSerial;
                stSystemPost.stCharInfo.dwUCID = dwUCID;
                stSystemPost.biMoney = postInfo.biGold;
                std::wcscpy(stSystemPost.strTitle, postInfo.strTitle);
                std::wcscpy(stSystemPost.strMsg, postInfo.strMsg);
                std::wcscpy(stSystemPost.stCharInfo.strName, postInfo.strName);

                for (int nCnt = 0; nCnt < 5; ++nCnt) {
                    stSystemPost.stItemList[nCnt].nItemID = postInfo.stSysItem[nCnt].nItemID;
                    stSystemPost.stItemList[nCnt].sCount = postInfo.stSysItem[nCnt].shCount;
                    stSystemPost.stItemList[nCnt].xSerial = postInfo.biItemSerial[nCnt];
                    stSystemPost.stItemList[nCnt].nAttack = postInfo.stSysItem[nCnt].nAttack;
                    stSystemPost.stItemList[nCnt].nDefense = postInfo.stSysItem[nCnt].nDefense;
                    stSystemPost.stItemList[nCnt].nTitleID = postInfo.stSysItem[nCnt].nTitleID;
                    stSystemPost.stItemList[nCnt].nDyeID = postInfo.stSysItem[nCnt].nDyeID;
                }

                SendPostItemRestore(pDBStmt, 0, &stSystemPost, &wPostCount, postInfo.biDelDate, postInfo.biNo, &nPostErrorCode);

                if (nPostErrorCode) {
                    LogHelper::LogError("game.contents", "SendPostItemRestore GMT error - [UCID:%d, No:%d, Error:%d]",
                                        dwUCID, postInfo.biNo, nPostErrorCode);
                } else {
                    ST_GMT_POST_SEND stGMTPost{};
                    stGMTPost.stPost = postInfo;
                    stGMTPost.stPost.dwUCID = dwUCID;
                    stGMTPost.stPost.bySystemMailSubType = 1;
                    stGMTPost.biNo = postInfo.biNo;
                    stGMTPost.biRegTime = stSystemPost.nRegTime;
                    stGMTPost.wPostCount = wPostCount;
                    stGMTPost.biPostSerial = postInfo.biPostSerial;

                    // TODO: XSQLItemProcess::SelectItemSerial 等辅助函数
                    ptSend.vecPostList.push_back(stGMTPost);
                    sqlReturn = 0;
                }
            }
            break;

        case 1:  // 账号邮件类型1 (byPostSubType = 3)
        case 2:  // 账号邮件类型2 (byPostSubType = 4)
        case 6:  // 账号邮件类型6 (byPostSubType = 8)
        case 7:  // 账号邮件类型7 (byPostSubType = 9)
            {
                ST_ACCOUNT_POST_DATA stAccountPost{};
                std::uint16_t wAccountPostCount[2] = {0, 0};
                int nSendError = 0;

                stAccountPost.byMainType = 4;
                stAccountPost.bySubType = postInfo.bySystemMailSubType;
                stAccountPost.biSerial = 0;
                stAccountPost.dwUAID = dwUAID;
                stAccountPost.biMoney = postInfo.biGold;
                std::wcscpy(stAccountPost.szTitle, postInfo.strTitle);
                std::wcscpy(stAccountPost.szMsg, postInfo.strMsg);
                std::wcscpy(stAccountPost.szSendName, postInfo.strName);

                for (int k = 0; k < 5; ++k) {
                    if (postInfo.biItemSerial[k]) {
                        stAccountPost.stItemList[k] = postInfo.stItem[k];
                    } else {
                        stAccountPost.stItemList[k].nItemID = postInfo.stSysItem[k].nItemID;
                        stAccountPost.stItemList[k].sCount = postInfo.stSysItem[k].shCount;
                        stAccountPost.stItemList[k].xSerial = postInfo.biItemSerial[k];
                        stAccountPost.stItemList[k].nAttack = postInfo.stSysItem[k].nAttack;
                        stAccountPost.stItemList[k].nDefense = postInfo.stSysItem[k].nDefense;
                    }
                }

#ifdef _WIN32
                stAccountPost.biRegTime = static_cast<std::int64_t>(time(nullptr));
#else
                stAccountPost.biRegTime = static_cast<std::int64_t>(std::time(nullptr));
#endif
                stAccountPost.biDelDate = postInfo.biDelDate;

                PostAccountSend(pDBStmt, &stAccountPost, wAccountPostCount, postInfo.biNo, &nSendError);

                if (nSendError) {
                    LogHelper::LogError("game.contents", "PostAccountSend GMT error - [UCID:%d, No:%d, Error:%d]",
                                        dwUCID, postInfo.biNo, nSendError);
                } else {
                    ST_GMT_POST_SEND stGMTPost{};
                    stGMTPost.stPost = postInfo;
                    stGMTPost.stPost.dwUCID = dwUCID;
                    stGMTPost.stPost.bySystemMailSubType = 4;
                    stGMTPost.biNo = postInfo.biNo;
                    stGMTPost.wPostCount = wAccountPostCount[0];
                    stGMTPost.biPostSerial = stAccountPost.biSerial;
                    stGMTPost.biRegTime = stAccountPost.biRegTime;

                    ptSend.vecPostList.push_back(stGMTPost);
                    sqlReturn = 0;
                }
            }
            break;

        case 3:  // 特殊邮件类型 (byPostSubType = 5)
            {
                // TODO: 完整实现需要更多类型支持
                LogHelper::LogDebug("game.contents", "GMT Post SubType 5 - Not fully implemented");
            }
            break;

        case 98:  // 不发送列表 (byPostSubType = 100)
            vecNoSendPostList.push_back(postInfo.biNo);
            break;

        default:  // 其他系统邮件
            {
                ST_SYSTEM_POST stSystemPost{};
                std::int64_t biRegTime = 0;
                std::uint16_t wPostCount = 0;
                int nPostErrorCode = 0;

                stSystemPost.byPostType = 1;
                stSystemPost.byPostSubType = postInfo.bySystemMailSubType;
                stSystemPost.biMoney = postInfo.biGold;
                std::wcscpy(stSystemPost.strTitle, postInfo.strTitle);
                std::wcscpy(stSystemPost.strMsg, postInfo.strMsg);
                std::wcscpy(stSystemPost.strName, postInfo.strName);

                for (int mm = 0; mm < 5; ++mm) {
                    stSystemPost.stSysItem[mm].nItemID = postInfo.stSysItem[mm].nItemID;
                    stSystemPost.stSysItem[mm].shCount = postInfo.stSysItem[mm].shCount;
                    stSystemPost.stSysItem[mm].nAttack = postInfo.stSysItem[mm].nAttack;
                    stSystemPost.stSysItem[mm].nDefense = postInfo.stSysItem[mm].nDefense;
                }

                SendPostSystemSend(pDBStmt, dwUCID, postInfo.biPostSerial, &stSystemPost,
                                   &biRegTime, &wPostCount, postInfo.biDelDate, postInfo.biNo, &nPostErrorCode);

                if (nPostErrorCode) {
                    LogHelper::LogError("game.contents", "SendPostSystemSend GMT error - [UCID:%d, No:%d, Error:%d]",
                                        dwUCID, postInfo.biNo, nPostErrorCode);
                } else {
                    ST_GMT_POST_SEND stGMTPost{};
                    stGMTPost.stPost = postInfo;
                    stGMTPost.stPost.dwUCID = dwUCID;
                    stGMTPost.stPost.bySystemMailSubType = 1;
                    stGMTPost.biNo = postInfo.biNo;
                    stGMTPost.biRegTime = biRegTime;
                    stGMTPost.wPostCount = wPostCount;
                    stGMTPost.biPostSerial = postInfo.biPostSerial;

                    ptSend.vecPostList.push_back(stGMTPost);
                    sqlReturn = 0;
                }
            }
            break;
        }
    }

    ptSend.dwUCID = dwUCID;
    ptSend.nRefreshPostType = ptList.nRefreshPostType;
    ptSend.bLast = ptList.bLast;

    if (!ptSend.vecPostList.empty()) {
        XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x11);
        xSendDBPacket << ptSend;
        Send(xSendDBPacket);
    } else if (ptSend.bLast) {
        XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x11);
        xSendDBPacket << ptSend;
        Send(xSendDBPacket);
    }

    // 更新未发送的邮件
    for (const auto& biNo : vecNoSendPostList) {
        UpdateGMTSendPost(pDBStmt, biNo, dwUCID);
    }

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009A390: SP_POST_SAVE_CHANGE（保存邮件状态变更）
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 1;  // 默认状态为1
    ST_POST_DATA stSaveData;

    xPacket.XParse >> dwUCID;
    xPacket >> stSaveData;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&stSaveData.biSerial, 1);
    xDBBinder.SetData(&byState, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_SAVE_CHANGE(?, ?, ?) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_SAVE_CHANGE ] [%d error] - Failed query( %d )", sqlReturn, 1649);
    }

    // 发送响应包 MainCmd=6, SubCmd=0x12
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x12);
    xSendDBPacket << stSaveData;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostSaveList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009A580: SP_POST_SAVELIST（保存邮件列表）
    std::int16_t sqlReturn = -1;
    std::uint32_t dwUCID = 0;

    xPacket.XParse >> dwUCID;

    ST_POST_LIST stSaveList{};
    ST_POST_LIST stDevideSaveList{};
    bool bLoadSaveList = false;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_SAVELIST(?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_SAVELIST ] [%d error] - Failed query( %d )", sqlReturn, 1705);
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_POST_DATA stData{};
            xDBBinder.GetData(&stData.biSerial);
            xDBBinder.GetData(&stData.stCharInfo.dwUCID);
            xDBBinder.GetData(&stData.stCharInfo.byClass);
            xDBBinder.GetWString(stData.stCharInfo.strName, 21);
            xDBBinder.GetData(&stData.stCharInfo.byAwaken);
            xDBBinder.GetData(&stData.stCharInfo.dwProfilePhotoID);
            xDBBinder.GetData(&stData.byFlag);
            xDBBinder.GetData(&stData.nRemainTime);
            xDBBinder.GetWString(stData.strTitle, 41);
            xDBBinder.GetWString(stData.strMsg, 401);
            xDBBinder.GetData(&stData.biMoney);
            xDBBinder.GetData(&stData.byPostType);
            xDBBinder.GetData(&stData.byPostSubType);

            for (int i = 0; i < 5; ++i) {
                xDBBinder.GetData(&stData.stItemList[i].xSerial);
                xDBBinder.GetData(&stData.stItemList[i].nItemID);
                xDBBinder.GetData(&stData.stItemList[i].sCount);
            }

            xDBBinder.GetData(&stData.nRegTime);
            xDBBinder.GetData(&stData.nRemainTime);  // ___u14 字段

            stSaveList.vecData.push_back(stData);
        }
    }

    xDBBinder.Close();

    // TODO: 完整实现需要 XSQLItemProcess::SelectItemSerial / SelectSocketItem / SelectBroachItem / SelectPackageItem
    // 当前简化实现，仅发送列表，不加载物品详情

    for (auto& stVal : stSaveList.vecData) {
        stDevideSaveList.vecData.push_back(stVal);

        if (stDevideSaveList.vecData.size() >= 10) {
            XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x13);
            xSendDBPacket.XParse << sqlReturn;
            xSendDBPacket.XParse << bLoadSaveList;
            xSendDBPacket << stDevideSaveList;
            Send(xSendDBPacket);
            stDevideSaveList.vecData.clear();
        }
    }

    bLoadSaveList = true;
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x13);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket.XParse << bLoadSaveList;
    xSendDBPacket << stDevideSaveList;
    Send(xSendDBPacket);
    stDevideSaveList.vecData.clear();

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostAccountList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009AC10: SP_POST_ACCOUNT_LIST（账号邮件列表）
    std::uint32_t dwUAID = 0;
    std::int16_t sqlReturn = -1;
    bool bLoadAccountList = false;

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_LIST( ? ) }")));

    PS_ACCOUNT_POST_LIST stAccountPostList;
    PS_ACCOUNT_POST_LIST stDevideList;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_LIST ] [%d error] - Failed query( %d )", sqlReturn, 1805);
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_ACCOUNT_POST_DATA stData;
            xDBBinder.GetData(&stData.biSerial);
            xDBBinder.GetWString(stData.szTitle, 41);
            xDBBinder.GetWString(stData.szMsg, 401);
            xDBBinder.GetData(&stData.biMoney);

            for (int i = 0; i < 5; ++i) {
                xDBBinder.GetData(&stData.stItemList[i].xSerial);
                xDBBinder.GetData(&stData.stItemList[i].nItemID);
                xDBBinder.GetData(&stData.stItemList[i].sCount);
                xDBBinder.GetData(&stData.stItemList[i].nAttack);
                xDBBinder.GetData(&stData.stItemList[i].nDefense);
                xDBBinder.GetData(&stData.stItemList[i].nTitleID);
            }

            xDBBinder.GetData(&stData.biRegTime);
            xDBBinder.GetData(&stData.byFlag);
            xDBBinder.GetData(&stData.biRemainTime);
            xDBBinder.GetWString(stData.szSendName, 21);
            xDBBinder.GetData(&stData.bySubType);
            xDBBinder.GetData(&stData.biDelDate);

            stData.byMainType = 4;  // 账号邮件类型
            stAccountPostList.vecAccountPostList.push_back(stData);
        }
    }

    xDBBinder.Close();

    // TODO: 完整实现需要 XSQLItemProcess::SelectItemSerial / SelectSocketItem / SelectBroachItem / SelectPackageItem
    // 当前简化实现，暂不加载物品详情

    // 遍历邮件，分批发送
    for (auto& stData : stAccountPostList.vecAccountPostList) {
        stDevideList.vecAccountPostList.push_back(stData);

        // 每10个邮件发送一次
        if (stDevideList.vecAccountPostList.size() >= 10) {
            XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x14);
            xSendDBPacket.XParse << sqlReturn;
            xSendDBPacket.XParse << bLoadAccountList;
            xSendDBPacket << stDevideList;
            Send(xSendDBPacket);
            stDevideList.vecAccountPostList.clear();
        }
    }

    // 发送剩余邮件
    bLoadAccountList = true;
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x14);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket.XParse << bLoadAccountList;
    xSendDBPacket << stDevideList;
    Send(xSendDBPacket);

    stAccountPostList.vecAccountPostList.clear();

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostAccountRead(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009B370: SP_POST_ACCOUNT_READ（账号邮件读取）
    std::uint32_t dwUAID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t byFlag = 0;
    std::int64_t biRemainTime = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> byFlag;
    xPacket.XParse >> biRemainTime;

    int nErrorCode = 0;
    std::int16_t sqlReturn = -1;
    std::uint16_t wPostAccountCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byFlag, 1);
    xDBBinder.SetData(&biRemainTime, 1);
    xDBBinder.SetData(&wPostAccountCount, 4);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_READ(?, ?, ?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_READ ] [%d error] - Failed query( %d )", sqlReturn, 1896);
        nErrorCode = 1;
    }

    // 发送响应包 MainCmd=6, SubCmd=0x15
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x15);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket.XParse << byFlag;
    xSendDBPacket.XParse << biRemainTime;
    xSendDBPacket.XParse << wPostAccountCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostAccountReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009B8D0: SP_POST_ACCOUNT_RECEIPT（账号邮件领取）
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t byFlag = 0;
    std::int64_t biRemainTime = 0;
    std::uint8_t byItemFlag = 0;
    std::int64_t biMoney = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> byFlag;
    xPacket.XParse >> biRemainTime;
    xPacket.XParse >> byItemFlag;
    xPacket.XParse >> biMoney;

    PS_RES_STORAGE_INFO stCreateList;
    PS_RES_STORAGE_INFO stUpdateList;
    PS_RES_STORAGE_INFO stUpdateSerial;

    xPacket >> stCreateList;
    xPacket >> stUpdateList;
    xPacket >> stUpdateSerial;

    std::int16_t sqlReturn = -1;
    int nError = 0;

    sqlReturn = UpdateAccountPostReceipt(pDBStmt, dwUAID, biSerial, byFlag, &biRemainTime, &nError);

    if (sqlReturn || nError) {
        nError = 1;
    } else {
        // TODO: 完整实现需要 XSQLItemProcess::CheckCreateItem / CreatePostItem / PostRecvItemUpdate
        // 当前简化实现，跳过物品创建流程
    }

    // 发送响应包 MainCmd=6, SubCmd=0x16
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x16);
    xSendDBPacket.XParse << nError;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket.XParse << byFlag;
    xSendDBPacket << stCreateList;
    xSendDBPacket << stUpdateList;
    xSendDBPacket << stUpdateSerial;
    xSendDBPacket.XParse << biRemainTime;
    xSendDBPacket.XParse << biMoney;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostAccountDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009B650: SP_POST_ACCOUNT_DEL（账号邮件删除）
    std::uint32_t dwUAID = 0;
    std::int64_t biSerial = 0;
    std::int64_t biDelTime = 0;
    std::uint8_t bDec = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> biDelTime;
    xPacket.XParse >> bDec;

    int nErrorCode = 0;
    std::int16_t sqlReturn = -1;
    std::uint16_t wAccountPostCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&wAccountPostCount, 4);
    xDBBinder.SetData(&nErrorCode, 4);
    xDBBinder.SetData(&bDec, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_DEL(?, ?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_DEL ] [%d error] - Failed query( %d )", sqlReturn, 1940);
    }

    // 发送响应包 MainCmd=6, SubCmd=0x17
    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x17);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << biSerial;
    xSendDBPacket.XParse << wAccountPostCount;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostAccountSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009C080: SP_POST_ACCOUNT_SEND（账号邮件发送）
    ST_ACCOUNT_POST_DATA stAccountPostData;
    xPacket >> stAccountPostData;

    std::int64_t biGMTNo = 0;
    xPacket.XParse >> biGMTNo;

    std::int16_t sqlReturn = -1;
    int nPostErrorCode = 0;
    std::uint16_t wAccountPostCount = 0;

    sqlReturn = PostAccountSend(pDBStmt, &stAccountPostData, &wAccountPostCount, biGMTNo, &nPostErrorCode);

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nPostErrorCode) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_SEND ] [%d error] [%d error] - Failed query( %d )",
                           sqlReturn, nPostErrorCode, 2060);
    }

    if (stAccountPostData.biSerial > 0) {
        stAccountPostData.byMainType = 4;  // 账号邮件类型
        XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x18);
        xSendDBPacket << stAccountPostData;
        xSendDBPacket.XParse << wAccountPostCount;
        Send(xSendDBPacket);
    }

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostDeleteAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009C5C0: 批量删除邮件
    PS_POST_DELETE_ALL_SERVER psPostDeleteInfo;
    xPacket >> psPostDeleteInfo;

    std::int16_t sqlReturn = -1;
    std::uint8_t byDecrease = 0;

    if (psPostDeleteInfo.byPostType == 0) {
        // 收件箱删除
        byDecrease = 0;
        for (std::size_t j = 0; j < psPostDeleteInfo.psDeleteList.vecInfo.size(); ++j) {
            const auto& deleteInfo = psPostDeleteInfo.psDeleteList.vecInfo[j];
            std::int64_t biSerial = deleteInfo.biSerial;
            std::int64_t biDeleteDate = deleteInfo.biDeleteDate;

            XDBBinder xDBBinder(pDBStmt);
            xDBBinder.SetData(&psPostDeleteInfo.dwUCID, 1);
            xDBBinder.SetData(&biSerial, 1);
            xDBBinder.SetData(&byDecrease, 1);
            xDBBinder.SetData(&psPostDeleteInfo.wPostCount, 4);
            xDBBinder.SetData(&biDeleteDate, 1);

            sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_RECV_DELETE( ?, ?, ?, ?, ? ) }")));

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                LogHelper::LogError("game.contents", "[ SP_POST_RECV_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 2172);
                psPostDeleteInfo.psFailedList.vecInfo.push_back(deleteInfo);
                psPostDeleteInfo.nErrorCode = sqlReturn;
            }
            xDBBinder.Close();
        }
    } else if (psPostDeleteInfo.byPostType == 1) {
        // 发件箱删除
        for (std::size_t i = 0; i < psPostDeleteInfo.psDeleteList.vecInfo.size(); ++i) {
            const auto& deleteInfo = psPostDeleteInfo.psDeleteList.vecInfo[i];
            std::int64_t biSerial = deleteInfo.biSerial;
            std::int64_t biDeleteDate = deleteInfo.biDeleteDate;

            XDBBinder xDBBinder(pDBStmt);
            xDBBinder.SetData(&psPostDeleteInfo.dwUCID, 1);
            xDBBinder.SetData(&biSerial, 1);
            xDBBinder.SetData(&psPostDeleteInfo.nErrorCode, 4);
            xDBBinder.SetData(&biDeleteDate, 1);

            sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_SEND_DELETE( ?, ?, ?, ? ) }")));

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                LogHelper::LogError("game.contents", "[ SP_POST_SEND_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 2142);
                psPostDeleteInfo.psFailedList.vecInfo.push_back(deleteInfo);
                psPostDeleteInfo.nErrorCode = sqlReturn;
            }
            xDBBinder.Close();
        }
    } else if (psPostDeleteInfo.byPostType == 2) {
        // 已读邮件删除（同收件箱逻辑）
        byDecrease = 0;
        for (std::size_t j = 0; j < psPostDeleteInfo.psDeleteList.vecInfo.size(); ++j) {
            const auto& deleteInfo = psPostDeleteInfo.psDeleteList.vecInfo[j];
            std::int64_t biSerial = deleteInfo.biSerial;
            std::int64_t biDeleteDate = deleteInfo.biDeleteDate;

            XDBBinder xDBBinder(pDBStmt);
            xDBBinder.SetData(&psPostDeleteInfo.dwUCID, 1);
            xDBBinder.SetData(&biSerial, 1);
            xDBBinder.SetData(&byDecrease, 1);
            xDBBinder.SetData(&psPostDeleteInfo.wPostCount, 4);
            xDBBinder.SetData(&biDeleteDate, 1);

            sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_RECV_DELETE( ?, ?, ?, ?, ? ) }")));

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                LogHelper::LogError("game.contents", "[ SP_POST_RECV_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 2172);
                psPostDeleteInfo.psFailedList.vecInfo.push_back(deleteInfo);
                psPostDeleteInfo.nErrorCode = sqlReturn;
            }
            xDBBinder.Close();
        }
    } else if (psPostDeleteInfo.byPostType == 3) {
        // 账号邮件删除
        std::uint16_t wAccountPostCount = 0;
        std::uint8_t in_value = 0;
        for (std::size_t k = 0; k < psPostDeleteInfo.psDeleteList.vecInfo.size(); ++k) {
            const auto& deleteInfo = psPostDeleteInfo.psDeleteList.vecInfo[k];
            std::int64_t biSerial = deleteInfo.biSerial;

            XDBBinder xDBBinder(pDBStmt);
            xDBBinder.SetData(&psPostDeleteInfo.dwUAID, 1);
            xDBBinder.SetData(&biSerial, 1);
            xDBBinder.SetData(&wAccountPostCount, 4);
            xDBBinder.SetData(&psPostDeleteInfo.nErrorCode, 4);
            xDBBinder.SetData(&in_value, 1);

            sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_DEL(?, ?, ?, ?, ?)}")));

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_DEL ] [%d error] - Failed query( %d )", sqlReturn, 2201);
                psPostDeleteInfo.psFailedList.vecInfo.push_back(deleteInfo);
                psPostDeleteInfo.nErrorCode = sqlReturn;
            }
            xDBBinder.Close();
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x19);
    xSendDBPacket << psPostDeleteInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostReceiptAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009CCF0: 批量领取邮件 - 复杂实现，暂用简化版本
    // TODO: 完整实现需要 XSQLItemProcess 辅助
    PS_POST_RECEIPT_ALL_SERVER psPostReceiptInfo;
    xPacket >> psPostReceiptInfo;

    std::int16_t sqlReturn = -1;
    std::uint8_t byFlag = 0;
    xPacket.XParse >> byFlag;

    if (psPostReceiptInfo.byPostType == 0) {
        // 角色邮件批量领取
        sqlReturn = UpdateReceiptAll(pDBStmt, psPostReceiptInfo.dwUCID, psPostReceiptInfo.biSerial,
                                     psPostReceiptInfo.byFlag, psPostReceiptInfo.biRemainTime,
                                     psPostReceiptInfo.bDecrease, &psPostReceiptInfo.wPostCount, &psPostReceiptInfo.nErrorCode);

        if (sqlReturn || psPostReceiptInfo.nErrorCode) {
            psPostReceiptInfo.nErrorCode = 1;
        }
    } else if (psPostReceiptInfo.byPostType == 1) {
        // 账号邮件批量领取
        sqlReturn = UpdateAccountPostReceiptAll(pDBStmt, psPostReceiptInfo.dwUAID, psPostReceiptInfo.biSerial,
                                                psPostReceiptInfo.byFlag, &psPostReceiptInfo.biRemainTime,
                                                psPostReceiptInfo.bDecrease, &psPostReceiptInfo.wPostCount, &psPostReceiptInfo.nErrorCode);

        if (sqlReturn || psPostReceiptInfo.nErrorCode) {
            psPostReceiptInfo.nErrorCode = 1;
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x20);
    xSendDBPacket << psPostReceiptInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostLevelUpEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009D840: 加载升级活动事件
    std::uint32_t dwUAID = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_EVENT_VALUE_LOAD( ? )}")));

    PS_POST_LEVEL_UP_EVENT_INFO_VEC stInfoList;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_EVENT_VALUE_LOAD ] [%d error] - Failed query( %d )", sqlReturn, dwUAID);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_POST_LEVEL_UP_EVENT_INFO stInfo;
            xDBBinder.GetData(&stInfo.nGroup);
            xDBBinder.GetData(&stInfo.nLv);
            xDBBinder.GetData(&stInfo.nUCID);
            stInfoList.vecInfo.push_back(stInfo);
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x21);
    xSendDBPacket << stInfoList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostLevelUpEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009DA60: 更新升级活动事件
    PS_POST_LEVEL_UP_EVENT_UPDATE stEvent;
    xPacket >> stEvent;

    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stEvent.nUAID, 1);
    xDBBinder.SetData(&stEvent.nGroup, 1);
    xDBBinder.SetData(&stEvent.nLv, 1);
    xDBBinder.SetData(&stEvent.nUCID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_EVENT_VALUE_UPDATE( ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_EVENT_VALUE_UPDATE ] [%d error] - Failed query( %d / %d )", sqlReturn, stEvent.nUAID, nErrorCode);
    }

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostLevelUpEventReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009DBC0: 重置升级活动事件
    std::uint32_t dwUAID = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ACCOUNT_EVENT_VALUE_RESET( ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ACCOUNT_EVENT_VALUE_RESET ] [%d error] - Failed query( %d )", sqlReturn, dwUAID);
    }

    return sqlReturn;
}

std::int32_t XSQLPostProcess::ReqPostRestoreLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14009DCA0: 加载恢复物品列表
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;
    PS_ITEM_RESTORE_LIST psRestoreItemList;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_CHARACTER_LOAD_RESTORE_ITEM( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_RESTORE_LOAD ] error - Failed query");
    } else {
        PS_ITEM_RESTORE_INFO stRestoreItemInfo;
        PS_ITEM_RESTORE_INFO stBroachItemInfo;

        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            stRestoreItemInfo.stItem.Init();
            stBroachItemInfo.stItem.Init();

            xDBBinder.GetData(&stRestoreItemInfo.stItem.nItemID);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.sCount);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.byEndurance);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.bBindType);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.eFlag);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.byUpgrade);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.bySocketActiveCount);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nCashDate);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.byUpgradeCount);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.byUpgradeLimit);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nExp);
            xDBBinder.GetString(stRestoreItemInfo.stItem.szBroachState, 16);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.byRestoreCount);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.bySealCount);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.bySealDelCount);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nAttack);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nDefense);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nTitleID);

            // 扩展选项
            for (int k = 0; k < 5; ++k) {
                xDBBinder.GetData(&stRestoreItemInfo.stItem.stExtendOption[k].byType);
                xDBBinder.GetData(&stRestoreItemInfo.stItem.stExtendOption[k].nOption);
            }

            // Broach 物品（15 个）
            for (int i = 0; i < 15; ++i) {
                xDBBinder.GetData(&stBroachItemInfo.stItem.nItemID);
                if (stBroachItemInfo.stItem.nItemID != -1) {
                    stBroachItemInfo.stItem.sCount = 1;
                    stBroachItemInfo.nPostNumber = -1;
                    psRestoreItemList.vecInfo.push_back(stBroachItemInfo);
                }
            }

            xDBBinder.GetData(&stRestoreItemInfo.nPostNumber);
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nDyeID);
            psRestoreItemList.vecInfo.push_back(stRestoreItemInfo);
        }
    }
    xDBBinder.Close();

    if (!psRestoreItemList.vecInfo.empty()) {
        sqlReturn = LoadPostRestoreItemSocket(pDBStmt, dwUCID, &psRestoreItemList);
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 6, 0x24);
    xSendDBPacket << psRestoreItemList;
    Send(xSendDBPacket);

    return sqlReturn;
}


// XSQLPostProcess helper functions for Account Post operations

std::int16_t XSQLPostProcess::UpdateAccountPostReceipt(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::int64_t biSerial,
                                                       std::uint8_t byFlag, std::int64_t* biRemainTime, int* nError) {
    // Per IDA 0x14009BF50: SP_POST_ACCOUNT_RECEIPT
    std::int16_t sqlReturn = -1;
    *nError = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byFlag, 1);
    xDBBinder.SetData(biRemainTime, 1);
    xDBBinder.SetData(nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_RECEIPT( ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_RECEIPT ] [%d error] - Failed query( %d )", sqlReturn, 2041);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::UpdateAccountPostReceiptAll(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::int64_t biSerial,
                                                          std::uint8_t byFlag, std::int64_t* biRemainTime, std::uint8_t bDecrease,
                                                          std::uint16_t* wPostCount, int* nError) {
    // Per IDA 0x14009D6E0: SP_POST_ACCOUNT_RECEIPT_ALL
    std::int16_t sqlReturn = -1;
    *nError = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byFlag, 1);
    xDBBinder.SetData(biRemainTime, 1);
    xDBBinder.SetData(&bDecrease, 1);
    xDBBinder.SetData(wPostCount, 4);
    xDBBinder.SetData(nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_RECEIPT_ALL( ?, ?, ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_RECEIPT_ALL ] [%d error] - Failed query( %d )", sqlReturn, 2349);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::PostAccountSend(XDBStmt* pDBStmt, ST_ACCOUNT_POST_DATA* pstData, std::uint16_t* wAccountPostCount,
                                              std::int64_t biGMTNo, int* nPostErrorCode) {
    // Per IDA 0x14009C260: SP_POST_ACCOUNT_SEND
    std::int16_t sqlReturn = -1;
    *nPostErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);

    // 输入参数
    xDBBinder.SetData(&pstData->dwUAID, 1);
    xDBBinder.SetData(&pstData->biSerial, 1);

    std::int64_t cbTitle = -3;  // SQL_NTS
    xDBBinder.SetWString(pstData->szTitle, 41, &cbTitle, 1);

    std::int64_t cbMsg = -3;
    xDBBinder.SetWString(pstData->szMsg, 401, &cbMsg, 1);

    xDBBinder.SetData(&pstData->biMoney, 1);

    // 物品参数
    for (int i = 0; i < 5; ++i) {
        xDBBinder.SetData(&pstData->stItemList[i].xSerial, 1);
        xDBBinder.SetData(&pstData->stItemList[i].nItemID, 1);
        xDBBinder.SetData(&pstData->stItemList[i].sCount, 1);
        xDBBinder.SetData(&pstData->stItemList[i].nAttack, 1);
        xDBBinder.SetData(&pstData->stItemList[i].nDefense, 1);
        xDBBinder.SetData(&pstData->stItemList[i].nTitleID, 1);
    }

    xDBBinder.SetData(&pstData->biRegTime, 1);
    xDBBinder.SetData(&pstData->byFlag, 1);
    xDBBinder.SetData(&pstData->biRemainTime, 1);

    std::int64_t cbSendName = -3;
    xDBBinder.SetWString(pstData->szSendName, 21, &cbSendName, 1);

    xDBBinder.SetData(&pstData->bySubType, 1);
    xDBBinder.SetData(&biGMTNo, 1);

    // 输出参数
    xDBBinder.SetData(wAccountPostCount, 4);
    xDBBinder.SetData(nPostErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_ACCOUNT_SEND( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_ACCOUNT_SEND ] [%d error] - Failed query( %d )", sqlReturn, 2070);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::SendPostSystemSend(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                                  ST_SYSTEM_POST* stSystemPost, std::int64_t* biRegTime,
                                                  std::uint16_t* wPostCount, std::int64_t biDelDate,
                                                  std::int64_t biGMTNo, int* nPostErrorCode) {
    // Per IDA 0x140099BD0: SP_POST_SYSTEMSEND
    std::int16_t sqlReturn = -1;
    *nPostErrorCode = 0;
    *wPostCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&stSystemPost->byPostType, 1);
    xDBBinder.SetData(&stSystemPost->byPostSubType, 1);

    std::int64_t cbTitle = -3;  // SQL_NTS
    xDBBinder.SetWString(stSystemPost->strTitle, 41, &cbTitle, 1);

    std::int64_t cbMsg = -3;
    xDBBinder.SetWString(stSystemPost->strMsg, 401, &cbMsg, 1);

    xDBBinder.SetData(&stSystemPost->biMoney, 1);

    for (int i = 0; i < 5; ++i) {
        xDBBinder.SetData(&stSystemPost->stSysItem[i].nItemID, 1);
        xDBBinder.SetData(&stSystemPost->stSysItem[i].shCount, 1);
        xDBBinder.SetData(&stSystemPost->stSysItem[i].nAttack, 1);
        xDBBinder.SetData(&stSystemPost->stSysItem[i].nDefense, 1);
        xDBBinder.SetData(&stSystemPost->stSysItem[i].nTitleID, 1);
        xDBBinder.SetData(&stSystemPost->stSysItem[i].nDyeID, 1);
    }

    xDBBinder.SetData(&biDelDate, 1);

    std::int64_t cbName = -3;
    xDBBinder.SetWString(stSystemPost->strName, 21, &cbName, 1);

    xDBBinder.SetData(biRegTime, 4);
    xDBBinder.SetData(wPostCount, 4);
    xDBBinder.SetData(&biDelDate, 1);
    xDBBinder.SetData(&biGMTNo, 1);
    xDBBinder.SetData(nPostErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
        "{call SP_POST_SYSTEMSEND( ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?, ?,?,?,?,? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || *nPostErrorCode) {
        LogHelper::LogError("game.contents", "[ SP_POST_SYSTEMSEND ] [%d error(%d)] - Failed query( %d )",
                            *nPostErrorCode, sqlReturn, 1575);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::SendPostItemRestore(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_POST_DATA* stPostData,
                                                   std::uint16_t* wPostCount, std::int64_t biDelDate,
                                                   std::int64_t biGMTNo, int* nPostErrorCode) {
    // Per IDA 0x140099F80: SP_POST_ITEM_RESTORE
    std::int16_t sqlReturn = -1;
    *nPostErrorCode = 0;
    *wPostCount = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stPostData->biSerial, 1);
    xDBBinder.SetData(&stPostData->stCharInfo.dwUCID, 1);
    xDBBinder.SetData(&stPostData->byPostType, 1);
    xDBBinder.SetData(&stPostData->byPostSubType, 1);

    std::int64_t cbTitle = -3;
    xDBBinder.SetWString(stPostData->strTitle, 41, &cbTitle, 1);

    std::int64_t cbMsg = -3;
    xDBBinder.SetWString(stPostData->strMsg, 401, &cbMsg, 1);

    xDBBinder.SetData(&stPostData->biMoney, 1);

    for (int i = 0; i < 5; ++i) {
        xDBBinder.SetData(&stPostData->stItemList[i].xSerial, 1);
        xDBBinder.SetData(&stPostData->stItemList[i].nItemID, 1);
        xDBBinder.SetData(&stPostData->stItemList[i].sCount, 1);
        xDBBinder.SetData(&stPostData->stItemList[i].nAttack, 1);
        xDBBinder.SetData(&stPostData->stItemList[i].nDefense, 1);
        xDBBinder.SetData(&stPostData->stItemList[i].nTitleID, 1);
        xDBBinder.SetData(&stPostData->stItemList[i].nDyeID, 1);
    }

    std::int64_t cbName = -3;
    xDBBinder.SetWString(stPostData->stCharInfo.strName, 21, &cbName, 1);

    xDBBinder.SetData(&stPostData->nRegTime, 4);
    xDBBinder.SetData(wPostCount, 4);
    xDBBinder.SetData(&biDelDate, 1);
    xDBBinder.SetData(&biGMTNo, 1);
    xDBBinder.SetData(nPostErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
        "{call SP_POST_ITEM_RESTORE( ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || *nPostErrorCode) {
        LogHelper::LogError("game.contents", "[ SP_POST_ITEM_RESTORE ] [%d error(%d)] - Failed query( %d )",
                            *nPostErrorCode, sqlReturn, 1620);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::UpdateGMTSendPost(XDBStmt* pDBStmt, std::int64_t biNo, std::uint32_t dwUCID) {
    // Per IDA: 更新GMT邮件发送状态
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&biNo, 1);
    xDBBinder.SetData(&dwUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_GMT_SEND_POST_UPDATE( ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_GMT_SEND_POST_UPDATE ] [%d error] - Failed query", sqlReturn);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::UpdateReceiptAll(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                              std::uint8_t byPostFlag, std::int64_t biRemainTime, std::uint8_t bDecrease,
                                              std::uint16_t* wPostCount, int* nError) {
    // Per IDA 0x14009D590: SP_POST_RECEIPT_ALL
    std::int16_t sqlReturn = -1;
    *nError = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byPostFlag, 1);
    xDBBinder.SetData(&biRemainTime, 1);
    xDBBinder.SetData(&bDecrease, 1);
    xDBBinder.SetData(wPostCount, 4);
    xDBBinder.SetData(nError, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_POST_RECEIPT_ALL( ?, ?, ?, ?, ?, ?, ? ) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_POST_DEL_TIME ] [%d error] - Failed query( %d )", sqlReturn, 2324);
    }

    return sqlReturn;
}

std::int16_t XSQLPostProcess::LoadPostRestoreItemSocket(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_ITEM_RESTORE_LIST* psRestoreItemList) {
    // Per IDA 0x14009E200: 加载恢复物品 socket 信息
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_CHARACTER_LOAD_RESTORE_ITEM_SOCKET( ? ) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_CHARACTER_LOAD_RESTORE_ITEM_SOCKET ] error - Failed query");
    } else {
        PS_ITEM_RESTORE_INFO stRestoreItemInfo;
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            stRestoreItemInfo.stItem.Init();
            stRestoreItemInfo.stItem.sCount = 1;
            xDBBinder.GetData(&stRestoreItemInfo.stItem.nItemID);

            // 扩展选项 (socket 信息)
            for (int i = 0; i < 5; ++i) {
                xDBBinder.GetData(&stRestoreItemInfo.stItem.stExtendOption[i].byType);
                xDBBinder.GetData(&stRestoreItemInfo.stItem.stExtendOption[i].nOption);
            }

            stRestoreItemInfo.nPostNumber = -1;
            psRestoreItemList->vecInfo.push_back(stRestoreItemInfo);
        }
    }
    xDBBinder.Close();

    return sqlReturn;
}

XSQLLeagueProcess::XSQLLeagueProcess() : TXSQLProcessBase<0x07>("XSQLLeagueProcess") {}
XSQLLeagueProcess::~XSQLLeagueProcess() = default;

std::int32_t XSQLLeagueProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006C630: SubCmd routing for League operations
    switch (xPacket.GetSubCmd()) {
    case 0x00: return ReqLeagueCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x01: return ReqLeagueDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqLeagueBoard(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqLeagueWIthDraw(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqLeagueApplicant(pDBStmt, xPacket, xReturnSessionID);
    case 0x0B: return ReqLeagueKick(pDBStmt, xPacket, xReturnSessionID);
    case 0x0F: return ReqLeagueInviteAccept(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqLeagueApplicantAccept(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqLeagueApplicantReject(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqLeagueOverlapName(pDBStmt, xPacket, xReturnSessionID);
    case 0x14: return ReqLeagueNoticeChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x15: return ReqLeagueNameChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x16: return ReqLeagueCardChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x17: return ReqLeaguePositionNameChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x18: return ReqLeagueAuthChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x19: return ReqLeagueMemberPositionChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ReqLeagueApplicantDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqLeagueNoticeDateReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqLeagueInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x27: return ReqLeagueOpenOrNot(pDBStmt, xPacket, xReturnSessionID);
    case 0x28: return ReqLeagueRecruitNotice(pDBStmt, xPacket, xReturnSessionID);
    case 0x29: return ReqLeagueSearch(pDBStmt, xPacket, xReturnSessionID);
    case 0x30: return ReqLeagueRecordUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x32: return ReqLeagueDelegate(pDBStmt, xPacket, xReturnSessionID);
    case 0x33: return ReqLeaguGetWealth(pDBStmt, xPacket, xReturnSessionID);
    case 0x34: return ReqLeagueLevelup(pDBStmt, xPacket, xReturnSessionID);
    case 0x35: return ReqLeagueSkillLearn(pDBStmt, xPacket, xReturnSessionID);
    case 0x36: return ReqLeagueMemberExpInit(pDBStmt, xPacket, xReturnSessionID);
    case 0x37: return ReqLeagueInventoryMove(pDBStmt, xPacket, xReturnSessionID);
    case 0x39: return ReqLeagueInventoryInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x40: return ReqLeagueInit(pDBStmt, xPacket, xReturnSessionID);
    case 0x41: return ReqLeagueList(pDBStmt, xPacket, xReturnSessionID);
    case 0x42: return ReqGMTLeagueInfo(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

// XSQLLeagueProcess SubCmd handlers
std::int32_t XSQLLeagueProcess::ReqLeagueCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006CFA0: SP_LEAGUE_CREATE（公会创建）
    PS_LEAGUE_CREATE_FOR_SERVER stCreateLeague;
    xPacket >> stCreateLeague;

    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stCreateLeague.dwActorID, 1);
    xDBBinder.SetWString(reinterpret_cast<wchar_t*>(&stCreateLeague), 10, &cbTID, 1);
    xDBBinder.SetWString(stCreateLeague.szMasterName, 21, &cbTID, 1);
    xDBBinder.SetData(&stCreateLeague.stCreateInfo.nLeagueID, 4);
    xDBBinder.SetData(&stCreateLeague.nCreateDate, 1);
    xDBBinder.SetData(&stCreateLeague.stCreateInfo.nErrorCode, 4);
    xDBBinder.SetData(&stCreateLeague.nAuth_Elder, 1);
    xDBBinder.SetData(&stCreateLeague.nAuth_Manager, 1);
    xDBBinder.SetData(&stCreateLeague.nAuth_SubMaster, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_CREATE(?, ?, ?, ?, ?, ?, ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 594);
    }

    // 发送响应包 MainCmd=7, SubCmd=0
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0);
    xSendDBPacket << stCreateLeague;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006D210: SP_LEAGUE_DELETE（公会删除）
    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;
    int nLeagueID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;

    int nErrorCode = 0;
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&biPenalty, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_DELETE(?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 629);
    }

    // 发送响应包 MainCmd=7, SubCmd=1
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 1);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << biPenalty;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueBoard(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006DAD0: SP_LEAGUE_BOARD_INSERT（公会公告插入）
    std::uint32_t dwServerID = 0;
    ST_LEAGUE_BOARD stBoard;
    int nLeagueID = 0;
    std::uint32_t dwActorID = 0;

    xPacket.XParse >> dwServerID;
    xPacket >> stBoard;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;

    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stBoard.nSerial, 4);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetWString(stBoard.szCharName, 21, &cbTID, 1);
    xDBBinder.SetWString(stBoard.szMsg, 61, &cbTID, 1);
    xDBBinder.SetData(&stBoard.biEnrollDate, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_BOARD_INSERT ( ?, ?, ?, ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_BOARD_INSERT ] [%d error] - Failed query( %d )", sqlReturn, 875);
        return sqlReturn;
    }

    // 发送响应包 MainCmd=7, SubCmd=5
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 5);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket << stBoard;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << dwActorID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueWIthDraw(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006D5E0: 公会退出
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::int64_t biPenalty = 0;
    std::uint32_t dwServerID = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;
    xPacket.XParse >> dwServerID;

    std::int16_t sqlReturn = DelLeagueMember(pDBStmt, dwActorID, nLeagueID, biPenalty);

    // 发送响应包 MainCmd=7, SubCmd=6
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 6);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << biPenalty;
    xSendDBPacket.XParse << dwServerID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueApplicant(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006D480: 公会申请
    ST_LEAGUE_APPLICANT stApplicant;
    int nServerID = 0;

    xPacket >> stApplicant;
    xPacket.XParse >> nServerID;

    std::int16_t sqlReturn = LeagueApplicant(pDBStmt, stApplicant);
    if (sqlReturn) {
        return sqlReturn;
    }

    // 发送响应包 MainCmd=7, SubCmd=9
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 9);
    xSendDBPacket << stApplicant;
    xSendDBPacket.XParse << nServerID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueKick(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006D790: 公会成员踢出
    std::uint32_t dwActorID = 0;
    std::uint32_t dwTargetActorID = 0;
    int nLeagueID = 0;
    std::uint32_t dwServerID = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwTargetActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwServerID;

    std::int16_t sqlReturn = KickoutLeagueMember(pDBStmt, dwTargetActorID, nLeagueID);

    // 发送响应包 MainCmd=7, SubCmd=0xB
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0xB);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << dwTargetActorID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << dwServerID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueInviteAccept(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006D930: 公会邀请接受
    ST_REQ_LEAGUE_INVITE_ACCEPT stAccept;
    std::uint32_t dwServerID = 0;
    std::int64_t biJoinDate = 0;

    xPacket >> stAccept;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> biJoinDate;

    std::int16_t sqlReturn = -1;
    ST_LEAGUE_MEMBER_EX stMemberEx;

    if (LeagueJoin(pDBStmt, stAccept, biJoinDate)) {
        sqlReturn = LoadLeagueMember(pDBStmt, stAccept.dwTargetUCID, stMemberEx);
    }

    // 发送响应包 MainCmd=7, SubCmd=0xF
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0xF);
    xSendDBPacket << stAccept;
    xSendDBPacket << stMemberEx;
    xSendDBPacket.XParse << dwServerID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueApplicantAccept(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006DDC0: 接受公会申请
    ST_REQ_LEAGUE_APPLICANT_ACCEPT stAccept;
    xPacket >> stAccept;

    int nServerID = 0;
    std::uint32_t dwActorID = 0;
    xPacket.XParse >> nServerID;
    xPacket.XParse >> dwActorID;

    std::int16_t sqlReturn = -1;
    ST_LEAGUE_MEMBER_EX stMemberEx;

    if (LeagueApplicantJoin(pDBStmt, &stAccept)) {
        sqlReturn = LoadLeagueMember(pDBStmt, stAccept.dwReqActorID, stMemberEx);
    }

    // 发送响应包 MainCmd=7, SubCmd=0x10
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x10);
    xSendDBPacket << stAccept;
    xSendDBPacket.XParse << nServerID;
    xSendDBPacket << stMemberEx;
    xSendDBPacket.XParse << dwActorID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueApplicantReject(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006DF80: 拒绝公会申请
    ST_REQ_LEAGUE_APPLICANT_REJECT stReject;
    xPacket >> stReject;

    int nServerID = 0;
    xPacket.XParse >> nServerID;

    std::int16_t sqlReturn = LeagueApplicantReject(pDBStmt, &stReject);

    // 发送响应包 MainCmd=7, SubCmd=0x11
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x11);
    xSendDBPacket << stReject;
    xSendDBPacket.XParse << nServerID;
    xSendDBPacket.XParse << sqlReturn;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueMemberPositionChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006E0C0: 更新成员职位
    ST_LEAGUE_MEMBER_POSITION stPosition;
    xPacket >> stPosition;

    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    int nServerID = 0;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> nServerID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&stPosition.dwActorID, 1);
    xDBBinder.SetData(&stPosition.byPosition, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_MEMBER_POSITION_UPDATE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_MEMBER_POSITION_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 955);
    }
    xDBBinder.Close();

    // 发送响应包 MainCmd=7, SubCmd=0x19
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x19);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket << stPosition;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << nServerID;
    Send(xSendDBPacket);

    return sqlReturn;
}

// League helper functions
std::int16_t XSQLLeagueProcess::LeagueApplicantJoin(XDBStmt* pDBStmt, ST_REQ_LEAGUE_APPLICANT_ACCEPT* stAccept) {
    // Per IDA: 接受申请加入公会
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stAccept->nLeagueID, 1);
    xDBBinder.SetData(&stAccept->dwReqActorID, 1);
    xDBBinder.SetWString(stAccept->szReqName, 21, &cbTID, 1);
    xDBBinder.SetData(&stAccept->biJoinDate, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_APPLICANT_ACCEPT(?, ?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_APPLICANT_ACCEPT ] [%d error] - Failed query", sqlReturn);
        return sqlReturn;
    }

    stAccept->nResult = nErrorCode;
    return sqlReturn;
}

std::int16_t XSQLLeagueProcess::LeagueApplicantReject(XDBStmt* pDBStmt, ST_REQ_LEAGUE_APPLICANT_REJECT* stReject) {
    // Per IDA: 拒绝申请加入公会
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stReject->nLeagueID, 1);
    xDBBinder.SetData(&stReject->dwTargetUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_APPLICANT_REJECT(?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_APPLICANT_REJECT ] [%d error] - Failed query", sqlReturn);
    }

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueOverlapName(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006CD70: SP_LEAGUE_OVERLAP_NAME（公会名称重复检查）
    wchar_t szLeagueName[11] = {0};
    short sLen = 0;
    xPacket.XParse.GetWString(szLeagueName, 10, sLen);

    int nErrorCode = -1;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetWString(szLeagueName, 10, &cbTID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_OVERLAP_NAME(?,?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_OVERLAP_NAME ] [%d error] - Failed query( %d )", sqlReturn, 561);
    }

    // 发送响应包 MainCmd=7, SubCmd=0x12
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x12);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << GreenDamTan_BoundedWideString(szLeagueName);
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueNoticeChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006CB30: SP_LEAGUE_NOTICE_UPDATE（公会公告更新）
    ST_LEAGUE_NOTICE stNotice;
    xPacket >> stNotice;

    int nServerID = 0;
    std::uint32_t dwActorID = 0;
    xPacket.XParse >> nServerID;
    xPacket.XParse >> dwActorID;

    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stNotice.nLeagueID, 1);
    xDBBinder.SetWString(stNotice.szNotice, 801, &cbTID, 1);
    xDBBinder.SetData(&stNotice.biEnrollDate, 1);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_NOTICE_UPDATE(?,?,?) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_NOTICE_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 154);
    }

    // 发送响应包 MainCmd=7, SubCmd=0x14
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x14);
    xSendDBPacket << stNotice;
    xSendDBPacket.XParse << nServerID;
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << sqlReturn;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueNameChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006F440: 公会名称更新
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS
    int nErrorCode = 0;

    PS_LEAGUE_NAME_CHANGE_SERVER psNameChangeInfo;
    xPacket >> psNameChangeInfo;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psNameChangeInfo.nLeagueID, 1);
    xDBBinder.SetWString(psNameChangeInfo.szLeagueName, 10, &cbTID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_NAME_UPDATE(?, ?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nErrorCode) {
        psNameChangeInfo.nResult = 57001;
        XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x15);
        xSendDBPacket << psNameChangeInfo;
        Send(xSendDBPacket);
        return sqlReturn;
    }
    else {
        // 遍历物品列表，调用 UpdateItemCount 或 DeleteItem
        XSQLItemProcess itemProcess;
        for (const auto& stInfo : psNameChangeInfo.psUpdateItemList.vecItem) {
            if (stInfo.stItem.sCount > 0) {
                sqlReturn = itemProcess.UpdateItemCount(pDBStmt, psNameChangeInfo.dwUCID, stInfo.stItem.xSerial, stInfo.stItem.sCount);
                if (sqlReturn == -1) {
                    break;
                }
            }
            else {
                sqlReturn = itemProcess.DeleteItem(pDBStmt, psNameChangeInfo.dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos);
                if (sqlReturn == -1) {
                    break;
                }
            }
        }
        psNameChangeInfo.nResult = sqlReturn;

        XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x15);
        xSendDBPacket << psNameChangeInfo;
        Send(xSendDBPacket);

        return sqlReturn;
    }
}

std::int32_t XSQLLeagueProcess::ReqLeagueCardChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006F880: 公会卡片更新
    std::int16_t sqlReturn = -1;

    PS_REQ_LEAGUE_CARD psCardInfo;
    PS_RES_STORAGE_INFO vecUpdateItem;

    xPacket >> psCardInfo;

    std::uint32_t dwUCID = 0;
    std::uint32_t dwServerID = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwServerID;
    xPacket >> vecUpdateItem;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psCardInfo.nLeagueID, 1);
    xDBBinder.SetData(&psCardInfo.dwLeagueCard, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_CARD_UPDATE(?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_CARD_UPDATE ] error - Failed query( %d )", 1454);
        xDBBinder.Close();
    }
    else {
        // 遍历物品列表，调用 UpdateItemCount 或 DeleteItem
        auto* pClientPtr = GetClientPtr();
        if (pClientPtr) {
            // Per IDA: XClient::GetProcessPtr<XSQLItemProcess>(ClientPtr, 0x21u)
            // 由于当前架构限制，使用临时 XSQLItemProcess 对象调用辅助方法
            XSQLItemProcess itemProcess;
            for (const auto& stInfo : vecUpdateItem.vecItem) {
                if (stInfo.stItem.sCount > 0) {
                    // 更新数量
                    sqlReturn = itemProcess.UpdateItemCount(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.stItem.sCount);
                    if (sqlReturn == -1) {
                        break;
                    }
                }
                else {
                    // 删除物品
                    sqlReturn = itemProcess.DeleteItem(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos);
                    if (sqlReturn == -1) {
                        break;
                    }
                }
            }
        }
        xDBBinder.Close();
    }

    // 发送响应包 (MainCmd=7, SubCmd=0x16)
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x16);
    xSendDBPacket << psCardInfo;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket << vecUpdateItem;
    xSendDBPacket.XParse << sqlReturn;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeaguePositionNameChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006F1F0: 公会职位名称更新
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS

    ST_LEAGUE_POSITION_NAME_CHANGE stChange;
    std::int32_t nLeagueID = 0;
    std::uint32_t dwActorID = 0;
    std::int32_t nServerID = 0;

    xPacket.XParse >> nLeagueID;
    xPacket >> stChange;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nServerID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&stChange.nPosition, 1);
    xDBBinder.SetWString(stChange.szLeagueName, 4, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_POSITION_NAME_UPDATE(?, ?, ?) }")));

    if (sqlReturn) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_POSITION_NAME_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1331);
        stChange.nResult = sqlReturn;
    }

    // 发送响应包 (MainCmd=7, SubCmd=0x17)
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x17);
    xSendDBPacket << stChange;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << nServerID;
    xSendDBPacket.XParse << dwActorID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueAuthChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006EF10: 公会权限更新
    std::int16_t sqlReturn = -1;

    ST_LEAGUE_AUTH_CHANGE stChange;
    std::int32_t nLeagueID = 0;
    std::int32_t nServerID = 0;
    std::uint32_t dwActorID = 0;

    xPacket >> stChange;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> nServerID;
    xPacket.XParse >> dwActorID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&stChange.nAuth[0], 1);
    xDBBinder.SetData(&stChange.nAuth[1], 1);
    xDBBinder.SetData(&stChange.nAuth[2], 1);
    xDBBinder.SetData(&stChange.nAuth[3], 1);
    xDBBinder.SetData(&stChange.nAuth[4], 1);
    xDBBinder.SetData(&stChange.nAuth[5], 1);
    xDBBinder.SetData(&stChange.nAuth[6], 1);
    xDBBinder.SetData(&stChange.nAuth[7], 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_AUTH_UPDATE(?,?,?,?,?,?,?,?,?) }")));

    if (sqlReturn) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_AUTH_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1294);
        stChange.nResult = sqlReturn;
    }

    // 发送响应包 (MainCmd=7, SubCmd=0x18)
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x18);
    xSendDBPacket << stChange;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << nServerID;
    xSendDBPacket.XParse << dwActorID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueApplicantDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006FC50: 删除公会申请
    int nLeagueID = 0;
    std::uint32_t dwActorID = 0;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_APPLICANT_DELETE(?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_APPLICANT_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 1486);
        return sqlReturn;
    }

    // 发送响应包 MainCmd=7, SubCmd=0x20
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x20);
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << dwActorID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueNoticeDateReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006E2F0: SP_LEAGUE_NOTICE_DATE_RESET - 无响应包
    int nLeagueID = 0;
    xPacket.XParse >> nLeagueID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_NOTICE_DATE_RESET (?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_NOTICE_DATE_RESET ] [%d error] - Failed query( %d )", sqlReturn, 980);
    }

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400728D0: League info load - calls multiple helper functions
    std::int16_t sqlReturn = -1;

    // Parse request packet
    PS_DB_LEAGUE_LOAD psDBLoadInfo;
    xPacket >> psDBLoadInfo;

    // Load league info
    ST_LEAGUE_INFO stLeagueInfo;
    sqlReturn = LoadLeagueInfo(pDBStmt, psDBLoadInfo.nLeagueID, stLeagueInfo);
    if (sqlReturn != 0) {
        return sqlReturn;
    }

    // Load member list
    ST_LEAGUE_MEMBER_LIST stMemberList;
    sqlReturn = LoadLeagueMemberList(pDBStmt, psDBLoadInfo.nLeagueID, stMemberList);
    if (sqlReturn != 0) {
        return sqlReturn;
    }

    // Load board list
    ST_LEAGUE_BOARD_LIST stBoardList;
    sqlReturn = LoadLeagueBoard(pDBStmt, psDBLoadInfo.nLeagueID, stBoardList);
    if (sqlReturn != 0) {
        return sqlReturn;
    }

    // Load applicant list
    ST_LEAGUE_APPLICANT_LIST stApplicantList;
    sqlReturn = LoadLeagueApplicantList(pDBStmt, psDBLoadInfo.nLeagueID, stApplicantList);
    if (sqlReturn != 0) {
        return sqlReturn;
    }

    // Load record list
    ST_LEAGUE_RECORD_LIST stRecordList;
    sqlReturn = LoadLeagueRecordList(pDBStmt, psDBLoadInfo.nLeagueID, stRecordList);
    if (sqlReturn != 0) {
        return sqlReturn;
    }

    // Send response packet MainCmd=7, SubCmd=0x23
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x23);
    xSendDBPacket << stLeagueInfo;
    xSendDBPacket << stMemberList;
    xSendDBPacket << stBoardList;
    xSendDBPacket << stApplicantList;
    xSendDBPacket << stRecordList;
    xSendDBPacket << psDBLoadInfo;
    xSendDBPacket.XParse << sqlReturn;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueOpenOrNot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006FE30: SP_LEAGUE_OPEN_UPDATE
    ST_LEAGUE_OPEN stOpen;
    xPacket >> stOpen;

    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;
    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stOpen.nLeagueID, 1);
    xDBBinder.SetData(&stOpen.bOpen, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_OPEN_UPDATE(?,?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_OPEN_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 1519);
        return sqlReturn;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x27);
    xSendDBPacket << stOpen;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket.XParse << dwUCID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueRecruitNotice(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140070060: SP_LEAGUE_RECRUIT_NOTICE
    ST_LEAGUE_RECRUIT_NOTICE stRecruitNotice;
    xPacket >> stRecruitNotice;

    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stRecruitNotice.nLeagueID, 1);
    xDBBinder.SetData(&stRecruitNotice.biRegDate, 1);
    xDBBinder.SetWString(stRecruitNotice.szNotice, 51, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_RECRUIT_NOTICE(?,?,?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_RECRUIT_NOTICE ] [%d error] - Failed query( %d )", sqlReturn, 1556);
        return sqlReturn;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x28);
    xSendDBPacket << stRecruitNotice;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket.XParse << dwUCID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueSearch(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400702C0: League search by name or master
    std::int16_t sqlReturn = -1;

    PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList;
    ST_REQ_LEAGUE_SEARCH stSearch;
    xPacket >> stSearch;

    std::uint32_t dwUCID = 0;
    std::uint32_t dwServerID = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwServerID;

    if (stSearch.nState == 0) {
        // Search by league name
        sqlReturn = SearchLeagueToName(pDBStmt, stSearch, psLeagueSummaryList);
    } else if (stSearch.nState == 1) {
        // Search by master name
        sqlReturn = SearchLeagueToMaster(pDBStmt, stSearch, psLeagueSummaryList);
    }

    // Send response packet MainCmd=7, SubCmd=0x29
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x29);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket << psLeagueSummaryList;
    Send(xSendDBPacket);

    return sqlReturn;
}

// Per IDA 0x1400704E0: Search league by name
std::int16_t XSQLLeagueProcess::SearchLeagueToName(XDBStmt* pDBStmt, const ST_REQ_LEAGUE_SEARCH& stSearch, PS_LEAGUE_SUMMARY_LIST& psLeagueSummaryList) {
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;
    std::int32_t nMaxCount = 10;

    wchar_t szLeagueName[10] = {};
    std::wcsncpy(szLeagueName, stSearch.szLeagueName, 9);

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetWString(szLeagueName, 10, &cbTID, 1);
    xDBBinder.SetData(&nMaxCount, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_SEARCH_NAME(?,?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_SEARCH_NAME ] [%d error] - Failed query( %d )", sqlReturn, 1633);
        return sqlReturn;
    }

    PS_LEAGUE_INFO_SUMMARY psSummaryInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        psSummaryInfo = PS_LEAGUE_INFO_SUMMARY{};
        xDBBinder.GetData(&psSummaryInfo.nLeagueID);
        xDBBinder.GetData(&psSummaryInfo.nMemberCount);
        std::uint32_t dwLeagueCard = 0;
        xDBBinder.GetData(&dwLeagueCard);
        psSummaryInfo.dwLeagueCard = dwLeagueCard;
        xDBBinder.GetData(&psSummaryInfo.byRating);
        short sLen = 0;
        xDBBinder.GetWString(psSummaryInfo.szLeagueName, 10, sLen);
        xDBBinder.GetWString(psSummaryInfo.szMaster, 21, sLen);
        xDBBinder.GetWString(psSummaryInfo.szRecruit, 51, sLen);
        xDBBinder.GetWString(psSummaryInfo.szSubMaster, 21, sLen);

        psLeagueSummaryList.vecInfo.push_back(psSummaryInfo);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x140070700: Search league by master name
std::int16_t XSQLLeagueProcess::SearchLeagueToMaster(XDBStmt* pDBStmt, const ST_REQ_LEAGUE_SEARCH& stSearch, PS_LEAGUE_SUMMARY_LIST& psLeagueSummaryList) {
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;
    std::int32_t nMaxCount = 10;

    wchar_t szMasterName[21] = {};
    std::wcsncpy(szMasterName, stSearch.szMasterName, 20);

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetWString(szMasterName, 21, &cbTID, 1);
    xDBBinder.SetData(&nMaxCount, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_SEARCH_MASTER(?,?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_SEARCH_MASTER ] [%d error] - Failed query( %d )", sqlReturn, 1679);
        return sqlReturn;
    }

    PS_LEAGUE_INFO_SUMMARY psSummaryInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        psSummaryInfo = PS_LEAGUE_INFO_SUMMARY{};
        xDBBinder.GetData(&psSummaryInfo.nLeagueID);
        xDBBinder.GetData(&psSummaryInfo.nMemberCount);
        std::uint32_t dwLeagueCard = 0;
        xDBBinder.GetData(&dwLeagueCard);
        psSummaryInfo.dwLeagueCard = dwLeagueCard;
        xDBBinder.GetData(&psSummaryInfo.byRating);
        short sLen = 0;
        xDBBinder.GetWString(psSummaryInfo.szLeagueName, 10, sLen);
        xDBBinder.GetWString(psSummaryInfo.szMaster, 21, sLen);
        xDBBinder.GetWString(psSummaryInfo.szRecruit, 51, sLen);
        xDBBinder.GetWString(psSummaryInfo.szSubMaster, 21, sLen);

        psLeagueSummaryList.vecInfo.push_back(psSummaryInfo);
    }
    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueRecordUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140070920: SP_LEAGUE_RECORD_INSERT - 无响应包
    ST_LEAGUE_RECORD stRecord;
    xPacket >> stRecord;

    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stRecord.nLeagueID, 1);
    xDBBinder.SetData(&stRecord.byFlag, 1);
    xDBBinder.SetData(&stRecord.biRegisterDate, 1);
    xDBBinder.SetWString(stRecord.szValue1, 21, &cbTID, 1);
    xDBBinder.SetWString(stRecord.szValue2, 21, &cbTID, 1);
    xDBBinder.SetData(&stRecord.nValue3, 1);
    xDBBinder.SetData(&stRecord.nValue4, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_RECORD_INSERT(?,?,?,?,?,?,?) }")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            LogHelper::LogError("game.contents", "[ SP_LEAGUE_RECORD_INSERT ] error - Failed query( %d )", 1714);
        }
    }

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueDelegate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140070B30: 公会委托
    PS_REQ_LEAGUE_DELEGATE psDelegateReq;
    xPacket >> psDelegateReq;

    std::uint32_t dwMasterUCID = 0;
    std::uint32_t dwServerID = 0;
    xPacket.XParse >> dwMasterUCID;
    xPacket.XParse >> dwServerID;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psDelegateReq.nLeagueID, 1);
    xDBBinder.SetData(&dwMasterUCID, 1);
    xDBBinder.SetData(&psDelegateReq.dwDelegatedUCID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_DELEGATE(?, ?, ?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_DELEGATE ] error - Failed query( %d )", 1750);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包 MainCmd=7, SubCmd=0x32
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x32);
    xSendDBPacket << psDelegateReq;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket.XParse << dwMasterUCID;
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeaguGetWealth(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140070DD0: 获取公会财富
    PS_LEAGUE_WEALTH_FOR_SERVER psLeagueWealthInfo;
    xPacket >> psLeagueWealthInfo;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psLeagueWealthInfo.dwUCID, 1);
    xDBBinder.SetData(&psLeagueWealthInfo.nLeagueID, 1);
    xDBBinder.SetData(&psLeagueWealthInfo.nTotalExp, 1);
    xDBBinder.SetData(&psLeagueWealthInfo.shExp, 1);
    xDBBinder.SetData(&psLeagueWealthInfo.nGold, 1);
    xDBBinder.SetData(&psLeagueWealthInfo.nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_GOODS_UPDATE(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_GOODS_UPDATE ] error - Failed query( %d )", 1782);
        xDBBinder.Close();
        return sqlReturn;
    }

    xDBBinder.Close();

    // 发送响应包 MainCmd=7, SubCmd=0x33
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x33);
    xSendDBPacket << psLeagueWealthInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueLevelup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140071040: 公会升级
    int nLeagueID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t bySkillPoint = 0;
    PS_AUTO_SKILL psAutoSkillInfo;
    bool bCheat = false;
    std::uint32_t dwUCID = 0;

    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> byLevel;
    xPacket.XParse >> bySkillPoint;
    xPacket >> psAutoSkillInfo;
    xPacket.XParse >> bCheat;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&byLevel, 1);
    xDBBinder.SetData(&bySkillPoint, 1);

    // Auto skill 信息 - 每个字节单独设置
    std::uint8_t* pSkillData = reinterpret_cast<std::uint8_t*>(&psAutoSkillInfo);
    for (int i = 0; i < 8; ++i) {
        xDBBinder.SetData(&pSkillData[i], 1);
    }

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_LEVELUP(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_LEVELUP ] error - Failed query( %d )", 1826);
        xDBBinder.Close();
        return sqlReturn;
    }

    xDBBinder.Close();

    // 只有非作弊模式才发送响应包
    if (!bCheat) {
        XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x34);
        xSendDBPacket.XParse << nLeagueID;
        xSendDBPacket.XParse << byLevel;
        xSendDBPacket.XParse << bySkillPoint;
        xSendDBPacket << psAutoSkillInfo;
        xSendDBPacket.XParse << dwUCID;
        Send(xSendDBPacket);
    }

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueSkillLearn(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140071330: 公会技能学习
    PS_RES_LEAGUE_SKILL psResSkillInfo;
    xPacket >> psResSkillInfo;

    std::uint8_t byType = 0;
    xPacket.XParse >> byType;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psResSkillInfo.nLeagueID, 1);
    xDBBinder.SetData(&psResSkillInfo.bySkillGroupID, 1);
    xDBBinder.SetData(&psResSkillInfo.bySkillPoint, 1);
    xDBBinder.SetData(&psResSkillInfo.bySkillLevel, 1);
    xDBBinder.SetData(&psResSkillInfo.biGold, 1);
    xDBBinder.SetData(&psResSkillInfo.dwUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_SKILL_UPDATE(?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_SKILL_UPDATE ] error - Failed query( %d )", 1869);
        xDBBinder.Close();
        return sqlReturn;
    }

    xDBBinder.Close();

    // 发送响应包 MainCmd=7, SubCmd=0x35
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x35);
    xSendDBPacket << psResSkillInfo;
    xSendDBPacket.XParse << byType;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueMemberExpInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400715C0: 成员经验初始化 - 无响应包
    int nLeagueID = 0;
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwUCID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&dwUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_MEMBER_EXP_INIT(?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_MEMBER_EXP_INIT ] error - Failed query( %d )", 1898);
    }

    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueInventoryMove(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140071DC0: 联赛仓库物品移动（插入/删除/位置更新）
    std::int16_t sqlReturn = -1;

    // 解析请求包
    std::uint32_t dwUCID = 0;
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME psItemMoveForGame;

    xPacket.XParse >> dwUCID;
    xPacket >> psItemMoveForGame;

    auto byType = psItemMoveForGame.psReqItemMoveInfo.byType;

    if (byType == 0) {
        // byType == 0: SP_LEAGUE_INVENTORY_INSERT（插入物品）
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&psItemMoveForGame.psStorageInfo.stItem.xSerial, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.nLeagueID, 1);
        xDBBinder.SetData(&psItemMoveForGame.psStorageInfo.shSlotPos, 1);
        xDBBinder.SetData(&psItemMoveForGame.psStorageInfo.stItem.nItemID, 1);
        xDBBinder.SetData(&psItemMoveForGame.nErrorCode, 4);

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_INVENTORY_INSERT(?, ?, ?, ?, ?)}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_LEAGUE_INVENTORY_INSERT ] error - Failed query( %d )", 2026);
        }

        xDBBinder.Close();
    }
    else if (byType == 1) {
        // byType == 1: SP_LEAGUE_INVENTORY_DELETE（删除物品，需获取结果行）
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&dwUCID, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.biSrcSerial, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.nLeagueID, 1);
        xDBBinder.SetData(&psItemMoveForGame.psStorageInfo.shSlotPos, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.shSrcSlotPos, 1);
        xDBBinder.SetData(&psItemMoveForGame.nErrorCode, 4);

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_INVENTORY_DELETE(?, ?, ?, ?, ?, ?)}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_LEAGUE_INVENTORY_DELETE ] error - Failed query( %d )", 2100);
        }
        else {
            // Fetch 返回删除的物品信息
            if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.nItemID);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.byUpgrade);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.sCount);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.xSerial);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.shSlotPos);

                // 读取 5 个扩展选项
                for (int i = 0; i < 5; ++i) {
                    std::uint8_t byOptType = 0;
                    xDBBinder.GetData(&byOptType);
                    psItemMoveForGame.psOutItemInfo.stItem.stExtendOption[i].byType = byOptType;
                    xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.stExtendOption[i].nOption);
                }

                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.bySocketActiveCount);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.byUpgradeCount);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.byUpgradeLimit);
                std::uint16_t wEndurance = 0;
                xDBBinder.GetData(&wEndurance);
                psItemMoveForGame.psOutItemInfo.stItem.byEndurance = static_cast<std::uint8_t>(wEndurance);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.byRestoreCount);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.bySealCount);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.bySealDelCount);
                xDBBinder.GetString(psItemMoveForGame.psOutItemInfo.stItem.szBroachState, 16);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.bBindType);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.nAttack);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.nDefense);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.nTitleID);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.eFlag);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.nExp);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.byUseCount);
                xDBBinder.GetData(&psItemMoveForGame.psOutItemInfo.stItem.nDyeID);
            }
            xDBBinder.Close();

            // 获取镶嵌/镂刻/套装信息（通过 XSQLItemProcess 辅助方法）
            auto* pClientPtr = GetClientPtr();
            if (pClientPtr) {
                // 需要通过 XServerCon 获取 XSQLItemProcess，但当前架构中 GetProcessPtr 方法不可用
                // 临时简化实现：直接调用静态辅助方法（需要在 XSQLItemProcess 中暴露）
                // TODO: 完整实现需要 XSQLItemProcess::SelectSocketItem / SelectBroachItem / SelectPackageItem
            }
        }
    }
    else if (byType == 2) {
        // byType == 2: SP_LEAGUE_INVENTORY_POS_UPDATE（位置更新/物品移动）
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.biSrcSerial, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.biDestcSerial, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.nLeagueID, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.shSrcSlotPos, 1);
        xDBBinder.SetData(&psItemMoveForGame.psReqItemMoveInfo.shDestSlotPos, 1);
        xDBBinder.SetData(&psItemMoveForGame.nErrorCode, 4);

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_INVENTORY_POS_UPDATE(?, ?, ?, ?, ?, ?)}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            LogHelper::LogError("game.contents", "[ SP_LEAGUE_INVENTORY_POS_UPDATE ] error - Failed query( %d )", 2134);
        }
        else {
            // 循环获取结果行，填充日志列表
            while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
                PS_LEAGUE_INVENTORY_FOR_LOG psMoveInfo;
                xDBBinder.GetData(&psMoveInfo.biSerial);
                xDBBinder.GetData(&psMoveInfo.nItemID);
                xDBBinder.GetData(&psMoveInfo.shItemCount);
                xDBBinder.GetData(&psMoveInfo.shPos);
                xDBBinder.GetData(&psMoveInfo.nAttack);
                xDBBinder.GetData(&psMoveInfo.nDefense);
                xDBBinder.GetData(&psMoveInfo.nItemTitleID);

                // 添加到 psItemLogList（当前使用 raw 数组占位，后续需要正确处理 vector）
                // TODO: 正确处理 psItemLogList vector
            }
        }
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x37);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << psItemMoveForGame;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueInventoryInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400716E0: 公会仓库信息查询
    std::int16_t sqlReturn = -1;

    // 解析请求包
    PS_REQ_LEAGUE_INVEN_INFO psReqInvenInfo;
    xPacket >> psReqInvenInfo;

    std::uint32_t dwReqUCID = 0;
    xPacket.XParse >> dwReqUCID;

    // 调用 SP_LEAGUE_INVENTORY_SELECT 存储过程
    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psReqInvenInfo.nLeagueID, 1);
    xDBBinder.SetData(&psReqInvenInfo.shStartPos, 1);
    xDBBinder.SetData(&psReqInvenInfo.shEndPos, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_INVENTORY_SELECT(?, ?, ?) }")));

    // 初始化响应结构
    PS_RES_STORAGE_INFO psResInvenInfo;
    PS_ITEM_SOCKET_LIST stSocketList;
    PS_ITEM_BROACH_LIST stBroachList;
    PS_ITEM_PACKAGE_LIST psPackageList;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_INVENTORY_SELECT ] error - Failed query( %d )", 1972);
        xDBBinder.Close();
        return sqlReturn;
    }

    // 循环读取仓库物品信息
    PS_STORAGE_INFO stStorageInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stStorageInfo.stItem.Init();

        xDBBinder.GetData(&stStorageInfo.stItem.nItemID);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgrade);
        xDBBinder.GetData(&stStorageInfo.stItem.sCount);
        xDBBinder.GetData(&stStorageInfo.stItem.xSerial);
        xDBBinder.GetData(&stStorageInfo.shSlotPos);

        // 扩展选项 (5个)
        for (int i = 0; i < 5; ++i) {
            std::uint8_t byType = 0;
            xDBBinder.GetData(&byType);
            stStorageInfo.stItem.stExtendOption[i].byType = byType;
            xDBBinder.GetData(&stStorageInfo.stItem.stExtendOption[i].nOption);
        }

        xDBBinder.GetData(&stStorageInfo.stItem.bySocketActiveCount);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgradeCount);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgradeLimit);
        std::uint16_t wEndurance = 0;
        xDBBinder.GetData(&wEndurance);
        stStorageInfo.stItem.byEndurance = static_cast<std::uint8_t>(wEndurance);
        xDBBinder.GetData(&stStorageInfo.stItem.byRestoreCount);
        xDBBinder.GetData(&stStorageInfo.stItem.bySealCount);
        xDBBinder.GetData(&stStorageInfo.stItem.bySealDelCount);
        xDBBinder.GetString(stStorageInfo.stItem.szBroachState, 16);
        xDBBinder.GetData(&stStorageInfo.stItem.nAttack);
        xDBBinder.GetData(&stStorageInfo.stItem.nDefense);
        xDBBinder.GetData(&stStorageInfo.stItem.nTitleID);
        xDBBinder.GetData(&stStorageInfo.stItem.eFlag);
        xDBBinder.GetData(&stStorageInfo.stItem.nExp);
        xDBBinder.GetData(&stStorageInfo.stItem.byUseCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nDyeID);

        psResInvenInfo.vecItem.push_back(stStorageInfo);
    }
    xDBBinder.Close();

    // 获取镶嵌/镂刻/套装信息（通过 XSQLItemProcess 辅助方法）
    auto* pClientPtr = GetClientPtr();
    if (pClientPtr) {
        XSQLItemProcess itemProcess;
        for (std::size_t j = 0; j < psResInvenInfo.vecItem.size(); ++j) {
            auto& item = psResInvenInfo.vecItem[j];
            // Per IDA: serial is in stItem.xSerial field (从 sCount 字段位置读取，实际是 xSerial)
            std::int64_t biSerial = item.stItem.xSerial;

            // 调用辅助方法获取镶嵌信息
            itemProcess.SelectSocketItem(pDBStmt, biSerial, &stSocketList);
            itemProcess.SelectBroachItem(pDBStmt, biSerial, &stBroachList);
            itemProcess.SelectPackageItem(pDBStmt, biSerial, &psPackageList);
        }
    }

    // 发送响应包 MainCmd=7, SubCmd=0x39
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x39);
    xSendDBPacket.XParse << psReqInvenInfo.nLeagueID;
    xSendDBPacket.XParse << dwReqUCID;
    xSendDBPacket << psResInvenInfo;
    xSendDBPacket << stBroachList;
    xSendDBPacket << stSocketList;
    xSendDBPacket << psPackageList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140072710: SP_LEAGUE_INIT - initialize league system
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_INIT }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_INIT ] error - Failed query( %d )", 2156);
    }
    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqLeagueList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140073700: League list load
    std::int16_t sqlReturn = -1;

    std::uint32_t dwUCID = 0;
    std::uint32_t dwServerID = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwServerID;

    XDBBinder xDBBinder(pDBStmt);
    std::int32_t nListCount = 12;
    xDBBinder.SetData(&nListCount, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_LIST(?) }")));

    PS_LEAGUE_SUMMARY_LIST psLeagueList;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "Failed SP_LEAGUE_LIST [Error:%d]", sqlReturn);
    } else {
        PS_LEAGUE_INFO_SUMMARY psLeagueInfo;
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            psLeagueInfo = PS_LEAGUE_INFO_SUMMARY{};
            xDBBinder.GetData(&psLeagueInfo.nLeagueID);
            xDBBinder.GetData(&psLeagueInfo.nMemberCount);
            xDBBinder.GetData(&psLeagueInfo.byRating);
            // Read union field
            std::uint32_t dwLeagueCard = 0;
            xDBBinder.GetData(&dwLeagueCard);
            short sLen = 0;
            xDBBinder.GetWString(psLeagueInfo.szLeagueName, 10, sLen);
            xDBBinder.GetWString(psLeagueInfo.szMaster, 21, sLen);
            xDBBinder.GetWString(psLeagueInfo.szSubMaster, 21, sLen);
            xDBBinder.GetWString(psLeagueInfo.szRecruit, 51, sLen);

            psLeagueList.vecInfo.push_back(psLeagueInfo);
        }
    }
    xDBBinder.Close();

    // Send response packet MainCmd=7, SubCmd=0x41
    XSendDBPacket xSendDBPacket(xReturnSessionID, 7, 0x41);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << dwServerID;
    xSendDBPacket << psLeagueList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLLeagueProcess::ReqGMTLeagueInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

// XSQLLeagueProcess Helper functions
std::int16_t XSQLLeagueProcess::DelLeagueMember(XDBStmt* pDBStmt, std::uint32_t dwUCID, int nLeagueID, std::int64_t biPenalty) {
    // Per IDA 0x14006ED50: SP_LEAGUE_MEMBER_DEL（公会成员删除）
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&nLeagueID, 1);
    xDBBinder.SetData(&biPenalty, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_MEMBER_DEL (?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_MEMBER_DEL ] [%d error] - Failed query( %d )", sqlReturn, 1246);
    }

    return sqlReturn;
}

std::int16_t XSQLLeagueProcess::KickoutLeagueMember(XDBStmt* pDBStmt, std::uint32_t dwUCID, int nLeagueID) {
    // Per IDA 0x14006EE40: SP_LEAGUE_MEMBER_KICKOUT（公会成员踢出）
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_MEMBER_KICKOUT (?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_MEMBER_KICKOUT ] [%d error] - Failed query( %d )", sqlReturn, 1262);
    }

    return sqlReturn;
}

bool XSQLLeagueProcess::LeagueJoin(XDBStmt* pDBStmt, ST_REQ_LEAGUE_INVITE_ACCEPT& stAccept, std::int64_t biJoinDate) {
    // Per IDA 0x14006E8B0: SP_LEAGUE_JOIN（加入公会）
    int nErrorCode = 0;
    std::int64_t cbTID = -3;
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stAccept.dwTargetUCID, 1);
    xDBBinder.SetData(&stAccept.nLeagueID, 1);
    xDBBinder.SetWString(stAccept.szTargetName, 21, &cbTID, 1);
    xDBBinder.SetData(&biJoinDate, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_JOIN (?, ?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nErrorCode) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_JOIN ] [%d error] - Failed query( %d )", sqlReturn, 1146);
        return false;
    }

    return true;
}

std::int16_t XSQLLeagueProcess::LeagueApplicant(XDBStmt* pDBStmt, ST_LEAGUE_APPLICANT& stApplicant) {
    // Per IDA 0x14006EA20: SP_LEAGUE_APPLICANT（公会申请）
    std::int64_t cbTID = -3;
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stApplicant.dwActorID, 1);
    xDBBinder.SetWString(stApplicant.szName, 21, &cbTID, 1);
    xDBBinder.SetData(&stApplicant.nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_APPLICANT (?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_APPLICANT ] [%d error] - Failed query( %d )", sqlReturn, 1162);
    }

    return sqlReturn;
}

std::int16_t XSQLLeagueProcess::LoadLeagueMember(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_LEAGUE_MEMBER_EX& stMemberEx) {
    // Per IDA 0x14006E500: Load league member info
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_MEMBER_SELECT (?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_MEMBER_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 1091);
        return sqlReturn;
    }

    // Fetch result
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&stMemberEx.dwUCID);
        xDBBinder.GetData(&stMemberEx.stMember.nLeagueID);
        xDBBinder.GetData(&stMemberEx.stMember.byPosition);
        xDBBinder.GetData(&stMemberEx.stMember.biLeagueExp);
        xDBBinder.GetData(&stMemberEx.stMember.biJoinDate);
        xDBBinder.GetData(&stMemberEx.stMember.biApplicationDate);
        short sLen = 0;
        xDBBinder.GetWString(stMemberEx.szName, 21, sLen);
        xDBBinder.GetData(&stMemberEx.byClass);
        xDBBinder.GetData(&stMemberEx.shLevel);
        xDBBinder.GetData(&stMemberEx.sWorldID);
        xDBBinder.GetData(&stMemberEx.byChannel);
        xDBBinder.GetData(&stMemberEx.biPlayDate);
        xDBBinder.GetData(&stMemberEx.byAwaken);
        xDBBinder.GetData(&stMemberEx.dwProfilePhotoID);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x140072D50: Load league info
std::int16_t XSQLLeagueProcess::LoadLeagueInfo(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_INFO& stLeagueInfo) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_INFO_SELECT( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_SELECT ] [%d error, LeagueID:%d] - Failed query", sqlReturn, nLeagueID);
        xDBBinder.Close();
        return sqlReturn;
    }

    if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&stLeagueInfo.nLeagueID);
        short sLen = 0;
        xDBBinder.GetWString(stLeagueInfo.szLeagueName, 10, sLen);
        xDBBinder.GetData(&stLeagueInfo.dwMasterUCID);
        xDBBinder.GetWString(stLeagueInfo.szMasterName, 21, sLen);
        std::uint32_t dwLeagueCard = 0;
        xDBBinder.GetData(&dwLeagueCard);
        stLeagueInfo.dwLeagueCard = dwLeagueCard;
        xDBBinder.GetData(&stLeagueInfo.byRating);
        xDBBinder.GetData(&stLeagueInfo.biExp);
        xDBBinder.GetData(&stLeagueInfo.biMoney);
        xDBBinder.GetData(&stLeagueInfo.nLeagueRank);
        xDBBinder.GetWString(stLeagueInfo.szNotice, 801, sLen);
        xDBBinder.GetData(&stLeagueInfo.nCreateDate);
        xDBBinder.GetWString(stLeagueInfo.szPosition_1, 11, sLen);
        xDBBinder.GetWString(stLeagueInfo.szPosition_2, 11, sLen);
        xDBBinder.GetWString(stLeagueInfo.szPosition_3, 11, sLen);
        xDBBinder.GetData(&stLeagueInfo.shMemberCount);
        for (int i = 0; i < 8; ++i) {
            xDBBinder.GetData(&stLeagueInfo.nAuth[i]);
        }
        xDBBinder.GetData(&stLeagueInfo.biNoticeDate);
        std::uint8_t bOpen = 0;
        xDBBinder.GetData(&bOpen);
        stLeagueInfo.bOpen = bOpen != 0;
        xDBBinder.GetWString(stLeagueInfo.szRecruitNotice, 51, sLen);
        xDBBinder.GetData(&stLeagueInfo.biRecruitNoticeDate);
        xDBBinder.GetData(&stLeagueInfo.bySkillPoint);
        for (int i = 0; i < 8; ++i) {
            xDBBinder.GetData(&stLeagueInfo.bySkill[i]);
        }
        xDBBinder.GetData(&stLeagueInfo.nLimitExp);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x140073140: Load league member list
std::int16_t XSQLLeagueProcess::LoadLeagueMemberList(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_MEMBER_LIST& stMemberList) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_LOAD_ALL_WITH_MEMBER(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_LOAD_ALL_WITH_MEMBER ] [%d error] - Failed query( %d )", sqlReturn, 2345);
        xDBBinder.Close();
        return sqlReturn;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stMemberEx = ST_LEAGUE_MEMBER_EX{};
        xDBBinder.GetData(&stMemberEx.dwUCID);
        xDBBinder.GetData(&stMemberEx.stMember.nLeagueID);
        xDBBinder.GetData(&stMemberEx.stMember.byPosition);
        xDBBinder.GetData(&stMemberEx.stMember.biLeagueExp);
        xDBBinder.GetData(&stMemberEx.stMember.biJoinDate);
        xDBBinder.GetData(&stMemberEx.stMember.biApplicationDate);
        short sLen = 0;
        xDBBinder.GetWString(stMemberEx.szName, 21, sLen);
        xDBBinder.GetData(&stMemberEx.byClass);
        xDBBinder.GetData(&stMemberEx.shLevel);
        xDBBinder.GetData(&stMemberEx.sWorldID);
        xDBBinder.GetData(&stMemberEx.byChannel);
        xDBBinder.GetData(&stMemberEx.biPlayDate);
        xDBBinder.GetData(&stMemberEx.byAwaken);
        xDBBinder.GetData(&stMemberEx.dwProfilePhotoID);

        stMemberList.vecInfo.push_back(stMemberEx);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x14006E720: Load league board
std::int16_t XSQLLeagueProcess::LoadLeagueBoard(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_BOARD_LIST& stBoardList) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_LEAGUE_BOARD_LIST( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_BOARD_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 1121);
        xDBBinder.Close();
        return sqlReturn;
    }

    ST_LEAGUE_BOARD stBoard;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stBoard = ST_LEAGUE_BOARD{};
        xDBBinder.GetData(&stBoard.nSerial);
        xDBBinder.GetData(&stBoard.nLeagueID);
        short sLen = 0;
        xDBBinder.GetWString(stBoard.szCharName, 21, sLen);
        xDBBinder.GetWString(stBoard.szMsg, 61, sLen);
        xDBBinder.GetData(&stBoard.biEnrollDate);

        stBoardList.vecInfo.push_back(stBoard);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x140073370: Load league applicant list
std::int16_t XSQLLeagueProcess::LoadLeagueApplicantList(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_APPLICANT_LIST& stApplicantList) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_APPLICANT_LIST(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_APPLICANT_LIST ] [%d error] - Failed query( %d )", sqlReturn, 2383);
        xDBBinder.Close();
        return sqlReturn;
    }

    ST_LEAGUE_APPLICANT stApplicant;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stApplicant = ST_LEAGUE_APPLICANT{};
        xDBBinder.GetData(&stApplicant.dwActorID);
        xDBBinder.GetData(&stApplicant.nLeagueID);
        xDBBinder.GetData(&stApplicant.biApplicantDate);
        xDBBinder.GetData(&stApplicant.shLevel);
        short sLen = 0;
        xDBBinder.GetWString(stApplicant.szName, 21, sLen);
        xDBBinder.GetData(&stApplicant.byClass);
        xDBBinder.GetData(&stApplicant.byAwaken);
        xDBBinder.GetData(&stApplicant.dwProfilePhotoID);

        stApplicantList.vecInfo.push_back(stApplicant);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x140073540: Load league record list
std::int16_t XSQLLeagueProcess::LoadLeagueRecordList(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_RECORD_LIST& stRecordList) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nLeagueID, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_LEAGUE_RECORD_LOAD(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_LEAGUE_RECORD_LOAD ] error - Failed query( %d )", 2420);
        xDBBinder.Close();
        return sqlReturn;
    }

    ST_LEAGUE_RECORD stRecord;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stRecord = ST_LEAGUE_RECORD{};
        xDBBinder.GetData(&stRecord.nLeagueID);
        xDBBinder.GetData(&stRecord.byFlag);
        xDBBinder.GetData(&stRecord.biRegisterDate);
        short sLen = 0;
        xDBBinder.GetWString(stRecord.szValue1, 21, sLen);
        xDBBinder.GetWString(stRecord.szValue2, 21, sLen);
        xDBBinder.GetData(&stRecord.nValue3);
        xDBBinder.GetData(&stRecord.nValue4);

        stRecordList.vecInfo.push_back(stRecord);
    }
    xDBBinder.Close();

    return sqlReturn;
}

XSQLForceProcess::XSQLForceProcess() : TXSQLProcessBase<0x08>("XSQLForceProcess") {}
XSQLForceProcess::~XSQLForceProcess() = default;

std::int32_t XSQLForceProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140041550: SubCmd routing for Force operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqCreateForce(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqJoinForce(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqLeaveForce(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqUpdateMemberInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqForceChangeMaster(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqForceDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqForceUpdateInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x0B: return ReqForceLoadAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x0D: return ReqForceMatchingCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x0E: return ReqForceTypeUpdate(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

// XSQLForceProcess SubCmd handlers
std::int32_t XSQLForceProcess::ReqCreateForce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140041710: 创建战队
    PS_REQ_FORCE_CREATE stForceReq;
    xPacket >> stForceReq;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    stForceReq.dwForceID = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceReq.masterInfo.dwMemberID, 1);
    xDBBinder.SetData(&stForceReq.memberInfo.dwMemberID, 1);
    xDBBinder.SetData(&stForceReq.dwForceID, 4);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_CREATE(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FORCE_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 100);
    } else {
        xDBBinder.Close();
    }

    stForceReq.nErrorCode = nErrorCode;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 1);
    xSendDBPacket << stForceReq;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqJoinForce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140041960: 加入战队
    PS_FORCE_ADDMEMBER stForceReq;
    xPacket >> stForceReq;

    std::int32_t dwRecruitID = 0;
    xPacket.XParse >> dwRecruitID;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceReq.dwForceID, 1);
    xDBBinder.SetData(&stForceReq.stMember.dwMemberID, 1);
    xDBBinder.SetData(&stForceReq.stMember.nChannel, 4);
    xDBBinder.SetData(&stForceReq.nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_JOIN(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FORCE_JOIN ] [%d error] - Failed query( %d )", sqlReturn, 147);
    } else {
        xDBBinder.Close();
    }

    stForceReq.nErrorCode = nErrorCode;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 2);
    xSendDBPacket << stForceReq;
    xSendDBPacket.XParse << dwRecruitID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqLeaveForce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140041C00: 离开战队
    PS_FORCE_LEAVE stForceReq;
    xPacket >> stForceReq;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceReq.dwForceID, 1);
    xDBBinder.SetData(&stForceReq.dwLeaveMember, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_LEAVE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_LEAVE ] [%d error] - Failed query( %d )", sqlReturn, 102);
        nErrorCode = -1;
    }
    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 3);
    xSendDBPacket << stForceReq;
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqUpdateMemberInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140042130: 更新成员信息
    PS_FORCE_ADDMEMBER stForceReq;
    xPacket >> stForceReq;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceReq.dwForceID, 1);
    xDBBinder.SetData(&stForceReq.stMember.dwMemberID, 1);
    xDBBinder.SetData(&stForceReq.nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_MEMBER_UPDATE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_MEMBER_UPDATE ] [%d error] - Failed query", sqlReturn);
        stForceReq.nErrorCode = -1;
    }
    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 4);
    xSendDBPacket << stForceReq;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqForceChangeMaster(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140042340: 更换战队队长
    PS_FORCE_CHANGE_MASTER stForceReq;
    xPacket >> stForceReq;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceReq.dwForceID, 1);
    xDBBinder.SetData(&stForceReq.dwReqActorID, 1);
    xDBBinder.SetData(&stForceReq.dwNewMasterID, 1);
    xDBBinder.SetData(&stForceReq.nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_MASTER_CHANGE(?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_MASTER_CHANGE ] [%d error] - Failed query", sqlReturn);
        stForceReq.nErrorCode = -1;
    }
    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 5);
    xSendDBPacket << stForceReq;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqForceDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140042530: 删除战队
    PS_FORCE_DELETE stForceReq;
    xPacket >> stForceReq;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceReq.dwForceID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_DELETE(?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_DELETE ] [%d error] - Failed query", sqlReturn);
        nErrorCode = -1;
    }
    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 6);
    xSendDBPacket << stForceReq;
    xSendDBPacket.XParse << nErrorCode;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqForceUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400425A0: 更新战队信息
    std::int32_t nForceID = 0;
    std::int64_t nInsID = 0;
    xPacket.XParse >> nForceID;
    xPacket.XParse >> nInsID;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&nForceID, 1);
    xDBBinder.SetData(&nInsID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_UPDATEINFO(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FORCE_UPDATEINFO ] [%d error] - Failed query( %d )", sqlReturn, 374);
    } else {
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 8);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << nForceID;
    xSendDBPacket.XParse << nInsID;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqForceLoadAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400427F0: 加载所有战队信息 - 复杂实现，暂用简化版本
    // TODO: 完整实现需要遍历成员和分批发送
    PS_FORCE_INFO_ALL stForceInfoAll;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_FORCE_LOAD_ALL }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_LOAD_ALL ] [%d error] - Failed query( %d )", sqlReturn, 418);
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 0xB);
        xSendDBPacket.XParse << static_cast<std::int32_t>(-1);
        xSendDBPacket << stForceInfoAll;
        Send(xSendDBPacket);
        return sqlReturn;
    }

    PS_FORCE_INFO stForceInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        xDBBinder.GetData(&stForceInfo.dwForceID);
        xDBBinder.GetData(&stForceInfo.dwMaster);
        xDBBinder.GetData(reinterpret_cast<std::int64_t*>(&stForceInfo.uxMazeID));
        xDBBinder.GetData(&stForceInfo.byForceType);
        stForceInfoAll.vecForceInfo.push_back(stForceInfo);
    }
    xDBBinder.Close();

    // TODO: 完整实现需要加载成员信息 SP_FORCE_LOAD_ALL_WITH_MEMBER

    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 0xB);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << static_cast<std::int8_t>(1);  // bLast
    xSendDBPacket << stForceInfoAll;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqForceTypeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140043330: 更新战队类型
    std::uint32_t dwForceID = 0;
    xPacket.XParse >> dwForceID;

    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwForceID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_TYPE_UPDATE(?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_TYPE_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 586);
    }

    return sqlReturn;
}

std::int16_t XSQLForceProcess::ForceDelete(XDBStmt* pDBStmt, int xReturnSessionID, PS_FORCE_LEAVE* stForceLeave) {
    // Per IDA 0x140041F90: ForceDelete 辅助函数 - 调用 SP_FORCE_DELETE
    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stForceLeave->dwForceID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_DELETE(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FORCE_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 252);
    } else {
        xDBBinder.Close();
    }

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 6);
    xSendDBPacket << *stForceLeave;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int16_t XSQLForceProcess::LoadForceID(XDBStmt* pDBStmt, std::uint32_t dwMemberID, std::uint32_t* dwForceID) {
    // Per IDA 0x140041E60: 加载成员所属战队ID
    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwMemberID, 1);
    xDBBinder.SetData(dwForceID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_LOAD_ID(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_FORCE_LOAD_ID ] [%d error] - Failed query( %d )", sqlReturn, 227);
    } else {
        xDBBinder.Close();
    }

    return sqlReturn;
}

std::int32_t XSQLForceProcess::ReqForceMatchingCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140042F30: 创建匹配战队
    PS_DB_FORCE_MATCHING_CREATE psCreate;
    xPacket >> psCreate;

    std::int16_t sqlReturn = -1;
    std::int32_t nErrorCode = 0;
    std::uint32_t dwForceID = 0;

    // 删除需要删除的队伍 - 直接内联调用 SP_PARTY_DELETE
    for (auto it = psCreate.setDeleteParty.begin(); it != psCreate.setDeleteParty.end(); ++it) {
        std::uint32_t dwPartyID = *it;
        XDBBinder xDBBinderParty(pDBStmt);
        xDBBinderParty.SetData(&dwPartyID, 1);
        std::int16_t partySqlReturn = xDBBinderParty.Execute(
            reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_PARTY_DELETE(?)}")));
        if ((partySqlReturn & 0xFFFFFFFE) != 0) {
            if (partySqlReturn != 100) {
                xDBBinderParty.Close();
            }
            LogHelper::LogError("game.contents", "[ SP_PARTY_DELETE ] [%d error] - Failed query( %d )", partySqlReturn, 254);
        } else {
            xDBBinderParty.Close();
        }
        // 发送 Party 删除响应包
        PS_PARTY_LEAVE stPartyLeave;
        stPartyLeave.dwPartyID = dwPartyID;
        stPartyLeave.dwLeaveMember = static_cast<std::uint32_t>(-1);
        stPartyLeave.bKickout = false;
        XSendDBPacket xPartyPacket(xReturnSessionID, 4, 0x06);
        xPartyPacket << stPartyLeave;
        Send(xPartyPacket);
    }

    // 删除需要删除的战队
    for (auto it = psCreate.setDeleteForce.begin(); it != psCreate.setDeleteForce.end(); ++it) {
        PS_FORCE_LEAVE stForceLeave;
        stForceLeave.dwForceID = *it;
        stForceLeave.dwLeaveMember = static_cast<std::uint32_t>(-1);
        stForceLeave.bKickout = false;
        ForceDelete(pDBStmt, xReturnSessionID, &stForceLeave);
    }

    // 调用存储过程创建战队
    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psCreate.stCreateForce.dwLeaderUCID, 1);
    for (int i = 0; i < 8; ++i) {
        xDBBinder.SetData(&psCreate.stCreateForce.dwMemberUCID[i], 1);
    }
    xDBBinder.SetData(&dwForceID, 4);  // output
    xDBBinder.SetData(&nErrorCode, 4);  // output

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_FORCE_MATCHING_CREATE(?, ?,?,?,?,?,?,?,?, ?,?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_FORCE_MATCHING_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 557);
        nErrorCode = -1;
    }
    xDBBinder.Close();

    // 发送响应包
    XSendDBPacket xSendDBPacket(xReturnSessionID, 8, 0x0D);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << psCreate.stCreateForce.dwMatchingID;
    xSendDBPacket.XParse << dwForceID;
    Send(xSendDBPacket);

    return sqlReturn;
}

XSQLItemProcess::XSQLItemProcess() : TXSQLProcessBase<0x21>("XSQLItemProcess") {}
XSQLItemProcess::~XSQLItemProcess() = default;

std::int32_t XSQLItemProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004DD10: SubCmd routing for Item operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqItemInventoryInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqItemMove(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqItemCombine(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqItemDivide(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqItemBreak(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqItemLoadQuickSlot(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqItemUpdateQuickSlot(pDBStmt, xPacket, xReturnSessionID);
    case 0x0C: return ReqItemCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x0D: return ReqItemAddSlot(pDBStmt, xPacket, xReturnSessionID);
    case 0x0E: return ReqItemMoveMoney(pDBStmt, xPacket, xReturnSessionID);
    case 0x0F: return ReqItemBankInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqItemLineUp(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqItemReduce(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqItemUse(pDBStmt, xPacket, xReturnSessionID);
    case 0x22: return ReqItemUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqItemMazeRewardItem(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqItemDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqItemEquipSlotOpen(pDBStmt, xPacket, xReturnSessionID);
    case 0x26: return ReqItemUseInfoSelect(pDBStmt, xPacket, xReturnSessionID);
    case 0x27: return ReqItemUseInfoUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x28: return ReqItemMoveEx(pDBStmt, xPacket, xReturnSessionID);
    case 0x29: return ReqItemAppearanceLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x2A: return ReqItemAppearanceUse(pDBStmt, xPacket, xReturnSessionID);
    case 0x2B: return ReqItemAppearanceEnd(pDBStmt, xPacket, xReturnSessionID);
    case 0x2C: return ReqItemAppearanceEquip(pDBStmt, xPacket, xReturnSessionID);
    case 0x2D: return ReqItemNameChange(pDBStmt, xPacket, xReturnSessionID);
    case 0x30: return ReqItemSocketLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x31: return ReqItemBroachLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x34: return ReqItemUseAkashicRecord(pDBStmt, xPacket, xReturnSessionID);
    case 0x35: return ReqLoadAkashicRecord(pDBStmt, xPacket, xReturnSessionID);
    case 0x36: return ReqAkashicRegisterAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x37: return ReqAkashicReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x38: return ReqItemUpdateEx(pDBStmt, xPacket, xReturnSessionID);
    case 0x39: return ReqItemPostRestoreCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x40: return ReqItemLimitUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x41: return ReqItemLimitLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x42: return ReqItemDeleteReserveAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x43: return ReqItemDeleteReserveDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x44: return ReqItemDeleteReserveLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x46: return ReqItemCooltimeLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x47: return ReqItemCooltimeUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x48: return ReqItemUseCountUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x49: return ReqItemNetCafeCostume(pDBStmt, xPacket, xReturnSessionID);
    case 0x50: return ReqItemUpdatePos(pDBStmt, xPacket, xReturnSessionID);
    case 0x51: return ReqItemMakeLimitLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x52: return ReqItemMakeLimitDelete(pDBStmt, xPacket, xReturnSessionID);
    case 0x53: return ReqItemResealPackageInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x54: return ReqItemResealPackage(pDBStmt, xPacket, xReturnSessionID);
    case 0x55: return ReqItemUseResealPackage(pDBStmt, xPacket, xReturnSessionID);
    case 0x56: return ReqQuickslotCardDeckUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x57: return ReqQuickSlotCardDeckOpen(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

// XSQLItemProcess SubCmd handlers
std::int32_t XSQLItemProcess::ReqItemInventoryInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004E4A0: Load item inventory info
    std::int16_t sqlReturn = -1;

    std::uint32_t dwUCID = 0;
    std::uint8_t byInvenType = 0;
    std::uint8_t byLoad = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byInvenType;
    xPacket.XParse >> byLoad;

    PS_RES_STORAGE_INFO psResStorageInfo;
    std::int32_t nErrorCode = 0;

    sqlReturn = SelectItem(pDBStmt, dwUCID, byInvenType, psResStorageInfo);

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "ReqItemInventoryInfo [%d error] UCID(%d), TYPE(%d)",
                            sqlReturn, dwUCID, byInvenType);
        nErrorCode = sqlReturn;
    }

    // Send response packet MainCmd=0x21, SubCmd=0x01
    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x01);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << byInvenType;
    xSendDBPacket.XParse << byLoad;
    xSendDBPacket << psResStorageInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

// Per IDA 0x140054040: Select item inventory
std::int16_t XSQLItemProcess::SelectItem(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint8_t byInvenType, PS_RES_STORAGE_INFO& psResStorage) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwID, 1);
    xDBBinder.SetData(&byInvenType, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_SELECT(?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 1898);
        xDBBinder.Close();
        return sqlReturn;
    }

    PS_STORAGE_INFO stStorageInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stStorageInfo = PS_STORAGE_INFO{};

        xDBBinder.GetData(&stStorageInfo.shSlotPos);
        xDBBinder.GetData(&stStorageInfo.stItem.xSerial);
        xDBBinder.GetData(&stStorageInfo.byInvenType);
        xDBBinder.GetData(&stStorageInfo.stItem.nItemID);
        xDBBinder.GetData(&stStorageInfo.stItem.sCount);

        std::uint16_t wEndurance = 0;
        std::uint8_t byBindType = 0;
        xDBBinder.GetData(&wEndurance);
        xDBBinder.GetData(&byBindType);
        stStorageInfo.stItem.byEndurance = static_cast<std::uint8_t>(wEndurance);
        stStorageInfo.stItem.bBindType = byBindType != 0;

        xDBBinder.GetData(&stStorageInfo.stItem.eFlag);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgrade);

        for (int i = 0; i < 5; ++i) {
            std::uint8_t byType = 0;
            xDBBinder.GetData(&byType);
            stStorageInfo.stItem.stExtendOption[i].byType = byType;
            xDBBinder.GetData(&stStorageInfo.stItem.stExtendOption[i].nOption);
        }

        xDBBinder.GetData(&stStorageInfo.stItem.bySocketActiveCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nCashDate);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgradeCount);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgradeLimit);
        xDBBinder.GetData(&stStorageInfo.stItem.nExp);
        xDBBinder.GetString(stStorageInfo.stItem.szBroachState, 16);
        xDBBinder.GetData(&stStorageInfo.stItem.byRestoreCount);
        xDBBinder.GetData(&stStorageInfo.stItem.bySealCount);
        xDBBinder.GetData(&stStorageInfo.stItem.bySealDelCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nAttack);
        xDBBinder.GetData(&stStorageInfo.stItem.nDefense);
        xDBBinder.GetData(&stStorageInfo.stItem.nTitleID);
        xDBBinder.GetData(&stStorageInfo.stItem.byUseCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nDyeID);

        psResStorage.vecItem.push_back(stStorageInfo);
    }
    xDBBinder.Close();

    return sqlReturn;
}

// Per IDA 0x140054450: Select item inventory with flag
std::int16_t XSQLItemProcess::SelectItem(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint8_t byInvenType, std::uint8_t byFlag, PS_RES_STORAGE_INFO& psResStorage) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&byFlag, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_SELECT_FLAG(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_SELECT_FLAG ] [%d error] - Failed query( %d )", sqlReturn, 1966);
        xDBBinder.Close();
        return sqlReturn;
    }

    PS_STORAGE_INFO stStorageInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        stStorageInfo = PS_STORAGE_INFO{};

        xDBBinder.GetData(&stStorageInfo.shSlotPos);
        xDBBinder.GetData(&stStorageInfo.stItem.xSerial);
        xDBBinder.GetData(&stStorageInfo.byInvenType);
        xDBBinder.GetData(&stStorageInfo.stItem.nItemID);
        xDBBinder.GetData(&stStorageInfo.stItem.sCount);

        std::uint16_t wEndurance = 0;
        std::uint8_t byBindType = 0;
        xDBBinder.GetData(&wEndurance);
        xDBBinder.GetData(&byBindType);
        stStorageInfo.stItem.byEndurance = static_cast<std::uint8_t>(wEndurance);
        stStorageInfo.stItem.bBindType = byBindType != 0;

        xDBBinder.GetData(&stStorageInfo.stItem.eFlag);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgrade);

        for (int i = 0; i < 5; ++i) {
            std::uint8_t byType = 0;
            xDBBinder.GetData(&byType);
            stStorageInfo.stItem.stExtendOption[i].byType = byType;
            xDBBinder.GetData(&stStorageInfo.stItem.stExtendOption[i].nOption);
        }

        xDBBinder.GetData(&stStorageInfo.stItem.bySocketActiveCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nCashDate);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgradeCount);
        xDBBinder.GetData(&stStorageInfo.stItem.byUpgradeLimit);
        xDBBinder.GetData(&stStorageInfo.stItem.nExp);
        xDBBinder.GetString(stStorageInfo.stItem.szBroachState, 16);
        xDBBinder.GetData(&stStorageInfo.stItem.byRestoreCount);
        xDBBinder.GetData(&stStorageInfo.stItem.bySealCount);
        xDBBinder.GetData(&stStorageInfo.stItem.bySealDelCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nAttack);
        xDBBinder.GetData(&stStorageInfo.stItem.nDefense);
        xDBBinder.GetData(&stStorageInfo.stItem.nTitleID);
        xDBBinder.GetData(&stStorageInfo.stItem.byUseCount);
        xDBBinder.GetData(&stStorageInfo.stItem.nDyeID);

        psResStorage.vecItem.push_back(stStorageInfo);
    }
    xDBBinder.Close();

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemMove(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004EA50: 物品移动处理，遍历物品列表调用 MoveItem
    PS_DB_ITEM_MOVE_VEC psItem;
    bool bBroadCast = false;
    int nTicknum = 0;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> bBroadCast;
    xPacket >> psItem;
    xPacket.XParse >> nTicknum;

    // Iterate over all items in the vector
    for (auto& item : psItem.vecItems) {
        // Check if source item has valid serial
        if (item.stSrcItem.xSerial != -1) {
            sqlReturn = MoveItem(pDBStmt, item.dwSrcActorID, item.stSrcItem.xSerial,
                                 item.bySrcInvenType, item.shSrcSlotPos,
                                 static_cast<std::uint8_t>(item.stSrcItem.bBindType), item.bySrcStoreType);
            if (sqlReturn == -1) {
                break;
            }
        }
        // Check if destination item has valid serial
        if (item.stDestItem.xSerial != -1) {
            sqlReturn = MoveItem(pDBStmt, item.dwDestActorID, item.stDestItem.xSerial,
                                 item.byDestInvenType, item.shDestSlotPos,
                                 static_cast<std::uint8_t>(item.stDestItem.bBindType), item.byDestStoreType);
            if (sqlReturn == -1) {
                break;
            }
        }
    }

    nErrorCode = sqlReturn;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 2);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << bBroadCast;
    xSendDBPacket << psItem;
    xSendDBPacket.XParse << nTicknum;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int16_t XSQLItemProcess::MoveItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                       std::uint8_t byInvenType, std::int16_t shSlotPos,
                                       std::uint8_t byBindType, std::uint8_t byStoreType) {
    // Per IDA 0x14004FA80: 物品移动，调用 SP_ITEM_MOVE 存储过程
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byBindType, 1);
    xDBBinder.SetData(&byStoreType, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_MOVE(?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_MOVE ] [%d error] - Failed query( %d )", sqlReturn, 539);
    }

    xDBBinder.Close();

    if (nErrorCode) {
        LogHelper::LogDebug("game.contents", "*** Error MoveItem() [%d]", sqlReturn);
        sqlReturn = -1;
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemCombine(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004EE50: 物品合并处理，调用 SP_ITEM_COMBINE 存储过程
    PS_DB_ITEM_MOVE psDBItemMove;
    std::uint32_t dwActorID = 0;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwActorID;
    xPacket >> psDBItemMove;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&psDBItemMove.stSrcItem.xSerial, 1);
    xDBBinder.SetData(&psDBItemMove.stSrcItem.sCount, 1);
    xDBBinder.SetData(&psDBItemMove.stDestItem.xSerial, 1);
    xDBBinder.SetData(&psDBItemMove.stDestItem.sCount, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_COMBINE(?, ?, ?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_COMBINE ] [%d error] - Failed query( %d )", sqlReturn, 362);
        nErrorCode = 1;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 3);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << psDBItemMove;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemDivide(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004F250: 物品分割处理，调用两次 UpdateItem
    PS_DB_ITEM_MOVE psDBItemMove;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket >> psDBItemMove;

    // First update: source item
    sqlReturn = UpdateItem(pDBStmt, psDBItemMove.dwSrcActorID, psDBItemMove.stSrcItem.xSerial,
                           psDBItemMove.bySrcInvenType, psDBItemMove.shSrcSlotPos, &psDBItemMove.stSrcItem);

    if (sqlReturn != -1) {
        // Second update: destination item
        sqlReturn = UpdateItem(pDBStmt, psDBItemMove.dwDestActorID, psDBItemMove.stDestItem.xSerial,
                               psDBItemMove.byDestInvenType, psDBItemMove.shDestSlotPos, &psDBItemMove.stDestItem);
    }

    nErrorCode = sqlReturn;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 4);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << psDBItemMove;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int16_t XSQLItemProcess::UpdateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                         std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* pItem) {
    // Per IDA 0x14004FC30: 完整物品更新，调用 SP_ITEM_UPDATE 存储过程
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;
    std::int64_t cbTID = -3;

    XDBBinder xDBBinder(pDBStmt);
    unsigned char* szQuery = reinterpret_cast<unsigned char*>(const_cast<char*>(
        "{call SP_ITEM_UPDATE(?, ?, ?, ?, ?, ?, ?, ?, ?, ?,  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )}"));

    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&pItem->nItemID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&pItem->sCount, 1);
    std::uint16_t wEndurance = pItem->byEndurance;
    xDBBinder.SetData(&wEndurance, 1);
    std::uint8_t byBindType = pItem->bBindType;
    xDBBinder.SetData(&byBindType, 1);
    xDBBinder.SetData(&pItem->eFlag, 1);
    xDBBinder.SetData(&pItem->byUpgrade, 1);

    // Set extend options (5 options, each has byType + nOption)
    for (int i = 0; i < 5; ++i) {
        std::uint8_t byType = pItem->stExtendOption[i].byType;
        xDBBinder.SetData(&byType, 1);
        xDBBinder.SetData(&pItem->stExtendOption[i].nOption, 1);
    }

    xDBBinder.SetData(&pItem->bySocketActiveCount, 1);
    xDBBinder.SetData(&pItem->nCashDate, 1);
    xDBBinder.SetData(&pItem->byUpgradeCount, 1);
    xDBBinder.SetData(&pItem->byUpgradeLimit, 1);
    xDBBinder.SetString(pItem->szBroachState, 16, &cbTID, 1);
    xDBBinder.SetData(&pItem->byRestoreCount, 1);
    xDBBinder.SetData(&pItem->bySealCount, 1);
    xDBBinder.SetData(&pItem->bySealDelCount, 1);
    xDBBinder.SetData(&pItem->nAttack, 1);
    xDBBinder.SetData(&pItem->nDefense, 1);
    xDBBinder.SetData(&pItem->nTitleID, 1);
    xDBBinder.SetData(&pItem->nExp, 1);
    xDBBinder.SetData(&pItem->byUseCount, 1);
    xDBBinder.SetData(&pItem->nDyeID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(szQuery);

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 619);
    }

    xDBBinder.Close();

    if (nErrorCode) {
        LogHelper::LogDebug("game.contents", "UCID %d ItemID %d update failed with error %d", dwUCID, pItem->nItemID, nErrorCode);
    } else {
        LogHelper::LogDebug("game.contents", "UCID %d ItemID %d Serial %lld updated successfully", dwUCID, pItem->nItemID, biSerial);
    }

    return static_cast<std::int16_t>(nErrorCode);
}

std::int16_t XSQLItemProcess::UpdateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                         std::uint8_t byInvenType, std::int16_t shSlotPos) {
    // Per IDA 0x140050190: 简化物品更新，仅更新位置信息
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    unsigned char* szQuery = reinterpret_cast<unsigned char*>(const_cast<char*>(
        "{call SP_ITEM_UPDATE_POS(?, ?, ?, ?, ?)}"));

    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(szQuery);

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_UPDATE_POS ] [%d error] - Failed query", sqlReturn);
        nErrorCode = 1;
    }

    xDBBinder.Close();
    return static_cast<std::int16_t>(nErrorCode);
}

std::int16_t XSQLItemProcess::LoadQuickSlotItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_ITEM* pQuickSlotInfo) {
    // IDA 0x1400514F0: 加载快捷栏物品
    std::int16_t sqlReturn = -1;

    pQuickSlotInfo->uxActorID = dwUCID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_QUICKSLOT_LOAD_ITEM( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_QUICKSLOT_LOAD_ITEM ] [%d error] - Failed query( %d )", sqlReturn, 1136);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(&pQuickSlotInfo->dwItem_1);
            xDBBinder.GetData(&pQuickSlotInfo->dwItem_2);
            xDBBinder.GetData(&pQuickSlotInfo->dwItem_3);
            xDBBinder.GetData(&pQuickSlotInfo->dwItem_4);
        }
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int16_t XSQLItemProcess::LoadQuickSlotCard(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_CARD_VEC* pQuickSlotCard) {
    // IDA 0x140051640: 加载快捷栏卡片
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_QUICKSLOT_LOAD_AKASHIC( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_QUICKSLOT_LOAD_AKASHIC ] [%d error] - Failed query( %d )", sqlReturn, 1179);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            PS_QUICKSLOT_CARD psCard{};
            std::uint8_t byActive = 0;

            xDBBinder.GetData(&psCard.byPage);
            xDBBinder.GetData(&psCard.nCard_1);
            xDBBinder.GetData(&psCard.nCard_2);
            xDBBinder.GetData(&psCard.nCard_3);
            xDBBinder.GetData(&psCard.nCard_4);
            xDBBinder.GetData(&psCard.nCard_5);
            xDBBinder.GetWString(psCard.szDeckName, 26);
            sqlReturn = xDBBinder.GetData(&byActive);

            if (byActive) {
                pQuickSlotCard->byActivePage = psCard.byPage;
            }
            pQuickSlotCard->vecInfo.push_back(psCard);
        }
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemBreak(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // IDA 0x14004F2A0: 物品分解处理
    std::uint32_t dwUCID = 0;
    std::uint8_t byInvenType = 0;
    std::int16_t shSlotPos = 0;
    std::uint32_t nItemID = 0;
    std::int64_t biSerial = 0;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byInvenType;
    xPacket.XParse >> shSlotPos;
    xPacket.XParse >> nItemID;
    xPacket.XParse >> biSerial;

    sqlReturn = DeleteItem(pDBStmt, dwUCID, biSerial, byInvenType, shSlotPos);
    nErrorCode = sqlReturn;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x05);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << byInvenType;
    xSendDBPacket.XParse << shSlotPos;
    xSendDBPacket.XParse << nItemID;
    xSendDBPacket.XParse << biSerial;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUpdateQuickSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // IDA 0x140051840: 快捷栏物品更新
    PS_QUICKSLOT_ITEM stItemSlot{};
    std::int16_t sqlReturn = -1;

    xPacket >> stItemSlot;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&stItemSlot.uxActorID, 1);
    xDBBinder.SetData(&stItemSlot.dwItem_1, 1);
    xDBBinder.SetData(&stItemSlot.dwItem_2, 1);
    xDBBinder.SetData(&stItemSlot.dwItem_3, 1);
    xDBBinder.SetData(&stItemSlot.dwItem_4, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_QUICKSLOT_UPDATE_ITEM(?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0 && sqlReturn != 100) {
        LogHelper::LogError("game.contents", "[ SP_QUICKSLOT_UPDATE_ITEM ] [%d error] - Failed query( %d )", sqlReturn, 1206);
    }

    xDBBinder.Close();
    return (sqlReturn == 100) ? sqlReturn : 0;
}

std::int32_t XSQLItemProcess::ReqItemLoadQuickSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // IDA 0x140051330: 快捷栏物品加载
    std::uint32_t dwUCID = 0;
    PS_QUICKSLOT_ITEM stQuickSlotItem{};
    PS_QUICKSLOT_CARD_VEC psQuikSlotCard;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;

    sqlReturn = LoadQuickSlotItem(pDBStmt, dwUCID, &stQuickSlotItem);
    if (sqlReturn == -1) {
        nErrorCode = 1;
    }

    sqlReturn = LoadQuickSlotCard(pDBStmt, dwUCID, &psQuikSlotCard);
    if (sqlReturn == -1) {
        nErrorCode = 2;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x06);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stQuickSlotItem;
    xSendDBPacket << psQuikSlotCard;
    Send(xSendDBPacket);

    return 0;
}

std::int32_t XSQLItemProcess::ReqItemCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004F5E0: 物品创建请求
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO vecCreateItem;
    PS_RES_STORAGE_INFO vecUpdateItem;
    std::uint8_t byLock[15] = {};
    std::uint8_t byCreateType = 0;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    for (int i = 0; i < 15; ++i) {
        xPacket.XParse >> byLock[i];
    }
    xPacket >> vecCreateItem;
    xPacket >> vecUpdateItem;
    xPacket.XParse >> byCreateType;

    // First, update existing items
    for (const auto& info : vecUpdateItem.vecItem) {
        sqlReturn = UpdateItemCount(pDBStmt, dwUCID, info.stItem.xSerial, info.stItem.sCount);
    }

    // Check create items
    for (const auto& info : vecCreateItem.vecItem) {
        sqlReturn = CheckCreateItem(pDBStmt, dwUCID, info.byInvenType, info.shSlotPos,
                                    info.stItem.nItemID, info.stItem.xSerial, byCreateType);
    }

    // Create items
    for (const auto& info : vecCreateItem.vecItem) {
        sqlReturn = CreateItem(pDBStmt, dwUCID, info.byInvenType, info.shSlotPos,
                               const_cast<STItem*>(&info.stItem), &nErrorCode);
    }

    if (sqlReturn == -1) {
        nErrorCode = 1;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x0C);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << byLock[0];
    xSendDBPacket << vecCreateItem;
    xSendDBPacket << vecUpdateItem;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemAddSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140050A30: 扩展背包槽位处理
    std::uint32_t dwUAID = 0;
    int nUCID = 0;
    std::uint8_t byInvenType = 0;
    std::uint8_t byExtendStep = 0;
    std::int16_t shAddSlot = 0;
    std::int64_t biMoney = 0;
    PS_RES_STORAGE_INFO stUpdateItem;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nUCID;
    xPacket.XParse >> byInvenType;
    xPacket.XParse >> byExtendStep;
    xPacket.XParse >> shAddSlot;
    xPacket.XParse >> biMoney;
    xPacket >> stUpdateItem;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&byExtendStep, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_EXTENDSLOTSTEP_UPDATE(?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_EXTENDSLOTSTEP_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 952);
    }

    xDBBinder.Close();

    // Update/delete items in the list
    for (const auto& info : stUpdateItem.vecItem) {
        if (info.stItem.sCount > 0) {
            sqlReturn = UpdateItemCount(pDBStmt, nUCID, info.stItem.xSerial, info.stItem.sCount);
        } else {
            sqlReturn = DeleteItem(pDBStmt, nUCID, info.stItem.xSerial, info.byInvenType, info.shSlotPos);
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x0D);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << byInvenType;
    xSendDBPacket.XParse << byExtendStep;
    xSendDBPacket.XParse << shAddSlot;
    xSendDBPacket.XParse << biMoney;
    xSendDBPacket << stUpdateItem;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemMoveMoney(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140057810: 金币移动（背包/银行之间）
    PS_DB_MOVE_MONEY psDBMoveMoney;
    PS_RES_MOVE_MONEY psResMoveMoney;
    std::int16_t sqlReturn = -1;

    xPacket >> psDBMoveMoney;

    XDBBinder xDBBinder(pDBStmt);

    if (psDBMoveMoney.psMoveMoneyInfo.byTargetType == 0) {
        // Move money to inventory
        sqlReturn = xDBBinder.SetData(&psDBMoveMoney.dwUCID, 1);
        sqlReturn = xDBBinder.SetData(&psDBMoveMoney.dwUAID, 1);
        sqlReturn = xDBBinder.SetData(&psDBMoveMoney.psMoveMoneyInfo.biMoney, 1);
        sqlReturn = xDBBinder.SetData(&psResMoveMoney.nInvenMoney, 4);
        sqlReturn = xDBBinder.SetData(&psResMoveMoney.nBankMoney, 4);
        sqlReturn = xDBBinder.SetData(&psResMoveMoney.nResult, 4);
        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_MOVE_MONEY_TO_INVEN(?, ?, ?, ?, ?, ?)}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0 && sqlReturn != 100) {
            psResMoveMoney.nResult = sqlReturn;
            sqlReturn = -1;
        }
    } else if (psDBMoveMoney.psMoveMoneyInfo.byTargetType == 1) {
        // Move money to bank
        sqlReturn = xDBBinder.SetData(&psDBMoveMoney.dwUCID, 1);
        sqlReturn = xDBBinder.SetData(&psDBMoveMoney.dwUAID, 1);
        sqlReturn = xDBBinder.SetData(&psDBMoveMoney.psMoveMoneyInfo.biMoney, 1);
        sqlReturn = xDBBinder.SetData(&psResMoveMoney.nInvenMoney, 4);
        sqlReturn = xDBBinder.SetData(&psResMoveMoney.nBankMoney, 4);
        sqlReturn = xDBBinder.SetData(&psResMoveMoney.nResult, 4);
        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_MOVE_MONEY_TO_BANK(?, ?, ?, ?, ?, ?)}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0 && sqlReturn != 100) {
            psResMoveMoney.nResult = sqlReturn;
            sqlReturn = -1;
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x0E);
    xSendDBPacket << psResMoveMoney;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemBankInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004E690: 银行物品信息请求
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byInvenType = 0;
    std::uint8_t byLoad = 0;
    std::int64_t nBankMoney = 0;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;
    PS_RES_STORAGE_INFO psResStorageInfo;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byInvenType;
    xPacket.XParse >> byLoad;

    // Get nation type to decide which ID to use (per IDA)
    // NationType == 2 (KR) uses UCID, others use UAID
    // TODO: 需人工审查 - 需要XDBAgent的GetNationType实现
    sqlReturn = SelectItem(pDBStmt, dwUCID, byInvenType, psResStorageInfo);

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.item", "ReqItemBankInfo [%d error] UCID(%d), UAID(%d), TYPE(%d)",
                           sqlReturn, dwUCID, dwUAID, byInvenType);
        // TODO: 需人工审查 - SendDBErrorMsg需要正确实现
        return sqlReturn;
    }

    // Load bank money if requested
    if (byLoad) {
        XDBBinder xDBBinder(pDBStmt);
        sqlReturn = xDBBinder.SetData(&dwUAID, 1);
        sqlReturn = xDBBinder.SetData(&nBankMoney, 4);
        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_SELECT_BANKMONEY(?, ?)}")));

        if ((sqlReturn & 0xFFFFFFFE) != 0) {
            if (sqlReturn != 100) {
                sqlReturn = -1;
            }
            LogHelper::LogError("game.contents", "[ SP_ITEM_SELECT_BANKMONEY ] [%d error] - Failed query( %d )", sqlReturn, 272);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x0F);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << byInvenType;
    xSendDBPacket.XParse << byLoad;
    xSendDBPacket.XParse << nBankMoney;
    xSendDBPacket << psResStorageInfo;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemLineUp(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140051160: 物品整理请求
    std::uint32_t dwActorID = 0;
    PS_ITEM_LINE_UP_VEC stLineUpVec;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwActorID;
    xPacket >> stLineUpVec;

    // Iterate through lineup items and update/delete as needed
    for (const auto& item : stLineUpVec.vecItem) {
        if (item.shCount > 0) {
            // Update item count
            sqlReturn = UpdateItemCount(pDBStmt, dwActorID, item.xItemID, item.shCount);
        } else {
            // Delete item
            sqlReturn = DeleteItem(pDBStmt, dwActorID, item.xItemID, item.byInvenType, item.shSlotPos);
        }
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemReduce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004F490: 物品数量减少请求
    PS_DB_ITEM_REDUCE stItemReduce;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket >> stItemReduce;

    sqlReturn = UpdateItemCount(pDBStmt, stItemReduce.dwUCID, stItemReduce.xSerial, stItemReduce.nReduceCount);

    if (nErrorCode) {
        LogHelper::LogError("game.contents", "*** Error ReduceItem() [%d]", nErrorCode);
        sqlReturn = -1;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x11);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << stItemReduce;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140052710: 物品使用请求
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO vecCreateItem;
    PS_RES_STORAGE_INFO vecUpdateItem;
    ST_GET_INFO stGetInfo;
    PS_GACHA_LIST stGachaList;
    std::uint32_t dwItemID = 0;
    std::uint8_t byState = 0;
    std::uint8_t byFlag = 0;
    std::int64_t biItemSerial = 0;
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> vecUpdateItem;
    xPacket >> vecCreateItem;
    xPacket >> stGetInfo;
    xPacket.XParse >> dwItemID;
    xPacket.XParse >> byState;
    xPacket >> stGachaList;
    xPacket.XParse >> byFlag;
    xPacket.XParse >> biItemSerial;

    // Update/delete items
    for (const auto& info : vecUpdateItem.vecItem) {
        if (info.stItem.sCount > 0) {
            sqlReturn = UpdateItemCount(pDBStmt, dwUCID, info.stItem.xSerial, info.stItem.sCount);
        } else {
            sqlReturn = DeleteItem(pDBStmt, dwUCID, info.stItem.xSerial, info.byInvenType, info.shSlotPos);
        }
    }

    // Check create items
    for (const auto& info : vecCreateItem.vecItem) {
        sqlReturn = CheckCreateItem(pDBStmt, dwUCID, info.byInvenType, info.shSlotPos,
                                    info.stItem.nItemID, info.stItem.xSerial, byFlag, biItemSerial);
    }

    // Update created items
    for (const auto& info : vecCreateItem.vecItem) {
        sqlReturn = UpdateItem(pDBStmt, dwUCID, info.stItem.xSerial, info.byInvenType, info.shSlotPos,
                               const_cast<STItem*>(&info.stItem));
    }

    nErrorCode = sqlReturn;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x12);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << vecUpdateItem;
    xSendDBPacket << vecCreateItem;
    xSendDBPacket << stGetInfo;
    xSendDBPacket.XParse << dwItemID;
    xSendDBPacket.XParse << biItemSerial;
    xSendDBPacket.XParse << byState;
    xSendDBPacket << stGachaList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140051E60: 物品更新处理，处理更新列表和创建列表
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO vecUpdateItem;
    PS_RES_STORAGE_INFO vecCreateItem;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> vecUpdateItem;
    xPacket >> vecCreateItem;

    // 遍历更新列表：如果数量>0调用UpdateItemCount，否则调用DeleteItem
    for (const auto& stInfo : vecUpdateItem.vecItem) {
        if (stInfo.stItem.sCount != 0) {
            sqlReturn = UpdateItemCount(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.stItem.sCount);
        } else {
            sqlReturn = DeleteItem(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos);
        }
    }

    // 遍历创建列表：调用完整UpdateItem
    for (const auto& stInfo : vecCreateItem.vecItem) {
        sqlReturn = UpdateItem(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos, &const_cast<STItem&>(stInfo.stItem));
    }

    nErrorCode = sqlReturn;

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x22);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << vecUpdateItem;
    xSendDBPacket << vecCreateItem;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemMazeRewardItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x1400521F0: 迷宫奖励物品请求
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO stCreateItem;
    PS_RES_STORAGE_INFO stUpdateItem;
    ST_CREATE_ITEM stRewardItem;
    ST_CREATE_ITEM stReduceItem;
    std::uint8_t byType = 0;
    std::uint8_t byFlag = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> stCreateItem;
    xPacket >> stUpdateItem;
    xPacket >> stRewardItem;
    xPacket >> stReduceItem;
    xPacket.XParse >> byType;
    xPacket.XParse >> byFlag;

    std::int16_t sqlReturn = -1;

    // 处理更新物品
    for (auto& info : stUpdateItem.vecItem) {
        if (info.stItem.sCount) {
            sqlReturn = UpdateItemCount(pDBStmt, dwUCID, info.stItem.xSerial, info.stItem.sCount);
        } else {
            sqlReturn = DeleteItem(pDBStmt, dwUCID, info.stItem.xSerial, info.byInvenType, info.shSlotPos);
        }
        if (sqlReturn == -1) break;
    }

    // 处理检查创建物品
    if (sqlReturn != -1) {
        for (auto& info : stCreateItem.vecItem) {
            sqlReturn = CheckCreateItem(pDBStmt, dwUCID, info.stItem.nItemID, info.byInvenType, info.shSlotPos,
                                        info.stItem.xSerial, byFlag);
            if (sqlReturn == -1) break;
        }
    }

    // 处理更新创建物品
    if (sqlReturn != -1) {
        for (auto& info : stCreateItem.vecItem) {
            sqlReturn = UpdateItem(pDBStmt, dwUCID, info.stItem.xSerial, info.byInvenType, info.shSlotPos, &info.stItem);
            if (sqlReturn == -1) break;
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 33, 0x23u);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket << stCreateItem;
    xSendDBPacket << stUpdateItem;
    xSendDBPacket << stRewardItem;
    xSendDBPacket << stReduceItem;
    xSendDBPacket.XParse << byType;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140052CD0: 物品删除处理
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    int nCount = 0;
    std::uint8_t byInvenType = 0;
    std::int16_t shSlot = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;
    xPacket.XParse >> nCount;
    xPacket.XParse >> byInvenType;
    xPacket.XParse >> shSlot;

    if (nCount != 0) {
        sqlReturn = UpdateItemCount(pDBStmt, dwUCID, biSerial, static_cast<std::int16_t>(nCount));
    } else {
        sqlReturn = DeleteItem(pDBStmt, dwUCID, biSerial, byInvenType, shSlot);
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemEquipSlotOpen(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140053030: 装备槽位开放处理
    std::uint32_t dwActorID = 0;
    PS_RES_STORAGE_INFO stUpdateItem;
    int nEquipPosBit = 0;
    std::uint8_t byEquipPos = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwActorID;
    xPacket >> stUpdateItem;
    xPacket.XParse >> nEquipPosBit;
    xPacket.XParse >> byEquipPos;

    // 遍历更新列表：如果数量>0调用UpdateItemCount，否则调用DeleteItem
    for (const auto& stInfo : stUpdateItem.vecItem) {
        if (stInfo.stItem.sCount != 0) {
            sqlReturn = UpdateItemCount(pDBStmt, dwActorID, stInfo.stItem.xSerial, stInfo.stItem.sCount);
        } else {
            sqlReturn = DeleteItem(pDBStmt, dwActorID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos);
        }
    }

    sqlReturn = EquipSlotOpen(pDBStmt, dwActorID, nEquipPosBit);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x25);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket << stUpdateItem;
    xSendDBPacket.XParse << nEquipPosBit;
    xSendDBPacket.XParse << byEquipPos;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int16_t XSQLItemProcess::EquipSlotOpen(XDBStmt* pDBStmt, std::uint32_t dwActorID, int nEquipPosBit) {
    // Per IDA 0x1400548B0: 调用 SP_EQUIP_SLOT_OPEN 存储过程
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&nEquipPosBit, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_EQUIP_SLOT_OPEN(?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_EQUIP_SLOT_OPEN ] [%d error] - Failed query( %d )", sqlReturn, 1991);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUseInfoSelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140055A50: 查询物品使用信息请求
    std::uint32_t dwActorID = 0;
    ST_USE_ITEM_INFO_LIST stUseItemInfoList;

    xPacket.XParse >> dwActorID;

    std::int16_t sqlReturn = UseItemInfoSelect(pDBStmt, dwActorID, &stUseItemInfoList);

    XSendDBPacket xSendDBPacket(xReturnSessionID, 33, 0x26u);
    xSendDBPacket << stUseItemInfoList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUseInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140054930: 更新物品使用信息请求
    std::uint32_t dwUCID = 0;
    ST_USE_ITEM_INFO_LIST stUseItemInfoList;

    xPacket.XParse >> dwUCID;
    xPacket >> stUseItemInfoList;

    std::int16_t sqlReturn = 0;
    for (const auto& info : stUseItemInfoList.vecInfo) {
        ST_USE_ITEM_INFO stInfo = info;
        sqlReturn = UseItemInfoUpdate(pDBStmt, dwUCID, &stInfo);
        if (sqlReturn == -1) break;
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 33, 0x27u);
    xSendDBPacket << stUseItemInfoList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemMoveEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140054AF0: 物品移动（扩展版本）请求
    std::uint8_t bBroadCast = 0;
    PS_DB_ITEM_MOVE_VEC psItem;
    int nTicknum = 0;

    xPacket.XParse >> bBroadCast;
    xPacket >> psItem;
    xPacket.XParse >> nTicknum;

    std::int16_t sqlReturn = -1;

    for (auto& item : psItem.vecItems) {
        // 处理源物品（xSerial != -1 表示需要移动）
        if (item.stSrcItem.xSerial != -1) {
            sqlReturn = MoveItemEx(pDBStmt, item.dwSrcActorID, item.stSrcItem.xSerial,
                                   item.bySrcInvenType, item.shSrcSlotPos,
                                   item.stSrcItem.bBindType, item.stSrcItem.nCashDate, item.bySrcStoreType);
            if (sqlReturn == -1) break;
        }

        // 处理目标物品（xSerial != -1 表示需要移动）
        if (item.stDestItem.xSerial != -1) {
            sqlReturn = MoveItem(pDBStmt, item.dwDestActorID, item.stDestItem.xSerial,
                                 item.byDestInvenType, item.shDestSlotPos,
                                 item.stDestItem.bBindType, item.byDestStoreType);
            if (sqlReturn == -1) break;
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 33, 2u);
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket.XParse << bBroadCast;
    xSendDBPacket << psItem;
    xSendDBPacket.XParse << nTicknum;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemAppearanceLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400554F0: 加载外观列表
    std::uint32_t dwUCID = 0;
    ST_APPEARANCE_LIST stAppearanceList;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_APPEARANCE_LOAD( ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_APPEARANCE_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 2216);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_APPEARANCE_INFO stInfo{};
            xDBBinder.GetData(&stInfo.wAppearanceID);
            xDBBinder.GetData(&stInfo.biEndDate);
            stAppearanceList.vecInfo.push_back(stInfo);
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x29);
    xSendDBPacket << stAppearanceList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemAppearanceUse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemAppearanceEnd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400556F0: 外观结束处理（简化实现）
    std::uint32_t dwUCID = 0;
    std::int64_t biAppearance = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biAppearance;

    // 调用 AppearanceUpdate 设置结束时间
    sqlReturn = AppearanceUpdate(pDBStmt, dwUCID, static_cast<std::uint16_t>(biAppearance), 0);

    return sqlReturn;
}

std::int16_t XSQLItemProcess::AppearanceUpdate(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint16_t wAppearanceID, std::int64_t biEndDate) {
    // Per IDA 0x140055970: 调用 SP_ITEM_APPEARANCE_UPDATE 存储过程
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&wAppearanceID, 1);
    xDBBinder.SetData(&biEndDate, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_APPEARANCE_UPDATE( ?, ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_APPEARANCE_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 2290);
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemAppearanceEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140055860: 装备外观
    std::uint32_t dwUCID = 0;
    std::int64_t biAppearance = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biAppearance;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biAppearance, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_APPEARANCE_EQUIP( ?, ? )}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_APPEARANCE_EQUIP ] [%d error] - Failed query( %d )", sqlReturn, 2272);
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemNameChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemSocketLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400562F0: 加载物品槽位列表
    std::uint32_t dwUCID = 0;
    std::uint8_t byStorageType = 0;
    PS_ITEM_SOCKET_LIST stSocketList;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byStorageType;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byStorageType, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_SOCKET_LOAD( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_SOCKET_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 2450);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_ITEM_SOCKET stSocket{};
            xDBBinder.GetData(&stSocket.biEquipSerial);
            xDBBinder.GetData(&stSocket.dwSocketID);
            xDBBinder.GetData(&stSocket.bySocketPos);

            for (int i = 0; i < 5; ++i) {
                std::uint8_t byType = 0;
                xDBBinder.GetData(&byType);
                stSocket.stExtendOption[i].byType = byType;
                xDBBinder.GetData(&stSocket.stExtendOption[i].nOption);
            }

            stSocketList.vecInfo.push_back(stSocket);
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x30);
    xSendDBPacket << stSocketList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemBroachLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400563A0: 加载物品镂刻列表
    std::uint32_t dwUCID = 0;
    std::uint8_t byStorageType = 0;
    PS_ITEM_BROACH_LIST stBroachList;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byStorageType;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byStorageType, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_BROACH_LOAD( ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_BROACH_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 2489);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_ITEM_BROACH stBroach{};
            xDBBinder.GetData(&stBroach.biSerial);
            for (int i = 0; i < 15; ++i) {
                xDBBinder.GetData(&stBroach.dwItemID[i]);
            }
            stBroachList.vecInfo.push_back(stBroach);
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x31);
    xSendDBPacket << stBroachList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUseAkashicRecord(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA: 使用阿卡夏记录（简化实现）
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biSerial;

    // TODO: 完整实现需要更多参数
    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqLoadAkashicRecord(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400566B0: 加载阿卡夏记录列表
    std::uint32_t dwUCID = 0;
    ST_AKASHIC_LIST stAkashicList;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_AKASHIC_LOAD(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_AKASHIC_LOAD ] [%d error] - Failed query( %d )", sqlReturn, 2535);
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_AKASHIC_RECORD stInfo{};
            std::uint8_t byState = 0;
            xDBBinder.GetData(&stInfo.dwAkashicID);
            xDBBinder.GetData(&byState);
            stInfo.byState = byState;
            xDBBinder.GetData(&stInfo.nAkashicExp);

            // 只添加状态不为1的记录
            if (byState != 1) {
                stAkashicList.vecInfo.push_back(stInfo);
            }
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x35);
    xSendDBPacket << stAkashicList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqAkashicRegisterAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140056870: 注册所有阿卡夏记录
    std::uint32_t dwUCID = 0;
    ST_AKASHIC_LIST stAkashicList;
    std::int16_t sqlReturn = -1;
    std::uint8_t byState = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> stAkashicList;

    for (std::size_t i = 0; i < stAkashicList.vecInfo.size(); ++i) {
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&dwUCID, 1);
        xDBBinder.SetData(&stAkashicList.vecInfo[i].dwAkashicID, 1);
        xDBBinder.SetData(&byState, 1);
        xDBBinder.SetData(&stAkashicList.vecInfo[i].nAkashicExp, 1);
        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ADD_AKASHIC_RECORD(?,?,?,?) }")));
        xDBBinder.Close();
    }

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ADD_AKASHIC_RECORD ] [%d error] - Failed query( %d )", sqlReturn, 2573);
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqAkashicReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemUpdateEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400570D0: 物品扩展更新处理
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO psUpdateItem;
    std::int16_t sqlReturn = -1;

    xPacket.XParse >> dwUCID;
    xPacket >> psUpdateItem;

    // 遍历更新列表，调用UpdateItem
    for (const auto& stInfo : psUpdateItem.vecItem) {
        sqlReturn = UpdateItem(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos, &const_cast<STItem&>(stInfo.stItem));
        if (sqlReturn != 0) {
            break;
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x38);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket.XParse << sqlReturn;
    xSendDBPacket << psUpdateItem;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemPostRestoreCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140057550: 从邮件恢复列表创建物品
    std::uint32_t dwUCID = 0;
    PS_ITEM_RESTORE_LIST psCreateItemInfo;
    std::uint8_t byType = 0;
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> psCreateItemInfo;
    xPacket.XParse >> byType;

    // 遍历恢复列表，调用 CreateItem
    for (const auto& stInfo : psCreateItemInfo.vecInfo) {
        STItem stItem = stInfo.stItem;
        sqlReturn = CreateItem(pDBStmt, dwUCID, stInfo.byInvenType, stInfo.shSlotPos, &stItem, &nErrorCode);
        if (sqlReturn != 0) {
            break;
        }
    }

    if (sqlReturn == -1) {
        LogHelper::LogError("game.contents", "[ ReqItemPostRestoreCreate error - Failed query[ ErrorCode:%d ] (%d)",
                           nErrorCode, 2787);
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x39);
    xSendDBPacket << psCreateItemInfo;
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket.XParse << byType;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemLimitUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemLimitLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemDeleteReserveAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400581F0: 添加删除预约
    std::uint32_t dwUCID = 0;
    PS_DELETE_RESERVE_ITEM stReduceItem;
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> stReduceItem.dwItemID;
    xPacket.XParse >> stReduceItem.nCount;
    xPacket.XParse >> stReduceItem.nMapID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&stReduceItem.dwItemID, 1);
    xDBBinder.SetData(&stReduceItem.nCount, 1);
    xDBBinder.SetData(&stReduceItem.nMapID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_DELETE_RESERVE_ADD(?, ?, ?, ?, ?)}")));
    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "SP_ITEM_DELETE_RESERVE_ADD [%d, %d error] UCID(%d)", sqlReturn, nErrorCode, dwUCID);
        sqlReturn = -1;
    }

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemDeleteReserveDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140058380: 删除预约物品
    std::uint32_t dwUCID = 0;
    PS_DELETE_RESERVE_ITEM stReduceItem;
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> stReduceItem.dwItemID;
    xPacket.XParse >> stReduceItem.nCount;
    xPacket.XParse >> stReduceItem.nMapID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&stReduceItem.dwItemID, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_DELETE_RESERVE_DEL(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0 || nErrorCode) {
        LogHelper::LogError("game.contents", "SP_ITEM_DELETE_RESERVE_DEL [%d, %d error] UCID(%d)", sqlReturn, nErrorCode, dwUCID);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemDeleteReserveLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140058CE0: 加载删除预约列表
    std::uint32_t dwUCID = 0;
    PS_DELETE_RESERVE_ITEM_LIST stReserveList;
    std::int16_t sqlReturn = 0;

    xPacket.XParse >> dwUCID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_DELETE_RESERVE_LOAD(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) == 0) {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            PS_DELETE_RESERVE_ITEM stReduceItem{};
            xDBBinder.GetData(&stReduceItem.nMapID);
            xDBBinder.GetData(&stReduceItem.nCount);
            xDBBinder.GetData(&stReduceItem.dwItemID);
            stReserveList.vecInfo.push_back(stReduceItem);
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x44);
    xSendDBPacket << stReserveList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemCooltimeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400584E0: 加载物品冷却时间列表
    std::uint32_t dwUCID = 0;
    std::int64_t biCurDate = 0;
    std::int16_t sqlReturn = -1;
    PS_ITEM_COOMTIME_LIST psCooltimeList;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biCurDate;

    XDBBinder xDBBinder(pDBStmt);
    sqlReturn = xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&biCurDate, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ITEM_COOLTIME_LOAD(?,?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "SP_ITEM_COOLTIME_LOAD[UCID:%d]", dwUCID);
        xDBBinder.Close();
        return sqlReturn;
    }

    PS_ITEM_COOLTIME_INFO psCooltimeInfo;
    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        sqlReturn = xDBBinder.GetData(&psCooltimeInfo.byCooltimeGroupID);
        sqlReturn = xDBBinder.GetData(&psCooltimeInfo.biRemainDate);
        sqlReturn = xDBBinder.GetData(&psCooltimeInfo.biCooltimeValue);
        psCooltimeList.vecInfo.push_back(psCooltimeInfo);
    }
    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x46);
    xSendDBPacket << psCooltimeList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemCooltimeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140058BA0: 更新物品冷却时间
    std::int16_t sqlReturn = -1;
    PS_DB_ITEM_COOLTIME_UPDATE psUpdateInfo;

    xPacket >> psUpdateInfo;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psUpdateInfo.dwUCID, 1);
    xDBBinder.SetData(&psUpdateInfo.psInfo.byCooltimeGroupID, 1);
    xDBBinder.SetData(&psUpdateInfo.psInfo.biRemainDate, 1);
    xDBBinder.SetData(&psUpdateInfo.psInfo.biCooltimeValue, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ITEM_COOLTIME_UPDATE( ?, ?, ?, ?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "SP_ITEM_COOLTIME_UPDATE[UCID:%d, GroupID:%d]",
                           psUpdateInfo.dwUCID, psUpdateInfo.psInfo.byCooltimeGroupID);
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUseCountUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400592C0: 物品使用数量更新（盒子物品处理）
    std::int16_t sqlReturn = 0;
    PS_DB_ITEM_COUNTBOX psDBUpdateItem;

    xPacket >> psDBUpdateItem;

    XDBBinder xDBBinder(pDBStmt);

    // 第一轮：处理 psUpdateItemList
    for (const auto& stInfo : psDBUpdateItem.psUpdateItemList.vecItem) {
        if (stInfo.stItem.sCount) {
            // 有数量则更新
            sqlReturn = UpdateItemCount(pDBStmt, psDBUpdateItem.dwUCID, stInfo.stItem.xSerial, stInfo.stItem.sCount);
        } else {
            // 无数量则删除
            sqlReturn = DeleteItem(pDBStmt, psDBUpdateItem.dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos);
        }
        if (sqlReturn) {
            psDBUpdateItem.nErrorCode = 1;
            XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x48);
            xSendDBPacket << psDBUpdateItem;
            Send(xSendDBPacket);
            xDBBinder.Close();
            return sqlReturn;
        }
    }

    // 第二轮：处理 psCreateItemList
    for (const auto& stInfo : psDBUpdateItem.psCreateItemList.vecItem) {
        sqlReturn = UpdateItem(pDBStmt, psDBUpdateItem.dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos, &const_cast<STItem&>(stInfo.stItem));
        if (sqlReturn) {
            psDBUpdateItem.nErrorCode = 1;
            XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x48);
            xSendDBPacket << psDBUpdateItem;
            Send(xSendDBPacket);
            xDBBinder.Close();
            return sqlReturn;
        }
    }

    // 第三轮：处理 psCountboxItem
    for (const auto& stInfo : psDBUpdateItem.psCountboxItem.vecItem) {
        sqlReturn = UpdateItem(pDBStmt, psDBUpdateItem.dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, stInfo.shSlotPos, &const_cast<STItem&>(stInfo.stItem));
        if (sqlReturn) {
            psDBUpdateItem.nErrorCode = 1;
            break;
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x48);
    xSendDBPacket << psDBUpdateItem;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemNetCafeCostume(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400587A0: 网吧服装物品处理
    std::uint32_t dwUCID = 0;
    PS_EVENT_NETCAFE_ITEM_BUY psBuy;
    PS_RES_STORAGE_INFO psCreateItemList;
    std::uint8_t byFlag = 0;
    std::int16_t sqlReturn = 0;
    int nErrorCode = 0;

    xPacket.XParse >> dwUCID;
    xPacket >> psBuy;
    xPacket >> psCreateItemList;
    xPacket.XParse >> byFlag;

    XDBBinder xDBBinder(pDBStmt);

    // 第一轮：CheckCreateItem
    for (const auto& stInfo : psCreateItemList.vecItem) {
        sqlReturn = CheckCreateItem(pDBStmt, dwUCID, stInfo.byInvenType, stInfo.shSlotPos,
                                    stInfo.stItem.nItemID, stInfo.stItem.xSerial, byFlag);
        if (sqlReturn) {
            break;
        }
    }

    // 第二轮：CreateItem
    if (sqlReturn == 0) {
        for (const auto& stInfo : psCreateItemList.vecItem) {
            STItem stItem = stInfo.stItem;
            sqlReturn = CreateItem(pDBStmt, dwUCID, stInfo.byInvenType, stInfo.shSlotPos, &stItem, &nErrorCode);
            if (sqlReturn) {
                break;
            }
        }
    }

    if (sqlReturn) {
        nErrorCode = 1;
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x49);
    xSendDBPacket.XParse << nErrorCode;
    xSendDBPacket << psBuy;
    xSendDBPacket << psCreateItemList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemUpdatePos(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140059830: 更新物品位置列表
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO psUpdateItemList;
    std::uint8_t byDBError = 0;
    std::int16_t sqlReturn = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;
    xPacket >> psUpdateItemList;

    XDBAgent* pAgent = TXSingleton<XDBAgent>::Instance();
    XOption& pOption = pAgent->GetOption();

    for (auto& stInfo : psUpdateItemList.vecItem) {
        // 韩服特殊处理：联赛仓库使用UAID作为UCID
        if (pOption.GetNationType() != 2 &&
            (stInfo.byInvenType == 16 || stInfo.byInvenType == 17 || stInfo.byInvenType == 18)) {
            dwUCID = dwUAID;
        }

        sqlReturn = UpdateItemPos(pDBStmt, dwUCID, stInfo.stItem.xSerial, stInfo.byInvenType, static_cast<std::int16_t>(stInfo.shSlotPos));
        if (sqlReturn) {
            LogHelper::LogError("game.contents", "ReqItemUpdatePos ERROR - Failed pos update[UCID:%d, UAID:%d, Serial:I%64d, Inven:%d, Pos:%d",
                dwUCID, dwUAID, static_cast<std::int32_t>(stInfo.stItem.xSerial), stInfo.byInvenType, stInfo.shSlotPos);
            byDBError = 1;
            break;
        }
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x50);
    xSendDBPacket.XParse << byDBError;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemMakeLimitLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140059C80: 加载制作限制列表
    std::uint32_t dwUCID = 0;
    std::uint32_t dwUAID = 0;
    std::int16_t sqlReturn = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwUAID;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ITEM_MAKE_LIMIT_LOAD( ?, ? ) }")));

    PS_ITEM_MAKE_LIMIT_LIST psList;
    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "SP_ITEM_MAKE_LIMIT_LOAD error - Failed query");
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            PS_ITEM_MAKE_LIMIT_INFO psInfo;
            xDBBinder.GetData(&psInfo.nMakeIndex);
            xDBBinder.GetData(&psInfo.nItemID);
            xDBBinder.GetData(&psInfo.shCount);
            xDBBinder.GetData(&psInfo.biEndDate);
            psList.vecInfo.push_back(psInfo);
        }
    }

    xDBBinder.Close();

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x51);
    xSendDBPacket << psList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemMakeLimitDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14005A0D0: 删除制作限制
    std::int16_t sqlReturn = 0;

    PS_DB_ITEM_MAKE_LIMIT_INIT psList;
    xPacket >> psList;

    for (auto& psDBInfo : psList.vecInfo) {
        XDBBinder xDBBinder(pDBStmt);
        xDBBinder.SetData(&psList.dwUAID, 1);
        xDBBinder.SetData(&psList.dwUCID, 1);
        xDBBinder.SetData(&psDBInfo.psInfo.nMakeIndex, 1);
        xDBBinder.SetData(&psDBInfo.byLimitType, 1);

        sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ITEM_MAKE_LIMIT_RESET( ?, ?, ?, ?) }")));

        if (sqlReturn) {
            LogHelper::LogError("game.contents", "Failed SP_ITEM_MAKE_LIMIT_RESET error - Type:%d, ucid:%d, uaid:%d",
                psDBInfo.byLimitType, psList.dwUCID, psList.dwUAID);
        }
        xDBBinder.Close();
    }

    XSendDBPacket xSendDBPacket(xReturnSessionID, 0x21, 0x52);
    xSendDBPacket << psList;
    Send(xSendDBPacket);

    return sqlReturn;
}

std::int32_t XSQLItemProcess::ReqItemResealPackageInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemResealPackage(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqItemUseResealPackage(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

std::int32_t XSQLItemProcess::ReqQuickslotCardDeckUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14005A2D0: 更新快捷栏卡片组
    std::uint32_t dwUCID = 0;
    PS_QUICKSLOT_UPDATE_CARD_VEC psUpdateCard;

    xPacket.XParse >> dwUCID;
    xPacket >> psUpdateCard;

    for (auto& psCard : psUpdateCard.vecInfo) {
        UpdateQuickSlotCard(pDBStmt, dwUCID, &psCard);
    }

    return 0;
}

std::int32_t XSQLItemProcess::ReqQuickSlotCardDeckOpen(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // TODO: 汇编还原
    return -1;
}

// XSQLItemProcess 辅助方法实现（用于联赛仓库物品操作）
std::int16_t XSQLItemProcess::SelectSocketItem(XDBStmt* pDBStmt, std::int64_t biSerial, PS_ITEM_SOCKET_LIST* pSocketList) {
    // Per IDA 0x1400530A0: SP_ITEM_SELECT_SOCKET（获取物品镶嵌信息）
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&biSerial, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_SELECT_SOCKET( ? )}   ")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_SELECT_SOCKET ] [%d error] - Failed query( %d )", sqlReturn, 1564);
    }
    else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_ITEM_SOCKET stSocket;
            xDBBinder.GetData(&stSocket.biEquipSerial);
            xDBBinder.GetData(&stSocket.dwSocketID);
            xDBBinder.GetData(&stSocket.bySocketPos);

            for (int i = 0; i < 5; ++i) {
                std::uint8_t byType = 0;
                xDBBinder.GetData(&byType);
                stSocket.stExtendOption[i].byType = byType;
                xDBBinder.GetData(&stSocket.stExtendOption[i].nOption);
            }

            pSocketList->vecInfo.push_back(stSocket);
        }
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int16_t XSQLItemProcess::SelectBroachItem(XDBStmt* pDBStmt, std::int64_t biSerial, PS_ITEM_BROACH_LIST* pBroachList) {
    // Per IDA 0x140056E30: SP_ITEM_SELECT_BROACH（获取物品镂刻信息）
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&biSerial, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_SELECT_BROACH( ? )}   ")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_SELECT_BROACH ] [%d error] - Failed query( %d )", sqlReturn, 2661);
    }
    else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            ST_ITEM_BROACH stBroach;
            xDBBinder.GetData(&stBroach.biSerial);

            for (int i = 0; i < 15; ++i) {
                xDBBinder.GetData(&stBroach.dwItemID[i]);
            }

            pBroachList->vecInfo.push_back(stBroach);
        }
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int16_t XSQLItemProcess::SelectPackageItem(XDBStmt* pDBStmt, std::int64_t biPackageSerial, PS_ITEM_PACKAGE_LIST* pPackageList) {
    // Per IDA 0x14005BA90: SP_ITEM_PACKAGE_SELECT（获取物品套装信息）
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&biPackageSerial, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ITEM_PACKAGE_SELECT( ? ) }")));

    PS_ITEM_PACKAGE psInfo;
    psInfo.biPackageSerial = biPackageSerial;

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.item", "[ SP_ITEM_PACKAGE_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 3876);
        xDBBinder.Close();
        return sqlReturn;
    }

    while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
        ST_ITEM_PACKAGE_PARTS stData;
        xDBBinder.GetData(&stData.biSerial);
        xDBBinder.GetData(&stData.nItemID);
        xDBBinder.GetData(&stData.nDyeID);

        psInfo.vecInfo.push_back(stData);
    }

    pPackageList->vecInfo.push_back(psInfo);
    xDBBinder.Close();

    return sqlReturn;
}

std::int16_t XSQLItemProcess::UpdateItemCount(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::int16_t shCount) {
    // Per IDA 0x140053E40: SP_ITEM_UPDATE_COUNT（更新物品数量）
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&shCount, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_UPDATE_COUNT( ?, ?, ?)}   ")));

    xDBBinder.Close();

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_UPDATE_COUNT ] [%d error] - Failed query( %d )", sqlReturn, 1808);
    }

    return sqlReturn;
}

std::int16_t XSQLItemProcess::DeleteItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos) {
    // Per IDA 0x140050480: SP_ITEM_DELETE（删除物品）
    std::int16_t sqlReturn = -1;
    int nErrorCode = 0;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_DELETE(?, ?, ?, ?, ?)}   ")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_DELETE ] [%d error] - Failed query( %d )", sqlReturn, 743);
    }

    xDBBinder.Close();

    if (nErrorCode) {
        LogHelper::LogError("game.contents", "*** Error DeleteItem() [%d]", nErrorCode);
        sqlReturn = -1;
    }

    LogHelper::LogDebug("game.contents", "[DeleteItem] UCID=%u InvenType=%d SlotPos=%d Serial=%lld",
                        dwUCID, byInvenType, shSlotPos, biSerial);

    return sqlReturn;
}

// 注意：XSQLShopProcess �?XSQLCommonProcess 的实现在文件末尾的新增区�?// 这些�?DBAgent 的处理器实现，完整实现在 4794 行之�?
// XSQLLoginProcess 辅助方法实现
// Per IDA 0x140057310: CheckCreateItem - 检查物品创建（调试用）
std::int16_t XSQLItemProcess::CheckCreateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t byInvenType, std::int16_t shSlotPos, std::uint32_t dwItemID, std::int64_t biSerial, std::uint8_t byFlag, std::int64_t biItemSerial) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&dwItemID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byFlag, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_DEBUG_CHECK(?, ?, ?, ?, ?, ?)}")));

    if (sqlReturn == -1) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_DEBUG_CHECK ] [%d error] - Failed query( %d )", sqlReturn, 2737);
    }

    xDBBinder.Close();
    return 0;
}

// Per IDA 0x140056A40: CreateItem - 创建物品
std::int16_t XSQLItemProcess::CreateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* stItem, int* nErrorCode) {
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&stItem->nItemID, 1);
    xDBBinder.SetData(&stItem->xSerial, 1);
    xDBBinder.SetData(&stItem->sCount, 1);

    // wEndurance (combine byEndurance and bBindType)
    std::uint16_t wEndurance = stItem->byEndurance;
    std::uint8_t byBindType = stItem->bBindType;
    xDBBinder.SetData(&wEndurance, 1);
    xDBBinder.SetData(&byBindType, 1);
    xDBBinder.SetData(&stItem->eFlag, 1);
    xDBBinder.SetData(&stItem->byUpgrade, 1);

    // Extend options (5 pairs)
    for (int i = 0; i < 5; ++i) {
        std::uint8_t byType = stItem->stExtendOption[i].byType;
        xDBBinder.SetData(&byType, 1);
        xDBBinder.SetData(&stItem->stExtendOption[i].nOption, 1);
    }

    xDBBinder.SetData(&stItem->bySocketActiveCount, 1);
    xDBBinder.SetData(&stItem->nCashDate, 1);
    xDBBinder.SetData(&stItem->byUpgradeLimit, 1);
    xDBBinder.SetString(stItem->szBroachState, 16, &cbTID, 1);
    xDBBinder.SetData(&stItem->nAttack, 1);
    xDBBinder.SetData(&stItem->nDefense, 1);
    xDBBinder.SetData(&stItem->nTitleID, 1);
    xDBBinder.SetData(&stItem->nExp, 1);
    xDBBinder.SetData(&stItem->byUseCount, 1);
    xDBBinder.SetData(&stItem->nDyeID, 1);
    xDBBinder.SetData(nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
        "{call SP_ITEM_CREATE(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        *nErrorCode = 1;
        LogHelper::LogError("game.contents", "[ SP_ITEM_CREATE ] [%d error] - Failed query( %d )", sqlReturn, 2626);
    }

    xDBBinder.Close();
    return sqlReturn;
}

// Per IDA 0x140053630: UpdateItemPos - 更新物品位置
std::int16_t XSQLItemProcess::UpdateItemPos(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&biSerial, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_UPDATE_POS( ?, ?, ?, ? )}   ")));

    xDBBinder.Close();
    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_UPDATE_POS ] [%d error] - Failed query( %d )", sqlReturn, 1664);
    }

    return sqlReturn;
}

// Per IDA 0x140059D80: UpdateItemMakeLimit - 更新制作限制
std::int16_t XSQLItemProcess::UpdateItemMakeLimit(XDBStmt* pDBStmt, PS_DB_ITEM_MAKE_LIMIT_UPDATE* psUpdate) {
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&psUpdate->dwUAID, 1);
    xDBBinder.SetData(&psUpdate->dwUCID, 1);
    xDBBinder.SetData(&psUpdate->psDBInfo.psInfo.nMakeIndex, 1);
    xDBBinder.SetData(&psUpdate->psDBInfo.psInfo.nItemID, 1);
    xDBBinder.SetData(&psUpdate->psDBInfo.psInfo.shCount, 1);
    xDBBinder.SetData(&psUpdate->psDBInfo.psInfo.biEndDate, 1);
    xDBBinder.SetData(&psUpdate->psDBInfo.byLimitType, 1);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{ call SP_ITEM_MAKE_LIMIT_UPDATE( ?, ?, ?, ?, ?, ?, ?) }")));

    xDBBinder.Close();
    if (sqlReturn) {
        LogHelper::LogError("game.contents", "Failed SP_ITEM_MAKE_LIMIT_UPDATE error - Type:%d, ucid:%d, uaid:%d",
            psUpdate->psDBInfo.byLimitType, psUpdate->dwUCID, psUpdate->dwUAID);
    }

    return sqlReturn;
}

// Per IDA 0x14005A380: UpdateQuickSlotCard - 更新快捷栏卡片
bool XSQLItemProcess::UpdateQuickSlotCard(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_UPDATE_CARD* psCard) {
    int nErrorCode = 0;
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);
    xDBBinder.SetData(&dwUCID, 1);
    sqlReturn = xDBBinder.SetData(&psCard->byPage, 1);

    // 设置5个卡片槽位
    for (int i = 0; i < 5; ++i) {
        sqlReturn = xDBBinder.SetData(&(&psCard->nCard_1)[i], 1);
    }

    sqlReturn = xDBBinder.SetData(&nErrorCode, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_QUICKSLOT_UPDATE_AKASHIC( ?, ?, ?, ?, ?, ?, ?, ? )}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_QUICKSLOT_UPDATE_AKASHIC ] [%d error] - Failed query( Error:%d )", sqlReturn, nErrorCode);
        if (nErrorCode) {
            xDBBinder.Close();
            return false;
        }
    }

    xDBBinder.Close();
    return true;
}

std::int16_t XSQLItemProcess::UseItemInfoSelect(XDBStmt* pDBStmt, std::uint32_t dwActorID, ST_USE_ITEM_INFO_LIST* stUseItemInfoList) {
// Per IDA 0x140054F00: 查询物品使用信息
    std::int16_t sqlReturn = -1;
    XDBBinder xDBBinder(pDBStmt);
    ST_USE_ITEM_INFO stUseItemInfo;

    xDBBinder.SetData(&dwActorID, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_USE_INFO_SELECT(?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn == 100) {
            sqlReturn = 0;
        } else {
            LogHelper::LogError("game.contents", "[ SP_ITEM_USE_INFO_SELECT ] [%d error] - Failed query( %d )", sqlReturn, 2084);
        }
    } else {
        while ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            stUseItemInfo.nItemType = 0;
            stUseItemInfo.byCount = 0;
            stUseItemInfo.nUseDate = 0;
            xDBBinder.GetData(&stUseItemInfo.nItemType);
            xDBBinder.GetData(&stUseItemInfo.byCount);
            xDBBinder.GetData(&stUseItemInfo.nUseDate);
            stUseItemInfoList->vecInfo.push_back(stUseItemInfo);
        }
    }

    xDBBinder.Close();
    return sqlReturn;
}

std::int16_t XSQLItemProcess::UseItemInfoUpdate(XDBStmt* pDBStmt, std::uint32_t dwActorID, ST_USE_ITEM_INFO* stUseItemInfo) {
// Per IDA 0x140055030: 更新物品使用信息
    std::int16_t sqlReturn = -1;
    XDBBinder xDBBinder(pDBStmt);

    xDBBinder.SetData(&dwActorID, 1);
    xDBBinder.SetData(&stUseItemInfo->nItemType, 1);
    xDBBinder.SetData(&stUseItemInfo->byCount, 1);
    xDBBinder.SetData(&stUseItemInfo->nUseDate, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_USE_INFO_UPDATE( ?, ?, ?, ? )}")));

    xDBBinder.Close();
    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        LogHelper::LogError("game.contents", "[ SP_ITEM_USE_INFO_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 2104);
    }

    return sqlReturn;
}

std::int16_t XSQLItemProcess::MoveItemEx(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSrcSerial,
                                         std::uint8_t byInvenType, std::int16_t shSlotPos,
                                         std::uint8_t byBindType, std::int64_t biCashDate, std::uint8_t byStoreType) {
// Per IDA 0x140055320: 移动物品（扩展版本）
    std::int16_t sqlReturn = -1;
    XDBBinder xDBBinder(pDBStmt);
    int nErrorCode = 0;

    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byInvenType, 1);
    xDBBinder.SetData(&shSlotPos, 1);
    xDBBinder.SetData(&biSrcSerial, 1);
    xDBBinder.SetData(&byBindType, 1);
    xDBBinder.SetData(&biCashDate, 1);
    xDBBinder.SetData(&byStoreType, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);

    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_ITEM_MOVE_EX(?, ?, ?, ?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            sqlReturn = -1;
        }
        LogHelper::LogError("game.contents", "[ SP_ITEM_MOVE_EX ] [%d error] - Failed query( %d )", sqlReturn, 2172);
    }

    xDBBinder.Close();

    if (nErrorCode) {
        LogHelper::LogError("game.contents", "*** Error MoveItemEx() [%d]", sqlReturn);
        sqlReturn = -1;
    }

    return sqlReturn;
}

bool XSQLLoginProcess::CheckServerState(XDBStmt* pDBStmt, wchar_t* szID, int nIP, int* pServerState, wchar_t* szResultMsg) {
// Per IDA 0x1400811A0: 检查服务器状�?
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    int nErrorCode = 0;

    sqlReturn = xDBBinder.SetWString(szID, 21, &cbTID, 1);
    sqlReturn = xDBBinder.SetData(&nIP, 1);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHECK_SERVER_STATE(?,?) }")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHECK_SERVER_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2752);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetData(pServerState);
            xDBBinder.GetWString(szResultMsg, 1025);
        }
        xDBBinder.Close();
    }

    return nErrorCode == 0;
}

bool XSQLLoginProcess::CheckBlockAccount(XDBStmt* pDBStmt, wchar_t* szID, unsigned int* pUAID, wchar_t* szResultMsg) {
// Per IDA 0x14007FAD0: 检查账户封禁状�?
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    *pUAID = 0;
    int nErrorCode = 0;

    sqlReturn = xDBBinder.SetWString(szID, 21, &cbTID, 1);
    sqlReturn = xDBBinder.SetData(pUAID, 4);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHECK_BLOCK_USER(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHECK_BLOCK_USER ] [%d error] - Failed query( %d )", sqlReturn, 2350);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetWString(szResultMsg, 1025);
        }
        xDBBinder.Close();
    }

    return nErrorCode == 0;
}

bool XSQLLoginProcess::CheckBlockIP(XDBStmt* pDBStmt, int nIP, unsigned char by1, unsigned char by2, unsigned char by3, wchar_t* szResultMsg) {
// Per IDA 0x14007FEA0: 检�?IP 封禁状�?
    std::int16_t sqlReturn = -1;

    XDBBinder xDBBinder(pDBStmt);

    unsigned char byIP4 = static_cast<unsigned char>((nIP >> 24) & 0xFF);
    int nErrorCode = 0;

    sqlReturn = xDBBinder.SetData(&by1, 1);
    sqlReturn = xDBBinder.SetData(&by2, 1);
    sqlReturn = xDBBinder.SetData(&by3, 1);
    sqlReturn = xDBBinder.SetData(&byIP4, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHECK_BLOCK_IP(?, ?, ?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHECK_BLOCK_IP ] [%d error] - Failed query( %d )", sqlReturn, 2451);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetWString(szResultMsg, 1025);
        }
        xDBBinder.Close();
    }

    return nErrorCode == 0;
}

bool XSQLLoginProcess::CheckBlockMac(XDBStmt* pDBStmt, wchar_t* szMacAddress, wchar_t* szResultMsg) {
    // Per IDA 0x140080050: 检�?MAC 封禁状态（宽字符版本）
    // 先转换为窄字符，再调�?char* 版本
    char szMac[18] = {};

#ifdef _WIN32
    WideCharToMultiByte(0, 0, szMacAddress, -1, szMac, 18, nullptr, nullptr);
#else
    // TODO: Linux 平台转换
    std::strncpy(szMac, reinterpret_cast<const char*>(szMacAddress), sizeof(szMac) - 1);
#endif

    return CheckBlockMac(pDBStmt, szMac, szResultMsg);
}

bool XSQLLoginProcess::CheckBlockMac(XDBStmt* pDBStmt, char* szMacAddress, wchar_t* szResultMsg) {
    // Per IDA 0x140080100: 检�?MAC 封禁状态（窄字符版本）
    std::int16_t sqlReturn = -1;
    std::int64_t cbTID = -3;  // SQL_NTS

    XDBBinder xDBBinder(pDBStmt);

    int nErrorCode = 0;

    sqlReturn = xDBBinder.SetString(szMacAddress, 18, &cbTID, 1);
    sqlReturn = xDBBinder.SetData(&nErrorCode, 4);
    sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_CHECK_BLOCK_MAC(?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        nErrorCode = -1;
        LogHelper::LogError("game.contents", "[ SP_CHECK_BLOCK_MAC ] [%d error] - Failed query( %d )", sqlReturn, 2496);
    } else {
        if ((xDBBinder.Fetch() & 0xFFFFFFFE) == 0) {
            xDBBinder.GetWString(szResultMsg, 1025);
        }
        xDBBinder.Close();
    }

    return nErrorCode == 0;
}

std::uint8_t XSQLLoginProcess::GetSecondePasswordState(XDBStmt* pDBStmt, int nUAID, int* nAccountState) {
    // Per IDA 0x14007FC60: 获取二级密码状�
    XDBBinder xDBBinder(pDBStmt);

    *nAccountState = 0;
    std::uint8_t byTempState = 0;

    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(nAccountState, 4);
    xDBBinder.SetData(&byTempState, 4);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SECOND_PASSWORD_STATE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_SECOND_PASSWORD_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2384);
    } else {
        xDBBinder.Close();
    }

    return byTempState;
}

std::uint8_t XSQLLoginProcess::GetTradePasswordState(XDBStmt* pDBStmt, int nUAID, int* nAccountState) {
    // Per IDA 0x14007FD80: 获取交易密码状�
    XDBBinder xDBBinder(pDBStmt);

    *nAccountState = 0;
    std::uint8_t byTempState = 0;

    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(nAccountState, 4);
    xDBBinder.SetData(&byTempState, 4);

    std::int16_t sqlReturn = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_TRADE_PASSWORD_STATE(?, ?, ?)}")));

    if ((sqlReturn & 0xFFFFFFFE) != 0) {
        if (sqlReturn != 100) {
            xDBBinder.Close();
        }
        LogHelper::LogError("game.contents", "[ SP_TRADE_PASSWORD_STATE ] [%d error] - Failed query( %d )", sqlReturn, 2415);
    } else {
        xDBBinder.Close();
    }

    return byTempState;
}

std::int32_t XSQLLoginProcess::ReqHanBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x1400804E0: HAN计费订单号获�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    PS_CASH_BUY_COUNT_LIST psCashbuyList;

    xPacket >> psHanBillOrder;
    // 解析 psCashbuyList（需要单独处�?vector�
    std::uint32_t nCashBuyCount = 0;
    xPacket.XParse >> nCashBuyCount;
    for (std::uint32_t i = 0; i < nCashBuyCount; ++i) {
        PS_CASH_BUY_COUNT item;
        xPacket.XParse >> item.dwItemID;
        xPacket.XParse >> item.nBuyCount;
        psCashbuyList.vecInfo.push_back(item);
    }

    psHanBillOrder.bResult = true;
    std::uint32_t dwGiftUCID = psHanBillOrder.dwRecvUCID;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult == 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_GET_KEY_NHN(?, ?, ?, ?, ?, ?, ?, ?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = 0;

            xDBBinder.SetData(&psHanBillOrder.dwUAID, 1);
            xDBBinder.SetData(&psHanBillOrder.dwUCID, 1);
            xDBBinder.SetData(&stHanBilling.dwItemID, 1);
            xDBBinder.SetData(&stHanBilling.nBanance, 1);
            std::int64_t cbCode = -3;  // SQL_NTS
            xDBBinder.SetString(stHanBilling.szCode, 21, &cbCode, 1);
            xDBBinder.SetData(&dwGiftUCID, 1);
            xDBBinder.SetData(&stHanBilling.nCount, 1);
            xDBBinder.SetData(&nOrderNo, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                psHanBillOrder.bResult = false;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_GET_ORDER_NO ] [%d error] - Failed query( %d )", sqlReturn, 2590);
                break;
            }

            xDBBinder.Close();
            stHanBilling.nOrderNo = nOrderNo;
        }
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x42);
    xSendPacket << psHanBillOrder;
    xSendPacket.XParse << static_cast<std::uint32_t>(psCashbuyList.vecInfo.size());
    for (const auto& item : psCashbuyList.vecInfo) {
        xSendPacket.XParse << item.dwItemID;
        xSendPacket.XParse << item.nBuyCount;
    }
    Send(xSendPacket);
    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqHanBillingUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140080270: HAN计费订单状态更�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    xPacket >> psHanBillOrder;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult <= 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_UPDATE_NHN(?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderCode = static_cast<std::int32_t>(stHanBilling.nOrderNo);

            xDBBinder.SetData(&nOrderCode, 1);
            xDBBinder.SetData(&stHanBilling.nResult, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 2539);
            } else {
                xDBBinder.Close();
            }
        }
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqHanBillingOrderNo2(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140080910: HAN计费订单号获取（版本2�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    PS_CASH_BUY_COUNT_LIST psCashbuyList;
    std::int32_t nDecreaseCash = 0;

    xPacket >> psHanBillOrder;
    xPacket.XParse >> nDecreaseCash;
    xPacket >> psCashbuyList;

    psHanBillOrder.bResult = 1;
    std::uint32_t dwGiftUCID = psHanBillOrder.dwRecvUCID;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult == 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_GET_KEY_NHN(?, ?, ?, ?, ?, ?, ?, ?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = 0;

            xDBBinder.SetData(&psHanBillOrder.dwUAID, 1);
            xDBBinder.SetData(&psHanBillOrder.dwUCID, 1);
            xDBBinder.SetData(&stHanBilling.dwItemID, 1);
            xDBBinder.SetData(&stHanBilling.nBanance, 1);
            std::int64_t cbTID = -3;
            xDBBinder.SetString(stHanBilling.szCode, 21, &cbTID, 1);
            xDBBinder.SetData(&dwGiftUCID, 1);
            xDBBinder.SetData(&stHanBilling.nCount, 1);
            xDBBinder.SetData(&nOrderNo, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                psHanBillOrder.bResult = 0;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_GET_ORDER_NO ] [%d error] - Failed query( %d )", sqlReturn, 2651);
                break;
            }

            xDBBinder.Close();
            stHanBilling.nOrderNo = nOrderNo;
        }
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x44);
    xSendPacket << psHanBillOrder;
    xSendPacket.XParse << nDecreaseCash;
    xSendPacket << psCashbuyList;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqSGBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140080D70: SG计费订单号获�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    PS_CASH_BUY_COUNT_LIST psCashbuyList;

    xPacket >> psHanBillOrder;
    xPacket >> psCashbuyList;

    psHanBillOrder.bResult = 1;
    std::uint32_t dwGiftUCID = psHanBillOrder.dwRecvUCID;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult == 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_GET_KEY_NHN(?, ?, ?, ?, ?, ?, ?, ?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = 0;

            xDBBinder.SetData(&psHanBillOrder.dwUAID, 1);
            xDBBinder.SetData(&psHanBillOrder.dwUCID, 1);
            xDBBinder.SetData(&stHanBilling.dwItemID, 1);
            xDBBinder.SetData(&stHanBilling.nBanance, 1);
            std::int64_t cbTID = -3;
            xDBBinder.SetString(stHanBilling.szCode, 21, &cbTID, 1);
            xDBBinder.SetData(&dwGiftUCID, 1);
            xDBBinder.SetData(&stHanBilling.nCount, 1);
            xDBBinder.SetData(&nOrderNo, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                psHanBillOrder.bResult = 0;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_GET_ORDER_NO ] [%d error] - Failed query( %d )", sqlReturn, 2709);
                break;
            }

            xDBBinder.Close();
            stHanBilling.nOrderNo = nOrderNo;
        }
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x48);
    xSendPacket << psHanBillOrder;
    xSendPacket << psCashbuyList;
    Send(xSendPacket);

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqTWNBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140082B00: TWN计费订单号获�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    PS_CASH_BUY_COUNT_LIST psCashbuyList;

    xPacket >> psHanBillOrder;
    xPacket >> psCashbuyList;

    psHanBillOrder.bResult = 1;
    std::uint32_t dwGiftUCID = psHanBillOrder.dwRecvUCID;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult == 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_GET_KEY_NHN(?, ?, ?, ?, ?, ?, ?, ?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = 0;

            xDBBinder.SetData(&psHanBillOrder.dwUAID, 1);
            xDBBinder.SetData(&psHanBillOrder.dwUCID, 1);
            xDBBinder.SetData(&stHanBilling.dwItemID, 1);
            xDBBinder.SetData(&stHanBilling.nBanance, 1);
            std::int64_t cbTID = -3;
            xDBBinder.SetString(stHanBilling.szCode, 21, &cbTID, 1);
            xDBBinder.SetData(&dwGiftUCID, 1);
            xDBBinder.SetData(&stHanBilling.nCount, 1);
            xDBBinder.SetData(&nOrderNo, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                psHanBillOrder.bResult = 0;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_GET_ORDER_NO ] [%d error] - Failed query( %d )", sqlReturn, 3214);
                break;
            }

            xDBBinder.Close();
            stHanBilling.nOrderNo = nOrderNo;
        }
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x60);
    xSendPacket << psHanBillOrder;
    xSendPacket << psCashbuyList;
    Send(xSendPacket);
    LogHelper::LogInfo("game.contents", "[TWN_CASH] eSUB_SQL_TWN_BILLING_ORDER_NO");

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqTWNBillingUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140082F40: TWN计费订单状态更�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    xPacket >> psHanBillOrder;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult <= 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_UPDATE_NHN(?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = stHanBilling.nOrderNo;

            xDBBinder.SetData(&nOrderNo, 1);
            xDBBinder.SetData(&stHanBilling.nResult, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 3266);
            } else {
                xDBBinder.Close();
            }
        }
    }

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqWMBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x140083590: WM计费订单号获�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    PS_CASH_BUY_COUNT_LIST psCashbuyList;

    xPacket >> psHanBillOrder;
    xPacket >> psCashbuyList;

    psHanBillOrder.bResult = 1;
    std::uint32_t dwGiftUCID = psHanBillOrder.dwRecvUCID;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult == 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_GET_KEY_NHN(?, ?, ?, ?, ?, ?, ?, ?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = 0;

            xDBBinder.SetData(&psHanBillOrder.dwUAID, 1);
            xDBBinder.SetData(&psHanBillOrder.dwUCID, 1);
            xDBBinder.SetData(&stHanBilling.dwItemID, 1);
            xDBBinder.SetData(&stHanBilling.nBanance, 1);
            std::int64_t cbTID = -3;
            xDBBinder.SetString(stHanBilling.szCode, 21, &cbTID, 1);
            xDBBinder.SetData(&dwGiftUCID, 1);
            xDBBinder.SetData(&stHanBilling.nCount, 1);
            xDBBinder.SetData(&nOrderNo, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                psHanBillOrder.bResult = 0;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_GET_ORDER_NO ] [%d error] - Failed query( %d )", sqlReturn, 3383);
                break;
            }

            xDBBinder.Close();
            stHanBilling.nOrderNo = nOrderNo;
        }
    }

    XSendDBPacket xSendPacket(xReturnSessionID, 0x02, 0x64);
    xSendPacket << psHanBillOrder;
    xSendPacket << psCashbuyList;
    Send(xSendPacket);
    LogHelper::LogInfo("game.contents", "[WM_CASH] eSUB_SQL_WM_BILLING_ORDER_NO");

    return sqlReturn;
}

std::int32_t XSQLLoginProcess::ReqWMBillingUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
// Per IDA 0x1400839D0: WM计费订单状态更�?
    std::int16_t sqlReturn = -1;

    PS_HAN_BILLING_ORDER_NO_VEC psHanBillOrder;
    xPacket >> psHanBillOrder;

    for (auto& stHanBilling : psHanBillOrder.vecOrderInfo) {
        if (stHanBilling.nResult <= 0) {
            unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_UPDATE_NHN(?)}";
            XDBBinder xDBBinder(pDBStmt);

            std::int32_t nOrderNo = stHanBilling.nOrderNo;

            xDBBinder.SetData(&nOrderNo, 1);
            xDBBinder.SetData(&stHanBilling.nResult, 4);

            sqlReturn = xDBBinder.Execute(szQuery);

            if ((sqlReturn & 0xFFFFFFFE) != 0) {
                if (sqlReturn != 100) {
                    xDBBinder.Close();
                }
                stHanBilling.nResult = -1;
                LogHelper::LogError("game.contents", "[ SP_HAN_BILLING_UPDATE ] [%d error] - Failed query( %d )", sqlReturn, 3435);
            } else {
                xDBBinder.Close();
            }
        }
    }

    return sqlReturn;
}

// ============================================================================
// XSQLShopProcess Implementation
// ============================================================================

XSQLShopProcess::XSQLShopProcess() : TXSQLProcessBase<0x22>("XSQLShopProcess") {}
XSQLShopProcess::~XSQLShopProcess() = default;

std::int32_t XSQLShopProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400B7680: SubCmd routing for Shop operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqItemBuy(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqItemSell(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqItemRepurchaser(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqItemDeleteRepurchase(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqNpcCreditGradeLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqNpcCreditGradeUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqNpcCreditGradeUpdateList(pDBStmt, xPacket, xReturnSessionID);
    case 0x13: return ReqShopItemLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x14: return ReqShopItemUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ReqShopCashItemBuy(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqShopCashItemSetLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x22: return ReqShopCashItemSet(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqShopCashItemSetDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqShopCashItemGift(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqShopCashItemGiftCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x26: return ReqCashItemBuyCountLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x27: return ReqCashItemBuyCountUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x28: return ReqShopAccountItemUpdate(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLShopProcess::ReqItemBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqItemSell(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqItemRepurchaser(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqItemDeleteRepurchase(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqNpcCreditGradeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqNpcCreditGradeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqNpcCreditGradeUpdateList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopItemLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopCashItemBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopCashItemSetLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopCashItemSet(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopCashItemSetDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopCashItemGift(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopCashItemGiftCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqCashItemBuyCountLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqCashItemBuyCountUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLShopProcess::ReqShopAccountItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLTradeProcess Implementation
// ============================================================================

XSQLTradeProcess::XSQLTradeProcess() : TXSQLProcessBase<0x23>("XSQLTradeProcess") {}
XSQLTradeProcess::~XSQLTradeProcess() = default;

std::int32_t XSQLTradeProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C4DC0: SubCmd routing for Trade operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqTradeConfirm(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqPrivateShopStart(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqPrivateShopBuy(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLTradeProcess::ReqTradeConfirm(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLTradeProcess::ReqPrivateShopStart(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLTradeProcess::ReqPrivateShopBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLItemUpgradeProcess Implementation
// ============================================================================

XSQLItemUpgradeProcess::XSQLItemUpgradeProcess() : TXSQLProcessBase<0x24>("XSQLItemUpgradeProcess") {}
XSQLItemUpgradeProcess::~XSQLItemUpgradeProcess() = default;

std::int32_t XSQLItemUpgradeProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14006B050: SubCmd routing for ItemUpgrade operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqItemUpgradeResult(pDBStmt, xPacket, xReturnSessionID);
    case 0x02:
    case 0x03: return ReqItemAddOptionResult(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLItemUpgradeProcess::ReqItemUpgradeResult(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemUpgradeProcess::ReqItemAddOptionResult(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLMyRoomProcess Implementation
// ============================================================================

XSQLMyRoomProcess::XSQLMyRoomProcess() : TXSQLProcessBase<0x25>("XSQLMyRoomProcess") {}
XSQLMyRoomProcess::~XSQLMyRoomProcess() = default;

std::int32_t XSQLMyRoomProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140086070: SubCmd routing for MyRoom operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqMyRoomCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqMyRoomCheck(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqMyRoomItemLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqMyRoomFurnitureLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqMyRoomIndexSelect(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqmyRoomFurnitureEdit(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqMyRoomItemAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqMyRoomItemDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqMyRoomSetup(pDBStmt, xPacket, xReturnSessionID);
    case 0x16: return ReqMyRoomPollenAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x17: return ReqMyRoomPollenCultivation(pDBStmt, xPacket, xReturnSessionID);
    case 0x18: return ReqMyRoomPollenHarvest(pDBStmt, xPacket, xReturnSessionID);
    case 0x19: return ReqMyRoomPollenItemUse(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ReqMyRoomPollenHelp(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqMYRoomPollenCancel(pDBStmt, xPacket, xReturnSessionID);
    case 0x22: return ReqMyRoomPollenLook(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqMyRoomPollenCheat(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqMyroomRecommend(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqMyroomFavorite(pDBStmt, xPacket, xReturnSessionID);
    case 0x26: return ReqMyroomBoardList(pDBStmt, xPacket, xReturnSessionID);
    case 0x27: return ReqMyroomBoardWrite(pDBStmt, xPacket, xReturnSessionID);
    case 0x29: return ReqMyroomCheckUAID(pDBStmt, xPacket, xReturnSessionID);
    case 0x30: return ReqMyroomRankReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x31: return ReqMyroomUpdateData(pDBStmt, xPacket, xReturnSessionID);
    case 0x32: return ReqMyroomInitData(pDBStmt, xPacket, xReturnSessionID);
    case 0x33: return ReqMyroomFavoriteInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x34: return ReqMyroomCommunityInfo(pDBStmt, xPacket, xReturnSessionID);
    default: return 1;
    }
}

std::int32_t XSQLMyRoomProcess::ReqMyRoomCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomItemLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomFurnitureLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomIndexSelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqmyRoomFurnitureEdit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomItemAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomItemDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomSetup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenCultivation(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenHarvest(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenItemUse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenHelp(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMYRoomPollenCancel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenLook(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyRoomPollenCheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomRecommend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomFavorite(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomBoardList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomBoardWrite(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomCheckUAID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomRankReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomUpdateData(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomInitData(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomFavoriteInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLMyRoomProcess::ReqMyroomCommunityInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLHelperProcess Implementation
// ============================================================================

XSQLHelperProcess::XSQLHelperProcess() : TXSQLProcessBase<0x26>("XSQLHelperProcess") {}
XSQLHelperProcess::~XSQLHelperProcess() = default;

std::int32_t XSQLHelperProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004B290: SubCmd routing for Helper operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqHelperListLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqHelperAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqHelperSupportEquip(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqHelperSupportRelease(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqHelperEquip(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqHelperChangeOrder(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqHelperChangeAutoSummon(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLHelperProcess::ReqHelperListLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLHelperProcess::ReqHelperAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLHelperProcess::ReqHelperSupportEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLHelperProcess::ReqHelperSupportRelease(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLHelperProcess::ReqHelperEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLHelperProcess::ReqHelperChangeOrder(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLHelperProcess::ReqHelperChangeAutoSummon(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLExchange Implementation
// ============================================================================

XSQLExchange::XSQLExchange() : TXSQLProcessBase<0x27>("XSQLExchange") {}
XSQLExchange::~XSQLExchange() = default;

std::int32_t XSQLExchange::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400386F0: SubCmd routing for Exchange operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqExchangeSearch(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqExchangePriceHistoryList(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqExchangeInterestList(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqExchangeInterestItem(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqExchangeSellRegister(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqExchangeItemBuy(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqExchangeItemRecall(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqExchangeMyList(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqExchangeItemInfo(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLExchange::ReqExchangeSearch(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangePriceHistoryList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeInterestList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeInterestItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeSellRegister(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeItemBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeItemRecall(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeMyList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLExchange::ReqExchangeItemInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLRankingProcess Implementation
// ============================================================================

XSQLRankingProcess::XSQLRankingProcess() : TXSQLProcessBase<0x28>("XSQLRankingProcess") {}
XSQLRankingProcess::~XSQLRankingProcess() = default;

std::int32_t XSQLRankingProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400AC810: SubCmd routing for Ranking operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqRankingList(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqRankingMyInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqRankingPointUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x13: return ReqLastRankingReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x16: return UpdateRankingOperationPoint(pDBStmt, xPacket, xReturnSessionID);
    case 0x17: return SelectRankingOperationList(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqRankingReset_Cheat(pDBStmt, xPacket, xReturnSessionID);
    case 0x22: return ReqRankingUpdateData_Cheat(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqRankingInsertDummy_Cheat(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqRankingRefresh_Cheat(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqRankingOperation_Cheat(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLRankingProcess::ReqRankingList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingMyInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingPointUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqLastRankingReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::UpdateRankingOperationPoint(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::SelectRankingOperationList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingReset_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingUpdateData_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingInsertDummy_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingRefresh_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLRankingProcess::ReqRankingOperation_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLQuestProcess Implementation
// ============================================================================

XSQLQuestProcess::XSQLQuestProcess() : TXSQLProcessBase<0x41>("XSQLQuestProcess") {}
XSQLQuestProcess::~XSQLQuestProcess() = default;

std::int32_t XSQLQuestProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400A4590: SubCmd routing for Quest operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return LoadQuest(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqQuestUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqQuestUpdateAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqQuestCompleteUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqDeleteEpisode(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqQuestReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqQuestRepeatAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqQuestRepeatUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqQuestFirstDropItemUpdate(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLQuestProcess::LoadQuest(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestUpdateAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestCompleteUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqDeleteEpisode(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestRepeatAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestRepeatUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLQuestProcess::ReqQuestFirstDropItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLLogGameProcess Implementation
// ============================================================================

XSQLLogGameProcess::XSQLLogGameProcess() : TXSQLProcessBase<0x42>("XSQLLogGameProcess") {}
XSQLLogGameProcess::~XSQLLogGameProcess() = default;

std::int32_t XSQLLogGameProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400CC010: SubCmd routing for LogGame operations
    switch (static_cast<std::uint8_t>(xPacket.GetSubCmd())) {
    case 0x00:
    case 0x01: return WriteAuthLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return WriteMoneyLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ResItemCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ResItemUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ResTradeStart(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ResTradeResult(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ResItemRepair(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return WriteChatLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return WriteStatLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ResAchieve(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ResPostReceipt(pDBStmt, xPacket, xReturnSessionID);
    case 0x13: return WriteCashLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x14: return ResAccountPostReceipt(pDBStmt, xPacket, xReturnSessionID);
    case 0x15: return ResClientLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x16: return WriteConnectServerLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x17: return ResClassEventLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x18: return ResSGLog(pDBStmt, xPacket, xReturnSessionID);
    case 0x19: return ResSGLogCharInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ResSGLogToken(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return WriteLogText(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return WriteSystemLog(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLLogGameProcess::WriteAuthLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteMoneyLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResItemCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResTradeStart(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResTradeResult(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResItemRepair(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteChatLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteStatLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResAchieve(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResPostReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteCashLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResAccountPostReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResClientLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteConnectServerLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResClassEventLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResSGLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResSGLogCharInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::ResSGLogToken(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteLogText(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLLogGameProcess::WriteSystemLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLWorldProcess Implementation
// ============================================================================

XSQLWorldProcess::XSQLWorldProcess() : TXSQLProcessBase<0x43>("XSQLWorldProcess") {}
XSQLWorldProcess::~XSQLWorldProcess() = default;

std::int32_t XSQLWorldProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400CE750: SubCmd routing for World operations
    switch (static_cast<std::uint8_t>(xPacket.GetSubCmd())) {
    case 0x00: return ReqAddChannelMap(pDBStmt, xPacket, xReturnSessionID);
    case 0x01: return ReqDeleteChannelMap(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqWorldUpdateMazeEnterLimitCount(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqWorldClearMazeEnterLimitCount(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqWorldGFBillingReloadList(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqWorldUpdateMazeEnterLimitCountGroup(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqWorldClearMazeEnterLimitCountGroup(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLWorldProcess::ReqAddChannelMap(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWorldProcess::ReqDeleteChannelMap(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWorldProcess::ReqWorldUpdateMazeEnterLimitCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWorldProcess::ReqWorldClearMazeEnterLimitCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWorldProcess::ReqWorldGFBillingReloadList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWorldProcess::ReqWorldUpdateMazeEnterLimitCountGroup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWorldProcess::ReqWorldClearMazeEnterLimitCountGroup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLSkillProcess Implementation
// ============================================================================

XSQLSkillProcess::XSQLSkillProcess() : TXSQLProcessBase<0x44>("XSQLSkillProcess") {}
XSQLSkillProcess::~XSQLSkillProcess() = default;

std::int32_t XSQLSkillProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400BCC50: SubCmd routing for Skill operations
    switch (static_cast<std::uint8_t>(xPacket.GetSubCmd())) {
    case 0x00: return ReqSkillLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x01: return ReqSkillLearn(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqSkillUpdatePoint(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqSkillReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqSkillAddDeckSlot(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqSkillUpdateDeck(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqSkillResetDeck(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqSkillDivergence(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqSkillDeckBonus(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqSkillResetTaget(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqBoosterLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqBoosterAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqBoosterDel(pDBStmt, xPacket, xReturnSessionID);
    case 0x13: return ReqDeckPageActive(pDBStmt, xPacket, xReturnSessionID);
    case 0x14: return ReqDeckPageName(pDBStmt, xPacket, xReturnSessionID);
    case 0x15: return ReqSkillDeckPageOpen(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLSkillProcess::ReqSkillLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillLearn(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillUpdatePoint(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillAddDeckSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillUpdateDeck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillResetDeck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillDivergence(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillDeckBonus(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillResetTaget(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqBoosterLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqBoosterAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqBoosterDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqDeckPageActive(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqDeckPageName(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSkillProcess::ReqSkillDeckPageOpen(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLOptionProcess Implementation
// ============================================================================

XSQLOptionProcess::XSQLOptionProcess() : TXSQLProcessBase<0x45>("XSQLOptionProcess") {}
XSQLOptionProcess::~XSQLOptionProcess() = default;

std::int32_t XSQLOptionProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14008ED60: 直接返回-1,无实际处理逻辑
    xPacket.GetSubCmd();
    return -1;
}

// ============================================================================
// XSQLSoulMetryProcess Implementation
// ============================================================================

XSQLSoulMetryProcess::XSQLSoulMetryProcess() : TXSQLProcessBase<0x46>("XSQLSoulMetryProcess") {}
XSQLSoulMetryProcess::~XSQLSoulMetryProcess() = default;

std::int32_t XSQLSoulMetryProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C1550: SubCmd routing for SoulMetry operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return LoadSoulMetry(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqSoulMetryUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqSoulMetryComplete(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqSoulMetryReset(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLSoulMetryProcess::LoadSoulMetry(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSoulMetryProcess::ReqSoulMetryUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSoulMetryProcess::ReqSoulMetryComplete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLSoulMetryProcess::ReqSoulMetryReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLGestureProcess Implementation
// ============================================================================

XSQLGestureProcess::XSQLGestureProcess() : TXSQLProcessBase<0x47>("XSQLGestureProcess") {}
XSQLGestureProcess::~XSQLGestureProcess() = default;

std::int32_t XSQLGestureProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14004A970: SubCmd routing for Gesture operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqGestureLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqGestureUpdate(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLGestureProcess::ReqGestureLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLGestureProcess::ReqGestureUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLDailyMissionProcess Implementation
// ============================================================================

XSQLDailyMissionProcess::XSQLDailyMissionProcess() : TXSQLProcessBase<0x48>("XSQLDailyMissionProcess") {}
XSQLDailyMissionProcess::~XSQLDailyMissionProcess() = default;

std::int32_t XSQLDailyMissionProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400329D0: SubCmd routing for DailyMission operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return LoadDailyMission(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return AddAllDailyMission(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqDailyMissionUpdate(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLDailyMissionProcess::LoadDailyMission(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLDailyMissionProcess::AddAllDailyMission(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLDailyMissionProcess::ReqDailyMissionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLEvent Implementation
// ============================================================================

XSQLEvent::XSQLEvent() : TXSQLProcessBase<0x49>("XSQLEvent") {}
XSQLEvent::~XSQLEvent() = default;

std::int32_t XSQLEvent::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140011D70: SubCmd routing for Event operations
    switch (static_cast<std::uint8_t>(xPacket.GetSubCmd())) {
    case 0x06: return ReqWorldModeLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqWorldModeUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqWorldModeEnter(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqModeMazeEventLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqModeMazeEventRankUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ReqClassEventLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqClassEventUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqNetCafeMissionLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqNetCafeMissionUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x26: return ReqPlayTimeByDay(pDBStmt, xPacket, xReturnSessionID);
    case 0x27: return ReqWorldEventInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x28: return ReqWorldEventRegister(pDBStmt, xPacket, xReturnSessionID);
    case 0x29: return ReqWorldEventReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x2A: return ReqWorldEventDailyReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x2B: return ReqRouletteEventInfo(pDBStmt, xPacket, xReturnSessionID);
    case 0x2C: return ReqRouletteEventUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x2D: return ReqRouletteRewardLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x2E: return ReqRouletteInit(pDBStmt, xPacket, xReturnSessionID);
    case 0x30: return ReqWorldEventReset_Cheat(pDBStmt, xPacket, xReturnSessionID);
    case 0x39: return ReqEventAccountPlayTimeLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x40: return ReqEventAccountPlayTimeUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x41: return ReqAttendanceLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x42: return ReqAttendanceReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x43: return ReqAttendanceContinueReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x44: return ReqAttendancePlayTimeUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x45: return ReqAttendancePlayTimeReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x46: return ReqAttendanceReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x47: return ReqAttendanceContinueReset(pDBStmt, xPacket, xReturnSessionID);
    case 0x48: return ReqAttendancePlayTimeReset(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLEvent::ReqWorldModeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldModeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldModeEnter(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqModeMazeEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqModeMazeEventRankUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqClassEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqClassEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqNetCafeMissionLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqNetCafeMissionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqPlayTimeByDay(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldEventInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldEventRegister(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldEventReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldEventDailyReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqRouletteEventInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqRouletteEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqRouletteRewardLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqRouletteInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqWorldEventReset_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqEventAccountPlayTimeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqEventAccountPlayTimeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendanceLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendanceReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendanceContinueReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendancePlayTimeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendancePlayTimeReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendanceReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendanceContinueReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLEvent::ReqAttendancePlayTimeReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLWeeklyMissionProcess Implementation
// ============================================================================

XSQLWeeklyMissionProcess::XSQLWeeklyMissionProcess() : TXSQLProcessBase<0x4A>("XSQLWeeklyMissionProcess") {}
XSQLWeeklyMissionProcess::~XSQLWeeklyMissionProcess() = default;

std::int32_t XSQLWeeklyMissionProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C62C0: SubCmd routing for WeeklyMission operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqWeeklyMissionLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqWeeklyMissionUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqWeeklyMissionReward(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqWeeklyMissionRewardWeek(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqWeeklyMissionReset(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLWeeklyMissionProcess::ReqWeeklyMissionLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWeeklyMissionProcess::ReqWeeklyMissionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWeeklyMissionProcess::ReqWeeklyMissionReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWeeklyMissionProcess::ReqWeeklyMissionRewardWeek(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLWeeklyMissionProcess::ReqWeeklyMissionReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLItemSetupProcess Implementation
// ============================================================================

XSQLItemSetupProcess::XSQLItemSetupProcess() : TXSQLProcessBase<0x81>("XSQLItemSetupProcess") {}
XSQLItemSetupProcess::~XSQLItemSetupProcess() = default;

std::int32_t XSQLItemSetupProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x14005F7B0: SubCmd routing for ItemSetup operations
    switch (static_cast<std::uint8_t>(xPacket.GetSubCmd())) {
    case 0x00: return ReqItemMake(pDBStmt, xPacket, xReturnSessionID);
    case 0x01: return ReqItemUpgrade(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqItemExchange(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqItemDisassemble(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqItemSocketEquip(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqItemSocketActive(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqItemSocketDetach(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqItemRepair(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqItemRepairNpc(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqItemRepairEquip(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqItemRepairAll(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqItemEndurance(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqItemEvolution(pDBStmt, xPacket, xReturnSessionID);
    case 0x13: return ReqItemAkashicMake(pDBStmt, xPacket, xReturnSessionID);
    case 0x14: return ReqAkashicDisassemble(pDBStmt, xPacket, xReturnSessionID);
    case 0x15: return ReqItemUpgradeLimit(pDBStmt, xPacket, xReturnSessionID);
    case 0x16: return ReqItemExpUpdate(pDBStmt, xPacket, xReturnSessionID);
    case 0x17: return ReqItemDisassembleEx(pDBStmt, xPacket, xReturnSessionID);
    case 0x18: return ReqItemBroachEquip(pDBStmt, xPacket, xReturnSessionID);
    case 0x19: return ReqItemBroachActive(pDBStmt, xPacket, xReturnSessionID);
    case 0x20: return ReqItemRestore(pDBStmt, xPacket, xReturnSessionID);
    case 0x21: return ReqItemBroachCompose(pDBStmt, xPacket, xReturnSessionID);
    case 0x22: return ReqItemUnSeal(pDBStmt, xPacket, xReturnSessionID);
    case 0x23: return ReqItemUseEffect(pDBStmt, xPacket, xReturnSessionID);
    case 0x24: return ReqItemRenovate(pDBStmt, xPacket, xReturnSessionID);
    case 0x25: return ReqItemBroachRemove(pDBStmt, xPacket, xReturnSessionID);
    case 0x26: return ReqItemRefine(pDBStmt, xPacket, xReturnSessionID);
    case 0x27: return ReqItemSocketExchange(pDBStmt, xPacket, xReturnSessionID);
    case 0x28: return ReqItemSocketUpgrade(pDBStmt, xPacket, xReturnSessionID);
    case 0x29: return ReqItemSocketExtract(pDBStmt, xPacket, xReturnSessionID);
    case 0x30: return ReqItemAkashicComposeEx(pDBStmt, xPacket, xReturnSessionID);
    case 0x31: return ReqItemAkashicGetInfoAdd(pDBStmt, xPacket, xReturnSessionID);
    case 0x32: return ReqItemAkashicGetInfoLoad(pDBStmt, xPacket, xReturnSessionID);
    case 0x33: return ReqItemDye(pDBStmt, xPacket, xReturnSessionID);
    case 0x34: return ReqItemTitleChange(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLItemSetupProcess::ReqItemMake(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemUpgrade(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemExchange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemDisassemble(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemSocketEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemSocketActive(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemSocketDetach(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRepair(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRepairNpc(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRepairEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRepairAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemEndurance(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemEvolution(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemAkashicMake(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqAkashicDisassemble(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemUpgradeLimit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemExpUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemDisassembleEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemBroachEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemBroachActive(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRestore(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemBroachCompose(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemUnSeal(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemUseEffect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRenovate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemBroachRemove(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemRefine(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemSocketExchange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemSocketUpgrade(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemSocketExtract(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemAkashicComposeEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemAkashicGetInfoAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemAkashicGetInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemDye(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLItemSetupProcess::ReqItemTitleChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLStatisticsProcess Implementation
// ============================================================================

XSQLStatisticsProcess::XSQLStatisticsProcess() : TXSQLProcessBase<0xF0>("XSQLStatisticsProcess") {}
XSQLStatisticsProcess::~XSQLStatisticsProcess() = default;

std::int32_t XSQLStatisticsProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400C22F0: SubCmd routing for Statistics operations
    switch (static_cast<std::uint8_t>(xPacket.GetSubCmd())) {
    case 0x01: return ReqCharacterCreate(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqCharacterSave(pDBStmt, xPacket, xReturnSessionID);
    case 0x03: return ReqItemExchange(pDBStmt, xPacket, xReturnSessionID);
    case 0x04: return ReqAkashic(pDBStmt, xPacket, xReturnSessionID);
    case 0x05: return ReqHelper(pDBStmt, xPacket, xReturnSessionID);
    case 0x06: return ReqQuest(pDBStmt, xPacket, xReturnSessionID);
    case 0x07: return ReqDailyMission(pDBStmt, xPacket, xReturnSessionID);
    case 0x08: return ReqSkill(pDBStmt, xPacket, xReturnSessionID);
    case 0x09: return ReqMyRoom(pDBStmt, xPacket, xReturnSessionID);
    case 0x10: return ReqMyRoomItem(pDBStmt, xPacket, xReturnSessionID);
    case 0x11: return ReqItem(pDBStmt, xPacket, xReturnSessionID);
    case 0x12: return ReqMapSave(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLStatisticsProcess::ReqCharacterCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqCharacterSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqItemExchange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqAkashic(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqHelper(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqQuest(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqDailyMission(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqSkill(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqMyRoom(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqMyRoomItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLStatisticsProcess::ReqMapSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLSGNetCafeProcess Implementation
// ============================================================================

XSQLSGNetCafeProcess::XSQLSGNetCafeProcess() : TXSQLProcessBase<0xF2>("XSQLSGNetCafeProcess") {}
XSQLSGNetCafeProcess::~XSQLSGNetCafeProcess() = default;

std::int32_t XSQLSGNetCafeProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x1400CE360: SubCmd routing for SGNetCafe operations
    if (static_cast<std::uint8_t>(xPacket.GetSubCmd()) == 0x01) {
        return ReqCharInfo(pDBStmt, xPacket, xReturnSessionID);
    }
    return -1;
}

std::int32_t XSQLSGNetCafeProcess::ReqCharInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }

// ============================================================================
// XSQLCommonProcess Implementation
// ============================================================================

XSQLCommonProcess::XSQLCommonProcess() : TXSQLProcessBase<0xF3>("XSQLCommonProcess") {}
XSQLCommonProcess::~XSQLCommonProcess() = default;

std::int32_t XSQLCommonProcess::DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
    // Per IDA 0x140010D10: SubCmd routing for Common operations
    switch (xPacket.GetSubCmd()) {
    case 0x01: return ReqKRRInsert(pDBStmt, xPacket, xReturnSessionID);
    case 0x02: return ReqKRRDelete(pDBStmt, xPacket, xReturnSessionID);
    default: return -1;
    }
}

std::int32_t XSQLCommonProcess::ReqKRRInsert(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }
std::int32_t XSQLCommonProcess::ReqKRRDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) { return -1; }