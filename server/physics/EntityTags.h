#pragma once

enum class EntityKind {
    Vehicle,
    Checkpoint,
};

struct FixtureTag {
    EntityKind kind;
    int id;  // Vehicle: playerId, Checkpoint: index
};