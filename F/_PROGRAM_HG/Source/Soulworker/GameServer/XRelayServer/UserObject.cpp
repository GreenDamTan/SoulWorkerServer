#include "Soulworker/GameServer/XRelayServer/UserObject.h"

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

// 对齐 IDA 0x1400D27E0: CUserObject::LoadFriend
// 从 DB 好友记录加载好友信息到内存，并填充 ST_FRIEND_INFO 用于后续通知
bool CUserObject::LoadFriend(PS_DB_FRIEND& stDbFriend,
                              std::shared_ptr<CUserObject> pFriendUser,
                              ST_FRIEND_INFO& stFriendRes) {
    // 对齐 IDA: 检查好友类型有效性 + 是否在黑名单中
    if (!m_Community.IsValidCommunityType(stDbFriend.byType) ||
        m_Community.IsBlockList(stDbFriend.dwUCID)) {
        return false;
    }

    // 对齐 IDA: 堆分配 CFriendMember 并包装为 shared_ptr
    auto pFriendMember = std::make_shared<CFriendMember>();
    ST_FRIEND_INFO& info = pFriendMember->m_stFriendInfo;

    info.dwID = stDbFriend.dwUCID;
    info.byLevel = stDbFriend.byLevel;
    info.byClass = stDbFriend.byClass;
    info.byAwaken = stDbFriend.byAwaken;
    info.dwProfilePhotoID = stDbFriend.dwProfilePhotoID;
    info.byType = stDbFriend.byType;
    info.byState = stDbFriend.byState;
    info.nFriendPoint = stDbFriend.nFriendPoint;
    info.tRemain = stDbFriend.tRemain;
    info.tLogOut = stDbFriend.tLogOut;

#ifdef _WIN32
    wcscpy_s(info.strName, stDbFriend.strName);
    wcscpy_s(info.strMemo, stDbFriend.strMemo);
#else
    std::wcsncpy(info.strName, stDbFriend.strName, 20);
    info.strName[20] = L'\0';
    std::wcsncpy(info.strMemo, stDbFriend.strMemo, 30);
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
        pFriendMember->m_pFriend = pFriendUser;
    }

    // 对齐 IDA: 复制到输出参数
    stFriendRes = info;

    // 对齐 IDA: 添加到社区好友列表（传递 shared_ptr）
    if (m_Community.AddFriend(pFriendMember)) {
        return true;
    }

    // 添加失败（好友列表已满等）
    LogHelper::LogDebug("game.relay",
                        "<FRIEND_ADD_FAILED> USER : %d, Friend : %d, Type : %d ",
                        static_cast<int>(GetCID()),
                        static_cast<int>(stDbFriend.dwUCID),
                        static_cast<int>(stDbFriend.byType));
    return false;
}

// 对齐 IDA 0x1400D30E0: CUserObject::LoginFriend
// 好友上线通知：检查好友信息变化，发送更新包给客户端
void CUserObject::LoginFriend(ST_FRIEND_INFO& stMyInfo,
                               std::shared_ptr<CUserObject> pMyUser) {  // 对齐 IDA: 非const引用 + 按值shared_ptr
    // 对齐 IDA: 获取好友类型
    const std::uint8_t byFriendType = m_Community.GetFriendType(stMyInfo.dwID);

    // 对齐 IDA: 仅 type==1（好友）或 type==2（推荐好友）时处理
    if (byFriendType == 1 || byFriendType == 2) {
        // 对齐 IDA: 检查好友信息是否变化
        ST_FRIEND_INFO stMyInfoCopy = stMyInfo;
        if (m_Community.IsChangeFriendInfo(stMyInfoCopy)) {
            // 对齐 IDA: 更新好友信息
            m_Community.UpdateFriendInfo(stMyInfoCopy, pMyUser);

            // 对齐 IDA: 发送好友更新包 (main=0xF5, sub=0x20)
            XSendPacket xSendPacket(0xF5, 0x20);
            xSendPacket.XParse << GetMatchingID();
            xSendPacket << stMyInfoCopy;
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
void CUserObject::ChangeMap(std::uint16_t wMapID) {  // 对齐 IDA: 参数 G (unsigned short = uint16_t)
    ST_FRIEND_INFO stFriendUpdate{};
    GetUserInfo(stFriendUpdate);
    stFriendUpdate.wMapID = static_cast<std::uint16_t>(wMapID);

    // 对齐 IDA: 使用对象列表 GetFriendList 替代 ForEachOnlineFriend
    std::vector<std::shared_ptr<CFriendMember>> vecFriendList;

    // 遍历好友列表 (type=1)，更新在线好友的地图信息
    m_Community.GetFriendList(vecFriendList, 1);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stFriendUpdate, 1);
        }
    }

    // 遍历邀请列表 (type=3)，更新在线好友的地图信息
    m_Community.GetFriendList(vecFriendList, 3);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stFriendUpdate, 1);
        }
    }

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

