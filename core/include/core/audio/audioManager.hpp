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
        void createSource(Audio::Internal &audio);
        void registerAudio(std::shared_ptr<Audio> &audio);

        void playAudio(Audio &clip);
        void pauseAudio(Audio &clip);
        void stopAudio(Audio &clip);

        friend class core::Manager<AudioManager>;
    };
}; // namespace core::audio

/** @} endgroup Audio */
