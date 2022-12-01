#include <bullet.hpp>

#include <core/utils/logging.hpp>

Bullet::Bullet(Entity *parent) : m_parent(parent), m_isShot(false) {}

Bullet::~Bullet() {}

void Bullet::init()
{
    m_transform = &this->addComponent<TransformComponent>();

    m_transform->setScale(glm::vec3(2));
}

void Bullet::preUpdate() {}

void Bullet::update(time_ms delta)
{
    L_TAG("Bullet::update");

    if (!m_isShot) return;

    glm::vec3 pos = m_transform->getPosition();

    if ((pos.y < -50) || (m_shootTime + time_ms(5000) < Game::time()->getTime<time_ms>()))
    {
        m_isShot = false;
        L_DEBUG("Freeing bullet object");
    }

    pos += (m_velocity / 1000.f);
    pos *= delta.count();

    m_transform->setPosition(pos);
}

void Bullet::postUpdate() {}

void Bullet::shoot(const glm::vec3 &position, const glm::vec3 &velocity)
{
    L_TAG("Bullet::shoot");

    assert(m_isShot == false);

    L_DEBUG("Shooting bullet object");

    m_transform->setPosition(position);
    m_velocity            = velocity;

    m_isShot    = true;
    m_shootTime = Game::time()->getTime<time_ms>();
}