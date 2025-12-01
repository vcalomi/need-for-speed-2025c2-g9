#include "progress_manager.h"

#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"


ProgressManager::ProgressManager(EventBus& bus): bus_(bus) {
    bus_.Subscribe<CheckPointCompletedEvent>([this](const CheckPointCompletedEvent& e) {
        if (e.username == localUsername_) {
            OnLocalCheckpoint(e.checkpoint_id);
        }
    });
}

void ProgressManager::SetLocalPlayer(const std::string& username) { localUsername_ = username; }

void ProgressManager::SetCheckpoints(const std::vector<Checkpoint>& cps) { checkpoints_ = cps; }

void ProgressManager::Reset() { progress_ = PlayerProgress{}; }

void ProgressManager::OnLocalCheckpoint(int checkpointId) {
    if (checkpoints_.empty())
        return;

    const auto& expected = checkpoints_[progress_.currentCheckpointIndex];

    if (checkpointId != expected.id)
        return;

    progress_.passedCheckpoints.insert(checkpointId);
    progress_.currentCheckpointIndex++;

    if (progress_.currentCheckpointIndex >= checkpoints_.size()) {
        progress_.currentCheckpointIndex = 0;
        progress_.lapsCompleted++;
    }
}

const Checkpoint& ProgressManager::GetActiveCheckpoint() const {
    return checkpoints_[progress_.currentCheckpointIndex];
}

const Checkpoint& ProgressManager::GetNextCheckpoint() const {
    int n = (progress_.currentCheckpointIndex + 1) % checkpoints_.size();
    return checkpoints_[n];
}

int ProgressManager::GetLapCount() const { return progress_.lapsCompleted; }

const std::set<int>& ProgressManager::GetPassedCheckpoints() const {
    return progress_.passedCheckpoints;
}
