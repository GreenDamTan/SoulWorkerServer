# GameServer.exe 当前目标进度

[2026-04-26 23:16 +08:00] [glm-5]

- 本轮处理文件：
  - `src/docs/GameServer.exe-current-target-progress.md`
  - `src/docs/GameServer.exe-func-index.md`
  - `src/docs/GameServer.exe-type-index.md`
  - `src/docs/GameServer.exe-path-recovery-index.md`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/IXArea.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/XArea.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/XDistrict.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/GameServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/GameServer.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/BattleZone.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/BattleZone.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/RespawnManager.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/RespawnManager.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/VaccumManager.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/VaccumManager.cpp`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/CMakeLists.txt`
  - `src/CMakeLists.txt` (添加 GameServer 子目录)
- 本轮完成函数数：骨架创建阶段 (0 实现完成)
- 本轮调查内容：
  - **IDA 完整类型布局验证**：
    - CBattleZone: 1104 bytes, 23 成员, 继承 XDistrict
    - XDistrict: 416 bytes, 13 成员, 继承 XArea
    - XArea: 192 bytes, 8 成员, 继承 IXArea
    - CRespawnManager: 72 bytes, 3 成员
    - CVaccumManager: 176 bytes, 7 成员
    - CMoverEx: 60392 bytes, 199 成员, 继承 CMover
    - CMover: 58592 bytes, 139 成员
  - **CUser 构造函数分析** (0x1406E2FA0):
    - 继承: XClient + CMoverEx (多重继承)
    - Vision Engine 多重 vtable 赋值 (8 个接口)
    - 成员初始化顺序完整记录
  - **架构骨架建立**：
    - XArea 基类层次
    - CBattleZone 战斗区域
    - CUser 玩家类 (骨架)
    - CMoverEx 移动实体扩展
- 当前阻塞点：
  - 大量 Vision Engine 类型缺失定义 (VisBaseEntity_cl, XActor 等)
  - 需要引入 Vision SDK 或创建兼容性存根
  - CUser 完整成员布局需要进一步验证
- 下一轮目标：
  - 编译验证骨架
  - 填充更多 IDA 验证的成员类型
  - 创建 Vision Engine 类型兼容层

## frontier / backlog 说明（更新）

- 当前真正处理的 frontier：
  - GameServer.exe 核心架构类骨架创建
  - 类型系统布局验证
- 当前只是发现但尚未处理的 backlog：
  - CUser 完整实现 (1426 函数)
  - CMover/CMoverEx 完整实现
  - CMonster/CNpc/CItem 等业务类
  - Vision Engine 类型兼容层
  - 完整 CMake 构建验证

[2026-04-26 22:50 +08:00]

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/docs/GameServer.exe-current-target-progress.md`
  - `src/docs/GameServer.exe-func-index.md`
  - `src/docs/GameServer.exe-type-index.md`
  - `src/docs/GameServer.exe-path-recovery-index.md`
- 本轮完成函数数：0
- 当前阻塞点：
  - GameServer.exe 为大型目标（56722 函数）
  - 尚未开始首轮 IDA/PDB 函数级恢复
  - 尚未建立文件归属与路径恢复首批确认样本
- 下一轮目标：
  - 先读取当前目标的进度/索引台账
  - 再围绕 `GameServer.exe` 做首轮函数、类型、路径归属梳理

## frontier / backlog 说明

- 当前真正处理的 frontier：
  - 按 `src/docs/promote.md` 为 `GameServer.exe` 建立标准台账骨架
- 当前只是发现但尚未处理的 backlog：
  - `GameServer.exe` 的函数恢复
  - `GameServer.exe` 的类型恢复
  - `GameServer.exe` 的路径恢复
  - 目标模块的首轮阻塞点梳理
- 当前阶段判断：
  - 这是新目标的初始化建账，不是已经推进到具体恢复 frontier 的函数级工作

[2026-04-26 23:00 +08:00] [glm-5]

- 本轮处理文件：
  - `src/docs/GameServer.exe-current-target-progress.md`
  - `src/docs/GameServer.exe-func-index.md`
  - `src/docs/GameServer.exe-type-index.md`
  - `src/docs/GameServer.exe-path-recovery-index.md`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/GameServer.h`
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/GameServer.cpp`
- 本轮完成函数数：0（骨架创建阶段）
- 本轮调查内容：
  - **核心架构类统计**：XGameServer (172 函数), CUser (1426 函数), CBattleZone (161 函数)
  - **XGameServer 构造函数分析**：IDA 0x1402D86D0 显示大量成员初始化
  - **XGameServer::InitServer 分析**：IDA 0x1402D8DE0 完整初始化流程
  - **路径恢复**：从 strings.txt 提取源文件路径
- 当前阻塞点：
  - GameServer.exe 是大型目标（56722 函数），需要分层恢复策略
  - 核心类成员布局需要逐个验证
  - 尚未建立 CMake 构建目标
- 下一轮目标：
  - 创建 CUser/CBattleZone 头文件骨架
  - 建立 GameServer 的 CMake 构建配置
  - 继续从 IDA 验证核心类型布局

## frontier / backlog 说明（更新）

- 当前真正处理的 frontier：
  - XGameServer 核心类骨架创建
  - GameServer 文档索引建立
- 当前只是发现但尚未处理的 backlog：
  - CUser 类恢复（1426 函数）
  - CBattleZone 类恢复（161 函数）
  - CMonster/CNpc/CItem 等业务类
  - CMake 构建系统接线
