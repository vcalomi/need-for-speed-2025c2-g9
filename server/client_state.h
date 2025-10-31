enum class ClientState {
    IN_LOBBY,           // en lobby (lista de salas/crear sala)
    IN_GAME_ROOM,       // En sala esperando que el host inicie
    IN_GAME             // en partida (eligiendo autos, carrera)
};