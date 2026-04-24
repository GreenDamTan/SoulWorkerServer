set(RELAYSERVER_RECON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

add_executable(RelayServer
    Force.cpp
    ForceManager.cpp
    ForceMatching.cpp
    FriendProcess.cpp
    ForceProcess.cpp
    GameDBSocket.cpp
    League.cpp
    LeagueManager.cpp
    LeagueMember.cpp
    LeagueProcess.cpp
    ModeMazeMatching.cpp
    ModeMazeMatchingMgr.cpp
    MonitorProcess.cpp
    ObserveSocket.cpp
    Party.cpp
    PartyManager.cpp
    PartyMatchingMgr.cpp
    PartyProcess.cpp
    PartyRecruit.cpp
    RelayControlSocket.cpp
    RelayServer.cpp
    ServerMain.cpp
    ServerModeMazeProcess.cpp
    ServerProcess.cpp
    UserObject.cpp
    UserProcess.cpp
    WorldModeProcess.cpp
    Thread/LogicThreadProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_XServerRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_LogHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_RuntimeFaultLogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XSCommon/Table/DBLoadTable.cpp)


target_include_directories(RelayServer PRIVATE ${RELAYSERVER_RECON_ROOT})
target_compile_definitions(RelayServer PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

if (GREENDAMTAN_ODBC_ENABLED)
    target_compile_definitions(RelayServer PRIVATE GREENDAMTAN_HAS_NATIVE_ODBC=1)
    target_link_libraries(RelayServer PRIVATE ${GREENDAMTAN_ODBC_LIBRARIES_EFFECTIVE})
endif()

if (WIN32)
    target_link_libraries(RelayServer PRIVATE ws2_32 iphlpapi dbghelp)
endif()
