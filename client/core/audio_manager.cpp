#include "./audio_manager.h"

#include <iostream>

#define CHANNELS 2
#define CHUNK_SIZE 512
#define STARTING_VOLUME 32

using SDL2pp::Mixer;

AudioManager::AudioManager(): audioEnabled_(true) {
#ifdef __linux__
    // Detectar si estamos dentro de WSL
    if (std::getenv("WSL_DISTRO_NAME")) {
        std::cerr << "[AudioManager] Detected WSL environment. Audio disabled.\n";
        audioEnabled_ = false;
        return;  // No inicializamos SDL_mixer
    }
#endif

    try {
        mixer_ = std::make_unique<Mixer>(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS,
                                         CHUNK_SIZE);
        mixer_->SetMusicVolume(STARTING_VOLUME);
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "[AudioManager] Failed to initialize audio: " << e.GetSDLFunction() << " - "
                  << e.GetSDLError() << "\n";
        audioEnabled_ = false;
    }
}

Mixer* AudioManager::GetMixer() { return audioEnabled_ ? mixer_.get() : nullptr; }

void AudioManager::PlayBackgroundMusic(const std::string& filepath, int loops) {
    if (!audioEnabled_)
        return;

    try {
        backgroundMusic_ = std::make_unique<SDL2pp::Music>(filepath);
        mixer_->PlayMusic(*backgroundMusic_, loops);
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "[AudioManager] Error playing music: " << e.GetSDLFunction() << " - "
                  << e.GetSDLError() << "\n";
    }
}

void AudioManager::StopBackgroundMusic() {
    if (audioEnabled_)
        mixer_->HaltMusic();
}

void AudioManager::PlaySoundEffect(const std::string& filepath, int loops) {
    if (!audioEnabled_)
        return;

    try {
        SDL2pp::Chunk sound(filepath);
        mixer_->PlayChannel(-1, sound, loops);
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "[AudioManager] Error playing sound effect: " << e.GetSDLFunction() << " - "
                  << e.GetSDLError() << "\n";
    }
}
