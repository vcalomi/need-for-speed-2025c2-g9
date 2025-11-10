# === Submódulo: lobby ===

add_library(lobby
    mainwindow.cpp
    navigation.cpp
    room_manager.cpp
    styles.cpp
)

qt_add_resources(lobby_resources assets/resources.qrc)
target_sources(lobby PRIVATE ${lobby_resources})


target_include_directories(lobby
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lobby
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Multimedia
        taller_common
)