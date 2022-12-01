#include <player.hpp>
#include <core/game.hpp>
#include <core/time.hpp>
#include <core/ecs/entityManager.hpp>

#include <core/utils/logging.hpp>

Player::Player() {}

Player::~Player() {}

void Player::init()
{
    L_TAG("Player::init");

    m_transform = &this->addComponent<TransformComponent>();

    m_transform->setScale(glm::vec3(2));

    /** Preallocate bullet objects */
    m_bullets = Game::entityManager()->addEntities<Bullet>(32, this);

    speed         = 300;
    shootInterval = time_ms(100);
    m_lastShoot   = time_ds(0);
}

void Player::preUpdate() { L_TAG("Player::preUpdate"); }

void Player::update(time_ms delta)
{
    L_TAG("Player::update");

    float d = Game::time()->scaledDeltaTime<time_fs>().count();
    /** Movement */
    glm::vec3 l_inputForce(0);
    if (Game::inputManager()->isPressed(SDL_SCANCODE_UP))
        l_inputForce.y = -1;
    else if (Game::inputManager()->isPressed(SDL_SCANCODE_DOWN))
        l_inputForce.y = 1;
    if (Game::inputManager()->isPressed(SDL_SCANCODE_LEFT))
        l_inputForce.x = -1;
    else if (Game::inputManager()->isPressed(SDL_SCANCODE_RIGHT))
        l_inputForce.x = 1;

    l_inputForce = glm::normalize(l_inputForce);
    if (l_inputForce.length())
    {
        glm::vec3 offset = l_inputForce * (delta.count() * (speed / 1000.f));
        m_transform->translate(offset);
    }

    /** Shooting */
    if (Game::inputManager()->isPressed(SDL_SCANCODE_SPACE)
        && (m_lastShoot + shootInterval < Game::time()->unscaledTime()))
    {
        /** Look for free bullet object */
        auto it =
            std::find_if(m_bullets.begin(), m_bullets.end(), [](Bullet *p) { return !p->isShot(); });
        if (it != m_bullets.end())
        {
            Bullet *bullet = (*it);
            bullet->shoot(m_transform->getPosition() + glm::vec3(0, 0, 0), glm::vec3(0, -500, 0));
            m_lastShoot = Game::time()->unscaledTime();
            L_TRACE("{},{}: Shooting bullet object", __LINE__, __func__);
        }
    }

    {
        int activeBullets = 0;
        std::for_each(m_bullets.begin(), m_bullets.end(), [&activeBullets](Bullet *b) {
            if (b->isShot()) activeBullets++;
        });
    }
}

void Player::postUpdate() { L_TAG("Player::postUpdate"); }