#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <SDL.h>
#include <vector>
#include <iostream>
#include "GameState.h"

// Item drop structure
struct GameItem {
    SDL_Texture* texture;
    SDL_Rect rect;
    int pointValue;
    bool isIce;
};

// Function declarations
void spawnGameItem(SDL_Renderer* renderer, std::vector<GameItem>& gameItems, Uint32 remainingTimeMs);
bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);
void updateItems(SDL_Renderer* renderer, std::vector<GameItem>& gameItems, SDL_Rect& characterRect,
                int& score, int& hearts, GameState& gameState, Uint32 remainingTimeMs);

#endif // ITEM_H_INCLUDED
