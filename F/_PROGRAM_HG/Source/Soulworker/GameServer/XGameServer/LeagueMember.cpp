// LeagueMember.cpp
// CLeagueMember - GameServer 侧 League 成员管理实现
// 对齐 IDA GameServer.exe；原始归属 XGameServer/LeagueMember.obj
// (PDB cvdump modules 0042)
//
// 说明: 本文件从 XGameServer/GocLeague.cpp 历史遗留聚合中迁移
// CLeagueMember 已还原实现，并补充本轮从 IDA 反编译的
// CompareSyncCount (0x14028CC30) 与 SendSyncLeagueInfo (0x14028CCE0)。
// 其余广播/操作方法仍为带详细 TODO 的 stub，待逐个反编译还原。

#include "LeagueMember.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/GameSockets.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#include <cstring>

// ============================================================================
// CLeagueMember Implementation
// ============================================================================

// IDA: ??0CLeagueMember@@QEAA@XZ @ 0x14028A5C0
CLeagueMember::CLeagueMember()
{
    Clear();
}

// IDA: ??1CLeagueMember@@QEAA@XZ
// 状态: STUB - 原始析构链未反编译
// TODO: 需人工审查 - 确认原始析构是否清理 m_mapUser 内指针所有权
CLeagueMember::~CLeagueMember()
{
}

// IDA: ?Clear@CLeagueMember@@QEAAXXZ @ 0x14028A600
// 已精确还原 - 清空成员表与同步计数
void CLeagueMember::Clear()
{
    m_mapUser.clear();
    m_nSyncCount = 0;
    m_nInventorySyncCount = 0;
}

// IDA: ?AddLeagueMember@CLeagueMember@@QEAAXPEAVCUser@@@Z @ 0x14028A630
// 状态: 部分还原 - 原始实现读取 pUser->GetActorID 后按 dwActorID 插入
void CLeagueMember::AddLeagueMember(CUser* pUser)
{
    if (!pUser) {
        return;
    }

    UXActorID actorID = pUser->GetActorID();
    m_mapUser[actorID.dwActorID] = pUser;
}

// ============================================================================
// DeleteLeagueMember / WithDrawLeagueMember / JoinLeagueUser
// ============================================================================

// IDA: ?DeleteLeagueMember@CLeagueMember@@QEAAXK@Z @ 0x14028B880
// 已精确还原 - 从成员表移除指定 UCID（无广播）
void CLeagueMember::DeleteLeagueMember(unsigned int dwActorID)
{
    auto it = m_mapUser.find(dwActorID);
    if (it != m_mapUser.end()) {
        m_mapUser.erase(it);
    }
}

// IDA: ?WithDrawLeagueMember@CLeagueMember@@QEAAXKAUST_LEAGUE_INFO_UPDATE@@@Z @ 0x14028A6A0
// 已精确还原 - 遍历成员（跳过本人 dwUCID 与 sync-flagged），UpdateLeagueSyncCount
// 后发送 (0x22,9)+dwUCID 退出通知 + (0x22,0x43)+ST_LEAGUE_INFO_UPDATE 信息更新；
// 之后从 m_mapUser 中移除 dwUCID 条目。
void CLeagueMember::WithDrawLeagueMember(unsigned int dwUCID, ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }
        if (dwUCID != pMember->GetActorID().dwActorID &&
            !pMember->GetLeagueSyncFlag()) {
            pMember->UpdateLeagueSyncCount(m_nSyncCount);

            XSendPacket xSendPacket(0x22, 9);
            xSendPacket.XParse << dwUCID;
            CGocNetwork::Send(pMember, xSendPacket);

            XSendPacket xSendPacket2(0x22, 0x43);
            xSendPacket2 << stUpdateInfo;
            CGocNetwork::Send(pMember, xSendPacket2);
        }
    }

    auto it = m_mapUser.find(dwUCID);
    if (it != m_mapUser.end()) {
        m_mapUser.erase(it);
    }
}

