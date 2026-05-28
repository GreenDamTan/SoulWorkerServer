// GameSockets.cpp
// Game Server Socket Implementations - Decompiled from IDA

#include "GameSockets.h"
#include "Soulworker/GameServer/XCore/XServer/XPacket.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "User.h"
#include "GameServer.h"

// ============================================================================
// CGameControlSocket Implementation
// ============================================================================

bool CGameControlSocket::ServerProcessEx(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x08: return RecvServerShutdown(xPacket);
        case 0x09: return RecvTimeEvent(xPacket);
        case 0x14: return RecvValueEvent(xPacket);
        case 0x15: return RecvCashShopBanner(xPacket);
        case 0x21: return RecvCreateMazeReq(xPacket);
        case 0x22: return RecvCreateMazeRes(xPacket);
        case 0x25: return RecvRemoveMaze(xPacket);
        case 0x28: return RecvGoBackMazeRes(xPacket);
        case 0x30: return RecvCreateMap(xPacket);
        case 0x31: return RecvEnterMap(xPacket);
        case 0x32: return RecvCheckPartyInMaze(xPacket);
        case 0x33: return RecvUsersInfo(xPacket);
        case 0x38: return ResCheckEnterMaze(xPacket);
        case 0x41: return RecvPartyMatching(xPacket);
        case 0x42: return RecvForceMatching(xPacket);
        case 0x50: return RecvMyRoomEnterReq(xPacket);
        case 0x51: return RecvMyRoomEnter(xPacket);
        case 0x52: return RecvMyRoomCreate(xPacket);
        case 0x53: return RecvMyRoomDeleteReq(xPacket);
        case 0x54: return RecvEnterMapToOther(xPacket);
        case 0x56: return RecvPostSend(xPacket);
        case 0x58: return RecvFindUser(xPacket);
        case 0x59: return RecvMyRoomDeleteRes(xPacket);
        case 0x60: return RecvEnterUserInfo(xPacket);
        case 0x61: return RecvServerOptionUpdate(xPacket);
        case 0x66: return RecvServerDayEventBoosterList(xPacket);
        case 0x70: return RecvCachingComplete(xPacket);
        case 0x72: return RecvServerCreateModeMazeReq(xPacket);
        case 0x74: return RecvServerRouletteEvent(xPacket);
        default: return true;
    }
}

bool CGameControlSocket::PartyProcess(XPacket* xPacket) {
    // IDA: subcmd 8 -> RecvPartyEnterMaze
    if (xPacket->GetSubCmd() == 8) {
        return RecvPartyEnterMaze(xPacket);
    }
    return true;
}

// Stub implementations for CGameControlSocket packet handlers
bool CGameControlSocket::RecvServerShutdown(XPacket*) { return true; }
bool CGameControlSocket::RecvTimeEvent(XPacket*) { return true; }
bool CGameControlSocket::RecvValueEvent(XPacket*) { return true; }
bool CGameControlSocket::RecvCashShopBanner(XPacket*) { return true; }
bool CGameControlSocket::RecvCreateMazeReq(XPacket*) { return true; }
bool CGameControlSocket::RecvCreateMazeRes(XPacket*) { return true; }
bool CGameControlSocket::RecvRemoveMaze(XPacket*) { return true; }
bool CGameControlSocket::RecvGoBackMazeRes(XPacket*) { return true; }
bool CGameControlSocket::RecvCreateMap(XPacket*) { return true; }
bool CGameControlSocket::RecvEnterMap(XPacket*) { return true; }
bool CGameControlSocket::RecvCheckPartyInMaze(XPacket*) { return true; }
bool CGameControlSocket::RecvUsersInfo(XPacket*) { return true; }
bool CGameControlSocket::ResCheckEnterMaze(XPacket*) { return true; }
bool CGameControlSocket::RecvPartyMatching(XPacket*) { return true; }
bool CGameControlSocket::RecvForceMatching(XPacket*) { return true; }
bool CGameControlSocket::RecvMyRoomEnterReq(XPacket*) { return true; }
bool CGameControlSocket::RecvMyRoomEnter(XPacket*) { return true; }
bool CGameControlSocket::RecvMyRoomCreate(XPacket*) { return true; }
bool CGameControlSocket::RecvMyRoomDeleteReq(XPacket*) { return true; }
bool CGameControlSocket::RecvEnterMapToOther(XPacket*) { return true; }
bool CGameControlSocket::RecvPostSend(XPacket*) { return true; }
bool CGameControlSocket::RecvFindUser(XPacket*) { return true; }
bool CGameControlSocket::RecvMyRoomDeleteRes(XPacket*) { return true; }
bool CGameControlSocket::RecvEnterUserInfo(XPacket*) { return true; }
bool CGameControlSocket::RecvServerOptionUpdate(XPacket*) { return true; }
bool CGameControlSocket::RecvServerDayEventBoosterList(XPacket*) { return true; }
bool CGameControlSocket::RecvCachingComplete(XPacket*) { return true; }
bool CGameControlSocket::RecvServerCreateModeMazeReq(XPacket*) { return true; }
bool CGameControlSocket::RecvServerRouletteEvent(XPacket*) { return true; }
bool CGameControlSocket::RecvPartyEnterMaze(XPacket*) { return true; }

