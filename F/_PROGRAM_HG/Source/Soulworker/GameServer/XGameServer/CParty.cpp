// CParty.cpp
// CParty and CPartyMember implementation
// 对齐 IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"

#include <algorithm>
#include <cstring>

// 前置声明 - 避免循环依赖
class CUser;
class CMover;
class XSendPacket;
class XArea;
class XMaze;
class XDistrict;
class CDropProcess;
class CGocNetwork;
class CGocRecode;
class CGocFriend;
class CGocQuest;
class CGocAttribute;
class CGocInventory;
class CGocEntity;
class CCommunitySocket;
class CGameControlSocket;
class XGameServer;
class ThreadLocalData;
class XResourceMgr;
class XWorldResMgr;
class LogHelper;
class VBaseObject;

// 外部函数声明
extern XGameServer* g_pGameServer;

// ============================================================================
// CPartyMember Implementation
// ============================================================================

// IDA: ??0CPartyMember@@QEAA@XZ @ 0x1401C9F50
CPartyMember::CPartyMember() {
    std::memset(&m_partyMemberInfo, 0, sizeof(m_partyMemberInfo));
    m_nPartyID = 0;
    m_nPartyMemberState = 0;
    m_pMember = nullptr;
    std::memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));
}

// IDA: ??1CPartyMember@@UEAA@XZ
CPartyMember::~CPartyMember() {
    // 虚析构函数
}

// IDA: ?Init@CPartyMember@@QEAAXAEAUST_PARTY_MEMBER@@@Z
void CPartyMember::Init(ST_PARTY_MEMBER& stPartyMember) {
    m_partyMemberInfo = stPartyMember;
}

// IDA: ?Clear@CPartyMember@@QEAAXXZ
void CPartyMember::Clear() {
    std::memset(&m_partyMemberInfo, 0, sizeof(m_partyMemberInfo));
    m_nPartyID = 0;
    m_nPartyMemberState = 0;
    m_pMember = nullptr;
    std::memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));
}

// IDA: ?Logout@CPartyMember@@QEAAXAEAUST_UPDATE_PARTY_MEMBER@@@Z @ 0x1401CA160
void CPartyMember::Logout(ST_UPDATE_PARTY_MEMBER& psUpdateMember) {
    m_partyMemberInfo.bLogin = 0;
    m_partyMemberInfo.nHP = 0;
    m_partyMemberInfo.nMaxHP = 0;
    psUpdateMember.stPartyMember = m_partyMemberInfo;
    m_pMember = nullptr;
}

// IDA: ?Login@CPartyMember@@QEAAXXZ
void CPartyMember::Login() {
    m_partyMemberInfo.bLogin = 1;
}

// IDA: ?SetLevel@CPartyMember@@QEAAXH@Z @ 0x1401CA200
void CPartyMember::SetLevel(std::uint8_t nLevel) {
    m_partyMemberInfo.byLevel = nLevel;
}

// IDA: ?SetChannel@CPartyMember@@QEAAXH@Z @ 0x1401C53B0
void CPartyMember::SetChannel(int nChannel) {
    m_partyMemberInfo.nChannel = nChannel;
}

// IDA: ?ChangeName@CPartyMember@@QEAAXPEA_W@Z @ 0x1401CA240
void CPartyMember::ChangeName(wchar_t* szName) {
    if (szName) {
        wcscpy_s(m_partyMemberInfo.strName, szName);
    }
}

// IDA: ?SetMember@CPartyMember@@QEAAXPEAVCUser@@@Z @ 0x1401C5430
void CPartyMember::SetMember(CUser* pMember) {
    m_pMember = pMember;
}

// IDA: ?SetProfilePhoto@CPartyMember@@QEAAXK@Z
void CPartyMember::SetProfilePhoto(std::uint32_t dwPhotoID) {
    m_partyMemberInfo.dwProfilePhotoID = dwPhotoID;
}

// IDA: ?SetHP@CPartyMember@@QEAAXH@Z
void CPartyMember::SetHP(int nHP) {
    m_partyMemberInfo.nHP = nHP;
}

// IDA: ?SetMaxHP@CPartyMember@@QEAAXH@Z
void CPartyMember::SetMaxHP(int nMaxHP) {
    m_partyMemberInfo.nMaxHP = nMaxHP;
}

