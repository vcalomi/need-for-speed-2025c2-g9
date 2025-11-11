#ifndef LOBBY_SERVICE_H
#define LOBBY_SERVICE_H

#include <QString>
#include <QStringList>
#include <string>
#include <vector>

#include "lobby_api.h"

struct LobbyState {
    bool started = false;
};
struct PlayersVm {
    unsigned maxPlayers = 0;
    std::vector<QString> players;
};

class LobbyService {
public:
    explicit LobbyService(LobbyApi& api): api(api) {}

    bool login(const QString& username);
    QStringList listRooms();
    bool createRoom(const QString& code, unsigned maxPlayers);
    bool joinRoom(const QString& code);
    PlayersVm listPlayers();
    LobbyState pollState();
    bool chooseCar(const QString& carKey);
    void startGame();

private:
    LobbyApi& api;
};

#endif
