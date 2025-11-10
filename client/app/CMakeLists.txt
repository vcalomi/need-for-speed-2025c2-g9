# === Submódulo: app ===

add_library(app
    engine.cpp
    game.cpp
)

target_include_directories(app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app
    PUBLIC
        systems
        graphics
        world
        events
        ui
        resources
        taller_common
        SDL2pp::SDL2pp
)
