#pragma once

#include <SDL2/SDL.h>

#include <map>
#include <vector>
#include <memory>
#include <iostream>

class Texture;
class TextureRegion;
class Renderer;

enum TextureType
{
    SIMPLE,
    ATLAS,
    REGION
};

class Texture
{
private:
    std::shared_ptr<SDL_Texture> m_texture;
    int posX, posY;
    int width, height;
    uint32_t m_pixelFormat;
    int m_textureAccess;
    TextureType m_type;

    static void deallocTexture(SDL_Texture *tex)
    {
        std::cout << "TextureDealloc: " << tex << std::endl;
        SDL_DestroyTexture(tex);
    }

protected:
public:
    Texture() {}
    Texture(Texture &&) = default;
    Texture(Texture &e) = default;
    // {
    //     std::cout << "Texture Copied" << std::endl;
    // }
    Texture(std::shared_ptr<SDL_Texture> texture) : m_type(SIMPLE),
                                                    m_texture(texture)
    {
        SDL_QueryTexture(m_texture.get(), &m_pixelFormat, &m_textureAccess, &width, &height);
    }
    Texture(std::shared_ptr<SDL_Texture> texture,
            const int &posX,
            const int &posY,
            const int &width,
            const int &height) : m_type(REGION),
                                 m_texture(texture),
                                 posX(posX),
                                 posY(posY),
                                 width(width),
                                 height(height)
    {
        int w, h;
        SDL_QueryTexture(m_texture.get(), &m_pixelFormat, &m_textureAccess, &w, &h);
        assert(w >= posX + width);
        assert(h >= posY + height);
    }
    ~Texture() {}

    void setSize(const int &w, const int &h)
    {
        this->width = w;
        this->height = h;
    }

    void setPosition(const int &x, const int& y) {
        this->posX = x;
        this->posY = y;
    }

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

    Texture loadTexture(const std::string &filename)
    {
        return Texture(loadTexturePtr(filename));
    }
    std::vector<Texture> loadTextureAtlas(const std::string &filename,
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

    void drawTexture(Texture &texture,
                     const int &dstX,
                     const int &dstY);

    void drawTexture(Texture &texture,
                     const int &dstX,
                     const int &dstY,
                     const int &srcX,
                     const int &srcY,
                     const int &sizeX,
                     const int &sizeY);
};
