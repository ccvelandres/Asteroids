#include <ecs/spriteComponent.hpp>

SpriteComponent::SpriteComponent()
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::init() {
    this->m_lastUpdate = time_ms(0);
    this->m_spriteIndex = 0;
    this->m_timingIndex = 0;
    this->m_sprites.clear();
    this->m_spriteTiming.clear();
}

void SpriteComponent::update(time_ms delta)
{
    if (!m_sprites.size())
        return;
    m_lastUpdate += delta;

    assert(m_sprites.find(m_spriteIndex) != m_sprites.end());
    assert(m_spriteTiming[m_spriteIndex].size() >= m_timingIndex);

    time_ds e = m_spriteTiming[m_spriteIndex][m_timingIndex];
    if (m_lastUpdate > e)
    {
        m_lastUpdate = time_ms(0);
        if (++m_timingIndex >= m_spriteTiming[m_spriteIndex].size())
            m_timingIndex = 0;
    }
}

void SpriteComponent::setSpriteHandle(const std::size_t &spriteHandle)
{
    this->m_spriteIndex = spriteHandle;
}

Texture2D &SpriteComponent::texture()
{
    return m_sprites[m_spriteIndex][m_timingIndex];
}

void SpriteComponent::addSpriteSet(std::size_t spriteHandle,
                                   const std::string &filename,
                                   const int &rW, const int &rH,
                                   const int &rows, const int &cols,
                                   const std::vector<time_ds> &timing)
{
    std::vector<Texture2D> spriteSet = Game::renderer()->loadTextureAtlas(filename,
                                                                         rW, rH,
                                                                         rows, cols);

    assert(spriteSet.size() == timing.size());

    m_sprites.insert(std::make_pair(spriteHandle, std::move(spriteSet)));
    m_spriteTiming.insert(std::make_pair(spriteHandle, timing));
    m_spriteIndex = 0;
}

void SpriteComponent::appendSpriteSet(std::size_t spriteHandle,
                                      const std::string &filename,
                                      const int &rW, const int &rH,
                                      const int &posX, const int &posY,
                                      const time_ds &timing)
{
    std::vector<Texture2D> &v = m_sprites[spriteHandle];
    std::vector<time_ds> &t = m_spriteTiming[spriteHandle];

    Texture2D tex = Game::renderer()->loadTexture(filename);
    tex.position(Vector2(posX, posY));
    tex.size(Vector2(rW, rH));

    v.emplace_back(tex);
    t.push_back(timing);
}