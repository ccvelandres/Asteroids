#pragma once

/**
 * @file core/ecs/components/textureComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "renderComponent.hpp"

/**
 * @brief The TextureComponent allows rendering of mesh based render targets
 *
 */
class TextureComponent
{
private:
    AssetID m_textureId;
protected:
    TextureComponent();
    TextureComponent(AssetID textureId);
public:
    TextureComponent(TextureComponent &o)             = delete;
    TextureComponent &operator=(TextureComponent &o)  = delete;
    TextureComponent(TextureComponent &&o)            = default;
    TextureComponent &operator=(TextureComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    // void init() override{};
    // void update(time_ms delta) override;
    // void clean() override;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
