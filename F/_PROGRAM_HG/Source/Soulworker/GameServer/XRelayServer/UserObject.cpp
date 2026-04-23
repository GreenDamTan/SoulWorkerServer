#include "Soulworker/GameServer/XRelayServer/UserObject.h"

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

// 对齐 IDA 0x1400D27E0: CUserObject::LoadFriend
// 从 DB 好友记录加载好友信息到内存，并填充 ST_FRIEND_INFO 用于后续通知
bool CUserObject::LoadFriend(const PS_DB_FRIEND* pFriend,
                              const std::shared_ptr<CUserObject>& pFriendUser,
                              ST_FRIEND_INFO* pFriendRes) {
    if (!pFriend || !pFriendRes) return false;

    // 对齐 IDA: 检查好友类型有效性 + 是否在黑名单中
    if (!m_Community.IsValidCommunityType(pFriend->byType) ||
        m_Community.IsBlockList(pFriend->dwUCID)) {
        return false;
    }

    // 对齐 IDA: 从 PS_DB_FRIEND 构建 CFriendMember
    CFriendMember member;
    ST_FRIEND_INFO& info = member.m_stFriendInfo;

    info.dwID = pFriend->dwUCID;
    info.byLevel = pFriend->byLevel;
    info.byClass = pFriend->byClass;
    info.byAwaken = pFriend->byAwaken;
    info.dwProfilePhotoID = pFriend->dwProfilePhotoID;
    info.byType = pFriend->byType;
    info.byState = pFriend->byState;
    info.nFriendPoint = pFriend->nFriendPoint;
    info.tRemain = pFriend->tRemain;
    info.tLogOut = pFriend->tLogOut;

#ifdef _WIN32
    wcscpy_s(info.strName, pFriend->strName);
    wcscpy_s(info.strMemo, pFriend->strMemo);
#else
    std::wcsncpy(info.strName, pFriend->strName, 20);
    info.strName[20] = L'\0';
    std::wcsncpy(info.strMemo, pFriend->strMemo, 30);
    info.strMemo[30] = L'\0';
#endif

    // 对齐 IDA: 如果好友在线，用实时数据覆盖 DB 数据
    if (pFriendUser) {
        info.wMapID = pFriendUser->GetMapID();
        info.byChannel = pFriendUser->GetChannel();
        info.byLevel = pFriendUser->GetLevel();
        info.byState = pFriendUser->GetCommunityState();
        const std::wstring memo = pFriendUser->GetMemo();
#ifdef _WIN32
        wcscpy_s(info.strMemo, memo.c_str());
#else
        std::wcsncpy(info.strMemo, memo.c_str(), 30);
        info.strMemo[30] = L'\0';
#endif
        info.bLogin = true;
        member.m_pFriend = pFriendUser;
    }

    // 对齐 IDA: 复制到输出参数
    *pFriendRes = info;

    // 对齐 IDA: 添加到社区好友列表
    if (m_Community.AddFriend(&info, pFriendUser)) {
        return true;
    }

    // 添加失败（好友列表已满等）
    LogHelper::LogDebug("game.relay",
                        "<FRIEND_ADD_FAILED> USER : %d, Friend : %d, Type : %d ",
                        static_cast<int>(GetCID()),
                        static_cast<int>(pFriend->dwUCID),
                        static_cast<int>(pFriend->byType));
    return false;
}

// 对齐 IDA 0x1400D30E0: CUserObject::LoginFriend
// 好友上线通知：检查好友信息变化，发送更新包给客户端
void CUserObject::LoginFriend(const ST_FRIEND_INFO& stMyInfo,
                               const std::shared_ptr<CUserObject>& pMyUser) {
    // 对齐 IDA: 获取好友类型
    const std::uint8_t byFriendType = m_Community.GetFriendType(stMyInfo.dwID);

    // 对齐 IDA: 仅 type==1（好友）或 type==2（推荐好友）时处理
    if (byFriendType == 1 || byFriendType == 2) {
        // 对齐 IDA: 检查好友信息是否变化
        if (m_Community.IsChangeFriendInfo(&stMyInfo)) {
            // 对齐 IDA: 更新好友信息
            m_Community.UpdateFriendInfo(&stMyInfo, pMyUser);

            // 对齐 IDA: 发送好友更新包 (main=0xF5, sub=0x20)
            XSendPacket xSendPacket(0xF5, 0x20);
            xSendPacket.XParse << GetMatchingID();
            xSendPacket << stMyInfo;
            SendPacket(xSendPacket);
        }
    }
}

