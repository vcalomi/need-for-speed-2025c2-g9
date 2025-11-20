add_library(graphics
    spritesheet.cpp
    particle_renderer.cpp
    player_renderer.cpp
    checkpoint_renderer.cpp
    background_renderer.cpp
    text_renderer.cpp
    screen_renderer.cpp
    hud_renderer.cpp
)

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(graphics
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        nfs_common
        systems
        world
        events
        ui
        network
        resources
)
