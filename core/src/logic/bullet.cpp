#include <logic/bullet.hpp>

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

void Bullet::update(time_ds delta)
{
    if (!m_isShot)
        return;

    if (m_shootTime + 5000 < SDL_GetTicks())
    {
        m_isShot = false;
        std::cout << "Freeing bullet object" << std::endl;
    }

    m_transform->position += m_velocity * delta.count();

    /** check if we are oob */
    if (m_transform->position.y < -50) {
        m_isShot = false;
    }
}

void Bullet::postUpdate()
{
}

void Bullet::shoot(const Vector3F &position,
                   const Vector3F &velocity)
{
    assert(m_isShot == false);

    m_transform->position = position;
    m_velocity = velocity;

    m_isShot = true;
    m_render->enabled(true);
    m_shootTime = SDL_GetTicks();
}