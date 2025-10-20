target_sources(taller_common
    PRIVATE
    # .cpp files
    foo.cpp
    liberror.cpp
    resolver.cpp
    resolvererror.cpp
    socket.cpp
    protocol.cpp
    PUBLIC
    # .h files
    foo.h
    liberror.h
    queue.h
    resolver.h
    resolvererror.h
    socket.h
    thread.h
    protocol.h
    command.h
    event.h
    )
