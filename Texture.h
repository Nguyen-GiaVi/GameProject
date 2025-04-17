#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

// Function to load texture from file
SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string &path);

// Function to render text using a specific font
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string &message, SDL_Color color);

#endif // TEXTURE_H_INCLUDED
