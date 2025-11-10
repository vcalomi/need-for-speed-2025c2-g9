#pragma once
#include <memory>
#include <string>

#include <SDL2pp/SDL2pp.hh>

class AudioSystem {
private:
    std::unique_ptr<SDL2pp::Mixer> mixer_;
    std::unique_ptr<SDL2pp::Music> backgroundMusic_;
    bool audioEnabled_;

public:
    AudioSystem();

    SDL2pp::Mixer* GetMixer();
    void PlayBackgroundMusic(const std::string& filepath, int loops = -1);
    void StopBackgroundMusic();
    void PlaySoundEffect(const std::string& filepath, int loops = 0);
};