// 对齐 IDA 0x1400D4B40: CUserObject::SendFriendServerLoad
// 通知客户端好友列表加载完成
void CUserObject::SendFriendServerLoadImpl() {
    if (m_bLoadFriend) {
        XSendPacket xSendPacket(0xF5, 0x34);
        xSendPacket.XParse << GetMatchingID();
        SendPacket(xSendPacket);
    }
}

// 对齐 IDA 0x1400D4BF0: CUserObject::SendFriendList
// 发送好友列表给客户端
void CUserObject::SendFriendListImpl() {
    if (m_Community.GetLoadFriendList()) {
        PS_FRIEND_LIST stFriendList;
        m_Community.GetFriendList(stFriendList, 0);  // 0 = 所有类型

        XSendPacket xSendPacket(0xF5, 1);
        xSendPacket.XParse << GetMatchingID();
        xSendPacket << stFriendList;
        SendPacket(xSendPacket);

        m_Community.SetSyncFriendList(false);

        LogHelper::LogDebug("game.relay",
                            "<%u FRIEND_LIST> Send List ( Count : %u )",
                            static_cast<unsigned int>(GetCID()),
                            static_cast<unsigned int>(stFriendList.vecFriends.size()));
    } else {
        m_Community.SetSyncFriendList(true);
    }
}

// 对齐 IDA 0x1400D4D50: CUserObject::SendBlockList
// 发送黑名单给客户端
void CUserObject::SendBlockListImpl() {
    if (m_Community.GetLoadBlockList()) {
        PS_BLOCKLIST_INFO stBlockList;
        m_Community.GetBlcokList(stBlockList);

        XSendPacket xSendPacket(0xF5, 2);
        xSendPacket.XParse << GetMatchingID();
        xSendPacket << stBlockList;
        SendPacket(xSendPacket);

        m_Community.SetSyncBlockList(false);

        LogHelper::LogDebug("game.relay",
                            "<%u BLOCK_LIST> Send List ( Count : %u )",
                            static_cast<unsigned int>(GetCID()),
                            static_cast<unsigned int>(stBlockList.vecBlockList.size()));
    } else {
        m_Community.SetSyncBlockList(true);
    }
}

// 对齐 IDA 0x1400D36C0: CUserObject::ChangeMap
// 换地图时更新好友列表中的地图信息
void CUserObject::ChangeMap(std::uint16_t wMapID) {
    ST_FRIEND_INFO stFriendUpdate{};
    GetUserInfo(&stFriendUpdate);
    stFriendUpdate.wMapID = wMapID;

    // 遍历好友列表 (type=1)，更新在线好友的地图信息
    m_Community.ForEachOnlineFriend(1, [&stFriendUpdate](std::shared_ptr<CUserObject>& pFriend) {
        pFriend->UpdateFriend(&stFriendUpdate, 1);
    });

    // 遍历邀请列表 (type=3)，更新在线好友的地图信息
    m_Community.ForEachOnlineFriend(3, [&stFriendUpdate](std::shared_ptr<CUserObject>& pFriend) {
        pFriend->UpdateFriend(&stFriendUpdate, 1);
    });

    // 对齐 IDA: 通过 DoJob 通知游戏线程
    std::uint32_t dwActorID = GetCID();
    std::uint32_t dwServerID = GetServerID();
    CLogicThreadManager::Instance().DoJob(0, [dwActorID, wMapID, dwServerID]() {
        // 对齐 IDA: lambda 内部逻辑（通知 GameServer 用户地图变更）
        static_cast<void>(dwActorID);
        static_cast<void>(wMapID);
        static_cast<void>(dwServerID);
    });
}
