#pragma once

#include <map>

#include "ecs.hpp"
#include <game.hpp>
#include <renderer.hpp>
#include <time.hpp>

class SpriteComponent : public Component
{
protected:
    std::map<std::size_t, std::vector<Texture2D>> m_sprites;
    std::map<std::size_t, std::vector<time_ds>> m_spriteTiming;
    time_ds m_lastUpdate = time_ds(0);
    std::size_t m_spriteIndex = 0;
    std::size_t m_timingIndex = 0;
    Texture2D m_texture;

public:
    SpriteComponent();
    ~SpriteComponent();

    /** Component overrides */
    // void awake() override;
    // void init() override;
    void update(time_ds delta) override;
    // void clean() override;

    void setSpriteHandle(const std::size_t& spriteHandle);
    void addSpriteSet(std::size_t spriteHandle,
                      const std::string &filename,
                      const int &rW, const int &rH,
                      const int &rows, const int &cols,
                      const std::vector<time_ds> &timing);

    void appendSpriteSet(std::size_t spriteHandle,
                         const std::string &filename,
                         const int &rw, const int &rH,
                         const int &posX, const int &posY,
                         const time_ds &timing);
    Texture2D &texture();
};