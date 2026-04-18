# RelayServer.exe 当前目标进度

[2026-04-16 23:44]

- 当前目标：`RelayServer.exe`
- 本轮处理文件：
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：0
- 当前阻塞点：
  - 尚未开始本轮 `RelayServer.exe` 的 IDA/PDB 函数级恢复
  - 尚未建立 `RelayServer.exe` 的文件归属与路径恢复首批确认样本
- 下一轮目标：
  - 先读取当前目标的进度/索引台账
  - 再围绕 `RelayServer.exe` 做首轮函数、类型、路径归属梳理

## frontier / backlog 说明

- 当前真正处理的 frontier：
  - 按 `src/docs/promote.md` 为 `RelayServer.exe` 建立标准台账骨架
- 当前只是发现但尚未处理的 backlog：
  - `RelayServer.exe` 的函数恢复
  - `RelayServer.exe` 的类型恢复
  - `RelayServer.exe` 的路径恢复
  - 目标模块的首轮阻塞点梳理
- 当前阶段判断：
  - 这是新目标的初始化建账，不是已经推进到具体恢复 frontier 的函数级工作

[2026-04-17 00:00]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerMain.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerMain.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/CMakeLists.txt`
  - `src/CMakeLists.txt`
- 本轮完成函数数：6
- 当前阻塞点：
  - `XRelayServer::XRelayServer` 的真实成员规模远大于本轮骨架，用户索引/管理器群尚未恢复
  - `CRelayControlSocket::ServerProcessEx` 的 `D/E/F/J` 分支当前只保留了边界和占位函数，未下钻具体业务实现
  - 还未进行本轮构建与 smoke，尚未验证 `RelayServer` 目标能否完整接线
- 下一轮目标：
  - 先构建 `RelayServer` 目标并修正首批接线问题
  - 再继续补首批控制面/启动链缺失类型与函数边界

## frontier / backlog 说明（更新）

- 当前真正处理的 frontier：
  - `ServerMain / ServiceInit`
  - `TXSingleton<XRelayServer>::Instance()` 对应的单例接入点
  - `XRelayServer` 最小启动骨架
  - `CRelayControlSocket::SetMyInfo`
  - `CRelayControlSocket::ServerProcessEx`
- 当前只是发现但尚未处理的 backlog：
  - `LogicThreadProcessor.cpp` 路径锚点对应函数群
  - `XRelayServer` 用户多索引与状态管理
  - `Community / Friend / League / Party / Force / Matching` 深层业务模块
- 当前阶段判断：
  - 本轮属于 bootstrap + control-plane skeleton 第一刀，尚未推进到深层业务恢复

[2026-04-17 00:18]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：4（在既有骨架上向前推进）
- 当前阻塞点：
  - `CRelayControlSocket::ResCreateMatchingMaze / SyncPartyMazeInfo / SyncForceMazeInfo / ResCreateMatchingModeMaze` 当前只恢复到参数解包与逻辑线程调度边界，未进入真实业务对象层
  - `XRelaySocket::OnStartThread` / `CLogicThreadManager` / `LogicThreadProcessor.cpp` 还未建源码承接面
  - 独立 verifier 尚未返回最终结论
- 下一轮目标：
  - 等 verifier 结果回收后继续补 `ServerProcess.cpp` / `LogicThreadProcessor.cpp` 切片
  - 或先对照 IDA 继续推进 `XRelaySocket` 的控制面细节

## frontier / backlog 说明（再更新）

- 当前真正处理的 frontier：
  - `XRelaySocket::OnConnect / SendAddServer / SendUpdateServerInfo`
  - `CRelayControlSocket` 四个 `D/E/F/J` 控制分支的参数解包与最小调度边界
- 当前只是发现但尚未处理的 backlog：
  - `CLogicThreadManager` / `LogicThreadProcessor.cpp` 真实线程模型
  - `ServerProcess.cpp` / `ServerModeMazeProcess.cpp` 承接的具体业务逻辑
  - `XRelayServer` 用户对象、多索引容器、DB/Observe/管理器群
- 当前阶段判断：
  - 已从“纯启动骨架”推进到“控制面参数解包 + 调度边界骨架”，但仍未进入业务层恢复

[2026-04-17 01:57]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSOption.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/CMakeLists.txt`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：11（`ServerProcess / ServerModeMazeProcess` 切片）
- 当前阻塞点：
  - `XRelayServer::AddServerInfo / SetUsersInfo` 目前只恢复到边界与最小骨架，真实容器/索引写入还未下钻
  - `CServerProcess::ReqUpdateServerInfo` 仍只到包体读取边界，`CServer::m_stSyncInfo / m_dwSyncLoad` 的后续语义未恢复
  - `CServerModeMazeProcess::*` 四个处理器当前只恢复到 payload 解包 + `CLogicThreadManager::DoJob` 投递边界，lambda 内业务对象链仍待展开
  - 独立 verifier 尚未执行，当前只完成本地 build 闭环
- 下一轮目标：
  - 继续下钻 `CServer / XRelayServer` 的服务器索引与在线用户同步链
  - 或沿 `Party / Force / ModeMazeMatching` 模块继续把 `DoJob` 后的业务函数承接回来

## frontier / backlog 说明（最新）

- 当前真正处理的 frontier：
  - `ServerProcess.cpp/.h` 的 `CServerProcess::{Parse, ReqCreateServer, ReqUpdateServerInfo, SyncUsersInfo}`
  - `ServerModeMazeProcess.cpp/.h` 的 `CServerModeMazeProcess::{Parse, ReqServerModeMazeEnter, ReqServerModeMazeExit, ReqServerModeMazeTime_Cheat, ReqServerModeMazeMatchingEvent}`
  - `PSServer.h / PSOption.h` 中与该切片直接相关的 payload/type 落地
  - `RelayServer` 目标重新接线并在 `src/build/RelayServer` 构建通过
- 当前只是发现但尚未处理的 backlog：
  - `AddGameServerInfo / SetUsersInfo` 下游真实容器与状态同步
  - `ServerProcess` 之外的 `PartyProcess / FriendProcess / LeagueProcess / ForceProcess` 独立编译单元
  - `XRelaySocket::OnStartThread` 的真实 IoContext 双缓冲解析循环
  - `ModeMazeMatching / PartyMatching / ForceMatching` 具体对象层与管理器层

[2026-04-17 03:18]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerMain.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserObject.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserPartyInfo.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
  - `src/README.md`
  - `CLAUDE.md`
- 本轮完成函数数：9（`XRelayServer` 索引/缓存链 + verifier 修正）
- 当前阻塞点：
  - `SendCachingLoad` 当前只恢复到缓存状态收口与日志边界，尚未把 `0xF2/0x70` 广播真正接到完整下游发送链
  - `SetUsersInfo` 已落地最小用户/队伍索引写入，但 `AddUser / AddLeagueUser / Friend/Recruit` 真实对象层尚未继续展开
  - 仍需回收新一轮 verifier 结果，确认 `/TEST`、构建说明、以及 `ServerProcessEx` 回退都已闭环
- 下一轮目标：
  - 继续把 `AddUser / GetServer / SendCachingLoad / CServer::SetSyncLoad` 与真实索引语义对齐
  - 或转入 `ModeMazeMatchingMgr / ModeMazeMatching` 这一条由 `CServerModeMazeProcess` 直接承接的对象层切片

## frontier / backlog 说明（再次更新）

- 当前真正处理的 frontier：
  - `XRelaySocket::ServerProcess` 对 `ServerProcessEx` 的回退修正
  - `CServerProcess` 的真实 `default -> false` 分流
  - 线程本地 `CServer` 绑定，避免 `Init(nullptr)` 导致的 process 空 client
  - `XRelayServer::{AddGameServerInfo, SetUsersInfo, SetCachingLoad, UnSetCachingLoad, SendCachingLoad, SendDBGame, GetServer}` 的最小落地
  - `CUserObject / CUserPartyInfo` 承接首批索引对象骨架
  - 文档与构建说明中 `RelayServer` 目标接线纠偏
- 当前只是发现但尚未处理的 backlog：
  - `AddUser / GetUser / GetPartyUser / AddLeagueUser` 的更完整对象链
  - `PartyProcess / ForceProcess / ModeMazeMatchingMgr` 业务层首批承接面
  - `ObserveSocket`、DBAgent 多槽真实接线与跨服状态广播
- 当前阶段判断：
  - 已从“server-process / mode-maze-process 首批承接面”推进到“server-index / caching-load / verifier-fix slice”，但仍未进入 party/force/mode-maze 深层业务对象层

