set(DBAGENT_RECON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

add_library(DBAgentObjects OBJECT
    DBAgent.cpp
    DBAgentDBManager.cpp
    DBThread.cpp
    LogDB.cpp
    SQLProcess.cpp
    SQLProcessImpl.cpp
    ServerCon.cpp)

target_include_directories(DBAgentObjects PRIVATE ${DBAGENT_RECON_ROOT})
target_compile_definitions(DBAgentObjects PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

if (GREENDAMTAN_ODBC_ENABLED)
    target_compile_definitions(DBAgentObjects PRIVATE GREENDAMTAN_HAS_NATIVE_ODBC=1)
endif()

if (WIN32)
    target_link_libraries(DBAgentObjects PRIVATE ws2_32 iphlpapi dbghelp)
endif()

# DBAgent 可执行目标
add_executable(DBAgent
    ServerMain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_XServerRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_LogHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_RuntimeFaultLogger.cpp)

target_include_directories(DBAgent PRIVATE ${DBAGENT_RECON_ROOT})
target_compile_definitions(DBAgent PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

if (GREENDAMTAN_ODBC_ENABLED)
    target_compile_definitions(DBAgent PRIVATE GREENDAMTAN_HAS_NATIVE_ODBC=1)
    target_link_libraries(DBAgent PRIVATE ${GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE})
endif()

if (WIN32)
    target_link_libraries(DBAgent PRIVATE ws2_32 iphlpapi dbghelp)
endif()

set(_dbagent_enable_ubsan ${ENABLE_UBSAN})
if (_dbagent_enable_ubsan AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(WARNING
        "ENABLE_UBSAN defaults to ON, but compiler '${CMAKE_CXX_COMPILER_ID}' "
        "does not support the current UBSan flag set; disabling UBSan for DBAgent.")
    set(_dbagent_enable_ubsan OFF)
endif()

if (_dbagent_enable_ubsan)
    set(_dbagent_ubsan_mode "${UBSAN_MODE}")
    if (_dbagent_ubsan_mode STREQUAL "auto")
        if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(_dbagent_ubsan_mode "trap")
        else()
            set(_dbagent_ubsan_mode "runtime")
        endif()
    endif()

    if (NOT _dbagent_ubsan_mode MATCHES "^(runtime|trap)$")
        message(FATAL_ERROR "UBSAN_MODE must be one of: auto, runtime, trap.")
    endif()

    if (MSVC)
        target_compile_options(DBAgentObjects PRIVATE /Oy-)
        target_compile_options(DBAgent PRIVATE /Oy-)
    else()
        target_compile_options(DBAgentObjects PRIVATE -fno-omit-frame-pointer)
        target_compile_options(DBAgent PRIVATE -fno-omit-frame-pointer)
    endif()
    target_compile_options(DBAgentObjects PRIVATE -fsanitize=undefined)
    target_compile_options(DBAgent PRIVATE -fsanitize=undefined)
    target_link_options(DBAgent PRIVATE -fsanitize=undefined)
    target_compile_definitions(DBAgentObjects PRIVATE ENABLE_UBSAN=1)
    target_compile_definitions(DBAgent PRIVATE ENABLE_UBSAN=1)

    if (UBSAN_PRINT_STACKTRACE)
        target_compile_definitions(DBAgentObjects PRIVATE UBSAN_PRINT_STACKTRACE=1)
        target_compile_definitions(DBAgent PRIVATE UBSAN_PRINT_STACKTRACE=1)
    endif()

    if (_dbagent_ubsan_mode STREQUAL "trap")
        target_compile_options(DBAgentObjects PRIVATE -fsanitize-trap=undefined)
        target_compile_options(DBAgent PRIVATE -fsanitize-trap=undefined)
        target_link_options(DBAgent PRIVATE -fsanitize-trap=undefined)
        target_compile_definitions(DBAgentObjects PRIVATE UBSAN_TRAP_MODE=1)
        target_compile_definitions(DBAgent PRIVATE UBSAN_TRAP_MODE=1)
    elseif (WIN32)
        get_filename_component(_dbagent_clang_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        if (EXISTS "${_dbagent_clang_bin_dir}/llvm-symbolizer.exe")
            file(TO_CMAKE_PATH "${_dbagent_clang_bin_dir}/llvm-symbolizer.exe"
                 _dbagent_ubsan_symbolizer_path)
            target_compile_definitions(
                DBAgentObjects
                PRIVATE UBSAN_SYMBOLIZER_PATH="${_dbagent_ubsan_symbolizer_path}")
            target_compile_definitions(
                DBAgent
                PRIVATE UBSAN_SYMBOLIZER_PATH="${_dbagent_ubsan_symbolizer_path}")
        endif()
        target_link_libraries(DBAgent PRIVATE shell32)
    endif()

    message(STATUS "DBAgent UBSan enabled (mode=${_dbagent_ubsan_mode})")
else()
    message(STATUS "DBAgent UBSan disabled")
endif()

# 链接 DBAgentObjects
target_link_libraries(DBAgent PRIVATE DBAgentObjects)

if (EMBED_DEBUG_SYMBOLS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(DBAgentObjects PRIVATE -g -gcodeview)
        target_compile_options(DBAgent PRIVATE -g -gcodeview)
    elseif (MSVC)
        target_compile_options(DBAgentObjects PRIVATE /Z7)
        target_compile_options(DBAgent PRIVATE /Z7)
    endif()
    message(STATUS "DBAgent: Debug symbols enabled for ${CMAKE_CXX_COMPILER_ID}")
endif()