// IDA: ?GetRecode@CPartyMember@@QEAAXPEAH@Z @ 0x1401C53D0
void CPartyMember::GetRecode(int* pMazeRecode) const {
    if (pMazeRecode) {
        std::memcpy(pMazeRecode, m_nMazeRecode, sizeof(m_nMazeRecode));
    }
}

// IDA: ?SetRecode@CPartyMember@@QEAAXPEAH@Z @ 0x1403AC5E0
void CPartyMember::SetRecode(const int* pMazeRecode) {
    if (pMazeRecode) {
        std::memcpy(m_nMazeRecode, pMazeRecode, sizeof(m_nMazeRecode));
    }
}

// IDA: ?ClearRecode@CPartyMember@@QEAAXXZ @ 0x1403B0240
void CPartyMember::ClearRecode() {
    std::memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));
}

// ============================================================================
// CParty Implementation
// ============================================================================

// IDA: ??0CParty@@QEAA@XZ @ 0x1403A4A80
CParty::CParty() {
    m_dwPartyID = 0;
    m_dwMasterID = 0;
    m_uxMazeID = UXMapID{};
    m_bLoad = 0;
    m_nMaxLevel = 0;
    m_nMinLevel = 0;
    m_byPartyType = 0;
    std::memset(&m_stEnterMazeRequst, 0, sizeof(m_stEnterMazeRequst));
    std::memset(&m_stNextMovePos, 0, sizeof(m_stNextMovePos));
}

// IDA: ??1CParty@@UEAA@XZ @ 0x1403A4BB0
CParty::~CParty() {
    // 清理所有成员
    for (auto& pair : m_mapPartyMember) {
        if (pair.second) {
            pair.second->Clear();
            delete pair.second;
            pair.second = nullptr;
        }
    }
    m_mapPartyMember.clear();
    m_vecReadyToMazeMember.clear();
    m_setAgreeToMazeMember.clear();
}

// IDA: ?Create@CParty@@QEAAXKAEAUST_PARTY_MEMBER@@@Z @ 0x1403A4C10
void CParty::Create(std::uint32_t dwPartyID, ST_PARTY_MEMBER& psMasterInfo) {
    m_dwPartyID = dwPartyID;
    m_dwMasterID = psMasterInfo.dwMemberID;
    AddMember(psMasterInfo, nullptr);
}

// IDA: ?Clear@CParty@@QEAAXXZ @ 0x1403A4DB0
void CParty::Clear() {
    // 遍历并删除所有成员
    auto it = m_mapPartyMember.begin();
    while (it != m_mapPartyMember.end()) {
        CPartyMember* pMember = it->second;
        if (pMember) {
            pMember->Clear();
            delete pMember;
        }
        ++it;
    }
    m_mapPartyMember.clear();

    m_dwPartyID = 0;
    m_bLoad = 0;
    m_dwMasterID = 0;
    m_byPartyType = 0;
}

// IDA: ?AddMember@CParty@@QEAA_NAEAUST_PARTY_MEMBER@@PEAVCUser@@@Z @ 0x1403A4EF0
bool CParty::AddMember(ST_PARTY_MEMBER& stPartyMember, CUser* pMember) {
    // 检查队伍是否已满
    if (m_mapPartyMember.size() >= 4) {
        return false;
    }

    // 创建新的CPartyMember
    CPartyMember* pPartyMember = new CPartyMember();
    if (!pPartyMember) {
        return false;
    }

    pPartyMember->Init(stPartyMember);
    pPartyMember->SetMember(pMember);

    // 添加到映射表
    m_mapPartyMember[stPartyMember.dwMemberID] = pPartyMember;

    // 更新等级范围
    int nLevel = stPartyMember.byLevel;
    if (m_nMinLevel == 0) {
        m_nMinLevel = nLevel;
    }
    if (m_nMinLevel > nLevel) {
        m_nMinLevel = nLevel;
    }
    if (m_nMaxLevel < nLevel) {
        m_nMaxLevel = nLevel;
    }

    return true;
}

