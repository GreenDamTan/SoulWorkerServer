#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
class CServer;
class CUserObject;

// CLeague and CLeagueMember are referenced but defined elsewhere
class CLeague;
class CLeagueMember;

// ============================================================================
// League info structures - defined before CLeagueManager for use in signatures
// ============================================================================

// 对齐 IDA ST_LEAGUE_INFO (总大小 0x800 = 2048 bytes)
struct ST_LEAGUE_INFO {
    std::int32_t nLeagueID = 0;                     // offset 0x0
    std::int32_t nLeagueRank = 0;                   // offset 0x4
    std::uint8_t byGroupType = 0;                   // offset 0x8
    std::uint8_t byRating = 0;                      // offset 0x9
    std::int16_t shMemberCount = 0;                 // offset 0xa
    // padding to 0x10
    std::int64_t biExp = 0;                         // offset 0x10
    wchar_t szLeagueName[10] = {};                  // offset 0x18, size 20
    // padding to 0x30
    std::int64_t biMoney = 0;                       // offset 0x30
    std::int64_t nCreateDate = 0;                   // offset 0x38
    std::int64_t biNoticeDate = 0;                  // offset 0x40
    std::uint32_t dwMasterUCID = 0;                 // offset 0x48
    wchar_t szMasterName[21] = {};                  // offset 0x4c, size 42
    wchar_t szSubMasterName[21] = {};               // offset 0x76, size 42
    std::int32_t nAuth[9] = {};                     // offset 0xa0, size 36
    std::int32_t nLimitGoldOut[9] = {};             // offset 0xc4, size 36
    bool bOpen = false;                             // offset 0xe8
    // padding to 0xec
    union {                                         // offset 0xec, size 4
        std::uint32_t dwLeagueCard = 0;
    };
    wchar_t szNotice[801] = {};                     // offset 0xf0, size 1602
    wchar_t szPosition_1[11] = {};                  // offset 0x732, size 22
    wchar_t szPosition_2[11] = {};                  // offset 0x748, size 22
    wchar_t szPosition_3[11] = {};                  // offset 0x75e, size 22
    wchar_t szRecruitNotice[51] = {};               // offset 0x774, size 102
    std::int64_t biRecruitNoticeDate = 0;           // offset 0x7e0
    std::uint8_t bySkillPoint = 0;                  // offset 0x7e8
    std::uint8_t bySkill[8] = {};                   // offset 0x7e9
    std::int32_t nLimitExp = 0;                     // offset 0x7f4
    std::int64_t biInitDate = 0;                    // offset 0x7f8
};

// 对齐 IDA ST_LEAGUE_INFO_UPDATE (32 bytes)
struct ST_LEAGUE_INFO_UPDATE {
    std::int32_t nLeagueID = 0;             // offset 0x0
    std::int32_t nLeagueRank = 0;           // offset 0x4
    std::int64_t biLeagueMoney = 0;         // offset 0x8
    std::int16_t shLeagueMemeberCnt = 0;    // offset 0x10
    std::uint8_t _pad0[2] = {};             // offset 0x12
    std::uint32_t dwLeagueCard = 0;         // offset 0x14
    std::int64_t biExp = 0;                 // offset 0x18
};

// ST_LEAGUE_INFO_UPDATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_INFO_UPDATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nLeagueRank;
    packet.XParse << value.biLeagueMoney;
    packet.XParse << value.shLeagueMemeberCnt;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.dwLeagueCard;
    packet.XParse << value.biExp;
    return packet;
}

// ST_LEAGUE_INFO_EX 已在 PSServer.h 中定义

struct ST_LEAGUE_MEMBER_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_MEMBER_EX> vecInfo;
};

struct ST_LEAGUE_BOARD_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_BOARD> vecInfo;
};

struct ST_LEAGUE_APPLICANT_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_APPLICANT> vecInfo;
};

struct ST_LEAGUE_RECORD_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_RECORD> vecInfo;
};

struct ST_LEAGUE_APPLICANT_CHECK_LIST {
    std::int32_t nCount = 0;
    std::vector<std::uint32_t> vecInfo;
};

struct ST_LEAGUE_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_INFO> vecInfo;
};

