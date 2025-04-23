#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "GameState.h"
#include "Item.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    bool init();
    bool loadMedia();
    void close();

    bool isButtonClicked(const Button &button, int x, int y);
    void resetGame();

    void handleEvents();
    void update();
    void render();

    bool isRunning() { return !quit; }

private:
    // SDL components
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    TTF_Font* gFont;
    TTF_Font* largeFont;
    Mix_Music* gMusic;

    // Game textures
    SDL_Texture* gTapScreenTexture;
    SDL_Texture* gGamePlayScreenTexture;
    SDL_Texture* gEndGameScreenTexture;
    SDL_Texture* gGameTitleTexture;

    // Button textures
    SDL_Texture* gPlayButtonTexture;
    SDL_Texture* gExitButtonTexture;
    SDL_Texture* gSoundOnTexture;
    SDL_Texture* gSoundOffTexture;
    SDL_Texture* gMenuButtonTexture;
    SDL_Texture* gContinueButtonTexture;
    SDL_Texture* gRetryButtonTexture;

    // Character textures
    SDL_Texture* gCharacterTexture;
    SDL_Texture* gStandTexture;
    SDL_Texture* gJumpTexture;
    SDL_Texture* gMoveRightTexture;
    SDL_Texture* gMoveLeftTexture;

    // Cloud texture
    SDL_Texture* gCloudTexture;

    // Game state
    GameState gameState;
    bool quit;
    bool soundEnabled;

    // Character state
    SDL_Rect characterRect;
    bool movingLeft, movingRight, jumping;
    int jumpVelocity;
    int gravity;

    bool isSlowed;
    Uint32 slowStartTime;

    // Game progress
    Uint32 startPlayTime;
    Uint32 currentTime;
    Uint32 lastSpawnTime;
    int score;
    int hearts;

    // Buttons
    Button playButton;
    Button exitButton;
    Button soundButton;
    Button menuButton;
    Button continueButton;
    Button retryButton;

    // Game items and clouds
    std::vector<GameItem> gameItems;
    std::vector<Cloud> clouds;
};

#endif // GAME_H