// IDA: ?GetMember@CParty@@QEAAPEAVCPartyMember@@K@Z @ 0x1403A5050
CPartyMember* CParty::GetMember(std::uint32_t dwActorID) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it != m_mapPartyMember.end()) {
        return it->second;
    }
    return nullptr;
}

// IDA: ?IsMember@CParty@@QEAA_NK@Z @ 0x1401B8640
bool CParty::IsMember(std::uint32_t dwActorID) const {
    auto it = m_mapPartyMember.find(dwActorID);
    return it != m_mapPartyMember.end();
}

// IDA: ?GetUserCount@CParty@@QEAAAEXZ
std::uint8_t CParty::GetUserCount() const {
    return static_cast<std::uint8_t>(m_mapPartyMember.size());
}

// IDA: ?GetPartyInfo@CParty@@QEAAXAEAUPS_PARTY_INFO@@@Z @ 0x1401B9740
void CParty::GetPartyInfo(PS_PARTY_INFO& stPartyInfo) const {
    stPartyInfo.dwPartyID = m_dwPartyID;
    stPartyInfo.dwMaster = m_dwMasterID;
    stPartyInfo.uxMazeID = m_uxMazeID;

    // 遍历所有成员并添加到列表
    for (auto it = m_mapPartyMember.begin(); it != m_mapPartyMember.end(); ++it) {
        CPartyMember* pMember = it->second;
        if (pMember) {
            stPartyInfo.vecPartyMember.push_back(pMember->GetMemberInfo());
        }
    }
}

// IDA: ?GetMazeID@CParty@@QEAA?ATUXMapID@@XZ @ 0x1402F69D0
UXMapID CParty::GetMazeID() const {
    return m_uxMazeID;
}

// IDA: ?UpdateMemberInfo@CParty@@QEAAXAEAUST_UPDATE_PARTY_MEMBER@@@Z @ 0x1403A50C0
void CParty::UpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stPartyMember) {
    auto it = m_mapPartyMember.find(stPartyMember.stPartyMember.dwMemberID);
    if (it != m_mapPartyMember.end()) {
        CPartyMember* pMember = it->second;
        if (pMember) {
            // 更新成员信息
            pMember->Init(stPartyMember.stPartyMember);
            UpdateMemberLevelOrder();
        }
    }
}

// IDA: ?UpdateMemberLevelOrder@CParty@@QEAAXXZ @ 0x1403A5150
void CParty::UpdateMemberLevelOrder() {
    // TODO: 完整实现需要访问CUser和ThreadLocalData
    // 当前简化实现：重置等级范围
    m_nMinLevel = 0;
    m_nMaxLevel = 0;

    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pMember = pair.second;
        if (pMember) {
            int nLevel = pMember->GetMemberInfo().byLevel;
            if (m_nMinLevel == 0 || m_nMinLevel > nLevel) {
                m_nMinLevel = nLevel;
            }
            if (m_nMaxLevel < nLevel) {
                m_nMaxLevel = nLevel;
            }
        }
    }
}

// IDA: ?UpdatePartyBooster@CParty@@QEAAXK@Z @ 0x1403A56D0
void CParty::UpdatePartyBooster(std::uint32_t dwLeaveMemberID) {
    // TODO: 完整实现需要访问ThreadLocalData, XGameServer, XResourceMgr
    // 需要检查成员在同一地图，计算好友数量等
}

// IDA: ?ChangeMonsterLevelStat@CParty@@QEAAX_N@Z @ 0x1403A5B20
void CParty::ChangeMonsterLevelStat(bool bDeleteParty) {
    // TODO: 完整实现需要访问XArea和XMaze
    // 遍历成员找到XMaze并调整怪物等级
}

// IDA: ?SendPartyInfo@CParty@@QEAAXPEAVCUser@@E@Z @ 0x1403A5C60
void CParty::SendPartyInfo(CUser* pMember, std::uint8_t updateType) {
    if (!pMember) {
        return;
    }

    // 构造PS_PARTY_INFO
    PS_PARTY_INFO psPartyInfo{};
    psPartyInfo.dwPartyID = m_dwPartyID;
    psPartyInfo.dwMaster = m_dwMasterID;
    psPartyInfo.byUpdateType = updateType;
    psPartyInfo.byPartyType = m_byPartyType;
    psPartyInfo.uxMazeID = m_uxMazeID;

    // 添加所有成员信息
    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pPartyMember = pair.second;
        if (pPartyMember) {
            psPartyInfo.vecPartyMember.push_back(pPartyMember->GetMemberInfo());
        }
    }

    // TODO: 发送数据包
    // XSendPacket xSendPacket(0x12, 9);
    // xSendPacket << psPartyInfo;
    // CGocNetwork::Send(&pMember->XActor, &xSendPacket);
}

