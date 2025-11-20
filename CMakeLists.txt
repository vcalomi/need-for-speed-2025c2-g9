cmake_minimum_required(VERSION 3.24)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS ON)

project(nfs_tp
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

# Define target: lib common between client, server and editor programs
# HEY TODO XXX: change the name to something that you like
add_library(nfs_common STATIC)

# add the source files that make the lib
# HEY!! TODO XXX: add more subfolders here
add_subdirectory(common/)

find_package(Threads REQUIRED)
target_link_libraries(nfs_common PRIVATE Threads::Threads)

# set compiler warnings to the targets
include(cmake/CompilerWarnings.cmake)
set_project_warnings(nfs_common ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

# Say where the headers will be found required by library target
# Make this PUBLIC so any dependant linking lib will inherit this
# location (so our dependants will not have to explicitly request this)
target_include_directories(nfs_common PUBLIC .)

# HEY!! TODO XXX: you need to install some runtime and dev libraries *before*
# compiling the client/editor code.
#
# In xubuntu 24.04 these are:
#
# sudo apt-get install libopus-dev libopusfile-dev libxmp-dev libfluidsynth-dev fluidsynth libwavpack1 libwavpack-dev libfreetype-dev wavpack
#

# Install libSDL2 and libSDL2pp (for client or editor)
# HEY!! TODO XXX: add more dependendies (either for client, server or editor)
if(NFS_CLIENT OR NFS_EDITOR OR NFS_SERVER)
    include(FetchContent)

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.8
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        libSDL2pp
        OVERRIDE_FIND_PACKAGE
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )
    # yaml-cpp
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        master
    )

    set(SDL2PP_WITH_IMAGE ON CACHE BOOL "Enable SDL2_image support in SDL2pp" FORCE)
    set(SDL2PP_WITH_TTF ON CACHE BOOL "Enable SDL2_ttf support in SDL2pp" FORCE)
    set(SDL2PP_WITH_MIXER ON CACHE BOOL "Enable SDL2_mixer support in SDL2pp" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)

    

    FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_mixer SDL2_ttf libSDL2pp yaml-cpp)

    if (NOT TARGET SDL2_image::SDL2_image)
        add_library(SDL2_image::SDL2_image ALIAS SDL2_image)
    endif()
    if (NOT TARGET SDL2_ttf::SDL2_ttf)
        add_library(SDL2_ttf::SDL2_ttf ALIAS SDL2_ttf)
    endif()
    if (NOT TARGET SDL2_mixer::SDL2_mixer)
        add_library(SDL2_mixer::SDL2_mixer ALIAS SDL2_mixer)
    endif()

endif()

## --- Dependencias del SERVER (Box2D) -------------------------------
if(NFS_SERVER)
    include(FetchContent)

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON  CACHE BOOL "" FORCE)

    # Box2D
    FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.1.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(box2d)

    # yaml-cpp
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(yaml-cpp)

    # nlohmann_json (header-only)
    # Tip: desactivamos tests para acelerar
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Program section
# ---------------
if(NFS_CLIENT)
    add_executable(nfs_client)
    add_dependencies(nfs_client nfs_common SDL2pp::SDL2pp)

    # --- Habilitar Qt6 globalmente para el cliente ---
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    # Ruta de instalación de Qt6 (Ubuntu/Debian)
    list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt6")

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

    add_subdirectory(client)

    set_project_warnings(nfs_client ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)
    target_include_directories(nfs_client PRIVATE ${libSDL2pp_SOURCE_DIR})

    target_link_libraries(nfs_client
        PRIVATE
            nfs_common
            SDL2pp::SDL2pp
            Qt${QT_VERSION_MAJOR}::Widgets
            yaml-cpp
    )
endif()



if(NFS_SERVER)
    # ejecutables
    add_executable(nfs_server)

    add_executable(physics_sandbox
        server/physics/main.cpp
        server/physics/LevelCreator.cpp    # <-- AÑADIDO
        server/YamlParser.cpp
        server/physics/vehicle.cpp
        server/LevelSetup.cpp
        server/physics/PhysicsEventCollector.cpp
    )

    add_dependencies(nfs_server nfs_common)
    add_subdirectory(server)

    set_project_warnings(nfs_server ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # links
    target_link_libraries(nfs_server PRIVATE
        nfs_common
        yaml-cpp 
        SDL2::SDL2
        box2d
        nlohmann_json::nlohmann_json
    )

    target_link_libraries(physics_sandbox PRIVATE
        box2d
        SDL2::SDL2
        SDL2pp::SDL2pp
        yaml-cpp
        nlohmann_json::nlohmann_json
        SDL2_image::SDL2_image 
                       # <-- AÑADIDO
    )
endif()


if(NFS_EDITOR)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
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

    find_package(yaml-cpp REQUIRED)
    
    # Make them depend on nfs_common lib
    add_dependencies(nfs_editor nfs_common SDL2pp::SDL2pp)

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(editor)

    set_project_warnings(nfs_editor ${NFS_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # add libSDL2pp's headers
    target_include_directories(nfs_editor PRIVATE ${libSDL2pp_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/editor/editor-mapas ${CMAKE_SOURCE_DIR}/server/maps)

    # Link the nfs_common lib target
    target_link_libraries(nfs_editor nfs_common SDL2pp::SDL2pp Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    yaml-cpp)
endif()

# Testing section
# ---------------

if(NFS_TESTS)
    # Let's use google test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    # (borrowed from https://google.github.io/googletest/quickstart-cmake.html)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    # target for testing
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(nfs_tests)

    # HEY!! TODO XXX: I'm assuming that the code under testing is in nfs_common
    # Change this if you have to.
    add_dependencies(nfs_tests nfs_common)

    # Say where to find the xoz headers
    # HEY!! TODO XXX: add more subfolders here
    target_include_directories(nfs_tests PUBLIC .)

    # Add the source files to the nfs_tests target
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(tests/)

    # Set the compiler warnings to the tests target (but be a little lesser strict)
    set_project_warnings(nfs_tests ${NFS_MAKE_WARNINGS_AS_ERRORS} TRUE)

    # Say where googletest headers can be found
    target_include_directories(nfs_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

    # HEY!! TODO XXX: I'm assuming that the code under testing is in nfs_common
    # Change this if you have to.
    target_link_libraries(nfs_tests
        nfs_common
        GTest::gtest_main
    )
endif()
