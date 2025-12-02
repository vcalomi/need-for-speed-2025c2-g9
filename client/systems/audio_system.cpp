#include "./audio_system.h"

#include <iostream>

#define CHANNELS 2
#define CHUNK_SIZE 512
#define STARTING_VOLUME 32

#include "../events/checkpoint_completed_event.h"
#include "../events/countdown_down_event.h"
#include "../events/countdown_go_event.h"
#include "../events/player_events.h"
#include "../events/vehicle_exploded_event.h"

using SDL2pp::Mixer;

AudioSystem::AudioSystem(EventBus& bus): eventBus_(bus), audioEnabled_(true) {
#ifdef __linux__
    // Detectar si estamos dentro de WSL
    if (std::getenv("WSL_DISTRO_NAME")) {
        std::cerr << "[AudioSystem] Detected WSL environment. Audio disabled.\n";
        audioEnabled_ = false;
        return;  // No inicializamos SDL_mixer
    }
#endif

    try {
        mixer_ = std::make_unique<Mixer>(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS,
                                         CHUNK_SIZE);
        mixer_->SetMusicVolume(STARTING_VOLUME);

        cache_["car_engine.wav"] = std::make_unique<SDL2pp::Chunk>(
                "../client/assets/need-for-speed/music/car_engine.wav");
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "[AudioSystem] Failed to initialize audio: " << e.GetSDLFunction() << " - "
                  << e.GetSDLError() << "\n";
        audioEnabled_ = false;
    }

    eventBus_.Subscribe<VehicleExplodedEvent>([this](const VehicleExplodedEvent&) {
        PlaySoundEffect("../client/assets/need-for-speed/music/car_explosion.wav");
    });

    eventBus_.Subscribe<CheckPointCompletedEvent>([this](const CheckPointCompletedEvent&) {
        PlaySoundEffect("../client/assets/need-for-speed/music/checkpoint_reached.wav");
    });

    eventBus_.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        uint8_t move_mask = static_cast<uint8_t>(e.move);
        bool isAccelerating = (move_mask & static_cast<uint8_t>(MoveMask::ACCELERATE)) != 0;

        if (isAccelerating && !enginePlaying_) {
            // Comenzar sonido de motor en loop infinito
            engineChannel_ = mixer_->PlayChannel(-1, *cache_["car_engine.wav"], -1);
            enginePlaying_ = true;
        }

        if (!isAccelerating && enginePlaying_) {
            // Detener sonido del motor
            mixer_->HaltChannel(engineChannel_);
            enginePlaying_ = false;
        }
    });


    eventBus_.Subscribe<CountdownDownEvent>([this](const CountdownDownEvent&) {
        PlaySoundEffect("../client/assets/need-for-speed/music/countdown_beep.wav");
    });

    eventBus_.Subscribe<CountdownGoEvent>([this](const CountdownGoEvent&) {
        PlaySoundEffect("../client/assets/need-for-speed/music/countdown_start.wav");
    });
}

Mixer* AudioSystem::GetMixer() { return audioEnabled_ ? mixer_.get() : nullptr; }

void AudioSystem::PlayBackgroundMusic(const std::string& filepath, int loops) {
    if (!audioEnabled_)
        return;

    try {
        backgroundMusic_ = std::make_unique<SDL2pp::Music>(filepath);
        mixer_->PlayMusic(*backgroundMusic_, loops);
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "[AudioSystem] Error playing music: " << e.GetSDLFunction() << " - "
                  << e.GetSDLError() << "\n";
    }
}

void AudioSystem::StopBackgroundMusic() {
    if (audioEnabled_)
        mixer_->HaltMusic();
}

void AudioSystem::PlaySoundEffect(const std::string& filepath, int loops) {
    if (!audioEnabled_)
        return;

    try {
        if (!cache_.count(filepath)) {
            cache_[filepath] = std::make_unique<SDL2pp::Chunk>(filepath);
        }

        mixer_->PlayChannel(-1, *cache_[filepath], loops);

    } catch (const SDL2pp::Exception& e) {
        std::cerr << "[AudioSystem] Error playing sound effect: " << e.GetSDLFunction() << " - "
                  << e.GetSDLError() << "\n";
    }
}
