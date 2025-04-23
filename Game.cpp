
#include "Game.h"
#include "Constants.h"
#include "Texture.h"
#include "Item.h"
#include <iostream>
#include <string>

Game::Game() :
    gWindow(nullptr),
    gRenderer(nullptr),
    gFont(nullptr),
    largeFont(nullptr),
    gMusic(nullptr),
    gTapScreenTexture(nullptr),
    gGamePlayScreenTexture(nullptr),
    gEndGameScreenTexture(nullptr),
    gGameTitleTexture(nullptr),
    gPlayButtonTexture(nullptr),
    gExitButtonTexture(nullptr),
    gSoundOnTexture(nullptr),
    gSoundOffTexture(nullptr),
    gMenuButtonTexture(nullptr),
    gContinueButtonTexture(nullptr),
    gRetryButtonTexture(nullptr),
    gCharacterTexture(nullptr),
    gStandTexture(nullptr),
    gJumpTexture(nullptr),
    gMoveRightTexture(nullptr),
    gMoveLeftTexture(nullptr),
    gCloudTexture(nullptr),
    gameState(TAP_SCREEN),
    quit(false),
    soundEnabled(true),
    movingLeft(false),
    movingRight(false),
    jumping(false),
    jumpVelocity(-15),
    gravity(1),
    isSlowed(false),
    slowStartTime(0),
    startPlayTime(0),
    currentTime(0),
    lastSpawnTime(0),
    score(0),
    hearts(5)
{
    // Initialize character position
    characterRect = { SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - SCREEN_HEIGHT/7 - 100, 50, 100 };
}

