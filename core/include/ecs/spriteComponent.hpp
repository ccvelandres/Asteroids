#pragma once

#include <map>

#include "ecs.hpp"
#include <game.hpp>
#include <renderer.hpp>

class SpriteComponent : public Component
{
protected:
    std::map<std::size_t, std::vector<Texture>> m_sprites;
    std::map<std::size_t, std::vector<int>> m_spriteTiming;
    std::size_t m_lastUpdate = 0;
    std::size_t m_spriteIndex = 0;
    std::size_t m_timingIndex = 0;
    Texture m_texture;

public:
    // using SpriteTiming = std::vector<Texture>;
    SpriteComponent() {}
    // SpriteComponent(const std::string &textureName) : m_texture(Game::renderer().loadTexture(textureName)) {}

    void init() override
    {
    }

    void update(float delta) override;

    void setSpriteHandle(const std::size_t& spriteHandle);
    void addSpriteSet(std::size_t spriteHandle,
                      const std::string &filename,
                      const int &rW, const int &rH,
                      const int &rows, const int &cols,
                      const std::vector<int> &timing);

    void appendSpriteSet(std::size_t spriteHandle,
                         const std::string &filename,
                         const int &rw, const int &rH,
                         const int &posX, const int &posY,
                         const int &timing);
    Texture &texture();
};