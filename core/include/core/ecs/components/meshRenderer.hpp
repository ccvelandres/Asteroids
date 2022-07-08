#pragma once

#include "../ecs.hpp"
#include "renderComponent.hpp"

class MeshRenderer : public RenderComponent
{
private:
    
protected:
    MeshRenderer() {}
public:

    /** Component overrides */
    // void awake() override;
    // void init() override
    // void update(time_ms delta) override;
    // void clean() override;


    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};