# GameServer.exe 当前目标进度

---

[2026-05-26 13:30 +08:00] [glm-5]

## 修复编译错误并成功构建 GameServer

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/VisionEngineTypes.h` - 添加缺失类型定义
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/XArea.cpp` - 新建
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/XDistrict.cpp` - 新建
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/ServerMain.cpp` - 新建
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/CMakeLists.txt` - 更新
- 本轮完成操作：
  - 从 IDA 获取 VPList (48 bytes)、VTypedObject (32 bytes)、CWayPoint、CMySkillList 类型信息
  - 添加 Vision Engine 类型 stub: VPList, VTypedObject, CWayPoint, CMySkillList
  - 添加用户相关结构: PS_TICKCOUNT_INFO, ST_CHECK_POS
  - 修复 XVec3 重定义冲突（移除别名，使用 PSCommon.h 中的定义）
  - 实现 XArea 基类方法: 构造/析构/EnterActor/ExitActor/FindActor
  - 实现 XDistrict 基类方法: 构造/析构/AddWaitForRecvInfo/RemoveWaitForRecvInfo
  - 添加 GameServer main 函数入口
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41835 | 73.7% |
| blocked | 14887 | 26.3% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

## 已还原的类型/结构

| 类型名 | 大小 | 来源 | 说明 |
|--------|------|------|------|
| VPList | 48 bytes | IDA get_struct_info | Vision Engine 指针列表 |
| VTypedObject | 32 bytes | IDA get_struct_info | Vision Engine 类型对象基类 |
| CWayPoint | ~40 bytes | IDA decompile | Waypoint 导航系统 |
| CMySkillList | 复杂 | IDA decompile | 技能管理器（前置声明） |
| PS_TICKCOUNT_INFO | 12 bytes | 推测 | Tick 计数信息 |
| ST_CHECK_POS | 20 bytes | 推测 | 位置检查结构 |

## 下一轮目标

1. 继续从 IDA 反编译核心函数
2. 实现 CMover、CMoverEx 关键方法
3. 实现 CUser 关键方法
4. 更新文档记录还原进度

## frontier / backlog 说明

- 当前真正处理的 frontier：
  - GameServer 编译通过，继续还原核心函数
- 当前只是发现但尚未处理的 backlog：
  - 41835 个 pending 函数的分析
  - 类型索引建账
  - 路径恢复索引建账

---

[2026-05-26 13:00 +08:00] [glm-5]

## 初始化 GameServer.exe 台账（覆盖重建）

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/docs/GameServer.exe-current-target-progress.md`
  - `src/docs/GameServer.exe-func-index.md`
  - `src/docs/GameServer.exe-type-index.md`
  - `src/docs/GameServer.exe-path-recovery-index.md`
- 本轮完成操作：
  - 从 IDA 导出 56722 个函数到 `GameServer.exe-func-dump.json`
  - 覆盖生成 `func-index.md` 台账文件
  - 覆盖生成空的 `type-index.md` 和 `path-recovery-index.md`
