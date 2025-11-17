# === Submódulo: systems ===

add_subdirectory(dto_handlers)

add_library(systems
    renderer_system.cpp
    network_system.cpp
    dto_handler_system.cpp
    input_system.cpp
    audio_system.cpp
    event_render_controller.cpp
)

target_include_directories(systems
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(systems
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        taller_common
        graphics
        world
        events
        ui
        network
        resources
        dto_handlers
)
