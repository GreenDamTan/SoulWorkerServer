# GameServer.exe 当前目标进度

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

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41835 | 73.7% |
| blocked | 14887 | 26.3% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

### blocked 函数分类

blocked 函数（14887 个）主要是：
- STL 模板实例化（vector, map, set, deque, list, shared_ptr 等）
- CRT/runtime 函数
- 编译器生成函数（向量析构、引用计数等）
- ATL 模板类

### pending 函数说明

pending 函数（41835 个）需要进一步分析：
- 业务逻辑函数
- 游戏服务器核心功能
- 网络/数据库处理
- 需要逐项验证还原

## 下一轮目标

1. 分析 pending 函数中的业务逻辑函数
2. 标记已还原的函数为 verified
3. 开始类型索引建账
4. 开始路径恢复索引建账

## frontier / backlog 说明

- 当前真正处理的 frontier：
  - 初始化 GameServer.exe 四本台账文件（覆盖重建）
- 当前只是发现但尚未处理的 backlog：
  - 41835 个 pending 函数的分析
  - 类型索引建账
  - 路径恢复索引建账
- 当前阶段判断：
  - 本轮仅完成台账初始化，尚未进入第一轮实质恢复