// IDA: ?Send@CParty@@QEAAXAEAVXSendPacket@@K@Z @ 0x1403A5FB0
// IDA反编译: 遍历成员，检查ActorID和MapInsID，发送到同线程区域的成员
void CParty::Send(XSendPacket& xSendPacket, std::uint32_t dwExceptID) {
    // IDA: 遍历所有成员
    for (auto it = m_mapPartyMember.begin(); it != m_mapPartyMember.end(); ++it) {
        CPartyMember* pMember = it->second;
        if (!pMember) {
            continue;
        }

        // IDA: 获取CUser指针
        CUser* pUser = pMember->GetMember();
        if (!pUser) {
            continue;
        }

        // IDA: 检查ActorID是否与dwExceptID匹配
        // v13 = pUser->__vftable;
        // v5 = v13->GetActorID(&pUser->XActor, (UXActorID *)&v11);
        // if (!UXActorID::operator==(v5, (const VBitmask *)&dwExceptIDa))
        UXActorID actorID{};
        // TODO: 需要调用 XActor::GetActorID 获取正确的 ActorID
        // 当前简化实现：使用 dwMemberID 比较
        if (pMember->GetMemberID() == dwExceptID) {
            continue;
        }

        // IDA: 获取MapInsID并检查是否在同一线程区域
        // MapInsID = XActor::GetMapInsID(&pUser->XActor, &v12);
        // Instance = ThreadLocalData::GetInstance();
        // if (ThreadLocalData::FindArea(Instance, (UXMapID)MapInsID->__s0))
        UXMapID mapInsID{};
        // TODO: 需要调用 XActor::GetMapInsID 获取正确的 MapInsID
        // 当前简化实现：使用 ThreadLocalData 检查
        ThreadLocalData* pInstance = ThreadLocalData::GetInstance();
        if (pInstance && pInstance->FindArea(mapInsID)) {
            // IDA: 调用 CGocNetwork::Send
            // if (pUser) pActor = &pUser->XActor; else pActor = nullptr;
            // CGocNetwork::Send(pActor, xSendPacket);
            XActor* pActor = reinterpret_cast<XActor*>(pUser);
            CGocNetwork::Send(pActor, xSendPacket);
        }
    }
}

// IDA: ?SendToLocal@CParty@@QEAAXAEAVXSendPacket@@TUXMapID@@K@Z @ 0x1403A6110
void CParty::SendToLocal(XSendPacket& xSendPacket, UXMapID uxMapID, std::uint32_t dwExceptID) {
    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pMember = pair.second;
        if (pMember) {
            // 检查是否是排除的成员
            if (pMember->GetMemberID() == dwExceptID) {
                continue;
            }

            CUser* pUser = pMember->GetMember();
            if (pUser) {
                // TODO: 检查地图ID是否匹配
                // TODO: 检查ThreadLocalData::IsThreadArea
                // CGocNetwork::Send(&pUser->XActor, &xSendPacket);
            }
        }
    }
}

// IDA: ?SendUpdateMemberInfo@CParty@@QEAAXK@Z @ 0x1403A7320
void CParty::SendUpdateMemberInfo(std::uint32_t dwActorID) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (!pMember) {
        return;
    }

    // 构造ST_UPDATE_PARTY_MEMBER
    ST_UPDATE_PARTY_MEMBER stUpdateMember{};
    stUpdateMember.dwPartyID = m_dwPartyID;
    stUpdateMember.stPartyMember = pMember->GetMemberInfo();

    // TODO: 通过CCommunitySocket发送到RelayServer
    // CCommunitySocket::SendPartyUpdateMemberInfo(&stUpdateMember);
}

