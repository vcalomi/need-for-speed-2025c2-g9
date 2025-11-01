target_sources(taller_client
    PRIVATE
    # .cpp files
    client_main.cpp
    client.cpp
    client_protocol.cpp
    client_receiver.cpp
    ../common/socket.cpp
    ../common/resolver.cpp
    ../common/liberror.cpp
    ../common/resolvererror.cpp
    ../common/common_protocol.cpp
    PUBLIC
    # .h files
    client.h
    client_protocol.h
    client_receiver.h
    ../common/socket.h
    ../common/resolver.h
    ../common/liberror.h
    ../common/resolvererror.h
    ../common/common_protocol.h
    )
    