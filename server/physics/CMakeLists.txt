target_sources(taller_server
    PRIVATE
    # .cpp files
    PhysicsWorld.cpp
    physics_sandbox.cpp
    LevelAdministrator.cpp
    main.cpp
    LevelCreator.cpp
    PUBLIC
    # .h files
    LevelAdministrator.h
    LevelCreator.h
    )