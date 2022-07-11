#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

namespace assets
{
    class Texture
    {
    private:
        SDL_Surface *m_surface;
    protected:
    public:
        Texture();
        Texture(const std::string &path);
        ~Texture();
        Texture(Texture &o)             = delete;
        Texture &operator=(Texture &o)  = delete;
        Texture(Texture &&o)            = default;
        Texture &operator=(Texture &&o) = default;

        SDL_Surface *getSurface() const;
    };
} // namespace assets