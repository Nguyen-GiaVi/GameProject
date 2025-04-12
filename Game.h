#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include "GameState.h"
#include "Item.h"

class Game {
private:
    // SDL Components
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    TTF_Font* gFont;
    TTF_Font* largeFont;
    Mix_Music* gMusic;

    // Textures
    SDL_Texture* gTapScreenTexture;
    SDL_Texture* gGamePlayScreenTexture;
    SDL_Texture* gEndGameScreenTexture;

    // Character textures
    SDL_Texture* gCharacterTexture;
    SDL_Texture* gStandTexture;
    SDL_Texture* gJumpTexture;
    SDL_Texture* gMoveRightTexture;
    SDL_Texture* gMoveLeftTexture;

    // Game state and variables
    GameState gameState;
    std::vector<GameItem> gameItems;
    SDL_Rect characterRect;
    Uint32 startPlayTime;
    Uint32 currentTime;
    Uint32 lastSpawnTime;
    int score;
    int hearts;

    // Character movement variables
    bool movingLeft;
    bool movingRight;
    bool jumping;
    int jumpVelocity;
    int gravity;

public:
    Game();
    ~Game();

    bool init();
    bool loadMedia();
    void close();
    void handleEvents(SDL_Event& e, bool& quit);
    void update();
    void render();
    void run();

    // Getter for renderer (needed by other components)
    SDL_Renderer* getRenderer() const { return gRenderer; }
};

#endif // GAME_H_INCLUDED
