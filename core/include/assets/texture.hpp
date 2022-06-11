#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

class Texture
{
private:
    SDL_Surface * m_surface;
protected:
public:
    Texture(const std::string &path);
    Texture(Texture&&o);
    Texture &operator= (Texture&&o);
    ~Texture();

    SDL_Surface *getSurface() const;
};