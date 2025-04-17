#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <SDL.h>

// Game state enumeration
enum GameState {
    TAP_SCREEN,
    GAME_PLAY,
    END_SCREEN,
    PAUSE_MENU
};

// Button structure
struct Button {
    SDL_Texture* texture;
    SDL_Rect rect;
};

#endif // GAMESTATE_H
