#pragma once

#include <ecs/ecs.hpp>
#include <ecs/transformComponent.hpp>

constexpr float MAX_VELOCITY = 400;
constexpr float MAX_ACCELERATION = 1500;
constexpr float MIN_VELOCITY = 10.f;
constexpr float MIN_ACCELERATION = 10.f;

class PhysicsComponent : public Component
{
private:
    TransformComponent *transform;

protected:
public:
    Vector3F velocity;
    Vector3F acceleration;
    Vector3F friction;
    Vector3F maxVelocity = MAX_VELOCITY;
    Vector3F maxAcceleration = MAX_ACCELERATION;

    PhysicsComponent() : velocity(), acceleration(), friction() {}

    void init() override
    {
        transform = &this->entity().getComponent<TransformComponent>();
    }

    void update(time_ds delta) override
    {
        // time_ds deltaS = delta / 1000;
        // /** Clamp acceleration before computing velocity */
        // if (fabs(acceleration.x) > maxAcceleration.x)
        //     acceleration.x = (acceleration.x > 0.f ? maxAcceleration.x : -maxAcceleration.x);
        // if (fabs(acceleration.y) > maxAcceleration.y)
        //     acceleration.y = (acceleration.y > 0.f ? maxAcceleration.y : -maxAcceleration.y);
        // if (fabs(acceleration.z) > maxAcceleration.z)
        //     acceleration.z = (acceleration.z > 0.f ? maxAcceleration.z : -maxAcceleration.z);

        // /** Get velocity */
        // velocity += (acceleration * deltaS);
        // /** Clamp velocity */
        // if (fabs(velocity.x) < MIN_VELOCITY)
        //     velocity.x = 0;
        // if (fabs(velocity.y) < MIN_VELOCITY)
        //     velocity.y = 0;

        // /** Apply friction if velocity is not zero */
        // if (!fabs(acceleration.x))
        //     if (fabs(velocity.x))
        //         velocity.x += deltaS * (velocity.x > 0 ? -friction.x : friction.x);
        // if (!fabs(acceleration.y))
        //     if (fabs(velocity.y))
        //         velocity.y += deltaS * (velocity.y > 0 ? -friction.y : friction.y);

        // /** Clamp acceleration before computing position */
        // if (fabs(velocity.x) > maxVelocity.x)
        //     velocity.x = (velocity.x > 0.f ? maxVelocity.x : -maxVelocity.x);
        // if (fabs(velocity.y) > maxVelocity.y)
        //     velocity.y = (velocity.y > 0.f ? maxVelocity.y : -maxVelocity.y);
        // if (fabs(velocity.z) > maxVelocity.z)
        //     velocity.z = (velocity.z > 0.f ? maxVelocity.z : -maxVelocity.z);

        // /** @todo: maybe move computation of position with collision detection */
        // /** Get position */
        // transform->position += (velocity * deltaS);
    }
};