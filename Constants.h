#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

// Play time
const Uint32 TOTAL_PLAY_TIME = 150000; // 150 seconds

// File paths
const std::string FONT_PATH = "./assets/font.ttf";
const std::string MUSIC_PATH = "./assets/gameMusic.mp3";
const std::string TAP_SCREEN_PATH = "./assets/tapScreen.png";
const std::string GAMEPLAY_SCREEN_PATH = "./assets/gamePlayScreen.png";
const std::string ENDGAME_SCREEN_PATH = "./assets/endGameScreen.png";

// Title
const std::string GAME_TITLE_PATH = "./assets/name.png";

// Buttons
const std::string PLAY_BUTTON_PATH = "./assets/play.png";
const std::string EXIT_BUTTON_PATH = "./assets/exit.png";
const std::string SOUND_ON_PATH = "./assets/soundOn.png";
const std::string SOUND_OFF_PATH = "./assets/soundOff.png";
const std::string MENU_BUTTON_PATH = "./assets/menu.png";
const std::string CONTINUE_BUTTON_PATH = "./assets/continue.png";
const std::string RETRY_BUTTON_PATH = "./assets/retry.png";

// Player state images
const std::string STAND_IMG = "./assets/stand.png";
const std::string JUMP_IMG = "./assets/jump.png";
const std::string MOVE_RIGHT_IMG = "./assets/move_right.png";
const std::string MOVE_LEFT_IMG = "./assets/move_left.png";

// Item images
const std::string ICECREAM_TYPE1_IMG = "./assets/icecream_type1.png";
const std::string ICECREAM_TYPE2_IMG = "./assets/icecream_type2.png";
const std::string ICECREAM_TYPE3_IMG = "./assets/icecream_type3.png";
const std::string ICE_IMG = "./assets/ice.png";

// Cloud image
const std::string CLOUD_IMG = "./assets/cloud.png";

#endif // CONSTANTS_H
