set(LOGINSERVER_RECON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

add_executable(LoginServer
    CharacterProcess.cpp
    GameDBSocket.cpp
    GreenDamTan_XItemFactory.cpp
    LoginControlSocket.cpp
    LoginProcess.cpp
    LoginServer.cpp
    ServerMain.cpp
    SystemProcess.cpp
    User.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XSCommon/Table/DBLoadTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_XServerRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_LogHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_RuntimeFaultLogger.cpp)

target_include_directories(LoginServer PRIVATE ${LOGINSERVER_RECON_ROOT})
target_compile_definitions(LoginServer PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

if (GREENDAMTAN_ODBC_ENABLED)
    target_compile_definitions(LoginServer PRIVATE GREENDAMTAN_HAS_NATIVE_ODBC=1)
    target_link_libraries(LoginServer PRIVATE ${GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE})
endif()

if (WIN32)
    target_link_libraries(LoginServer PRIVATE ws2_32 iphlpapi dbghelp)
endif()

set(_loginserver_enable_ubsan ${ENABLE_UBSAN})
if (_loginserver_enable_ubsan AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(WARNING
        "ENABLE_UBSAN defaults to ON, but compiler '${CMAKE_CXX_COMPILER_ID}' "
        "does not support the current UBSan flag set; disabling UBSan for LoginServer.")
    set(_loginserver_enable_ubsan OFF)
endif()

if (_loginserver_enable_ubsan)
    set(_loginserver_ubsan_mode "${UBSAN_MODE}")
    if (_loginserver_ubsan_mode STREQUAL "auto")
        if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            # The local Windows clang-cl runtime currently links more reliably in trap mode.
            set(_loginserver_ubsan_mode "trap")
        else()
            set(_loginserver_ubsan_mode "runtime")
        endif()
    endif()

    if (NOT _loginserver_ubsan_mode MATCHES "^(runtime|trap)$")
        message(FATAL_ERROR "UBSAN_MODE must be one of: auto, runtime, trap.")
    endif()

    if (MSVC)
        target_compile_options(LoginServer PRIVATE /Oy-)
    else()
        target_compile_options(LoginServer PRIVATE -fno-omit-frame-pointer)
    endif()
    target_compile_options(LoginServer PRIVATE -fsanitize=undefined)
    target_link_options(LoginServer PRIVATE -fsanitize=undefined)
    target_compile_definitions(LoginServer PRIVATE ENABLE_UBSAN=1)

    if (UBSAN_PRINT_STACKTRACE)
        target_compile_definitions(LoginServer PRIVATE UBSAN_PRINT_STACKTRACE=1)
    endif()

    if (_loginserver_ubsan_mode STREQUAL "trap")
        target_compile_options(LoginServer PRIVATE -fsanitize-trap=undefined)
        target_link_options(LoginServer PRIVATE -fsanitize-trap=undefined)
        target_compile_definitions(LoginServer PRIVATE UBSAN_TRAP_MODE=1)
    elseif (WIN32)
        get_filename_component(_loginserver_clang_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        if (EXISTS "${_loginserver_clang_bin_dir}/llvm-symbolizer.exe")
            file(TO_CMAKE_PATH "${_loginserver_clang_bin_dir}/llvm-symbolizer.exe"
                 _loginserver_ubsan_symbolizer_path)
            target_compile_definitions(
                LoginServer
                PRIVATE UBSAN_SYMBOLIZER_PATH="${_loginserver_ubsan_symbolizer_path}")
        endif()
        target_link_libraries(LoginServer PRIVATE shell32)
        message(WARNING
            "Runtime UBSan on the current Windows clang toolchain may still fail to link "
            "because the bundled ubsan runtime pulls in extra custom symbols; "
            "use UBSAN_MODE=trap if that happens.")
    endif()

    message(STATUS "LoginServer UBSan enabled (mode=${_loginserver_ubsan_mode})")
else()
    if (ENABLE_UBSAN)
        message(STATUS
            "LoginServer UBSan disabled because the current compiler/toolchain "
            "does not support the requested UBSan flag set.")
    else()
        message(STATUS "LoginServer UBSan disabled (ENABLE_UBSAN=OFF).")
    endif()
endif ()
