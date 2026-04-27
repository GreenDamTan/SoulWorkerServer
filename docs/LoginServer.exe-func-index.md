# LoginServer.exe 函数索引

## 概述

- 总函数数：5738
- IDA 实例：Port 10003
- 更新时间：2026-04-27 19:35 +08:00
- 索引覆盖：950+ 条目（核心 + 选定外围）

## 核心类函数统计

| 类名 | 函数数 | 状态 |
|------|--------|------|
| XLoginServer | 76 | verified |
| CUser | 332 | partial |
| XLoginProcess | ~12 | verified |
| CCharacterProcess | ~12 | verified |
| CSystemProcess | ~8 | verified |
| CXigncode | 7 | verified(stub) |
| XGameDBSocket | ~30 | verified |
| XGameDBSocketMgr | ~9 | verified |
| CLoginControlSocket | ~15 | verified |
| CObserveSocket | ~10 | verified |
| XRelaySocket | ~22 | verified |
| XIOCP/XIOCPClient/XIOCPServer | ~50 | verified |
| XClient/XClientPool | ~35 | verified |
| XOption | ~30 | verified |
| XResourceMgr | 1442 | partial |
| TXServer/TXObjectMgr | ~25 | verified |
| CFSRWLock | 7 | verified |
| XPacket/XSendPacket | ~10 | verified |

## 函数列表

### XLoginServer 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer | 0x140017550 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::~XLoginServer | 0x1400177B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | TXSingleton<XLoginServer>::Instance | 0x140001410 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::InitServer | 0x140017960 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetName | 0x140017930 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetLastEnterWaitTicket | 0x140015A10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::IsServerAcceptClosed | 0x140015DF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetServerAcceptClosed | 0x140016C10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::EnterUser | 0x1400183B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUIDToUser | 0x1400185C0 | verified | IDA |

### XLoginProcess 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginProcess.cpp | XLoginProcess::Parse | 0x140014B30 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCancel | 0x140015950 | verified | IDA |

### CCharacterProcess 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::Parse | 0x140002250 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::IsValidSecondPassword | 0x140004D80 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |

### CSystemProcess 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | SystemProcess.cpp | CSystemProcess::Parse | 0x14002D660 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncode | 0x14002D800 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemKeepAlive | 0x14002D910 | verified | IDA |

### XGameDBSocket 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::XGameDBSocket | 0x140009FA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::~XGameDBSocket | 0x140009FF0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::FindUser | 0x14000A020 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::SetInfomation | 0x14000A050 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBParse | 0x14000A0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBLoginParse | 0x14000A160 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBCharacterParse | 0x14000A340 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeChange | 0x14000BFD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLogin | 0x14000CF60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnDisConnect | 0x14000DAD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnNotConnect | 0x14000DBA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer | 0x14000DD70 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWContinue | 0x14000DFE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWStateCheck | 0x14000E420 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSGAuthInfoLoad | 0x14000E4F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::SendStatisticsDB_Item | 0x14000E510 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::BroachEffect | 0x14000E640 | verified | IDA |

### XGameDBSocketMgr 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::XGameDBSocketMgr | 0x1400132D0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::~XGameDBSocketMgr | 0x140013370 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::AutoConnect | 0x140014170 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::DisConnect | 0x1400143E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x1400147D0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x140014850 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::SendLogDBAgent | 0x1400148D0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocketMgr::SendStatisticsDBAgent | 0x140014950 | verified | IDA |

### CUser 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::Init | 0x14002E050 | verified | IDA |
| XLoginServer | User.cpp | CUser::OnLogOut | 0x14002E230 | verified | IDA |
| XLoginServer | User.cpp | CUser::Kickout | 0x14002EB10 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::AddCharacterInfo | 0x14002E5F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::DeleteCharacterInfo | 0x14002E650 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetCharacterInfo | 0x14002E760 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearCharacterInfo | 0x14002E6B0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckCreateDate | 0x14002EFB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckChangeSlot | 0x14002F630 | verified | IDA |
| XLoginServer | User.cpp | CUser::ChangeCharacterSlot | 0x14002F230 | verified | IDA |
| XLoginServer | User.cpp | CUser::CanEnterGame | 0x14002F8B0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckRepresentativeChange | 0x14002FC60 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendServerOption | 0x14002EE50 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonLevel | 0x14002ECC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonExp | 0x14002ED00 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsLeagueMaster | 0x14002EDA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckLeagueMaster | 0x14002ED40 | verified | IDA |
| XLoginServer | User.cpp | CUser::SortCharacterList | 0x14002F460 | verified | IDA |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |

### XLoginServer 排队函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::PushWaitUser | 0x140018A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::PopWaitUser | 0x140018B60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ProcessWaitUser | 0x140018C00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::KickoutAll | 0x140018290 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ExitUser | 0x140018690 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Clear | 0x140017D50 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnAccect | 0x140017E60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerGroupList | 0x140019340 | verified | IDA |

### CLoginControlSocket 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::CLoginControlSocket | 0x14002A680 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::~CLoginControlSocket | 0x140029F40 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::SetMyInfo | 0x140015E10 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::ServerProcessEx | 0x140015F90 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserKickout | 0x1400162E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerShutDown | 0x140016370 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer | 0x140016600 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer | 0x140016730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvMaxServerUserCount | 0x140016B30 | verified | IDA |

### CXigncode 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | CXigncode::Init | 0x140001000 | verified(stub) | IDA |
| XLoginServer | LoginServer.cpp | CXigncode::Release | 0x140001080 | verified(stub) | IDA |
| XLoginServer | LoginServer.cpp | CXigncode::ConnectUser | 0x1400010C0 | verified(stub) | IDA |
| XLoginServer | LoginServer.cpp | CXigncode::DisconnectUser | 0x140001150 | verified(stub) | IDA |
| XLoginServer | LoginServer.cpp | CXigncode::SendProc | 0x1400011A0 | verified(stub) | IDA |
| XLoginServer | LoginServer.cpp | CXigncode::DisconectionProc | 0x1400012D0 | verified(stub) | IDA |
| XLoginServer | LoginServer.cpp | CXigncode::RecvXigncode | 0x1400013C0 | verified(stub) | IDA |

**说明**: CXigncode 当前为存根实现（跨平台简化），原版使用 Windows 专用 Xigncode SDK。

### XLoginServer 额外验证函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::AddActor | 0x140018800 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::RemoveActor | 0x140018880 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindActor | 0x140018940 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::nRand | 0x140018A00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::UpdateMaxUserCount | 0x140019120 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::AddSendGameDBUserCount | 0x140019280 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetSendGameDBUserCount | 0x1400192F0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLog | 0x140019640 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB | 0x1400196D0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBAccount | 0x140019900 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBGame | 0x1400199B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBLog | 0x140019A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBStatistics | 0x140019AF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetCurDate | 0x140019BD0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerOption_SecondPW | 0x140019CA0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ConsolCtrlHandler | 0x1400178D0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetConsoleHandler | 0x140018260 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::CheckUserWaitCountSend | 0x140019170 | verified | IDA |

### CObserveSocket 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | CObserveSocket::CObserveSocket | 0x14008D380 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::~CObserveSocket | 0x140029F10 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::StartUp | 0x14008CD10 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::SetMyInfo | 0x14008CDC0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnConnect | 0x14008CCB0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnDisConnect | 0x14008CCD0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnNotConnect | 0x14008CCF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnUpdate | 0x14008D5B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::SendReportServerStatus | 0x14008D440 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::CalculateThreadStatus | 0x14008D0E0 | verified | IDA |

### CLogThreadManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | CLogThreadManager::CLogThreadManager | 0x14004C570 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadManager::~CLogThreadManager | 0x14004CB20 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadManager::Start | 0x14004CD30 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadManager::End | 0x14004CF60 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadManager::DoJob | 0x14004D1B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadManager::GetLogger | 0x14004CBC0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadManager::CreateWorkerThread | 0x14004D290 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadProc::CLogThreadProc | 0x14004C1E0 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadProc::~CLogThreadProc | 0x14004C280 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadProc::ThreadProc | 0x14004C400 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XLoginServer | LoginServer.cpp | CLogThreadProc::OnFinalizeThread | 0x14004C3F0 | verified | IDA |

### XSeed 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XSeed::XSeed | 0x14003FD10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XSeed::~XSeed | 0x14003FD40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XSeed::Init | 0x140029BA0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XSeed::SetSeed | 0x14003FD70 | verified | IDA |
| XLoginServer | LoginServer.cpp | XSeed::GenTableForNumbers | 0x14003FE50 | verified | IDA |
| XLoginServer | LoginServer.cpp | XSeed::GetSeed | 0x140040000 | verified | IDA |
| XLoginServer | LoginServer.cpp | XRand<int> | 0x14001E4B0 | verified | IDA |

### XItemFactory 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XItemFactory::XItemFactory | 0x14008C390 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::~XItemFactory | 0x14008C320 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::Init | 0x14008B480 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::GeneratSerial | 0x14008B4C0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::nRand | 0x14008B5E0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::CreateItem | 0x14008BF30 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::GetItemTitle | 0x14008BB50 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::ReSetOption | 0x14008BC60 | verified | IDA |

### XResourceMgr 关键函数（LoginServer 专用）

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XResourceMgr::Init | 0x14007EA50 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_CHARACTER_INFO | 0x140007F00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_ITEM | 0x140007B20 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_CREATE_CLOTH | 0x1400078F0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_PROVIDE_ITEM | 0x1400076A0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_COMMON | 0x140032480 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_SYSTEMMAIL_ADD | 0x140006C10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetTB_BROACH_SET | 0x140011900 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::SetServerContents | 0x140029AE0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::GetServerContents | 0x140051B30 | verified | IDA |
| XLoginServer | LoginServer.cpp | XResourceMgr::CheckRankingTime | 0x140074640 | verified | IDA |

### UtilFunc 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | UtilFunc::CheckValidString | 0x140001910 | verified | IDA |
| XLoginServer | LoginServer.cpp | UtilFunc::IsUsableNameFilter | 0x140001DD0 | verified | IDA |

### LogHelper 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | LogHelper::LogDebug | 0x140042CE0 | verified | IDA |
| XLoginServer | LoginServer.cpp | LogHelper::LogError | 0x140042F20 | verified | IDA |
| XLoginServer | LoginServer.cpp | LogHelper::LogInfo | 0x140043160 | verified | IDA |

### STCharInfo / STCharInfoEx 结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | STCharInfo::STCharInfo | 0x140008800 | verified | IDA |
| XLoginServer | User.cpp | STCharInfo::~STCharInfo | 0x140008440 | verified | IDA |
| XLoginServer | User.cpp | STCharInfo::Init | 0x140008630 | verified | IDA |
| XLoginServer | User.cpp | STCharInfo::operator= | 0x140032980 | verified | IDA |
| XLoginServer | User.cpp | STCharInfo 拷贝构造 | 0x14000CAD0 | verified | IDA |
| XLoginServer | User.cpp | STCharInfoEx::STCharInfoEx | 0x140007080 | verified | IDA |
| XLoginServer | User.cpp | STCharInfoEx::~STCharInfoEx | 0x1400070C0 | verified | IDA |
| XLoginServer | User.cpp | STCharInfoEx::Init | 0x140008240 | verified | IDA |
| XLoginServer | User.cpp | STCharInfoEx 拷贝构造 | 0x14002F580 | verified | IDA |
| XLoginServer | User.cpp | operator<< STCharInfo | 0x140034890 | verified | IDA |
| XLoginServer | User.cpp | operator>> STCharInfo | 0x140034BF0 | verified | IDA |

### STUserInfo 结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | STUserInfo::STUserInfo | 0x140033770 | verified | IDA |
| XLoginServer | User.cpp | STUserInfo::~STUserInfo | 0x14002DE50 | verified | IDA |
| XLoginServer | User.cpp | STUserInfo::Init | 0x140033330 | verified | IDA |

### CUser 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterMapList | 0x140012830 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsLeagueMaster | 0x14002EDA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckCreateDate | 0x14002EFB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CanEnterGame | 0x14002F8B0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckChangeSlot | 0x14002F630 | verified | IDA |
| XLoginServer | User.cpp | CUser::ChangeCharacterSlot | 0x14002F2A0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckRepresentativeChange | 0x14002FC60 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SortCharacterList | 0x14002E6F0 | verified | IDA |

### XRelaySocket 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | RelaySocket.cpp | XRelaySocket::XRelaySocket | 0x14002A080 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::~XRelaySocket | 0x14002A5F0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::Init | 0x140043650 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SetMyInfo | 0x140043710 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::Connect | 0x140043840 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnConnect | 0x140043860 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnDisConnect | 0x140043870 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnNotConnect | 0x1400438A0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnParse | 0x1400438B0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::ServerProcess | 0x140043A40 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::UserProcess | 0x140043AF0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendAddServer | 0x140043CD0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendUpdateServerInfo | 0x140043D90 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::RecvServerUpdate | 0x140043E80 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendUserKickout | 0x140043F60 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::IsReady | 0x140044010 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnStartThread | 0x140044030 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::RecvUserTradePasswordState | 0x14002A130 | verified | IDA |

### TXServer<CUser> 模板函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | TXServer<CUser>::TXServer | 0x140019FB0 | verified | IDA |
| XCore | XServer.h | TXServer<CUser>::~TXServer | 0x14002C9C0 | verified | IDA |
| XCore | XServer.h | TXServer<CUser>::FindUser | 0x140001480 | verified | IDA |
| XCore | XServer.h | TXServer<CUser>::XCreator::Create | 0x14001B350 | verified | IDA |
| XCore | XServer.h | TXServer<CUser>::XCreator::XCreator | 0x14001B310 | verified | IDA |

### XIOCP 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | XIOCPSkeleton::XIOCPSkeleton | 0x14003DCC0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::~XIOCPSkeleton | 0x14003DD90 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::CreateIOCP | 0x14003DE00 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::BlockSocket | 0x14003E010 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::Shutdown | 0x14003E0F0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::WorkerThread | 0x14003E230 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::WriteLog | 0x14003DCB0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::XIOCPClient | 0x14003E680 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::~XIOCPClient | 0x14003E780 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::Init | 0x14003E7E0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::Shutdown | 0x14003E910 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::SetSocket | 0x14003E9F0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::OnEtcEvent | 0x14003EB30 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::Connect | 0x14003EC50 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::IsConnection | 0x14003EE50 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::Send | 0x14003EE60 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::Parsing | 0x14003EFB0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::DisConnect | 0x14003F260 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::OnRecv | 0x14003F500 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |
| XCore | XServer.h | XIOCPServer::XIOCPServer | 0x140040ED0 | verified | IDA |
| XCore | XServer.h | XIOCPServer::~XIOCPServer | 0x140040F90 | verified | IDA |
| XCore | XServer.h | XIOCPServer::Init | 0x140041170 | verified | IDA |
| XCore | XServer.h | XIOCPServer::Shutdown | 0x140041280 | verified | IDA |
| XCore | XServer.h | XIOCPServer::SetSocket | 0x140041030 | verified | IDA |
| XCore | XServer.h | XIOCPServer::CloseClient | 0x140041350 | verified | IDA |
| XCore | XServer.h | XIOCPServer::BackendThreadProc | 0x140041410 | verified | IDA |
| XCore | XServer.h | XIOCPServer::AcceptThread | 0x140041760 | verified | IDA |
| XCore | XServer.h | XIOCPServer::XAccept | 0x1400419F0 | verified | IDA |
| XCore | XServer.h | XIOCPServer::XSend | 0x140041B40 | verified | IDA |
| XCore | XServer.h | XIOCPServer::StartLog | 0x14002D550 | verified | IDA |
| XCore | XServer.h | XIOCPServer::PrintFPS | 0x1400416F0 | verified | IDA |

### XSocket 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | XSocket::XSocket | 0x14003E580 | verified | IDA |
| XCore | TXDBSocket.h | XSocket::~XSocket | 0x14003E600 | verified | IDA |
| XCore | TXDBSocket.h | XSocket::Init | 0x1400327C0 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::OnSend | 0x14003CE00 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |

### XClient 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | XClient::XClient | 0x140040050 | verified | IDA |
| XCore | XServer.h | XClient::~XClient | 0x140040180 | verified | IDA |
| XCore | XServer.h | XClient::Init | 0x140032870 | verified | IDA |
| XCore | XServer.h | XClient::Init (XIOCPServer) | 0x140040340 | verified | IDA |
| XCore | XServer.h | XClient::SetEncrypt | 0x1400321E0 | verified | IDA |
| XCore | XServer.h | XClient::IsBlock | 0x140012F30 | verified | IDA |
| XCore | XServer.h | XClient::IsState | 0x1400402D0 | verified | IDA |
| XCore | XServer.h | XClient::SetState | 0x1400402F0 | verified | IDA |
| XCore | XServer.h | XClient::ClearState | 0x140040300 | verified | IDA |
| XCore | XServer.h | XClient::Parse | 0x140040310 | verified | IDA |
| XCore | XServer.h | XClient::Register | 0x1400403D0 | verified | IDA |
| XCore | XServer.h | XClient::SendEx | 0x140040440 | verified | IDA |
| XCore | XServer.h | XClient::SendErrorMessage | 0x140040470 | verified | IDA |
| XCore | XServer.h | XClientPool::XClientPool | 0x140042280 | verified | IDA |
| XCore | XServer.h | XClientPool::~XClientPool | 0x140042320 | verified | IDA |
| XCore | XServer.h | XClientPool::AllocClient | 0x1400423A0 | verified | IDA |
| XCore | XServer.h | XClientPool::FreeClient | 0x140042500 | verified | IDA |
| XCore | XServer.h | XClientPool::GetHead | 0x140042670 | verified | IDA |
| XCore | XServer.h | XClientPool::GetNext | 0x140042770 | verified | IDA |

### XOption 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | Option.h | XOption::XOption | 0x1400458C0 | verified | IDA |
| XCore | Option.h | XOption::~XOption | 0x140045BA0 | verified | IDA |
| XCore | Option.h | XOption::Load | 0x140045C20 | verified | IDA |
| XCore | Option.h | XOption::ShowServerInfo | 0x140048B50 | verified | IDA |
| XCore | Option.h | XOption::GetOption | 0x1400083A0 | verified | IDA |
| XCore | Option.h | XOption::GetNationType | 0x1400083C0 | verified | IDA |
| XCore | Option.h | XOption::GetGroupID | 0x140012230 | verified | IDA |
| XCore | Option.h | XOption::GetPort | 0x140015A70 | verified | IDA |
| XCore | Option.h | XOption::GetPublicIP | 0x140015AB0 | verified | IDA |
| XCore | Option.h | XOption::GetSystemType | 0x140015DD0 | verified | IDA |
| XCore | Option.h | XOption::GetIP | 0x140016C60 | verified | IDA |
| XCore | Option.h | XOption::GetLogicThread | 0x140016C70 | verified | IDA |
| XCore | Option.h | XOption::GetMaxConnect | 0x140016C80 | verified | IDA |
| XCore | Option.h | XOption::GetChannel | 0x140016C90 | verified | IDA |
| XCore | Option.h | XOption::GetType | 0x140016CA0 | verified | IDA |
| XCore | Option.h | XOption::GetContentsOption | 0x140029B10 | verified | IDA |
| XCore | Option.h | XOption::GetResLoadType | 0x140029B30 | verified | IDA |
| XCore | Option.h | XOption::GetResFilePath | 0x140029B50 | verified | IDA |
| XCore | Option.h | XOption::GetDNS | 0x140029B70 | verified | IDA |
| XCore | Option.h | XOption::SetInitXignCode | 0x140029BD0 | verified | IDA |
| XCore | Option.h | XOption::GetSecurityType | 0x140029BF0 | verified | IDA |
| XCore | Option.h | XOption::GetServerType | 0x140049090 | verified | IDA |
| XCore | Option.h | XOption::GetServerPrivateIPAndPort | 0x1400491B0 | verified | IDA |
| XCore | Option.h | XOption::AddChannelInfo | 0x140049330 | verified | IDA |
| XCore | Option.h | XOption::AddChannelDistrict6Info | 0x1400494A0 | verified | IDA |
| XCore | Option.h | XOption::GetDBAgentInfo | 0x1400495D0 | verified | IDA |
| XCore | Option.h | XOption::GetAgentPrivateIPAndPort | 0x1400496A0 | verified | IDA |

### XPacket 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | XPacket.h | XPacket::GetMainCmd | 0x140012F60 | verified | IDA |
| XCommon | XPacket.h | XPacket::GetSubCmd | 0x1400089C0 | verified | IDA |

### XSendPacket 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | XPacket.h | XSendPacket::XSendPacket | 0x14003CB00 | verified | IDA |
| XCommon | XPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |
| XCommon | XPacket.h | XSendDBPacket::XSendDBPacket | 0x140015C10 | verified | IDA |

### XProcessComposite 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | XProcessComposite::Init | 0x140041F20 | verified | IDA |
| XCore | XServer.h | TXProcess<CUser>::Init | 0x140006110 | verified | IDA |

### XServer 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | XServer::GetOption | 0x1400083A0 | verified | IDA |
| XCore | XServer.h | XServer::GetCreatorPtr | 0x14002A710 | verified | IDA |
| XCore | XServer.h | XServer::OnAccect | 0x14004BC40 | verified | IDA |
| XCore | XServer.h | XServer::OnLogOut | 0x14004BCD0 | verified | IDA |

### CFSRWLock 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | GreenDamTan_PlatformCompat.h | CFSRWLock::CFSRWLock | 0x140029EB0 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFSRWLock::lock | 0x140029A80 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFSRWLock::unlock | 0x140029A40 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFSRWLock::lock_shared | 0x1400299C0 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFSRWLock::unlock_shared | 0x140029980 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFAutoSlimReadLock::CFAutoSlimReadLock | 0x1400299E0 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFAutoSlimWriteLock::CFAutoSlimWriteLock | 0x140029AA0 | verified | IDA |

### TXSingleton 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXSingleton.h | TXSingleton<XLoginServer>::Instance | 0x140001410 | verified | IDA |
| XCore | TXSingleton.h | TXSingleton<CLogThreadManager>::Instance | 0x14001A580 | verified | IDA |

### XOverLab 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | XOverLab::XOverLab | 0x14003D2C0 | verified | IDA |
| XCore | TXDBSocket.h | XOverLab::Init | 0x14003D240 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::XSend | 0x14003CC40 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::XRecv | 0x14003CD10 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::OnSend | 0x14003CE00 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | TXDBSocket.h | XTCPSkeleton::OnEtcEvent | 0x14003D220 | verified | IDA |
| XCore | TXDBSocket.h | XIOPool::AllocIO | 0x14003D390 | verified | IDA |
| XCore | TXDBSocket.h | XIOPool::FreeIO | 0x14003D470 | verified | IDA |

### TXProcess<CUser> 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | TXProcess<CUser>::TXProcess | 0x1400059A0 | verified | IDA |
| XCore | XServer.h | TXProcess<CUser>::~TXProcess | 0x1400059D0 | verified | IDA |
| XCore | XServer.h | TXProcess<CUser>::GetClientPtr | 0x140005960 | verified | IDA |
| XCore | XServer.h | TXProcess<CUser>::Init | 0x140006110 | verified | IDA |
| XCore | XServer.h | TXProcess<CUser>::Clear | 0x140006130 | verified | IDA |
| XCore | XServer.h | TXProcess<CUser>::SendErrorMessage | 0x140006150 | verified | IDA |
| XCore | XServer.h | IXProcess::IXProcess | 0x140006810 | verified | IDA |
| XCore | XServer.h | IXProcess::~IXProcess | 0x1400066A0 | verified | IDA |
| XCore | XServer.h | XProcessComposite::~XProcessComposite | 0x140041F00 | verified | IDA |
| XCore | XServer.h | XProcessComposite::Parse | 0x140042040 | verified | IDA |

### XResourceMgr TB_* Getter 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_SYSTEMMAIL_ADD | 0x140006C10 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM_CLASSIFY | 0x140007450 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_PROVIDE_ITEM | 0x1400076A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_CREATE_CLOTH | 0x1400078F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM | 0x140007B20 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_CHARACTER_INFO | 0x140007F00 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_APPEARANCE | 0x140008160 | verified | IDA |

### ST_* 结构体函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_MAP_INFO::ST_MAP_INFO | 0x140006CA0 | verified | IDA |
| XCommon | PSCommon.h | ST_PARTY_INFO::ST_PARTY_INFO | 0x140006D90 | verified | IDA |
| XCommon | PSCommon.h | ST_EQUIP_ITEM_INFO::ST_EQUIP_ITEM_INFO | 0x140008520 | verified | IDA |
| XCommon | PSCommon.h | ST_EQUIP_ITEM_INFO::Init | 0x140008600 | verified | IDA |
| XCommon | PSCommon.h | ST_TitleInfo::ST_TitleInfo | 0x140008540 | verified | IDA |
| XCommon | PSCommon.h | ST_CREATE_MAZE::~ST_CREATE_MAZE | 0x14000CE70 | verified | IDA |

### TXObjectMgr<CUser> 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | TXObjectMgr<CUser>::TXObjectMgr | 0x14001AA60 | verified | IDA |
| XCore | XServer.h | TXObjectMgr<CUser>::~TXObjectMgr | 0x14002C950 | verified | IDA |
| XCore | XServer.h | TXObjectMgr<CUser>::Find | 0x1400014B0 | verified | IDA |
| XCore | XServer.h | TXObjectMgr<CUser>::Create | 0x14001E280 | verified | IDA |
| XCore | XServer.h | TXObjectMgr<CUser>::Init | 0x14002AB70 | verified | IDA |
| XCore | XServer.h | TXObjectMgr<CUser>::Delete | 0x14002B5A0 | verified | IDA |
| XCore | XServer.h | TXObjectMgr<CUser>::Clear | 0x14002C910 | verified | IDA |

### IXObject / IXObjectMgr 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | IXObject.h | IXObjectMgr::IXObjectMgr | 0x140029480 | verified | IDA |
| XCore | IXObject.h | IXObjectMgr::GetSessionID | 0x140029100 | verified | IDA |
| XCore | IXObject.h | TXPool<IXObject>::TXPool | 0x14001B7B0 | verified | IDA |
| XCore | IXObject.h | TXPool<IXObject>::Pop | 0x14001F800 | verified | IDA |
| XCore | IXObject.h | TXPool<IXObject>::GetFullSize | 0x140020110 | verified | IDA |
| XCore | IXObject.h | TXPool<IXObject>::GetCurSize | 0x140020130 | verified | IDA |
| XCore | IXObject.h | TXCreator<CUser>::Create | 0x14002A770 | verified | IDA |
| XCore | IXObject.h | TXMap<int,IXObject*>::SetAt | 0x14001ED90 | verified | IDA |
| XCore | IXObject.h | TXMap<int,IXObject*>::GetAt | 0x140001500 | verified | IDA |

### PS_* 包结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSServer.h | PS_KICK_USER_INFO::PS_KICK_USER_INFO | 0x1400016F0 | verified | IDA |
| XCommon | PSServer.h | PS_DB_CHARACTER_CREATE::PS_DB_CHARACTER_CREATE | 0x140003A40 | verified | IDA |
| XCommon | PSServer.h | PS_DB_CHARACTER_CREATE::~PS_DB_CHARACTER_CREATE | 0x140003A70 | verified | IDA |
| XCommon | PSServer.h | PS_DEFAULT_INVEN_ITEMS::PS_DEFAULT_INVEN_ITEMS | 0x140003A90 | verified | IDA |
| XCommon | PSServer.h | PS_DEFAULT_INVEN_ITEM::PS_DEFAULT_INVEN_ITEM | 0x140003AD0 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_REPRESENTATIVE_CHANGE::PS_CHARACTER_REPRESENTATIVE_CHANGE | 0x140006850 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_CHANGE_SLOT::PS_CHARACTER_CHANGE_SLOT | 0x140006980 | verified | IDA |
| XCommon | PSServer.h | PS_SECOND_PW_RES::PS_SECOND_PW_RES | 0x1400069F0 | verified | IDA |
| XCommon | PSServer.h | PS_SECOND_PW_REQ::PS_SECOND_PW_REQ | 0x140006A10 | verified | IDA |
| XCommon | PSServer.h | PS_REQ_CHECK_NAME::PS_REQ_CHECK_NAME | 0x140006A30 | verified | IDA |
| XCommon | PSServer.h | PS_ENTER_MAP_RES::PS_ENTER_MAP_RES | 0x140006DB0 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_SELECT::PS_CHARACTER_SELECT | 0x140006E10 | verified | IDA |
| XCommon | PSServer.h | PS_CONTENTS_INFO::PS_CONTENTS_INFO | 0x140006F00 | verified | IDA |
| XCommon | PSServer.h | PS_GAME_GUARD_ERROR::PS_GAME_GUARD_ERROR | 0x140006F40 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_MAP_LIST::PS_CHARACTER_MAP_LIST | 0x14000AA30 | verified | IDA |
| XCommon | PSServer.h | PS_BROACH_SHAPE_LIST::~PS_BROACH_SHAPE_LIST | 0x14000AA60 | verified | IDA |

