#include "Game.h"
#include "Constants.h"
#include "Texture.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

Game::Game() :
    gWindow(nullptr),
    gRenderer(nullptr),
    gFont(nullptr),
    largeFont(nullptr),
    gMusic(nullptr),
    gTapScreenTexture(nullptr),
    gGamePlayScreenTexture(nullptr),
    gEndGameScreenTexture(nullptr),
    gCharacterTexture(nullptr),
    gStandTexture(nullptr),
    gJumpTexture(nullptr),
    gMoveRightTexture(nullptr),
    gMoveLeftTexture(nullptr),
    gameState(TAP_SCREEN),
    startPlayTime(0),
    currentTime(0),
    lastSpawnTime(0),
    score(0),
    hearts(5),
    movingLeft(false),
    movingRight(false),
    jumping(false),
    jumpVelocity(-15),
    gravity(1)
{
    characterRect = { SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 125, 50, 100 };
}

Game::~Game() {
    close();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("Doraemon and Ice Cream World", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Game::loadMedia() {
    // Load fonts
    largeFont = TTF_OpenFont(FONT_PATH.c_str(), 70);
    if (largeFont == nullptr) {
        std::cout << "Failed to load large font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    gFont = TTF_OpenFont(FONT_PATH.c_str(), 28);
    if (gFont == nullptr) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Load music
    gMusic = Mix_LoadMUS(MUSIC_PATH.c_str());
    if (gMusic == nullptr) {
        std::cout << "Failed to load music! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load game screens
    gTapScreenTexture = loadTexture(gRenderer, TAP_SCREEN_PATH);
    gGamePlayScreenTexture = loadTexture(gRenderer, GAMEPLAY_SCREEN_PATH);
    gEndGameScreenTexture = loadTexture(gRenderer, ENDGAME_SCREEN_PATH);

    if (!gTapScreenTexture || !gGamePlayScreenTexture || !gEndGameScreenTexture) {
        return false;
    }

    // Load character images
    gStandTexture = loadTexture(gRenderer, STAND_IMG);
    gJumpTexture = loadTexture(gRenderer, JUMP_IMG);
    gMoveRightTexture = loadTexture(gRenderer, MOVE_RIGHT_IMG);
    gMoveLeftTexture = loadTexture(gRenderer, MOVE_LEFT_IMG);

    if (!gStandTexture || !gJumpTexture || !gMoveRightTexture || !gMoveLeftTexture) {
        return false;
    }

    // Initialize character texture to standing
    gCharacterTexture = gStandTexture;

    return true;
}

void Game::close() {
    // Free textures
    SDL_DestroyTexture(gTapScreenTexture);
    SDL_DestroyTexture(gGamePlayScreenTexture);
    SDL_DestroyTexture(gEndGameScreenTexture);
    SDL_DestroyTexture(gStandTexture);
    SDL_DestroyTexture(gJumpTexture);
    SDL_DestroyTexture(gMoveRightTexture);
    SDL_DestroyTexture(gMoveLeftTexture);

    gTapScreenTexture = gGamePlayScreenTexture = gEndGameScreenTexture = nullptr;
    gStandTexture = gJumpTexture = gMoveRightTexture = gMoveLeftTexture = nullptr;
    gCharacterTexture = nullptr;

    // Free game items textures
    for (auto &item : gameItems) {
        if (item.texture != nullptr) {
            SDL_DestroyTexture(item.texture);
            item.texture = nullptr;
        }
    }

    // Free font and music
    TTF_CloseFont(gFont);
    gFont = nullptr;

    TTF_CloseFont(largeFont);
    largeFont = nullptr;

    Mix_FreeMusic(gMusic);
    gMusic = nullptr;

    // Destroy renderer and window
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    // Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::handleEvents(SDL_Event& e, bool& quit) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }

    if (gameState == TAP_SCREEN) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            gameState = GAME_PLAY;
            startPlayTime = SDL_GetTicks();
        }
    }
    else if (gameState == GAME_PLAY) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    movingLeft = true;
                    gCharacterTexture = gMoveLeftTexture;
                    break;
                case SDLK_RIGHT:
                    movingRight = true;
                    gCharacterTexture = gMoveRightTexture;
                    break;
                case SDLK_UP:
                    if (!jumping) {
                        jumping = true;
                        gCharacterTexture = gJumpTexture;
                    }
                    break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    movingLeft = false;
                    if (!movingRight && !jumping) {
                        gCharacterTexture = gStandTexture;
                    }
                    break;
                case SDLK_RIGHT:
                    movingRight = false;
                    if (!movingLeft && !jumping) {
                        gCharacterTexture = gStandTexture;
                    }
                    break;
            }
        }
    }
    else if (gameState == END_SCREEN) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            quit = true;
        }
    }
}

