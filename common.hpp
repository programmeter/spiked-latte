#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <tuple>
#include <string>

#include "sprite.hpp"
#include "text.hpp"

#define PLAYER_IMG_PATH "assets/player.png"
#define PLAYER_DEAD_IMG_PATH "assets/playerdead.png"
#define OBSTACLE_TOP_IMG_PATH "assets/obstacletop.png"
#define OBSTACLE_BOTTOM_IMG_PATH "assets/obstaclebottom.png"
#define FONT_PATH "assets/font.ttf"
#define HI_SCORE_PATH "hiscore.txt"
#define NUM_OBSTACLES 4

using namespace std;

bool running = true;

SDL_DisplayMode display;
int winWidth;
int winHeight;
int displayRefreshRate;

SDL_Renderer *render;

Sprite player;
tuple<Sprite, Sprite, bool> obstacles[NUM_OBSTACLES];
Sprite *obstacleTop;
Sprite *obstacleBottom;

Text missingMessage;
Text deathMessage;
Text scoreMessage;
Text hiScoreMessage;

string playerDeadImgPath = PLAYER_DEAD_IMG_PATH;
string playerImgPath = PLAYER_IMG_PATH;

int obstacleGap;
int obstacleSpeed;

int hiScore = 0;

#endif