// IDA: ?JoinLeagueUser@CLeagueMember@@QEAAXUST_LEAGUE_MEMBER_EX@@UST_LEAGUE_INFO_UPDATE@@E@Z @ 0x14028A930
// 已精确还原 - 向未同步成员广播入会通知 (0x22,0x17) + 信息更新 (0x22,0x43)，
// byApplyState==1 时附加申请者通知 (0x22,0x26)
void CLeagueMember::JoinLeagueUser(ST_LEAGUE_MEMBER_EX stMemberEx,
                                   ST_LEAGUE_INFO_UPDATE stInfoUpdate,
                                   unsigned char byApplyState)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }
        if (pMember->GetLeagueSyncFlag()) {
            continue;
        }
        if (stMemberEx.dwUCID == pMember->GetLeagueID()) {
            continue;
        }
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x17);
        xSendPacket << stMemberEx;
        CGocNetwork::Send(pMember, xSendPacket);

        XSendPacket packet(0x22, 0x43);
        packet << stInfoUpdate;
        CGocNetwork::Send(pMember, packet);

        if (byApplyState == 1) {
            XSendPacket v20(0x22, 0x26);
            v20.XParse << stMemberEx.dwUCID;
            CGocNetwork::Send(pMember, v20);
        }
    }
}

// ============================================================================
// Broadcasting functions - stub 群
// ============================================================================

// IDA: ?SendLeagueInfo@CLeagueMember@@QEAAXUST_LEAGUE_INFO@@@Z @ 0x14028B210
// 已精确还原 - 按汇编形态: 仅遍历 m_mapUser 取成员指针后无副作用
// (0x66 字节小函数，循环体内对 pUser 无任何调用，疑似原始内联发送调用
// 被链接器/优化裁剪，或调用方通过其他途径完成发送)。
// TODO: 需人工审查 - 若后续找到该函数的调用方实际收包证据，按收包命令号
// 补回成员遍历发送逻辑。
void CLeagueMember::SendLeagueInfo(ST_LEAGUE_INFO stInfo)
{
    (void)stInfo;
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pUser = it->second;
        (void)pUser;
    }
}

// IDA: ?SendLeagueBoard@CLeagueMember@@QEAAXUST_LEAGUE_BOARD@@K@Z @ 0x14028B0D0
// 已精确还原 - 遍历成员（跳过 sync-flagged），发送 (0x22,8)+ST_LEAGUE_BOARD。
// dwWriterUCID 参数在原始二进制中未参与循环内判断（仅按 PDB 签名保留）。
void CLeagueMember::SendLeagueBoard(ST_LEAGUE_BOARD stBoard, unsigned int dwWriterUCID)
{
    (void)dwWriterUCID;
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 8);
        xSendPacket << stBoard;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?SendLeagueNotice@CLeagueMember@@QEAAXUST_LEAGUE_NOTICE@@K@Z @ 0x14028B280
// 已精确还原 - 遍历成员，跳过 dwWriterUCID 本人（GetActorID 比对）与
// 未同步标记成员，向其余成员发送 (0x22,0x29) + ST_LEAGUE_NOTICE
void CLeagueMember::SendLeagueNotice(ST_LEAGUE_NOTICE stNotice, unsigned int dwWriterUCID)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }
        if (pMember->GetActorID().dwActorID == dwWriterUCID) {
            continue;
        }
        if (pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 0x29);
        xSendPacket << stNotice;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?SendLeagueApply@CLeagueMember@@QEAAXUST_LEAGUE_APPLICANT@@@Z @ 0x14028AC50
// 已精确还原 - 遍历成员，跳过申请者本人（stApplicant.dwActorID 比对）与
// sync-flagged 成员，发送 (0x22,0x24)+ST_LEAGUE_APPLICANT。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::SendLeagueApply(ST_LEAGUE_APPLICANT stApplicant)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }
        if (stApplicant.dwActorID != pMember->GetActorID().dwActorID &&
            !pMember->GetLeagueSyncFlag()) {
            XSendPacket xSendPacket(0x22, 0x24);
            xSendPacket << stApplicant;
            CGocNetwork::Send(pMember, xSendPacket);
        }
    }
}

