#include "Texture.h"
#include <SDL_image.h>
#include <iostream>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string &path) {
    if (renderer == nullptr) {
        std::cout << "Cannot load texture: renderer is null" << std::endl;
        return nullptr;
    }

    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == nullptr) {
        std::cout << "Cannot load texture from " << path << " - SDL_Error: " << SDL_GetError() << std::endl;
    }
    return newTexture;
}

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string &message, SDL_Color color) {
    if (renderer == nullptr) {
        std::cout << "Cannot render text: renderer is null" << std::endl;
        return nullptr;
    }

    if (font == nullptr) {
        std::cout << "Cannot render text: font is null" << std::endl;
        return nullptr;
    }

    SDL_Surface* surf = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surf == nullptr) {
        std::cout << "Cannot render text: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}
