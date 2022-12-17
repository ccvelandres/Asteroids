#include <core/ecs/components/audioListener.hpp>
#include <core/ecs/entity.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>

AudioListener::AudioListener() {}
AudioListener::~AudioListener() = default;

void AudioListener::init()
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

void AudioListener::listen()
{
    L_TAG("AudioListener::listen");
    auto listener = this->m_entity->getComponentPtr<AudioListener>();
    L_ASSERT(listener.get() == this, "Component mismatch for this entity");
    core::audio::AudioManager::Instance().setListener(listener);
}
