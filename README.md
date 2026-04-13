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

在仓库根目录执行：

```powershell
cmake -S src -B build/recon-loginserver-clangcl -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe"
```

然后执行构建：

```powershell
cmake --build build/recon-loginserver-clangcl --target LoginServer
```

如果后续工程中增加了新的可执行目标或库目标，可按同样方式指定对应 `target` 进行编译。

## 输出目录

构建产物默认输出到：

```text
build/recon-loginserver-clangcl/
```

当前已接入目标的可执行文件位于类似如下目录：

```text
build/recon-loginserver-clangcl/F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer/
```

## 运行方式

建议从仓库根目录启动生成的可执行文件，以保持相对路径资源访问一致。

例如：

```powershell
& "build/recon-loginserver-clangcl/F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer/LoginServer.exe"
```

运行时通常会依赖仓库根目录下的以下资源：

- `config/`
- `Log/`
- `res/`
- `table.res`

## 日志

不同模块的日志会写到各自对应的日志目录中。

当前已接入目标的常用日志位置例如：

- `build/recon-loginserver-clangcl/F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer/Log/Login/System.log`
- `build/recon-loginserver-clangcl/F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer/Log/Login/Game.log`

建议在调试前先确认：

- 构建输出目录存在
- 日志目录具备写权限
- 运行目录下所需配置和资源文件完整

## 注意事项

- 工程路径层级较深，建议直接在仓库根目录进行配置、编译和运行。
- 如修改了 `CMakeLists.txt` 或新增源码文件，建议先重新执行一次 `cmake -S src -B build/recon-loginserver-clangcl ...`，再执行构建。
- 若更换编译器、SDK 或生成器，建议使用新的构建目录，避免旧缓存干扰。
- 若运行时没有生成日志，优先检查：
  - 可执行文件是否从正确工作目录启动
  - 配置文件是否可访问
  - 依赖资源是否齐全
- 文档文件建议统一使用 UTF-8 编码保存，避免中文显示异常。

## 常用命令

重新配置：

```powershell
cmake -S src -B build/recon-loginserver-clangcl -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe"
```

重新编译：

```powershell
cmake --build build/recon-loginserver-clangcl --target LoginServer
```

清理后重配时，建议直接删除旧的构建目录后重新执行配置命令。
