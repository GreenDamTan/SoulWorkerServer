#pragma once

#ifdef _WIN32
struct _EXCEPTION_POINTERS;
using GreenDamTan_UnhandledExceptionFilter = long (__stdcall*)(_EXCEPTION_POINTERS*);
#else
using GreenDamTan_UnhandledExceptionFilter = void*;
#endif

// 统一封装当前重建版进程级 fault 日志与 CRT hook，
// 让控制台入口、服务入口以及后续其它服务都能复用同一套诊断安装链。
class GreenDamTan_RuntimeFaultLogger {
public:
    using UnhandledExceptionFilter = GreenDamTan_UnhandledExceptionFilter;

    // 安装 terminate / signal / CRT report 等进程级诊断 hook。
    static void Install();
    // 在日志线程就绪后补打一条阶段日志，确认 hook 已经进入可落盘状态。
    static void LogReady(const char* stage);
    // 安装顶层未处理异常过滤器，并返回旧过滤器供外层恢复。
    static UnhandledExceptionFilter InstallUnhandledExceptionFilter();
    // 在服务停机或进程退出前恢复旧过滤器，避免把全局 SEH 状态永久改掉。
    static void RestoreUnhandledExceptionFilter(UnhandledExceptionFilter previousFilter);
};
