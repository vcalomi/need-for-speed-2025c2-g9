#include "lobby_service.h"

bool LobbyService::login(const QString& username) { return api.login(username.toStdString()); }

QStringList LobbyService::listRooms() {
    QStringList out;
    auto rooms = api.listRooms();
    for (auto& r: rooms) out << QString::fromStdString(r);
    return out;
}

bool LobbyService::createRoom(const QString& code, unsigned maxPlayers) {
    return api.createRoom(code.toStdString(), maxPlayers);
}

bool LobbyService::joinRoom(const QString& code) { return api.joinRoom(code.toStdString()); }

PlayersVm LobbyService::listPlayers() {
    auto snap = api.listPlayers();
    PlayersVm vm;
    vm.maxPlayers = snap.maxPlayers;
    for (auto& p: snap.players) vm.players.push_back(QString::fromStdString(p));
    return vm;
}

LobbyState LobbyService::pollState() { return LobbyState{api.pollStarted()}; }

bool LobbyService::chooseCar(const QString& carKey) { return api.chooseCar(carKey.toStdString()); }

void LobbyService::startGame() { api.startGame(); }

bool LobbyService::selectMaps(const std::vector<uint8_t>& mapCodes) {
    return api.selectMaps(mapCodes);
}