void Game::update() {
    if (gameState == GAME_PLAY) {
        currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startPlayTime;

        if (elapsedTime >= TOTAL_PLAY_TIME) {
            gameState = END_SCREEN;
        }

        Uint32 remainingTimeMs = (TOTAL_PLAY_TIME > elapsedTime) ? TOTAL_PLAY_TIME - elapsedTime : 0;
        Uint32 spawnInterval = (remainingTimeMs <= 30000) ? 250 : 500;

        if (currentTime > lastSpawnTime + spawnInterval) {
            spawnGameItem(gRenderer, gameItems, remainingTimeMs);
            lastSpawnTime = currentTime;
        }

        // Update character position
        if (movingLeft)
            characterRect.x -= 10;
        if (movingRight)
            characterRect.x += 10;

        // Keep character in bounds
        if (characterRect.x < 0)
            characterRect.x = 0;
        if (characterRect.x + characterRect.w > SCREEN_WIDTH)
            characterRect.x = SCREEN_WIDTH - characterRect.w;

        // Handle jumping
        if (jumping) {
            characterRect.y += jumpVelocity;
            jumpVelocity += gravity;

            int groundY = SCREEN_HEIGHT - SCREEN_HEIGHT/7 - characterRect.h;
            if (characterRect.y >= groundY) {
                characterRect.y = groundY;
                jumping = false;
                jumpVelocity = -15;

                if (!movingLeft && !movingRight) {
                    gCharacterTexture = gStandTexture;
                } else if (movingLeft) {
                    gCharacterTexture = gMoveLeftTexture;
                } else if (movingRight) {
                    gCharacterTexture = gMoveRightTexture;
                }
            }
        }

        // Update game items
        updateItems(gRenderer, gameItems, characterRect, score, hearts, gameState, remainingTimeMs);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    if (gameState == TAP_SCREEN) {
        SDL_RenderCopy(gRenderer, gTapScreenTexture, NULL, NULL);
        SDL_Color white = {255, 255, 255, 255};
        SDL_Texture* playText = renderText(gRenderer, largeFont, "Play", white);

        if (playText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(playText, NULL, NULL, &texW, &texH);
            SDL_Rect dstrect = { SCREEN_WIDTH/2 - texW/2, SCREEN_HEIGHT/2 - texH/2, texW, texH };
            SDL_RenderCopy(gRenderer, playText, NULL, &dstrect);
            SDL_DestroyTexture(playText);
        }
    }
    else if (gameState == GAME_PLAY) {
        SDL_RenderCopy(gRenderer, gGamePlayScreenTexture, NULL, NULL);
        SDL_RenderCopy(gRenderer, gCharacterTexture, NULL, &characterRect);

        SDL_Rect groundRect = {0, SCREEN_HEIGHT - SCREEN_HEIGHT/7, SCREEN_WIDTH, SCREEN_HEIGHT/7};
        SDL_SetRenderDrawColor(gRenderer, 139, 69, 19, 255);
        SDL_RenderFillRect(gRenderer, &groundRect);

        int remainingTime = (TOTAL_PLAY_TIME - (currentTime - startPlayTime)) / 1000;
        std::string info = "Score: " + std::to_string(score) + "  Time: " + std::to_string(remainingTime) +
                          "  Hearts: " + std::to_string(hearts) + "  Target point: 5000";

        SDL_Color yellow = {255, 255, 0, 255};
        SDL_Texture* infoText = renderText(gRenderer, gFont, info, yellow);

        if (infoText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(infoText, NULL, NULL, &texW, &texH);
            SDL_Rect infoRect = {10, SCREEN_HEIGHT - texH - 10, texW, texH};
            SDL_RenderCopy(gRenderer, infoText, NULL, &infoRect);
            SDL_DestroyTexture(infoText);
        }

        // Render game items
        for (const auto &item : gameItems) {
            SDL_RenderCopy(gRenderer, item.texture, NULL, &item.rect);
        }
    }
    else if (gameState == END_SCREEN) {
        SDL_RenderCopy(gRenderer, gEndGameScreenTexture, NULL, NULL);

        std::string finalScore = "", state = "";
        if (score > 5000) {
            state = "Congratulations!";
        }
        else {
            state = "You Lose!";
        }
        finalScore = "Total Score: " + std::to_string(score);

        SDL_Color orange = {255, 165, 0, 255};
        SDL_Texture* stateText = renderText(gRenderer, largeFont, state, orange);

        if (stateText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(stateText, NULL, NULL, &texW, &texH);
            SDL_Rect stateRect = { SCREEN_WIDTH/2 - texW/2, SCREEN_HEIGHT/2 - texH/2 - 70, texW, texH};
            SDL_RenderCopy(gRenderer, stateText, NULL, &stateRect);
            SDL_DestroyTexture(stateText);
        }

        SDL_Texture* scoreText = renderText(gRenderer, largeFont, finalScore, orange);

        if (scoreText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(scoreText, NULL, NULL, &texW, &texH);
            SDL_Rect scoreRect = { SCREEN_WIDTH/2 - texW/2, SCREEN_HEIGHT/2 - texH/2, texW, texH };
            SDL_RenderCopy(gRenderer, scoreText, NULL, &scoreRect);
            SDL_DestroyTexture(scoreText);
        }
    }

    SDL_RenderPresent(gRenderer);
}

void Game::run() {
    bool quit = false;
    SDL_Event e;

    // Play music
    Mix_PlayMusic(gMusic, -1);

    // Game loop
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            handleEvents(e, quit);
        }

        update();
        render();

        SDL_Delay(16); // 60FPS
    }
}