// 对齐 IDA 0x1400D3AF0: CUserObject::Levelup
// 升级时通知好友列表
void CUserObject::Levelup(std::uint8_t byLevel) {
    SetLevel(byLevel);

    ST_FRIEND_INFO stMyUserInfo{};
    GetUserInfo(stMyUserInfo);
    stMyUserInfo.byLevel = byLevel;

    // 对齐 IDA: 使用对象列表 GetFriendList 替代 ForEachOnlineFriend
    std::vector<std::shared_ptr<CFriendMember>> vecFriendList;

    // 遍历好友列表 (type=1)，更新在线好友
    m_Community.GetFriendList(vecFriendList, 1);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stMyUserInfo, 1);
        }
    }

    // 遍历邀请列表 (type=3)，更新在线好友
    m_Community.GetFriendList(vecFriendList, 3);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stMyUserInfo, 1);
        }
    }

    // 对齐 IDA: 通过 DoJob 通知游戏线程
    std::uint32_t dwActorID = GetCID();
    CLogicThreadManager::Instance().DoJob(0, [dwActorID, byLevel]() {
        static_cast<void>(dwActorID);
        static_cast<void>(byLevel);
    });
}

// 对齐 IDA 0x1400D3EC0: CUserObject::UpdateProfilePhoto
// 更新头像时通知好友列表
void CUserObject::UpdateProfilePhoto(std::uint32_t dwPhotoID) {
    SetProfilePhoto(dwPhotoID);

    ST_FRIEND_INFO stMyUserInfo{};
    GetUserInfo(stMyUserInfo);
    stMyUserInfo.dwProfilePhotoID = dwPhotoID;

    // 对齐 IDA: 使用对象列表 GetFriendList 替代 ForEachOnlineFriend
    std::vector<std::shared_ptr<CFriendMember>> vecFriendList;

    // 遍历好友列表 (type=1)，更新在线好友
    m_Community.GetFriendList(vecFriendList, 1);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stMyUserInfo, 1);
        }
    }

    // 遍历邀请列表 (type=3)，更新在线好友
    m_Community.GetFriendList(vecFriendList, 3);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stMyUserInfo, 1);
        }
    }
}

// 对齐 IDA 0x1400D4EA0: CUserObject::SendUpdateCommunity
// 发送社区状态更新给好友
void CUserObject::SendUpdateCommunity() {
    ST_FRIEND_COMMUNITY stCommunity{};
    stCommunity.byState = m_Community.GetCommunityState();
    const std::wstring memo = m_Community.GetMemo();
#ifdef _WIN32
    wcscpy_s(stCommunity.strMemo, memo.c_str());
#else
    std::wcsncpy(stCommunity.strMemo, memo.c_str(), 30);
    stCommunity.strMemo[30] = L'\0';
#endif

    // 对齐 IDA: 使用对象列表 GetFriendList 替代 ForEachOnlineFriend
    // 遍历所有好友列表 (type=0 = 全部)
    std::vector<std::shared_ptr<CFriendMember>> vecFriendList;
    m_Community.GetFriendList(vecFriendList, 0);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            XSendPacket xSendPacket(0xF5, 0x21);
            xSendPacket.XParse << pFriendMember->m_pFriend->GetCID();
            xSendPacket.XParse << GetMatchingID();
            xSendPacket << stCommunity;
            pFriendMember->m_pFriend->SendPacket(xSendPacket);
        }
    }
}

