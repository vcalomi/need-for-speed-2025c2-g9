# === Submódulo: resources ===

add_library(resources
  resource_loader.cpp
)

target_include_directories(resources
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(resources
    PUBLIC
        SDL2pp::SDL2pp
        yaml-cpp
        nfs_common
        systems
        graphics
        world
        events
        ui
        network
)

# === Copiar carpeta static al directorio de ejecución ===

# Ruta fuente de los recursos
set(RESOURCE_STATIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/static)

# Ruta destino dentro del build (junto al ejecutable)
set(OUTPUT_STATIC_DIR ${CMAKE_BINARY_DIR}/client/resources/static)

# Asegurar que la carpeta de destino exista y copiar el contenido
add_custom_command(
    TARGET resources POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_STATIC_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCE_STATIC_DIR}
            ${OUTPUT_STATIC_DIR}
    COMMENT "📦 Copiando recursos estáticos a ${OUTPUT_STATIC_DIR}"
)