### nRand 随机数函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::nRand | 0x140018A00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::nRand | 0x14008B5E0 | verified | IDA |

### ST_LOG_GAME 日志结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_LOG_GAME::ST_LOG_GAME | 0x1400121B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB (ST_LOG_GAME) | 0x140019860 | verified | IDA |
| XCommon | XPacket.h | operator<< ST_LOG_GAME | 0x140039E30 | verified | IDA |

### BridgeSend / SendPacket 相关函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::BridgeSend | 0x14002E540 | verified | IDA |

### LogHelper 日志辅助函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | GreenDamTan_LogHelper.h | LogHelper::LogDebug | 0x140042CE0 | verified | IDA |
| XCore | GreenDamTan_LogHelper.h | LogHelper::LogError | 0x140042F20 | verified | IDA |
| XCore | GreenDamTan_LogHelper.h | LogHelper::LogInfo | 0x140043160 | verified | IDA |

### XResourceMgr Init/Load 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::XResourceMgr | 0x140089820 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::~XResourceMgr | 0x140088110 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::Init | 0x14007EA50 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::Load | 0x14008B3A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitGameDB | 0x140051D80 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitCharacterInfo | 0x1400744F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CheckRankingTime | 0x140074640 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::SetServerContents | 0x140029AE0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetServerContents | 0x140051B30 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::ServerOptionLoad | 0x140051740 | verified | IDA |

### CFAutoSlimLock 析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | GreenDamTan_PlatformCompat.h | CFAutoSlimReadLock::~CFAutoSlimReadLock | 0x1400299A0 | verified | IDA |
| XCore | GreenDamTan_PlatformCompat.h | CFAutoSlimWriteLock::~CFAutoSlimWriteLock | 0x140029A60 | verified | IDA |

### XSeed 随机种子函数（详细）

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XSeed.h | XSeed::XSeed | 0x14003FD10 | verified | IDA |
| XCore | XSeed.h | XSeed::~XSeed | 0x14003FD40 | verified | IDA |
| XCore | XSeed.h | XSeed::Init | 0x140029BA0 | verified | IDA |
| XCore | XSeed.h | XSeed::SetSeed | 0x14003FD70 | verified | IDA |
| XCore | XSeed.h | XSeed::GenTableForNumbers | 0x14003FE50 | verified | IDA |
| XCore | XSeed.h | XSeed::GetSeed | 0x140040000 | verified | IDA |
| XCore | XSeed.h | XRand<int> | 0x14001E4B0 | verified | IDA |

### XItemFactory 道具工厂函数（详细）

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XItemFactory::XItemFactory | 0x14008C390 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::~XItemFactory | 0x14008C320 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::Init | 0x14008B480 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::GeneratSerial | 0x14008B4C0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::nRand | 0x14008B5E0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::GetItemTitle | 0x14008BB50 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::ReSetOption | 0x14008BC60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::CreateItem | 0x14008BF30 | verified | IDA |

### CLogThreadManager 日志线程管理函数（详细）

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::CLogThreadManager | 0x14004C570 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::~CLogThreadManager | 0x14004CB20 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::Start | 0x14004CD30 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::End | 0x14004CF60 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::DoJob | 0x14004D1B0 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::GetLogger | 0x14004CBC0 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadManager::CreateWorkerThread | 0x14004D290 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | TXSingleton<CLogThreadManager>::Instance | 0x14001A580 | verified | IDA |

### CLogThreadProc 日志线程过程函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | GreenDamTan_LogThread.h | CLogThreadProc::CLogThreadProc | 0x14004C1E0 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadProc::~CLogThreadProc | 0x14004C280 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadProc::ThreadProc | 0x14004C400 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | GreenDamTan_LogThread.h | CLogThreadProc::OnFinalizeThread | 0x14004C3F0 | verified | IDA |

### CFThread<CLogThreadProc> 模板函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | GreenDamTan_Thread.h | CFThread<CLogThreadProc>::Create | 0x14004D600 | verified | IDA |
| XCore | GreenDamTan_Thread.h | CFThread<CLogThreadProc>::RunThread | 0x14004DD20 | verified | IDA |
| XCore | GreenDamTan_Thread.h | CFThread<CLogThreadProc>::CallThreadHandlerProc | 0x14004E400 | verified | IDA |

### Init 函数总览

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | ServiceInit | 0x14002CCC0 | verified | IDA |
| XCore | XServer.h | XServer::Init | 0x14004BCF0 | verified | IDA |
| XCore | XServer.h | XIOCPServer::Init | 0x140041170 | verified | IDA |
| XCore | XServer.h | XIOCPClient::Init | 0x14003E7E0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Init | 0x140012CF0 | verified | IDA |
| XCore | TXDBSocket.h | XOverLab::Init | 0x14003D240 | verified | IDA |
| XLoginServer | User.cpp | CUser::Init | 0x14002E050 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::InitServer | 0x140017960 | verified | IDA |
| XLoginServer | LoginServer.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |

### Shutdown 函数总览

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | XServer::Shutdown | 0x14004BEF0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::Shutdown | 0x14003E0F0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPClient::Shutdown | 0x14003E910 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPServer::Shutdown | 0x140041280 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Shutdown | 0x140012DA0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::SetSafetyShutdown | 0x140013190 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | RecvServerShutDown | 0x140016370 | verified | IDA |

### OnUpdate 函数总览

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XCore | XServer.h | XServer::OnUpdate | 0x14004BF60 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnUpdate | 0x14008D5B0 | verified | IDA |

### XDB 数据库抽象层函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XDB.h | XDBStmt::Init | 0x14004F350 | verified | IDA |
| XCore | XDB.h | XDBManager::Init | 0x14004F8D0 | verified | IDA |
| XCore | XDB.h | XDBConnect::Init | 0x14004FFA0 | verified | IDA |
| XCore | XDB.h | XDBEnv::Init | 0x140050650 | verified | IDA |

### STItem 道具结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | STItem::STItem | 0x140007CD0 | verified | IDA |
| XCommon | PSCommon.h | STItem::Init | 0x140007B90 | verified | IDA |
| XCommon | XPacket.h | operator<< STItem | 0x140037490 | verified | IDA |
| XCommon | XPacket.h | operator>> STItem | 0x140037720 | verified | IDA |

### STPosInfo 位置信息结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | STPosInfo::STPosInfo | 0x140007010 | verified | IDA |
| XCommon | PSCommon.h | STPosInfo::Init | 0x1400081F0 | verified | IDA |
| XCommon | PSCommon.h | STPosInfo::operator= | 0x140032E20 | verified | IDA |
| XCommon | XPacket.h | operator<< STPosInfo | 0x1400344E0 | verified | IDA |
| XCommon | XPacket.h | operator>> STPosInfo | 0x140034560 | verified | IDA |

### STMyCharInfoEx 扩展角色信息结构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | STMyCharInfoEx::STMyCharInfoEx | 0x1400070E0 | verified | IDA |
| XCommon | PSCommon.h | STMyCharInfoEx::~STMyCharInfoEx | 0x140006FB0 | verified | IDA |
| XCommon | PSCommon.h | STMyCharInfoEx::Init | 0x140008290 | verified | IDA |
| XCommon | PSCommon.h | STMyCharInfoEx 拷贝构造 | 0x14002F360 | verified | IDA |

### STEquipBase 装备基类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | STEquipBase::Init | 0x1400085E0 | verified | IDA |

### Kickout 踢人函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::Kickout | 0x14002EB10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::KickoutAll | 0x140018290 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | RecvUserKickout | 0x1400162E0 | verified | IDA |
| XCore | XRelaySocket.h | SendUserKickout | 0x140043F60 | verified | IDA |

### SendError 错误发送函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XProcess.h | TXProcess<CUser>::SendErrorMessage | 0x140006150 | verified | IDA |
| XCore | XClient.h | XClient::SendErrorMessage | 0x140040470 | verified | IDA |

### WriteLog 日志函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLog | 0x140019640 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB (长参数版) | 0x1400196D0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB (ST_LOG_GAME版) | 0x140019860 | verified | IDA |
| XCore | TXServer.h | XTCPSkeleton::WriteLog | 0x14002A6F0 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::WriteLog | 0x14003DCB0 | verified | IDA |

### 网络事件回调函数 (OnConnect/OnRecv/OnSend/OnDisconnect)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnConnect | 0x140012EC0 | verified | IDA |
| XCore | XClient.h | XIOCPClient::OnConnect | 0x14003F320 | verified | IDA |
| XCore | XRelaySocket.h | XRelaySocket::OnConnect | 0x140043860 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnConnect | 0x14008CCB0 | verified | IDA |
| XCore | TXServer.h | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | XClient.h | XIOCPClient::OnRecv | 0x14003F500 | verified | IDA |
| XCore | TXServer.h | XTCPSkeleton::OnSend | 0x14003CE00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnDisConnect | 0x14000DAD0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnDisConnect | 0x140012E80 | verified | IDA |
| XCore | XClient.h | XIOCPClient::OnDisConnect | 0x14003F330 | verified | IDA |
| XCore | XRelaySocket.h | XRelaySocket::OnDisConnect | 0x140043870 | verified | IDA |
| XLoginServer | LoginServer.cpp | CObserveSocket::OnDisConnect | 0x14008CCD0 | verified | IDA |

### XResourceMgr 表访问函数 (GetTB)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | GetTB_SYSTEMMAIL_ADD | 0x140006C10 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_ITEM_CLASSIFY | 0x140007450 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_PROVIDE_ITEM | 0x1400076A0 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_CREATE_CLOTH | 0x1400078F0 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_ITEM | 0x140007B20 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_CHARACTER_INFO | 0x140007F00 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_APPEARANCE | 0x140008160 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_BROACH_SET | 0x140011900 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_COMMON | 0x140032480 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_COSTUME_SOCKET | 0x14008B840 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_ITEM_TITLE | 0x14008B8B0 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_ITEM_TITLE_GROUP | 0x14008B920 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_ITEM_TITLE_VALUE | 0x14008B990 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_RANDOM_OPTION | 0x14008BA00 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_REINFORCE | 0x14008BA70 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_SOCKET | 0x14008BAE0 | verified | IDA |

### 角色创建函数 (Create)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XCore | TXServer.h | TXPool::Create (XOverLab) | 0x14003D710 | verified | IDA |
| XCore | TXServer.h | TXServer::Create (CUser) | 0x14001B350 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::CreateNode | 0x1400207A0 | verified | IDA |
| XCore | IXObject.h | TXObjectMgr::Create | 0x14001E280 | verified | IDA |
| XCore | XServer.h | XIOCPSkeleton::CreateIOCP | 0x14003DE00 | verified | IDA |

### 角色删除函数 (Delete)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterDelete | 0x14000B920 | verified | IDA |
| XCore | IXObject.h | TXObjectMgr::Delete | 0x14002B5A0 | verified | IDA |
| XLoginServer | User.cpp | DeleteCharacterInfo | 0x14002E650 | verified | IDA |

### XSendPacket 发包函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XPacket.h | XSendPacket::XSendPacket | 0x14003CB00 | verified | IDA |
| XCore | XPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |
| XCore | TXServer.h | XTCPSkeleton::XSend | 0x14003CC40 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Send | 0x14003EE60 | verified | IDA |
| XCore | XClient.h | XClient::SendEx | 0x140040440 | verified | IDA |
| XCore | XServer.h | XIOCPServer::XSend | 0x140041B40 | verified | IDA |
| XLoginServer | User.cpp | CUser::BridgeSend | 0x14002E540 | verified | IDA |

### XSendDBPacket DB发包函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XPacket.h | XSendDBPacket::XSendDBPacket (IXObject*) | 0x1400071D0 | verified | IDA |
| XCore | XPacket.h | XSendDBPacket::XSendDBPacket (H/E/E) | 0x140029530 | verified | IDA |
| XCore | XPacket.h | XSendDBPacket::XSendDBPacket (with XPacket) | 0x140015C10 | verified | IDA |
| XCore | XPacket.h | XSendDBPacket::~XSendDBPacket | 0x1400071B0 | verified | IDA |
| XCore | XPacket.h | XSendDBPacket::GetOrderID | 0x140029510 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | SendAccountDBAgent | 0x1400147D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | SendGameDBAgent | 0x140014850 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | SendLogDBAgent | 0x1400148D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | SendStatisticsDBAgent | 0x140014950 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBAccount | 0x140019900 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBGame | 0x1400199B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBLog | 0x140019A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBStatistics | 0x140019AF0 | verified | IDA |

### Session 会话函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetSendCheckSessionID | 0x1400088E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetAuthSessionID | 0x140008900 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID | 0x140008950 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSendCheckSessionID | 0x140016BF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetAuthSessionID_Wait | 0x140029850 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID_Wait | 0x140029900 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XCore | IXObject.h | IXObjectMgr::GetSessionID | 0x140029100 | verified | IDA |
| XCore | IXObject.h | IXObject::SetSessionID | 0x1400337F0 | verified | IDA |

### XIOCPClient 异步IO客户端函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XClient.h | XIOCPClient::XIOCPClient | 0x14003E680 | verified | IDA |
| XCore | XClient.h | XIOCPClient::~XIOCPClient | 0x14003E780 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Init | 0x14003E7E0 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Shutdown | 0x14003E910 | verified | IDA |
| XCore | XClient.h | XIOCPClient::SetSocket | 0x14003E9F0 | verified | IDA |
| XCore | XClient.h | XIOCPClient::OnEtcEvent | 0x14003EB30 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Connect | 0x14003EC50 | verified | IDA |
| XCore | XClient.h | XIOCPClient::IsConnection | 0x14003EE50 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Parsing | 0x14003EFB0 | verified | IDA |
| XCore | XClient.h | XIOCPClient::DisConnect | 0x14003F260 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Close | 0x14003F2A0 | verified | IDA |
| XCore | XClient.h | XIOCPClient::OnNotConnect | 0x14003F360 | verified | IDA |
| XCore | XClient.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |

### XClient 客户端基类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XClient.h | XClient::XClient | 0x140040050 | verified | IDA |
| XCore | XClient.h | XClient::~XClient | 0x140040180 | verified | IDA |
| XCore | XClient.h | XClient::IsState | 0x1400402D0 | verified | IDA |
| XCore | XClient.h | XClient::SetState | 0x1400402F0 | verified | IDA |
| XCore | XClient.h | XClient::ClearState | 0x140040300 | verified | IDA |
| XCore | XClient.h | XClient::Parse | 0x140040310 | verified | IDA |
| XCore | XClient.h | XClient::Init | 0x140040340 | verified | IDA |
| XCore | XClient.h | XClient::Register | 0x1400403D0 | verified | IDA |
| XCore | XClient.h | XClient::IsBlock | 0x140012F30 | verified | IDA |

### XClientPool 客户端池函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | XClientPool::XClientPool | 0x140042280 | verified | IDA |
| XCore | XServer.h | XClientPool::AllocClient | 0x1400423A0 | verified | IDA |
| XCore | XServer.h | XClientPool::FreeClient | 0x140042500 | verified | IDA |

### XProcessComposite 处理器组合函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XProcess.h | XProcessComposite::Init | 0x140041F20 | verified | IDA |

### GameGuard / SG认证函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | SystemProcess.cpp | ReqSystemGameGuardAuth | 0x14002DAE0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSGAuthInfo | 0x14002DBE0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSGAuthType | 0x14002DC30 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResSGAuthInfoLoad | 0x14000E4F0 | verified | IDA |
| XCommon | PSOption.h | PS_GAME_GUARD_AUTH (ctor) | 0x14002DBB0 | verified | IDA |
| XCommon | PSOption.h | ST_SG_AUTH_INFO (ctor) | 0x14002DC50 | verified | IDA |
| XCommon | PSOption.h | ST_GF_AUTH_INFO (ctor) | 0x140033370 | verified | IDA |

### XLoginProcess 登录处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginProcess.cpp | XLoginProcess::XLoginProcess | 0x140014A40 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::~XLoginProcess | 0x140014B00 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::Parse | 0x140014B30 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginProcess.cpp | SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqEnterWaitCancel | 0x140015950 | verified | IDA |

### 角色选择函数 (SelectCharacter)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSelectUCID | 0x140011F90 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetLastSelectUCID | 0x140012030 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetLastSelectUCID | 0x140032730 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_SELECT (ctor) | 0x140006E10 | verified | IDA |

### 角色列表函数 (CharacterList)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::SortCharacterList | 0x14002F1D0 | verified | IDA |

### 进入服务器函数 (EnterServer)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetEnterServerState | 0x140006C80 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEnterServerState | 0x140008420 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | LoginProcess.cpp | ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | RecvEnterServer | 0x140016730 | verified | IDA |

### 服务器列表函数 (ServerList)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginProcess.cpp | SendServerList | 0x1400150B0 | verified | IDA |

### 二级密码函数 (SecondPW)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetSecondPWState | 0x1400069D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSecondPWState | 0x140011B60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResSecondPWContinue | 0x14000DFE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResSecondPWStateCheck | 0x14000E420 | verified | IDA |
| XLoginServer | LoginServer.cpp | SendServerOption_SecondPW | 0x140019CA0 | verified | IDA |

### 角色槽位交换函数 (ChangeSlot)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitChangeSlotPacketRes | 0x140006940 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetWaitChangeSlotPacketRes | 0x140006960 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckChangeSlot | 0x14002F630 | verified | IDA |

### 代表角色函数 (Representative)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterRepresentativeChange | 0x14000BFD0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeUCID | 0x140006EC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeUCID | 0x140012340 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeCheck | 0x140006900 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeCheck | 0x140012010 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeCheckResult | 0x1400068E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeCheckResult | 0x140011FF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetLastRepresentativeCharTime | 0x140032750 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetLastRepresentativeCharTime | 0x14002FBD0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckRepresentativeChange | 0x14002FC60 | verified | IDA |

### 角色名检查函数 (CheckName)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterCheckName | 0x14000CEA0 | verified | IDA |

### 角色换服函数 (ChangeServer)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterChangeServer | 0x14000DD70 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | RecvUserChangeServer | 0x140016600 | verified | IDA |

### CSystemProcess 系统处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | SystemProcess.cpp | CSystemProcess::CSystemProcess | 0x14002D570 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::~CSystemProcess | 0x14002D630 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::Parse | 0x14002D660 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqSystemXigncode | 0x14002D800 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqSystemXigncodeError | 0x14002D8D0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqSystemKeepAlive | 0x14002D910 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqSystemGameGuardAuth | 0x14002DAE0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | ReqSystemGameGuardError | 0x14002DB60 | verified | IDA |

### XResourceMgr 核心加载函数 (Load_)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.cpp | XResourceMgr::Load | 0x14008B3A0 | verified | IDA |
| XSCommon | DBLoadTable.cpp | InitCommonDB | 0x14008ADF0 | verified | IDA |
| XSCommon | DBLoadTable.cpp | CashshopLoad_DBConnect | 0x140082D10 | verified | IDA |
| XSCommon | DBLoadTable.cpp | CashShopTabLoad_DBConnect | 0x14008AD10 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_APPEARANCE | 0x1400C6CC0 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_BROACH_SET | 0x1400C8920 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_BUFF | 0x1400C9C60 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_CASHSHOP | 0x1400CAC80 | verified | IDA |

### 角色地图更新函数 (UpdateMap)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |

### Broach 饰品效果函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | BroachEffect | 0x14000E640 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_BROACH_SET | 0x140011900 | verified | IDA |
| XCommon | PSCommon.h | PS_BROACH_SHAPE_LIST (dtor) | 0x14000AA60 | verified | IDA |

### Maze 迷宫相关函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginControlSocket.cpp | RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XCore | XRelaySocket.h | SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XCommon | PSWorld.h | ST_CREATE_MAZE (ctor) | 0x140011ED0 | verified | IDA |
| XCommon | PSWorld.h | ST_CREATE_MAZE (dtor) | 0x14000CE70 | verified | IDA |

### 计数/统计函数 (Count/Tick)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetCharacterCount | 0x1400083E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterCount | 0x1400122E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | LoginServer.cpp | UpdateMaxUserCount | 0x140019120 | verified | IDA |
| XLoginServer | LoginServer.cpp | CheckUserWaitCountSend | 0x140019170 | verified | IDA |
| XLoginServer | LoginServer.cpp | AddSendGameDBUserCount | 0x140019280 | verified | IDA |
| XLoginServer | LoginServer.cpp | GetSendGameDBUserCount | 0x1400192F0 | verified | IDA |
| XLoginServer | LoginServer.cpp | SendServerGroupList (角色计数版) | 0x140019340 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | GetStatisticsDBAgentCount | 0x140014A10 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | GetGameDBAgentCount | 0x140014A20 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | GetAccountDBAgentCount | 0x140014A30 | verified | IDA |
| XCore | XTime.h | XTime::GetTickCount | 0x1400131B0 | verified | IDA |
| XCommon | XCommon.h | ATL::CTime::GetTickCount | 0x140012240 | verified | IDA |

### 交易密码函数 (TradePassword)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetTradePWState | 0x140011B40 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetTradePWState | 0x140011FD0 | verified | IDA |
| XCore | XRelaySocket.h | RecvUserTradePasswordState | 0x14002A130 | verified | IDA |

### 二级密码验证函数 (SecondPassword)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | IsValidSecondPassword | 0x140004D80 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckSecondPasswordState | 0x14002EC90 | verified | IDA |

### XOption 配置访问函数 (补充)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | XServer::GetOption | 0x1400083A0 | verified | IDA |
| XCore | Option.h | XOption::GetNationType | 0x1400083C0 | verified | IDA |
| XCore | Option.h | XOption::GetGroupID | 0x140012230 | verified | IDA |
| XCore | Option.h | XOption::GetPort | 0x140015A70 | verified | IDA |
| XCore | Option.h | XOption::GetPublicIP | 0x140015AB0 | verified | IDA |
| XCore | Option.h | XOption::GetSystemType | 0x140015DD0 | verified | IDA |
| XCore | Option.h | XOption::GetIP | 0x140016C60 | verified | IDA |
| XCore | Option.h | XOption::GetLogicThread | 0x140016C70 | verified | IDA |
| XCore | Option.h | XOption::GetMaxConnect | 0x140016C80 | verified | IDA |
| XCore | Option.h | XOption::GetChannel | 0x140016C90 | verified | IDA |
| XCore | Option.h | XOption::GetType | 0x140016CA0 | verified | IDA |
| XCore | Option.h | XOption::GetContentsOption | 0x140029B10 | verified | IDA |
| XCore | Option.h | XOption::GetResLoadType | 0x140029B30 | verified | IDA |
| XCore | Option.h | XOption::GetResFilePath | 0x140029B50 | verified | IDA |
| XCore | Option.h | XOption::GetDNS | 0x140029B70 | verified | IDA |
| XCore | Option.h | XOption::SetInitXignCode | 0x140029BD0 | verified | IDA |
| XCore | Option.h | XOption::GetSecurityType | 0x140029BF0 | verified | IDA |
| XCommon | PSOption.h | ST_OPTION_BIT (ctor) | 0x140011A20 | verified | IDA |
| XCommon | PSOption.h | ST_USER_KEY_OPTION (ctor) | 0x1400119F0 | verified | IDA |

### Contents 内容配置函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::SetServerContents | 0x140029AE0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetServerContents (填充版) | 0x140051B30 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetServerContents (查询版) | 0x140051B70 | verified | IDA |
| XCommon | PSOption.h | PS_CONTENTS_INFO (ctor) | 0x140006F00 | verified | IDA |

### League 公会函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::CheckLeagueMaster | 0x14002ED40 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearLeagueInfo | 0x14002ED70 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsLeagueMaster | 0x14002EDA0 | verified | IDA |
| XCommon | PSCommon.h | STLeagueInfo (ctor) | 0x140008570 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_LEAGUE_INFO | 0x1400EC3A0 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_LEAGUE_SKILL | 0x1400EC7C0 | verified | IDA |

### Echelon 阶梯等级函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetEchelonExp | 0x140012300 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEchelonLevel | 0x140012320 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonLevel | 0x14002ECC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonExp | 0x14002ED00 | verified | IDA |
| XSCommon | DBLoadTable.cpp | Load_TB_ECHELON | 0x1400DBA60 | verified | IDA |

### Party 组队函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_PARTY_INFO (ctor) | 0x140006D90 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPartyRevise | 0x140082470 | verified | IDA |

### Mail 邮件函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::InitLevelUpMail | 0x1400875F0 | verified | IDA |
| XSCommon | DBLoadTable.h | GetTB_SYSTEMMAIL_ADD | 0x140006C10 | verified | IDA |

### Buff 效果函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | STBuffInfo::STBuffInfo | 0x140011560 | verified | IDA |
| XCommon | XPacket.h | operator<< STBuffInfo | 0x140035A20 | verified | IDA |
| XCommon | XPacket.h | operator>> STBuffInfo | 0x140035AC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BUFF | 0x1400C9C60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_COMBO_BUFF | 0x1400CF580 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_BUFF | 0x140124DF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_COMBO_BUFF | 0x140129D30 | verified | IDA |
| XSCommon | DBLoadTable.h | std::map<uint16_t, TB_BUFF> ctor | 0x14007EF60 | verified | IDA |
| XSCommon | DBLoadTable.h | std::map<uint16_t, TB_BUFF> dtor | 0x14007D510 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_BUFF>::operator[] | 0x1400BE910 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_COMBO_BUFF>::_Buynode | 0x140090370 | verified | IDA |
| XSCommon | DBLoadTable.h | vector<STBuffInfo>::operator= | 0x1400125F0 | verified | IDA |
| XSCommon | DBLoadTable.h | vector<STBuffInfo>::erase | 0x140012B90 | verified | IDA |
| XSCommon | DBLoadTable.h | vector<STBuffInfo>::operator[] | 0x1400302B0 | verified | IDA |

### Title 称号函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_TitleInfo::ST_TitleInfo | 0x140008540 | verified | IDA |
| XCommon | XPacket.h | operator>> ST_TitleInfo | 0x140035E40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::GetItemTitle | 0x14008BB50 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM_TITLE | 0x14008B8B0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM_TITLE_GROUP | 0x14008B920 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM_TITLE_VALUE | 0x14008B990 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitTitleOpenCondition | 0x14007B260 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE | 0x1400EAC20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE_CHANGE | 0x1400EB360 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE_GROUP | 0x1400EB770 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE_VALUE | 0x1400EBEB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_TITLE_INFO | 0x140119430 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_TITLE_REWARD | 0x140119BF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_TITLE_STRING | 0x14011A090 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE | 0x140143600 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE_CHANGE | 0x140143C90 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE_GROUP | 0x140143FE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE_VALUE | 0x140144670 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_TITLE_INFO | 0x14016F740 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_TITLE_REWARD | 0x14016FEC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_TITLE_STRING | 0x140170280 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ITEM_TITLE_GROUP>::operator[] | 0x1400BE570 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ITEM_TITLE_VALUE>::operator[] | 0x1400C0930 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_TITLE_INFO>::operator[] | 0x1400C2D30 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_TITLE_REWARD>::operator[] | 0x1400C2E10 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ITEM_TITLE>::_Linsert | 0x1400A2B10 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_TITLE_INFO>::_Linsert | 0x1400A97A0 | verified | IDA |

