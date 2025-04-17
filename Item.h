#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <SDL.h>
#include <vector>

// Game Item structure
struct GameItem {
    SDL_Texture* texture;
    SDL_Rect rect;
    int pointValue;
    bool isIce;
};

// Cloud structure
struct Cloud {
    SDL_Texture* texture;
    SDL_Rect rect;
    int speed; // Horizontal speed
};

// Functions for handling game items
void spawnGameItem(SDL_Renderer* renderer, std::vector<GameItem>& gameItems);
bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);

// Functions for handling clouds
void spawnCloud(SDL_Texture* cloudTexture, std::vector<Cloud>& clouds);
bool checkCloudCollision(const SDL_Rect &newCloud, const std::vector<Cloud>& clouds);
void updateClouds(std::vector<Cloud>& clouds);

#endif // ITEM_H
