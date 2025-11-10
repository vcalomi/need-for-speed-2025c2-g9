# === Submódulo: world ===

add_library(world
    camera.cpp
    player.cpp
    world.cpp
)

target_include_directories(world
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(world
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        taller_common
        systems
        graphics
        events
        ui
        network
        resources
)