[2026-04-17 04:00-04:29]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserObject.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/CMakeLists.txt`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：13（`CUserProcess + user-index update` 首批承接）
- 当前阻塞点：
  - `CUserProcess::Parse` 目前只承接 `2/3/4/8/0x36/0x38` 这批已直接收束到现有索引接口的子命令，`9/0x11/0x17/0x28/0x29/0x31/0x33/0x34` 仍未继续展开
  - `XRelayServer::UpdateUserMap / RemoveUser / UpdateUserLevelUp / UpdateUserAwaken / UpdateUserProfilePhoto` 仍是最小 map/object 同步，尚未补回 world/league/accountdb/party 侧副作用链
  - `AddPartyUser` / `AddLeagueUser` 仍是最小骨架，距离原版 logic-thread lambda 和 DB 请求链还有缺口
  - 尚未对这轮 `UserProcess` 新文件回收独立 verifier
- 下一轮目标：
  - 继续下钻 `UpdateUserMap` 调用的 world/party/accountdb 副作用链
  - 或补 `SyncLogoutUser -> RemoveUser` 与 `ReqUserOption` 这类已经有清晰证据的用户子流程

## frontier / backlog 说明（UserProcess 更新）

- 当前真正处理的 frontier：
  - `0xF3` 主命令已接回 `CUserProcess`
  - `CUserProcess::{Parse, SyncLoginUser, SyncLogoutUser, SyncUpdateUserMap, SyncUserLevelup, SyncUserAwaken, SyncUserProfilePhoto}`
  - `PS_UPDATE_USER_MAP_INFO` 与其反序列化已落地到 `PSServer.h`
  - `XRelayServer::{UpdateUserMap, RemoveUser, UpdateUserLevelUp, UpdateUserAwaken, UpdateUserProfilePhoto}` 的最小对象索引承接
  - `RelayServer` 目标加入 `UserProcess.cpp` 后再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `ReqUserChatWhisper / ReqUserChatNotice / ReqUserChatMegaPhone / ReqExchangePrice* / ReqNameChange / ReqUserOption / ReqMyRoomPollenSync`
  - `UpdateUserMap` 的 world manager / accountdb / kick packet / old map state 链
  - `RemoveUser` 的 accountdb、party/force matching 与推荐/招募清理链
  - `KickOutUser / SendDBAccount` 以及其他 `CUserProcess` 依赖的基础接口
- 当前阶段判断：
  - 已把 `RelayServer` 从“只有 server/mode-maze 进程骨架”推进到“用户同步主命令开始有真实承接”，但仍属于第一批低风险用户同步切片，尚未进入完整聊天/交易/昵称/世界状态链

[2026-04-17 04:30-04:59]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatching.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：11（`ModeMazeMatching / ModeMazeMatchingMgr` 首批对象层承接）
- 当前阻塞点：
  - `CModeMazeMatching::AutoMatchingCreate` 目前仍是证据受限的最小实现，尚未补回 `XResourceMgr::GetTB_MAZE_INFO / GetOperationInfoTable` 与等待时间计算
  - `CModeMazeMatchingMgr::EnterMatching` 仅落地等待队列收口，尚未补回真正的配队、建房与 `m_mapMatchingInfo` 写入链
  - `CModeMazeMatching::SendCreateMatchingModeMaze` / `CRelayControlSocket::ResCreateMatchingModeMaze` 已接回真实 `ST_CREATE_MODE_MAZE` 包体，但还未补回成员逐个下发、主服选择和 DBLog 链
  - 尚未对这批新文件回收独立 verifier
- 下一轮目标：
  - 继续下钻 `CModeMazeMatching::{MakeOperationMaze, Update, SendCreateMatchingModeMaze}` 真实对象链
  - 或补 `CModeMazeMatchingMgr::{Update, MakeMatchingList, m_mapMatchingInfo 写入}`，把 `EnterMatching -> event/create` 之间的缺口接上

## frontier / backlog 说明（ModeMaze 更新）

- 当前真正处理的 frontier：
  - `PS_SERVER_MODE_MAZE_MATCHING_*` payload 承接到 `ModeMazeMatchingMgr`
  - `ST_CREATE_MODE_MAZE` 从 PDB / export-for-ai 独立落地，并接回 `RelayControlSocket::ResCreateMatchingModeMaze`
  - `CModeMazeMatching::{AutoMatchingCreate, AutoMatchingEnter, ExitMatching, SendMatchingWait, SendMatchingExit, SendCreateMatchingModeMaze}`
  - `CModeMazeMatchingMgr::{EnterMatching, AddModeMazeMatchingWait, ExitMatching, ModeMazeMatchingEvent, SendCreateMatchingModeMaze}`
  - `RelayServer` 目标包含新文件后再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `CModeMazeMatching::MakeOperationMaze` 的真实资源查询 / 发包 / 主服选举链
  - `CModeMazeMatchingMgr` 的状态机、等待时间更新与 `m_mapMatchingInfo` 管理
  - `PartyMatching / ForceMatching` 与 ModeMaze 的横向同构切片
  - `ResCreateMatchingMaze / SyncPartyMazeInfo / SyncForceMazeInfo` 更深对象层
- 当前阶段判断：
  - 已进入 `CServerModeMazeProcess` 后的首批业务对象层，但仍停留在最小证据闭环，尚未进入完整匹配状态机与下游广播链

[2026-04-17 05:00-05:59]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：2（`ReqUserChatNotice + SendChatNotice`）
- 当前阻塞点：
  - `PS_CHAT_NOTICE` 已按当前工程 `wchar_t=4` 环境收口成 PDB 字节布局等价形式，但后续若补更大聊天包仍需继续注意 Win32 原版 UTF-16 与跨平台宽字符尺寸差异
  - `SendChatNotice` 当前通过 `m_xClientPool.GetHead/GetNext + XClient::SendEx` 逐个广播，尚未恢复原版 `SendPacketAll` 包装层
  - `ReqUserChatMegaPhone` 仍缺 `PS_CHAT_MEGAPHONE / PS_CHAT_ITEM_LINK_FOR_SERVER` 正式落地与发送后析构链
  - 本轮独立 verifier 尚未回收
- 下一轮目标：
  - 继续补 `ReqUserChatMegaPhone / SendChatMegaPhone`
  - 或先回收 verifier 结果，再根据失败点继续修正聊天切片

## frontier / backlog 说明（chat notice 更新）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `PS_CHAT_NOTICE` 及其序列化/反序列化
  - `CUserProcess::Parse` 对 `0x11` 的分派恢复
  - `CUserProcess::ReqUserChatNotice`
  - `XRelayServer::SendChatNotice`
  - `RelayServer` 目标再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `PS_CHAT_MEGAPHONE / PS_CHAT_ITEM_LINK_FOR_SERVER`
  - `ReqUserChatMegaPhone / SendChatMegaPhone`
  - `ReqUserChatWhisper / ReqExchangePrice* / ReqNameChange / ReqMyRoomPollenSync`
  - `SendPacketAll` 原版包装层与更完整广播辅助接口
- 当前阶段判断：
  - 已把 `CUserProcess` 从用户状态同步推进到第一条真实聊天广播子命令，但仍属于低风险聊天切片的首刀，未进入 megaphone/item-link/交易等更复杂 payload 链

[2026-04-17 06:00-06:29]

- 本轮处理文件：
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：0（纯证据梳理）
- 当前阻塞点：
  - `PS_CHAT_MEGAPHONE` 与 `PS_CHAT_ITEM_LINK*` 需要先在共享头里补完整依赖，否则无法像 `PS_CHAT_NOTICE` 那样低风险直接落地
  - `PS_CHAT_ITEM_LINK` 依赖 `STItem`、`PS_ITEM_SOCKET_LIST`、`ST_ITEM_BROACH`、`PS_ITEM_PACKAGE`，当前工程内只有 `STItem` 已明确落地
  - `PS_CHAT_ITEM_LINK_FOR_SERVER` 在原版中固定承载 3 个 `PS_CHAT_ITEM_LINK`，反编译和 LoginServer.exe.h 已交叉确认
- 下一轮目标：
  - 先判断 `PS_ITEM_SOCKET_LIST / ST_ITEM_BROACH / PS_ITEM_PACKAGE` 是否能在当前仓内直接复用现有定义或最小补齐
  - 再落 `ReqUserChatMegaPhone / SendChatMegaPhone`

## frontier / backlog 说明（megaphone 预研）

- 当前真正处理的 frontier：
  - `PS_CHAT_MEGAPHONE` 字段与尺寸确认（`0x234`）
  - `PS_CHAT_ITEM_LINK` 字段骨架与尺寸确认（`0x198`）
  - `PS_CHAT_ITEM_LINK_FOR_SERVER` 固定 3 元素容器与析构/序列化链确认
  - `CUserProcess::ReqUserChatMegaPhone` 与 `XRelayServer::SendChatMegaPhone` 调用链确认
- 当前只是发现但尚未处理的 backlog：
  - `PS_CHAT_ITEM_LINK` 依赖类型的共享头落地
  - `operator<< / operator>>` 对 megaphone 与 item-link 的工程实现
  - `SendChatMegaPhone` 真正落地到 `RelayServer.cpp`
- 当前阶段判断：
  - 已确认 megaphone 是下一条低风险聊天切片，但在正式编码前仍需先打通 item-link 依赖面，避免把共享类型层做成半残骨架

[2026-04-17 08:07]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：2（`ReqUserChatMegaPhone + SendChatMegaPhone`）
- 当前阻塞点：
  - `PS_CHAT_MEGAPHONE / PS_CHAT_ITEM_LINK*` 已在共享头落地序列化链，但当前仍未回收本轮独立 verifier
  - `SendChatMegaPhone` 当前仍沿用 `m_xClientPool.GetHead/GetNext + XClient::SendEx` 广播，尚未恢复原版 `SendPacketAll` 包装层
  - `PS_CHAT_ITEM_LINK` 依赖的宽字符串与向量序列化已按现工程可编译闭环落地，后续继续补聊天/交易链时仍需持续注意 Win32 UTF-16 与当前 `wchar_t=4` 的字节布局等价问题
- 下一轮目标：
  - 回收并修正本轮独立 verifier 结果
  - 继续下钻剩余 `CUserProcess` 聊天/交易相关子命令

## frontier / backlog 说明（megaphone 落地）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `PS_CHAT_MEGAPHONE`、`ST_ITEM_SOCKET`、`PS_ITEM_SOCKET_LIST`、`ST_ITEM_BROACH`、`ST_ITEM_PACKAGE_PARTS`、`PS_ITEM_PACKAGE`、`PS_CHAT_ITEM_LINK`、`PS_CHAT_ITEM_LINK_FOR_SERVER` 及其序列化/反序列化
  - `CUserProcess::Parse` 对 `0x17` 的分派恢复
  - `CUserProcess::ReqUserChatMegaPhone`
  - `XRelayServer::SendChatMegaPhone`
  - `RelayServer` 目标在 `src/build/RelayServer` 再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `ReqUserChatWhisper / ReqExchangePrice* / ReqNameChange / ReqMyRoomPollenSync`
  - `SendPacketAll` 原版包装层与更完整广播辅助接口
  - `PS_CHAT_ITEM_LINK` 下游真实析构/所有权语义在更深聊天链中的进一步对齐
- 当前阶段判断：
  - 已把 `CUserProcess` 的第二条真实聊天广播子命令接回工程，并打通 megaphone + item-link 共享 payload 依赖面；当前仍属于低风险聊天广播切片，尚未进入 whisper / 交易 / 更深副作用链

[2026-04-17 09:00]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
- 本轮完成函数数：0（chat serializer 修正）
- 当前阻塞点：
  - `PS_CHAT_NOTICE` 旧实现对固定宽字符数组直接构造 `std::wstring`，在数组满载且未显式 NUL 终止时会突破原版固定字段边界；本轮已改成 `GreenDamTan_BoundedWideString` 按字段上限截断，但仍未回收针对该修正的新 verifier
  - `PS_CHAT_MEGAPHONE / PS_CHAT_ITEM_LINK` 的写包路径也同步切到 bounded wide-string，需与后续聊天分支继续保持同样策略，避免再次引入同类越界序列化问题
  - 下一条候选切片 `ReqUserChatWhisper` 已确认会牵出 `CCommunity` / friend-list 最小承接面，范围明显大于 megaphone
- 下一轮目标：
  - 回收并核对本轮 chat serializer 修正的独立 verifier
  - 若验证通过，再决定是继续落 whisper，还是先补更小的 shared helper / enum / payload 基础面

## frontier / backlog 说明（chat serializer 修正）

- 当前真正处理的 frontier：
  - `PSServer.h` 中固定宽字符聊天包的 bounded serializer 修正
  - `PS_CHAT_NOTICE`、`PS_CHAT_MEGAPHONE`、`PS_CHAT_ITEM_LINK` 的 `operator<<` 统一改为按字段上限截断，而不是依赖 NUL 终止
  - `RelayServer` 目标在 `src/build/RelayServer` 再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `PSCommon.h` 内其他 `std::wstring(value.xxx)` 写包点是否也存在同类 fixed-buffer 风险
  - `ReqUserChatWhisper` 所需的 `PS_CHAT_WHISPER` / option enums / `CUserObject` community wrappers
  - `ReqExchangePrice* / ReqNameChange / ReqMyRoomPollenSync` 对更大共享 payload 面的恢复
- 当前阶段判断：
  - 已把 notice-slice verifier 暴露出的固定宽字符序列化问题在共享层修正；当前仍是 bounded safety fix，不代表 whisper/community 链已经具备可直接落地条件

[2026-04-17 09:24]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSCommon.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserObject.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserProcess.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：3（`PS_CHAT_WHISPER + ReqUserChatWhisper + SendChatWhisper` 最小闭环）
- 当前阻塞点：
  - `PSCommon.h` 中 relay 侧相邻 payload 的 fixed-wide 写包点已全部切到 `FixedWideArrayToWString`，但 chat serializer 专项 verifier 仍在后台执行，尚未回收最终 PASS/FAIL
  - `XRelayServer::SendChatWhisper` 当前只补到最小 sender/receiver 分发与 whisper option gating；`SendDBChatLog`、更细的 sender result packing、以及 login relay 对位细节仍待继续对齐
  - 当前源码 `CUserObject` 仍只有 `SendPacket / CheckGameOption / GetFriendUCID / IsFriendList / IsBlockList` 这组 whisper 所需 query-only 接口，friend/block list push 与 load/sync 标志位尚未继续展开
- 下一轮目标：
  - 先回收 whisper/community 切片 verifier 结果并按 FAIL 点修正
  - 若 verifier 通过，再继续下钻 `SendDBChatLog` 最小落地或 `SendFriendList / SendBlockList` 这一条 community-sync 切片

## frontier / backlog 说明（shared serializer audit + whisper payload）

- 当前真正处理的 frontier：
  - `PSCommon.h` 中 `PS_KICK_USER_INFO`、`PS_LOGIN_RES`、`STBaseCharInfo`、`STLeagueInfo`、`STPrivateShopInfo`、`ST_STATISTICS_CHARACTER_CREATE`、`PS_CHARACTER_REPRESENTATIVE_CHANGE`、`PS_REQ_CHECK_NAME`、`PS_RES_CHECK_NAME` 的 fixed-buffer 写包路径统一改为 bounded helper
  - `PSServer.h` 中 `PS_CHAT_WHISPER` 已按 IDA type layout 与 `ReqUserChatWhisper(1400D7A10)` 的解包顺序落地，含 size/offset assert 与 `operator>>/<<`
  - `CUserProcess::ReqUserChatWhisper`
  - `XRelayServer::SendChatWhisper`
  - 重新构建 `src/build/RelayServer`，确认 shared serializer 审计与 whisper payload 落地未引入新的编译错误
  - 用 IDA type inspect 再次确认 whisper 下一刀的最小类型面：`CCommunity(9 fields)`、`CFriendMember(2 fields)`、`CBlockUser(1 field)`、`ST_FRIEND_COMMUNITY(2 fields)`
- 当前只是发现但尚未处理的 backlog：
  - 独立 verifier 对 chat serializer / `RelayServer.exe /TEST` 的最终结论
  - `CUserObject` 的 whisper 所需 query-only 包装接口与 community 成员落地
  - `ReqUserChatWhisper` 对 friend/block 查询、option gating、DB chat log、login relay `0xF3/0x10` 返回路径的最小闭环
- 当前阶段判断：
  - 已把“聊天包 bounded 修正”扩展到 relay 相邻共享包层，并把 whisper 的共享 payload、用户子命令与最小分发路径接回工程；当前仍处于最小闭环阶段，尚未进入 DBLog 与完整 community-sync 承接

[2026-04-17 10:xx]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserObject.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：4（`SendFriendList / SendBlockList` 与 community-sync payload 最小落地）
- 当前阻塞点：
  - `PS_FRIEND_LIST / PS_BLOCKLIST_INFO` 当前先落在 `UserObject.h` 内联定义与序列化，尚未确认是否需要在后续扩大切片时再上提到共享协议头
  - `CCommunity::GetFriendList / GetBlcokList` 目前按 query-only 容器快照实现，未恢复原版 multi-index 容器与在线好友联动刷新
  - `XRelayServer::SendFriendList / SendBlockList` 当前只补到最小 kickout/log/send/flag 闭环，尚未继续接回调用点与更完整的好友/屏蔽同步链
- 下一轮目标：
  - 先构建 `RelayServer` 并修正本轮 community-sync 落地点
  - 再同步 func/type 台账，并决定是否继续下钻 `SetLoadFriendList / SetLoadBlockList` 的真实调用链

## frontier / backlog 说明（community-sync 首批落地）

- 当前真正处理的 frontier：
  - `UserObject.h` 中 `PS_FRIEND_LIST`、`PS_BLOCKLIST_INFO` 及 `ST_FRIEND_INFO / ST_BLOCK_INFO` 的写包序列化
  - `CCommunity::{GetFriendList, GetBlcokList, SetLoadFriendList, SetLoadBlockList, GetLoadFriendList, GetLoadBlockList, SetSyncFriendList, SetSyncBlockList}`
  - `CUserObject::{GetFriendList, GetBlcokList, SetLoadFriendList, SetLoadBlockList, GetLoadFriendList, GetLoadBlockList, SetSyncFriendList, SetSyncBlockList}`
  - `XRelayServer::{SendFriendList, SendBlockList}`
- 当前只是发现但尚未处理的 backlog：
  - `SendFriendList / SendBlockList` 的上游触发点与 `KickOutUser` 原版闭环
  - community multi-index 真容器、在线好友状态联动、推荐/招募链
  - `SendDBChatLog` 与 whisper 剩余 sender/result parity
- 当前阶段判断：
  - 已把 whisper 之后的下一条 bounded community-sync 切片接回工程，但仍是最小 query-only 承接，不代表原版社交系统容器和刷新链已恢复

[2026-04-17 10:yy]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/FriendProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/FriendProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/CMakeLists.txt`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：5（`CFriendProcess::{Parse, ReqFriendListLoad, ReqBlockListLoad}` + `KickOutUser / SendPacketAll` 最小回补）
- 当前阻塞点：
  - `CFriendProcess::Parse` 当前只接回 `0x01/0x02` 两个 load 子命令，其余 friend 子命令仍先按原版 parser 的 success default 留空
  - `XRelayServer::SendFriendList / SendBlockList` 仍依赖 `GetLoadFriendList / GetLoadBlockList`，因此真实发包是否立即发生仍受后续 load 链恢复程度影响
  - `XRelaySocket::OnParse` 目前只新增了 `0xF5`，`0xF4 / 0xF6 / 0xFA / 0xFB` 仍未恢复对应 process
