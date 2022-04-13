#include <ecs/spriteComponent.hpp>

void SpriteComponent::update(float delta)
{
    m_lastUpdate += delta;

    assert(m_sprites.find(m_spriteIndex) != m_sprites.end());
    assert(m_spriteTiming[m_spriteIndex].size() >= m_timingIndex);

    int e = m_spriteTiming[m_spriteIndex][m_timingIndex];
    if (m_lastUpdate > e)
    {
        m_lastUpdate %= e;
        if (++m_timingIndex >= m_spriteTiming[m_spriteIndex].size())
            m_timingIndex = 0;
    }
}

void SpriteComponent::setSpriteHandle(const std::size_t& spriteHandle) {
    this->m_spriteIndex
}

Texture &SpriteComponent::texture()
{
    return m_sprites[m_spriteIndex][m_timingIndex];
}

void SpriteComponent::addSpriteSet(std::size_t spriteHandle,
                                   const std::string &filename,
                                   const int &rW, const int &rH,
                                   const int &rows, const int &cols,
                                   const std::vector<int> &timing)
{
    std::vector<Texture> spriteSet = Game::renderer().loadTextureAtlas(filename,
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
                     const int &timing)
{
    std::vector<Texture> &v = m_sprites[spriteHandle];
    std::vector<int> &t = m_spriteTiming[spriteHandle];

    Texture tex = Game::renderer().loadTexture(filename);
    tex.setPosition(posX, posY);
    tex.setSize(rW, rH);

    v.emplace_back(tex);
    t.push_back(timing);
}