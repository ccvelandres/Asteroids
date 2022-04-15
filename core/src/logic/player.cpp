#include <logic/player.hpp>
#include <game.hpp>

Player::Player()
{
}

Player::~Player()
{
}

void Player::init()
{
    m_transform = &this->addComponent<TransformComponent>();
    m_sprite = &this->addComponent<SpriteComponent>();
    m_render = &this->addComponent<RenderComponent>();

    m_transform->scale = 2;

    m_sprite->appendSpriteSet(0, "assets/ship.png", 16, 24, 0, 0, time_ms(60));
    m_sprite->appendSpriteSet(0, "assets/ship.png", 16, 24, 0, 24, time_ms(60));
    m_sprite->appendSpriteSet(1, "assets/ship.png", 16, 24, 16, 0, time_ms(60));
    m_sprite->appendSpriteSet(1, "assets/ship.png", 16, 24, 16, 24, time_ms(60));
    m_sprite->appendSpriteSet(2, "assets/ship.png", 16, 24, 32, 0, time_ms(60));
    m_sprite->appendSpriteSet(2, "assets/ship.png", 16, 24, 32, 24, time_ms(60));
    m_sprite->appendSpriteSet(3, "assets/ship.png", 16, 24, 48, 0, time_ms(60));
    m_sprite->appendSpriteSet(3, "assets/ship.png", 16, 24, 48, 24, time_ms(60));
    m_sprite->appendSpriteSet(4, "assets/ship.png", 16, 24, 64, 0, time_ms(60));
    m_sprite->appendSpriteSet(4, "assets/ship.png", 16, 24, 64, 24, time_ms(60));

    /** Preallocate bullet objects */
    m_bullets = Game::entityManager().addEntities<Bullet>(32, *this);

    speed = 300;
    shootInterval = time_ms(100);
    m_lastShoot = time_ds(0);
}

void Player::preUpdate()
{
}

void Player::update(time_ds delta)
{
    /** Movement */
    Vector3F l_inputForce(0);
    if (Game::inputManager().isPressed(SDL_SCANCODE_UP))
        l_inputForce.y = -1;
    else if (Game::inputManager().isPressed(SDL_SCANCODE_DOWN))
        l_inputForce.y = 1;
    if (Game::inputManager().isPressed(SDL_SCANCODE_LEFT))
        l_inputForce.x = -1;
    else if (Game::inputManager().isPressed(SDL_SCANCODE_RIGHT))
        l_inputForce.x = 1;

    Vector3F::normalize(l_inputForce);
    if (l_inputForce)
    {
        Vector3F offset = l_inputForce * delta.count() * speed;
        std::cout << offset << std::endl;
        m_transform->position += offset;
    }

    /** Shooting */
    if (Game::inputManager().isPressed(SDL_SCANCODE_SPACE) &&
        (m_lastShoot + shootInterval < Time::time()))
    {
        /** Look for free bullet object */
        auto it = std::find_if(m_bullets.begin(),
                               m_bullets.end(),
                               [](std::shared_ptr<Bullet> &p)
                               {
                                   return !p->isShot();
                               });
        if (it != m_bullets.end())
        {
            Bullet &bullet = (*it->get());
            bullet.shoot(m_transform->position + Vector3F(0, 0, 0),
                         Vector3F(0, -500, 0));
            m_lastShoot = Time::time();
            std::cout << "Shooting bullet object" << std::endl;
        }
    }


    std::cout << "Player@ " << m_transform->position << std::endl;
    std::cout << " force@ " << l_inputForce << std::endl;
    {
        int activeBullets = 0;
        std::for_each(m_bullets.begin(), m_bullets.end(),
            [&activeBullets](std::shared_ptr<Bullet> &b) {
                if (b->isShot()) activeBullets++;
            });
        std::cout << "   acb@ " << activeBullets << std::endl;
    }
}

void Player::postUpdate()
{
}