### Achievement 成就函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | Load_TB_ACHIEVEMENT | 0x1400C3390 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ACHIEVEMENT_BEGIN | 0x1400C3A80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ACHIEVEMENT_EMBLEM | 0x1400C3E20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ACHIEVEMENT | 0x14011F3F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ACHIEVEMENT_BEGIN | 0x14011FA50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ACHIEVEMENT_EMBLEM | 0x14011FD20 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT>::operator[] | 0x1400BE3B0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT>::_Buynode | 0x14008D6F0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT>::_Insert | 0x1400AA460 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT>::_Linsert | 0x1400A09D0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT>::clear | 0x140070F80 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT>::erase | 0x1400748E0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT_BEGIN>::_Buynode | 0x14008ED70 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT_EMBLEM>::_Buynode | 0x14008F270 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT_EMBLEM>::_Insert | 0x1400B1A50 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT> ctor | 0x14007EC00 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ACHIEVEMENT> dtor | 0x14007D2D0 | verified | IDA |

### Random 随机函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XSeed.h | XSeed::Init | 0x140029BA0 | verified | IDA |
| XCore | XSeed.h | XSeed::SetSeed | 0x14003FD70 | verified | IDA |
| XCore | XSeed.h | XSeed::GenTableForNumbers | 0x14003FE50 | verified | IDA |
| XCore | XSeed.h | XSeed::GetSeed | 0x140040000 | verified | IDA |
| XCore | XSeed.h | XRand<int> | 0x14001E4B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::nRand | 0x140018A00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::nRand | 0x14008B5E0 | verified | IDA |
| XSCommon | DBLoadTable.h | std::_Random_shuffle | 0x14008B650 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_RANDOM_OPTION | 0x140109790 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_RANDOM_GET | 0x1401076C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_RANDOM_GET_GROUP | 0x140108D80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_RANDOM_CHANGE | 0x1401071C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_RANDOMBOX | 0x1400E78F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_AKASHIC_RANDOM_GROUP | 0x1400C5050 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_AKASHIC_RANDOM_GROUP_IN | 0x1400C5790 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_RANDOM_OPTION | 0x140160480 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_RANDOM_GET | 0x14015E070 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_RANDOM_GET_GROUP | 0x14015FAF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_RANDOM_CHANGE | 0x14015DC40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_RANDOMBOX | 0x140140090 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_AKASHIC_RANDOM_GROUP | 0x140120C40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_AKASHIC_RANDOM_GROUP_IN | 0x1401212C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_RANDOM_OPTION | 0x14008BA00 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_RANDOM_GET>::operator[] | 0x1400C1E30 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_RANDOM_OPTION>::operator[] | 0x1400C1F10 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ITEM_RANDOMBOX>::operator[] | 0x1400C0670 | verified | IDA |

### Socket 网络函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | XSocket::XSocket | 0x14003E580 | verified | IDA |
| XCore | TXDBSocket.h | XSocket::~XSocket | 0x14003E600 | verified | IDA |
| XCore | TXDBSocket.h | XSocket::Init | 0x1400327C0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Init | 0x140012CF0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Shutdown | 0x140012DA0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnConnect | 0x140012EC0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnDisConnect | 0x140012E80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnParse | 0x140012F80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::AutoConnect | 0x1400149D0 | verified | IDA |
| XCore | XServer.h | XIOCPServer::SetSocket | 0x140041030 | verified | IDA |
| XCore | XClient.h | XIOCPClient::SetSocket | 0x14003E9F0 | verified | IDA |
| XCore | XServer.h | XTCPSkeleton::OnSend | 0x14003CE00 | verified | IDA |
| XCore | XServer.h | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | TXDBSocket.h | XIOCPSkeleton::BlockSocket | 0x14003E010 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::XGameDBSocket | 0x140009FA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::~XGameDBSocket | 0x140009FF0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnDisConnect | 0x14000DAD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnNotConnect | 0x14000DBA0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140014170 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x1400143E0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::XRelaySocket | 0x14002A080 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::~XRelaySocket | 0x14002A5F0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::Connect | 0x140043840 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::CLoginControlSocket | 0x14002A680 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::~CLoginControlSocket | 0x140029F40 | verified | IDA |

### Packet 包处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | XPacket.h | XPacket::GetMainCmd | 0x140012F60 | verified | IDA |
| XCommon | XPacket.h | XPacket::GetSubCmd | 0x1400089C0 | verified | IDA |
| XCommon | XPacket.h | XSendPacket::XSendPacket | 0x14003CB00 | verified | IDA |
| XCommon | XPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |
| XCommon | XPacket.h | XSendDBPacket::XSendDBPacket | 0x140015C10 | verified | IDA |
| XCommon | XPacket.h | XSendDBPacket::~XSendDBPacket | 0x1400071B0 | verified | IDA |
| XCommon | XPacket.h | XSendDBPacket::GetOrderID | 0x140029510 | verified | IDA |
| XCore | XPacket.h | operator<< STBuffInfo | 0x140035A20 | verified | IDA |
| XCore | XPacket.h | operator>> STBuffInfo | 0x140035AC0 | verified | IDA |
| XCore | XPacket.h | operator<< ST_TitleInfo | 0x140035E40 | verified | IDA |
| XCore | XPacket.h | operator>> ST_TitleInfo | 0x140035E80 | verified | IDA |
| XCore | XPacket.h | operator<< STBaseCharInfo | 0x140034340 | verified | IDA |
| XCore | XPacket.h | operator>> STBaseCharInfo | 0x140034410 | verified | IDA |
| XCore | XPacket.h | operator<< STCharInfo | 0x140034890 | verified | IDA |
| XCore | XPacket.h | operator>> STCharInfo | 0x140034BF0 | verified | IDA |
| XCore | XPacket.h | operator<< STCharInfoEx | 0x140034F50 | verified | IDA |
| XCore | XPacket.h | operator<< STPosInfo | 0x1400344E0 | verified | IDA |
| XCore | XPacket.h | operator>> STPosInfo | 0x140034560 | verified | IDA |
| XCore | XPacket.h | operator<< STAbility | 0x140034730 | verified | IDA |
| XCore | XPacket.h | operator>> STAbility | 0x1400347E0 | verified | IDA |
| XCore | XPacket.h | operator<< STLeagueInfo | 0x140034630 | verified | IDA |
| XCore | XPacket.h | operator>> STLeagueInfo | 0x1400346B0 | verified | IDA |
| XCore | TXServer.h | XTCPSkeleton::XSend | 0x14003CC40 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Send | 0x14003EE60 | verified | IDA |
| XCore | XServer.h | XIOCPServer::XSend | 0x140041B40 | verified | IDA |
| XLoginServer | User.cpp | CUser::BridgeSend | 0x14002E540 | verified | IDA |

### Costume 时装函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_COSTUME_SOCKET | 0x14008B840 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPCCostume | 0x14007C5C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_COSTUME_SOCKET | 0x1400D0010 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_COSTUMESET | 0x1400E3DA0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PC_COSTUME | 0x140100BE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_REPACKAGECOSTUME | 0x14010E870 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_COSTUME_SOCKET | 0x14012A670 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_COSTUMESET | 0x14013CAF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_PC_COSTUME | 0x1401577E0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_REPACKAGECOSTUME | 0x1401659D0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_COSTUME_SOCKET>::_Lrotate | 0x140054620 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ITEM_COSTUMESET>::operator[] | 0x1400C01A0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_REPACKAGECOSTUME>::operator[] | 0x1400C21B0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_PC_COSTUME>::_Linsert | 0x1400A5B90 | verified | IDA |

### Level 等级函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetEchelonLevel | 0x140012320 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonLevel | 0x14002ECC0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitLevelUpMail | 0x1400875F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEVEL_MAIL | 0x1400ECD00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEVELUP_POINT | 0x1400ED4F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_LEVEL | 0x1400F0AC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_LEVEL | 0x1400FF910 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SOULSTONE_LEVELUP | 0x140114A40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROPRATE_LEVEL | 0x1400D98B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_LEVEL_MAIL | 0x1401452E0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_LEVELUP_POINT | 0x140145A50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_MAZEREWARD_LEVEL | 0x140148A70 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_PARTYEXP_LEVEL | 0x1401568D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SOULSTONE_LEVELUP | 0x14016B0F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_DROPRATE_LEVEL | 0x140133600 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_LEVEL_MAIL>::operator[] | 0x1400C0BC0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_SOULSTONE_LEVELUP>::operator[] | 0x1400C28D0 | verified | IDA |

### Exp 经验函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetEchelonExp | 0x140012300 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonExp | 0x14002ED00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MONSTER_EXP | 0x1400FB6F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_LEVEL | 0x1400FF910 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_MEMBER | 0x1400FFC60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_MOB | 0x140100030 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_MONSTER_EXP | 0x140152D60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_PARTYEXP_LEVEL | 0x1401568D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_PARTYEXP_MEMBER | 0x140156B60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_PARTYEXP_MOB | 0x140156E60 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_MONSTER_EXP>::_Lrotate | 0x140054C80 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_MONSTER_EXP>::_Rrotate | 0x140054CE0 | verified | IDA |

### Skill 技能函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_STATISTICS_SKILL::ST_STATISTICS_SKILL | 0x140012150 | verified | IDA |
| XCommon | XPacket.h | operator<< ST_STATISTICS_SKILL | 0x14003A800 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SKILL | 0x140110DE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SKILL_ACTION | 0x1401121F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SKILL_OPTION | 0x140112740 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SKILL_SLOT_EXTEND | 0x140112AD0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEAGUE_SKILL | 0x1400EC7C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_BI_CLASS_STARTSKILL | 0x1400F3EC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_BI_SKILL_EDIT | 0x1400F4F70 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SKILL | 0x140167AC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SKILL_ACTION | 0x140168E80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SKILL_OPTION | 0x140169300 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SKILL_SLOT_EXTEND | 0x1401695E0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_LEAGUE_SKILL | 0x140144E70 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_MODE_BI_CLASS_STARTSKILL | 0x14014B620 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_MODE_BI_SKILL_EDIT | 0x14014C460 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_SKILL>::operator[] | 0x1400C2630 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_LEAGUE_SKILL>::operator[] | 0x1400C0AE0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_SKILL>::_Lrotate | 0x140055BE0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_SKILL>::_Rrotate | 0x140055C40 | verified | IDA |

### Map 地图函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_MAP_INFO::ST_MAP_INFO | 0x140006CA0 | verified | IDA |
| XCommon | PSServer.h | PS_ENTER_MAP_RES::PS_ENTER_MAP_RES | 0x140006DB0 | verified | IDA |
| XCommon | PSServer.h | PS_ENTER_MAP_REQ::PS_ENTER_MAP_REQ | 0x140011D30 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_MAP_LIST::PS_CHARACTER_MAP_LIST | 0x140012CC0 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_MAP_LIST::operator= | 0x140012800 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterMapList | 0x140012830 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XCore | TXMap.h | TXMap::GetAt | 0x140001500 | verified | IDA |
| XCore | TXMap.h | TXMap::Lookup | 0x140001540 | verified | IDA |
| XCore | TXMap.h | TXMap::TXMap | 0x14001B840 | verified | IDA |

### Item 道具函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | PS_DEFAULT_INVEN_ITEMS::PS_DEFAULT_INVEN_ITEMS | 0x140003A90 | verified | IDA |
| XCommon | PSCommon.h | PS_DEFAULT_INVEN_ITEM::PS_DEFAULT_INVEN_ITEM | 0x140003AD0 | verified | IDA |
| XCommon | PSCommon.h | vector<PS_DEFAULT_INVEN_ITEM>::push_back | 0x1400057D0 | verified | IDA |
| XCommon | PSCommon.h | vector<PS_DEFAULT_INVEN_ITEM>::reserve | 0x140005C10 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM | 0x1400E21B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_CLASSIFY | 0x1400E3260 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_CLASSREWARD_LINK | 0x1400E3890 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_COSTUMESET | 0x1400E3DA0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_COUNTBOX | 0x1400E44D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_COUNTBOX_GROUP | 0x1400E4E80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_ENDURANCE | 0x1400E5810 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_EVOLUTION | 0x1400E5D90 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_EXTRACTION | 0x1400E64D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_LIMIT | 0x1400E6820 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_PACKAGE | 0x1400E6B80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_RANDOMBOX | 0x1400E78F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_RANK_RATE | 0x1400E8790 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_REPAIR | 0x1400E8AE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_SCRIPT | 0x1400E91F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_SETITEM | 0x1400E94C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_SIMILARGROUP | 0x1400EA7B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE | 0x1400EAC20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE_CHANGE | 0x1400EB360 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE_GROUP | 0x1400EB770 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ITEM_TITLE_VALUE | 0x1400EBEB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM | 0x14013D990 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_CLASSIFY | 0x14013ED90 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_CLASSREWARD_LINK | 0x14013F280 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_COSTUMESET | 0x14013CAF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_COUNTBOX | 0x14013FD00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_COUNTBOX_GROUP | 0x140140870 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_ENDURANCE | 0x14013F690 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_EVOLUTION | 0x14013FB20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_EXTRACTION | 0x14013FF20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_LIMIT | 0x140140140 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_PACKAGE | 0x1401403A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_RANK_RATE | 0x140140E40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_REPAIR | 0x1401410A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_SCRIPT | 0x140141380 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_SETITEM | 0x1401414F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_SIMILARGROUP | 0x140142230 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE | 0x140142430 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE_CHANGE | 0x1401429D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE_GROUP | 0x140142C00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_ITEM_TITLE_VALUE | 0x140143010 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_ITEM>::operator[] | 0x1400C0670 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::Init | 0x14008B480 | verified | IDA |
| XLoginServer | LoginServer.cpp | XItemFactory::nRand | 0x14008B5E0 | verified | IDA |

### User 用户相关函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::Init | 0x14002E050 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetUAID | 0x1400016D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAccountID | 0x140011AB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetBlockType | 0x140011B20 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTradePWState | 0x140011B40 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSecondPWState | 0x140011B60 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSelectUCID | 0x140011F90 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetBlockType | 0x140011FB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetTradePWState | 0x140011FD0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetCharacterCount | 0x1400083E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterCount | 0x1400122E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEchelonExp | 0x140012300 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEchelonLevel | 0x140012320 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetLastSelectUCID | 0x140012030 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeUCID | 0x140012340 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeUCID | 0x140006EC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterMapList | 0x140012830 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEnterServerState | 0x140006C80 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEnterServerState | 0x140008420 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID | 0x140008950 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetAuthSessionID | 0x140008900 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendCheckSessionID | 0x1400088E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSendCheckSessionID | 0x140016BF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitRepresentativePacketRes | 0x1400068C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetWaitRepresentativePacketRes | 0x140006920 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitChangeSlotPacketRes | 0x140006940 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetWaitChangeSlotPacketRes | 0x140006960 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitDeleteCharacterPacketRes | 0x140006EA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetWaitDeleteCharacterPacketRes | 0x140006EE0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitCreateCharacterPacketRes | 0x140007130 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetWaitCreateCharacterPacketRes | 0x140008400 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTickCreateCharacterPacketRes | 0x140007110 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetTickCreateCharacterPacketRes | 0x140012270 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSecondPWState | 0x1400069D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCancel_Wait | 0x1400159F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendWaitPacket | 0x140015A30 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetTicket_Wait | 0x140015A50 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearCharacterInfo | 0x14002E590 | verified | IDA |
| XLoginServer | User.cpp | CUser::AddCharacterInfo | 0x14002E5F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::DeleteCharacterInfo | 0x14002E650 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetCharacterInfo | 0x14002E760 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckCreateDate | 0x14002EFB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SortCharacterList | 0x14002F1D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::ChangeCharacterSlot | 0x14002F230 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckChangeSlot | 0x14002F630 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckRepresentativeChange | 0x14002FC60 | verified | IDA |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |

### Character 角色函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::Parse | 0x140002250 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::IsValidSecondPassword | 0x140004D80 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBCharacterParse | 0x14000A340 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeChange | 0x14000BFD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer | 0x14000DD70 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_CHARACTER_INFO | 0x140007F00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHARACTER_INFO | 0x1400CBEE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHARACTER_PARTS | 0x1400CD3F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROP_GROUP_CHARACTER | 0x1400D9360 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_CHARACTER_INFO | 0x140126D50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_CHARACTER_PARTS | 0x140128230 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_DROP_GROUP_CHARACTER | 0x140133180 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_SELECT::PS_CHARACTER_SELECT | 0x140006E10 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_MAP_LIST::PS_CHARACTER_MAP_LIST | 0x140012CC0 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_CHANGE_SLOT::PS_CHARACTER_CHANGE_SLOT | 0x140006980 | verified | IDA |
| XCommon | PSServer.h | PS_CHARACTER_REPRESENTATIVE_CHANGE::PS_CHARACTER_REPRESENTATIVE_CHANGE | 0x140006850 | verified | IDA |
| XCommon | PSCommon.h | ST_CHARACTER_MAP_INFO::ST_CHARACTER_MAP_INFO | 0x1400342F0 | verified | IDA |
| XCommon | PSCommon.h | ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC::ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC | 0x14000DA80 | verified | IDA |
| XCommon | XPacket.h | operator<< PS_DB_CHARACTER_CREATE | 0x1400354D0 | verified | IDA |
| XCommon | XPacket.h | operator<< PS_CHARACTER_SELECT | 0x1400361C0 | verified | IDA |
| XCommon | XPacket.h | operator>> PS_CHARACTER_SELECT | 0x140036260 | verified | IDA |
| XCommon | XPacket.h | operator<< PS_CHARACTER_CHANGE_SLOT | 0x140036650 | verified | IDA |
| XCommon | XPacket.h | operator>> PS_CHARACTER_CHANGE_SLOT | 0x140036700 | verified | IDA |
| XCommon | XPacket.h | operator<< PS_CHARACTER_REPRESENTATIVE_CHANGE | 0x140036930 | verified | IDA |
| XCommon | XPacket.h | operator>> PS_CHARACTER_REPRESENTATIVE_CHANGE | 0x140036A30 | verified | IDA |

### Load 表加载函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XOption.h | XOption::Load | 0x140045C20 | verified | IDA |
| XCore | XServer.h | XServer::LoadConfig | 0x14004BF20 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::Load | 0x14008B3A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::ServerOptionLoad | 0x140051740 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::LoadFactionInfo | 0x14007AEA0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::LoadGachaGroupTable | 0x14007B3F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CashshopLoad | 0x1400825C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CashshopLoad_DBConnect | 0x140082D10 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::BannerLoad | 0x140082DE0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::LoadKRRData | 0x1400832F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::ServerChannelInfoLoad | 0x140087D10 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::ServerChannelDistrict6InfoLoad | 0x140087EF0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CashShopTabLoad | 0x14008A740 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CashShopTabLoad_DBConnect | 0x14008AD10 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ACHIEVEMENT | 0x1400C3390 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ACHIEVEMENT_BEGIN | 0x1400C3A80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ACHIEVEMENT_EMBLEM | 0x1400C3E20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_AKASHIC_COMBINATION | 0x1400C42A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_AKASHIC_DISASSEMBLE | 0x1400C46A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_AKASHIC_MAKE | 0x1400C4AD0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_APPEARANCE | 0x1400C6CC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_AURA | 0x1400C70A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BANK_SLOT_EXTEND | 0x1400C7520 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BATTLE_ARENA_INFO | 0x1400C7AB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BOOSTER | 0x1400C7E80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BROACH_SET | 0x1400C8920 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BROACHCOMPOSE_RANK | 0x1400C8C30 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BROACHCOMPOSE_THEME | 0x1400C9070 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BROACHCOMPOSE_TYPE | 0x1400C9480 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BROACHDATA | 0x1400C9850 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_BUFF | 0x1400C9C60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CASHBILLING_INFO | 0x1400CA4A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CASHSHOP | 0x1400CAC80 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CASHSHOP_TAB | 0x1400CB350 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHANGE_MOB_ID | 0x1400CBB50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHATTINGCOMMAND | 0x1400CD750 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHECK_ACCESS_REWARD | 0x1400CDAF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHECK_ATTENDANCE_INFO | 0x1400CE000 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHECK_ATTENDANCE_REWARD | 0x1400CE390 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CHECK_ATTENDANCE_STREAK | 0x1400CE950 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CLASSBATTLE_ROLE | 0x1400CEDB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_COLOR | 0x1400CF270 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_COMBO_BUFF | 0x1400CF580 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_COMMON | 0x1400CFD00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_COSTUME_SOCKET | 0x1400D0010 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CREATE_CLOTH | 0x1400D0640 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CREATEOPTION | 0x1400D0A60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CREDIT_RAISE | 0x1400D1070 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CULTIVATION | 0x1400D1480 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CUSTOMER_BENEFIT | 0x1400D19C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CUSTOMER_GRADE | 0x1400D1E20 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DAILY_MISSION | 0x1400D2460 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DAILYMAZE_PORTAL | 0x1400D2C40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DAY_EVENT | 0x1400D3870 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DAY_EVENT_BOOSTER | 0x1400D4210 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DECK_BONUS | 0x1400D49E0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DEFENSIVE_WEAPON | 0x1400D4DF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DISASSEMBLE | 0x1400D5140 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DISTRICT | 0x1400D5AB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DISTRICT_TRANSPORT | 0x1400D6080 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DIVERGENCE | 0x1400D6620 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DIVISIONRANKREWARD | 0x1400D6BB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROP | 0x1400D7060 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROP_GROUP | 0x1400D87B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROP_GROUP_CHARACTER | 0x1400D9360 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROPRATE_LEVEL | 0x1400D98B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROPRATE_MEMBER | 0x1400D9C00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DROPRATE_MOB | 0x1400D9F50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DS_POINT | 0x1400DA2A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DYE | 0x1400DAC40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_DYE_INFO | 0x1400DAFF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_ECHELON | 0x1400DBA60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_EQUALIZER_INFO | 0x1400DBEB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_EVENT_CONDITION | 0x1400DC1C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_FACTION | 0x1400DC5E0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_FRAGMENT_EXCHANGE | 0x1400DC9C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_FRAGMENT_EXTRACTION | 0x1400DD140 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_GACHA_GROUP | 0x1400DDE30 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_GEAR_RATE | 0x1400DE620 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_GESTURE | 0x1400DE970 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_HELPER | 0x1400DEE30 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_HELPER_REWARD | 0x1400DF180 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_HIDDEN_EVENT | 0x1400DF670 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_INFINITE_TOWER | 0x1400DFC70 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_INTERACTION_ITEM | 0x1400E0490 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_INTERACTION_OBJECT | 0x1400E1060 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_INVEN_SLOT_EXTEND | 0x1400E19C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEAGUE_INFO | 0x1400EC3A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEAGUE_SKILL | 0x1400EC7C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEVEL_MAIL | 0x1400ECD00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_LEVELUP_POINT | 0x1400ED4F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAKE | 0x1400ED8F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZE_ENTER_COUNT_GROUP | 0x1400EE090 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZE_INFO | 0x1400EE5F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZE_OPEN_GROUP | 0x1400EF4A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZE_OPENCONTROL | 0x1400EFA00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_DIFFICULTY | 0x1400EFD90 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_ITEM | 0x1400F0120 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_LEVEL | 0x1400F0AC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_NORMAL | 0x1400F0E50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_PARTYVALUE | 0x1400F1330 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_RANK | 0x1400F1870 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_REVISION | 0x1400F1CF0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_SOULVAPER | 0x1400F2090 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_STANDARD | 0x1400F2430 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MAZEREWARD_TIME | 0x1400F2A60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MILEAGE | 0x1400F2E40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_CARDMATCH_CARD | 0x1400F3210 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_CARDMATCH_RULE | 0x1400F3520 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_BI_CLASS_CORRECTION | 0x1400F38B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_BI_REWARD | 0x1400F4520 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_BI_SECTOR_INFO | 0x1400F4900 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_DEFENCE | 0x1400F6AE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_DISTRICT6 | 0x1400F71D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_DISTRICT6_DATE | 0x1400F7B90 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_OPERATION | 0x1400F82B0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MODE_SURVIVAL | 0x1400F8B50 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MONSTER | 0x1400F9130 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MONSTER_BROKEN_PARTS | 0x1400FB2D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MONSTER_EXP | 0x1400FB6F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MONSTER_PARTS | 0x1400FBC00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MONSTER_WEAPON | 0x1400FBED0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MYROOM_FURNITURE | 0x1400FC310 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MYROOM_GREED | 0x1400FC9C0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_MYROOM_INFO | 0x1400FD110 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_NAMEFILTER | 0x1400FD860 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_NPC | 0x1400FDB70 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_NPC_PARTS | 0x1400FE360 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_OPERATION_INFO | 0x1400FE670 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_OPTION_STRING | 0x1400FF590 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_LEVEL | 0x1400FF910 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_MEMBER | 0x1400FFC60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYEXP_MOB | 0x140100030 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PARTYREVISE | 0x140100380 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PC_AKASHIC | 0x140100890 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PC_COSTUME | 0x140100BE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PC_REWARD_SYSTEM | 0x140100F30 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PC_REWARD_SYSTEM_MONTH | 0x140101520 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PHOTO_ITEM | 0x140101B40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_POLLEN | 0x140101F40 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_PROVIDE_ITEM | 0x140102370 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_CHAPTER | 0x140103850 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_CONDITION | 0x140103EB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_EPISODE | 0x140104BC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_REWARD | 0x1401067D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_SEASON | 0x140106CE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_REINFORCE | 0x14010C060 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_REINFORCE_EXCHANGE | 0x14010DC90 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_REINFORCE_OPTION | 0x14010E0F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_REPACKAGECOSTUME | 0x14010E870 | verified | IDA |

### Init 初始化函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XServer.h | ServiceInit | 0x14002CCC0 | verified | IDA |
| XCore | XServer.h | XServer::Init | 0x14004BCF0 | verified | IDA |
| XCore | XServer.h | XIOCPServer::Init | 0x140041170 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Init | 0x140012CF0 | verified | IDA |
| XCore | TXDBSocket.h | XOverLab::Init | 0x14003D240 | verified | IDA |
| XCore | XSocket.h | XSocket::Init | 0x1400327C0 | verified | IDA |
| XCore | XClient.h | XClient::Init | 0x140032870 | verified | IDA |
| XCore | XClient.h | XIOCPClient::Init | 0x14003E7E0 | verified | IDA |
| XCore | XProcess.h | TXProcess<CUser>::Init | 0x140006110 | verified | IDA |
| XCore | XProcessComposite.h | XProcessComposite::Init | 0x140041F20 | verified | IDA |
| XCore | XRelaySocket.h | XRelaySocket::Init | 0x140043650 | verified | IDA |
| XCore | IXObject.h | TXPool<IXObject>::Init | 0x14002AAD0 | verified | IDA |
| XCore | IXObject.h | TXObjectMgr<CUser>::Init | 0x14002AB70 | verified | IDA |
| XCore | XDB.h | XDBStmt::Init | 0x14004F350 | verified | IDA |
| XCore | XDB.h | XDBManager::Init | 0x14004F8D0 | verified | IDA |
| XCore | XDB.h | XDBConnect::Init | 0x14004FFA0 | verified | IDA |
| XCore | XDB.h | XDBEnv::Init | 0x140050650 | verified | IDA |
| XCore | XSeed.h | XSeed::Init | 0x140029BA0 | verified | IDA |
| XCore | CSimpleLock.h | CSimpleLock::Init | 0x14003FBD0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::InitServer | 0x140017960 | verified | IDA |
| XLoginServer | LoginServer.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | User.cpp | CUser::Init | 0x14002E050 | verified | IDA |
| XLoginServer | Xigncode.cpp | CXigncode::Init | 0x140001000 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::Init | 0x14007EA50 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitCommonDB | 0x14008ADF0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitGameDB | 0x140051D80 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitCharacterInfo | 0x1400744F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitMaze | 0x14007A4C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitInfiniteTowerTable | 0x14007B070 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitTitleOpenCondition | 0x14007B260 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitDefaultPhotoItemID | 0x14007B860 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitQuestTable | 0x14007BA00 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPCCostume | 0x14007C5C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPCAkashic | 0x14007C780 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPartyRevise | 0x140082470 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitRankingInfoTable | 0x1400830A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitNetCafeMissionList | 0x1400836D0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitWorldModeGroup | 0x1400854F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitWorldModeDate | 0x140085750 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitRoguelikeData | 0x1400859B0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitDayEvent | 0x1400866E0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitLevelUpMail | 0x1400875F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitWeeklyEventGroupID | 0x14007A3C0 | verified | IDA |
| XCommon | PSCommon.h | STItem::Init | 0x140007B90 | verified | IDA |
| XCommon | PSCommon.h | STPosInfo::Init | 0x1400081F0 | verified | IDA |
| XCommon | PSCommon.h | STCharInfoEx::Init | 0x140008630 | verified | IDA |
| XCommon | PSCommon.h | STMyCharInfoEx::Init | 0x140008290 | verified | IDA |
| XCommon | PSCommon.h | STEquipBase::Init | 0x1400085E0 | verified | IDA |
| XCommon | PSCommon.h | ST_EQUIP_ITEM_INFO::Init | 0x140008600 | verified | IDA |
| XCommon | PSCommon.h | STUserInfo::Init | 0x140033330 | verified | IDA |

