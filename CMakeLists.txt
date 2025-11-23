cmake_minimum_required(VERSION 3.24)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(nfs
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
    )

option(NFS_TESTS "Enable / disable tests." ON)
option(NFS_CLIENT "Enable / disable client program." ON)
option(NFS_SERVER "Enable / disable server program." ON)
option(NFS_EDITOR "Enable / disable editor program." ON)
option(NFS_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

# ============================================================================
# CONFIGURACIÓN DE RUTAS
# ============================================================================

set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
set(MAPS_DIR "/usr/local/share/nfs/maps")
set(ASSETS_DIR "/usr/local/share/nfs/assets")
set(LOBBY_ASSETS_DIR "/usr/local/share/nfs/lobby_assets")
set(VEHICLES_SPECS_DIR "/usr/local/share/nfs/vehicles")
set(PHYSICS_LEVELS_DIR "${CMAKE_SOURCE_DIR}/server/physics/Levels")
set(EDITOR_DIR "${CMAKE_SOURCE_DIR}/editor/editor-mapas")

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DMAPS_DIR="${MAPS_DIR}")
add_definitions(-DASSETS_DIR="${ASSETS_DIR}")
add_definitions(-DLOBBY_ASSETS_DIR="${LOBBY_ASSETS_DIR}")
add_definitions(-DVEHICLES_SPECS_DIR="${VEHICLES_SPECS_DIR}")
add_definitions(-DPHYSICS_LEVELS_DIR="${PHYSICS_LEVELS_DIR}")
add_definitions(-DEDITOR_DIR="${EDITOR_DIR}")

message(STATUS "Ruta de mapas: ${MAPS_DIR}")
message(STATUS "Ruta de assets: ${ASSETS_DIR}")
message(STATUS "Ruta de assets del lobby: ${LOBBY_ASSETS_DIR}")
message(STATUS "Ruta de vehículos: ${VEHICLES_SPECS_DIR}")
message(STATUS "Ruta de niveles de física: ${PHYSICS_LEVELS_DIR}")
message(STATUS "Ruta del editor: ${EDITOR_DIR}")

# ============================================================================
# DEPENDENCIAS BÁSICAS
# ============================================================================

find_package(Threads REQUIRED)

# ============================================================================
# YAML-CPP
# ============================================================================

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master
)

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(yaml-cpp)

# ============================================================================
# BIBLIOTECA COMÚN
# ============================================================================

add_library(nfs_common STATIC)
add_subdirectory(common/)

target_link_libraries(nfs_common PUBLIC yaml-cpp Threads::Threads)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(nfs_common ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

target_include_directories(nfs_common PUBLIC .)

# ============================================================================
# DEPENDENCIAS SDL2 (PARA CLIENTE, EDITOR Y SERVIDOR)
# ============================================================================

if(NFS_CLIENT OR NFS_EDITOR OR NFS_SERVER)
    include(FetchContent)

    # SOLO SDL2pp (más rápido)
    set(SDL2PP_WITH_IMAGE YES)
    set(SDL2PP_WITH_MIXER YES) 
    set(SDL2PP_WITH_TTF YES)

    FetchContent_Declare(
        libSDL2pp
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )

    FetchContent_MakeAvailable(libSDL2pp)

    message(STATUS "SDL2pp configurado (incluye SDL2 automáticamente)")
endif()

# ============================================================================
# QT6 (SOLO PARA CLIENTE Y EDITOR)
# ============================================================================

if(NFS_CLIENT OR NFS_EDITOR)
    list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt6")
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)
    
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    
    set(QT_VERSION_MAJOR 6)
    set(Qt${QT_VERSION_MAJOR}_FOUND TRUE)
    set(Qt_FOUND TRUE)
    
    message(STATUS "Qt6 configurado para cliente y editor (compatibilidad habilitada)")
endif()

# ============================================================================
# DEPENDENCIAS PARA SERVIDOR (Box2D)
# ============================================================================

if(NFS_SERVER)
    include(FetchContent)

    FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.1.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(box2d)
    
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_MakeAvailable(nlohmann_json)
    
    message(STATUS "Box2D configurado para servidor")
endif()

# ============================================================================
# PROGRAMA CLIENTE
# ============================================================================