Game::~Game() {
    close();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("Doraemon and Ice Cream World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Không tạo được cửa sổ! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cout << "Không tạo được renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_Image không khởi tạo được! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf không khởi tạo được! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer không khởi tạo được! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Game::loadMedia() {
    // Load fonts
    largeFont = TTF_OpenFont(FONT_PATH.c_str(), 70);
    if (largeFont == nullptr) {
        std::cout << "Không load được font (large)! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    gFont = TTF_OpenFont(FONT_PATH.c_str(), 28);
    if (gFont == nullptr) {
        std::cout << "Không load được font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Load background music
    gMusic = Mix_LoadMUS(MUSIC_PATH.c_str());
    if (gMusic == nullptr) {
        std::cout << "Không load được nhạc! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load screen textures
    gTapScreenTexture = loadTexture(gRenderer, TAP_SCREEN_PATH);
    gGamePlayScreenTexture = loadTexture(gRenderer, GAMEPLAY_SCREEN_PATH);
    gEndGameScreenTexture = loadTexture(gRenderer, ENDGAME_SCREEN_PATH);
    gGameTitleTexture = loadTexture(gRenderer, GAME_TITLE_PATH);

    // Load button textures
    gPlayButtonTexture = loadTexture(gRenderer, PLAY_BUTTON_PATH);
    gExitButtonTexture = loadTexture(gRenderer, EXIT_BUTTON_PATH);
    gSoundOnTexture = loadTexture(gRenderer, SOUND_ON_PATH);
    gSoundOffTexture = loadTexture(gRenderer, SOUND_OFF_PATH);
    gMenuButtonTexture = loadTexture(gRenderer, MENU_BUTTON_PATH);
    gContinueButtonTexture = loadTexture(gRenderer, CONTINUE_BUTTON_PATH);
    gRetryButtonTexture = loadTexture(gRenderer, RETRY_BUTTON_PATH);

    // Check if all screen and button textures loaded successfully
    if (!gTapScreenTexture || !gGamePlayScreenTexture || !gEndGameScreenTexture ||
        !gGameTitleTexture || !gPlayButtonTexture || !gExitButtonTexture ||
        !gSoundOnTexture || !gSoundOffTexture || !gMenuButtonTexture ||
        !gContinueButtonTexture || !gRetryButtonTexture) {
        return false;
    }

    // Initialize buttons
    int buttonWidth = 200;
    int buttonHeight = 80;
    int smallButtonSize = 50;

    // Play button
    playButton.texture = gPlayButtonTexture;
    playButton.rect = {SCREEN_WIDTH/2 - buttonWidth/2, SCREEN_HEIGHT/2 - buttonHeight/2 + 80, buttonWidth, buttonHeight};

    // Exit button
    exitButton.texture = gExitButtonTexture;
    exitButton.rect = {SCREEN_WIDTH/2 - buttonWidth/2, SCREEN_HEIGHT/2 - buttonHeight/2 + 180, buttonWidth, buttonHeight};

    // Sound button
    int soundButtonSize = 50;
    soundButton.texture = gSoundOnTexture;
    soundButton.rect = {SCREEN_WIDTH - soundButtonSize - 20, SCREEN_HEIGHT - soundButtonSize - 20, soundButtonSize, soundButtonSize};

    // Menu button
    menuButton.texture = gMenuButtonTexture;
    menuButton.rect = {SCREEN_WIDTH - 2*smallButtonSize - 30, SCREEN_HEIGHT - smallButtonSize - 20, smallButtonSize, smallButtonSize};

    // Continue button (used in pause menu)
    continueButton.texture = gContinueButtonTexture;
    continueButton.rect = {SCREEN_WIDTH/2 - buttonWidth/2, SCREEN_HEIGHT/2 - buttonHeight/2 + 80, buttonWidth, buttonHeight};

    // Retry button (used in end screen)
    retryButton.texture = gRetryButtonTexture;
    retryButton.rect = {SCREEN_WIDTH/2 - buttonWidth/2, SCREEN_HEIGHT/2 - buttonHeight/2 + 80, buttonWidth, buttonHeight};

    // Character textures
    gStandTexture = loadTexture(gRenderer, STAND_IMG);
    gJumpTexture = loadTexture(gRenderer, JUMP_IMG);
    gMoveRightTexture = loadTexture(gRenderer, MOVE_RIGHT_IMG);
    gMoveLeftTexture = loadTexture(gRenderer, MOVE_LEFT_IMG);

    if (!gStandTexture || !gJumpTexture || !gMoveRightTexture || !gMoveLeftTexture) {
        return false;
    }

    gCharacterTexture = gStandTexture;

    // Cloud texture (used only for tapScreen)
    gCloudTexture = loadTexture(gRenderer, CLOUD_IMG);
    if (!gCloudTexture) {
        std::cout << "Không load được đám mây!" << std::endl;
        return false;
    }

    // Initialize some clouds
    for (int i = 0; i < 3; i++) {
        spawnCloud(gCloudTexture, clouds);
        // Set initial positions evenly spread
        if (i > 0 && !clouds.empty()) {
            clouds.back().rect.x = i * (SCREEN_WIDTH / 3);
        }
    }

    // Start playing background music
    Mix_PlayMusic(gMusic, -1);

    return true;
}

void Game::close() {
    // Free textures
    SDL_DestroyTexture(gTapScreenTexture);
    SDL_DestroyTexture(gGamePlayScreenTexture);
    SDL_DestroyTexture(gEndGameScreenTexture);
    SDL_DestroyTexture(gGameTitleTexture);

    SDL_DestroyTexture(gPlayButtonTexture);
    SDL_DestroyTexture(gExitButtonTexture);
    SDL_DestroyTexture(gSoundOnTexture);
    SDL_DestroyTexture(gSoundOffTexture);
    SDL_DestroyTexture(gMenuButtonTexture);
    SDL_DestroyTexture(gContinueButtonTexture);
    SDL_DestroyTexture(gRetryButtonTexture);

    SDL_DestroyTexture(gStandTexture);
    SDL_DestroyTexture(gJumpTexture);
    SDL_DestroyTexture(gMoveRightTexture);
    SDL_DestroyTexture(gMoveLeftTexture);

    SDL_DestroyTexture(gCloudTexture);

    // Set pointers to nullptr
    gTapScreenTexture = gGamePlayScreenTexture = gEndGameScreenTexture = gGameTitleTexture = nullptr;
    gPlayButtonTexture = gExitButtonTexture = gSoundOnTexture = gSoundOffTexture = gMenuButtonTexture = gContinueButtonTexture = gRetryButtonTexture = nullptr;
    gCharacterTexture = gStandTexture = gJumpTexture = gMoveRightTexture = gMoveLeftTexture = nullptr;
    gCloudTexture = nullptr;

    // Free game items
    for (auto &item : gameItems) {
        if (item.texture != nullptr) {
            SDL_DestroyTexture(item.texture);
        }
    }
    gameItems.clear();

    // Close fonts
    TTF_CloseFont(gFont);
    gFont = nullptr;
    TTF_CloseFont(largeFont);
    largeFont = nullptr;

    // Free music
    Mix_FreeMusic(gMusic);
    gMusic = nullptr;

    // Destroy renderer and window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    // Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::isButtonClicked(const Button &button, int x, int y) {
    return (x >= button.rect.x && x < button.rect.x + button.rect.w &&
            y >= button.rect.y && y < button.rect.y + button.rect.h);
}

void Game::resetGame() {
    // Clear all items
    for (auto &item : gameItems) {
        if (item.texture != nullptr) {
            SDL_DestroyTexture(item.texture);
        }
    }
    gameItems.clear();

    // Reset score and hearts
    score = 0;
    hearts = 5;

    // Reset character position
    characterRect = { SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - SCREEN_HEIGHT/7 - 100, 50, 100 };
    gCharacterTexture = gStandTexture;

    // Reset control variables
    movingLeft = false;
    movingRight = false;
    jumping = false;
    jumpVelocity = -15;
    isSlowed = false;
    slowStartTime = 0;
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            // Check sound button on all screens
            if (isButtonClicked(soundButton, mouseX, mouseY)) {
                soundEnabled = !soundEnabled;
                soundButton.texture = soundEnabled ? gSoundOnTexture : gSoundOffTexture;

                if (soundEnabled) {
                    Mix_ResumeMusic();
                } else {
                    Mix_PauseMusic();
                }
            }

            if (gameState == TAP_SCREEN) {
                // Handle buttons on tap screen
                if (isButtonClicked(playButton, mouseX, mouseY)) {
                    gameState = GAME_PLAY;
                    startPlayTime = SDL_GetTicks();
                } else if (isButtonClicked(exitButton, mouseX, mouseY)) {
                    quit = true;
                }
            } else if (gameState == GAME_PLAY) {
                // Check menu button in gameplay
                if (isButtonClicked(menuButton, mouseX, mouseY)) {
                    gameState = PAUSE_MENU;
                    Mix_PauseMusic(); // Pause music when opening menu
                }
            } else if (gameState == PAUSE_MENU) {
                // Handle buttons in pause menu
                if (isButtonClicked(continueButton, mouseX, mouseY)) {
                    gameState = GAME_PLAY;
                    if (soundEnabled) {
                        Mix_ResumeMusic();
                    }
                } else if (isButtonClicked(exitButton, mouseX, mouseY)) {
                    quit = true;
                }
            } else if (gameState == END_SCREEN) {
                // Handle buttons in end screen
                if (isButtonClicked(retryButton, mouseX, mouseY)) {
                    // Reset game and start over
                    resetGame();
                    gameState = GAME_PLAY;
                    startPlayTime = SDL_GetTicks();
                } else if (isButtonClicked(exitButton, mouseX, mouseY)) {
                    quit = true;
                }
            }
        }

        if (gameState == GAME_PLAY) {
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
            } else if (e.type == SDL_KEYUP) {
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
    }
}

void Game::update() {
    // Update clouds only on TAP_SCREEN
    if (gameState == TAP_SCREEN) {
        updateClouds(clouds);

        // Spawn new clouds if needed
        if (clouds.size() < 6 && rand() % 100 < 5) { // 5% chance each frame
            spawnCloud(gCloudTexture, clouds);
        }
    }

    if (gameState == GAME_PLAY) {
        currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startPlayTime;

        // Check if game time is over
        if (elapsedTime >= TOTAL_PLAY_TIME) {
            gameState = END_SCREEN;
        }

        Uint32 remainingTimeMs = (TOTAL_PLAY_TIME > elapsedTime) ? TOTAL_PLAY_TIME - elapsedTime : 0;
        Uint32 spawnInterval = (remainingTimeMs <= 30000) ? 250 : 500; // More frequent spawns in last 30 seconds

        // Spawn new items
        if (currentTime > lastSpawnTime + spawnInterval) {
            spawnGameItem(gRenderer, gameItems);
            lastSpawnTime = currentTime;
        }

        // Update character position
        if (movingLeft) {
            characterRect.x -= (isSlowed) ? 5 : 10;
        }
        if (movingRight) {
            characterRect.x += (isSlowed) ? 5 : 10;
        }

        // Keep character within screen bounds
        if (characterRect.x < 0) {
            characterRect.x = 0;
        }
        if (characterRect.x + characterRect.w > SCREEN_WIDTH) {
            characterRect.x = SCREEN_WIDTH - characterRect.w;
        }

        // Handle jumping
        if (jumping) {
            characterRect.y += jumpVelocity;
            jumpVelocity += gravity;

            int groundY = SCREEN_HEIGHT - SCREEN_HEIGHT/7 - characterRect.h;
            if (characterRect.y >= groundY) {
                characterRect.y = groundY;
                jumping = false;
                jumpVelocity = -15;

                // Reset to stand texture only if not moving left/right
                if (!movingLeft && !movingRight) {
                    gCharacterTexture = gStandTexture;
                } else if (movingLeft) {
                    gCharacterTexture = gMoveLeftTexture;
                } else if (movingRight) {
                    gCharacterTexture = gMoveRightTexture;
                }
            }
        }

        // Update items and check for collisions
        for (auto it = gameItems.begin(); it != gameItems.end(); ) {

            // Ice items move randomly
            if (it->isIce) {
                int randomShift = (remainingTimeMs > 30000) ? (rand() % 5) - 2 : (rand() % 11) - 5;
                it->rect.x += randomShift;
                it->rect.y += (remainingTimeMs > 30000) ? 3 : 5;

                // Keep ice items within screen bounds
                if (it->rect.x < 0) {
                    it->rect.x = 0;
                }
                if (it->rect.x + it->rect.w > SCREEN_WIDTH) {
                    it->rect.x = SCREEN_WIDTH - it->rect.w;
                }
            }
            else {
                it->rect.y += 2;
            }

            // Remove items that have fallen out of screen
            if (it->rect.y > SCREEN_HEIGHT - SCREEN_HEIGHT/7) {
                SDL_DestroyTexture(it->texture);
                it = gameItems.erase(it);
            }
            // Check for collision with character
            else if (checkCollision(characterRect, it->rect)) {
                if (it->isIce) {
                    isSlowed = true;
                    slowStartTime = SDL_GetTicks();

                    score += (score > 300) ? it->pointValue : -score;
                    hearts -= 1;

                    if (hearts <= 0) {
                        gameState = END_SCREEN;
                    }
                } else {
                    score += it->pointValue; // Gain points
                }

                SDL_DestroyTexture(it->texture);
                it = gameItems.erase(it);
            } else {
                ++it;
            }
            if (isSlowed && (SDL_GetTicks() - slowStartTime >= 3000)) {
                isSlowed = false;
            }
        }
    }
}

void Game::render() {
    SDL_RenderClear(gRenderer);

    if (gameState == TAP_SCREEN) {
        // Draw background
        SDL_RenderCopy(gRenderer, gTapScreenTexture, NULL, NULL);

        // Draw clouds
        for (const auto &cloud : clouds) {
            SDL_RenderCopy(gRenderer, cloud.texture, NULL, &cloud.rect);
        }

        // Draw game title in the middle of the screen
        const int gameTitleWidth = 700;
        const int gameTitleHeight = 200;
        SDL_Rect gameTitleRect = {
            SCREEN_WIDTH/2 - gameTitleWidth/2,
            SCREEN_HEIGHT/4 - gameTitleHeight/2,
            gameTitleWidth,
            gameTitleHeight
        };
        SDL_RenderCopy(gRenderer, gGameTitleTexture, NULL, &gameTitleRect);

        // Draw buttons
        SDL_RenderCopy(gRenderer, playButton.texture, NULL, &playButton.rect);
        SDL_RenderCopy(gRenderer, exitButton.texture, NULL, &exitButton.rect);
    }
    else if (gameState == GAME_PLAY || gameState == PAUSE_MENU) {
        // Draw background
        SDL_RenderCopy(gRenderer, gGamePlayScreenTexture, NULL, NULL);

        // Draw character
        SDL_RenderCopy(gRenderer, gCharacterTexture, NULL, &characterRect);

        // Draw ground
        SDL_Rect groundRect = {0, SCREEN_HEIGHT - SCREEN_HEIGHT/7, SCREEN_WIDTH, SCREEN_HEIGHT/7};
        SDL_SetRenderDrawColor(gRenderer, 139, 69, 19, 255);
        SDL_RenderFillRect(gRenderer, &groundRect);

        // Draw game info (score, time, hearts)
        int remainingTime = (TOTAL_PLAY_TIME - (currentTime - startPlayTime)) / 1000;
        std::string info = "Score: " + std::to_string(score) + "  Time: " + std::to_string(remainingTime) + "  Hearts: " + std::to_string(hearts) + "  Target: 5000";
        SDL_Color yellow = {255, 255, 0, 255};
        SDL_Texture* infoText = renderText(gRenderer, gFont, info, yellow);

        if (infoText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(infoText, NULL, NULL, &texW, &texH);
            SDL_Rect infoRect = {10, SCREEN_HEIGHT - texH - 10, texW, texH};
            SDL_RenderCopy(gRenderer, infoText, NULL, &infoRect);
            SDL_DestroyTexture(infoText);
        }

        // Draw game items
        for (const auto &item : gameItems) {
            SDL_RenderCopy(gRenderer, item.texture, NULL, &item.rect);
        }

        // Draw menu button
        SDL_RenderCopy(gRenderer, menuButton.texture, NULL, &menuButton.rect);

        // If in pause menu, draw overlay and menu buttons
        if (gameState == PAUSE_MENU) {
            // Draw semi-transparent overlay
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 128);
            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(gRenderer, &overlayRect);

            // Draw pause menu buttons
            SDL_RenderCopy(gRenderer, continueButton.texture, NULL, &continueButton.rect);
            SDL_RenderCopy(gRenderer, exitButton.texture, NULL, &exitButton.rect);

            // Draw pause menu title
            SDL_Color white = {255, 255, 255, 255};
            SDL_Texture* pauseText = renderText(gRenderer, largeFont, "GAME PAUSED", white);

            if (pauseText != nullptr) {
                int texW = 0, texH = 0;
                SDL_QueryTexture(pauseText, NULL, NULL, &texW, &texH);
                SDL_Rect pauseRect = {SCREEN_WIDTH/2 - texW/2, SCREEN_HEIGHT/3 - texH/2, texW, texH};
                SDL_RenderCopy(gRenderer, pauseText, NULL, &pauseRect);
                SDL_DestroyTexture(pauseText);
            }
        }
    }
    else if (gameState == END_SCREEN) {
        // Draw end screen background
        SDL_RenderCopy(gRenderer, gEndGameScreenTexture, NULL, NULL);

        // Show game result
        std::string state = (score > 5000) ? "Congratulations!" : "You Lose!";
        std::string finalScore = "Total Score: " + std::to_string(score);

        SDL_Color white = {255, 255, 255, 255};
        SDL_Texture* stateText = renderText(gRenderer, largeFont, state, white);

        if (stateText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(stateText, NULL, NULL, &texW, &texH);
            SDL_Rect stateRect = { SCREEN_WIDTH/2 - texW/2, SCREEN_HEIGHT/2 - texH/2 - 70, texW, texH };
            SDL_RenderCopy(gRenderer, stateText, NULL, &stateRect);
            SDL_DestroyTexture(stateText);
        }

        SDL_Texture* scoreText = renderText(gRenderer, largeFont, finalScore, white);

        if (scoreText != nullptr) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(scoreText, NULL, NULL, &texW, &texH);
            SDL_Rect scoreRect = { SCREEN_WIDTH/2 - texW/2, SCREEN_HEIGHT/2 - texH/2, texW, texH };
            SDL_RenderCopy(gRenderer, scoreText, NULL, &scoreRect);
            SDL_DestroyTexture(scoreText);
        }

        // Draw end screen buttons
        SDL_RenderCopy(gRenderer, retryButton.texture, NULL, &retryButton.rect);
        SDL_RenderCopy(gRenderer, exitButton.texture, NULL, &exitButton.rect);
    }

    // Always draw sound button
    SDL_RenderCopy(gRenderer, soundButton.texture, NULL, &soundButton.rect);

    // Update screen
    SDL_RenderPresent(gRenderer);
}