- 下一轮目标：
  - 先构建 `RelayServer` 并回收这轮 friend-process load slice 的 verifier
  - 若通过，再决定是继续补 friend 子命令，还是转去下一个 main-cmd process 缺口

## frontier / backlog 说明（friend-process load slice）

- 当前真正处理的 frontier：
  - `FriendProcess.h/.cpp` 最小骨架
  - `CFriendProcess` 构造 `SetCmd(0xF5)` / `SetName("CServerFriendProcess")`
  - `CFriendProcess::Parse`
  - `CFriendProcess::{ReqFriendListLoad, ReqBlockListLoad}`
  - `XRelaySocket::OnParse` 对 `0xF5` 的接线
  - `XRelayServer::{KickOutUser, SendPacketAll}` 的最小回补，用于对齐 friend/block list 缺用户时的 kickout 副作用
- 当前只是发现但尚未处理的 backlog：
  - `ReqFriendInvite / ReqFriendAccept / ReqFriendDelete / ReqBlockListAdd / ReqBlockListDelete`
  - `ReqUpdateFriendCommunity`、招募/推荐、helper-support 子流程
  - 其他缺失 main-cmd process：`0xF4 / 0xF6 / 0xFA / 0xFB`
- 当前阶段判断：
  - 已把 `0xF5` friend-process 的最小 load 请求与缺用户 kickout 副作用链接回工程，但仍未进入更深的社交对象变更和 DB/推荐/招募链

[2026-04-17 14:00-14:30]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：3（`CForceManager::{GetForce(UXActorID), EnterServer}` 对齐 + `ReqForceInfo` 状态层回补）
- 当前阻塞点：
  - `CForceManager::EnterServer` 已补回“不自动建 force、进服写 `m_mapForceUser`、仅 `bReqForceInfo` 时发送 `0xFA/0x0A`”这条最小 manager 状态闭环，但原版 master 进服后追加的 `0xF4/0x2E` 招募信息仍未恢复，因为当前工程里还没有 `CPartyMatchingMgr / ST_PARTY_RECRUIT_INFO` 的源码承接面
  - `m_factoryForce` 当前仅按 PDB 字节规模保留 48-byte 占位，尚未恢复真实 `ClassFactory<CForce,64>` 语义
  - `ReqForceInfo` 仍按 `dwForceID -> PS_FORCE_INFO` 最小回包链收口，虽然 `UXActorID` 重载已补回字段面，但调用点尚未切到 actor-based 查询
  - `ReqForceMatchingEnter`、`ForceMatching / ForceMatchingMgr`、以及招募/maze/stateful 链仍未展开
- 下一轮目标：
  - 先回收本轮 `ForceManager` 状态层对齐的 verifier
  - 若通过，再决定是继续补 `CPartyMatchingMgr` / `ST_PARTY_RECRUIT_INFO` 承接面，把 `EnterServer` 的 master 招募回包闭上，还是转入 `ForceMatching` 前的更小 manager/object 切片

## frontier / backlog 说明（force manager state closure）

- 当前真正处理的 frontier：
  - `CForceManager::GetForce(UXActorID)`
  - `CForceManager::EnterServer` 的真实状态差异：不自动建 force、写 `m_mapForceUser`、仅 `bReqForceInfo` 时发 `0xFA/0x0A`
  - `CForceManager` 的首批真实字段面：`m_mapForceUser / m_factoryForce(占位) / m_nRequestNo / m_bLoadForce / m_mapForceInvite`
  - `RelayServer` 目标再次构建通过，并完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `EnterServer` 中 master 进服后的 `0xF4/0x2E` 招募信息补发
  - `ReqForceInfo` 切到 `UXActorID -> m_mapForceUser -> GetForce` 的 actor-based 查询语义
  - `m_factoryForce` 的真实工厂语义
  - `ReqForceMatchingEnter` 与 `PS_SERVER_FORCE_MATCHING_ENTER*`
  - `ForceMatching / ForceMatchingMgr` 横向对象层
