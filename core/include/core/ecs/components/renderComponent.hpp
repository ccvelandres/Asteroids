#pragma once

#include "../component.hpp"
#include "transformComponent.hpp"

class RenderComponent : public Component
{
private:
protected:
    RenderComponent() : transform(this->entity().getComponent<TransformComponent>()){}
    
    TransformComponent &transform;
public:

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void update(time_ms delta) override;
    // void clean() override;


    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};