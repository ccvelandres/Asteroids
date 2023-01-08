#include <core/ecs/components/audioComponent.hpp>
#include <core/ecs/entity.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>

#include <core/audio/audio.hpp>
#include <audio/audio_p.hpp>

AudioComponent::AudioComponent() {}
AudioComponent::~AudioComponent() = default;

void AudioComponent::init()
{
    if(this->m_entity->hasComponent<TransformComponent>())
    {
        this->m_transform = &this->m_entity->getComponent<TransformComponent>();
    }
    else
    {
        this->m_transform = &this->m_entity->addComponent<TransformComponent>();
    }
}

void AudioComponent::addListener() {}

core::audio::Audio &AudioComponent::addAudioClip(const AssetName &audioName, const bool relative)
{
    L_TAG("AudioComponent::addAudioClip");
    std::shared_ptr<core::audio::Audio> clip =
        std::make_shared<core::audio::Audio>(audioName, *this);

    // Register to audio manager
    core::audio::AudioManager::Instance().registerAudio(clip);

    core::audio::Audio *p = clip.get();
    this->m_audioClips.push_back(std::move(clip));
    return *p;
}
