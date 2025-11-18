# === Submódulo: dto_handlers ===

file(GLOB DTO_HANDLERS_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(dto_handlers ${DTO_HANDLERS_SRC})

target_include_directories(dto_handlers
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dto_handlers
    PUBLIC
        events
        taller_common
)
