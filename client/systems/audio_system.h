#pragma once
#include <memory>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../events/event.h"

class AudioSystem {
private:
    std::unique_ptr<SDL2pp::Mixer> mixer_;
    std::unique_ptr<SDL2pp::Music> backgroundMusic_;
    bool audioEnabled_;
    EventBus& eventBus_;

public:
    explicit AudioSystem(EventBus& bus);

    SDL2pp::Mixer* GetMixer();
    void PlayBackgroundMusic(const std::string& filepath, int loops = -1);
    void StopBackgroundMusic();
    void PlaySoundEffect(const std::string& filepath, int loops = 0);
};
