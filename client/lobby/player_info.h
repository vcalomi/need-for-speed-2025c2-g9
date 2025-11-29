#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <QString>

#include "car.h"

struct PlayerInfo {
    QString username;
    QString roomCode;
    bool isHost = false;
    int maxPlayers = 8;
    int currentPlayers = 1;  // host siempre cuenta
    CarType selectedCar;     // luego lo reemplazamos por enum
    QStringList selectedMaps;
};


#endif
