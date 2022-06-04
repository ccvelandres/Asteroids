#include <bullet.hpp>

#include <utils/logging.hpp>

Bullet::Bullet(Entity &parent) : m_parent(&parent),
                                 m_isShot(false)
{
}

Bullet::~Bullet()
{
}

void Bullet::init()
{
    m_transform = &this->addComponent<TransformComponent>();

    m_transform->scale = 2;
}

void Bullet::preUpdate()
{
}

void Bullet::update(time_ms delta)
{
    L_TAG("Bullet::update");

    if (!m_isShot)
        return;

    if ((m_transform->position.y < -50) ||
        (m_shootTime + time_ms(5000) < Game::time()->getTime<time_ms>()))
    {
        m_isShot = false;
        L_DEBUG("Freeing bullet object");
    }

    m_transform->position += (m_velocity / 1000.f) * delta.count();
}

void Bullet::postUpdate()
{
}

void Bullet::shoot(const Vector3F &position,
                   const Vector3F &velocity)
{
    L_TAG("Bullet::shoot");
    
    assert(m_isShot == false);

   L_DEBUG("Shooting bullet object");

    m_transform->position = position;
    m_velocity = velocity;

    m_isShot = true;
    m_shootTime = Game::time()->getTime<time_ms>();
}