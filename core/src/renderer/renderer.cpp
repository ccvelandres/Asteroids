#include <game.hpp>
#include <renderer.hpp>

#include <SDL_image.h>

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

std::shared_ptr<SDL_Texture> Renderer::loadTexturePtr(const std::string &filename)
{
    std::weak_ptr<SDL_Texture> &e = m_textures[filename];
    std::shared_ptr<SDL_Texture> p;

    if (e.expired())
    {
        SDL_Texture *t = IMG_LoadTexture(m_renderer, filename.c_str());
        if (t == nullptr)
        {
            std::cerr << "Could not load texture: " << filename << std::endl;
        }
        else
            std::cout << "TextureAlloc: " << t << std::endl;
        p.reset(t, Texture2D::deallocTexture);
        e = p;
    }
    else
    {
        p = e.lock();
    }

    return p;
}

std::vector<Texture2D> Renderer::loadTextureAtlas(const std::string &filename,
                                                  const int &regionWidth,
                                                  const int &regionHeight,
                                                  const int &rows,
                                                  const int &cols)
{
    std::vector<Texture2D> l_textures;
    l_textures.reserve(rows * cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::shared_ptr<SDL_Texture> t = loadTexturePtr(filename);
            Texture2D e(t,
                        Vector2(regionWidth * r,
                                regionHeight * c),
                        Vector2(regionWidth,
                                regionHeight));
            l_textures.emplace_back((e));
        }
    }
    return std::move(l_textures);
}

void Renderer::startRender()
{
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
}

void Renderer::endRender()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::drawLine(const float &p1x,
                        const float &p1y,
                        const float &p2x,
                        const float &p2y)
{
    SDL_RenderDrawLineF(m_renderer, p1x, p1y, p2x, p2y);
}

void Renderer::drawTexture2D(Texture2D &texture,
                             const float &dstX,
                             const float &dstY)
{
    SDL_Rect srcRect = {
        .x = texture.m_position.x,
        .y = texture.m_position.y,
        .w = texture.m_size.x,
        .h = texture.m_size.y};
    SDL_FRect dstRect = {
        .x = dstX,
        .y = dstY,
        .w = static_cast<float>(texture.m_size.x),
        .h = static_cast<float>(texture.m_size.y)};
    SDL_RenderCopyF(m_renderer, texture.m_texture.get(), &srcRect, &dstRect);
}

void Renderer::drawTexture2D(Texture2D &texture,
                             const float &dstX,
                             const float &dstY,
                             const float &scaleW,
                             const float &scaleH)
{
    SDL_Rect srcRect = {
        .x = texture.m_position.x,
        .y = texture.m_position.y,
        .w = texture.m_size.x,
        .h = texture.m_size.y};
    SDL_FRect dstRect = {
        .x = dstX,
        .y = dstY,
        .w = texture.m_size.x * scaleW,
        .h = texture.m_size.y * scaleH};
    SDL_RenderCopyF(m_renderer, texture.m_texture.get(), &srcRect, &dstRect);
}

void Renderer::drawTexture2D(Texture2D &texture,
                             const float &dstW,
                             const float &dstH,
                             const float &dstX,
                             const float &dstY,
                             const int &srcW,
                             const int &srcH,
                             const int &srcX,
                             const int &srcY)
{
    SDL_Rect srcRect = {
        .x = srcX,
        .y = srcY,
        .w = srcW,
        .h = srcH};
    SDL_FRect dstRect = {
        .x = dstX,
        .y = dstY,
        .w = dstW,
        .h = dstH};
    SDL_RenderCopyF(m_renderer, texture.m_texture.get(), &srcRect, &dstRect);
}