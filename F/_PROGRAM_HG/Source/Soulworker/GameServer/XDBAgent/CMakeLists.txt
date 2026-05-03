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

# 链接 DBAgentObjects
target_link_libraries(DBAgent PRIVATE DBAgentObjects)

if (EMBED_DEBUG_SYMBOLS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(DBAgent PRIVATE -g -gcodeview)
    elseif (MSVC)
        target_compile_options(DBAgent PRIVATE /Z7)
    endif()
endif()
