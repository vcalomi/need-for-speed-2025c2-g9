add_library(graphics
    spritesheet.cpp
    particle_system.cpp
)

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(graphics
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        taller_common
        systems
        world
        events
        ui
        network
        resources
)