[2026-04-17 14:31-15:20]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserPartyInfo.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：8（`CPartyProcess::{ReqPartyRecruitAdd, ReqPartyRecruitDel, ReqPartyRecruitList}` + `CPartyMatchingMgr::{ReqPartyRecruitCreate, ReqPartyRecruitDel, SendPartyRecruitList}` + `CPartyRecruit::{GetApplyCount, IsApplied}`）
- 当前阻塞点：
  - 本轮 `0xF4` 只补到 recruit add/del/list 这一条 bounded request/object 闭环，后续 `0x27/0x28/0x29/0x2C/0x2D/0x2F/0x30/0x32` 仍未展开
  - `ReqPartyRecruitAdd` 目前按已确认 IDA 结构恢复了 `PS_SERVER_PARTY_RECRUIT_ADD_REQ`、基础 result code 与 `SendDBLog` 主干，但 party/force 主体对象存在性校验、max-member 分支、以及 `ClearRecruitParty(1)` 等更细状态副作用仍未继续下钻
  - `SendPartyRecruitList` 现阶段为 bounded `ST_PARTY_RECRUIT_INFO_LIST` 汇总发送面；`my-apply/apply-list` 已在后续切片独立推进，不再属于这里的未实现内容
  - `/TEST` smoke 当时仅确认构建通过；更完整 startup 输出已在后续验证中补回
- 下一轮目标：
  - 优先沿 `0x2C/0x2D` 把 `ReqPartyRecruitMyApplyList / ReqPartyRecruitApplyList` 这组同样只读列表子命令补回，继续复用 `CPartyMatchingMgr` / `CPartyRecruit` 现有骨架
  - 或转而下钻 `ReqPartyRecruitAdd` 的 party/force owner 校验与 `ClearRecruitParty` 细节，把 `0x25` 对齐度再提高一层

## frontier / backlog 说明（party recruit add/del/list slice）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `PS_SERVER_PARTY_RECRUIT_ADD_REQ` 与 `ST_PARTY_RECRUIT_INFO_LIST` 及其序列化/反序列化
  - `CPartyProcess::{Parse, ReqPartyRecruitAdd, ReqPartyRecruitDel, ReqPartyRecruitList}`
  - `CPartyMatchingMgr::{ReqPartyRecruitCreate, ReqPartyRecruitDel, SendPartyRecruitList}`
  - `CPartyRecruit::{GetApplyCount, IsApplied}`
  - `CUserPartyInfo::GetRemainRecruitPenalty`
  - `RelayServer` 目标重新构建通过
- 当前只是发现但尚未处理的 backlog：
  - `ReqPartyRecruitAdd` 内更细的 party/force master/max-count/error parity
  - `ST_PARTY_RECRUIT_APPLY*` payload 的更细 parity 与完整 `0x32` accept-check / party-manager 入队链
  - 历史上属于后续切片的 `ReqPartyRecruitApply / ReqPartyRecruitApplyAccept / ReqPartyRecruitApplyReject / ReqPartyRecruitMyApplyList / ReqPartyRecruitApplyList / ReqPartyRecruitApplyDel / ReqPartyRecruitApplyInfo` 已在后续 2026-04-18 切片中落地，不再属于当前 backlog
- 当前阶段判断：
  - 已把 recruit 前沿从 delete/expiry 继续推进到 add/del/list 三个 `0xF4` 子命令的最小对象层闭环；随后已在后续切片继续向 apply-list 扩展，但该段记录本身只描述 add/del/list 这一刀

[2026-04-17 15:21-15:35]

- 本轮处理文件：
  - `src/docs/RelayServer.exe-current-target-progress.md`
- 本轮完成函数数：0（下一刀证据梳理）
- 当前阻塞点：
  - `ForceMatching` 的最小恢复面已通过 IDA / export-for-ai 重新确认，但尚未开始源码落地
  - `CForceMatchginMember(4 fields)`、`CForceMatching(15 fields)`、`CForceMatchingMgr(2 maps)`、`ST_CREATE_FORCE(3 fields)`、`ST_FORCE_MATCHING_INFO(3 fields)` 与 `ReqForceMatchingEnter(0x140024A40)` / `ExitMatching(0x1400215C0)` / `MatchingRemoveUser(0x140021C90)` / `SendMatchingStart(0x14001D620)` 的函数面已确认
  - verifier 仍未返回最终 verdict
- 下一轮目标：
  - 等 verifier 回收后，优先实现 `ForceMatching / CForceMatchingMgr::MatchingRemoveUser` 的最小对象层切片

## frontier / backlog 说明（next frontier evidence only）

- 当前真正处理的 frontier：
  - 仅确认下一刀边界，不做新代码落地
- 当前只是发现但尚未处理的 backlog：
  - `ForceMatching / ForceMatchingMgr`
  - `ReqForceInfo` actor-based 查询
- 当前阶段判断：
  - 下一刀已明确优先 `ForceMatching`，避免过早切入更宽的 invite/accept/create 状态链

