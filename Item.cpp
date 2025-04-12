#include "Item.h"
#include "Texture.h"
#include "Constants.h"
#include "GameState.h"
#include <cstdlib>
#include <iostream>

void spawnGameItem(SDL_Renderer* renderer, std::vector<GameItem>& gameItems, Uint32 remainingTimeMs) {
    GameItem item;
    int type = rand() % 4;

    if (type == 0) {
        item.texture = loadTexture(renderer, ICECREAM_TYPE1_IMG);
        item.pointValue = 50;
        item.isIce = false;
    } else if (type == 1) {
        item.texture = loadTexture(renderer, ICECREAM_TYPE2_IMG);
        item.pointValue = 30;
        item.isIce = false;
    } else if (type == 2) {
        item.texture = loadTexture(renderer, ICECREAM_TYPE3_IMG);
        item.pointValue = 20;
        item.isIce = false;
    } else {
        item.texture = loadTexture(renderer, ICE_IMG);
        item.pointValue = -300;
        item.isIce = true;
    }

    if (item.texture == nullptr) return;

    int itemWidth = 50, itemHeight = 50;
    item.rect = { rand() % (SCREEN_WIDTH - itemWidth), 0, itemWidth, itemHeight };
    gameItems.push_back(item);
}

bool checkCollision(const SDL_Rect &a, const SDL_Rect &b) {
    return SDL_HasIntersection(&a, &b);
}

void updateItems(SDL_Renderer* renderer, std::vector<GameItem>& gameItems, SDL_Rect& characterRect,
                int& score, int& hearts, GameState& gameState, Uint32 remainingTimeMs) {
    for (auto it = gameItems.begin(); it != gameItems.end(); ) {
        it->rect.y += 2;

        if (it->isIce) {
            int randomShift = (remainingTimeMs > 30000) ? (rand() % 5) - 2 : (rand() % 11) - 5;
            it->rect.x += randomShift;

            if (it->rect.x < 0) it->rect.x = 0;
            if (it->rect.x + it->rect.w > SCREEN_WIDTH)
                it->rect.x = SCREEN_WIDTH - it->rect.w;
        }

        if (it->rect.y > SCREEN_HEIGHT - SCREEN_HEIGHT/7) {
            it = gameItems.erase(it);
        }
        else if (checkCollision(characterRect, it->rect)) {
            if (it->isIce) {
                if (score > 300) {
                    score += it->pointValue;
                }
                else {
                    score = 0;
                }
                hearts -= 1;
                if (hearts <= 0) {
                    gameState = END_SCREEN;
                }
            } else {
                score += it->pointValue;
            }
            it = gameItems.erase(it);
        } else {
            ++it;
        }
    }
}
