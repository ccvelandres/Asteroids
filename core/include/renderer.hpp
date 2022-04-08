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
    TextureType m_type;

    static void deallocTexture(SDL_Texture *tex)
    {
        std::cout << "TextureDealloc: " << tex << std::endl;
        SDL_DestroyTexture(tex);
    }

protected:
public:
    Texture() {}
    Texture(std::shared_ptr<SDL_Texture> texture) : m_type(SIMPLE),
                                                    m_texture(texture) {}
    Texture(SDL_Texture *texture) : m_type(SIMPLE),
                                    m_texture(texture, deallocTexture)
    {
        std::cout << "TextureCreated: " << texture << std::endl;
    }

    friend Renderer;
};

class TextureRegion : public Texture
{
private:
    const int &posX, posY, sizeX, sizeY;

public:
    TextureRegion(Texture texture,
                  const int &posX,
                  const int &posY,
                  const int &sizeX,
                  const int &sizeY) : Texture(texture),
                                      posX(posX),
                                      posY(posY),
                                      sizeX(sizeX),
                                      sizeY(sizeY) {}
    TextureRegion(SDL_Texture *texture,
                  const int &posX,
                  const int &posY,
                  const int &sizeX,
                  const int &sizeY) : Texture(texture),
                                      posX(posX),
                                      posY(posY),
                                      sizeX(sizeX),
                                      sizeY(sizeY) {}

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

protected:
public:
    Renderer();
    ~Renderer();

    Texture loadTexture(const std::string &filename);

    void refresh();

    /** Starts the rendering process */
    void startRender();
    /** Ends the render process and presents to screen */
    void endRender();

    void drawTexture(const Texture &texture,
                     const int &dstX,
                     const int &dstY);

    void drawTexture(const Texture &texture,
                     const int &dstX,
                     const int &dstY,
                     const int &srcX,
                     const int &srcY,
                     const int &sizeX,
                     const int &sizeY);
};
