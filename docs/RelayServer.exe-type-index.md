# RelayServer.exe 类型索引

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| `GameServer/XRelayServer` | `RelayControlSocket.h` | `XRelaySocket::RelayInfo` | 4 | verified | 现有 LoginServer 类比 + 共享协议结构 |
| `GameServer/XRelayServer` | `RelayControlSocket.h` | `XRelaySocket` | 5+核心字段 | asm_restored | export-for-ai `OnStartThread/ServerProcess` + PDB symbols |
| `GameServer/XRelayServer` | `RelayControlSocket.h` | `CRelayControlSocket` | 1（最小调度接口） | decompiled | IDA `SetMyInfo/ServerProcessEx` + 四个子处理器符号 |
| `GameServer/XRelayServer` | `RelayServer.h` | `XRelayServer` | 10+核心字段（补回 `CRelayPartyMatchingConfig` 接线） | asm_restored | export-for-ai + PDB type chain + current recruit/manager hooks |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject` | 12+query-only wrappers | asm_restored | export-for-ai `1400B0A90.c` + `SendPacket/CheckGameOption/GetFriendUCID/IsFriendList/IsBlockList` wrappers |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserPartyInfo` | 8（补回 `GetRemainRecruitPenalty`、apply-slot query/clear/count 与 `DelPartyRecruit`） | asm_restored | export-for-ai `1400B1980.c` / `0x1400D6F00 / 0x1400D6F50 / 0x1400D6FA0 / 0x1400D6FE0 / 0x1400D7030 / 0x1400D72A0` |
| `GameServer/XRelayServer` | `ServerProcess.h` | `ST_SYNC_INFO` | 1（union） | verified | IDA type inspect + `CServer::SetServerInfo` |
| `GameServer/XRelayServer` | `ServerProcess.h` | `CServer` | 5 | verified | IDA type inspect + `CServer::SetServerInfo` |
| `GameServer/XRelayServer` | `ServerProcess.h` | `CServerProcess` | 1（`TXProcess<CServer>`） | verified | IDA type inspect + ServerProcess.obj |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.h` | `CServerModeMazeProcess` | 1（`TXProcess<CServer>`） | verified | IDA type inspect + ServerModeMazeProcess.obj |
| `GameServer/XRelayServer/Thread` | `LogicThreadProcessor.h` | `CLogicThreadProc` | 9+核心字段 | asm_restored | `ThreadProc/OnUpdate` + worker 0/1/2 routing skeleton evidence（含 party/force `Clear()` parity hook） |
| `GameServer/XRelayServer/Thread` | `LogicThreadProcessor.h` | `CLogicThreadManager` | 3+核心字段 | asm_restored | `DoJob` + `TXSingleton<CLogicThreadManager>` 符号 |
| `GameServer/XRelayServer` | `ModeMazeMatching.h` | `CModeMazeMatchginMember` | 4 | asm_restored | IDA type inspect + bounded rank getter / actor+uaid+server/member slice |
| `GameServer/XRelayServer` | `ModeMazeMatching.h` | `CModeMazeMatching` | 12（补回最小 OnUpdate/MakeOperationMaze 状态机 + event fanout 承接） | asm_restored | PDB type dump + export-for-ai `140032A30.c / 140033980.c / 140033AA0.c / 140034170.c / 140039C60.c` |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.h` | `CModeMazeMatchingMgr` | 10 | asm_restored | IDA type inspect + PDB type dump |
| `GameServer/XRelayServer` | `UserProcess.h` | `CUserProcess` | 1（`TXProcess<CServer>`） | asm_restored | RelayServer.pdb type dump + export-for-ai parse/login/logout/update map chain |
| `GameServer/XRelayServer` | `RelayServer.h` | `XGameDBSocketMgr` | 4 | asm_restored | export-for-ai `SendDBAccount/SendDBGame` + current wrapper |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_USERS_INFO` | 3 | verified | IDA type inspect + `CServerProcess::SyncUsersInfo` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_UPDATE_USER_MAP_INFO` | 6 | asm_restored | RelayServer.pdb type dump + export-for-ai `1400EDDE0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_NOTICE` | 4 | asm_restored | XBaseServer.pdb type dump + export-for-ai `1400D7B30.c` / `1400BA3C0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_MEGAPHONE` | 5 | asm_restored | XBaseServer.pdb type dump + export-for-ai `1400D7BA0.c` / `1400BA450.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_SOCKET_DATA` | 3 | asm_restored | XBaseServer.pdb type dump + LoginServer.exe.h + export-for-ai `1400EAAE0.c` / `1400EAB80.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_ITEM_SOCKET` | 2 | asm_restored | XBaseServer.pdb type dump + LoginServer.exe.h + export-for-ai `1400EAAE0.c` / `1400EAB80.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_SOCKET_LIST` | 1 | asm_restored | XBaseServer.pdb type dump + export-for-ai `1400EAC20.c` / `1400EACB0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_ITEM_BROACH` | 2 | asm_restored | XBaseServer.pdb type dump + LoginServer.exe.h + export-for-ai `1400EAD60.c` / `1400EADD0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_ITEM_PACKAGE_PARTS` | 3 | asm_restored | XBaseServer.pdb type dump + LoginServer.exe.h + export-for-ai `1400EB500.c` / `1400EB570.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_PACKAGE` | 2 | asm_restored | XBaseServer.pdb type dump + LoginServer.exe.h + export-for-ai `1400EB5E0.c` / `1400EB680.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_ITEM_LINK` | 8 | asm_restored | XBaseServer.pdb type dump + LoginServer.exe.h + export-for-ai `1400DB550.c` / `1400DB670.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_ITEM_LINK_FOR_SERVER` | 2 | asm_restored | XBaseServer.pdb type dump + export-for-ai `1400DB780.c` / `1400DB800.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_MODE_MAZE_MEMBER_INFO` | 11 | verified | IDA type inspect + PDB types |
| `GameServer/XRelayServer` | `PartyProcess.h` | `CPartyProcess` | 1（`TXProcess<CServer>`，补回 recruit add/del/apply/accept-reject/list + my-apply/apply-list + apply-del 分派） | asm_restored | RelayServer decompile `1400A1C50.c / 1400A1D40.c / 1400A47F0.c / 1400A5520.c / 1400A5900.c / 1400A6010.c / 1400A62D0.c / 1400A6560.c / 1400A6BB0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_MEMBER` | 12 | asm_restored | IDA type inspect + LoginServer.exe.h + RelayServer party decompile |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_UPDATE_PARTY_MEMBER` | 2 | asm_restored | IDA type inspect + RelayServer decompile `1400A32E0.c / 140098990.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_PARTY_ENTER_SERVER` | 5 | asm_restored | IDA type inspect + RelayServer decompile `1400A3450.c / 140096FB0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_PARTY_INFO` | 6 | asm_restored | IDA type inspect + LoginServer.exe.h + RelayServer manager send path |
| `GameServer/XRelayServer` | `FriendProcess.h` | `CFriendProcess` | 1（`TXProcess<CServer>`） | asm_restored | RelayServer decompile `140040280.c` + `SetCmd(0xF5)` / `SetName("CServerFriendProcess")` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ` | 3 | verified | IDA type inspect + PDB types |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_CREATE_MODE_MAZE` | 10 | asm_restored | RelayServer.pdb type dump + export-for-ai `140036AB0.c` / `1400DFAB0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_WHISPER` | 5 | asm_restored | IDA type inspect + RelayServer decompile `1400D7A10.c` + login relay `RecvUserWhisperRes` 对位 |
| `GameServer/XRelayServer` | `UserObject.h` | `CCommunity` | 9 | asm_restored | IDA type inspect + decompile `140001000.c / 140001110.c / 140001270.c / 1400013E0.c / 140001600.c / 140001710.c / 140001AE0.c` |
| `GameServer/XRelayServer` | `UserObject.h` | `CFriendMember` | 2 | asm_restored | IDA type inspect + `CCommunity::IsFriend/GetFriendUCID` 调用点 |
| `GameServer/XRelayServer` | `UserObject.h` | `CBlockUser` | 1 | asm_restored | IDA type inspect + `CCommunity::IsBlockList` 调用点 |
| `GameServer/XRelayServer` | `UserObject.h` | `ST_FRIEND_COMMUNITY` | 2 | asm_restored | IDA type inspect + `CCommunity` 构造/查询切片 |
| `GameServer/XRelayServer` | `UserObject.h` | `ST_FRIEND_INFO` | 15 | asm_restored | IDA type inspect + `CFriendMember` / `CCommunity::IsFriend/GetFriendUCID` |
| `GameServer/XRelayServer` | `UserObject.h` | `ST_BLOCK_INFO` | 3 | asm_restored | IDA type inspect + `CBlockUser` / `CCommunity::IsBlockList` |
| `GameServer/XRelayServer` | `UserObject.h` | `PS_FRIEND_LIST` | 1 | asm_restored | LoginServer.exe.h + RelayServer decompile `140001D80.c / 1400D4BF0.c / 1400E09F0.c` |
| `GameServer/XRelayServer` | `UserObject.h` | `PS_BLOCKLIST_INFO` | 1 | asm_restored | LoginServer.exe.h + RelayServer decompile `140001E80.c / 1400D4D50.c / 1400E0A70.c` |
| `GameServer/XRelayServer` | `ForceProcess.h` | `CForceProcess` | 1（`TXProcess<CServer>`） | asm_restored | RelayServer decompile `140023270.c` + `SetCmd(0xFA)` / `SetName("CForceProcess")` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_FORCE_MEMBER` | 12 | asm_restored | IDA type inspect + force decompile `140024760.c / 140016380.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_UPDATE_FORCE_MEMBER` | 2 | asm_restored | IDA type inspect + RelayServer decompile `140024760.c / 140017CC0.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_FORCE_ENTER_SERVER` | 5 | asm_restored | IDA type inspect + RelayServer decompile `1400248D0.c / 140016380.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_INFO` | 6 | asm_restored | IDA type inspect + RelayServer decompile `140026870.c / 140016380.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_PARTY` | 3 | asm_restored | IDA type inspect + RelayServer decompile `140023570.c / 140017D50.c / 1400DE170.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_FORCE_ENTER_SERVER` | 3 | asm_restored | IDA type inspect + RelayServer decompile `140016380.c / 1400DC820.c` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_PARTY_ENTER_SERVER` | 3 | asm_restored | IDA type inspect + RelayServer decompile `140096FB0.c / 14009716D` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_APPLY_MEMBER` | 2 | asm_restored | IDA type inspect + `ST_APPLY_MEMBER_LIST` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_APPLY_MEMBER_LIST` | 1 | asm_restored | IDA type inspect + `CPartyRecruit` layout |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT` | 12 | asm_restored | IDA type inspect + RelayServer decompile `1400971D2 / 14009E5C0` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_INFO` | 2 | asm_restored | IDA type inspect + `CPartyRecruit::GetRecruitInfo(0x1400AE0B0)` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_INFO_LIST` | 1 | asm_restored | RelayServer decompile `0x1400A6640 / 0x1400A668E` + bounded list serializer recovery |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_APPLY` | 2 | asm_restored | RelayServer decompile `ReqPartyRecruitApply(0x1400A5900)` + bounded serializer recovery |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_PARTY_ADDMEMBER` | 3 | asm_restored | IDA type inspect + `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK::stMember` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT` | 3 | asm_restored | IDA type inspect + `ReqPartyRecruitApplyAccept/Reject(0x1400A6010 / 0x1400A62D0)` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK` | 6 | asm_restored | IDA type inspect + serializer decompile `0x1400DE270 / 0x1400DE320` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_MEMBER_LIST` | 1 | asm_restored | RelayServer decompile `ReqPartyRecruitApplyList(0x1400A68C0)` + applicant member-list serializer recovery |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_APPLY_INFO` | 6 | asm_restored | IDA type inspect + bounded recruit apply-info serializer recovery |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_PARTY_RECRUIT_ADD_REQ` | 4 | asm_restored | IDA type inspect + `ReqPartyRecruitAdd(0x1400A47F0)` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_PARTY_RECRUIT_ADD_RES` | 5 | asm_restored | IDA type inspect + `ReqPartyRecruitAdd(0x1400A47F0)` result path |
| `GameServer/XRelayServer` | `PartyRecruit.h` | `CPartyRecruit` | 5（补回 `GetApplyCount/IsApplied`、`GetPartyMemberList`、`RecruitApply/RecruitAccept`、`DelApplyMember` 与 `SendApplyUserList`，含 accept-check / apply-info 最小派发） | asm_restored | IDA type inspect + party recruit apply docs + `0x1400AE120 / 0x1400AE570 / 0x1400AEAF0` |
| `GameServer/XRelayServer` | `PartyMatchingMgr.h` | `CPartyMatchginMember` | 4 | asm_restored | RelayServer decompile `0x14009D9A0` + bounded party matching member slice |
| `GameServer/XRelayServer` | `PartyMatchingMgr.h` | `CPartyMatching` | 13 | asm_restored | RelayServer decompile `0x14009B830 / 0x14009D050 / 0x14009C2A0 / 0x14009C750` + bounded party matching state-machine slice |
| `GameServer/XRelayServer` | `PartyMatchingMgr.h` | `CPartyMatchingMgr` | 4（补回 recruit create/del/apply/accept-reject/apply-info manager 面与 apply-list query 面） | asm_restored | IDA type inspect + `0x14009ED70 / 0x14009F220 / 0x14009F330 / 0x14009E000 / 0x14009EF00 / 0x14009EE50 / 0x14009EF70 / 0x1400A52BA / 0x1400A5768 / 0x1400A668E / 0x1400A6D40` |
| `GameServer/XRelayServer` | `Force.h` | `ST_FORCE_INVITE_INFO` | 2 | asm_restored | IDA type inspect + RelayServer.pdb files `psforce.h` |
| `GameServer/XRelayServer` | `Force.h` | `CForceMember` | 3 | asm_restored | IDA type inspect + export-for-ai `1400147A0.c / 140094650.c` |
| `GameServer/XRelayServer` | `Force.h` | `CForce` | 5 | asm_restored | IDA type inspect + export-for-ai `1400135A0.c / 1400944A0.c / 1400945A0.c` |
| `GameServer/XRelayServer` | `ForceManager.h` | `CForceManager` | 6（含 `m_factoryForce` 48-byte 占位） | asm_restored | IDA type inspect + RelayServer.pdb modules/files (`ForceManager.obj`) + `0x140016380 / 0x140014970 / 0x140017440 / 0x140017FE0` decompile |
| `GameServer/XRelayServer` | `ForceMatching.h` | `CForceMatchginMember` | 4 | asm_restored | IDA type inspect + `SendMatchingExit/MatchingRemoveUser` 成员访问 |
| `GameServer/XRelayServer` | `ForceMatching.h` | `CForceMatching` | 15 | asm_restored | IDA type inspect + `AutoMatchingExit/SendMatchingExit/SendMatchingCheck/SendMatchingReset/SendMatchingStart/MatchingPossible/MatchingCheck/MatchingWait/CreateMazeMatching/SendCreateMatchingMaze` decompile |
| `GameServer/XRelayServer` | `ForceMatching.h` | `CForceMatchingMgr` | 2 | asm_restored | IDA type inspect + `ExitMatching/MatchingRemoveUser/CheckMatching/ResForceMatchingCreate/SendCreateMatchingMaze` decompile |
| `GameServer/XRelayServer` | `GameDBSocket.h` | `CGameDBSocket` | 0（`TXDBSocketT<CServer>` 最小回包承接面） | asm_restored | RelayServer decompile `DBParse/DBForceParse/ResForceMatchingCreate` + current TXDBSocketT analogue |
| `GameServer/XRelayServer` | `GameDBSocket.h` | `XGameDBSocketMgr` | 4（最小 Game/Account agent 组） | asm_restored | LoginServer 类比 + RelayServer DB send path |
| `GameServer/XRelayServer` | `RelayServer.h` | `CRelayPartyMatchingConfig` | 3 | asm_restored | `CPartyMatching::SendMatchingWait(0x14009CEE0)` + bounded CommonDB `tb_Common[30002]` loader |
| `GameServer/XRelayServer` | `RelayServer.h` | `CRelayMazeOpenControl` | 4 | asm_restored | RelayServer decompile `CheckMazeOpenTime / SetCheckMazeOpenTime` + bounded CommonDB table loader |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_DB_FORCE_MATCHING_CREATE` | 3 | asm_restored | RelayServer decompile `0x14001D620 / 0x1400209A0 / 0x1400DCCC0` + bounded set serializer recovery |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_FORCE_MATCHING_ENTER` | 3 | asm_restored | IDA type inspect + `ReqForceMatchingEnter(0x140024A40)` + shared serializer recovery |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_FORCE_MATCHING_ENTER_MEMBER` | 10 | asm_restored | IDA type inspect + `ReqForceMatchingEnter(0x140024A40)` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_FORCE_MATCHING_CHECK` | 4 | asm_restored | IDA type inspect + `ReqForceMatchingCheck(0x1400265E0)` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_FORCE_MATCHING_INFO` | 3 | asm_restored | IDA type inspect + `ReqForceMatchingEnter/SendMatchingExit` decompile |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_CREATE_FORCE` | 3 | asm_restored | IDA type inspect + `SendMatchingStart(0x14001D620)` |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_DEL` | 3 | asm_restored | RelayServer decompile `0x14009E000 / 0x1400DDC40` + recruit-expiry delete broadcast payload |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_DEL_LIST` | 1 | asm_restored | RelayServer decompile `0x1400DDDF0` + `0xF4/0x26` vector payload |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RECRUIT_DELETE` | 1 | asm_restored | RelayServer decompile `0x1400E1A30 / 0x1400B8930 / 0x1400B93A0` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RECRUIT_STATE` | 1 | asm_restored | RelayServer decompile `0x1400C93D0 / 0x1400E1420` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_RECRUIT_DELETE` | 2 | asm_restored | RelayServer decompile `0x1400C9920 / 0x14004C150 / 0x1400B8D00` |
| `GameServer/XRelayServer` | `RelayServer.h` | `CFriendRecruitManager` | 4（最小 recruit/login/expire 状态） | asm_restored | RelayServer decompile `0x140045110 / 0x140045580` + bounded worker-2 recruit manager |
| `GameServer/XRelayServer` | `PartyManager.h` | `CPartyManager` | 2（`m_mapParty` + `m_mapPartyUser`） | verified | RelayServer decompile `0x1400995A0` + `ResRecruitAccept` party/force branching |
| `GameServer/XRelayServer` | `Party.h` | `CParty` | 3（party ID + master ID + member map） | verified | bounded party-member index + `GetUserCount/AddMember` |
| `GameServer/XRelayServer` | `Party.h` | `CPartyMember` | 1（`ST_PARTY_MEMBER`） | verified | bounded member struct wrapper |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_ADDMEMBER` | 2 | verified | RelayServer decompile `0x1400166F0` + `main=8/sub=2` DB game packet |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_FORCE_CREATE` | 3 | verified | RelayServer decompile `0x1400149C0` + `main=8/sub=1` DB game packet |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_LEAVE` | 3 | asm_restored | IDA type inspect `PS_FORCE_LEAVE` (dwForceID + dwLeaveMember + bKickout) + 0x14004B500 decompile |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_DELETE` | 2 | asm_restored | IDA type inspect `PS_FORCE_DELETE` (dwForceID + dwLeaveMember) + 0x14004B800 decompile |
