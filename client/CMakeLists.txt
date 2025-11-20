# === CLIENT MAIN CMAKE ===

set_target_properties(nfs_client PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

# Submódulos del cliente
add_subdirectory(app)
add_subdirectory(systems)
add_subdirectory(world)
add_subdirectory(graphics)
add_subdirectory(events)
add_subdirectory(ui)
add_subdirectory(lobby)
add_subdirectory(network)
add_subdirectory(resources)

# === Ejecutable principal del cliente ===
target_sources(nfs_client
    PRIVATE
        client_main.cpp   # o el nombre de tu archivo main real
)

# === Enlazar todo al ejecutable ===
target_link_libraries(nfs_client
    PRIVATE
        lobby
        network
        app
        systems
        world
        graphics
        events
        ui
        resources
        nfs_common
        SDL2pp::SDL2pp
        yaml-cpp
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Multimedia
)