// 对齐 IDA 命名约定 - 扩展以匹配 SP_LEAGUE_LIST 返回
struct PS_LEAGUE_INFO_SUMMARY {
    std::int32_t nLeagueID = 0;
    std::int32_t nMemberCount = 0;
    std::uint8_t byRating = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwLeagueCard = 0;     // union ___u3
    wchar_t szLeagueName[10] = {};      // szName (20 bytes)
    wchar_t szMaster[21] = {};          // szMaster (42 bytes)
    wchar_t szSubMaster[21] = {};       // szSubMaster (42 bytes)
    wchar_t szRecruit[51] = {};         // szRecruit (102 bytes)
    // Legacy fields for compatibility
    std::uint32_t dwMasterUCID = 0;
    std::int32_t nLeagueRank = 0;
    std::int32_t nApplicantCount = 0;
    bool bOpen = false;
};

struct PS_LEAGUE_SUMMARY_LIST {
    std::int32_t nCount = 0;
    std::vector<PS_LEAGUE_INFO_SUMMARY> vecInfo;
};

// PS_CHANGE_NAME 已在 PSServer.h 中定义，此处仅引用
struct PS_SERVER_CHANGE_CHARACTER_NAME {
    PS_CHANGE_NAME psChangeInfo{};
    ST_PARTY_INFO stPartyInfo{};
    std::int32_t nLeagueID = 0;
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList{};
};

// 对齐 IDA 0x1400E5850: ST_LEAGUE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.nLeagueRank;
    packet.XParse >> value.byGroupType;
    packet.XParse >> value.byRating;
    packet.XParse >> value.shMemberCount;
    packet.XParse >> value.biExp;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse >> value.biMoney;
    packet.XParse >> value.nCreateDate;
    packet.XParse >> value.biNoticeDate;
    packet.XParse >> value.dwMasterUCID;
    packet.XParse.GetWString(value.szMasterName, 21, outLen);
    packet.XParse.GetWString(value.szSubMasterName, 21, outLen);
    for (int i = 0; i < 9; ++i) packet.XParse >> value.nAuth[i];
    for (int i = 0; i < 9; ++i) packet.XParse >> value.nLimitGoldOut[i];
    packet.XParse >> value.dwLeagueCard;
    packet.XParse.GetWString(value.szNotice, 801, outLen);
    packet.XParse.GetWString(value.szPosition_1, 11, outLen);
    packet.XParse.GetWString(value.szPosition_2, 11, outLen);
    packet.XParse.GetWString(value.szPosition_3, 11, outLen);
    packet.XParse >> value.bOpen;
    packet.XParse.GetWString(value.szRecruitNotice, 51, outLen);
    packet.XParse >> value.biRecruitNoticeDate;
    packet.XParse >> value.bySkillPoint;
    for (int j = 0; j < 8; ++j) packet.XParse >> value.bySkill[j];
    packet.XParse >> value.nLimitExp;
    packet.XParse >> value.biInitDate;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_BOARD_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_APPLICANT_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_RECORD_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_APPLICANT_CHECK_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_LIST& value) {
    packet.XParse >> value.nCount;
}

// PS_LEAGUE_INFO_SUMMARY 反序列化
inline void operator>>(XPacket& packet, PS_LEAGUE_INFO_SUMMARY& value) {
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse >> value.dwMasterUCID;
    packet.XParse >> value.nLeagueRank;
    packet.XParse >> value.nMemberCount;
    packet.XParse >> value.nApplicantCount;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value.bOpen), 1);
}

// PS_LEAGUE_SUMMARY_LIST 反序列化
inline void operator>>(XPacket& packet, PS_LEAGUE_SUMMARY_LIST& value) {
    packet.XParse >> value.nCount;
    value.vecInfo.clear();
    for (std::int32_t i = 0; i < value.nCount && i < 100; ++i) {
        PS_LEAGUE_INFO_SUMMARY item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// 对齐 IDA 0x1400E27C0: PS_SERVER_CHANGE_CHARACTER_NAME 反序列化
inline void operator>>(XPacket& packet, PS_SERVER_CHANGE_CHARACTER_NAME& value) {
    packet >> value.psChangeInfo;
    packet >> value.stPartyInfo;
    packet.XParse >> value.nLeagueID;
    packet >> value.stApplyList;
}

// ST_LEAGUE_APPLICANT_CHECK_LIST 输出序列化前置声明（完整定义在文件末尾）
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_APPLICANT_CHECK_LIST& value);

// 对齐 IDA 0x1400E9DD0: PS_SERVER_CHANGE_CHARACTER_NAME 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_CHANGE_CHARACTER_NAME& value) {
    packet << value.psChangeInfo;
    packet << value.stPartyInfo;
    packet.XParse << value.nLeagueID;
    packet << value.stApplyList;
    return packet;
}

