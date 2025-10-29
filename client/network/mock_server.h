// mock_server.h
#pragma once
#include <cstdint>
#include <map>
#include <queue>
#include <string>

struct ServerPlayer {
    std::string id;
    std::string carType;
    float x;
    float y;
    float angle = 0.0f;
    float speed = 0.0f;
};

class MockServer {
private:
    float time_accumulator = 0.0f;
    std::queue<std::string> messages;
    bool CheckCollision(const ServerPlayer& a, const ServerPlayer& b);
    std::map<std::string, ServerPlayer> players_;

public:
    MockServer();
    void ReceiveInput(uint8_t inputByte);
    void Update(float delta);
    bool HasMessage() const { return !messages.empty(); }
    std::map<std::string, ServerPlayer>& GetPlayers() { return players_; }
    std::string PopMessage();
};