// IDA: ?SendLeagueMsg@CLeagueMember@@QEAAXUPS_CHAT_LEAGUE@@UPS_CHAT_ITEM_LINK_FOR_SERVER@@@Z @ 0x14028BA30
// 已精确还原 - 遍历成员（跳过 sync-flagged），按原始二进制形态发送聊天包:
// (7,1) + dwActorID + 4 + szMsg + byItemLinkCount + 逐项 PS_CHAT_ITEM_LINK
void CLeagueMember::SendLeagueMsg(PS_CHAT_LEAGUE psChat, PS_CHAT_ITEM_LINK_FOR_SERVER psItemLink)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(7, 1);
        xSendPacket.XParse << psChat.dwActorID;
        xSendPacket.XParse << 4;
        xSendPacket.XParse << GreenDamTan_BoundedWideString(psChat.szMsg);
        xSendPacket.XParse << psItemLink.byItemLinkCount;
        for (int i = 0; i < psItemLink.byItemLinkCount; ++i) {
            PS_CHAT_ITEM_LINK psInfo = psItemLink.psItemLinkInfo[i];
            xSendPacket << psInfo;
        }
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?SendLeagueRecruitNotice@CLeagueMember@@QEAAXUST_LEAGUE_RECRUIT_NOTICE@@_J@Z @ 0x14028BDB0
// 已精确还原 - 遍历成员（跳过 sync-flagged），发送 (0x22,0x47)+
// ST_LEAGUE_RECRUIT_NOTICE + biRemainTime(int64)。
// (PDB 签名第 2 参为 __int64 biRemainTime)
void CLeagueMember::SendLeagueRecruitNotice(ST_LEAGUE_RECRUIT_NOTICE stNotice, __int64 nDate)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 0x47);
        xSendPacket << stNotice;
        xSendPacket.XParse << nDate;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// ============================================================================
// SendSyncLeagueInfo (0x14028CCE0) - 已精确还原
// 构造 PS_SYNC_LEAGUE_INFO{nLeagueID, dwUCID} 经 (0xF6,0x58) 经由
// CCommunitySocket::SendCmd(0x22,7) 请求 RelayServer 重新同步。
// ============================================================================
void CLeagueMember::SendSyncLeagueInfo(CUser* pUser)
{
    if (!pUser) {
        return;
    }

    PS_SYNC_LEAGUE_INFO stSyncLeagueInfo;
    stSyncLeagueInfo.nLeagueID = pUser->GetLeagueID();
    stSyncLeagueInfo.dwUCID = pUser->GetActorID().dwActorID;

    XSendPacket xSendPacket(0xF6, 0x58);
    xSendPacket << stSyncLeagueInfo;
    XGameServer::Instance()->GetCommunitySocket().SendCmd(
        &xSendPacket, pUser, 0x22, 7);
}

// ============================================================================
// Guild operations
// ============================================================================

// IDA: ?LoginLeagueMember@CLeagueMember@@QEAAXUST_LEAGUE_MEMBER_UPDATE@@@Z @ 0x14028ADE0
// 已精确还原 - 遍历成员，跳过登录者本人（UXActorID 整体比对）与 sync-flagged
// 成员，发送 (0x22,0x22)+ST_LEAGUE_MEMBER_UPDATE。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::LoginLeagueMember(ST_LEAGUE_MEMBER_UPDATE stUpdate)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }
        UXActorID otherID;
        otherID.dwActorID = stUpdate.dwActorID;
        if (!(pMember->GetActorID() == otherID) &&
            !pMember->GetLeagueSyncFlag()) {
            XSendPacket xSendPacket(0x22, 0x22);
            xSendPacket << stUpdate;
            CGocNetwork::Send(pMember, xSendPacket);
        }
    }
}

// IDA: ?KickoutLeagueMember@CLeagueMember@@QEAAXHKUST_LEAGUE_INFO_UPDATE@@@Z @ 0x14028BF00
// 已精确还原 - 向未同步成员广播踢出通知 (0x22,0x41) + 信息更新 (0x22,0x43)，
// 之后清理被踢用户的 League 信息并从表中移除
void CLeagueMember::KickoutLeagueMember(int dwReqUCID, unsigned int dwKickoutUCID,
                                        ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }
        if (dwReqUCID == pMember->GetLeagueID() ||
            dwKickoutUCID == pMember->GetLeagueID()) {
            continue;
        }
        if (pMember->GetLeagueSyncFlag()) {
            continue;
        }
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x41);
        xSendPacket.XParse << static_cast<int>(dwReqUCID);
        xSendPacket.XParse << static_cast<std::uint32_t>(dwKickoutUCID);
        CGocNetwork::Send(pMember, xSendPacket);

        XSendPacket xSendPacket2(0x22, 0x43);
        xSendPacket2 << stUpdateInfo;
        CGocNetwork::Send(pMember, xSendPacket2);
    }

    auto it = m_mapUser.find(dwKickoutUCID);
    if (it != m_mapUser.end()) {
        CUser* pKickoutUser = it->second;
        if (pKickoutUser) {
            pKickoutUser->ClearLeagueInfo();
        }
        m_mapUser.erase(it);
    }
}

