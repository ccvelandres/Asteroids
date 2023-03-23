#pragma once

/**
 * @file core/graphics/renderer.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @defgroup Renderer
 * @brief Common implementation for the Rendering System under the Graphics module
 * @ingroup Graphics
 * @{
 */

#include "vertex.hpp"
#include "asset-manager.hpp"
#include "../time.hpp"

/**
 * @brief Base class for renderer implementations
 *
 */
class Renderer
{
private:
    std::string m_rendererName;
protected:
public:
    Renderer(const std::string &rendererName) : m_rendererName(rendererName) {}
    virtual ~Renderer() = default;

    virtual void init()                      = 0;
    virtual void update(const time_ms delta) = 0;
    virtual void clean()                     = 0;
    virtual void refresh()                   = 0;

    virtual bool renderBegin() = 0;
    virtual void render()      = 0;
    virtual void renderEnd()   = 0;

    virtual AssetManager &getAssetManager() = 0;

    const std::string      &rendererName() { return m_rendererName; }
};

/** @} endgroup Renderer */