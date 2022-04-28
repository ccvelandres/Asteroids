#include <logic/enemy.hpp>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::init()
{
    m_transform = &this->addComponent<TransformComponent>();
    m_sprite = &this->addComponent<SpriteComponent>();
    m_render = &this->addComponent<RenderComponent>();

    m_transform->scale = 2;

    m_sprite->appendSpriteSet(0, "assets/enemy-small.png", 16, 16, 0, 0, time_ms(60));
    m_sprite->appendSpriteSet(0, "assets/enemy-small.png", 16, 16, 16, 0, time_ms(60));
    m_sprite->appendSpriteSet(1, "assets/enemy-medium.png", 32, 16, 0, 0, time_ms(60));
    m_sprite->appendSpriteSet(1, "assets/enemy-medium.png", 32, 16, 32, 0, time_ms(60));

    m_isActive = false;
}

void Enemy::preUpdate()
{
}

void Enemy::update(time_ms delta)
{
    if (!m_isActive)
        return;

}

void Enemy::postUpdate()
{
}

void Enemy::spawn(int type, const Vector3F &position)
{
    assert(m_isActive == false);
    m_sprite->setSpriteHandle(type);
    m_transform->position = position;
}