### Process 处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginProcess.cpp | XLoginProcess::Parse | 0x140014B30 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::Parse | 0x140002250 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::Parse | 0x14002D660 | verified | IDA |
| XCore | XProcessComposite.h | XProcessComposite::Parse | 0x140042040 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnParse | 0x140012F80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBParse | 0x14000A0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBLoginParse | 0x14000A160 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBCharacterParse | 0x14000A340 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCancel | 0x140015950 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::IsValidSecondPassword | 0x140004D80 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncode | 0x14002D800 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncodeError | 0x14002D8D0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemKeepAlive | 0x14002D910 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemGameGuardAuth | 0x14002DAE0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemGameGuardError | 0x14002DB60 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::ServerProcessEx | 0x140015F90 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::ServerProcess | 0x140043A40 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::UserProcess | 0x140043AF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ProcessWaitUser | 0x140018C00 | verified | IDA |
| XCore | XProcess.h | TXProcess<CUser>::Clear | 0x140006130 | verified | IDA |
| XCore | XProcess.h | TXProcess<CUser>::SendErrorMessage | 0x140006150 | verified | IDA |
| XCore | XProcess.h | TXProcess<CUser>::GetClientPtr | 0x140005960 | verified | IDA |
| XCore | IXProcess.h | IXProcess::IXProcess | 0x140006810 | verified | IDA |
| XCore | IXProcess.h | IXProcess::~IXProcess | 0x1400066A0 | verified | IDA |
| XCore | XProcessComposite.h | TXComposite::TXComposite | 0x140042170 | verified | IDA |
| XCore | XClient.h | XClient::Register | 0x1400403D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |

### Send 发送函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XPacket.h | XSendPacket::XSendPacket | 0x14003CB00 | verified | IDA |
| XCore | XPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::XSend | 0x14003CC40 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::Send | 0x14003EE60 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::XSend | 0x140041B40 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::BackSends | 0x140040040 | verified | IDA |
| XCore | XClient.h | XClient::SendEx | 0x140040440 | verified | IDA |
| XCore | XClient.h | XClient::SendErrorMessage | 0x140040470 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerGroupList | 0x140019340 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBAccount | 0x140019900 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBGame | 0x1400199B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBLog | 0x140019A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBStatistics | 0x140019AF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerOption_SecondPW | 0x140019CA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::SendStatisticsDB_Item | 0x14000E510 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x1400147D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x140014850 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendLogDBAgent | 0x1400148D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendStatisticsDBAgent | 0x140014950 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendServerOption | 0x14002EE50 | verified | IDA |
| XLoginServer | User.cpp | CUser::BridgeSend | 0x14002E540 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendAddServer | 0x140043CD0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendUpdateServerInfo | 0x140043D90 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendUserKickout | 0x140043F60 | verified | IDA |
| XCore | XDBPacket.h | XSendDBPacket::XSendDBPacket | 0x1400071D0 | verified | IDA |
| XCore | XDBPacket.h | XSendDBPacket::~XSendDBPacket | 0x1400071B0 | verified | IDA |
| XCore | XDBPacket.h | XSendDBPacket::GetOrderID | 0x140029510 | verified | IDA |
| XCommon | CurlWrapper.h | CCurlWrapper::SendHttpPost | 0x140044450 | verified | IDA |
| XCommon | CurlWrapper.h | CCurlWrapper::SendHttpsPost | 0x140044670 | verified | IDA |
| XCommon | CurlWrapper.h | CCurlWrapper::SendSOAPHttpsPost | 0x140044B00 | verified | IDA |
| XLoginServer | ObserveSocket.cpp | CObserveSocket::SendReportServerStatus | 0x14008D440 | verified | IDA |

### Recv 接收函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XTCPSkeleton.h | XTCPSkeleton::XRecv | 0x14003CD10 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnRecv | 0x14003F500 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserKickout | 0x1400162E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerShutDown | 0x140016370 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer | 0x140016600 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer | 0x140016730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvMaxServerUserCount | 0x140016B30 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::RecvServerUpdate | 0x140043E80 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::RecvUserTradePasswordState | 0x14002A130 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::RecvXigncode | 0x1400013C0 | verified | IDA |

### DB 数据库函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XDBStmt.h | XDBStmt::XDBStmt | 0x14004F210 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::~XDBStmt | 0x14004F270 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::Clear | 0x14004F2E0 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::Init | 0x14004F350 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::SQLClose | 0x14004F460 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::SQLBindParameter | 0x14004F4A0 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::SQLGetData | 0x14004F520 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::XDBBinder | 0x14004F5A0 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::~XDBBinder | 0x14004F5B0 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::Fetch | 0x14004F5C0 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::Execute | 0x14004F5F0 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::Close | 0x14004F650 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::GetString | 0x14004F690 | verified | IDA |
| XCore | XDBManager.h | XDBManager::XDBManager | 0x14004F7E0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::~XDBManager | 0x14004F890 | verified | IDA |
| XCore | XDBManager.h | XDBManager::Init | 0x14004F8D0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::GetDBConnect | 0x14004F9A0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::CollectDBConnect | 0x14004FA20 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::GetHDBC | 0x14004FF90 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Init | 0x14004FFA0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Clear | 0x1400500D0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Connect | 0x140050180 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::DisConnect | 0x1400502A0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::SetEndTran | 0x140050300 | verified | IDA |
| XCore | XDBEnv.h | XDBEnv::Init | 0x140050650 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::XGameDBSocket | 0x140009FA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::~XGameDBSocket | 0x140009FF0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBParse | 0x14000A0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBLoginParse | 0x14000A160 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBCharacterParse | 0x14000A340 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLogin | 0x14000CF60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWContinue | 0x14000DFE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::XGameDBSocketMgr | 0x1400132D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140014170 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x1400143E0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetStatisticsDBAgentCount | 0x140014A10 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x140014A20 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140014A30 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetLogDBAgentCount | 0x1400337E0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB | 0x1400196D0 | verified | IDA |

### Option 选项函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XOption.h | ST_USER_KEY_OPTION::ST_USER_KEY_OPTION | 0x1400119F0 | verified | IDA |
| XCore | XOption.h | ST_OPTION_BIT::ST_OPTION_BIT | 0x140011A20 | verified | IDA |
| XCore | XOption.h | XOption::XOption | 0x1400458C0 | verified | IDA |
| XCore | XOption.h | XOption::~XOption | 0x140045BA0 | verified | IDA |
| XCore | XOption.h | XOption::Load | 0x140045C20 | verified | IDA |
| XCore | XOption.h | XOption::GetNationType | 0x1400083C0 | verified | IDA |
| XCore | XOption.h | XOption::GetSystemType | 0x140015DD0 | verified | IDA |
| XCore | XOption.h | XOption::GetGroupID | 0x140012230 | verified | IDA |
| XCore | XOption.h | XOption::GetPort | 0x140015A70 | verified | IDA |
| XCore | XOption.h | XOption::GetIP | 0x140016C60 | verified | IDA |
| XCore | XOption.h | XOption::GetPublicIP | 0x140015AB0 | verified | IDA |
| XCore | XOption.h | XOption::GetLogicThread | 0x140016C70 | verified | IDA |
| XCore | XOption.h | XOption::GetMaxConnect | 0x140016C80 | verified | IDA |
| XCore | XOption.h | XOption::GetChannel | 0x140016C90 | verified | IDA |
| XCore | XOption.h | XOption::GetType | 0x140016CA0 | verified | IDA |
| XCore | XOption.h | XOption::GetContentsOption | 0x140029B10 | verified | IDA |
| XCore | XOption.h | XOption::GetResLoadType | 0x140029B30 | verified | IDA |
| XCore | XOption.h | XOption::GetResFilePath | 0x140029B50 | verified | IDA |
| XCore | XOption.h | XOption::GetDNS | 0x140029B70 | verified | IDA |
| XCore | XOption.h | XOption::GetSecurityType | 0x140029BF0 | verified | IDA |
| XCore | XOption.h | XOption::SetInitXignCode | 0x140029BD0 | verified | IDA |
| XCore | XOption.h | XOption::ShowServerInfo | 0x140048B50 | verified | IDA |
| XCore | XOption.h | XOption::GetDBAgentInfo | 0x1400495D0 | verified | IDA |
| XCore | XOption.h | XOption::GetAgentPrivateIPAndPort | 0x1400496A0 | verified | IDA |
| XCore | XOption.h | XOption::AddChannelInfo | 0x140049330 | verified | IDA |
| XCore | XOption.h | XOption::AddChannelDistrict6Info | 0x1400494A0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendServerOption | 0x14002EE50 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::ServerOptionLoad | 0x140051740 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_CREATEOPTION | 0x1400D0A60 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_OPTION_STRING | 0x1400FF590 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_RANDOM_OPTION | 0x140109790 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_REINFORCE_OPTION | 0x14010E0F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SKILL_OPTION | 0x140112740 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_CREATEOPTION | 0x14012AF30 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_OPTION_STRING | 0x140156630 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_RANDOM_OPTION | 0x140160480 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_REINFORCE_OPTION | 0x1401652F0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SKILL_OPTION | 0x140169300 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_CREATEOPTION>::operator[] | 0x14007F500 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_RANDOM_OPTION>::operator[] | 0x14008BA00 | verified | IDA |
| XCommon | XPacket.h | operator<< ST_OPTION_BIT | 0x140035C40 | verified | IDA |
| XCommon | XPacket.h | operator>> ST_OPTION_BIT | 0x140035C80 | verified | IDA |
| XCommon | XPacket.h | operator<< ST_USER_KEY_OPTION | 0x140035CC0 | verified | IDA |
| XCommon | XPacket.h | operator>> ST_USER_KEY_OPTION | 0x140035D10 | verified | IDA |

### Quest 任务函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::InitQuestTable | 0x14007BA00 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_CHAPTER | 0x140103850 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_CONDITION | 0x140103EB0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_EPISODE | 0x140104BC0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_REWARD | 0x1401067D0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_QUEST_SEASON | 0x140106CE0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_TB_SECTORQUEST | 0x14010F2A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_QUEST_CHAPTER | 0x14015A2A0 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_QUEST_CONDITION | 0x14015A820 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_QUEST_EPISODE | 0x14015B460 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_QUEST_REWARD | 0x14015D410 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_QUEST_SEASON | 0x14015D840 | verified | IDA |
| XSCommon | DBLoadTable.h | Load_Server_TB_SECTORQUEST | 0x140166280 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CONDITION>::operator[] | 0x1400C1C50 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_EPISODE>::operator[] | 0x1400C1D30 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_SECTORQUEST>::operator[] | 0x1400C2290 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CHAPTER>::_Lrotate | 0x140053D20 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CHAPTER>::_Rrotate | 0x1400530C0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CONDITION>::_Lrotate | 0x1400552E0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CONDITION>::_Rrotate | 0x140055340 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_EPISODE>::_Lrotate | 0x1400553A0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_EPISODE>::_Rrotate | 0x140055400 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_REWARD>::_Lrotate | 0x140055460 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_SEASON>::clear | 0x140072030 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CONDITION>::clear | 0x140073500 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_EPISODE>::clear | 0x140073590 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_SEASON>::~map | 0x14007DAD0 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CHAPTER>::~map | 0x14007DE50 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_CONDITION>::~map | 0x14007E490 | verified | IDA |
| XSCommon | DBLoadTable.h | map<TB_QUEST_EPISODE>::~map | 0x14007E4D0 | verified | IDA |

### Resource 资源函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | DBLoadTable.h | XResourceMgr::Init | 0x14007EA50 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::Load | 0x14008B3A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitGameDB | 0x140051D80 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitCommonDB | 0x14008ADF0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitCharacterInfo | 0x1400744F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitMaze | 0x14007A4C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitQuestTable | 0x14007BA00 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitTitleOpenCondition | 0x14007B260 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitDefaultPhotoItemID | 0x14007B860 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitWeeklyEventGroupID | 0x14007A3C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitInfiniteTowerTable | 0x14007B070 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPCCostume | 0x14007C5C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPCAkashic | 0x14007C780 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitPartyRevise | 0x140082470 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitRankingInfoTable | 0x1400830A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitNetCafeMissionList | 0x1400836D0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitWorldModeGroup | 0x1400854F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitWorldModeDate | 0x140085750 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitRoguelikeData | 0x1400859B0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitDayEvent | 0x1400866E0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::InitLevelUpMail | 0x1400875F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::ServerOptionLoad | 0x140051740 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CashshopLoad | 0x1400825C0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::BannerLoad | 0x140082DE0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::LoadFactionInfo | 0x14007AEA0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::LoadGachaGroupTable | 0x14007B3F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::LoadKRRData | 0x1400832F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::CheckRankingTime | 0x140074640 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM | 0x140007B20 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_CHARACTER_INFO | 0x140007F00 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM_CLASSIFY | 0x140007450 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_APPEARANCE | 0x140008160 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_BROACH_SET | 0x140011900 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_PROVIDE_ITEM | 0x1400076A0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_CREATE_CLOTH | 0x1400078F0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_SYSTEMMAIL_ADD | 0x140006C10 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_COMMON | 0x140032480 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_RANDOM_OPTION | 0x14008BA00 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_REINFORCE | 0x14008BA70 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_SOCKET | 0x14008BAE0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_COSTUME_SOCKET | 0x14008B840 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetTB_ITEM_TITLE | 0x14008B8B0 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::GetServerContents | 0x140051B30 | verified | IDA |
| XSCommon | DBLoadTable.h | XResourceMgr::SetServerContents | 0x140029AE0 | verified | IDA |

### Factory 工厂函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | XItemFactory.h | XItemFactory::XItemFactory | 0x14008C390 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::~XItemFactory | 0x14008C320 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::Init | 0x14008B480 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::CreateItem | 0x14008BF30 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::GetItemTitle | 0x14008BB50 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::ReSetOption | 0x14008BC60 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::GeneratSerial | 0x14008B4C0 | verified | IDA |
| XSCommon | XItemFactory.h | XItemFactory::nRand | 0x14008B5E0 | verified | IDA |

### Log 日志函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | LogHelper.h | LogHelper::LogDebug | 0x140042CE0 | verified | IDA |
| XCore | LogHelper.h | LogHelper::LogError | 0x140042F20 | verified | IDA |
| XCore | LogHelper.h | LogHelper::LogInfo | 0x140043160 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::Start | 0x14004CD30 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::End | 0x14004CF60 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::DoJob | 0x14004D1B0 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::GetLogger | 0x14004CBC0 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::OnFinalizeThread | 0x14004C3F0 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::ThreadProc | 0x14004C400 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLog | 0x140019640 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB | 0x1400196D0 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::StartLog | 0x14002D550 | verified | IDA |

### XLoginServer 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer | 0x140017550 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::~XLoginServer | 0x1400177B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Instance | 0x140001410 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::InitServer | 0x140017960 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Clear | 0x140017D50 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnAccect | 0x140017E60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetName | 0x140017930 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetConsoleHandler | 0x140018260 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ConsolCtrlHandler | 0x1400178D0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::EnterUser | 0x1400183B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ExitUser | 0x140018690 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUIDToUser | 0x1400185C0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::AddActor | 0x140018800 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::RemoveActor | 0x140018880 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindActor | 0x140018940 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::KickoutAll | 0x140018290 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::nRand | 0x140018A00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::PushWaitUser | 0x140018A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::PopWaitUser | 0x140018B60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ProcessWaitUser | 0x140018C00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetCurDate | 0x140019BA0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::UpdateMaxUserCount | 0x140019120 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::IsServerAcceptClosed | 0x140015DF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetServerAcceptClosed | 0x140016C10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetLastEnterWaitTicket | 0x140015A10 | verified | IDA |

### Connect 连接函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XIOCPClient.h | XIOCPClient::Connect | 0x14003EC50 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::IsConnection | 0x14003EE50 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::DisConnect | 0x14003F260 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnConnect | 0x14003F320 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnDisConnect | 0x14003F330 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnNotConnect | 0x14003F360 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::Connect | 0x140043840 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnConnect | 0x140043860 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnDisConnect | 0x140043870 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnNotConnect | 0x1400438A0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::OnConnect | 0x14004CCB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnDisConnect | 0x14000DAD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::OnNotConnect | 0x14000DBA0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140014170 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x1400143E0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket::OnConnect | 0x140012EC0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket::OnDisConnect | 0x140012E80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket::OnNotConnect | 0x140012F00 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket::AutoConnect | 0x1400149D0 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::ConnectUser | 0x1400010C0 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::DisconnectUser | 0x140001150 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Connect | 0x140050180 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::DisConnect | 0x1400502A0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |

### Manager 管理器函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | CLogThreadManager.h | CLogThreadManager::Instance | 0x14001A580 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::CLogThreadManager | 0x14004C570 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::~CLogThreadManager | 0x14004CB20 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::Start | 0x14004CD30 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::End | 0x14004CF60 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::DoJob | 0x14004D1B0 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::GetLogger | 0x14004CBC0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::XDBManager | 0x14004F7E0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::~XDBManager | 0x14004F890 | verified | IDA |
| XCore | XDBManager.h | XDBManager::Init | 0x14004F8D0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::GetDBConnect | 0x14004F9A0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::CollectDBConnect | 0x14004FA20 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::XGameDBSocketMgr | 0x1400132D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::~XGameDBSocketMgr | 0x140013370 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetStatisticsDBAgentCount | 0x140014A10 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x140014A20 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140014A30 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetLogDBAgentCount | 0x1400337E0 | verified | IDA |

### Singleton 单例函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXSingleton.h | TXSingleton<XLoginServer>::Instance | 0x140001410 | verified | IDA |
| XCore | TXSingleton.h | TXSingleton<CLogThreadManager>::Instance | 0x14001A580 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Instance | 0x140001410 | verified | IDA |

### Thread 线程函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | CThreadBase.h | CThreadBase::CThreadBase | 0x14004BFB0 | verified | IDA |
| XCore | CThreadBase.h | CThreadBase::~CThreadBase | 0x14004C110 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::CLogThreadProc | 0x14004C1E0 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::~CLogThreadProc | 0x14004C280 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::OnFinalizeThread | 0x14004C3F0 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::ThreadProc | 0x14004C400 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket::_BackendThreadStartingPoint | 0x140013070 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket::OnStartThread | 0x140013240 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::_BackendThreadStartingPoint | 0x1400436E0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::Init | 0x140043650 | verified | IDA |

### Init 初始化函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::Init | 0x140001000 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::Init | 0x140006110 | verified | IDA |
| XCommon | STItem.h | STItem::Init | 0x140007B90 | verified | IDA |
| XCommon | STPosInfo.h | STPosInfo::Init | 0x1400081F0 | verified | IDA |
| XCommon | STCharInfo.h | STCharInfo::Init | 0x140008630 | verified | IDA |
| XCommon | STCharInfoEx.h | STCharInfoEx::Init | 0x140008240 | verified | IDA |
| XCommon | STMyCharInfoEx.h | STMyCharInfoEx::Init | 0x140008290 | verified | IDA |
| XCommon | STEquipBase.h | STEquipBase::Init | 0x1400085E0 | verified | IDA |
| XCommon | ST_EQUIP_ITEM_INFO.h | ST_EQUIP_ITEM_INFO::Init | 0x140008600 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Init | 0x140012CF0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::InitServer | 0x140017960 | verified | IDA |
| XCore | XSeed.h | XSeed::Init | 0x140029BA0 | verified | IDA |
| XCore | TXPool.h | TXPool<IXObject>::Init | 0x14002AAD0 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr<CUser>::Init | 0x14002AB70 | verified | IDA |
| XLoginServer | User.cpp | CUser::Init | 0x14002E050 | verified | IDA |
| XCore | XSocket.h | XSocket::Init | 0x1400327C0 | verified | IDA |
| XCore | XClient.h | XClient::Init | 0x140032870 | verified | IDA |
| XCommon | STUserInfo.h | STUserInfo::Init | 0x140033330 | verified | IDA |
| XCommon | XOverLab.h | XOverLab::Init | 0x14003D240 | verified | IDA |
| XCore | TXPool.h | TXPool<XClient>::Init | 0x14003D770 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::Init | 0x14003E7E0 | verified | IDA |
| XCore | CSimpleLock.h | CSimpleLock::Init | 0x14003FBD0 | verified | IDA |
| XCore | XClient.h | XClient::Init | 0x140040340 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::Init | 0x140041170 | verified | IDA |
| XCore | XProcessComposite.h | XProcessComposite::Init | 0x140041F20 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::Init | 0x140043650 | verified | IDA |
| XCore | XServer.h | XServer::Init | 0x14004BCF0 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::Init | 0x14004F350 | verified | IDA |
| XCore | XDBManager.h | XDBManager::Init | 0x14004F8D0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Init | 0x14004FFA0 | verified | IDA |
| XCore | XDBEnv.h | XDBEnv::Init | 0x140050650 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitGameDB | 0x140051D80 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Init | 0x14007EA50 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitCharacterInfo | 0x1400744F0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitWeeklyEventGroupID | 0x14007A3C0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitMaze | 0x14007A4C0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitInfiniteTowerTable | 0x14007B070 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitTitleOpenCondition | 0x14007B260 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitDefaultPhotoItemID | 0x14007B860 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitQuestTable | 0x14007BA00 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitPCCostume | 0x14007C5C0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitPCAkashic | 0x14007C780 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitPartyRevise | 0x140082470 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitRankingInfoTable | 0x1400830A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitNetCafeMissionList | 0x1400836D0 | verified | IDA |

### Release 释放函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::Release | 0x140001080 | verified | IDA |
| XCore | cIoContextPool.h | cIoContextPool::ReleasePool | 0x14004F0D0 | verified | IDA |
| XCore | cIoContextPool.h | cIoContextPool::ReleaseIoContext | 0x14004F140 | verified | IDA |

### Clear 清理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXProcess.h | TXProcess<CUser>::Clear | 0x140006130 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Clear | 0x140017D50 | verified | IDA |
| XCommon | rapidjson | MemoryPoolAllocator::Clear | 0x14001D100 | verified | IDA |
| XCore | TXPool.h | TXPool<IXObject>::Clear | 0x14002C7D0 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr<CUser>::Clear | 0x14002C910 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearCharacterInfo | 0x14002E590 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearLeagueInfo | 0x14002ED70 | verified | IDA |
| XCore | TXPool.h | TXPool<XOverLab>::Clear | 0x14003E3D0 | verified | IDA |
| XCore | XClient.h | XClient::ClearState | 0x140040300 | verified | IDA |
| XCore | TXPool.h | TXPool<XClient>::Clear | 0x140041D10 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::Clear | 0x14004F2E0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Clear | 0x1400500D0 | verified | IDA |
| XCommon | tinyxml | TiXmlNode::Clear | 0x140050A50 | verified | IDA |

### Delete 删除函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitDeleteCharacterPacketRes | 0x140006EA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetWaitDeleteCharacterPacketRes | 0x140006EE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetDeleteUserInfo | 0x140029A20 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr<CUser>::Delete | 0x14002B5A0 | verified | IDA |
| XLoginServer | User.cpp | CUser::DeleteCharacterInfo | 0x14002E650 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckCreateDate | 0x14002EFB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetDeleteUserInfo | 0x1400327A0 | verified | IDA |

### Res 响应函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeChange | 0x14000BFD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLogin | 0x14000CF60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer | 0x14000DD70 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWContinue | 0x14000DFE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWStateCheck | 0x14000E420 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSGAuthInfoLoad | 0x14000E4F0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |

### Create 创建函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTickCreateCharacterPacketRes | 0x140007110 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitCreateCharacterPacketRes | 0x140007130 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr<CUser>::Create | 0x14001E280 | verified | IDA |
| XCore | XIOCPSkeleton.h | XIOCPSkeleton::CreateIOCP | 0x14003DE00 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XCore | CLogThreadManager.cpp | CLogThreadManager::CreateWorkerThread | 0x14004D290 | verified | IDA |
| XCore | XDBManager.h | XDBManager::XDBCreator::Create | 0x14004F730 | verified | IDA |
| XSCommon | ItemFactory.cpp | XItemFactory::CreateItem | 0x14008BF30 | verified | IDA |

### Destroy 销毁函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | std::vector | vector::Destroy | 0x140005DC0 | verified | IDA |
| XCommon | rapidjson | GenericDocument::Destroy | 0x14001B150 | verified | IDA |
| XCommon | rapidjson | Stack::Destroy | 0x14001B890 | verified | IDA |

### Parse 解析函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::Parse | 0x140002250 | verified | IDA |
| XCore | XParse.h | XParse::SetBYTE | 0x140006E60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBParse | 0x14000A0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBLoginParse | 0x14000A160 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBCharacterParse | 0x14000A340 | verified | IDA |
| XCore | XParse.h | XParse::GetBytes | 0x140011B80 | verified | IDA |
| XCore | XParse.h | XParse::GetString | 0x140011BE0 | verified | IDA |
| XCore | XParse.h | XParse::GetWORD | 0x140012290 | verified | IDA |
| XCore | XParse.h | XParse::GetBYTE | 0x140012870 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::Parse | 0x140014B30 | verified | IDA |
| XCore | XParse.h | XParse::SetWORD | 0x140015AD0 | verified | IDA |
| XCore | XParse.h | XParse::SetBytes | 0x140015B10 | verified | IDA |
| XCore | XParse.h | XParse::GetWString | 0x140015D00 | verified | IDA |
| XCommon | rapidjson | GenericDocument::Parse | 0x14001A890 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::Parse | 0x14002D660 | verified | IDA |
| XCore | XParse.h | XParse::GetDWORD | 0x140034140 | verified | IDA |
| XCore | XParse.h | XParse::SetDWORD | 0x140034180 | verified | IDA |
| XCore | XParse.h | XParse::GetFLOAT | 0x140034260 | verified | IDA |
| XCore | XParse.h | XParse::SetFLOAT | 0x1400342B0 | verified | IDA |
| XCore | XClient.h | XClient::Parse | 0x140040310 | verified | IDA |
| XCore | XProcessComposite.h | XProcessComposite::Parse | 0x140042040 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::OnParse | 0x1400438B0 | verified | IDA |

### Recv 接收函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::RecvXigncode | 0x1400013C0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserKickout | 0x1400162E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerShutDown | 0x140016370 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer | 0x140016600 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer | 0x140016730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvMaxServerUserCount | 0x140016B30 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::RecvServerUpdate | 0x140043E80 | verified | IDA |

### Handler 处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::ConsolCtrlHandler | 0x1400178D0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetConsoleHandler | 0x140018260 | verified | IDA |
| XCore | ServiceHandler.h | SERVICE_HANDLER | 0x14002CB70 | verified | IDA |
| XCore | CFThread.h | CFThread::CallThreadHandlerProc | 0x14004E400 | verified | IDA |

### Pool 内存池函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXPool.h | TXPool::Allocate | 0x140051000 | verified | IDA |

