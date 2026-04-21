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
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::OnUpdate` | asm_restored | IDA decompile `0x14007b740` + 每秒定时器 + 每日9点 `InitLeaguExp` + 遍历 `UpdateApplyList` | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::InitLeaguExp` | asm_restored | IDA decompile `0x14007bb00` + 遍历 `ResetExp` + DB `(7, 0x40)` | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpdateApplyList` | asm_restored | IDA decompile `0x140067820` + 申请者超时1天(86400秒) + DB `(7, 0x20)` + map erase | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::UpDateLeagueMemberInfo` | asm_restored | IDA decompile `0x140067cb0` + 遍历成员 + GetUser + GetLeagueMemberInfo + SetLeagueMember | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::UpdateLeagueMemberInfo` | asm_restored | IDA decompile `0x14007d270` + 遍历 league + UpDateLeagueMemberInfo + UpdateSyncCount + 广播 `(0xF6, 0x59)` | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::GetLeagueMemberInfo` | asm_restored | IDA decompile `0x1400d5210` + 填充 ST_LEAGUE_MEMBER_EX + name/ucid/level/class/awaken/profile/channel/worldid/login | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueMember` | asm_restored | IDA decompile `0x14007ca50` + 遍历联赛 + AddMember + 副会长(position=7)设置 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueApplicant` | asm_restored | IDA decompile `0x14007ccb0` + 遍历联赛 + AddApplicant | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueBoard` | asm_restored | IDA decompile `0x14007ce90` + 遍历联赛 + AddBoard | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueRecord` | asm_restored | IDA decompile `0x14007d080` + 遍历联赛 + LoadRecord + SendInfoToGameServer | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendInfoToGameServer` | asm_restored | IDA decompile `0x14007d200` + 检查5个加载标志 + SetCachingLoad(2u) | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ChangeLeagueMemberName` | asm_restored | IDA decompile `0x140081c70` + 查找联赛 + CLeague::ChangeMemberName | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ChangeLeagueApplicant` | asm_restored | IDA decompile `0x140081d60` + 遍历申请者联赛列表 + CLeague::UpdateApplicantName | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::Delegate` | asm_restored | IDA decompile `0x140065910` + position切换(0/100) + 名称获取 + ST_LEAGUE_RECORD(byFlag=12) + UpdateRecord + SendDBLog(15,22) + TODO: byRating缺失 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueDelegate` | asm_restored | IDA + 查找联赛 + CheckLeagueDelegate + DB包(7,0x32) + 错误响应(0xF6,7) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLeagueDelegate` | asm_restored | IDA decompile `0x1400657c0` + IsMaster + GetLeagueMemberPtr + GetMemberCount<2 + TODO: byRating>=2检查缺失 | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLeagueCardChange` | decompiled | IDA decompile `0x140065ee0` + HaveSkill(E_SKILL_CARD) + IsMaster | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CardChange` | decompiled | IDA decompile `0x140065fe0` + m_stLeagueInfo.dwLeagueCard = stCard.dwLeagueCard | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::HaveSkill` | decompiled | IDA decompile `0x140066010` + bySkill[eSkill] != 0 + range check E_LEAGUE_SKILL_MAX | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::CheckLearnSkill` | decompiled | IDA decompile `0x140066050` + IsMaster + GetTB_LEAGUE_SKILL + 前置技能/等级/技能点/金币检查 + 填充PS_RES_LEAGUE_SKILL | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::LearnSkill` | decompiled | IDA decompile `0x140066280` + 扣除金币/技能点 + 更新bySkill + SendLearnSkillToMember + SendDBLog(15,21)+(15,20) | no |
| `GameServer/XRelayServer` | `League.cpp` | `CLeague::SendLearnSkillToMember` | decompiled | IDA decompile `0x140068f30` + 广播(0xF6, 0x53) + 序列化PS_RES_LEAGUE_SKILL + m_nSyncCount | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueCardChange` | decompiled | IDA decompile `0x14007ed40` + 查找联赛 + CheckLeagueCardChange + DB(7,0x16) / 错误(0xF6,0x48) | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueCardChange` | decompiled | IDA decompile `0x14007f090` + GetLeagueMemberPtr + SetLeagueInfoForGame + CardChange + UpdateRecord(byFlag=16) + UpdateSyncCount + SendChangeCardToMember + 响应(0xF6,0x48) | no |
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
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueMemberInitExp(nLeagueID, dwUCID)` | asm_restored | IDA 0x140080430 + 补充完整业务体：查找联赛 + ResetExpInitDate | no |
| `GameServer/XRelayServer` | `LeagueProcess.cpp` | `CLeagueProcess::ReqLeagueInventoryInfo lambda` | asm_restored | IDA 0x1400899c0 + 修正参数传递为 dwReqUCID（原错误传 stReq.dwNpcID） | no |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_LEAGUE_INVEN_INFO::operator<<(XPacket&, ...)` | decompiled | IDA 0x140080860 + 新增序列化函数 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqInviteAccept` | asm_restored | IDA 0x1400758a0 + 补充成员数量检查 GetMemberCount vs TB_LEAGUE_INFO.League_Member | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueApplicantAccept` | asm_restored | IDA 0x1400774b0 + 添加 GetTB_LEAGUE_INFO 查表 + 满员检查(57018) + TB表null检查(57019) + wcscpy_s 申请者名 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueApplicantReject` | asm_restored | IDA 0x140077b50 + 修正签名(移除dwActorID) + 改用stReject.dwUCID | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ReqLeagueInvite` | asm_restored | IDA 0x1400786d0 + 添加TB表检查 + 满员检查(57018) + 退会惩罚检查(57037) + 重复邀请检查(57009) | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::GetLeagueWithdrawPenalty/GetLeagueDeletePenalty` | decompiled | 新增 getter 方法 | no |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject::GetMatchingID` | decompiled | 新增 getter，返回 GetCID() 作为 matchingID | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendLeagueMemberKick` | asm_restored | IDA 0x14007d690 + 修正签名从(bool,bool)改为(int32_t nErrorCode, int32_t nLeagueID, uint32_t dwUCID, uint32_t dwTargetUCID, ST_LEAGUE_INFO_UPDATE&, uint16_t shLevel, wchar_t* pName) + 修正 SendKickoutToMember/GetName 调用 | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueKickout` | asm_restored | IDA 0x1400815d0 + 查找联赛 + GetMemberInfo + 副盟主清空(position==7) + DeleteLeagueMember + UpdateLeagueInfo + SetLeagueID(0) + SendLeagueMemberKick | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResInviteUser` | asm_restored | IDA 0x140075c70 + 查找联赛 + 更新用户登录状态/频道 + AddMember + SetLeagueInfoForGame + DelApplicant + DeleteApplicantList + UpdateLeagueInfo + SetMemberCount + 创建加入记录(byFlag=1) + UpdateRecord + UpdateSyncCount + SendLeagueInfo + SendLeagueInviteJoin | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendLeagueInviteJoin` | asm_restored | IDA 0x140076d60 + 广播(0xF6, 0x41) + 序列化 stMemberEx/byApplyState/stInfoEx/stInfoUpdate/dwActorID/stLeagueInfoForGame/nSyncCount | no |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::AppliCantJoinSucc` | asm_restored | IDA 0x140076330 + const 参数修复 + stMemberInfo 可修改副本 + 成员加入成功处理 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::SendLeagueApplicantJoin` | asm_restored | IDA 0x140076c60 + 广播(0xF6, 0x37) + 申请者加入通知 |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::UpdateLeagueInfo` | asm_restored | IDA 0x14007bc00 + 从空桩实现为完整函数 + GetLeagueInfo + 填充 stUpdate + SetMemberCount |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ApplicantRejectSucc` | asm_restored | IDA 0x140076fa0 + 申请者拒绝成功处理 + DelApplicant + UpdateSyncCount + 发送(0xF6,0x21/0x22) |

| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueAuthChange` | asm_restored | IDA 0x14007a070 + SetLeagueAuth + UpdateSyncCount + 广播(0xF6, 0x28) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueNoticeChange` | asm_restored | IDA 0x1400795e0 + SetLeagueNotice + SendNoticeToMember + UpdateRecord(byFlag=5) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeaguePositionNameChange` | asm_restored | IDA 0x14007a500 + SetLeaguePosition + 广播(0xF6, 0x27) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueMemberPositionChange` | asm_restored | IDA 0x14007b040 + UpdateSyncCount + ChangeMemberPosition + GetMemberPtr/GetName + UpdateRecord(byFlag=4) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueOpenOrNot` | asm_restored | IDA 0x14007db60 + SetLeagueOpenOrNot + m_vecLeagueList管理(open时push_back/close时erase) + 发送(0xF6, 0x45) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResCreateLeague` | asm_restored | IDA 0x140079a50 + GetUser + CreateLeague + DeleteApplicantList + 发送(0xF6, 1) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::CreateLeague` | asm_restored | IDA 0x1400797c0 + 填充ST_LEAGUE_INFO和ST_LEAGUE_MEMBER_EX + AddLeague |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueApplicantDelete_TimeOver` | asm_restored | IDA 0x14007b5c0 + DelApplicant + 广播(0xF6, 0x22) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueDel` | asm_restored | IDA + DeleteLeague + 发送(0xF6, 2) + 广播(0xF6, 0x3F) |
| `GameServer/XRelayServer` | `League.h` | `CLeague::GetLeagueID` | decompiled | 内联访问器 m_stLeagueInfo.nLeagueID |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResApplyLeagueWealth` | asm_restored | IDA 0x14007f690 + 查找联赛 + 复制财富结构 + ApplyWealth |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueInventoryInfo` | asm_restored | IDA 0x1400808b0 + 查找联赛 + SendInventoryInfo |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueInventoryMove` | asm_restored | IDA 0x140080cf0 + 查找联赛 + UpdateInventorySyncCount + 创建记录(byFlag=6存/7取) + SendInventoryMove |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::UpdateGMTLeagueInfo` | asm_restored | IDA 0x140082590 + 两阶段更新: ST_LEAGUE_LIST→SetLeagueInfo + ST_LEAGUE_MEMBER_LIST→成员position同步(处理sub-league-master position==7) |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueWealth` lambda | asm_restored | IDA + 修正lambda从ReqApplyLeagueExp改为ResApplyLeagueWealth |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueInventoryMove` lambda | asm_restored | IDA + 修正从Req改为Res + 值语义参数 |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueInventoryInfo` lambda | asm_restored | IDA + 修正从Req改为Res + 反序列化PS_RES_STORAGE_INFO+PS_ITEM_BROACH_LIST+PS_ITEM_SOCKET_LIST+PS_ITEM_PACKAGE_LIST |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_BROACH_LIST operator>>` | decompiled | 新增反序列化器(nCount+ST_ITEM_BROACH数组) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_PACKAGE_LIST operator>>` | decompiled | 新增反序列化器(nCount+PS_ITEM_PACKAGE数组) |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueNameChange` | asm_restored | IDA 0x140081a80 (ResLeaugeNameChange) + SetLeagueName + UpdateSyncCount + SendChangeLeagueName |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueNameChange` lambda | asm_restored | IDA + 修正从ReqLeagueNameChange改为ResLeagueNameChange |
| `GameServer/XRelayServer` | `GameDBSocket.cpp` | `CGameDBSocket::ResLeagueRecord` | asm_restored | IDA 0x14004f0b0 + 修正从ReqLeagueRecordUpdate改为ResLoadLeagueRecord + 直接调用不经过DoJob + 解析bLoadRecord+ST_LEAGUE_RECORD_LIST |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLoadLeagueRecord` | asm_restored | IDA 0x14007d080 + m_bRecord赋值 + 遍历联赛匹配nLeagueID调用LoadRecord + SendInfoToGameServer |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueLevelup` | asm_restored | IDA 0x14007f7d0 + UpdateSyncCount + ApplyLevelup + 创建记录(byFlag=8) + UpdateRecord |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::LoadLeagueInfo` | asm_restored | IDA 0x140081e90 + 大函数(0x6f1 bytes) + 用户检查 + 联赛已存在时LoginMember/SendLeagueInfo + 新联赛时SetLeagueInfo/AddMember/AddBoard/AddApplicant/LoadRecord + dwUCID==0时ReqLeagueApplicant |
| `GameServer/XRelayServer` | `LeagueManager.cpp` | `CLeagueManager::ResLeagueApplicant` | asm_restored | IDA 0x140073ac0 + AddApplicant + 发送(0xF6,0x19)给申请者 + 广播(0xF6,0x20)通知成员 |