// ============================================================================
// CCommunitySocket Implementation
// ============================================================================

bool CCommunitySocket::PartyProcess(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x01: return RecvPartyCreate(xPacket);
        case 0x02: return RecvPartyJoinMember(xPacket);
        case 0x03: return RecvPartyLeaveMember(xPacket);
        case 0x04: return RecvPartyChangeMaster(xPacket);
        case 0x05: return RecvUpdatePartyMember(xPacket);
        case 0x06: return RecvPartyDelete(xPacket);
        case 0x08: return RecvPartyEnterMaze(xPacket);
        case 0x09: return RecvPartyUpdateInfo(xPacket);
        case 0x10: return RecvPartyEnterServer(xPacket);
        case 0x11: return RecvPartyInvite(xPacket);
        case 0x12: return RecvPartyAccept(xPacket);
        case 0x13: return RecvPartyReject(xPacket);
        case 0x14: return RecvPartyMessage(xPacket);
        case 0x20: return RecvPartyMatchingEnter(xPacket);
        case 0x21: return RecvPartyMatchingExit(xPacket);
        case 0x22: return RecvPartyMatchingCheck(xPacket);
        case 0x23: return RecvPartyMatchingReset(xPacket);
        case 0x24: return RecvPartyMatchingWait(xPacket);
        case 0x25: return RecvPartyRecruitAdd(xPacket);
        case 0x26: return RecvPartyRecruitDel(xPacket);
        case 0x27: return RecvPartyRecruitApply(xPacket);
        case 0x28: return RecvPartyRecruitApplyAccept(xPacket);
        case 0x29: return RecvPartyRecruitApplyReject(xPacket);
        case 0x2A: return RecvPartyRecruitApplyUpdate(xPacket);
        case 0x2B: return RecvPartyRecruitList(xPacket);
        case 0x2C: return RecvPartyRecruitMyApplyList(xPacket);
        case 0x2D: return RecvPartyRecruitApplyList(xPacket);
        case 0x2E: return RecvPartyRecruitInfo(xPacket);
        case 0x2F: return RecvPartyRecruitApplyDel(xPacket);
        case 0x30: return RecvPartyRecruitApplyInfo(xPacket);
        case 0x31: return RecvPartyRecruitApplyNotice(xPacket);
        case 0x32: return RecvPartyRecruitApplyAcceptCheck(xPacket);
        case 0x40: return RecvPartyInfo(xPacket);
        case 0x41: return RecvPartyNameChange(xPacket);
        case 0x42: return RecvPartyMatchingMaze(xPacket);
        case 0x43: return RecvPartyMazeClear(xPacket);
        default: return true;
    }
}