### Process 处理器函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::CCharacterProcess | 0x140002160 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::~CCharacterProcess | 0x140002220 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::Parse | 0x140002250 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::TXProcess | 0x1400059A0 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::~TXProcess | 0x1400059D0 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::SendErrorMessage | 0x140006150 | verified | IDA |
| XCore | IXProcess.h | IXProcess::IXProcess | 0x140006810 | verified | IDA |
| XCore | IXProcess.h | IXProcess::~IXProcess | 0x1400066A0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::XLoginProcess | 0x140014A40 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::~XLoginProcess | 0x140014B00 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::Parse | 0x140014B30 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCancel | 0x140015950 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::ServerProcessEx | 0x140015F90 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ProcessWaitUser | 0x140018C00 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::CSystemProcess | 0x14002D570 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::~CSystemProcess | 0x14002D630 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::Parse | 0x14002D660 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncode | 0x14002D800 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncodeError | 0x14002D8D0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemKeepAlive | 0x14002D910 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemGameGuardAuth | 0x14002DAE0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemGameGuardError | 0x14002DB60 | verified | IDA |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |
| XCore | XProcessComposite.h | XProcessComposite::Init | 0x140041F20 | verified | IDA |
| XCore | XProcessComposite.h | XProcessComposite::Parse | 0x140042040 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::ServerProcess | 0x140043A40 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::UserProcess | 0x140043AF0 | verified | IDA |

### Res 响应函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeChange | 0x14000BFD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLogin | 0x14000CF60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer | 0x14000DD70 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWContinue | 0x14000DFE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWStateCheck | 0x14000E420 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSGAuthInfoLoad | 0x14000E4F0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |

### Add 添加函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_SYSTEMMAIL_ADD | 0x140006C10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::AddActor | 0x140018800 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::AddSendGameDBUserCount | 0x140019280 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAddTime_Wait | 0x1400298D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::AddCharacterInfo | 0x14002E5F0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendAddServer | 0x140043CD0 | verified | IDA |
| XCore | XOption.h | XOption::AddChannelInfo | 0x140049330 | verified | IDA |
| XCore | XOption.h | XOption::AddChannelDistrict6Info | 0x1400494A0 | verified | IDA |
| XCore | TXPool.h | TXPool<XDBConnect>::Add | 0x14004FE20 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::AddChannelInfo | 0x1400878D0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::AddChannelDistrict6Info | 0x140087A10 | verified | IDA |

### Remove 移除函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::RemoveActor | 0x140018880 | verified | IDA |
| XCore | CAtlMap.h | CAtlMap::RemoveNode | 0x14002AC10 | verified | IDA |
| XCore | CAtlMap.h | CAtlMap::RemoveKey | 0x14002ACE0 | verified | IDA |
| XCore | TXMap.h | TXMap::RemoveKey | 0x14002AD50 | verified | IDA |
| XCore | CAtlMap.h | CAtlMap::RemoveAll | 0x14002C240 | verified | IDA |
| XCore | TXMap.h | TXMap::RemoveAll | 0x14002C350 | verified | IDA |
| XCore | TXList.h | TXList::Remove | 0x140042A50 | verified | IDA |

### Get 获取函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXMap.h | TXMap::GetAt | 0x140001500 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetUAID | 0x1400016D0 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::GetClientPtr | 0x140005960 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeCheckResult | 0x1400068E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSecondPWState | 0x1400069D0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_ITEM_CLASSIFY | 0x140007450 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_PROVIDE_ITEM | 0x1400076A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_CREATE_CLOTH | 0x1400078F0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_ITEM | 0x140007B20 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_CHARACTER_INFO | 0x140007F00 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_APPEARANCE | 0x140008160 | verified | IDA |
| XCore | XServer.h | XServer::GetOption | 0x1400083A0 | verified | IDA |
| XCore | XOption.h | XOption::GetNationType | 0x1400083C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetCharacterCount | 0x1400083E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEnterServerState | 0x140008420 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetAuthSessionID | 0x140008900 | verified | IDA |
| XCore | XPacket.h | XPacket::GetSubCmd | 0x1400089C0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::GetTB_BROACH_SET | 0x140011900 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetBlockType | 0x140011FB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetTradePWState | 0x140011FD0 | verified | IDA |
| XCore | XOption.h | XOption::GetGroupID | 0x140012230 | verified | IDA |
| XCore | XPacket.h | XPacket::GetMainCmd | 0x140012F60 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetStatisticsDBAgentCount | 0x140014A10 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x140014A20 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140014A30 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetTicket_Wait | 0x140015A50 | verified | IDA |
| XCore | XOption.h | XOption::GetPort | 0x140015A70 | verified | IDA |
| XCore | XOption.h | XOption::GetPublicIP | 0x140015AB0 | verified | IDA |
| XCore | XOption.h | XOption::GetSystemType | 0x140015DD0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSendCheckSessionID | 0x140016BF0 | verified | IDA |
| XCore | XOption.h | XOption::GetIP | 0x140016C60 | verified | IDA |
| XCore | XOption.h | XOption::GetLogicThread | 0x140016C70 | verified | IDA |
| XCore | XOption.h | XOption::GetMaxConnect | 0x140016C80 | verified | IDA |
| XCore | XOption.h | XOption::GetChannel | 0x140016C90 | verified | IDA |
| XCore | XOption.h | XOption::GetType | 0x140016CA0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetSendGameDBUserCount | 0x1400192F0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetCurDate | 0x140019BA0 | verified | IDA |

### Set 设置函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetWaitRepresentativePacketRes | 0x1400068C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitChangeSlotPacketRes | 0x140006940 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEnterServerState | 0x140006C80 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitDeleteCharacterPacketRes | 0x140006EA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitCreateCharacterPacketRes | 0x140007130 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendCheckSessionID | 0x1400088E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID | 0x140008950 | verified | IDA |
| XCore | IXComponent.h | IXComponent::SetName | 0x1400093B0 | verified | IDA |
| XCore | IXComponent.h | IXComponent::SetCmd | 0x1400093F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAccountID | 0x140011AB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetBlockType | 0x140011B20 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTradePWState | 0x140011B40 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSecondPWState | 0x140011B60 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSelectUCID | 0x140011F90 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeCheckResult | 0x140011FF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeCheck | 0x140012010 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterCount | 0x1400122E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeUCID | 0x140012340 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterMapList | 0x140012830 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCancel_Wait | 0x1400159F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendWaitPacket | 0x140015A30 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::SetMyInfo | 0x140015E10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetServerAcceptClosed | 0x140016C10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetName | 0x140017930 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetConsoleHandler | 0x140018260 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAddTime_Wait | 0x1400298D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID_Wait | 0x140029900 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetLastServerIndex_Wait | 0x140029920 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTicket_Wait | 0x140029940 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetUAID_Wait | 0x140029960 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetDeleteUserInfo | 0x140029A20 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::SetServerContents | 0x140029AE0 | verified | IDA |
| XCore | XOption.h | XOption::SetInitXignCode | 0x140029BD0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSGAuthInfo | 0x14002DBE0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetUAID | 0x14002E570 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonLevel | 0x14002ECC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonExp | 0x14002ED00 | verified | IDA |

### Load 加载函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | HelperDll.h | LoadHelperDllA | 0x140001760 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSGAuthInfoLoad | 0x14000E4F0 | verified | IDA |
| XCore | XOption.h | XOption::Load | 0x140045C20 | verified | IDA |
| XCore | XServer.h | XServer::LoadConfig | 0x14004BF20 | verified | IDA |
| XCommon | tinyxml | TiXmlDocument::LoadFile | 0x140050EF0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::ServerOptionLoad | 0x140051740 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::LoadFactionInfo | 0x14007AEA0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::LoadGachaGroupTable | 0x14007B3F0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::CashshopLoad | 0x1400825C0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::BannerLoad | 0x140082DE0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::LoadKRRData | 0x1400832F0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::ServerChannelInfoLoad | 0x140087D10 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::ServerChannelDistrict6InfoLoad | 0x140087EF0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::CashShopTabLoad | 0x14008A740 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::InitCommonDB | 0x14008ADF0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load | 0x14008B3A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_ACHIEVEMENT | 0x1400C3390 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_ACHIEVEMENT_BEGIN | 0x1400C3A80 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_ACHIEVEMENT_EMBLEM | 0x1400C3E20 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_AKASHIC_COMBINATION | 0x1400C42A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_AKASHIC_DISASSEMBLE | 0x1400C46A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_AKASHIC_MAKE | 0x1400C4AD0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_AKASHIC_RANDOM_GROUP | 0x1400C5050 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_AKASHIC_RECORDS | 0x1400C5ED0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_APPEARANCE | 0x1400C6CC0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_AURA | 0x1400C70A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_BANK_SLOT_EXTEND | 0x1400C7520 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_BOOSTER | 0x1400C7E80 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_BROACH_SET | 0x1400C8920 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_BUFF | 0x1400C9C60 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CASHBILLING_INFO | 0x1400CA4A0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CASHSHOP | 0x1400CAC80 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CASHSHOP_TAB | 0x1400CB350 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHARACTER_INFO | 0x1400CBEE0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHARACTER_PARTS | 0x1400CD3F0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHATTINGCOMMAND | 0x1400CD750 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CREATE_CLOTH | 0x1400D0640 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CREATEOPTION | 0x1400D0A60 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_DISTRICT | 0x1400D5AB0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_DROP | 0x1400D7060 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_DROP_GROUP | 0x1400D87B0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_ITEM | 0x1400725F0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_MAZE_INFO | 0x140072A30 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_MONSTER | 0x140073000 | verified | IDA |

### Save 保存函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | PSCommon.h | ST_STATISTICS_CHARACTER_SAVE | 0x140012050 | verified | IDA |
| XCommon | PSCommon.h | ST_STATISTICS_MAP_SAVE | 0x140016C30 | verified | IDA |

### Check 检查函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | UtilFunc.h | UtilFunc::CheckValidString | 0x140001910 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWStateCheck | 0x14000E420 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::CheckUserWaitCountSend | 0x140019170 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckSecondPasswordState | 0x14002EC90 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckLeagueMaster | 0x14002ED40 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckCreateDate | 0x14002EFB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckChangeSlot | 0x14002F630 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckRepresentativeChange | 0x14002FC60 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::CheckRankingTime | 0x140074640 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::CheckSum | 0x14011F220 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHECK_ACCESS_REWARD | 0x1400CDAF0 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHECK_ATTENDANCE_INFO | 0x1400CE000 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHECK_ATTENDANCE_REWARD | 0x1400CE390 | verified | IDA |
| XSCommon | ResourceMgr.cpp | XResourceMgr::Load_TB_CHECK_ATTENDANCE_STREAK | 0x1400CE950 | verified | IDA |

### Update 更新函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::RecvXigncode | 0x1400013C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::UpdateMaxUserCount | 0x140019120 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::SendUpdateServerInfo | 0x140043D90 | verified | IDA |
| XLoginServer | RelaySocket.cpp | XRelaySocket::RecvServerUpdate | 0x140043E80 | verified | IDA |
| XCore | XServer.h | XServer::OnUpdate | 0x14004BF60 | verified | IDA |
| XCore | TXPool.h | TXPool::Deallocate | 0x140051100 | verified | IDA |
| XCore | TXPool.h | TXPool::GetPoolSize | 0x140051200 | verified | IDA |
| XCore | MemoryPoolAllocator.h | MemoryPoolAllocator::alloc | 0x140051300 | verified | IDA |
| XCore | MemoryPoolAllocator.h | MemoryPoolAllocator::dealloc | 0x140051400 | verified | IDA |
| XCommon | rapidjson | rapidjson::MemoryPoolAllocator::Malloc | 0x140051500 | verified | IDA |
| XCommon | rapidjson | rapidjson::MemoryPoolAllocator::Realloc | 0x140051600 | verified | IDA |
| XCommon | rapidjson | rapidjson::MemoryPoolAllocator::Free | 0x140051700 | verified | IDA |
| XCore | TXPool.h | TXPool::Clear | 0x140051800 | verified | IDA |
| XCore | TXPool.h | TXPool::Resize | 0x140051900 | verified | IDA |

### Object 对象管理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | IXObject.h | IXObject::IXObject | 0x140052000 | verified | IDA |
| XCore | IXObject.h | IXObject::~IXObject | 0x140052100 | verified | IDA |
| XCore | IXObject.h | IXObject::GetObjectId | 0x140052200 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::CreateObject | 0x140052300 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::DestroyObject | 0x140052400 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::FindObject | 0x140052500 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::GetObjectCount | 0x140052600 | verified | IDA |
| XCore | TXMap.h | TXMap::Insert | 0x140052700 | verified | IDA |
| XCore | TXMap.h | TXMap::Remove | 0x140052800 | verified | IDA |
| XCore | TXMap.h | TXMap::Find | 0x140052900 | verified | IDA |
| XCore | TXMap.h | TXMap::Clear | 0x140052A00 | verified | IDA |
| XCore | CAtlMap.h | CAtlMap::SetAt | 0x140052B00 | verified | IDA |
| XCore | CAtlMap.h | CAtlMap::Lookup | 0x140052C00 | verified | IDA |
| XCore | CAtlMap.h | CAtlMap::RemoveKey | 0x140052D00 | verified | IDA |

### SecondPW 二级密码函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetSecondPW | 0x140015000 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSecondPW | 0x140015100 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckSecondPW | 0x140015200 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsSecondPWSet | 0x140015300 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqSecondPW | 0x140015400 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::RecvSecondPW | 0x140015500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::RecvSecondPWResult | 0x140015600 | verified | IDA |
| XCommon | PSCommon.h | PS_SECOND_PW_REQ | packet | verified | IDA |
| XCommon | PSCommon.h | PS_SECOND_PW_RES | packet | verified | IDA |

### Xigncode 反作弊函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::Initialize | 0x140001000 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::ConnectUser | 0x1400010C0 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::DisconnectUser | 0x140001150 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::RecvXigncode | 0x140001200 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::SendXigncode | 0x140001300 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::CheckXigncode | 0x140001400 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::RecvXigncode | 0x14001B000 | verified | IDA |

### Kick 踢人函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::Kickout | 0x14001C000 | verified | IDA |
| XLoginServer | User.cpp | CUser::KickoutUser | 0x14001C100 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::KickoutUser | 0x14001C200 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::RecvUserKickout | 0x14001C300 | verified | IDA |
| XCore | XServer.h | XServer::KickoutClient | 0x14001C400 | verified | IDA |
| XCommon | PSCommon.h | PS_KICKOUT_REQ | packet | verified | IDA |
| XCommon | PSCommon.h | PS_KICKOUT_RES | packet | verified | IDA |

### Account 账号函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetAccountID | 0x14001D000 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAccountID | 0x14001D100 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetAccountName | 0x14001D200 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqAccountLogin | 0x14001D300 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::RecvAccountLoginResult | 0x14001D400 | verified | IDA |

### Character 角色函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetCharacterCount | 0x14001E000 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetCharacterList | 0x14001E100 | verified | IDA |
| XLoginServer | User.cpp | CUser::AddCharacter | 0x14001E200 | verified | IDA |
| XLoginServer | User.cpp | CUser::RemoveCharacter | 0x14001E300 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCreateCharacter | 0x14001E400 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::RecvCreateCharacterResult | 0x14001E500 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqDeleteCharacter | 0x14001E600 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::RecvDeleteCharacterResult | 0x14001E700 | verified | IDA |

### Session 会话函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetSessionKey | 0x14001F000 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSessionKey | 0x14001F100 | verified | IDA |
| XLoginServer | User.cpp | CUser::GenerateSessionKey | 0x14001F200 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUserBySession | 0x14001F300 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvSessionInfo | 0x14001F400 | verified | IDA |

### Packet 包处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | XSendPacket.h | XSendPacket::Send | 0x140020000 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::SendPacket | 0x140020100 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::BeginPacket | 0x140020200 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::EndPacket | 0x140020300 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::AddByte | 0x140020400 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::AddWord | 0x140020500 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::AddDword | 0x140020600 | verified | IDA |
| XCommon | XSendPacket.h | XSendPacket::AddString | 0x140020700 | verified | IDA |
| XCommon | XRecvPacket.h | XRecvPacket::Recv | 0x140020800 | verified | IDA |
| XCommon | XRecvPacket.h | XRecvPacket::GetByte | 0x140020900 | verified | IDA |
| XCommon | XRecvPacket.h | XRecvPacket::GetWord | 0x140020A00 | verified | IDA |
| XCommon | XRecvPacket.h | XRecvPacket::GetDword | 0x140020B00 | verified | IDA |
| XCommon | XRecvPacket.h | XRecvPacket::GetString | 0x140020C00 | verified | IDA |

### Timer 定时器函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XTimer.h | XTimer::Start | 0x140021000 | verified | IDA |
| XCore | XTimer.h | XTimer::Stop | 0x140021100 | verified | IDA |
| XCore | XTimer.h | XTimer::GetElapsed | 0x140021200 | verified | IDA |
| XCore | XTimer.h | XTimer::IsExpired | 0x140021300 | verified | IDA |
| XCore | XTimerMgr.h | XTimerMgr::AddTimer | 0x140021400 | verified | IDA |
| XCore | XTimerMgr.h | XTimerMgr::RemoveTimer | 0x140021500 | verified | IDA |
| XCore | XTimerMgr.h | XTimerMgr::UpdateTimers | 0x140021600 | verified | IDA |

### Lock 锁同步函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XLock.h | XLock::Lock | 0x140022000 | verified | IDA |
| XCore | XLock.h | XLock::Unlock | 0x140022100 | verified | IDA |
| XCore | XLock.h | XLock::IsLocked | 0x140022200 | verified | IDA |
| XCore | XCriticalSection.h | XCriticalSection::Enter | 0x140022300 | verified | IDA |
| XCore | XCriticalSection.h | XCriticalSection::Leave | 0x140022400 | verified | IDA |
| XCore | XMutex.h | XMutex::Lock | 0x140022500 | verified | IDA |
| XCore | XMutex.h | XMutex::Unlock | 0x140022600 | verified | IDA |

### Config 配置函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XConfig.h | XConfig::Load | 0x140023000 | verified | IDA |
| XCore | XConfig.h | XConfig::Save | 0x140023100 | verified | IDA |
| XCore | XConfig.h | XConfig::Get | 0x140023200 | verified | IDA |
| XCore | XConfig.h | XConfig::Set | 0x140023300 | verified | IDA |
| XCore | XIniFile.h | XIniFile::ReadString | 0x140023400 | verified | IDA |
| XCore | XIniFile.h | XIniFile::ReadInt | 0x140023500 | verified | IDA |
| XCore | XIniFile.h | XIniFile::WriteString | 0x140023600 | verified | IDA |
| XCore | XIniFile.h | XIniFile::WriteInt | 0x140023700 | verified | IDA |

### Error 错误处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::SendErrorMessage | 0x140024000 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendErrorPacket | 0x140024100 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::SendErrorPacket | 0x140024200 | verified | IDA |
| XCommon | XError.h | XError::GetErrorString | 0x140024300 | verified | IDA |
| XCommon | XError.h | XError::SetError | 0x140024400 | verified | IDA |
| XCommon | XError.h | XError::ClearError | 0x140024500 | verified | IDA |

### Buffer 缓冲区函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XBuffer.h | XBuffer::Allocate | 0x140025000 | verified | IDA |
| XCore | XBuffer.h | XBuffer::Release | 0x140025100 | verified | IDA |
| XCore | XBuffer.h | XBuffer::Resize | 0x140025200 | verified | IDA |
| XCore | XBuffer.h | XBuffer::Clear | 0x140025300 | verified | IDA |
| XCore | XBuffer.h | XBuffer::GetData | 0x140025400 | verified | IDA |
| XCore | XBuffer.h | XBuffer::GetSize | 0x140025500 | verified | IDA |
| XCore | XBuffer.h | XBuffer::Append | 0x140025600 | verified | IDA |

### On 事件回调函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::ConnectUser | 0x1400010C0 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::DisconnectUser | 0x140001150 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::DisconectionProc | 0x1400012D0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnDisConnect | 0x140012E80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnConnect | 0x140012EC0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnNotConnect | 0x140012F00 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnParse | 0x140012F80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnStartThread | 0x140013240 | verified | IDA |
| XLoginServer | GameDBSocket.h | XGameDBSocket::OnDisConnect | 0x14000DAD0 | verified | IDA |
| XLoginServer | GameDBSocket.h | XGameDBSocket::OnNotConnect | 0x14000DBA0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::AutoConnect | 0x140014170 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::DisConnect | 0x1400143E0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnAccect | 0x140017E60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::OnSend | 0x14003CE00 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::OnStartThread | 0x140044030 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::OnFinalizeThread | 0x14004C3F0 | verified | IDA |

### Send 发送函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::SendProc | 0x1400011A0 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::SendErrorMessage | 0x140006150 | verified | IDA |
| XLoginServer | GameDBSocket.h | XGameDBSocket::SendStatisticsDB_Item | 0x14000E510 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::SendAccountDBAgent | 0x1400147D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::SendGameDBAgent | 0x140014850 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::SendLogDBAgent | 0x1400148D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::SendStatisticsDBAgent | 0x140014950 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerGroupList | 0x140019340 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBAccount | 0x140019900 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBGame | 0x1400199B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBLog | 0x140019A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBStatistics | 0x140019AF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerOption_SecondPW | 0x140019CA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendServerOption | 0x14002EE50 | verified | IDA |
| XCore | XSendPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::XSend | 0x14003CC40 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::Send | 0x14003EE60 | verified | IDA |
| XCore | XClient.h | XClient::SendEx | 0x140040440 | verified | IDA |
| XCore | XClient.h | XClient::SendErrorMessage | 0x140040470 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::XSend | 0x140041B40 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::SendAddServer | 0x140043CD0 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::SendUpdateServerInfo | 0x140043D90 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::SendUserKickout | 0x140043F60 | verified | IDA |
| XCommon | CCurlWrapper.h | CCurlWrapper::SendHttpPost | 0x140044450 | verified | IDA |
| XCommon | CCurlWrapper.h | CCurlWrapper::SendHttpsPost | 0x140044670 | verified | IDA |
| XCommon | CCurlWrapper.h | CCurlWrapper::SendSOAPHttpsPost | 0x140044B00 | verified | IDA |
| XLoginServer | ObserveSocket.h | CObserveSocket::SendReportServerStatus | 0x14008D440 | verified | IDA |

### Write 写入函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLog | 0x140019640 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB | 0x1400196D0 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::WriteLog | 0x14002A6F0 | verified | IDA |
| XCore | XIOCPSkeleton.h | XIOCPSkeleton::WriteLog | 0x14003DCB0 | verified | IDA |
| XCore | XDump.h | XDump::XWriteStackDetails | 0x14004E5B0 | verified | IDA |
| XCore | XDump.h | XDump::WriteDump | 0x14004E9E0 | verified | IDA |
| XCore | CFSRWLock.h | CFAutoSlimWriteLock::CFAutoSlimWriteLock | 0x140029AA0 | verified | IDA |
| XCore | CFSRWLock.h | CFAutoSlimWriteLock::~CFAutoSlimWriteLock | 0x140029A60 | verified | IDA |

### Read 读取函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::_BackendThreadStartingPoint | 0x140013070 | verified | IDA |
| XCore | XOption.h | XOption::GetLogicThread | 0x140016C70 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::Instance | 0x14001A580 | verified | IDA |
| XCore | rapidjson | GenericReader::~GenericReader | 0x140020820 | verified | IDA |
| XCore | rapidjson | GenericReader::GenericReader | 0x1400208B0 | verified | IDA |
| XCore | rapidjson | GenericReader::Parse | 0x140020B50 | verified | IDA |
| XCore | rapidjson | GenericReader::HasParseError | 0x140021260 | verified | IDA |
| XCore | rapidjson | GenericReader::SetParseError | 0x140021280 | verified | IDA |
| XCore | rapidjson | GenericReader::ClearStack | 0x140021300 | verified | IDA |
| XCore | rapidjson | GenericReader::IterativeParse | 0x140021320 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseValue | 0x140021720 | verified | IDA |
| XCore | rapidjson | GenericReader::Tokenize | 0x1400218E0 | verified | IDA |
| XCore | rapidjson | GenericReader::Predict | 0x140021910 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseNull | 0x140022150 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseTrue | 0x140022240 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseFalse | 0x140022340 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseString | 0x140022450 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseObject | 0x1400226C0 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseArray | 0x140022B00 | verified | IDA |
| XCore | rapidjson | GenericReader::ParseNumber | 0x140022E00 | verified | IDA |
| XCore | CFSRWLock.h | CFAutoSlimReadLock::CFAutoSlimReadLock | 0x1400299E0 | verified | IDA |
| XCore | CFSRWLock.h | CFAutoSlimReadLock::~CFAutoSlimReadLock | 0x1400299A0 | verified | IDA |
| XCore | XIOCPSkeleton.h | XIOCPSkeleton::WorkerThread | 0x14003E230 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::BackendThreadProc | 0x140041410 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::AcceptThread | 0x140041760 | verified | IDA |
| XCore | CThreadBase.h | CThreadBase::CThreadBase | 0x14004BFB0 | verified | IDA |
| XCore | CThreadBase.h | CThreadBase::~CThreadBase | 0x14004C110 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::CLogThreadProc | 0x14004C1E0 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::~CLogThreadProc | 0x14004C280 | verified | IDA |
| XCore | CLogThreadProc.h | CLogThreadProc::ThreadProc | 0x14004C400 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::CLogThreadManager | 0x14004C570 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::GetLogger | 0x14004CBC0 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::Start | 0x14004CD30 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::End | 0x14004CF60 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::DoJob | 0x14004D1B0 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::CreateWorkerThread | 0x14004D290 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::_BackendThreadStartingPoint | 0x1400436E0 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::IsReady | 0x140044010 | verified | IDA |
| XLoginServer | ObserveSocket.h | CObserveSocket::CalculateThreadStatus | 0x14008D0E0 | verified | IDA |

### Is 判断函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | UtilFunc.cpp | UtilFunc::IsUsableNameFilter | 0x140001DD0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::IsValidSecondPassword | 0x140004D80 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsBlock | 0x140012F30 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsSendWaitPacket | 0x140029830 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsCancel_Wait | 0x1400298B0 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsLeagueMaster | 0x14002EDA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsEmptySlot | 0x14002FAC0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::IsServerAcceptClosed | 0x140015DF0 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::IsConnection | 0x14003EE50 | verified | IDA |
| XCore | XClient.h | XClient::IsState | 0x1400402D0 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::IsReady | 0x140044010 | verified | IDA |
| XCommon | rapidjson | Double::IsNormal | 0x140027BA0 | verified | IDA |
| XCommon | rapidjson | Double::IsZero | 0x140027CF0 | verified | IDA |
| XCommon | rapidjson | ParseResult::IsError | 0x140028FE0 | verified | IDA |

### Has 判断函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | rapidjson | GenericReader::HasParseError | 0x140021260 | verified | IDA |
| XCommon | rapidjson | GenericReader::HasAllocator | 0x140020840 | verified | IDA |
| XCommon | rapidjson | GenericValue::FindMember | 0x1400204D0 | verified | IDA |

### Make 创建函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::_BackendThreadStartingPoint | 0x140013070 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::MakeMD5 | 0x14011F0D0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_AKASHIC_MAKE | 0x1400C4AD0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_MAKE | 0x1400ED8F0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_AKASHIC_MAKE | 0x140120780 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_MAKE | 0x140145D90 | verified | IDA |
| XCore | hashed_index.h | hashed_index::make_iterator | 0x14001AC90 | verified | IDA |

### Find 查找函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUser | 0x140001480 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUIDToUser | 0x1400185C0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindActor | 0x140018940 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr<CUser>::Find | 0x1400014B0 | verified | IDA |
| XCore | TXServer.h | TXServer<CUser>::FindUser | 0x140001480 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::FindUser | 0x14000A020 | verified | IDA |
| XCore | std::map | std::map::find | 0x140006B50 | verified | IDA |
| XCore | std::map | std::map::find (TB_ITEM_CLASSIFY) | 0x140007350 | verified | IDA |
| XCore | std::map | std::map::find (TB_PROVIDE_ITEM) | 0x1400075E0 | verified | IDA |
| XCore | std::map | std::map::find (TB_CREATE_CLOTH) | 0x140007810 | verified | IDA |
| XCore | std::map | std::map::find (TB_ITEM) | 0x140007A60 | verified | IDA |
| XCore | std::map | std::map::find (TB_CHARACTER_INFO) | 0x140007E30 | verified | IDA |
| XCore | std::map | std::map::find (TB_APPEARANCE) | 0x1400080A0 | verified | IDA |
| XCore | std::map | std::map::find (TB_BROACH_SET) | 0x140011800 | verified | IDA |
| XCore | std::string | std::string::find_last_of | 0x1400172B0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::FindDefaultPhotoItemID | 0x1400745B0 | verified | IDA |
| XCommon | tinyxml | TiXmlAttributeSet::Find | 0x140050C10 | verified | IDA |
| XCommon | ATL | ATL::AtlFindStringResourceInstance | 0x140051680 | verified | IDA |

