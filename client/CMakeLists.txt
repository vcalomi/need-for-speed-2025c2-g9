set_target_properties(taller_client PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

target_sources(taller_client
    PRIVATE
    # .cpp files
    client_main.cpp
    network/client.cpp
    network/client_protocol.cpp
    network/client_receiver.cpp
    network/client_sender.cpp
    ../common/socket.cpp
    ../common/resolver.cpp
    ../common/liberror.cpp
    ../common/resolvererror.cpp
    ../common/common_protocol.cpp
    # lobby
    lobby/mainwindow.cpp
    lobby/navigation.cpp
    lobby/room_manager.cpp
    lobby/styles.cpp

    # --- .ui files ---
    lobby/mainwindow.ui

    # --- .qrc (si tenés recursos de Qt) ---
    lobby/assets/resources.qrc

    PUBLIC
    # .h files
    network/client.h
    network/client_protocol.h
    network/client_receiver.h
    network/client_sender.h
    network/client_input_handler.h
    ../common/socket.h
    ../common/resolver.h
    ../common/liberror.h
    ../common/resolvererror.h
    ../common/common_protocol.h
    
    # --- LOBBY headers ---
    lobby/mainwindow.h
    lobby/navigation.h
    lobby/room_manager.h
    lobby/styles.h
    lobby/player_info.h
    lobby/car.h
    )

  target_link_libraries(taller_client PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Multimedia
)