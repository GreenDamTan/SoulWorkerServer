set(GAMESERVER_RECON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

add_executable(GameServer
    BattleZone.cpp
    GameServer.cpp
    User.cpp
    Mover.cpp
    MoverEx.cpp
    RespawnManager.cpp
    VaccumManager.cpp
    ServerMain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XSCommon/Table/DBLoadTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_XServerRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_LogHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_RuntimeFaultLogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/XArea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/XDistrict.cpp)

target_include_directories(GameServer PRIVATE ${GAMESERVER_RECON_ROOT})
target_compile_definitions(GameServer PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

if (EMBED_DEBUG_SYMBOLS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(GameServer PRIVATE -g -gcodeview)
    elseif (MSVC)
        target_compile_options(GameServer PRIVATE /Z7)
    endif()
    message(STATUS "GameServer: Debug symbols enabled for ${CMAKE_CXX_COMPILER_ID}")
endif()

if (GREENDAMTAN_ODBC_ENABLED)
    target_compile_definitions(GameServer PRIVATE GREENDAMTAN_HAS_NATIVE_ODBC=1)
    target_link_libraries(GameServer PRIVATE ${GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE})
endif()

if (WIN32)
    target_link_libraries(GameServer PRIVATE ws2_32 iphlpapi dbghelp)
endif()

set(_gameserver_enable_ubsan ${ENABLE_UBSAN})
if (_gameserver_enable_ubsan AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(WARNING
        "ENABLE_UBSAN defaults to ON, but compiler '${CMAKE_CXX_COMPILER_ID}' "
        "does not support the current UBSan flag set; disabling UBSan for GameServer.")
    set(_gameserver_enable_ubsan OFF)
endif()

if (_gameserver_enable_ubsan)
    set(_gameserver_ubsan_mode "${UBSAN_MODE}")
    if (_gameserver_ubsan_mode STREQUAL "auto")
        if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(_gameserver_ubsan_mode "trap")
        else()
            set(_gameserver_ubsan_mode "runtime")
        endif()
    endif()

    if (NOT _gameserver_ubsan_mode MATCHES "^(runtime|trap)$")
        message(FATAL_ERROR "UBSAN_MODE must be one of: auto, runtime, trap.")
    endif()

    if (MSVC)
        target_compile_options(GameServer PRIVATE /Oy-)
    else()
        target_compile_options(GameServer PRIVATE -fno-omit-frame-pointer)
    endif()
    target_compile_options(GameServer PRIVATE -fsanitize=undefined)
    target_link_options(GameServer PRIVATE -fsanitize=undefined)
    target_compile_definitions(GameServer PRIVATE ENABLE_UBSAN=1)

    if (UBSAN_PRINT_STACKTRACE)
        target_compile_definitions(GameServer PRIVATE UBSAN_PRINT_STACKTRACE=1)
    endif()

    if (_gameserver_ubsan_mode STREQUAL "trap")
        target_compile_options(GameServer PRIVATE -fsanitize-trap=undefined)
        target_link_options(GameServer PRIVATE -fsanitize-trap=undefined)
        target_compile_definitions(GameServer PRIVATE UBSAN_TRAP_MODE=1)
    elseif (WIN32)
        get_filename_component(_gameserver_clang_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        if (EXISTS "${_gameserver_clang_bin_dir}/llvm-symbolizer.exe")
            file(TO_CMAKE_PATH "${_gameserver_clang_bin_dir}/llvm-symbolizer.exe"
                 _gameserver_ubsan_symbolizer_path)
            target_compile_definitions(
                GameServer
                PRIVATE UBSAN_SYMBOLIZER_PATH="${_gameserver_ubsan_symbolizer_path}")
        endif()
        target_link_libraries(GameServer PRIVATE shell32)
        message(WARNING
            "Runtime UBSan on the current Windows clang toolchain may still fail to link "
            "because the bundled ubsan runtime pulls in extra custom symbols; "
            "use UBSAN_MODE=trap if that happens.")
    endif()

    message(STATUS "GameServer UBSan enabled (mode=${_gameserver_ubsan_mode})")
else()
    if (ENABLE_UBSAN)
        message(STATUS
            "GameServer UBSan disabled because the current compiler/toolchain "
            "does not support the requested UBSan flag set.")
    else()
        message(STATUS "GameServer UBSan disabled (ENABLE_UBSAN=OFF).")
    endif()
endif ()
