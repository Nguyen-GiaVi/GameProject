#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <ctime>
#include <cstdlib>
#include "Game.h"

int main(int argc, char* args[]) {
    // Seed random number generator
    srand((unsigned int)time(0));

    // Create game instance
    Game game;

    // Initialize game
    if (!game.init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    // Load game media
    if (!game.loadMedia()) {
        std::cout << "Failed to load media!" << std::endl;
        return -1;
    }

    // Run the game
    game.run();

    return 0;
}
