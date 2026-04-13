cmake_minimum_required(VERSION 3.20)
project(LoginServerReconstruction LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_UBSAN "Enable UBSan instrumentation when the current toolchain supports it" ON)
set(UBSAN_MODE "auto" CACHE STRING "UBSan mode: auto, runtime, or trap")
set_property(CACHE UBSAN_MODE PROPERTY STRINGS auto runtime trap)
option(UBSAN_PRINT_STACKTRACE "Request stack traces for UBSan-enabled targets" ON)

add_subdirectory(F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer)
