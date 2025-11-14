#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "../../common/Dto/dto.h"
#include "../../common/broadcaster.h"
#include "../../common/car_config.h"
#include "../../common/common_codes.h"
#include "../../common/queue.h"
#include "../../common/thread.h"
#include "../LevelSetup.h"

struct PlayerRaceProgress {
    int currentLap = 0;       
    int nextCheckpoint = 0;   
    bool finished = false;    
};

class GameLoop: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& gameLoopQueue;
    std::map<int, CarConfig>& chosenCars_;
    std::map<int, std::string>& playerUsernames_;
    Broadcaster& broadcaster_;
    std::optional<LevelSetup> setup;
    int maxPlayers;
    void handlerProcessCommand(std::shared_ptr<Dto> dto);
    Vehicle* getVehicleByPlayer(const std::string& username);
    Vehicle* getVehicleById(int vehicleId);
    std::unordered_map<int, PlayerRaceProgress> raceProgress_; 
    void handleRaceProgress(int vehicleId, int checkpointIndex);
    float computeCollisionDamage(float impactSpeed);
    void handleVehicleVehicleCollision(const RawVehicleVehicle& event);
    void handleVehicleWallCollision(const RawVehicleWall& event);
    

public:
    explicit GameLoop(Queue<std::shared_ptr<Dto>>& gameLoopQueue,
                      std::map<int, CarConfig>& chosenCars,
                      std::map<int, std::string>& playerUsernames, Broadcaster& broadcaster,
                      int maxPlayers);

    void run() override;
    void processCommands();
    void sendVehiclesPositions();
    void sendInitialPlayersCars();
    void sendCheckpoints();
    void processGameEvents();




    ~GameLoop();
};

#endif
