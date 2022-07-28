#pragma once

/**
 * @file core/ecs/components/renderComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "transformComponent.hpp"

/**
 * @brief The RenderComponent allows an entity to be rendered to the screen
 * 
 */
class RenderComponent : public Component
{
private:
    TransformComponent *m_transform;
protected:
    
    /** Protected Constructors (use entity to add components) */
    RenderComponent();
public:
    ~RenderComponent();
    RenderComponent(RenderComponent &o)             = delete;
    RenderComponent &operator=(RenderComponent &o)  = delete;
    RenderComponent(RenderComponent &&o)            = default;
    RenderComponent &operator=(RenderComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    virtual void getModelMatrix() = 0;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