if(NFS_CLIENT)
    add_executable(nfs_client)
    add_dependencies(nfs_client nfs_common SDL2pp::SDL2pp)

    add_subdirectory(client)

    set_project_warnings(nfs_client ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_include_directories(nfs_client PRIVATE ${libSDL2pp_SOURCE_DIR})

    target_link_libraries(nfs_client
        PRIVATE
        nfs_common
        SDL2pp::SDL2pp 
        yaml-cpp
        Qt6::Widgets
        Qt6::Multimedia
    )

    message(STATUS "Cliente configurado")
endif()

# ============================================================================
# PROGRAMA SERVIDOR (CON SDL)
# ============================================================================

if(NFS_SERVER)
    add_executable(nfs_server)
    add_dependencies(nfs_server nfs_common SDL2pp::SDL2pp)

    add_subdirectory(server)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(nfs_server PRIVATE -Wno-error)
    endif()

    set_project_warnings(nfs_server ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_include_directories(nfs_server PRIVATE 
        ${libSDL2pp_SOURCE_DIR}
    )

    target_link_libraries(nfs_server PRIVATE 
        nfs_common
        yaml-cpp 
        box2d
        nlohmann_json::nlohmann_json
        SDL2pp::SDL2pp
    )

    add_executable(physics_sandbox
        server/physics/main.cpp
        server/physics/LevelCreator.cpp
        server/YamlParser.cpp
        server/physics/vehicle.cpp
        server/LevelSetup.cpp
        server/physics/PhysicsEventCollector.cpp
    )
    add_dependencies(physics_sandbox nfs_common SDL2pp::SDL2pp)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(physics_sandbox PRIVATE -Wno-error)
    endif()
    
    target_include_directories(physics_sandbox PRIVATE 
        ${libSDL2pp_SOURCE_DIR}
    )
    
    target_link_libraries(physics_sandbox PRIVATE
        nfs_common
        yaml-cpp
        box2d
        nlohmann_json::nlohmann_json
        SDL2pp::SDL2pp
    )

    message(STATUS "Servidor configurado (con SDL2pp - más rápido)")
endif()

# ============================================================================
# PROGRAMA EDITOR
# ============================================================================

if(NFS_EDITOR)
    add_executable(nfs_editor
        editor/main.cpp
        editor/editor-mapas/mainwindow.cpp
        editor/editor-mapas/mainwindow.h
        editor/editor-mapas/mainwindow.ui
        editor/editor-mapas/mapview.cpp
        editor/editor-mapas/mapview.h
        editor/editor-mapas/yamlhandler.cpp     
        editor/editor-mapas/yamlhandler.h
    )

    add_dependencies(nfs_editor nfs_common SDL2pp::SDL2pp)

    set_project_warnings(nfs_editor ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_include_directories(nfs_editor PRIVATE ${libSDL2pp_SOURCE_DIR})

    target_link_libraries(nfs_editor
        PRIVATE
        nfs_common 
        SDL2pp::SDL2pp 
        yaml-cpp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
    )

    message(STATUS "Editor configurado")
endif()

# ============================================================================
# TESTING SECTION
# ============================================================================

if(NFS_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_subdirectory(tests/)

    set_project_warnings(nfs_tests ${NFS_MAKE_WARNINGS_AS_ERRORS} TRUE)

    target_include_directories(nfs_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
    )

    target_link_libraries(nfs_tests PRIVATE
        nfs_common
        GTest::gtest_main
    )
endif()

# ============================================================================
# INSTALACIÓN
# ============================================================================

install(TARGETS 
    nfs_server 
    nfs_client 
    nfs_editor 
    physics_sandbox
    RUNTIME DESTINATION bin
)

if(NFS_TESTS)
    install(TARGETS nfs_tests RUNTIME DESTINATION bin)
endif()

install(DIRECTORY server/maps/ DESTINATION share/nfs/maps)
install(DIRECTORY client/assets/need-for-speed/ DESTINATION share/nfs/assets)
install(DIRECTORY client/lobby/assets/ DESTINATION share/nfs/lobby_assets)
install(DIRECTORY server/vehicles_specs/ DESTINATION share/nfs/vehicles)
install(DIRECTORY client/resources/static/ DESTINATION share/nfs/resources)

# ============================================================================
# RESUMEN FINAL
# ============================================================================

message(STATUS "")
message(STATUS "=== CONFIGURACIÓN COMPLETADA ===")
message(STATUS "Proyecto: nfs")
message(STATUS "Cliente: ${NFS_CLIENT}")
message(STATUS "Servidor: ${NFS_SERVER}") 
message(STATUS "Editor: ${NFS_EDITOR}")
message(STATUS "Tests: ${NFS_TESTS}")
message(STATUS "=================================")