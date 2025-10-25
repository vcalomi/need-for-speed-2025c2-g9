#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <QString>

struct PlayerInfo {
    QString host;
    int port = 0;

    QString username;
    QString roomCode;

    bool isHost = false;
    int minPlayers = 2;
    int maxPlayers = 8;
    int currentPlayers = 1; // host siempre cuenta
    int selectedCar = -1;  // luego lo reemplazamos por enum
};


#endif  // PLAYER_INFO_H
