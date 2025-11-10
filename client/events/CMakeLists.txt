# === Submódulo: events ===

# Crear una librería de solo headers (INTERFACE)
add_library(events INTERFACE)

# Hacer visibles los headers a otros targets
target_include_directories(events
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
