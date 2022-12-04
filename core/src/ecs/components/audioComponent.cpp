#include <core/ecs/components/audioComponent.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>

AudioComponent::AudioComponent() {}
AudioComponent::~AudioComponent() = default;

void AudioComponent::addListener() {}

AudioClip *AudioComponent::addAudioClip(const AssetName &audioName, const AudioType &audioType) {
    L_TAG("AudioComponent::addAudioClip");
    std::shared_ptr<AudioClip> clip = AudioManager::getInstance().createAudioClip(audioName, *this);

    clip->m_loop = false;
    clip->m_fade = 0;
    clip->m_length = 0;
    clip->m_type = audioType;
    clip->m_volume = maxVolume;

    AudioClip *p = clip.get();
    this->m_audioClips.push_back(std::move(clip));
    return p;
}