[2026-04-17 15:36-16:10]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/CMakeLists.txt`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：4（`CForceMatchginMember::Clear` + `CForceMatching::{AutoMatchingExit, SendMatchingExit}` + `CForceMatchingMgr::{ExitMatching, MatchingRemoveUser}`）
- 当前阻塞点：
  - 本轮严格停留在 `MatchingRemoveUser` 清理链；`ReqForceMatchingEnter/Exit/Check` 与 `PS_SERVER_FORCE_MATCHING_ENTER* / ST_FORCE_MATCHING_INFO / ST_CREATE_FORCE` 的源码承接仍未开始
  - `CForceMatchingMgr` 当前只恢复到 `m_mpAutoMatching / m_mpAutoMatching_Waiter` 两张表上的退出/移除逻辑，`CreateMatching / EnterMatching / OnUpdate / SendCreateMatchingMaze / ResForceMatchingCreate` 仍缺失
  - `CForceMatching::SendMatchingExit` 当前只补回最小 `0xFA/0x14` 广播、`CUserPartyInfo` 匹配状态清零与均级回写边界，未继续补 DBLog 与完整 `ST_FORCE_MATCHING_INFO` 广播链
  - 独立 verifier 尚未执行，当前只完成本地构建闭环
- 下一轮目标：
  - 回收 verifier 后继续下钻 `ReqForceMatchingExit` / `ReqForceMatchingCheck` 或 `CForceMatchingMgr::EnterMatching`，把 `ForceProcess` 与新对象层接起来

## frontier / backlog 说明（ForceMatching remove-user slice）

- 当前真正处理的 frontier：
  - `ForceMatching.h/.cpp` 新建最小对象层承接 `CForceMatchginMember / CForceMatching / CForceMatchingMgr`
  - `XRelayServer` 挂入 `m_ForceMatchingMgr`
  - `RemovePartyUser` 在 `matchingState == 2` 时改走 `CForceMatchingMgr::MatchingRemoveUser`
  - `RelayServer` 目标加入 `ForceMatching.cpp` 后再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `ReqForceMatchingEnter / ReqForceMatchingExit / ReqForceMatchingCheck`
  - `PS_SERVER_FORCE_MATCHING_ENTER* / ST_FORCE_MATCHING_INFO / ST_CREATE_FORCE` 的共享 payload 落地
  - `CForceMatchingMgr::{CreateMatching, EnterMatching, CheckMatching, OnUpdate, ResForceMatchingCreate, SendCreateMatchingMaze}`
  - `CForceMatching::{AutoMatchingEnter, SendMatchingStart, SendMatchingInfo, SendCreateMatchingMaze, CreateMazeMatching}`
[2026-04-17 16:11-16:45]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：7（`CForceProcess::{ReqForceMatchingExit, ReqForceMatchingCheck}` + `CForceMatching::{AutoMatchingAccept, LeaderSelect, SendMatchingWait}` + `CForceMatchingMgr::CheckMatching` + `ForceProcess::Parse` ForceMatching 分支回补）
- 当前阻塞点：
  - `ReqForceMatchingEnter(0x13)` 与 `CForceMatchingMgr::EnterMatching` 的 party/force group 路径仍未源码承接；这一刀只把 `0x14/0x15` exit/check 链接回已有对象层
  - `CForceMatching::AutoMatchingAccept` 目前只补到 `m_bCheck[]`、leader 选择与 `0xFA/0x17` wait 广播；`SendMatchingCheck / SendMatchingReset / SendMatchingStart` 等后续状态机仍未继续展开
  - `PS_SERVER_FORCE_MATCHING_ENTER` 共享 payload 已通过字段级写包规避前置 operator 可见性问题并重新构建通过，但仍未继续对齐 `ReqForceMatchingEnter -> EnterMatching` 的完整使用链
  - 独立 verifier 已启动，尚未回收最终 verdict
- 下一轮目标：
  - 等 verifier 回收后，优先实现 `ReqForceMatchingEnter` 与 `CForceMatchingMgr::EnterMatching` 的最小源码承接
  - 若 verifier 指出本轮 exit/check 语义缺口，则先按 FAIL 点修正再继续向 enter/create 推进

## frontier / backlog 说明（ForceMatching exit/check bridge）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `PS_SERVER_FORCE_MATCHING_ENTER* / PS_SERVER_FORCE_MATCHING_CHECK / ST_FORCE_MATCHING_INFO / ST_CREATE_FORCE` 的共享 payload 承接与可编译序列化修正
  - `CForceProcess::Parse` 对 `0x14 / 0x15` 的分派恢复
  - `CForceProcess::{ReqForceMatchingExit, ReqForceMatchingCheck}`
  - `CForceMatching::{AutoMatchingAccept, LeaderSelect, SendMatchingWait}`
  - `CForceMatchingMgr::CheckMatching`
  - `RelayServer` 目标再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `ReqForceMatchingEnter`
  - `CForceMatchingMgr::{CreateMatching, EnterMatching, OnUpdate, ResForceMatchingCreate, SendCreateMatchingMaze}`
  - `CForceMatching::{SendMatchingCheck, SendMatchingReset, SendMatchingStart, SendMatchingInfo, SendCreateMatchingMaze, CreateMazeMatching}`
  - force/party group 整体退出时的更完整 manager/container 语义
- 当前阶段判断：
  - 已把 `ForceMatching` 从“remove-user bounded slice”推进到“exit/check process bridge + shared payload compile closure”，但仍未进入 enter/create/start 的更宽状态机

[2026-04-17 16:46-17:20]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：6（`CForceMatching::{SendMatchingCheck, SendMatchingReset, SendMatchingStart, MatchingPossible, MatchingCheck, MatchingWait}`）
- 当前阻塞点：
  - `CForceMatching::SendMatchingStart` 当前只恢复到最小状态切换、人数下界检查与日志边界，`ST_CREATE_FORCE` 发包、主服选择、建迷宫请求与后续成功/失败回调仍未继续承接
  - `MatchingPossible / MatchingCheck / MatchingWait` 目前只把 `reset/check/start` 这段局部状态机接起来，`OnUpdate` 驱动与 `ResForceMatchingCreate` 回调闭环仍缺失
  - `CForceMatchingMgr::CheckMatching` 当前仍直接落到 `AutoMatchingAccept`，尚未按更新循环切到更完整的 waiter/active 容器状态推进
  - 本轮 docs 已同步，但新的独立 verifier 尚未回收
- 下一轮目标：
  - 先回收本轮 state-machine slice 的 verifier
  - 若通过，再继续补 `CForceMatchingMgr::{ResForceMatchingCreate, SendCreateMatchingMaze}` 与 `CForceMatching::{SendCreateMatchingMaze, CreateMazeMatching, OnUpdate}` 的下一刀

## frontier / backlog 说明（ForceMatching local state-machine slice）

- 当前真正处理的 frontier：
  - `CForceMatching::{SendMatchingCheck, SendMatchingReset, SendMatchingStart, MatchingPossible, MatchingCheck, MatchingWait}`
  - `ForceMatching` 内部 `check -> wait -> start / reset / exit` 的局部状态切换
  - `RelayServer` 目标在修复中途编辑损坏后再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - `CForceMatchingMgr::{ResForceMatchingCreate, SendCreateMatchingMaze}`
  - `CForceMatching::{SendCreateMatchingMaze, CreateMazeMatching, OnUpdate}`
  - `ReqForceMatchingEnter` 后半段与 force/party group create 路径的更完整 manager 语义
  - `ST_CREATE_FORCE` 真正下游发送链与 DBLog/控制面回调
- 当前阶段判断：
  - 已把 `ForceMatching` 从 enter/check/exit bridge 继续推进到局部状态机恢复，但还没有接上 create-maze/control-plane 这一层更宽的后续闭环

[2026-04-17 17:21-18:10]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayControlSocket.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：7（`CForceManager::CreateForceMatching`、actor-first `ReqForceInfo` 修正、`CForceMatching::{CreateMazeMatching, SendCreateMatchingMaze}`、`CForceMatchingMgr::{ResForceMatchingCreate, SendCreateMatchingMaze}`、`CRelayControlSocket::ResCreateMatchingMaze` 回调接线）
- 当前阻塞点：
  - 这轮只补了 force create callback 最小闭环；DB 侧 `ResForceMatchingCreate` 主动回调入口与 `OnUpdate` 驱动仍未在当前源码树落地
  - `CForceMatching::SendCreateMatchingMaze` 当前按最小 `0xFA/0x18` 广播 + matching state 清零 + DBLog 收口，尚未继续补更细的 comment/UAID 分支语义
  - `ReqForceInfo` 已改为 actor-first、forceID fallback 的 bounded 修正，但尚未做独立 verifier 复核
  - 本轮独立 verifier 仍在后台，尚未回收 verdict
- 下一轮目标：
  - 先回收 verifier；若 PASS，再决定是否继续补 DB create-result 主动入口或 `OnUpdate` 相邻状态推进
  - 若 verifier FAIL，则先按 FAIL 点修正当前 create callback slice

## frontier / backlog 说明（ForceMatching create callback slice）

- 当前真正处理的 frontier：
  - `CRelayControlSocket::ResCreateMatchingMaze` -> `CForceMatchingMgr::SendCreateMatchingMaze`
  - `CForceMatchingMgr::ResForceMatchingCreate` -> `CForceMatching::CreateMazeMatching`
  - `CForceMatching::CreateMazeMatching` 的最小 `0xF2/0x43` control send
  - `CForceMatching::SendCreateMatchingMaze` 的最小 `0xFA/0x18` fanout
  - `CForceManager::CreateForceMatching` 与 actor-first `ReqForceInfo`
  - `PSServer.h` 中 `PS_FORCE_INFO` 反序列化补齐
  - `RelayServer` 目标再次构建通过
- 当前只是发现但尚未处理的 backlog：
  - DB force-create 完成回调的源码承接面
  - `CForceMatching::OnUpdate`
  - waiter/active 容器的更完整迁移/清理语义
  - `SyncForceMazeInfo` 的后续状态更新链
- 当前阶段判断：
  - 已把 `ForceMatching` 从局部 state-machine 推进到 create-maze/control callback 最小闭环，但仍刻意避免扩成更宽的 DB/update-loop reconstruction

[2026-04-17 18:11-18:55]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：4（`PS_DB_FORCE_MATCHING_CREATE` 序列化、`CForceMatching::SendMatchingStart` verifier-fix、`CForceManager::DeleteForce`、`XRelayServer` accessor 补齐）
- 当前阻塞点：
  - 这轮主要是按 verifier FAIL 修正 `SendMatchingStart` 与 `0xF2/0x43` 发包顺序，不是新的大 slice；DB 主动回调入口仍未落地到 `src/`
  - `CForceMatching::SendMatchingStart` 当前已补最小 DB handoff，但 `DeleteParty/DeleteForce` 仍是 bounded skeleton，不代表 party/force manager 真状态机已恢复
  - `CForceMatching::OnUpdate` 与 `CForceMatchingMgr::OnUpdate` 仍未承接，create 完成后的 manager tick 清理链还没进入当前源码树
  - 旧 verifier 结果已过期；新的独立 verifier 已启动，尚未回收 verdict
- 下一轮目标：
  - 先回收新的 verifier verdict
  - 若 PASS，再继续下钻 DB create-result entrypoint 或 manager `OnUpdate` 最小切片
  - 若 FAIL，则先按新 FAIL 点修正

## frontier / backlog 说明（ForceMatching verifier-fix slice）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `PS_DB_FORCE_MATCHING_CREATE` / `ST_CREATE_FORCE` 的最小可编译位置与 set 序列化
  - `CForceMatching::SendMatchingStart` 的 `PS_DB_FORCE_MATCHING_CREATE + XSendDBPacket(8u, 0xDu)` handoff
  - `CForceMatching::CreateMazeMatching` 的 `matchingID -> createMaze -> party -> force` 发包顺序修正
  - `XRelayServer::{GetForceMatchingMgr, GetPartyManager}` accessor 补齐
  - `CForceManager::DeleteForce` 最小占位与 `RelayServer` 目标再次 build 通过
  - `RelayServer.exe /TEST` 再次 smoke 通过
- 当前只是发现但尚未处理的 backlog：
  - RelayServer DB socket / `ResForceMatchingCreate` 主动入口源码承接
  - `CForceMatching::OnUpdate`
  - `CForceMatchingMgr::OnUpdate`
  - GameServer 侧 `0xF2/0x43` 接收后的更深承接链
- 当前阶段判断：
  - 已把上一个 verifier 指出的 build-break 和 packet-order 问题修掉，并补齐了 `SendMatchingStart` 的最小 DB handoff；当前仍严格停留在 verifier-fix + bounded closure，不算进入新的宽 reconstruction slice

[2026-04-18 00:55-01:20]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：1（`CForceMatching::SendMatchingInfo` verifier-fix）
- 当前阻塞点：
  - corrected verifier 在这轮指出 `SendMatchingInfo` 的均级计算仍按 caller 而不是全队；该问题现已本地修正并重新 build/smoke，但 fresh verifier 尚未回收
  - 当前仍未进入 `OnUpdate` / manager `OnUpdate` 实作；这轮只是 verifier-driven correctness fix
  - DB force-create 主动入口仍未落到 `src/`
- 下一轮目标：
  - 先回收 fresh verifier
  - 若 PASS，再进入 `CForceMatching::OnUpdate` 与 `CForceMatchingMgr::OnUpdate` 的 bounded slice
  - 若 FAIL，则继续按 verifier 指向逐项修正

## frontier / backlog 说明（ForceMatching average-level fix）

- 当前真正处理的 frontier：
  - `CForceMatching::SendMatchingInfo` 的 `m_shAveLevel` 计算修正为按当前 active group 全体成员求均值
  - 修正后 `RelayServer` 目标再次 build 通过
  - 修正后 `RelayServer.exe /TEST` 再次 smoke 通过
- 当前只是发现但尚未处理的 backlog：
  - `CForceMatching::OnUpdate`
  - `CForceMatchingMgr::OnUpdate`
  - RelayServer `GameDBSocket` / `DBForceParse` / `ResForceMatchingCreate` 最小源码承接
- 当前阶段判断：
  - 这轮只是 verifier-driven semantic correction，确保平均等级门槛不再随 caller 漂移；还没有进入新的大切片

[2026-04-18 01:21-02:05]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：4（`CForceMatching::{CheckMazeOpenTime, OnUpdate}` + `CForceMatchingMgr::OnUpdate` + `CLogicThreadProc::OnUpdate`）
- 当前阻塞点：
  - 本轮 `CForceMatching::CheckMazeOpenTime` 只恢复到与原版一致的节流边界，实际 `XResourceMgr::CheckMazeOpenTime(dwMazeID)` 仍未在当前源码树承接；因此当前 `OnUpdate` 先保持最小可编译/可驱动闭环
  - `CLogicThreadProc::OnUpdate` 目前只补回 `m_nIndex==0 -> CForceMatchingMgr::OnUpdate()` 这一条与当前 frontier 直接相关的调度；原版同轮还会继续驱动 `party / mode-maze / league / recruit` 更新链
  - RelayServer `GameDBSocket` / `DBForceParse` / `ResForceMatchingCreate` 主动入口仍未落到 `src/`
  - 新的独立 verifier 尚未回收 verdict
- 下一轮目标：
  - 先回收 fresh verifier
  - 若 PASS，再进入最小 `GameDBSocket` force create-result shim
  - 若 FAIL，则先按 verifier 指向修正这轮 `OnUpdate` slice

## frontier / backlog 说明（ForceMatching OnUpdate slice）

- 当前真正处理的 frontier：
  - `CForceMatching::{CheckMazeOpenTime, OnUpdate}`
  - `CForceMatchingMgr::OnUpdate`
  - `CLogicThreadProc::OnUpdate` 上的 worker-0 routing
  - `RelayServer` 目标再次 build 通过
  - `RelayServer.exe /TEST` 再次 smoke 通过
- 当前只是发现但尚未处理的 backlog：
  - `XResourceMgr::CheckMazeOpenTime(dwMazeID)` 的最小源码承接
  - RelayServer `GameDBSocket` / `DBForceParse` / `ResForceMatchingCreate`
  - `CLogicThreadProc::OnUpdate` 中其余 manager 更新链
- 当前阶段判断：
  - 已把 `ForceMatching` 从 create-callback/average-fix 推进到最小 update-loop 驱动闭环，但仍严格避免把 `XResourceMgr` 和 DB socket 一并扩成更宽切片

[2026-04-18 03:15-03:38]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/GameDBSocket.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/GameDBSocket.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ServerProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/CMakeLists.txt`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：4（`CGameDBSocket::{OnParse, DBParse, DBForceParse, ResForceMatchingCreate}`）
- 当前阻塞点：
  - 当前 `CGameDBSocket::OnParse` 采用 bounded 直通：仅为 `main=8` 绕过 `TXDBSocketT` 的 null-user allowlist，尚未恢复原版 `FindUser -> TXMultiPoolServer<CServer>::FindUser` 会话查询面
  - `XGameDBSocketMgr` 目前只恢复 `GameDB / AccountDB` 两组最小 agent 管理；原版更宽的 Log/Statistics/SGLog 组仍未继续承接
  - `ForceProcess` 的 `0x21` maze-open toggle 与更完整的 resource-side open-control check 仍未落到 `src/`
  - 新一轮独立 verifier 尚未回收 verdict
