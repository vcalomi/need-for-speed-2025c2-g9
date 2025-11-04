#include "./game.h"

#include <sstream>

Game::Game():
        engine_(),
        audioManager_(),
        resources_(engine_.GetRenderer()),
        world_(),
        inputSystem_(),
        rendererSystem_(engine_.GetRenderer(), resources_.GetCarSprites()),
        map_(engine_.GetRenderer(), "../client/assets/need-for-speed/cities/san_andreas.png"),
        mockServer_() {

    const auto& players = mockServer_.GetPlayers();
    for (const auto& [id, p]: players) {
        world_.AddPlayer(id, "CamionetaRoja", p.x, p.y, id == "1");
    }

    audioManager_.PlayBackgroundMusic("../client/assets/need-for-speed/music/background.wav");
}

void Game::ProcessServerMessage(const std::string& msg) {
    std::istringstream ss(msg);
    std::string type;
    ss >> type;

    if (type == "UPDATE") {
        std::string id;
        float x, y, angle;
        ss >> id >> x >> y >> angle;
        world_.UpdateFromServer(id, x, y, angle);
    }

    if (type == "COLLISION") {
        std::string id1, id2;
        ss >> id1 >> id2;
        world_.OnCollision(id1, id2);
    }
}


void Game::Run() {
    bool running = true;
    Uint64 prevTicks = SDL_GetTicks64();

    while (running) {
        Uint64 currentTicks = SDL_GetTicks64();
        float delta = (currentTicks - prevTicks) / 1000.f;
        prevTicks = currentTicks;

        // --- INPUT ---
        inputSystem_.PollEvents(running);
        uint8_t inputByte = inputSystem_.GetInputByte();

        // Enviamos el input al servidor simulado (mock)
        mockServer_.ReceiveInput(inputByte);

        // --- SIMULACIÓN SERVIDOR ---
        mockServer_.Update(delta);

        // --- MENSAJES DEL SERVIDOR ---
        while (mockServer_.HasMessage()) {
            std::string msg = mockServer_.PopMessage();
            ProcessServerMessage(msg);
        }

        // --- RENDERIZADO ---
        rendererSystem_.Render(world_, map_);

        SDL_Delay(16);
    }
}
