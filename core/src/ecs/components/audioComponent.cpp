#include <core/ecs/components/audioComponent.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>

AudioComponent::AudioComponent() {}
AudioComponent::~AudioComponent() = default;

void AudioComponent::addListener() {}

AudioClip &AudioComponent::addAudioClip(const AssetName &audioName, const AudioType &audioType) {
    L_TAG("AudioComponent::addAudioClip");
    std::unique_ptr<AudioClip> clip = std::make_unique<AudioClip>(AudioClip(audioName, *this));

    AudioClip *p = clip.get();
    this->m_audioClips.push_back(std::move(clip));
    return *p;
}