// IDA: ?SendPartyUpdateMemberInfo@CParty@@QEAAXPEAVCUser@@@Z @ 0x1403A8720
void CParty::SendPartyUpdateMemberInfo(CUser* pUser) {
    if (!pUser) {
        return;
    }

    // TODO: 获取当前用户的ActorID
    // 遍历所有成员并发送更新信息

    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pMember = pair.second;
        if (pMember) {
            // TODO: 检查是否是当前用户
            // 构造并发送ST_UPDATE_PARTY_MEMBER
        }
    }
}

// IDA: ?ShowPartyInfo@CParty@@QEAAXXZ @ 0x1403A7BE0
void CParty::ShowPartyInfo() {
    // TODO: 使用LogHelper记录队伍信息
    // LogHelper::LogDebug("game.party", "<PARTY_INFO> PID : %d / MasterID : %d / Maze : %I64d",
    //     m_dwPartyID, m_dwMasterID, m_uxMazeID.nMapID);

    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pMember = pair.second;
        if (pMember) {
            const ST_PARTY_MEMBER& info = pMember->GetMemberInfo();
            // TODO: 转换名称并记录
            // char strName[24];
            // WideCharToMultiByte(0, 0, info.strName, -1, strName, 21, nullptr, nullptr);
            // LogHelper::LogDebug("game.party", "<PARTY_MEMBER> ( PID : %d ) ( Name : %s ) ( MapID : %d ) ( Login %d )",
            //     info.dwMemberID, strName, info.nMapID, info.bLogin);
        }
    }
}

// IDA: ?SetMemberMapID@CParty@@QEAAXKHHTUXMapID@@@Z @ 0x1403A6DB0
void CParty::SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, UXMapID uxMapID) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (pMember) {
        pMember->Login();
        // TODO: 调用CForceMember::SetMapID
        pMember->SetChannel(nChannel);
        // TODO: 设置UXMapID
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberHP@CParty@@QEAAXKTUXMapID@@H@Z @ 0x1403A6E80
void CParty::SetMemberHP(std::uint32_t dwActorID, UXMapID uxMapID, int nHP) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (pMember) {
        pMember->SetHP(nHP);

        // 构造并发送HP更新包
        // TODO: 实现PS_PARTY_MEMEBER_HP数据包发送
        // SendToLocal(xSendPacket, uxMapID, dwActorID);
    }
}

// IDA: ?SetMemberMaxHP@CParty@@QEAAXKTUXMapID@@H@Z @ 0x1403A6FE0
void CParty::SetMemberMaxHP(std::uint32_t dwActorID, UXMapID uxMapID, int nMaxHP) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (pMember) {
        pMember->SetMaxHP(nMaxHP);

        // 构造并发送MaxHP更新包
        // TODO: 实现PS_PARTY_MEMEBER_HP数据包发送
        // SendToLocal(xSendPacket, uxMapID, dwActorID);
    }
}