- 下一轮目标：
  - 先回收 fresh verifier
  - 若 PASS，再按 bounded 路线补 maze-open helper / `0x21` toggle 或 worker-0 其余 manager 更新链
  - 若 FAIL，则先按 verifier 指向修正 `GameDBSocket` shim

## frontier / backlog 说明（Relay DB force-create shim）

- 当前真正处理的 frontier：
  - `CGameDBSocket::OnParse` 对 `main=8` 的 bounded bypass
  - `CGameDBSocket::{DBParse, DBForceParse, ResForceMatchingCreate}`
  - `XGameDBSocketMgr` 的最小 `Init/AutoConnect/DisConnect/Send*DBAgent` 接线
  - `RelayServer` 目标加入 `GameDBSocket.cpp` 后再次 build 通过
  - `RelayServer.exe /TEST` 再次 smoke 通过
- 当前只是发现但尚未处理的 backlog：
  - `FindUser -> TXMultiPoolServer<CServer>` 的真实会话查询面
  - `LogDB / StatisticsDB / SGLogDB` 其余 DB agent 组
  - maze-open helper / `ReqForceChangeMazeOpenCheck(0x21)`
  - worker-0 `PartyMatchingMgr::OnUpdate` / `ModeMazeMatchingMgr::OnUpdate` 的继续恢复
- 当前阶段判断：
  - 已把 RelayServer force-create DB callback 的最小主动入口接回工程，并保持 scope 严格收束在 `main=8/sub=0x0D -> worker-0 lambda -> existing ForceMatchingMgr::ResForceMatchingCreate` 这一路径


[2026-04-18 04:10-04:43]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceMatching.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：4（`CRelayMazeOpenControl::{Init, LoadRows, CheckMazeOpenTime}` + `CForceProcess::ReqForceChangeMazeOpenCheck`）
- 当前阻塞点：
  - 当前 `CRelayMazeOpenControl` 只恢复单表 `TB_MAZE_OPENCONTROL` 的最小 CommonDB 读取与 weekday/time window 判定，没有把 `XResourceMgr` 其余资源表重新拉回 Relay target
  - `ReqForceChangeMazeOpenCheck(0x21)` 现仅恢复为 worker-0 上切换 `m_bCheckMazeOpenTime`，未继续补原版更宽的日志或周边副作用
  - `CForceMatching::CheckMazeOpenTime` 现已切到 relay-local helper，但 `PartyMatchingMgr::OnUpdate` / `ModeMazeMatchingMgr::OnUpdate` 与 worker 1/2 routing 仍未恢复
  - 本轮尚未回收独立 verifier，当前只完成本地 build + `/TEST` smoke
- 下一轮目标：
  - 先回收 verifier；若 PASS，再继续 worker-0 其余 manager 更新链
  - 若 verifier 指出 `CommonDB`/startup 或 `0x21` toggle 缺口，则先按 FAIL 点修正

## frontier / backlog 说明（maze-open helper slice）

- 当前真正处理的 frontier：
  - `RelayServer.h` 新增 `CRelayMazeOpenControl` 最小承接面
  - `RelayServer.cpp` 中 `CRelayMazeOpenControl::{Init, LoadRows, CheckMazeOpenTime}` 的 bounded CommonDB loader
  - `XRelayServer::InitServer` 接入 `GetDNS(2)` 初始化 maze-open 控制表
  - `CForceProcess::Parse` 恢复 `0x21` 分派，`CForceProcess::ReqForceChangeMazeOpenCheck` 接回 worker-0 toggle
  - `CForceMatching::CheckMazeOpenTime` 从本地节流 stub 收口到 relay-local open-control checker
  - `RelayServer` 目标再次 build 通过，并完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `PartyMatchingMgr::OnUpdate`
  - `ModeMazeMatchingMgr::OnUpdate`
  - worker 1 / worker 2 routing
  - `FindUser -> TXMultiPoolServer<CServer>` 的真实 DB session 查询面
- 当前阶段判断：
  - 已按 bounded 路线把 maze-open 检查与 `0x21` toggle 接回 RelayServer，而不重新引入完整 `XResourceMgr`；当前 frontier 仍聚焦 force-matching update 邻接面，而非扩成更宽的资源系统重建


[2026-04-18 05:06-05:42]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatching.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatching.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：9（`CModeMazeMatching::{OnUpdate, MatchingPossible, MatchingWait, MakeOperationMaze}` + `CModeMazeMatchingMgr::{OnUpdate, ProcessWaitList, ProcessMazeMake, DestroyMatchingWait, SetMatchingState}`）
- 当前阻塞点：
  - 本轮 `ModeMazeMatching` 仍保持 bounded 恢复：`SendMatchingWait/SendMatchingExit/SendCreateMatchingModeMaze` 仅接回最小状态推进、control create-maze 请求和 per-member 下发，没有继续补回 `DBLog / CUserPartyInfo::SetMatchingState / reward-state` 全链
  - `ModeMazeMatchingMgr::EnterMatching` 目前只补回 wait-state / timer / member-bucket 推进，没有继续回补原始 `CheckModeMazeOpenTime` 依赖的 `TB_OPERATION_INFO HotTime_*` 与 `PS_MODE_MAZE_MATCHING_ENTER_RES(0xFD/0x01)` 回包分支
  - `m_mapMatchingWait` 仍使用 `std::map` 近似原始 multi-index；当前只保留 actorID 去重 + rank 排序所需语义
  - `PartyMatchingMgr::OnUpdate` 与 worker 1/2 routing 仍未恢复
  - 独立 verifier 尚未针对本轮 mode-maze slice 执行
- 下一轮目标：
  - 先回收 fresh verifier；若 PASS，再继续补 `ModeMazeMatching` 的 client notify / state-reset 缺口，或转入 `PartyMatchingMgr::OnUpdate` 的 bounded active-matching cleanup 半刀

## frontier / backlog 说明（mode-maze update loop slice）

- 当前真正处理的 frontier：
  - `CModeMazeMatching` 补回 `OnUpdate -> MatchingPossible -> SendMatchingWait -> MatchingWait -> MakeOperationMaze` 的最小状态机闭环
  - `CModeMazeMatchingMgr` 补回 `OnUpdate / ProcessWaitList / ProcessMazeMake / DestroyMatchingWait / SetMatchingState`
  - `ProcessWaitList` 先按 `actorID` 去重后的 wait map + rank 升序做 bounded 分桶，并生成 `m_mapMatchingInfo`
  - `MakeOperationMaze` 先按当前匹配成员聚合 `ST_CREATE_MODE_MAZE`，选取 server-count 最大者作为 `dwMasterServerID`，再经 control `0xF2/0x49` 发起 create request
  - `CLogicThreadProc::OnUpdate` 的 worker-0 现已同时驱动 `ForceMatchingMgr::OnUpdate` 与 `CModeMazeMatchingMgr::OnUpdate`
  - `RelayServer` 目标再次 build 通过，并再次完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `PS_MODE_MAZE_MATCHING_ENTER_RES / WAIT / CREATE_MAZE` 客户端通知包体与 DBLog 补全
  - `CheckModeMazeOpenTime` 对 `TB_OPERATION_INFO::HotTime_*` 的原始开窗逻辑
  - `FindModeMazeMatching` / enter duplicate guard / reward-state guard
  - `PartyMatchingMgr::OnUpdate`
  - worker 1 / worker 2 routing
- 当前阶段判断：
  - 这一刀已把 worker-0 上 mode-maze manager 的“等待队列 -> matching 列表 -> create request -> active cleanup”最小更新链接回，但仍明确停留在 bounded skeleton，不继续扩展到 `XResourceMgr` / party-state / DBLog 整体系统


[2026-04-18 06:18-06:47]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：12（`CPartyMatchginMember::Clear` + `CPartyMatching::{OnUpdate, MatchingPossible, MatchingCheck, MatchingWait, SendMatchingCheck, SendMatchingWait, SendMatchingStart, SendMatchingExit, AutoMatchingExit, AutoMatchingAccept, LeaderSelect}` + `CPartyMatchingMgr::OnUpdate`）
- 当前阻塞点：
  - 当前 `CPartyMatchingMgr::OnUpdate` 只恢复 active matching 清理半刀；原始 recruit expiry / `DeletePartyRecruit` / `ST_PARTY_RECRUIT_DEL_LIST(0xF4/0x26)` 广播链仍未接回
  - `CPartyMatching` 当前仍保持 bounded：`SendMatchingWait / SendMatchingCheck / SendMatchingStart / SendMatchingExit` 只补最小 client packet / DB game send / `CUserPartyInfo` 清理，没有继续回补 `XResourceMgr::GetTB_COMMON(0x7532)`、party-state 聚合或完整 DBLog 文案
  - 逻辑线程 worker-0 现已补到 `PartyMatchingMgr -> ForceMatchingMgr -> ModeMazeMatchingMgr`，但原始 `CParty::Clear` 前置与 worker 1/2 的 `LeagueManager / FriendRecruitManager` 仍未恢复
  - 独立 verifier 尚未针对本轮 PartyMatching slice 执行
- 下一轮目标：
  - 先回收 fresh verifier；若 PASS，再决定继续补 `DeletePartyRecruit`/recruit-expiry 半刀，或转入 worker-1/2 routing

## frontier / backlog 说明（party-matching update cleanup slice）

- 当前真正处理的 frontier：
  - `PartyMatchingMgr.h/.cpp` 新建 `CPartyMatchginMember / CPartyMatching` 最小对象层，补回 `OnUpdate` 主状态机与 `MatchingPossible / MatchingCheck / MatchingWait`
  - `SendMatchingCheck(0xF4/0x22)`、`SendMatchingWait(0xF4/0x24)`、`SendMatchingExit(0xF4/0x21)`、`SendMatchingStart(main=4/sub=0x13 DB game send)` 的 bounded 包路恢复
  - `CPartyMatchingMgr::OnUpdate` 现已按原始 `m_mpAutoMatching` 遍历/收集/erase 完成 active matching cleanup
  - `CLogicThreadProc::OnUpdate` 的 worker-0 路由现已补到 `PartyMatchingMgr::OnUpdate`，并保持后续 `ForceMatchingMgr::OnUpdate` 与 `CModeMazeMatchingMgr::OnUpdate`
  - `RelayServer` 目标再次 build 通过，并再次完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `DeletePartyRecruit`
  - recruit expiry + `GetCurDateSec`
  - `ST_PARTY_RECRUIT_DEL_LIST` / `0xF4/0x26`
  - worker 1 / worker 2 routing
  - `CParty::Clear` 前置调用
- 当前阶段判断：
  - 这一刀把 worker-0 上 party-matching 的 active update/erase 链最小接回，但仍明确停留在 bounded cleanup slice，没有继续扩张到 recruit-expiry / party-cache / broader resource dependencies


