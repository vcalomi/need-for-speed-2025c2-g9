# === Submódulo: lobby ===

add_library(lobby
    mainwindow.cpp
    navigation.cpp
    room_manager.cpp
    styles.cpp
    lobby_api.h lobby_api.cpp
    lobby_service.h lobby_service.cpp
    rooms_pager.h rooms_pager.cpp
    wait_room_controller.h wait_room_controller.cpp
    car_selection_controller.h car_selection_controller.cpp
    background_music.h background_music.cpp
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