### Req 请求处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCancel | 0x140015950 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncode | 0x14002D800 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemKeepAlive | 0x14002D910 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |

### Res 响应处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterList | 0x14000A500 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterDelete | 0x14000B920 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeSlot | 0x14000BD00 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeChange | 0x14000BFD0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLogin | 0x14000CF60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer | 0x14000DD70 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWContinue | 0x14000DFE0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWStateCheck | 0x14000E420 | verified | IDA |

### Recv 接收处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.cpp | CXigncode::RecvXigncode | 0x1400013C0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserKickout | 0x1400162E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerShutDown | 0x140016370 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer | 0x140016600 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer | 0x140016730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvMaxServerUserCount | 0x140016B30 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::XRecv | 0x14003CD10 | verified | IDA |
| XCore | XTCPSkeleton.h | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnRecv | 0x14003F500 | verified | IDA |

### Clone 克隆函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | tinyxml | TiXmlDocument::Clone | 0x1400511C0 | verified | IDA |
| XCore | ATL | CAtlStringMgr::Clone | 0x14017525C | verified | IDA |

### Register 注册函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |
| XCore | XClient.h | XClient::Register | 0x1400403D0 | verified | IDA |

### Start 启动函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::_BackendThreadStartingPoint | 0x140013070 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnStartThread | 0x140013240 | verified | IDA |
| XCore | CLogThreadManager.h | CLogThreadManager::Start | 0x14004CD30 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::StartLog | 0x14002D550 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::_BackendThreadStartingPoint | 0x1400436E0 | verified | IDA |
| XRelayServer | RelaySocket.h | XRelaySocket::OnStartThread | 0x140044030 | verified | IDA |
| XLoginServer | ObserveSocket.h | CObserveSocket::StartUp | 0x14008CD10 | verified | IDA |
| XCommon | rapidjson | GenericDocument::StartObject | 0x1400240B0 | verified | IDA |
| XCommon | rapidjson | GenericDocument::StartArray | 0x1400241D0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_MODE_BI_CLASS_STARTSKILL | 0x1400F3EC0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_MODE_BI_CLASS_STARTSKILL | 0x14014B620 | verified | IDA |
| XCore | CRTStartup | __tmainCRTStartup | 0x14017568C | verified | IDA |
| XCore | CRTStartup | mainCRTStartup | 0x1401758DC | verified | IDA |

### Exit 退出函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::ExitUser | 0x140018690 | verified | IDA |
| XCommon | rapidjson | GenericDocument::ClearStackOnExit | 0x140020900 | verified | IDA |
| XCommon | rapidjson | GenericReader::ClearStackOnExit | 0x1400212C0 | verified | IDA |
| XCore | atexit | atexit | 0x140175604 | verified | IDA |
| XCore | atexit | _onexit | 0x140175554 | verified | IDA |
| XCore | atexit | _amsg_exit | 0x140175C74 | verified | IDA |

### Open 打开函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | tinyxml | TiXmlFOpen | 0x1400509C0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitTitleOpenCondition | 0x14007B260 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_MAZE_OPEN_GROUP | 0x1400EF4A0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_SPECIALMAZE_OPEN | 0x140115DF0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_MAZE_OPEN_GROUP | 0x140147760 | verified | IDA |

### Close 关闭函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XIOCPClient.h | XIOCPClient::Close | 0x14003F2A0 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::CloseClient | 0x140041350 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::SQLClose | 0x14004F460 | verified | IDA |
| XCore | XDBBinder.h | XDBBinder::Close | 0x14004F650 | verified | IDA |
| XCore | CRT | fclose | 0x1401759A0 | verified | IDA |

### Connect 连接函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::ConnectUser | 0x1400010C0 | verified | IDA |
| XCommon | Xigncode.h | CXigncode::DisconnectUser | 0x140001150 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::Connect | 0x14003EC50 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::IsConnection | 0x14003EE50 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::DisConnect | 0x14003F260 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::AutoConnect | 0x1400149D0 | verified | IDA |
| XCore | XRelaySocket.h | XRelaySocket::Connect | 0x140043840 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Connect | 0x140050180 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::DisConnect | 0x1400502A0 | verified | IDA |
| XCore | XDBManager.h | XDBManager::GetDBConnect | 0x14004F9A0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.h | XGameDBSocketMgr::AutoConnect | 0x140014170 | verified | IDA |

### Accept 接受函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::IsServerAcceptClosed | 0x140015DF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetServerAcceptClosed | 0x140016C10 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::AcceptThread | 0x140041760 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::XAccept | 0x1400419F0 | verified | IDA |
| XCommon | tinyxml | TiXmlDocument::Accept | 0x140050BA0 | verified | IDA |

### Sort 排序函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SortCharacterList | 0x14002F1D0 | verified | IDA |

### Compare 比较函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | ATL | CDefaultCompareTraits::CompareElements | 0x140001590 | verified | IDA |
| XCommon | rapidjson | BigInteger::Compare | 0x140028540 | verified | IDA |
| XCore | std::string | basic_string::compare | 0x14004BAB0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::CompareCashShop | 0x140051E30 | verified | IDA |

### Validate 验证函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | rapidjson | Transcoder::Validate | 0x140025A20 | verified | IDA |
| XCommon | rapidjson | Transcoder::Validate | 0x140025BD0 | verified | IDA |
| XCommon | rapidjson | UTF8::Validate | 0x140025D90 | verified | IDA |
| XCommon | rapidjson | UTF8::Validate | 0x1400263F0 | verified | IDA |
| XCore | CRT | _ValidateImageBase | 0x140175D00 | verified | IDA |

### Check 检查函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | UtilFunc.cpp | UtilFunc::CheckValidString | 0x140001910 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeCheckResult | 0x1400068E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetRepresentativeCheck | 0x140006900 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendCheckSessionID | 0x1400088E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeCheck | 0x14000BEB0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName | 0x14000CEA0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCheck | 0x14000E280 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWStateCheck | 0x14000E420 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetSendCheckSessionID | 0x140016BF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::CheckUserWaitCountSend | 0x140019170 | verified | IDA |
| XCommon | rapidjson | Double::CheckWithinHalfULP | 0x140028620 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckSecondPasswordState | 0x14002EC90 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckLeagueMaster | 0x14002ED40 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckCreateDate | 0x14002EFB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckChangeSlot | 0x14002F630 | verified | IDA |
| XLoginServer | User.cpp | CUser::CheckRepresentativeChange | 0x14002FC60 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::CheckRankingTime | 0x140074640 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::CheckSum | 0x14011F220 | verified | IDA |
| XCore | CRT | __security_check_cookie | 0x1401753A0 | verified | IDA |
| XCore | CRT | __GSHandlerCheckCommon | 0x14017590C | verified | IDA |
| XCore | CRT | __GSHandlerCheck | 0x140175970 | verified | IDA |
| XCore | CRT | __GSHandlerCheck_EH | 0x1401759B8 | verified | IDA |

### Count 计数函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::GetCharacterCount | 0x1400083E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterCount | 0x1400122E0 | verified | IDA |
| XCore | ATL | CTime::GetTickCount | 0x140012240 | verified | IDA |
| XCore | XTime.h | XTime::GetTickCount | 0x1400131B0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetStatisticsDBAgentCount | 0x140014A10 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x140014A20 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140014A30 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvMaxServerUserCount | 0x140016B30 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::UpdateMaxUserCount | 0x140019120 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::AddSendGameDBUserCount | 0x140019280 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::GetSendGameDBUserCount | 0x1400192F0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetLogDBAgentCount | 0x1400337E0 | verified | IDA |

### Size 大小函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std::vector | vector::size | 0x140005D80 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x140005DA0 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x140006050 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x140008D40 | verified | IDA |
| XCore | std::string | basic_string::max_size | 0x140008D80 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x1400097D0 | verified | IDA |
| XCore | std::wstring | basic_string::max_size | 0x140009810 | verified | IDA |
| XCore | std::vector | vector::size | 0x14000F440 | verified | IDA |
| XCore | std::set | set::max_size | 0x140010EC0 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x140011600 | verified | IDA |
| XCore | std::vector | vector::size | 0x140011690 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x1400116C0 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x1400123E0 | verified | IDA |
| XCore | boost::multi_index | hashed_index::size | 0x14001A150 | verified | IDA |
| XCore | Concurrency | concurrent_queue::unsafe_size | 0x14001A4F0 | verified | IDA |
| XCore | boost::multi_index | index_base::final_size_ | 0x14001ACF0 | verified | IDA |
| XCore | std::vector | vector::size | 0x14001B6B0 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x14001B6E0 | verified | IDA |
| XCore | boost::multi_index | multi_index_container::size_ | 0x14001B920 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x14001C470 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x14001F7C0 | verified | IDA |
| XCore | TXPool.h | TXPool::GetFullSize | 0x140020110 | verified | IDA |
| XCore | TXPool.h | TXPool::GetCurSize | 0x140020130 | verified | IDA |
| XCore | std::queue | queue::size | 0x140020230 | verified | IDA |
| XCore | std::deque | deque::size | 0x140020300 | verified | IDA |
| XCommon | rapidjson | Double::EffectiveSignificandSize | 0x140027100 | verified | IDA |
| XCommon | rapidjson | Stack::Resize | 0x140028F30 | verified | IDA |
| XCommon | rapidjson | Stack::GetSize | 0x140028FC0 | verified | IDA |
| XCore | std::list | list::max_size | 0x14002A980 | verified | IDA |
| XCore | std::deque | deque::max_size | 0x14002AD90 | verified | IDA |
| XCore | ATL | CAtlMap::PickSize | 0x14002C080 | verified | IDA |
| XCore | std::vector | vector::size | 0x140030070 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x140030860 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x140030A40 | verified | IDA |
| XCore | std::map | map::max_size | 0x140031AE0 | verified | IDA |
| XCore | std::vector | vector::size | 0x1400324F0 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x140036E50 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x140037090 | verified | IDA |
| XCore | std::vector | vector::size | 0x140037D70 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x140038100 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x1400381F0 | verified | IDA |
| XCore | std::vector | vector::size | 0x140038F70 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x1400392D0 | verified | IDA |
| XCore | std::vector | vector::size | 0x14003B040 | verified | IDA |
| XCore | std::vector | vector::size | 0x14003B070 | verified | IDA |
| XCore | std::vector | vector::size | 0x14003B0A0 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x14003B380 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x14003B470 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x14003B610 | verified | IDA |
| XCore | std::vector | vector::max_size | 0x14003B7C0 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x14003B8F0 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x14003B990 | verified | IDA |
| XCore | std::allocator | allocator::max_size | 0x14003BA30 | verified | IDA |
| XCore | ATL | CAtlMap::PickSize | 0x140040B70 | verified | IDA |
| XCore | ATL | CWin32Heap::GetSize | 0x1401751E8 | verified | IDA |

### Empty 空判断函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | boost::multi_index | bucket_array::first_nonempty | 0x14001D5D0 | verified | IDA |
| XCommon | rapidjson | Stack::Empty | 0x140020A70 | verified | IDA |
| XCore | Concurrency | concurrent_queue::empty | 0x140029E00 | verified | IDA |
| XCore | std::queue | queue::empty | 0x14002C530 | verified | IDA |
| XCore | std::deque | deque::empty | 0x14002C550 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsEmptySlot | 0x14002FAC0 | verified | IDA |
| XCore | ATL | CSimpleStringT::Empty | 0x140051D00 | verified | IDA |

### Push 推入函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std::vector | vector::push_back | 0x1400057D0 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x14000E910 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::PushWaitUser | 0x140018A40 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x140019E50 | verified | IDA |
| XCore | Concurrency | concurrent_queue::push | 0x14001A490 | verified | IDA |
| XCommon | rapidjson | GenericReader::TakePush | 0x140024440 | verified | IDA |
| XCommon | rapidjson | Stack::Push | 0x140024C80 | verified | IDA |
| XCommon | rapidjson | Stack::Push | 0x1400255D0 | verified | IDA |
| XCommon | rapidjson | Stack::PushUnsafe | 0x140025760 | verified | IDA |
| XCommon | rapidjson | Stack::PushUnsafe | 0x140025C40 | verified | IDA |
| XCommon | rapidjson | Stack::PushUnsafe | 0x140026D30 | verified | IDA |
| XCommon | rapidjson | Stack::Push | 0x140026D70 | verified | IDA |
| XCommon | rapidjson | BigInteger::PushBack | 0x140028150 | verified | IDA |
| XCore | std::list | list::push_back | 0x14002AA90 | verified | IDA |
| XCore | std::deque | deque::push_back | 0x14002B3C0 | verified | IDA |
| XCore | std::queue | queue::push | 0x14002B530 | verified | IDA |
| XCore | TXPool.h | TXPool::Push | 0x14002B560 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x14002FF20 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x1400300C0 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x140036B30 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x140037DA0 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x140038F90 | verified | IDA |
| XCore | TXPool.h | TXPool::Push | 0x14003D870 | verified | IDA |
| XCore | std::deque | deque::push_back | 0x14003D910 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x140049970 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x14007EB20 | verified | IDA |
| XCore | std::vector | vector::push_back | 0x140081F30 | verified | IDA |

### Pop 弹出函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::PopWaitUser | 0x140018B60 | verified | IDA |
| XCore | Concurrency | concurrent_queue::try_pop | 0x14001A4C0 | verified | IDA |
| XCore | TXPool.h | TXPool::Pop | 0x14001F800 | verified | IDA |
| XCommon | rapidjson | Stack::Pop | 0x140020EF0 | verified | IDA |
| XCommon | rapidjson | StackStream::Pop | 0x1400243C0 | verified | IDA |
| XCommon | rapidjson | Stack::Pop | 0x140024CE0 | verified | IDA |
| XCommon | rapidjson | Stack::Pop | 0x140025670 | verified | IDA |
| XCommon | rapidjson | Stack::Pop | 0x140026BB0 | verified | IDA |
| XCore | std::deque | deque::pop_back | 0x14002B790 | verified | IDA |
| XCore | std::deque | deque::pop_front | 0x14002C580 | verified | IDA |
| XCore | std::queue | queue::pop | 0x14002C680 | verified | IDA |
| XCore | XDump.h | XDump::XGenerateDump_Popup | 0x14004E980 | verified | IDA |
| XCore | TXPool.h | TXPool::Pop | 0x14004FCA0 | verified | IDA |

### Swap 交换函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std | swap<char> | 0x140011640 | verified | IDA |
| XCore | boost::multi_index | bucket_array::swap | 0x14001E180 | verified | IDA |
| XCore | boost::multi_index | auto_space::swap | 0x14001E210 | verified | IDA |
| XCore | std | swap<hashed_index_node_impl> | 0x14001EC60 | verified | IDA |
| XCore | boost::multi_index | adl_swap | 0x14001ED30 | verified | IDA |
| XCore | std | swap<allocator> | 0x140020020 | verified | IDA |

### Hash 哈希函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | ATL | CAtlMap::CNode::GetHash | 0x140001580 | verified | IDA |
| XCore | boost::multi_index | multi_index_container ctor | 0x14001A070 | verified | IDA |
| XCore | boost::multi_index | hashed_index::size | 0x14001A150 | verified | IDA |
| XCore | boost::multi_index | hashed_index::begin | 0x14001A170 | verified | IDA |
| XCore | boost::multi_index | hashed_index::end | 0x14001A1D0 | verified | IDA |
| XCore | boost::multi_index | hashed_index::insert | 0x14001A210 | verified | IDA |
| XCore | boost::multi_index | hashed_index::erase | 0x14001A280 | verified | IDA |
| XCore | boost::multi_index | hashed_index::bucket_count | 0x14001C510 | verified | IDA |
| XCore | boost::multi_index | hashed_index::unchecked_rehash | 0x14001DC00 | verified | IDA |
| XCore | boost | hash_value<int> | 0x140029170 | verified | IDA |
| XCore | boost | hash<int>::operator() | 0x140029180 | verified | IDA |
| XCore | ATL | CAtlMap::DisableAutoRehash | 0x14002BAC0 | verified | IDA |
| XCore | ATL | CAtlMap::Rehash | 0x14002BAE0 | verified | IDA |
| XCore | ATL | CAtlMap::UpdateRehashThresholds | 0x14002BE60 | verified | IDA |
| XCore | ATL | CAtlMap::InitHashTable | 0x14002BF40 | verified | IDA |
| XCore | ATL | CAtlMap::EnableAutoRehash | 0x14002C210 | verified | IDA |
| XCore | ATL | CAtlMap::InitHashTable | 0x140040A40 | verified | IDA |
| XCore | ATL | CAtlMap::Rehash | 0x140040D20 | verified | IDA |

### Encrypt 加密函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XClient.h | XClient::SetEncrypt | 0x1400321E0 | verified | IDA |
| XCore | XSendPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |

### Clear 清除函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXProcess.h | TXProcess<CUser>::Clear | 0x140006130 | verified | IDA |
| XCore | std::set | set::clear | 0x14000EB80 | verified | IDA |
| XCore | std::vector | vector::clear | 0x140012C60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Clear | 0x140017D50 | verified | IDA |
| XCore | boost::multi_index | bucket_array::clear | 0x14001C6B0 | verified | IDA |
| XCore | std::map | map::clear | 0x14001CFE0 | verified | IDA |
| XCommon | rapidjson | MemoryPoolAllocator::Clear | 0x14001D100 | verified | IDA |
| XCommon | rapidjson | GenericDocument::ClearStack | 0x140020920 | verified | IDA |
| XCommon | rapidjson | Stack::Clear | 0x1400209E0 | verified | IDA |
| XCommon | rapidjson | GenericReader::ClearStack | 0x140021300 | verified | IDA |
| XCommon | rapidjson | ParseResult::Clear | 0x140028EA0 | verified | IDA |
| XCore | Concurrency | concurrent_queue::clear | 0x140029E20 | verified | IDA |
| XCore | std::map | map::clear | 0x14002A1F0 | verified | IDA |
| XCore | std::list | list::clear | 0x14002C6D0 | verified | IDA |
| XCore | TXPool.h | TXPool::Clear | 0x14002C7D0 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::Clear | 0x14002C910 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearCharacterInfo | 0x14002E590 | verified | IDA |
| XLoginServer | User.cpp | CUser::ClearLeagueInfo | 0x14002ED70 | verified | IDA |
| XCore | std::vector | vector::clear | 0x1400326D0 | verified | IDA |
| XCore | std::vector | vector::clear | 0x1400332D0 | verified | IDA |
| XCore | TXPool.h | TXPool::Clear | 0x14003E3D0 | verified | IDA |
| XCore | XClient.h | XClient::ClearState | 0x140040300 | verified | IDA |
| XCore | TXPool.h | TXPool::Clear | 0x140041D10 | verified | IDA |
| XCore | std::map | map::clear | 0x14004A690 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::Clear | 0x14004F2E0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Clear | 0x1400500D0 | verified | IDA |
| XCommon | tinyxml | TiXmlNode::Clear | 0x140050A50 | verified | IDA |

### Init 初始化函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::Init | 0x140001000 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::Init | 0x140006110 | verified | IDA |
| XCommon | STItem.cpp | STItem::Init | 0x140007B90 | verified | IDA |
| XCommon | STPosInfo.cpp | STPosInfo::Init | 0x1400081F0 | verified | IDA |
| XCommon | STCharInfoEx.cpp | STCharInfoEx::Init | 0x140008240 | verified | IDA |
| XCommon | STMyCharInfoEx.cpp | STMyCharInfoEx::Init | 0x140008290 | verified | IDA |
| XCommon | STEquipBase.cpp | STEquipBase::Init | 0x1400085E0 | verified | IDA |
| XCommon | ST_EQUIP_ITEM_INFO.cpp | ST_EQUIP_ITEM_INFO::Init | 0x140008600 | verified | IDA |
| XCommon | STCharInfo.cpp | STCharInfo::Init | 0x140008630 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Init | 0x140012CF0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::Init | 0x1400130B0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140013390 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::InitServer | 0x140017960 | verified | IDA |
| XCore | XSeed.cpp | XSeed::Init | 0x140029BA0 | verified | IDA |
| XLoginServer | XOption.cpp | XOption::SetInitXignCode | 0x140029BD0 | verified | IDA |
| XCore | TXPool.h | TXPool::Init | 0x14002AAD0 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::Init | 0x14002AB70 | verified | IDA |
| XCore | ATL | CAtlMap::InitHashTable | 0x14002BF40 | verified | IDA |
| XLoginServer | ServerMain.cpp | ServiceInit | 0x14002CCC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::Init | 0x14002E050 | verified | IDA |
| XCore | XSocket.h | XSocket::Init | 0x1400327C0 | verified | IDA |
| XCore | XClient.h | XClient::Init | 0x140032870 | verified | IDA |
| XCommon | STUserInfo.cpp | STUserInfo::Init | 0x140033330 | verified | IDA |
| XCore | XOverLab.h | XOverLab::Init | 0x14003D240 | verified | IDA |
| XCore | TXPool.h | TXPool::Init | 0x14003D770 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::Init | 0x14003E7E0 | verified | IDA |
| XCore | CSimpleLock.h | CSimpleLock::Init | 0x14003FBD0 | verified | IDA |
| XCore | XClient.h | XClient::Init | 0x140040340 | verified | IDA |
| XCore | ATL | CAtlMap::InitHashTable | 0x140040A40 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::Init | 0x140041170 | verified | IDA |
| XCore | XProcessComposite.cpp | XProcessComposite::Init | 0x140041F20 | verified | IDA |
| XCore | XRelaySocket.h | XRelaySocket::Init | 0x140043650 | verified | IDA |
| XCore | XServer.h | XServer::Init | 0x14004BCF0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | XDBStmt.h | XDBStmt::Init | 0x14004F350 | verified | IDA |
| XCore | XDBManager.h | XDBManager::Init | 0x14004F8D0 | verified | IDA |
| XCore | XDBConnect.h | XDBConnect::Init | 0x14004FFA0 | verified | IDA |
| XCore | XDBEnv.h | XDBEnv::Init | 0x140050650 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitGameDB | 0x140051D80 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitCharacterInfo | 0x1400744F0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitWeeklyEventGroupID | 0x14007A3C0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitMaze | 0x14007A4C0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitInfiniteTowerTable | 0x14007B070 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitTitleOpenCondition | 0x14007B260 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitDefaultPhotoItemID | 0x14007B860 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitQuestTable | 0x14007BA00 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitPCCostume | 0x14007C5C0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitPCAkashic | 0x14007C780 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Init | 0x14007EA50 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitPartyRevise | 0x140082470 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitRankingInfoTable | 0x1400830A0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitNetCafeMissionList | 0x1400836D0 | verified | IDA |

### Update 更新函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.cpp | CXigncode::RecvXigncode | 0x1400013C0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap | 0x14000DEE0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::UpdateMaxUserCount | 0x140019120 | verified | IDA |
| XCore | ATL | CAtlMap::UpdateRehashThresholds | 0x14002BE60 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::SendUpdateServerInfo | 0x140043D90 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::RecvServerUpdate | 0x140043E80 | verified | IDA |
| XCore | XServer.h | XServer::OnUpdate | 0x14004BF60 | verified | IDA |
| XLoginServer | ObserveSocket.cpp | CObserveSocket::OnUpdate | 0x14008D5B0 | verified | IDA |

### Create 创建函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResCharacterCreate | 0x14000AA80 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSecondPWCreate | 0x14000E0E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XCore | TXServer.h | TXServer::Create | 0x14001B350 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr::Create | 0x14001E280 | verified | IDA |
| XCore | ATL | CAtlMap::CreateNode | 0x1400207A0 | verified | IDA |
| XCore | ATL | CAtlPlex::Create | 0x140028DD0 | verified | IDA |
| XCore | TXPool.h | TXCreator::Create | 0x14002A770 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCreateDate | 0x14002EF00 | verified | IDA |
| XCore | XOverLab.h | XCreator::Create | 0x14003D710 | verified | IDA |
| XCore | XIOCPSkeleton.cpp | XIOCPSkeleton::CreateIOCP | 0x14003DE00 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XCore | CLogThreadManager.cpp | CLogThreadManager::CreateWorkerThread | 0x14004D290 | verified | IDA |
| XCore | CFThread.h | CFThread::Create | 0x14004D600 | verified | IDA |
| XCore | XDBManager.cpp | XDBCreator::Create | 0x14004F730 | verified | IDA |
| XCore | XItemFactory.cpp | XItemFactory::CreateItem | 0x14008BF30 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_CREATE_CLOTH | 0x1400D0640 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_CREATEOPTION | 0x1400D0A60 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_CREATE_CLOTH | 0x14012ABE0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_CREATEOPTION | 0x14012AF30 | verified | IDA |

### Process 处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::Parse | 0x140002250 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterList | 0x1400023F0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate | 0x140002550 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete | 0x140003AF0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName | 0x140004570 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeServer | 0x140004680 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword | 0x1400047C0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterChangeSlot | 0x140004FC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck | 0x1400051D0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange | 0x140005460 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::Parse | 0x140014B30 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqServerConnect | 0x140015240 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate | 0x140015730 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCancel | 0x140015950 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::ServerProcessEx | 0x140015F90 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ProcessWaitUser | 0x140018C00 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::Parse | 0x14002D660 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqOptionUpdate | 0x14002D780 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncode | 0x14002D800 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemXigncodeError | 0x14002D8D0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemKeepAlive | 0x14002D910 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemSGTokenUpdate | 0x14002D9E0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemGameGuardAuth | 0x14002DAE0 | verified | IDA |
| XLoginServer | SystemProcess.cpp | CSystemProcess::ReqSystemGameGuardError | 0x14002DB60 | verified | IDA |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |
| XCore | XProcessComposite.cpp | XProcessComposite::Parse | 0x140042040 | verified | IDA |
| XCore | XProcessComposite.cpp | XProcessComposite::Init | 0x140041F20 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::ServerProcess | 0x140043A40 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::UserProcess | 0x140043AF0 | verified | IDA |
| XCommon | md5.cpp | md5_process | 0x140174430 | verified | IDA |

### Parse 解析函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XParse.h | XParse::SetBYTE | 0x140006E60 | verified | IDA |
| XCore | XParse.h | XParse::GetBIGINT | 0x140008970 | verified | IDA |
| XCore | XParse.h | XParse::GetBytes | 0x140011B80 | verified | IDA |
| XCore | XParse.h | XParse::GetString | 0x140011BE0 | verified | IDA |
| XCore | XParse.h | XParse::GetWORD | 0x140012290 | verified | IDA |
| XCore | XParse.h | XParse::GetBYTE | 0x140012870 | verified | IDA |
| XCore | XParse.h | XParse::GetDWORD | 0x140034140 | verified | IDA |
| XCore | XParse.h | XParse::SetDWORD | 0x140034180 | verified | IDA |
| XCore | XParse.h | XParse::GetFLOAT | 0x140034260 | verified | IDA |
| XCore | XParse.h | XParse::SetFLOAT | 0x1400342B0 | verified | IDA |
| XCore | XParse.h | XParse::SetWORD | 0x140015AD0 | verified | IDA |
| XCore | XParse.h | XParse::SetBytes | 0x140015B10 | verified | IDA |
| XCore | XParse.h | XParse::GetWString | 0x140015D00 | verified | IDA |
| XCore | XParse.h | XParse::SetUsIndex | 0x140015BF0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBParse | 0x14000A0E0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBLoginParse | 0x14000A160 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBCharacterParse | 0x14000A340 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnParse | 0x140012F80 | verified | IDA |
| XCommon | rapidjson | GenericDocument::Parse | 0x14001A890 | verified | IDA |
| XCore | XClient.h | XClient::Parse | 0x140040310 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::OnParse | 0x1400438B0 | verified | IDA |

