#pragma once

/**
 * @file core/audio/audio_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the audioClip class
 * @addtogroup Audio
 * @{
 */

#include <audio/audioManager_p.hpp>

namespace core::audio
{
    struct Audio::Internal
    {
        AssetName       assetName;
        AudioType       type;
        AudioComponent &audioComponent;
        std::size_t offset;

        ALuint              sourceId;
        bool                isPlaying;
        TransformComponent *transform;

        /** Shared pointer to audio data */
        std::shared_ptr<core::audio::AudioData> audioData;
    };
} // namespace core::audio

/** @} endgroup Audio */
