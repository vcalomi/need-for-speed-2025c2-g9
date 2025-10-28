// mock_server.cpp
#include "./MockServer.h"

#include <cmath>
#include <sstream>

#define INPUT_FORWARD 0x01
#define INPUT_BACKWARD 0x02
#define INPUT_LEFT 0x04
#define INPUT_RIGHT 0x08

MockServer::MockServer() {
    // Simula que el servidor ya conoce a los jugadores
    players_["1"] = {"1", "CamionetaRoja", 400.f, 300.f, 0.f, 0.f};  // tu jugador local
    players_["2"] = {"2", "CamionetaRoja", 500.f, 300.f, 0.f, 0.f};  // otro jugador
}

void MockServer::ReceiveInput(uint8_t inputByte) {
    bool forward = inputByte & INPUT_FORWARD;
    bool backward = inputByte & INPUT_BACKWARD;
    bool left = inputByte & INPUT_LEFT;
    bool right = inputByte & INPUT_RIGHT;

    auto& p = players_["1"];  // tu jugador local

    float accel = 0.f;
    float turn = 0.f;

    if (forward)
        accel = 1.f;
    else if (backward)
        accel = -1.f;
    if (left)
        turn = -1.f;
    else if (right)
        turn = 1.f;

    p.angle += turn * 2.5f;
    p.speed += accel * 50.f;
    p.speed *= 0.95f;
}

void MockServer::Update(float delta) {
    // --- Movimiento ---
    for (auto& [id, p]: players_) {
        p.x += std::cos(p.angle * M_PI / 180.0f) * p.speed * delta;
        p.y += std::sin(p.angle * M_PI / 180.0f) * p.speed * delta;
    }

    // --- Detección de colisiones ---
    for (auto it1 = players_.begin(); it1 != players_.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != players_.end(); ++it2) {
            ServerPlayer& p1 = it1->second;
            ServerPlayer& p2 = it2->second;

            if (CheckCollision(p1, p2)) {
                // Efecto físico simple: rebote
                float tmpSpeed = p1.speed;
                p1.speed = -p2.speed * 0.5f;
                p2.speed = -tmpSpeed * 0.5f;

                // Separarlos un poco
                float dx = p1.x - p2.x;
                float dy = p1.y - p2.y;
                float len = std::sqrt(dx * dx + dy * dy) + 0.001f;
                float overlap = 0.5f * (40.f - len);
                p1.x += (dx / len) * overlap;
                p1.y += (dy / len) * overlap;
                p2.x -= (dx / len) * overlap;
                p2.y -= (dy / len) * overlap;

                // Enviar evento COLLISION
                std::ostringstream colMsg;
                colMsg << "COLLISION " << p1.id << " " << p2.id;
                messages.push(colMsg.str());
            }
        }
    }

    // --- Enviar posiciones actualizadas ---
    for (auto& [id, p]: players_) {
        std::ostringstream msg;
        msg << "UPDATE " << id << " " << p.x << " " << p.y << " " << p.angle;
        messages.push(msg.str());
    }
}


std::string MockServer::PopMessage() {
    auto msg = messages.front();
    messages.pop();
    return msg;
}

bool MockServer::CheckCollision(const ServerPlayer& a, const ServerPlayer& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    float minDistance = 40.f;  // tamaño aproximado del auto

    return distance < minDistance;
}