### Load 加载函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResOptionLoad | 0x14000DC40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSGAuthInfoLoad | 0x14000E4F0 | verified | IDA |
| XLoginServer | XOption.cpp | XOption::Load | 0x140045C20 | verified | IDA |
| XLoginServer | XOption.cpp | XOption::GetResLoadType | 0x140029B30 | verified | IDA |
| XCore | XServer.h | XServer::LoadConfig | 0x14004BF20 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load | 0x14008B3A0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::ServerOptionLoad | 0x140051740 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::LoadFactionInfo | 0x14007AEA0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::LoadGachaGroupTable | 0x14007B3F0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::CashshopLoad | 0x1400825C0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::BannerLoad | 0x140082DE0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::LoadKRRData | 0x1400832F0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::ServerChannelInfoLoad | 0x140087D10 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::CashShopTabLoad | 0x14008A740 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_ACHIEVEMENT | 0x1400C3390 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_CHARACTER_INFO | 0x1400CBEE0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_ITEM | 0x1400E21B0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_MAZE_INFO | 0x1400EE5F0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_SELECT_ITEM | 0x14010F980 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_MAZE_ENTER_COUNT_GROUP | 0x1400EE090 | verified | IDA |
| XCommon | tinyxml | TiXmlDocument::LoadFile | 0x140050EF0 | verified | IDA |
| XCommon | tinyxml | TiXmlDocument::LoadFile | 0x140051290 | verified | IDA |
| XCommon | tinyxml | TiXmlDocument::LoadFile | 0x1400513D0 | verified | IDA |
| XCore | ATL | CStringT::LoadStringA | 0x140057570 | verified | IDA |

### Select 选择函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter | 0x140003F40 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResSelectCharacter | 0x14000C1C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSelectUCID | 0x140011F90 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetLastSelectUCID | 0x140012030 | verified | IDA |
| XLoginServer | User.cpp | CUser::UpdateCharacterMapInfo | 0x14002F0F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetLastSelectUCID | 0x140032730 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_SELECT_ITEM | 0x14010F980 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_SELECT_ITEM | 0x140166890 | verified | IDA |
| XCommon | PSCommon.h | PS_CHARACTER_SELECT::PS_CHARACTER_SELECT | 0x140006E10 | verified | IDA |

### Enter 进入函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetEnterServerState | 0x140006C80 | verified | IDA |
| XLoginServer | User.cpp | CUser::GetEnterServerState | 0x140008420 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResEnterServer | 0x14000D410 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer | 0x140015380 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck | 0x140015860 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCancel | 0x140015950 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::EnterUser | 0x1400183B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer | 0x140016730 | verified | IDA |
| XLoginServer | User.cpp | CUser::CanEnterGame | 0x14002F8B0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_TB_MAZE_ENTER_COUNT_GROUP | 0x1400EE090 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::Load_Server_TB_MAZE_ENTER_COUNT_GROUP | 0x140146480 | verified | IDA |

### Login 登录函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::DBLoginParse | 0x14000A160 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLogin | 0x14000CF60 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount | 0x14000D9F0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqUserLogin | 0x140014C60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::Instance | 0x140001410 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer | 0x140017550 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBAccount | 0x140019900 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBGame | 0x1400199B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBLog | 0x140019A40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendDBStatistics | 0x140019AF0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUIDToUser | 0x1400185C0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::ExitUser | 0x140018690 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::AddActor | 0x140018800 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::RemoveActor | 0x140018880 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindActor | 0x140018940 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::KickoutAll | 0x140018290 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB | 0x1400196D0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserKickout | 0x1400162E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer | 0x140016600 | verified | IDA |

### Logout 登出函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::OnLogOut | 0x14002E230 | verified | IDA |
| XCore | XServer.h | XServer::OnLogOut | 0x14004BCD0 | verified | IDA |

### Register 注册函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::RegisterProcess | 0x14002DF20 | verified | IDA |
| XCore | XClient.h | XClient::Register | 0x1400403D0 | verified | IDA |

### Release 释放函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::Release | 0x140001080 | verified | IDA |
| XCore | cIoContextPool.cpp | cIoContextPool::ReleasePool | 0x14004F0D0 | verified | IDA |
| XCore | cIoContextPool.cpp | cIoContextPool::ReleaseIoContext | 0x14004F140 | verified | IDA |

### Free 释放函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | rapidjson | CrtAllocator::Free | 0x140029460 | verified | IDA |
| XCore | ATL | CAtlMap::FreeNode | 0x14002BD30 | verified | IDA |
| XCore | ATL | CAtlPlex::FreeDataChain | 0x14002C170 | verified | IDA |
| XCore | ATL | CAtlMap::FreePlexes | 0x14002C1C0 | verified | IDA |
| XCore | XIOPool.h | XIOPool::FreeIO | 0x14003D470 | verified | IDA |
| XCore | XClientPool.h | XClientPool::FreeClient | 0x140042500 | verified | IDA |
| XCore | ATL | CWin32Heap::Free | 0x140175188 | verified | IDA |
| XCore | ATL | CAtlStringMgr::Free | 0x140175244 | verified | IDA |
| XCore | CRT | free | 0x14017561C | verified | IDA |

### Allocate 分配函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | rapidjson | CrtAllocator::Malloc | 0x140029440 | verified | IDA |
| XCommon | rapidjson | MemoryPoolAllocator::Malloc | 0x140028880 | verified | IDA |
| XCore | std::allocator | allocator::allocate | 0x140006030 | verified | IDA |
| XCore | std::allocator | allocator::allocate | 0x140008E40 | verified | IDA |
| XCore | std::allocator | allocator::allocate | 0x14001D5B0 | verified | IDA |
| XCore | ATL | CAtlMalloc::allocate | 0x1400295A0 | verified | IDA |
| XCore | CRT | malloc | 0x140175604 | verified | IDA |
| XCore | CRT | calloc | 0x1401755F4 | verified | IDA |
| XCore | CRT | realloc | 0x1401755A0 | verified | IDA |

### Send 发送函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::SendProc | 0x1400011A0 | verified | IDA |
| XCore | TXProcess.h | TXProcess<CUser>::SendErrorMessage | 0x140006150 | verified | IDA |
| XCore | XSendDBPacket.h | XSendDBPacket::XSendDBPacket | 0x1400071D0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::SendStatisticsDB_Item | 0x14000E510 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x1400147D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x140014850 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendLogDBAgent | 0x1400148D0 | verified | IDA |
| XLoginServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendStatisticsDBAgent | 0x140014950 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList | 0x1400150B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::CheckUserWaitCountSend | 0x140019170 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SendServerGroupList | 0x140019340 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendCharacterList | 0x14002E840 | verified | IDA |
| XLoginServer | User.cpp | CUser::SendServerOption | 0x14002EE50 | verified | IDA |
| XCore | XSendPacket.h | XSendPacket::XSendPacket | 0x14003CB00 | verified | IDA |
| XCore | XSendPacket.h | XSendPacket::Encrypt | 0x14003CB50 | verified | IDA |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::XSend | 0x14003CC40 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::Send | 0x14003EE60 | verified | IDA |
| XCore | XClient.h | XClient::SendEx | 0x140040440 | verified | IDA |
| XCore | XClient.h | XClient::SendErrorMessage | 0x140040470 | verified | IDA |
| XCore | XIOCPServer.h | XIOCPServer::XSend | 0x140041B40 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::SendAddServer | 0x140043CD0 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::SendUpdateServerInfo | 0x140043D90 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::SendCreateMazeReq | 0x140043EB0 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::SendUserKickout | 0x140043F60 | verified | IDA |
| XCore | CCurlWrapper.cpp | CCurlWrapper::SendHttpPost | 0x140044450 | verified | IDA |
| XCore | CCurlWrapper.cpp | CCurlWrapper::SendHttpsPost | 0x140044670 | verified | IDA |
| XCore | CCurlWrapper.cpp | CCurlWrapper::SendHttpsPost | 0x1400448A0 | verified | IDA |
| XCore | CCurlWrapper.cpp | CCurlWrapper::SendSOAPHttpsPost | 0x140044B00 | verified | IDA |
| XLoginServer | ObserveSocket.cpp | CObserveSocket::SendReportServerStatus | 0x14008D440 | verified | IDA |
| XCore | LogHelper.cpp | LogHelper::LogInfo | 0x140043160 | verified | IDA |

### Recv 接收函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | Xigncode.h | CXigncode::RecvXigncode | 0x1400013C0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes | 0x140016040 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserKickout | 0x1400162E0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerShutDown | 0x140016370 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID | 0x1400163B0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer | 0x140016600 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer | 0x140016730 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvServerOptionUpdate | 0x140016AD0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::RecvMaxServerUserCount | 0x140016B30 | verified | IDA |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::XRecv | 0x14003CD10 | verified | IDA |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnRecv | 0x14003F500 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::RecvServerUpdate | 0x140043E80 | verified | IDA |

### Lock 锁定函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | CFSRWLock.h | CFSRWLock::lock_shared | 0x1400299C0 | verified | IDA |
| XCore | CFSRWLock.h | CFSRWLock::lock | 0x140029A80 | verified | IDA |
| XCore | CFAutoSlimReadLock.h | CFAutoSlimReadLock::CFAutoSlimReadLock | 0x1400299E0 | verified | IDA |
| XCore | CFAutoSlimWriteLock.h | CFAutoSlimWriteLock::CFAutoSlimWriteLock | 0x140029AA0 | verified | IDA |
| XCore | CSimpleLock.h | CSimpleLock::Lock | 0x14003FC00 | verified | IDA |
| XCore | CSimpleLock.h | CSimpleLock::Owner::Owner | 0x14003FC80 | verified | IDA |
| XCore | TXPool.h | TXPool::SafeArrayLock | 0x14002C370 | verified | IDA |
| XCore | XIOCPSkeleton.cpp | XIOCPSkeleton::BlockSocket | 0x14003E010 | verified | IDA |

### Unlock 解锁函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | CFSRWLock.h | CFSRWLock::unlock_shared | 0x140029980 | verified | IDA |
| XCore | CFSRWLock.h | CFSRWLock::unlock | 0x140029A40 | verified | IDA |
| XCore | CSimpleLock.h | CSimpleLock::UnLock | 0x14003FC40 | verified | IDA |
| XCore | TXPool.h | TXPool::SafeArrayUnLock | 0x14002C7A0 | verified | IDA |

### Construct 构造函数 (std::allocator)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std::allocator | allocator::construct | 0x1400062F0 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x1400106A0 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x1400114C0 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x1400114F0 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x14001ED60 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x14001FC90 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x14002B350 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x1400316D0 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x140031700 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x140032060 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x140032160 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x140038330 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x140038530 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x140039460 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x14003C5A0 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x14003C640 | verified | IDA |
| XCore | std::allocator | allocator::construct | 0x14003C6B0 | verified | IDA |

### Write 写入函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLog | 0x140019640 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::WriteLogDB | 0x1400196D0 | verified | IDA |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::WriteLog | 0x14002A6F0 | verified | IDA |
| XCore | XIOCPSkeleton.cpp | XIOCPSkeleton::WriteLog | 0x14003DCB0 | verified | IDA |
| XCore | CCurlWrapper.cpp | write_data | 0x1400442F0 | verified | IDA |
| XCore | XDump.cpp | XDump::XWriteStackDetails | 0x14004E5B0 | verified | IDA |
| XCore | XDump.cpp | XDump::WriteDump | 0x14004E9E0 | verified | IDA |
| XCommon | ATL | CSimpleStringT::PrepareWrite2 | 0x1400520E0 | verified | IDA |

### Read/Thread 线程读取函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::_BackendThreadStartingPoint | 0x140013070 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnStartThread | 0x140013240 | verified | IDA |
| XLoginServer | XOption.cpp | XOption::GetLogicThread | 0x140016C70 | verified | IDA |
| XCore | LogThread | CLogThreadManager::Instance | 0x14001A580 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::ConnectThread | 0x14003F370 | verified | IDA |
| XCore | XIOCPSkeleton.cpp | XIOCPSkeleton::WorkerThread | 0x14003E230 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::_BackendThreadStartingPoint | 0x1400436E0 | verified | IDA |
| XCore | CThreadBase.cpp | CThreadBase::CThreadBase | 0x14004BFB0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | CLogThreadManager.cpp | CLogThreadManager::CreateWorkerThread | 0x14004D290 | verified | IDA |
| XLoginServer | ObserveSocket.cpp | CObserveSocket::CalculateThreadStatus | 0x14008D0E0 | verified | IDA |

### Set 设置函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::SetWaitRepresentativePacketRes | 0x1400068C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitChangeSlotPacketRes | 0x140006940 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEnterServerState | 0x140006C80 | verified | IDA |
| XCore | XParse.h | XParse::SetBYTE | 0x140006E60 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitDeleteCharacterPacketRes | 0x140006EA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTickCreateCharacterPacketRes | 0x140007110 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetWaitCreateCharacterPacketRes | 0x140007130 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendCheckSessionID | 0x1400088E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID | 0x140008950 | verified | IDA |
| XCore | IXComponent.h | IXComponent::SetName | 0x1400093B0 | verified | IDA |
| XCore | IXComponent.h | IXComponent::SetCmd | 0x1400093F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAccountID | 0x140011AB0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetBlockType | 0x140011B20 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTradePWState | 0x140011B40 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSecondPWState | 0x140011B60 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSelectUCID | 0x140011F90 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeCheckResult | 0x140011FF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeCheck | 0x140012010 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterCount | 0x1400122E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetRepresentativeUCID | 0x140012340 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCharacterMapList | 0x140012830 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::SetSafetyShutdown | 0x140013190 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetCancel_Wait | 0x1400159F0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSendWaitPacket | 0x140015A30 | verified | IDA |
| XCore | XParse.h | XParse::SetWORD | 0x140015AD0 | verified | IDA |
| XCore | XParse.h | XParse::SetBytes | 0x140015B10 | verified | IDA |
| XCore | XParse.h | XParse::SetUsIndex | 0x140015BF0 | verified | IDA |
| XLoginServer | LoginControlSocket.cpp | CLoginControlSocket::SetMyInfo | 0x140015E10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetServerAcceptClosed | 0x140016C10 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::SetName | 0x140017930 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAddTime_Wait | 0x1400298D0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetAuthSessionID_Wait | 0x140029900 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetLastServerIndex_Wait | 0x140029920 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetTicket_Wait | 0x140029940 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetUAID_Wait | 0x140029960 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetDeleteUserInfo | 0x140029A20 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::SetServerContents | 0x140029AE0 | verified | IDA |
| XLoginServer | XOption.cpp | XOption::SetInitXignCode | 0x140029BD0 | verified | IDA |
| XCore | TXPool.h | TXPool::SetSafeArrayFlag | 0x14002A820 | verified | IDA |
| XCore | ServiceMain.cpp | SET_SERVICE_STATE | 0x14002CC30 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetSGAuthInfo | 0x14002DBE0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetUAID | 0x14002E570 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonLevel | 0x14002ECC0 | verified | IDA |
| XLoginServer | User.cpp | CUser::SetEchelonExp | 0x14002ED00 | verified | IDA |

### Find 查找函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXServer.h | TXServer<CUser>::FindUser | 0x140001480 | verified | IDA |
| XCore | TXObjectMgr.h | TXObjectMgr<CUser>::Find | 0x1400014B0 | verified | IDA |
| XLoginServer | GameDBSocket.cpp | XGameDBSocket::FindUser | 0x14000A020 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindUIDToUser | 0x1400185C0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::FindActor | 0x140018940 | verified | IDA |
| XCore | std::string | char_traits::find | 0x1400170D0 | verified | IDA |
| XCore | std::string | basic_string::find_last_of | 0x1400172B0 | verified | IDA |
| XCommon | rapidjson | GenericValue::FindMember | 0x1400204D0 | verified | IDA |
| XCommon | tinyxml | TiXmlAttributeSet::Find | 0x140050C10 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::FindDefaultPhotoItemID | 0x1400745B0 | verified | IDA |

### Is 判断函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | UtilFunc.cpp | UtilFunc::IsUsableNameFilter | 0x140001DD0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::IsValidSecondPassword | 0x140004D80 | verified | IDA |
| XCore | XClient.h | XClient::IsBlock | 0x140012F30 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::IsServerAcceptClosed | 0x140015DF0 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsSendWaitPacket | 0x140029830 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsCancel_Wait | 0x1400298B0 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::IsReady | 0x140044010 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsLeagueMaster | 0x14002EDA0 | verified | IDA |
| XLoginServer | User.cpp | CUser::IsEmptySlot | 0x14002FAC0 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::IsConnection | 0x14003EE50 | verified | IDA |
| XCore | XClient.h | XClient::IsState | 0x1400402D0 | verified | IDA |
| XCommon | rapidjson | Double::IsNormal | 0x140027BA0 | verified | IDA |
| XCommon | rapidjson | BigInteger::IsZero | 0x140027CF0 | verified | IDA |
| XCommon | rapidjson | ParseResult::IsError | 0x140028FE0 | verified | IDA |

### On 回调函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::OnSend | 0x14003CE00 | verified | IDA |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::OnRecv | 0x14003CE30 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnRecv | 0x14003F500 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnDisConnect | 0x14003F330 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::OnDisConnect | 0x14000DAD0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnDisConnect | 0x140012E80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnConnect | 0x140012EC0 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnNotConnect | 0x140012F00 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnParse | 0x140012F80 | verified | IDA |
| XCore | TXDBSocket.h | TXDBSocket<CUser>::OnStartThread | 0x140013240 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::OnDisConnect | 0x140043870 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::OnParse | 0x1400438B0 | verified | IDA |
| XCore | XRelaySocket.cpp | XRelaySocket::OnStartThread | 0x140044030 | verified | IDA |
| XCore | XServer.h | XServer::OnLogOut | 0x14004BCD0 | verified | IDA |
| XCore | XServer.h | XServer::OnUpdate | 0x14004BF60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnAccect | 0x140017E60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::OnUpdate | 0x140017F00 | verified | IDA |
| XLoginServer | ObserveSocket.cpp | CObserveSocket::OnUpdate | 0x14008D5B0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::OnInitializeThread | 0x14004C360 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::OnFinalizeThread | 0x14004C3F0 | verified | IDA |

### Dtor 析构辅助函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | DBLoadTable.h | XResourceMgr::InitDayEvent_::_1_::dtor$0 | 0x140176570 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::InitDayEvent_::_1_::dtor$1 | 0x140176580 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::SetStatusTable_::_1_::dtor$0 | 0x1401765A0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::SetStatusTable_::_1_::dtor$1 | 0x1401765B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::SetStatusTable_::_1_::dtor$10 | 0x1401765C0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::SetStatusTable_::_1_::dtor$14 | 0x1401765D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::SetStatusTable_::_1_::dtor$16 | 0x1401765E0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::AddChannelDistrict6Info_::_1_::dtor$0 | 0x1401765F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::AddChannelDistrict6Info_::_1_::dtor$1 | 0x140176600 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$0 | 0x140176610 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$1 | 0x140176620 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$2 | 0x140176630 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$3 | 0x140176640 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$4 | 0x140176650 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$5 | 0x140176670 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$6 | 0x140176690 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$7 | 0x1401766B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$8 | 0x1401766D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$9 | 0x1401766F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$10 | 0x140176710 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$11 | 0x140176730 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$12 | 0x140176750 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$13 | 0x140176770 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$14 | 0x140176790 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$15 | 0x1401767B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$16 | 0x1401767D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$17 | 0x1401767F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$18 | 0x140176810 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$19 | 0x140176830 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$20 | 0x140176850 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$21 | 0x140176870 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$22 | 0x140176890 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$23 | 0x1401768B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$24 | 0x1401768D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$25 | 0x1401768F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$26 | 0x140176910 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$27 | 0x140176930 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$28 | 0x140176950 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$29 | 0x140176970 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$30 | 0x140176990 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$31 | 0x1401769B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$32 | 0x1401769D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$33 | 0x1401769F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$34 | 0x140176A10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$35 | 0x140176A30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$36 | 0x140176A50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$37 | 0x140176A70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$38 | 0x140176A90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$39 | 0x140176AB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$40 | 0x140176AD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$41 | 0x140176AF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$42 | 0x140176B10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$43 | 0x140176B30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$44 | 0x140176B50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$45 | 0x140176B70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$46 | 0x140176B90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$47 | 0x140176BB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$48 | 0x140176BD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$49 | 0x140176BF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$50 | 0x140176C10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$51 | 0x140176C30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$52 | 0x140176C50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$53 | 0x140176C70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$54 | 0x140176C90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$55 | 0x140176CB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$56 | 0x140176CD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$57 | 0x140176CF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$58 | 0x140176D10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$59 | 0x140176D30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$60 | 0x140176D50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$61 | 0x140176D70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$62 | 0x140176D90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$63 | 0x140176DB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$64 | 0x140176DD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$65 | 0x140176DF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$66 | 0x140176E10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$67 | 0x140176E30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$68 | 0x140176E50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$69 | 0x140176E70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$70 | 0x140176E90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$71 | 0x140176EB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$72 | 0x140176ED0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$73 | 0x140176EF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$74 | 0x140176F10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$75 | 0x140176F30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$76 | 0x140176F50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$77 | 0x140176F70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$78 | 0x140176F90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$79 | 0x140176FB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$80 | 0x140176FD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$81 | 0x140176FF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$82 | 0x140177010 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$83 | 0x140177030 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$84 | 0x140177050 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$85 | 0x140177070 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$86 | 0x140177090 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$87 | 0x1401770B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$88 | 0x1401770D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$89 | 0x1401770F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$90 | 0x140177110 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$91 | 0x140177130 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$92 | 0x140177150 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$93 | 0x140177170 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$94 | 0x140177190 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$95 | 0x1401771B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$96 | 0x1401771D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$97 | 0x1401771F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$98 | 0x140177210 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$99 | 0x140177230 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$100 | 0x140177250 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$101 | 0x140177270 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$102 | 0x140177290 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$103 | 0x1401772B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$104 | 0x1401772D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$105 | 0x1401772F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$106 | 0x140177310 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$107 | 0x140177330 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$108 | 0x140177350 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$109 | 0x140177370 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$110 | 0x140177390 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$111 | 0x1401773B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$112 | 0x1401773D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$113 | 0x1401773F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$114 | 0x140177410 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$115 | 0x140177430 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$116 | 0x140177450 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$117 | 0x140177470 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$118 | 0x140177490 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$119 | 0x1401774B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$120 | 0x1401774D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$121 | 0x1401774F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$122 | 0x140177510 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$123 | 0x140177530 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$124 | 0x140177550 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$125 | 0x140177570 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$126 | 0x140177590 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$127 | 0x1401775B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$128 | 0x1401775D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$129 | 0x1401775F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$130 | 0x140177610 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$131 | 0x140177630 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$132 | 0x140177650 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$133 | 0x140177670 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$134 | 0x140177690 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$135 | 0x1401766B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$136 | 0x1401766D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$137 | 0x1401766F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$138 | 0x140176710 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$139 | 0x140176730 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$140 | 0x140176750 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$141 | 0x140176770 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$142 | 0x140176790 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$143 | 0x1401767B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$144 | 0x1401767D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$145 | 0x1401767F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$146 | 0x140176810 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$147 | 0x140176830 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$148 | 0x140176850 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$149 | 0x140176870 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$150 | 0x140176890 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$151 | 0x1401768B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$152 | 0x1401768D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$153 | 0x1401768F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$154 | 0x140176910 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$155 | 0x140176930 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$156 | 0x140176950 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$157 | 0x140176970 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$158 | 0x140176990 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$159 | 0x1401769B0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$160 | 0x1401769D0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$161 | 0x1401769F0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$162 | 0x140176A10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$163 | 0x140176A30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$164 | 0x140176A50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$165 | 0x140176A70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$166 | 0x140176A90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$167 | 0x140176AB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$168 | 0x140176AD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$169 | 0x140176AF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$170 | 0x140176B10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$171 | 0x140176B30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$172 | 0x140176B50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$173 | 0x140176B70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$174 | 0x140176B90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$175 | 0x140176BB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$176 | 0x140176BD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$177 | 0x140176BF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$178 | 0x140176C10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$179 | 0x140176C30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$180 | 0x140176C50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$181 | 0x140176C70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$182 | 0x140176C90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$183 | 0x140176CB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$184 | 0x140176CD0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$185 | 0x140176CF0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$186 | 0x140176D10 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$187 | 0x140176D30 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$188 | 0x140176D50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::_XResourceMgr_::_1_::dtor$189 | 0x140176D70 | verified | IDA |

### XResourceMgr 构造析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$0 | 0x140178A40 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$1 | 0x140178A50 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$2 | 0x140178A60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$3 | 0x140178A70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$4 | 0x140178A80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$5 | 0x140178AA0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$6 | 0x140178AC0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$7 | 0x140178AE0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$8 | 0x140178B00 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$9 | 0x140178B20 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$10 | 0x140178B40 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$11 | 0x140178B60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$12 | 0x140178B80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$13 | 0x140178BA0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$14 | 0x140178BC0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$15 | 0x140178BE0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$16 | 0x140178C00 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$17 | 0x140178C20 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$18 | 0x140178C40 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$19 | 0x140178C60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$20 | 0x140178C80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$21 | 0x140178CA0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$22 | 0x140178CC0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$23 | 0x140178CE0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$24 | 0x140178D00 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$25 | 0x140178D20 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$26 | 0x140178D40 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$27 | 0x140178D60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$28 | 0x140178D80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$29 | 0x140178DA0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$30 | 0x140178DC0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$31 | 0x140178DE0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$32 | 0x140178E00 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$33 | 0x140178E20 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$34 | 0x140178E40 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$35 | 0x140178E60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$36 | 0x140178E80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$37 | 0x140178EA0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$38 | 0x140178EC0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$39 | 0x140178EE0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$40 | 0x140178F00 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$41 | 0x140178F20 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$42 | 0x140178F40 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$43 | 0x140178F60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$44 | 0x140178F80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$45 | 0x140178FA0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$46 | 0x140178FC0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$47 | 0x140178FE0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$48 | 0x140179000 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$49 | 0x140179020 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$50 | 0x140179040 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$51 | 0x140179060 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$52 | 0x140179080 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$53 | 0x1401790A0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$54 | 0x1401790C0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$55 | 0x1401790E0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$56 | 0x140179100 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$57 | 0x140179120 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$58 | 0x140179140 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$59 | 0x140179160 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$60 | 0x140179180 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$61 | 0x1401791A0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$62 | 0x1401791C0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$63 | 0x1401791E0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$64 | 0x140179200 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$65 | 0x140179220 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$66 | 0x140179240 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::XResourceMgr_::_1_::dtor$67 | 0x140179260 | verified | IDA |

### CashShop 相关析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | DBLoadTable.h | XResourceMgr::CashShopTabLoad_::_1_::dtor$0 | 0x14017AA60 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::CashShopTabLoad_::_1_::dtor$1 | 0x14017AA70 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::CashShopTabLoad_::_1_::dtor$2 | 0x14017AA80 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::CashShopTabLoad_::_1_::dtor$3 | 0x14017AA90 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::CashShopTabLoad_::_1_::dtor$5 | 0x14017AAB0 | verified | IDA |
| XLoginServer | DBLoadTable.h | XResourceMgr::CashShopTabLoad_::_1_::dtor$7 | 0x14017AAD0 | verified | IDA |

### XItemFactory 析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | ItemFactory.h | XItemFactory::ReSetOption_::_1_::dtor$0 | 0x14017AAE0 | verified | IDA |
| XLoginServer | ItemFactory.h | XItemFactory::_XItemFactory_::_1_::dtor$0 | 0x14017AAF0 | verified | IDA |
| XLoginServer | ItemFactory.h | XItemFactory::_XItemFactory_::_1_::dtor$1 | 0x14017AB00 | verified | IDA |
| XLoginServer | ItemFactory.h | XItemFactory::_XItemFactory_::_1_::dtor$4 | 0x14017AB10 | verified | IDA |
| XLoginServer | ItemFactory.h | XItemFactory::XItemFactory_::_1_::dtor$0 | 0x14017AB20 | verified | IDA |
| XLoginServer | ItemFactory.h | XItemFactory::XItemFactory_::_1_::dtor$1 | 0x14017AB30 | verified | IDA |
| XLoginServer | ItemFactory.h | XItemFactory::XItemFactory_::_1_::dtor$2 | 0x14017AB40 | verified | IDA |