// IDA: ?LeagueApplicantDelete@CLeagueMember@@QEAAXK@Z @ 0x14028AF80
// 已精确还原 - 遍历成员（跳过 sync-flagged），UpdateLeagueSyncCount 后
// 发送 (0x22,0x26)+dwUCID 申请人删除通知。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::LeagueApplicantDelete(unsigned int dwUCID)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x26);
        xSendPacket.XParse << dwUCID;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?ChangeLeagueAuth@CLeagueMember@@QEAAXUST_LEAGUE_AUTH_CHANGE@@@Z @ 0x14028B410
// 已精确还原 - 遍历成员（跳过 sync-flagged），SetLeagueAuth(局部副本) +
// UpdateLeagueSyncCount 后发送 (0x22,0x32)+ST_LEAGUE_AUTH_CHANGE
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::ChangeLeagueAuth(ST_LEAGUE_AUTH_CHANGE stAuthChange)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        ST_LEAGUE_AUTH_CHANGE stCopy = stAuthChange;
        pMember->SetLeagueAuth(stCopy);
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x32);
        xSendPacket << stAuthChange;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?ChangeLeaguePositionName@CLeagueMember@@QEAAXUST_LEAGUE_POSITION_NAME_CHANGE@@@Z @ 0x14028B590
// 已精确还原 - 遍历成员（跳过 sync-flagged），发送 (0x22,0x33)+ST_LEAGUE_POSITION_NAME_CHANGE
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::ChangeLeaguePositionName(ST_LEAGUE_POSITION_NAME_CHANGE stPosNameChange)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 0x33);
        xSendPacket << stPosNameChange;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?ChangeLeagueMemberPosition@CLeagueMember@@QEAAXUST_LEAGUE_MEMBER_POSITION@@K@Z @ 0x14028B6D0
// 已精确还原 - 遍历成员（跳过 sync-flagged），成员 ActorID == stPosition.dwActorID
// 时 SetLeaguePosition(byPosition)，之后 UpdateLeagueSyncCount 并发送
// (0x22,0x39)+ST_LEAGUE_MEMBER_POSITION
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 2 参:
//  ST_LEAGUE_MEMBER_POSITION + unsigned int dwActorID)
void CLeagueMember::ChangeLeagueMemberPosition(ST_LEAGUE_MEMBER_POSITION stMemberPos, unsigned int dwUCID)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        if (pMember->GetActorID().dwActorID == stMemberPos.dwActorID) {
            pMember->SetLeaguePosition(stMemberPos.byPosition);
        }
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x39);
        xSendPacket << stMemberPos;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?UpdateLeagueMember@CLeagueMember@@QEAAXUST_LEAGUE_MEMBER_UPDATE@@@Z @ 0x14028B8F0
// 已精确还原 - 遍历成员，跳过未同步标记成员，向其余成员广播
// (0x22,0x22) + ST_LEAGUE_MEMBER_UPDATE
void CLeagueMember::UpdateLeagueMember(ST_LEAGUE_MEMBER_UPDATE stUpdate)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 0x22);
        xSendPacket << stUpdate;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?UpdateApplicantList@CLeagueMember@@QEAAXK@Z @ 0x14028BC70
// 已精确还原 - 遍历成员（跳过 sync-flagged），发送 (0x22,0x26)+dwUCID
// 通知申请人列表刷新
void CLeagueMember::UpdateApplicantList(unsigned int dwUCID)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 0x26);
        xSendPacket.XParse << dwUCID;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// ============================================================================
// Guild progression
// ============================================================================

// IDA: ?Record@CLeagueMember@@QEAAXUST_LEAGUE_RECORD@@@Z @ 0x14028C210
// 已精确还原 - 遍历成员（跳过 sync-flagged），发送 (0x22,0x48)+ST_LEAGUE_RECORD。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::Record(ST_LEAGUE_RECORD stRecord)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        XSendPacket xSendPacket(0x22, 0x48);
        xSendPacket << stRecord;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?Wealth@CLeagueMember@@QEAAXUST_LEAGUE_INFO_UPDATE@@@Z @ 0x14028C590
// 已精确还原 - 向未同步成员广播财富更新 (0x22,0x55)
void CLeagueMember::Wealth(ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x55);
        xSendPacket << stUpdateInfo;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?Levelup@CLeagueMember@@QEAAXHECUPS_AUTO_SKILL@@@Z @ 0x14028C6D0
