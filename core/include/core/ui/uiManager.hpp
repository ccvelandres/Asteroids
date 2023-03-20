#pragma once

/**
 * @file core/ui/UIManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup UI
 * @{
 */

#include "../time.hpp"  
#include "../ecs/component.hpp"

#include <memory>
#include <core/assets/font.hpp>

/**
 * @brief Manager for all UI related system
 *
 */
class UIManager
{
private:
    /** UIManager is a singleton */
    UIManager();
protected:
public:
    ~UIManager();
    UIManager(UIManager &o)             = delete;
    UIManager(UIManager &&o)            = delete;
    UIManager &operator=(UIManager &o)  = delete;
    UIManager &operator=(UIManager &&o) = delete;

    /**
     * @brief Get the Instance object
     *
     * @return UIManager& reference to UIManager
     */
    static UIManager &getInstance();

    bool init();
    void preUpdate();
    void fixedUpdate(const time_ms &delta);
    void update(const time_ms &delta);
    void postUpdate();
    void refresh();

    core::assets::Font loadFont(const AssetName &name);
};

/** @} endgroup UI */
