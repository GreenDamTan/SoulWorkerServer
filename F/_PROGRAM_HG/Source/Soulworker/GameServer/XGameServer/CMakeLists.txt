set(GAMESERVER_RECON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

add_executable(GameServer
    BattleZone.cpp
    GameServer.cpp
    XItemFactory.cpp
    GameSockets.cpp
    GameDBSocket.cpp
    GameDBSocketMgr.cpp
    User.cpp
    # Mover.cpp - disabled, using actor/Mover/Mover.cpp instead
    MoverEx.cpp
    MoverLinkStubs.cpp
    MoverCollision.cpp
    Physics.cpp
    Monster.cpp
    Monster_Extended.cpp
    # Npc.cpp - TODO: Fix incomplete type errors (TB_NPC, ThreadLocalData, XArea, SetSocketActive)
    Ai.cpp
    MySkillList.cpp
    RespawnManager.cpp
    VaccumManager.cpp
    VaccumGroup.cpp
    VaccumCube.cpp
    VaccumCubeProcess.cpp
    ServerMain.cpp
    Xigncode.cpp
    Achieve.cpp
    AchieveType.cpp
    ActionResMgr.cpp
    WorldManager.cpp
    GroupAggro.cpp
    GameWorldMode.cpp
    DBOperations.cpp
    Party.cpp
    CParty.cpp
    CForce.cpp
    XPartyManager.cpp
    XForceManager.cpp
    TraceHPState.cpp
    Maze.cpp
    CellPosMgr.cpp
    Timer.cpp
    ModeMaze.cpp
    ModeMazeProcess.cpp
    Sector.cpp
    # StatusEffect.cpp - TODO: Fix compilation errors (missing members, wrong includes)
    # SectorRaid.cpp - TODO: Fix missing CSector method declarations and type definitions
    # MazeRaid.cpp - TODO: Fix missing types, methods and dependencies (STWarp, VEventObjectInfo, m_textDBLog, etc.)
    CutsceneManager.cpp
    XMonsterMgr.cpp
    # ThreadLocalData.cpp - TODO: Fix incomplete type dependencies
    ThreadLocalData_Stub.cpp
    Process/CharacterProcess.cpp
    Process/SystemProcess.cpp
    Process/LoginProcess.cpp
    Process/MonsterProcess.cpp
    Process/ItemProcess.cpp
    actor/component/gocattribute.cpp
    actor/component/gocskill.cpp
    actor/component/gocinventory.cpp
    actor/component/XBaseInventory.cpp
    actor/component/gocentity.cpp
    actor/component/gocevent.cpp
    actor/component/gocquest.cpp
    actor/component/gocpost.cpp
    actor/component/gocfriend.cpp
    actor/component/gocachieve.cpp
    actor/component/gocbooster.cpp
    actor/component/GocNetwork.cpp
    actor/component/GocForce.cpp
    actor/component/GocLeague.cpp
    actor/component/GocDailyMission.cpp
    actor/component/GocRecode.cpp
    actor/component/GocNpcAttribute.cpp
    # actor/component/GocMyRoom.cpp - TODO: Fix many compilation errors
    # actor/component/GocHelper.cpp - TODO: Fix GetGOC calls, UpdateStat, Finalize issues
    actor/component/CFriend.cpp
    PvPSystem.cpp
    actor/component/Skill.cpp
    actor/component/GocAkashicRecord.cpp
    UtilFunc.cpp
    Item/CItem.cpp
    SocialItemObject.cpp
    HiddenEvent.cpp
    LogicThreadManager_Stub.cpp
    # ManagerStubs.cpp - removed, causes header conflicts with XRelayServer
    # CBlockUser is defined in UserObject.h (XRelayServer)
    actor/Mover/Mover.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XSCommon/Table/DBLoadTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_XServerRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_LogHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XServer/GreenDamTan_RuntimeFaultLogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/XArea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/XDistrict.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/XMyRoom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/DohHavokNavMeshInstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/DohHavokResourceManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/HavokNavMeshResource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/Range2DScanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/XArea/XActor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/VisionEngineTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../XCore/HavokTypes.cpp
    )

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
