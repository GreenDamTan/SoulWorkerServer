#include "Soulworker/GameServer/XGameServer/GameModeBase.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"

GameModeBase::GameModeBase()
    : m_bPlayMode(false)
    , m_byModeResult(0)
    , m_pMaze(nullptr)
    , m_eModeType(static_cast<eGAMEMODE_TYPE>(0)) {
}

GameModeBase::~GameModeBase() = default;

void GameModeBase::Init() {
    m_pMaze = nullptr;
    m_bPlayMode = false;
    m_byModeResult = 0;
}

// PDB 中 Tick (0001:00187DB0) 与其他空虚函数共用机器码；IDA 中该地址折叠到 VResourceManager::OnTickFunction。
void GameModeBase::Tick(float) {
}

// PDB 中 StartMode (0001:0018E110) 与 Intrusion 共用空函数机器码；IDA 函数名另有所属。
void GameModeBase::StartMode(XMaze*) {
}

// PDB 中 CheckEndMode (0001:001ABFA0) 与其他恒假函数共用机器码；IDA 指令为 xor al,al; ret。
bool GameModeBase::CheckEndMode() {
    return false;
}

void GameModeBase::SetModeState(int nState) {
    m_nModeState = nState;
}

// PDB 中 Intrusion (0001:0018E110) 与 StartMode 共用空函数机器码。
void GameModeBase::Intrusion(CUser*) {
}

void GameModeBase::SendNoticePacket(XMaze* pMaze, int nType, int nValue, float fTime, bool bExceptDie) {
    if (pMaze) {
        XSendPacket xPacket(0x11, 0x51);
        xPacket << nType;
        xPacket << nValue;
        xPacket << fTime;
        // TODO: 需人工审查：XModeMaze 的原始四参数虚覆盖尚未按 XSendPacket& 签名还原，完整虚槽顺序也未核对。
        pMaze->SendBroadCast(xPacket, nullptr, bExceptDie, E_BROADCAST_TYPE::eNoneSelf);
    }
}

void GameModeBase::SendNoticePacket(CUser* pUser, int nType, int nValue, float fTime) {
    if (pUser) {
        XSendPacket xPacket(0x11, 0x51);
        xPacket << nType;
        xPacket << nValue;
        xPacket << fTime;
        CGocNetwork::Send(static_cast<XActor*>(pUser), xPacket);
    }
}
