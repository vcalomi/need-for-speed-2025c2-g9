#pragma once
#include <set>
#include <string>
#include <vector>

#include "../events/event.h"

#include "checkpoint.h"

struct PlayerProgress {
    int currentCheckpointIndex = 0;
    int lapsCompleted = 0;
    std::set<int> passedCheckpoints;
};

class ProgressManager {
public:
    explicit ProgressManager(EventBus& bus);

    void SetLocalPlayer(const std::string& username);
    void SetCheckpoints(const std::vector<Checkpoint>& checkpoints);
    void Reset();

    void OnLocalCheckpoint(int checkpointId);

    const Checkpoint& GetActiveCheckpoint() const;
    const Checkpoint& GetNextCheckpoint() const;

    int GetLapCount() const;
    const std::set<int>& GetPassedCheckpoints() const;

private:
    EventBus& bus_;

    std::string localUsername_;
    PlayerProgress progress_;

    std::vector<Checkpoint> checkpoints_;
    int totalLaps_ = 3;
};
