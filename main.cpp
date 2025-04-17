#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Game.h"

int main(int argc, char* args[]) {
    // Seed the random number generator
    srand((unsigned int)time(0));

    // Create game instance
    Game game;

    // Initialize game
    if (!game.init()) {
        std::cout << "Khởi tạo thất bại!\n";
        return -1;
    }

    // Load media
    if (!game.loadMedia()) {
        std::cout << "Load media thất bại!\n";
        return -1;
    }

    // Game loop
    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();

        // Cap the frame rate
        SDL_Delay(16); // ~60 FPS
    }

    return 0;
}
