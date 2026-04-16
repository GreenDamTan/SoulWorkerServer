cmake_minimum_required(VERSION 3.20)
project(LoginServerReconstruction LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_UBSAN "Enable UBSan instrumentation when the current toolchain supports it" ON)
set(UBSAN_MODE "auto" CACHE STRING "UBSan mode: auto, runtime, or trap")
set_property(CACHE UBSAN_MODE PROPERTY STRINGS auto runtime trap)
option(UBSAN_PRINT_STACKTRACE "Request stack traces for UBSan-enabled targets" ON)
option(GREENDAMTAN_ENABLE_NATIVE_ODBC "Enable native ODBC-backed DB wrappers when driver headers/libs are available" ON)

set(GREENDAMTAN_ODBC_LIBRARIES "" CACHE STRING "Optional semicolon-separated ODBC libraries to link when GREENDAMTAN_ENABLE_NATIVE_ODBC=ON")

if(GREENDAMTAN_ENABLE_NATIVE_ODBC)
    if(WIN32)
        set(_greendamtan_default_odbc_libs odbc32)
    else()
        set(_greendamtan_default_odbc_libs odbc)
    endif()

    set(GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE "${GREENDAMTAN_ODBC_LIBRARIES}")
    if(NOT GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE)
        set(GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE "${_greendamtan_default_odbc_libs}")
    endif()

    set(GREENDAMTAN_ODBC_ENABLED TRUE)
else()
    set(GREENDAMTAN_ODBC_ENABLED FALSE)
    set(GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE "")
endif()

add_subdirectory(F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer)
