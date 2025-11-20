# === Submódulo: network ===

add_library(network
    client_protocol.cpp
    client_receiver.cpp
    client_sender.cpp
    client.cpp
)

target_include_directories(network
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(network
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        nfs_common
        systems
        graphics
        world
        events
        ui
        resources
)
