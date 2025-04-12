#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <iostream>
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

#endif // CONSTANTS_H_INCLUDED