// 已精确还原 - 更新成员 League 等级/技能并广播升级包 (0x22,0x52)
void CLeagueMember::Levelup(int nLeagueID, unsigned char byLevel,
                            char bySkillPoint, PS_AUTO_SKILL psAutoSkillInfo)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }
        pMember->SetLeagueLevel(byLevel);

        for (int i = 0; i < 8; ++i) {
            if (psAutoSkillInfo.bySkillInfo[i]) {
                pMember->SetLeagueSkill(static_cast<std::uint8_t>(i),
                                        psAutoSkillInfo.bySkillInfo[i]);
            }
        }
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x52);
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << byLevel;
        xSendPacket.XParse << bySkillPoint;
        xSendPacket << psAutoSkillInfo;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?SkillLearn@CLeagueMember@@QEAAXUPS_RES_LEAGUE_SKILL@@@Z @ 0x14028C8F0
// 已精确还原 - 遍历成员（跳过 sync-flagged），SetLeagueSkill(bySkillGroupID,
// bySkillLevel) + UpdateLeagueSyncCount 后发送 (0x22,0x53)+PS_RES_LEAGUE_SKILL。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::SkillLearn(PS_RES_LEAGUE_SKILL psSkill)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        pMember->SetLeagueSkill(psSkill.bySkillGroupID, psSkill.bySkillLevel);
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x53);
        xSendPacket << psSkill;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?Delegate@CLeagueMember@@QEAAXUPS_RES_LEAGUE_DELEGATE@@K@Z @ 0x14028CA60
// 已精确还原 - 遍历成员（跳过 sync-flagged），SetLeagueMaster(dwDelegatedUCID) +
// UpdateLeagueSyncCount 后发送 (0x22,0xA)+PS_RES_LEAGUE_DELEGATE(局部副本)。
// (PDB 签名为 2 参; 台账旧注释地址 0x14028BA60 有误，实际为 0x14028CA60)
void CLeagueMember::Delegate(PS_RES_LEAGUE_DELEGATE psDelegate, unsigned int dwUCID)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        pMember->SetLeagueMaster(dwUCID);
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0xA);
        PS_RES_LEAGUE_DELEGATE st = psDelegate;
        xSendPacket << st;
        CGocNetwork::Send(pMember, xSendPacket);
    }
}

// IDA: ?CardChange@CLeagueMember@@QEAAXUPS_REQ_LEAGUE_CARD@@@Z @ 0x14028C350
// 已精确还原 - 遍历成员（跳过 sync-flagged），SetLeagueCard + UpdateLeagueSyncCount
// 后发送 (0x22,0x31)+PS_REQ_LEAGUE_CARD，再 GetLeagueInfo 填 ST_LEAGUE_INFO_EX、
// dwUCID 取成员 ActorID（无掩码），(0x22,0x44) BroadcastNearby 广播。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::CardChange(PS_REQ_LEAGUE_CARD psCard)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        pMember->SetLeagueCard(psCard.dwLeagueCard);
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x31);
        xSendPacket << psCard;
        CGocNetwork::Send(pMember, xSendPacket);

        ST_LEAGUE_INFO_EX stInfoEx;
        pMember->GetLeagueInfo(stInfoEx);
        stInfoEx.dwUCID = pMember->GetActorID().dwActorID;

        XSendPacket packet(0x22, 0x44);
        packet << stInfoEx;
        CGocNetwork::BroadcastNearby(pMember, nullptr, packet);
    }
}

// IDA: ?InventoryMove@CLeagueMember@@QEAAXKUPS_RES_ITEM_MOVE_LEAGUE_INVEN@@@Z @ 0x14028CE30
// 已精确还原 - 遍历全部成员（含 sync-flagged），UpdateLeagueInventorySyncCount
// 后，向 ActorID != dwReqUCID 的成员发送 (0x22,0x57)+PS_RES_ITEM_MOVE_LEAGUE_INVEN。
// (PDB 签名为 2 参)
void CLeagueMember::InventoryMove(unsigned int dwUCID, PS_RES_ITEM_MOVE_LEAGUE_INVEN psMove)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember) {
            continue;
        }

        pMember->UpdateLeagueInventorySyncCount(m_nInventorySyncCount);
        if (pMember->GetActorID().dwActorID != dwUCID) {
            XSendPacket xSendPacket(0x22, 0x57);
            xSendPacket << psMove;
            CGocNetwork::Send(pMember, xSendPacket);
        }
    }
}

