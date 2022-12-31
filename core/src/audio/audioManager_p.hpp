#pragma once

/**
 * @file core/audio/audioManager_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the audioManager
 * @addtogroup Audio
 * @{
 */

#include <core/audio/audioManager.hpp>
#include <core/ecs/components/audioComponent.hpp>

#include <AL/al.h>
#include <AL/alc.h>
#include <SDL2/SDL_sound.h>

namespace core::audio
{
    struct SoundDeleter
    {
        void operator()(Sound_Sample *s) { Sound_FreeSample(s); }
    };

    typedef std::unique_ptr<Sound_Sample, SoundDeleter> UniqueSoundSample;

    struct AudioData
    {
        ALuint   bufferId;
        ALint    frequency;
        ALint    bitDepth;
        ALint    channels;
        ALsizei  size;
        ALenum   format;
        uint32_t duration;
    };

}; // namespace core::audio

/** @} endgroup Audio */
