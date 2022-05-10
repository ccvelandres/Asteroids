#include <logic/bullet.hpp>

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
    m_sprite = &this->addComponent<SpriteComponent>();
    m_render = &this->addComponent<RenderComponent>();

    m_transform->scale = 2;

    m_sprite->appendSpriteSet(0, "assets/laser-bolts-sheet.png", 16, 16, 0, 0, time_ms(60));
    m_sprite->appendSpriteSet(0, "assets/laser-bolts-sheet.png", 16, 16, 16, 0, time_ms(60));
    m_sprite->appendSpriteSet(1, "assets/laser-bolts-sheet.png", 16, 16, 0, 16, time_ms(60));
    m_sprite->appendSpriteSet(1, "assets/laser-bolts-sheet.png", 16, 16, 16, 16, time_ms(60));

    /** disable rendering until we shoot */
    m_render->enabled(false);
}

void Bullet::preUpdate()
{
}

void Bullet::update(time_ms delta)
{
    if (!m_isShot)
        return;

    if ((m_transform->position.y < -50) ||
        (m_shootTime + time_ms(5000) < Game::time()->getTime<time_ms>()))
    {
        m_isShot = false;
        logging::trace("{},{}: Freeing bullet object",
                       __LINE__, __func__);
    }

    m_transform->position += (m_velocity / 1000.f) * delta.count();
}

void Bullet::postUpdate()
{
}

void Bullet::shoot(const Vector3F &position,
                   const Vector3F &velocity)
{
    assert(m_isShot == false);

    logging::debug("{},{}: Shooting bullet object",
                   __LINE__, __func__);

    m_transform->position = position;
    m_velocity = velocity;

    m_isShot = true;
    m_render->enabled(true);
    m_shootTime = Game::time()->getTime<time_ms>();
}