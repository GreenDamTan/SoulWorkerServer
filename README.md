# 工程说明

本工程为 SoulWorker (即：首尔工人) 的服务端实现

对应服务端版本为： 
```ini
[Server] 
revision=293162
```

工作的客户端版本为`1.11.13.6`

本目录为服务端源码工程目录，使用 `CMake` 作为构建入口。

## 目录结构

- `CMakeLists.txt`
  - 顶层构建入口。
- `F/_PROGRAM_HG/Source/Soulworker/`
  - 工程源码目录。

## 推荐环境

推荐在 Windows 下使用以下工具链进行构建：

- `CMake 3.20+`
- `Ninja`
- `LLVM/Clang`
  - `clang-cl`
  - `llvm-mt.exe`
- 可用的 Windows SDK / MSVC 兼容环境

## 配置与编译

当前已配置的 CMake 目标包括 `LoginServer`、`RelayServer`、`GameServer` 与 `ControlServer`。

建议始终在当前目录执行配置、编译和运行，并统一使用 `build/` 目录保存构建产物。

### 配置

在当前目录执行：

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe"
```

说明：

- `GREENDAMTAN_ENABLE_NATIVE_ODBC` 当前默认是 `ON`
- 如果需要退回旧的 stub 路径，可显式追加：

```powershell
-DGREENDAMTAN_ENABLE_NATIVE_ODBC=OFF
```

如果当前 Windows `clang-cl` 工具链遇到 UBSan 运行时链接问题，可改用 trap 模式或直接关闭 UBSan：

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe" `
  -DUBSAN_MODE=trap
```

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe" `
  -DENABLE_UBSAN=OFF
```

### 编译

配置完成后执行：

```powershell
cmake --build build --target LoginServer
cmake --build build --target RelayServer
cmake --build build --target GameServer
cmake --build build --target ControlServer
```

如果当前环境下并行编译触发 LLVM 内存问题，可使用串行构建作为稳定回退：

```powershell
cmake --build build --target LoginServer -- -j1
```

## 输出目录

构建产物统一输出到：

```text
build/bin/
build/lib/
```

当前已接入目标的可执行文件位于：

```text
build/bin/LoginServer.exe
build/bin/RelayServer.exe
build/bin/GameServer.exe
build/bin/ControlServer.exe
```

## 运行方式

建议从当前目录启动生成的可执行文件，以保持相对路径资源访问一致。

例如：

```powershell
& "build/bin/LoginServer.exe"
& "build/bin/RelayServer.exe"
```

运行时通常会依赖当前目录下的以下资源：

- `config/`
- `Log/`
- `res/`
- `table.res`

## 测试 / 校验

当前 CMake 中**没有配置测试目标或 lint 目标**：

- 没有 `enable_testing()` / `add_test()` / `ctest` 接线
- 没有发现独立 lint 脚本

因此当前仓库的标准校验方式是：

1. 重新构建目标
2. 必要时从当前目录运行可执行文件
3. 检查生成日志

常用 smoke 命令例如：

```powershell
$env:GREENDAMTAN_AUTOSTOP_MS=5000
& "build/bin/LoginServer.exe"
```

RelayServer 一次性启动 smoke 可直接使用 `/TEST`：

```powershell
& "build/bin/RelayServer.exe" /TEST
```

## 日志

不同模块的日志会写到当前工作目录下各自对应的日志目录中。

当前常用日志位置例如：

- `Log/Login/System.log`
- `Log/Login/Game.log`

建议在调试前先确认：

- 构建输出目录存在
- 日志目录具备写权限
- 运行目录下所需配置和资源文件完整

## 注意事项

- 工程路径层级较深，建议直接在当前目录进行配置、编译和运行。
- 如修改了 `CMakeLists.txt`、切换了工具链，或需要调整 UBSan / ODBC 配置，建议重新执行一次 `cmake -S . -B build ...`。
- 若更换编译器、SDK 或生成器，建议使用新的构建目录，避免旧缓存干扰。
- 若运行时没有生成日志，优先检查：
  - 可执行文件是否从正确工作目录启动
  - 配置文件是否可访问
  - 依赖资源是否齐全
- 文档文件建议统一使用 UTF-8 编码保存，避免中文显示异常。

## 常用命令

重新配置：

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe"
```

重新编译：

```powershell
cmake --build build --target LoginServer
cmake --build build --target RelayServer
```

串行重编译：

```powershell
cmake --build build --target LoginServer -- -j1
```

5 秒 smoke：

```powershell
$env:GREENDAMTAN_AUTOSTOP_MS=5000
& "build/bin/LoginServer.exe"
```

清理后重配时，建议直接删除旧的构建目录后重新执行配置命令。
