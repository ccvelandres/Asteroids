#pragma once

/**
 * @file core/ecs/components/templateComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "transformComponent.hpp"
#include "../../graphics/renderer/renderer.hpp"

#include <bitset>

/**
 * @brief DESCRIPTION HERE
 *
 */
class TemplateComponent : public Component
{
private:
protected:
    /** Protected Constructors (use entity to add components) */
    TemplateComponent();
public:

    ~TemplateComponent();
    TemplateComponent(TemplateComponent &o)             = delete;
    TemplateComponent &operator=(TemplateComponent &o)  = delete;
    TemplateComponent(TemplateComponent &&o)            = default;
    TemplateComponent &operator=(TemplateComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