[2026-04-18 06:48-07:02]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：14（延续 PartyMatching slice，外加 worker-1/2 最小路由占位）
- 当前阻塞点：
  - `CPartyMatchingMgr::OnUpdate` 仍只恢复 active matching cleanup；recruit expiry / `DeletePartyRecruit` / `SendRecruitDelete` 仍未接回
  - worker 1/2 当前只补到 `CLeagueManager::OnUpdate()` / `CFriendRecruitManager::OnUpdate()` 的最小 no-op 承接面，用来把 `CLogicThreadProc::OnUpdate` 路由骨架对齐到 `0x1400D0660`；真实 league/recruit 语义仍缺失
  - `CParty::Clear` 及 `partyManager` 前置清理仍未恢复
  - 本轮 fresh verifier 尚未执行
- 下一轮目标：
  - 先回收 verifier；若 PASS，再决定继续补 party recruit expiry 半刀，或继续把 worker-1/2 的 league / recruit update 面从 no-op 推到更接近原始语义

## frontier / backlog 说明（worker routing parity follow-up）

- 当前真正处理的 frontier：
  - `CPartyMatchingMgr` 最小 active-map `OnUpdate` 恢复
  - `CLogicThreadProc::OnUpdate` 现已接成：worker-0 `PartyMatchingMgr -> ForceMatchingMgr -> ModeMazeMatchingMgr`，worker-1 `CLeagueManager::OnUpdate()`，worker-2 `CFriendRecruitManager::OnUpdate()`
  - `RelayServer.h` 为 worker-1/2 增补最小 `OnUpdate()` 承接面，保持当前 slice 可编译可运行
  - `RelayServer` 目标再次 build 通过，并再次完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `CParty::Clear`
  - `DeletePartyRecruit` / recruit expiry
  - `CLeagueManager::OnUpdate` 真实逻辑
  - `CFriendRecruitManager::OnUpdate` 真实逻辑
- 当前阶段判断：
  - 这一刀主要把 logic-thread routing skeleton 从 worker-0 单点推进到 0/1/2 全分支可编译骨架；worker-1/2 仍是 bounded no-op，不应误认为已恢复真实业务


[2026-04-18 07:03-07:18]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserObject.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
- 本轮完成函数数：1（修正 `CModeMazeMatchingMgr::ModeMazeMatchingEvent` 的 event-member 装配缺口）
- 当前阻塞点：
  - 本轮只修 verifier 命中的 event path 断链：把 `eventInfo.vecInfo` 还原为 `GetUser/GetPartyUser/GetServer` 校验 + `CModeMazeMatchginMember` 装配 + `AutoMatchingEnter` + `CUserPartyInfo::SetMatchingID(0, 3)`；仍未继续回补原始 `FindModeMazeMatching` / `reward-state` / `PS_MODE_MAZE_MATCHING_ENTER_RES(0xFD/0x01)` 回包细节
  - `ModeMazeMatching` 仍保持 bounded，不含 `XResourceMgr::GetOperationInfoTable` / `SendDBLog` / state-reset 全链
  - party recruit expiry / worker-1/2 真实语义仍未恢复
- 下一轮目标：
  - 先回收 mode-maze verifier；若 PASS，再继续下一刀（party recruit expiry 或 worker-1/2 非 no-op 语义）

## frontier / backlog 说明（mode-maze event fanout fix）

- 当前真正处理的 frontier：
  - `ModeMazeMatchingMgr::ModeMazeMatchingEvent` 现在不再只建 event matching 空壳，而是按 `eventInfo.vecInfo` 逐个装配 `CModeMazeMatchginMember`
  - 新增 `CUserObject::{GetMapID, GetClass, GetLevel, GetAwaken, GetProfilePhoto}` 查询接口，供 bounded event-member 填充 `ST_MODE_MAZE_MEMBER_INFO`
  - event path 现已补上 `GetUser / GetPartyUser / GetServer` 校验、`AutoMatchingEnter` 和 `partyUser->SetMatchingID(0, 3u)` 状态推进，避免 verifier 报出的“MemberCount 0 / fanout 空跑”问题
  - `RelayServer` 目标再次 build 通过，并再次完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `PS_MODE_MAZE_MATCHING_ENTER_RES` 细节回包
  - `FindModeMazeMatching` 去重检查
  - `reward-state` 更贴近原始的判定面
  - `SendDBLog` / `XResourceMgr` 依赖

[2026-04-18 07:19-07:42]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：3（补回 `CRelayPartyMatchingConfig::Init`、`XRelayServer::InitServer` 中的 party-wait 初始化接线、`CLogicThreadProc::OnUpdate` 中的两处 `Clear()` parity hook）
- 当前阻塞点：
  - `CRelayPartyMatching::Init` 当前仍是 bounded CommonDB 直查 `tb_Common[30002]`，用于把 `0x7532` 的 wait 秒值折算到 `m_dw64CheckTick`；尚未回补原始 `XRelayServer::m_xResourceMgr` 实体与完整资源装载链
  - `CPartyMatchingMgr::OnUpdate` 仍未继续补 `DeletePartyRecruit` / recruit expiry / `0xF4/0x26` 删除广播
  - worker-1/2 仍是 no-op `OnUpdate` 壳
- 下一轮目标：
  - 先等 party-worker verifier 回收这一刀；若 PASS，再转去 recruit expiry 或 worker-1/2 的下一段真实语义

## frontier / backlog 说明（party worker parity follow-up）

- 当前真正处理的 frontier：
  - `CLogicThreadProc::OnUpdate` 的 worker-0 现在补回了 original `0x1400D0660` 中 party/force 两处 `Clear()` no-op hook，然后再跑 `PartyMatchingMgr / ForceMatchingMgr / ModeMazeMatchingMgr`
  - `CPartyMatching::SendMatchingWait` 已不再写死 `+10000`，而是走 `GetPartyMatchingConfig().GetMatchingWaitMs()`
  - `CRelayPartyMatchingConfig::Init` 现在于 `InitServer` 启动期读取 CommonDB `tb_Common[30002]`（即原始 `GetTB_COMMON(0x7532)` 对应值）并转换为毫秒
  - `RelayServer` 目标已再次 build 通过，并再次完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `XRelayServer::m_xResourceMgr` 全量恢复
  - `DeletePartyRecruit` / recruit-expiry sweep / `PS_RECRUIT_DELETE` 与 `0xF4/0x26`
  - `CLeagueManager::OnUpdate` / `CFriendRecruitManager::OnUpdate` 真实逻辑
- 当前阶段判断：
  - 这一刀只修 verifier 命中的 worker-0 parity 与 wait-time source 偏差，仍保持 bounded helper 设计，不扩大到整条资源系统重建

[2026-04-18 19:42]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserPartyInfo.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：5（`CPartyProcess::{ReqPartyRecruitMyApplyList, ReqPartyRecruitApplyList}` + `CPartyMatchingMgr::{SendPartyRecruitMyApplyList, SendPartyRecruitApplyList}` + `CPartyRecruit::SendApplyUserList`）
- 当前阻塞点：
  - `ReqPartyRecruitMyApplyList` 目前只按 `CUserPartyInfo::m_dwApplyRecruitID[5]` 做 bounded query-only 回包；原版 apply-slot 写入链 `0x27/0x28/0x29` 仍未接回，因此列表内容仍取决于后续 apply state-machine 恢复
  - `ReqPartyRecruitApplyList` 当前只补到 master 校验、`ST_APPLY_MEMBER_LIST(0xF4/0x2D)` 回包与注册时间倒计时换算，未继续恢复 accept/reject/apply-del/info 等相邻子命令
  - 这轮同步修正了 verifier 命中的 recruit penalty parity（`SetRecruitPenalty` + remaining-seconds 语义）和路径台账缺口，但 fresh 独立 verifier 尚未回收 verdict
  - `RelayServer` 构建本轮通过；`/TEST` smoke 与 verifier 仍待回收，之后才能继续向 `0x27/0x28/0x29` 推进
- 下一轮目标：
  - 先回收 fresh verifier；若 PASS，再继续 `ReqPartyRecruitApply / ReqPartyRecruitApplyAccept / ReqPartyRecruitApplyReject` 的 bounded apply state-machine 下一刀
  - 若 verifier 仍报 docs / parity / packet-order 问题，则先按 FAIL 点修正，再继续扩展 recruit 子命令

## frontier / backlog 说明（party recruit apply-list slice）

- 当前真正处理的 frontier：
  - `CPartyProcess::Parse` 对 `0x2C / 0x2D` 的分派恢复
  - `CPartyProcess::{ReqPartyRecruitMyApplyList, ReqPartyRecruitApplyList}`
  - `CPartyMatchingMgr::{SendPartyRecruitMyApplyList, SendPartyRecruitApplyList}`
  - `CPartyRecruit::SendApplyUserList`
  - `CUserPartyInfo::{GetRemainRecruitPenalty, SetRecruitPenalty, SetApplyRecruitID, GetMYApplyRecruitInfo}`
  - `PSServer.h` 中 `ST_PARTY_RECRUIT_LIST / ST_PARTY_MEMBER_LIST / ST_PARTY_RECRUIT_APPLY_INFO` 与相关序列化/反序列化
  - `RelayServer` 目标再次 build 通过
- 当前只是发现但尚未处理的 backlog：
  - `ReqPartyRecruitApply / ReqPartyRecruitApplyAccept / ReqPartyRecruitApplyReject`
  - `ReqPartyRecruitApplyInfo`
  - apply-list 上游的真实 applicant / my-apply 状态写入链
  - recruit apply / accept / reject 与 party/force owner/max-count 的更细 parity
- 当前阶段判断：
  - 这一刀把 party recruit 前沿从 add/del/list 推进到两个只读 list/query 子命令，并顺手修复了 penalty parity 与文档台账缺口；当前仍严格停留在 bounded apply-list slice，还没有进入完整 apply state-machine

[2026-04-18 20:32]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserPartyInfo.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：8（`CPartyProcess::ReqPartyRecruitApply` + `CPartyMatchingMgr::ReqPartyRecruitApply` + `CPartyRecruit::RecruitApply` + `CUserPartyInfo::{GetApplyRecruitCount, IsApplyRecruit, CanApplyRecruit, ClearApplyParty}`）
- 当前阻塞点：
  - `0x27` 当前已按 bounded apply-entry slice 接回，但仍未继续进入 `0x28/0x29` 的 accept/reject 状态机与 `0x32` 受邀者确认回包链
  - `CPartyRecruit::RecruitApply` 现按最小证据恢复了 master 在线、招募未过期、等级区间、重复申请、10 槽容量与 `0xF4/0x31` master 通知；原版里基于同址字段别名访问的奇异布局表达未继续机械复刻
  - `ReqPartyRecruitApply` 当前沿原始结果码发送 `0xF4/0x27`，但仍未补完整 `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT` / `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK` 共享 payload 面
  - 本轮已完成 build + `/TEST` 闭环，fresh 独立 verifier 尚未启动
- 下一轮目标：
  - 先回收当前 `0x27` 的 fresh verifier
  - 若 PASS，再继续 `0x28/0x29`；若 FAIL，则先按 verifier 指向修正当前 apply slice

