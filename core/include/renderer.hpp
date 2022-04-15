#pragma once

#include <SDL2/SDL.h>
#include <utils/vector.hpp>

#include <map>
#include <vector>
#include <memory>
#include <iostream>

class Texture2D;
class Renderer;

class Texture2D
{
private:
    std::shared_ptr<SDL_Texture> m_texture;
    Vector2 m_position;
    Vector2 m_size;
    uint32_t m_pixelFormat;
    int m_textureAccess;

    static void deallocTexture(SDL_Texture *tex)
    {
        std::cout << "TextureDealloc: " << tex << std::endl;
        SDL_DestroyTexture(tex);
    }

protected:
public:
    Texture2D() {}
    Texture2D(Texture2D &) = default;
    Texture2D(Texture2D &&) = default;

    Texture2D(std::shared_ptr<SDL_Texture> texture);
    Texture2D(std::shared_ptr<SDL_Texture> texture,
              const Vector2 &position,
              const Vector2 &size);
    ~Texture2D() {}

    Vector2 position() { return m_position; }
    void position(const Vector2 &position) { m_position = position; }

    Vector2 size() { return m_size; }
    void size(const Vector2 &size) { m_size = size; }

    friend Renderer;
};

/**
 * @brief This renderer currently only works with native SDL2 so it's 2D
 * Future plans is to support OpenGL or Vulkan or whatever for 3D
 */
class Renderer
{
private:
    SDL_Renderer *m_renderer;

    std::map<std::string, std::weak_ptr<SDL_Texture>> m_textures;
    std::shared_ptr<SDL_Texture> loadTexturePtr(const std::string &filename);

protected:
public:
    Renderer();
    ~Renderer();

    Texture2D loadTexture(const std::string &filename)
    {
        return Texture2D(loadTexturePtr(filename));
    }
    std::vector<Texture2D> loadTextureAtlas(const std::string &filename,
                                            const int &regionWidth,
                                            const int &regionHeight,
                                            const int &rows,
                                            const int &cols);

    void refresh();

    /** Starts the rendering process */
    void startRender();
    /** Ends the render process and presents to screen */
    void endRender();

    void drawLine(const float &p1x,
                  const float &p1y,
                  const float &p2x,
                  const float &p2y);

    void drawTexture2D(Texture2D &texture,
                       const float &dstX,
                       const float &dstY);

    void drawTexture2D(Texture2D &texture,
                       const float &dstX,
                       const float &dstY,
                       const float &scaleX,
                       const float &scaleY);

    void drawTexture2D(Texture2D &texture,
                       const float &dstW,
                       const float &dstH,
                       const float &dstX,
                       const float &dstY,
                       const int &srcW,
                       const int &srcH,
                       const int &srcX,
                       const int &srcY);
};
