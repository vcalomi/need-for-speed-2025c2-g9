target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    spritesheet.cpp
    game.cpp
    player.cpp
    input_system.cpp
    renderer_system.cpp
    world.cpp
    engine.cpp
    resource_loader.cpp
    PUBLIC
    # .h files
    spritesheet.h
    player.h
    game.h
    input_system.h
    renderer_system.h
    world.h
    engine.h
    resource_loader.h
    map.h
    )
