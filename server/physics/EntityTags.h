#pragma once
#include <vector>

enum class EntityKind {
    Vehicle,
    Checkpoint,
};

struct FixtureTag {
    EntityKind kind;
    int id;  // Vehicle: playerId, Checkpoint: index
};

inline FixtureTag* makeTag(std::vector<FixtureTag>& container,
                           EntityKind kind, int id) {
    container.push_back({kind, id});
    return &container.back();
}