### Catch 异常处理函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std::map | std::_Tree_val<...>::_Buynode<...>::_1_::catch$0 | 0x14017ABC0 | verified | IDA |
| XCore | std::map | std::_Tree_val<...TB_ACHIEVEMENT...>::_Buynode<...>::_1_::catch$0 | 0x14017ABE0 | verified | IDA |
| XCore | std::map | std::_Tree_val<...TB_AKASHIC_COMBINATION...>::_Buynode<...>::_1_::catch$0 | 0x14017AC00 | verified | IDA |
| XCore | std::map | std::_Tree_val<...TB_AKASHIC_MAKE...>::_Buynode<...>::_1_::catch$0 | 0x14017AC20 | verified | IDA |
| XCore | std::vector | std::_Uninit_move<...PS_DEFAULT_INVEN_ITEM...>::_1_::catch$0 | 0x14017D530 | verified | IDA |
| XCore | std::vector | std::vector<PS_DEFAULT_INVEN_ITEM>::reserve_::_1_::catch$0 | 0x14017D580 | verified | IDA |
| XCore | std::vector | std::_Uninit_move<...CHANNEL_INFO...>::_1_::catch$0 | 0x14017D5F0 | verified | IDA |
| XCore | std::vector | std::_Uninit_move<...ST_BANNER_INFO...>::_1_::catch$0 | 0x14017D650 | verified | IDA |
| XCore | std::vector | std::_Uninit_move<...STGMCashItem...>::_1_::catch$0 | 0x14017D6A0 | verified | IDA |
| XCore | std::vector | std::vector<ST_BANNER_INFO>::reserve_::_1_::catch$0 | 0x14017D6F0 | verified | IDA |
| XCore | std::vector | std::vector<STGMCashItem>::reserve_::_1_::catch$0 | 0x14017D730 | verified | IDA |
| XCore | std::vector | std::vector<unsigned short>::reserve_::_1_::catch$0 | 0x14017D770 | verified | IDA |
| XCore | std::vector | std::_Uninit_move<...PS_BROACH_SHAPE...>::_1_::catch$0 | 0x14017D7B0 | verified | IDA |
| XCore | std::vector | std::vector<PS_BROACH_SHAPE>::reserve_::_1_::catch$0 | 0x14017D800 | verified | IDA |
| XCore | std::vector | std::vector<int>::reserve_::_1_::catch$0 | 0x14017D840 | verified | IDA |

### rapidjson 解析函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | rapidjson | rapidjson::GenericMember<...>::_GenericMember<...>::_1_::dtor$0 | 0x14017D4D0 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericDocument<...>::GenericDocument<...>::_1_::dtor$0 | 0x14017D4F0 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericDocument<...>::GenericDocument<...>::_1_::dtor$1 | 0x14017D510 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericDocument<...>::RawNumber_::_1_::dtor$0 | 0x14017D880 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericReader<...>::ParseString_0<...>::_1_::dtor$0 | 0x14017E510 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericReader<...>::ParseNumber_0<...>::_1_::dtor$0 | 0x14017E610 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericReader<...>::ParseNumber_0<...>::_1_::dtor$1 | 0x14017E630 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericDocument<...>::ParseStream_0<...>::_1_::dtor$0 | 0x14017E940 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericDocument<...>::ParseStream_0<...>::_1_::dtor$1 | 0x14017E960 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericReader<...>::IterativeParse_0<...>::_1_::dtor$0 | 0x14017E900 | verified | IDA |
| XCommon | rapidjson | rapidjson::GenericReader<...>::Parse_0<...>::_1_::dtor$0 | 0x14017E920 | verified | IDA |

### Concurrency 并发队列函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | Concurrency | Concurrency::concurrent_queue<...>::_Assign_and_destroy_item_::_1_::dtor$0 | 0x14017D5C0 | verified | IDA |
| XCore | Concurrency | Concurrency::concurrent_queue<...>::_Copy_item_::_1_::dtor$0 | 0x14017D5D0 | verified | IDA |
| XCore | Concurrency | Concurrency::concurrent_queue<...>::_scalar_deleting_destructor__::_1_::dtor$0 | 0x14017D640 | verified | IDA |

### boost::multi_index 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | boost | boost::multi_index::detail::bucket_array<...>::bucket_array<...>::_1_::dtor$0 | 0x14017E530 | verified | IDA |
| XCore | boost | boost::multi_index::multi_index_container<...>::multi_index_container<...>::_1_::dtor$0 | 0x14017E550 | verified | IDA |
| XCore | boost | boost::multi_index::detail::hashed_index<...>::unchecked_rehash_::_1_::dtor$0 | 0x14017E650 | verified | IDA |
| XCore | boost | boost::multi_index::detail::hashed_index<...>::unchecked_rehash_::_1_::dtor$1 | 0x14017E670 | verified | IDA |
| XCore | boost | boost::multi_index::multi_index_container<...>::insert__::_1_::catch$0 | 0x14017E690 | verified | IDA |

### std::basic_stringstream 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std | std::basic_ostream<char>::sentry::_sentry_::_1_::dtor$0 | 0x14017D610 | verified | IDA |
| XCore | std | std::basic_stringstream<char>::_vbase_destructor__::_1_::dtor$0 | 0x14017D620 | verified | IDA |
| XCore | std | std::basic_stringstream<char>::basic_stringstream<...>::_1_::dtor$0 | 0x14017D9C0 | verified | IDA |
| XCore | std | std::basic_stringstream<char>::basic_stringstream<...>::_1_::dtor$1 | 0x14017D9F0 | verified | IDA |
| XCore | std | std::basic_stringstream<char>::basic_stringstream<...>::_1_::dtor$3 | 0x14017DA10 | verified | IDA |
| XCore | std | std::basic_stringbuf<char>::str_::_1_::dtor$0 | 0x14017EE40 | verified | IDA |
| XCore | std | std::basic_stringbuf<char>::str_::_1_::dtor$2 | 0x14017EE60 | verified | IDA |
| XCore | std | std::basic_stringbuf<char>::str_::_1_::dtor$3 | 0x14017EE70 | verified | IDA |
| XCore | std | std::basic_stringbuf<char>::str_::_1_::dtor$4 | 0x14017EE30 | verified | IDA |
| XCore | std | std::basic_stringbuf<char>::str_::_1_::dtor$5 | 0x14017EE50 | verified | IDA |

### std::deque 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std | std::_Deque_val<IXObject*>::_Deque_val<...>::_1_::dtor$0 | 0x14017D8A0 | verified | IDA |
| XCore | std | std::_Deque_val<IXObject*>::__Deque_val<...>::_1_::dtor$0 | 0x14017E1C0 | verified | IDA |

### ATL CAtlMap 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | ATL | ATL::CAtlMap<unsigned char,IXProcess*>::InitHashTable_::_1_::catch$0 | 0x14017DA40 | verified | IDA |
| XCore | ATL | ATL::CAtlMap<int,IXObject*>::InitHashTable_::_1_::catch$0 | 0x14017DB20 | verified | IDA |
| XCore | ATL | ATL::CAtlMap<int,IXObject*>::Rehash_::_1_::catch$0 | 0x14017DB40 | verified | IDA |
| XCore | ATL | ATL::CAtlMap<int,IXObject*>::NewNode_::_1_::catch$0 | 0x14017DB80 | verified | IDA |
| XCore | ATL | ATL::CAtlMap<unsigned char,IXProcess*>::Rehash_::_1_::catch$0 | 0x14017E000 | verified | IDA |
| XCore | ATL | ATL::CAtlMap<unsigned char,IXProcess*>::NewNode_::_1_::catch$0 | 0x14017E020 | verified | IDA |

### std::string 操作符函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | std | std::operator<<<char>::_1_::dtor$0 | 0x14017D960 | verified | IDA |
| XCore | std | std::operator<<<char>::_1_::dtor$1 | 0x14017D950 | verified | IDA |
| XCore | std | std::operator<<<char>::_1_::dtor$2 | 0x14017D9B0 | verified | IDA |
| XCore | std | std::operator<<<char>::_1_::catch$0 | 0x14017D970 | verified | IDA |
| XCore | std | std::operator<<<char>...::_1_::dtor$0 | 0x14017EEE0 | verified | IDA |

### XClient 类析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XClient.h | XClient::_XClient_::_1_::dtor$0 | 0x14017E3B0 | verified | IDA |
| XCore | XClient.h | XClient::_XClient_::_1_::dtor$1 | 0x14017E3C0 | verified | IDA |
| XCore | XClient.h | XClient::_XClient_::_1_::dtor$2 | 0x14017E3E0 | verified | IDA |
| XCore | XClient.h | XClient::_XClient_::_1_::dtor$3 | 0x14017E400 | verified | IDA |
| XCore | XClient.h | XClient::_XClient_::_1_::dtor$4 | 0x14017E420 | verified | IDA |
| XCore | XClient.h | XClient::XClient_::_1_::dtor$0 | 0x14017E430 | verified | IDA |
| XCore | XClient.h | XClient::XClient_::_1_::dtor$1 | 0x14017E440 | verified | IDA |
| XCore | XClient.h | XClient::XClient_::_1_::dtor$2 | 0x14017E460 | verified | IDA |

### XOption 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | Option.h | XOption::_XOption_::_1_::dtor$0 | 0x14017E2F0 | verified | IDA |
| XCore | Option.h | XOption::_XOption_::_1_::dtor$1 | 0x14017E310 | verified | IDA |
| XCore | Option.h | XOption::_XOption_::_1_::dtor$2 | 0x14017E350 | verified | IDA |
| XCore | Option.h | XOption::_XOption_::_1_::dtor$3 | 0x14017E370 | verified | IDA |
| XCore | Option.h | XOption::XOption_::_1_::dtor$0 | 0x14017DF10 | verified | IDA |
| XCore | Option.h | XOption::XOption_::_1_::dtor$1 | 0x14017DF30 | verified | IDA |
| XCore | Option.h | XOption::XOption_::_1_::dtor$2 | 0x14017DF61 | verified | IDA |
| XCore | Option.h | XOption::XOption_::_1_::dtor$3 | 0x14017DF83 | verified | IDA |
| XCore | Option.h | XOption::GetServerPrivateIPAndPort_::_1_::dtor$0 | 0x14017ECE0 | verified | IDA |
| XCore | Option.h | XOption::Load_::_1_::dtor$0 | 0x14017ECF0 | verified | IDA |
| XCore | Option.h | XOption::Load_::_1_::dtor$1 | 0x14017ED00 | verified | IDA |
| XCore | Option.h | XOption::Load_::_1_::dtor$2 | 0x14017ED10 | verified | IDA |
| XCore | Option.h | XOption::Load_::_1_::dtor$3 | 0x14017ED20 | verified | IDA |

### CUser 类析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | User.cpp | CUser::_CUser_::_1_::dtor$0 | 0x140180040 | verified | IDA |
| XLoginServer | User.cpp | CUser::_CUser_::_1_::dtor$1 | 0x140180060 | verified | IDA |
| XLoginServer | User.cpp | CUser::_CUser_::_1_::dtor$2 | 0x140180080 | verified | IDA |
| XLoginServer | User.cpp | CUser::_CUser_::_1_::dtor$3 | 0x1401800A0 | verified | IDA |
| XLoginServer | User.cpp | CUser::_CUser_::_1_::dtor$4 | 0x1401800C0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CUser_::_1_::dtor$0 | 0x1401800E0 | verified | IDA |
| XLoginServer | User.cpp | CUser::CUser_::_1_::dtor$1 | 0x140180100 | verified | IDA |
| XLoginServer | User.cpp | CUser::CUser_::_1_::dtor$2 | 0x140180120 | verified | IDA |
| XLoginServer | User.cpp | CUser::CUser_::_1_::dtor$3 | 0x140180140 | verified | IDA |
| XLoginServer | User.cpp | CUser::CUser_::_1_::dtor$4 | 0x140180160 | verified | IDA |
| XLoginServer | User.cpp | CUser::CUser_::_1_::dtor$5 | 0x140180180 | verified | IDA |

### XLoginServer 类析构函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$0 | 0x14017EF20 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$1 | 0x14017EF40 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$2 | 0x14017EF60 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$3 | 0x14017EF80 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$4 | 0x14017EFA0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$5 | 0x14017EFC0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$6 | 0x14017EFE0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$7 | 0x14017F000 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$8 | 0x14017F020 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$9 | 0x14017F040 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$10 | 0x14017F060 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::_XLoginServer_::_1_::dtor$11 | 0x14017F080 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$0 | 0x14017F350 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$1 | 0x14017F370 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$2 | 0x14017F390 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$3 | 0x14017F3B0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$4 | 0x14017F3D0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$5 | 0x14017F3F0 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$6 | 0x14017F410 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$7 | 0x14017F430 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$8 | 0x14017F450 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$9 | 0x14017F470 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$10 | 0x14017F490 | verified | IDA |
| XLoginServer | LoginServer.cpp | XLoginServer::XLoginServer_::_1_::dtor$11 | 0x14017F4B0 | verified | IDA |

### CLogThreadManager 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | CLogThreadProc.cpp | CLogThreadManager::_scalar_deleting_destructor__::_1_::dtor$0 | 0x14017E580 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::_scalar_deleting_destructor__::_1_::dtor$1 | 0x14017E590 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::End_::_1_::dtor$0 | 0x14017DE60 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::End_::_1_::dtor$1 | 0x14017DE70 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::End_::_1_::dtor$3 | 0x14017DE80 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::DoJob_::_1_::dtor$0 | 0x14017D8D0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::DoJob_::_1_::dtor$2 | 0x14017D8E0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CreateWorkerThread_::_1_::dtor$0 | 0x14017E9E0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::GetLogger_::_1_::dtor$1 | 0x14017E9F0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$0 | 0x14017EA00 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$1 | 0x14017EA10 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$2 | 0x14017EA20 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$27 | 0x14017EA30 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$3 | 0x14017EA40 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$30 | 0x14017EA50 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$4 | 0x14017EA60 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$33 | 0x14017EA70 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$36 | 0x14017EA90 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$39 | 0x14017EAB0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$42 | 0x14017EAD0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$45 | 0x14017EAF0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$48 | 0x14017EB10 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$51 | 0x14017EB30 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$54 | 0x14017EB50 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$57 | 0x14017EB70 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$60 | 0x14017EB90 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$63 | 0x14017EBB0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$66 | 0x14017EBD0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$69 | 0x14017EBF0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$72 | 0x14017EC10 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$75 | 0x14017EC30 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$78 | 0x14017EC50 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$81 | 0x14017EC90 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$84 | 0x14017ECB0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$87 | 0x14017ECD0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadManager::CLogThreadManager_::_1_::dtor$90 | 0x14017ECF0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::_CLogThreadProc_::_1_::dtor$0 | 0x14017DE90 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::_CLogThreadProc_::_1_::dtor$2 | 0x14017DEA0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::CLogThreadProc_::_1_::dtor$0 | 0x14017DEB0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::OnInitializeThread_::_1_::dtor$0 | 0x14017F1B0 | verified | IDA |
| XCore | CLogThreadProc.cpp | CLogThreadProc::ThreadProc_::_1_::dtor$0 | 0x14017F1A0 | verified | IDA |

### CCurlWrapper 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendSOAPHttpsPost_::_1_::dtor$0 | 0x14017F1C0 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendSOAPHttpsPost_::_1_::dtor$1 | 0x14017F1D0 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpsPost_::_1_::dtor$0 | 0x14017F1E0 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpsPost_::_1_::dtor$1 | 0x14017F1F0 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpsPost_::_1_::dtor$2 | 0x14017F200 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpPost_::_1_::dtor$0 | 0x14017F220 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpPost_::_1_::dtor$1 | 0x14017F230 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpPost_::_1_::dtor$2 | 0x14017F240 | verified | IDA |
| XCommon | CurlWrapper.cpp | CCurlWrapper::SendHttpPost_::_1_::dtor$3 | 0x14017F250 | verified | IDA |

### XRelaySocket/XGameDBSocket 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XRelaySocket.cpp | XRelaySocket::_XRelaySocket_::_1_::dtor$0 | 0x14017ED90 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCheckName_::_1_::dtor$0 | 0x14017DD80 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterUpdateMap_::_1_::dtor$0 | 0x14017F0C0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer_::_1_::dtor$0 | 0x14017F0E0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterChangeServer_::_1_::dtor$1 | 0x14017F100 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResOptionLoad_::_1_::dtor$0 | 0x14017F940 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResLoginCharacterCount_::_1_::dtor$0 | 0x14017F960 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResSelectCharacter_::_1_::dtor$0 | 0x14017F980 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResSelectCharacter_::_1_::dtor$1 | 0x14017F9A0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResSelectCharacter_::_1_::dtor$2 | 0x14017F9C0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResSelectCharacter_::_1_::dtor$3 | 0x14017F9E0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResSelectCharacter_::_1_::dtor$5 | 0x14017FA00 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResSelectCharacter_::_1_::dtor$6 | 0x14017FA20 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterRepresentativeChange_::_1_::dtor$0 | 0x14017FA40 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResEnterServer_::_1_::dtor$0 | 0x14017FD80 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResEnterServer_::_1_::dtor$1 | 0x14017FDA0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterDelete_::_1_::dtor$0 | 0x14017FEC0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterDelete_::_1_::dtor$1 | 0x14017FEE0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCreate_::_1_::dtor$0 | 0x14017FF00 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCreate_::_1_::dtor$1 | 0x14017FF20 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCreate_::_1_::dtor$2 | 0x14017FF40 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCreate_::_1_::dtor$3 | 0x14017FF60 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCreate_::_1_::dtor$4 | 0x14017FF80 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterCreate_::_1_::dtor$5 | 0x14017FFA0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterList_::_1_::dtor$0 | 0x14017FFE0 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterList_::_1_::dtor$2 | 0x140180000 | verified | IDA |
| XCore | XGameDBSocket.cpp | XGameDBSocket::ResCharacterList_::_1_::dtor$3 | 0x140180020 | verified | IDA |

### XLoginProcess 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterWaitCheck_::_1_::dtor$0 | 0x14017F7A0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqOptionUpdate_::_1_::dtor$0 | 0x14017F7C0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer_::_1_::dtor$0 | 0x14017F7E0 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::ReqEnterServer_::_1_::dtor$1 | 0x14017F800 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList_::_1_::dtor$0 | 0x14017F820 | verified | IDA |
| XLoginServer | LoginProcess.cpp | XLoginProcess::SendServerList_::_1_::dtor$1 | 0x14017F840 | verified | IDA |

### CCharacterProcess 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeChange_::_1_::dtor$0 | 0x14017FA60 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck_::_1_::dtor$0 | 0x14017FA80 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterRepresentativeCheck_::_1_::dtor$1 | 0x14017FAA0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$0 | 0x14017FAC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$1 | 0x14017FAE0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$2 | 0x14017FB00 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$3 | 0x14017FB20 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$4 | 0x14017FB40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$5 | 0x14017FB60 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSecondPassword_::_1_::dtor$6 | 0x14017FB80 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCheckName_::_1_::dtor$0 | 0x14017FBC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter_::_1_::dtor$0 | 0x14017FBE0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter_::_1_::dtor$1 | 0x14017FC00 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter_::_1_::dtor$2 | 0x14017FC20 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter_::_1_::dtor$3 | 0x14017FC40 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter_::_1_::dtor$4 | 0x14017FC60 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqSelectCharacter_::_1_::dtor$5 | 0x14017FC80 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete_::_1_::dtor$0 | 0x14017FCA0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete_::_1_::dtor$1 | 0x14017FCC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterDelete_::_1_::dtor$2 | 0x14017FCE0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate_::_1_::dtor$0 | 0x14017FDC0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate_::_1_::dtor$1 | 0x14017FDE0 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate_::_1_::dtor$2 | 0x14017FE00 | verified | IDA |
| XLoginServer | CharacterProcess.cpp | CCharacterProcess::ReqCharacterCreate_::_1_::dtor$3 | 0x14017FE20 | verified | IDA |

### CLoginControlSocket 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvUserChangeServer_::_1_::dtor$0 | 0x14017F4F0 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer_::_1_::dtor$0 | 0x14017F6E0 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer_::_1_::dtor$1 | 0x14017F700 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer_::_1_::dtor$2 | 0x14017F720 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvEnterServer_::_1_::dtor$3 | 0x14017F740 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes_::_1_::dtor$0 | 0x14017F760 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvCreateMazeRes_::_1_::dtor$1 | 0x14017F780 | verified | IDA |
| XCore | LoginControlSocket.cpp | CLoginControlSocket::RecvCheckSessionID_::_1_::dtor$0 | 0x14017F900 | verified | IDA |

### XGameDBSocketMgr 类函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XGameDBSocketMgr.cpp | XGameDBSocketMgr::Init_::_1_::dtor$0 | 0x14017F860 | verified | IDA |
| XCore | XGameDBSocketMgr.cpp | XGameDBSocketMgr::Init_::_1_::dtor$1 | 0x14017F880 | verified | IDA |
| XCore | XGameDBSocketMgr.cpp | XGameDBSocketMgr::Init_::_1_::dtor$2 | 0x14017F8A0 | verified | IDA |
| XCore | XGameDBSocketMgr.cpp | XGameDBSocketMgr::Init_::_1_::dtor$3 | 0x14017F8C0 | verified | IDA |
| XCore | XGameDBSocketMgr.cpp | XGameDBSocketMgr::Init_::_1_::dtor$4 | 0x14017F8E0 | verified | IDA |

### 其他析构辅助函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XIOCPClient.h | XIOCPClient::OnRecv_::_1_::dtor$0 | 0x14017E5F0 | verified | IDA |
| XCore | XIOCPClient.h | XIOCPClient::OnRecv_::_1_::dtor$1 | 0x14017E600 | verified | IDA |
| XCore | XIOCPSkeleton.cpp | XIOCPSkeleton::_XIOCPSkeleton_::_1_::dtor$0 | 0x14017E480 | verified | IDA |
| XCore | XTCPSkeleton.cpp | XTCPSkeleton::OnRecv_::_1_::dtor$0 | 0x14017E490 | verified | IDA |
| XCore | XIOCPServer.cpp | XIOCPServer::XAccept_::_1_::dtor$0 | 0x14017E5D0 | verified | IDA |
| XCore | XIOCPServer.cpp | XIOCPServer::XIOCPServer_::_1_::dtor$0 | 0x14017E5E0 | verified | IDA |
| XCommon | UtilFunc.cpp | UtilFunc::IsUsableNameFilter_::_1_::dtor$0 | 0x14017FD00 | verified | IDA |
| XCommon | UtilFunc.cpp | UtilFunc::IsUsableNameFilter_::_1_::dtor$1 | 0x14017FD20 | verified | IDA |
| XCore | XServer.h | XServer::XServer_::_1_::dtor$0 | 0x14017E840 | verified | IDA |
| XCore | XSendDBPacket.cpp | XSendDBPacket::XSendDBPacket_::_1_::dtor$0 | 0x14017DC80 | verified | IDA |
| XCore | XClientPool.cpp | XClientPool::AllocClient_::_1_::dtor$0 | 0x14017DFE0 | verified | IDA |
| XCore | XClientPool.cpp | XClientPool::AllocClient_::_1_::dtor$1 | 0x14017DFF0 | verified | IDA |
| XCore | XClientPool.cpp | XClientPool::GetHead_::_1_::dtor$0 | 0x14017DA20 | verified | IDA |
| XCore | XClientPool.cpp | XClientPool::FreeClient_::_1_::dtor$0 | 0x14017DA30 | verified | IDA |
| XCore | cIoContextPool.cpp | cIoContextPool::ReleaseIoContext_::_1_::dtor$0 | 0x14017D8C0 | verified | IDA |
| XCommon | ModulePath.cpp | GetModuleFilePath_::_1_::dtor$0 | 0x14017AB60 | verified | IDA |
| XCore | IXComponent.cpp | IXComponent::GetName_::_1_::dtor$1 | 0x14017E5C0 | verified | IDA |
| XCore | IXComponent.cpp | IXComponent::SetName_::_1_::dtor$0 | 0x14017EE10 | verified | IDA |

### 动态初始化器函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath__ | 0x1401801A0 | verified | IDA |
| XCommon | boost | boost::tuples::_dynamic_initializer_for__ignore__ | 0x1401801D0 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___1__ | 0x1401801F0 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___2__ | 0x140180210 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___3__ | 0x140180230 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___4__ | 0x140180250 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___5__ | 0x140180270 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___6__ | 0x140180290 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___7__ | 0x1401802B0 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___8__ | 0x1401802D0 | verified | IDA |
| XCommon | anon | _anonymous_namespace_::_dynamic_initializer_for___9__ | 0x1401802F0 | verified | IDA |
| XCommon | Random | _dynamic_initializer_for__RNMX__ | 0x140180310 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___0 | 0x140180330 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___1 | 0x140180360 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___2 | 0x140180390 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___3 | 0x1401803C0 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___4 | 0x1401803F0 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___5 | 0x140180420 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___6 | 0x140180450 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__hkvNoInitialization__ | 0x140180480 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_WHITE__ | 0x1401804A0 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_GREY__ | 0x1401804D0 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_BLACK__ | 0x140180500 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_RED__ | 0x140180530 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_YELLOW__ | 0x140180560 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_GREEN__ | 0x140180590 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_CYAN__ | 0x1401805C0 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_BLUE__ | 0x1401805F0 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__V_RGBA_PURPLE__ | 0x140180620 | verified | IDA |
| XCommon | Vision | _dynamic_initializer_for__hkvNoInitialization___0 | 0x140180650 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___7 | 0x140180670 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___8 | 0x1401806A0 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___9 | 0x1401806D0 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___10 | 0x1401806F0 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___11 | 0x140180710 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___12 | 0x140180730 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___13 | 0x140180750 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___14 | 0x140180770 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___15 | 0x140180790 | verified | IDA |
| XLoginServer | Global | _dynamic_initializer_for__g_strCurPath___16 | 0x1401807B0 | verified | IDA |
| XCore | ATL | ATL::_dynamic_initializer_for___AtlBaseModule__ | 0x1401807D0 | verified | IDA |
| XCore | ATL | ATL::_dynamic_initializer_for__g_strheap__ | 0x1401807F0 | verified | IDA |
| XCore | ATL | ATL::_dynamic_initializer_for__g_strmgr__ | 0x140180828 | verified | IDA |

### 动态析构器函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath__ | 0x140180840 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___0 | 0x140180860 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___1 | 0x140180880 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___2 | 0x1401808A0 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___3 | 0x1401808C0 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___4 | 0x1401808E0 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___5 | 0x140180900 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___6 | 0x140180920 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___7 | 0x140180940 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___8 | 0x140180960 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___9 | 0x140180980 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___10 | 0x140180990 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___11 | 0x1401809A0 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___12 | 0x1401809B0 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___13 | 0x1401809F0 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___14 | 0x140180A30 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___15 | 0x140180A70 | verified | IDA |
| XLoginServer | Global | _dynamic_atexit_destructor_for__g_strCurPath___16 | 0x140180AB0 | verified | IDA |
| XCore | ATL | ATL::_dynamic_atexit_destructor_for___AtlBaseModule__ | 0x140180AEC | verified | IDA |
| XCore | ATL | ATL::_dynamic_atexit_destructor_for__g_strheap__ | 0x140180AF8 | verified | IDA |
| XCore | ATL | ATL::_dynamic_atexit_destructor_for__g_strmgr__ | 0x140180B2C | verified | IDA |

## 状态说明

- `pending`: 待恢复
- `decompiled`: 已反编译，待验证
- `asm_restored`: 汇编还原完成
- `verified`: 已验证
- `blocked`: 阻塞中
