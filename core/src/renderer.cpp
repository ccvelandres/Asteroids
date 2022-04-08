#include <game.hpp>
#include <renderer.hpp>

#include <SDL2/SDL_image.h>

Renderer::Renderer()
{
    m_renderer = SDL_CreateRenderer(Game::this_game().m_window,
                                    -1,
                                    SDL_RENDERER_ACCELERATED);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
}

Texture Renderer::loadTexture(const std::string &filename)
{
    std::weak_ptr<SDL_Texture> &e = m_textures[filename];

    if (e.expired())
    {
        SDL_Surface *s = IMG_Load(filename.c_str());
        SDL_Texture *t = SDL_CreateTextureFromSurface(m_renderer, s);
        if (t == nullptr)
        {
            std::cerr << "TextureFromSurface Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(s);
        Texture tx(t);
        e = tx.m_texture;
        return tx;
    }
    else
    {
        return Texture(e.lock());
    }
}

void Renderer::startRender()
{
}

void Renderer::endRender()
{
}

void Renderer::drawTexture(const Texture &texture,
                           const int &dstX,
                           const int &dstY)
{
}

void Renderer::drawTexture(const Texture &texture,
                           const int &dstX,
                           const int &dstY,
                           const int &srcX,
                           const int &srcY,
                           const int &sizeX,
                           const int &sizeY)
{
}