#include <core/ecs/components/audioComponent.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>

#include <audio/audioClip_p.hpp>

AudioComponent::AudioComponent() {}
AudioComponent::~AudioComponent() = default;

void AudioComponent::addListener() {}

core::audio::AudioClip &AudioComponent::addAudioClip(const AssetName &audioName, const core::audio::AudioType &audioType) {
    L_TAG("AudioComponent::addAudioClip");
    std::shared_ptr<core::audio::AudioClip> clip = std::make_unique<core::audio::AudioClip>(core::audio::AudioClip(audioName, *this));

    // Register to audio manager
    core::audio::AudioManager::Instance().registerAudio(clip);

    core::audio::AudioClip *p = clip.get();
    this->m_audioClips.push_back(std::move(clip));
    return *p;
}