// 对齐 IDA 0x1400D5310: CUserObject::ChangeFriendName
// 更名后通知好友列表更新名称
void CUserObject::ChangeFriendName(PS_CHANGE_NAME stChangeName) {
    ST_FRIEND_INFO stFriendUpdate{};
    GetUserInfo(stFriendUpdate);
#ifdef _WIN32
    wcscpy_s(stFriendUpdate.strName, stChangeName.szChangeName);
#else
    std::wcsncpy(stFriendUpdate.strName, stChangeName.szChangeName, 20);
    stFriendUpdate.strName[20] = L'\0';
#endif

    // 对齐 IDA: 使用对象列表 GetFriendList 替代 ForEachOnlineFriend
    std::vector<std::shared_ptr<CFriendMember>> vecFriendList;

    // 遍历好友列表 (type=1)，更新在线好友
    m_Community.GetFriendList(vecFriendList, 1);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stFriendUpdate, 1);
        }
    }

    // 遍历邀请列表 (type=3)，更新在线好友
    m_Community.GetFriendList(vecFriendList, 3);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stFriendUpdate, 1);
        }
    }
}

// 对齐 IDA 0x1400D2D30: CUserObject::LoadBlock
// 从 DB 记录加载黑名单到内存
bool CUserObject::LoadBlock(ST_BLOCK_INFO& stBlockInfo) {
    // 对齐 IDA: 检查是否已是好友
    if (m_Community.IsFriend(stBlockInfo.dwUCID, 1u)) {
        return false;
    }

    // 对齐 IDA: 添加到黑名单
    return m_Community.AddBlockList(stBlockInfo);
}

// 对齐 IDA 0x1400D3270: CUserObject::Logout
// 登出处理：通知好友下线，清理资源
void CUserObject::Logout() {
    // 对齐 IDA: 获取用户信息并设置下线状态
    ST_FRIEND_INFO stFriendUpdate{};
    GetUserInfo(stFriendUpdate);
    stFriendUpdate.bLogin = false;
    // 对齐 IDA: ATL::CTime::GetTickCount() → tLogOut
    stFriendUpdate.tLogOut = static_cast<std::int64_t>(std::time(nullptr));
    stFriendUpdate.wMapID = 0;
    stFriendUpdate.byChannel = 0;

    // 对齐 IDA: 使用对象列表 GetFriendList 替代 ForEachOnlineFriend
    std::vector<std::shared_ptr<CFriendMember>> vecFriendList;

    // 遍历好友列表 (type=1)，通知在线好友下线
    m_Community.GetFriendList(vecFriendList, 1);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stFriendUpdate, 1);
        }
    }

    // 遍历邀请列表 (type=3)，通知在线好友下线
    m_Community.GetFriendList(vecFriendList, 3);
    for (const auto& pFriendMember : vecFriendList) {
        if (pFriendMember->m_pFriend) {
            pFriendMember->m_pFriend->UpdateFriend(stFriendUpdate, 1);
        }
    }

    // 对齐 IDA: DoJob(0, lambda with dwActorID)
    std::uint32_t dwActorID = GetCID();
    CLogicThreadManager::Instance().DoJob(0, [dwActorID]() {
        // 对齐 IDA: lambda 内部逻辑
        static_cast<void>(dwActorID);
    });

    // 对齐 IDA: DoJob(1, lambda with dwCID)
    std::uint32_t dwCID = GetMatchingID();
    CLogicThreadManager::Instance().DoJob(1, [dwCID]() {
        // 对齐 IDA: lambda 内部逻辑
        static_cast<void>(dwCID);
    });
}

// 对齐 IDA: CCommunity::AddBlockList (兼容接口，内部使用 AddBlock)
bool CCommunity::AddBlockList(ST_BLOCK_INFO& stBlock) {
    // 对齐 IDA: 堆分配 CBlockUser 并包装为 shared_ptr
    auto pBlockUser = std::make_shared<CBlockUser>();
    pBlockUser->m_stBlockInfo = stBlock;
    return AddBlock(pBlockUser);
}

// 对齐 IDA 0x140002570: CCommunity::DeleteBlockList
void CCommunity::DeleteBlockList(std::uint32_t dwUCID) {
    m_vecBlockList.erase(
        std::remove_if(m_vecBlockList.begin(), m_vecBlockList.end(),
                       [dwUCID](const std::shared_ptr<CBlockUser>& u) { return u->GetUCID() == dwUCID; }),
        m_vecBlockList.end());
}