/**
 * @brief 联赛管理器
 *
 * 管理联赛的创建、成员管理、邀请和相关操作。
 * 方法通过工作线程分发从 CLeagueProcess 处理器调用。
 */
class CLeagueManager {
public:
    CLeagueManager() = default;
    ~CLeagueManager() = default;

    // 禁止拷贝
    CLeagueManager(const CLeagueManager&) = delete;
    CLeagueManager& operator=(const CLeagueManager&) = delete;

    // 联赛生命周期
    void ReqLeagueCreate(CServer* pServer, PS_LEAGUE_CREATE_FOR_SERVER stCreate);  // 对齐 IDA: 按值传递
    void ResCreateLeague(CServer* pServer, PS_LEAGUE_CREATE_FOR_SERVER& stCreate);
    void CreateLeague(PS_LEAGUE_CREATE_FOR_SERVER stCreateInfo, ST_LEAGUE_INFO& stLeagueInfo, ST_LEAGUE_MEMBER_EX& stMemberInfo, std::uint8_t byChannel);  // 对齐 IDA 0x1400797C0: 第一参数按值传递
    void ReqLeagueDel(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID, std::int64_t biPenalty);
    void ResLeagueDel(CServer* pServer, std::uint32_t dwUCID, std::int32_t nLeagueID, std::int64_t biPenalty, std::int32_t nErrorCode);
    void DelLeague(std::int32_t nLeagueID);  // 对齐 IDA: 原名 DelLeague
    void Clear();

    // 联赛信息
    void ReqLeagueInfo(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID);
    bool GetMemberInfo(std::int32_t nLeagueID, std::uint32_t dwActorID, ST_LEAGUE_MEMBER_EX& stMember);
    void AddLeague(ST_LEAGUE_INFO stInfo, ST_LEAGUE_MEMBER_EX stMember);

    // 联赛登录/登出
    bool ReqLeagueLogin(std::uint32_t dwUCID, std::int32_t nLeagueID);
    void LogOutLeagueMember(std::int32_t nLeagueID, std::uint32_t dwActorID, std::int64_t biLogoutDate);  // 对齐 IDA 0x14007B360: HK_J
    void SendFailLeagueLogin(std::uint32_t dwUCID);

    // 成员管理
    void DeleteLeagueMember(std::int32_t nLeagueID, std::uint32_t dwActorID, wchar_t* pName);
    void ResLeagueWithdraw(std::int32_t nLeagueID, std::uint32_t dwUCID, std::int64_t biPenalty);
    void SendLeagueMemberWithdraw(std::int32_t nLeagueID, std::uint32_t dwUCID, std::int64_t biPenalty, ST_LEAGUE_INFO_UPDATE& stUpdate, wchar_t* pName);
    void ResLeagueKickout(std::uint32_t dwUCID, std::uint32_t dwKickoutUCID, std::int32_t nLeagueID, std::int32_t nErrorCode, CServer* pServer);

    // 邀请管理
    void ReqLeagueInvite(CServer* pServer, ST_REQ_LEAGUE_INVITE& stInvite, std::shared_ptr<CUserObject> pUser);  // 对齐 IDA: 非const引用
    void ReqInviteAccept(CServer* pServer, ST_REQ_LEAGUE_INVITE_ACCEPT& stAccept, std::int64_t biJoinDate);  // 对齐 IDA: 非const引用
    void ResInviteUser(CServer* pServer, std::int32_t nLeagueID, ST_LEAGUE_MEMBER_EX stMemberEx, std::uint32_t dwReqActorID);
    void SendLeagueInviteJoin(ST_LEAGUE_MEMBER_EX& stMemberEx, std::uint8_t byApplyState, ST_LEAGUE_INFO_EX& stInfoEx,
                              ST_LEAGUE_INFO_UPDATE& stInfoUpdate, std::uint32_t dwActorID,
                              ST_LEAGUE_MEMBER_LIST& stMemberList, ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame,
                              std::int32_t nSyncCount);
    void AddInviteUser(std::uint32_t dwUCID, std::int32_t nLeagueID);
    bool CheckInviteUser(std::uint32_t dwUCID);
    std::int32_t DeleteInviteUser(std::uint32_t dwActorID);  // 对齐 IDA 0x140078110: 返回int32(H), 参数K

