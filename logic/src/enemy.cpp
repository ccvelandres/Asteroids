#include <enemy.hpp>
#include <core/utils/logging.hpp>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::init()
{
    L_TAG("Player::init");

    m_transform = &this->addComponent<TransformComponent>();

    m_transform->setScale(glm::vec3(2));
    m_isActive = false;
}

void Enemy::preUpdate() { L_TAG("Player::preUpdate"); }

void Enemy::update(time_ms delta)
{
    L_TAG("Player::update");

    if (!m_isActive) return;
}

void Enemy::postUpdate() { L_TAG("Player::postUpdate"); }

void Enemy::spawn(int type, const glm::vec3 &position)
{
    L_TAG("Player::spawn");

    assert(m_isActive == false);
    m_transform->setPosition(position);
}