// IDA: ?ChangeName@CLeagueMember@@QEAAXUPS_RES_LEAGUE_NAME_CHANGE@@@Z @ 0x14028D080
// 已精确还原 - 遍历成员（跳过 sync-flagged），SetLeagueName + UpdateLeagueSyncCount
// 后发送 (0x22,0x30)+PS_RES_LEAGUE_NAME_CHANGE，再 GetLeagueInfo 填 ST_LEAGUE_INFO_EX、
// dwUCID 取成员 ActorID（无掩码），(0x22,0x44) BroadcastNearby 广播。
// (IDA 反编译显示的 float a3 为栈布局污染，真实签名为 1 参)
void CLeagueMember::ChangeName(PS_RES_LEAGUE_NAME_CHANGE psNameChange)
{
    for (auto it = m_mapUser.begin(); it != m_mapUser.end(); ++it) {
        CUser* pMember = it->second;
        if (!pMember || pMember->GetLeagueSyncFlag()) {
            continue;
        }

        pMember->SetLeagueName(psNameChange.szLeagueName);
        pMember->UpdateLeagueSyncCount(m_nSyncCount);

        XSendPacket xSendPacket(0x22, 0x30);
        xSendPacket << psNameChange;
        CGocNetwork::Send(pMember, xSendPacket);

        ST_LEAGUE_INFO_EX stInfoEx;
        pMember->GetLeagueInfo(stInfoEx);
        stInfoEx.dwUCID = pMember->GetActorID().dwActorID;

        XSendPacket packet(0x22, 0x44);
        packet << stInfoEx;
        CGocNetwork::BroadcastNearby(pMember, nullptr, packet);
    }
}

// ============================================================================
// Sync operations
// ============================================================================

// IDA: ?UpdateSyncCount@CLeagueMember@@QEAAXH@Z @ 0x14028BDF0
// 已精确还原 - 设置同步计数
void CLeagueMember::UpdateSyncCount(int nCount)
{
    m_nSyncCount = nCount;
}

// IDA: ?UpdateInventorySyncCount@CLeagueMember@@QEAAXH@Z @ 0x14028BE10
// 已精确还原 - 设置物品栏同步计数
void CLeagueMember::UpdateInventorySyncCount(int nCount)
{
    m_nInventorySyncCount = nCount;
}

// ============================================================================
// CompareSyncCount (0x14028CC30) - 已精确还原
// 按 dwUCID 查找成员；未同步(GetLeagueSyncFlag==false) 且成员计数与
// m_nSyncCount 不一致时 UpdateLeagueSyncFlag(true) + SendSyncLeagueInfo。
// ============================================================================
void CLeagueMember::CompareSyncCount(unsigned int dwUCID)
{
    auto iter = m_mapUser.find(dwUCID);
    if (iter == m_mapUser.end()) {
        return;
    }

    CUser* pMember = iter->second;
    if (!pMember) {
        return;
    }
    if (pMember->GetLeagueSyncFlag()) {
        return;
    }
    if (m_nSyncCount != pMember->GetLeagueSyncCount()) {
        pMember->UpdateLeagueSyncFlag(true);
        SendSyncLeagueInfo(pMember);
    }
}

// IDA: ?CompareInventorySyncCount@CLeagueMember@@QEAA_NK@Z @ 0x14028CFC0
// 已精确还原 - 按 dwUCID 查 m_mapUser；未命中或空返回 false；
// 成员物品栏计数与 m_nInventorySyncCount 一致返回 false；不一致时
// LogError "CompareInventorySyncCount[ UCID:%d, UserSync:%d, LeagueSync:%d ]( 707 )"
// 并返回 true（需要重新同步）。
bool CLeagueMember::CompareInventorySyncCount(unsigned int dwUCID)
{
    auto iter = m_mapUser.find(dwUCID);
    if (iter == m_mapUser.end()) {
        return false;
    }

    CUser* pMember = iter->second;
    if (!pMember) {
        return false;
    }
    int nMemberSyncCount = pMember->GetLeagueInventorySyncCount();
    if (m_nInventorySyncCount == nMemberSyncCount) {
        return false;
    }
    LogHelper::LogError("game.contents",
        "CompareInventorySyncCount[ UCID:%d, UserSync:%d, LeagueSync:%d ]( %d )",
        dwUCID, nMemberSyncCount, m_nInventorySyncCount, 707);
    return true;
}
