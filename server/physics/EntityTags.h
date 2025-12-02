#pragma once
#include <vector>
#include <deque>

enum class EntityKind {
    Vehicle,
    Checkpoint,
    BridgeToggle,
    Npc,
};

struct FixtureTag {
    EntityKind kind;
    int id;     // Vehicle: playerId, Checkpoint: index 
    int data;  //BridgeToggle: 1 = under, 0 = over
};

inline FixtureTag* makeTag(std::deque<FixtureTag>& pool, EntityKind kind, int id, int data = 0) {
    pool.push_back(FixtureTag{kind, id, data});
    return &pool.back();
}
