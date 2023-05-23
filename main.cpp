#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <fstream>

#include "sprite.hpp"
#include "common.hpp"
#include "game-loop.hpp"
#include "text.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Fatal error initializing SDL: %s\n", SDL_GetError());
    }

    TTF_Init(); 

    // Get display attributes and create fullscreen window
    SDL_GetCurrentDisplayMode(0, &display);

    winWidth = display.w;
    winHeight = display.h;
    displayRefreshRate = display.refresh_rate;

    SDL_Window *window = SDL_CreateWindow("Spiked latte",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          winWidth,
                                          winHeight,
                                          SDL_WINDOW_FULLSCREEN);

    render = SDL_CreateRenderer(window,
                                -1,
                                SDL_RENDERER_ACCELERATED);

    // Read previous high score from file
    ifstream hiScoreFile(HI_SCORE_PATH);
    string hiScoreStr;

    getline(hiScoreFile, hiScoreStr);

    if (hiScoreStr != "")
    {
        hiScore = stoi(hiScoreStr);
    }

    hiScoreFile.close();

    // Set player attributes
    player = new Sprite();

    player->width = winHeight / 12;
    player->height = winHeight / 12;
    player->x = winWidth / 2 - player->width / 2;
    player->y = winHeight / 2 + player->width / 2;

    // Create starting message
    Text *startMessage = new Text();

    startMessage->init(render, "PRESS SPACE TO START", FONT_PATH, 24);
    startMessage->x = winWidth / 2 - startMessage->hitbox.w / 2;
    startMessage->y = player->y + player->height + 100;

    // Create death message
    deathMessage = new Text();

    deathMessage->init(render, "COFFEE SPILLED! PRESS SPACE TO START", FONT_PATH, 24);
    deathMessage->x = winWidth / 2 - deathMessage->hitbox.w / 2;
    deathMessage->y = player->y + player->height + 100;

    // Create "coffee missing" message
    missingMessage = new Text();

    missingMessage->init(render, "COFFEE MISSING! PRESS SPACE TO START", FONT_PATH, 24);
    missingMessage->x = winWidth / 2 - missingMessage->hitbox.w / 2;
    missingMessage->y = player->y + player->height + 100;

    // Create score counter
    scoreMessage = new Text();

    scoreMessage->init(render, "SCORE 0", FONT_PATH, 24);
    scoreMessage->x = 25;
    scoreMessage->y = 25;

    // Create high score counter
    hiScoreMessage = new Text();

    hiScoreMessage->init(render, ("HI SCORE "s + to_string(hiScore)).c_str(), FONT_PATH, 24);
    hiScoreMessage->x = scoreMessage->x;
    hiScoreMessage->y = scoreMessage->y + scoreMessage->hitbox.h + 10;

    // Create obstacle sprites
    obstacleGap = player->height * 3.5;
    obstacleSpeed = winWidth / 1000;

    resetObstacles();

    GRAVITY_ACCELERATION = winHeight;

    player->init(render, playerImgPath.c_str());

    // Draw the scene
    playFrame();
    startMessage->draw();    
    SDL_RenderPresent(render);
    // Wait for player to press space to start game
    waitForSpacePress();

    // Main game loop
    while (running)
    {
        playFrame();
    }

    player->destroy();
    obstacleTop->destroy();
    obstacleBottom->destroy();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}