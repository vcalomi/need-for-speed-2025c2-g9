# === Submódulo: ui ===

add_library(ui
    minimap.cpp
    checkpoint_indicator.cpp
    speedometer.cpp
)

target_include_directories(ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ui
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        nfs_common
        systems
        graphics
        world
        events
        network
        resources
)
