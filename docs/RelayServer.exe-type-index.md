# RelayServer.exe 类型索引

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| `GameServer/XRelayServer` | `RelayControlSocket.h` | `XRelaySocket::RelayInfo` | 4 | verified | 现有 LoginServer 类比 + 共享协议结构 |
| `GameServer/XRelayServer` | `RelayControlSocket.h` | `XRelaySocket` | 5+核心字段 | verified | IDA decompile `Init/SetMyInfo/OnParse/ServerProcess` 验证 + m_myInfo/m_relayInfo/m_nSyncServerData/m_mapChannelInfo 字段验证 + 虚方法边界确认 |
| `GameServer/XRelayServer` | `RelayControlSocket.h` | `CRelayControlSocket` | 1（最小调度接口） | verified | IDA `SetMyInfo(0x14003CDB0)/ServerProcessEx(0x14003CF30)` + 四个子处理器 `'D'/'E'/'F'/'J'` 分发验证 + ResCreateMatchingMaze/SyncPartyMazeInfo/SyncForceMazeInfo/ResCreateMatchingModeMaze 四方法签名确认 |
| `GameServer/XRelayServer` | `RelayServer.h` | `XRelayServer` | 10+核心字段 | verified | IDA decompile `OnUpdate/LoadDataReq` + PDB type chain 验证 + recruit/manager hooks 验证 + CRelayPartyMatchingConfig 接线验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserObject` | 12+query-only wrappers + IsMaze() | verified | IDA decompile `SendPacket/CheckGameOption/GetFriendUCID/IsFriendList/IsBlockList` 验证 + IsMaze() bounded mapID/10000==2 验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `CUserPartyInfo` | 10 | verified | IDA decompile `GetRemainRecruitPenalty/GetRewardState/SetRewardState` 验证 + m_byRewardState 字段验证 |
| `GameServer/XRelayServer` | `ServerProcess.h` | `ST_SYNC_INFO` | 1（union） | verified | IDA type inspect + `CServer::SetServerInfo` |
| `GameServer/XRelayServer` | `ServerProcess.h` | `CServer` | 5 | verified | IDA type inspect + `CServer::SetServerInfo` |
| `GameServer/XRelayServer` | `ServerProcess.h` | `CServerProcess` | 1（`TXProcess<CServer>`） | verified | IDA type inspect + ServerProcess.obj |
| `GameServer/XRelayServer` | `ServerModeMazeProcess.h` | `CServerModeMazeProcess` | 1（`TXProcess<CServer>`） | verified | IDA type inspect + ServerModeMazeProcess.obj |
| `GameServer/XRelayServer/Thread` | `LogicThreadProcessor.h` | `CLogicThreadProc` | 9+核心字段 | verified | `ThreadProc/OnUpdate` + worker 0/1/2 routing skeleton evidence（含 party/force `Clear()` parity hook） |
| `GameServer/XRelayServer/Thread` | `LogicThreadProcessor.h` | `CLogicThreadManager` | 3+核心字段 | verified | `DoJob` + `TXSingleton<CLogicThreadManager>` 符号 |
| `GameServer/XRelayServer` | `ModeMazeMatching.h` | `CModeMazeMatchginMember` | 4 | verified | IDA 0x14003CBE0(Ctor calls Clear) + 0x14003CBA0(Clear:m_pCurServer=null+memset+rank=0xFA00) + 0x14003CAC0(SetRank:rank==0→0xFA00哨兵); 2026-04-26 对齐完整 API |
| `GameServer/XRelayServer` | `ModeMazeMatching.h` | `CModeMazeMatching` | 12（补回最小 OnUpdate/MakeOperationMaze 状态机 + event fanout 承接） | verified | PDB type dump + export-for-ai `140032A30.c / 140033980.c / 140033AA0.c / 140034170.c / 140039C60.c` |
| `GameServer/XRelayServer` | `ModeMazeMatchingMgr.h` | `CModeMazeMatchingMgr` | 10 | verified | IDA type inspect + PDB type dump |
| `GameServer/XRelayServer` | `UserProcess.h` | `CUserProcess` | 1（`TXProcess<CServer>`） | verified | RelayServer.pdb type dump + export-for-ai parse/login/logout/update map chain 验证 + SetCmd(0xF3) 验证 |
| `GameServer/XRelayServer` | `RelayServer.h` | `XGameDBSocketMgr` | 4 | verified | export-for-ai `SendDBAccount/SendDBGame` + current wrapper |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_USERS_INFO` | 3 | verified | IDA type inspect + `CServerProcess::SyncUsersInfo` |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_UPDATE_USER_MAP_INFO` | 6 | verified | static_assert size=0x28 + offset 验证 (dwActorID@0x4, uxMapID@0x8, stPartyInfo@0x10, biAuthSessionID@0x18, bLeaveParty@0x20) + IDA decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_NOTICE` | 4 | verified | static_assert size=0x214 + offset 验证 (strMsg@0x2, strColor@0x202, nMessageCode@0x210) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_MEGAPHONE` | 5 | verified | static_assert size=0x234 + offset 验证 (shSlot@0x2, dwUCID@0x4, strName@0x8, strMsg@0x32) |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_SOCKET_DATA` | 3 | verified | static_assert size=0x30 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_ITEM_SOCKET` | 2 | verified | static_assert size=0x38 + offset 验证 (biEquipSerial@0x30) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_SOCKET_LIST` | 1 | verified | static_assert size=0x20 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_ITEM_BROACH` | 2 | verified | static_assert size=0x48 + offset 验证 (dwItemID@0x8) |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_ITEM_PACKAGE_PARTS` | 3 | verified | static_assert size=0x10 + offset 验证 (nItemID@0x8, nDyeID@0xC) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_ITEM_PACKAGE` | 2 | verified | static_assert size=0x28 + offset 验证 (vecInfo@0x8) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_ITEM_LINK` | 8 | verified | static_assert size=0x198 + offset 验证 (i64ID@0x8, szLinkString@0x10, kItem@0x90, psSocketInfo@0x108, psBroachInfo@0x128, psRePackageCostumeInfo@0x170) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_ITEM_LINK_FOR_SERVER` | 2 | verified | static_assert size=0x4D0 + offset 验证 (psItemLinkInfo@0x8) |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_MODE_MAZE_MEMBER_INFO` | 11 | verified | IDA type inspect + PDB types |
| `GameServer/XRelayServer` | `PartyProcess.h` | `CPartyProcess` | 1（`TXProcess<CServer>`，补回 recruit add/del/apply/accept-reject/list + my-apply/apply-list + apply-del 分派） | verified | RelayServer decompile + 36 subcommand handler 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_MEMBER` | 12 | verified | static_assert size=0x58 + offset 验证 (strName@0x4, byLevel@0x2E) + IDA decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_UPDATE_PARTY_MEMBER` | 2 | verified | IDA decompile `CPartyProcess::ReqPartyUpdateMember(0x1400A32E0)` + 结构嵌套验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_PARTY_ENTER_SERVER` | 5 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_PARTY_INFO` | 6 | verified | IDA type inspect + RelayServer manager send path 验证 |
| `GameServer/XRelayServer` | `FriendProcess.h` | `CFriendProcess` | 1（`TXProcess<CServer>`） | verified | RelayServer decompile `140040280.c` + SetCmd(0xF5) / SetName("CServerFriendProcess") 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ` | 3 | verified | IDA type inspect + PDB types |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_CREATE_MODE_MAZE` | 10 | verified | static_assert size=0x2A8 + offset 验证 (wReqMapID@0x258, uxParentMazeID@0x260, wEnterDistrictID@0x268, dwMatchingID@0x26C, nModeType@0x270, nResult@0x274, bHotTime@0x278, dwMasterServerID@0x27C, dwEventRoomID@0x280, vecEnterMember@0x288) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_WHISPER` | 5 | verified | IDA decompile `CUserProcess::ReqUserChatWhisper(0x1400D7A10)` + static_assert size=0x25C + offset 验证 (strReciver@0x2A, strMsg@0x54, nResult@0x254, dwSenderUCID@0x258) + 序列化器验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `CCommunity` | 9 | verified | IDA type inspect + decompile 验证 + 好友/黑名单容器布局确认 |
| `GameServer/XRelayServer` | `UserObject.h` | `CFriendMember` | 2 | verified | IDA type inspect + CCommunity::IsFriend/GetFriendUCID 调用点验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `CBlockUser` | 1 | verified | IDA type inspect + CCommunity::IsBlockList 调用点验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `ST_FRIEND_COMMUNITY` | 2 | verified | IDA type inspect + CCommunity 构造/查询切片验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `ST_FRIEND_INFO` | 15 | verified | IDA type inspect + CFriendMember / CCommunity 验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `ST_BLOCK_INFO` | 3 | verified | IDA type inspect + CBlockUser / CCommunity 验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `PS_FRIEND_LIST` | 1 | verified | LoginServer.exe.h + RelayServer decompile 验证 |
| `GameServer/XRelayServer` | `UserObject.h` | `PS_BLOCKLIST_INFO` | 1 | verified | LoginServer.exe.h + RelayServer decompile 验证 |
| `GameServer/XRelayServer` | `ForceProcess.h` | `CForceProcess` | 1（`TXProcess<CServer>`） | verified | RelayServer decompile `140023270.c` + SetCmd(0xFA) / SetName("CForceProcess") 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_FORCE_MEMBER` | 12 | verified | static_assert size=0x58 + offset 验证 (strName@0x4, byLevel@0x2E, byAwaken@0x30, dwProfilePhotoID@0x34, nMapID@0x38, nChannel@0x3C, nMaxHP@0x40, nHP@0x44, bLogin@0x48, uxMapID@0x50) |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_UPDATE_FORCE_MEMBER` | 2 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_FORCE_ENTER_SERVER` | 5 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_INFO` | 6 | verified | static_assert size=0x38 + offset 验证 (uxMazeID@0x8, byUpdateType@0x10, vecForceMember@0x18) |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_CHAT_PARTY` | 3 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_FORCE_ENTER_SERVER` | 3 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_PARTY_ENTER_SERVER` | 3 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_APPLY_MEMBER` | 2 | verified | IDA type inspect + ST_APPLY_MEMBER_LIST 嵌套验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_APPLY_MEMBER_LIST` | 1 | verified | IDA type inspect + CPartyRecruit layout 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT` | 12 | verified | IDA type inspect + RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_INFO` | 2 | verified | IDA type inspect + CPartyRecruit::GetRecruitInfo 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_INFO_LIST` | 1 | verified | RelayServer decompile + bounded list serializer 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_APPLY` | 2 | verified | RelayServer decompile `ReqPartyRecruitApply(0x1400A5900)` 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_PARTY_ADDMEMBER` | 3 | verified | IDA type inspect + PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT` | 3 | verified | IDA type inspect + ReqPartyRecruitApplyAccept/Reject 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK` | 6 | verified | IDA type inspect + serializer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_MEMBER_LIST` | 1 | verified | RelayServer decompile + bounded member-list serializer 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_APPLY_INFO` | 6 | verified | IDA type inspect + bounded recruit apply-info serializer 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_PARTY_RECRUIT_ADD_REQ` | 4 | verified | IDA type inspect + ReqPartyRecruitAdd(0x1400A47F0) 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_PARTY_RECRUIT_ADD_RES` | 5 | verified | IDA type inspect + ReqPartyRecruitAdd result 验证 |
| `GameServer/XRelayServer` | `PartyRecruit.h` | `CPartyRecruit` | 5（补回 `GetApplyCount/IsApplied`、`GetPartyMemberList`、`RecruitApply/RecruitAccept`、`DelApplyMember` 与 `SendApplyUserList`，含 accept-check / apply-info 最小派发） | verified | IDA type inspect + party recruit apply docs + `0x1400AE120 / 0x1400AE570 / 0x1400AEAF0` |
| `GameServer/XRelayServer` | `PartyMatchingMgr.h` | `CPartyMatchginMember` | 4 | verified | RelayServer decompile `0x14009D9A0` + bounded party matching member slice |
| `GameServer/XRelayServer` | `PartyMatchingMgr.h` | `CPartyMatching` | 13 | verified | RelayServer decompile `0x14009B830 / 0x14009D050 / 0x14009C2A0 / 0x14009C750` + bounded party matching state-machine slice |
| `GameServer/XRelayServer` | `PartyMatchingMgr.h` | `CPartyMatchingMgr` | 4（补回 recruit create/del/apply/accept-reject/apply-info manager 面与 apply-list query 面） | verified | IDA type inspect + `0x14009ED70 / 0x14009F220 / 0x14009F330 / 0x14009E000 / 0x14009EF00 / 0x14009EE50 / 0x14009EF70 / 0x1400A52BA / 0x1400A5768 / 0x1400A668E / 0x1400A6D40` |
| `GameServer/XRelayServer` | `Force.h` | `ST_FORCE_INVITE_INFO` | 2 | verified | IDA type inspect + RelayServer.pdb files `psforce.h` |
| `GameServer/XRelayServer` | `Force.h` | `CForceMember` | 3 | verified | IDA type inspect + export-for-ai `1400147A0.c / 140094650.c` |
| `GameServer/XRelayServer` | `Force.h` | `CForce` | 5 | verified | IDA type inspect + export-for-ai `1400135A0.c / 1400944A0.c / 1400945A0.c` |
| `GameServer/XRelayServer` | `ForceManager.h` | `CForceManager` | 6（含 `m_factoryForce` 48-byte 占位） | verified | IDA type inspect + RelayServer.pdb modules/files (`ForceManager.obj`) + `0x140016380 / 0x140014970 / 0x140017440 / 0x140017FE0` decompile |
| `GameServer/XRelayServer` | `ForceMatching.h` | `CForceMatchginMember` | 4 | verified | IDA type inspect + `SendMatchingExit/MatchingRemoveUser` 成员访问 |
| `GameServer/XRelayServer` | `ForceMatching.h` | `CForceMatching` | 15 | verified | IDA type inspect + `AutoMatchingExit/SendMatchingExit/SendMatchingCheck/SendMatchingReset/SendMatchingStart/MatchingPossible/MatchingCheck/MatchingWait/CreateMazeMatching/SendCreateMatchingMaze` decompile |
| `GameServer/XRelayServer` | `ForceMatching.h` | `CForceMatchingMgr` | 2 | verified | IDA type inspect + `ExitMatching/MatchingRemoveUser/CheckMatching/ResForceMatchingCreate/SendCreateMatchingMaze` decompile |
| `GameServer/XRelayServer` | `GameDBSocket.h` | `CGameDBSocket` | 0（`TXDBSocketT<CServer>` 最小回包承接面） | verified | RelayServer decompile `DBParse/DBForceParse/ResForceMatchingCreate` + current TXDBSocketT analogue |
| `GameServer/XRelayServer` | `GameDBSocket.h` | `XGameDBSocketMgr` | 4（最小 Game/Account agent 组） | verified | LoginServer 类比 + RelayServer DB send path |
| `GameServer/XRelayServer` | `RelayServer.h` | `CRelayPartyMatchingConfig` | 3 | verified | `CPartyMatching::SendMatchingWait(0x14009CEE0)` + bounded CommonDB `tb_Common[30002]` loader |
| `GameServer/XRelayServer` | `RelayServer.h` | `CRelayMazeOpenControl` | 4 | verified | RelayServer decompile `CheckMazeOpenTime / SetCheckMazeOpenTime` + bounded CommonDB table loader |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_DB_FORCE_MATCHING_CREATE` | 3 | verified | RelayServer decompile + bounded set serializer 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_FORCE_MATCHING_ENTER` | 3 | verified | IDA type inspect + ReqForceMatchingEnter 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_FORCE_MATCHING_ENTER_MEMBER` | 10 | verified | IDA type inspect + ReqForceMatchingEnter 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_SERVER_FORCE_MATCHING_CHECK` | 4 | verified | IDA type inspect + ReqForceMatchingCheck 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_FORCE_MATCHING_INFO` | 3 | verified | static_assert size=0x2D0 + offset 验证 (stMemberInfo@0x8, nRemainTick@0x2C8) |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_CREATE_FORCE` | 3 | verified | static_assert size=0x28 + offset 验证 (dwLeaderUCID@0x4, dwMemberUCID@0x8) |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_DEL` | 3 | verified | RelayServer decompile + recruit-expiry delete broadcast 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_PARTY_RECRUIT_DEL_LIST` | 1 | verified | RelayServer decompile + 0xF4/0x26 vector payload 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RECRUIT_DELETE` | 1 | verified | RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RECRUIT_STATE` | 1 | verified | RelayServer decompile 验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_RECRUIT_DELETE` | 2 | verified | RelayServer decompile 验证 |
| `GameServer/XRelayServer` | `RelayServer.h` | `CFriendRecruitManager` | 4（最小 recruit/login/expire 状态） | verified | RelayServer decompile `0x140045110 / 0x140045580` + bounded worker-2 recruit manager |
| `GameServer/XRelayServer` | `PartyManager.h` | `CPartyManager` | 2（`m_mapParty` + `m_mapPartyUser`） | verified | RelayServer decompile `0x1400995A0` + `ResRecruitAccept` party/force branching |
| `GameServer/XRelayServer` | `Party.h` | `CParty` | 3（party ID + master ID + member map） | verified | bounded party-member index + `GetUserCount/AddMember` |
| `GameServer/XRelayServer` | `Party.h` | `CPartyMember` | 1（`ST_PARTY_MEMBER`） | verified | bounded member struct wrapper |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_ADDMEMBER` | 2 | verified | RelayServer decompile `0x1400166F0` + `main=8/sub=2` DB game packet |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_FORCE_CREATE` | 3 | verified | RelayServer decompile `0x1400149C0` + `main=8/sub=1` DB game packet |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_LEAVE` | 3 | verified | IDA type inspect `PS_FORCE_LEAVE` (dwForceID + dwLeaveMember + bKickout) + 0x14004B500 decompile |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_FORCE_DELETE` | 2 | verified | IDA type inspect `PS_FORCE_DELETE` (dwForceID + dwLeaveMember) + 0x14004B800 decompile |
| `GameServer/XRelayServer` | `LeagueProcess.h` | `CLeagueProcess` | 1（`TXProcess<CServer>`） | verified | RelayServer decompile `0x1400849B0` + PDB symbols CServerLeagueProcess + SetCmd(0xF6) + 36 subcommand handler declarations 验证 |
| `GameServer/XRelayServer` | `WorldModeProcess.h` | `CServerWorldModeProcess` | 1（`TXProcess<CServer>`） | deprecated | **IDA 证实主命令 0xFB 未注册**：`CServer::RegisterProcess(0x1400D1B40)` 注册 0xF2/0xF3/0xF4/0xF5/0xF6/0xF7/0xFA/0xFD，无 0xFB。此为重构 helper stub（错误的主命令号），应删除或改用 GreenDamTan_ 前缀。GameServer `WorldModeProcess` 是 0xFD 命令处理器（CServerModeMazeProcess）的子协议。 |
| `GameServer/XRelayServer` | `League.h` | `CLeague` | 16（m_stLeagueInfo + m_mpLeagueMember + m_mpApplicant + m_deqBoard + m_deqRecord + m_szSubMasterName + m_szMasterName + m_szPositionName + m_stNotice + m_stRecruitNotice + m_biNoticeDate + m_biRecruitNoticeDate + m_nSyncCount + m_nInventorySyncCount + m_nSkillPoint + m_stInfoForGame） | verified | IDA decompile `CLeague::CLeague(0x140064270)` + 构造函数字段初始化验证 + 0x8A8 bytes total + 源码标注一致 |
| `GameServer/XRelayServer` | `LeagueMember.h` | `CLeagueMember` | 1（`ST_LEAGUE_MEMBER_EX`） | verified | IDA decompile `CLeagueMember::CLeagueMember(0x140064080)` + 构造函数验证 (m_stMember + m_bEnrollBoard=1) + PDB symbols |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_LEAGUE_MEMBER_EX` | 13（stMember + bLogin + sWorldID + byChannel + dwUCID + szName + shLevel + biBoardLimitTime + byClass + byAwaken + dwProfilePhotoID + biPlayDate + padding） | verified | IDA type inspect + CUserObject::GetLeagueMemberInfo 填充验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `ST_LEAGUE_APPLICANT` | 11（nLeagueID + dwActorID + szName + shLevel + padding + biApplicantDate + byClass + byAwaken + padding + dwProfilePhotoID + nResult） | verified | IDA type inspect + CLeague::UpdateApplyList 使用验证 |
| `GameServer/XRelayServer` | `LeagueManager.h` | `PS_SERVER_CHANGE_CHARACTER_NAME` | 4（psChangeInfo + stPartyInfo + nLeagueID + stApplyList） | verified | IDA type inspect + ChangeLeagueApplicant 使用验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_LEAGUE_DELEGATE` | 4（nLeagueID + szDelegatedName[21] + szDelegateName[21] + nResult） | verified | IDA type inspect + CLeague::Delegate 使用验证 + 序列化器修正 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_REQ_LEAGUE_CARD` | 4（nLeagueID + shSlot + padding + dwLeagueCard + nResult） | verified | IDA type inspect + ReqLeagueCardChange/ResLeagueCardChange 使用验证 + 序列化器修复 |
| `Common/XNet/XCommon` | `PSServer.h` | `PS_RES_LEAGUE_SKILL` | 8（nLeagueID + dwUCID + bySkillIndex + bySkillGroupID + bySkillLevel + bySkillPoint + biGold + nResult） | verified | IDA type inspect + CheckLearnSkill/LearnSkill 使用验证 |
| `Common/XNet/XCommon` | `PSServer.h` | `E_LEAGUE_SKILL` | 8（NONE=0, SKILL_1=1, SKILL_CARD=2, SKILL_3=3 ... SKILL_MAX=8） | verified | IDA type inspect + HaveSkill/CheckLeagueCardChange 使用验证 |
| `Common/XNet/XCommon` | `PSCommon.h` | `PS_STORAGE_INFO` | 3（byInvenType + shSlotPos + stItem） | verified | IDA decompile + 序列化器添加 |
| `Common/XNet/XCommon` | `PSCommon.h` | `PS_RES_STORAGE_INFO` | 2（vecItem + byType） | verified | IDA decompile + ResLeagueCardChange 使用验证 + 序列化器添加 |
| `GameServer/XRelayServer` | `LeagueManager.h` | `ST_LEAGUE_INFO` | 27（nLeagueID + nLeagueRank + byGroupType + byRating + shMemberCount + biExp + szLeagueName + biMoney + nCreateDate + biNoticeDate + dwMasterUCID + szMasterName + szSubMasterName + nAuth[9] + nLimitGoldOut[9] + bOpen + dwLeagueCard + szNotice + szPosition_1~3 + szRecruitNotice + biRecruitNoticeDate + bySkillPoint + bySkill[8] + nLimitExp + biInitDate） | verified | IDA type inspect `ST_LEAGUE_INFO` 0x800 bytes + League.cpp/LeagueManager.cpp 使用验证 |

| `Common/XNet/XCommon` | `PSServer.h` | `PS_AUTO_SKILL` | 1（bySkillInfo[8]） | verified | IDA CLeague::Levelup(0x140066590) + SendLevelupToMember(0x140068e30) 字段布局确认 + 序列化器验证 |
| `GameServer/XRelayServer` | `LeagueManager.h` | `ST_LEAGUE_INFO_UPDATE` | 6 | verified | IDA CLeague::ApplyWealth(0x140067390) 字段布局确认 + 序列化器验证 |
