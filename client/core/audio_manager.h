#pragma once

#include <memory>
#include <string>

#include <SDL2pp/SDL2pp.hh>

class AudioManager {
public:
    AudioManager();
    void PlayBackgroundMusic(const std::string& filepath, int loops = -1);
    void StopBackgroundMusic();
    void PlaySoundEffect(const std::string& filepath, int loops = 0);
    SDL2pp::Mixer& GetMixer();

private:
    SDL2pp::Mixer mixer_;
    std::unique_ptr<SDL2pp::Music> backgroundMusic_;
};