    // 申请者管理
    void ReqLeagueApplicant(ST_LEAGUE_APPLICANT& stApplicant, CServer* pServer);  // 对齐 IDA: 非const引用
    void ResLeagueApplicant(CServer* pServer, ST_LEAGUE_APPLICANT stApplicant);  // 对齐 IDA: 按值传递
    void ReqLeagueApplicantAccept(CServer* pServer, ST_REQ_LEAGUE_APPLICANT_ACCEPT& stAccept, std::uint32_t dwActorID);  // 对齐 IDA: 非const引用
    void ReqLeagueApplicantReject(CServer* pServer, ST_REQ_LEAGUE_APPLICANT_REJECT stReject);  // 对齐 IDA: 按值传递
    void AppliCantJoinSucc(CServer* pServer, ST_REQ_LEAGUE_APPLICANT_ACCEPT& stAccept, ST_LEAGUE_MEMBER_EX& stMemberEx, std::uint32_t dwActorID);  // 对齐 IDA: 非const引用
    void SendLeagueApplicantJoin(ST_LEAGUE_MEMBER_EX& stMemberEx, ST_LEAGUE_INFO_EX& stInfoEx,
                                  ST_LEAGUE_INFO_UPDATE& stInfoUpdate, std::uint32_t dwActorID,
                                  ST_LEAGUE_MEMBER_LIST& stMemberList, ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame,
                                  std::int32_t nSyncCount);
    void DeleteApplicantList(CServer* pServer, std::uint32_t dwActorID);
    void ApplicantRejectSucc(CServer* pServer, ST_REQ_LEAGUE_APPLICANT_REJECT stReject);
    void ResLeagueApplicantDelete_TimeOver(std::int32_t nLeagueID, std::uint32_t dwActorID);

    // 公告板管理
    void ReqLeagueBoard(CServer* pServer, std::int32_t nLeagueID, std::uint32_t dwActorID, ST_LEAGUE_BOARD stBoard);  // 对齐 IDA: 参数顺序 + 按值传递
    void ResLeagueBoard(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID, ST_LEAGUE_BOARD stBoard);

    // 公告管理
    void ReqLeagueNoticeChange(CServer* pServer, std::uint32_t dwActorID, ST_LEAGUE_NOTICE& stNotice);  // 对齐 IDA: 非const引用
    void ResLeagueNoticeChange(CServer* pServer, ST_LEAGUE_NOTICE stNotice, std::uint32_t dwActorID);
    void ReqLeagueRecruitNotice(CServer* pServer, std::uint32_t dwActorID, ST_LEAGUE_RECRUIT_NOTICE& stNotice);  // 对齐 IDA: 非const引用
    void ResLeagueRecruitNotice(CServer* pServer, std::uint32_t dwActorID, ST_LEAGUE_RECRUIT_NOTICE& stNotice);  // 对齐 IDA: 非const引用

    // 权限和职位
    void ReqLeagueChangeAuth(CServer* pServer, std::int32_t nLeagueID, std::uint32_t dwActorID, ST_LEAGUE_AUTH_CHANGE& stAuth);  // 对齐 IDA: 非const引用
    void ResLeagueAuthChange(CServer* pServer, std::int32_t nLeagueID, ST_LEAGUE_AUTH_CHANGE stChange, std::uint32_t dwActorID);
    void ReqLeaguePositionNameChange(CServer* pServer, std::int32_t nLeagueID, ST_LEAGUE_POSITION_NAME_CHANGE stChange, std::uint32_t dwSomething);  // 对齐 IDA: 按值传递
    void ResLeaguePositionNameChange(CServer* pServer, std::int32_t nLeagueID, std::uint32_t dwActorID, ST_LEAGUE_POSITION_NAME_CHANGE stChange);
    void ReqLeagueMemberPositionChange(CServer* pServer, ST_LEAGUE_MEMBER_POSITION& stPos, std::int32_t nLeagueID, std::uint32_t dwActorID);  // 对齐 IDA: 非const引用 + 参数顺序
    void ResLeagueMemberPositionChange(CServer* pServer, ST_LEAGUE_MEMBER_POSITION& stPosition, std::int32_t nLeagueID, std::uint32_t dwActorID);  // 对齐 IDA: 非const引用

