target_sources(nfs_server
    PRIVATE
    # .cpp files
    main.cpp
    LevelCreator.cpp
    vehicle.cpp
    PUBLIC
    # .h files
    LevelCreator.h
    spawn.h
    vehicle.h
    EntityTags.h
    )