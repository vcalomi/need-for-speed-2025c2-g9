#include "./audio_manager.h"

#define CHANNELS 2
#define CHUNK_SIZE 512
#define STARTING_VOLUME 32

using SDL2pp::Mixer;

AudioManager::AudioManager():
        mixer_(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNK_SIZE) {
    mixer_.SetMusicVolume(STARTING_VOLUME);
}

Mixer& AudioManager::GetMixer() { return mixer_; }

void AudioManager::PlayBackgroundMusic(const std::string& filepath, int loops) {
    try {
        backgroundMusic_ = std::make_unique<SDL2pp::Music>(filepath);
        mixer_.PlayMusic(*backgroundMusic_, loops);
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "Error playing music: " << e.GetSDLFunction() << " - " << e.GetSDLError()
                  << std::endl;
    }
}


void AudioManager::StopBackgroundMusic() { mixer_.HaltMusic(); }

void AudioManager::PlaySoundEffect(const std::string& filepath, int loops) {
    SDL2pp::Chunk sound(filepath);
    mixer_.PlayChannel(-1, sound, loops);
}