// IDA: ?SetMemberLevel@CParty@@QEAAXKH@Z @ 0x1403A7140
void CParty::SetMemberLevel(std::uint32_t dwActorID, int nLevel) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (pMember) {
        pMember->SetLevel(static_cast<std::uint8_t>(nLevel));
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberAwaken@CParty@@QEAAXKE@Z @ 0x1403A71E0
void CParty::SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (pMember) {
        // TODO: 设置觉醒状态
        // pMember->SetAwaken(byAwaken);
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberProfilePhoto@CParty@@QEAAXKK@Z @ 0x1403A7280
void CParty::SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID) {
    auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end()) {
        return;
    }

    CPartyMember* pMember = it->second;
    if (pMember) {
        pMember->SetProfilePhoto(dwPhotoID);
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?DropItem@CParty@@QEAAXHHAEAUXVec3@@_NHH@Z @ 0x1403A7460
void CParty::DropItem(int nDropID, int nMonsterLv, struct XVec3* vPos, bool bBoss, int nMonsterID, int nMapID) {
    // TODO: 完整实现需要访问XResourceMgr, TB_DROPRATE_LEVEL等表格
    // 计算掉落率和分配
}

// IDA: ?UserKickOut@CParty@@QEAAHK@Z @ 0x1403A7D90
std::uint32_t CParty::UserKickOut(std::uint32_t dwMember) {
    auto it = m_mapPartyMember.find(dwMember);
    if (it != m_mapPartyMember.end()) {
        CPartyMember* pMember = it->second;
        if (pMember) {
            pMember->Clear();
            delete pMember;
        }
        m_mapPartyMember.erase(it);
    }

    return static_cast<std::uint32_t>(m_mapPartyMember.size());
}

// IDA: ?ResEnterMaze@CParty@@QEAAXPEAVCUser@@AEAUPS_ENTER_MAP_RES@@@Z @ 0x1403A7E90
void CParty::ResEnterMaze(CUser* pUser, PS_ENTER_MAP_RES* stEnterMap) {
    if (!pUser) {
        // LogHelper::LogError("game.contents", "ResEnterMaze error - Invalid User( %d )", 970);
        return;
    }

    int nResult = stEnterMap->nResult;
    if (nResult) {
        // 根据错误码发送不同的错误消息
        switch (nResult) {
        case 1:
            // CUser::SendErrorMessage(pUser, 0x11, 0x41, 0xD6EE);
            break;
        case 2:
            // CUser::SendErrorMessage(pUser, 0x11, 0x41, 0xD6EF);
            break;
        case 3:
            // CUser::SendErrorMessage(pUser, 0x11, 0x41, 0xD6F0);
            break;
        }
    } else {
        SendEnterMaze(pUser, stEnterMap);
    }
}

// IDA: ?SendEnterMaze@CParty@@QEAAXPEAVCUser@@AEAUPS_ENTER_MAP_RES@@@Z @ 0x1403A8090
void CParty::SendEnterMaze(CUser* pUser, PS_ENTER_MAP_RES* stEnterMapRes) {
    if (!pUser) {
        return;
    }

    // TODO: 完整实现需要访问XWorldResMgr, XSendDBPacket等
    // 设置stEnterMapRes->byChangeType
    // 获取传送门位置
    // 发送DB数据包
}

// IDA: ?EnterMaze@CParty@@QEAA_NPEAVCUser@@TUXMapID@@AEAUPS_ENTER_MAP_REQ@@@Z @ 0x1403A8940
bool CParty::EnterMaze(CUser* pReqUser, UXMapID uxMazeID, PS_ENTER_MAP_REQ* stEnterMap) {
    if (!pReqUser) {
        return false;
    }

    // 检查迷宫ID
    UXMapID zeroID{};
    if (uxMazeID.nMapID == zeroID.nMapID) {
        // 清空准备列表
        m_vecReadyToMazeMember.clear();
        m_setAgreeToMazeMember.clear();

        // TODO: 检查TB_MAZE_INFO
        // 检查是否是队长
        // 检查所有成员条件
        // 发送进入请求
    } else {
        // 已在迷宫中
        // TODO: 发送到ControlServer
    }

    return true;
}

// IDA: ?OrderPlayPoint@CParty@@QEAAXXZ @ 0x1403A6250
void CParty::OrderPlayPoint() {
    // TODO: 完整实现需要访问CGocRecode获取PlayPoint
    // 排序并设置排名
}

// IDA: ?MazeReward@CParty@@QEAAHK@Z @ 0x1403A6600
int CParty::MazeReward(std::uint32_t dwPlayTime) {
    int nHighRank = 0;
    int nPartyCount = 0;

    // 计算在同一迷宫的成员数量
    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pMember = pair.second;
        if (pMember) {
            CUser* pUser = pMember->GetMember();
            if (pUser) {
                // TODO: 检查是否在同一迷宫
                // nPartyCount++;
            }
        }
    }

    // 发放奖励
    for (auto& pair : m_mapPartyMember) {
        CPartyMember* pMember = pair.second;
        if (pMember) {
            // TODO: 调用CGocRecode::MazeReward
            // 更新nHighRank
        }
    }

    OrderPlayPoint();
    ApplyReward();
    SendMazeClear(0);

    return nHighRank;
}

// IDA: ?ApplyReward@CParty@@QEAAXXZ @ 0x1403A6990
void CParty::ApplyReward() {
    // TODO: 应用奖励逻辑
}

// IDA: ?SendMazeClear@CParty@@QEAAXH@Z
void CParty::SendMazeClear(int nParam) {
    // TODO: 发送迷宫清除通知
}
