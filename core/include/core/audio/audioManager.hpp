#pragma once

/**
 * @file core/audio/audioManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Audio
 * @{
 */

#include "../manager.hpp"
#include "../time.hpp"
#include "../ecs/component.hpp"
#include "../ecs/components/audioComponent.hpp"
#include "../ecs/components/audioListener.hpp"

namespace core::audio
{
    /** @brief Container for audio data */
    struct AudioData;

    class AudioManager : public core::Manager<AudioManager>
    {
    private:
        static void driverCallback(void *userdata, uint8_t *stream, int len);
        void buildStream(uint8_t *stream, std::size_t streamSize);

    protected:
        AudioManager();
    public:
        ~AudioManager();

        bool init();
        void cleanup();
        void preUpdate();
        void fixedUpdate(const time_ms &delta);
        void update(const time_ms &delta);
        void postUpdate();
        void refresh();

        void setListener(const ComponentPtr<AudioListener> &audioListener);

        std::shared_ptr<AudioData> loadAudioFile(const AssetName &assetName);
        void registerAudio(std::shared_ptr<AudioClip> &audio);

        friend class core::Manager<AudioManager>;
    };
}; // namespace core::audio

/** @} endgroup Audio */
