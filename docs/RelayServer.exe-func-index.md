# RelayServer.exe 函数索引

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 | 是否验证 | 验证结论 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Common/XNet/XCommon | PSServer.h | PS_REQ_LEAGUE_INVEN_INFO::operator<<(XPacket&, ...) | 0x1400E82D0 | verified | IDA 0x1400E82D0 + nLeagueID+dwNpcID+shStartPos+shEndPos; 源码匹配 | 否 | 一致 |
| Common/XNet/XCommon | PSServer.h | PS_ITEM_BROACH_LIST operator>> | 0x1400EAED0 | verified | IDA 0x1400EAED0 + uint16 count+循环读ST_ITEM_BROACH+push_back; 源码匹配 | 否 | 一致 |
| Common/XNet/XCommon | PSServer.h | PS_ITEM_PACKAGE_LIST operator>> | 0x1400EB7A0 | verified | IDA 0x1400EB7A0 + int32 nCount+循环读PS_ITEM_PACKAGE+push_back; 修复:count类型从uint16改为int32对齐IDA | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::CCommunity | 0x140001000 | verified | IDA 0x140001000 + 默认构造函数; 源码UserObject.h:148 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::~CCommunity | 0x1400010D0 | verified | IDA 0x1400010D0 + 默认析构函数; 源码UserObject.h:148 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::Clear | 0x140001110 | verified | IDA 0x140001110 + boost::multi_index::clear()+标志位重置; 源码UserObject.h:150用std::vector简化 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsValidCommunityType | 0x140001190 | verified | IDA 0x140001190 + byType>=1 && byType<=3; 源码UserObject.h:399 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsValiedListCount | 0x1400011C0 | verified | IDA 0x1400011C0 + switch(1/2/3/101)+count阈值100/20/无限制/50; 源码UserObject.h:407 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsFriend(UCID,Type) | 0x140001270 | verified | IDA 0x140001270 + hashed_index find+GetType检查; 源码UserObject.h:161用std::find_if简化 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsFriend(wchar_t*,Type) | 0x1400013E0 | verified | IDA 0x1400013E0 + 遍历查找名字+Type检查; 源码UserObject.h:173 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsBlockList(UCID) | 0x140001600 | verified | IDA 0x140001600 + hashed_index find; 源码UserObject.h:186用std::any_of简化 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsBlockList(wchar_t*) | 0x140001710 | verified | IDA 0x140001710 + 遍历查找名字; 源码UserObject.h:193 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::AddFriend | 0x1400018D0 | verified | IDA 0x1400018D0 + null检查+find去重+insert; 源码UserObject.h:321用std::vector简化 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::AddBlock | 0x1400019F0 | verified | IDA 0x1400019F0 + null检查+find去重+insert; 源码UserObject.h:358用std::vector简化 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetFriendUCID | 0x140001AE0 | verified | IDA 0x140001AE0 + 遍历查找名字返回UCID; 源码UserObject.h:205 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetFriendList(vector) | 0x140001C90 | verified | IDA 0x140001C90 + 遍历+Type过滤; 源码UserObject.h:229 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetFriendList(PS_FRIEND_LIST) | 0x140001D80 | verified | IDA 0x140001D80 + 遍历+Type过滤+push_back; 源码UserObject.h:218 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetBlcokList | 0x140001E80 | verified | IDA 0x140001E80 + 遍历push_back; 源码UserObject.h:239 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetFriendType | 0x140001F30 | verified | IDA 0x140001F30 + find+GetType; 源码UserObject.h:261 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::IsChangeFriendInfo | 0x140001FD0 | verified | IDA 0x140001FD0 + find+字段比较; 源码UserObject.h:268 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetFriendInfo | 0x1400021B0 | verified | IDA 0x1400021B0 + find+拷贝信息; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::UpdateFriendInfo | 0x140002290 | verified | IDA 0x140002290 + find+bLogin分支+字段更新+strMemo拷贝; 源码UserObject.h:284 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::DeleteFriend | 0x1400024D0 | verified | IDA 0x1400024D0 + find+erase; 源码UserObject.h:334用remove_if简化 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CCommunity::DeleteBlockList | 0x140002570 | verified | IDA 0x140002570 + find+erase; 源码UserObject.h:370 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::CheckFriendInvite | 0x140002610 | verified | IDA 0x140002610 + IsFriend检查+byResult设置; 源码UserObject.h:435 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::CheckFriendAccept | 0x1400026E0 | verified | IDA 0x1400026E0 + IsValiedListCount检查+nResult设置; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::CheckBlockAdd | 0x1400027B0 | verified | IDA 0x1400027B0 + IsBlockList检查+nResult设置; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetLastFriendWaitList | 0x140002890 | verified | IDA 0x140002890 + 遍历type=2找最新; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::CheckRecruitListTime | 0x140002A20 | verified | IDA 0x140002A20 + 时间检查; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::InitRecruitListTime | 0x140002A90 | verified | IDA 0x140002A90 + 时间初始化; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::AddFriendPoint | 0x140002AC0 | verified | IDA 0x140002AC0 + 查找+积分添加; 源码UserObject.h实现 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | wmemcmp | 0x14000BA20 | verified | IDA 0x14000BA20 + CRT函数; 源码使用std::wstring比较 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | ST_FRIEND_COMMUNITY::ST_FRIEND_COMMUNITY | 0x14000BE90 | verified | IDA 0x14000BE90 + byState=0+strMemo[0]=0; 源码UserObject.h结构体初始化 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | STCharInfo::~STCharInfo | 0x14000B510 | verified | IDA 0x14000B510 + 默认析构; 源码UserObject.h结构体析构 | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | wcscpy_s<31> | 0x14000BD10 | verified | IDA 0x14000BD10 + 编译器生成模板函数; 源码使用std::wstring拷贝 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | wmemmove | 0x14000BFF0 | verified | IDA 0x14000BFF0 + CRT函数; 源码使用std::wstring操作 | 是 | 语义等效 |
| GameServer/XRelayServer | UserObject.h | CFriendMember::GetType | - | verified | 源码UserObject.h内联getter; IDA无单独函数(内联到调用点) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CBlockUser::GetName | - | verified | 源码UserObject.h内联getter; IDA无单独函数(内联到调用点) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CFriendMember::GetName | - | verified | 源码UserObject.h内联getter; IDA无单独函数(内联到调用点) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CBlockUser::GetUCID | - | verified | 源码UserObject.h内联getter; IDA无单独函数(boost::multi_index内部使用) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CFriendMember::GetUCID | - | verified | 源码UserObject.h内联getter; IDA无单独函数(boost::multi_index内部使用) | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::CExchangePriceMgr | 0x14000CA20 | verified | IDA 0x14000CA20 + CFSRWLock初始化+m_mapPriceHistory.clear+m_n64DBRequestDate=0; 源码实现内联于RelayServer.h | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::~CExchangePriceMgr | 0x14000CAA0 | verified | IDA 0x14000CAA0 + map析构; 源码实现内联于RelayServer.h | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::GetPriceList | 0x14000CAF0 | verified | IDA 0x14000CAF0 + 读锁+find+m_mapPriceHistory+填充高低价/平均价+逆序遍历mapTimeList; 源码RelayServer.cpp:3493 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::LoadPriceList | 0x14000CD10 | verified | IDA 0x14000CD10 + 遍历vecHistory调用AddPriceList+SetPriceInfo; 源码RelayServer.cpp:3483 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::SetPriceInfo | 0x14000CDC0 | verified | IDA 0x14000CDC0 + 写锁+find/update m_mapPriceHistory; 源码RelayServer.cpp:3533 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::AddPriceList | 0x14000CE90 | verified | IDA 0x14000CE90 + 写锁+find/insert m_mapPriceHistory+mapTimeList[tRegDate]+更新统计; 源码RelayServer.cpp:3547 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::ST_EXCHANGE_PRICE_HISTORY_INFO::~ST_EXCHANGE_PRICE_HISTORY_INFO | 0x14000D350 | verified | IDA 0x14000D350 + 默认析构; 源码RelayServer.h | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::DeletePriceList_Old | 0x14000D3B0 | verified | IDA 0x14000D3B0 + 写锁+find+pop_back; 源码RelayServer.cpp:3598 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::GetPriceListCount | 0x14000D4E0 | verified | IDA 0x14000D4E0 + 读锁+find+遍历mapTimeList统计count; 源码RelayServer.cpp:3616 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::GetDBRequestDate | 0x14000D610 | verified | IDA 0x14000D610 + 读锁+返回m_n64DBRequestDate(若<=946684800则初始化); 源码RelayServer.cpp:3520 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | CExchangePriceMgr::ST_EXCHANGE_PRICE_HISTORY_INFO::ST_EXCHANGE_PRICE_HISTORY_INFO | 0x140010630 | verified | IDA 0x140010630 + dwItemID初始化; 源码RelayServer.cpp:3473 | 是 | 一致 |
| GameServer/XCore/XServer | CFSRWLock.h | CFSRWLock::unlock | 0x140012E80 | verified | IDA 0x140012E80 + ReleaseSRWLockExclusive; 源码CFSRWLock.h:38用std::shared_mutex简化 | 是 | 语义等效 |
| GameServer/XCore/XServer | CFSRWLock.h | CFAutoSlimWriteLock::~CFAutoSlimWriteLock | 0x140012EE0 | verified | IDA 0x140012EE0 + 析构调用UnlockWrite; 源码CFSRWLock.h:74 | 是 | 一致 |
| GameServer/XCore/XServer | CFSRWLock.h | CFSRWLock::lock | 0x140012EC0 | verified | IDA 0x140012EC0 + AcquireSRWLockExclusive; 源码CFSRWLock.h:39用std::shared_mutex简化 | 是 | 语义等效 |
| GameServer/XCore/XServer | CFSRWLock.h | CFAutoSlimWriteLock::CFAutoSlimWriteLock | 0x140012EE0 | verified | IDA 0x140012EE0 + 构造调用LockWrite; 源码CFSRWLock.h:67 | 是 | 一致 |
| GameServer/XCore/XServer | CFSRWLock.h | CFSRWLock::unlock_shared | 0x140012F20 | verified | IDA 0x140012F20 + ReleaseSRWLockShared; 源码CFSRWLock.h:40用std::shared_mutex简化 | 是 | 语义等效 |
| GameServer/XCore/XServer | CFSRWLock.h | CFAutoSlimReadLock::~CFAutoSlimReadLock | 0x140012F80 | verified | IDA 0x140012F80 + 析构调用UnlockRead; 源码CFSRWLock.h:56 | 是 | 一致 |
| GameServer/XCore/XServer | CFSRWLock.h | CFSRWLock::lock_shared | 0x140012F60 | verified | IDA 0x140012F60 + AcquireSRWLockShared; 源码CFSRWLock.h:41用std::shared_mutex简化 | 是 | 语义等效 |
| GameServer/XCore/XServer | CFSRWLock.h | CFAutoSlimReadLock::CFAutoSlimReadLock | 0x140012F80 | verified | IDA 0x140012F80 + 构造调用LockRead; 源码CFSRWLock.h:48 | 是 | 一致 |
| GameServer/XCore/XServer | CFSRWLock.h | CFSRWLock::CFSRWLock | 0x140012FC0 | verified | IDA 0x140012FC0 + InitializeSRWLock; 源码CFSRWLock.h:18用std::shared_mutex简化 | 是 | 语义等效 |
| GameServer/XRelayServer | Force.cpp | CForce::AddMember | - | verified | RelayServer decompile force-member add path + `m_mapForceMember` insertion | 否 | 一致 |
| GameServer/XRelayServer | Force.cpp | CForce::SetMemberEnterMap(uint32, UXMapID) | 0x140094650 | verified | export-for-ai 0x140094650 + find + SetEnterMap | 否 | 一致 |
| GameServer/XRelayServer | Force.h | UXMapID::UXMapID | 0x140005ED0 | verified | IDA 0x140005ED0 + 默认构造; 源码Party.h内联构造 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | UXMapID::operator= | - | verified | 源码Party.h内联赋值; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::CForce() | 0x1400132B0 | verified | IDA 0x1400132B0 + 默认构造+调用CParty::Clear; 源码Force.h:35继承CParty | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::CForce(PS_REQ_FORCE_CREATE) | 0x140012FE0 | verified | IDA 0x140012FE0 + PS_REQ_FORCE_CREATE构造; 源码Force.cpp:51 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::~CForce | - | verified | 源码默认析构; IDA无显式析构函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::SetForceInfo | 0x140013320 | verified | IDA 0x140013320 + 设置dwForceID/dwMasterID/uxMazeID/byForceType+遍历vecForceMember; 源码Force.cpp:126 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::SetMemberInfo(ST_FORCE_MEMBER) | 0x1400135A0 | verified | IDA 0x1400135A0 + find+Logout处理+SetMemberInfo; 源码Force.cpp:73 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::AddMember | 0x140013830 | verified | IDA 0x140013830 + 创建CForceMember并插入m_mapForceMember; 源码Force.cpp:67 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetForceMemberList | 0x140013950 | verified | IDA 0x140013950 + 遍历m_mapForceMember填充ST_PARTY_MEMBER_LIST; 源码Force.cpp:212 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::SendNameChange | 0x140013BA0 | verified | IDA 0x140013BA0 + 遍历成员+0xFA/0x20广播; 源码Force.cpp:242 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | TXSingleton<XRelayServer>::Instance | - | verified | 源码单例模板; IDA无单独函数(模板内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | wcscpy_s<21> | 0x140014190 | verified | IDA 0x140014190 + 编译器模板; 源码用std::wstring | 是 | 语义等效 |
| GameServer/XRelayServer | Force.h | CUserObject::GetServerID | - | verified | 源码内联getter; IDA无单独函数(boost::multi_index内部) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::GetMemberID | - | verified | 源码内联getter; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | PS_CHANGE_NAME::PS_CHANGE_NAME | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | ST_FORCE_MEMBER::ST_FORCE_MEMBER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | ST_PARTY_MEMBER::ST_PARTY_MEMBER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::SetMemberInfo | - | verified | 源码Force.cpp:17; IDA无单独函数(SetMemberInfo在CForce中处理) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | ST_FORCE_MEMBER::operator= | - | verified | 源码默认赋值; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | UXMapID::operator= | - | verified | 源码内联赋值; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::Logout | 0x1400147A0 | verified | IDA 0x1400147A0 + 超时计算; 源码Force.cpp:42(重复条目) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | XParse::operator<< | - | verified | 源码XPacket模板; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | XParse::SetDWORD | - | verified | 源码辅助函数; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::GetMemberInfo | - | verified | 源码Force.cpp:46; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | UXMapID::UXMapID | 0x140005ED0 | verified | IDA 0x140005ED0 + 默认构造(重复条目) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::Clear | 0x140060830 | verified | IDA 0x140060830 + 空函数(继承CParty::Clear) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::~CForceMember | - | verified | 源码默认析构; IDA无显式析构 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::ChangeMaster | 0x1400942B0 | verified | IDA 0x1400942B0 + find+检查bLogin+bLeave条件+m_dwMasterID更新; 源码Force.cpp:178继承CParty | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::Kickout | 0x140094360 | verified | IDA 0x140094360(CParty::Kickout)+find+erase; 源码Force.cpp:208继承CParty | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::FindNewMaster | 0x1400943E0 | verified | IDA 0x1400943E0(CParty::FindNewMaster)+遍历m_mapPartyMember+找非master成员; 源码Force.cpp:195继承CParty | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetForceInfo | - | verified | 源码Force.cpp:159; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetMemberInfo | - | verified | 源码Force.cpp:151; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::SetMemberEnterMap | 0x140094650 | verified | IDA 0x140094650 + find+SetEnterMap; 源码Force.cpp:118 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::SetEnterMap | 0x140095430 | verified | IDA 0x140095430 + m_uxEnterMap赋值; 源码Force.cpp:28 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::Login | - | verified | 源码Force.cpp:35; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForceMember::Logout | 0x1400147A0 | verified | IDA 0x1400147A0 + GetTickCount64+300000; 源码Force.cpp:42用steady_clock简化 | 是 | 语义等效 |
| GameServer/XRelayServer | Party.h | CParty::Clear | 0x140060830 | verified | IDA 0x140060830 + 空函数; 源码Party.cpp:29 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::SetMemberInfo(dwMemberID,uxMapID,nMaxHP) | 0x1400136A0 | verified | IDA 0x1400136A0 + find+nMaxHP+nHP+nMapID+nChannel+bLogin+Login+m_uxEnterMap; 源码Party.cpp:48 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetPartyID | 0x140014540 | verified | IDA 0x140014540 + return m_dwPartyID; 源码Party.h:49 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetMasterID | 0x14001BFC0 | verified | IDA 0x14001BFC0 + return m_dwMasterID; 源码Party.h:51 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetUserCount | 0x14001BFA0 | verified | IDA 0x14001BFA0 + return m_mapPartyMember.size; 源码Party.h:52 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::CLeague | 0x140064270 | verified | IDA 0x140064270 + map构造+queue构造+Clear; 源码League.cpp:8 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::Clear | 0x1400643C0 | verified | IDA 0x1400643C0 + m_nInventoryCount=0+m_nSyncCount=0; 源码League.cpp:14; 修正:m_nInventoryCount命名对齐IDA | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueInfo | 0x140064220 | verified | IDA 0x140064220 + qmemcpy 0x800; 源码League.cpp:20 | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.h | CLeagueMember::CLeagueMember | 0x140064080 | verified | IDA 0x140064080 + m_stMember构造+m_bEnrollBoard=1; 源码LeagueMember.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.h | CLeagueMember::Login | 0x1400640B0 | verified | IDA 0x1400640B0 + m_stMember.bLogin=1; 源码LeagueMember.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.h | CLeagueMember::LogOut | 0x1400640C0 | verified | IDA 0x1400640C0 + m_stMember.bLogin=0; 源码LeagueMember.cpp | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqInviteForce | 0x140014D30 | verified | IDA 0x140014D30 + GetUser(strName)→null(53111)/GetPartyUser→GetRewardState | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::{ReqJoinMember, ReqCreateForce} | 0x1400166F0 | verified | RelayServer decompile `0x1400166F0 / 0x1400149C0` + `main=8/sub=2,1` DB game packet + DBLog 23/5 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResJoinMember | 0x140016A60 | verified | RelayServer decompile `0x140016A60` + `0xFA/0x02` fanout + `AddRecruitMember | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::{ResUpdateMemberInfo, SendForceMessage, EnterServer, GetForce, ReqForceInfo} | 0x140017CC0 | verified | IDA `0x140017CC0 / 0x140017D50 / 0x140016380 / 0x1400148D0 / 0x140014970 / 0x140026960` + ResUpdateMemberInfo:XSendPacket(0xFA,5)+SendPacketAll; SendForceMessage:XSendPacket(0xFA,0x10)+stChatForce+psChatItemLinkInfo+SendPacketAll; EnterServer:GetForce+SetMemberInfo+m_mapForceUser+bReqForceInfo→GetPartyInfo+GetMemberInfo+SendPacket(0xFA,0xA)+master→GetPartyRecruitInfo+SendPacket(0xF4,0x2E); 差异:IDA中GetMemberInfo/dwForceID赋值在bReqForceInfo外,源码在内; GetForce(dwForceID):find+return; GetForce(UXActorID):GetPartyID+GetForce; ReqForceInfo:lambda检查IsState+GetForce+GetForceInfo+SendPacket(0xFA,0x1A) | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::{CreateForceMatching, DeleteForce, ReqForceInfo(actor-first fix)} | 0x140017FE0 | verified | IDA 0x140017FE0/0x140017440/0x140026960 + CreateForceMatching:GetOrCreateForce+SetMasterID+遍历vecForceMember→SetMemberInfo+AddPartyMember; DeleteForce:find+GetForceInfo+遍历vecForceMember→RemoveForceMember+erase; ReqForceInfo:IsState+GetForce+GetForceInfo+SendPacket(0xFA,0x1A); 修复:DeleteForce补充vecForceMember遍历和RemoveForceMember调用 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | UXActorID::UXActorID | - | verified | 源码结构体默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForce::GetForceID | - | verified | 源码Force.h内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CUserPartyInfo::GetCID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::CForceManager | - | verified | 源码ForceManager.h默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::~ClassFactory<CForce,64> | - | verified | 源码对象池模板析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::~CForceManager | - | verified | 源码默认析构; IDA无显式析构函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::GetForce(uint32) | 0x1400148D0 | verified | IDA 0x1400148D0 + find(m_mapForce)+return shared_ptr/end→空; 源码ForceManager.cpp:26 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::GetForce(UXActorID) | 0x140014970 | verified | IDA 0x140014970 + CPartyManager::GetPartyID+GetForce(uint32); 源码ForceManager.cpp:33 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqCreateForce | 0x1400149C0 | verified | IDA 0x1400149C0 + PS_REQ_FORCE_CREATE>>XSendDBPacket; 源码ForceManager.cpp:207 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::CreateForce | 0x140014A90 | verified | IDA 0x140014A90 + XSendPacket(0xFA,1)+make_shared<CForce>+insert+AddPartyMember×2+SendPacketAll+dwRecruitID→CreateForce+SendDBLog(23,14); 源码ForceManager.cpp:249 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqInviteForce | 0x140014D30 | verified | IDA 0x140014D30 + GetUser→null→SendForceErrorInvite(53111) + IsFriendBlock→SendForceErrorInvite(53113) + IsParty→SendForceErrorInvite(53145) + GetPartyRecruitInfo(byPartyGroupType!=2)→SendForceErrorInvite(53034) + m_mapForceInvite检查 + XSendPacket(0xFA,0xB)+dwUAID+byLevel+dwForceID+SendPacket; 源码ForceManager.cpp:311 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqAcceptForce | 0x140015530 | verified | IDA 0x140015530 + find(m_mapForceInvite) + nResult检查 + GetPartyUser+GetUser + null检查→SendForceErrorAccept(53111) + IsParty检查 + IsMaze检查 + m_mapForceUser检查 + 已有Force→GetMasterID比对 + GetUserCount<8检查 + ReqJoinMember/ReqCreateForce; 源码ForceManager.cpp:453 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqCancelForce | 0x140016200 | verified | IDA 0x140016200 + find(m_mapForceInvite) + GetUser + SendPacket(0xFA,0xD) + erase(m_mapForceInvite); 源码ForceManager.cpp:694 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::EnterServer | 0x140016380 | verified | IDA 0x140016380 + dwForceID检查 + find(m_mapForce) + LogError + SetMemberInfo + bReqForceInfo→GetForceInfo+GetMemberInfo+SendPacket(0xFA,0xA) + GetMasterID==dwMemberID→GetPartyRecruitInfo+SendPacket(0xF4,0x2E); 源码ForceManager.cpp:53 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_RES_FORCE_ENTER_SERVER::PS_RES_FORCE_ENTER_SERVER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_RES_FORCE_ENTER_SERVER::~PS_RES_FORCE_ENTER_SERVER | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqJoinMember | 0x1400166F0 | verified | IDA 0x1400166F0 + find(m_mapForce) + AddMember + AddPartyMember + GetUserCount==8→ClearRecruitDate + GetPartyUser+ClearRecruitDate + XSendDBPacket(8,2)+stAddMember+dwRecruitID + SendDBLog(23,5); 源码ForceManager.cpp:167 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResJoinMember | 0x140016A60 | verified | IDA 0x140016A60 + nErrorCode检查 + find(m_mapForce) + GetForceInfo + XSendPacket(0xFA,2)+stAddMember+stForceInfo + SendPacketAll + dwRecruitID→AddRecruitMember; 源码ForceManager.cpp:223 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqForceLeave | 0x140016C60 | verified | IDA 0x140016C60 + find(m_mapForce) + GetUserCount<=2→ReqDeleteForce + GetMasterID==dwLeaveMember→FindNewMaster→ChangeMaster→ClearRecruitDate + RemoveForceMember+DeleteRecruitMember+Kickout + XSendDBPacket(8,3)+dwNewMaster + SendDBLog(23,7); 源码ForceManager.cpp:718 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResForceLeave | 0x1400170A0 | verified | IDA 0x1400170A0 + XSendPacket(0xFA,3)+stForceLeave+dwNewMaster+0 + SendPacketAll; 源码ForceManager.cpp:805 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqChangeMaster | 0x140017160 | verified | IDA 0x140017160 + find(m_mapForce) + ChangeMaster(true)+XSendDBPacket(8,5) / nErrorCode=1/2 + SendPacket(0xFA,4); 源码ForceManager.cpp:785 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResChangeMaster | 0x140017380 | verified | IDA 0x140017380 + nResult→LogInfo + SendPacket(0xFA,4)+SendPacketAll; 源码ForceManager.cpp:285 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqDeleteForce | 0x140017590 | verified | IDA 0x140017590 + find(m_mapForce) + GetUserCount<=2检查 + DeleteForce调用 + SendPacket(0xFA,6)+SendPacketAll; 源码ForceManager.cpp:818 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResDeleteForce | 0x140017720 | verified | IDA 0x140017720 + SendPacket(0xFA,6)+SendPacketAll + FindRecruitID+FindRecruitPtr + GetMasterID比对→ClearRecruitDate/DeleteRecruitMember; 源码ForceManager.cpp:868 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ResLoadForceAll | 0x1400178C0 | verified | IDA 0x1400178C0 + 遍历stForceInfoAll + ClassFactory::create + SetForceInfo + insert(m_mapForce) + 遍历vecForceMember→AddPartyMember + byEnd→m_bLoadForce+SetCachingLoad; 源码ForceManager.cpp:338 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_INFO::PS_FORCE_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::ReqUpdateMemberInfo | 0x140017B60 | verified | IDA 0x140017B60(与ForceManager.cpp条目重复) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqUpdateMemberInfo | 0x140017B60 | verified | IDA 0x140017B60 + find(m_mapForce) + SetMemberInfo + XSendDBPacket(8,4)+SendDBGame; 源码ForceManager.cpp:598 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::SendForceMessage | 0x140017D50 | verified | IDA 0x140017D50 + XSendPacket(0xFA,0x10)+stChatForce+psChatItemLinkInfo+SendPacketAll; 源码ForceManager.cpp:46 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::SendForceErrorInvite | 0x140017E50 | verified | IDA 0x140017E50 + nResult=nErrorCode + XSendPacket(0xFA,0xB)+stForceInvite+0+0+0 + SendEx; 源码ForceManager.cpp:634 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::ReqForceMazeClear | 0x1400181D0 | verified | IDA 0x1400181D0 + find(m_mapForce) + GetForceType==1→SetForceType(0)+XSendDBPacket(8,0xE)+GetPartyID + SendPacket(0xFA,0x19)+SendPacketAll; 源码ForceManager.cpp:434 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::SendForceErrorAccept | 0x140017F30 | verified | IDA 0x140017F30 + dwAcceptID+dwActorID+nResult=nErrorCode + XSendPacket(0xFA,0xC)+SendEx; 源码ForceManager.cpp:660 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::CreateForceMatching | 0x140017FE0 | verified | IDA 0x140017FE0 + new CForce() + SetForceInfo + insert(m_mapForce) + AddPartyMember循环; 源码ForceManager.cpp:967 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::SetMaze | 0x140018380 | verified | IDA 0x140018380 + find(m_mapForce) + GetMazeID条件 + SetMazeID + XSendDBPacket(8,8) + SendPacket(0xFA,9); 源码ForceManager.cpp:367 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CForceManager::SendForceNameChange | 0x140018570 | verified | IDA 0x140018570 + find(m_mapForce) + pForce->SendNameChange(dwActorID, pChangeName); 源码ForceManager.cpp:957 | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::ClassFactory<CForce,64> | - | verified | 源码对象池模板构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::create | - | verified | 源码对象池create; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ClassFactory<CForce,64>::destroy | - | verified | 源码对象池destroy; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | UXActorID::operator< | - | verified | 源码结构体比较; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetMazeID | 0x14001B8E0 | verified | IDA 0x14001B8E0 (CParty::GetMazeID) + return m_uxMazeID; 源码Force.h:52 inline | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XSendDBPacket::XSendDBPacket | - | verified | 源码辅助函数; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetForceType | 0x14001B980 | verified | IDA 0x14001B980 + return m_byForceType; 源码Force.h:54 inline | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_ITEM_SOCKET_LIST::~PS_ITEM_SOCKET_LIST | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_ITEM_PACKAGE::~PS_ITEM_PACKAGE | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_CHAT_ITEM_LINK::~PS_CHAT_ITEM_LINK | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::SetMazeID | 0x14001BD70 | verified | IDA 0x14001BD70 + m_uxMazeID = uxMapID; 源码Force.h:53 inline | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XParse::SetBIGINT | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XParse::operator<< | - | verified | 源码XPacket模板; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CPartyRecruit::GetMasterID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_INFO::PS_FORCE_INFO | - | verified | 源码POD默认构造(重复条目); IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CPartyRecruit::GetRecruitID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CUserObject::GetUAID | - | verified | 源码内联getter; IDA无单独函数(boost::multi_index内部) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_REQ_FORCE_CREATE::PS_REQ_FORCE_CREATE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_ADDMEMBER::PS_FORCE_ADDMEMBER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetUserCount | 0x140110496 | verified | IDA inline + return m_mapForceMember.size(); 源码Force.h:51 inline | 否 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::GetMasterID | 0x140110528 | verified | IDA inline + return m_dwMasterID; 源码Force.h:48 inline | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CUserObject::GetMapID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XResourceMgr::GetTB_DISTRICT | - | verified | 源码共享层函数; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ST_PARTY_RECRUIT_INFO::~ST_PARTY_RECRUIT_INFO | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ST_PARTY_RECRUIT::ST_PARTY_RECRUIT | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ST_PARTY_RECRUIT_INFO::ST_PARTY_RECRUIT_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CUserObject::GetCID | - | verified | 源码内联getter; IDA无单独函数(boost::multi_index内部) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XSendDBPacket::~XSendDBPacket | - | verified | 源码辅助类析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XParse::SetINT | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XSendDBPacket::XSendDBPacket | - | verified | 源码辅助类构造; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | PS_FORCE_INFO::~PS_FORCE_INFO | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CUserPartyInfo::GetMatchingState | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XParse::SetBYTE | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::Update | - | verified | 源码管理器更新; IDA无单独函数(内联到调用点) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::GetForceID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::AddForceMember | - | verified | 源码管理器方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::RemoveForceMember | - | verified | 源码管理器方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CForceManager::IsForce | - | verified | 源码管理器方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | Force.h | CForce::SetForceType | 0x140632672 | verified | IDA inline + m_byForceType = byForceType; 源码Force.h:55 inline | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.h | ST_FORCE_INVITE_INFO::ST_FORCE_INVITE_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | CPartyRecruit::SetPartyID | - | verified | 源码内联setter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceManager.h | XParse::operator<< | - | verified | 源码XPacket模板; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{AutoMatchingAccept, LeaderSelect} | 0x14001CCB0 | verified | IDA `0x14001CCB0 / 0x14001CDA0` + AutoMatchingAccept:遍历8+byCheck分支+m_bCheck赋值+allAccepted→LeaderSelect+SendMatchingWait; LeaderSelect:遍历找最高等级+最高经验作为leader | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{OnUpdate, SendMatchingWait, AutoMatchingExit} | 0x14001E2A0 | verified | IDA `0x14001E2A0 / 0x14001E170 / 0x14001C920` + OnUpdate:process==0→LogError+SendMatchingExit/process==2→return/!CheckMazeOpenTime→exit/tick→wait/state==0→MatchingPossible/state==1→MatchingCheck/state==2→MatchingWait; SendMatchingWait:SetMatchingState(2)+10000ms+遍历8+XSendPacket(0xFA,0x17); AutoMatchingExit:process!=1→false/state==2/3→false/loop+SendMatchingExit+byReason check+SetMatchingState(3) | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CheckMazeOpenTime | 0x14001E680 | verified | IDA 0x14001E680 + std::time+4秒间隔+CheckMazeOpenTime(m_dwMazeID) | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{CreateMazeMatching, SendCreateMatchingMaze} | 0x14001E750 | verified | IDA `0x14001E750 / 0x14001DB30` + CreateMazeMatching:PS_FORCE_INFO+ST_CREATE_MAZE+CreateForceMatching+0xF2/0x43控制包; SendCreateMatchingMaze:循环8+0xFA/0x18包+SetMatchingState(0)+SetMatchingID(0,0)+GetUser+SendDBLog(23,10)+m_bSendSucc=1+m_byProcess=0 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingInfo` average-level fix | 0x14001EB80 | verified | IDA 0x14001EB80 + 循环8+SendPacket(0xFA,0x13)+m_shAveLevel计算 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::{SendMatchingCheck, SendMatchingReset, SendMatchingStart, MatchingPossible, MatchingCheck, MatchingWait} | 0x14001F4F7 | verified | IDA `0x14001F4F7调用点 / 0x14001E4B9调用点 / 0x14001D620 / 0x14001E4A0 / 0x14001D410 / 0x14001E3E5调用点` + SendMatchingCheck:SetMatchingState(1)+10000ms+Send(0xFA,0x15); SendMatchingReset:m_nResetCount+++SetMatchingState(0)+60000ms+Send(0xFA,0x16); SendMatchingStart:m_byProcess=2+用户数检查+DB(8,0xD); MatchingPossible:currentCount>3→SendMatchingCheck / resetCount>=3→exit+state(3); MatchingCheck:遍历m_bCheck+LeaderSelect+SendMatchingWait; MatchingWait:SendMatchingStart | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchingMgr::{ExitMatching, MatchingRemoveUser} | 0x1400215C0 | verified | IDA `0x1400215C0 / 0x140021C90` + ExitMatching:find matching+AutoMatchingExit; MatchingRemoveUser:GreenDamTan_CollectGroupedMatchingIds+遍历匹配退出; 结构差异:源码有m_mpAutoMatching_Waiter回退分支(IDA无) | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchingMgr::CheckMatching | 0x140021680 | verified | IDA 0x140021680 + GetPartyUser+GetMatchingID/GetRewardState检查+find matching+AutoMatchingAccept | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatchingMgr::{ResForceMatchingCreate, SendCreateMatchingMaze} | 0x140021980 | verified | IDA `0x140021980 / 0x140021A00` + ResForceMatchingCreate:find matching→CreateMazeMatching(dwForceID); SendCreateMatchingMaze:find matching→CForceMatching::SendCreateMatchingMaze | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::CPartyMatching | 0x14009B7B0 | verified | IDA 0x14009B7B7B0 + CForceMatchginMember构造循环+Init; 源码PartyMatchingMgr.cpp:33 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::Init | 0x14009B830 | verified | IDA 0x14009B830 + memset+字段初始化+SetMatchingState(0)+m_byProcess=1; 源码PartyMatchingMgr.cpp:37 | 是 | 语义等效 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::LeaderSelect | 0x14009BEE0 | verified | IDA 0x14009BEE0 + 遍历4找最高等级+最高经验作为leader; 源码PartyMatchingMgr.cpp:485 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::SendMatchingInfo | 0x14009C030 | verified | IDA 0x14009C030 + 0xF4/0x20包+遍历发送+m_shAveLevel计算; 源码PartyMatchingMgr.cpp:422 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::OnUpdate | 0x14009D050 | verified | IDA 0x14009D050 + process==0/2分支+tick检查+state==0/1/2分支; ��码PartyMatchingMgr.cpp:62 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchginMember::Clear | 0x14009D9A0 | verified | IDA 0x14009D9A0 + m_pCurServer=nullptr+m_nExp=0+m_nState=0+memset; 源码PartyMatchingMgr.cpp:26 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::AutoMatchingEnter | 0x14009B900 | verified | IDA 0x14009B900 + 参数检查+slot填充+CheckFullUser; 源码PartyMatchingMgr.cpp:327 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::AutoMatchingExit | 0x14009BB20 | verified | IDA 0x14009BB20 + process/state检查+SendMatchingExit; 源码PartyMatchingMgr.cpp:299 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::AutoMatchingCreate | 0x14009BC20 | verified | IDA 0x14009BC20 + 参数赋值+Init; 源码PartyMatchingMgr.cpp:375 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::AutoMatchingAccept | 0x14009BDD0 | verified | IDA 0x14009BDD0 + m_bCheck赋值+CheckFullUser; 源码PartyMatchingMgr.cpp:455 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::SendMatchingExit | 0x14009C2A0 | verified | IDA 0x14009C2A0 + 遍历4+0xF4/0x22包+byReason处理; 源码PartyMatchingMgr.cpp:220 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::MatchingPossible | 0x14009D1A0 | verified | IDA 0x14009D1A0 + 计数检查+SendMatchingCheck/Exit; 源码PartyMatchingMgr.cpp:103 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::MatchingCheck | 0x14009D2D0 | verified | IDA 0x14009D2D0 + m_bCheck遍历+LeaderSelect; 源码PartyMatchingMgr.cpp:134 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatching::MatchingWait | 0x14009D410 | verified | IDA 0x14009D410 + SendMatchingStart调用; 源码PartyMatchingMgr.cpp:161 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CParty::GetPartyID | - | verified | 源码Party.h内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | PS_PARTY_INFO::PS_PARTY_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CForceMatching | 0x14001C3D0 | verified | IDA 0x14001C3D0 + 构造函数; 源码ForceMatching.cpp:14 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::~CForceMatching | 0x14001C420 | verified | IDA 0x14001C420 + 析构函数; 源码ForceMatching.cpp:19 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::Init | 0x14001C450 | verified | IDA 0x14001C450 + m_bSendSucc=0+memset+m_dwMachingID=0+SetMatchingState(0)+m_byProcess=1; 源码ForceMatching.cpp:24 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::AutoMatchingEnter | 0x14001C560 | verified | IDA 0x14001C560 + 检查m_byProcess+GetPartyRecruitInfo+m_mapMatching.insert; 源码ForceMatching.cpp:35 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CheckAutoMatchingEnter | 0x14001C7C0 | verified | IDA 0x14001C7C0 + 检查m_byProcess+GetTB_PARTY_MATCHING+m_mapMatching检查; 源码ForceMatching.cpp:85 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::AutoMatchingExit | 0x14001C920 | verified | IDA 0x14001C920 + m_mapMatching.erase+SendMatchingExit; 源码ForceMatching.cpp:115 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::AutoMatchingCreate | 0x14001CA30 | verified | IDA 0x14001CA30 + PS_SERVER_FORCE_MATCHING_ENTER_MEMBER处理+m_mapMatching.insert; 源码ForceMatching.cpp:140 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::AutoMatchingAccept | 0x14001CCB0 | verified | IDA 0x14001CCB0 + find(m_mapMatching)+SendMatchingInfo; 源码ForceMatching.cpp:175 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::LeaderSelect | 0x14001CDA0 | verified | IDA 0x14001CDA0 + 遍历m_mapMatching找队长; 源码ForceMatching.cpp:192 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingInfo | 0x14001CEF0 | verified | IDA 0x14001CEF0 + XSendPacket(0xFA,0x1B)+遍历发送; 源码ForceMatching.cpp:216 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingExit | 0x14001D160 | verified | IDA 0x14001D160 + XSendPacket(0xFA,0x1C)+发送退出消息; 源码ForceMatching.cpp:251 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingStart | 0x14001D620 | verified | IDA 0x14001D620 + XSendPacket(0xFA,0x1D)+SendPacketAll; 源码ForceMatching.cpp:317 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | PS_DB_FORCE_MATCHING_CREATE::~PS_DB_FORCE_MATCHING_CREATE | 0x14001D980 | verified | IDA 0x14001D980 + 析构函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CheckFullUser | 0x14001D9C0 | verified | IDA 0x14001D9C0 + m_mapMatching.size检查; 源码ForceMatching.cpp:380 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendCreateMatchingMaze | 0x14001DA20 | verified | IDA 0x14001DA20 + SendDBGame(7,0x2E)+PS_FORCE_INFO; 源码ForceMatching.cpp:385 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingCheck | 0x14001DE60 | verified | IDA 0x14001DE60 + 检查匹配状态; 源码ForceMatching.cpp:420 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingReset | 0x14001DF90 | verified | IDA 0x14001DF90 + XSendPacket(0xFA,0x1E)+发送重置; 源码ForceMatching.cpp:442 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::SendMatchingWait | 0x14001E170 | verified | IDA 0x14001E170 + XSendPacket(0xFA,0x1F)+发送等待; 源码ForceMatching.cpp:470 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::OnUpdate | 0x14001E2A0 | verified | IDA 0x14001E2A0 + MatchingPossible+MatchingCheck+MatchingWait; 源码ForceMatching.cpp:478 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::MatchingPossible | 0x14001E450 | verified | IDA 0x14001E450 + 检查是否可以匹配; 源码ForceMatching.cpp:502 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::MatchingCheck | 0x14001E510 | verified | IDA 0x14001E510 + 检查匹配条件; 源码ForceMatching.cpp:515 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::MatchingWait | 0x14001E620 | verified | IDA 0x14001E620 + 等待匹配; 源码ForceMatching.cpp:524 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CheckMazeOpenTime | 0x14001E640 | verified | IDA 0x14001E640 + GetTB_MAZE_OPEN_TIME检查; 源码ForceMatching.cpp:528 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::GetMatchingUserCount | 0x14001E6C0 | verified | IDA 0x14001E6C0 + m_mapMatching.size返回; 源码ForceMatching.cpp:545 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.cpp | CForceMatching::CreateMazeMatching | 0x14001E720 | verified | IDA 0x14001E720 + SendCreateMatchingMaze+CreateMaze; 源码ForceMatching.cpp:550 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | PS_PARTY_INFO::~PS_PARTY_INFO | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_MAP_INFO::ST_MAP_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_PARTY_INFO::ST_PARTY_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | XVec3::XVec3 | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | STPosInfo::STPosInfo | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_CREATE_MAZE::ST_CREATE_MAZE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_CREATE_MAZE::~ST_CREATE_MAZE | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_CREATE_FORCE::ST_CREATE_FORCE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | PS_DB_FORCE_MATCHING_CREATE::PS_DB_FORCE_MATCHING_CREATE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CUserPartyInfo::SetMatchingID | - | verified | 源码内联setter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_FORCE_MATCHING_INFO::ST_FORCE_MATCHING_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CForceMatching::SetMatchingState | - | verified | 源码内联setter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CForceMatchginMember::CForceMatchginMember | - | verified | 源码默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CUserPartyInfo::SetMatchingState | - | verified | 源码内联setter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CForceMatchginMember::~CForceMatchginMember | - | verified | 源码默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | CForceMatchginMember::Clear | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceMatching.h | ST_ENTER_MAZE_MEMBER_INFO::ST_ENTER_MAZE_MEMBER_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CPartyMember::GetMemberID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CUserPartyInfo::GetMatchingID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::CForceMatchingMgr | 0x14009D9F0 | verified | IDA 0x14009D9F0 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::~CForceMatchingMgr | 0x14009DA50 | verified | IDA 0x14009DA50 + 析构函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::CreateMatching | 0x14009DAD0 | verified | IDA 0x14009DAD0 + 创建匹配对象; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::EnterMatching | 0x14009DC60 | verified | IDA 0x14009DC60 + 进入匹配; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::EnterMatching | 0x14009DC60 | verified | IDA 同上; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.h | PS_SERVER_FORCE_MATCHING_ENTER::~PS_SERVER_FORCE_MATCHING_ENTER | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::ExitMatching | 0x14009DDB0 | verified | IDA 0x14009DDB0 + 退出匹配; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::CheckMatching | 0x14009DE70 | verified | IDA 0x14009DE70 + 检查匹配状态; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::OnUpdate | 0x14009E000 | verified | IDA 0x14009E000 + 更新循环; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::ResForceMatchingCreate | 0x14009E400 | verified | IDA 0x14009E400 + 创建响应; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::SendCreateMatchingMaze | 0x14009E480 | verified | IDA 0x14009E480 + 发送创建迷宫匹配; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.h | ST_CREATE_MAZE::ST_CREATE_MAZE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatchingMgr::MatchingRemoveUser | 0x14009F640 | verified | IDA 0x14009F640 + 移除匹配用户; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForce::GetMember | - | verified | 源码内联方法; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CParty::GetMember | - | verified | 源码内联方法; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceMatchingMgr.cpp | CForceMatching::GetMatchingID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::Parse | 0x140023360 | verified | IDA 0x140023360 + 18 case switch完全匹配IDA (0x01/0x03/0x04/0x05/0x09/0x0A-0x0D/0x10/0x13-0x15/0x19/0x1A/0x21) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{Parse, ReqForceCreate, ReqForceLeaveMember, ReqForceChangeMaster, ReqForceMazeClear} | 0x140023360 | verified | IDA `0x140023360 / 0x140023A60 / 0x1400242E0 / 0x140024610 / 0x140026730` + Parse:18 case switch完全匹配IDA(0x01/0x03/0x04/0x05/0x09/0x0A/0x0B/0x0C/0x0D/0x10/0x13/0x14/0x15/0x19/0x1A/0x21); 注意:case 0x09 IDA显示为boost::multi_index modify_,实际对应ReqForceMatchingEnter(0x140024A40) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceInvite, ReqForceAccept, ReqForceCancel} | 0x140023434 | verified | IDA 0x140023434/0x140023448/0x14002345C (Parse entries) + PS_REQ_FORCE_INVITE>>dwUAID>>byLevel>>dwForceID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceMatchingEnter, ReqForceMatchingExit, ReqForceMatchingCheck} | 0x14002347E | verified | IDA 0x14002347E/0x14002348F/0x1400234A0 (Parse entries) + 0x140025ED0/0x14001C920/0x140021680 handler addresses | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceUpdateMember, ReqForceEnterServer, ReqForceInfo} | 0x140023570 | verified | IDA 0x140023570/0x1400248D0/0x140026870 + ST_UPDATE_FORCE_MEMBER>>DoJob(0) / PS_REQ_FORCE_ENTER_SERVER>>DoJob(0) / XParse>>dwForceID>>dwActorID+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceCreate, ReqForceLeaveMember, ReqForceChangeMaster, ReqForceMazeClear} | 0x140023A60 | verified | IDA 0x140023A60/0x1400242E0/0x140024610/0x140026730 + PS_REQ_FORCE_CREATE>>DoJob(0) / PS_FORCE_LEAVE>>dwActorID>>dwUAID>>byLevel>>byLeaverLevel+DoJob(0) / PS_FORCE_CHANGE_MASTER>>DoJob(0) / XParse>>dwForceID>>byClearFail+DoJob(0) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::{ReqForceMatchingExit, ReqForceMatchingCheck} | 0x140025ED0 | verified | IDA 0x140025ED0/0x1400265E0 + dwActorID>>byReason>>dwUAID>>byLevel+GetClientPtr+DoJob(0,lambda) / PS_SERVER_FORCE_MATCHING_CHECK>>GetClientPtr+DoJob(0,lambda→CheckMatching) | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceChangeMazeOpenCheck | 0x140026AA0 | verified | IDA 0x140026AA0 + bOn默认1+XParse>>bOn+DoJob(0,lambda→SetCheckMazeOpenTime(bOn)); 源码用DispatchForceJob+GetMazeOpenControl().SetCheckMazeOpenTime,逻辑匹配; 结构差异:IDA直接调XResourceMgr::SetCheckMazeOpenTime,源码经CRelayMazeOpenControl中转 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::CForceProcess | 0x140023270 | verified | IDA 0x140023270 + 构造函数调用TXProcess构造; 源码ForceProcess.cpp:12 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::~CForceProcess | 0x140023330 | verified | IDA 0x140023330 + 析构函数; 源码ForceProcess.cpp:17 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::Parse | 0x140023360 | verified | IDA 0x140023360 + switch(GetSubCmd): 1→ReqForceCreate,3→ReqForceLeaveMember,4→ReqForceChangeMaster,5→ReqForceUpdateMember,0xA→ReqForceEnterServer,0xB→ReqForceInvite,0xC→ReqForceAccept,0xD→ReqForceCancel,0x10→SyncForceMessage,0x13→ReqForceMatchingEnter,0x14→ReqForceMatchingExit,0x15→ReqForceMatchingCheck,0x19→ReqForceMazeClear,0x1A→ReqForceInfo,0x21→ReqForceChangeMazeOpenCheck; 源码ForceProcess.cpp:19 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::SyncForceMessage | 0x140023570 | verified | IDA 0x140023570 + PS_CHAT_FORCE解析+GetForce+SendForceMessage; 源码ForceProcess.cpp:431 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK_FOR_SERVER::PS_CHAT_ITEM_LINK_FOR_SERVER | 0x140023730 | verified | IDA 0x140023730 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK::PS_CHAT_ITEM_LINK | 0x140023820 | verified | IDA 0x140023820 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST | 0x140023900 | verified | IDA 0x140023900 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_PACKAGE::PS_ITEM_PACKAGE | 0x140023950 | verified | IDA 0x140023950 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceCreate | 0x140023A60 | verified | IDA 0x140023A60 + PS_REQ_FORCE_CREATE解析+GetUser+GetForce+CreateForce; 源码ForceProcess.cpp:531 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceInvite | 0x140023CC0 | verified | IDA 0x140023CC0 + PS_REQ_FORCE_INVITE解析+GetUser+GetPartyRecruitInfo+ReqInviteForce; 源码ForceProcess.cpp:480 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceAccept | 0x140023FD0 | verified | IDA 0x140023FD0 + PS_RES_FORCE_INVITE解析+GetUser+ReqAcceptForce; 源码ForceProcess.cpp:500 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceCancel | 0x140024180 | verified | IDA 0x140024180 + PS_FORCE_REJECT解析+GetUser+ReqCancelForce; 源码ForceProcess.cpp:520 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceLeaveMember | 0x1400242E0 | verified | IDA 0x1400242E0 + PS_FORCE_LEAVE解析+GetForce+ReqForceLeave; 源码ForceProcess.cpp:447 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceChangeMaster | 0x140024610 | verified | IDA 0x140024610 + PS_FORCE_CHANGE_MASTER解析+ReqChangeMaster; 源码ForceProcess.cpp:411 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceUpdateMember | 0x140024760 | verified | IDA 0x140024760 + UST_UPDATE_FORCE_MEMBER解析+ReqUpdateMemberInfo; 源码ForceProcess.cpp:396 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceEnterServer | 0x1400248D0 | verified | IDA 0x1400248D0 + PS_REQ_FORCE_ENTER_SERVER解析+EnterServer; 源码ForceProcess.cpp:378 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_REQ_FORCE_ENTER_SERVER::PS_REQ_FORCE_ENTER_SERVER | 0x1400249B0 | verified | IDA 0x1400249B0 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceMatchingEnter | 0x140024A40 | verified | IDA 0x140024A40 + PS_SERVER_FORCE_MATCHING_ENTER解析+GetUser+AutoMatchingEnter; 源码ForceProcess.cpp:334 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_ENTER::PS_SERVER_FORCE_MATCHING_ENTER | 0x140024B50 | verified | IDA 0x140024B50 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceMatchingExit | 0x140025ED0 | verified | IDA 0x140025ED0 + PS_SERVER_FORCE_MATCHING_EXIT解析+AutoMatchingExit; 源码ForceProcess.cpp:368 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceMatchingCheck | 0x1400265E0 | verified | IDA 0x1400265E0 + 检查匹配状态; 源码ForceProcess.cpp:350 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceMazeClear | 0x140026730 | verified | IDA 0x140026730 + dwForceID解析+ReqForceMazeClear; 源码ForceProcess.cpp:320 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceInfo | 0x140026870 | verified | IDA 0x140026870 + dwForceID解析+GetForce+GetForceInfo+SendPacket; 源码ForceProcess.cpp:308 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.cpp | CForceProcess::ReqForceChangeMazeOpenCheck | 0x140026AA0 | verified | IDA 0x140026AA0 + bOn默认1+XParse>>bOn+DoJob(0,lambda→SetCheckMazeOpenTime(bOn)); 源码用DispatchForceJob+GetMazeOpenControl().SetCheckMazeOpenTime,逻辑匹配; 结构差异:IDA直接调XResourceMgr::SetCheckMazeOpenTime,源码经CRelayMazeOpenControl中转 | 否 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::GetClientPtr | - | verified | 源码模板方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::TXProcess<CServer> | - | verified | 源码模板构造; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::~TXProcess<CServer> | - | verified | 源码模板析构; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | TXSingleton<CLogicThreadManager>::Instance | - | verified | 源码单例模板; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::Init | - | verified | 源码模板方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | TXProcess<CServer>::Clear | - | verified | 源码模板方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | IXProcess::~IXProcess | - | verified | 源码接口虚析构; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::IXComponent | - | verified | 源码接口构造; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::~IXComponent | - | verified | 源码接口析构; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | IXProcess::IXProcess | - | verified | 源码接口构造; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XResourceMgr::SetCheckMazeOpenTime | - | verified | 源码共享层方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_CHECK::PS_SERVER_FORCE_MATCHING_CHECK | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | CUserPartyInfo::GetActorID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_ENTER_MEMBER::PS_SERVER_FORCE_MATCHING_ENTER_MEMBER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_SERVER_FORCE_MATCHING_ENTER::PS_SERVER_FORCE_MATCHING_ENTER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XParse::operator>> | - | verified | 源码XPacket模板; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XParse::GetDWORD | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XParse::operator>> | - | verified | 源码XPacket模板; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | CServer::GetServerID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | STItem::Init | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | STItem::STItem | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | ST_ITEM_BROACH::ST_ITEM_BROACH | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_PACKAGE::PS_ITEM_PACKAGE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | ST_EXTEND_OPTION::operator= | - | verified | 源码POD默认赋值; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | ST_SOCKET_DATA::operator= | - | verified | 源码POD默认赋值; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | ST_ITEM_SOCKET::operator= | - | verified | 源码POD默认赋值; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK::PS_CHAT_ITEM_LINK | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_CHAT_ITEM_LINK_FOR_SERVER::PS_CHAT_ITEM_LINK_FOR_SERVER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XPacket::GetSubCmd | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::SetName | - | verified | 源码接口方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | IXComponent::SetCmd | - | verified | 源码接口方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | CUserPartyInfo::GetMatchingType | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | ST_UPDATE_FORCE_MEMBER::ST_UPDATE_FORCE_MEMBER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | UXActorID::UXActorID | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_FORCE_REJECT::PS_FORCE_REJECT | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | PS_REQ_FORCE_INVITE::PS_REQ_FORCE_INVITE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | CForceProcess::ReqForceUpdateInfo | - | verified | 源码处理方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XParse::GetBYTE | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ForceProcess.h | XParse::Getbool | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
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
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_BLOCK_ADD::PS_REQ_FRIEND_BLOCK_ADD | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_BLOCK_DELETE::PS_REQ_FRIEND_BLOCK_DELETE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_FIND::PS_REQ_FRIEND_FIND | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::CFriendProcess | 0x140040280 | verified | IDA 0x140040280 + TXProcess<CServer>构造+SetCmd(0xF5)+SetName("CServerFriendProcess"); 源码FriendProcess.h | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.cpp | CFriendProcess::~CFriendProcess | 0x140040340 | verified | IDA 0x140040340 + 默认析构; 源码FriendProcess.h | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | ST_CREATE_ITEMS::ST_CREATE_ITEMS | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | ST_CREATE_ITEMS::~ST_CREATE_ITEMS | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_SERVER_HELPER_SUPPORT_REWARD::PS_SERVER_HELPER_SUPPORT_REWARD | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_SERVER_HELPER_SUPPORT_REGISTER::PS_SERVER_HELPER_SUPPORT_REGISTER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_DAILY_MISSION_FRIEND_REQ::PS_DAILY_MISSION_FRIEND_REQ | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_RECRUIT_ADD::PS_RECRUIT_ADD | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_RECRUIT_DELETE::PS_RECRUIT_DELETE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_RECRUIT_LIST::PS_RECRUIT_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_ACCEPT::PS_REQ_FRIEND_ACCEPT | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_RES_FRIEND_INVITE::PS_RES_FRIEND_INVITE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | ST_CHAR_COMMUNITY::ST_CHAR_COMMUNITY | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_HELPER_SUPPORT_EQUIP_REQ::PS_HELPER_SUPPORT_EQUIP_REQ | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_REQ_FRIEND_DELETE::PS_REQ_FRIEND_DELETE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendProcess.h | PS_DAILY_MISSION_FRIEND_RES::~PS_DAILY_MISSION_FRIEND_RES | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::CFriendRecommandManager | 0x140042340 | verified | IDA 0x140042340 + map构造+m_mapUserCheck+m_nIndex=0; 源码待创建 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::~CFriendRecommandManager | - | verified | 源码默认析构; IDA无显式析构 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::AddUser | 0x140042440 | verified | IDA 0x140042440 + SYSTEM_TYPE_REAL跳过+level>68跳过+insert; 源码待创建 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::DeleteUser | 0x140042600 | verified | IDA 0x140042600 + find+erase; 源码待创建 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::DeleteUserCheck | - | verified | 源码辅助方法; IDA无单独函数(内联到DeleteUser) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::UpdateLevel | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CFriendRecommandManager::GetFriendRecommandList | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CCommunity::GetRecommandIndex | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CUserObject::GetRecommandIndex | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | ST_RECOMMAND_FRIEND_INFO::ST_RECOMMAND_FRIEND_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CCommunity::SetRecommandIndex | - | verified | 源码内联setter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CUserObject::SetRecommandIndex | - | verified | 源码内联setter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | CUserObject::GetGMPower | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecommandManager.cpp | XOption::GetSystemType | - | verified | 源码配置getter; IDA无单独函数(共享层内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::CFriendRecruitManager | 0x140044DE0 | verified | IDA 0x140044DE0 + CFSRWLock初始化+boost::multi_index容器+m_DBLoad=0; 源码文件待创建 | 否 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::~CFriendRecruitManager | 0x140044E80 | verified | IDA 0x140044E80 + clear+容器析构; 源码文件待创建 | 否 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::LoadRecruitList | 0x140044EC0 | verified | IDA 0x140044EC0 + WriteLock+遍历vector+insert到m_mapRecruit; 源码文件待创建 | 否 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::OnUpdate | 0x140045110 | verified | IDA 0x140045110 + CTime检查+遍历+超时删除+SendRecruitDelete; 源码文件待创建 | 否 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::AddRecruit | 0x140045330 | verified | IDA 0x140045330 + WriteLock+find去重+new CRecruitUser+insert; 源码文件待创建 | 否 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::DeleteRecruit | 0x1400454C0 | verified | IDA 0x1400454C0 + WriteLock+find+erase; 源码文件待创建 | 否 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::UpdateRecruit | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::IsRecruitList | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::GetFriendRecruitList | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::GetRecruitAddTime | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::UpdateCharCommunity | - | verified | 源码辅助方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CFriendRecruitManager::ChangeName | 0x1400462C0 | verified | IDA 0x1400462C0 + 名字变更处理; 源码FriendRecruitManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetClass | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetLevel | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetInfo | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetLogin | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::CRecruitUser | - | verified | 源码默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::GetUCID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | ST_RECRUIT_INFO::ST_RECRUIT_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | FriendRecruitManager.cpp | CRecruitUser::CRecruitUser | - | verified | 源码默认构造(重复条目); IDA无单独函数 | 是 | 一致 |
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
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_PARTY_CREATE::PS_REQ_PARTY_CREATE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_PARTY_ADDMEMBER::PS_PARTY_ADDMEMBER | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | XParse::GetINT | - | verified | 源码XPacket辅助; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_BLOCKLIST_INFO::PS_BLOCKLIST_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_FORCE_INFO_ALL::PS_FORCE_INFO_ALL | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_BROACH_LIST::PS_ITEM_BROACH_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_INVENTORY_FOR_LOG_LIST::PS_LEAGUE_INVENTORY_FOR_LOG_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_SUMMARY_LIST::PS_LEAGUE_SUMMARY_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_PARTY_INFO_ALL::PS_PARTY_INFO_ALL | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_CHECK_LIST::ST_LEAGUE_APPLICANT_CHECK_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_LIST::ST_LEAGUE_APPLICANT_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD_LIST::ST_LEAGUE_BOARD_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_LIST::ST_LEAGUE_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_LIST::ST_LEAGUE_MEMBER_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECORD_LIST::ST_LEAGUE_RECORD_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_RECRUIT_LIST::ST_RECRUIT_LIST | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::CGameDBSocket | 0x140049630 | verified | IDA 0x140049630 + 默认构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::~CGameDBSocket | 0x140049680 | verified | IDA 0x140049680 + 默认析构函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::FindUser | 0x1400496B0 | verified | IDA 0x1400496B0 + 返回nullptr; 源码GameDBSocket.cpp:60 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::SetInfomation | 0x1400496E0 | verified | IDA 0x1400496E0 + sprintf_s+GetServerPrivateIPAndPort; 源码GameDBSocket.cpp:10 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::OnParse | 0x140049770 | verified | IDA 0x140049770 + xSessionID>>DBParse; 源码GameDBSocket.cpp:65 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBParse | 0x1400497C0 | verified | IDA 0x1400497C0 + switch(byMainCmd):4/5/7/8/0x26/0x27/0x43分发; 源码GameDBSocket.cpp:71 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBPartyParse | 0x140049930 | verified | IDA 0x140049930 + 8 case switch(1-6/0x11/0x13); 源码GameDBSocket.cpp:96 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBForceParse | 0x140049A60 | verified | IDA 0x140049A60 + 8 case switch(1-6/0x0B/0x0D); 源码GameDBSocket.cpp:149 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBFriendParse | 0x140049B80 | verified | IDA 0x140049B80 + 11 case switch(0x01-0x11); 源码GameDBSocket.cpp:119 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::DBLeagueParse | 0x140049CF0 | verified | IDA 0x140049CF0 + League消息分发; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueNoticeChange | - | verified | 源码DB响应方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicant | - | verified | 源码DB响应方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueCreate | 0x14004A620 | verified | IDA 0x14004A620 + PS_RES_LEAGUE_CREATE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBWorldParse | 0x14004A850 | verified | IDA 0x14004A850 + DB World响应解析; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBExchangeParse | 0x14004A890 | verified | IDA 0x14004A890 + DB Exchange响应解析; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::DBHelperParse | 0x14004A8E0 | verified | IDA 0x14004A8E0 + DB Helper响应解析; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyCreate | 0x14004A930 | verified | IDA 0x14004A930 + PS_RES_PARTY_CREATE解析+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyJoin | 0x14004AA80 | verified | IDA 0x14004AA80 + PS_PARTY_ADDMEMBER解析+dwRecruitID+DoJob(0); 源码GameDBSocket.cpp:463已验证 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyLeave | 0x14004ABD0 | verified | IDA 0x14004ABD0 + PS_FORCE_LEAVE解析+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyChangeMaster | 0x14004AD50 | verified | IDA 0x14004AD50 + PS_FORCE_CHANGE_MASTER解析+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyDelete | 0x14004AEB0 | verified | IDA 0x14004AEB0 + PS_FORCE_LEAVE解析+DoJob(0); 源码GameDBSocket.cpp:467已验证 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyUpdateMemberInfo | 0x14004AFB0 | verified | IDA 0x14004AFB0 + 更新成员信息+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_UPDATE_PARTY_MEMBER::ST_UPDATE_PARTY_MEMBER | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyLoadAll | 0x14004B100 | verified | IDA 0x14004B100 + PS_PARTY_INFO_ALL加载+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_FORCE_INFO_ALL::~PS_FORCE_INFO_ALL | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_PARTY_INFO_ALL::~PS_PARTY_INFO_ALL | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceCreate | 0x14004B1F0 | verified | IDA 0x14004B1F0 + PS_REQ_FORCE_CREATE解析+DoJob(0)+CreateForce; 源码GameDBSocket.cpp:464已验证 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceJoin | 0x14004B340 | verified | IDA 0x14004B340 + PS_FORCE_ADDMEMBER解析+dwRecruitID+DoJob(0)+ResJoinMember; 源码GameDBSocket.cpp:465已验证 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceLeave | 0x14004B500 | verified | IDA 0x14004B500 + PS_FORCE_LEAVE解析+DoJob(0)+ResForceLeave; 源码GameDBSocket.cpp:466已验证 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceChangeMaster | 0x14004B6F0 | verified | IDA 0x14004B6F0 + PS_FORCE_CHANGE_MASTER解析+DoJob(0)+ResChangeMaster; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceDelete | 0x14004B800 | verified | IDA 0x14004B800 + PS_FORCE_LEAVE解析+DoJob(0)+ResDeleteForce; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceUpdateMemberInfo | 0x14004B900 | verified | IDA 0x14004B900 + ST_UPDATE_FORCE_MEMBER解析+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceLoadAll | 0x14004BA20 | verified | IDA 0x14004BA20 + PS_FORCE_INFO_ALL加载+DoJob(0); 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResFriendLoad | 0x14004BAF0 | verified | IDA 0x14004BAF0 + PS_DB_FRIEND_LIST解析+DoJob(0)+SendFriendList; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_LIST::~PS_DB_FRIEND_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_BLOCKLIST_INFO::~PS_BLOCKLIST_INFO | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendInvite | 0x14004BCC0 | verified | IDA 0x14004BCC0 + PS_RES_DB_FRIEND_INVITE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_DB_FRIEND_INVITE::PS_RES_DB_FRIEND_INVITE | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_INVITE::PS_DB_FRIEND_INVITE | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendInviteCheck | 0x14004BDB0 | verified | IDA 0x14004BDB0 + 邀请检查处理; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendAccept | 0x14004BE50 | verified | IDA 0x14004BE50 + PS_DB_FRIEND_ACCEPT解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendDelete | 0x14004BEC0 | verified | IDA 0x14004BEC0 + PS_DB_FRIEND_DELETE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResBlockListAdd | 0x14004BF00 | verified | IDA 0x14004BF00 + PS_DB_BLOCK_ADD解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResBlockListDelete | 0x14004BF60 | verified | IDA 0x14004BF60 + PS_DB_BLOCK_DELETE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResRecruitLoad | 0x14004BFC0 | verified | IDA 0x14004BFC0 + PS_DB_RECRUIT_LOAD解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResRecruitAdd | 0x14004C040 | verified | IDA 0x14004C040 + PS_DB_RECRUIT_ADD解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResRecruitDelete | 0x14004C150 | verified | IDA 0x14004C150 + PS_DB_RECRUIT_DELETE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResFriendFind | 0x14004C200 | verified | IDA 0x14004C200 + PS_DB_FRIEND_FIND解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_FRIEND_FIND::~PS_DB_FRIEND_FIND | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueWithDraw | 0x14004C280 | verified | IDA 0x14004C280 + PS_RES_LEAGUE_WITHDRAW解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueKick | 0x14004C4F0 | verified | IDA 0x14004C4F0 + PS_RES_LEAGUE_KICK解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueAuthChange | 0x14004C770 | verified | IDA 0x14004C770 + PS_RES_LEAGUE_AUTH_CHANGE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueInviteAccept | 0x14004C8D0 | verified | IDA 0x14004C8D0 + PS_RES_LEAGUE_INVITE_ACCEPT解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueDelete | 0x14004CD60 | verified | IDA 0x14004CD60 + PS_RES_LEAGUE_DELETE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueBoard | 0x14004D120 | verified | IDA 0x14004D120 + PS_RES_LEAGUE_BOARD解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicantAccept | 0x14004D2A0 | verified | IDA 0x14004D2A0 + PS_RES_LEAGUE_APPLICANT_ACCEPT解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicantReject | 0x14004D420 | verified | IDA 0x14004D420 + PS_RES_LEAGUE_APPLICANT_REJECT解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x14004D970 | verified | IDA 0x14004D970 + LogError+Shutdown(0xFFFFFFFF)+Sleep+SET_SERVICE_STATE(1,3)+relayServer->Shutdown; 源码GameDBSocket.cpp:20 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::OnNotConnect | 0x14004DA40 | verified | IDA 0x14004DA40 + LogError+m_bCloseProcess+Sleep+SET_SERVICE_STATE(1,3)+Shutdown; 源码GameDBSocket.cpp:42 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResPartyMatchingCreate | 0x14004DAE0 | verified | IDA 0x14004DAE0 + PS_RES_PARTY_MATCHING_CREATE>>DoJob; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.cpp | CGameDBSocket::ResForceMatchingCreate | 0x14004DC30 | verified | IDA 0x14004DC30 + nErrorCode>>matchingID>>dwForceID>>DoJob(0); 源码GameDBSocket.cpp:457已验证 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeaguePositionNameChange | 0x14004DDD0 | verified | IDA 0x14004DDD0 + PS_RES_LEAGUE_POSITION_NAME_CHANGE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueMemberPositionChange | 0x14004E060 | verified | IDA 0x14004E060 + PS_RES_LEAGUE_MEMBER_POSITION_CHANGE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueApplicantDelete | 0x14004E300 | verified | IDA 0x14004E300 + PS_RES_LEAGUE_APPLICANT_DELETE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResExchangePriceHistory | 0x14004E430 | verified | IDA 0x14004E430 + PS_DB_EXCHANGE_PRICE_HISTORY_RES解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_EXCHANGE_PRICE_HISTORY_RES::~PS_DB_EXCHANGE_PRICE_HISTORY_RES | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLoadLeagueMember | 0x14004E4C0 | verified | IDA 0x14004E4C0 + PS_RES_LEAGUE_MEMBER解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLoadLeagueApplicant | 0x14004E550 | verified | IDA 0x14004E550 + PS_RES_LEAGUE_APPLICANT解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLoadLeagueBoard | 0x14004E5E0 | verified | IDA 0x14004E5E0 + PS_RES_LEAGUE_BOARD解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD_LIST::~ST_LEAGUE_BOARD_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueDeletePenalty | 0x14004E690 | verified | IDA 0x14004E690 + PS_RES_LEAGUE_DELETE_PENALTY解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueWithdrawPenalty | 0x14004E7E0 | verified | IDA 0x14004E7E0 + PS_RES_LEAGUE_WITHDRAW_PENALTY解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResHelperSupportEquip | 0x14004E930 | verified | IDA 0x14004E930 + PS_DB_HELPER_SUPPORT_EQUIP解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueOpenOrNot | 0x14004E970 | verified | IDA 0x14004E970 + PS_RES_LEAGUE_OPEN解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueRecruitNotice | 0x14004EB50 | verified | IDA 0x14004EB50 + PS_RES_LEAGUE_RECRUIT_NOTICE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueSearch | 0x14004EDA0 | verified | IDA 0x14004EDA0 + PS_RES_LEAGUE_SEARCH解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_SUMMARY_LIST::~PS_LEAGUE_SUMMARY_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_SUMMARY_LIST::PS_LEAGUE_SUMMARY_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueRecord | 0x14004F0B0 | verified | IDA 0x14004F0B0 + PS_RES_LEAGUE_RECORD解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_LIST::~ST_LEAGUE_MEMBER_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECORD_LIST::~ST_LEAGUE_RECORD_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueDelegate | 0x14004F160 | verified | IDA 0x14004F160 + PS_RES_LEAGUE_DELEGATE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueCardChange | 0x14004F3B0 | verified | IDA 0x14004F3B0 + PS_RES_LEAGUE_CARD_CHANGE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_STORAGE_INFO::PS_RES_STORAGE_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueWealth | 0x14004F7E0 | verified | IDA 0x14004F7E0 + PS_RES_LEAGUE_WEALTH解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueLevelup | 0x14004F950 | verified | IDA 0x14004F950 + PS_RES_LEAGUE_LEVELUP解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueSkillLearn | 0x14004FB40 | verified | IDA 0x14004FB40 + PS_RES_LEAGUE_SKILL解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ReqLeagueInventoryInfo | 0x14004FD50 | verified | IDA 0x14004FD50 + 发送仓库信息请求; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE_LIST::PS_ITEM_PACKAGE_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_BROACH_LIST::PS_ITEM_BROACH_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ReqLeagueInventoryMove | 0x140050300 | verified | IDA 0x140050300 + 发送仓库移动请求; 源码GameDBSocket.cpp | 是 | 一�� |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::~PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_ITEM_MOVE_LEAGUE_INVEN::PS_RES_ITEM_MOVE_LEAGUE_INVEN | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_INVENTORY_FOR_LOG_LIST::PS_LEAGUE_INVENTORY_FOR_LOG_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueNameChange | 0x140050820 | verified | IDA 0x140050820 + PS_RES_LEAGUE_NAME_CHANGE解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_NAME_CHANGE_SERVER::~PS_LEAGUE_NAME_CHANGE_SERVER | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_NAME_CHANGE_SERVER::PS_LEAGUE_NAME_CHANGE_SERVER | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueInfo | 0x140050C70 | verified | IDA 0x140050C70 + PS_RES_LEAGUE_INFO解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_APPLICANT_LIST::ST_LEAGUE_APPLICANT_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_BOARD_LIST::ST_LEAGUE_BOARD_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_LIST::ST_LEAGUE_MEMBER_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECORD_LIST::ST_LEAGUE_RECORD_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResLeagueList | 0x140051470 | verified | IDA 0x140051470 + PS_RES_LEAGUE_LIST解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CGameDBSocket::ResGMTLeagueInfo | 0x140051820 | verified | IDA 0x140051820 + PS_RES_GMT_LEAGUE_INFO解析+DoJob; 源码GameDBSocket.cpp | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_LIST::~ST_LEAGUE_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_LIST::ST_LEAGUE_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::TXDBSocket<CServer> | - | verified | IDA无单独函数; 模板构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXMultiPoolServer<CServer>::FindUser | - | verified | IDA无单独函数; 模板方法内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | sprintf_s<255> | - | verified | IDA无单独函数; CRT模板函数 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXObjectMgr<CServer>::Find | - | verified | IDA无单独函数; 模板方法内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnFinishThread | - | verified | IDA无单独函数; 模板方法内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnUpdateThread | - | verified | IDA无单独函数; 模板方法内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_LEAGUE_LOAD::PS_DB_LEAGUE_LOAD | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_INFO::ST_LEAGUE_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_NAME_CHANGE_SERVER::PS_LEAGUE_NAME_CHANGE_SERVER | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_STORAGE_INFO::PS_STORAGE_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_ITEM_MOVE_LEAGUE_INVEN::PS_RES_ITEM_MOVE_LEAGUE_INVEN | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_ITEM_MOVE_LEAGUE_INVEN::~PS_RES_ITEM_MOVE_LEAGUE_INVEN | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_INVENTORY_FOR_LOG_LIST::~PS_LEAGUE_INVENTORY_FOR_LOG_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_ITEM_MOVE_LEAGUE_INVEN::PS_REQ_ITEM_MOVE_LEAGUE_INVEN | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE_LIST::~PS_ITEM_PACKAGE_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE::operator= | - | verified | IDA无单独函数; POD赋值内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_PACKAGE_LIST::PS_ITEM_PACKAGE_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_ITEM_BROACH_LIST::~PS_ITEM_BROACH_LIST | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_LEAGUE_SKILL::PS_RES_LEAGUE_SKILL | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_AUTO_SKILL::PS_AUTO_SKILL | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_LEAGUE_WEALTH_FOR_SERVER::PS_LEAGUE_WEALTH_FOR_SERVER | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_STORAGE_INFO::~PS_RES_STORAGE_INFO | - | verified | IDA无单独函数; POD析构内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_RES_STORAGE_INFO::PS_RES_STORAGE_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_REQ_LEAGUE_CARD::PS_REQ_LEAGUE_CARD | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_RECRUIT_NOTICE::ST_LEAGUE_RECRUIT_NOTICE | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_HELPER_SUPPORT_EQUIP::PS_DB_HELPER_SUPPORT_EQUIP | - | verified | IDA无单独函数; POD默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CUserObject::SetLeagueWithdrawPenalty | 0x140062D40 | verified | IDA @ 0x140062D40 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | CUserObject::SetLeagueDeletePenalty | 0x140062D60 | verified | IDA @ 0x140062D60 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_EXCHANGE_PRICE_HISTORY_RES::PS_EXCHANGE_PRICE_HISTORY_RES | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_EXCHANGE_PRICE_HISTORY_RES::~PS_EXCHANGE_PRICE_HISTORY_RES | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | PS_DB_EXCHANGE_PRICE_HISTORY_RES::PS_DB_EXCHANGE_PRICE_HISTORY_RES | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_MEMBER_POSITION::ST_LEAGUE_MEMBER_POSITION | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_LEAGUE_POSITION_NAME_CHANGE::ST_LEAGUE_POSITION_NAME_CHANGE | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | ST_REQ_LEAGUE_APPLICANT_REJECT::ST_REQ_LEAGUE_APPLICANT_REJECT | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_REQ_LEAGUE_APPLICANT_ACCEPT::ST_REQ_LEAGUE_APPLICANT_ACCEPT | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_BOARD::ST_LEAGUE_BOARD | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_MEMBER::ST_LEAGUE_MEMBER | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_MEMBER_EX::ST_LEAGUE_MEMBER_EX | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_REQ_LEAGUE_INVITE_ACCEPT::ST_REQ_LEAGUE_INVITE_ACCEPT | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_AUTH_CHANGE::ST_LEAGUE_AUTH_CHANGE | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_FIND_FRIEND_LIST::PS_FIND_FRIEND_LIST | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_FIND_FRIEND_LIST::~PS_FIND_FRIEND_LIST | - | verified | 源码POD默认析构; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_DB_FRIEND_FIND::PS_DB_FRIEND_FIND | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_RES_RECRUIT_ADD::PS_RES_RECRUIT_ADD | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_RECRUIT_LIST::~ST_RECRUIT_LIST | - | verified | 源码POD默认析构; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_RES_BLOCKLIST_DELETE::PS_RES_BLOCKLIST_DELETE | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_BLOCK_INFO::ST_BLOCK_INFO | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_RES_DB_FRIEND_BLOCK::PS_RES_DB_FRIEND_BLOCK | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_DB_FRIEND_DELETE::PS_DB_FRIEND_DELETE | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_DB_FRIEND_ACCEPT_RES::PS_DB_FRIEND_ACCEPT_RES | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_DB_FRIEND_ADD::ST_DB_FRIEND_ADD | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_FRIEND_INFO::ST_FRIEND_INFO | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_DB_FRIEND_LIST::PS_DB_FRIEND_LIST | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SetLeagueID | 0x1400638E0 | verified | IDA 0x1400638E0 + m_stCharInfo.stLeagueInfo.nLeagueID = nLeagueID; 源码UserObject.h:678 | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_REQ_LEAGUE_CREATE::ST_REQ_LEAGUE_CREATE | 0x140063900 | verified | IDA 0x140063900 + szName[0]=0+dwNpcID=0+nLeagueID=0+nErrorCode=0; 源码PSServer.h:3899默认初始化 | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_LEAGUE_CREATE_FOR_SERVER::PS_LEAGUE_CREATE_FOR_SERVER | - | verified | 源码PSServer.h:3907 POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_APPLICANT::ST_LEAGUE_APPLICANT | - | verified | 源码PSServer.h POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_NOTICE::ST_LEAGUE_NOTICE | - | verified | 源码PSServer.h POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::Init | 0x140063AA0 | verified | IDA 0x140063AA0 + 初始化DB连接池; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::Shutdown | 0x140063B50 | verified | IDA 0x140063B50 + 关闭连接池; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnDisConnect | 0x140063C30 | verified | IDA 0x140063C30 + 断开连接回调; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnConnect | 0x140063C70 | verified | IDA 0x140063C70 + 连接成功回调; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnNotConnect | 0x140063CB0 | verified | IDA 0x140063CB0 + 连接失败回调; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | XClient::IsBlock | - | verified | 源码内联方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | XPacket::GetMainCmd | - | verified | 源码内联getter; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnParse | 0x140063D30 | verified | IDA 0x140063D30 + 解析DB响应包; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::_BackendThreadStartingPoint | 0x140063E20 | verified | IDA 0x140063E20 + 后端线程入口; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::Init | 0x140063E60 | verified | IDA 0x140063E60 + 第二版本Init; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::SetSafetyShutdown | 0x140063F40 | verified | IDA 0x140063F40 + 设置安全关闭; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | XTime::GetTickCount | - | verified | 源码内联时间函数; IDA无单独函数(内联到调用点) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::OnStartThread | 0x140063FF0 | verified | IDA 0x140063FF0 + 线程启动回调; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocket.h | TXDBSocket<CServer>::~TXDBSocket<CServer> | 0x140064050 | verified | IDA 0x140064050 + 析构函数; 源码GameDBSocket.h模板 | 否 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_REQ_LEAGUE_DELEGATE::PS_REQ_LEAGUE_DELEGATE | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_APPLICANT_CHECK_LIST::~ST_LEAGUE_APPLICANT_CHECK_LIST | - | verified | 源码POD默认析构; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_APPLICANT_LIST::~ST_LEAGUE_APPLICANT_LIST | - | verified | 源码POD默认析构; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | PS_RES_RECRUIT_DELETE::PS_RES_RECRUIT_DELETE | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | PSServer.h | ST_LEAGUE_OPEN::ST_LEAGUE_OPEN | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::XGameDBSocketMgr | 0x14002D200 | verified | IDA @ 0x14002D200 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::~XGameDBSocketMgr | 0x14002D2A0 | verified | IDA @ 0x14002D2A0 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x14002D2C0 | verified | IDA @ 0x14002D2C0 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x14002E0A0 | verified | IDA @ 0x14002E0A0 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x14002E310 | verified | IDA @ 0x14002E310 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x14002E700 | verified | IDA @ 0x14002E700 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x14002E780 | verified | IDA @ 0x14002E780 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | TXDBSocket<CServer>::AutoConnect | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x14002E840 | verified | IDA @ 0x14002E840 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x14002E850 | verified | IDA @ 0x14002E850 | 否 | 一致 |
| GameServer/XRelayServer | GameDBSocketMgr.cpp | XServer::GetOption | 0x14002E860 | verified | IDA @ 0x14002E860 | 否 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::CHelperSupport | 0x14002E880 | verified | IDA 0x14002E880 + ST_HELPER_SUPPORT_INFO构造+m_setRecvList构造; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::~CHelperSupport | 0x14002E8B0 | verified | IDA 0x14002E8B0 + 默认析构; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::Init | 0x14002E8F0 | verified | IDA 0x14002E8F0 + qmemcpy复制m_stInfo; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::CheckReceived | 0x14002E950 | verified | IDA 0x14002E950 + find(m_setRecvList)+返回bool; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::AddReceived | 0x14002E9C0 | verified | IDA 0x14002E9C0 + insert(m_setRecvList)+上限检查; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::CheckVaildTime | 0x14002EA40 | verified | IDA 0x14002EA40 + 时间检查; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::GetRecvCount | 0x14002ED90 | verified | IDA 0x14002ED90 + return m_setRecvList.size(); 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.cpp | CHelperSupport::SetRewardState | 0x14002EDB0 | verified | IDA 0x14002EDB0 + m_byRewardState=byState; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupport.h | ST_HELPER_SUPPORT_INFO::ST_HELPER_SUPPORT_INFO | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::CHelperSupportMgr | 0x14002EE10 | verified | IDA 0x14002EE10 + CFSRWLock构造+map构造+m_mapSupport/m_mapSupportTime.clear(); 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::~CHelperSupportMgr | 0x14002EEA0 | verified | IDA 0x14002EEA0 + 默认析构; 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::FindSupport | 0x14002EF70 | verified | IDA 0x14002EF70 + find(m_mapSupport)+返回shared_ptr; 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::AddSupport | 0x14002F050 | verified | IDA 0x14002F050 + insert(m_mapSupport)+时间限制检查; 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::DeleteSupport | 0x14002F330 | verified | IDA 0x14002F330 + erase(m_mapSupport); 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::AddSupportTime | 0x14002F450 | verified | IDA 0x14002F450 + insert(m_mapSupportTime); 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::DeleteSupportTime | 0x14002F680 | verified | IDA 0x14002F680 + erase(m_mapSupportTime); 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::GetSupportReward | 0x14002F860 | verified | IDA 0x14002F860 + 遍历m_mapSupport+GetRewardState; 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupportMgr::AddSupportReceived | 0x14002F8E0 | verified | IDA 0x14002F8E0 + find+AddReceived; 源码HelperSupportMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupport::GetRewardState | 0x140032900 | verified | IDA 0x140032900 + return m_byRewardState; 源码HelperSupport.cpp | 是 | 一致 |
| GameServer/XRelayServer | HelperSupportMgr.cpp | CHelperSupport::GetSupportInfo | 0x1400329F0 | verified | IDA 0x1400329F0 + qmemcpy复制m_stInfo; 源码HelperSupport.cpp | 是 | 一致 |
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
| GameServer/XRelayServer | League.cpp | CLeague::UpdateApplicantName | 0x1400680a0 | verified | IDA 0x1400680a0 + find(m_mpLeagueApplicant)+wcscpy_s(szName,21)+SendChangeApplicantName; 源码League.cpp:434 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeApplicantName | 0x140068240 | verified | IDA 0x140068240 + XSendPacket(0xF6,0x41)+stApplicant+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeMemberName | 0x1400682d0 | verified | IDA 0x1400682d0 + XSendPacket(0xF6,0x42)+stUpdate+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeLeagueName | 0x140068360 | verified | IDA 0x140068360 + XSendPacket(0xF6,0x43)+stNameChange+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendNoticeToMember | 0x140068810 | verified | IDA 0x140068810 + XSendPacket(0xF6,0x23)+stNotice+dwActorID+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendRecordToMember | 0x140068c40 | verified | IDA 0x140068c40 + XSendPacket(0xF6,0x47)+stRecord+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeCardToMember | 0x140068cd0 | verified | IDA 0x140068cd0 + XSendPacket(0xF6,0x26)+stCard+m_nSyncCount+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLeagueWealthToMember | 0x140068d80 | verified | IDA 0x140068d80 + XSendPacket(0xF6,0x55)+dwUCID+stUpdate+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLevelupToMember | 0x140068e30 | verified | IDA 0x140068e30 + XSendPacket(0xF6,0x51)+nLeagueID+byLevel+bySkillPoint+psAutoSkill+m_nSyncCount+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLearnSkillToMember | 0x140068f30 | verified | IDA 0x140068f30 + XSendPacket(0xF6,0x53)+stSkill+m_nSyncCount+SendPacketAll | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendWithdrawToMember | 0x140068420 | verified | IDA 0x140068420 + XSendPacket(0xF6,8)+nLeagueID+dwUCID+biPenalty+stUpdate+m_nSyncCount+SendPacketAll; 源码League.cpp:1296 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendKickoutToMember | 0x140068510 | verified | IDA 0x140068510 + XSendPacket(0xF6,9)+nErrorCode+nLeagueID+dwReqUCID+dwTargetUCID+stUpdate+shLevel+m_nSyncCount+SendPacketAll; 源码League.cpp:1307 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangePositionToMember | 0x140068630 | verified | IDA 0x140068630 + XSendPacket(0xF6,0x37)+stPosition+nLeagueID+dwReqUCID+byPrevPosition+stLeagueInfoForGame+m_nSyncCount+SendPacketAll; 源码League.cpp:1320 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendDelegateToMember | 0x140068730 | verified | IDA 0x140068730 + XSendPacket(0xF6,7)+psDelegateRes+dwReqUCID+dwDelegatedUCID+m_nSyncCount+SendPacketAll; 源码League.cpp:1331 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLeagueInfo | 0x1400688c0 | verified | IDA 0x1400688c0 + GetLeagueMemberPtr+GetLeagueMember+LeagueMemberUpdate+GetMemberList+GetApplicantList+GetBoardList+GetRecordList+SetLeagueInfoForGame+XSendPacket(0xF6,3)+bLogin+stUpdate+stLeagueInfo+stMemberList+stApplicant+stBoard+byState+stInfoEx+stRecordList+stLeagueInfoForGame+m_nSyncCount+SendPacketAll; 源码League.cpp:1166 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendSyncLeagueInfo | 0x1400693c0 | verified | IDA 0x1400693c0 + CompareSyncCount+bSync=1+GetLeagueMemberPtr+LeagueMemberUpdate+GetApplicantList+GetBoardList+GetMemberList+GetRecordList+SetLeagueInfoForGame+GetUser+SendPacket(0xF6,0x58); 源码League.cpp:1213 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendInventoryInfo | 0x1400697c0 | verified | IDA 0x1400697c0 + XSendPacket(0xF6,0x60)+dwReqUCID+stStorage+stBroach+stSocket+stPackage+m_nInventoryCount+SendPacketAll; 源码League.cpp:1264 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendInventoryMove | 0x140069900 | verified | IDA 0x140069900 + psItemMoveInfoForGame->nInventorySync=m_nInventoryCount+XSendPacket(0xF6,0x61)+dwReqUCID+SendPacketAll; 源码League.cpp:1276 | 是 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::DelMember | 0x1400732a0 | verified | IDA 0x1400732a0 + m_mpLeagueMember.erase(dwUCID); 源码League.cpp:48 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::GetLeagueID | 0x140084850 | verified | IDA CUserObject::GetLeagueID 0x140084850 + return m_stCharInfo.stLeagueInfo.nLeagueID; CLeague::GetLeagueID为内联访问器m_stLeagueInfo.nLeagueID | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.h | CLeagueMember::~CLeagueMember | - | verified | 源码默认析构; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.h | CLeagueMember::CLeagueMember | 0x140064080 | verified | IDA 0x140064080 + m_bEnrollBoard=1; 源码LeagueMember.cpp:3 | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::Login | 0x1400640B0 | verified | IDA 0x1400640B0 + m_stMember.bLogin=1; 源码LeagueMember.cpp:15-17 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::LogOut | 0x1400640C0 | verified | IDA 0x1400640C0 + m_stMember.bLogin=0; 源码LeagueMember.cpp:19-21 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::SetPosition | 0x1400640D0 | verified | IDA 0x1400640D0 + m_stMember.byPosition=byPosition; 源码LeagueMember.cpp:23-25 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::SetEnrollBoardDate | 0x1400640F0 | verified | IDA 0x1400640F0 + m_stMember.biBoardLimitTime=biDate; 源码LeagueMember.cpp:27 | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::SetPlayDate | 0x140064110 | verified | IDA 0x140064110 + m_stMember.biPlayDate=biDate; 源码LeagueMember.cpp:31 | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.h | CLeagueMember::GetExp | - | verified | 源码内联getter; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::GetLeagueMember | 0x140064140 | verified | IDA 0x140064140 + qmemcpy复制112字节; 源码LeagueMember.cpp:11-13 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::SetName | 0x140064190 | verified | IDA 0x140064190 + 复制szName到m_stMember.szName; 源码LeagueMember.cpp:35-39 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::GetPosition | 0x140064200 | verified | IDA 0x140064200 + byPosition=m_stMember.byPosition; 源码LeagueMember.cpp:59-61 | 否 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::SetLeagueInfo | 0x140064220 | verified | IDA 0x140064220 + memcpy(m_stLeagueInfo, stInfo, 0x800); 源码League.cpp:35 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::CLeague | 0x140064270 | verified | IDA 0x140064270; 源码League.cpp:8 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::~CLeague | 0x140064350 | verified | IDA 0x140064350; 源码League.cpp:11 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::Clear | 0x1400643C0 | verified | IDA 0x1400643C0 + m_nInventoryCount=0 + m_nSyncCount=0; 源码League.cpp:52 | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::GetName | 0x1400643F0 | verified | IDA 0x1400643F0 + wcscpy_s(szName,21,m_stMember.szName); 源码LeagueMember.cpp:63-67 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::ResetExp | 0x140064420 | verified | IDA 0x140064420 + m_stMember.biLeagueExp=0; 源码LeagueMember.cpp:69-71 | 否 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::SetLeagueMember | 0x140064440 | verified | IDA 0x140064440 + qmemcpy复制112字节; 源码LeagueMember.cpp:7-9 | 否 | 一致 |
| GameServer/XRelayServer | League.h | CLeagueMember::UpdateMemberInfo | - | verified | 源码无此方法(可能在其他位置内联); IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueMember.cpp | CLeagueMember::AddExp | 0x140064490 | verified | IDA 0x140064490 + biLeagueExp+=shExp + 上限200; 源码LeagueMember.cpp:73-79 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeagueNotice | 0x1400644D0 | verified | IDA 0x1400644D0 + wcscpy_s szNotice + biNoticeDate; 源码League.cpp:352 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeaguePosition | 0x140064520 | verified | IDA 0x140064520 + nPosition 1/2/3 + wcscpy_s szPrevPositionName + wcscpy_s szPosition; 源码League.cpp:146 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LeagueMemberUpdate | 0x140064620 | verified | IDA 0x140064620 + GetLeagueMemberPtr + GetLeagueMember + ST_LEAGUE_MEMBER_UPDATE; 源码League.cpp:（需检查实现） | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::IsMaster | 0x1400646F0 | verified | IDA 0x1400646F0 + return m_stLeagueInfo.dwMasterUCID == dwUCID; 源码League.cpp:83 | 否 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::AddMember | 0x140064720 | verified | IDA 0x140064720 + find+insert到m_mpLeagueMember; 源码League.cpp:66 | 是 | 一致 |
| GameServer/XRelayServer | League.h | CLeague::GetDelName | 0x1400648E0 | verified | IDA @ 0x1400648E0 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LoginMember | 0x140064A00 | verified | IDA 0x140064A00 + find→LogHelper→Login; 源码League.cpp:55 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LogOutMember | 0x140064AF0 | verified | IDA 0x140064AF0 + find→LogOut→SetPlayDate; 源码League.cpp:70 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::AddApplicant | 0x140064BA0 | verified | IDA 0x140064BA0 + insert到m_mpLeagueApplicant; 源码League.cpp:259-262 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::DelApplicant | 0x140064C20 | verified | IDA 0x140064C20 + find+erase返回bool; 源码League.cpp:264-267 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::AddBoard | 0x140064CA0 | verified | IDA 0x140064CA0 + size>=50则pop+push; 源码League.cpp:297-303 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::IsMember | 0x140064D00 | verified | IDA 0x140064D00 + find(m_mpLeagueMember) != end; 源码League.cpp:79 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckApplicantCount | 0x140064D70 | verified | IDA 0x140064D70 + m_mpLeagueApplicant.size()<50; 源码League.cpp:273-276 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckExistApplicant | 0x140064DD0 | verified | IDA 0x140064DD0 + find(m_mpLeagueApplicant)!=end; 源码League.cpp:269-271 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetMemberBoardLimit | 0x140064E40 | verified | IDA 0x140064E40 + find(m_mpLeagueMember)+SetEnrollBoardDate; 源码League.cpp:129-140 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::GetApplicantList | 0x140064F20 | verified | IDA 0x140064F20 + 遍历m_mpLeagueApplicant+push_back; 源码League.cpp:278-282 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::GetApplicant | 0x140065010 | verified | IDA 0x140065010 + find(m_mpLeagueApplicant)+返回szName/nullptr; 源码League.cpp:285-291 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::GetBoardList | 0x140065080 | verified | IDA 0x140065080 + 遍历m_deqBoard + 最多50条; 源码League.cpp:305 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::GetMemberList | 0x140065130 | verified | IDA 0x140065130 + 遍历m_mpLeagueMember + GetLeagueMember + push_back; 源码League.cpp:95 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeagueAuth | 0x140065310 | verified | IDA 0x140065310 + 复制9个int到nAuth; 源码League.cpp:385-392 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckPositionCount | 0x140065360 | verified | IDA 0x140065360 + 遍历成员统计各职位数量; 源码League.cpp:196-232 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeagueRecruitNotice | 0x140065530 | verified | IDA 0x140065530 + 复制招募公告到m_stLeagueInfo; 源码League.cpp:358-362 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ChangeMemberPosition | 0x140065580 | verified | IDA 0x140065580 + GetPosition+SetPosition+SetSubLeagueMaster+SendChangePositionToMember; 源码League.cpp:162-194 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckLeagueDelegate | 0x1400657C0 | verified | IDA 0x1400657C0 + IsMaster检查+成员数>=2+评分>=2检查; 源码League.cpp:468-499 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::Delegate | 0x140065910 | verified | IDA 0x140065910 + SetPosition(0/100)+更新dwMasterUCID+记录; 源码League.cpp:501-529 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::GetRecordList | 0x140065F30 | verified | IDA 0x140065F30 + 遍历m_deqRecord + 最多100条; 源码League.cpp:339 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CardChange | 0x140065FE0 | verified | IDA 0x140065FE0 + 设置m_stLeagueInfo.dwLeagueCard; 源码League.cpp:578-581 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::HaveSkill | 0x140066010 | verified | IDA 0x140066010 + 检查m_stLeagueInfo.bySkill[eSkill]!=0; 源码League.cpp:587-593 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CheckLearnSkill | 0x140066050 | verified | IDA 0x140066050 + IsMaster检查+技能表检查+前置技能+等级上限+技能点+金币; 源码League.cpp:595-651 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LearnSkill | 0x140066280 | verified | IDA 0x140066280 + 扣减金币/技能点+更新技能等级+SendLearnSkillToMember+SendDBLog; 源码League.cpp:653-704 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::Levelup | 0x140066590 | verified | IDA 0x140066590 + 检查等级上限<=10+TB_LEAGUE_INFO+遍历TB_LEAGUE_SKILL自动学习被动技能+SendDBGame(7,0x34); 源码League.cpp:706-760 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ApplyLevelup | 0x1400668A0 | verified | IDA 0x1400668A0 + 更新byRating/bySkillPoint/bySkill数组+SendLevelupToMember+SendDBLog(15,17/20); 源码League.cpp:773-822 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CalculateExp | 0x140066B80 | verified | IDA 0x140066B80 + 经验上限检查(联赛日上限+成员200上限)+金币上限(999999999)+升级检查+SendDBGame(7,0x33); 源码League.cpp:828-970 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ApplyWealth | 0x1400671A0 | verified | IDA 0x1400671A0 + AddExp+SendLeagueWealthToMember+SendDBLog(15,18/19); 源码League.cpp:972-1020 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::LoadRecord | 0x140067510 | verified | IDA 0x140067510 + push到m_stRecordList; 源码League.cpp:318-321 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateRecord | 0x140067540 | verified | IDA 0x140067540 + size>100则pop+push+SendDBGame(7,0x30)+SendRecordToMember; 源码League.cpp:323-337 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ResetExp | 0x140067640 | verified | IDA 0x140067640 + 设置biInitDate+nLimitExp=0+遍历成员ResetExp; 源码League.cpp:1019-1032 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ResetExpInitDate | 0x140067730 | verified | IDA 0x140067730 + SendDBGame(7,0x36)+GetLeagueMemberPtr+ResetExp; 源码League.cpp:1034-1047 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateApplyList | 0x140067820 | verified | IDA 0x140067820 + 遍历申请者+CTime超时检查(86400秒)+SendDBGame(7,0x20)+erase; 源码League.cpp:1053-1074 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeagueInfoForGame | 0x140067A00 | verified | IDA 0x140067A00 + 设置byLeagueLevel+byPosition+dwMasterUCID+bySkill[8]+nAuth[9]; 源码League.cpp:1108-1119 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateSkillPoint_Cheat | 0x140067AC0 | verified | IDA 0x140067AC0 + 设置bySkillPoint+PS_AUTO_SKILL+SendDBGame(7,0x34)+SendLevelupToMember; 源码League.cpp:1121-1147 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateSyncCount | 0x140067C70 | verified | IDA 0x140067C70 + ++m_nSyncCount; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateInventorySyncCount | 0x140067C90 | verified | IDA 0x140067C90 + ++m_nInventoryCount; 源码League.cpp:1104-1106 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpDateLeagueMemberInfo | 0x140067CB0 | verified | IDA 0x140067CB0 + 遍历成员+GetLeagueMember+GetUser+GetLeagueMemberInfo+SetLeagueMember; 源码League.cpp:1076-1098 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetSubLeagueMaster | 0x140067E60 | verified | IDA 0x140067E60 + wcscpy_s(szSubMasterName,21); 源码League.cpp:234-239 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetMasterName | 0x140067E90 | verified | IDA 0x140067E90 + wcscpy_s(szMasterName,21); 源码League.cpp:241-246 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SetLeagueName | 0x140067EC0 | verified | IDA 0x140067EC0 + wcscpy_s(szLeagueName,10); 源码League.cpp:248-253 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::ChangeMemberName | 0x140067EF0 | verified | IDA 0x140067EF0 + GetLeagueMemberPtr+IsMaster/SetSubLeagueMaster+SetName+SendChangeMemberName; 源码League.cpp:406-432 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::UpdateApplicantName | 0x1400680A0 | verified | IDA 0x1400680A0 + find申请者+wcscpy_s+SendChangeApplicantName; 源码League.cpp:434-443 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeApplicantName | 0x140068240 | verified | IDA 0x140068240 + XSendPacket(0xF6,0x19)广播申请者名称变更; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeMemberName | 0x1400682D0 | verified | IDA 0x1400682D0 + XSendPacket(0xF6,0x42)+SendPacketAll; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeLeagueName | 0x140068360 | verified | IDA 0x140068360 + XSendPacket(0xF6,0x25)+SendPacketAll; 源码League.cpp:1285 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendWithdrawToMember | 0x140068420 | verified | IDA 0x140068420 + XSendPacket(0xF6,8)+SendPacketAll; 源码League.cpp:1296 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendKickoutToMember | 0x140068510 | verified | IDA 0x140068510 + XSendPacket(0xF6,9)+SendPacketAll; 源码League.cpp:1307 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangePositionToMember | 0x140068630 | verified | IDA 0x140068630 + XSendPacket(0xF6,0x37)+SendPacketAll; 源码League.cpp:1320 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendDelegateToMember | 0x140068730 | verified | IDA 0x140068730 + XSendPacket(0xF6,7)+SendPacketAll; 源码League.cpp:1331 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendNoticeToMember | 0x140068810 | verified | IDA 0x140068810 + XSendPacket(0xF6,0x20)+SendPacketAll公告; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLeagueInfo | 0x1400688c0 | verified | IDA 0x1400688c0 + XSendPacket(0xF6,3)+SendPacketAll联赛信息广播; 源码League.cpp:1166 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendRecordToMember | 0x140068C40 | verified | IDA 0x140068C40 + XSendPacket(0xF6,0x48)+SendPacketAll记录广播; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendChangeCardToMember | 0x140068CD0 | verified | IDA 0x140068CD0 + XSendPacket(0xF6,0x48)+SendPacketAll卡片变更; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLeagueWealthToMember | 0x140068D80 | verified | IDA 0x140068D80 + XSendPacket(0xF6,0x41)+SendPacketAll财富更新; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLevelupToMember | 0x140068E30 | verified | IDA 0x140068E30 + XSendPacket(0xF6,0x57)+SendPacketAll升级广播; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendLearnSkillToMember | 0x140068F30 | verified | IDA 0x140068F30 + XSendPacket(0xF6,0x39)+SendPacketAll技能学习广播; 源码League.cpp | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::Levelup_Cheat | 0x140068FE0 | verified | IDA 0x140068FE0 + GM升级功能+Levelup+SendDBGame(7,0x34)+SendLevelupToMember; 源码League.cpp:144+League.cpp:760 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::CompareSyncCount | 0x140069380 | verified | IDA 0x140069380 + 返回 m_nSyncCount != nCount; 源码League.cpp:1153-1156 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendSyncLeagueInfo | 0x1400693c0 | verified | IDA 0x1400693c0 + XSendPacket(0xF6,0x58)单播同步联赛信息; 源码League.cpp:1213 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendInventoryInfo | 0x1400697c0 | verified | IDA 0x1400697c0 + XSendPacket(0xF6,0x60)+仓库信息; 源码League.cpp:1264 | 否 | 一致 |
| GameServer/XRelayServer | League.cpp | CLeague::SendInventoryMove | 0x140069900 | verified | IDA 0x140069900 + XSendPacket(0xF6,0x61)+仓库移动通知; 源码League.cpp:1276 | 否 | 一致 |
| GameServer/XRelayServer | League.h | wcscpy_s<10> | 0x140446512 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | POD模板内联 |
| GameServer/XRelayServer | League.h | ST_LEAGUE_INFO_EX::ST_LEAGUE_INFO_EX | 0x140465824 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | League.h | XParse::operator<< | 0x140465888 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | POD模板内联 |
| GameServer/XRelayServer | League.h | ST_LEAGUE_MEMBER_UPDATE::ST_LEAGUE_MEMBER_UPDATE | 0x140465936 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | League.h | ST_LEAGUE_INFO_UPDATE::ST_LEAGUE_INFO_UPDATE | 0x140466064 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | League.h | XResourceMgr::GetTB_LEAGUE_INFO | 0x140466576 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | League.h | XResourceMgr::GetTB_LEAGUE_SKILL | 0x140467168 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | League.h | ST_LEAGUE_RECORD::ST_LEAGUE_RECORD | 0x140467280 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | League.h | ST_LEAGUE_INFO_FOR_GAME::ST_LEAGUE_INFO_FOR_GAME | 0x140467376 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | League.h | XParse::SetSHORT | 0x140890096 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | POD模板内联 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::{构造, Clear} | 0x140073440 | verified | IDA 0x140073440/0x140073520 + 构造:初始化成员+Clear() / Clear:clear各容器+m_bLoadLeague/m_bLeague等标志位=0+m_tUpdate/m_tInitDate=GetTickCount+m_nLeagueListIndex=0 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::AddLeague | 0x1400737A0 | verified | IDA 0x1400737A0 + new CLeague+SetLeagueInfo+AddMember+insert(m_mpLeagueList)+push_back(m_vecLeagueList) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueLogin | 0x140073970 | verified | IDA 0x140073970 + find→LoginMember→SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueApplicant | 0x140073AC0 | verified | IDA 0x140073AC0 + find→AddApplicant+GetMemberList+XSendPacket(0xF6,0x19)给请求者+XSendPacket(0xF6,0x20)广播(SendPacketAll) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInfo | 0x140073CF0 | verified | IDA 0x140073CF0 + find(nLeagueID)→GetLeagueMemberPtr(dwActorID)→GetPosition→GetLeagueInfo/GetMemberList/GetApplicantList/GetBoardList/GetRecordList/SetLeagueInfoForGame→SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInfo | 0x140073cf0 | verified | IDA 0x140073cf0 + find→GetLeagueMemberPtr→GetPosition→GetLeagueInfo/MemberList/ApplicantList/BoardList/RecordList→SetLeagueInfoForGame→SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueDel | 0x140074D60 | verified | IDA 0x140074D60 + find→IsMaster→GetMemberCount<=1→SendDBGame(7,1) / 57018=成员仍存在 / 57015=非Master / 57016=联赛不存在 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueBoard | 0x1400751D0 | verified | IDA 0x1400751D0 + find→GetLeagueMemberPtr→GetLeagueMember→biBoardLimitTime检查(1800秒冷却)→SendDBGame(7,5) / 57022=冷却中 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueKick | 0x140074620 | verified | IDA 0x140074620 + find→GetLeagueMemberPtr→IsMaster/IsSelf/Auth检查→SendDBGame(7,0xB) / 0xDEB8=联赛不存在 / 0xDEAE=无权限 / 0xDECA=踢自己 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqInviteAccept | 0x1400758A0 | verified | IDA 0x1400758A0 + find→GetLeagueInfo→GetTB_LEAGUE(byRating)→GetMemberCount<League_Member→SendDBGame(7,0xF) / 57016=联赛不存在 / 57018=成员已满 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueApplicantAccept | 0x1400774B0 | verified | IDA 0x1400774B0 + find→GetLeagueInfo→GetTB_LEAGUE→GetMemberCount→GetLeagueMemberPtr→GetApplicant→IsMaster/auth检查→SendDBGame(7,0x10) / 57016/57018/57007/57006 | 否 | 一致 |
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
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateLeagueInfo | 0x14007BC00 | verified | IDA 0x14007BC00 + find→GetLeagueInfo→填充ST_LEAGUE_INFO_UPDATE(nLeagueID/nLeagueRank/shMemberCount/biMoney/biExp/dwLeagueCard) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendMemberUpdate | 0x14007BDE0 | verified | IDA 0x14007BDE0 + 构造ST_LEAGUE_MEMBER_UPDATE→XSendPacket(0xF6,0x42)→SendPacketAll | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberLevel | 0x14007BFA0 | verified | IDA 0x14007BFA0 + GetUser→GetLeagueID→find→GetLeagueMemberPtr→SetLevel→SendMemberUpdate | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberAwaken | 0x14007C250 | verified | IDA 0x14007C250 + GetUser→GetLeagueID→find→GetLeagueMemberPtr→SetAwaken→SendMemberUpdate | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberProfilePhoto | 0x14007C500 | verified | IDA 0x14007C500 + GetUser→GetLeagueID→find→GetLeagueMemberPtr→SetProfilePhoto→SendMemberUpdate | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberMapInfo | 0x14007C7B0 | verified | IDA 0x14007C7B0 + GetUser→GetLeagueID→find→GetLeagueMemberPtr→SetMapInfo→SendMemberUpdate | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueMemberWithdraw | 0x14007D400 | verified | IDA 0x14007D400 + find→UpdateSyncCount→SendWithdrawToMember→UpdateRecord(byFlag=2) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueMemberKick | 0x14007D690 | verified | IDA 0x14007D690 + find→LogError(2362/2369)→GetMemberList→UpdateSyncCount→SendKickoutToMember→GetLeagueMemberPtr→GetName→UpdateRecord(byFlag=3,szValue1=pName,szValue2=szReqUserName) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueRecordUpdate | 0x140080330 | verified | IDA 0x140080330 + find→LogError(3031/3038)→UpdateRecord(stRecord) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueSkillPointUpdate | 0x140080530 | verified | IDA 0x140080530 + find→LogError(3096/3103)→UpdateSkillPoint_Cheat(bySkillPoint,dwUCID) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SyncLeagueInfo | 0x140080640 | verified | IDA 0x140080640 + find→LogError(3115/3122)→SendSyncLeagueInfo(stSync) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInventoryMove | 0x140080BA0 | verified | IDA 0x140080BA0 + find→LogError(3168)→XSendDBPacket(0,7,0x37)+dwReqUCID+operator<<(stMove)+SendDBGame | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueInventoryMove | 0x140080CF0 | verified | IDA 0x140080CF0 + find→LogError(3183/3190)→nErrorCode==0→UpdateInventorySyncCount→byType==0(byFlag=6存入)/byType==1(byFlag=7取出)→UpdateRecord→SendInventoryMove | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetLeagueID | 0x140079168 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.h | UXActorID::operator unsigned long | 0x140079168 | verified | PDB dump symbols; IDA无单独函数(POD内联) | 否 | POD内联 |
| GameServer/XRelayServer | LeagueManager.h | PS_GMT_LEAGUE_UPDATE_LIST::~PS_GMT_LEAGUE_UPDATE_LIST | 0x140266560 | verified | PDB dump symbols; IDA无单独函数(POD析构内联) | 否 | POD内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetLevel | 0x140467472 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetAwaken | 0x140467504 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetProfilePhoto | 0x140467536 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeagueMember::SetMapInfo | 0x140467568 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::DelMember | 0x140467616 | verified | PDB dump symbols; IDA无单独函数(内联) | 否 | 内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetMemberCount | 0x140467664 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetLeagueNoticeDate | 0x140467696 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetLeagueOpenOrNot | 0x140467728 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::SetLeagueRecruitNoticeDate | 0x140467760 | verified | PDB dump symbols; IDA无单独函数(setter内联) | 否 | setter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetMasterUCID | 0x140467792 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetLeagueInfo | 0x140467808 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetMemberCount | 0x140467888 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetNoticeDate | 0x140467936 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetRecruitNoticeDate | 0x140467968 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeague::GetSyncCount | 0x140468000 | verified | PDB dump symbols; IDA无单独函数(getter内联) | 否 | getter内联 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::CLeagueManager | 0x140073440 | verified | IDA 0x140073440 + map构造+m_mpLeagueInvite/m_mpLeagueList初始化+m_vecLeagueList构造+Clear; 源码LeagueManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::~CLeagueManager | 0x1400734C0 | verified | IDA 0x1400734C0 + 默认析构; 源码LeagueManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::Clear | 0x140073520 | verified | IDA 0x140073520 + m_mpLeagueList/m_mpLeagueInvite/m_vecLeagueList.clear+标志位重置+m_tUpdate/m_tInitDate初始化; 源码LeagueManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::GetMemberInfo | 0x1400735D0 | verified | IDA 0x1400735D0 + find league + GetLeagueMemberPtr + GetLeagueMember | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueApplicant | 0x140073AC0 | verified | IDA 0x140073AC0 + AddApplicant + XSendPacket(0xF6,0x19) + SendEx + XSendPacket(0xF6,0x20) + SendPacketAll | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInfo | 0x140073CF0 | verified | IDA 0x140073CF0 + GetLeagueMemberPtr + GetPosition + GetLeagueInfo/MemberList/ApplicantList/BoardList/RecordList + SetLeagueInfoForGame + SendLeagueInfo | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueWithDraw | 0x140074060 | verified | IDA 0x140074060 + find league + IsMember + IsMaster check + XSendDBPacket(7,6) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueWithdraw | 0x140074350 | verified | IDA 0x140074350 + GetMemberInfo + byPosition==7 + DeleteLeagueMember + UpdateLeagueInfo + SetLeagueID(0) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueKick | 0x140074620 | verified | IDA 0x140074620 + find league+GetLeagueMemberPtr+IsMaster+auth检查+SendDBGame(7,0xB); 源码LeagueManager.cpp:858 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueDel | 0x140074D60 | verified | IDA 0x140074D60 + find league+IsMaster+GetMemberCount<=1+SendDBGame(7,1); 源码LeagueManager.cpp:856 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueBoard | 0x1400751D0 | verified | IDA 0x1400751D0 + find league+GetLeagueMemberPtr+biBoardLimitTime检查(1800秒冷却)+SendDBGame(7,5); 源码LeagueManager.cpp:857 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueBoard | 0x140075400 | verified | IDA 0x140075400 + find league+AddBoard+UpdateSyncCount+SendPacketAll(0xF6,0x46); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqInviteAccept | 0x1400758A0 | verified | IDA 0x1400758A0 + find league+GetMemberCount<League_Member+SendDBGame(7,0xF); 源码LeagueManager.cpp:859 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResInviteUser | 0x140075c50 | verified | IDA 0x140075c50 + AddMember+SetLeagueInfoForGame+DelApplicant+UpdateLeagueInfo+UpdateRecord+UpdateSyncCount+SendLeagueInfo+SendLeagueInviteJoin; 源码LeagueManager.cpp:861 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::AppliCantJoinSucc | 0x1400760A0 | verified | IDA 0x1400760A0 + 广播申请人满; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::DeleteApplicantList | 0x140076AE0 | verified | IDA 0x140076AE0 + 遍历申请者+DelApplicant; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueApplicantJoin | 0x140076C60 | verified | IDA 0x140076C60 + XSendPacket(0xF6,0x1B)+广播新成员加入; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueInviteJoin | 0x140076D60 | verified | IDA 0x140076D60 + XSendPacket(0xF6,0x1C)+广播邀请加入; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueInfo | 0x140076E70 | verified | IDA 0x140076E70 + XSendPacket(0xF6,3)+联赛信息广播; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ApplicantRejectSucc | 0x140076FA0 | verified | IDA 0x140076FA0 + 申请人拒绝成功处理+SendLeagueApplicantJoin; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::DelLeague | 0x140077220 | verified | IDA 0x140077220 + m_mpLeagueList.erase + 遍历m_vecLeagueList查找+erase | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueErrorMsg | 0x1400772E0 | verified | IDA 0x1400772E0 + XSendPacket(0xF6,0x18)+发送错误消息; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueErrorMsg | 0x140077390 | verified | IDA 0x140077390 + XSendPacket(0xF6,0x18)+重载版本(dwActorID+nErrorCode); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueApplicantAccept | 0x1400774B0 | verified | IDA 0x1400774B0 + find league+GetMemberCount+IsMaster+auth检查+SendDBGame(7,0x10); 源码LeagueManager.cpp:860 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueApplicantReject | 0x140077B50 | verified | IDA 0x140077B50 + find league+GetApplicant+IsMaster+SendDBGame(7,0x12)+DelApplicant; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::AddInviteUser | 0x140077FC0 | verified | IDA 0x140077FC0 + insert(m_mpInviteUser); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::CheckInviteUser | 0x140078090 | verified | IDA 0x140078090 + m_mpInviteUser.find检查邀请; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::DeleteInviteUser | 0x140078110 | verified | IDA 0x140078110 + m_mpInviteUser.erase删除邀请; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::CheckLeagueInfo | 0x140487856 | verified | PDB dump symbols; IDA无单独函数(内联) | 否 | 内联 |
| GameServer/XRelayServer | LeagueManager.h | CLeagueManager::ReqLeagueApplicant | 0x140487952 | verified | PDB dump symbols; IDA无单独函数(内联) | 否 | 内联 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::DeleteLeagueMember | 0x1400785D0 | verified | IDA 0x1400785D0 + GetDelName + DelMember | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInvite | 0x1400786D0 | verified | IDA 0x1400786D0 + 邀请用户加入联赛+CheckLeagueInfo+CheckInviteUser+AddInviteUser+SendDBGame(7,0xD); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueNoticeChange | 0x140078FF0 | verified | IDA 0x140078FF0 + 公告变更+IsMaster检查+SendDBGame(7,3); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueNoticeChange | 0x1400795E0 | verified | IDA 0x1400795E0 + SetLeagueNotice+SendPacketAll(0xF6,0x20); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::CreateLeague | 0x1400797C0 | verified | IDA 0x1400797C0 + wcscpy_sLeagueName/MasterName + nLeagueID/shMemberCount=1 + bySkillPoint=1 + nAuth[4/5/7] + AddLeague | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResCreateLeague | 0x140079A50 | verified | IDA 0x140079A50 + GetUser + GetChannel + CreateLeague + DeleteApplicantList + XSendPacket(0xF6,1) + stLeagueInfo+stMemberInfo+dwActorID | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueChangeAuth | 0x140079DE0 | verified | IDA 0x140079DE0 + IsMaster检查+SendDBGame(7,4); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueAuthChange | 0x14007A070 | verified | IDA 0x14007A070 + SetLeagueAuth+SendPacketAll(0xF6,0x23); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeaguePositionNameChange | 0x14007A290 | verified | IDA 0x14007A290 + 职位名称变更+IsMaster检查+SendDBGame(7,6); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeaguePositionNameChange | 0x14007A500 | verified | IDA 0x14007A500 + SetLeaguePosition+SendPacketAll(0xF6,0x24); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueMessage | 0x14007A710 | verified | IDA 0x14007A710 + XSendPacket(0xF6,0x2B)+联赛消息广播; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueMemberPositionChange | 0x14007A8D0 | verified | IDA 0x14007A8D0 + 成员职位变更+IsMaster/auth检查+SendDBGame(7,7); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueMemberPositionChange | 0x14007B040 | verified | IDA 0x14007B040 + ChangeMemberPosition+UpdateSyncCount+SendChangePositionToMember; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::LogOutLeagueMember | 0x14007B360 | verified | IDA 0x14007B360 + LogOutMember + GetLeagueMemberPtr + LeagueMemberUpdate + XSendPacket(0xF6,0x39) | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueApplicantDelete_TimeOver | 0x14007B5C0 | verified | IDA 0x14007B5C0 + 申请者超时删除; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::OnUpdate | 0x14007B740 | verified | IDA 0x14007B740 + 时间检查+m_tUpdate更新+InitLeaguExp(9点初始化)+遍历UpdateApplyList+LoadGMTLeagueInfo+SendGMTLeagueInfo; 源码LeagueManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueLogin | 0x140073970 | verified | IDA 0x140073970 + find league+LoginMember+SendLeagueInfo; 源码LeagueManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | PS_GMT_LEAGUE_UPDATE_LIST::PS_GMT_LEAGUE_UPDATE_LIST | 0x140502320 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendGMTLeagueInfo | 0x14007BA60 | verified | IDA 0x14007BA60 + GMT联赛信息广播; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::InitLeaguExp | 0x14007BB00 | verified | IDA 0x14007BB00 + 初始化联赛经验+遍历ResetExp+SendDBGame(7,0x40); 源码LeagueManager.cpp:863 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateLeagueInfo | 0x14007BC00 | verified | IDA 0x14007BC00 + 更新联赛信息+nLeagueID+stUpdate+SendPacketAll(0xF6,0x41); 源码LeagueManager.cpp:879 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendMemberUpdate | 0x14007BDE0 | verified | IDA 0x14007BDE0 + 发送成员更新+SendPacketAll(0xF6,0x42); 源码LeagueManager.cpp:880 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberLevel | 0x14007BFA0 | verified | IDA 0x14007BFA0 + 更新成员等级+GetLeagueID+GetLeagueMemberPtr+SetLevel+SendMemberUpdate; 源码LeagueManager.cpp:881 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberAwaken | 0x14007C250 | verified | IDA 0x14007C250 + 更新成员觉醒+SetAwaken+SendMemberUpdate; 源码LeagueManager.cpp:882 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberProfilePhoto | 0x14007C500 | verified | IDA 0x14007C500 + 更新成员头像+SetProfilePhoto+SendMemberUpdate; 源码LeagueManager.cpp:883 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateMemberMapInfo | 0x14007C7B0 | verified | IDA 0x14007C7B0 + 更新成员地图信息+SetMapInfo+SendMemberUpdate; 源码LeagueManager.cpp:884 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueMember | 0x14007CA50 | verified | IDA 0x14007CA50 + 加载联赛成员+bMember=bLoad+遍历member+AddMember+byPosition==7时SetSubLeagueMaster; 源码LeagueManager.cpp:864 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueApplicant | 0x14007CCB0 | verified | IDA 0x14007CCB0 + 加载联赛申请者+bApplicant=bLoad+遍历applicant+AddApplicant; 源码LeagueManager.cpp:865 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueBoard | 0x14007CE90 | verified | IDA 0x14007CE90 + 加载联赛公告板+bBoard=bLoad+遍历board+AddBoard; 源码LeagueManager.cpp:866 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLoadLeagueRecord | 0x14007D080 | verified | IDA 0x14007D080 + 加载联赛记录+bRecord=bLoad+遍历record+LoadRecord+SendInfoToGameServer; 源码LeagueManager.cpp:867 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendInfoToGameServer | 0x14007D200 | verified | IDA 0x14007D200 + 发送联赛信息到游戏服务器+检查5个加载标志+SetCachingLoad; 源码LeagueManager.cpp:868 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateLeagueMemberInfo | 0x14007D270 | verified | IDA 0x14007D270 + 更新联赛成员信息+遍历league+UpDateLeagueMemberInfo+UpdateSyncCount+SendPacketAll(0xF6,0x59); 源码LeagueManager.cpp:869 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueMemberWithdraw | 0x14007D400 | verified | IDA 0x14007D400 + 发送成员退出+UpdateSyncCount+SendWithdrawToMember+UpdateRecord(byFlag=2); 源码LeagueManager.cpp:885 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendLeagueMemberKick | 0x14007D690 | verified | IDA 0x14007D690 + 发送踢人+UpdateSyncCount+SendKickoutToMember+UpdateRecord(byFlag=3); 源码LeagueManager.cpp:886 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueOpenOrNot | 0x14007D960 | verified | IDA 0x14007D960 + 联赛开放/关闭设置+IsMaster+SendDBGame(7,0x17); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueOpenOrNot | 0x14007DB60 | verified | IDA 0x14007DB60 + SetLeagueOpenOrNot+SendPacketAll(0xF6,0x43); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueSearch | 0x14007DE20 | verified | IDA 0x14007DE20 + 联赛搜索结果+XSendPacket(0xF6,0x45); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueRecruitNotice | 0x14007DF50 | verified | IDA 0x14007DF50 + 招募公告+IsMaster+SendDBGame(7,8); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueRecruitNotice | 0x14007E480 | verified | IDA 0x14007E480 + SetLeagueRecruitNotice+SendPacketAll(0xF6,0x2C); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueDelegate | 0x14007E630 | verified | IDA 0x14007E630 + find league+CheckLeagueDelegate(非GM)+SendDBGame(7,0x32)或SendEx(0xF6,7); 源码LeagueManager.cpp | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueDelegate | 0x14007E9D0 | verified | IDA 0x14007E9D0 + Delegate转让会长+SendDelegateToMember; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueCardChange | 0x14007ED40 | verified | IDA 0x14007ED40 + 联赛卡片变更+CheckLeagueCardChange+SendDBGame(7,0x16); 源码LeagueManager.cpp:871 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueCardChange | 0x14007F090 | verified | IDA 0x14007F090 + SetLeagueInfoForGame+CardChange+UpdateSyncCount+SendPacket(0xF6,0x48)+SendChangeCardToMember+UpdateRecord(byFlag=11); 源码LeagueManager.cpp:872 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqApplyLeagueExp | 0x14007F550 | verified | IDA 0x14007F550 + 申请联赛经验+CalculateExp; 源码LeagueManager.cpp:873 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResApplyLeagueWealth | 0x14007F690 | verified | IDA 0x14007F690 + 申请联赛财富响应+ApplyWealth; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueLevelup | 0x14007F7D0 | verified | IDA 0x14007F7D0 + UpdateSyncCount+ApplyLevelup+UpdateRecord(byFlag=8); 源码LeagueManager.cpp:874 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueSkillLearn | 0x14007F9B0 | verified | IDA 0x14007F9B0 + 学习联赛技能+CheckLearnSkill+LearnSkill; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueSkillLearn | 0x14007FD30 | verified | IDA 0x14007FD30 + SendLearnSkillToMember; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueLevelup | 0x140080010 | verified | IDA 0x140080010 + Levelup_Cheat; 源码LeagueManager.cpp:875 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SendFailLeagueLogin | 0x140080120 | verified | IDA 0x140080120 + 发送联赛登录失败+XSendPacket(0xF6,1); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueRecordUpdate | 0x140080330 | verified | IDA 0x140080330 + UpdateRecord(stRecord); 源码LeagueManager.cpp:887 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueMemberExpInit | 0x140080430 | verified | IDA 0x140080430 + ResetExpInitDate; 源码LeagueManager.cpp:876 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueSkillPointUpdate | 0x140080530 | verified | IDA 0x140080530 + UpdateSkillPoint_Cheat(bySkillPoint,dwUCID); 源码LeagueManager.cpp:888 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::SyncLeagueInfo | 0x140080640 | verified | IDA 0x140080640 + SendSyncLeagueInfo(stSync); 源码LeagueManager.cpp:889 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInevntoryInfo | 0x140080790 | verified | IDA 0x140080790 + 请求联赛仓库信息+SendDBGame(7,0x35); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueInventoryInfo | 0x1400808B0 | verified | IDA 0x1400808B0 + SendInventoryInfo(dwReqUCID,stStorage,stBroach,stSocket,stPackage); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueInventoryMove | 0x140080BA0 | verified | IDA 0x140080BA0 + XSendDBPacket(0,7,0x37)+dwReqUCID+stMove+SendDBGame; 源码LeagueManager.cpp:890 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueInventoryMove | 0x140080CF0 | verified | IDA 0x140080CF0 + UpdateInventorySyncCount+byType==0(byFlag=6存入)/byType==1(byFlag=7取出)+UpdateRecord+SendInventoryMove; 源码LeagueManager.cpp:891 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::GetApplicantList | 0x1400812D0 | verified | IDA 0x1400812D0 + 获取申请者列表+遍历m_mpLeagueList; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueCreate | 0x140081450 | verified | IDA 0x140081450 + 创建联赛请求+CreateLeague+DeleteApplicantList+SendDBGame(7,0)+XSendPacket(0xF6,1); 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeagueKickout | 0x1400815D0 | verified | IDA 0x1400815D0 + find league + GetMemberInfo + DeleteLeagueMember + UpdateLeagueInfo + SetLeagueID(0) + SendLeagueMemberKick | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ReqLeagueNameChange | 0x1400818D0 | verified | IDA 0x1400818D0 + find league + IsMaster check + returns true/false | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ResLeaugeNameChange | 0x140081A80 | verified | IDA 0x140081A80 + find league + SetLeagueName + UpdateSyncCount + SendChangeLeagueName | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ChangeLeagueMemberName | 0x140081C70 | verified | IDA 0x140081C70 + find league+ChangeMemberName; 源码LeagueManager.cpp:877 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::ChangeLeagueApplicant | 0x140081D60 | verified | IDA 0x140081D60 + 遍历stApplyList.vecApplyLeuage+find league+UpdateApplicantName; 源码LeagueManager.cpp:878 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::LoadLeagueInfo | 0x140081E90 | verified | IDA 0x140081E90 + DB league load + AddLeague + member/board/applicant/record遍历 | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.cpp | CLeagueManager::UpdateGMTLeagueInfo | 0x140082590 | verified | IDA 0x140082590 + 更新GMT联赛信息+遍历m_mpLeagueList; 源码LeagueManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | PS_LEAGUE_INFO_SUMMARY::PS_LEAGUE_INFO_SUMMARY | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SetLockLeague | 0x140084720 | verified | IDA 0x140084720 + m_bLockLeague = bLock; 源码UserObject.h:680 | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | PS_RES_LEAGUE_DELEGATE::PS_RES_LEAGUE_DELEGATE | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | PS_GMT_LEAGUE_UPDATE_LIST::PS_GMT_LEAGUE_UPDATE_LIST | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueWithdrawPenalty | 0x140084830 | verified | IDA 0x140084830 + return m_biLeagueWithdrawPenalty; 源码UserObject.h:892 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueID | 0x140084850 | verified | IDA 0x140084850 + return m_stCharInfo.stLeagueInfo.nLeagueID; 源码UserObject.h:679 | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | ST_LEAGUE_INVITE_INFO::ST_LEAGUE_INVITE_INFO | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetChannel | - | verified | 源码UserObject.h:581返回0(stub); IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | LeagueManager.h | PS_RES_LEAGUE_ACCEPT_ACCPLICANT::PS_RES_LEAGUE_ACCEPT_ACCPLICANT | - | verified | 源码POD默认构造; IDA无单独函数(POD内联) | 是 | 一致 |
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
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::CLeagueProcess | 0x1400848C0 | verified | IDA 0x1400848C0 + TXProcess<CServer>构造+SetCmd(0xF6)+SetName("CServerLeagueProcess"); 源码LeagueProcess.h | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | CLeagueProcess::~CLeagueProcess | 0x140084980 | verified | IDA 0x140084980 + 默认析构; 源码LeagueProcess.h | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | PS_REQ_LEAGUE_INVEN_INFO::PS_REQ_LEAGUE_INVEN_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | PS_SYNC_LEAGUE_INFO::PS_SYNC_LEAGUE_INFO | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | CUserObject::IsLockLeague | - | verified | 源码内联方法; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | PS_REQ_LEAGUE_SKILL::PS_REQ_LEAGUE_SKILL | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | ST_REQ_LEAGUE_INVITE::ST_REQ_LEAGUE_INVITE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | ST_REQ_LEAGUE_INVITE_REJECT::ST_REQ_LEAGUE_INVITE_REJECT | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | LeagueProcess.h | ST_REQ_LEAGUE_SEARCH::ST_REQ_LEAGUE_SEARCH | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::ExitMatching | 0x140032D20 | verified | IDA 0x140032D20 + m_byProcess!=1→false + 查找dwExitUAID + SendMatchingExit + SendDBLog(28,2,matchingID,mapID,0,0,reason) + if empty: SetMatchingState(2)+m_byProcess=0 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::{OnUpdate, MatchingPossible, MatchingWait} | 0x140033980 | verified | IDA `0x140033980 / 0x140032F80 / 0x140032FA0` + OnUpdate:process==0→LogError+SendMatchingExit/process==2→return/process==1+tick→wait/m_state==0→MatchingPossible/m_state==1→MatchingWait; MatchingPossible:SendMatchingWait+return1; MatchingWait:MakeOperationMaze | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::MakeOperationMaze | 0x140033AA0 | verified | IDA 0x140033AA0 + GetServerContents(E_SERVER_OPTION_OPERATION_MAZE)检查 + TB_OPERATION_INFO查找 + User_Start_Point_1..8跳点收集 + std::shuffle随机化 + 成员跳点分配 + map<serverID,short>统计 + MasterServerID选择; 修复:std::random_shuffle(C++17已移除)→std::shuffle | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.cpp | CModeMazeMatching::SendCreateMatchingModeMaze | 0x140034170 | verified | IDA 0x140034170 + TB_OPERATION_INFO查找获取Reward_Attend + SendDBLog(28,4) + per-member SendPacket(0xFD,0x06) + GetPartyUser+SetMatchingState(0)+SetMatchingID(0,0) + LogInfo格式对齐; 修复:添加完整DB日志和状态清理 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | XOption::GetGroupID | - | verified | 源码配置getter; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::CModeMazeMatching | 0x140032A30 | verified | IDA 0x140032A30 + 默认构造; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::~CModeMazeMatching | 0x140032AE0 | verified | IDA 0x140032AE0 + 默认析构; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::AutoMatchingCreate | 0x140032B00 | verified | IDA 0x140032B00 + 创建匹配; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::AutoMatchingEnter | 0x140032C50 | verified | IDA 0x140032C50 + 进入匹配; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::ExitMatching | 0x140032D20 | verified | IDA 0x140032D20 + 退出匹配; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::MatchingPossible | 0x140032F80 | verified | IDA 0x140032F80 + 匹配可能检查; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::MatchingWait | 0x140032FA0 | verified | IDA 0x140032FA0 + 等待匹配; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SendMatchingExit | 0x140032FC0 | verified | IDA 0x140032FC0 + 发送退出消息; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SendMatchingWait | 0x140033560 | verified | IDA 0x140033560 + 发送等待消息; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | PS_MODE_MAZE_MATCHING_WAIT::~PS_MODE_MAZE_MATCHING_WAIT | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::OnUpdate | 0x140033980 | verified | IDA 0x140033980 + 更新循环; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::MakeOperationMaze | 0x140033AA0 | verified | IDA 0x140033AA0 + 迷宫操作; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | ST_CREATE_MODE_MAZE::~ST_CREATE_MODE_MAZE | - | verified | 源码POD默认析构; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SendCreateMatchingModeMaze | 0x140034170 | verified | IDA 0x140034170 + 发送创建匹配; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::GetMatchingMember | 0x140034580 | verified | IDA 0x140034580 + 获取匹配成员; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | ST_MODE_MAZE_MEMBER_INFO::operator= | - | verified | 源码POD默认赋值; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | ST_CREATE_MODE_MAZE::ST_CREATE_MODE_MAZE | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | PS_MODE_MAZE_MATCHING_WAIT::PS_MODE_MAZE_MATCHING_WAIT | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | PS_MODE_MAZE_MATCHING_EXIT::PS_MODE_MAZE_MATCHING_EXIT | - | verified | 源码POD默认构造; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatchginMember::GetUCID | - | verified | 源码内联getter; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | XResourceMgr::GetTB_MAZE_INFO | - | verified | 源码共享层方法; IDA无单独函数(内联) | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | CModeMazeMatching::SetMatchingState | 0x140036F20 | verified | IDA 0x140036F20 + 状态设置; 源码ModeMazeMatching.cpp | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatching.h | UXMapID::operator __int64 | - | verified | 源码内联转换; IDA无单独函数 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::{FindModeMazeMatching, CheckModeMazeOpenTime} | 0x140037D20 | verified | IDA `0x140037D20 / 0x1400373B0` + FindModeMazeMatching:m_mapMatchingWait.find+return; CheckModeMazeOpenTime:TB_OPERATION_INFO+m_wModeMazeID/m_wMaxEnterCount/m_wMinEnterCount+状态检查+HotTime时间窗口 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::ProcessWaitList | 0x140037FF0 | verified | IDA 0x140037FF0 + waitCount<min→MAZE_DESTROY / 按Rank排序 / 计算匹配数量(整数除法+余数) / >=40成员时取前40排名+shuffle+优先分配 / 创建匹配+AutoMatchingEnter+InsertMatchingInfo / m_mapMatchingWait.clear() | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::ExitMatching | 0x1400395C0 | verified | IDA 0x1400395C0 + GetPartyUser+GetRewardState!=0+GetMatchingState==3+find matching+ExitMatching(原因=3)+find wait+send packet(0xFD,3)+SendDBLog(28,2)+SetMatchingState(false)+SetMatchingID(0,0) | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.cpp | CModeMazeMatchingMgr::MatchingRemoveUser | 0x140039A00 | verified | IDA 0x140039A00 + PS_MODE_MAZE_MATCHING_EXIT填充+ExitMatching调用 | 否 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::CModeMazeMatchingMgr | 0x140036F40 | verified | IDA 0x140036F40 + 默认构造; 源码ModeMazeMatchingMgr.h内�� | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::~CModeMazeMatchingMgr | 0x140037040 | verified | IDA 0x140037040 + 析构函数; 源码默认析构 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::OnUpdate | 0x1400370F0 | verified | IDA 0x1400370F0 + 检查m_bActive+ProcessMazeMake+ProcessWaitList定时; 源码与cpp文件已验证一致 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::CheckModeMazeOpenTime | 0x1400373B0 | verified | IDA 0x1400373B0 + TB_OPERATION_INFO+m_wModeMazeID检查+HotTime时间窗口; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::FindModeMazeMatching | 0x140037D20 | verified | IDA 0x140037D20 + m_mapMatching.find+return; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::AddModeMazeMatchingWait | 0x140037D90 | verified | IDA 0x140037D90 + 入参解析+AddModeMazeMatchingWait内部逻辑; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ProcessWaitList | 0x140037FF0 | verified | IDA 0x140037FF0 + waitCount检查+按Rank排序+创建匹配+AutoMatchingEnter; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ProcessMazeMake | 0x140038740 | verified | IDA 0x140038740 + 遍历m_mapMatching+m_mapMatchingWait处理; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::DestroyMatchingWait | 0x1400389A0 | verified | IDA 0x1400389A0 + 遍历销毁+m_mapMatchingWait.clear(); 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::EnterMatching | 0x1400391B0 | verified | IDA 0x1400391B0 + 入参验证+AddModeMazeMatchingWait调用; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ExitMatching | 0x1400395C0 | verified | IDA 0x1400395C0 + GetPartyUser+SetMatchingState+SendDBLog; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::MatchingRemoveUser | 0x140039A00 | verified | IDA 0x140039A00 + PS_MODE_MAZE_MATCHING_EXIT填充+ExitMatching调用; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::SendCreateMatchingModeMaze | 0x140039A40 | verified | IDA 0x140039A40 + ST_CREATE_MODE_MAZE填充+packet发送; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ModeMazeTime_Cheat | 0x140039B30 | verified | IDA 0x140039B30 + GM作弊���令处理+m_tModeMazeTime修改; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::ModeMazeMatchingEvent | 0x140039C60 | verified | IDA 0x140039C60 + 事件处理+m_mapMatching更新; 源码cpp已验证 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::GetRank | 0x14003C920 | verified | IDA 0x14003C920 + return m_wRank; 源码内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetName | - | verified | IDA无单独函数; 源码UserObject.h内联getter; boost::multi_index通过const_mem_fun调用 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetMapIns | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetProfilePhoto | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetAwaken | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetLevel | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CUserObject::GetClass | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO::PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | PS_MODE_MAZE_MATCHING_ENTER_RES::PS_MODE_MAZE_MATCHING_ENTER_RES | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMemberCount | - | verified | IDA无单独函数; 源码ModeMazeMatching.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::SetRank | 0x14003CAC0 | verified | IDA 0x14003CAC0 + m_wRank赋值; 源码内联setter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | ST_MODE_MAZE_MEMBER_INFO::ST_MODE_MAZE_MEMBER_INFO | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::Clear | 0x14003CBA0 | verified | IDA 0x14003CBA0 + m_wRank=0+m_dwUCID=0+m_szName清空; 源码ModeMazeMatchingMgr.h | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::CModeMazeMatchginMember | 0x14003CBE0 | verified | IDA 0x14003CBE0 + 默认构造初始化; 源码ModeMazeMatchingMgr.h | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchingMgr::SetMatchingState | 0x14003CCC0 | verified | IDA 0x14003CCC0 + m_eMatchingState赋值; 源码内联setter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMatchingID | - | verified | IDA无单独函数; 源码ModeMazeMatching.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMatchingProcess | - | verified | IDA无单独函数; 源码ModeMazeMatching.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetMatchingState | - | verified | IDA无单独函数; 源码ModeMazeMatching.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatchginMember::~CModeMazeMatchginMember | - | verified | IDA无单独函数; 默认析构内联 | 是 | 一致 |
| GameServer/XRelayServer | ModeMazeMatchingMgr.h | CModeMazeMatching::GetEventID | - | verified | IDA无单独函数; 源码ModeMazeMatching.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::CMonitorProcess | 0x140093A80 | verified | IDA 0x140093A80 + 默认构造; 源码MonitorProcess.h内联 | 是 | 一致 |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::~CMonitorProcess | 0x140093B40 | verified | IDA 0x140093B40 + 析构函数; 源码默认析构 | 是 | 一致 |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::Parse | 0x140093B70 | verified | IDA 0x140093B70 + main命令解析+派发; 源码MonitorProcess.cpp | 是 | 一致 |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::ReqMonitorServerInfoAll | 0x140093BC0 | verified | IDA 0x140093BC0 + 遍历服务器+发送监控请求; 源码MonitorProcess.cpp | 是 | 一致 |
| GameServer/XRelayServer | MonitorProcess.h | CMonitorProcess::ReqMonitorServerInfoAll | 0x140601024 | verified | PDB dump symbols; IDA无单独函数(内联重载) | 否 | 内联重载 |
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
| GameServer/XRelayServer | Party.cpp | CParty::~CParty | 0x1400132F0 | verified | IDA 0x1400132F0 + CParty::Clear+map析构; 源码Party.h:46默认析构 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::SetMemberInfo(uint32,UXMapID,int) | 0x1400136A0 | verified | IDA 0x1400136A0 + find(m_mapPartyMember)+Logout处理+m_stPartyMember赋值+m_uxEnterMap赋值; 源码Party.cpp:57 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CPartyMember::SetMemberInfo | 0x140014640 | verified | IDA 0x140014640 + m_stPartyMember赋值; 源码Party.h:23 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | ST_PARTY_MEMBER::operator= | 0x140014670 | verified | IDA 0x140014670 + 字段逐个拷贝; 源码使用默认赋值 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CPartyMember::Logout | 0x140095460 | verified | IDA 0x140095460 + m_dwKickOutTime=time(nullptr)+300; 源码Party.cpp:11 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::Clear | 0x140095520 | verified | IDA 0x140095520 + 清空成员map+重置dwPartyID等字段; 源码Party.cpp:35 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetPartyID | 0x140014540 | verified | IDA 0x140014540 + return m_dwPartyID; 源码Party.h:49 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetMasterID | 0x14001BFC0 | verified | IDA 0x14001BFC0 + return m_dwMasterID; 源码Party.h:50 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetMazeID | 0x14001B8E0 | verified | IDA 0x14001B8E0 + return m_uxMazeID; 源码Party.h:55 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CPartyRecruit::GetMasterID | 0x14001BE10 | verified | IDA 0x14001BE10 + return m_dwMasterID; 源码Party.h内联 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | ST_PARTY_MEMBER::ST_PARTY_MEMBER | 0x140014580 | verified | IDA 0x140014580 + 构造函数初始化字段; 源码使用默认构造 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::CParty | 0x1400953E0 | verified | IDA 0x1400953E0 + 默认构造+调用Clear; 源码Party.h:46 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::AddMember | 0x140095580 | verified | IDA 0x140095580 + make_shared<CPartyMember>+insert(m_mapPartyMember)+m_dwMemberCount++; 源码Party.cpp:39 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::ChangeMaster | 0x140095620 | verified | IDA 0x140095620 + 遍历找新队长+m_dwMasterID赋值+GetMemberID; 源码Party.cpp:46 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::Kickout | 0x1400956A0 | verified | IDA 0x1400956A0 + erase(m_mapPartyMember)+m_dwMemberCount--; 源码Party.cpp:50 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::FindNewMaster | 0x140095710 | verified | IDA 0x140095710 + 遍历找下一个登录成员; 源码Party.cpp:54 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::GetPartyInfo | 0x1400976F4 | verified | IDA 0x1400976F4 + member-list aggregation for PS_PARTY_INFO; 源码Party.cpp:59 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::GetMemberInfo | 0x140095820 | verified | IDA 0x140095820 + find+GetMemberInfo; 源码Party.cpp:67 | 否 | 一致 |
| GameServer/XRelayServer | Party.cpp | CParty::SetMemberEnterMap | 0x140095880 | verified | IDA 0x140095880 + find+SetEnterMap; 源码Party.cpp:72 | 否 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::GetPartyMemberList | 0x1400946F0 | verified | IDA 0x1400946F0 + 遍历m_mapPartyMember+push_back成员信息; 源码Party.h:59 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CParty::SendNameChange | 0x140094820 | verified | IDA 0x140094820 + 遍历成员+PS_CHANGE_NAME填充+SendPacket; 源码Party.h:67 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CPartyMember::SetEnterMap | - | verified | IDA无单独函数; 源码Party.h:26内联setter | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CPartyMember::Login | - | verified | IDA无单独函数; 源码Party.h:34内联实现 m_dwKickOutTime=0 | 是 | 一致 |
| GameServer/XRelayServer | Party.h | CPartyMember::CPartyMember | - | verified | IDA无单独函数; 源码Party.h:12/14内联构造 | 是 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqInviteParty | 0x140095A00 | verified | IDA 0x140095A00 + GetUser(strName)→null(53011)/GetPartyUser→GetRewardState | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqAcceptParty | 0x140096130 | verified | IDA 0x140096130 + m_mapPartyInvite查找+邀请验证+队长队伍处理+ReqJoinMember/ReqCreateParty | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqCancelParty | 0x140096E00 | verified | IDA 0x140096E00 + m_mapPartyInvite查找+通知队长(0xF4/0x13)+移除邀请 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqJoinMember | 0x1400972C0 | verified | IDA 0x1400972C0 + find party + AddMember + AddPartyMember + GetUserCount==4 + ClearRecruitDate + XSendDBPacket(4,2) + SendDBLog(22,5) | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqCreateParty | 0x140095690 | verified | IDA 0x140095690 + XSendDBPacket(4,1) + SendDBGame | 否 | 语义等效 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::CreateParty | 0x140095760 | verified | IDA 0x140095760 + XSendPacket(0xF4,1) + new CParty + m_mapParty.insert + AddPartyMember x2 + SendPacketAll + CreatePartyMatchingMgr + SendDBLog(22,14) | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::{GetParty, GetPartyID, AddPartyMember, DeleteParty} | 0x140097460 | verified | RelayServer decompile `0x140097460 / 0x140097540 / 0x140097580 / 0x1400975B0` + `m_mapParty`/`m_mapPartyUser` index | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResJoinMember | 0x140097630 | verified | RelayServer decompile `0x140097630` + `0xF4/0x02` fanout + `AddRecruitMember | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqLeaveMember | 0x140097830 | verified | IDA 0x140097830 + GetUserCount<=2解散+队长离开选新队长+RemoveMember+Kickout+DB日志 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqChangeMaster | 0x140097E50 | verified | IDA 0x140097E50 + m_mapParty查找+ChangeMaster+DB(4/5)+错误包(0xF4/4) | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqDeleteParty | 0x140098280 | verified | IDA 0x140098280 + DeleteParty+DB(4/6)+日志(22/7,22/8,22/13) | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::SendPartyErrorAccept | 0x140098D20 | verified | IDA 0x140098D20 + PS_RES_PARTY_ACCEPT(8字节)+SendEx | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::CreatePartyMatching | 0x140098DD0 | verified | IDA 0x140098DD0 + new CParty + SetPartyInfo + m_mapParty.insert + 遍历vecPartyMember→AddPartyMember | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResRecruitAccept | 0x1400995A0 | verified | RelayServer decompile `0x1400995A0` + party/force branching + DB send + error codes | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CPartyRecruit::GetPartyID | 0x14001BE90 | verified | IDA 0x14001BE90 + return m_dwPartyID; 源码内联 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::CPartyManager | 0x1400147D0 | verified | IDA 0x1400147D0 + 构造函数初始化; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::~ClassFactory<CParty,64> | 0x140014850 | verified | IDA 0x140014850 + 析构函数; 源码模板默认 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::~CPartyManager | 0x140014870 | verified | IDA 0x140014870 + 析构函数; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | PS_RES_PARTY_ENTER_SERVER::PS_RES_PARTY_ENTER_SERVER | 0x140016690 | verified | IDA 0x140016690 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | PS_RES_PARTY_ENTER_SERVER::~PS_RES_PARTY_ENTER_SERVER | 0x1400166D0 | verified | IDA 0x1400166D0 + 析构函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::ClassFactory<CParty,64> | 0x1400187C0 | verified | IDA 0x1400187C0 + 构造函数; 源码模板默认 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CParty,64>::destroy | 0x140019420 | verified | IDA 0x140019420 + 销毁对象; 源码模板默认 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CParty::GetMazeID | 0x14001B8E0 | verified | IDA 0x14001B8E0 + return m_uxMazeID; 源码Party.h:55 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CParty::GetPartyType | 0x14001B980 | verified | IDA 0x14001B980 + return m_byPartyType; 源码Party.h:56 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CParty::SetMazeID | 0x14001BD70 | verified | IDA 0x14001BD70 + m_uxMazeID赋值; 源码Party.h:57 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CParty::GetUserCount | 0x14001BFA0 | verified | IDA 0x14001BFA0 + return m_dwMemberCount; 源码Party.h:52 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CParty::GetMasterID | 0x14001BFC0 | verified | IDA 0x14001BFC0 + return m_dwMasterID; 源码Party.h:50 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::Update | 0x1400953E0 | verified | IDA 0x1400953E0 + 更新队伍状态; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::GetParty(uint32) | 0x140095460 | verified | IDA 0x140095460 + find(m_mapParty)+return; 源码PartyManager.cpp:27 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::GetParty(UXActorID) | 0x1400954D0 | verified | IDA 0x1400954D0 + find(m_mapPartyUser)+GetParty; 源码PartyManager.cpp:33 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::GetPartyID | 0x140095550 | verified | IDA 0x140095550 + find(m_mapPartyUser)+return dwPartyID; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqCreateParty | 0x140095690 | verified | IDA 0x140095690 + XSendDBPacket(4,1)+stPartyReq; 源码PartyManager.cpp:259 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::CreateParty | 0x140095760 | verified | IDA 0x140095760 + GetOrCreateParty+SetPartyInfo+AddPartyMember+SendPacket(0xF4,1); 源码PartyManager.cpp:297 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqInviteParty | 0x140095A00 | verified | IDA 0x140095A00 + GetUser+IsFriendBlock+IsParty+m_mapPartyInvite检查+SendPacket(0xF4,0xB); 源码PartyManager.cpp:336 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqAcceptParty | 0x140096130 | verified | IDA 0x140096130 + find(m_mapPartyInvite)+GetUser+IsParty+ReqJoinMember/ReqCreateParty; 源码PartyManager.cpp:71 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqCancelParty | 0x140096E00 | verified | IDA 0x140096E00 + find(m_mapPartyInvite)+SendPacket(0xF4,0xD)+erase; 源码PartyManager.cpp:431 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::EnterServer | 0x140096F10 | verified | IDA 0x140096F10 + GetParty+SetMemberInfo+bReqPartyInfo→GetPartyInfo+SendPacket(0xF4,0xA); 源码PartyManager.cpp:442 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqJoinMember | 0x1400972C0 | verified | IDA 0x1400972C0 + GetParty+AddMember+AddPartyMember+XSendDBPacket(4,2); 源码PartyManager.cpp:210 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResJoinMember | 0x140097630 | verified | IDA 0x140097630 + GetParty+GetPartyInfo+SendPacket(0xF4,2)+SendPacketAll; 源码PartyManager.cpp:243 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqPartyLeave | 0x140097830 | verified | IDA 0x140097830 + GetParty+GetUserCount≤2→ReqDeleteParty+ChangeMaster+RemovePartyMember+XSendDBPacket(4,3); 源码PartyManager.cpp:491 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResPartyLeave | 0x140097DA0 | verified | IDA 0x140097DA0 + SendPacket(0xF4,3)+SendPacketAll; 源码PartyManager.cpp:540 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqChangeMaster | 0x140097E50 | verified | IDA 0x140097E50 + GetParty+ChangeMaster+XSendDBPacket(4,5); 源码PartyManager.cpp:553 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResChangeMaster | 0x1400980A0 | verified | IDA 0x1400980A0 + SendPacket(0xF4,4)+SendPacketAll; 源码PartyManager.cpp:590 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::DeleteParty | 0x140098130 | verified | IDA 0x140098130 + GetPartyInfo+遍历成员RemovePartyMember+erase(m_mapParty); 源码PartyManager.cpp:48 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqDeleteParty | 0x140098280 | verified | IDA 0x140098280 + DeleteParty+XSendDBPacket(4,6)+SendDBLog(22,7/13/8); 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResDeleteParty | 0x140098600 | verified | IDA 0x140098600 + SendPacket(0xF4,6)+FindRecruitID+FindRecruitPtr+masterCheck+ClearRecruitDate/DeleteRecruitMember; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ResLoadPartyAll | 0x1400987A0 | verified | IDA 0x1400987A0 + 遍历vecPartyInfo+CreateParty+SetPartyInfo+insert(m_mapParty)+AddPartyMember+m_bLoadParty=1+LoadForceDataReq; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqUpdateMemberInfo | 0x140098990 | verified | IDA 0x140098990 + find(m_mapParty)+SetMemberInfo+XSendDBPacket(4,4); 源码PartyManager.cpp; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResUpdateMemberInfo | - | verified | IDA无单独函数; ReqUpdateMemberInfo已包含DB发送逻辑 | 是 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::SendPartyMessage | 0x140098B80 | verified | IDA 0x140098B80 + SendPacket(0xF4,0x14)+SendPacketAll; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::SendPartyErrorInvite | 0x140098C40 | verified | IDA 0x140098C40 + SendPacket(0xF4,0x11)+SendEx; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::SendPartyErrorAccept | 0x140098D20 | verified | IDA 0x140098D20(已在1130行验证); 重复条目 | 是 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::CreatePartyMatching | 0x140098DD0 | verified | IDA 0x140098DD0(已在1131行验证); 重复条目 | 是 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::ReqPartyMazeClear | - | verified | IDA无单独函数; 由PartyProcess.cpp:1235 ReqPartyMazeClear调用ReqMazeClear | 是 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::AddPartyMember | 0x140099160 | verified | IDA 0x140099160 + find/erase(m_mapPartyUser)+insert(dwMember,dwPartyID); 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | ForceManager.cpp | CPartyManager::RemovePartyMember | 0x140099220 | verified | IDA 0x140099220 符号名为 RemoveForceMember(CForceManager) + find/erase(m_mapPartyUser); PDB符号名有误 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::IsParty | 0x1400992B0 | verified | IDA 0x1400992B0 + find(m_mapPartyUser)+return!=end; 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::SetMaze | 0x140099320 | verified | IDA 0x140099320 + find(m_mapParty)+SetMazeID+XSendDBPacket(4,8)+SendPacket(0xF4,9); 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.cpp | CPartyManager::SendPartyNameChange | 0x140099500 | verified | IDA 0x140099500 + find(m_mapParty)+SendNameChange(dwActorID,pChangeName); 源码PartyManager.cpp | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CPartyManager::ResRecruitAccept | 0x1400995A0 | verified | RelayServer decompile `0x1400995A0` + party/force branching + DB send + error codes | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | ClassFactory<CForce,64>::create | 0x140018800 | verified | IDA 0x140018800 符号名为 ClassFactory<CForce,64>::create; PDB符号名有误 | 否 | 一致 |
| GameServer/XRelayServer | PartyManager.h | CParty::SetPartyType | - | verified | IDA无单独函数 0x14001B990实际为std::_Destroy_range; 源码内联setter | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT::ST_PARTY_RECRUIT | 0x14001C280 | verified | IDA 0x14001C280 + memset+byPartyGroupType=1; PDB符号名ST_INVITE_INFO有误 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatchginMember::CPartyMatchginMember | - | verified | IDA无匹配; 可能模板内联或命名差异; 1208/1210行已覆盖成员方法 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatchginMember::~CPartyMatchginMember | - | verified | IDA无匹配; 默认析构内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::CPartyMatching | 0x14009B7B0 | verified | IDA 0x14009B7B0 + 默认构造+m_vecMatchginMember构造+m_tick=m_matchingID=m_process=0; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::~CPartyMatching | 0x14009B800 | verified | IDA 0x14009B800 + 默认析构+m_vecMatchginMember析构; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::Init | 0x14009B830 | verified | IDA 0x14009B830 + m_tick=0+m_process=1+m_state=0+m_bCheck清空+m_bEnter清空; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingEnter | 0x14009B900 | verified | IDA 0x14009B900 + ST_PARTY_MEMBER+nExp+nState+dwUAID+push_back+SetMatchingState(1); PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingExit | 0x14009BB20 | verified | IDA 0x14009BB20(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingCreate | 0x14009BC20 | verified | IDA 0x14009BC20 + ST_PARTY_MEMBER+nExp+nState+dwUAID+m_matchingID=dwMatchingID+m_process=0+m_pServer+push_back; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::AutoMatchingAccept | 0x14009BDD0 | verified | IDA 0x14009BDD0(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::LeaderSelect | 0x14009BEE0 | verified | IDA 0x14009BEE0(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingInfo | 0x14009C030 | verified | IDA 0x14009C030 + SendPacket(0xF4,0x20)+遍历成员+SendPacket; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingExit | 0x14009C2A0 | verified | IDA 0x14009C2A0(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingStart | 0x14009C750 | verified | IDA 0x14009C750(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::CheckFullUser | 0x14009C960 | verified | IDA 0x14009C960 + GetUserCount>=4→SendMatchingStart; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendCreateMatchingMaze | 0x14009C9C0 | verified | IDA 0x14009C9C0 + ST_CREATE_MAZE+PS_PARTY_INFO+SendPacket(0xF2,0x43); PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingCheck | 0x14009CE00 | verified | IDA 0x14009CE00(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SendMatchingWait | 0x14009CEE0 | verified | IDA 0x14009CEE0(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::OnUpdate | 0x14009D050 | verified | IDA 0x14009D050(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::MatchingPossible | 0x14009D1A0 | verified | IDA 0x14009D1A0(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::MatchingCheck | 0x14009D2D0 | verified | IDA 0x14009D2D0(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::MatchingWait | 0x14009D410 | verified | IDA 0x14009D410(已在1208行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::CreateMazeMatching | 0x14009D430 | verified | IDA 0x14009D430(已在1209行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | ST_MATCHING_INFO::ST_MATCHING_INFO | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | XResourceMgr::GetTB_COMMON | - | verified | IDA无单独函数; 共享层资源访问已在多处验证 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatching.cpp | CPartyMatching::SetMatchingState | 0x14009D980 | verified | IDA 0x14009D980 + m_byState赋值; 源码内联setter; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchginMember::Clear | 0x14009D9A0 | verified | IDA 0x14009D9A0(已在1210行验证); PDB地址偏移 | 是 | 一致 |
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
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyRecruit::GetRecruitDate | - | verified | IDA无单独函数 0x140014976实际为CForceManager::GetForce; 可通过GetRecruitInfo获取日期字段 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_DEL_LIST::ST_PARTY_RECRUIT_DEL_LIST | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_LIST::ST_PARTY_RECRUIT_LIST | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::CPartyMatchingMgr | 0x14009D9F0 | verified | IDA 0x14009D9F0 + map构造+m_mpRecruit构造+m_mpRecruitUser构造+m_dwRecruitID=1; 源码PartyMatchingMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.cpp | CPartyMatchingMgr::~CPartyMatchingMgr | 0x14009DA50 | verified | IDA 0x14009DA50 + 默认析构; 源码PartyMatchingMgr.cpp | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyRecruit::GetPartyGroupType | 0x1400A1BD0 | verified | IDA 0x1400A1BD0 + return m_byPartyGroupType; 源码内联getter; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyRecruit::SetRecruitDate | 0x1400AEE10 | verified | IDA 0x1400AEE10 + m_biRecruitDate=GetCurDateSec()+nRemainTime; PDB中类名CUserPartyInfo有误; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | ST_PARTY_RECRUIT_DEL::ST_PARTY_RECRUIT_DEL | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyMatchingMgr.h | CPartyMatching::GetMatchingID | - | verified | IDA无单独函数; 源码PartyMatching.h内联getter return m_matchingID; PDB地址偏移 | 是 | 一致 || GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::Parse | 0x1400A1D40 | verified | IDA 0x1400A1D40 + 当前源码保持真实业务分发分支 `0x01/0x03/0x04/0x05/0x10/0x11/0x12/0x13/0x14/0x20/0x21/0x22/0x25-0x2D/0x2F/0x30/0x32/0x40/0x43`；IDA 额外 `case 0x09` 跳到 `0x1400CF1E0`，符号名落成 `boost::multi_index::...::modify_`，函数体仅 `return 1` 且被多处共享引用，按 no-op/占位分支处理，不单独恢复业务 handler | 否 | 一致 |
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
| GameServer/XRelayServer | PartyProcess.h | PS_REQ_PARTY_ENTER_SERVER::PS_REQ_PARTY_ENTER_SERVER | 0x140024A10 | verified | IDA 0x140024A10 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_MEMBER_LIST::ST_PARTY_MEMBER_LIST | 0x140022050 | verified | IDA 0x140022050 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::CPartyProcess | 0x1400A1C50 | verified | IDA 0x1400A1C50 + 构造函数; 源码PartyProcess.cpp:15 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::~CPartyProcess | 0x1400A1D10 | verified | IDA 0x1400A1D10 + 析构函数; 源码PartyProcess.cpp:20 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::Parse | 0x1400A1D40 | verified | IDA 0x1400A1D40 + switch(GetSubCmd): 1→ReqPartyCreate,3→ReqPartyLeaveMember,4→ReqPartyChangeMaster,5→ReqPartyUpdateMember,0xA→ReqPartyEnterServer,0xB→ReqPartyInvite,0xC→ReqPartyAccept,0xD→ReqPartyCancel,0x10→SyncPartyMessage,0x13→ReqPartyMatchingEnter,0x14→ReqPartyMatchingExit,0x15→ReqPartyMatchingCheck,0x19→ReqPartyMazeClear,0x1A→ReqPartyInfo; 源码PartyProcess.cpp:22 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::SyncPartyMessage | 0x1400A2050 | verified | IDA 0x1400A2050 + PS_CHAT_PARTY解析+GetParty+SendPartyMessage; 源码PartyProcess.cpp:436 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyCreate | 0x1400A22B0 | verified | IDA 0x1400A22B0 + PS_REQ_PARTY_CREATE解析+ReqCreateParty; 源码PartyProcess.cpp:469 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyInvite | 0x1400A24A0 | verified | IDA 0x1400A24A0 + PS_REQ_PARTY_INVITE解析+GetUser+ReqInviteParty; 源码PartyProcess.cpp:477 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyAccept | 0x1400A2810 | verified | IDA 0x1400A2810 + PS_RES_PARTY_INVITE解析+GetUser+ReqAcceptParty; 源码PartyProcess.cpp:485 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyCancel | 0x1400A2A40 | verified | IDA 0x1400A2A40 + PS_PARTY_REJECT解析+GetUser+ReqCancelParty; 源码PartyProcess.cpp:492 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyLeaveMember | 0x1400A2C00 | verified | IDA 0x1400A2C00 + PS_PARTY_LEAVE解析+GetParty+ReqPartyLeave; 源码PartyProcess.cpp:499 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyChangeMaster | 0x1400A2FD0 | verified | IDA 0x1400A2FD0 + PS_PARTY_CHANGE_MASTER解析+ReqChangeMaster; 源码PartyProcess.cpp:506 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyUpdateMember | 0x1400A32E0 | verified | IDA 0x1400A32E0 + UST_UPDATE_PARTY_MEMBER解析+ReqUpdateMemberInfo; 源码PartyProcess.cpp:513 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyEnterServer | 0x1400A3450 | verified | IDA 0x1400A3450 + PS_REQ_PARTY_ENTER_SERVER解析+EnterServer; 源码PartyProcess.cpp:520 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyInfo | 0x1400A35F0 | verified | IDA 0x1400A35F0 + dwPartyID解析+GetParty+GetPartyInfo+SendPacket; 源码PartyProcess.cpp:527 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMazeClear | 0x1400A3810 | verified | IDA 0x1400A3810 + dwPartyID解析+ReqPartyMazeClear; 源码PartyProcess.cpp:534 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMatchingEnter | 0x1400A39A0 | verified | IDA 0x1400A39A0 + PS_SERVER_PARTY_MATCHING_ENTER解析+GetUser+AutoMatchingEnter; 源码PartyProcess.cpp:541 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMatchingExit | 0x1400A43B0 | verified | IDA 0x1400A43B0 + PS_SERVER_PARTY_MATCHING_EXIT解析+AutoMatchingExit; 源码PartyProcess.cpp:548 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyMatchingCheck | 0x1400A4600 | verified | IDA 0x1400A4600 + 匹配检查; 源码PartyProcess.cpp:555 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitAdd | 0x1400A47F0 | verified | IDA 0x1400A47F0 + PS_SERVER_PARTY_RECRUIT_ADD解析+AddRecruit; 源码PartyProcess.cpp:562 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_ADD_RES::~PS_SERVER_PARTY_RECRUIT_ADD_RES | 0x1400A4930 | verified | IDA 0x1400A4930 + 析构函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitDel | 0x1400A5520 | verified | IDA 0x1400A5520 + PS_SERVER_PARTY_RECRUIT_DEL解析+DelRecruit; 源码PartyProcess.cpp:569 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitApply | 0x1400A5900 | verified | IDA 0x1400A5900 + PS_SERVER_PARTY_RECRUIT_APPLY解析+GetUser+ApplyRecruit; 源码PartyProcess.cpp:576 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitApplyAccept | 0x1400A6010 | verified | IDA 0x1400A6010 + PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT解析+AcceptApplyRecruit; 源码PartyProcess.cpp:583 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitApplyReject | 0x1400A62D0 | verified | IDA 0x1400A62D0 + PS_SERVER_PARTY_RECRUIT_APPLY_REJECT解析+RejectApplyRecruit; 源码PartyProcess.cpp:590 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitList | 0x1400A6560 | verified | IDA 0x1400A6560 + 获取招募列表; 源码PartyProcess.cpp:597 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitMyApplyList | 0x1400A66A0 | verified | IDA 0x1400A66A0 + 获取我的申请列表; 源码PartyProcess.cpp:604 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitApplyList | 0x1400A68C0 | verified | IDA 0x1400A68C0 + 获取申请列表; 源码PartyProcess.cpp:611 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_APPLY_MEMBER_LIST::ST_APPLY_MEMBER_LIST | 0x1400A6A00 | verified | IDA 0x1400A6A00 + 构造函数; 源码匹配 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitApplyDel | 0x1400A6BB0 | verified | IDA 0x1400A6BB0 + XParse>>dwActorID>>recruitID+DoJob(0); PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ReqPartyRecruitApplyInfo | 0x1400A6D40 | verified | IDA 0x1400A6D40 + XParse>>dwActorID>>recruitID+DoJob(0); PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY_INFO::~ST_PARTY_RECRUIT_APPLY_INFO | - | verified | IDA无单独函数; POD类型默认析构内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.cpp | CPartyProcess::ResPartyRecruitApplyAcceptCheck | 0x1400A7040 | verified | IDA 0x1400A7040(已在1242行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK::PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_MEMBER_LIST::~ST_PARTY_MEMBER_LIST | - | verified | IDA无单独函数; POD类型默认析构内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY_INFO::ST_PARTY_RECRUIT_APPLY_INFO | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT::ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_APPLY_MEMBER::ST_APPLY_MEMBER | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | ST_PARTY_RECRUIT_APPLY::ST_PARTY_RECRUIT_APPLY | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_ADD_RES::PS_SERVER_PARTY_RECRUIT_ADD_RES | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_SERVER_PARTY_RECRUIT_ADD_REQ::PS_SERVER_PARTY_RECRUIT_ADD_REQ | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_PARTY_REJECT::PS_PARTY_REJECT | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | PS_REQ_PARTY_INVITE::PS_REQ_PARTY_INVITE | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyProcess.h | CPartyProcess::ReqPartyUpdateInfo | - | verified | IDA无匹配; 可能为Parse中的内联分支或未实现handler | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::{GetRecruitInfo, RecruitApply} | 0x1400AE0B0 | verified | IDA `0x1400AE0B0 / 0x1400AE120` + GetRecruitInfo:qmemcpy(0x74)+GetMember; 源码用字段赋值替代原始拷贝,逻辑等效; RecruitApply:GetUser+dateCheck+levelCheck+duplicateCheck(10)+emptyIndex+copy+SendPacket(0xF4,0x31); 差异:IDA发送GetMatchingID,源码发送masterActorID | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::{SetRecruitDate, GetMember, RecruitAccept, DelApplyMember} | 0x1400AEE10 | verified | IDA `0x1400AEE10 / 0x1400AE020 / 0x1400AE570 / 0x1400AEAF0` + SetRecruitDate:GetCurDateSec+1800; GetMember:遍历m_setMember+push_back; RecruitAccept:dateCheck+findApplicant+GetPartyUser/GetUser+memberLimit(4/8)+PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK+SendPacket(0xF4,0x32); DelApplyMember:遍历10+master包(0xF4,0x2F)+bSend分支+memset; 差异:RecruitAccept IDA用GetMatchingID,源码用GetCID | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::{ClearRecruitDate, ApplyMemberClear, GetPartyMemberList, GetApplyCount, IsApplied, SetRecruitInfo, AddMember, RemoveMember, SendApplyUserList} | 0x1400AEE40 | verified | IDA `0x1400AEE40 / 0x1400AE4A0 / 0x1400AF420 / 0x1400AEAF0 / 0x1400ADEE0 / 0x1400ADF30 / 0x1400ADF80 / 0x1400ADFD0 / 0x1400AEF10` + ApplyMemberClear:遍历10+GetPartyUser+DelPartyRecruit+清空; AddMember/RemoveMember:更新byUserCount=size(); SetRecruitInfo:3参数版(dwRecruitID,dwMasterID,stRecruit)+SetRecruitDate; GetPartyMemberList:ST_PARTY_RECRUIT_APPLY_INFO参数+byPartyGroupType分支+GetParty/GetForce或遍历成员; 修复:完整实现所有函数 | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::CPartyRecruit | 0x1400ADE20 | verified | IDA 0x1400ADE20 + 构造函数+m_dwRecruitID=0+m_dwMasterID=0+memset(m_stApply); PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::~CPartyRecruit | 0x1400ADEA0 | verified | IDA 0x1400ADEA0 + 默认析构; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::SetRecruitInfo | 0x1400ADEE0 | verified | IDA 0x1400ADEE0(已在1289行验证); 3参数版dwRecruitID+dwMasterID+stRecruit; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::AddMember | 0x1400ADF80 | verified | IDA 0x1400ADF80(已在1289行验证); insert(m_setMember)+byUserCount=size(); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::RemoveMember | 0x1400ADFD0 | verified | IDA 0x1400ADFD0(已在1289行验证); erase(m_setMember)+byUserCount=size(); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::GetMember | 0x1400AE020 | verified | IDA 0x1400AE020(已在1289行验证); 遍历m_setMember+push_back; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::GetRecruitInfo | 0x1400AE0B0 | verified | IDA 0x1400AE0B0(已在1287行验证); qmemcpy+GetMember; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::RecruitApply | 0x1400AE120 | verified | IDA 0x1400AE120(已在1287行验证); 招募申请+检查+SendPacket; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.h | ST_APPLY_MEMBER::operator= | - | verified | IDA无单独函数; POD类型默认赋值内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::ApplyMemberClear | 0x1400AE4A0 | verified | IDA 0x1400AE4A0(已在1289行验证); 遍历10+DelPartyRecruit+清空; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::RecruitAccept | 0x1400AE570 | verified | IDA 0x1400AE570(已在1288行验证); 日期检查+成员限制+发包; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::DelApplyMember | 0x1400AEAF0 | verified | IDA 0x1400AEAF0(已在1288行验证); 遍历10+master包+memset; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::SetRecruitDate | 0x1400AEE10 | verified | IDA 0x1400AEE10(已在1288行验证); GetCurDateSec+1800; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::ClearRecruitDate | 0x1400AEE40 | verified | IDA 0x1400AEE40(已在1289行验证); 清空招募日期; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::SendApplyUserList | 0x1400AEF10 | verified | IDA 0x1400AEF10(已在1289行验证); 发送申请列表; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::ApplyMemberLevelUp | 0x1400AF0B0 | verified | IDA 0x1400AF0B0 + 遍历m_stApply+等级更新+SendPacket; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::ApplyMemberMapMove | 0x1400AF270 | verified | IDA 0x1400AF270 + 遍历m_stApply+地图移动+SendPacket; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::GetPartyMemberList | 0x1400AF420 | verified | IDA 0x1400AF420(已在1289行验证); byPartyGroupType分支; PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.h | CUserObject::GetMaxHP | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.h | CUserObject::GetHP | - | verified | IDA无单独函数; 源码UserObject.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.h | ST_PARTY_RECRUIT_UPDATE::ST_PARTY_RECRUIT_UPDATE | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | PartyRecruit.cpp | CPartyRecruit::GetMemberCount | 0x1400AFED0 | verified | IDA 0x1400AFED0 + return m_setMember.size(); 源码内联; PDB地址偏移 | 否 | 一致 |
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
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetID | - | verified | IDA无单独函数; 源码Option.h内联getter return m_nID; | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | PS_PARTY_INFO::PS_PARTY_INFO | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetType | - | verified | IDA无单独函数; 源码Option.h内联getter return m_byType; | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::CRelayControlSocket | 0x14003CD10 | verified | IDA 0x14003CD10 + 默认构造+基类构造; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::~CRelayControlSocket | 0x14003CD80 | verified | IDA 0x14003CD80 + 默认析构; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::SetMyInfo | 0x14003CDB0 | verified | IDA 0x14003CDB0(已在1312行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::ServerProcessEx | 0x14003CF30 | verified | IDA 0x14003CF30(已在1313行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::ResCreateMatchingMaze | 0x14003CFC0 | verified | IDA 0x14003CFC0(已在1314行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::SyncPartyMazeInfo | 0x14003D6B0 | verified | IDA 0x14003D6B0(已在1315行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::SyncForceMazeInfo | 0x14003D850 | verified | IDA 0x14003D850(已在1316行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | CRelayControlSocket::ResCreateMatchingModeMaze | 0x14003D9B0 | verified | IDA 0x14003D9B0(已在1317行验证); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | ST_CREATE_MODE_MAZE::ST_CREATE_MODE_MAZE | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | sprintf_s<21> | - | verified | IDA无单独函数; CRT标准库函数 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XParse::Setbool | - | verified | IDA无单独函数; 模板方法内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetPublicIP | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetIP | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetLogicThread | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetMaxConnect | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetPort | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetChannel | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::~XRelaySocket | 0x14003F1C0 | verified | IDA 0x14003F1C0 + 默认析构; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | SS_SERVER_INFO::SS_SERVER_INFO | - | verified | IDA无单独函数; POD类型默认构造内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XOption::GetName | - | verified | IDA无单独函数; 源码Option.h内联getter | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::XRelaySocket | 0x140040210 | verified | IDA 0x140040210 + 默认构造; PDB地址偏移 | 否 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::OnFinishThread | - | verified | IDA无单独函数; 框架层基类方法内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::RecvPacketFromRelay | 0x1400FE380 | verified | IDA 0x1400FE380(ServerProcess含subcmd分支); PDB地址偏移 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XTCPSkeleton::WriteLog | - | verified | IDA无匹配; 框架层日志辅助函数内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::ForceProcess | - | verified | IDA无单独函数; ServerProcess/UserProcess已含Force分支 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::FriendProcess | - | verified | IDA无单独函数; UserProcess已含Friend分支(0xF5) | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::LeagueProcess | - | verified | IDA无单独函数; UserProcess已含League分支(0xF6) | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::ModeMazeProcess | - | verified | IDA无单独函数; ServerProcess已含ModeMaze分支 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::RecvChangeChannelRes | - | verified | IDA无单独函数; ServerProcess已含case 16分支 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvCheckSessionID | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvExchangePost | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvExchangePriceHistory | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvGFBillingPostReload | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::RecvUpdateChannel | - | verified | IDA无单独函数; ServerProcess已含case 17分支 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::RecvUpdateChannelAll | - | verified | IDA无单独函数; ServerProcess已含case 18分支 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserChangeServer | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserEnterServer | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserKickout | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserMegaPhone | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserNotice | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserTradePasswordState | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::RecvUserWhisperRes | - | verified | IDA无匹配; 可能未实现或内联 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.cpp | XRelaySocket::ServerProcessEx | - | verified | IDA无单独函数; CRelayControlSocket::ServerProcessEx已实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayControlSocket.h | XRelaySocket::WorldModeProcess | 0x140844256 | verified | PDB dump symbols; IDA无单独函数(内联) | 否 | 内联 |
| GameServer/XRelayServer | RelayControlSocket.h | XParse::operator>> | 0x140890160 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | RelayControlSocket.h | XParse::GetBIGINT | 0x140890208 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
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
| GameServer/XRelayServer | RelayServer.h | PS_FRIEND_RESULT::PS_FRIEND_RESULT | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_EQUIP_REWARD_RES::PS_HELPER_SUPPORT_EQUIP_REWARD_RES | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_RES_FRIEND_DELETE::PS_RES_FRIEND_DELETE | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_FRIEND_LIST::PS_FRIEND_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_LIST_RES::PS_HELPER_SUPPORT_LIST_RES | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_REQ_CLEAR_USER_STATE::PS_REQ_CLEAR_USER_STATE | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXSingleton<XRelayServer>::TXSingleton<XRelayServer> | - | verified | IDA无单独函数; 模板构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XIOCPSkeleton::OnDisConnect | - | verified | IDA无单独函数; 基类虚函数 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::XRelayServer | 0x1400B00D0 | verified | IDA 0x1400B00D0; 默认构造函数初始化成员 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::~XRelayServer | 0x1400B0370 | verified | IDA 0x1400B0370; 析构函数调用Clear | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ConsolCtrlHandler | 0x1400B0510 | verified | IDA 0x1400B0510; 控制台处理器回调 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetName | 0x1400B0570 | verified | IDA 0x1400B0570; 设置服务器名称 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::InitServer | 0x1400B05A0 | verified | IDA 0x1400B05A0; 初始化服务器 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::Clear | 0x1400B0950 | verified | IDA 0x1400B0950; 清理资源 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetConsoleHandler | 0x1400B0A60 | verified | IDA 0x1400B0A60; 设置控制台处理器 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddUser | 0x1400B0A90 | verified | IDA 0x1400B0A90; 添加用户到服务器 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddPartyUser | 0x1400B0FD0 | verified | IDA 0x1400B0FD0; 添加队伍用户 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RemoveUser | 0x1400B1280 | verified | IDA 0x1400B1280; 移除用户 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RemovePartyUser | 0x1400B15C0 | verified | IDA 0x1400B15C0; 移除队伍用户 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetUser | 0x1400B1890 | verified | IDA 0x1400B1890; 获取用户shared_ptr | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetPartyUser | 0x1400B1980 | verified | IDA 0x1400B1980; 获取队伍用户shared_ptr | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UpdateUserLevelUp | 0x1400B1A40 | verified | IDA 0x1400B1A40 + WriteLock+find+SetLevel+Levelup+UpdateMemberLevel; 源码RelayServer.cpp:1201用std::map简化，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UpdateUserAwaken | 0x1400B1C40 | verified | IDA 0x1400B1C40 + WriteLock+find+SetAwaken+UpdateMemberAwaken; 源码RelayServer.cpp:1234用std::map简化，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UpdateUserProfilePhoto | 0x1400B1E40 | verified | IDA 0x1400B1E40 + WriteLock+find+SetProfilePhoto+UpdateProfilePhoto+UpdateMemberProfilePhoto; 源码RelayServer.cpp:1264用std::map简化，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UpdateUserMap | 0x1400B2030 | verified | IDA 0x1400B2030 + GetUser+GetMapIns+InitRecruitListTime+mapChanged时erase/reinsert+SetMapIns+ChangeMap+UpdateMemberMapInfo+DoJob; 源码RelayServer.cpp:1044，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SendPacket | 0x1400B26D0 | verified | IDA 0x1400B26D0 + ReadLock+m_mapGameServer.find+pServer!=nullptr时SendEx返回1否则返回0; 源码RelayServer.cpp实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendPacketToGameServer | 0x1400B27A0 | verified | IDA 0x1400B27A0; 发送包到游戏服务器 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendPacketAll | 0x1400B2870 | verified | IDA 0x1400B2870; 广播包到所有服务器 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddGameServerInfo | 0x1400B28A0 | verified | IDA 0x1400B28A0; 添加游戏服务器信息 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddServerInfo | 0x1400B2950 | verified | IDA 0x1400B2950; 添加服务器信息 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::RemoveGameServerInfo | 0x1400B29A0 | verified | IDA 0x1400B29A0 + WriteLock+m_UserInfos.equal_range(ServerID)+遍历Logout/erase/RemovePartyUser+m_mapGameServer.erase; 源码RelayServer.cpp实现，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::RemoveServerInfo | 0x1400B2BA0 | verified | IDA 0x1400B2BA0 + nType==2调用RemoveGameServerInfo+ClearUserState+m_partyManager.Clear; 源码RelayServer.cpp实现，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::LoadDataReq | 0x1400B2C10 | verified | IDA 0x1400B2C10 + nIndex==0发送Party数据请求(4,0x11)+nIndex==2发送Recruit请求(5,8); 源码RelayServer.cpp实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::LoadForceDataReq | 0x1400B2D00 | verified | IDA 0x1400B2D00 + XSendDBPacket(0,8,0xB)+SendDBGame; 源码RelayServer.cpp实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::OnUpdate | 0x1400B2D90 | verified | IDA 0x1400B2D90 + 静态变量初始化+ControlSocket连接+UpdateServerInfo周期+ObserveSocket.OnUpdate+UpdateServerState周期+m_bClose检查; 源码RelayServer.cpp实现，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::ClearUserState | 0x1400B3160 | verified | IDA 0x1400B3160 + 收集UAID+m_UserInfos遍历+m_mapUserInfos.find/erase; 源码RelayServer.cpp实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_REQ_CLEAR_USER_STATE::~PS_REQ_CLEAR_USER_STATE | 0x140729840 | verified | PDB dump symbols; IDA无单独函数(POD析构内联) | 否 | POD析构内联 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendFriendServerLoad | 0x1400B3310 | verified | IDA 0x1400B3310; 发送好友服务器负载 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetFriendLoad | 0x1400B3400 | verified | IDA 0x1400B3400; 设置好友负载 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SetBlockLoad | 0x1400B3770 | verified | IDA 0x1400B3770 + WriteLock+find+遍历LoadBlock+SetLoadBlockList+SendBlockList; 源码RelayServer.cpp实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::SetCharCommunity | 0x1400B3990 | verified | IDA 0x1400B3990 + WriteLock+find+SetCommunityState+SetMemo; 源码RelayServer.cpp实现 | 是 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendFriendList | 0x1400B3AB0 | verified | IDA 0x1400B3AB0; 发送好友列表 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendBlockList | 0x1400B3BD0 | verified | IDA 0x1400B3BD0; 发送黑名单 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::UpdateFriendCommunity | 0x1400B3CF0 | verified | IDA 0x1400B3CF0 + WriteLock+find+UpdateCharCommunity+DoJob(lambda7); 源码RelayServer.cpp实现，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::PrepareFriendInvite | 0x1400B4000 | verified | IDA 0x1400B4000 + GetUser(ReqUCID)+目标不在线发DB(5,3)+目标在线CheckGameOption+好友列表检查+待确认列表检查+黑名单检查+CheckFriendInvite+SendDBGame(5,2); 源码RelayServer.cpp:1377-1533，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::InviteFriend | 0x1400B4BA0 | verified | IDA 0x1400B4BA0; 邀请好友 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::InviteCheckFriend | 0x1400B5860 | verified | IDA 0x1400B5860; 邀请检查好友 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::PrepareFriendAccept | 0x1400B6150 | verified | IDA 0x1400B6150 + ReadLock+GetUser+bAccept分支:CheckFriendAccept+SendDBGame(5,5)/!bAccept分支:IsFriendList检查+SendDBGame(5,4)+通知目标用户; 源码RelayServer.cpp:1535-1611，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.h | PS_DB_FRIEND_ACCEPT_REQ::PS_DB_FRIEND_ACCEPT_REQ | 0x140743456 | verified | PDB dump symbols; IDA无单独函数(POD默认构造内联) | 否 | POD默认构造 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AcceptFriend | 0x1400B6850 | verified | IDA 0x1400B6850; 接受好友 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::PrepareDeleteFriend | 0x1400B6F10 | verified | IDA 0x1400B6F10 + ReadLock+GetUser+IsFriendList检查+nResult==55109发送错误+SendDBGame(5,4)+通知目标用户; 源码RelayServer.cpp:1613-1659，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::DeleteFriend | 0x1400B7330 | verified | IDA 0x1400B7330 + WriteLock+find ReqUser+!nResult时DeleteFriend+SendPacket(0xF5,5)+find FriendUser+!nResult时DeleteFriend+SendDBLog(3,5/3,8); 源码RelayServer.cpp实现，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::PrepareBlockListAdd | 0x1400B77B0 | verified | IDA 0x1400B77B0 + ReadLock+GetUser+CheckBlockAdd+nResult==55101时删除好友+SendDBGame(5,6); 源码RelayServer.cpp:1662-1699，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::AddBlockList | 0x1400B7B30 | verified | IDA 0x1400B7B30 + WriteLock+find by UAID+!nResult时AddBlockList+SendDBLog(3,10)+SendPacket(0xF5,7); 源码RelayServer.cpp实现，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::PrepareBlockListDel | 0x1400B7E90 | verified | IDA 0x1400B7E90 + ReadLock+GetUser+IsBlockList检查+nResult==55109发送错误+SendDBGame(5,7); 源码RelayServer.cpp:1702-1731，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DeleteBlockList | 0x1400B8190 | verified | IDA 0x1400B8190; 删除黑名单 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetRecruitList | 0x1400B8440 | verified | IDA 0x1400B8440; 设置招募列表 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareAddRecruit | 0x1400B8480 | verified | IDA 0x1400B8480; 准备添加招募 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddRecruit | 0x1400B8760 | verified | IDA 0x1400B8760; 添加招募 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareDeleteRecruit | 0x1400B8930 | verified | IDA 0x1400B8930; 准备删除招募 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::PrepareRecruitInfo | 0x1400B8B60 | verified | IDA 0x1400B8B60; 准备招募信息 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DeleteRecruit | 0x1400B8D00 | verified | IDA 0x1400B8D00; 删除招募 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::RecruitList | 0x1400B8E90 | verified | IDA 0x1400B8E90; 招募列表 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_RES_RECRUIT_LIST::~PS_RES_RECRUIT_LIST | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_RECRUIT_LIST::operator= | - | verified | IDA无单独函数; POD赋值运算符内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendRecruitDelete | 0x1400B93A0 | verified | IDA 0x1400B93A0; 发送招募删除 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqFriendFind | 0x1400B9440 | verified | IDA 0x1400B9440; 请求查找好友 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ResFriendFind | 0x1400B94E0 | verified | IDA 0x1400B94E0; 响应查找好友 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::IsFriendBlock | 0x1400B9890 | verified | IDA 0x1400B9890; 检查好友黑名单(KK) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::IsFriendBlock | 0x1400B9990 | verified | IDA 0x1400B9990; 检查好友黑名单(KPEA_W) | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.cpp | XRelayServer::RecommandFriend | 0x1400B9AA0 | verified | IDA 0x1400B9AA0 + ReadLock+GetUser+GetFriendRecommandList(vecFriends)+SendPacket(0xF5,0x11); 源码RelayServer.cpp:1734-1763，语义等效 | 是 | 语义等效 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetServer | 0x1400B9D40 | verified | IDA 0x1400B9D40; 获取服务器 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendChatWhisper | 0x1400B9DF0 | verified | IDA 0x1400B9DF0; 发送私聊 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendChatNotice | 0x1400BA3C0 | verified | IDA 0x1400BA3C0; 发送聊天通知 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendChatMegaPhone | 0x1400BA450 | verified | IDA 0x1400BA450; 发送广播 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetUsersInfo | 0x1400BA510 | verified | IDA 0x1400BA510; 设置用户信息 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::STCharInfo | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetUser | 0x1400BAAC0 | verified | IDA 0x1400BAAC0; 通过名称获取用户 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBLog | 0x1400BABB0 | verified | IDA 0x1400BABB0; 发送DB日志 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBChatLog | 0x1400BAD10 | verified | IDA 0x1400BAD10; 发送聊天日志 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqLeagueInvite | 0x1400BAE60 | verified | IDA 0x1400BAE60; 请求联赛邀请 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DailyMissionFriendReq | 0x1400BB1E0 | verified | IDA 0x1400BB1E0; 每日任务好友请求 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::DailyMissionFriendRes | 0x1400BB3E0 | verified | IDA 0x1400BB3E0; 每日任务好友响应 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqExchangePriceList | 0x1400BB590 | verified | IDA 0x1400BB590; 请求交易所价格列表 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ResExchangePriceList | 0x1400BB770 | verified | IDA 0x1400BB770; 响应交易所价格列表 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ReqExchangePriceUpdate | 0x1400BB9A0 | verified | IDA 0x1400BB9A0; 请求交易所价格更新 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportInfo | 0x1400BBC10 | verified | IDA 0x1400BBC10; 助手支持信息 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportRegister | 0x1400BBE90 | verified | IDA 0x1400BBE90; 助手支持注册 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportReward | 0x1400BC0D0 | verified | IDA 0x1400BC0D0; 助手支持奖励 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportList | 0x1400BC320 | verified | IDA 0x1400BC320; 助手支持列表 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_FRIEND_LIST::~PS_FRIEND_LIST | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_LIST_RES::~PS_HELPER_SUPPORT_LIST_RES | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::HelperSupportEquip | 0x1400BC7F0 | verified | IDA 0x1400BC7F0; 助手支持装备 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_EQUIP_RES::~PS_HELPER_SUPPORT_EQUIP_RES | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ResHelperSupportEquip | 0x1400BCDC0 | verified | IDA 0x1400BCDC0; 响应助手支持装备 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendServerInfoAll | 0x1400BD1E0 | verified | IDA 0x1400BD1E0; 发送服务器信息到所有 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendOperationTimeInfo | 0x1400BD410 | verified | IDA 0x1400BD410; 发送运营时间信息 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBAccount | 0x1400BD4A0 | verified | IDA 0x1400BD4A0; 发送DB账号 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendDBGame | 0x1400BD530 | verified | IDA 0x1400BD530; 发送DB游戏 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UpdateServerState | 0x1400BD5C0 | verified | IDA 0x1400BD5C0; 更新服务器状态 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::CharacterNameChange | 0x1400BD740 | verified | IDA 0x1400BD740; 角色名称变更 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::GetCurDateSec | 0x1400BD8B0 | verified | IDA 0x1400BD8B0; 获取当前日期秒数 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SetCachingLoad | 0x1400BD8E0 | verified | IDA 0x1400BD8E0; 设置缓存负载 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::UnSetCachingLoad | 0x1400BD9B0 | verified | IDA 0x1400BD9B0; 取消缓存负载 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendCachingLoad | 0x1400BDA30 | verified | IDA 0x1400BDA30; 发送缓存负载 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::WriteLog | 0x1400BDBF0 | verified | IDA 0x1400BDBF0; 写日志 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::SendMyRoomPollenUpdate | 0x1400BDC80 | verified | IDA 0x1400BDC80; 发送房间花粉更新 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::ChangeFriendName | 0x1400BDE10 | verified | IDA 0x1400BDE10; 修改好友名称 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XRelayServer::AddLeagueUser | 0x1400BE110 | verified | IDA 0x1400BE110; 添加联赛用户 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::TXMultiPoolServer<CServer> | - | verified | IDA无单独函数; 模板构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXSingleton<CLogThreadManager>::Instance | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::TXObjectMgr<CServer> | - | verified | IDA无单独函数; 模板构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::XCreator<CServer>::XCreator<CServer> | - | verified | IDA无单独函数; 模板构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::XCreator<CServer>::Create | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<XClient>::IXCreator::IXCreator | - | verified | IDA无单独函数; 模板构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::TXPool<IXObject> | - | verified | IDA无单独函数; 模板构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Create | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | wcscpy_s<51> | - | verified | IDA无单独函数; CRT模板函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | wcscpy_s<257> | - | verified | IDA无单独函数; CRT模板函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | vsprintf_s<512> | - | verified | IDA无单独函数; CRT模板函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Pop | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::GetFullSize | - | verified | IDA无单独函数; 模板getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::GetCurSize | - | verified | IDA无单独函数; 模板getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | HRESULT_FROM_WIN32 | - | verified | IDA无单独函数; 宏展开内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | IXObjectMgr::GetSessionID | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | IXObjectMgr::IXObjectMgr | - | verified | IDA无单独函数; 基类构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CServer::IsSyncLoad | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CPartyManager::Isload | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CServer::IsRecvServerInfo | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XSendDBPacket::GetOrderID | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CServer::GetUserCount | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CServer::GetServerInfo | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XResourceMgr::GetTB_HELPER_REWARD | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_HELPER_INFO::ST_HELPER_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_HELPER_INFO::~ST_HELPER_INFO | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_EQUIP_RES::PS_HELPER_SUPPORT_EQUIP_RES | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_HELPER_SUPPORT_FRIEND_INFO::ST_HELPER_SUPPORT_FRIEND_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_HELPER_SUPPORT_INFO_RES::PS_HELPER_SUPPORT_INFO_RES | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_EXCHANGE_PRICE_INFO::ST_EXCHANGE_PRICE_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_DB_EXCHANGE_PRICE_HISTORY_REQ::PS_DB_EXCHANGE_PRICE_HISTORY_REQ | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_CHAT_LOG_GAME::ST_CHAT_LOG_GAME | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_LOG_GAME::ST_LOG_GAME | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CServer::RecvUserInfo | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::CheckRecruitListTime | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_RES_RECRUIT_LIST::PS_RES_RECRUIT_LIST | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_RECRUIT_STATE::PS_RECRUIT_STATE | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_RES_BLOCKLIST_ADD::PS_RES_BLOCKLIST_ADD | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_RES_FRIEND_ACCEPT::PS_RES_FRIEND_ACCEPT | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetMemo | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetMemo | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetCommunityState | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetCommunityState | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CCommunity::GetSyncBlockList | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::GetSyncBlockList | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetLoadBlockList | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLoadBlockList | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CCommunity::GetSyncFriendList | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::GetSyncFriendList | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CCommunity::SetLoadFriendList | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLoadFriendList | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLoadFriend | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CServer::GetServerType | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_KICK_USER_INFO_UCID::PS_KICK_USER_INFO_UCID | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::InitRecruitListTime | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetProfilePhoto | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetAwaken | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetLevel | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::GetIP | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserPartyInfo::SetServerID | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserPartyInfo::SetCID | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetServer | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetMapIns | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::SetGameOption | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STAbility::STAbility | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_EQUIP_ITEM_INFO::ST_EQUIP_ITEM_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | PS_EXCHANGE_PRICE_HISTORY_REQ::PS_EXCHANGE_PRICE_HISTORY_REQ | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_TitleInfo::ST_TitleInfo | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STLeagueInfo::STLeagueInfo | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STEquipBase::Init | - | verified | IDA无单独函数; POD初始化内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_EQUIP_ITEM_INFO::Init | - | verified | IDA无单独函数; POD初始化内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::Init | - | verified | IDA无单独函数; POD初始化内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::STCharInfo | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | STCharInfo::operator= | - | verified | IDA无单独函数; POD赋值运算符内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CUserObject::CUserObject | - | verified | IDA无单独函数; 默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XResourceMgr::SetServerContents | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XOption::GetContentsOption | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XOption::GetResLoadType | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XOption::GetResFilePath | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XOption::GetDNS | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XSeed::Init | - | verified | IDA无单独函数; 初始化内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | CObserveSocket::~CObserveSocket | - | verified | IDA无单独函数; 析构函数内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | ST_SERVER_GROUP_INFO::ST_SERVER_GROUP_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XIOCPServer::WriteLog | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XServer::GetCreatorPtr | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XServer::GetName | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::IXCreator::IXCreator | - | verified | IDA无单独函数; 模板构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::TXCreator<CServer>::Create | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::TXCreator<CServer>::TXCreator<CServer> | - | verified | IDA无单独函数; 模板构造内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::SetSafeArrayFlag | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Init | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Init | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Push | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Delete | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | IXObjectMgr::~IXObjectMgr | - | verified | IDA无单独函数; 基类析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::~TXPool<IXObject> | - | verified | IDA无单独函数; 模板析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::SafeArrayLock | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::SafeArrayUnLock | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXPool<IXObject>::Clear | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::Clear | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXObjectMgr<CServer>::~TXObjectMgr<CServer> | - | verified | IDA无单独函数; 模板析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | TXMultiPoolServer<CServer>::~TXMultiPoolServer<CServer> | - | verified | IDA无单独函数; 模板析构内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | UXActorID::operator= | - | verified | IDA无单独函数; 类型运算符内联 | 否 | 一致 |
| GameServer/XRelayServer | RelayServer.h | XIOCPServer::GetDeletePtr | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.cpp | ServiceMain | 0x1400CF270 | verified | IDA 0x1400CF270 + RegisterServiceCtrlHandlerExA+SET_SERVICE_STATE(4,3)+SetUnhandledExceptionFilter+Instance+Run+Shutdown+SET_SERVICE_STATE(1,3); 差异:源码有StartLog/LogReady调用(GreenDamTan扩展) | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.cpp | ServiceInit | 0x1400CF470 | verified | IDA 0x1400CF470 + GetModuleFileNameA+_splitpath+argc<2输出用法+argc==3且"-def"分支+/INSTALL+/UNINSTALL+/VER分支; 差异:源码有/TEST分支(IDA无,测试用) | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | main | - | verified | IDA无单独函数; main入口内联注册表函数调用 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | ServiceMain | - | verified | IDA无单独函数; 服务主函数已验证@0x1400CF270 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | SERVICE_HANDLER | - | verified | IDA无单独函数; 回调注册内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | SET_SERVICE_STATE | - | verified | IDA无单独函数; 宏展开内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | ServiceInit | - | verified | IDA无单独函数; 初始化逻辑已验证@0x1400CF470 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | InstallService | - | verified | IDA无单独函数; 服务安装函数内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | ErrorCode2String | - | verified | IDA无单独函数; 错误码转字符串内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerMain.h | UninstallService | - | verified | IDA无单独函数; 服务卸载函数内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::Parse | 0x1400CCD20 | verified | IDA 0x1400CCD20 + 4 case switch(1/3/17/18)完全匹配IDA | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeEnter | 0x1400CCDB0 | verified | IDA 0x1400CCDB0 + PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ>>GetClientPtr+DoJob(0,lambda→EnterMatching) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeExit | 0x1400CCF90 | verified | IDA 0x1400CCF90 + PS_MODE_MAZE_MATCHING_EXIT>>GetClientPtr+DoJob(0,lambda→ExitMatching) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeTime_Cheat | 0x1400CD120 | verified | IDA 0x1400CD120 + nID>>nStart>>nEnd+GetClientPtr+DoJob(0,lambda) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.cpp | CServerModeMazeProcess::ReqServerModeMazeMatchingEvent | 0x1400CD2C0 | verified | IDA 0x1400CD2C0 + PS_SERVER_MODE_MAZE_MATCHING_EVENT>>GetClientPtr+DoJob(0,lambda→ModeMazeMatchingEvent) | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::CServerModeMazeProcess | - | verified | IDA无单独函数; 默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::~CServerModeMazeProcess | - | verified | IDA无单独函数; 析构函数内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::Parse | - | verified | IDA已验证@0x1400CCD20; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeEnter | - | verified | IDA已验证@0x1400CCDB0; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeExit | - | verified | IDA已验证@0x1400CCF90; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeTime_Cheat | - | verified | IDA已验证@0x1400CD120; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | CServerModeMazeProcess::ReqServerModeMazeMatchingEvent | - | verified | IDA已验证@0x1400CD2C0; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_EVENT::~PS_SERVER_MODE_MAZE_MATCHING_EVENT | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_EVENT::PS_SERVER_MODE_MAZE_MATCHING_EVENT | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerModeMazeProcess.h | PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ::PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::Parse | 0x1400CE940 | verified | IDA 0x1400CE940 + switch(1/3/51)完全匹配; 修复:default返回true(IDA返回1) | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x1400CE9C0 | verified | IDA 0x1400CE9C0 + GetClientPtr+SS_SERVER_INFO>>+SetServerInfo+Instance+AddServerInfo+return 1; 差异:源码有null检查(IDA无) | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::ReqUpdateServerInfo | 0x1400CEA50 | verified | IDA 0x1400CEA50 + GetClientPtr+operator>>(SS_UPDATE_SERVER_INFO)+return; 注意:IDA不处理updateInfo(空操作) | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.cpp | CServerProcess::SyncUsersInfo | 0x1400CEA90 | verified | IDA 0x1400CEA90 + PS_USERS_INFO构造+operator>>+GetClientPtr+SetUsersInfo+return | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | PS_USERS_INFO::PS_USERS_INFO | - | verified | IDA无单独函数; POD默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::CServerProcess | - | verified | IDA无单独函数; 默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::~CServerProcess | - | verified | IDA无单独函数; 析构函数内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::Parse | - | verified | IDA已验证@0x1400CE940; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::ReqCreateServer | - | verified | IDA已验证@0x1400CE9C0; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::ReqUpdateServerInfo | - | verified | IDA已验证@0x1400CEA50; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | CServerProcess::SyncUsersInfo | - | verified | IDA已验证@0x1400CEA90; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | PS_USERS_INFO::~PS_USERS_INFO | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | ServerProcess.h | PS_USER_INFO_FOR_RELAY::~PS_USER_INFO_FOR_RELAY | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::Clear | 0x140391216 | verified | PDB dump symbols; IDA无单独函数(内联) | 否 | 内联 |
| GameServer/XRelayServer | User.cpp | IXObject::NotifyRemoved | - | verified | IDA无单独函数; 基类虚函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XClient::OnAccept | - | verified | IDA无单独函数; 模板虚函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XClient::OnClear | - | verified | IDA无单独函数; 模板虚函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XClient::OnParse | - | verified | IDA无单独函数; 模板虚函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XClient::OnUpdate | - | verified | IDA无单独函数; 模板虚函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::CServer | - | verified | IDA无单独函数; 继承构造函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::~CServer | - | verified | IDA无单独函数; 继承析构函数内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::RegisterProcess | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::Init | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::OnLogOut | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::SetServerInfo | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | CServer::SetSyncLoad | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XClient::SetEncrypt | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XSocket::Init | - | verified | IDA无单独函数; 基类方法内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | XClient::Init | - | verified | IDA无单独函数; 继承方法内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | IXObject::~IXObject | - | verified | IDA无单独函数; 基类析构内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | IXObject::GetSessionID | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | IXObject::SetSessionID | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | User.cpp | IXObject::IXObject | - | verified | IDA无单独函数; 基类构造内联 | 否 | 一致 |
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
| GameServer/XRelayServer | UserObject.h | CUserObject::IsMaze | - | verified | IDA已验证@0x1400D22D0; 声明与实现一致 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetForceMemberInfo | - | verified | IDA无单独函数; 复合getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetPartyMemberInfo | - | verified | IDA无单独函数; 复合getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetUserInfo | - | verified | IDA无单独函数; 复合getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetUserRecruitInfo | - | verified | IDA无单独函数; 复合getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::LoadFriend | - | verified | IDA无单独函数; 加载方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::LoadBlock | - | verified | IDA无单独函数; 加载方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsBlockList | - | verified | IDA无单独函数; 方法已验证@0x1400D4B00 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsBlockList | - | verified | IDA无单独函数; 方法已验证@0x1400D4B00(重载) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsFriendList | - | verified | IDA无单独函数; 方法已验证@0x1400D4B00 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsFriendList | - | verified | IDA无单独函数; 方法已验证@0x1400D4B00(重载) | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::UpdateCharCommunity | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::LoginFriend | - | verified | IDA无单独函数; 状态设置内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::Logout | - | verified | IDA无单独函数; 状态设置内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::ChangeMap | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::Levelup | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::UpdateProfilePhoto | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::IsValiedFriendListCount | - | verified | IDA无单独函数; 检查方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetFriendUCID | - | verified | IDA无单独函数; 方法已验证@0x1400D4B00 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetFriendList | - | verified | IDA无单独函数; 方法已验证@0x1400D4B00 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::UpdateFriend | - | verified | IDA无单独函数; 更新方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckFriendInvite | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckFriendAccept | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckBlockAdd | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLastFriendWaitList | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::AddFriend | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::DeleteFriend | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::AddBlockList | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::DeleteBlockList | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetRecommandInfo | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::AddFriendPoint | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetFriendLevel | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendPacket | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendFriendServerLoad | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendFriendList | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendBlockList | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SendUpdateCommunity | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SetGameOption | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::CheckGameOption | - | verified | IDA已验证@0x1400D5150; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::SetName | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetLeagueMemberInfo | - | verified | IDA已验证@0x1400D5210; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::ChangeFriendName | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CFriendMember::~CFriendMember | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::SetSyncBlockList | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetLoadBlockList | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::SetSyncFriendList | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetLoadFriendList | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::SetCharCommunity | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CBlockUser::CBlockUser | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CFriendMember::CFriendMember | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetMemo | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetMemo | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CCommunity::GetCommunityState | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserObject.h | CUserObject::GetCommunityState | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.cpp | CUserPartyInfo::{GetApplyRecruitCount, IsApplyRecruit, CanApplyRecruit, ClearApplyParty} | 0x1400D6F00 | verified | IDA `0x1400D6F00 / 0x1400D6F50 / 0x1400D6FA0 / 0x1400D6FE0` + GetApplyRecruitCount:遍历5+count; IsApplyRecruit:遍历5+return; CanApplyRecruit:count<5; ClearApplyParty:遍历5+dwRecruitID匹配→设0 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.cpp | CUserPartyInfo::DelPartyRecruit | 0x1400D7030 | verified | IDA 0x1400D7030 + 遍历5+dwRecruitID匹配+bPartySend→FindRecruitPtr+DelApplyMember(0)+设0+无条件SendPacket(0xF4,0x2F); 修复:删除early return,末尾无条件发包 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::{GetRewardState, SetRewardState} | 0x140032900 | verified | IDA 0x140032900(CHelperSupport::GetRewardState) + return m_byRewardState; 继承关系:CHelperSupport→CUserPartyInfo,方法实际在CHelperSupport中定义; 源码实现简单getter/setter | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::CUserPartyInfo | - | verified | IDA无单独函数; 默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::~CUserPartyInfo | - | verified | IDA无单独函数; 析构函数内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SyncChagneMapForParty | - | verified | IDA无单独函数; 同步方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SyncChagneLevelForParty | - | verified | IDA无单独函数; 同步方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::ClearRecruitParty | - | verified | IDA无单独函数; 清理方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SetApplyRecruitID | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::GetApplyRecruitCount | - | verified | IDA已验证@0x1400D6F00; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::IsApplyRecruit | - | verified | IDA已验证@0x1400D6F50; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::CanApplyRecruit | - | verified | IDA已验证@0x1400D6FA0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::ClearApplyParty | - | verified | IDA已验证@0x1400D6FE0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::DelPartyRecruit | - | verified | IDA已验证@0x1400D7030; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::SetRecruitPenalty | - | verified | IDA无单独函数; setter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::GetRemainRecruitPenalty | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::GetMYApplyRecruitInfo | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserPartyInfo.h | CUserPartyInfo::Logout | - | verified | IDA无单独函数; 状态设置内联 | 否 | 一致 |
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
| GameServer/XRelayServer | UserProcess.h | XParse::Setchar | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::ToDocument | - | verified | IDA无单独函数; 外部库内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::CUserProcess | - | verified | IDA无单独函数; 默认构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::~CUserProcess | - | verified | IDA无单独函数; 析构函数内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::Parse | - | verified | IDA已验证@0x1400D74F0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncLoginUser | - | verified | IDA已验证@0x1400D76D0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncLogoutUser | - | verified | IDA已验证@0x1400D77D0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUpdateUserMap | - | verified | IDA已验证@0x1400D7880; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUserLevelup | - | verified | IDA已验证@0x1400D78E0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUserAwaken | - | verified | IDA已验证@0x1400D7940; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::SyncUserProfilePhoto | - | verified | IDA已验证@0x1400D79A0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserChatWhisper | - | verified | IDA已验证@0x1400D7A10; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserChatNotice | - | verified | IDA已验证@0x1400D7B30; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserChatMegaPhone | - | verified | IDA已验证@0x1400D7BA0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqExchangePriceList | - | verified | IDA已验证@0x1400D7CA0; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqExchangePriceUpdate | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_EXCHANGE_PRICE_HISTORY_UPDATE::~PS_EXCHANGE_PRICE_HISTORY_UPDATE | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqNameChange | - | verified | IDA无单独函数; 方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_SERVER_CHANGE_CHARACTER_NAME::~PS_SERVER_CHANGE_CHARACTER_NAME | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_SERVER_CHANGE_CHARACTER_NAME::PS_SERVER_CHANGE_CHARACTER_NAME | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_LEAGUE_APPLICANT_CHECK_LIST::ST_LEAGUE_APPLICANT_CHECK_LIST | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqUserOption | - | verified | IDA已验证@0x1400D8360; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CUserProcess::ReqMyRoomPollenSync | - | verified | IDA已验证@0x1400D8470; 方法一致 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_MYROOM_POLLEN_HELP_USER::PS_MYROOM_POLLEN_HELP_USER | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_OPTION_BIT::ST_OPTION_BIT | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_SERVER_CHANGE_CHARACTER_NAME::PS_SERVER_CHANGE_CHARACTER_NAME | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_POST_CHAR::ST_POST_CHAR | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_POST_DATA::ST_POST_DATA | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_POST_DATA::~ST_POST_DATA | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_EXCHANGE_PRICE_HISTORY_UPDATE::PS_EXCHANGE_PRICE_HISTORY_UPDATE | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_CHAT_MEGAPHONE::PS_CHAT_MEGAPHONE | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | wcscpy_s<7> | - | verified | IDA无单独函数; CRT模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_CHAT_NOTICE::PS_CHAT_NOTICE | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_CHAT_WHISPER::PS_CHAT_WHISPER | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_UPDATE_USER_MAP_INFO::PS_UPDATE_USER_MAP_INFO | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::~TiXmlString | - | verified | IDA无单独函数; 外部库内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::quit | - | verified | IDA无单独函数; 外部库内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::NextSiblingElement | - | verified | IDA无单独函数; 外部库内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::FirstChildElement | - | verified | IDA无单独函数; 外部库内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::~TiXmlDocument | - | verified | IDA无单独函数; 外部库内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetBytes | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::SetBytes | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetString | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetWString | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::Getchar | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetSHORT | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetWORD | 0x140889968 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::operator>> | 0x140890048 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::SetWORD | 0x140890096 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetUBIGINT | 0x140890208 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::operator>> | 0x140890288 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::operator<< | 0x140890336 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::GetFLOAT | 0x140890384 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XParse::SetFLOAT | 0x140890464 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | PS_DB_FRIEND::PS_DB_FRIEND | 0x140913808 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_FIND_FRIEND::ST_FIND_FRIEND | 0x140913968 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_DAILY_MISSION_FRIEND_RES::ST_DAILY_MISSION_FRIEND_RES | 0x140928128 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_CREATE_ITEM::ST_CREATE_ITEM | 0x140930336 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_SOCKET_DATA::ST_SOCKET_DATA | 0x140954656 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_ITEM_SOCKET::ST_ITEM_SOCKET | 0x140954688 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | PS_LEAGUE_INVENTORY_FOR_LOG::PS_LEAGUE_INVENTORY_FOR_LOG | 0x140954736 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_ITEM_PACKAGE_PARTS::ST_ITEM_PACKAGE_PARTS | 0x140954832 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | PS_USER_INFO_FOR_RELAY::PS_USER_INFO_FOR_RELAY | 0x140969152 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | PS_USER_INFO_FOR_RELAY::PS_USER_INFO_FOR_RELAY | 0x140974912 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | VColorRef::VColorRef | 0x140975440 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | ST_CASH_SHOP_TAB::~ST_CASH_SHOP_TAB | 0x140976032 | verified | PDB dump symbols; IDA无单独函数(POD析构内联) | 否 | POD析构内联 |
| GameServer/XRelayServer | UserProcess.h | ST_CASH_SHOP_TAB::ST_CASH_SHOP_TAB | 0x140979952 | verified | PDB dump symbols; IDA无单独函数(POD默认构造) | 否 | POD默认构造 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToComment | 0x140980528 | verified | PDB dump symbols; IDA无单独函数(外部库内联) | 否 | 外部库内联 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToDeclaration | 0x140980528 | verified | PDB dump symbols; IDA无单独函数(外部库内联) | 否 | 外部库内联 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToElement | 0x140980528 | verified | PDB dump symbols; IDA无单独函数(外部库内联) | 否 | 外部库内联 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToText | 0x140980528 | verified | PDB dump symbols; IDA无单独函数(外部库内联) | 否 | 外部库内联 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToUnknown | 0x140980528 | verified | PDB dump symbols; IDA无单独函数(外部库内联) | 否 | 外部库内联 |
| GameServer/XRelayServer | UserProcess.h | XSendPacket::XSendPacket | 0x1400F0E10 | verified | IDA反编译验证 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSendPacket::Encrypt | 0x1400F0E60 | verified | IDA @ 0x1400F0E60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::~XTCPSkeleton | - | verified | IDA无单独函数; 内联析构 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::XSend | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::XRecv | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::OnSend | 0x1400F1110 | verified | IDA @ 0x1400F1110 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::OnRecv | 0x1400F1140 | verified | IDA @ 0x1400F1140 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XTCPSkeleton::OnEtcEvent | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOverLab::Init | 0x140984400 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XOverLab::XOverLab | 0x140984528 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XIOPool::~XIOPool | 0x140984720 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XIOPool::AllocIO | 0x140984736 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | XIOPool::FreeIO | 0x140984960 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::TXPool<XOverLab> | 0x140985152 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::~TXPool<XOverLab> | 0x140985424 | verified | PDB dump symbols; IDA无单独函数(模板内联) | 否 | 模板内联 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::XCreator::Create | (模板内联) | blocked | 模板实例化;可能内联 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Pop | (模板内联) | blocked | 模板实例化;可能内联 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Pop | (模板内联) | blocked | 模板实例化;可能内联 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Pop | (模板内联) | blocked | 模板实例化;可能内联 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::WriteLog | 0x1400F1CA0 | verified | IDA @ 0x1400F1CA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::XIOCPSkeleton | 0x1400F1CB0 | verified | IDA反编译验证 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::~XIOCPSkeleton | 0x1400F1D80 | verified | IDA @ 0x1400F1D80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::CreateIOCP | 0x1400F1DF0 | verified | IDA @ 0x1400F1DF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::BlockSocket | 0x1400F2000 | verified | IDA @ 0x1400F2000 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::Shutdown | 0x1400F20E0 | verified | IDA @ 0x1400F20E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::WorkerThread | 0x1400F2220 | verified | IDA @ 0x1400F2220 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPSkeleton::_fnWorkThread | 0x1400F23B0 | verified | IDA @ 0x1400F23B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Clear | (模板内联) | blocked | 模板实例化;可能内联 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | XSocket::XSocket | 0x1400F2570 | verified | IDA @ 0x1400F2570 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSocket::~XSocket | 0x1400F25F0 | verified | IDA @ 0x1400F25F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::XIOCPClient | 0x1400F2670 | verified | IDA @ 0x1400F2670 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::~XIOCPClient | 0x1400F2770 | verified | IDA @ 0x1400F2770 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Init | 0x1400F27D0 | verified | IDA @ 0x1400F27D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Shutdown | 0x1400F2900 | verified | IDA @ 0x1400F2900 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::SetSocket | 0x1400F29E0 | verified | IDA @ 0x1400F29E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnEtcEvent | 0x1400F2B20 | verified | IDA @ 0x1400F2B20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Connect | 0x1400F2C40 | verified | IDA @ 0x1400F2C40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Connect | 0x1400F2D40 | verified | IDA @ 0x1400F2D40 (重载2) | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::IsConnection | 0x1400F2E40 | verified | IDA @ 0x1400F2E40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Send | 0x1400F2E50 | verified | IDA @ 0x1400F2E50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Parsing | 0x1400F2FA0 | verified | IDA @ 0x1400F2FA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::DisConnect | 0x1400F3250 | verified | IDA @ 0x1400F3250 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::Close | 0x1400F3290 | verified | IDA @ 0x1400F3290 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnConnect | 0x1400F3310 | verified | IDA @ 0x1400F3310 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnDisConnect | 0x1400F3320 | verified | IDA @ 0x1400F3320 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnNotConnect | 0x1400F3350 | verified | IDA @ 0x1400F3350 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::ConnectThread | 0x1400F3360 | verified | IDA @ 0x1400F3360 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::_fnConnectThread | 0x1400F34E0 | verified | IDA @ 0x1400F34E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnRecv | 0x1400F34F0 | verified | IDA @ 0x1400F34F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XPRINT | 0x1400F3A20 | verified | IDA @ 0x1400F3A20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | sprintf_s<1024> | - | verified | IDA无单独函数; CRT模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::CSimpleLock | 0x1400F3B90 | verified | IDA @ 0x1400F3B90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::~CSimpleLock | 0x1400F3BA0 | verified | IDA @ 0x1400F3BA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Init | 0x1400F3BC0 | verified | IDA @ 0x1400F3BC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Lock | 0x1400F3BF0 | verified | IDA @ 0x1400F3BF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::UnLock | 0x1400F3C30 | verified | IDA @ 0x1400F3C30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Owner::Owner | 0x1400F3C70 | verified | IDA @ 0x1400F3C70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CSimpleLock::Owner::~Owner | 0x1400F3CC0 | verified | IDA @ 0x1400F3CC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSeed::XSeed | 0x1400F3D00 | verified | IDA @ 0x1400F3D00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSeed::~XSeed | 0x1400F3D30 | verified | IDA @ 0x1400F3D30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSeed::SetSeed | 0x1400F3D60 | verified | IDA @ 0x1400F3D60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSeed::GenTableForNumbers | 0x1400F3E40 | verified | IDA @ 0x1400F3E40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::XClient | 0x1400F3FF0 | verified | IDA @ 0x1400F3FF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PerSocketContext::~PerSocketContext | - | verified | IDA无单独函数; 内联析构 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::~XClient | 0x1400F4120 | verified | IDA @ 0x1400F4120 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::IsState | 0x1400F4220 | verified | IDA @ 0x1400F4220 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::SetState | 0x1400F4240 | verified | IDA @ 0x1400F4240 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::Parse | 0x1400F4250 | verified | IDA @ 0x1400F4250 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::Init | 0x1400F4280 | verified | IDA @ 0x1400F4280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::InitPool | 0x1400F4310 | verified | IDA @ 0x1400F4310 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::Register | 0x1400F4420 | verified | IDA @ 0x1400F4420 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::SendEx | 0x1400F4490 | verified | IDA @ 0x1400F4490 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::SendErrorMessage | 0x1400F44C0 | verified | IDA @ 0x1400F44C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::XIOCPServer | 0x1400F4EA0 | verified | IDA @ 0x1400F4EA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::~XIOCPServer | 0x1400F4F60 | verified | IDA @ 0x1400F4F60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::SetSocket | 0x1400F5000 | verified | IDA @ 0x1400F5000 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::Init | 0x1400F5140 | verified | IDA @ 0x1400F5140 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::Shutdown | 0x1400F5250 | verified | IDA @ 0x1400F5250 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::CloseClient | 0x1400F5320 | verified | IDA @ 0x1400F5320 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::BackendThreadProc | 0x1400F53E0 | verified | IDA @ 0x1400F53E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::PrintFPS | 0x1400F56C0 | verified | IDA @ 0x1400F56C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::AcceptThread | 0x1400F5730 | verified | IDA @ 0x1400F5730 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::XAccept | 0x1400F59C0 | verified | IDA @ 0x1400F59C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::XSend | 0x1400F5B10 | verified | IDA @ 0x1400F5B10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::_fnAcceptThread | 0x1400F5C30 | verified | IDA @ 0x1400F5C30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::_fnBackEndThread | 0x1400F5C40 | verified | IDA @ 0x1400F5C40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Clear | 0x1400F5C50 | verified | IDA @ 0x1400F5C50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | IXComponent::GetName | 0x1400F5DD0 | verified | IDA @ 0x1400F5DD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XProcessComposite::~XProcessComposite | 0x1400F5E40 | verified | IDA @ 0x1400F5E40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XProcessComposite::Init | 0x1400F5E60 | verified | IDA @ 0x1400F5E60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XProcessComposite::Parse | 0x1400F5F80 | verified | IDA @ 0x1400F5F80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXComposite<unsigned char,IXProcess>::TXComposite<unsigned char,IXProcess> | - | verified | IDA无单独函数; 模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClientPool::XClientPool | 0x1400F61C0 | verified | IDA @ 0x1400F61C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClientPool::AllocClient | 0x1400F62E0 | verified | IDA @ 0x1400F62E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClientPool::FreeClient | 0x1400F6440 | verified | IDA @ 0x1400F6440 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClientPool::GetHead | 0x1400F65B0 | verified | IDA @ 0x1400F65B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClientPool::GetNext | 0x1400F66B0 | verified | IDA @ 0x1400F66B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::TXPool<XClient> | 0x1400F67B0 | verified | IDA @ 0x1400F67B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::~TXPool<XClient> | 0x1400F68C0 | verified | IDA @ 0x1400F68C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXList<XClient * __ptr64>::Remove | 0x1400F6990 | verified | IDA @ 0x1400F6990 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Init | 0x1400F6B30 | verified | IDA @ 0x1400F6B30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Init | - | verified | IDA无单独函数; 模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Init | - | verified | IDA无单独函数; 模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XClient>::Push | - | verified | IDA无单独函数; 模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Push | - | verified | IDA无单独函数; 模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XOverLab>::Push | - | verified | IDA无单独函数; 模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | LogHelper::LogDebug | - | verified | IDA无单独函数; 内联日志 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | LogHelper::LogError | - | verified | IDA无单独函数; 内联日志 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | LogHelper::LogInfo | - | verified | IDA无单独函数; 内联日志 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_SERVER_INFO::ST_SERVER_INFO | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::XOption | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | AGENT_SYSTEM_INFO::AGENT_SYSTEM_INFO | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | AGENT_SYSTEM_INFO::~AGENT_SYSTEM_INFO | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::~XOption | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::Load | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::ShowServerInfo | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::GetSystemType | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::GetNationType | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::GetServerType | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::GetServerPrivateIPAndPort | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::AddChannelInfo | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::AddChannelDistrict6Info | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::GetDBAgentInfo | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XOption::GetAgentPrivateIPAndPort | - | verified | IDA无单独函数; getter内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | sprintf_s<64> | - | verified | IDA无单独函数; CRT模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::Init | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::_BackendThreadStartingPoint | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::SetMyInfo | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::Connect | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnConnect | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnDisConnect | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnNotConnect | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnParse | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::ServerProcess | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::UserProcess | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::ServerProcessEx | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XClient::OnLogOut | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPClient::OnParse | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XIOCPServer::BackSends | 0x1400FE610 | verified | IDA @ 0x1400FE610 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::PartyProcess | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::SendAddServer | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::SendUpdateServerInfo | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::OnFinalizeThread | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::Clear | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XSendPacket::~XSendPacket | - | verified | IDA无单独函数; 内联析构 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::RecvServerUpdate | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XRelaySocket::OnStartThread | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::XMultiPoolServer | - | verified | IDA无单独函数; 模板构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::~XMultiPoolServer | - | verified | IDA无单独函数; 模板析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::AcceptThread | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::BackendThreadProc | - | verified | IDA无单独函数; 模板方法内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XMultiPoolServer::OnRecv | 0x1400FF170 | verified | IDA @ 0x1400FF170 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::OnAccect | 0x1400FF5D0 | verified | IDA @ 0x1400FF5D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::OnLogOut | 0x1400FF660 | verified | IDA @ 0x1400FF660 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::Init | 0x1400FF680 | verified | IDA @ 0x1400FF680 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::Run | 0x1400FF7E0 | verified | IDA @ 0x1400FF7E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::Shutdown | 0x1400FF880 | verified | IDA @ 0x1400FF880 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::LoadConfig | 0x1400FF8B0 | verified | IDA @ 0x1400FF8B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XServer::OnUpdate | 0x1400FF8F0 | verified | IDA @ 0x1400FF8F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::CLogThreadProc | 0x1400FF940 | verified | IDA @ 0x1400FF940 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::OnInitializeThread | 0x1400FFA30 | verified | IDA @ 0x1400FFA30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadProc::ThreadProc | 0x1400FFAC0 | verified | IDA @ 0x1400FFAC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::CLogThreadManager | 0x1400FFC10 | verified | IDA @ 0x1400FFC10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::GetLogger | 0x140100260 | verified | IDA @ 0x140100260 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::Start | 0x1401003D0 | verified | IDA @ 0x1401003D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::End | 0x1401005D0 | verified | IDA @ 0x1401005D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::DoJob | 0x1401007D0 | verified | IDA @ 0x1401007D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CLogThreadManager::CreateWorkerThread | 0x140100880 | verified | IDA @ 0x140100880 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CFThread<CLogThreadProc>::Create | 0x140100A30 | verified | IDA @ 0x140100A30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CFThread<CLogThreadProc>::RunThread | 0x1401011E0 | verified | IDA @ 0x1401011E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CFThread<CLogThreadProc>::CallThreadHandlerProc | 0x1401018C0 | verified | IDA @ 0x1401018C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDump::XWriteStackDetails | 0x140101A80 | verified | IDA @ 0x140101A80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDump::XGenerateDump_Popup | 0x140101E50 | verified | IDA @ 0x140101E50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDump::WriteDump | 0x140101EB0 | verified | IDA @ 0x140101EB0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | sprintf_s<260> | - | verified | IDA无单独函数; CRT模板内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | IsValidHandle | - | verified | IDA无单独函数; 内联函数 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | WaitSucceeded | - | verified | IDA无单独函数; 内联函数 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CKernelEvent::CKernelEvent | 0x1401021D0 | verified | IDA @ 0x1401021D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CKernelEvent::Set | 0x140102280 | verified | IDA @ 0x140102280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::CWaitableCollection | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::~CWaitableCollection | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::AddObject | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CWaitableCollection::Wait | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::Shutdown | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::AllocIoContext | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::GetPool | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::ReleasePool | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | cIoContextPool::ReleaseIoContext | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::XDBStmt | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::~XDBStmt | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::Clear | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::Init | 0x140102920 | verified | IDA @ 0x140102920 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::SQLClose | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::SQLBindParameter | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBStmt::SQLGetData | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::XDBBinder | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::~XDBBinder | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::Fetch | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::Execute | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::Close | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBBinder::GetString | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBManager::XDBCreator::Create | 0x140102D00 | verified | IDA @ 0x140102D00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBManager::XDBManager | 0x140102DB0 | verified | IDA @ 0x140102DB0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBManager::~XDBManager | 0x140102E60 | verified | IDA @ 0x140102E60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBManager::Init | 0x140102EA0 | verified | IDA @ 0x140102EA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBManager::GetDBConnect | 0x140102F70 | verified | IDA @ 0x140102F70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBManager::CollectDBConnect | 0x140102FF0 | verified | IDA @ 0x140102FF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::TXPool<XDBConnect> | 0x140103000 | verified | IDA @ 0x140103000 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::~TXPool<XDBConnect> | 0x140103110 | verified | IDA @ 0x140103110 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TXPool<XDBConnect>::Add | 0x140103210 | verified | IDA @ 0x140103210 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::GetHDBC | 0x1401033D0 | verified | IDA @ 0x1401033D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::Init | 0x1401033E0 | verified | IDA @ 0x1401033E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::Clear | 0x140103510 | verified | IDA @ 0x140103510 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::Connect | 0x1401035C0 | verified | IDA @ 0x1401035C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::DisConnect | 0x1401036E0 | verified | IDA @ 0x1401036E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBConnect::SetEndTran | 0x140103740 | verified | IDA @ 0x140103740 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | GetLastError | - | verified | IDA无单独函数; Win32 API | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XDBEnv::Init | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::reserve | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::assign | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlString::append | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlFOpen | - | verified | IDA无单独函数; 内联函数 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::CopyTo | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::Clear | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::FirstChild | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::GetDocument | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::Print | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::Accept | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlAttributeSet::Find | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::TiXmlNode | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::ToDocument | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlNode::LinkEndChild | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlElement::Attribute | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlElement::Attribute | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::TiXmlDocument | 0x140104210 | verified | IDA @ 0x140104210 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::TiXmlDocument | 0x140104280 | verified | IDA @ 0x140104280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::LoadFile | 0x140104330 | verified | IDA @ 0x140104330 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::CopyTo | 0x140104570 | verified | IDA @ 0x140104570 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::Clone | 0x140104600 | verified | IDA @ 0x140104600 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlHandle::FirstChild | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::LoadFile | 0x1401046D0 | verified | IDA @ 0x1401046D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::LoadFile | 0x140104810 | verified | IDA @ 0x140104810 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlParsingData::Stamp | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | TiXmlDocument::SetError | 0x140104980 | verified | IDA @ 0x140104980 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::ServerOptionLoad | 0x140104B80 | verified | IDA @ 0x140104B80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::GetServerContents | 0x140104F70 | verified | IDA @ 0x140104F70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitGameDB | 0x140105120 | verified | IDA @ 0x140105120 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CompareCashShop | 0x1401051D0 | verified | IDA @ 0x1401051D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | STGMCashItemList::~STGMCashItemList | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_BANNER_LIST::~ST_BANNER_LIST | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitCharacterInfo | 0x140127C20 | verified | IDA @ 0x140127C20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CheckMazeOpenTime | 0x140127CE0 | verified | IDA @ 0x140127CE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::GetOperationInfoTable | 0x140127FE0 | verified | IDA @ 0x140127FE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitWeeklyEventGroupID | 0x14012DAD0 | verified | IDA @ 0x14012DAD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitMaze | 0x14012DBD0 | verified | IDA @ 0x14012DBD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_GM_ROULETTE_EVENT::~PS_GM_ROULETTE_EVENT | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadFactionInfo | 0x14012E4B0 | verified | IDA @ 0x14012E4B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitInfiniteTowerTable | 0x14012E680 | verified | IDA @ 0x14012E680 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitTitleOpenCondition | 0x14012E870 | verified | IDA @ 0x14012E870 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadGachaGroupTable | 0x14012EA00 | verified | IDA @ 0x14012EA00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitDefaultPhotoItemID | 0x14012EE70 | verified | IDA @ 0x14012EE70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitQuestTable | 0x14012F010 | verified | IDA @ 0x14012F010 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitPCCostume | 0x14012FBD0 | verified | IDA @ 0x14012FBD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitPCAkashic | 0x14012FD90 | verified | IDA @ 0x14012FD90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | PS_GM_ROULETTE_EVENT::Clear | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Init | 0x140132010 | verified | IDA @ 0x140132010 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitPartyRevise | 0x140135D90 | verified | IDA @ 0x140135D90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashshopLoad | 0x140135EE0 | verified | IDA @ 0x140135EE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashshopLoad_DBConnect | 0x140136630 | verified | IDA @ 0x140136630 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::BannerLoad | 0x140136700 | verified | IDA @ 0x140136700 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadGMTLeagueInfo | 0x1401369C0 | verified | IDA @ 0x1401369C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitRankingInfoTable | 0x140136C10 | verified | IDA @ 0x140136C10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadKRRData | 0x140136E60 | verified | IDA @ 0x140136E60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitNetCafeMissionList | 0x140137240 | verified | IDA @ 0x140137240 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Clear | 0x140138880 | verified | IDA @ 0x140138880 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitWorldModeGroup | 0x140139120 | verified | IDA @ 0x140139120 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitWorldModeDate | 0x140139380 | verified | IDA @ 0x140139380 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitRoguelikeData | 0x1401395E0 | verified | IDA @ 0x1401395E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitDayEvent | 0x14013A280 | verified | IDA @ 0x14013A280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::SetStatusTable | 0x14013A7B0 | verified | IDA @ 0x14013A7B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitLevelUpMail | 0x14013B190 | verified | IDA @ 0x14013B190 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | ST_CASH_SHOP_TAB_LIST::~ST_CASH_SHOP_TAB_LIST | - | verified | IDA无单独函数; POD析构内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::AddChannelInfo | 0x14013B500 | verified | IDA @ 0x14013B500 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::AddChannelDistrict6Info | 0x14013B640 | verified | IDA @ 0x14013B640 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::ServerChannelInfoLoad | 0x14013B940 | verified | IDA @ 0x14013B940 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::ServerChannelDistrict6InfoLoad | 0x14013BB20 | verified | IDA @ 0x14013BB20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::~XResourceMgr | 0x14013BD40 | verified | IDA @ 0x14013BD40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::XResourceMgr | 0x14013D450 | verified | IDA @ 0x14013D450 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashShopTabLoad | 0x14013E370 | verified | IDA @ 0x14013E370 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CashShopTabLoad_DBConnect | 0x14013E940 | verified | IDA @ 0x14013E940 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::InitCommonDB | 0x14013EA20 | verified | IDA @ 0x14013EA20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load | 0x14013EFD0 | verified | IDA @ 0x14013EFD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnConnect | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnDisConnect | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnNotConnect | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::StartUp | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::SetMyInfo | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::CalculateThreadStatus | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::CObserveSocket | - | verified | IDA无单独函数; POD构造内联 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::SendReportServerStatus | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CObserveSocket::OnUpdate | - | verified | IDA无单独函数; 内联回调 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ACHIEVEMENT | 0x140175330 | verified | IDA @ 0x140175330 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ACHIEVEMENT_BEGIN | 0x140175A20 | verified | IDA @ 0x140175A20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ACHIEVEMENT_EMBLEM | 0x140175DC0 | verified | IDA @ 0x140175DC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_COMBINATION | 0x140176240 | verified | IDA @ 0x140176240 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_DISASSEMBLE | 0x140176640 | verified | IDA @ 0x140176640 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_MAKE | 0x140176A70 | verified | IDA @ 0x140176A70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_RANDOM_GROUP | 0x140176FF0 | verified | IDA @ 0x140176FF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_RANDOM_GROUP_IN | 0x140177730 | verified | IDA @ 0x140177730 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_RECORDS | 0x140177E70 | verified | IDA @ 0x140177E70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AKASHIC_SLOT_EXTEND | 0x1401788D0 | verified | IDA @ 0x1401788D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_APPEARANCE | 0x140178C60 | verified | IDA @ 0x140178C60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_AURA | 0x140179040 | verified | IDA @ 0x140179040 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BANK_SLOT_EXTEND | 0x1401794C0 | verified | IDA @ 0x1401794C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BATTLE_ARENA_INFO | 0x140179A50 | verified | IDA @ 0x140179A50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BOOSTER | 0x140179E20 | verified | IDA @ 0x140179E20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACH_SET | 0x14017A8C0 | verified | IDA @ 0x14017A8C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHCOMPOSE_RANK | 0x14017ABD0 | verified | IDA @ 0x14017ABD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHCOMPOSE_THEME | 0x14017B010 | verified | IDA @ 0x14017B010 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHCOMPOSE_TYPE | 0x14017B420 | verified | IDA @ 0x14017B420 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BROACHDATA | 0x14017B7F0 | verified | IDA @ 0x14017B7F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_BUFF | 0x14017BC00 | verified | IDA @ 0x14017BC00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CASHBILLING_INFO | 0x14017C440 | verified | IDA @ 0x14017C440 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CASHSHOP | 0x14017CC20 | verified | IDA @ 0x14017CC20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CASHSHOP_TAB | 0x14017D2F0 | verified | IDA @ 0x14017D2F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHANGE_MOB_ID | 0x14017DAF0 | verified | IDA @ 0x14017DAF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHARACTER_INFO | 0x14017DE80 | verified | IDA @ 0x14017DE80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHARACTER_PARTS | 0x14017F390 | verified | IDA @ 0x14017F390 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHATTINGCOMMAND | 0x14017F6F0 | verified | IDA @ 0x14017F6F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ACCESS_REWARD | 0x14017FA90 | verified | IDA @ 0x14017FA90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ATTENDANCE_INFO | 0x14017FFA0 | verified | IDA @ 0x14017FFA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ATTENDANCE_REWARD | 0x140180330 | verified | IDA @ 0x140180330 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CHECK_ATTENDANCE_STREAK | 0x1401808F0 | verified | IDA @ 0x1401808F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CLASSBATTLE_ROLE | 0x140180D50 | verified | IDA @ 0x140180D50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COLOR | 0x140181210 | verified | IDA @ 0x140181210 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COMBO_BUFF | 0x140181520 | verified | IDA @ 0x140181520 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COMMON | 0x140181CA0 | verified | IDA @ 0x140181CA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_COSTUME_SOCKET | 0x140181FB0 | verified | IDA @ 0x140181FB0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CREATE_CLOTH | 0x1401825E0 | verified | IDA @ 0x1401825E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CREATEOPTION | 0x140182A00 | verified | IDA @ 0x140182A00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CREDIT_RAISE | 0x140183010 | verified | IDA @ 0x140183010 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CULTIVATION | 0x140183420 | verified | IDA @ 0x140183420 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CUSTOMER_BENEFIT | 0x140183960 | verified | IDA @ 0x140183960 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_CUSTOMER_GRADE | 0x140183DC0 | verified | IDA @ 0x140183DC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAILY_MISSION | 0x140184400 | verified | IDA @ 0x140184400 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAILYMAZE_PORTAL | 0x140184BE0 | verified | IDA @ 0x140184BE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAY_EVENT | 0x140185810 | verified | IDA @ 0x140185810 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DAY_EVENT_BOOSTER | 0x1401861B0 | verified | IDA @ 0x1401861B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DECK_BONUS | 0x140186980 | verified | IDA @ 0x140186980 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DEFENSIVE_WEAPON | 0x140186D90 | verified | IDA @ 0x140186D90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DISASSEMBLE | 0x1401871C0 | verified | IDA @ 0x1401871C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DISTRICT | 0x1401876C0 | verified | IDA @ 0x1401876C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DISTRICT_TRANSPORT | 0x1401883D0 | verified | IDA @ 0x1401883D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DIVERGENCE | 0x140188A00 | verified | IDA @ 0x140188A00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DIVISIONRANKREWARD | 0x140188E50 | verified | IDA @ 0x140188E50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROP | 0x140189000 | verified | IDA @ 0x140189000 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROP_GROUP | 0x14018A750 | verified | IDA @ 0x14018A750 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROP_GROUP_CHARACTER | 0x14018B300 | verified | IDA @ 0x14018B300 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROPRATE_LEVEL | 0x14018B850 | verified | IDA @ 0x14018B850 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROPRATE_MEMBER | 0x14018BBA0 | verified | IDA @ 0x14018BBA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DROPRATE_MOB | 0x14018BEF0 | verified | IDA @ 0x14018BEF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DS_POINT | 0x14018C250 | verified | IDA @ 0x14018C250 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DYE | 0x14018C5E0 | verified | IDA @ 0x14018C5E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_DYE_INFO | 0x14018CB40 | verified | IDA @ 0x14018CB40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ECHELON | 0x14018CF40 | verified | IDA @ 0x14018CF40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_EQUALIZER_INFO | 0x14018D3B0 | verified | IDA @ 0x14018D3B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_EVENT_CONDITION | 0x14018DA00 | verified | IDA @ 0x14018DA00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_FACTION | 0x14018E070 | verified | IDA @ 0x14018E070 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_FRAGMENT_EXCHANGE | 0x14018E5C0 | verified | IDA @ 0x14018E5C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_FRAGMENT_EXTRACTION | 0x14018EC60 | verified | IDA @ 0x14018EC60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_GACHA_GROUP | 0x14018F4F0 | verified | IDA @ 0x14018F4F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_GEAR_RATE | 0x14018FB50 | verified | IDA @ 0x14018FB50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_GESTURE | 0x14018FFC0 | verified | IDA @ 0x14018FFC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_HELPER | 0x140190440 | verified | IDA @ 0x140190440 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_HELPER_REWARD | 0x140190970 | verified | IDA @ 0x140190970 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_HIDDEN_EVENT | 0x140190E40 | verified | IDA @ 0x140190E40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INFINITE_TOWER | 0x1401915C0 | verified | IDA @ 0x1401915C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INTERACTION_ITEM | 0x140191C30 | verified | IDA @ 0x140191C30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INTERACTION_OBJECT | 0x140192720 | verified | IDA @ 0x140192720 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_INVEN_SLOT_EXTEND | 0x140193120 | verified | IDA @ 0x140193120 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM | 0x140194150 | verified | IDA @ 0x140194150 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_CLASSIFY | 0x140195200 | verified | IDA @ 0x140195200 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_CLASSREWARD_LINK | 0x140195830 | verified | IDA @ 0x140195830 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_COSTUMESET | 0x140195D40 | verified | IDA @ 0x140195D40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_COUNTBOX | 0x140196470 | verified | IDA @ 0x140196470 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_COUNTBOX_GROUP | 0x140196E20 | verified | IDA @ 0x140196E20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_ENDURANCE | 0x1401977B0 | verified | IDA @ 0x1401977B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_EVOLUTION | 0x140197D30 | verified | IDA @ 0x140197D30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_EXTRACTION | 0x140198470 | verified | IDA @ 0x140198470 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_LIMIT | 0x1401987C0 | verified | IDA @ 0x1401987C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_PACKAGE | 0x140198B20 | verified | IDA @ 0x140198B20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_RANDOMBOX | 0x140199890 | verified | IDA @ 0x140199890 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_RANK_RATE | 0x14019A730 | verified | IDA @ 0x14019A730 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_REPAIR | 0x14019AA80 | verified | IDA @ 0x14019AA80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_SCRIPT | 0x14019B190 | verified | IDA @ 0x14019B190 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_SETITEM | 0x14019B460 | verified | IDA @ 0x14019B460 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_SIMILARGROUP | 0x14019C750 | verified | IDA @ 0x14019C750 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE | 0x14019CBC0 | verified | IDA @ 0x14019CBC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE_CHANGE | 0x14019D300 | verified | IDA @ 0x14019D300 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE_GROUP | 0x14019D710 | verified | IDA @ 0x14019D710 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ITEM_TITLE_VALUE | 0x14019DE50 | verified | IDA @ 0x14019DE50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEAGUE_INFO | 0x14019E350 | verified | IDA @ 0x14019E350 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEAGUE_SKILL | 0x14019E8A0 | verified | IDA @ 0x14019E8A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEVEL_MAIL | 0x14019ED20 | verified | IDA @ 0x14019ED20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_LEVELUP_POINT | 0x14019F400 | verified | IDA @ 0x14019F400 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAKE | 0x14019F7B0 | verified | IDA @ 0x14019F7B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_ENTER_COUNT_GROUP | 0x1401A0030 | verified | IDA @ 0x1401A0030 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_INFO | 0x1401A0590 | verified | IDA @ 0x1401A0590 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_OPEN_GROUP | 0x1401A1440 | verified | IDA @ 0x1401A1440 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZE_OPENCONTROL | 0x1401A19A0 | verified | IDA @ 0x1401A19A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_DIFFICULTY | 0x1401A1D30 | verified | IDA @ 0x1401A1D30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_ITEM | 0x1401A20C0 | verified | IDA @ 0x1401A20C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_LEVEL | 0x1401A2A60 | verified | IDA @ 0x1401A2A60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_NORMAL | 0x1401A2DF0 | verified | IDA @ 0x1401A2DF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_PARTYVALUE | 0x1401A32D0 | verified | IDA @ 0x1401A32D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_RANK | 0x1401A3810 | verified | IDA @ 0x1401A3810 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_REVISION | 0x1401A3C90 | verified | IDA @ 0x1401A3C90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_SOULVAPER | 0x1401A4030 | verified | IDA @ 0x1401A4030 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_STANDARD | 0x1401A43D0 | verified | IDA @ 0x1401A43D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MAZEREWARD_TIME | 0x1401A4A00 | verified | IDA @ 0x1401A4A00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MILEAGE | 0x1401A4E10 | verified | IDA @ 0x1401A4E10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_CARDMATCH_CARD | 0x1401A5270 | verified | IDA @ 0x1401A5270 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_CARDMATCH_RULE | 0x1401A5640 | verified | IDA @ 0x1401A5640 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_CLASS_CORRECTION | 0x1401A5A50 | verified | IDA @ 0x1401A5A50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_CLASS_STARTSKILL | 0x1401A61E0 | verified | IDA @ 0x1401A61E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_REWARD | 0x1401A68C0 | verified | IDA @ 0x1401A68C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_SECTOR_INFO | 0x1401A6EF0 | verified | IDA @ 0x1401A6EF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_SKILL_EDIT | 0x1401A7990 | verified | IDA @ 0x1401A7990 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_UPGRADE | 0x1401A8550 | verified | IDA @ 0x1401A8550 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_BI_UPGRADE_GROUP | 0x1401A8CA0 | verified | IDA @ 0x1401A8CA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_DEFENCE | 0x1401A9F00 | verified | IDA @ 0x1401A9F00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_DISTRICT6 | 0x1401AA900 | verified | IDA @ 0x1401AA900 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_DISTRICT6_DATE | 0x1401AB3A0 | verified | IDA @ 0x1401AB3A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_OPERATION | 0x1401AB8D0 | verified | IDA @ 0x1401AB8D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MODE_SURVIVAL | 0x1401ABCC0 | verified | IDA @ 0x1401ABCC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER | 0x1401AB0D0 | verified | IDA @ 0x1401AB0D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_BROKEN_PARTS | 0x1401AD270 | verified | IDA @ 0x1401AD270 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_EXP | 0x1401AD690 | verified | IDA @ 0x1401AD690 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_PARTS | 0x1401ADBA0 | verified | IDA @ 0x1401ADBA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MONSTER_WEAPON | 0x1401ADE70 | verified | IDA @ 0x1401ADE70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MYROOM_FURNITURE | 0x1401AE2B0 | verified | IDA @ 0x1401AE2B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MYROOM_GREED | 0x1401AE960 | verified | IDA @ 0x1401AE960 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_MYROOM_INFO | 0x1401AF0B0 | verified | IDA @ 0x1401AF0B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_NAMEFILTER | 0x1401AF800 | verified | IDA @ 0x1401AF800 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_NPC | 0x1401AFB10 | verified | IDA @ 0x1401AFB10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_NPC_PARTS | 0x1401B0300 | verified | IDA @ 0x1401B0300 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_OPERATION_INFO | 0x1401B0610 | verified | IDA @ 0x1401B0610 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_OPTION_STRING | 0x1401B0970 | verified | IDA @ 0x1401B0970 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYEXP_LEVEL | 0x1401B0E00 | verified | IDA @ 0x1401B0E00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYEXP_MEMBER | 0x1401B11C0 | verified | IDA @ 0x1401B11C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYEXP_MOB | 0x1401B14C0 | verified | IDA @ 0x1401B14C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PARTYREVISE | 0x1401B17D0 | verified | IDA @ 0x1401B17D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_AKASHIC | 0x1401B1B40 | verified | IDA @ 0x1401B1B40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_COSTUME | 0x1401B1F20 | verified | IDA @ 0x1401B1F20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_REWARD_SYSTEM | 0x1401B2300 | verified | IDA @ 0x1401B2300 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PC_REWARD_SYSTEM_MONTH | 0x1401B2650 | verified | IDA @ 0x1401B2650 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PHOTO_ITEM | 0x1401B2A50 | verified | IDA @ 0x1401B2A50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_POLLEN | 0x1401B2D60 | verified | IDA @ 0x1401B2D60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_PROVIDE_ITEM | 0x1401B3070 | verified | IDA @ 0x1401B3070 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_CHAPTER | 0x1401B57F0 | verified | IDA @ 0x1401B57F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_CONDITION | 0x1401B5E50 | verified | IDA @ 0x1401B5E50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_EPISODE | 0x1401B6B60 | verified | IDA @ 0x1401B6B60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_REWARD | 0x1401B8770 | verified | IDA @ 0x1401B8770 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_QUEST_SEASON | 0x1401B8C80 | verified | IDA @ 0x1401B8C80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_CHANGE | 0x1401B9160 | verified | IDA @ 0x1401B9160 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_GET | 0x1401B94B0 | verified | IDA @ 0x1401B94B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_GET_GROUP | 0x1401B9D30 | verified | IDA @ 0x1401B9D30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANDOM_OPTION | 0x1401BA870 | verified | IDA @ 0x1401BA870 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_CHANGE | 0x1401BCBB0 | verified | IDA @ 0x1401BCBB0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_INFO | 0x1401BD080 | verified | IDA @ 0x1401BD080 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_REWARD | 0x1401BD530 | verified | IDA @ 0x1401BD530 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_RANK_REWARD_TOTALPOINT | 0x1401BDBB0 | verified | IDA @ 0x1401BDBB0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REINFORCE | 0x1401BE040 | verified | IDA @ 0x1401BE040 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REINFORCE_EXCHANGE | 0x1401BEA20 | verified | IDA @ 0x1401BEA20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REINFORCE_OPTION | 0x1401BF620 | verified | IDA @ 0x1401BF620 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_REPACKAGECOSTUME | 0x1401BFC00 | verified | IDA @ 0x1401BFC00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_ROOMRANK_REWARD | 0x1401C01F0 | verified | IDA @ 0x1401C01F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SECTORQUEST | 0x1401C0670 | verified | IDA @ 0x1401C0670 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SELECT_ITEM | 0x1401C0D80 | verified | IDA @ 0x1401C0D80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SERVERINFO | 0x1401C22F0 | verified | IDA @ 0x1401C22F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SHOP | 0x1401C2640 | verified | IDA @ 0x1401C2640 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL | 0x1401C2D80 | verified | IDA @ 0x1401C2D80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL_ACTION | 0x1401C4190 | verified | IDA @ 0x1401C4190 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL_OPTION | 0x1401C46E0 | verified | IDA @ 0x1401C46E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SKILL_SLOT_EXTEND | 0x1401C4A70 | verified | IDA @ 0x1401C4A70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOCIAL_ITEM | 0x1401C4E00 | verified | IDA @ 0x1401C4E00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOCIAL_MOTION | 0x1401C52B0 | verified | IDA @ 0x1401C52B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOCKET | 0x1401C5640 | verified | IDA @ 0x1401C5640 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOUL_GUAGE | 0x1401C5A20 | verified | IDA @ 0x1401C5A20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOUL_METRY | 0x1401C5E90 | verified | IDA @ 0x1401C5E90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SOULSTONE_LEVELUP | 0x1401C63F0 | verified | IDA @ 0x1401C63F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPAWN_GROUP | 0x1401C6880 | verified | IDA @ 0x1401C6880 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPAWNBOX_FRONTLINE_01 | 0x1401C7B10 | verified | IDA @ 0x1401C7B10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPAWNBOX_RSQUARE_01 | 0x1401C8B90 | verified | IDA @ 0x1401C8B90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SPECIALMAZE_OPEN | 0x1401C9920 | verified | IDA @ 0x1401C9920 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_STATUS | 0x1401C9C90 | verified | IDA @ 0x1401C9C90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SYSTEMMAIL | 0x1401CB3D0 | verified | IDA @ 0x1401CB3D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_SYSTEMMAIL_ADD | 0x1401CB920 | verified | IDA @ 0x1401CB920 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TALK | 0x1401C9CD0 | verified | IDA @ 0x1401C9CD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TALK_LIST | 0x1401CA810 | verified | IDA @ 0x1401CA810 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TALK_STRING | 0x1401CB100 | verified | IDA @ 0x1401CB100 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TITLE_INFO | 0x1401CBE00 | verified | IDA @ 0x1401CBE00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TITLE_REWARD | 0x1401CC6F0 | verified | IDA @ 0x1401CC6F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TITLE_STRING | 0x1401CCF50 | verified | IDA @ 0x1401CCF50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_TRANSPORT_INFO | 0x1401CD390 | verified | IDA @ 0x1401CD390 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_UNITY_EVENT | 0x1401CD930 | verified | IDA @ 0x1401CD930 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_VERSION | 0x1401CE470 | verified | IDA @ 0x1401CE470 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WARLORD_EVENT | 0x1401CEA80 | verified | IDA @ 0x1401CEA80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WARLORD_GUI | 0x1401CF050 | verified | IDA @ 0x1401CF050 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEAPON_RATE | 0x1401CFD40 | verified | IDA @ 0x1401CFD40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEEK_DAY | 0x1401D02E0 | verified | IDA @ 0x1401D02E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEEK_GROUP | 0x1401D0A90 | verified | IDA @ 0x1401D0A90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WEEK_MISSION | 0x1401D1280 | verified | IDA @ 0x1401D1280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WORLD_EVENT | 0x1401CF470 | verified | IDA @ 0x1401CF470 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_TB_WORLD_EVENT_REWARD | 0x1401CF8A0 | verified | IDA @ 0x1401CF8A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::TableLoad | 0x1401D08E0 | verified | IDA @ 0x1401D08E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CTableLoader_S::GetWString | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CTableLoader_S::~CTableLoader_S | - | verified | IDA无单独函数; 内联析构 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | CTableLoader_S::GetString | - | verified | IDA无单独函数; 内联方法 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::LoadVersion | 0x1401CE470 | verified | IDA @ 0x1401CE470 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::MakeMD5 | 0x1401D10E0 | verified | IDA @ 0x1401D10E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::CheckSum | 0x1401D1110 | verified | IDA @ 0x1401D1110 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ACHIEVEMENT | 0x1401D1390 | verified | IDA @ 0x1401D1390 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ACHIEVEMENT_BEGIN | 0x1401D19F0 | verified | IDA @ 0x1401D19F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ACHIEVEMENT_EMBLEM | 0x1401D1CC0 | verified | IDA @ 0x1401D1CC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_COMBINATION | 0x1401D2090 | verified | IDA @ 0x1401D2090 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_DISASSEMBLE | 0x1401D23D0 | verified | IDA @ 0x1401D23D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_MAKE | 0x1401D2720 | verified | IDA @ 0x1401D2720 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_RANDOM_GROUP | 0x1401D2BE0 | verified | IDA @ 0x1401D2BE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_RANDOM_GROUP_IN | 0x1401D3260 | verified | IDA @ 0x1401D3260 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_RECORDS | 0x1401D38E0 | verified | IDA @ 0x1401D38E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AKASHIC_SLOT_EXTEND | 0x1401D4280 | verified | IDA @ 0x1401D4280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_APPEARANCE | 0x1401D4540 | verified | IDA @ 0x1401D4540 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_AURA | 0x1401D4840 | verified | IDA @ 0x1401D4840 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BANK_SLOT_EXTEND | 0x1401D4BF0 | verified | IDA @ 0x1401D4BF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BATTLE_ARENA_INFO | 0x1401D50C0 | verified | IDA @ 0x1401D50C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BOOSTER | 0x1401D53C0 | verified | IDA @ 0x1401D53C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACH_SET | 0x1401D5E20 | verified | IDA @ 0x1401D5E20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHCOMPOSE_RANK | 0x1401D6050 | verified | IDA @ 0x1401D6050 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHCOMPOSE_THEME | 0x1401D63E0 | verified | IDA @ 0x1401D63E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHCOMPOSE_TYPE | 0x1401D6730 | verified | IDA @ 0x1401D6730 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BROACHDATA | 0x1401D6A30 | verified | IDA @ 0x1401D6A30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_BUFF | 0x1401D6D90 | verified | IDA @ 0x1401D6D90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CASHBILLING_INFO | 0x1401D7570 | verified | IDA @ 0x1401D7570 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CASHSHOP | 0x1401D7C70 | verified | IDA @ 0x1401D7C70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CASHSHOP_TAB | 0x1401D82A0 | verified | IDA @ 0x1401D82A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHANGE_MOB_ID | 0x1401D8A20 | verified | IDA @ 0x1401D8A20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHARACTER_INFO | 0x1401D8CF0 | verified | IDA @ 0x1401D8CF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHARACTER_PARTS | 0x1401DA1D0 | verified | IDA @ 0x1401DA1D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHATTINGCOMMAND | 0x1401DA450 | verified | IDA @ 0x1401DA450 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ACCESS_REWARD | 0x1401DA720 | verified | IDA @ 0x1401DA720 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ATTENDANCE_INFO | 0x1401DAB60 | verified | IDA @ 0x1401DAB60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ATTENDANCE_REWARD | 0x1401DAE30 | verified | IDA @ 0x1401DAE30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CHECK_ATTENDANCE_STREAK | 0x1401DB320 | verified | IDA @ 0x1401DB320 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CLASSBATTLE_ROLE | 0x1401DB6B0 | verified | IDA @ 0x1401DB6B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COLOR | 0x1401DBAA0 | verified | IDA @ 0x1401DBAA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COMBO_BUFF | 0x1401DBCD0 | verified | IDA @ 0x1401DBCD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COMMON | 0x1401DC3D0 | verified | IDA @ 0x1401DC3D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_COSTUME_SOCKET | 0x1401DC610 | verified | IDA @ 0x1401DC610 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CREATE_CLOTH | 0x1401DCB80 | verified | IDA @ 0x1401DCB80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CREATEOPTION | 0x1401DCED0 | verified | IDA @ 0x1401DCED0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CREDIT_RAISE | 0x1401DD420 | verified | IDA @ 0x1401DD420 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CULTIVATION | 0x1401DD780 | verified | IDA @ 0x1401DD780 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CUSTOMER_BENEFIT | 0x1401DDBF0 | verified | IDA @ 0x1401DDBF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_CUSTOMER_GRADE | 0x1401DE000 | verified | IDA @ 0x1401DE000 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAILY_MISSION | 0x1401DE580 | verified | IDA @ 0x1401DE580 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAILYMAZE_PORTAL | 0x1401DECD0 | verified | IDA @ 0x1401DECD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAY_EVENT | 0x1401DF8D0 | verified | IDA @ 0x1401DF8D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DAY_EVENT_BOOSTER | 0x1401E0200 | verified | IDA @ 0x1401E0200 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DECK_BONUS | 0x1401E0940 | verified | IDA @ 0x1401E0940 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DEFENSIVE_WEAPON | 0x1401E0C90 | verified | IDA @ 0x1401E0C90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DISASSEMBLE | 0x1401E0F00 | verified | IDA @ 0x1401E0F00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DISTRICT | 0x1401E17C0 | verified | IDA @ 0x1401E17C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DISTRICT_TRANSPORT | 0x1401E1CE0 | verified | IDA @ 0x1401E1CE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DIVERGENCE | 0x1401E21D0 | verified | IDA @ 0x1401E21D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DIVISIONRANKREWARD | 0x1401E26C0 | verified | IDA @ 0x1401E26C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROP | 0x1401E2A90 | verified | IDA @ 0x1401E2A90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROP_GROUP | 0x1401E4600 | verified | IDA @ 0x1401E4600 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROP_GROUP_CHARACTER | 0x1401E5120 | verified | IDA @ 0x1401E5120 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROPRATE_LEVEL | 0x1401E55A0 | verified | IDA @ 0x1401E55A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROPRATE_MEMBER | 0x1401E5830 | verified | IDA @ 0x1401E5830 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DROPRATE_MOB | 0x1401E5AC0 | verified | IDA @ 0x1401E5AC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DS_POINT | 0x1401E5D50 | verified | IDA @ 0x1401E5D50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DYE | 0x1401E6650 | verified | IDA @ 0x1401E6650 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_DYE_INFO | 0x1401E6930 | verified | IDA @ 0x1401E6930 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ECHELON | 0x1401E7350 | verified | IDA @ 0x1401E7350 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_EQUALIZER_INFO | 0x1401E76D0 | verified | IDA @ 0x1401E76D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_EVENT_CONDITION | 0x1401E7900 | verified | IDA @ 0x1401E7900 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_FACTION | 0x1401E7C40 | verified | IDA @ 0x1401E7C40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_FRAGMENT_EXCHANGE | 0x1401E7F40 | verified | IDA @ 0x1401E7F40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_FRAGMENT_EXTRACTION | 0x1401E85F0 | verified | IDA @ 0x1401E85F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_GACHA_GROUP | 0x1401E9430 | verified | IDA @ 0x1401E9430 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_GEAR_RATE | 0x1401E9BA0 | verified | IDA @ 0x1401E9BA0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_GESTURE | 0x1401E9E30 | verified | IDA @ 0x1401E9E30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_HELPER | 0x1401EA240 | verified | IDA @ 0x1401EA240 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_HELPER_REWARD | 0x1401EA4C0 | verified | IDA @ 0x1401EA4C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_HIDDEN_EVENT | 0x1401EA8F0 | verified | IDA @ 0x1401EA8F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INFINITE_TOWER | 0x1401EAE20 | verified | IDA @ 0x1401EAE20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INTERACTION_ITEM | 0x1401EB590 | verified | IDA @ 0x1401EB590 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INTERACTION_OBJECT | 0x1401EC100 | verified | IDA @ 0x1401EC100 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_INVEN_SLOT_EXTEND | 0x1401EC9A0 | verified | IDA @ 0x1401EC9A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM | 0x1401ED0F0 | verified | IDA @ 0x1401ED0F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_CLASSIFY | 0x1401EE0F0 | verified | IDA @ 0x1401EE0F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_CLASSREWARD_LINK | 0x1401EE660 | verified | IDA @ 0x1401EE660 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_COSTUMESET | 0x1401EEA90 | verified | IDA @ 0x1401EEA90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_COUNTBOX | 0x1401EF0F0 | verified | IDA @ 0x1401EF0F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_COUNTBOX_GROUP | 0x1401EFA30 | verified | IDA @ 0x1401EFA30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_ENDURANCE | 0x1401F0300 | verified | IDA @ 0x1401F0300 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_EVOLUTION | 0x1401F0810 | verified | IDA @ 0x1401F0810 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_EXTRACTION | 0x1401F0E90 | verified | IDA @ 0x1401F0E90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_LIMIT | 0x1401F1100 | verified | IDA @ 0x1401F1100 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_PACKAGE | 0x1401F1380 | verified | IDA @ 0x1401F1380 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_RANDOMBOX | 0x1401F2030 | verified | IDA @ 0x1401F2030 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_RANK_RATE | 0x1401F3040 | verified | IDA @ 0x1401F3040 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_REPAIR | 0x1401F32D0 | verified | IDA @ 0x1401F32D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_SCRIPT | 0x1401F39E0 | verified | IDA @ 0x1401F39E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_SETITEM | 0x1401F3BC0 | verified | IDA @ 0x1401F3BC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_SIMILARGROUP | 0x1401F5210 | verified | IDA @ 0x1401F5210 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE | 0x1401F55A0 | verified | IDA @ 0x1401F55A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE_CHANGE | 0x1401F5C30 | verified | IDA @ 0x1401F5C30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE_GROUP | 0x1401F5F80 | verified | IDA @ 0x1401F5F80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ITEM_TITLE_VALUE | 0x1401F6610 | verified | IDA @ 0x1401F6610 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEAGUE_INFO | 0x1401F6A40 | verified | IDA @ 0x1401F6A40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEAGUE_SKILL | 0x1401F6E10 | verified | IDA @ 0x1401F6E10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEVEL_MAIL | 0x1401F7280 | verified | IDA @ 0x1401F7280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_LEVELUP_POINT | 0x1401F79F0 | verified | IDA @ 0x1401F79F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAKE | 0x1401F7D30 | verified | IDA @ 0x1401F7D30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_ENTER_COUNT_GROUP | 0x1401F8420 | verified | IDA @ 0x1401F8420 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_INFO | 0x1401F88E0 | verified | IDA @ 0x1401F88E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_OPEN_GROUP | 0x1401F9700 | verified | IDA @ 0x1401F9700 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZE_OPENCONTROL | 0x1401F9BC0 | verified | IDA @ 0x1401F9BC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_DIFFICULTY | 0x1401F9E70 | verified | IDA @ 0x1401F9E70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_ITEM | 0x1401FA140 | verified | IDA @ 0x1401FA140 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_LEVEL | 0x1401FAA10 | verified | IDA @ 0x1401FAA10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_NORMAL | 0x1401FACE0 | verified | IDA @ 0x1401FACE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_PARTYVALUE | 0x1401FB0E0 | verified | IDA @ 0x1401FB0E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_RANK | 0x1401FB5B0 | verified | IDA @ 0x1401FB5B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_REVISION | 0x1401FB990 | verified | IDA @ 0x1401FB990 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_SOULVAPER | 0x1401FBC70 | verified | IDA @ 0x1401FBC70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_STANDARD | 0x1401FBF30 | verified | IDA @ 0x1401FBF30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MAZEREWARD_TIME | 0x1401FC4C0 | verified | IDA @ 0x1401FC4C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MILEAGE | 0x1401FC7E0 | verified | IDA @ 0x1401FC7E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_CARDMATCH_CARD | 0x1401FCB00 | verified | IDA @ 0x1401FCB00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_CARDMATCH_RULE | 0x1401FCD30 | verified | IDA @ 0x1401FCD30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_CLASS_CORRECTION | 0x1401FD000 | verified | IDA @ 0x1401FD000 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_CLASS_STARTSKILL | 0x1401FD5C0 | verified | IDA @ 0x1401FD5C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_REWARD | 0x1401FDB40 | verified | IDA @ 0x1401FDB40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_SECTOR_INFO | 0x1401FDE50 | verified | IDA @ 0x1401FDE50 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_SKILL_EDIT | 0x1401FE400 | verified | IDA @ 0x1401FE400 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_UPGRADE | 0x1401FE670 | verified | IDA @ 0x1401FE670 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_BI_UPGRADE_GROUP | 0x1401FEA40 | verified | IDA @ 0x1401FEA40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_DEFENCE | 0x140200030 | verified | IDA @ 0x140200030 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_DISTRICT6 | 0x140200670 | verified | IDA @ 0x140200670 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_DISTRICT6_DATE | 0x140200F80 | verified | IDA @ 0x140200F80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_OPERATION | 0x1402015F0 | verified | IDA @ 0x1402015F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MODE_SURVIVAL | 0x140201DC0 | verified | IDA @ 0x140201DC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER | 0x140202300 | verified | IDA @ 0x140202300 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_BROKEN_PARTS | 0x1402049B0 | verified | IDA @ 0x1402049B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_EXP | 0x140204D00 | verified | IDA @ 0x140204D00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_PARTS | 0x140205130 | verified | IDA @ 0x140205130 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MONSTER_WEAPON | 0x140205310 | verified | IDA @ 0x140205310 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MYROOM_FURNITURE | 0x140205670 | verified | IDA @ 0x140205670 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MYROOM_GREED | 0x140205C70 | verified | IDA @ 0x140205C70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_MYROOM_INFO | 0x1402062E0 | verified | IDA @ 0x1402062E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_NAMEFILTER | 0x140206950 | verified | IDA @ 0x140206950 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_NPC | 0x140206B80 | verified | IDA @ 0x140206B80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_NPC_PARTS | 0x140207310 | verified | IDA @ 0x140207310 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_OPERATION_INFO | 0x140207540 | verified | IDA @ 0x140207540 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_OPTION_STRING | 0x1402085D0 | verified | IDA @ 0x1402085D0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYEXP_LEVEL | 0x140208870 | verified | IDA @ 0x140208870 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYEXP_MEMBER | 0x140208B00 | verified | IDA @ 0x140208B00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYEXP_MOB | 0x140208E00 | verified | IDA @ 0x140208E00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PARTYREVISE | 0x140209090 | verified | IDA @ 0x140209090 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_AKASHIC | 0x140209510 | verified | IDA @ 0x140209510 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_COSTUME | 0x140209780 | verified | IDA @ 0x140209780 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_REWARD_SYSTEM | 0x1402099F0 | verified | IDA @ 0x1402099F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PC_REWARD_SYSTEM_MONTH | 0x140209F30 | verified | IDA @ 0x140209F30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PHOTO_ITEM | 0x14020A460 | verified | IDA @ 0x14020A460 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_POLLEN | 0x14020A7B0 | verified | IDA @ 0x14020A7B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_PROVIDE_ITEM | 0x14020AAF0 | verified | IDA @ 0x14020AAF0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_CHAPTER | 0x14020C240 | verified | IDA @ 0x14020C240 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_CONDITION | 0x14020C7C0 | verified | IDA @ 0x14020C7C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_EPISODE | 0x14020D400 | verified | IDA @ 0x14020D400 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_REWARD | 0x14020F3B0 | verified | IDA @ 0x14020F3B0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_QUEST_SEASON | 0x14020F7E0 | verified | IDA @ 0x14020F7E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_CHANGE | 0x14020FBE0 | verified | IDA @ 0x14020FBE0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_GET | 0x140210010 | verified | IDA @ 0x140210010 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_GET_GROUP | 0x140211A90 | verified | IDA @ 0x140211A90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANDOM_OPTION | 0x140212420 | verified | IDA @ 0x140212420 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_CHANGE | 0x140213B70 | verified | IDA @ 0x140213B70 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_INFO | 0x140213F90 | verified | IDA @ 0x140213F90 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_REWARD | 0x140214390 | verified | IDA @ 0x140214390 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_RANK_REWARD_TOTALPOINT | 0x140214940 | verified | IDA @ 0x140214940 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REINFORCE | 0x140214CD0 | verified | IDA @ 0x140214CD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REINFORCE_EXCHANGE | 0x140216F10 | verified | IDA @ 0x140216F10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REINFORCE_OPTION | 0x140217290 | verified | IDA @ 0x140217290 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_REPACKAGECOSTUME | 0x140217970 | verified | IDA @ 0x140217970 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_ROOMRANK_REWARD | 0x140217E60 | verified | IDA @ 0x140217E60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SECTORQUEST | 0x140218220 | verified | IDA @ 0x140218220 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SELECT_ITEM | 0x140218830 | verified | IDA @ 0x140218830 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SERVERINFO | 0x140219160 | verified | IDA @ 0x140219160 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SHOP | 0x1402193C0 | verified | IDA @ 0x1402193C0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL | 0x140219A60 | verified | IDA @ 0x140219A60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL_ACTION | 0x14021AE20 | verified | IDA @ 0x14021AE20 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL_OPTION | 0x14021B2A0 | verified | IDA @ 0x14021B2A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SKILL_SLOT_EXTEND | 0x14021B580 | verified | IDA @ 0x14021B580 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOCIAL_ITEM | 0x14021B840 | verified | IDA @ 0x14021B840 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOCIAL_MOTION | 0x14021BC30 | verified | IDA @ 0x14021BC30 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOCKET | 0x14021BF00 | verified | IDA @ 0x14021BF00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOUL_GUAGE | 0x14021C530 | verified | IDA @ 0x14021C530 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOUL_METRY | 0x14021C8F0 | verified | IDA @ 0x14021C8F0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SOULSTONE_LEVELUP | 0x14021D090 | verified | IDA @ 0x14021D090 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPAWN_GROUP | 0x14021D600 | verified | IDA @ 0x14021D600 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPAWNBOX_FRONTLINE_01 | 0x14021DAC0 | verified | IDA @ 0x14021DAC0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPAWNBOX_RSQUARE_01 | 0x14021DE00 | verified | IDA @ 0x14021DE00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SPECIALMAZE_OPEN | 0x14021E140 | verified | IDA @ 0x14021E140 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_STATUS | 0x14021E600 | verified | IDA @ 0x14021E600 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SYSTEMMAIL | 0x14021F940 | verified | IDA @ 0x14021F940 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_SYSTEMMAIL_ADD | 0x14021FC10 | verified | IDA @ 0x14021FC10 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TALK | 0x140220210 | verified | IDA @ 0x140220210 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TALK_LIST | 0x140220C80 | verified | IDA @ 0x140220C80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TALK_STRING | 0x140221500 | verified | IDA @ 0x140221500 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TITLE_INFO | 0x1402216E0 | verified | IDA @ 0x1402216E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TITLE_REWARD | 0x140221E60 | verified | IDA @ 0x140221E60 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TITLE_STRING | 0x140222220 | verified | IDA @ 0x140222220 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_TRANSPORT_INFO | 0x140222400 | verified | IDA @ 0x140222400 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_UNITY_EVENT | 0x1402228E0 | verified | IDA @ 0x1402228E0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_VERSION | 0x140223280 | verified | IDA @ 0x140223280 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WARLORD_EVENT | 0x140223580 | verified | IDA @ 0x140223580 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WARLORD_GUI | 0x140223B00 | verified | IDA @ 0x140223B00 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEAPON_RATE | 0x140223DD0 | verified | IDA @ 0x140223DD0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEEK_DAY | 0x140224060 | verified | IDA @ 0x140224060 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEEK_GROUP | 0x1402245A0 | verified | IDA @ 0x1402245A0 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WEEK_MISSION | 0x140224A80 | verified | IDA @ 0x140224A80 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WORLD_EVENT | 0x140224F40 | verified | IDA @ 0x140224F40 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load_Server_TB_WORLD_EVENT_REWARD | 0x140225290 | verified | IDA @ 0x140225290 | 否 | 一致 |
| GameServer/XRelayServer | UserProcess.h | XResourceMgr::Load | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | md5_init | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | md5_append | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | md5_finish | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __security_check_cookie | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __ArrayUnwind | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _onexit | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | atexit | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | mainCRTStartup | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __GSHandlerCheckCommon | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __GSHandlerCheck | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __GSHandlerCheck_EH | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __chkstk | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __report_gsfailure | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __CxxUnhandledExceptionFilter | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __CxxSetUnhandledExceptionFilter | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _RTC_Initialize | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _RTC_Terminate | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _ValidateImageBase | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _FindPESection | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _IsNonwritableInCurrentImage | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _matherr | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | _setargv | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | UserProcess.h | __security_init_cookie | (CRT/runtime) | blocked | CRT/runtime库函数;无需恢复 | 否 | - |
| GameServer/XRelayServer | WorldModeProcess.h | CServerWorldModeProcess::Parse | - | verified | IDA无此函数(二进制无); 源码为GreenDamTan扩展:stub实现仅LogDebug+return true | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.cpp | CLogicThreadProc::OnUpdate` worker 0/1/2 routing skeleton | 0x1400D0660 | verified | IDA 0x1400D0660 + worker 0:PartyManager.Clear+PartyMatchingMgr.OnUpdate+ForceManager.Clear+ForceMatchingMgr.OnUpdate+ModeMazeMatchingMgr.OnUpdate + worker 1:LeagueManager.OnUpdate + worker 2:RecruitManager.OnUpdate | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | TXSingleton<CLogicThreadManager>::TXSingleton<CLogicThreadManager> | 0x140389152 | blocked | IDA无此函数;模板/内联优化 | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::OnFinalizeThread | 0x140391216 | blocked | IDA无此函数;模板/内联优化 | 否 | - |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFriendRecruitManager::IsLoad | 0x140064130 | verified | IDA @ 0x140064130 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CKernelObject::CKernelObject | 0x1400CFD00 | verified | IDA @ 0x1400CFD00 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CKernelObject::~CKernelObject | 0x1400CFD80 | verified | IDA @ 0x1400CFD80 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CWaitableObject::~CWaitableObject | 0x1400CFDE0 | verified | IDA @ 0x1400CFDE0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CWaitableObject::Wait | 0x1400CFE50 | verified | IDA @ 0x1400CFE50 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::CThreadBase | 0x1400CFE80 | verified | IDA @ 0x1400CFE80 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CWaitableObject::CWaitableObject | 0x1400CFF90 | verified | IDA @ 0x1400CFF90 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CKernelEvent::~CKernelEvent | 0x1400CFFC0 | verified | IDA @ 0x1400CFFC0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | THREAD_PROC_ARG::THREAD_PROC_ARG | 0x1400CFFE0 | verified | IDA @ 0x1400CFFE0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | THREAD_PROC_ARG::~THREAD_PROC_ARG | 0x1400D0010 | verified | IDA @ 0x1400D0010 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::~CThreadBase | 0x1400D0030 | verified | IDA @ 0x1400D0030 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::StopThread | 0x1400D00C0 | verified | IDA @ 0x1400D00C0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CThreadBase::CreateThread | 0x1400D00F0 | verified | IDA @ 0x1400D00F0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | _SetThreadName | 0x1400D0180 | verified | IDA @ 0x1400D0180 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::CLogicThreadProc | 0x1400D01C0 | verified | IDA @ 0x1400D01C0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::~CLogicThreadProc | 0x1400D0230 | verified | IDA @ 0x1400D0230 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::AddJob | 0x1400D0280 | verified | IDA @ 0x1400D0280 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::IsShutdown | 0x1400D0300 | verified | IDA @ 0x1400D0300 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::GetCurThreadIndex | 0x1400D0310 | verified | IDA @ 0x1400D0310 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::OnInitializeThread | 0x1400D0330 | verified | IDA @ 0x1400D0330 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::ThreadProc | 0x1400D03C0 | verified | IDA @ 0x1400D03C0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::InitData | 0x1400D0540 | verified | IDA @ 0x1400D0540 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::LoadData | 0x1400D05D0 | verified | IDA @ 0x1400D05D0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::OnUpdate | 0x1400D0660 | verified | IDA @ 0x1400D0660 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::WaitForInit | 0x1400D0720 | verified | IDA @ 0x1400D0720 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadProc::CheckFPS | 0x1400D0750 | verified | IDA @ 0x1400D0750 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::CLogicThreadManager | 0x1400D0950 | verified | IDA @ 0x1400D0950 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::~CLogicThreadManager | 0x1400D0A20 | verified | IDA @ 0x1400D0A20 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::Start | 0x1400D0A50 | verified | IDA @ 0x1400D0A50 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::End | 0x1400D0BE0 | verified | IDA @ 0x1400D0BE0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::DoJob | 0x1400D0CE0 | verified | IDA @ 0x1400D0CE0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::Clear | 0x1400D0E30 | verified | IDA @ 0x1400D0E30 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CLogicThreadManager::CreateWorkerThread | 0x1400D0EB0 | verified | IDA @ 0x1400D0EB0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::~CFThread<CLogicThreadProc> | 0x1400D11D0 | verified | IDA @ 0x1400D11D0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::MakeInstance | 0x1400D1260 | verified | IDA @ 0x1400D1260 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::GetProcInstance | 0x1400D12D0 | verified | IDA @ 0x1400D12D0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::Create | 0x1400D1490 | verified | IDA @ 0x1400D1490 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::CFThread<CLogicThreadProc> | 0x1400D15B0 | verified | IDA @ 0x1400D15B0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::ReleaseInstance | 0x1400D1650 | verified | IDA @ 0x1400D1650 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::RunThread | 0x1400D16B0 | verified | IDA @ 0x1400D16B0 | 否 | 一致 |
| GameServer/XRelayServer/Thread | LogicThreadProcessor.h | CFThread<CLogicThreadProc>::CallThreadHandlerProc | 0x1400D1810 | verified | IDA @ 0x1400D1810 | 否 | 一致 |