    // 退出/踢人
    bool ReqLeagueWithDraw(CServer* pServer, UXActorID uxActorID, std::int32_t nLeagueID, std::int64_t biPenalty);  // 对齐 IDA: 返回bool
    bool ReqLeagueKick(CServer* pServer, std::uint32_t dwActorID, std::uint32_t dwTargetID, std::int32_t nLeagueID);
    void SendLeagueMemberKick(CServer* pServer, std::int32_t nErrorCode, std::int32_t nLeagueID, std::uint32_t dwUCID, std::uint32_t dwTargetUCID, ST_LEAGUE_INFO_UPDATE& stUpdate, std::int16_t shLevel, wchar_t* pName);  // 对齐 IDA: shLevel类型int16_t

    // 转让
    void ReqLeagueDelegate(CServer* pServer, std::uint32_t dwReqUCID, PS_REQ_LEAGUE_DELEGATE& stDelegate, bool bGMDelegate);  // 对齐 IDA: 非const引用
    void ResLeagueDelegate(CServer* pServer, std::uint32_t dwReqUCID, PS_REQ_LEAGUE_DELEGATE& stDelegate, int nErrorCode);  // 对齐 IDA: 非const引用

    // 开放/关闭
    void ReqLeagueOpenOrNot(CServer* pServer, ST_LEAGUE_OPEN& stOpen, std::uint32_t dwActorID);  // 对齐 IDA: 非const引用
    void ResLeagueOpenOrNot(CServer* pServer, ST_LEAGUE_OPEN& stOpen, std::uint32_t dwUCID);  // 对齐 IDA: 非const引用

    // 名称变更
    bool ReqLeagueNameChange(PS_LEAGUE_NAME_CHANGE_SERVER& stChange);  // 对齐 IDA: 非const引用
    bool ResLeaugeNameChange(PS_LEAGUE_NAME_CHANGE_SERVER& stChange);  // 对齐 IDA: 原始拼写 ResLeaugeNameChange

    // 卡片变更
    void ReqLeagueCardChange(CServer* pServer, std::uint32_t dwActorID, PS_REQ_LEAGUE_CARD& stCard, PS_RES_STORAGE_INFO stStorage);  // 对齐 IDA: 非const引用 + 按值传递
    void ResLeagueCardChange(CServer* pServer, PS_REQ_LEAGUE_CARD& stCard, std::uint32_t dwUCID, PS_RES_STORAGE_INFO stStorage, int nErrorCode);  // 对齐 IDA: stStorage按值传递

    // 技能和等级
    void ReqLeagueSkillLearn(CServer* pServer, PS_REQ_LEAGUE_SKILL stSkill);  // 对齐 IDA: 按值传递
    void ResLeagueSkillLearn(PS_RES_LEAGUE_SKILL stSkill, std::uint8_t byType);  // 对齐 IDA: 按值传递
    void ReqLeagueLevelup(std::int32_t nLeagueID, std::uint8_t byType, std::uint32_t dwActorID);
    void ResLeagueLevelup(std::int32_t nLeagueID, std::uint8_t byLevel, std::uint8_t bySkillPoint, PS_AUTO_SKILL stSkill, std::uint32_t dwUCID);  // 对齐 IDA: 按值传递
    void ReqLeagueSkillPointUpdate(std::int32_t nLeagueID, std::uint8_t byType, std::uint32_t dwSomething);

    // 财富
    void ReqApplyLeagueExp(PS_LEAGUE_WEALTH_FOR_SERVER stWealth);  // 对齐 IDA: 按值传递
    void ResApplyLeagueWealth(PS_LEAGUE_WEALTH_FOR_SERVER stWealth);  // 对齐 IDA: 按值传递

    // 记录
    void ReqLeagueRecordUpdate(ST_LEAGUE_RECORD stRecord);  // 对齐 IDA: 按值传递

    // 成员经验
    void ReqLeagueMemberExpInit(std::int32_t nLeagueID, std::uint32_t dwUCID);  // 对齐 IDA 0x140080430: HK
    void ReqLeagueMemberInitExp(std::int32_t nLeagueID, std::uint32_t dwUCID);

    // 同步
    bool SyncLeagueInfo(PS_SYNC_LEAGUE_INFO stSync);  // 对齐 IDA: 按值传递