## frontier / backlog 说明（party recruit apply slice）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `ST_PARTY_RECRUIT_APPLY` 与其序列化/反序列化
  - `CPartyProcess::Parse` 对 `0x27` 的分派恢复
  - `CPartyProcess::ReqPartyRecruitApply`
  - `CPartyMatchingMgr::ReqPartyRecruitApply`
  - `CPartyRecruit::RecruitApply`
  - `CUserPartyInfo::{GetApplyRecruitCount, IsApplyRecruit, CanApplyRecruit, ClearApplyParty}`
  - `RelayServer` 目标再次 build 通过，并完成 `/TEST` smoke
- 当前只是发现但尚未处理的 backlog：
  - `ReqPartyRecruitApplyAccept / ReqPartyRecruitApplyReject`
  - `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT`
  - `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK`
  - `CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}`
  - `CPartyRecruit::{RecruitAccept, GetPartyMemberList}`
- 当前阶段判断：
  - 这一刀已把招募链从 apply-del 继续推进到实际 `apply` 入口，但仍严格停留在 bounded apply slice，还没有进入 accept/reject/accept-check 的更宽闭环

[2026-04-18 20:20]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserPartyInfo.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/RelayServer.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：4（`CPartyProcess::ReqPartyRecruitApplyDel` + `CUserPartyInfo::DelPartyRecruit` + `CPartyRecruit::DelApplyMember` + `XRelayServer::SendPacket`）
- 当前阻塞点：
  - `0x2F` 当前已按最小本地状态写回切片落地，但仍未接 `ST_PARTY_RECRUIT_APPLY` 的正式共享 payload；现阶段继续沿原始已确认的 actorID + recruitID 解包顺序收口
  - `CUserPartyInfo::DelPartyRecruit` 目前按 bounded 语义发送 `0xF4/0x2F` 到 `m_dwServerID`，并在 `bPartySend` 时回写 `CPartyRecruit::DelApplyMember`；原版 `CParty::GetPartyID((CPartyRecruit*)this)` 一类更怪异的布局复用未继续机械照抄
  - `0x28/0x29` 仍依赖 `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT`、`CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}`、`CPartyRecruit::RecruitAccept` 等更宽接受/拒绝状态机，尚未进入本轮
  - 本轮仅完成 build + `/TEST` 闭环，独立 verifier 尚未回收
- 下一轮目标：
  - 先完成 `/TEST` smoke 与独立 verifier
  - 若 PASS，再继续 `0x28/0x29` 的 accept/reject 下一刀；若 FAIL，则先按 verifier 指向修正当前 apply-del slice

## frontier / backlog 说明（party recruit apply-del slice）

- 当前真正处理的 frontier：
  - `CPartyProcess::Parse` 对 `0x2F` 的分派恢复
  - `CPartyProcess::ReqPartyRecruitApplyDel`
  - `CUserPartyInfo::DelPartyRecruit`
  - `CPartyRecruit::DelApplyMember`
  - `XRelayServer::SendPacket` 的 server-targeted 发包 shim
  - `RelayServer` 目标再次 build 通过
- 当前只是发现但尚未处理的 backlog：
  - `ST_PARTY_RECRUIT_APPLY`
  - `ReqPartyRecruitApplyAccept / ReqPartyRecruitApplyReject`
  - `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT`
  - `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK`
  - `CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}`
  - `CPartyRecruit::{RecruitAccept, GetPartyMemberList}`
- 当前阶段判断：
  - 这一刀按既定顺序把招募链从 query-only 推进到最小 apply-slot 删除写回，但仍严格停留在 bounded apply-del slice，还没有进入 accept/reject 的更宽状态机

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserPartyInfo.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceManager.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/ForceProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
- 本轮完成函数数：0（下一刀证据梳理）
- 当前阻塞点：
  - IDA 已确认 `CPartyProcess::Parse(0x1400A1D40)` 在当前招募链紧邻的下一组子命令为 `0x27/0x28/0x29/0x2F/0x30/0x32`；其中 `0x27` 已在后续 20:32 切片完成，这里保留的是当时的边界判断
  - `0x28/0x29` 依赖 `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT`、`CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}`、`CPartyRecruit::{RecruitAccept, DelApplyMember}`；其中 `DelApplyMember` 已在后续切片完成，其余类型/函数当前仍待承接
  - `0x30` 依赖 `ST_PARTY_RECRUIT_APPLY_INFO` 的完整成员列表填充与 `CPartyRecruit::GetPartyMemberList`，而 `0x32` 依赖 `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK` 和受邀者确认回包链；范围明显大于当前 query-only slice
  - `0x2F` 后续已在 20:20 切片中实现，因此这里保留的只是当时的边界判断记录；当前更小的未落地点已经前移到 `0x28/0x29`
- 下一轮目标：
  - 优先实现 `0x2F ReqPartyRecruitApplyDel`，作为 apply-list 上游最小状态写回切片（已在后续切片完成）
  - 随后再进入 `0x27`（已在 20:32 切片完成）、`0x28/0x29`，最后才考虑 `0x30/0x32` 的更宽 accept-check / member-list 回包链

## frontier / backlog 说明（next recruit slice evidence）

- 当前真正处理的 frontier：
  - 仅确认下一刀边界，不做新代码落地
  - 已核实：`0x2F` 是最小下一刀，直接对应 `CUserPartyInfo::DelPartyRecruit`
  - 已核实：`0x28/0x29` 可复用现有 `ForceManager::EnterServer` 的 `0xF4/0x2E` 招募信息发送面、`Force/Party` 成员列表填充模式，以及现有 worker-0 调度模板
- 当前只是发现但尚未处理的 backlog：
  - `ST_PARTY_RECRUIT_APPLY`
  - `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT`
  - `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK`
  - `CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}`
  - `CPartyRecruit::{RecruitAccept, GetPartyMemberList, DelApplyMember, GetPartyGroupType, GetMemberCount}`
  - `CUserPartyInfo::DelPartyRecruit`
- 当前阶段判断：
  - 招募链下一刀不应直接跳去 `0x30/0x32`；最稳妥顺序是 `0x2F -> 0x28/0x29 -> 0x30/0x32`

[2026-04-18 20:45]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/UserObject.h`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：5（`CPartyProcess::{ReqPartyRecruitApplyAccept, ReqPartyRecruitApplyReject}` + `CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}` + `CPartyRecruit::RecruitAccept`）
- 当前阻塞点：
  - `0x28/0x29` 当前只补到 master 侧 accept/reject 入口、manager 校验和 applicant 侧 `0xF4/0x32` accept-check 派发；真正的 `0x32` 受邀确认回包链仍未落到 `src/`
  - `PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK` 当前按 bounded payload 恢复，`ST_PARTY_MEMBER` 中的 channel / HP 字段仍沿用 relay 侧现有 query-only 最小填充
  - 本轮 build + `/TEST` 已在先前切片完成，但 fresh verifier 尚未针对最新 accept/reject 代码与台账同步重新执行
- 下一轮目标：
  - 先回收最新 accept/reject slice 的独立 verifier
  - 若 PASS，再继续 `0x32` accept-check / `ResRecruitAccept` 相邻闭环；若 FAIL，则先按 verifier 指向修正当前 `0x28/0x29` 切片

## frontier / backlog 说明（party recruit accept-reject entry slice）

- 当前真正处理的 frontier：
  - `PSServer.h` 中 `ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT`、`PS_PARTY_ADDMEMBER`、`PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK` 与相关序列化/反序列化
  - `CPartyProcess::Parse` 对 `0x28 / 0x29` 的分派恢复
  - `CPartyProcess::{ReqPartyRecruitApplyAccept, ReqPartyRecruitApplyReject}`
  - `CPartyMatchingMgr::{ReqRecruitAccept, ReqRecruitReject}`
  - `CPartyRecruit::RecruitAccept`
  - `UserObject.h` 中 accept-check 填充所需的最小 query-only getter
- 当前只是发现但尚未处理的 backlog：
  - `0x32` 受邀者确认包的 handler / process 接线
  - `CPartyManager::ResRecruitAccept` 或等价下游承接面
  - `GetPartyMemberList` / `0x30` apply-info member-list 回包链
  - accept 后更完整的 party/force owner/max-count/result-code parity
- 当前阶段判断：
  - 这一刀已把招募链推进到 accept/reject 入口，但仍是 partial closure：现在只有 master 端 request -> manager/recruit -> applicant 端 `0xF4/0x32` 派发闭环，还未继续下钻到受邀确认与真正入队结果链

[2026-04-18 21:05]

- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyProcess.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyMatchingMgr.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer/PartyRecruit.cpp`
  - `src/docs/RelayServer.exe-current-target-progress.md`
  - `src/docs/RelayServer.exe-func-index.md`
  - `src/docs/RelayServer.exe-type-index.md`
  - `src/docs/RelayServer.exe-path-recovery-index.md`
- 本轮完成函数数：4（`CPartyProcess::ReqPartyRecruitApplyInfo` + `CPartyMatchingMgr::SendPartyRecruitApplyInfo` + `CPartyRecruit::GetPartyMemberList` + `CPartyRecruit::{GetPartyGroupType, GetMemberCount}` 内联 query）
- 当前阻塞点：
  - `0x30` 当前按 bounded apply-info slice 恢复为 actor/recruit 双参数 -> worker-0 -> manager query -> `0xF4/0x30` 回包；仍未继续进入 `0x32` applicant confirm 或 party-manager 真正入队链
  - `GetPartyMemberList` 当前沿现有 relay query-only 能力填充 `ST_PARTY_MEMBER_LIST`，其中 channel / HP 仍使用当前工程最小 getter/stub，而不是更宽的 live party state
  - 最新 `0x30` 代码已重新 build 通过，但尚未纳入独立 verifier 的最新范围
- 下一轮目标：
  - 等待 verifier 回收当前招募切片 verdict；若需要则补跑一轮覆盖 `0x30`
  - 并行继续收缩 `0x32 ResPartyRecruitApplyAcceptCheck` 的最小承接边界

## frontier / backlog 说明（party recruit apply-info slice）

- 当前真正处理的 frontier：
  - `CPartyProcess::Parse` 对 `0x30` 的分派恢复
  - `CPartyProcess::ReqPartyRecruitApplyInfo`
  - `CPartyMatchingMgr::SendPartyRecruitApplyInfo`
  - `CPartyRecruit::GetPartyMemberList`
  - `CPartyRecruit::{GetPartyGroupType, GetMemberCount}` query 面补齐
  - `RelayServer` 目标再次 build 通过
- 当前只是发现但尚未处理的 backlog：
  - `0x32 ResPartyRecruitApplyAcceptCheck`
  - applicant confirm 后的 party-manager / add-member 真结果链
  - apply-info 更细的 result-code parity（当前只保持 NULL recruit / member lookup 失败的 bounded 收口）
- 当前阶段判断：
  - 这一刀把招募链从 `0x28/0x29` 的 partial entry 再推进到 `0x30` member-list 查询，但仍严格保持在 bounded query slice，没有越界进入 `0x32` 与更宽的 party manager 系统
