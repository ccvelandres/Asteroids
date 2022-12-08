#pragma once

/**
 * @file core/audio/audioManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Audio
 * @{
 */

#include "../time.hpp"
#include "../ecs/component.hpp"
#include "../ecs/components/audioComponent.hpp"

namespace core::audio::manager
{
    /** Initialize Manager */

    /**
     * @brief Initialize Audio Manager
     * 
     * @return true on success
     * @return false on failure
     */
    bool init();

    /**
     * @brief Preupdate step for manager
     * 
     */
    void preUpdate();

    /**
     * @brief Fixed interval update for manager
     * 
     * @param delta time in ms
     */
    void fixedUpdate(const time_ms &delta);

    /**
     * @brief Scaled time update for manager
     * 
     * @param delta time in ms
     */
    void update(const time_ms &delta);

    /**
     * @brief Post update step for manager
     * 
     */
    void postUpdate();

    /**
     * @brief Refresh step for manager. Cleans unused resources
     * 
     */
    void refresh();
};

/** @} endgroup Audio */