    // 仓库
    void ReqLeagueInevntoryInfo(std::uint32_t dwActorID, PS_REQ_LEAGUE_INVEN_INFO stReq);  // 对齐 IDA: 按值传递
    void ResLeagueInventoryInfo(std::int32_t nLeagueID, std::uint32_t dwReqUCID, PS_RES_STORAGE_INFO stStorage, PS_ITEM_BROACH_LIST stBroach, PS_ITEM_SOCKET_LIST stSocket, PS_ITEM_PACKAGE_LIST stPackage);
    void ReqLeagueInventoryMove(std::uint32_t dwActorID, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove);  // 对齐 IDA: 按值传递
    void ResLeagueInventoryMove(std::uint32_t dwReqUCID, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove);

    // 聊天
    void SendLeagueMessage(PS_CHAT_LEAGUE& stChat, PS_CHAT_ITEM_LINK_FOR_SERVER stItemLink);  // 对齐 IDA 0x14007A710: 非const引用 + 按值传递

    // 搜索
    void ReqLeagueSearch(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID);
    void ResLeagueSearch(CServer* pServer, PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList, std::uint32_t dwUCID);

    // 列表
    void ReqLeagueList(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID);
    void ResLeagueList(CServer* pServer, PS_LEAGUE_SUMMARY_LIST psLeagueList, std::uint32_t dwUCID);

    // 更新回调
    void OnUpdate();
    void InitLeaguExp(std::int64_t biTodayInit);
    void UpdateLeagueInfo(std::int32_t nLeagueID, ST_LEAGUE_INFO_UPDATE& stUpdate);
    void UpdateMemberLevel(std::shared_ptr<CUserObject> pUser, std::uint8_t byLevel);
    void UpdateMemberAwaken(std::shared_ptr<CUserObject> pUser, std::uint8_t byAwaken);
    void UpdateMemberProfilePhoto(std::shared_ptr<CUserObject> pUser, std::uint32_t dwProfilePhotoID);
    void UpdateMemberMapInfo(std::uint32_t dwUCID, std::int16_t wMapID, std::uint8_t byChannel);  // 对齐 IDA 0x14007C7B0: KGE (无bool参数)
    void SendMemberUpdate(ST_LEAGUE_MEMBER_EX stMember);
    void SendInfoToGameServer();
    void UpdateLeagueMemberInfo();

    // 发送联赛信息
    // 对齐 IDA 0x140076E70: K=uint32 + 4个AEAU引用 + E=uint8 + U按值ST_LEAGUE_RECORD_LIST + U按值ST_LEAGUE_INFO_FOR_GAME
    void SendLeagueInfo(std::uint32_t dwActorID, ST_LEAGUE_INFO& stLeagueInfo, ST_LEAGUE_MEMBER_LIST& stMemberList,
                        ST_LEAGUE_APPLICANT_LIST& stApplicantList, ST_LEAGUE_BOARD_LIST& stBoardList,
                        std::uint8_t byState, ST_LEAGUE_RECORD_LIST stRecordList, ST_LEAGUE_INFO_FOR_GAME stInfoForGame);

    // 加载回调
    void LoadLeagueInfo(PS_DB_LEAGUE_LOAD stLoad, ST_LEAGUE_INFO stInfo, struct ST_LEAGUE_MEMBER_LIST stMembers, struct ST_LEAGUE_BOARD_LIST stBoards, struct ST_LEAGUE_APPLICANT_LIST stApplicants, struct ST_LEAGUE_RECORD_LIST stRecords);  // 对齐 IDA: 全部按值传递
    void ResLoadLeagueMember(bool bSuccess, struct ST_LEAGUE_MEMBER_LIST& stMembers);
    void ResLoadLeagueApplicant(bool bSuccess, struct ST_LEAGUE_APPLICANT_LIST& stApplicants);
    void ResLoadLeagueBoard(bool bSuccess, struct ST_LEAGUE_BOARD_LIST& stBoards);
    void ResLoadLeagueRecord(bool bSuccess, struct ST_LEAGUE_RECORD_LIST& stRecords);

    // 名称变更回调
    void ChangeLeagueMemberName(std::int32_t nLeagueID, PS_CHANGE_NAME stChange);  // 对齐 IDA: 按值传递
    void ChangeLeagueApplicant(const struct PS_SERVER_CHANGE_CHARACTER_NAME& stChange);

