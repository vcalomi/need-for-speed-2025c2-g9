target_sources(taller_client
    PRIVATE
    # Core
    core/audio_manager.cpp
    core/engine.cpp
    core/game.cpp
    core/resource_loader.cpp
    # Graphics
    graphics/renderer_system.cpp
    graphics/spritesheet.cpp
    # Input
    input/input_system.cpp
    # Network
    network/mock_server.cpp
    # World
    world/player.cpp
    world/world.cpp
    # Main
    main.cpp
    PUBLIC
    # Core
    core/audio_manager.h
    core/engine.h
    core/game.h
    core/map.h
    core/resource_loader.h
    # Graphics
    graphics/renderer_system.h
    graphics/spritesheet.h
    # Input
    input/input_system.h
    # Network
    network/mock_server.h
    # World
    world/player.h
    world/world.h
    )
