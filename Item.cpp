#include "Item.h"
#include "Texture.h"
#include "Constants.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

void spawnGameItem(SDL_Renderer* renderer, std::vector<GameItem>& gameItems) {
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

    if (item.texture == nullptr)
        return;

    int itemWidth = 50, itemHeight = 50;
    item.rect = { rand() % (SCREEN_WIDTH - itemWidth), 0, itemWidth, itemHeight };
    gameItems.push_back(item);
}

bool checkCollision(const SDL_Rect &a, const SDL_Rect &b) {
    return SDL_HasIntersection(&a, &b);
}

bool checkCloudCollision(const SDL_Rect &newCloud, const std::vector<Cloud>& clouds) {
    for (const auto &cloud : clouds) {
        // Expand check area to create distance between clouds
        SDL_Rect expandedRect = cloud.rect;
        expandedRect.x -= 20;
        expandedRect.y -= 20;
        expandedRect.w += 40;
        expandedRect.h += 40;

        if (SDL_HasIntersection(&newCloud, &expandedRect)) {
            return true; // Collision detected
        }
    }
    return false; // No collision
}

void spawnCloud(SDL_Texture* cloudTexture, std::vector<Cloud>& clouds) {
    Cloud cloud;
    cloud.texture = cloudTexture;
    int cloudWidth = 100, cloudHeight = 60;
    cloud.rect.w = cloudWidth;
    cloud.rect.h = cloudHeight;

    // Try up to 10 times to find a position without overlapping
    bool validPosition = false;
    int attempts = 0;

    while (!validPosition && attempts < 10) {
        // X position can be outside the screen or within part of the screen
        cloud.rect.x = (rand() % 3 == 0) ? -cloudWidth : -cloudWidth - (rand() % 10);
        // Y position in the upper part of the screen
        cloud.rect.y = rand() % (SCREEN_HEIGHT/3 - 30);

        // Check for collision
        if (!checkCloudCollision(cloud.rect, clouds)) {
            validPosition = true;
        }

        attempts++;
    }

    // If no suitable position is found after many attempts, place the cloud outside the screen
    if (!validPosition) {
        cloud.rect.x = -cloudWidth - (rand() % 100); // Place further to the left of the screen
    }

    // Random movement speed (1-3)
    cloud.speed = 1 + rand() % 3;
    clouds.push_back(cloud);
}

void updateClouds(std::vector<Cloud>& clouds) {
    for (auto it = clouds.begin(); it != clouds.end(); ) {
        it->rect.x += it->speed;
        if (it->rect.x > SCREEN_WIDTH) {
            it = clouds.erase(it);
        } else {
            ++it;
        }
    }

    // Always ensure at least 6 clouds
    if (clouds.size() < 6 && rand() % 100 < 5) { // 5% chance each frame
        // Note: We can't spawn new clouds here because we need the renderer
        // New clouds will be spawned in the main game loop
    }
}