    // GMT
    void SendGMTLeagueInfo(PS_GMT_LEAGUE_UPDATE_LIST stList);  // 对齐 IDA: 按值传递
    void UpdateGMTLeagueInfo(struct ST_LEAGUE_LIST stList, struct ST_LEAGUE_MEMBER_LIST stMembers);

    // 检查
    bool CheckLeagueInfo(std::int32_t nLeagueID);

    // 错误消息发送
    void SendLeagueErrorMsg(CServer* pServer, ST_REQ_LEAGUE_INVITE& stInvite, std::int32_t nErrorCode);
    void SendLeagueErrorMsg(std::uint32_t dwActorID, std::int32_t nErrorCode);

    // 获取器
    void GetApplicantList(std::uint32_t dwUCID, PS_LEAGUE_SUMMARY_LIST stList, ST_LEAGUE_APPLICANT_CHECK_LIST& stCheckList);  // 对齐 IDA: stList按值传递

private:
    bool m_bLeague = false;
    bool m_bApplicant = false;
    bool m_bMember = false;
    bool m_bBoard = false;
    bool m_bRecord = false;
    bool m_bLoadLeague = false;
    // Note: ATL::CTime omitted - using simpler types where needed
    std::int64_t m_tUpdate = 0;
    std::int64_t m_tInitDate = 0;
    std::int32_t m_nLeagueListIndex = 0;
    std::map<std::uint32_t, ST_LEAGUE_INVITE_INFO> m_mpLeagueInvite;
    std::map<std::int32_t, std::shared_ptr<CLeague>> m_mpLeagueList;
    std::vector<std::shared_ptr<CLeague>> m_vecLeagueList;
};

// ============================================================================
// 联赛信息输出序列化器
// ============================================================================

// 对齐 IDA 0x1400E5370: ST_LEAGUE_INFO 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nLeagueRank;
    packet.XParse << value.byGroupType;
    packet.XParse << value.byRating;
    packet.XParse << value.shMemberCount;
    packet.XParse << value.biExp;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.biMoney;
    packet.XParse << value.nCreateDate;
    packet.XParse << value.biNoticeDate;
    packet.XParse << value.dwMasterUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szMasterName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szSubMasterName);
    for (int i = 0; i < 9; ++i) packet.XParse << value.nAuth[i];
    for (int i = 0; i < 9; ++i) packet.XParse << value.nLimitGoldOut[i];
    packet.XParse << value.dwLeagueCard;
    packet.XParse << GreenDamTan_BoundedWideString(value.szNotice);
    packet.XParse << GreenDamTan_BoundedWideString(value.szPosition_1);
    packet.XParse << GreenDamTan_BoundedWideString(value.szPosition_2);
    packet.XParse << GreenDamTan_BoundedWideString(value.szPosition_3);
    packet.XParse << value.bOpen;
    packet.XParse << GreenDamTan_BoundedWideString(value.szRecruitNotice);
    packet.XParse << value.biRecruitNoticeDate;
    packet.XParse << value.bySkillPoint;
    for (int j = 0; j < 8; ++j) packet.XParse << value.bySkill[j];
    packet.XParse << value.nLimitExp;
    packet.XParse << value.biInitDate;
    return packet;
}

// ST_LEAGUE_MEMBER 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.byPosition;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.biLeagueExp;
    packet.XParse << value.biJoinDate;
    packet.XParse << value.biApplicationDate;
    return packet;
}

// ST_LEAGUE_MEMBER_EX 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_EX& value) {
    packet << value.stMember;
    packet.XParse << value.bLogin;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.shLevel;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad2), sizeof(value._pad2));
    packet.XParse << value.biBoardLimitTime;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad3), sizeof(value._pad3));
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad4), sizeof(value._pad4));
    packet.XParse << value.biPlayDate;
    return packet;
}

// ST_LEAGUE_BOARD 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_BOARD& value) {
    packet.XParse << value.nSerial;
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szCharName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    packet.XParse << value.biEnrollDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_APPLICANT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_APPLICANT& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.shLevel;
    packet.XParse << value.biApplicantDate;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_RECORD 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_RECORD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.byFlag;
    packet.XParse << value.biRegisterDate;
    packet.XParse << GreenDamTan_BoundedWideString(value.szValue1);
    packet.XParse << GreenDamTan_BoundedWideString(value.szValue2);
    packet.XParse << value.nValue3;
    packet.XParse << value.nValue4;
    return packet;
}