bool CCommunitySocket::LeagueProcess(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x01: return RecvCreateLeague(xPacket);
        case 0x02: return RecvLeagueDelete(xPacket);
        case 0x03: return RecvLeagueLogin(xPacket);
        case 0x06: return RecvLeagueInfo(xPacket);
        case 0x07: return RecvLeagueDelegate(xPacket);
        case 0x08: return RecvLeagueWithDraw(xPacket);
        case 0x09: return RecvLeagueMemberKick(xPacket);
        case 0x0A: return RecvLeagueInfoChange(xPacket);
        case 0x0C: return RecvLeagueInvite(xPacket);
        case 0x0D: return RecvLeagueInviteAccept(xPacket);
        case 0x10: return RecvLeagueInviteReject(xPacket);
        case 0x14: return RecvLeagueBoard(xPacket);
        case 0x16: return RecvLeagueApplicantAcceptRes(xPacket);
        case 0x18: return RecvLeagueSearch(xPacket);
        case 0x19: return RecvLeagueApplicantRes(xPacket);
        case 0x20: return RecvLeagueApplicantAdd(xPacket);
        case 0x21: return RecvLeagueApplicantReject(xPacket);
        case 0x22: return RecvLeagueApplicantDelete(xPacket);
        case 0x23: return RecvLeagueNoticeChange(xPacket);
        case 0x24: return RecvLeagueList(xPacket);
        case 0x25: return RecvLeagueNameChange(xPacket);
        case 0x26: return RecvLeagueCardChange(xPacket);
        case 0x27: return RecvLeaguePositionNameChange(xPacket);
        case 0x28: return RecvLeagueAuthChange(xPacket);
        case 0x36: return RecvLeagueMessage(xPacket);
        case 0x37: return RecvLeagueMemberPositionChange(xPacket);
        case 0x38: return RecvLeagueApplicantUpdate(xPacket);
        case 0x39: return RecvLeagueMemberLogOut(xPacket);
        case 0x40: return RecvLeagueApplicantJoinUser(xPacket);
        case 0x41: return RecvLeagueInviteJoinUser(xPacket);
        case 0x42: return RecvLeagueMemberUpdate(xPacket);
        case 0x45: return RecvLeagueOpenOrNot(xPacket);
        case 0x46: return RecvLeagueRecruitNotice(xPacket);
        case 0x47: return RecvLeagueRecordUpdate(xPacket);
        case 0x48: return RecvLeagueCardChangeRes(xPacket);
        case 0x51: return RecvLeagueLevelup(xPacket);
        case 0x53: return RecvLeagueSkillLearn(xPacket);
        case 0x55: return RecvLeagueWealth(xPacket);
        case 0x58: return RecvLeagueSyncInfo(xPacket);
        case 0x59: return RecvLeagueSyncLoad(xPacket);
        case 0x60: return RecvLeagueInventoryInfo(xPacket);
        case 0x61: return RecvLeagueInventoryMove(xPacket);
        default: return true;
    }
}

// Stub implementations for CCommunitySocket packet handlers
bool CCommunitySocket::RecvPartyCreate(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyJoinMember(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyLeaveMember(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyChangeMaster(XPacket*) { return true; }
bool CCommunitySocket::RecvUpdatePartyMember(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyDelete(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyEnterMaze(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyUpdateInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyEnterServer(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyInvite(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyAccept(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyReject(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMessage(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingEnter(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingExit(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingCheck(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingReset(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingWait(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitAdd(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitDel(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApply(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyAccept(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyReject(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitList(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitMyApplyList(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyList(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyDel(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyNotice(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyAcceptCheck(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyNameChange(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingMaze(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMazeClear(XPacket*) { return true; }

// League stub implementations
bool CCommunitySocket::RecvCreateLeague(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueDelete(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueLogin(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueDelegate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueWithDraw(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMemberKick(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInfoChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInvite(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInviteAccept(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInviteReject(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueBoard(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantAcceptRes(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSearch(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantRes(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantAdd(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantReject(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantDelete(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueNoticeChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueList(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueNameChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueCardChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeaguePositionNameChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueAuthChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMessage(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMemberPositionChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMemberLogOut(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantJoinUser(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInviteJoinUser(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMemberUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueOpenOrNot(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueRecruitNotice(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueRecordUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueCardChangeRes(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueLevelup(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSkillLearn(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueWealth(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSyncInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSyncLoad(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInventoryInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInventoryMove(XPacket*) { return true; }
