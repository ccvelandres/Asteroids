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
        AssetName                               assetName;
        AudioComponent                         *audioComponent;
        std::shared_ptr<core::audio::AudioData> audioData; /** Shared pointer to audio data */
        ALuint                                  sourceId;
        bool                                    isRelative    = false;
        bool                                    isLooping     = false;
        bool                                    isAnchored    = false;
        bool                                    isDirectional = false;
    };
} // namespace core::audio

/** @} endgroup Audio */
