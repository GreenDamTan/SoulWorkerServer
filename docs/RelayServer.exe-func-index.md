# RelayServer.exe 函数索引

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 | 是否验证 | 验证结论 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Common/XNet/XCommon | PSServer.h | PS_REQ_LEAGUE_INVEN_INFO::operator<<(XPacket&, ...) | 0x1400E82D0 | verified | IDA 0x1400E82D0 + nLeagueID+dwNpcID+shStartPos+shEndPos; 源码匹配 | 否 | 一致 |
| Common/XNet/XCommon | PSServer.h | PS_ITEM_BROACH_LIST operator>> | 0x1400EAED0 | verified | IDA 0x1400EAED0 + uint16 count+循环读ST_ITEM_BROACH+push_back; 源码匹配 | 否 | 一致 |
| Common/XNet/XCommon | PSServer.h | PS_ITEM_PACKAGE_LIST operator>> | 0x1400EB7A0 | verified | IDA 0x1400EB7A0 + int32 nCount+循环读PS_ITEM_PACKAGE+push_back; 修复:count类型从uint16改为int32对齐IDA | 否 | 一致 |
| GameServer/XRelayServer | Community.cpp | CCommunity::CCommunity | 0x140000000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::~CCommunity | 0x140000208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::Clear | 0x140000272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsValidCommunityType | 0x140000400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsValiedListCount | 0x140000448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsFriend | 0x140000624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsFriend | 0x140000992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsBlockList | 0x140001536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsBlockList | 0x140001808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::AddFriend | 0x140002256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::AddBlock | 0x140002544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetFriendUCID | 0x140002784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetFriendList | 0x140003216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetFriendList | 0x140003456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetBlcokList | 0x140003712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetFriendType | 0x140003888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::IsChangeFriendInfo | 0x140004048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetFriendInfo | 0x140004528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::UpdateFriendInfo | 0x140004752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::DeleteFriend | 0x140005328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::DeleteBlockList | 0x140005488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::CheckFriendInvite | 0x140005648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::CheckFriendAccept | 0x140005856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::CheckBlockAdd | 0x140006064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::GetLastFriendWaitList | 0x140006288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::CheckRecruitListTime | 0x140006688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::InitRecruitListTime | 0x140006800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CCommunity::AddFriendPoint | 0x140006848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CUserObject::~CUserObject | 0x140035136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | STCharInfo::~STCharInfo | 0x140042256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | wmemcmp | 0x140043552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | wcscpy_s<31> | 0x140044304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CBlockUser::GetName | 0x140044496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CFriendMember::GetName | 0x140044576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CFriendMember::GetType | 0x140044640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | ST_FRIEND_COMMUNITY::ST_FRIEND_COMMUNITY | 0x140044688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | wmemmove | 0x140045040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | wmemcpy | 0x140047056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CBlockUser::GetUCID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Community.cpp | CFriendMember::GetUCID | 0x140296208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::CExchangePriceMgr | 0x140047648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::~CExchangePriceMgr | 0x140047776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::GetPriceList | 0x140047856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::LoadPriceList | 0x140048400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::SetPriceInfo | 0x140048576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::AddPriceList | 0x140048784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::ST_EXCHANGE_PRICE_HISTORY_INFO::~ST_EXCHANGE_PRICE_HISTORY_INFO | 0x140050000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::DeletePriceList_Old | 0x140050096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::GetPriceListCount | 0x140050400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::GetDBRequestDate | 0x140050704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::ST_EXCHANGE_PRICE_HISTORY_INFO::ST_EXCHANGE_PRICE_HISTORY_INFO | 0x140063024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CExchangePriceMgr::ST_EXCHANGE_PRICE_HISTORY_INFO::ST_EXCHANGE_PRICE_HISTORY_INFO | 0x140073200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFSRWLock::unlock | 0x140073344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFAutoSlimWriteLock::~CFAutoSlimWriteLock | 0x140073376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFSRWLock::lock | 0x140073408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFAutoSlimWriteLock::CFAutoSlimWriteLock | 0x140073440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFSRWLock::unlock_shared | 0x140073504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFAutoSlimReadLock::~CFAutoSlimReadLock | 0x140073536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFSRWLock::lock_shared | 0x140073568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFAutoSlimReadLock::CFAutoSlimReadLock | 0x140073600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ExchangePriceMgr.cpp | CFSRWLock::CFSRWLock | 0x140073664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.cpp | CForce::AddMember | - | verified | RelayServer decompile force-member add path + `m_mapForceMember` insertion | 否 | 一致 |
| GameServer/XRelayServer | Force.cpp | CForce::SetMemberEnterMap(uint32, UXMapID) | 0x140094650 | verified | export-for-ai 0x140094650 + find + SetEnterMap | 否 | 一致 |
| GameServer/XRelayServer | Force.h | UXMapID::UXMapID | 0x140020176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | UXMapID::operator= | 0x140020176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::CForce | 0x140073696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::CForce | 0x140074416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::~CForce | 0x140074480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::SetForceInfo | 0x140074528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::SetMemberInfo | 0x140075168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::SetMemberInfo | 0x140075424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::AddMember | 0x140075824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::GetForceMemberList | 0x140076112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::SendNameChange | 0x140076704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | TXSingleton<XRelayServer>::Instance | 0x140077296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | wcscpy_s<21> | 0x140078224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CUserObject::GetServerID | 0x140079136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::GetMemberID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | PS_CHANGE_NAME::PS_CHANGE_NAME | 0x140079184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | ST_FORCE_MEMBER::ST_FORCE_MEMBER | 0x140079232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | ST_PARTY_MEMBER::ST_PARTY_MEMBER | 0x140079232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::SetMemberInfo | 0x140079424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | ST_FORCE_MEMBER::operator= | 0x140079472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | UXMapID::operator= | 0x140079744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::Logout | 0x140079776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | XParse::operator<< | 0x140110112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | XParse::SetDWORD | 0x140111376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::GetMemberInfo | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | UXMapID::UXMapID | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::Clear | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::~CForceMember | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::ChangeMaster | 0x140602800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::Kickout | 0x140602976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::FindNewMaster | 0x140603104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::GetForceInfo | 0x140603296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::GetMemberInfo | 0x140603552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForce::SetMemberEnterMap | 0x140603728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::SetEnterMap | 0x140607280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::Login | 0x140607328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Force.h | CForceMember::CForceMember | 0x140607360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqInviteForce | 0x140014D30 | verified | IDA 0x140014D30 + GetUser(strName)→null(53111)/GetPartyUser→GetRewardState | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::{ReqJoinMember, ReqCreateForce} | 0x1400166F0 | verified | RelayServer decompile `0x1400166F0 / 0x1400149C0` + `main=8/sub=2,1` DB game packet + DBLog 23/5 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResJoinMember | 0x140016A60 | verified | RelayServer decompile `0x140016A60` + `0xFA/0x02` fanout + `AddRecruitMember | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::{ResUpdateMemberInfo, SendForceMessage, EnterServer, GetForce, ReqForceInfo} | 0x140017CC0 | verified | IDA `0x140017CC0 / 0x140017D50 / 0x140016380 / 0x1400148D0 / 0x140014970 / 0x140026960` + ResUpdateMemberInfo:XSendPacket(0xFA,5)+SendPacketAll; SendForceMessage:XSendPacket(0xFA,0x10)+stChatForce+psChatItemLinkInfo+SendPacketAll; EnterServer:GetForce+SetMemberInfo+m_mapForceUser+bReqForceInfo→GetPartyInfo+GetMemberInfo+SendPacket(0xFA,0xA)+master→GetPartyRecruitInfo+SendPacket(0xF4,0x2E); 差异:IDA中GetMemberInfo/dwForceID赋值在bReqForceInfo外,源码在内; GetForce(dwForceID):find+return; GetForce(UXActorID):GetPartyID+GetForce; ReqForceInfo:lambda检查IsState+GetForce+GetForceInfo+SendPacket(0xFA,0x1A) | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::{CreateForceMatching, DeleteForce, ReqForceInfo(actor-first fix)} | 0x140017FE0 | verified | IDA 0x140017FE0/0x140017440/0x140026960 + CreateForceMatching:GetOrCreateForce+SetMasterID+遍历vecForceMember→SetMemberInfo+AddPartyMember; DeleteForce:find+GetForceInfo+遍历vecForceMember→RemoveForceMember+erase; ReqForceInfo:IsState+GetForce+GetForceInfo+SendPacket(0xFA,0x1A); 修复:DeleteForce补充vecForceMember遍历和RemoveForceMember调用 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | UXActorID::UXActorID | 0x140067488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::GetForceID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CUserPartyInfo::GetCID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::CForceManager | 0x140079824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::~ClassFactory<CForce,64> | 0x140079952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::~CForceManager | 0x140079984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::GetForce | 0x140080080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::GetForce | 0x140080240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqCreateForce | 0x140080320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::CreateForce | 0x140080528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqInviteForce | 0x140081200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqAcceptForce | 0x140083248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqCancelForce | 0x140086528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::EnterServer | 0x140086912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_RES_FORCE_ENTER_SERVER::PS_RES_FORCE_ENTER_SERVER | 0x140087696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_RES_FORCE_ENTER_SERVER::~PS_RES_FORCE_ENTER_SERVER | 0x140087760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqJoinMember | 0x140087792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ResJoinMember | 0x140088672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqForceLeave | 0x140089184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ResForceLeave | 0x140090272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqChangeMaster | 0x140090464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ResChangeMaster | 0x140091008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::DeleteForce | 0x140091200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqDeleteForce | 0x140091536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ResDeleteForce | 0x140091936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ResLoadForceAll | 0x140092352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_INFO::PS_FORCE_INFO | 0x140092880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqUpdateMemberInfo | 0x140093024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ResUpdateMemberInfo | 0x140093376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::SendForceMessage | 0x140093520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_CHAT_ITEM_LINK_FOR_SERVER::~PS_CHAT_ITEM_LINK_FOR_SERVER | 0x140093712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::SendForceErrorInvite | 0x140093776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::SendForceErrorAccept | 0x140094000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::CreateForceMatching | 0x140094176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqForceMazeClear | 0x140094672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::SetMaze | 0x140095104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::SendForceNameChange | 0x140095600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::ClassFactory<CForce,64> | 0x140096192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::create | 0x140096256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::destroy | 0x140099360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | UXActorID::operator< | 0x140108704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::GetMazeID | 0x140108768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XSendDBPacket::XSendDBPacket | 0x140108816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::GetForceType | 0x140108928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_ITEM_SOCKET_LIST::~PS_ITEM_SOCKET_LIST | 0x140109376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_ITEM_PACKAGE::~PS_ITEM_PACKAGE | 0x140109840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_CHAT_ITEM_LINK::~PS_CHAT_ITEM_LINK | 0x140109872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::SetMazeID | 0x140109936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XParse::SetBIGINT | 0x140109984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XParse::operator<< | 0x140110048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CPartyRecruit::GetMasterID | 0x140110096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_INFO::PS_FORCE_INFO | 0x140110160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CPartyRecruit::GetRecruitID | 0x140110288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CUserObject::GetUAID | 0x140110288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_REQ_FORCE_CREATE::PS_REQ_FORCE_CREATE | 0x140110304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_ADDMEMBER::PS_FORCE_ADDMEMBER | 0x140110432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::GetUserCount | 0x140110496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::GetMasterID | 0x140110528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CUserObject::GetMapID | 0x140110544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XResourceMgr::GetTB_DISTRICT | 0x140111088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ST_PARTY_RECRUIT_INFO::~ST_PARTY_RECRUIT_INFO | 0x140111200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ST_PARTY_RECRUIT::ST_PARTY_RECRUIT | 0x140111232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ST_PARTY_RECRUIT_INFO::ST_PARTY_RECRUIT_INFO | 0x140111280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CUserObject::GetCID | 0x140111328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XSendDBPacket::~XSendDBPacket | 0x140111344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XParse::SetINT | 0x140111376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XSendDBPacket::XSendDBPacket | 0x140111440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_INFO::~PS_FORCE_INFO | 0x140121328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CUserPartyInfo::GetMatchingState | 0x140203008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XParse::SetBYTE | 0x140254224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::Update | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::GetForceID | 0x140607776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::AddForceMember | 0x140622944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::RemoveForceMember | 0x140623136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForceManager::IsForce | 0x140623280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CForce::SetForceType | 0x140632672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | ST_FORCE_INVITE_INFO::ST_FORCE_INVITE_INFO | 0x140632704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | CPartyRecruit::SetPartyID | 0x140821136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceManager.h | XParse::operator<< | 0x140889856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{AutoMatchingAccept, LeaderSelect} | 0x14001CCB0 | verified | IDA `0x14001CCB0 / 0x14001CDA0` + AutoMatchingAccept:遍历8+byCheck分支+m_bCheck赋值+allAccepted→LeaderSelect+SendMatchingWait; LeaderSelect:遍历找最高等级+最高经验作为leader | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{OnUpdate, SendMatchingWait, AutoMatchingExit} | 0x14001E2A0 | verified | IDA `0x14001E2A0 / 0x14001E170 / 0x14001C920` + OnUpdate:process==0→LogError+SendMatchingExit/process==2→return/!CheckMazeOpenTime→exit/tick→wait/state==0→MatchingPossible/state==1→MatchingCheck/state==2→MatchingWait; SendMatchingWait:SetMatchingState(2)+10000ms+遍历8+XSendPacket(0xFA,0x17); AutoMatchingExit:process!=1→false/state==2/3→false/loop+SendMatchingExit+byReason check+SetMatchingState(3) | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CheckMazeOpenTime | 0x14001E680 | verified | IDA 0x14001E680 + std::time+4秒间隔+CheckMazeOpenTime(m_dwMazeID) | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{CreateMazeMatching, SendCreateMatchingMaze} | 0x14001E750 | verified | IDA `0x14001E750 / 0x14001DB30` + CreateMazeMatching:PS_FORCE_INFO+ST_CREATE_MAZE+CreateForceMatching+0xF2/0x43控制包; SendCreateMatchingMaze:循环8+0xFA/0x18包+SetMatchingState(0)+SetMatchingID(0,0)+GetUser+SendDBLog(23,10)+m_bSendSucc=1+m_byProcess=0 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingInfo` average-level fix | 0x14001EB80 | verified | IDA 0x14001EB80 + 循环8+SendPacket(0xFA,0x13)+m_shAveLevel计算 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{SendMatchingCheck, SendMatchingReset, SendMatchingStart, MatchingPossible, MatchingCheck, MatchingWait} | 0x14001F4F7 | verified | IDA `0x14001F4F7调用点 / 0x14001E4B9调用点 / 0x14001D620 / 0x14001E4A0 / 0x14001D410 / 0x14001E3E5调用点` + SendMatchingCheck:SetMatchingState(1)+10000ms+Send(0xFA,0x15); SendMatchingReset:m_nResetCount+++SetMatchingState(0)+60000ms+Send(0xFA,0x16); SendMatchingStart:m_byProcess=2+用户数检查+DB(8,0xD); MatchingPossible:currentCount>3→SendMatchingCheck / resetCount>=3→exit+state(3); MatchingCheck:遍历m_bCheck+LeaderSelect+SendMatchingWait; MatchingWait:SendMatchingStart | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchingMgr::{ExitMatching, MatchingRemoveUser} | 0x1400215C0 | verified | IDA `0x1400215C0 / 0x140021C90` + ExitMatching:find matching+AutoMatchingExit; MatchingRemoveUser:GreenDamTan_CollectGroupedMatchingIds+遍历匹配退出; 结构差异:源码有m_mpAutoMatching_Waiter回退分支(IDA无) | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchingMgr::CheckMatching | 0x140021680 | verified | IDA 0x140021680 + GetPartyUser+GetMatchingID/GetRewardState检查+find matching+AutoMatchingAccept | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchingMgr::{ResForceMatchingCreate, SendCreateMatchingMaze} | 0x140021980 | verified | IDA `0x140021980 / 0x140021A00` + ResForceMatchingCreate:find matching→CreateMazeMatching(dwForceID); SendCreateMatchingMaze:find matching→CForceMatching::SendCreateMatchingMaze | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchginMember::Clear | 0x14009D9A0 | verified | IDA 0x14009D9A0 + m_pCurServer=nullptr + m_nExp=0 + m_nState=0 + memset(&m_stMemberInfo,0); 与CPartyMatchginMember::Clear共享同一实现(同名IDA函数),源码逻辑匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CParty::GetPartyID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | PS_PARTY_INFO::PS_PARTY_INFO | 0x140110160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::CForceMatching | 0x140111568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::~CForceMatching | 0x140111648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::Init | 0x140111696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::AutoMatchingEnter | 0x140111968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::CheckAutoMatchingEnter | 0x140112576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::AutoMatchingExit | 0x140112928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::AutoMatchingCreate | 0x140113200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::AutoMatchingAccept | 0x140113840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::LeaderSelect | 0x140114080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendMatchingInfo | 0x140114416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendMatchingExit | 0x140115040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendMatchingStart | 0x140116256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | PS_DB_FORCE_MATCHING_CREATE::~PS_DB_FORCE_MATCHING_CREATE | 0x140117120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::CheckFullUser | 0x140117184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendCreateMatchingMaze | 0x140117280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendMatchingCheck | 0x140118368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendMatchingReset | 0x140118672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SendMatchingWait | 0x140119152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::OnUpdate | 0x140119456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::MatchingPossible | 0x140119888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::MatchingCheck | 0x140120080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::MatchingWait | 0x140120352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::CheckMazeOpenTime | 0x140120384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::GetMatchingUserCount | 0x140120512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::CreateMazeMatching | 0x140120608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | PS_PARTY_INFO::~PS_PARTY_INFO | 0x140121328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_MAP_INFO::ST_MAP_INFO | 0x140124112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_PARTY_INFO::ST_PARTY_INFO | 0x140124352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | XVec3::XVec3 | 0x140124384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | STPosInfo::STPosInfo | 0x140124448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_CREATE_MAZE::ST_CREATE_MAZE | 0x140124560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_CREATE_MAZE::~ST_CREATE_MAZE | 0x140124960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_CREATE_FORCE::ST_CREATE_FORCE | 0x140125008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | PS_DB_FORCE_MATCHING_CREATE::PS_DB_FORCE_MATCHING_CREATE | 0x140129440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CUserPartyInfo::SetMatchingID | 0x140129520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_FORCE_MATCHING_INFO::ST_FORCE_MATCHING_INFO | 0x140129648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SetMatchingState | 0x140129728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatchginMember::CForceMatchginMember | 0x140129760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CUserPartyInfo::SetMatchingState | 0x140187824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatchginMember::~CForceMatchginMember | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | CForceMatchginMember::Clear | 0x140641440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatching.h | ST_ENTER_MAZE_MEMBER_INFO::ST_ENTER_MAZE_MEMBER_INFO | 0x140821536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CPartyMember::GetMemberID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CUserPartyInfo::GetMatchingID | 0x140111328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::CForceMatchingMgr | 0x140129808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::~CForceMatchingMgr | 0x140129872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::CreateMatching | 0x140129936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::EnterMatching | 0x140130272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::EnterMatching | 0x140130544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | PS_SERVER_FORCE_MATCHING_ENTER::~PS_SERVER_FORCE_MATCHING_ENTER | 0x140132480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::ExitMatching | 0x140132544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::CheckMatching | 0x140132736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::OnUpdate | 0x140133136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::ResForceMatchingCreate | 0x140133504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::SendCreateMatchingMaze | 0x140133632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | ST_CREATE_MAZE::ST_CREATE_MAZE | 0x140133952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::MatchingRemoveUser | 0x140134288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForce::GetMember | 0x140139408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CParty::GetMember | 0x140139408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatching::GetMatchingID | 0x140139856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::Parse | 0x140023360 | verified | IDA 0x140023360 + 18 case switch完全匹配IDA (0x01/0x03/0x04/0x05/0x09/0x0A-0x0D/0x10/0x13-0x15/0x19/0x1A/0x21) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{Parse, ReqForceCreate, ReqForceLeaveMember, ReqForceChangeMaster, ReqForceMazeClear} | 0x140023360 | verified | IDA `0x140023360 / 0x140023A60 / 0x1400242E0 / 0x140024610 / 0x140026730` + Parse:18 case switch完全匹配IDA(0x01/0x03/0x04/0x05/0x09/0x0A/0x0B/0x0C/0x0D/0x10/0x13/0x14/0x15/0x19/0x1A/0x21); 注意:case 0x09 IDA显示为boost::multi_index modify_,实际对应ReqForceMatchingEnter(0x140024A40) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceInvite, ReqForceAccept, ReqForceCancel} | 0x140023434 | verified | IDA 0x140023434/0x140023448/0x14002345C (Parse entries) + PS_REQ_FORCE_INVITE>>dwUAID>>byLevel>>dwForceID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceMatchingEnter, ReqForceMatchingExit, ReqForceMatchingCheck} | 0x14002347E | verified | IDA 0x14002347E/0x14002348F/0x1400234A0 (Parse entries) + 0x140025ED0/0x14001C920/0x140021680 handler addresses | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceUpdateMember, ReqForceEnterServer, ReqForceInfo} | 0x140023570 | verified | IDA 0x140023570/0x1400248D0/0x140026870 + ST_UPDATE_FORCE_MEMBER>>DoJob(0) / PS_REQ_FORCE_ENTER_SERVER>>DoJob(0) / XParse>>dwForceID>>dwActorID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceCreate, ReqForceLeaveMember, ReqForceChangeMaster, ReqForceMazeClear} | 0x140023A60 | verified | IDA 0x140023A60/0x1400242E0/0x140024610/0x140026730 + PS_REQ_FORCE_CREATE>>DoJob(0) / PS_FORCE_LEAVE>>dwActorID>>dwUAID>>byLevel>>byLeaverLevel+DoJob(0) / PS_FORCE_CHANGE_MASTER>>DoJob(0) / XParse>>dwForceID>>byClearFail+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceMatchingExit, ReqForceMatchingCheck} | 0x140025ED0 | verified | IDA 0x140025ED0/0x1400265E0 + dwActorID>>byReason>>dwUAID>>byLevel+GetClientPtr+DoJob(0,lambda) / PS_SERVER_FORCE_MATCHING_CHECK>>GetClientPtr+DoJob(0,lambda→CheckMatching) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceChangeMazeOpenCheck | 0x140026AA0 | verified | IDA 0x140026AA0 + bOn默认1+XParse>>bOn+DoJob(0,lambda→SetCheckMazeOpenTime(bOn)); 源码用DispatchForceJob+GetMazeOpenControl().SetCheckMazeOpenTime,逻辑匹配; 结构差异:IDA直接调XResourceMgr::SetCheckMazeOpenTime,源码经CRelayMazeOpenControl中转 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::CForceProcess | 0x140139888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::~CForceProcess | 0x140140080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::Parse | 0x140140128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::SyncForceMessage | 0x140140656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK_FOR_SERVER::PS_CHAT_ITEM_LINK_FOR_SERVER | 0x140141136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK::PS_CHAT_ITEM_LINK | 0x140141424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST | 0x140141792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_PACKAGE::PS_ITEM_PACKAGE | 0x140141840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceCreate | 0x140141920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceInvite | 0x140142528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceAccept | 0x140143312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceCancel | 0x140143744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceLeaveMember | 0x140144096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceChangeMaster | 0x140144912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceUpdateMember | 0x140145248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceEnterServer | 0x140145616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_REQ_FORCE_ENTER_SERVER::PS_REQ_FORCE_ENTER_SERVER | 0x140145936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceMatchingEnter | 0x140145984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_ENTER::PS_SERVER_FORCE_MATCHING_ENTER | 0x140150944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceMatchingExit | 0x140151248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceMatchingCheck | 0x140153056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceMazeClear | 0x140153392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceInfo | 0x140153712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceChangeMazeOpenCheck | 0x140154272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::GetClientPtr | 0x140154912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::TXProcess<CServer> | 0x140154976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::~TXProcess<CServer> | 0x140155024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | TXSingleton<CLogicThreadManager>::Instance | 0x140155104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::Init | 0x140155792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::Clear | 0x140155824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | IXProcess::~IXProcess | 0x140174592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::IXComponent | 0x140174640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::~IXComponent | 0x140174768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | IXProcess::IXProcess | 0x140174880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XResourceMgr::SetCheckMazeOpenTime | 0x140175296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_CHECK::PS_SERVER_FORCE_MATCHING_CHECK | 0x140175328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CUserPartyInfo::GetActorID | 0x140175392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_ENTER_MEMBER::PS_SERVER_FORCE_MATCHING_ENTER_MEMBER | 0x140175456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_ENTER::PS_SERVER_FORCE_MATCHING_ENTER | 0x140175696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XParse::operator>> | 0x140175776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XParse::GetDWORD | 0x140175824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XParse::operator>> | 0x140175888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CServer::GetServerID | 0x140175936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | STItem::Init | 0x140175968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | STItem::STItem | 0x140176288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | ST_ITEM_BROACH::ST_ITEM_BROACH | 0x140176320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_PACKAGE::PS_ITEM_PACKAGE | 0x140176384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | ST_EXTEND_OPTION::operator= | 0x140176464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | ST_SOCKET_DATA::operator= | 0x140176512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | ST_ITEM_SOCKET::operator= | 0x140176640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK::PS_CHAT_ITEM_LINK | 0x140177728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK_FOR_SERVER::PS_CHAT_ITEM_LINK_FOR_SERVER | 0x140177920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XPacket::GetSubCmd | 0x140178000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::SetName | 0x140180128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::SetCmd | 0x140180192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CUserPartyInfo::GetMatchingType | 0x140244992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | ST_UPDATE_FORCE_MEMBER::ST_UPDATE_FORCE_MEMBER | 0x140303312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | UXActorID::UXActorID | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_FORCE_REJECT::PS_FORCE_REJECT | 0x140707984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | PS_REQ_FORCE_INVITE::PS_REQ_FORCE_INVITE | 0x140708048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceUpdateInfo | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XParse::GetBYTE | 0x140889904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ForceProcess.h | XParse::Getbool | 0x140889904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{Parse, ReqFriendListLoad, ReqBlockListLoad} | 0x140040370 | verified | IDA 0x140040370/0x1400405F0/0x140040640 + Parse:21 case switch完全匹配IDA + ReqFriendListLoad:XParse>>dwReqUCID+SendFriendList + ReqBlockListLoad:XParse>>dwReqUCID+SendBlockList | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendListLoad | 0x1400405F0 | verified | export-for-ai 0x1400405F0 + XParse >> dwReqUCID + TXSingleton::Instance()->SendFriendList | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqBlockListLoad | 0x140040640 | verified | export-for-ai 0x140040640 + XParse >> dwReqUCID + TXSingleton::Instance()->SendBlockList | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendDelete | 0x140040690 | verified | export-for-ai 0x140040690 + PS_REQ_FRIEND_DELETE >> + PrepareDeleteFriend | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqUpdateFriendCommunity | 0x1400406D0 | verified | export-for-ai 0x1400406D0 + XParse >> dwActorID + ST_CHAR_COMMUNITY >> + UpdateFriendCommunity | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendInvite | 0x140040760 | verified | export-for-ai 0x140040760 + PS_RES_FRIEND_INVITE >> + PrepareFriendInvite | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{ReqFriendInvite, ReqFriendAccept, ReqFriendDelete} | 0x140040760 | verified | IDA 0x140040760/0x1400407D0/0x140040690 + PS_RES_FRIEND_INVITE>>PrepareFriendInvite / PS_REQ_FRIEND_ACCEPT>>PrepareFriendAccept / PS_REQ_FRIEND_DELETE>>PrepareDeleteFriend | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendAccept | 0x1400407D0 | verified | export-for-ai 0x1400407D0 + PS_REQ_FRIEND_ACCEPT >> + PrepareFriendAccept | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqBlockListAdd | 0x140040830 | verified | export-for-ai 0x140040830 + PS_REQ_FRIEND_BLOCK_ADD >> + PrepareBlockListAdd | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{ReqBlockListAdd, ReqBlockListDelete} | 0x140040830 | verified | IDA 0x140040830/0x140040890 + PS_REQ_FRIEND_BLOCK_ADD>>PrepareBlockListAdd / PS_REQ_FRIEND_BLOCK_DELETE>>PrepareBlockListDel | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqBlockListDelete | 0x140040890 | verified | export-for-ai 0x140040890 + PS_REQ_FRIEND_BLOCK_DELETE >> + PrepareBlockListDel | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendRecommand | 0x1400408F0 | verified | export-for-ai 0x1400408F0 + PS_RES_FRIEND_RECOMMAND >> + RecommandFriend | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendRecruitList | 0x140040970 | verified | export-for-ai 0x140040970 + GetClientPtr + DoJob(2) + RecruitList; 线程优先级: Friend=worker-2 | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{ReqFriendRecruitList, ReqFriendRecruitAdd, ReqFriendRecruitInfo} | 0x140040970 | verified | IDA 0x140040970/0x140040AD0/0x140040C30 + PS_RECRUIT_LIST>>GetClientPtr+DoJob(2) / PS_RECRUIT_ADD>>DoJob(2) / XParse>>dwUCID+DoJob(2) | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendRecruitAdd | 0x140040AD0 | verified | export-for-ai 0x140040AD0 + DoJob(2) + PrepareAddRecruit | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendRecruitDelete | 0x140040B80 | verified | export-for-ai 0x140040B80 + DoJob(2) + PrepareDeleteRecruit | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::ReqFriendFind | 0x140040CF0 | verified | IDA 0x140040CF0 + PS_REQ_FRIEND_FIND>> + ReqFriendFind | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{ReqCheckDailyMissionFirend, ResCheckDailyMissionFirend} | 0x140040D50 | verified | IDA 0x140040D50/0x140040DD0 + PS_DAILY_MISSION_FRIEND_REQ>>DailyMissionFriendReq / PS_DAILY_MISSION_FRIEND_RES>>DailyMissionFriendRes | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{ReqHelperSupportInfo, ReqHelperSupportList} | 0x140040E60 | verified | IDA 0x140040E60/0x140040F70 + XParse>>dwUCID+HelperSupportInfo / XParse>>dwUCID+HelperSupportList | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::{ReqHelperSupportRegister, ReqHelperSupportReward, ReqHelperSupportEquip} | 0x140040EB0 | verified | IDA 0x140040EB0/0x140040EF0/0x140040FC0 + PS_SERVER_HELPER_SUPPORT_REGISTER>>HelperSupportRegister / PS_SERVER_HELPER_SUPPORT_REWARD>>HelperSupportReward / PS_HELPER_SUPPORT_EQUIP_REQ>>HelperSupportEquip | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendRecommandManager::{构造, AddUser, DeleteUser} | 0x140042340 | verified | IDA 0x140042340/0x140042440/0x140042600 + 构造:map+m_mapUserCheck+m_nIndex=0 / AddUser:SYSTEM_TYPE_REAL+GMPower跳过/level>68跳过/m_nIndex[level]+++dwKey=index+10000*level+insert+SetRecommandIndex / DeleteUser:m_mapUserCheck.find(GetMatchingID)→find+erase+DeleteUserCheck | 否 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_BLOCK_ADD::PS_REQ_FRIEND_BLOCK_ADD | 0x140079184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_BLOCK_DELETE::PS_REQ_FRIEND_BLOCK_DELETE | 0x140079184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_FIND::PS_REQ_FRIEND_FIND | 0x140079184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::CFriendProcess | 0x140258688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::~CFriendProcess | 0x140258880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::Parse | 0x140258928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendListLoad | 0x140259568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqBlockListLoad | 0x140259648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendDelete | 0x140259728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqUpdateFriendCommunity | 0x140259792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendInvite | 0x140259936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendAccept | 0x140260048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqBlockListAdd | 0x140260144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqBlockListDelete | 0x140260240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendRecommand | 0x140260336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_RES_FRIEND_RECOMMAND::~PS_RES_FRIEND_RECOMMAND | 0x140260432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendRecruitList | 0x140260464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendRecruitAdd | 0x140260816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendRecruitDelete | 0x140260992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendRecruitInfo | 0x140261168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqFriendFind | 0x140261360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqCheckDailyMissionFirend | 0x140261456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_DAILY_MISSION_FRIEND_REQ::~PS_DAILY_MISSION_FRIEND_REQ | 0x140261552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ResCheckDailyMissionFirend | 0x140261584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_DAILY_MISSION_FRIEND_RES::PS_DAILY_MISSION_FRIEND_RES | 0x140261680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_RES_FRIEND_RECOMMAND::PS_RES_FRIEND_RECOMMAND | 0x140261680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqHelperSupportInfo | 0x140261728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqHelperSupportRegister | 0x140261808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqHelperSupportReward | 0x140261872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_SERVER_HELPER_SUPPORT_REWARD::~PS_SERVER_HELPER_SUPPORT_REWARD | 0x140261968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqHelperSupportList | 0x140262000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | CFriendProcess::ReqHelperSupportEquip | 0x140262080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | ST_CREATE_ITEMS::ST_CREATE_ITEMS | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | ST_CREATE_ITEMS::~ST_CREATE_ITEMS | 0x140266560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_SERVER_HELPER_SUPPORT_REWARD::PS_SERVER_HELPER_SUPPORT_REWARD | 0x140266592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_SERVER_HELPER_SUPPORT_REGISTER::PS_SERVER_HELPER_SUPPORT_REGISTER | 0x140266672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_DAILY_MISSION_FRIEND_REQ::PS_DAILY_MISSION_FRIEND_REQ | 0x140266720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_RECRUIT_ADD::PS_RECRUIT_ADD | 0x140266800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_RECRUIT_DELETE::PS_RECRUIT_DELETE | 0x140266800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_RECRUIT_LIST::PS_RECRUIT_LIST | 0x140266832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_ACCEPT::PS_REQ_FRIEND_ACCEPT | 0x140266896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_RES_FRIEND_INVITE::PS_RES_FRIEND_INVITE | 0x140266960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | ST_CHAR_COMMUNITY::ST_CHAR_COMMUNITY | 0x140267040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_HELPER_SUPPORT_EQUIP_REQ::PS_HELPER_SUPPORT_EQUIP_REQ | 0x140658432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_DELETE::PS_REQ_FRIEND_DELETE | 0x140821536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendProcess.h | PS_DAILY_MISSION_FRIEND_RES::~PS_DAILY_MISSION_FRIEND_RES | 0x140976032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::CFriendRecommandManager | 0x140267072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::~CFriendRecommandManager | 0x140267232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::AddUser | 0x140267328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::DeleteUser | 0x140267776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::DeleteUserCheck | 0x140268224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::UpdateLevel | 0x140268368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::GetFriendRecommandList | 0x140268512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CCommunity::GetRecommandIndex | 0x140277648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CUserObject::GetRecommandIndex | 0x140277680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | ST_RECOMMAND_FRIEND_INFO::ST_RECOMMAND_FRIEND_INFO | 0x140277728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CCommunity::SetRecommandIndex | 0x140277840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CUserObject::SetRecommandIndex | 0x140277872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CUserObject::GetGMPower | 0x140277920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecommandManager.cpp | XOption::GetSystemType | 0x140277952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::CFriendRecruitManager | 0x140277984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::~CFriendRecruitManager | 0x140278144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::LoadRecruitList | 0x140278208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::OnUpdate | 0x140278800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::AddRecruit | 0x140279344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::DeleteRecruit | 0x140279744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::UpdateRecruit | 0x140279936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::IsRecruitList | 0x140280160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::GetFriendRecruitList | 0x140280320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::GetRecruitAddTime | 0x140282816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::UpdateCharCommunity | 0x140283072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::ChangeName | 0x140283328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetClass | 0x140295760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetLevel | 0x140296000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetInfo | 0x140296016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetLogin | 0x140296096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::CRecruitUser | 0x140296112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetUCID | 0x140296208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | ST_RECRUIT_INFO::ST_RECRUIT_INFO | 0x140296304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::CRecruitUser | 0x140296464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::{OnParse, DBParse, DBPartyParse, DBForceParse, ResForceMatchingCreate} | 0x140049770 | verified | IDA `0x140049770 / 0x1400497C0 / 0x140049930 / 0x140049A60 / 0x14004DC30` + OnParse:简化为仅xSessionID>>DBParse(nullptr); DBParse:补充case 0x26/0x27/0x43; DBPartyParse:case 1/2/3/4/5/6/0x11/0x13完全匹配; DBForceParse:case 1/2/3/4/5/6/0x0B/0x0D完全匹配; ResForceMatchingCreate:nErrorCode/matchingID/dwForceID>>DoJob(0); 修复:OnParse移除FindUser分支, DBParse补充DBHelperParse/DBExchangeParse/DBWorldParse分发 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBPartyParse | 0x140049930 | verified | export-for-ai 0x140049930 + 完整8 case switch(1-6/0x11/0x13) | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBForceParse | 0x140049A60 | verified | export-for-ai 0x140049A60 + 完整8 case switch(1-6/0x0B/0x0D) | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBFriendParse | 0x140049B80 | verified | export-for-ai 0x140049B80 + 完整12 case switch(0x01-0x11) | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::{DBFriendParse, ResRecruitDelete} | 0x140049B80 | verified | IDA `0x140049B80 / 0x14004C150` + DBFriendParse:12 case switch(0x01-0x09/0x10/0x11)完全匹配IDA; ResRecruitDelete:PS_RES_RECRUIT_DELETE>>+DoJob(2)+DeleteRecruit; 修复:添加缺失的11个DBFriendParse handler声明和stub实现 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResLeagueCreate | 0x14004A620 | verified | export-for-ai 0x14004A620 + PS_LEAGUE_CREATE_FOR_SERVER >> + DoJob(1); 线程优先级: League=worker-1 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::{ResPartyJoin, ResForceJoin} | 0x14004AA80 | verified | RelayServer decompile `0x14004AA80 / 0x14004B340` + worker-0 dispatch | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceCreate | 0x14004B1F0 | verified | export-for-ai 0x14004B1F0 + PS_REQ_FORCE_CREATE>> + DoJob(0) + CreateForce | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceJoin | 0x14004B340 | verified | export-for-ai 0x14004B340 + PS_FORCE_ADDMEMBER>>dwRecruitID + DoJob(0) + ResJoinMember | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceLeave | 0x14004B500 | verified | export-for-ai 0x14004B500 + nErrorCode>>stForceLeave>>dwNewMaster + DoJob(0) + ResForceLeave | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceDelete | 0x14004B800 | verified | export-for-ai 0x14004B800 + PS_FORCE_LEAVE>> + DoJob(0) + ResDeleteForce | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResLeagueDeletePenalty | 0x14004e690 | verified | IDA 0x14004e690 + dwUCID>>biPenalty + DoJob(1,GetUser+null检查+SetLeagueDeletePenalty) | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResLeagueWithdrawPenalty | 0x14004e7e0 | verified | IDA 0x14004e7e0 + dwUCID>>biPenalty + DoJob(1,GetUser+null检查+SetLeagueWithdrawPenalty) | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::{ResLeagueWithdrawPenalty, ResLeagueDeletePenalty} | 0x14004e7e0 | verified | IDA 0x14004e7e0/0x14004e690 + XParse>>dwUCID>>biPenalty + DoJob(1,lambda) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_PARTY_CREATE::PS_REQ_PARTY_CREATE | 0x140110304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_PARTY_ADDMEMBER::PS_PARTY_ADDMEMBER | 0x140110432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | XParse::GetINT | 0x140175824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_BLOCKLIST_INFO::PS_BLOCKLIST_INFO | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_FORCE_INFO_ALL::PS_FORCE_INFO_ALL | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_BROACH_LIST::PS_ITEM_BROACH_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_INVENTORY_FOR_LOG_LIST::PS_LEAGUE_INVENTORY_FOR_LOG_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_SUMMARY_LIST::PS_LEAGUE_SUMMARY_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_PARTY_INFO_ALL::PS_PARTY_INFO_ALL | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_CHECK_LIST::ST_LEAGUE_APPLICANT_CHECK_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_LIST::ST_LEAGUE_APPLICANT_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD_LIST::ST_LEAGUE_BOARD_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_LIST::ST_LEAGUE_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_LIST::ST_LEAGUE_MEMBER_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECORD_LIST::ST_LEAGUE_RECORD_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_RECRUIT_LIST::ST_RECRUIT_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::CGameDBSocket | 0x140296496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::~CGameDBSocket | 0x140296576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::FindUser | 0x140296624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::SetInfomation | 0x140296672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::OnParse | 0x140296816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBParse | 0x140296896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBPartyParse | 0x140297264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBForceParse | 0x140297568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBFriendParse | 0x140297856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBLeagueParse | 0x140298224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueNoticeChange | 0x140299248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicant | 0x140300000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueCreate | 0x140300576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBWorldParse | 0x140301136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBExchangeParse | 0x140301200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBHelperParse | 0x140301280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyCreate | 0x140301360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyJoin | 0x140301696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyLeave | 0x140302032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyChangeMaster | 0x140302416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyDelete | 0x140302768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyUpdateMemberInfo | 0x140303024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_UPDATE_PARTY_MEMBER::ST_UPDATE_PARTY_MEMBER | 0x140303312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyLoadAll | 0x140303360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_FORCE_INFO_ALL::~PS_FORCE_INFO_ALL | 0x140303568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_PARTY_INFO_ALL::~PS_PARTY_INFO_ALL | 0x140303568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceCreate | 0x140303600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceJoin | 0x140303936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceLeave | 0x140304384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceChangeMaster | 0x140304880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceDelete | 0x140305152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceUpdateMemberInfo | 0x140305408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceLoadAll | 0x140305696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendLoad | 0x140305904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_LIST::~PS_DB_FRIEND_LIST | 0x140306304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_BLOCKLIST_INFO::~PS_BLOCKLIST_INFO | 0x140306336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendInvite | 0x140306368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_DB_FRIEND_INVITE::PS_RES_DB_FRIEND_INVITE | 0x140306480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_INVITE::PS_DB_FRIEND_INVITE | 0x140306528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendInviteCheck | 0x140306608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendAccept | 0x140306768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendDelete | 0x140306880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResBlockListAdd | 0x140306944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResBlockListDelete | 0x140307040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResRecruitLoad | 0x140307136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResRecruitAdd | 0x140307264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResRecruitDelete | 0x140307536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendFind | 0x140307712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_FIND::~PS_DB_FRIEND_FIND | 0x140307808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueWithDraw | 0x140307840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueKick | 0x140308464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueAuthChange | 0x140309104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueInviteAccept | 0x140309824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueDelete | 0x140310624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueBoard | 0x140311584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicantAccept | 0x140312352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicantReject | 0x140313168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::OnDisConnect | 0x140313712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::OnNotConnect | 0x140313920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResPartyMatchingCreate | 0x140314080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResForceMatchingCreate | 0x140314416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeaguePositionNameChange | 0x140314832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueMemberPositionChange | 0x140315488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicantDelete | 0x140316160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResExchangePriceHistory | 0x140316464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_EXCHANGE_PRICE_HISTORY_RES::~PS_DB_EXCHANGE_PRICE_HISTORY_RES | 0x140316576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLoadLeagueMember | 0x140316608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLoadLeagueApplicant | 0x140316752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLoadLeagueBoard | 0x140316896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD_LIST::~ST_LEAGUE_BOARD_LIST | 0x140317040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueDeletePenalty | 0x140317072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueWithdrawPenalty | 0x140317408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResHelperSupportEquip | 0x140317744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueOpenOrNot | 0x140317808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueRecruitNotice | 0x140318288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueSearch | 0x140318880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_SUMMARY_LIST::~PS_LEAGUE_SUMMARY_LIST | 0x140319472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_SUMMARY_LIST::PS_LEAGUE_SUMMARY_LIST | 0x140319504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueRecord | 0x140319664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_LIST::~ST_LEAGUE_MEMBER_LIST | 0x140319808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECORD_LIST::~ST_LEAGUE_RECORD_LIST | 0x140319808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueDelegate | 0x140319840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueCardChange | 0x140320432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_STORAGE_INFO::PS_RES_STORAGE_INFO | 0x140321248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueWealth | 0x140321504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueLevelup | 0x140321872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueSkillLearn | 0x140322480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ReqLeagueInventoryInfo | 0x140322896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE_LIST::PS_ITEM_PACKAGE_LIST | 0x140323984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_BROACH_LIST::PS_ITEM_BROACH_LIST | 0x140324032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ReqLeagueInventoryMove | 0x140324352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::~PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME | 0x140324848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME | 0x140324912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_ITEM_MOVE_LEAGUE_INVEN::PS_RES_ITEM_MOVE_LEAGUE_INVEN | 0x140325264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_INVENTORY_FOR_LOG_LIST::PS_LEAGUE_INVENTORY_FOR_LOG_LIST | 0x140325616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueNameChange | 0x140325664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_NAME_CHANGE_SERVER::~PS_LEAGUE_NAME_CHANGE_SERVER | 0x140326576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_NAME_CHANGE_SERVER::PS_LEAGUE_NAME_CHANGE_SERVER | 0x140326608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueInfo | 0x140326768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_LIST::ST_LEAGUE_APPLICANT_LIST | 0x140328208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD_LIST::ST_LEAGUE_BOARD_LIST | 0x140328256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_LIST::ST_LEAGUE_MEMBER_LIST | 0x140328304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECORD_LIST::ST_LEAGUE_RECORD_LIST | 0x140328304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueList | 0x140328816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResGMTLeagueInfo | 0x140329760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_LIST::~ST_LEAGUE_LIST | 0x140330320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_LIST::ST_LEAGUE_LIST | 0x140330352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::TXDBSocket<CServer> | 0x140332368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXMultiPoolServer<CServer>::FindUser | 0x140336496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | sprintf_s<255> | 0x140336544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXObjectMgr<CServer>::Find | 0x140341584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnFinishThread | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnUpdateThread | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_LEAGUE_LOAD::PS_DB_LEAGUE_LOAD | 0x140395664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_INFO::ST_LEAGUE_INFO | 0x140395744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_NAME_CHANGE_SERVER::PS_LEAGUE_NAME_CHANGE_SERVER | 0x140396144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_STORAGE_INFO::PS_STORAGE_INFO | 0x140396256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_ITEM_MOVE_LEAGUE_INVEN::PS_RES_ITEM_MOVE_LEAGUE_INVEN | 0x140396320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_ITEM_MOVE_LEAGUE_INVEN::~PS_RES_ITEM_MOVE_LEAGUE_INVEN | 0x140396496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_INVENTORY_FOR_LOG_LIST::~PS_LEAGUE_INVENTORY_FOR_LOG_LIST | 0x140397024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_ITEM_MOVE_LEAGUE_INVEN::PS_REQ_ITEM_MOVE_LEAGUE_INVEN | 0x140397056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME | 0x140397200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE_LIST::~PS_ITEM_PACKAGE_LIST | 0x140397344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE::operator= | 0x140398464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE_LIST::PS_ITEM_PACKAGE_LIST | 0x140399440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_BROACH_LIST::~PS_ITEM_BROACH_LIST | 0x140399504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_LEAGUE_SKILL::PS_RES_LEAGUE_SKILL | 0x140399760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_AUTO_SKILL::PS_AUTO_SKILL | 0x140399856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_WEALTH_FOR_SERVER::PS_LEAGUE_WEALTH_FOR_SERVER | 0x140399888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_STORAGE_INFO::~PS_RES_STORAGE_INFO | 0x140399984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_STORAGE_INFO::PS_RES_STORAGE_INFO | 0x140400448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_LEAGUE_CARD::PS_REQ_LEAGUE_CARD | 0x140400496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECRUIT_NOTICE::ST_LEAGUE_RECRUIT_NOTICE | 0x140400560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_HELPER_SUPPORT_EQUIP::PS_DB_HELPER_SUPPORT_EQUIP | 0x140400624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CUserObject::SetLeagueWithdrawPenalty | 0x140400704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CUserObject::SetLeagueDeletePenalty | 0x140400736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_EXCHANGE_PRICE_HISTORY_RES::PS_EXCHANGE_PRICE_HISTORY_RES | 0x140400768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_EXCHANGE_PRICE_HISTORY_RES::~PS_EXCHANGE_PRICE_HISTORY_RES | 0x140400896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_EXCHANGE_PRICE_HISTORY_RES::PS_DB_EXCHANGE_PRICE_HISTORY_RES | 0x140400928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_POSITION::ST_LEAGUE_MEMBER_POSITION | 0x140400992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_POSITION_NAME_CHANGE::ST_LEAGUE_POSITION_NAME_CHANGE | 0x140401056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_REQ_LEAGUE_APPLICANT_REJECT::ST_REQ_LEAGUE_APPLICANT_REJECT | 0x140401120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_REQ_LEAGUE_APPLICANT_ACCEPT::ST_REQ_LEAGUE_APPLICANT_ACCEPT | 0x140401184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD::ST_LEAGUE_BOARD | 0x140401264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER::ST_LEAGUE_MEMBER | 0x140401360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_EX::ST_LEAGUE_MEMBER_EX | 0x140401440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_REQ_LEAGUE_INVITE_ACCEPT::ST_REQ_LEAGUE_INVITE_ACCEPT | 0x140401600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_AUTH_CHANGE::ST_LEAGUE_AUTH_CHANGE | 0x140401680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_FIND_FRIEND_LIST::PS_FIND_FRIEND_LIST | 0x140401744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_FIND_FRIEND_LIST::~PS_FIND_FRIEND_LIST | 0x140402224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_FIND::PS_DB_FRIEND_FIND | 0x140402256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_RECRUIT_ADD::PS_RES_RECRUIT_ADD | 0x140402320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_RECRUIT_LIST::~ST_RECRUIT_LIST | 0x140402368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_BLOCKLIST_DELETE::PS_RES_BLOCKLIST_DELETE | 0x140402608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_BLOCK_INFO::ST_BLOCK_INFO | 0x140402672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_DB_FRIEND_BLOCK::PS_RES_DB_FRIEND_BLOCK | 0x140402720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_DELETE::PS_DB_FRIEND_DELETE | 0x140402784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_ACCEPT_RES::PS_DB_FRIEND_ACCEPT_RES | 0x140402864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_DB_FRIEND_ADD::ST_DB_FRIEND_ADD | 0x140402944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_FRIEND_INFO::ST_FRIEND_INFO | 0x140403008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_LIST::PS_DB_FRIEND_LIST | 0x140403632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | CUserObject::SetLeagueID | 0x140403680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_REQ_LEAGUE_CREATE::ST_REQ_LEAGUE_CREATE | 0x140403712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_CREATE_FOR_SERVER::PS_LEAGUE_CREATE_FOR_SERVER | 0x140403776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT::ST_LEAGUE_APPLICANT | 0x140403952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_NOTICE::ST_LEAGUE_NOTICE | 0x140404064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::Init | 0x140404128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::Shutdown | 0x140404304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnDisConnect | 0x140404528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnConnect | 0x140404592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnNotConnect | 0x140404656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | XClient::IsBlock | 0x140404704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | XPacket::GetMainCmd | 0x140404752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnParse | 0x140404784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::_BackendThreadStartingPoint | 0x140405024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::Init | 0x140405088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::SetSafetyShutdown | 0x140405312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | XTime::GetTickCount | 0x140405344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnStartThread | 0x140405488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::~TXDBSocket<CServer> | 0x140405584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_LEAGUE_DELEGATE::PS_REQ_LEAGUE_DELEGATE | 0x140658432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_CHECK_LIST::~ST_LEAGUE_APPLICANT_CHECK_LIST | 0x140729840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_LIST::~ST_LEAGUE_APPLICANT_LIST | 0x140767952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_RECRUIT_DELETE::PS_RES_RECRUIT_DELETE | 0x140821536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_OPEN::ST_LEAGUE_OPEN | 0x140928128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::XGameDBSocketMgr | 0x140180736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::~XGameDBSocketMgr | 0x140180896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140180928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140184480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x140185104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x140186112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x140186240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | TXDBSocket<CServer>::AutoConnect | 0x140186368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x140186432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140186448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XServer::GetOption | 0x140186464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::CHelperSupport | 0x140186496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::~CHelperSupport | 0x140186544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::Init | 0x140186608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::CheckReceived | 0x140186704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::AddReceived | 0x140186816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::CheckVaildTime | 0x140186944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::GetRecvCount | 0x140187792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::SetRewardState | 0x140187824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupport.cpp | ST_HELPER_SUPPORT_INFO::ST_HELPER_SUPPORT_INFO | 0x140187856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::CHelperSupportMgr | 0x140187920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::~CHelperSupportMgr | 0x140188064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::FindSupport | 0x140188272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::AddSupport | 0x140188496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::DeleteSupport | 0x140189232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::AddSupportTime | 0x140189520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::DeleteSupportTime | 0x140190080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::GetSupportReward | 0x140190560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::AddSupportReceived | 0x140190688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupport::GetRewardState | 0x140203008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupport::GetSupportInfo | 0x140203248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.cpp | CLeague::{SetLeagueInfo, IsMaster, Clear} | 0x140064220 | verified | IDA 0x140064220/0x1400646F0/0x1400643C0 + SetLeagueInfo:qmemcpy(0x800) / IsMaster:m_stLeagueInfo.dwMasterUCID比对 / Clear:m_nInventoryCount=m_nSyncCount=0 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CLeague() | 0x140064270 | verified | export-for-ai 0x140064270 + ST_LEAGUE_INFO构造 + map构造 + queue构造 + Clear() | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::Clear() | 0x1400643C0 | verified | export-for-ai 0x1400643C0 + m_nInventoryCount=0 + m_nSyncCount=0 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeaguePosition | 0x140064520 | verified | IDA 0x140064520 + 职位名称设置(1/2/3 -> szPosition_1/2/3) + szPrevPositionName保存 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LeagueMemberUpdate | 0x140064620 | verified | IDA 0x140064620 + 从ST_LEAGUE_MEMBER_EX填充ST_LEAGUE_MEMBER_UPDATE | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::AddMember(ST_LEAGUE_MEMBER_EX) | 0x140064720 | verified | export-for-ai 0x140064720 + find→存在:SetLeagueMember / 不存在:new CLeagueMember(0x78)+SetLeagueMember+insert | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::{AddMember, IsMember} | 0x140064720 | verified | IDA 0x140064720/0x140064D00 + AddMember:find→SetLeagueMember/新建shared_ptr+insert / IsMember:find+return iter!=end | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LoginMember(uint32) | 0x140064A00 | verified | export-for-ai 0x140064A00 + find→null→false + shared_ptr→Login() + return true | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::{LoginMember, LogOutMember} | 0x140064A00 | verified | IDA 0x140064A00/0x140064AF0 + LoginMember:find→Login()+SendLeagueInfo / LogOutMember:find→LogOut()+SetPlayDate(biLogoutDate) | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LogOutMember(uint32, int64) | 0x140064AF0 | verified | export-for-ai 0x140064AF0 + find→LogOut() + SetPlayDate(biLogoutDate) | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::AddApplicant | 0x140064BA0 | verified | IDA 0x140064BA0 + pair(wOrder,stApplicant)+insert(m_mpLeagueApplicant) | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::GetLeagueMemberPtr(uint32) | 0x140065250 | verified | export-for-ai 0x140065250 + find→return shared_ptr / end→return nullptr | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckPositionCount | 0x140065360 | verified | IDA 0x140065360 + 成员遍历+职位计数(副盟主最多1/管理员最多3/长老最多10) | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ChangeMemberPosition | 0x140065580 | verified | IDA 0x140065580 + GetLeagueMemberPtr+GetPosition+SetPosition+副盟主清空/设置+SetLeagueInfoForGame+SendChangePositionToMember | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckLeagueDelegate | 0x1400657c0 | verified | IDA 0x1400657c0 + IsMaster→57015 + GetLeagueMemberPtr(null→57007) + shMemberCount<2→57044 + byRating<2→57044 + return 0 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::Delegate | 0x140065910 | verified | IDA 0x140065910 + position切换(0/100)+名称获取+ST_LEAGUE_RECORD(byFlag=12)+UpdateRecord+GetUAID+SendDBLog(15,22,byRating) | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckLeagueCardChange | 0x140065ee0 | verified | IDA 0x140065ee0 + HaveSkill(E_SKILL_CARD)→57046 + IsMaster→0/57015 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CardChange | 0x140065fe0 | verified | IDA 0x140065fe0 + m_stLeagueInfo.dwLeagueCard = stCard.dwLeagueCard | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::HaveSkill | 0x140066010 | verified | IDA 0x140066010 + return m_stLeagueInfo.bySkill[eSkill] != 0 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckLearnSkill | 0x140066050 | verified | IDA 0x140066050 + IsMaster→GetTB_LEAGUE_SKILL→前置技能/等级/技能点/金币检查→填充PS_RES_LEAGUE_SKILL→return 0/错误码 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LearnSkill | 0x140066280 | verified | IDA 0x140066280 + 扣除金币/技能点+更新bySkill+SendLearnSkillToMember+SendDBLog(15,21)+(15,20) | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::Levelup | 0x1400666c0 | verified | IDA 0x1400666c0 + 等级上限10检查+GetTB_LEAGUE_INFO+自动技能遍历+UpdateSyncCount+DB(7,0x34) | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ApplyLevelup | 0x140066910 | verified | IDA 0x140066910 + 更新byRating/bySkillPoint/bySkill[]+SendLevelupToMember+SendDBLog(15,17)+(15,20) | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CalculateExp | 0x140066d90 | verified | IDA 0x140066d90 + GetLeagueMemberPtr+GetTB_LEAGUE_INFO+联赛每日上限/成员上限200检查+金币上限999999999+升级循环+Levelup+DB(7,0x33) | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ApplyWealth | 0x140067390 | verified | IDA 0x140067390 + GetLeagueMemberPtr+AddExp+ST_LEAGUE_INFO_UPDATE+SendLeagueWealthToMember+SendDBLog(15,18)+(15,19) | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateRecord | 0x140067540 | verified | IDA 0x140067540 + size>100→pop+push+DB(7,0x30)+SendRecordToMember | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateApplyList | 0x140067820 | verified | IDA 0x140067820 + 遍历m_mpLeagueApplicant + CTimeSpan(86400)超时 + DB(7,0x20) + erase; 注意:源码用m_mpApplicant命名,IDA PDB为m_mpLeagueApplicant | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpDateLeagueMemberInfo | 0x140067cb0 | verified | IDA 0x140067cb0 + 遍历m_mpLeagueMember + GetLeagueMember + GetUser + GetLeagueMemberInfo + SetLeagueMember | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ChangeMemberName | 0x140067ef0 | verified | IDA 0x140067ef0 + GetLeagueMemberPtr+GetPosition+IsMaster/SetMasterName+position==7/SetSubLeagueMaster+SetName+LeagueMemberUpdate+SendChangeMemberName | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeMemberName | 0x1400682d0 | verified | IDA 0x1400682d0 + XSendPacket(0xF6,0x42)+stUpdate+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendNoticeToMember | 0x140068810 | verified | IDA 0x140068810 + XSendPacket(0xF6,0x23)+stNotice+dwActorID+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendRecordToMember | 0x140068c40 | verified | IDA 0x140068c40 + XSendPacket(0xF6,0x47)+stRecord+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeCardToMember | 0x140068cd0 | verified | IDA 0x140068cd0 + XSendPacket(0xF6,0x26)+stCard+m_nSyncCount+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLeagueWealthToMember | 0x140068d80 | verified | IDA 0x140068d80 + XSendPacket(0xF6,0x55)+dwUCID+stUpdate+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLevelupToMember | 0x140068e30 | verified | IDA 0x140068e30 + XSendPacket(0xF6,0x51)+nLeagueID+byLevel+bySkillPoint+psAutoSkill+m_nSyncCount+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLearnSkillToMember | 0x140068f30 | verified | IDA 0x140068f30 + XSendPacket(0xF6,0x53)+stSkill+m_nSyncCount+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::GetLeagueID | 0x140084850 | verified | IDA CUserObject::GetLeagueID 0x140084850 + return m_stCharInfo.stLeagueInfo.nLeagueID; CLeague::GetLeagueID为内联访问器m_stLeagueInfo.nLeagueID | 否 | 一致 |
| GameServer/XRelayServer | League.h | CLeagueMember::~CLeagueMember | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::CLeagueMember | 0x140405632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::Login | 0x140405680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::LogOut | 0x140405696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::SetPosition | 0x140405712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::SetEnrollBoardDate | 0x140405744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::SetPlayDate | 0x140405776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::GetExp | 0x140405808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::GetLeagueMember | 0x140405824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::SetName | 0x140405904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::GetPosition | 0x140406016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueInfo | 0x140406048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CLeague | 0x140406128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::~CLeague | 0x140406352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::Clear | 0x140406464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::GetName | 0x140406512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::ResetExp | 0x140406560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::SetLeagueMember | 0x140406592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::UpdateMemberInfo | 0x140406592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeagueMember::AddExp | 0x140406672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueNotice | 0x140406736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeaguePosition | 0x140406816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::LeagueMemberUpdate | 0x140407072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::IsMaster | 0x140407280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::AddMember | 0x140407328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetDelName | 0x140407776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::LoginMember | 0x140408064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::LogOutMember | 0x140408304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::AddApplicant | 0x140408480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::DelApplicant | 0x140408608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::AddBoard | 0x140408736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::IsMember | 0x140408832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CheckApplicantCount | 0x140408944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CheckExistApplicant | 0x140409040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetMemberBoardLimit | 0x140409152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetApplicantList | 0x140409376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetApplicant | 0x140409616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetBoardList | 0x140409728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetMemberList | 0x140409904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetLeagueMemberPtr | 0x140410192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueAuth | 0x140410384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CheckPositionCount | 0x140410464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueRecruitNotice | 0x140410928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::ChangeMemberPosition | 0x140411008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CheckLeagueDelegate | 0x140411584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::Delegate | 0x140411920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CheckLeagueCardChange | 0x140413408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::GetRecordList | 0x140413488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CardChange | 0x140413664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::HaveSkill | 0x140413712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CheckLearnSkill | 0x140413776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::LearnSkill | 0x140414336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::Levelup | 0x140415120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::ApplyLevelup | 0x140415904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CalculateExp | 0x140416640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::ApplyWealth | 0x140418208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::LoadRecord | 0x140419088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpdateRecord | 0x140419136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::ResetExp | 0x140419392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::ResetExpInitDate | 0x140419632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpdateApplyList | 0x140419872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueInfoForGame | 0x140420352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpdateSkillPoint_Cheat | 0x140420544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpdateSyncCount | 0x140420976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpdateInventorySyncCount | 0x140421008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpDateLeagueMemberInfo | 0x140421040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetSubLeagueMaster | 0x140421472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetMasterName | 0x140421520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueName | 0x140421568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::ChangeMemberName | 0x140421616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::UpdateApplicantName | 0x140422048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendChangeApplicantName | 0x140422464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendChangeMemberName | 0x140422608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendChangeLeagueName | 0x140422752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendWithdrawToMember | 0x140422944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendKickoutToMember | 0x140423184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendChangePositionToMember | 0x140423472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendDelegateToMember | 0x140423728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendNoticeToMember | 0x140423952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendLeagueInfo | 0x140424128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendRecordToMember | 0x140425024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendChangeCardToMember | 0x140425168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendLeagueWealthToMember | 0x140425344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendLevelupToMember | 0x140425520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendLearnSkillToMember | 0x140425776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::Levelup_Cheat | 0x140425952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::CompareSyncCount | 0x140426880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendSyncLeagueInfo | 0x140426944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendInventoryInfo | 0x140427968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | CLeague::SendInventoryMove | 0x140428288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | wcscpy_s<10> | 0x140446512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | ST_LEAGUE_INFO_EX::ST_LEAGUE_INFO_EX | 0x140465824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | XParse::operator<< | 0x140465888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | ST_LEAGUE_MEMBER_UPDATE::ST_LEAGUE_MEMBER_UPDATE | 0x140465936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | ST_LEAGUE_INFO_UPDATE::ST_LEAGUE_INFO_UPDATE | 0x140466064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | XResourceMgr::GetTB_LEAGUE_INFO | 0x140466576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | XResourceMgr::GetTB_LEAGUE_SKILL | 0x140467168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | ST_LEAGUE_RECORD::ST_LEAGUE_RECORD | 0x140467280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | ST_LEAGUE_INFO_FOR_GAME::ST_LEAGUE_INFO_FOR_GAME | 0x140467376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | League.h | XParse::SetSHORT | 0x140890096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::{构造, Clear} | 0x140073440 | verified | IDA 0x140073440/0x140073520 + 构造:初始化成员+Clear() / Clear:clear各容器+m_bLoadLeague/m_bLeague等标志位=0+m_tUpdate/m_tInitDate=GetTickCount+m_nLeagueListIndex=0 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::AddLeague | 0x1400737A0 | verified | IDA 0x1400737A0 + new CLeague+SetLeagueInfo+AddMember+insert(m_mpLeagueList)+push_back(m_vecLeagueList) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueLogin | 0x140073970 | verified | IDA 0x140073970 + find→LoginMember→SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueApplicant | 0x140073AC0 | verified | IDA 0x140073AC0 + find→AddApplicant+GetMemberList+XSendPacket(0xF6,0x19)给请求者+XSendPacket(0xF6,0x20)广播(SendPacketAll) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInfo | 0x140073CF0 | verified | IDA 0x140073CF0 + find(nLeagueID)→GetLeagueMemberPtr(dwActorID)→GetPosition→GetLeagueInfo/GetMemberList/GetApplicantList/GetBoardList/GetRecordList/SetLeagueInfoForGame→SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInfo | 0x140073cf0 | verified | IDA 0x140073cf0 + find→GetLeagueMemberPtr→GetPosition→GetLeagueInfo/MemberList/ApplicantList/BoardList/RecordList→SetLeagueInfoForGame→SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueDel | 0x140074D60 | verified | IDA 0x140074D60 + find→IsMaster→GetMemberCount<=1→SendDBGame(7,1) / 57018=成员仍存在 / 57015=非Master / 57016=联赛不存在 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResInviteUser | 0x140075c50 | verified | IDA 0x140075c50 + 查找联赛+GetUser+更新登录状态/频道+AddMember+SetLeagueInfoForGame+DelApplicant+DeleteApplicantList+UpdateLeagueInfo+SetMemberCount+创建加入记录(byFlag=1)+UpdateRecord+UpdateSyncCount+SendLeagueInfo+SendLeagueInviteJoin | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::OnUpdate | 0x14007b740 | verified | IDA 0x14007b740 + 每分钟定时器(CTimeSpan(0,0,1,0)) + 每日9点InitLeaguExp + 遍历UpdateApplyList + LoadGMTLeagueInfo+SendGMTLeagueInfo (修复：1秒→60秒间隔) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::InitLeaguExp | 0x14007bb00 | verified | IDA 0x14007bb00 + 遍历ResetExp(biTodayInit) + SendDBPacket(nullptr,7,0x40) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueMember | 0x14007ca50 | verified | IDA 0x14007ca50 + m_bMember=bLoad + 遍历league×member双层循环 + nLeagueID匹配AddMember + byPosition==7时SetSubLeagueMaster | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueApplicant | 0x14007ccb0 | verified | IDA 0x14007ccb0 + m_bApplicant=bLoad + 遍历league×applicant双层循环 + nLeagueID匹配AddApplicant | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueBoard | 0x14007ce90 | verified | IDA 0x14007ce90 + m_bBoard=bLoad + 遍历league×board双层循环 + nLeagueID匹配AddBoard | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueRecord | 0x14007d080 | verified | IDA 0x14007d080 + m_bRecord=bLoad + 遍历league×record双层循环 + nLeagueID匹配LoadRecord + SendInfoToGameServer | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendInfoToGameServer | 0x14007d200 | verified | IDA 0x14007d200 + 检查5个加载标志(m_bLeague/Member/Applicant/Board/Record)→m_bLoadLeague=1→SetCachingLoad(E_SERVER_CACHING_LOAD_LEAGUE) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateLeagueMemberInfo | 0x14007d270 | verified | IDA 0x14007d270 + 遍历league→LogError(2306)→UpDateLeagueMemberInfo+UpdateSyncCount →nSyncCount=1→SendPacketAll(0xF6,0x59) (修复：添加null错误日志) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueDelegate | 0x14007e630 | verified | IDA 0x14007e630 + find→LogError(2583)→nResult=57016→!bGMDelegate?CheckLeagueDelegate→SendDBPacket(7,0x32)/SendPacket(0xF6,7) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueCardChange | 0x14007ed40 | verified | IDA 0x14007ed40 + find→LogError(2694/2701)→CheckLeagueCardChange→SendDBPacket(7,0x16,nullptr)/SendPacket(0xF6,0x48) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueCardChange | 0x14007f090 | verified | IDA 0x14007f090 + find→GetLeagueMemberPtr→SetLeagueInfoForGame→CardChange+UpdateSyncCount+SendPacket(0xF6,0x48)+SendChangeCardToMember+UpdateRecord(byFlag=11) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqApplyLeagueExp | 0x14007f560 | verified | IDA 0x14007f560 + find→LogError(2811/2818)→CalculateExp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueLevelup | 0x14007f7d0 | verified | IDA 0x14007f7d0 + find→LogError(2849/2856)→UpdateSyncCount→ApplyLevelup→UpdateRecord(byFlag=8) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueLevelup | 0x140080010 | verified | IDA 0x140080010 + find→LogError(2982/2989)→Levelup_Cheat | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueMemberInitExp(nLeagueID, dwUCID) | 0x140080430 | verified | IDA 0x140080430 + 补充完整业务体：查找联赛 + ResetExpInitDate | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ChangeLeagueMemberName | 0x140081c70 | verified | IDA 0x140081c70 + find→pLeague→ChangeMemberName | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ChangeLeagueApplicant | 0x140081d60 | verified | IDA 0x140081d60 + 遍历stApplyList.vecApplyLeuage→find→UpdateApplicantName | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetLeagueID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | UXActorID::operator unsigned long | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | PS_GMT_LEAGUE_UPDATE_LIST::~PS_GMT_LEAGUE_UPDATE_LIST | 0x140266560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetLevel | 0x140467472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetAwaken | 0x140467504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetProfilePhoto | 0x140467536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetMapInfo | 0x140467568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::DelMember | 0x140467616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetMemberCount | 0x140467664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetLeagueNoticeDate | 0x140467696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetLeagueOpenOrNot | 0x140467728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetLeagueRecruitNoticeDate | 0x140467760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetMasterUCID | 0x140467792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetLeagueInfo | 0x140467808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetMemberCount | 0x140467888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetNoticeDate | 0x140467936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetRecruitNoticeDate | 0x140467968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetSyncCount | 0x140468000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::CLeagueManager | 0x140468032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::~CLeagueManager | 0x140468160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::Clear | 0x140468256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::GetMemberInfo | 0x140468432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::AddLeague | 0x140468896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueLogin | 0x140469360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueApplicant | 0x140469696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueInfo | 0x140470256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueWithDraw | 0x140471136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueWithdraw | 0x140471888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueKick | 0x140472608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueDel | 0x140474464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueBoard | 0x140475600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueBoard | 0x140476784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqInviteAccept | 0x140477344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResInviteUser | 0x140478288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::AppliCantJoinSucc | 0x140480048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::DeleteApplicantList | 0x140482016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueApplicantJoin | 0x140482400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueInviteJoin | 0x140482656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueInfo | 0x140482928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ApplicantRejectSucc | 0x140483232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::DelLeague | 0x140483872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueErrorMsg | 0x140484064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueErrorMsg | 0x140484240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueApplicantAccept | 0x140484528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueApplicantReject | 0x140486224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::AddInviteUser | 0x140487360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::CheckInviteUser | 0x140487568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::DeleteInviteUser | 0x140487696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::CheckLeagueInfo | 0x140487856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueApplicant | 0x140487952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::DeleteLeagueMember | 0x140488912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueInvite | 0x140489168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueNoticeChange | 0x140491504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueNoticeChange | 0x140493024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::CreateLeague | 0x140493504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResCreateLeague | 0x140494160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueChangeAuth | 0x140495072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueAuthChange | 0x140495728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeaguePositionNameChange | 0x140496272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeaguePositionNameChange | 0x140496896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueMessage | 0x140497424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueMemberPositionChange | 0x140497872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueMemberPositionChange | 0x140499776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::LogOutLeagueMember | 0x140500576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueApplicantDelete_TimeOver | 0x140501184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::OnUpdate | 0x140501568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | PS_GMT_LEAGUE_UPDATE_LIST::PS_GMT_LEAGUE_UPDATE_LIST | 0x140502320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendGMTLeagueInfo | 0x140502368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::InitLeaguExp | 0x140502528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateLeagueInfo | 0x140502784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendMemberUpdate | 0x140503264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateMemberLevel | 0x140503712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateMemberAwaken | 0x140504400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateMemberProfilePhoto | 0x140505088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateMemberMapInfo | 0x140505776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLoadLeagueMember | 0x140506448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLoadLeagueApplicant | 0x140507056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLoadLeagueBoard | 0x140507536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLoadLeagueRecord | 0x140508032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendInfoToGameServer | 0x140508416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateLeagueMemberInfo | 0x140508528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueMemberWithdraw | 0x140508928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendLeagueMemberKick | 0x140509584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueOpenOrNot | 0x140510304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueOpenOrNot | 0x140510816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueSearch | 0x140511520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueRecruitNotice | 0x140511824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueRecruitNotice | 0x140513152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueDelegate | 0x140513584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueDelegate | 0x140514512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueCardChange | 0x140515392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueCardChange | 0x140516240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqApplyLeagueExp | 0x140517456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResApplyLeagueWealth | 0x140517776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueLevelup | 0x140518096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueSkillLearn | 0x140518576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueSkillLearn | 0x140519472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueLevelup | 0x140520208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SendFailLeagueLogin | 0x140520480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueRecordUpdate | 0x140521008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueMemberExpInit | 0x140521264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueSkillPointUpdate | 0x140521520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::SyncLeagueInfo | 0x140521792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueInevntoryInfo | 0x140522128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueInventoryInfo | 0x140522416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueInventoryMove | 0x140523168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueInventoryMove | 0x140523504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::GetApplicantList | 0x140525008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueCreate | 0x140525392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeagueKickout | 0x140525776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueNameChange | 0x140526544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ResLeaugeNameChange | 0x140526976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ChangeLeagueMemberName | 0x140527472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ChangeLeagueApplicant | 0x140527712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::LoadLeagueInfo | 0x140528016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::UpdateGMTLeagueInfo | 0x140529808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | PS_LEAGUE_INFO_SUMMARY::PS_LEAGUE_INFO_SUMMARY | 0x140538288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CUserObject::SetLockLeague | 0x140538400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | PS_RES_LEAGUE_DELEGATE::PS_RES_LEAGUE_DELEGATE | 0x140538432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | PS_GMT_LEAGUE_UPDATE_LIST::PS_GMT_LEAGUE_UPDATE_LIST | 0x140538544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CUserObject::GetLeagueWithdrawPenalty | 0x140538672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CUserObject::GetLeagueID | 0x140538704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | ST_LEAGUE_INVITE_INFO::ST_LEAGUE_INVITE_INFO | 0x140538736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | CUserObject::GetChannel | 0x140538784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueManager.h | PS_RES_LEAGUE_ACCEPT_ACCPLICANT::PS_RES_LEAGUE_ACCEPT_ACCPLICANT | 0x140821536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::{ReqLeagueCreate, ReqLeagueDelete, ReqLeagueApplicant, ReqLeagueInfo, ReqLeagueDelegate, ReqLeagueWithDraw, ReqLeagueKick, ReqLeagueInvite, ReqLeagueInviteAccept, ReqLeagueInviteReject, ReqLeagueBoard, ReqLeagueApplicantAccept, ReqLeagueApplicantReject, ReqLeagueSearch, ReqLeagueNoticeChange, ReqLeagueList, ReqLeagueNameChange, ReqLeagueCardChange, ReqLeaguePositionNameChange, ReqLeagueAuthChange, ReqLeagueMessage, ReqLeagueMemberPositionChange, ReqLeagueMemberLogOut, ReqLeagueDeletePenalty, ReqLeagueWithdrawPenalty, ReqLeagueOpenOrNot, ReqLeagueRecruitNotice, ReqLeagueRecordUpdate, ReqLeagueLevelup, ReqLeagueSkillLearn, ReqLeagueWealth, ReqLeagueMemberInitExp, ReqLeagueSkillPointUpdate, ReqSyncLeagueInfo, ReqLeagueInventoryInfo, ReqLeagueInventoryMove} | - | verified | IDA decompile 36 handlers全部逐个验证; 详见各行313-226和708-733等独立条目 | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueNoticeChange | 0x140084dd0 | verified | IDA 0x140084dd0 + ST_LEAGUE_NOTICE>>dwActorID + GetClientPtr + DoJob(1) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueInviteReject | 0x140085370 | verified | IDA 0x140085370 + ST_REQ_LEAGUE_INVITE_REJECT>>packet + GetClientPtr + DoJob(1,lambda) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::{ReqLeagueCreate, ReqLeagueDelete, ReqLeagueApplicant} | 0x140085610 | verified | IDA 0x140085610/0x1400858A0/0x140085A30 + Create:PS_LEAGUE_CREATE_FOR_SERVER>>+DoJob(1) / Delete:dwActorID>>nLeagueID>>biPenalty+DoJob(1) / Applicant:ST_LEAGUE_APPLICANT>>+CTime+DoJob(1) | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueApplicant | 0x140085a30 | verified | IDA 0x140085a30 + stApplicant>>GetClientPtr + biApplicantDate=GetTickCount + DoJob→ReqLeagueApplicant | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueCardChange | 0x140087860 | verified | IDA 0x140087860 + PS_REQ_LEAGUE_CARD>>dwUCID>>PS_RES_STORAGE_INFO + GetClientPtr + DoJob→ReqLeagueCardChange | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeaguePositionNameChange | 0x140087c70 | verified | IDA 0x140087c70 + ST_LEAGUE_POSITION_NAME_CHANGE>>dwActorID>>nLeagueID + GetClientPtr + DoJob(1) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueMessage | 0x140087e50 | verified | IDA 0x140087e50 + 读取 PS_CHAT_LEAGUE + PS_CHAT_ITEM_LINK_FOR_SERVER + worker-1 dispatch | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueMemberPositionChange | 0x140088310 | verified | IDA 0x140088310 + ST_LEAGUE_MEMBER_POSITION>>nLeagueID>>dwActorID + GetClientPtr + DoJob(1) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::{ReqLeagueWithdrawPenalty, ReqLeagueDeletePenalty} | 0x140088560 | verified | IDA 0x140088560/0x1400886e0 + XParse>>dwUCID>>biPenalty + DoJob(1,lambda→GetUser+SetLeague*Penalty) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueRecruitNotice | 0x140088a20 | verified | IDA 0x140088a20 + dwUCID>>ST_LEAGUE_RECRUIT_NOTICE + GetClientPtr + DoJob(1) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueDelegate | 0x140088c60 | verified | IDA 0x140088c60 + PS_REQ_LEAGUE_DELEGATE>>dwUCID>>bGMDelegate + GetClientPtr(null检查) + DoJob(1) | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueMemberInitExp | 0x140089680 | verified | IDA 0x140089680 + 读取 nLeagueID + dwUCID + worker-1 dispatch | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueInventoryInfo | 0x140089880 | verified | IDA 0x140089880 + 读取 PS_REQ_LEAGUE_INVEN_INFO + dwReqUCID + worker-1 dispatch | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueInventoryInfo lambda | 0x1400899c0 | verified | IDA 0x1400899c0 + qmemcpy(0xC)+ReqLeagueInevntoryInfo(dwReqUCID,stReq); 源码正确传递dwReqUCID | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.cpp | CLeagueProcess::ReqLeagueInventoryMove | 0x140089a10 | verified | IDA 0x140089a10 + 先读 dwReqUCID 再读 PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME + worker-1 dispatch | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::Parse | 0x1400849B0 | verified | IDA 0x1400849B0 + 36 subcommand switch完全匹配IDA (与行287重复,已验证) | 否 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::CLeagueProcess | 0x140538816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::~CLeagueProcess | 0x140539008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::Parse | 0x140539056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueNoticeChange | 0x140540112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueSearch | 0x140540672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueInviteReject | 0x140541552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueCreate | 0x140542224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueDelete | 0x140542880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueApplicant | 0x140543280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueInfo | 0x140544272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueWithDraw | 0x140544704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueKick | 0x140545232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueAuthChange | 0x140545936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueInvite | 0x140546544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueInviteAccept | 0x140547408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueBoard | 0x140547920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueApplicantAccept | 0x140548608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueApplicantReject | 0x140549152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueList | 0x140549488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueNameChange | 0x140550048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueCardChange | 0x140551008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeaguePositionNameChange | 0x140551888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueMessage | 0x140552528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueMemberPositionChange | 0x140553456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueMemberLogOut | 0x140554016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueWithdrawPenalty | 0x140554336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueDeletePenalty | 0x140554720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueOpenOrNot | 0x140555104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueRecruitNotice | 0x140555552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueDelegate | 0x140556128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueWealth | 0x140556688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueSkillLearn | 0x140556960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueRecordUpdate | 0x140557568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueLevelup | 0x140557968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueSkillPointUpdate | 0x140558384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueMemberInitExp | 0x140558720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqSyncLeagueInfo | 0x140558976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueInventoryInfo | 0x140559232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::ReqLeagueInventoryMove | 0x140559632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | PS_REQ_LEAGUE_INVEN_INFO::PS_REQ_LEAGUE_INVEN_INFO | 0x140600272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | PS_SYNC_LEAGUE_INFO::PS_SYNC_LEAGUE_INFO | 0x140600336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | CUserObject::IsLockLeague | 0x140600400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | PS_REQ_LEAGUE_SKILL::PS_REQ_LEAGUE_SKILL | 0x140600432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | ST_REQ_LEAGUE_INVITE::ST_REQ_LEAGUE_INVITE | 0x140600496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | ST_REQ_LEAGUE_INVITE_REJECT::ST_REQ_LEAGUE_INVITE_REJECT | 0x140600592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | LeagueProcess.h | ST_REQ_LEAGUE_SEARCH::ST_REQ_LEAGUE_SEARCH | 0x140600656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::ExitMatching | 0x140032D20 | verified | IDA 0x140032D20 + m_byProcess!=1→false + 查找dwExitUAID + SendMatchingExit + SendDBLog(28,2,matchingID,mapID,0,0,reason) + if empty: SetMatchingState(2)+m_byProcess=0 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::{OnUpdate, MatchingPossible, MatchingWait} | 0x140033980 | verified | IDA `0x140033980 / 0x140032F80 / 0x140032FA0` + OnUpdate:process==0→LogError+SendMatchingExit/process==2→return/process==1+tick→wait/m_state==0→MatchingPossible/m_state==1→MatchingWait; MatchingPossible:SendMatchingWait+return1; MatchingWait:MakeOperationMaze | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::MakeOperationMaze | 0x140033AA0 | verified | IDA 0x140033AA0 + GetServerContents(E_SERVER_OPTION_OPERATION_MAZE)检查 + TB_OPERATION_INFO查找 + User_Start_Point_1..8跳点收集 + std::shuffle随机化 + 成员跳点分配 + map<serverID,short>统计 + MasterServerID选择; 修复:std::random_shuffle(C++17已移除)→std::shuffle | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::SendCreateMatchingModeMaze | 0x140034170 | verified | IDA 0x140034170 + TB_OPERATION_INFO查找获取Reward_Attend + SendDBLog(28,4) + per-member SendPacket(0xFD,0x06) + GetPartyUser+SetMatchingState(0)+SetMatchingID(0,0) + LogInfo格式对齐; 修复:添加完整DB日志和状态清理 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | XOption::GetGroupID | 0x140110528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::CModeMazeMatching | 0x140203312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::~CModeMazeMatching | 0x140203488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::AutoMatchingCreate | 0x140203520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::AutoMatchingEnter | 0x140203856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::ExitMatching | 0x140204064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::MatchingPossible | 0x140204672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::MatchingWait | 0x140204704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SendMatchingExit | 0x140204736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SendMatchingWait | 0x140206176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | PS_MODE_MAZE_MATCHING_WAIT::~PS_MODE_MAZE_MATCHING_WAIT | 0x140207200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::OnUpdate | 0x140207232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::MakeOperationMaze | 0x140207520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | ST_CREATE_MODE_MAZE::~ST_CREATE_MODE_MAZE | 0x140209216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SendCreateMatchingModeMaze | 0x140209264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::GetMatchingMember | 0x140210304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | ST_MODE_MAZE_MEMBER_INFO::operator= | 0x140218656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | ST_CREATE_MODE_MAZE::ST_CREATE_MODE_MAZE | 0x140219824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | PS_MODE_MAZE_MATCHING_WAIT::PS_MODE_MAZE_MATCHING_WAIT | 0x140220256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | PS_MODE_MAZE_MATCHING_EXIT::PS_MODE_MAZE_MATCHING_EXIT | 0x140220336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatchginMember::GetUCID | 0x140220384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | XResourceMgr::GetTB_MAZE_INFO | 0x140220848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SetMatchingState | 0x140220960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatching.h | UXMapID::operator __int64 | 0x140271792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::{FindModeMazeMatching, CheckModeMazeOpenTime} | 0x140037D20 | verified | IDA `0x140037D20 / 0x1400373B0` + FindModeMazeMatching:m_mapMatchingWait.find+return; CheckModeMazeOpenTime:TB_OPERATION_INFO+m_wModeMazeID/m_wMaxEnterCount/m_wMinEnterCount+状态检查+HotTime时间窗口 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::ProcessWaitList | 0x140037FF0 | verified | IDA 0x140037FF0 + waitCount<min→MAZE_DESTROY / 按Rank排序 / 计算匹配数量(整数除法+余数) / >=40成员时取前40排名+shuffle+优先分配 / 创建匹配+AutoMatchingEnter+InsertMatchingInfo / m_mapMatchingWait.clear() | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::ExitMatching | 0x1400395C0 | verified | IDA 0x1400395C0 + GetPartyUser+GetRewardState!=0+GetMatchingState==3+find matching+ExitMatching(原因=3)+find wait+send packet(0xFD,3)+SendDBLog(28,2)+SetMatchingState(false)+SetMatchingID(0,0) | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::MatchingRemoveUser | 0x140039A00 | verified | IDA 0x140039A00 + PS_MODE_MAZE_MATCHING_EXIT填充+ExitMatching调用 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::CModeMazeMatchingMgr | 0x140220992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::~CModeMazeMatchingMgr | 0x140221248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::OnUpdate | 0x140221424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::CheckModeMazeOpenTime | 0x140222128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::FindModeMazeMatching | 0x140224544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::AddModeMazeMatchingWait | 0x140224656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ProcessWaitList | 0x140225264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ProcessMazeMake | 0x140227136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::DestroyMatchingWait | 0x140227744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::EnterMatching | 0x140229808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ExitMatching | 0x140230848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::MatchingRemoveUser | 0x140231936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::SendCreateMatchingModeMaze | 0x140232000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ModeMazeTime_Cheat | 0x140232240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ModeMazeMatchingEvent | 0x140232544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::GetRank | 0x140244000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetName | 0x140244016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetMapIns | 0x140244128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetProfilePhoto | 0x140244176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetAwaken | 0x140244192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetLevel | 0x140244208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetClass | 0x140244224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO::PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO | 0x140244240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | PS_MODE_MAZE_MATCHING_ENTER_RES::PS_MODE_MAZE_MATCHING_ENTER_RES | 0x140244336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMemberCount | 0x140244384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::SetRank | 0x140244416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | ST_MODE_MAZE_MEMBER_INFO::ST_MODE_MAZE_MEMBER_INFO | 0x140244480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::Clear | 0x140244640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::CModeMazeMatchginMember | 0x140244704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::SetMatchingState | 0x140244928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMatchingID | 0x140244960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMatchingProcess | 0x140244976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMatchingState | 0x140244992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::~CModeMazeMatchginMember | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetEventID | 0x140467792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::CMonitorProcess | 0x140600704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::~CMonitorProcess | 0x140600896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::Parse | 0x140600944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::ReqMonitorServerInfoAll | 0x140601024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ObserveSocket.cpp | CObserveSocket::{OnConnect, OnDisConnect, OnNotConnect} | 0x14013F0B0 | verified | IDA 0x14013F0B0/0x14013F0D0/0x14013F0F0 + OnConnect:m_nSyncServerData=0+XPRINT / OnDisConnect:空操作 / OnNotConnect:空操作 | 否 | 一致 |
| GameServer/XRelayServer | ObserveSocket.cpp | CObserveSocket::{StartUp, SetMyInfo} | 0x14013F110 | verified | IDA 0x14013F110/0x14013F1C0 + StartUp:SetMyInfo+Init_2+Connect / SetMyInfo:m_myInfo从pOption拷贝+GetAgentPrivateIPAndPort | 否 | 一致 |
| GameServer/XRelayServer | ObserveSocket.cpp | CObserveSocket::OnUpdate | 0x14013F9B0 | verified | IDA 0x14013F9B0 + m_bActivate检查+首次初始化tick+断线重连10s间隔+3s定时SendReportServerStatus | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::{AddMember, GetUserCount, GetMasterID, GetPartyID} | - | verified | bounded party-member index + map container | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::{RemoveMember, ChangeMaster, FindNewMaster, Kickout} | 0x140094 | verified | IDA 0x140094xxx + RemoveMember:erase / ChangeMaster:m_dwMasterID=dwNewMasterID / FindNewMaster:遍历返回非队长 / Kickout:RemoveMember | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::ChangeMaster(uint32, bool) | 0x1400942B0 | verified | export-for-ai 0x1400942B0 + find + bLeave检查在线 + 设置m_dwMasterID | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::Kickout(uint32) | 0x140094360 | verified | export-for-ai 0x140094360 + find + erase | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::FindNewMaster() | 0x1400943E0 | verified | export-for-ai 0x1400943E0 + 遍历map + return第一个非master的memberID | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::GetPartyMemberList(ST_PARTY_MEMBER_LIST&) | 0x1400946F0 | verified | export-for-ai 0x1400946F0 + 遍历map + push_back成员信息 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::GetPartyInfo | 0x1400976f4 | verified | RelayServer decompile `0x1400976f4` + member-list aggregation for `PS_PARTY_INFO` broadcast | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::~CParty | 0x140074480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::SetMemberInfo | 0x140075168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::SetMemberInfo | 0x140075424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::SetMemberInfo | 0x140079424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | ST_PARTY_MEMBER::operator= | 0x140079472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::Logout | 0x140079776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::GetMemberInfo | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::Clear | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::~CPartyMember | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::CParty | 0x140601088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::CParty | 0x140601808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::SetPartyInfo | 0x140601872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::AddMember | 0x140602512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::ChangeMaster | 0x140602800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::Kickout | 0x140602976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::FindNewMaster | 0x140603104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::GetPartyInfo | 0x140603296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::GetMemberInfo | 0x140603552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::SetMemberEnterMap | 0x140603728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::GetPartyMemberList | 0x140603888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CParty::SendNameChange | 0x140604192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::SetEnterMap | 0x140607280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::Login | 0x140607328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | Party.h | CPartyMember::CPartyMember | 0x140607360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqInviteParty | 0x140095A00 | verified | IDA 0x140095A00 + GetUser(strName)→null(53011)/GetPartyUser→GetRewardState | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqAcceptParty | 0x140096130 | verified | IDA 0x140096130 + m_mapPartyInvite查找+邀请验证+队长队伍处理+ReqJoinMember/ReqCreateParty | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqCancelParty | 0x140096E00 | verified | IDA 0x140096E00 + m_mapPartyInvite查找+通知队长(0xF4/0x13)+移除邀请 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::{ReqJoinMember, ReqCreateParty} | 0x1400972C0 | verified | RelayServer decompile `0x1400972C0 / 0x140095690` + `main=4/sub=2,1` DB game packet + DBLog 22/5 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::{GetParty, GetPartyID, AddPartyMember, DeleteParty} | 0x140097460 | verified | RelayServer decompile `0x140097460 / 0x140097540 / 0x140097580 / 0x1400975B0` + `m_mapParty`/`m_mapPartyUser` index | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResJoinMember | 0x140097630 | verified | RelayServer decompile `0x140097630` + `0xF4/0x02` fanout + `AddRecruitMember | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqLeaveMember | 0x140097830 | verified | IDA 0x140097830 + GetUserCount<=2解散+队长离开选新队长+RemoveMember+Kickout+DB日志 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqChangeMaster | 0x140097E50 | verified | IDA 0x140097E50 + m_mapParty查找+ChangeMaster+DB(4/5)+错误包(0xF4/4) | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqDeleteParty | 0x140098280 | verified | IDA 0x140098280 + DeleteParty+DB(4/6)+日志(22/7,22/8,22/13) | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::SendPartyErrorAccept | 0x140098D20 | verified | IDA 0x140098D20 + PS_RES_PARTY_ACCEPT(8字节)+SendEx | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::CreatePartyMatching | 0x140098DD0 | verified | IDA 0x140098DD0 + new CParty + SetPartyInfo + m_mapParty.insert + 遍历vecPartyMember→AddPartyMember | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResRecruitAccept | 0x1400995A0 | verified | RelayServer decompile `0x1400995A0` + party/force branching + DB send + error codes | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CPartyRecruit::GetPartyID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::CPartyManager | 0x140079824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::~ClassFactory<CParty,64> | 0x140079952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::~CPartyManager | 0x140079984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | PS_RES_PARTY_ENTER_SERVER::PS_RES_PARTY_ENTER_SERVER | 0x140087696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | PS_RES_PARTY_ENTER_SERVER::~PS_RES_PARTY_ENTER_SERVER | 0x140087760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::ClassFactory<CParty,64> | 0x140096192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::destroy | 0x140099360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CParty::GetMazeID | 0x140108768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CParty::GetPartyType | 0x140108928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CParty::SetMazeID | 0x140109936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CParty::GetUserCount | 0x140110496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CParty::GetMasterID | 0x140110528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::Update | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::GetParty | 0x140607536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::GetParty | 0x140607696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::GetPartyID | 0x140607776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqCreateParty | 0x140607888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::CreateParty | 0x140608096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqInviteParty | 0x140608768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqAcceptParty | 0x140610608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqCancelParty | 0x140613888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::EnterServer | 0x140614320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqJoinMember | 0x140615104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResJoinMember | 0x140615984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqPartyLeave | 0x140616496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResPartyLeave | 0x140617872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqChangeMaster | 0x140618064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResChangeMaster | 0x140618608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::DeleteParty | 0x140618800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqDeleteParty | 0x140619136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResDeleteParty | 0x140620032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResLoadPartyAll | 0x140620448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqUpdateMemberInfo | 0x140620944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResUpdateMemberInfo | 0x140621296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::SendPartyMessage | 0x140621440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::SendPartyErrorInvite | 0x140621632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::SendPartyErrorAccept | 0x140621856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::CreatePartyMatching | 0x140622032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ReqPartyMazeClear | 0x140622528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::AddPartyMember | 0x140622944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::RemovePartyMember | 0x140623136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::IsParty | 0x140623280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::SetMaze | 0x140623392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::SendPartyNameChange | 0x140623872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResRecruitAccept | 0x140624032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::create | 0x140626864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | CParty::SetPartyType | 0x140632672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyManager.h | ST_INVITE_INFO::ST_INVITE_INFO | 0x140632704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatchginMember::CPartyMatchginMember | 0x140129760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatchginMember::~CPartyMatchginMember | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::CPartyMatching | 0x140632752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::~CPartyMatching | 0x140632832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::Init | 0x140632880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingEnter | 0x140633088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingExit | 0x140633632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingCreate | 0x140633888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingAccept | 0x140634320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::LeaderSelect | 0x140634592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingInfo | 0x140634928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingExit | 0x140635552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingStart | 0x140636752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::CheckFullUser | 0x140637280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendCreateMatchingMaze | 0x140637376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingCheck | 0x140638464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingWait | 0x140638688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::OnUpdate | 0x140639056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::MatchingPossible | 0x140639392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::MatchingCheck | 0x140639696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::MatchingWait | 0x140640016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::CreateMazeMatching | 0x140640048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | ST_MATCHING_INFO::ST_MATCHING_INFO | 0x140640768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | XResourceMgr::GetTB_COMMON | 0x140641296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SetMatchingState | 0x140641408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatchginMember::Clear | 0x140641440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::{OnUpdate, MatchingPossible, MatchingCheck, MatchingWait, SendMatchingCheck, SendMatchingWait, SendMatchingStart, SendMatchingExit, AutoMatchingExit, AutoMatchingAccept, LeaderSelect} | 0x14009D050 | verified | IDA `0x14009D050 / 0x14009D1A0 / 0x14009D2D0 / 0x14009D410 / 0x14009CE00 / 0x14009CEE0 / 0x14009C750 / 0x14009C2A0 / 0x14009BB20 / 0x14009BDD0 / 0x14009BEE0` + OnUpdate:process==0→LogError+exit/process==2→return/tick→wait/state==0→MatchingPossible/state==1→MatchingCheck/state==2→MatchingWait; MatchingPossible:count<2→exit/count>=2+limitCheck→SendMatchingCheck+state(1)+10000ms; MatchingCheck:遍历m_bCheck==10→bMazeEnter=false+exit/LeaderSelect+SendMatchingWait; SendMatchingWait:GetTB_COMMON(0x7532)*1000ms+state(2)+0xF4/0x24包; SendMatchingStart:process=2+countCheck+DB(4,0x13); SendMatchingExit:双循环(发送+清理)+SendDBLog(22,11或10); 差异:MatchingCheck源码缺少nValueLevel累加(未使用); SendMatchingWait源码用GetPartyMatchingConfig封装 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::CreateMazeMatching | 0x14009d430 | verified | IDA 0x14009d430 + PS_PARTY_INFO/ST_CREATE_MAZE构造+CreatePartyMatching+0xF2/0x43控制包发送 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchginMember::Clear | 0x14009D9A0 | verified | IDA 0x14009D9A0 + m_pCurServer=nullptr + m_nExp=0 + m_nState=0 + memset(&m_stMemberInfo,0); 源码顺序略有不同(m_stMemberInfo={}先执行)但逻辑等效 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::{OnUpdate, FindRecruitID, FindRecruitPtr, GetPartyRecruitInfo, ReqRecruitAccept, ReqRecruitReject} | 0x14009E000 | verified | IDA `0x14009E000 / 0x14009EF00 / 0x14009EE50 / 0x14009EF70 / 0x14009F220 / 0x14009F330` + OnUpdate:delMatchingIDs队列+recruit过期检查+ST_PARTY_RECRUIT_DEL_LIST+SendPacketAll(0xF4,0x26); FindRecruitID:m_mpRecruitUser.find→return; FindRecruitPtr:m_mpRecruit.find→return shared_ptr; GetPartyRecruitInfo:FindRecruitID+FindRecruitPtr+GetRecruitInfo; ReqRecruitAccept:FindRecruitPtr+masterCheck+partyGroupTypeCheck+RecruitAccept; ReqRecruitReject:FindRecruitPtr+DelApplyMember | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::ResPartyMatchingCreate | 0x14009e400 | verified | IDA 0x14009e400 + find(dwMatchingID)→CreateMazeMatching(dwPartyID) | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::ReqPartyRecruitCreate | 0x14009E5C0 | verified | IDA 0x14009E5C0 + SetRecruitInfo(3参数:m_dwRecruitID,dwMasterID,stRecruit)+nRemainTime=1800+insert(m_mpRecruit)+byPartyGroupType分支:1=GetParty遍历成员/SetRecruitDate/AddRecruitMember,2=GetForce遍历成员+最后m_dwRecruitID++; 修复:添加完整byPartyGroupType分支和Party/Force成员遍历 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::ReqPartyRecruitDel | 0x14009EC90 | verified | IDA 0x14009EC90 + FindRecruitID+FindRecruitPtr+masterCheck+GetPartyUser+SetRecruitPenalty+ClearRecruitDate(非DeletePartyRecruit); 修复:改为部分清理逻辑,不调用DeletePartyRecruit | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::{DeletePartyRecruit, ReqPartyRecruitApply, SendPartyRecruitMyApplyList, SendPartyRecruitApplyList, SendPartyRecruitApplyInfo} | 0x14009ED70 | verified | RelayServer decompile `0x14009ED70 / 0x1400A52BA / 0x1400A5768 / 0x1400A668E / 0x1400A6D40` + bounded recruit apply + accept-reject + apply-info manager slice; SendPartyRecruitApplyInfo调用新的GetPartyMemberList签名 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::SendPartyRecruitList | 0x14009F3A0 | verified | IDA 0x14009F3A0 + GetCurDateSec+biCurDate<=GetRecruitDate过滤+nRemainTime=GetRecruitDate-biCurDate+size>=30时bLast=0发送+清空+bLast=1发送剩余; 修复:添加过期过滤、剩余时间计算、分批发送逻辑 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::AddRecruitMember | 0x14009F980 | verified | RelayServer decompile `0x14009F980` + `AddMember` + `m_mpRecruitUser` insert | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyRecruit::GetRecruitDate | 0x140014976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_DEL_LIST::ST_PARTY_RECRUIT_DEL_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_LIST::ST_PARTY_RECRUIT_LIST | 0x140401744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::CPartyMatchingMgr | 0x140641520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::~CPartyMatchingMgr | 0x140641616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::CreateMatching | 0x140641744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::EnterMatching | 0x140642144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ExitMatching | 0x140642480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::CheckMatching | 0x140642672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::OnUpdate | 0x140643072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_DEL_LIST::~ST_PARTY_RECRUIT_DEL_LIST | 0x140644064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ResPartyMatchingCreate | 0x140644096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::SendCreateMatchingMaze | 0x140644224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ReqPartyRecruitCreate | 0x140644544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ReqPartyRecruitDel | 0x140646288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ReqPartyRecruitApply | 0x140646512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::FindRecruitPtr | 0x140646736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::FindRecruitID | 0x140646912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::GetPartyRecruitInfo | 0x140647024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::DeletePartyRecruit | 0x140647184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ClearRecruitDate | 0x140647584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ReqRecruitAccept | 0x140647712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::ReqRecruitReject | 0x140647984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::SendPartyRecruitList | 0x140648096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_LIST::~ST_PARTY_RECRUIT_LIST | 0x140648736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::MatchingRemoveUser | 0x140648768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::CreateParty | 0x140648832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::CreateForce | 0x140649216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::AddRecruitMember | 0x140649600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatchingMgr::DeleteRecruitMember | 0x140649760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyRecruit::GetRecruitInfo | 0x140657872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyRecruit::GetPartyGroupType | 0x140658384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CUserPartyInfo::SetRecruitDate | 0x140658400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_DEL::ST_PARTY_RECRUIT_DEL | 0x140658432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatching::GetMatchingID | 0x140658480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::Parse | 0x1400A1D40 | verified | IDA 0x1400A1D40 + 当前源码保持真实业务分发分支 `0x01/0x03/0x04/0x05/0x10/0x11/0x12/0x13/0x14/0x20/0x21/0x22/0x25-0x2D/0x2F/0x30/0x32/0x40/0x43`；IDA 额外 `case 0x09` 跳到 `0x1400CF1E0`，符号名落成 `boost::multi_index::...::modify_`，函数体仅 `return 1` 且被多处共享引用，按 no-op/占位分支处理，不单独恢复业务 handler | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::SyncPartyMessage | 0x1400A2050 | verified | IDA 0x1400A2050 + PS_CHAT_PARTY+PS_CHAT_ITEM_LINK_FOR_SERVER>> + DoJob(0) + SendPartyMessage | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyCreate | 0x1400A22B0 | verified | IDA 0x1400A22B0 + PS_REQ_PARTY_CREATE>> + DoJob(0) + ReqCreateParty | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyAccept | 0x1400A28D0 | verified | IDA 0x1400A28D0 + PS_RES_PARTY_INVITE(12字节)>> + DoJob(0) + ReqAcceptParty | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyCancel | 0x1400A2A40 | verified | IDA 0x1400A2A40 + PS_PARTY_REJECT>> + DoJob(0) + ReqCancelParty | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyLeaveMember | 0x1400A2C00 | verified | IDA 0x1400A2C00 + PS_PARTY_LEAVE>> + DoJob(0) + ReqLeaveMember | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyChangeMaster | 0x1400A2FD0 | verified | IDA 0x1400A2FD0 + PS_PARTY_CHANGE_MASTER>> + DoJob(0) + ReqChangeMaster | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::{ReqPartyUpdateMember, ReqPartyEnterServer, ReqPartyInfo} | 0x1400A32E0 | verified | IDA 0x1400A32E0/0x1400A3450/0x1400A35F0 + ST_UPDATE_PARTY_MEMBER>>DoJob(0) / PS_REQ_PARTY_ENTER_SERVER>>DoJob(0) / XParse>>dwPartyID>>dwActorID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMazeClear | 0x1400A3810 | verified | IDA 0x1400A3810 + dwPartyID+byClearFail>> + DoJob(0) + ReqMazeClear | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMatchingEnter | 0x1400A39A0 | verified | IDA 0x1400A39A0 + ST_PARTY_MEMBER+nExp+ST_CREATE_MAZE+dwUAID+nState>> + DoJob(0) + EnterMatching/CreateMatching | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMatchingExit | 0x1400A43B0 | verified | IDA 0x1400A43B0 + dwActorID+byReason+dwUAID>> + DoJob(0) + ExitMatching | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMatchingCheck | 0x1400A4600 | verified | IDA 0x1400A4600 + dwActorID+byCheck+dwUAID>> + DoJob(0) + CheckMatching | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::{ReqPartyRecruitAdd, ReqPartyRecruitDel} | 0x1400A47F0 | verified | IDA 0x1400A47F0/0x1400A5520 + PS_SERVER_PARTY_RECRUIT_ADD_REQ>>DoJob(0) / XParse>>dwActorID>>dwUAID>>byLevel+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::{ReqPartyRecruitApply, ReqPartyRecruitApplyAccept, ReqPartyRecruitApplyReject} | 0x1400A5900 | verified | IDA 0x1400A5900/0x1400A6010/0x1400A62D0 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::{ReqPartyRecruitList, ReqPartyRecruitMyApplyList, ReqPartyRecruitApplyList} | 0x1400A6560 | verified | IDA 0x1400A6560/0x1400A66A0/0x1400A68C0 + XParse>>dwActorID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::{ReqPartyRecruitApplyDel, ReqPartyRecruitApplyInfo} | 0x1400A6BB0 | verified | IDA 0x1400A6BB0/0x1400A6D40 + XParse>>dwActorID>>recruitID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ResPartyRecruitApplyAcceptCheck | 0x1400A7040 | verified | RelayServer decompile `0x1400A7040` + `CLogicThreadManager::DoJob(0)` + `CPartyManager::ResRecruitAccept | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_REQ_PARTY_ENTER_SERVER::PS_REQ_PARTY_ENTER_SERVER | 0x140145936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_MEMBER_LIST::ST_PARTY_MEMBER_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::CPartyProcess | 0x140658512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::~CPartyProcess | 0x140658704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::Parse | 0x140658752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::SyncPartyMessage | 0x140659536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyCreate | 0x140660144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyInvite | 0x140660640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyAccept | 0x140661520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyCancel | 0x140662080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyLeaveMember | 0x140662528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyChangeMaster | 0x140663504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyUpdateMember | 0x140664288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyEnterServer | 0x140664656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyInfo | 0x140665072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyMazeClear | 0x140665616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyMatchingEnter | 0x140666016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyMatchingExit | 0x140668592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyMatchingCheck | 0x140669184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitAdd | 0x140669680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_ADD_RES::~PS_SERVER_PARTY_RECRUIT_ADD_RES | 0x140673024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitDel | 0x140673056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitApply | 0x140674048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitApplyAccept | 0x140675856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitApplyReject | 0x140676560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitList | 0x140677216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitMyApplyList | 0x140677536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitApplyList | 0x140678080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_APPLY_MEMBER_LIST::ST_APPLY_MEMBER_LIST | 0x140678784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitApplyDel | 0x140678832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyRecruitApplyInfo | 0x140679232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY_INFO::~ST_PARTY_RECRUIT_APPLY_INFO | 0x140679968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ResPartyRecruitApplyAcceptCheck | 0x140680000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK::PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK | 0x140707472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_MEMBER_LIST::~ST_PARTY_MEMBER_LIST | 0x140707568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY_INFO::ST_PARTY_RECRUIT_APPLY_INFO | 0x140707600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT::ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT | 0x140707696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_APPLY_MEMBER::ST_APPLY_MEMBER | 0x140707744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY::ST_PARTY_RECRUIT_APPLY | 0x140707792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_ADD_RES::PS_SERVER_PARTY_RECRUIT_ADD_RES | 0x140707824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_ADD_REQ::PS_SERVER_PARTY_RECRUIT_ADD_REQ | 0x140707920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | PS_PARTY_REJECT::PS_PARTY_REJECT | 0x140707984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | PS_REQ_PARTY_INVITE::PS_REQ_PARTY_INVITE | 0x140708048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyUpdateInfo | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::{GetRecruitInfo, RecruitApply} | 0x1400AE0B0 | verified | IDA `0x1400AE0B0 / 0x1400AE120` + GetRecruitInfo:qmemcpy(0x74)+GetMember; 源码用字段赋值替代原始拷贝,逻辑等效; RecruitApply:GetUser+dateCheck+levelCheck+duplicateCheck(10)+emptyIndex+copy+SendPacket(0xF4,0x31); 差异:IDA发送GetMatchingID,源码发送masterActorID | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::{SetRecruitDate, GetMember, RecruitAccept, DelApplyMember} | 0x1400AEE10 | verified | IDA `0x1400AEE10 / 0x1400AE020 / 0x1400AE570 / 0x1400AEAF0` + SetRecruitDate:GetCurDateSec+1800; GetMember:遍历m_setMember+push_back; RecruitAccept:dateCheck+findApplicant+GetPartyUser/GetUser+memberLimit(4/8)+PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK+SendPacket(0xF4,0x32); DelApplyMember:遍历10+master包(0xF4,0x2F)+bSend分支+memset; 差异:RecruitAccept IDA用GetMatchingID,源码用GetCID | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::{ClearRecruitDate, ApplyMemberClear, GetPartyMemberList, GetApplyCount, IsApplied, SetRecruitInfo, AddMember, RemoveMember, SendApplyUserList} | 0x1400AEE40 | verified | IDA `0x1400AEE40 / 0x1400AE4A0 / 0x1400AF420 / 0x1400AEAF0 / 0x1400ADEE0 / 0x1400ADF30 / 0x1400ADF80 / 0x1400ADFD0 / 0x1400AEF10` + ApplyMemberClear:遍历10+GetPartyUser+DelPartyRecruit+清空; AddMember/RemoveMember:更新byUserCount=size(); SetRecruitInfo:3参数版(dwRecruitID,dwMasterID,stRecruit)+SetRecruitDate; GetPartyMemberList:ST_PARTY_RECRUIT_APPLY_INFO参数+byPartyGroupType分支+GetParty/GetForce或遍历成员; 修复:完整实现所有函数 | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::CPartyRecruit | 0x140708128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::~CPartyRecruit | 0x140708256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::SetRecruitInfo | 0x140708320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::AddMember | 0x140708480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::RemoveMember | 0x140708560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::GetMember | 0x140708640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::GetRecruitInfo | 0x140708784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::RecruitApply | 0x140708896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | ST_APPLY_MEMBER::operator= | 0x140709728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::ApplyMemberClear | 0x140709792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::RecruitAccept | 0x140710000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::DelApplyMember | 0x140711408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::SetRecruitDate | 0x140712208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::ClearRecruitDate | 0x140712256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::SendApplyUserList | 0x140712464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::ApplyMemberLevelUp | 0x140712880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::ApplyMemberMapMove | 0x140713328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::GetPartyMemberList | 0x140713760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CUserObject::GetMaxHP | 0x140716400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CUserObject::GetHP | 0x140716432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | ST_PARTY_RECRUIT_UPDATE::ST_PARTY_RECRUIT_UPDATE | 0x140716448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | PartyRecruit.h | CPartyRecruit::GetMemberCount | 0x140716496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::SetMyInfo | 0x14003CDB0 | verified | IDA 0x14003CDB0 + XRelaySocket::SetMyInfo委托调用; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::ServerProcessEx | 0x14003CF30 | verified | IDA 0x14003CF30 + subcmd分支('D'=ResCreateMatchingMaze/'E'=SyncPartyMazeInfo/'F'=SyncForceMazeInfo/'J'=ResCreateMatchingModeMaze); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::ResCreateMatchingMaze | 0x14003CFC0 | verified | IDA 0x14003CFC0 + matchingID>>ST_CREATE_MAZE>>PS_PARTY_INFO>>PS_FORCE_INFO+DoJob(0,lambda); lambda内byGroupType分支:1=party(SetMaze+SendCreateMatchingMaze+DB(4,8)) / 2=force(SetMaze+SendCreateMatchingMaze+DB(4,8)) | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::SyncPartyMazeInfo | 0x14003D6B0 | verified | IDA 0x14003D6B0 + dwPartyID+uxMapID+uxBeforeMapID反序列化+DoJob(0,lambda) | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::SyncForceMazeInfo | 0x14003D850 | verified | IDA 0x14003D850 + dwForceID+uxMapID+uxBeforeMapID反序列化+DoJob(0,lambda) | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::ResCreateMatchingModeMaze | 0x14003D9B0 | verified | IDA 0x14003D9B0 + ST_CREATE_MODE_MAZE反序列化+DoJob(0,lambda) | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::OnConnect | 0x1400FE1A0 | verified | IDA 0x1400FE1A0 + m_nSyncServerData=0+SendAddServer(); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::ServerProcess | 0x1400FE380 | verified | IDA 0x1400FE380 + subcmd分支(3=RecvServerUpdate/6=RecvPacketFromRelay/16=RecvChangeChannelRes/17=RecvUpdateChannelAll/18=RecvUpdateChannel/default=ServerProcessEx); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::SendAddServer | 0x1400FE620 | verified | IDA 0x1400FE620 + m_myInfo.nState=1+XSendPacket(eCMD_SERVER,eSUB_CMD_SERVER_ADD)+m_myInfo<<+Send; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::SendUpdateServerInfo | 0x1400FE6E0 | verified | IDA 0x1400FE6E0 + nUserCount>=0.95*MaxUser→nState=3+SS_UPDATE_SERVER_INFO构造+XSendPacket+Send; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::RecvServerUpdate | 0x1400FE7E0 | verified | IDA 0x1400FE7E0 + SS_UPDATE_SERVER_INFO>>+m_relayInfo.nState=updateInfo.nState; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::OnStartThread | 0x1400FE810 | verified | IDA 0x1400FE810 + IOCP客户端线程循环(ReConnect+双缓冲swap+OnParse包解析+Sleep节流); 框架层函数由TXDBSocket基类提供 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetID | 0x140079168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | PS_PARTY_INFO::PS_PARTY_INFO | 0x140092880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetType | 0x140220384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::CRelayControlSocket | 0x140245008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::~CRelayControlSocket | 0x140245120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::SetMyInfo | 0x140245168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::ServerProcessEx | 0x140245552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::ResCreateMatchingMaze | 0x140245696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::SyncPartyMazeInfo | 0x140247472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::SyncForceMazeInfo | 0x140247888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | CRelayControlSocket::ResCreateMatchingModeMaze | 0x140248240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | ST_CREATE_MODE_MAZE::ST_CREATE_MODE_MAZE | 0x140248672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | sprintf_s<21> | 0x140249056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XParse::Setbool | 0x140254224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetPublicIP | 0x140254288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetIP | 0x140254320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetLogicThread | 0x140254336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetMaxConnect | 0x140254352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetPort | 0x140254368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetChannel | 0x140254384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::~XRelaySocket | 0x140254400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | SS_SERVER_INFO::SS_SERVER_INFO | 0x140254544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetName | 0x140255728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::XRelaySocket | 0x140258576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::OnFinishThread | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvPacketFromRelay | 0x140818448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XTCPSkeleton::WriteLog | 0x140826640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::ForceProcess | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::FriendProcess | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::LeagueProcess | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::ModeMazeProcess | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvChangeChannelRes | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvCheckSessionID | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvExchangePost | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvExchangePriceHistory | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvGFBillingPostReload | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUpdateChannel | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUpdateChannelAll | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserChangeServer | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserEnterServer | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserKickout | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserMegaPhone | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserNotice | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserTradePasswordState | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserWhisperRes | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::ServerProcessEx | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::WorldModeProcess | 0x140844256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XParse::operator>> | 0x140890160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayControlSocket.h | XParse::GetBIGINT | 0x140890208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.cpp | TXSingleton<XRelayServer>::Instance()` 接入点 | - | verified | IDA 单例模式 + 模板实例化; 源码用CRTP静态局部变量实现,逻辑等价 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CFriendRecruitManager::{UpdateRecruit, IsRecruitList, DeleteRecruit, OnUpdate} | 0x140045580 | verified | IDA `0x140045580 / 0x140045660 / 0x1400454C0 / 0x140045110` + UpdateRecruit:WriteLock+find+set bLogin; IsRecruitList:ReadLock+find+return; DeleteRecruit:WriteLock+find+erase; OnUpdate:CTime+1hr定时+遍历过期+SendRecruitDelete; 结构差异:源码用std::map替代boost::multi_index | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::XRelayServer` 骨架 | 0x1400B00D0 | verified | IDA 0x1400B00D0 + 编译器生成的default构造器;源码用`= default;`声明,编译器自动生成m_UserInfos/m_mapUserPartyInfos/m_mapGameServer/m_partyManager/m_ForceManager等成员初始化 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::InitServer | 0x1400B05A0 | verified | IDA 0x1400B05A0 + CLogThreadManager::Start + XOption::ShowServerInfo + m_dwCachingLoad=0 + XSeed::Init + XResourceMgr::Init/Load + XGameDBSocketMgr::Init/AutoConnect + m_scControlSocket.Init("127.0.0.1",5001)+Connect + CObserveSocket::StartUp + CLogicThreadManager::Start(3) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::AddUser | 0x1400B0A90 | verified | IDA 0x1400B0A90 + boost::multi_index find/insert/modify + AddLeagueUser+DeleteUser/AddUser(Recommand)+UpdateRecruit+SendDBPacket(5,1); 结构差异:源码用unordered_map替代multi_index | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::AddPartyUser | 0x1400B0FD0 | verified | IDA 0x1400B0FD0 + DoJob(0,lambda)+lock+m_mapUserPartyInfos[actorID]; 差异:源码有pServer空指针检查 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::RemoveUser | 0x1400B1280 | verified | IDA 0x1400B1280 + Logout+SendDBAccount(2,2)+DeleteUser(Recommand)+UpdateRecruit(0)+RemovePartyUser+erase; 修复:添加DeleteUser+UpdateRecruit调用 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::RemovePartyUser | 0x1400B16C0 | verified | IDA 0x1400B16C0 lambda + GetMatchingState分支(1→PartyMatching,2→Force,3→ModeMaze)+Logout+erase; 修复:移除UpdateRecruit(已在RemoveUser中) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::GetUser(uint32) | 0x1400B1890 | verified | IDA 0x1400B1890 + CFAutoSlimReadLock + m_UserInfos.find(dwActorID) + return shared_ptr/null | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::GetPartyUser | 0x1400B1980 | verified | IDA 0x1400B1980 + m_mapUserPartyInfos.find+return; 差异:IDA无锁源码有锁(安全增强) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UpdateUserMap | 0x1400B2030 | verified | IDA 0x1400B2030 + GetUser+GetMapIns+InitRecruitListTime+mapChanged时erase/reinsert+SetMapIns+ChangeMap+UpdateMemberMapInfo+DoJob; 修复:重构为IDA结构 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::{KickOutUser, SendPacketAll, SendPacket} | 0x1400B25F0 | verified | IDA 0x1400B25F0 + PS_KICK_USER_INFO_UCID+XSendPacket(0xF3,0x07)+SendPacketAll+LogInfo | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::AddGameServerInfo | 0x1400B28A0 | verified | IDA 0x1400B28A0 + CFAutoSlimWriteLock+m_mapGameServer.insert+LogInfo+UnSetCachingLoad(锁外); 修复:UnSetCachingLoad移到锁外 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::AddServerInfo | 0x1400B2950 | verified | IDA 0x1400B2950 + GetServerType==2→AddGameServerInfo | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::{SendFriendList, SendBlockList} | 0x1400B3AB0 | verified | IDA 0x1400B3AB0/0x1400B3BD0 + find user+KickOutUser(0xC)+CUserObject::SendFriendList/SendBlockList; 结构差异:源码内联包构造逻辑,IDA委托给CUserObject方法; SendFriendList用ReadLock,SendBlockList用WriteLock | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::{PrepareDeleteRecruit, SendRecruitDelete, DeleteRecruit} | 0x1400B8930 | verified | IDA `0x1400B8930 / 0x1400B93A0 / 0x1400B8D00` + PrepareDeleteRecruit:find user+!IsRecruitList→XSendPacket(0xF5,0x18)+SendPacket/IsRecruitList→XSendDBPacket(5,0x10); SendRecruitDelete:PS_RECRUIT_DELETE+SendDBGame; DeleteRecruit:find+!nResult→DeleteRecruit+SendPacket; 修复:PrepareDeleteRecruit添加IsRecruitList检查分支 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::GetServer | 0x1400B9D40 | verified | IDA 0x1400B9D40 + CFAutoSlimReadLock + m_mapGameServer.find + return ptr/null | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SendChatNotice | 0x1400BA3C0 | verified | IDA 0x1400BA3C0 + XSendPacket(0xF3,0x11)+operator<<+SendPacketAll; 修复:源码用手动client循环,改用SendPacketAll | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SendChatMegaPhone | 0x1400BA450 | verified | IDA 0x1400BA450 + XSendPacket(0xF3,0x17)+stMegaPhone<<+psItemLinkInfo<<+SendPacketAll; 修复:源码用手动client循环,改用SendPacketAll | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SetUsersInfo | 0x1400BA510 | verified | IDA 0x1400BA510 + 遍历vecUserInfo+AddUser+bFinish→RecvUserInfo+SetSyncLoad+UpdateLeagueMemberInfo; 修复:缺失UpdateLeagueMemberInfo调用 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::GetUser(wchar_t*) | 0x1400BAAC0 | verified | IDA 0x1400BAAC0 + CFAutoSlimReadLock+find(GetName索引)+return; 结构差异:源码用线性查找替代multi_index索引 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::ReqExchangePriceList | 0x1400BB590 | verified | IDA 0x1400BB590 + CExchangePriceMgr::GetPriceList(成功)→XSendPacket(0xF3,0x28) / 失败→PS_DB_EXCHANGE_PRICE_HISTORY_REQ+SendDBGame(0x27,2) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::ReqExchangePriceUpdate | 0x1400BB9A0 | verified | IDA 0x1400BB9A0 + ReadLock+find user(GetCID索引)+SendPacket(0xF3,0x30)+CExchangePriceMgr::AddPriceList | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SendDBAccount | 0x1400BD4A0 | verified | IDA 0x1400BD4A0 + GetOrderID%GetAccountDBAgentCount + SendAccountDBAgent + LogError | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SendDBGame | 0x1400BD530 | verified | IDA 0x1400BD530 + GetOrderID%GetGameDBAgentCount + SendGameDBAgent + LogError | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SetCachingLoad | 0x1400BD8E0 | verified | IDA 0x1400BD8E0 + m_dwCachingLoad | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UnSetCachingLoad | 0x1400BD9B0 | verified | IDA 0x1400BD9B0 + m_dwCachingLoad^= + LogInfo + SendCachingLoad | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SendCachingLoad | 0x1400BDA30 | verified | IDA 0x1400BDA30 + CFAutoSlimReadLock+遍历IsSyncLoad+SetCachingLoad(USER)+XSendPacket(0xF2,0x70)+SendPacketAll+LogInfo; 修复:源码缺失SendPacketAll调用 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::AddLeagueUser | 0x1400BE110 | verified | IDA 0x1400BE110 + nLeagueID==0→false + CheckLeagueInfo→ReqLeagueLogin/SendFailLeagueLogin + !CheckLeagueInfo→XSendDBPacket(7,0x23)+psDBLeagueInfo; 修复:源码仅SetLeagueID,缺失完整联赛登录逻辑 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CRelayMazeOpenControl::{Init, LoadRows, CheckMazeOpenTime, SetCheckMazeOpenTime} | 0x140127CE0 | verified | IDA `0x140127CE0 / 0x14002BCC0` + CheckMazeOpenTime:m_bCheckMazeOpenTime检查+time64+localtime64_s+weekday+rowID范围100*(100*mazeID+wday)+1~20+m_mapTB_MAZE_OPENCONTROL查找+Open_Time/Remain_Time时间窗口; SetCheckMazeOpenTime:m_bCheckMazeOpenTime=bOn; 结构差异:IDA中属于XResourceMgr(m_mapTB_MAZE_OPENCONTROL),源码封装为CRelayMazeOpenControl(m_mapMazeOpenControl) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_FRIEND_RESULT::PS_FRIEND_RESULT | 0x140079184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_EQUIP_REWARD_RES::PS_HELPER_SUPPORT_EQUIP_REWARD_RES | 0x140079184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_RES_FRIEND_DELETE::PS_RES_FRIEND_DELETE | 0x140175328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_FRIEND_LIST::PS_FRIEND_LIST | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_LIST_RES::PS_HELPER_SUPPORT_LIST_RES | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_REQ_CLEAR_USER_STATE::PS_REQ_CLEAR_USER_STATE | 0x140266304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXSingleton<XRelayServer>::TXSingleton<XRelayServer> | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XIOCPSkeleton::OnDisConnect | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::XRelayServer | 0x140717008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::~XRelayServer | 0x140717680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ConsolCtrlHandler | 0x140718096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetName | 0x140718192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::InitServer | 0x140718240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::Clear | 0x140719184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetConsoleHandler | 0x140719456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddUser | 0x140719504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddPartyUser | 0x140720848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RemoveUser | 0x140721536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RemovePartyUser | 0x140722368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetUser | 0x140723088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetPartyUser | 0x140723328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateUserLevelUp | 0x140723520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateUserAwaken | 0x140724032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateUserProfilePhoto | 0x140724544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateUserMap | 0x140725040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::KickOutUser | 0x140726512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendPacket | 0x140726736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendPacketToGameServer | 0x140726944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendPacketAll | 0x140727152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddGameServerInfo | 0x140727200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddServerInfo | 0x140727376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RemoveGameServerInfo | 0x140727456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RemoveServerInfo | 0x140727968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::LoadDataReq | 0x140728080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::LoadForceDataReq | 0x140728320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::OnUpdate | 0x140728464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ClearUserState | 0x140729440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_REQ_CLEAR_USER_STATE::~PS_REQ_CLEAR_USER_STATE | 0x140729840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendFriendServerLoad | 0x140729872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetFriendLoad | 0x140730112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetBlockLoad | 0x140730992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetCharCommunity | 0x140731536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendFriendList | 0x140731824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendBlockList | 0x140732112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateFriendCommunity | 0x140732400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareFriendInvite | 0x140733184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::InviteFriend | 0x140736160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::InviteCheckFriend | 0x140739424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareFriendAccept | 0x140741712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_DB_FRIEND_ACCEPT_REQ::PS_DB_FRIEND_ACCEPT_REQ | 0x140743456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AcceptFriend | 0x140743504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareDeleteFriend | 0x140745232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DeleteFriend | 0x140746288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareBlockListAdd | 0x140747440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddBlockList | 0x140748336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareBlockListDel | 0x140749200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DeleteBlockList | 0x140749968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetRecruitList | 0x140750656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareAddRecruit | 0x140750720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddRecruit | 0x140751456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareDeleteRecruit | 0x140751920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareRecruitInfo | 0x140752480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DeleteRecruit | 0x140752896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RecruitList | 0x140753296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_RES_RECRUIT_LIST::~PS_RES_RECRUIT_LIST | 0x140754512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_RECRUIT_LIST::operator= | 0x140754544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendRecruitDelete | 0x140754592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqFriendFind | 0x140754752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ResFriendFind | 0x140754912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::IsFriendBlock | 0x140755856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::IsFriendBlock | 0x140756112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RecommandFriend | 0x140756384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetServer | 0x140757056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendChatWhisper | 0x140757232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendChatNotice | 0x140758720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendChatMegaPhone | 0x140758864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetUsersInfo | 0x140759056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::STCharInfo | 0x140759584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetUser | 0x140760512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBLog | 0x140760752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBChatLog | 0x140761104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqLeagueInvite | 0x140761440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DailyMissionFriendReq | 0x140762336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DailyMissionFriendRes | 0x140762848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqExchangePriceList | 0x140763280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ResExchangePriceList | 0x140763760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqExchangePriceUpdate | 0x140764320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportInfo | 0x140764944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportRegister | 0x140765584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportReward | 0x140766160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportList | 0x140766752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_FRIEND_LIST::~PS_FRIEND_LIST | 0x140767920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_LIST_RES::~PS_HELPER_SUPPORT_LIST_RES | 0x140767952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportEquip | 0x140767984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_EQUIP_RES::~PS_HELPER_SUPPORT_EQUIP_RES | 0x140769440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ResHelperSupportEquip | 0x140769472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendServerInfoAll | 0x140770528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendOperationTimeInfo | 0x140771088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBAccount | 0x140771232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBGame | 0x140771376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateServerState | 0x140771520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::CharacterNameChange | 0x140771904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetCurDateSec | 0x140772272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetCachingLoad | 0x140772320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UnSetCachingLoad | 0x140772528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendCachingLoad | 0x140772656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::WriteLog | 0x140773104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendMyRoomPollenUpdate | 0x140773248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ChangeFriendName | 0x140773648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddLeagueUser | 0x140774416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::TXMultiPoolServer<CServer> | 0x140776528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXSingleton<CLogThreadManager>::Instance | 0x140778080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::TXObjectMgr<CServer> | 0x140779552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::XCreator<CServer>::XCreator<CServer> | 0x140781024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::XCreator<CServer>::Create | 0x140781088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<XClient>::IXCreator::IXCreator | 0x140781168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::TXPool<IXObject> | 0x140781872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Create | 0x140793360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | wcscpy_s<51> | 0x140794880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | wcscpy_s<257> | 0x140794928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | vsprintf_s<512> | 0x140795056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Pop | 0x140800032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::GetFullSize | 0x140804608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::GetCurSize | 0x140804640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | HRESULT_FROM_WIN32 | 0x140817104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | IXObjectMgr::GetSessionID | 0x140817424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | IXObjectMgr::IXObjectMgr | 0x140817536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CServer::IsSyncLoad | 0x140817632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CPartyManager::Isload | 0x140817680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CServer::IsRecvServerInfo | 0x140817696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XSendDBPacket::GetOrderID | 0x140817744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CServer::GetUserCount | 0x140817776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CServer::GetServerInfo | 0x140817808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XResourceMgr::GetTB_HELPER_REWARD | 0x140818336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_HELPER_INFO::ST_HELPER_INFO | 0x140818960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_HELPER_INFO::~ST_HELPER_INFO | 0x140819152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_EQUIP_RES::PS_HELPER_SUPPORT_EQUIP_RES | 0x140819216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_HELPER_SUPPORT_FRIEND_INFO::ST_HELPER_SUPPORT_FRIEND_INFO | 0x140819280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_INFO_RES::PS_HELPER_SUPPORT_INFO_RES | 0x140819808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_EXCHANGE_PRICE_INFO::ST_EXCHANGE_PRICE_INFO | 0x140819856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_DB_EXCHANGE_PRICE_HISTORY_REQ::PS_DB_EXCHANGE_PRICE_HISTORY_REQ | 0x140819936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_CHAT_LOG_GAME::ST_CHAT_LOG_GAME | 0x140819984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_LOG_GAME::ST_LOG_GAME | 0x140820016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CServer::RecvUserInfo | 0x140820048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::CheckRecruitListTime | 0x140820080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_RES_RECRUIT_LIST::PS_RES_RECRUIT_LIST | 0x140820128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_RECRUIT_STATE::PS_RECRUIT_STATE | 0x140820176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_RES_BLOCKLIST_ADD::PS_RES_BLOCKLIST_ADD | 0x140820208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_RES_FRIEND_ACCEPT::PS_RES_FRIEND_ACCEPT | 0x140820272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetMemo | 0x140820320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetMemo | 0x140820368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetCommunityState | 0x140820416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetCommunityState | 0x140820448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CCommunity::GetSyncBlockList | 0x140820496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::GetSyncBlockList | 0x140820528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetLoadBlockList | 0x140820576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLoadBlockList | 0x140820608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CCommunity::GetSyncFriendList | 0x140820656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::GetSyncFriendList | 0x140820688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetLoadFriendList | 0x140820736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLoadFriendList | 0x140820768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLoadFriend | 0x140820816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CServer::GetServerType | 0x140820848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_KICK_USER_INFO_UCID::PS_KICK_USER_INFO_UCID | 0x140820880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::InitRecruitListTime | 0x140820944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetProfilePhoto | 0x140820992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetAwaken | 0x140821024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLevel | 0x140821056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::GetIP | 0x140821088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserPartyInfo::SetServerID | 0x140821104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserPartyInfo::SetCID | 0x140821136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetServer | 0x140821168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetMapIns | 0x140821280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetGameOption | 0x140821328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STAbility::STAbility | 0x140821408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_EQUIP_ITEM_INFO::ST_EQUIP_ITEM_INFO | 0x140821504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | PS_EXCHANGE_PRICE_HISTORY_REQ::PS_EXCHANGE_PRICE_HISTORY_REQ | 0x140821536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_TitleInfo::ST_TitleInfo | 0x140821536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STLeagueInfo::STLeagueInfo | 0x140821584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STEquipBase::Init | 0x140821632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_EQUIP_ITEM_INFO::Init | 0x140821664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::Init | 0x140821712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::STCharInfo | 0x140822176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::operator= | 0x140824736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CUserObject::CUserObject | 0x140825808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XResourceMgr::SetServerContents | 0x140826144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XOption::GetContentsOption | 0x140826192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XOption::GetResLoadType | 0x140826224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XOption::GetResFilePath | 0x140826256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XOption::GetDNS | 0x140826288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XSeed::Init | 0x140826336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | CObserveSocket::~CObserveSocket | 0x140826448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | ST_SERVER_GROUP_INFO::ST_SERVER_GROUP_INFO | 0x140826496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XIOCPServer::WriteLog | 0x140826640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XServer::GetCreatorPtr | 0x140826672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XServer::GetName | 0x140826704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::IXCreator::IXCreator | 0x140826736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::TXCreator<CServer>::Create | 0x140826768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::TXCreator<CServer>::TXCreator<CServer> | 0x140826896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::SetSafeArrayFlag | 0x140826944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Init | 0x140827472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Init | 0x140827632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Push | 0x140829824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Delete | 0x140829888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | IXObjectMgr::~IXObjectMgr | 0x140830064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::~TXPool<IXObject> | 0x140830976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::SafeArrayLock | 0x140833296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::SafeArrayUnLock | 0x140833952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Clear | 0x140834000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Clear | 0x140834320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::~TXObjectMgr<CServer> | 0x140834384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::~TXMultiPoolServer<CServer> | 0x140834496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | UXActorID::operator= | 0x140861808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | RelayServer.h | XIOCPServer::GetDeletePtr | 0x140980528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.cpp | ServiceMain | 0x1400CF270 | verified | IDA 0x1400CF270 + RegisterServiceCtrlHandlerExA+SET_SERVICE_STATE(4,3)+SetUnhandledExceptionFilter+Instance+Run+Shutdown+SET_SERVICE_STATE(1,3); 差异:源码有StartLog/LogReady调用(GreenDamTan扩展) | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.cpp | ServiceInit | 0x1400CF470 | verified | IDA 0x1400CF470 + GetModuleFileNameA+_splitpath+argc<2输出用法+argc==3且"-def"分支+/INSTALL+/UNINSTALL+/VER分支; 差异:源码有/TEST分支(IDA无,测试用) | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | main | 0x140844272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | ServiceMain | 0x140844400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | SERVICE_HANDLER | 0x140844576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | SET_SERVICE_STATE | 0x140844768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | ServiceInit | 0x140844912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | InstallService | 0x140846080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | ErrorCode2String | 0x140846576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerMain.h | UninstallService | 0x140846848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::Parse | 0x1400CCD20 | verified | IDA 0x1400CCD20 + 4 case switch(1/3/17/18)完全匹配IDA | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeEnter | 0x1400CCDB0 | verified | IDA 0x1400CCDB0 + PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ>>GetClientPtr+DoJob(0,lambda→EnterMatching) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeExit | 0x1400CCF90 | verified | IDA 0x1400CCF90 + PS_MODE_MAZE_MATCHING_EXIT>>GetClientPtr+DoJob(0,lambda→ExitMatching) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeTime_Cheat | 0x1400CD120 | verified | IDA 0x1400CD120 + nID>>nStart>>nEnd+GetClientPtr+DoJob(0,lambda) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeMatchingEvent | 0x1400CD2C0 | verified | IDA 0x1400CD2C0 + PS_SERVER_MODE_MAZE_MATCHING_EVENT>>GetClientPtr+DoJob(0,lambda→ModeMazeMatchingEvent) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::CServerModeMazeProcess | 0x140834608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::~CServerModeMazeProcess | 0x140834800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::Parse | 0x140834848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeEnter | 0x140834992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeExit | 0x140835472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeTime_Cheat | 0x140835872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeMatchingEvent | 0x140836288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_EVENT::~PS_SERVER_MODE_MAZE_MATCHING_EVENT | 0x140836752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_EVENT::PS_SERVER_MODE_MAZE_MATCHING_EVENT | 0x140836784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_EVENT::PS_SERVER_MODE_MAZE_MATCHING_EVENT | 0x140841680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ::PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ | 0x140841744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::Parse | 0x1400CE940 | verified | IDA 0x1400CE940 + switch(1/3/51)完全匹配; 修复:default返回true(IDA返回1) | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x1400CE9C0 | verified | IDA 0x1400CE9C0 + GetClientPtr+SS_SERVER_INFO>>+SetServerInfo+Instance+AddServerInfo+return 1; 差异:源码有null检查(IDA无) | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::ReqUpdateServerInfo | 0x1400CEA50 | verified | IDA 0x1400CEA50 + GetClientPtr+operator>>(SS_UPDATE_SERVER_INFO)+return; 注意:IDA不处理updateInfo(空操作) | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::SyncUsersInfo | 0x1400CEA90 | verified | IDA 0x1400CEA90 + PS_USERS_INFO构造+operator>>+GetClientPtr+SetUsersInfo+return | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | PS_USERS_INFO::PS_USERS_INFO | 0x140261680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::CServerProcess | 0x140841808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::~CServerProcess | 0x140842000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::Parse | 0x140842048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::ReqCreateServer | 0x140842176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::ReqUpdateServerInfo | 0x140842320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::SyncUsersInfo | 0x140842384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | PS_USERS_INFO::~PS_USERS_INFO | 0x140842496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | ServerProcess.h | PS_USER_INFO_FOR_RELAY::~PS_USER_INFO_FOR_RELAY | 0x140843136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::Clear | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | IXObject::NotifyRemoved | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XClient::OnAccept | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XClient::OnClear | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XClient::OnParse | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XClient::OnUpdate | 0x140818448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::CServer | 0x140854560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::~CServer | 0x140854768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::RegisterProcess | 0x140854848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::Init | 0x140855728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::OnLogOut | 0x140855824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::SetServerInfo | 0x140855904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | CServer::SetSyncLoad | 0x140856048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XClient::SetEncrypt | 0x140856224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XSocket::Init | 0x140856256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | XClient::Init | 0x140856432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | IXObject::~IXObject | 0x140856544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | IXObject::GetSessionID | 0x140856592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | IXObject::SetSessionID | 0x140856608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | User.cpp | IXObject::IXObject | 0x140856704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueDeletePenalty | - | verified | IDA无此函数(可能被内联或丢弃); 源码实现简单getter: return m_biLeagueDeletePenalty | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetMatchingID | - | verified | IDA无独立函数(CUserPartyInfo有); 源码GetMatchingID返回GetCID(),用于作为matchingID | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::{Clear, IsFriend, IsBlockList, GetFriendUCID, GetFriendList, GetBlcokList, SetLoadFriendList, SetLoadBlockList, GetLoadFriendList, GetLoadBlockList, SetSyncFriendList, SetSyncBlockList} | 0x140001110 | verified | IDA 0x140001110/0x140001270/0x1400013E0/0x140001600 + Clear:m_mapFriend.clear+m_mapBlockList.clear+flags=0 / IsFriend(dwUCID):find+GetType比对 / IsFriend(strName):find_by_name+compare+GetType / IsBlockList(dwUCID):find; 结构差异:源码用unordered_map替代multi_index | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CBlockUser::{GetUCID, GetName} | 0x14000BDD0 | verified | IDA 0x14000BDD0 + GetUCID:return m_stBlockInfo.dwUCID; GetName:return wstring(m_stBlockInfo.strName) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CFriendMember::{GetUCID, GetType, GetName} | 0x14000BDD0 | verified | IDA 0x14000BDD0/0x140009BC0 + GetUCID:return m_stFriendInfo.dwID; GetType:return m_stFriendInfo.byType; GetName:return wstring(m_stFriendInfo.strName) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::{SetLeagueWithdrawPenalty, SetLeagueDeletePenalty} | 0x140062d40 | verified | IDA 0x140062d40/0x140062d60 + m_biLeague*Penalty = biPenalty | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueWithdrawPenalty | 0x140084830 | verified | IDA 0x140084830 + return m_biLeagueWithdrawPenalty; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsMaze | 0x1400D22D0 | verified | IDA 0x1400D22D0 + GetMapID/10000==2 + GetTB_MAZE_INFO→Maze_Type!=6; 注意:源码仅实现范围检查,未实现完整Maze_Type检查(RelayServer不加载maze表) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::{SendPacket, CheckGameOption, GetFriendUCID, GetFriendList, GetBlcokList, SetLoadFriendList, SetLoadBlockList, GetLoadFriendList, GetLoadBlockList, SetSyncFriendList, SetSyncBlockList, IsFriendList, IsBlockList} | 0x1400D4B00 | verified | IDA 0x1400D4B00/0x1400D5150/0x1400D4170/0x1400D41A0 + SendPacket:m_pServer?SendEx / CheckGameOption:switch eIndex + GetFriendUCID:GetFriendUCID(m_Community) / GetFriendList:GetFriendList(m_Community) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueMemberInfo | 0x1400D5210 | verified | IDA 0x1400D5210 + GetName→wcscpy_s<21>+CUserPartyInfo::GetMatchingID+GetLevel+GetClass+GetAwaken+GetProfilePhoto+GetChannel+GetMapID+bLogin=1; 差异:IDA用CUserPartyInfo::GetMatchingID,源码用GetCID()(等价,因CUserObject::GetMatchingID返回GetCID) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsMaze | 0x140856784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetForceMemberInfo | 0x140856896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetPartyMemberInfo | 0x140856896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetUserInfo | 0x140857136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetUserRecruitInfo | 0x140857584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::LoadFriend | 0x140858080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::LoadBlock | 0x140859440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsBlockList | 0x140859856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsBlockList | 0x140859904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsFriendList | 0x140859952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsFriendList | 0x140860016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::UpdateCharCommunity | 0x140860080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::LoginFriend | 0x140860384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::Logout | 0x140860784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::ChangeMap | 0x140861888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::Levelup | 0x140862960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::UpdateProfilePhoto | 0x140863936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsValiedFriendListCount | 0x140864576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetFriendUCID | 0x140864624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetFriendList | 0x140864672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::UpdateFriend | 0x140864736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckFriendInvite | 0x140865072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckFriendAccept | 0x140865120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckBlockAdd | 0x140865184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLastFriendWaitList | 0x140865248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::AddFriend | 0x140865296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::DeleteFriend | 0x140866304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::AddBlockList | 0x140866352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::DeleteBlockList | 0x140866640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetRecommandInfo | 0x140866688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::AddFriendPoint | 0x140866880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetFriendLevel | 0x140866944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendPacket | 0x140867072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendFriendServerLoad | 0x140867136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendFriendList | 0x140867312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendBlockList | 0x140867664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendUpdateCommunity | 0x140868000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SetGameOption | 0x140868592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckGameOption | 0x140868688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::SetName | 0x140868832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueMemberInfo | 0x140868880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::ChangeFriendName | 0x140869136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CFriendMember::~CFriendMember | 0x140870992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::SetSyncBlockList | 0x140874608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetLoadBlockList | 0x140874640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::SetSyncFriendList | 0x140874672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetLoadFriendList | 0x140874704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::SetCharCommunity | 0x140874736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CBlockUser::CBlockUser | 0x140874800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CFriendMember::CFriendMember | 0x140874832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetMemo | 0x140874880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetMemo | 0x140874960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetCommunityState | 0x140875040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetCommunityState | 0x140875056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.cpp | CUserPartyInfo::{GetApplyRecruitCount, IsApplyRecruit, CanApplyRecruit, ClearApplyParty} | 0x1400D6F00 | verified | IDA `0x1400D6F00 / 0x1400D6F50 / 0x1400D6FA0 / 0x1400D6FE0` + GetApplyRecruitCount:遍历5+count; IsApplyRecruit:遍历5+return; CanApplyRecruit:count<5; ClearApplyParty:遍历5+dwRecruitID匹配→设0 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.cpp | CUserPartyInfo::DelPartyRecruit | 0x1400D7030 | verified | IDA 0x1400D7030 + 遍历5+dwRecruitID匹配+bPartySend→FindRecruitPtr+DelApplyMember(0)+设0+无条件SendPacket(0xF4,0x2F); 修复:删除early return,末尾无条件发包 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::{GetRewardState, SetRewardState} | 0x140032900 | verified | IDA 0x140032900(CHelperSupport::GetRewardState) + return m_byRewardState; 继承关系:CHelperSupport→CUserPartyInfo,方法实际在CHelperSupport中定义; 源码实现简单getter/setter | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::CUserPartyInfo | 0x140875104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::~CUserPartyInfo | 0x140875232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SyncChagneMapForParty | 0x140875264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SyncChagneLevelForParty | 0x140875520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::ClearRecruitParty | 0x140875760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SetApplyRecruitID | 0x140876208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::GetApplyRecruitCount | 0x140876288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::IsApplyRecruit | 0x140876368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::CanApplyRecruit | 0x140876448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::ClearApplyParty | 0x140876512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::DelPartyRecruit | 0x140876592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SetRecruitPenalty | 0x140877072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::GetRemainRecruitPenalty | 0x140877120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::GetMYApplyRecruitInfo | 0x140877168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::Logout | 0x140877472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::Parse | 0x1400D74F0 | verified | IDA 0x1400D74F0 + sub=2→SyncLogin/3→SyncLogout/4→SyncUpdateMap/8→Levelup/9→Whisper/0x11→Notice/0x17→MegaPhone/0x28→ExchangePriceList/0x29→ExchangePriceUpdate/0x31→NameChange/0x33→Option/0x34→MyRoomPollen/0x36→Awaken/0x38→ProfilePhoto; 修复:添加缺失的4个switch分支 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::SyncLoginUser | 0x1400D76D0 | verified | IDA 0x1400D76D0 + GetClientPtr+STCharInfo+ST_GAME_OPTION+UXMapID反序列化+AddUser | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400D77D0 | verified | IDA 0x1400D77D0 + dwActorID+nAccountState+byKick反序列化+RemoveUser | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x1400D7880 | verified | IDA 0x1400D7880 + GetClientPtr+PS_UPDATE_USER_MAP_INFO构造+operator>>+UpdateUserMap+return | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::SyncUserLevelup | 0x1400D78E0 | verified | IDA 0x1400D78E0 + XParse>>dwActorID>>byLevel + UpdateUserLevelUp | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::SyncUserAwaken | 0x1400D7940 | verified | IDA 0x1400D7940 + XParse>>dwActorID>>byAwaken + UpdateUserAwaken | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::SyncUserProfilePhoto | 0x1400D79A0 | verified | IDA 0x1400D79A0 + XParse>>dwActorID>>dwProfilePhotoID + UpdateUserProfilePhoto | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::ReqUserChatWhisper | 0x1400D7A10 | verified | IDA 0x1400D7A10 + dwActorID>>PS_CHAT_WHISPER>>PS_CHAT_ITEM_LINK_FOR_SERVER+SendChatWhisper | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::ReqUserChatNotice | 0x1400D7B30 | verified | IDA 0x1400D7B30 + PS_CHAT_NOTICE>> + SendChatNotice | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::ReqUserChatMegaPhone | 0x1400D7BA0 | verified | IDA 0x1400D7BA0 + PS_CHAT_MEGAPHONE>>PS_CHAT_ITEM_LINK_FOR_SERVER>> + SendChatMegaPhone | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::ReqExchangePriceList | 0x1400D7CA0 | verified | IDA 0x1400D7CA0 + GetClientPtr+PS_EXCHANGE_PRICE_HISTORY_REQ>>+ReqExchangePriceList | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::ReqUserOption | 0x1400D8360 | verified | IDA 0x1400D8360 + XParse>>dwUCID>>ST_OPTION_BIT + GetUser + null check + SetGameOption | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.cpp | CUserProcess::ReqMyRoomPollenSync | 0x1400D8470 | verified | IDA 0x1400D8470 + XParse>>dwUAID>>nPollenIndex>>PS_MYROOM_POLLEN_HELP_USER>>biHarvestDate + SendMyRoomPollenUpdate | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::Setchar | 0x140254224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::ToDocument | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::CUserProcess | 0x140877568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::~CUserProcess | 0x140877760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::Parse | 0x140877808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncLoginUser | 0x140878288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncLogoutUser | 0x140878544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUpdateUserMap | 0x140878720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUserLevelup | 0x140878816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUserAwaken | 0x140878912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUserProfilePhoto | 0x140879008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserChatWhisper | 0x140879120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserChatNotice | 0x140879408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserChatMegaPhone | 0x140879520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqExchangePriceList | 0x140879776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqExchangePriceUpdate | 0x140879872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_EXCHANGE_PRICE_HISTORY_UPDATE::~PS_EXCHANGE_PRICE_HISTORY_UPDATE | 0x140880048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqNameChange | 0x140880080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_SERVER_CHANGE_CHARACTER_NAME::~PS_SERVER_CHANGE_CHARACTER_NAME | 0x140881264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_SERVER_CHANGE_CHARACTER_NAME::PS_SERVER_CHANGE_CHARACTER_NAME | 0x140881328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_LEAGUE_APPLICANT_CHECK_LIST::ST_LEAGUE_APPLICANT_CHECK_LIST | 0x140881456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserOption | 0x140881504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqMyRoomPollenSync | 0x140881776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_MYROOM_POLLEN_HELP_USER::PS_MYROOM_POLLEN_HELP_USER | 0x140885920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_OPTION_BIT::ST_OPTION_BIT | 0x140885984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_SERVER_CHANGE_CHARACTER_NAME::PS_SERVER_CHANGE_CHARACTER_NAME | 0x140886016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_POST_CHAR::ST_POST_CHAR | 0x140886096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_POST_DATA::ST_POST_DATA | 0x140886160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_POST_DATA::~ST_POST_DATA | 0x140886448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_EXCHANGE_PRICE_HISTORY_UPDATE::PS_EXCHANGE_PRICE_HISTORY_UPDATE | 0x140886544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_CHAT_MEGAPHONE::PS_CHAT_MEGAPHONE | 0x140886688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | wcscpy_s<7> | 0x140886752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_CHAT_NOTICE::PS_CHAT_NOTICE | 0x140886800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_CHAT_WHISPER::PS_CHAT_WHISPER | 0x140886880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_UPDATE_USER_MAP_INFO::PS_UPDATE_USER_MAP_INFO | 0x140886960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::~TiXmlString | 0x140887328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::quit | 0x140887360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::NextSiblingElement | 0x140887424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::FirstChildElement | 0x140887472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::~TiXmlDocument | 0x140887520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetBytes | 0x140889056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::SetBytes | 0x140889152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetString | 0x140889376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetWString | 0x140889648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::Getchar | 0x140889904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetSHORT | 0x140889968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetWORD | 0x140889968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::operator>> | 0x140890048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::SetWORD | 0x140890096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetUBIGINT | 0x140890208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::operator>> | 0x140890288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::operator<< | 0x140890336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::GetFLOAT | 0x140890384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XParse::SetFLOAT | 0x140890464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_DB_FRIEND::PS_DB_FRIEND | 0x140913808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_FIND_FRIEND::ST_FIND_FRIEND | 0x140913968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_DAILY_MISSION_FRIEND_RES::ST_DAILY_MISSION_FRIEND_RES | 0x140928128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_CREATE_ITEM::ST_CREATE_ITEM | 0x140930336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_SOCKET_DATA::ST_SOCKET_DATA | 0x140954656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_ITEM_SOCKET::ST_ITEM_SOCKET | 0x140954688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_LEAGUE_INVENTORY_FOR_LOG::PS_LEAGUE_INVENTORY_FOR_LOG | 0x140954736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_ITEM_PACKAGE_PARTS::ST_ITEM_PACKAGE_PARTS | 0x140954832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_USER_INFO_FOR_RELAY::PS_USER_INFO_FOR_RELAY | 0x140969152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_USER_INFO_FOR_RELAY::PS_USER_INFO_FOR_RELAY | 0x140974912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | VColorRef::VColorRef | 0x140975440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_CASH_SHOP_TAB::~ST_CASH_SHOP_TAB | 0x140976032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_CASH_SHOP_TAB::ST_CASH_SHOP_TAB | 0x140979952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToComment | 0x140980528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToDeclaration | 0x140980528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToElement | 0x140980528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToText | 0x140980528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToUnknown | 0x140980528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSendPacket::XSendPacket | 0x140982544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSendPacket::Encrypt | 0x140982624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::~XTCPSkeleton | 0x140982848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::XSend | 0x140982864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::XRecv | 0x140983072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::OnSend | 0x140983312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::OnRecv | 0x140983360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::OnEtcEvent | 0x140984368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOverLab::Init | 0x140984400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOverLab::XOverLab | 0x140984528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOPool::~XIOPool | 0x140984720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOPool::AllocIO | 0x140984736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOPool::FreeIO | 0x140984960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::TXPool<XOverLab> | 0x140985152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::~TXPool<XOverLab> | 0x140985424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::XCreator::Create | 0x140985632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Pop | 0x140985728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Pop | 0x140985728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Pop | 0x140985728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::WriteLog | 0x140986272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::XIOCPSkeleton | 0x140986288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::~XIOCPSkeleton | 0x140986496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::CreateIOCP | 0x140986608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::BlockSocket | 0x140987136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::Shutdown | 0x140987360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::WorkerThread | 0x140987680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::_fnWorkThread | 0x140988080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Clear | 0x140988096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSocket::XSocket | 0x140988528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSocket::~XSocket | 0x140988656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::XIOCPClient | 0x140988784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::~XIOCPClient | 0x140989040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Init | 0x140989136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Shutdown | 0x140989440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::SetSocket | 0x140989664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnEtcEvent | 0x140989984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Connect | 0x140990272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Connect | 0x140990528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::IsConnection | 0x140990784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Send | 0x140990800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Parsing | 0x140991136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::DisConnect | 0x140991824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Close | 0x140991888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnConnect | 0x140992016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnDisConnect | 0x140992032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnNotConnect | 0x140992080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::ConnectThread | 0x140992096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::_fnConnectThread | 0x140992480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnRecv | 0x140992496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XPRINT | 0x140993824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | sprintf_s<1024> | 0x140994144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::CSimpleLock | 0x140994192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::~CSimpleLock | 0x140994208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Init | 0x140994240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Lock | 0x140994288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::UnLock | 0x140994352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Owner::Owner | 0x140994416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Owner::~Owner | 0x140994496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSeed::XSeed | 0x140994560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSeed::~XSeed | 0x140994608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSeed::SetSeed | 0x140994656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSeed::GenTableForNumbers | 0x140994880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::XClient | 0x140995312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PerSocketContext::~PerSocketContext | 0x140995552 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::~XClient | 0x140995616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::IsState | 0x140995872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::SetState | 0x140995904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::Parse | 0x140995920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::Init | 0x140995968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::InitPool | 0x140996112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::Register | 0x140996384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::SendEx | 0x140996496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::SendErrorMessage | 0x140996544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::XIOCPServer | 0x140999072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::~XIOCPServer | 0x140999264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::SetSocket | 0x140999424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::Init | 0x140999744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::Shutdown | 0x141000016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::CloseClient | 0x141000224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::BackendThreadProc | 0x141000416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::PrintFPS | 0x141001152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::AcceptThread | 0x141001264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::XAccept | 0x141001920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::XSend | 0x141002256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::_fnAcceptThread | 0x141002544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::_fnBackEndThread | 0x141002560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Clear | 0x141002576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | IXComponent::GetName | 0x141002960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XProcessComposite::~XProcessComposite | 0x141003072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XProcessComposite::Init | 0x141003104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XProcessComposite::Parse | 0x141003392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXComposite<unsigned char,IXProcess>::TXComposite<unsigned char,IXProcess> | 0x141003696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClientPool::XClientPool | 0x141003968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClientPool::AllocClient | 0x141004256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClientPool::FreeClient | 0x141004608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClientPool::GetHead | 0x141004976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClientPool::GetNext | 0x141005232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::TXPool<XClient> | 0x141005488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::~TXPool<XClient> | 0x141005760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXList<XClient * __ptr64>::Remove | 0x141005968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Init | 0x141006384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Init | 0x141006384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Init | 0x141006384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Push | 0x141006640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Push | 0x141006640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Push | 0x141006640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | LogHelper::LogDebug | 0x141007824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | LogHelper::LogError | 0x141008336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | LogHelper::LogInfo | 0x141008848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_SERVER_INFO::ST_SERVER_INFO | 0x141010816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::XOption | 0x141010848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | AGENT_SYSTEM_INFO::AGENT_SYSTEM_INFO | 0x141011472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | AGENT_SYSTEM_INFO::~AGENT_SYSTEM_INFO | 0x141011568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::~XOption | 0x141011584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::Load | 0x141011712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::ShowServerInfo | 0x141023792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::GetSystemType | 0x141024848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::GetNationType | 0x141024976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::GetServerType | 0x141025136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::GetServerPrivateIPAndPort | 0x141025424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::AddChannelInfo | 0x141025808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::AddChannelDistrict6Info | 0x141026240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::GetDBAgentInfo | 0x141026608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XOption::GetAgentPrivateIPAndPort | 0x141026816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | sprintf_s<64> | 0x141032240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::Init | 0x141036176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::_BackendThreadStartingPoint | 0x141036320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::SetMyInfo | 0x141036368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::Connect | 0x141036672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnConnect | 0x141036704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnDisConnect | 0x141036720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnNotConnect | 0x141036768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnParse | 0x141036784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::ServerProcess | 0x141037184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::UserProcess | 0x141037360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::ServerProcessEx | 0x141037840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XClient::OnLogOut | 0x141037840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnParse | 0x141037840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::BackSends | 0x141037840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::PartyProcess | 0x141037840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::SendAddServer | 0x141037856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::SendUpdateServerInfo | 0x141038048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::OnFinalizeThread | 0x141038288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::Clear | 0x141038288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSendPacket::~XSendPacket | 0x141038288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::RecvServerUpdate | 0x141038304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnStartThread | 0x141038352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::XMultiPoolServer | 0x141039056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::~XMultiPoolServer | 0x141039248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::AcceptThread | 0x141039312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::BackendThreadProc | 0x141039984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::OnRecv | 0x141040752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::OnAccect | 0x141041872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::OnLogOut | 0x141042016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::Init | 0x141042048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::Run | 0x141042400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::Shutdown | 0x141042560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::LoadConfig | 0x141042608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XServer::OnUpdate | 0x141042672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::CLogThreadProc | 0x141042752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::OnInitializeThread | 0x141042992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::ThreadProc | 0x141043136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::CLogThreadManager | 0x141043472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::GetLogger | 0x141045088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::Start | 0x141045456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::End | 0x141045968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::DoJob | 0x141046480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::CreateWorkerThread | 0x141046656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CFThread<CLogThreadProc>::Create | 0x141047088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CFThread<CLogThreadProc>::RunThread | 0x141049056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CFThread<CLogThreadProc>::CallThreadHandlerProc | 0x141050816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDump::XWriteStackDetails | 0x141051264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDump::XGenerateDump_Popup | 0x141052240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDump::WriteDump | 0x141052336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | sprintf_s<260> | 0x141052832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | IsValidHandle | 0x141053088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | WaitSucceeded | 0x141053120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CKernelEvent::CKernelEvent | 0x141053136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CKernelEvent::Set | 0x141053312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::CWaitableCollection | 0x141053328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::~CWaitableCollection | 0x141053408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::AddObject | 0x141053424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::Wait | 0x141053472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::Shutdown | 0x141053792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::AllocIoContext | 0x141054000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::GetPool | 0x141054192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::ReleasePool | 0x141054384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::ReleaseIoContext | 0x141054496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::XDBStmt | 0x141054704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::~XDBStmt | 0x141054832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::Clear | 0x141054896 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::Init | 0x141055008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::SQLClose | 0x141055280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::SQLBindParameter | 0x141055344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::SQLGetData | 0x141055472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::XDBBinder | 0x141055600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::~XDBBinder | 0x141055616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::Fetch | 0x141055632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::Execute | 0x141055680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::Close | 0x141055776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::GetString | 0x141055840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBManager::XDBCreator::Create | 0x141056000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBManager::XDBManager | 0x141056176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBManager::~XDBManager | 0x141056352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBManager::Init | 0x141056416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBManager::GetDBConnect | 0x141056624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBManager::CollectDBConnect | 0x141056752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::TXPool<XDBConnect> | 0x141056768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::~TXPool<XDBConnect> | 0x141057040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Add | 0x141057296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::GetHDBC | 0x141057744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::Init | 0x141057760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::Clear | 0x141058064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::Connect | 0x141058240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::DisConnect | 0x141058528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::SetEndTran | 0x141058624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | GetLastError | 0x141058736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XDBEnv::Init | 0x141059472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::reserve | 0x141059824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::assign | 0x141060000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::append | 0x141060224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlFOpen | 0x141060352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::CopyTo | 0x141060416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::Clear | 0x141060496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::FirstChild | 0x141060592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::GetDocument | 0x141060672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::Print | 0x141060736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::Accept | 0x141060832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlAttributeSet::Find | 0x141060944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::TiXmlNode | 0x141061024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToDocument | 0x141061088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::LinkEndChild | 0x141061104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlElement::Attribute | 0x141061264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlElement::Attribute | 0x141061312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::TiXmlDocument | 0x141061392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::TiXmlDocument | 0x141061504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::LoadFile | 0x141061680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::CopyTo | 0x141062256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::Clone | 0x141062400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlHandle::FirstChild | 0x141062496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::LoadFile | 0x141062608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::LoadFile | 0x141062928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlParsingData::Stamp | 0x141062960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::SetError | 0x141063296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::ServerOptionLoad | 0x141063808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::GetServerContents | 0x141064816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitGameDB | 0x141065248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CompareCashShop | 0x141065424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | STGMCashItemList::~STGMCashItemList | 0x141192512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_BANNER_LIST::~ST_BANNER_LIST | 0x141192512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitCharacterInfo | 0x141207328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CheckMazeOpenTime | 0x141207520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::GetOperationInfoTable | 0x141208288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitWeeklyEventGroupID | 0x141231568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitMaze | 0x141231824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_GM_ROULETTE_EVENT::~PS_GM_ROULETTE_EVENT | 0x141234032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadFactionInfo | 0x141234096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitInfiniteTowerTable | 0x141234560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitTitleOpenCondition | 0x141235056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadGachaGroupTable | 0x141235456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitDefaultPhotoItemID | 0x141236592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitQuestTable | 0x141237008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitPCCostume | 0x141240016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitPCAkashic | 0x141240464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | PS_GM_ROULETTE_EVENT::Clear | 0x141243152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Init | 0x141249296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitPartyRevise | 0x141265040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashshopLoad | 0x141265376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashshopLoad_DBConnect | 0x141267248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::BannerLoad | 0x141267456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadGMTLeagueInfo | 0x141268160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitRankingInfoTable | 0x141268752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadKRRData | 0x141269344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitNetCafeMissionList | 0x141270336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Clear | 0x141276032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitWorldModeGroup | 0x141278240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitWorldModeDate | 0x141278848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitRoguelikeData | 0x141279456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitDayEvent | 0x141282688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::SetStatusTable | 0x141284016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitLevelUpMail | 0x141286544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | ST_CASH_SHOP_TAB_LIST::~ST_CASH_SHOP_TAB_LIST | 0x141287408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::AddChannelInfo | 0x141287424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::AddChannelDistrict6Info | 0x141287744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::ServerChannelInfoLoad | 0x141288512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::ServerChannelDistrict6InfoLoad | 0x141288992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::~XResourceMgr | 0x141289536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::XResourceMgr | 0x141295440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashShopTabLoad | 0x141299312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashShopTabLoad_DBConnect | 0x141300800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitCommonDB | 0x141301024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load | 0x141302480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnConnect | 0x141302704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnDisConnect | 0x141302736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnNotConnect | 0x141302768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::StartUp | 0x141302800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::SetMyInfo | 0x141302976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::CalculateThreadStatus | 0x141303776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::CObserveSocket | 0x141304448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::SendReportServerStatus | 0x141304640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnUpdate | 0x141305008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ACHIEVEMENT | 0x141524528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ACHIEVEMENT_BEGIN | 0x141526304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ACHIEVEMENT_EMBLEM | 0x141527232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_COMBINATION | 0x141528384 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_DISASSEMBLE | 0x141529408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_MAKE | 0x141530480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_RANDOM_GROUP | 0x141531888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_RANDOM_GROUP_IN | 0x141533744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_RECORDS | 0x141535600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_SLOT_EXTEND | 0x141538256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_APPEARANCE | 0x141539168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AURA | 0x141540160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BANK_SLOT_EXTEND | 0x141541312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BATTLE_ARENA_INFO | 0x141542736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BOOSTER | 0x141543712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACH_SET | 0x141546432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHCOMPOSE_RANK | 0x141547216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHCOMPOSE_THEME | 0x141548304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHCOMPOSE_TYPE | 0x141549344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHDATA | 0x141550320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BUFF | 0x141551360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CASHBILLING_INFO | 0x141553472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CASHSHOP | 0x141555488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CASHSHOP_TAB | 0x141557232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHANGE_MOB_ID | 0x141559280 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHARACTER_INFO | 0x141560192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHARACTER_PARTS | 0x141565584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHATTINGCOMMAND | 0x141566448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ACCESS_REWARD | 0x141567376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ATTENDANCE_INFO | 0x141568672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ATTENDANCE_REWARD | 0x141569584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ATTENDANCE_STREAK | 0x141571056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CLASSBATTLE_ROLE | 0x141572176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COLOR | 0x141573392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COMBO_BUFF | 0x141574176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COMMON | 0x141576096 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COSTUME_SOCKET | 0x141576880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CREATE_CLOTH | 0x141578464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CREATEOPTION | 0x141579520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CREDIT_RAISE | 0x141581072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CULTIVATION | 0x141582112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CUSTOMER_BENEFIT | 0x141583456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CUSTOMER_GRADE | 0x141584576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAILY_MISSION | 0x141586176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAILYMAZE_PORTAL | 0x141588192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAY_EVENT | 0x141591312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAY_EVENT_BOOSTER | 0x141593776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DECK_BONUS | 0x141595776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DEFENSIVE_WEAPON | 0x141596816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DISASSEMBLE | 0x141597664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DISTRICT | 0x141600080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DISTRICT_TRANSPORT | 0x141601568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DIVERGENCE | 0x141603008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DIVISIONRANKREWARD | 0x141604432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROP | 0x141605632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROP_GROUP | 0x141611600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROP_GROUP_CHARACTER | 0x141614592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROPRATE_LEVEL | 0x141615952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROPRATE_MEMBER | 0x141616800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROPRATE_MOB | 0x141617648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DS_POINT | 0x141618496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DYE | 0x141620960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DYE_INFO | 0x141621904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ECHELON | 0x141624576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_EQUALIZER_INFO | 0x141625680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_EVENT_CONDITION | 0x141626464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_FACTION | 0x141627520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_FRAGMENT_EXCHANGE | 0x141628512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_FRAGMENT_EXTRACTION | 0x141630432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_GACHA_GROUP | 0x141633744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_GEAR_RATE | 0x141635776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_GESTURE | 0x141636624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_HELPER | 0x141637840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_HELPER_REWARD | 0x141638688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_HIDDEN_EVENT | 0x141639952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INFINITE_TOWER | 0x141641488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INTERACTION_ITEM | 0x141643568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INTERACTION_OBJECT | 0x141646592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INVEN_SLOT_EXTEND | 0x141648992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM | 0x141651024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_CLASSIFY | 0x141655296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_CLASSREWARD_LINK | 0x141656880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_COSTUMESET | 0x141658176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_COUNTBOX | 0x141660016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_COUNTBOX_GROUP | 0x141662496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_ENDURANCE | 0x141664944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_EVOLUTION | 0x141666352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_EXTRACTION | 0x141668208 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_LIMIT | 0x141669056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_PACKAGE | 0x141669920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_RANDOMBOX | 0x141673360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_RANK_RATE | 0x141677104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_REPAIR | 0x141677952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_SCRIPT | 0x141679760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_SETITEM | 0x141680480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_SIMILARGROUP | 0x141685328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE | 0x141686464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE_CHANGE | 0x141688320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE_GROUP | 0x141689360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE_VALUE | 0x141691216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEAGUE_INFO | 0x141692480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEAGUE_SKILL | 0x141693536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEVEL_MAIL | 0x141694880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEVELUP_POINT | 0x141696912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAKE | 0x141697936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_ENTER_COUNT_GROUP | 0x141699888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_INFO | 0x141701264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_OPEN_GROUP | 0x141705024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_OPENCONTROL | 0x141706400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_DIFFICULTY | 0x141707312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_ITEM | 0x141708224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_LEVEL | 0x141710688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_NORMAL | 0x141711600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_PARTYVALUE | 0x141712848 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_RANK | 0x141714192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_REVISION | 0x141715344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_SOULVAPER | 0x141716272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_STANDARD | 0x141717200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_TIME | 0x141718784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MILEAGE | 0x141719776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_CARDMATCH_CARD | 0x141720752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_CARDMATCH_RULE | 0x141721536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_CLASS_CORRECTION | 0x141722448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_CLASS_STARTSKILL | 0x141724000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_REWARD | 0x141725632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_SECTOR_INFO | 0x141726624 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_SKILL_EDIT | 0x141728272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_UPGRADE | 0x141729120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_UPGRADE_GROUP | 0x141730320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_DEFENCE | 0x141735296 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_DISTRICT6 | 0x141737072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_DISTRICT6_DATE | 0x141739568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_OPERATION | 0x141741392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_SURVIVAL | 0x141743600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER | 0x141745104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_BROKEN_PARTS | 0x141753712 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_EXP | 0x141754768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_PARTS | 0x141756064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_WEAPON | 0x141756784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MYROOM_FURNITURE | 0x141757872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MYROOM_GREED | 0x141759584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MYROOM_INFO | 0x141761456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_NAMEFILTER | 0x141763328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_NPC | 0x141764112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_NPC_PARTS | 0x141766144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_OPERATION_INFO | 0x141766928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_OPTION_STRING | 0x141770800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYEXP_LEVEL | 0x141771696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYEXP_MEMBER | 0x141772544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYEXP_MOB | 0x141773520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYREVISE | 0x141774368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_AKASHIC | 0x141775664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_COSTUME | 0x141776512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_REWARD_SYSTEM | 0x141777360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_REWARD_SYSTEM_MONTH | 0x141778880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PHOTO_ITEM | 0x141780448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_POLLEN | 0x141781472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PROVIDE_ITEM | 0x141782544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_CHAPTER | 0x141787888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_CONDITION | 0x141789520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_EPISODE | 0x141792864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_REWARD | 0x141800048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_SEASON | 0x141801344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_CHANGE | 0x141802592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_GET | 0x141803872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_GET_GROUP | 0x141809696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_OPTION | 0x141812272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_CHANGE | 0x141817520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_INFO | 0x141818752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_REWARD | 0x141819952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_REWARD_TOTALPOINT | 0x141821616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REINFORCE | 0x141822720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REINFORCE_EXCHANGE | 0x141829936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REINFORCE_OPTION | 0x141831056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REPACKAGECOSTUME | 0x141832976 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ROOMRANK_REWARD | 0x141834448 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SECTORQUEST | 0x141835584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SELECT_ITEM | 0x141837344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SERVERINFO | 0x141839856 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SHOP | 0x141840704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL | 0x141842560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL_ACTION | 0x141847696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL_OPTION | 0x141849056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL_SLOT_EXTEND | 0x141849968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOCIAL_ITEM | 0x141850880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOCIAL_MOTION | 0x141852080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOCKET | 0x141852992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOUL_GUAGE | 0x141854752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOUL_METRY | 0x141855888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOULSTONE_LEVELUP | 0x141858016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPAWN_GROUP | 0x141859632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPAWNBOX_FRONTLINE_01 | 0x141861008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPAWNBOX_RSQUARE_01 | 0x141862032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPECIALMAZE_OPEN | 0x141863056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_STATUS | 0x141864432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SYSTEMMAIL | 0x141868464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SYSTEMMAIL_ADD | 0x141869376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TALK | 0x141871056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TALK_LIST | 0x141873936 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TALK_STRING | 0x141876224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TITLE_INFO | 0x141876944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TITLE_REWARD | 0x141878928 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TITLE_STRING | 0x141880112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TRANSPORT_INFO | 0x141880832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_UNITY_EVENT | 0x141882256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_VERSION | 0x141884800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WARLORD_EVENT | 0x141885792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WARLORD_GUI | 0x141887360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEAPON_RATE | 0x141888288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEEK_DAY | 0x141889136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEEK_GROUP | 0x141890640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEEK_MISSION | 0x141892064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WORLD_EVENT | 0x141893488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WORLD_EVENT_REWARD | 0x141894560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::TableLoad | 0x141895728 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CTableLoader_S::GetWString | 0x141899568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CTableLoader_S::~CTableLoader_S | 0x141899888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | CTableLoader_S::GetString | 0x141899968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadVersion | 0x141900240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::MakeMD5 | 0x141900656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CheckSum | 0x141900992 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ACHIEVEMENT | 0x141901456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ACHIEVEMENT_BEGIN | 0x141903088 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ACHIEVEMENT_EMBLEM | 0x141903808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_COMBINATION | 0x141904784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_DISASSEMBLE | 0x141905616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_MAKE | 0x141906464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_RANDOM_GROUP | 0x141907680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_RANDOM_GROUP_IN | 0x141909344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_RECORDS | 0x141911008 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_SLOT_EXTEND | 0x141913472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_APPEARANCE | 0x141914176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AURA | 0x141914944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BANK_SLOT_EXTEND | 0x141915888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BATTLE_ARENA_INFO | 0x141917120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BOOSTER | 0x141917888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACH_SET | 0x141920544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHCOMPOSE_RANK | 0x141921104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHCOMPOSE_THEME | 0x141922016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHCOMPOSE_TYPE | 0x141922864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHDATA | 0x141923632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BUFF | 0x141924496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CASHBILLING_INFO | 0x141926512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CASHSHOP | 0x141928304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CASHSHOP_TAB | 0x141929888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHANGE_MOB_ID | 0x141931808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHARACTER_INFO | 0x141932528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHARACTER_PARTS | 0x141937872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHATTINGCOMMAND | 0x141938512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ACCESS_REWARD | 0x141939232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ATTENDANCE_INFO | 0x141940320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ATTENDANCE_REWARD | 0x141941040 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ATTENDANCE_STREAK | 0x141942304 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CLASSBATTLE_ROLE | 0x141943216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COLOR | 0x141944224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COMBO_BUFF | 0x141944784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COMMON | 0x141946576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COSTUME_SOCKET | 0x141947152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CREATE_CLOTH | 0x141948544 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CREATEOPTION | 0x141949392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CREDIT_RAISE | 0x141950752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CULTIVATION | 0x141951616 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CUSTOMER_BENEFIT | 0x141952752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CUSTOMER_GRADE | 0x141953792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAILY_MISSION | 0x141955200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAILYMAZE_PORTAL | 0x141957072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAY_EVENT | 0x141960144 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAY_EVENT_BOOSTER | 0x141962496 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DECK_BONUS | 0x141964352 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DEFENSIVE_WEAPON | 0x141965200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DISASSEMBLE | 0x141965824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DISTRICT | 0x141968064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DISTRICT_TRANSPORT | 0x141969376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DIVERGENCE | 0x141970640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DIVISIONRANKREWARD | 0x141971904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROP | 0x141972880 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROP_GROUP | 0x141979904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROP_GROUP_CHARACTER | 0x141982752 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROPRATE_LEVEL | 0x141983904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROPRATE_MEMBER | 0x141984560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROPRATE_MOB | 0x141985216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DS_POINT | 0x141985872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DYE | 0x141988176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DYE_INFO | 0x141988912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ECHELON | 0x141991504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_EQUALIZER_INFO | 0x141992400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_EVENT_CONDITION | 0x141992960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_FACTION | 0x141993792 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_FRAGMENT_EXCHANGE | 0x141994560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_FRAGMENT_EXTRACTION | 0x141996272 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_GACHA_GROUP | 0x141999920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_GEAR_RATE | 0x142001824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_GESTURE | 0x142002480 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_HELPER | 0x142003520 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_HELPER_REWARD | 0x142004160 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_HIDDEN_EVENT | 0x142005232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INFINITE_TOWER | 0x142006560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INTERACTION_ITEM | 0x142008464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INTERACTION_OBJECT | 0x142011392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INVEN_SLOT_EXTEND | 0x142013600 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM | 0x142015472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_CLASSIFY | 0x142019568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_CLASSREWARD_LINK | 0x142020960 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_COSTUMESET | 0x142022032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_COUNTBOX | 0x142023664 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_COUNTBOX_GROUP | 0x142026032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_ENDURANCE | 0x142028288 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_EVOLUTION | 0x142029584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_EXTRACTION | 0x142031248 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_LIMIT | 0x142031872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_PACKAGE | 0x142032512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_RANDOMBOX | 0x142035760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_RANK_RATE | 0x142039872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_REPAIR | 0x142040528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_SCRIPT | 0x142042336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_SETITEM | 0x142042816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_SIMILARGROUP | 0x142048528 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE | 0x142049440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE_CHANGE | 0x142051120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE_GROUP | 0x142051968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE_VALUE | 0x142053648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEAGUE_INFO | 0x142054720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEAGUE_SKILL | 0x142055696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEVEL_MAIL | 0x142056832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEVELUP_POINT | 0x142058736 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAKE | 0x142059568 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_ENTER_COUNT_GROUP | 0x142061344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_INFO | 0x142062560 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_OPEN_GROUP | 0x142066176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_OPENCONTROL | 0x142067392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_DIFFICULTY | 0x142068080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_ITEM | 0x142068800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_LEVEL | 0x142071056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_NORMAL | 0x142071776 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_PARTYVALUE | 0x142072800 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_RANK | 0x142074032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_REVISION | 0x142075024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_SOULVAPER | 0x142075760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_STANDARD | 0x142076464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_TIME | 0x142077888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MILEAGE | 0x142078688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_CARDMATCH_CARD | 0x142079488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_CARDMATCH_RULE | 0x142080048 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_CLASS_CORRECTION | 0x142080768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_CLASS_STARTSKILL | 0x142082240 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_REWARD | 0x142083648 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_SECTOR_INFO | 0x142084432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_SKILL_EDIT | 0x142085888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_UPGRADE | 0x142086512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_UPGRADE_GROUP | 0x142087488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_DEFENCE | 0x142093104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_DISTRICT6 | 0x142094704 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_DISTRICT6_DATE | 0x142097024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_OPERATION | 0x142098672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_SURVIVAL | 0x142100672 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER | 0x142102016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_BROKEN_PARTS | 0x142111920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_EXP | 0x142112768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_PARTS | 0x142113840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_WEAPON | 0x142114320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MYROOM_FURNITURE | 0x142115184 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MYROOM_GREED | 0x142116720 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MYROOM_INFO | 0x142118368 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_NAMEFILTER | 0x142120016 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_NPC | 0x142120576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_NPC_PARTS | 0x142122512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_OPERATION_INFO | 0x142123072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_OPTION_STRING | 0x142127312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYEXP_LEVEL | 0x142127984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYEXP_MEMBER | 0x142128640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYEXP_MOB | 0x142129408 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYREVISE | 0x142130064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_AKASHIC | 0x142131216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_COSTUME | 0x142131840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_REWARD_SYSTEM | 0x142132464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_REWARD_SYSTEM_MONTH | 0x142133808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PHOTO_ITEM | 0x142135136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_POLLEN | 0x142135984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PROVIDE_ITEM | 0x142136816 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_CHAPTER | 0x142142784 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_CONDITION | 0x142144192 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_EPISODE | 0x142147328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_REWARD | 0x142155440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_SEASON | 0x142156512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_CHANGE | 0x142157536 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_GET | 0x142158608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_GET_GROUP | 0x142165392 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_OPTION | 0x142167840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_CHANGE | 0x142173808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_INFO | 0x142174864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_REWARD | 0x142175888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_REWARD_TOTALPOINT | 0x142177344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REINFORCE | 0x142178256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REINFORCE_EXCHANGE | 0x142187024 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REINFORCE_OPTION | 0x142187920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REPACKAGECOSTUME | 0x142189680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ROOMRANK_REWARD | 0x142190944 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SECTORQUEST | 0x142191904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SELECT_ITEM | 0x142193456 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SERVERINFO | 0x142195808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SHOP | 0x142196416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL | 0x142198112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL_ACTION | 0x142203168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL_OPTION | 0x142204320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL_SLOT_EXTEND | 0x142205056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOCIAL_ITEM | 0x142205760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOCIAL_MOTION | 0x142206768 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOCKET | 0x142207488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOUL_GUAGE | 0x142209072 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOUL_METRY | 0x142210032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOULSTONE_LEVELUP | 0x142211984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPAWN_GROUP | 0x142213376 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPAWNBOX_FRONTLINE_01 | 0x142214592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPAWNBOX_RSQUARE_01 | 0x142215424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPECIALMAZE_OPEN | 0x142216256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_STATUS | 0x142217472 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SYSTEMMAIL | 0x142222400 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SYSTEMMAIL_ADD | 0x142223120 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TALK | 0x142224656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TALK_LIST | 0x142227328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TALK_STRING | 0x142229504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TITLE_INFO | 0x142229984 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TITLE_REWARD | 0x142231904 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TITLE_STRING | 0x142232864 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TRANSPORT_INFO | 0x142233344 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_UNITY_EVENT | 0x142234592 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_VERSION | 0x142237056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WARLORD_EVENT | 0x142237824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WARLORD_GUI | 0x142239232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEAPON_RATE | 0x142239952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEEK_DAY | 0x142240608 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEEK_GROUP | 0x142241952 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEEK_MISSION | 0x142243200 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WORLD_EVENT | 0x142244416 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WORLD_EVENT_REWARD | 0x142245264 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load | 0x142246224 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | md5_init | 0x142251824 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | md5_append | 0x142251872 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | md5_finish | 0x142252128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __security_check_cookie | 0x142253584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __ArrayUnwind | 0x142253748 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _onexit | 0x142254136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | atexit | 0x142254312 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | mainCRTStartup | 0x142255032 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __GSHandlerCheckCommon | 0x142255080 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __GSHandlerCheck | 0x142255180 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __GSHandlerCheck_EH | 0x142255252 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __chkstk | 0x142255424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __report_gsfailure | 0x142255504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __CxxUnhandledExceptionFilter | 0x142255900 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __CxxSetUnhandledExceptionFilter | 0x142255968 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _RTC_Initialize | 0x142256000 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _RTC_Terminate | 0x142256056 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _ValidateImageBase | 0x142256128 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _FindPESection | 0x142256176 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _IsNonwritableInCurrentImage | 0x142256256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _matherr | 0x142256336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _setargv | 0x142256336 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __security_init_cookie | 0x142256340 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer | WorldModeProcess.h | CServerWorldModeProcess::Parse | - | verified | IDA无此函数(二进制无); 源码为GreenDamTan扩展:stub实现仅LogDebug+return true | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.cpp | CLogicThreadProc::OnUpdate` worker 0/1/2 routing skeleton | 0x1400D0660 | verified | IDA 0x1400D0660 + worker 0:PartyManager.Clear+PartyMatchingMgr.OnUpdate+ForceManager.Clear+ForceMatchingMgr.OnUpdate+ModeMazeMatchingMgr.OnUpdate + worker 1:LeagueManager.OnUpdate + worker 2:RecruitManager.OnUpdate | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | TXSingleton<CLogicThreadManager>::TXSingleton<CLogicThreadManager> | 0x140389152 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::OnFinalizeThread | 0x140391216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFriendRecruitManager::IsLoad | 0x140405808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CKernelObject::CKernelObject | 0x140847104 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CKernelObject::~CKernelObject | 0x140847232 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CWaitableObject::~CWaitableObject | 0x140847328 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CWaitableObject::Wait | 0x140847440 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::CThreadBase | 0x140847488 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CWaitableObject::CWaitableObject | 0x140847760 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CKernelEvent::~CKernelEvent | 0x140847808 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | THREAD_PROC_ARG::THREAD_PROC_ARG | 0x140847840 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | THREAD_PROC_ARG::~THREAD_PROC_ARG | 0x140847888 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::~CThreadBase | 0x140847920 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::StopThread | 0x140848064 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::CreateThread | 0x140848112 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | _SetThreadName | 0x140848256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::CLogicThreadProc | 0x140848320 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::~CLogicThreadProc | 0x140848432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::AddJob | 0x140848512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::IsShutdown | 0x140848640 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::GetCurThreadIndex | 0x140848656 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::OnInitializeThread | 0x140848688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::ThreadProc | 0x140848832 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::InitData | 0x140849216 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::LoadData | 0x140849360 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::OnUpdate | 0x140849504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::WaitForInit | 0x140849696 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::CheckFPS | 0x140849744 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::CLogicThreadManager | 0x140850256 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::~CLogicThreadManager | 0x140850464 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::Start | 0x140850512 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::End | 0x140850912 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::DoJob | 0x140851168 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::Clear | 0x140851504 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::CreateWorkerThread | 0x140851632 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::~CFThread<CLogicThreadProc> | 0x140852432 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::MakeInstance | 0x140852576 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::GetProcInstance | 0x140852688 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::Create | 0x140853136 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::CFThread<CLogicThreadProc> | 0x140853424 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::ReleaseInstance | 0x140853584 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::RunThread | 0x140853680 | pending | PDB dump symbols | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::CallThreadHandlerProc | 0x140854032 | pending | PDB dump symbols | 否 | - |
