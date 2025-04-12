#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

// Texture loading and rendering functions
SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string &path);
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string &message, SDL_Color color);

#endif // TEXTURE_H_INCLUDED
