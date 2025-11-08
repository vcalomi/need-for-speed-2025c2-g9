#include "game_room.h"

#include <algorithm>
#include <iostream>

#include "../../common/Dto/vehicle.h"
#include "../../common/common_codes.h"
#include "../../common/queue.h"

GameRoom::GameRoom(const std::string& roomName, int hostId, int maxPlayers):
        roomName(roomName),
        hostId(hostId),
        maxPlayers_(maxPlayers),
        state(RoomState::WAITING_FOR_PLAYERS),
        gameQueue(),
        broadcaster(),
        gameLoop(gameQueue, chosenCars, broadcaster, maxPlayers) {}

bool GameRoom::addPlayer(int clientId, Queue<std::shared_ptr<Dto>>& senderQueue) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!canJoin())
        return false;

    players[clientId] = &senderQueue;
    broadcaster.addQueue(&senderQueue);
    return true;
}

bool GameRoom::startGame() {
    std::lock_guard<std::mutex> lock(mtx);

    if (state != RoomState::WAITING_FOR_PLAYERS || players.size() < 1) {
        return false;
    }

    for (const auto& [id, queue]: players) {
        if (chosenCars.find(id) == chosenCars.end()) {
            CarConfig defaultCar{};
            defaultCar.carType = "fiat_600";
            chosenCars[id] = defaultCar;
        }
    }

    state = RoomState::IN_RACE;

    // uint8_t idx = 0;
    // for (const auto& entry: players) {
    //     uint8_t id = static_cast<uint8_t>(entry.first % 255);
    //     auto dto = std::make_shared<VehicleDto>(id, 1.0f + idx, 0.0f, 0.0f);
    //     {}
    //     std::cout << "CREATED VehicleDto: id=" << (int)dto->id << " x=" << dto->x << " y=" <<
    //     dto->y
    //               << std::endl;

    //     broadcaster.broadcast(dto);
    //     idx++;
    // }

    gameLoop.start();
    std::cout << "GameLoop is alive: " << gameLoop.is_alive() << std::endl;
    return true;
}

bool GameRoom::chooseCar(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);
    if (players.find(clientId) == players.end()) {
        return false;
    }

    chosenCars[clientId] = car;
    return true;
}

void GameRoom::removePlayer(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = players.find(clientId);
    if (it != players.end()) {
        broadcaster.removeQueue(it->second);
        players.erase(it);
    }
}

bool GameRoom::canJoin() const {
    return state == RoomState::WAITING_FOR_PLAYERS &&
           static_cast<int>(players.size()) < maxPlayers_;
}

bool GameRoom::isHost(int clientId) const { return hostId == clientId; }

Queue<std::shared_ptr<Dto>>& GameRoom::getGameQueue() { return gameQueue; }

std::vector<int> GameRoom::getPlayerIds() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> ids;
    ids.reserve(players.size());
    for (const auto& player: players) {
        ids.push_back(player.first);
    }
    return ids;
}

bool GameRoom::isInRace() {
    std::lock_guard<std::mutex> lock(mtx);
    return state == RoomState::IN_RACE;
}

GameRoom::~GameRoom() {
    try {
        gameLoop.stop();
    } catch (...) {}
    try {
        gameQueue.close();
    } catch (...) {}
    if (gameLoop.is_alive()) {
        gameLoop.join();
    }
    for (const auto& entry: players) {
        broadcaster.removeQueue(entry.second);
    }
}
#include <memory>
