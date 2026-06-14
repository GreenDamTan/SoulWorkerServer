set(CONTROLSERVER_RECON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

add_executable(ControlServer
    ControlServer.cpp
    CMyRoom.cpp
    MazeInfo.cpp
    UserObject.cpp
    ChannelManager.cpp
    ServerMain.cpp
    ServerProcess.cpp
    CServer.cpp
    UserProcess.cpp
    GMToolProcess.cpp
    WorldModeProcess.cpp
    WorldMode.cpp
    WorldModeMgr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XLoginServer/PartyManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XLoginServer/DayEventManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XLoginServer/RouletteEventManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XSCommon/Table/DBLoadTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_XServerRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_LogHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_RuntimeFaultLogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/XGameDBSocketMgr.cpp)

target_include_directories(ControlServer PRIVATE ${CONTROLSERVER_RECON_ROOT})
target_compile_definitions(ControlServer PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

if (EMBED_DEBUG_SYMBOLS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(ControlServer PRIVATE -g -gcodeview)
    elseif (MSVC)
        target_compile_options(ControlServer PRIVATE /Z7)
    endif()
    message(STATUS "ControlServer: Debug symbols enabled for ${CMAKE_CXX_COMPILER_ID}")
endif()

if (GREENDAMTAN_ODBC_ENABLED)
    target_compile_definitions(ControlServer PRIVATE GREENDAMTAN_HAS_NATIVE_ODBC=1)
    target_link_libraries(ControlServer PRIVATE ${GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE})
endif()

if (WIN32)
    target_link_libraries(ControlServer PRIVATE ws2_32 iphlpapi dbghelp)
endif()

# Boost headers are typically available via compiler include paths

set(_controlserver_enable_ubsan ${ENABLE_UBSAN})
if (_controlserver_enable_ubsan AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(WARNING
        "ENABLE_UBSAN defaults to ON, but compiler '${CMAKE_CXX_COMPILER_ID}' "
        "does not support the current UBSan flag set; disabling UBSan for ControlServer.")
    set(_controlserver_enable_ubsan OFF)
endif()

if (_controlserver_enable_ubsan)
    set(_controlserver_ubsan_mode "${UBSAN_MODE}")
    if (_controlserver_ubsan_mode STREQUAL "auto")
        if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(_controlserver_ubsan_mode "trap")
        else()
            set(_controlserver_ubsan_mode "runtime")
        endif()
    endif()

    if (NOT _controlserver_ubsan_mode MATCHES "^(runtime|trap)$")
        message(FATAL_ERROR "UBSAN_MODE must be one of: auto, runtime, trap.")
    endif()

    if (MSVC)
        target_compile_options(ControlServer PRIVATE /Oy-)
    else()
        target_compile_options(ControlServer PRIVATE -fno-omit-frame-pointer)
    endif()
    target_compile_options(ControlServer PRIVATE -fsanitize=undefined)
    target_link_options(ControlServer PRIVATE -fsanitize=undefined)
    target_compile_definitions(ControlServer PRIVATE ENABLE_UBSAN=1)

    if (UBSAN_PRINT_STACKTRACE)
        target_compile_definitions(ControlServer PRIVATE UBSAN_PRINT_STACKTRACE=1)
    endif()

    if (_controlserver_ubsan_mode STREQUAL "trap")
        target_compile_options(ControlServer PRIVATE -fsanitize-trap=undefined)
        target_link_options(ControlServer PRIVATE -fsanitize-trap=undefined)
        target_compile_definitions(ControlServer PRIVATE UBSAN_TRAP_MODE=1)
    elseif (WIN32)
        get_filename_component(_controlserver_clang_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        if (EXISTS "${_controlserver_clang_bin_dir}/llvm-symbolizer.exe")
            file(TO_CMAKE_PATH "${_controlserver_clang_bin_dir}/llvm-symbolizer.exe"
                 _controlserver_ubsan_symbolizer_path)
            target_compile_definitions(
                ControlServer
                PRIVATE UBSAN_SYMBOLIZER_PATH="${_controlserver_ubsan_symbolizer_path}")
        endif()
        target_link_libraries(ControlServer PRIVATE shell32)
    endif()

    message(STATUS "ControlServer UBSan enabled (mode=${_controlserver_ubsan_mode})")
else()
    message(STATUS "ControlServer UBSan disabled")
endif()