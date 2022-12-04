#pragma once

/**
 * @file core/ecs/components/audioComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../../assets/asset-inventory.hpp"
#include "../component.hpp"

class AudioManager;
class AudioComponent;

typedef std::size_t AudioID;

/**
 * @brief Describes audio type and attenuation via distance
 *
 */
enum class AudioType
{
    Global,
    Spatial
};

class AudioClip
{
private:
    const AssetName m_assetName;
    AudioType       m_type;
    AudioComponent &m_component;

    uint8_t     m_volume;
    uint8_t     m_fade;
    bool        m_loop;
    std::size_t m_length;

    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
    // Use AudioComponent to create AudioClip
    AudioClip(const AssetName &assetName, AudioComponent &component);
public:
    ~AudioClip() = default;
    AudioClip(AudioClip &o)             = delete;
    AudioClip &operator=(AudioClip &o)  = delete;
    AudioClip(AudioClip &&o)            = default;
    AudioClip &operator=(AudioClip &&o) = default;

    AudioClip &setVolume(const uint8_t &volume);
    AudioClip &setFade(const uint8_t &fade);
    AudioClip &setLoop(const bool &loop);
    AudioClip &setLength(const std::size_t &length);

    uint8_t     getVolume();
    uint8_t     getFade();
    bool        getLoop();
    std::size_t getLength();

    /**
     * @brief Retrieve the audioComponent this clip is attached to
     *
     * @return AudioComponent& audioComponent
     */
    AudioComponent &getComponent();

    friend AudioManager;
    friend AudioComponent;
};

/**
 * @brief The AudioComponent enables to play/hear audio
 *
 */
class AudioComponent : public Component
{
private:
    std::vector<std::shared_ptr<AudioClip>> m_audioClips;
protected:
    /** Protected Constructors (use entity to add components) */
    AudioComponent();
public:

    ~AudioComponent();
    AudioComponent(AudioComponent &o)             = delete;
    AudioComponent &operator=(AudioComponent &o)  = delete;
    AudioComponent(AudioComponent &&o)            = default;
    AudioComponent &operator=(AudioComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    /**
     * @brief Attaches a listener to the attached object. Only one listener can exist
     * in a scene
     * @todo: add option for 3D, 2D, global sounds
     */
    void addListener();

    /**
     * @brief Attaches an audio clip to this component. The AudioClip lifespan is the same as the
     * audioComponent that owns it.
     * @todo: add an audioClip class? to handle audio data and audio controls
     */
    AudioClip *addAudioClip(const AssetName &audioName, const AudioType &audioType);

    /**
     * @brief Retrieves a pointer array to all audio clips attached to this component
     *
     * @return std::vector<AudioClip*>
     */
    std::vector<AudioClip *> getAudioClips();

    AudioClip &operator[](std::size_t id);

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
