#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <vector>



AudioManager::AudioManager()  = default;
AudioManager::~AudioManager() = default;

AudioManager &AudioManager::getInstance()
{
    /** Meyers Singleton (another option is std::call_once) */
    static AudioManager instance = AudioManager();
    return instance;
}

void AudioManager::init()
{
    L_TAG("AudioManager::init");

}

void AudioManager::preUpdate()
{
    L_TAG("AudioManager::preUpdate");

}

void AudioManager::fixedUpdate(const time_ms &delta) {
    L_TAG("AudioManager::fixedUpdate");

}

void AudioManager::update(const time_ms &delta)
{
    L_TAG("AudioManager::update");
    
}