// ST_LEAGUE_INFO_FOR_GAME 输出序列化 - 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_INFO_FOR_GAME& value) {
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.byLeagueLevel;
    packet.XParse << value.byPosition;
    for (int i = 0; i < 8; ++i) {
        packet.XParse << value.bySkillInfo[i];
    }
    for (int i = 0; i < 9; ++i) {
        packet.XParse << value.nAuth[i];
    }
    return packet;
}

// 列表结构输出序列化（带vector）
inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_MEMBER_EX>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_BOARD>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_APPLICANT>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_RECORD>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

// 列表结构输出序列化（包含nCount和vector的结构体）
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_LIST& value) {
    packet << value.vecInfo;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_BOARD_LIST& value) {
    packet << value.vecInfo;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_APPLICANT_LIST& value) {
    packet << value.vecInfo;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_RECORD_LIST& value) {
    packet << value.vecInfo;
    return packet;
}

// ST_LEAGUE_MEMBER_UPDATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_UPDATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwActorID;
    packet.XParse << value.bLogin;
    packet.XParse << value.byLevel;
    packet.XParse << value.sWorld;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.biPlayDate;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.byChannel;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    return packet;
}

// ST_LEAGUE_INFO_EX 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_INFO_EX& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.nMemberCount;
    return packet;
}

// ST_REQ_LEAGUE_INVITE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_INVITE& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szTargetName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szReqName);
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwTargetActorID;
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_INVITE_ACCEPT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_INVITE_ACCEPT& value) {
    packet.XParse << value.dwReqUCID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szTargetName);
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_INVITE_REJECT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_INVITE_REJECT& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.szTargetName);
    packet.XParse << value.dwReqUCID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_NOTICE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_NOTICE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szNotice);
    packet.XParse << value.biEnrollDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_AUTH_CHANGE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_AUTH_CHANGE& value) {
    for (int i = 0; i < 9; ++i) packet.XParse << value.nAuth[i];
    for (int i = 0; i < 9; ++i) packet.XParse << value.nLimitGoldOut[i];
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_OPEN 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_OPEN& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.bOpen;
    return packet;
}

// ST_LEAGUE_RECRUIT_NOTICE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_RECRUIT_NOTICE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szNotice);
    packet.XParse << value.biRegDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_APPLICANT_ACCEPT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_APPLICANT_ACCEPT& value) {
    packet.XParse << value.dwReqActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szReqName);
    packet.XParse << value.nLeagueID;
    packet.XParse << value.biJoinDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_APPLICANT_REJECT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_APPLICANT_REJECT& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_MEMBER_POSITION 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_POSITION& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.byPosition;
    packet.XParse << value.byState;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_POSITION_NAME_CHANGE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_POSITION_NAME_CHANGE& value) {
    packet.XParse << value.nPosition;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.nResult;
    packet.XParse << GreenDamTan_BoundedWideString(value.szPrevPositionName);
    return packet;
}

// PS_REQ_LEAGUE_DELEGATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_LEAGUE_DELEGATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwNpcID;
    packet.XParse << value.dwDelegatedUCID;
    return packet;
}

// PS_RES_LEAGUE_DELEGATE 输出序列化（对齐 PSServer.h 当前定义）
inline XPacket& operator<<(XPacket& packet, const PS_RES_LEAGUE_DELEGATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szDelegatedName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szDelegateName);
    packet.XParse << value.nResult;
    return packet;
}

// PS_CHAT_LEAGUE 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_CHAT_LEAGUE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwLeagueID;
    packet.XParse << value.dwMemberID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    return packet;
}

// ST_REQ_LEAGUE_SEARCH 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_SEARCH& value) {
    packet.XParse << value.nState;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMasterName);
    return packet;
}

// PS_LEAGUE_INFO_SUMMARY 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_INFO_SUMMARY& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.nLeagueRank;
    packet.XParse << value.nMemberCount;
    packet.XParse << value.nApplicantCount;
    packet.XParse << value.bOpen;
    return packet;
}

// PS_LEAGUE_SUMMARY_LIST 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_SUMMARY_LIST& value) {
    packet.XParse << static_cast<std::int32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ST_LEAGUE_APPLICANT_CHECK_LIST 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_APPLICANT_CHECK_LIST& value) {
    packet.XParse << static_cast<std::int32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet.XParse << item;
    }
    return packet;
}

// PS_CHAT_ITEM_LINK_FOR_SERVER 输出序列化已在 PSServer.h 中定义
