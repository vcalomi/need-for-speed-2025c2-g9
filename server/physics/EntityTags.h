#pragma once
#include <vector>
#include <deque>

enum class EntityKind {
    Vehicle,
    Checkpoint,
};

struct FixtureTag {
    EntityKind kind;
    int id;  // Vehicle: playerId, Checkpoint: index
};

inline FixtureTag* makeTag(std::deque<FixtureTag>& container, EntityKind kind, int id) {
    container.push_back(FixtureTag{kind, id});
    return &container.back();   
}