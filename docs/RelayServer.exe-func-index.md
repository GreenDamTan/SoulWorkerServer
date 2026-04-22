# RelayServer.exe 函数索引

| 所属目录 | 文件名 | 函数名 | 当前状态 | 来源 | 是否验证 |
| --- | --- | --- | --- | --- | --- |
| `GameServer/XRelayServer` | `ServerMain.cpp` | `ServiceInit` | decompiled | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ServerMain.cpp` | `ServiceMain` | decompiled | XLoginServer 类比 + IDA 调用链 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `TXSingleton<XRelayServer>::Instance()` 接入点 | decompiled | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::XRelayServer` 骨架 | asm_restored | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::AddServerInfo` | asm_restored | export-for-ai + PDB symbols + `CServerProcess::ReqCreateServer` 调用链 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::AddGameServerInfo` | asm_restored | export-for-ai `1400B28A0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SetUsersInfo` | asm_restored | export-for-ai + PDB symbols + `CServerProcess::SyncUsersInfo` 调用链 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SetCachingLoad` | asm_restored | export-for-ai `1400BD8E0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::UnSetCachingLoad` | asm_restored | export-for-ai `1400BD9B0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SendCachingLoad` | asm_restored | export-for-ai `1400BDA30.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SendDBGame` | decompiled | export-for-ai `1400BD530.c` + LoginServer 类比 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SendDBAccount` | asm_restored | export-for-ai `1400BD4A0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::GetServer` | asm_restored | export-for-ai `1400B9D40.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::AddUser` | asm_restored | export-for-ai `1400B0A90.c` + `SetUsersInfo` 调用链 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::AddPartyUser` | asm_restored | export-for-ai `1400B0FD0.c` + `AddUser` 调用链 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::AddLeagueUser` | asm_restored | export-for-ai `1400BE110.c` + `AddUser` 调用链 | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::GetUser(uint32)` | asm_restored | export-for-ai `1400B1890.c` + callers graph | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::GetUser(wchar_t*)` | asm_restored | export-for-ai `1400BAAC0.c` + callers graph | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::GetPartyUser` | asm_restored | export-for-ai `1400B1980.c` + callers graph | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::UpdateUserMap` | asm_restored | export-for-ai `1400B2030.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::RemoveUser` | asm_restored | export-for-ai `1400B1280.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::RemovePartyUser` | asm_restored | export-for-ai `1400B16C0.c` + callers graph | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SendChatNotice` | asm_restored | export-for-ai `1400BA3C0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::SendChatMegaPhone` | asm_restored | export-for-ai `1400BA450.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `XRelaySocket::OnConnect` | decompiled | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `XRelaySocket::ServerProcess` | decompiled | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `XRelaySocket::RecvServerUpdate` | decompiled | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `XRelaySocket::OnStartThread` | asm_restored | export-for-ai + PDB symbols + `TXDBSocket` 线程循环类比 | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `XRelaySocket::SendAddServer` | decompiled | IDA + export-for-ai | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `XRelaySocket::SendUpdateServerInfo` | decompiled | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::SetMyInfo` | decompiled | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::ServerProcessEx` | decompiled | IDA + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::ResCreateMatchingMaze` | asm_restored | IDA + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::SyncPartyMazeInfo` | asm_restored | IDA + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::SyncForceMazeInfo` | asm_restored | IDA + PDB symbols | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::ResCreateMatchingModeMaze` | asm_restored | IDA + PDB symbols | no |
| `GameServer/XRelayServer` | `ServerProcess.cpp` | `CServerProcess::Parse` | decompiled | ServerProcess.obj + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ServerProcess.cpp` | `CServerProcess::ReqCreateServer` | decompiled | export-for-ai + PDB symbols + `CServer::SetServerInfo` 调用链 | no |
| `GameServer/XRelayServer` | `ServerProcess.cpp` | `CServerProcess::ReqUpdateServerInfo` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ServerProcess.cpp` | `CServerProcess::SyncUsersInfo` | asm_restored | export-for-ai + PDB symbols + `XRelayServer::SetUsersInfo` 调用链 | no |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.cpp` | `CServerModeMazeProcess::Parse` | decompiled | ServerModeMazeProcess.obj + export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.cpp` | `CServerModeMazeProcess::ReqServerModeMazeEnter` | asm_restored | export-for-ai + PDB symbols + `CLogicThreadManager::DoJob` 调用链 | no |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.cpp` | `CServerModeMazeProcess::ReqServerModeMazeExit` | asm_restored | export-for-ai + PDB symbols + `CLogicThreadManager::DoJob` 调用链 | no |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.cpp` | `CServerModeMazeProcess::ReqServerModeMazeTime_Cheat` | asm_restored | export-for-ai + PDB symbols + `CLogicThreadManager::DoJob` 调用链 | no |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.cpp` | `CServerModeMazeProcess::ReqServerModeMazeMatchingEvent` | asm_restored | export-for-ai + PDB symbols + `CLogicThreadManager::DoJob` 调用链 | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::Parse` | asm_restored | export-for-ai `1400D74F0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::SyncLoginUser` | asm_restored | export-for-ai `1400D76D0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::SyncLogoutUser` | asm_restored | export-for-ai `1400D77D0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::SyncUpdateUserMap` | asm_restored | export-for-ai `1400D7880.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::SyncUserLevelup` | asm_restored | export-for-ai `1400D78E0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `FriendProcess.cpp` | `CFriendProcess::{Parse, ReqFriendListLoad, ReqBlockListLoad}` | asm_restored | RelayServer decompile `140040370.c / 1400405F0.c / 140040640.c` + `XRelayServer::{SendFriendList, SendBlockList}` 调用链 | no |
| `GameServer/XRelayServer` | `PartyProcess.cpp` | `CPartyProcess::{Parse, ReqPartyUpdateMember, ReqPartyEnterServer, ReqPartyInfo, ReqPartyRecruitAdd, ReqPartyRecruitDel, ReqPartyRecruitApply, ReqPartyRecruitApplyAccept, ReqPartyRecruitApplyReject, ReqPartyRecruitList, ReqPartyRecruitMyApplyList, ReqPartyRecruitApplyList, ReqPartyRecruitApplyDel, ReqPartyRecruitApplyInfo}` | asm_restored | RelayServer decompile `1400A1D40.c / 1400A32E0.c / 1400A3450.c / 1400A35F0.c / 1400A47F0.c / 1400A5520.c / 1400A5900.c / 1400A6010.c / 1400A62D0.c / 1400A6560.c / 1400A66A0.c / 1400A68C0.c / 1400A6BB0.c / 1400A6D40.c` + `CLogicThreadManager::DoJob` / bounded party recruit apply + accept-reject + apply-info continuation | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::ReqUserChatWhisper` | asm_restored | RelayServer decompile `1400D7A10.c` + `PS_CHAT_WHISPER / PS_CHAT_ITEM_LINK_FOR_SERVER` 解包顺序 | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::{SendPacket, CheckGameOption, GetFriendUCID, GetFriendList, GetBlcokList, SetLoadFriendList, SetLoadBlockList, GetLoadFriendList, GetLoadBlockList, SetSyncFriendList, SetSyncBlockList, IsFriendList, IsBlockList}` | asm_restored | RelayServer decompile `1400D4B00.c / 1400D5150.c / 1400D4170.c / 1400D41A0.c / 1400C9580.c / 1400C9620.c` + existing query wrappers | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CCommunity::{Clear, IsFriend, IsBlockList, GetFriendUCID, GetFriendList, GetBlcokList, SetLoadFriendList, SetLoadBlockList, GetLoadFriendList, GetLoadBlockList, SetSyncFriendList, SetSyncBlockList}` | asm_restored | RelayServer decompile `140001110.c / 140001270.c / 1400013E0.c / 140001600.c / 140001710.c / 140001AE0.c / 140001D80.c / 140001E80.c / 1400C9560.c / 1400C9600.c / 1400D6870.c / 1400D6890.c / 1400D68B0.c / 1400D68D0.c` | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CFriendMember::{GetUCID, GetType, GetName}` | asm_restored | `CCommunity::IsFriend/GetFriendUCID` 调用点 + IDA type inspect | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CBlockUser::{GetUCID, GetName}` | asm_restored | `CCommunity::IsBlockList` 调用点 + IDA type inspect | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::{SendFriendList, SendBlockList}` | asm_restored | RelayServer decompile `1400B3AB0.c / 1400B3BD0.c` + `CUserObject::{SendFriendList, SendBlockList}` send path | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::{KickOutUser, SendPacketAll, SendPacket}` | asm_restored | RelayServer decompile `1400B25F0.c` + friend/block list missing-user path callers + bounded server-targeted send shim for recruit apply-del | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::ReqUserChatNotice` | asm_restored | export-for-ai `1400D7B30.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::ReqUserChatMegaPhone` | asm_restored | export-for-ai `1400D7BA0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::ReqUserOption` | asm_restored | export-for-ai `1400D8360.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::SyncUserAwaken` | asm_restored | export-for-ai `1400D7940.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `UserProcess.cpp` | `CUserProcess::SyncUserProfilePhoto` | asm_restored | export-for-ai `1400D79A0.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatching.cpp` | `CModeMazeMatching::AutoMatchingEnter` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatching.cpp` | `CModeMazeMatching::ExitMatching` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatching.cpp` | `CModeMazeMatching::SendMatchingWait` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatching.cpp` | `CModeMazeMatching::SendMatchingExit` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatching.cpp` | `CModeMazeMatching::{OnUpdate, MatchingPossible, MatchingWait, MakeOperationMaze, SendCreateMatchingModeMaze}` | asm_restored | export-for-ai `140033980.c / 140032F80.c / 140032FA0.c / 140033AA0.c / 140034170.c` + current bounded control-create path | no |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.cpp` | `CModeMazeMatchingMgr::EnterMatching` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.cpp` | `CModeMazeMatchingMgr::AddModeMazeMatchingWait` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatchginMember::Clear` + `CPartyMatching::{OnUpdate, MatchingPossible, MatchingCheck, MatchingWait, SendMatchingCheck, SendMatchingWait, SendMatchingStart, SendMatchingExit, AutoMatchingExit, AutoMatchingAccept, LeaderSelect}` + `CPartyMatchingMgr::{OnUpdate, FindRecruitID, FindRecruitPtr, GetPartyRecruitInfo, ReqPartyRecruitApply, ReqRecruitAccept, ReqRecruitReject, DeletePartyRecruit, ReqPartyRecruitCreate, ReqPartyRecruitDel, SendPartyRecruitList, SendPartyRecruitMyApplyList, SendPartyRecruitApplyList, SendPartyRecruitApplyInfo}` | asm_restored | RelayServer decompile `0x14009D9A0 / 0x14009D050 / 0x14009D1A0 / 0x14009D2D0 / 0x14009D410 / 0x14009CE00 / 0x14009CEE0 / 0x14009C750 / 0x14009C2A0 / 0x14009BB20 / 0x14009BDD0 / 0x14009BEE0 / 0x14009ED70 / 0x14009F220 / 0x14009F330 / 0x14009E000 / 0x14009EF00 / 0x14009EE50 / 0x14009EF70 / 0x1400A52BA / 0x1400A5768 / 0x1400A668E / 0x1400A6D40.c-adjacent manager query` + bounded recruit apply + accept-reject + apply-info manager slice | no |
| `GameServer/XRelayServer` | `PartyRecruit.cpp` | `CPartyRecruit::{GetRecruitInfo, SetRecruitDate, ClearRecruitDate, ApplyMemberClear, GetMember, GetPartyMemberList, GetApplyCount, IsApplied, RecruitApply, RecruitAccept, SetRecruitInfo, AddMember, RemoveMember, DelApplyMember, SendApplyUserList}` + `CUserPartyInfo::{GetApplyRecruitCount, IsApplyRecruit, CanApplyRecruit, ClearApplyParty, DelPartyRecruit}` | asm_restored | RelayServer decompile `0x1400AE0B0 / 0x1400AE150 / 0x1400AE180 / 0x1400AE100 / 0x1400AE1E0 / 0x1400AE570 / 0x1400AEAF0 / 0x1400D6F00 / 0x1400D6F50 / 0x1400D6FA0 / 0x1400D6FE0 / 0x1400D7030` + bounded recruit apply + accept-check + apply-info member-list helpers | no |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::EnterServer` master-enter recruit follow-up (`0xF4/0x2E`) | asm_restored | RelayServer decompile `0x140016380` + `CPartyMatchingMgr::GetPartyRecruitInfo(0x14009EF70)` callsite | no |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.cpp` | `CModeMazeMatchingMgr::ExitMatching` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.cpp` | `CModeMazeMatchingMgr::MatchingRemoveUser` | asm_restored | export-for-ai `140039A00.c` + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.cpp` | `CModeMazeMatchingMgr::ModeMazeMatchingEvent` | asm_restored | export-for-ai + PDB symbols | no |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.cpp` | `CModeMazeMatchingMgr::{ModeMazeMatchingEvent, OnUpdate, ProcessWaitList, ProcessMazeMake, DestroyMatchingWait, SetMatchingState, SendCreateMatchingModeMaze}` | asm_restored | export-for-ai `140039C60.c / 1400370F0.c / 140037FF0.c / 140038740.c / 1400389A0.c / 14003CCC0.c / 140039A40.c` + verifier-driven bounded event-member recovery | no |
| `GameServer/XRelayServer` | `ForceProcess.cpp` | `CForceProcess::{Parse, ReqForceUpdateMember, ReqForceEnterServer, SyncForceMessage, ReqForceInfo}` | asm_restored | RelayServer decompile `140023360.c / 140024760.c / 1400248D0.c / 140023570.c / 140026870.c` + IDA type inspect | no |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::{ResUpdateMemberInfo, SendForceMessage, EnterServer, GetForce, ReqForceInfo}` | asm_restored | RelayServer PDB symbols + decompile `0x140017CC0 / 0x140017D50 / 0x140016380 / 0x1400148D0 / 0x140014970 / 0x140026960` + `0xFA/0x05` / `0xFA/0x10` / `0xFA/0x0A` / `0xFA/0x1A` 链 | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatchginMember::Clear` | asm_restored | RelayServer decompile `0x14009D9A0` + IDA type inspect | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatching::{AutoMatchingExit, SendMatchingExit}` | asm_restored | RelayServer decompile `0x14001C920 / 0x14001D160` + IDA type inspect | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatchingMgr::{ExitMatching, MatchingRemoveUser}` | asm_restored | RelayServer decompile `0x1400215C0 / 0x140021C90` + `XRelayServer::RemovePartyUser` 清理链 | no |
| `GameServer/XRelayServer` | `ForceProcess.cpp` | `CForceProcess::{ReqForceMatchingExit, ReqForceMatchingCheck}` | asm_restored | RelayServer decompile `0x140025ED0 / 0x1400265E0` + `CForceMatchingMgr::{ExitMatching, CheckMatching}` 调用链 | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatching::{AutoMatchingAccept, LeaderSelect, SendMatchingWait}` | asm_restored | RelayServer decompile `0x14001CCB0 / 0x14001CDA0 / 0x14001E170` + IDA type inspect | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatchingMgr::CheckMatching` | asm_restored | RelayServer decompile `0x140021680` + `ReqForceMatchingCheck(0x1400265E0)` 调用链 | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatching::{SendMatchingCheck, SendMatchingReset, SendMatchingStart, MatchingPossible, MatchingCheck, MatchingWait}` | asm_restored | RelayServer decompile `0x14001DB30 / 0x14001DBD0 / 0x14001D620 / 0x14001D270 / 0x14001D410 / 0x14001D5C0` + IDA type inspect | no |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::{CreateForceMatching, DeleteForce, ReqForceInfo(actor-first fix)}` | asm_restored | RelayServer decompile `0x140017FE0 / 0x140017440 / 0x140026960 / 0x140014970` + current `m_mapForceUser` path | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatching::{CreateMazeMatching, SendCreateMatchingMaze}` | asm_restored | RelayServer decompile `0x14001E720 / 0x14001DA20` + control callback chain | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatchingMgr::{ResForceMatchingCreate, SendCreateMatchingMaze}` | asm_restored | RelayServer decompile `0x140021980 / 0x140021A00` + `RelayControlSocket::ResCreateMatchingMaze` callers | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatching::SendMatchingInfo` average-level fix | asm_restored | verifier-driven semantic correction to restore active-group average used by `CheckAutoMatchingEnter` | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatching::{CheckMazeOpenTime, OnUpdate}` | asm_restored | RelayServer decompile `0x14001E640 / 0x14001E2A0` + relay-local maze-open checker | no |
| `GameServer/XRelayServer` | `ForceMatching.cpp` | `CForceMatchingMgr::OnUpdate` | asm_restored | RelayServer decompile `0x140021810` + current active-map erase loop | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::InitServer` (party wait config + XResourceMgr 初始化) | asm_restored | RelayServer decompile `0x1400b05a0` + CommonDB shim + XResourceMgr::Init/Load 对齐 | no |
| `GameServer/XRelayServer/Thread` | `LogicThreadProcessor.cpp` | `CLogicThreadProc::OnUpdate` worker 0/1/2 routing skeleton | asm_restored | RelayServer decompile `0x1400D0660` + bounded hooks (`PartyManager::Clear` + `PartyMatchingMgr` + `ForceManager::Clear` + `ForceMatchingMgr` + `ModeMazeMatchingMgr` + league/recruit no-op OnUpdate)` | no |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::{OnParse, DBParse, DBForceParse, ResForceMatchingCreate}` | asm_restored | RelayServer decompile `0x1400497C0 / 0x140049A60 / 0x14004DC30 / 0x14004DD70` + current TXDBSocketT gate workaround | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `CRelayMazeOpenControl::{Init, LoadRows, CheckMazeOpenTime}` | asm_restored | RelayServer decompile `0x140127CE0 / 0x14002BCC0` + current bounded CommonDB single-table loader | no |
| `GameServer/XRelayServer` | `ForceProcess.cpp` | `CForceProcess::ReqForceChangeMazeOpenCheck` | asm_restored | RelayServer decompile `0x140026AA0 / 0x140026B30` + worker-0 toggle to relay-local maze-open helper | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `CFriendRecruitManager::{UpdateRecruit, IsRecruitList, DeleteRecruit, OnUpdate}` | asm_restored | RelayServer decompile `0x140045580 / 0x1400452D0 / 0x140045410 / 0x140045110` + bounded worker-2 recruit manager | no |
| `GameServer/XRelayServer` | `RelayServer.cpp` | `XRelayServer::{PrepareDeleteRecruit, SendRecruitDelete, DeleteRecruit}` | asm_restored | RelayServer decompile `0x1400B8930 / 0x1400B93A0 / 0x1400B8D00` + friend recruit delete request/response closure | no |
| `GameServer/XRelayServer` | `FriendProcess.cpp` | `CFriendProcess::ReqFriendRecruitDelete` | asm_restored | RelayServer decompile friend recruit delete request path + `CLogicThreadManager::DoJob(2)` worker dispatch | no |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::{DBFriendParse, ResRecruitDelete}` | asm_restored | RelayServer decompile `0x14004C150` + bounded `main=5/sub=0x10` DB response path | no |
| `GameServer/XRelayServer` | `PartyProcess.cpp` | `CPartyProcess::ResPartyRecruitApplyAcceptCheck` | asm_restored | RelayServer decompile `0x1400A7040` + `CLogicThreadManager::DoJob(0)` + `CPartyManager::ResRecruitAccept` | verified |
| `GameServer/XRelayServer` | `PartyManager.cpp` | `CPartyManager::{GetParty, GetPartyID, AddPartyMember, DeleteParty}` | asm_restored | RelayServer decompile `0x140097460 / 0x140097540 / 0x140097580 / 0x1400975B0` + `m_mapParty`/`m_mapPartyUser` index | verified |
| `GameServer/XRelayServer` | `PartyManager.cpp` | `CPartyManager::ResRecruitAccept` | asm_restored | RelayServer decompile `0x1400995A0` + party/force branching + DB send + error codes | verified |
| `GameServer/XRelayServer` | `PartyManager.cpp` | `CPartyManager::{ReqJoinMember, ReqCreateParty}` | asm_restored | RelayServer decompile `0x1400972C0 / 0x140095690` + `main=4/sub=2,1` DB game packet + DBLog 22/5 | verified |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::{ReqJoinMember, ReqCreateForce}` | asm_restored | RelayServer decompile `0x1400166F0 / 0x1400149C0` + `main=8/sub=2,1` DB game packet + DBLog 23/5 | verified |
| `GameServer/XRelayServer` | `Party.cpp` | `CParty::{AddMember, GetUserCount, GetMasterID, GetPartyID}` | asm_restored | bounded party-member index + map container | verified |
| `GameServer/XRelayServer` | `Party.cpp` | `CParty::GetPartyInfo` | asm_restored | RelayServer decompile `0x1400976f4` + member-list aggregation for `PS_PARTY_INFO` broadcast | verified |
| `GameServer/XRelayServer` | `Force.cpp` | `CForce::AddMember` | asm_restored | RelayServer decompile force-member add path + `m_mapForceMember` insertion | verified |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::DBPartyParse` | asm_restored | RelayServer decompile `0x140049930` + bounded `main=4/sub=2 -> ResPartyJoin` | verified |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::{ResPartyJoin, ResForceJoin}` | asm_restored | RelayServer decompile `0x14004AA80 / 0x14004B340` + worker-0 dispatch | verified |
| `GameServer/XRelayServer` | `PartyManager.cpp` | `CPartyManager::ResJoinMember` | asm_restored | RelayServer decompile `0x140097630` + `0xF4/0x02` fanout + `AddRecruitMember` | verified |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::ResJoinMember` | asm_restored | RelayServer decompile `0x140016A60` + `0xFA/0x02` fanout + `AddRecruitMember` | verified |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatchingMgr::AddRecruitMember` | asm_restored | RelayServer decompile `0x14009F980` + `AddMember` + `m_mpRecruitUser` insert | verified |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatchingMgr::DeleteRecruitMember` | asm_restored | RelayServer decompile `0x14009FA20` + `RemoveMember` + `m_mpRecruitUser` erase | no |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::{ResForceLeave, ResForceDelete}` | asm_restored | RelayServer decompile `0x14004B500 / 0x14004B800` + worker-0 dispatch | no |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::{ResChangeMaster, ResForceLeave, ResDeleteForce}` | asm_restored | RelayServer decompile `0x140017610 / 0x1400170A0 / 0x140017720` + `0xFA/0x05,0x03,0x06` fanout + recruit cleanup | no |
| `GameServer/XRelayServer` | `PartyManager.cpp` | `CPartyManager::CreatePartyMatching` | asm_restored | RelayServer decompile `0x140098dd0` + `ST_PARTY_INFO` based party creation for matching | no |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatching::CreateMazeMatching` | asm_restored | RelayServer decompile `0x14009d430` + `PS_PARTY_INFO / ST_CREATE_MAZE` construction + `0xF2/0x43` control send | no |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatchingMgr::ResPartyMatchingCreate` | asm_restored | RelayServer decompile `0x14009e400` + matching lookup + `CreateMazeMatching` dispatch | no |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatching::SendCreateMatchingMaze` | asm_restored | RelayServer decompile `0x14009c9c0` + `0xF4/0x42` send + `SetMatchingState(false)` + `SetMatchingID(0,0)` + `SendDBLog` | no |
| `GameServer/XRelayServer` | `PartyMatchingMgr.cpp` | `CPartyMatchingMgr::SendCreateMatchingMaze` | asm_restored | RelayServer decompile `0x14009e480` + matching lookup + `SendCreateMatchingMaze` dispatch | no |
| `GameServer/XRelayServer` | `PartyManager.cpp` | `CPartyManager::SetMaze` | asm_restored | RelayServer decompile `0x140099320` + `SetMazeID` + DB `0x04/0x08` + broadcast `0xF4/0x09` | no |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::SetMaze` | asm_restored | RelayServer decompile `0x140018380` + `SetMazeID` + DB `0x04/0x08` + broadcast `0xFA/0x09` | no |
| `GameServer/XRelayServer` | `RelayControlSocket.cpp` | `CRelayControlSocket::ResCreateMatchingMaze` (party path) | asm_restored | RelayServer decompile `0x14003cfc0` + party/force branch by `byGroupType` + `SetMaze` + `SendCreateMatchingMaze` + DB update | no |
| `GameServer/XRelayServer` | `ForceProcess.cpp` | `CForceProcess::{Parse, ReqForceCreate, ReqForceLeaveMember, ReqForceChangeMaster, ReqForceMazeClear}` | asm_restored | RelayServer decompile `0x140023360 / 0x140023a60 / 0x1400242e0 / 0x140024610 / 0x140026730` + Parse switch for cases 0x01/0x03/0x04/0x19 + worker-0 dispatch | no |
| `GameServer/XRelayServer` | `ForceManager.cpp` | `CForceManager::{ReqForceLeave, ReqChangeMaster, ReqForceMazeClear, ReqDeleteForce, RemoveForceMember}` | asm_restored | RelayServer decompile `0x140016c60 / 0x140017160 / 0x1400181d0 / 0x140017590` + leave/delete/change-master/maze-clear request handlers | no |
| `GameServer/XRelayServer` | `Force.cpp` | `CForce::{ChangeMaster, FindNewMaster, RemoveMember, Kickout, GetForceType, SetForceType}` | asm_restored | RelayServer decompile `CForce` method implementations for force management operations | no |
| `GameServer/XRelayServer` | `LeagueProcess.h` | `CLeagueProcess::Parse` | asm_restored | IDA decompile `0x1400849B0` + PDB symbols `CServerLeagueProcess` + 36 subcommand switch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::{ReqLeagueCreate, ReqLeagueDelete, ReqLeagueApplicant, ReqLeagueInfo, ReqLeagueDelegate, ReqLeagueWithDraw, ReqLeagueKick, ReqLeagueInvite, ReqLeagueInviteAccept, ReqLeagueInviteReject, ReqLeagueBoard, ReqLeagueApplicantAccept, ReqLeagueApplicantReject, ReqLeagueSearch, ReqLeagueNoticeChange, ReqLeagueList, ReqLeagueNameChange, ReqLeagueCardChange, ReqLeaguePositionNameChange, ReqLeagueAuthChange, ReqLeagueMessage, ReqLeagueMemberPositionChange, ReqLeagueMemberLogOut, ReqLeagueDeletePenalty, ReqLeagueWithdrawPenalty, ReqLeagueOpenOrNot, ReqLeagueRecruitNotice, ReqLeagueRecordUpdate, ReqLeagueLevelup, ReqLeagueSkillLearn, ReqLeagueWealth, ReqLeagueMemberInitExp, ReqLeagueSkillPointUpdate, ReqSyncLeagueInfo, ReqLeagueInventoryInfo, ReqLeagueInventoryMove}` | pending | IDA decompile `0x14004E910` Parse switch + PDB handler symbols + bounded stub body | no |
| `GameServer/XRelayServer` | `WorldModeProcess.h` | `CServerWorldModeProcess::Parse` | pending | IDA decompile + PDB symbols + minimal logging stub | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::IsMaze` | asm_restored | IDA decompile `0x1400D22D0` + bounded `mapID/10000==2` range check（缺 `XResourceMgr::GetTB_MAZE_INFO` 完整校验） | no |
| `GameServer/XRelayServer` | `UserPartyInfo.h` | `CUserPartyInfo::{GetRewardState, SetRewardState}` | asm_restored | IDA decompile + bounded `m_byRewardState` 字段返回（非原版布局完全确认） | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::OnUpdate` | verified | IDA 0x14007b740 + 每分钟定时器(CTimeSpan(0,0,1,0)) + 每日9点InitLeaguExp + 遍历UpdateApplyList + LoadGMTLeagueInfo+SendGMTLeagueInfo (修复：1秒→60秒间隔) | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::InitLeaguExp` | verified | IDA 0x14007bb00 + 遍历ResetExp(biTodayInit) + SendDBPacket(nullptr,7,0x40) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateApplyList` | asm_restored | IDA decompile `0x140067820` + 申请者超时1天(86400秒) + DB `(7, 0x20)` + map erase | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpDateLeagueMemberInfo` | asm_restored | IDA decompile `0x140067cb0` + 遍历成员 + GetUser + GetLeagueMemberInfo + SetLeagueMember | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::UpdateLeagueMemberInfo` | verified | IDA 0x14007d270 + 遍历league→LogError(2306)→UpDateLeagueMemberInfo+UpdateSyncCount →nSyncCount=1→SendPacketAll(0xF6,0x59) (修复：添加null错误日志) | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::GetLeagueMemberInfo` | asm_restored | IDA decompile `0x1400d5210` + 填充 ST_LEAGUE_MEMBER_EX + name/ucid/level/class/awaken/profile/channel/worldid/login | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueMember` | verified | IDA 0x14007ca50 + m_bMember=bLoad + 遍历league×member双层循环 + nLeagueID匹配AddMember + byPosition==7时SetSubLeagueMaster | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueApplicant` | verified | IDA 0x14007ccb0 + m_bApplicant=bLoad + 遍历league×applicant双层循环 + nLeagueID匹配AddApplicant | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueBoard` | verified | IDA 0x14007ce90 + m_bBoard=bLoad + 遍历league×board双层循环 + nLeagueID匹配AddBoard | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueRecord` | verified | IDA 0x14007d080 + m_bRecord=bLoad + 遍历league×record双层循环 + nLeagueID匹配LoadRecord + SendInfoToGameServer | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendInfoToGameServer` | verified | IDA 0x14007d200 + 检查5个加载标志(m_bLeague/Member/Applicant/Board/Record)→m_bLoadLeague=1→SetCachingLoad(E_SERVER_CACHING_LOAD_LEAGUE) | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ChangeLeagueMemberName` | verified | IDA 0x140081c70 + find→pLeague→ChangeMemberName | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ChangeLeagueApplicant` | verified | IDA 0x140081d60 + 遍历stApplyList.vecApplyLeuage→find→UpdateApplicantName | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Delegate` | asm_restored | IDA decompile `0x140065910` + position切换(0/100) + 名称获取 + ST_LEAGUE_RECORD(byFlag=12) + UpdateRecord + SendDBLog(15,22) + TODO: byRating缺失 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueDelegate` | verified | IDA 0x14007e630 + find→LogError(2583)→nResult=57016→!bGMDelegate?CheckLeagueDelegate→SendDBPacket(7,0x32)/SendPacket(0xF6,7) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLeagueDelegate` | asm_restored | IDA decompile `0x1400657c0` + IsMaster + GetLeagueMemberPtr + GetMemberCount<2 + TODO: byRating>=2检查缺失 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLeagueCardChange` | decompiled | IDA decompile `0x140065ee0` + HaveSkill(E_SKILL_CARD) + IsMaster | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CardChange` | decompiled | IDA decompile `0x140065fe0` + m_stLeagueInfo.dwLeagueCard = stCard.dwLeagueCard | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::HaveSkill` | decompiled | IDA decompile `0x140066010` + bySkill[eSkill] != 0 + range check E_LEAGUE_SKILL_MAX | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLearnSkill` | decompiled | IDA decompile `0x140066050` + IsMaster + GetTB_LEAGUE_SKILL + 前置技能/等级/技能点/金币检查 + 填充PS_RES_LEAGUE_SKILL | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LearnSkill` | decompiled | IDA decompile `0x140066280` + 扣除金币/技能点 + 更新bySkill + SendLearnSkillToMember + SendDBLog(15,21)+(15,20) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLearnSkillToMember` | decompiled | IDA decompile `0x140068f30` + 广播(0xF6, 0x53) + 序列化PS_RES_LEAGUE_SKILL + m_nSyncCount | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueCardChange` | verified | IDA 0x14007ed40 + find→LogError(2694/2701)→CheckLeagueCardChange→SendDBPacket(7,0x16,nullptr)/SendPacket(0xF6,0x48) | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueCardChange` | verified | IDA 0x14007f090 + find→GetLeagueMemberPtr→SetLeagueInfoForGame→CardChange+UpdateSyncCount+SendPacket(0xF6,0x48)+SendChangeCardToMember+UpdateRecord(byFlag=11) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangeCardToMember` | decompiled | IDA 0x140068cd0 + 广播(0xF6, 0x26) + 序列化PS_REQ_LEAGUE_CARD + m_nSyncCount | no |

| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Levelup` | decompiled | IDA 0x1400666c0 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ApplyLevelup` | decompiled | IDA 0x140066910 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CalculateExp` | decompiled | IDA 0x140066d90 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ApplyWealth` | decompiled | IDA 0x140067390 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLeagueWealthToMember` | decompiled | IDA 0x140068d80 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLevelupToMember` | decompiled | IDA 0x140068e30 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateRecord` | decompiled | IDA 0x140067540 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendRecordToMember` | decompiled | IDA | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeaguePosition` | decompiled | IDA 0x140064520 + 职位名称设置(1/2/3 -> szPosition_1/2/3) + szPrevPositionName保存 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckPositionCount` | decompiled | IDA 0x140065360 + 成员遍历 + 职位计数(副盟主1/管理员3/长老10) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ChangeMemberPosition` | decompiled | IDA 0x140065580 + SetPosition + 副盟主名称更新 + SetLeagueInfoForGame + SendChangePositionToMember | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LeagueMemberUpdate` | decompiled | IDA 0x140064620 + 从 ST_LEAGUE_MEMBER_EX 填充 ST_LEAGUE_MEMBER_UPDATE | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ChangeMemberName` | decompiled | IDA 0x140067ef0 + IsMaster/SetMasterName + SetSubLeagueMaster + SetName + SendChangeMemberName | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangeMemberName` | decompiled | IDA 0x1400682d0 + 广播(0xF6, 0x42) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendNoticeToMember` | decompiled | IDA 0x140068810 + 广播(0xF6, 0x23) | no |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::{ResLeagueWithdrawPenalty, ResLeagueDeletePenalty}` | decompiled | IDA 0x14004e7e0 / 0x14004e690 + 解析 dwUCID+biPenalty + worker-1 dispatch LogOutLeagueMember | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::{ReqLeagueWithdrawPenalty, ReqLeagueDeletePenalty}` | decompiled | IDA 0x140088560 / 0x1400886e0 + 解析 dwUCID+biPenalty + GetUser + SetLeague*Penalty | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::{SetLeagueWithdrawPenalty, SetLeagueDeletePenalty}` | decompiled | IDA 0x140062d40 / 0x140062d60 + m_biLeague*Penalty = biPenalty | no |
| `GameServer/XCore/XServer` | `GreenDamTan_XServerRuntime.cpp` | `XClient::SendErrorMessage` | asm_restored | IDA + XSendPacket(mainCmd, subCmd\|0x80) + errorCode + SendEx | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueCardChange` | asm_restored | IDA 0x140087860 + 读取 PS_REQ_LEAGUE_CARD + dwUCID + PS_RES_STORAGE_INFO + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueMemberPositionChange` | asm_restored | IDA 0x140088310 + 读取 ST_LEAGUE_MEMBER_POSITION + nLeagueID + dwActorID + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeaguePositionNameChange` | asm_restored | IDA 0x140087c70 + 读取 ST_LEAGUE_POSITION_NAME_CHANGE + dwActorID + nLeagueID + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueMessage` | asm_restored | IDA 0x140087e50 + 读取 PS_CHAT_LEAGUE + PS_CHAT_ITEM_LINK_FOR_SERVER + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueApplicant` | asm_restored | IDA 0x140085a30 + 读取 ST_LEAGUE_APPLICANT + 设置 biApplicantDate 为当前时间 + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueNoticeChange` | asm_restored | IDA 0x140084dd0 + 读取 ST_LEAGUE_NOTICE + dwActorID + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueRecruitNotice` | asm_restored | IDA 0x140088a20 + 先读 dwUCID 再读 ST_LEAGUE_RECRUIT_NOTICE + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueMemberInitExp` | asm_restored | IDA 0x140089680 + 读取 nLeagueID + dwUCID + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInventoryInfo` | asm_restored | IDA 0x140089880 + 读取 PS_REQ_LEAGUE_INVEN_INFO + dwReqUCID + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInventoryMove` | asm_restored | IDA 0x140089a10 + 先读 dwReqUCID 再读 PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueDelegate` | asm_restored | IDA 0x140088c60 + 读取 PS_REQ_LEAGUE_DELEGATE + dwUCID + bGMDelegate + worker-1 dispatch | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueMemberInitExp(nLeagueID, dwUCID)` | decompiled | IDA + 新增重载版本接收 nLeagueID + dwUCID 参数 | no |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueWithdrawPenalty` | asm_restored | IDA 0x14004e7e0 + 修正为 GetUser + SetLeagueWithdrawPenalty（原错误实现 LogOutLeagueMember） | no |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueDeletePenalty` | asm_restored | IDA 0x14004e690 + 修正为 GetUser + SetLeagueDeletePenalty（原错误实现 LogOutLeagueMember） | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInviteReject` | asm_restored | IDA 0x140085370 + lambda 0x140085480 改用 SendErrorMessage 替代手动构造错误包 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueLogin` | decompiled | IDA 0x140073970 + find→LoginMember→SendLeagueInfo | verified |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueLevelup` | decompiled | IDA 0x140080010 + find→LogError(2982/2989)→Levelup_Cheat | verified |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueInfo` | decompiled | IDA 0x140073cf0 + find→GetLeagueMemberPtr→GetPosition→GetLeagueInfo/MemberList/ApplicantList/BoardList/RecordList→SetLeagueInfoForGame→SendLeagueInfo | verified |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqApplyLeagueExp` | decompiled | IDA 0x14007f560 + find→LogError(2811/2818)→CalculateExp | verified |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueLevelup` | decompiled | IDA 0x14007f7d0 + find→LogError(2849/2856)→UpdateSyncCount→ApplyLevelup→UpdateRecord(byFlag=8) | verified |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueMemberInitExp(nLeagueID, dwUCID)` | asm_restored | IDA 0x140080430 + 补充完整业务体：查找联赛 + ResetExpInitDate | verified |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInventoryInfo lambda` | asm_restored | IDA 0x1400899c0 + 修正参数传递为 dwReqUCID（原错误传 stReq.dwNpcID） | no |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_LEAGUE_INVEN_INFO::operator<<(XPacket&, ...)` | decompiled | IDA 0x140080860 + 新增序列化函数 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqInviteAccept` | verified | IDA 0x1400758a0 + pTBLeague为null时不发送DB包（修复：DB发送移入成员未满分支内） |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueApplicantAccept` | verified | IDA 0x1400774b0 + GetTB_LEAGUE_INFO查表 + 满员(57018) + TB表null(57019) + 成员null(57007) + 申请者名null(57007) + IsMaster/权限(auth&1)→SendDBPacket(7,0x10) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueApplicantReject` | verified | IDA 0x140077b50 + find→null(1098/1108)→IsMaster/权限(auth&1)→SendDBPacket(7,0x11)或57006错误 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueInvite` | verified | IDA 0x1400786d0 + find→null(1282/1290)→GetTB_LEAGUE_INFO→57018/57021/57008→退会惩罚57037→IsMaster/权限(auth&1)→AddInviteUser→CheckInviteUser(57009)→SendPacket(0xF6,0xC) |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::GetLeagueWithdrawPenalty/GetLeagueDeletePenalty` | decompiled | 新增 getter 方法 | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::GetMatchingID` | decompiled | 新增 getter，返回 GetCID() 作为 matchingID | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendLeagueMemberKick` | asm_restored | IDA 0x14007d690 + 修正签名从(bool,bool)改为(int32_t nErrorCode, int32_t nLeagueID, uint32_t dwUCID, uint32_t dwTargetUCID, ST_LEAGUE_INFO_UPDATE&, uint16_t shLevel, wchar_t* pName) + 修正 SendKickoutToMember/GetName 调用 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueKickout` | asm_restored | IDA 0x1400815d0 + 查找联赛 + GetMemberInfo + 副盟主清空(position==7) + DeleteLeagueMember + UpdateLeagueInfo + SetLeagueID(0) + SendLeagueMemberKick | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResInviteUser` | verified | IDA 0x140075c50 + 查找联赛+GetUser+更新登录状态/频道+AddMember+SetLeagueInfoForGame+DelApplicant+DeleteApplicantList+UpdateLeagueInfo+SetMemberCount+创建加入记录(byFlag=1)+UpdateRecord+UpdateSyncCount+SendLeagueInfo+SendLeagueInviteJoin | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendLeagueInviteJoin` | verified | IDA 0x140076d60 + XSendPacket(0xF6,0x41)<<stMemberEx<<byApplyState<<stInfoEx<<stInfoUpdate<<dwActorID<<stLeagueInfoForGame<<nSyncCount→SendPacketAll |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::AppliCantJoinSucc` | asm_restored | IDA 0x140076330 + const 参数修复 + stMemberInfo 可修改副本 + 成员加入成功处理 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendLeagueApplicantJoin` | asm_restored | IDA 0x140076c60 + 广播(0xF6, 0x37) + 申请者加入通知 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::UpdateLeagueInfo` | asm_restored | IDA 0x14007bc00 + 从空桩实现为完整函数 + GetLeagueInfo + 填充 stUpdate + SetMemberCount |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ApplicantRejectSucc` | asm_restored | IDA 0x140076fa0 + 申请者拒绝成功处理 + DelApplicant + UpdateSyncCount + 发送(0xF6,0x21/0x22) |

| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueAuthChange` | verified | IDA 0x14007a070 + SetLeagueAuth + UpdateSyncCount + 广播(0xF6, 0x28)+stChange+LeagueID+dwActorID+nSyncCount |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueNoticeChange` | verified | IDA 0x1400795e0 + SetLeagueNotice + SendNoticeToMember + UpdateRecord(byFlag=5) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeaguePositionNameChange` | verified | IDA 0x14007a500 + GetLeagueInfo+SetLeaguePosition + 广播(0xF6, 0x27)+dwActorID+stChange+nLeagueID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueMemberPositionChange` | verified | IDA 0x14007b040 + UpdateSyncCount + ChangeMemberPosition + GetMemberPtr/GetName + UpdateRecord(byFlag=4) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueOpenOrNot` | verified | IDA 0x14007db60 + SetLeagueOpenOrNot + m_vecLeagueList管理(open时push_back/close时erase) + 发送(0xF6, 0x45)+stOpen+dwUCID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResCreateLeague` | asm_restored | IDA 0x140079a50 + GetUser + CreateLeague + DeleteApplicantList + 发送(0xF6, 1) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::CreateLeague` | asm_restored | IDA 0x1400797c0 + 填充ST_LEAGUE_INFO和ST_LEAGUE_MEMBER_EX + AddLeague |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueApplicantDelete_TimeOver` | verified | IDA 0x14007b5c0 + find→DelApplicant→XSendPacket(0xF6,0x22)+nLeagueID+dwActorID→SendPacketAll |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::DeleteLeague` | verified | IDA 0x140077220 (DelLeague) + m_mpLeagueList.erase + m_vecLeagueList遍历删除 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueDel` | legacy | 注意：实际删除逻辑在 GameDBSocket lambda 中，此函数为遗留占位 |
| `GameServer/XRelayServer` | `League.h` | `CLeague::GetLeagueID` | decompiled | 内联访问器 m_stLeagueInfo.nLeagueID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResApplyLeagueWealth` | verified | IDA 0x14007f690 + 查找联赛+LogError(2830/2837)+qmemcpy复制+ApplyWealth |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueInventoryInfo` | verified | IDA 0x1400808b0 + 查找联赛 + SendInventoryInfo |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueInventoryMove` | verified | IDA 0x140080cf0 + 查找联赛+UpdateInventorySyncCount+nErrorCode检查+创建记录(byFlag=6存入/7取出)+GetUser name+UpdateRecord+SendInventoryMove |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueRecruitNotice` | verified | IDA 0x14007e480 + 查找联赛+SetLeagueRecruitNotice+GetMemberList+广播(0xF6,0x46)+stNotice+0+dwActorID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueSearch` | verified | IDA 0x14007de20 + GetApplicantList+SendPacket(0xF6,0x18)+dwUCID+psList+stApplyList+SendEx |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::UpdateGMTLeagueInfo` | asm_restored | IDA 0x140082590 + 两阶段更新: ST_LEAGUE_LIST→SetLeagueInfo + ST_LEAGUE_MEMBER_LIST→成员position同步(处理sub-league-master position==7) |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueWealth` lambda | asm_restored | IDA + 修正lambda从ReqApplyLeagueExp改为ResApplyLeagueWealth |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueInventoryMove` lambda | asm_restored | IDA + 修正从Req改为Res + 值语义参数 |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueInventoryInfo` lambda | asm_restored | IDA + 修正从Req改为Res + 反序列化PS_RES_STORAGE_INFO+PS_ITEM_BROACH_LIST+PS_ITEM_SOCKET_LIST+PS_ITEM_PACKAGE_LIST |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_BROACH_LIST operator>>` | decompiled | 新增反序列化器(nCount+ST_ITEM_BROACH数组) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_PACKAGE_LIST operator>>` | decompiled | 新增反序列化器(nCount+PS_ITEM_PACKAGE数组) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueNameChange` | verified | IDA 0x140081a80 (ResLeaugeNameChange) + find→nResult=57005→SetLeagueName→UpdateSyncCount→SendChangeLeagueName |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueNameChange` lambda | asm_restored | IDA + 修正从ReqLeagueNameChange改为ResLeagueNameChange |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueRecord` | asm_restored | IDA 0x14004f0b0 + 修正从ReqLeagueRecordUpdate改为ResLoadLeagueRecord + 直接调用不经过DoJob + 解析bLoadRecord+ST_LEAGUE_RECORD_LIST |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueLevelup` | verified | IDA 0x14007f7d0 + find→LogError(2849/2856)→UpdateSyncCount→ApplyLevelup→UpdateRecord(byFlag=8, nValue3=byLevel) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::LoadLeagueInfo` | asm_restored | IDA 0x140081e90 + 大函数(0x6f1 bytes) + 用户检查 + 联赛已存在时LoginMember/SendLeagueInfo + 新联赛时SetLeagueInfo/AddMember/AddBoard/AddApplicant/LoadRecord + dwUCID==0时ReqLeagueApplicant |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueApplicant` | verified | IDA 0x140073ac0 + find→AddApplicant→SendEx(0xF6,0x19)给申请者→SendPacketAll(0xF6,0x20)广播 |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueCreate` lambda | verified | IDA 0x14004a710 + 完整重写: GetServer+GetUser+SetLeagueID+nErrorCode检查 |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueDelete` lambda | verified | IDA 0x14004cf20 + 完整重写: DeleteLeagueMember+DeleteLeague+SetLeagueID(0)+SendPacket |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeaguePositionNameChange` | verified | IDA 0x14004ddd0 + 第四参数改为dwActorID, 移除nErrorCode |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueMemberPositionChange` | verified | IDA 0x14004e060 + 参数顺序修正: nErrorCode>>stPos>>nLeagueID>>dwActorID>>dwServerID |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueApplicantDelete` | verified | IDA 0x14004e300 + 添加nLeagueID参数 |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueOpenOrNot` | verified | IDA 0x14004e970 + 移除nErrorCode, 第三参数改为dwUCID |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueRecruitNotice` | verified | IDA 0x14004eb50 + 移除nErrorCode, 第三参数改为dwUCID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResCreateLeague` | verified | IDA 0x140079a50 + 添加dwLeagueCard赋值, nAuth循环改为8 |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResGMTLeagueInfo` | verified | IDA 0x140051820 + 类型从PS_GMT_LEAGUE_UPDATE_LIST改为ST_LEAGUE_LIST+ST_LEAGUE_MEMBER_LIST, 调用UpdateGMTLeagueInfo |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueCardChange` | verified | IDA 0x14004f3b0 + 修复参数顺序(psCard>>dwUCID>>dwServerID>>vecUpdateItem>>nErrorCode), 添加dwServerID和GetServer |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Levelup` | verified | IDA 0x140066590 + 等级上限10 + 技能点累加 + PS_AUTO_SKILL自动学习 + DB(7,0x34) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ApplyLevelup` | verified | IDA 0x140066910 + byRating/bySkillPoint更新 + SendLevelupToMember + SendDBLog(15,17)+(15,20) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CalculateExp` | verified | IDA 0x140066d90 + 每日经验上限/成员200上限/金币999999999上限 + Levelup循环 + DB(7,0x33) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ApplyWealth` | verified | IDA 0x140067390 + AddExp + SendLeagueWealthToMember + SendDBLog(15,18经验)+(15,19金币) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateApplyList` | verified | IDA 0x140067820 + m_mpLeagueApplicant遍历 + CTimeSpan(86400)超时 + DB(7,0x20) + erase; 注意:源码用m_mpApplicant命名,IDA PDB为m_mpLeagueApplicant |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueWealth` | verified | IDA 0x14004f7e0 + PS_LEAGUE_WEALTH_FOR_SERVER反序列化 + DoJob→ResApplyLeagueWealth |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueLevelup` | verified | IDA 0x14004f950 + nLeagueID>>byLevel>>bySkillPoint>>psAutoSkill>>dwUCID + DoJob→ResLeagueLevelup |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueInfo` | verified | IDA 0x140050c70 + stLeagueInfo>>stMemberList>>stBoardList>>stApplicantList>>stRecordList>>psDBLoadInfo>>nDBErrorCode + DoJob→LoadLeagueInfo |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::Parse` | verified | IDA 0x1400849b0 + 36 subcommand switch完全匹配IDA |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::DBLeagueParse` | verified | IDA 0x140049cf0 + 34 case entry完全匹配IDA; 注意:0x37/0x39 IDA PDB为Req前缀,源码用Res前缀 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetDelName` | verified | IDA 0x1400648e0 + GetLeagueMemberPtr→GetName→wcscpy; 修复:从空占位改为完整实现 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::DelMember` | verified | IDA 0x1400732a0 + m_mpLeagueMember.erase(dwUCID) |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueApplicant` | verified | IDA 0x14004a3e0 + stApplicant>>nServerID + DoJob→ResLeagueApplicant(nullptr, stApplicant) |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueApplicantAccept` | verified | IDA 0x14004d420 + stAccept>>nServerID>>stMemberEx>>dwActorID + DoJob→AppliCantJoinSucc |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueApplicantReject` | verified | IDA 0x14004d750 + stReject>>nServerID>>nErrorCode + DoJob→ApplicantRejectSucc |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueBoard` | verified | IDA 0x14004d120 + nErrorCode>>stBoard>>dwServerID>>nLeagueID>>dwActorID + DoJob→ResLeagueBoard |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueBoard` | verified | IDA 0x140075670 + find→AddBoard→SetMemberBoardLimit→SendPacket(0xF6,0x14)→SendPacketAll |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueDelegate` | verified | IDA 0x14007eab0 + find→57016/57044错误→Delegate→UpdateSyncCount→SendDelegateToMember或SendPacket(0xF6,7) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueKickout` | verified | IDA 0x1400815d0 + find→GetMemberInfo→byPosition==7清副盟主→DeleteLeagueMember→UpdateLeagueInfo→SetLeagueID(0)→SendLeagueMemberKick |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueWithdraw` | verified | IDA 0x140074350 + find→GetMemberInfo→byPosition==7清副盟主→DeleteLeagueMember→UpdateLeagueInfo→SetLeagueID(0)+SetLeagueWithdrawPenalty→SendLeagueMemberWithdraw |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::DeleteLeagueMember` | verified | IDA 0x1400785d0 + find→GetDelName→DelMember |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueCreate` | verified | IDA 0x140085610 + PS_LEAGUE_CREATE_FOR_SERVER>>GetClientPtr+DoJob→ReqLeagueCreate |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueDelete` | verified | IDA 0x1400858a0 + dwActorID>>nLeagueID>>biPenalty+GetClientPtr+DoJob→ReqLeagueDel |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueApplicant` | verified | IDA 0x140085a30 + stApplicant>>GetClientPtr+biApplicantDate=GetTickCount+DoJob→ReqLeagueApplicant |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueWithDraw` | verified | IDA 0x140085fc0 + dwActorID>>nLeagueID>>biPenalty+GetClientPtr+DoJob→ReqLeagueWithDraw |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueKick` | verified | IDA 0x1400861d0 + dwActorID>>dwTargetID>>nLeagueID+GetClientPtr+DoJob→ReqLeagueKick |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInvite` | verified | IDA 0x1400866f0 + ST_REQ_LEAGUE_INVITE>>GetClientPtr+DoJob→lambda |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInviteAccept` | verified | IDA 0x140086a50 + stAccept>>biJoinDate+GetClientPtr+DoJob→ReqInviteAccept |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueBoard` | verified | IDA 0x140086c50 + stBoard>>dwActorID>>nLeagueID+GetClientPtr+DoJob→ReqLeagueBoard |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueApplicantAccept` | verified | IDA 0x140086f00 + stAccept>>dwActorID+GetClientPtr+DoJob→ReqLeagueApplicantAccept |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueApplicantReject` | verified | IDA 0x140087120 + stReject>>GetClientPtr+DoJob→ReqLeagueApplicantReject |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueList` | verified | IDA 0x140087270 + dwUCID>>byType+GetClientPtr+DoJob→SendDBGame(7,0x41) |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueNameChange` | verified | IDA 0x1400874a0 + PS_LEAGUE_NAME_CHANGE_SERVER>>GetClientPtr+DoJob→ReqLeagueNameChange |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueCardChange` | verified | IDA 0x140087860 + PS_REQ_LEAGUE_CARD>>dwUCID>>PS_RES_STORAGE_INFO+GetClientPtr+DoJob→ReqLeagueCardChange |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeaguePositionNameChange` | verified | IDA 0x140087bd0 + stChange>>dwActorID>>nLeagueID+GetClientPtr+DoJob→ReqLeaguePositionNameChange |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueAuthChange` | verified | IDA 0x140086490 + ST_LEAGUE_AUTH_CHANGE>>nLeagueID>>dwActorID+GetClientPtr+DoJob→ReqLeagueChangeAuth |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueMemberLogOut` | verified | IDA 0x140088420 + nLeagueID>>dwActorID>>biLogoutDate+DoJob→LogOutLeagueMember |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLeagueDelegate` | verified | IDA 0x1400657c0 + IsMaster→57015; GetLeagueMemberPtr→57007; shMemberCount<2→57044; byRating<2→57044; return 0 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Delegate` | verified | IDA 0x140065910 + SetPosition(0/0x64)+GetName+dwMasterUCID+szMasterName+UpdateRecord(byFlag=12)+SendDBLog(15,22) |
| `GameServer/XRelayServer` | `LeagueMember.cpp` | `CLeagueMember::GetLeagueMember` | verified | IDA 0x140064140 + memcpy(m_stMember→stMember) |
| `GameServer/XRelayServer` | `LeagueMember.cpp` | `CLeagueMember::AddExp` | verified | IDA 0x140064490 + biLeagueExp+=shExp; if(biLeagueExp>200)biLeagueExp=200 |
| `GameServer/XRelayServer` | `LeagueMember.cpp` | `CLeagueMember::ResetExp` | verified | IDA 0x140064420 + biLeagueExp=0 |
| `GameServer/XRelayServer` | `LeagueMember.cpp` | `CLeagueMember::GetName` | verified | IDA 0x1400643f0 + wcscpy(szName, m_stMember.szName) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpDateLeagueMemberInfo` | verified | IDA 0x140067cb0 + iterate m_mpLeagueMember→GetLeagueMember→GetUser→GetLeagueMemberInfo→SetLeagueMember |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangeLeagueName` | verified | IDA 0x140068390 + nSysnCount=m_nSyncCount+XSendPacket(0xF6,0x25)+serialize+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangeMemberName` | verified | IDA 0x1400682d0 + XSendPacket(0xF6,0x42)+serialize+SendPacketAll |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueCardChange` | verified | IDA 0x14007f090 + find→GetLeagueMemberPtr→SetLeagueInfoForGame→CardChange+UpdateSyncCount+SendPacket(0xF6,0x48)+SendChangeCardToMember+UpdateRecord(byFlag=11) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueSkillLearn` | verified | IDA 0x14007fd30 + GetUser→find→SetLockLeague(0)+UpdateSyncCount+LearnSkill+UpdateRecord(byFlag=9/10) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueInventoryInfo` | verified | IDA 0x1400808b0 + find→SendInventoryInfo(dwReqUCID, storage, broach, socket, package) |
| `GameServer/XRelayServer` | `PSServer.h` | `operator<<(XPacket&, PS_LEAGUE_NAME_CHANGE_SERVER)` | verified | IDA 0x140068390 序列化对齐 + dwUCID+nLeagueID+psUpdateItemList+szLeagueName+dwServerID+nSysnCount+nResult |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendWithdrawToMember` | verified | IDA 0x140068420 + XSendPacket(0xF6,8)+nLeagueID+dwUCID+biPenalty+stUpdate+m_nSyncCount+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendKickoutToMember` | verified | IDA 0x140068510 + XSendPacket(0xF6,9)+nErrorCode+nLeagueID+dwReqUCID+dwTargetUCID+stUpdate+shLevel+m_nSyncCount+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangePositionToMember` | verified | IDA 0x140068630 + XSendPacket(0xF6,0x37)+stPosition+nLeagueID+dwReqUCID+byPrevPosition+stLeagueInfoForGame+m_nSyncCount+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendDelegateToMember` | verified | IDA 0x140068730 + XSendPacket(0xF6,7)+stRes+dwReqUCID+dwDelegatedUCID+m_nSyncCount+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendNoticeToMember` | verified | IDA 0x140068810 + XSendPacket(0xF6,0x23)+stNotice+dwUCID+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangeCardToMember` | verified | IDA 0x140068cd0 + XSendPacket(0xF6,0x26)+stCard+m_nSyncCount+SendPacketAll (dwActorID参数不序列化) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLeagueWealthToMember` | verified | IDA 0x140068d80 + XSendPacket(0xF6,0x55)+dwActorID+stUpdate+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLevelupToMember` | verified | IDA 0x140068e30 + XSendPacket(0xF6,0x51)+nLeagueID+byLevel+bySkillPoint+stSkill+m_nSyncCount+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLearnSkillToMember` | verified | IDA 0x140068f30 + XSendPacket(0xF6,0x53)+stSkill+m_nSyncCount+SendPacketAll |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendChangeApplicantName` | verified | IDA 0x140068240 + XSendPacket(0xF6,0x20)+stApplicant+SendPacketAll (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::AddMember` | verified | IDA 0x140064720 + find→if exists: SetLeagueMember(update) → if not: new CLeagueMember + SetLeagueMember + insert (修复：原代码只创建不更新) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LoginMember` | verified | IDA 0x140064a00 + find→判空→LogDebug "LoginMember Member NULL(123)"→Login() (修复：添加LogDebug调用) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LogOutMember` | verified | IDA 0x140064af0 + find→LogOut() + SetPlayDate(biLogoutDate) (修复：添加SetPlayDate调用，参数名改为biLogoutDate) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::IsMember` | verified | IDA 0x140064d00 + find != end |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CompareSyncCount` | verified | IDA 0x140069380 + return m_nSyncCount != nCount (修复：原代码返回==，改为!=) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetApplicantList` | verified | IDA 0x140064f20 + iterate m_mpApplicant → push_back(it->second) (修复：从只设nCount改为完整遍历) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetBoardList` | verified | IDA 0x140065080 + copy m_deqBoard→最多50条→push_back (修复：从只设nCount改为完整遍历) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetRecordList` | verified | IDA 0x140065f30 + copy m_deqRecord→最多100条→push_back (修复：从只设nCount改为完整遍历) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLeagueInfo` | verified | IDA 0x1400688c0 + GetLeagueMemberPtr→GetLeagueMember→LeagueMemberUpdate→GetMemberList→GetApplicantList→GetBoardList→GetRecordList→SetLeagueInfoForGame→XSendPacket(0xF6,3)+bLogin+stUpdate+stInfo+stMemberList+stApplicant+stBoard+byState+stInfoEx+stRecordList+stLeagueInfoForGame+m_nSyncCount+SendPacketAll (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendSyncLeagueInfo` | verified | IDA 0x1400693c0 + CompareSyncCount→stSync.bSync=1→GetLeagueMemberPtr→GetLeagueMember→LeagueMemberUpdate→GetApplicantList→GetBoardList→GetMemberList→GetRecordList→SetLeagueInfoForGame→GetUser→LogDebug if NULL→XSendPacket(0xF6,0x58)+byState+stInfo+stMemberList+stApplicant+stBoard+stRecordList+stLeagueInfoForGame+stSync→SendPacket (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendInventoryInfo` | verified | IDA 0x1400697c0 + XSendPacket(0xF6,0x60)+dwActorID+stStorage+stBroach+stSocket+stPackage+m_nInventorySyncCount+SendPacketAll (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendInventoryMove` | verified | IDA 0x140069900 + stMove.nInventorySync=m_nInventorySyncCount→XSendPacket(0xF6,0x61)+dwActorID+stMove+SendPacketAll (修复：从占位改为完整实现，参数改为非const) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Clear` | verified | IDA 0x1400643c0 + 仅重置 m_nInventorySyncCount=0 + m_nSyncCount=0 (修复：原代码清空所有容器) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueInfo` | verified | IDA 0x140064220 + qmemcpy 0x800 字节 = m_stLeagueInfo赋值 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetMemberList` | verified | IDA 0x140065130 + 遍历 m_mpLeagueMember→GetLeagueMember→push_back (修复：从只设nCount改为完整遍历) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetLeagueMemberPtr` | verified | IDA 0x140065250 + find→返回shared_ptr拷贝/nullptr |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateSyncCount` | verified | IDA 0x140067c70 + ++m_nSyncCount |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateInventorySyncCount` | verified | IDA 0x140067c90 + ++m_nInventorySyncCount |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendRecordToMember` | verified | IDA 0x140068c40 + XSendPacket(0xF6,0x47)+stRecord+SendPacketAll (修复：子命令从0x25改为0x47) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::AddApplicant` | verified | IDA 0x140064ba0 + m_mpApplicant.insert({dwActorID,stApplicant}) (修复：从operator[]改为insert) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::DelApplicant` | verified | IDA 0x140064c20 + find→erase→return 0/1 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::AddBoard` | verified | IDA 0x140064ca0 + size>=50则pop_front→push_back (修复：添加大小限制) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LeagueMemberUpdate` | verified | IDA 0x140064620 + wcscpy_s(szName,21)+字段赋值 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Levelup` | verified | IDA 0x140066590 + 检查等级上限→GetTB_LEAGUE_INFO→遍历TB_LEAGUE_SKILL自动学习→UpdateSyncCount→SendDBPacket(7,0x34) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueNotice` | verified | IDA 0x1400644d0 + wcscpy_s(szNotice,801)+biNoticeDate=biEnrollDate (修复：从m_stNotice改为m_stLeagueInfo) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueRecruitNotice` | verified | IDA 0x140065530 + wcscpy_s(szRecruitNotice,51)+biRecruitNoticeDate=biRegDate (修复：从m_stRecruitNotice改为m_stLeagueInfo) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueNoticeDate` | verified | IDA 0x1400732f0 + m_stLeagueInfo.biNoticeDate=0 (修复：从空实现改为实际赋值) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueOpenOrNot` | verified | IDA 0x140073310 + m_stLeagueInfo.bOpen=bOpen |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetLeagueInfo` | verified | IDA 0x140073360 + qmemcpy 0x800字节 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeaguePosition` | verified | IDA 0x140064520 + 三组独立if判断+wcscpy_s |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueAuth` | verified | IDA 0x140065310 + 循环复制nAuth[9] |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::LogOutLeagueMember` | verified | IDA 0x14007b360 + find→LogOutMember→GetLeagueMemberPtr→GetLeagueMember→LeagueMemberUpdate→SendPacketAll(0xF6,0x39) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueInfo` | verified | IDA 0x140073cf0 + find→GetLeagueMemberPtr→GetPosition→GetLeagueInfo→GetMemberList→GetApplicantList→GetBoardList→GetRecordList→SetLeagueInfoForGame→SendLeagueInfo |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::IsMaster` | verified | IDA 0x1400646f0 + m_stLeagueInfo.dwMasterUCID==dwUCID |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::GetDelName` | verified | IDA 0x1400648e0 + GetLeagueMemberPtr→GetName→wcscpy |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckExistApplicant` | verified | IDA 0x140064dd0 + find != end |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckApplicantCount` | verified | IDA 0x140064d70 + m_mpApplicant.size()<50 (修复：从return true改为实际检查) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetMemberBoardLimit` | verified | IDA 0x140064e40 + find→null check+LogDebug→SetEnrollBoardDate |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueOpenOrNot` | verified | IDA 0x14007d960 + find→IsMaster→SendDBPacket(7,0x27)+ServerID+dwUCID / else SendErrorMessage(0xF6,0x45,0xDEB7) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLearnSkill` | verified | IDA 0x140066050 + IsMaster→GetTB_LEAGUE_SKILL→条件检查(57015/57046/57047/57048/57049/57050)→填充响应→return 0 |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LearnSkill` | verified | IDA 0x140066280 + 更新技能/金钱/技能点→SendLearnSkillToMember→GetUser→SendDBLog(15,21)+SendDBLog(15,20) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueNotice` | verified | IDA 0x1400644d0 + wcscpy_s(szNotice,801)+biNoticeDate=biEnrollDate (修复：从m_stNotice改为m_stLeagueInfo) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueRecruitNotice` | verified | IDA 0x140065530 + wcscpy_s(szRecruitNotice,51)+biRecruitNoticeDate=biRegDate (修复：从m_stRecruitNotice改为m_stLeagueInfo) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueNoticeDate` | verified | IDA 0x1400732f0 + m_stLeagueInfo.biNoticeDate=0 (修复：从空实现改为实际赋值) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ResetExp` | verified | IDA 0x140067640 + biInitDate+nLimitExp=0+遍历成员ResetExp |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ResetExpInitDate` | verified | IDA 0x140067730 + SendDBPacket(7,0x36)+GetLeagueMemberPtr+ResetExp (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SetLeagueInfoForGame` | verified | IDA 0x140067a00 + byRating+byPosition+dwMasterUCID+bySkill[8]+nAuth[9] |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateSkillPoint_Cheat` | verified | IDA 0x140067ac0 + 更新技能点+复制技能+SendDBPacket(7,0x34)+SendLevelupToMember (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Levelup_Cheat` | verified | IDA 0x140068fe0 + 等级检查+技能点累加+自动学习技能+SendDBPacket(7,0x34) (修复：从占位改为完整实现) |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::ChangeMemberPosition` | verified | IDA 0x140065580 + GetPosition+SetPosition+SetSubLeagueMaster+SetLeagueInfoForGame+SendChangePositionToMember |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueBoard` | verified | IDA 0x1400751d0 + find+GetLeagueMemberPtr+biBoardLimitTime检查+冷却时间1800秒+SendDBPacket(7,5) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueNoticeChange` | verified | IDA 0x140078ff0 + find+公告冷却(57029)+无权限(57006)+IsMaster分支+SendDBPacket(7,0x14) (修复：添加完整错误处理) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueRecordUpdate` | verified | IDA 0x140080330 + find+LogError(3031/3038)+UpdateRecord |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueNameChange` | verified | IDA 0x1400818d0 + find+nResult=57005+LogError(3434/3443/3452)+IsMaster检查+nResult=57015 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueApplicant` | verified | IDA 0x140078210 + find→null(1207)→CheckApplicantCount(57023)→CheckExistApplicant(57026)→SendDBPacket(7,9) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueWithDraw` | verified | IDA 0x140074060 + find→IsMember→IsMaster(master不能退)+SendErrorMessage(0xF6,8,0xDEC4)→SendDBPacket(7,6) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueKick` | verified | IDA 0x140074620 + find→IsMaster(target)不能踢(0xDEAE)+踢自己(0xDECA)+IsMaster(authority)/权限(auth&2)+同级不能踢(0xDEAE)+SendDBPacket(7,0xB) (修复：错误码0xC73E→0xDECA, 0xDECA→0xDEAE) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueDel` | verified | IDA 0x140074d60 + find→57016→null(394)→IsMaster(57015)→GetMemberCount>1(57018)→SendDBPacket(7,1) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueChangeAuth` | verified | IDA 0x140079de0 + find→SendError(0xDEB8)→null(1584)→IsMaster(否则0xDEAE)→SendDBPacket(7,0x18)<<stAuth<<nLeagueID<<ServerID<<dwActorID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueMemberPositionChange` | verified | IDA 0x14007a8d0 + find(57016)→null(1740/1746)→改自己(57036)→CheckPositionCount(57030)→IsMaster或权限(0x40)→SendDBPacket(7,0x19)<<stPos<<dwActorID<<nLeagueID<<ServerID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeaguePositionNameChange` | verified | IDA 0x14007a290 + find→SendError(0xDEB8)→null(1648)→IsMaster→SendDBPacket(7,0x17) else SendError(0xDEAE) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueRecruitNotice` | verified | IDA 0x14007df50 + find→GetLeagueMemberPtr→冷却检查(57040,1800s)→IsMaster或权限(0x100)→SendDBPacket(7,0x28) else SendError(57006) (修复：权限位0x10→0x100, 错误码57015→57006) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueSkillLearn` | verified | IDA 0x14007f9ce + GetUser→find→LogError(2883/2893)→SetLockLeague(0)→CheckLearnSkill→SendDBPacket(7,0x35,nullptr)/SendPacket(0xF6,0x53) (修复：SendDBPacket第一个参数改为nullptr) |


