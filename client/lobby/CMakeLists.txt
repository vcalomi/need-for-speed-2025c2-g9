cmake_minimum_required(VERSION 3.16)

project(loby VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuración SIMPLE de Qt6
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt6")

# Buscar Qt6 Core primero
find_package(Qt6 REQUIRED COMPONENTS Core)

# Luego buscar Multimedia específicamente
find_package(Qt6Multimedia REQUIRED)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --------------------------
set(QT_VERSION_MAJOR 6)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        # --- CLIENTE ---
        ../network/client_protocol.cpp
        ../network/client_receiver.cpp

        # --- COMÚN ---
        ../../common/socket.cpp
        ../../common/resolver.cpp
        ../../common/resolvererror.cpp
        ../../common/liberror.cpp
        ../../common/common_protocol.cpp
        ../../common/broadcaster.cpp

        # --- SERIALIZERS ---
        ../../common/serializer/serializer.cpp
        ../../common/serializer/vehicle_serializer.cpp
        ../../common/serializer/player_serializer.cpp
        ../../common/serializer/player_move_serializer.cpp

        # --- DTOs ---
        ../../common/Dto/dto.cpp
        ../../common/Dto/player.cpp
        ../../common/Dto/player_move.cpp
        ../../common/Dto/vehicle.cpp
)

# qt_add_resources(APP_RESOURCES "client/lobby/assets/resources.qrc")
set(APP_RESOURCES assets/resources.qrc)

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(loby
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${APP_RESOURCES}
        styles.h styles.cpp
        navigation.h navigation.cpp
        room_manager.h room_manager.cpp
        player_info.h
        car.h

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET loby APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(loby SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(loby
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(loby PRIVATE 
    Qt6::Core
    Qt6::Widgets 
    Qt6::Multimedia
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.

# if(${QT_VERSION} VERSION_LESS 6.1.0)
if(DEFINED QT_VERSION AND QT_VERSION VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.loby)
endif()
set_target_properties(loby PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS loby
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(loby)
endif()
