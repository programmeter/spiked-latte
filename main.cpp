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

    if (TTF_Init() != 0)
    {
        printf("Fatal error initializing TTF: %s\n", SDL_GetError());
    }
    
    SDL_GetCurrentDisplayMode(0, &display);

    displayRefreshRate = display.refresh_rate;

    SDL_Window *window = SDL_CreateWindow("Spiked latte",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          display.w,
                                          display.h,
                                          SDL_WINDOW_FULLSCREEN);

    render = SDL_CreateRenderer(window,
                                -1,
                                SDL_RENDERER_ACCELERATED);

    SDL_ShowCursor(SDL_DISABLE);

    // Read previous high score from file
    ifstream hiScoreFile(HI_SCORE_PATH);
    string hiScoreStr;

    getline(hiScoreFile, hiScoreStr);

    if (hiScoreStr != "")
    {
        hiScore = stoi(hiScoreStr);
    }

    hiScoreFile.close();

    // Set player properties
    player.w = display.h / 12;
    player.h = player.w;
    player.x = display.w / 2 - player.w / 2;
    player.y = display.h / 2 + player.w / 2;

    // Create starting message
    Text startMessage;

    startMessage.setImg(render, "PRESS SPACE TO START", FONT_PATH, 24);
    startMessage.x = display.w / 2 - startMessage.hitbox.w / 2;
    startMessage.y = player.y + player.h + 100;

    // Set death message properties
    deathMessage.setImg(render, "COFFEE SPILLED! PRESS SPACE TO START", FONT_PATH, 24);
    deathMessage.x = display.w / 2 - deathMessage.hitbox.w / 2;
    deathMessage.y = player.y + player.h + 100;

    // Set "coffee missing" message properties
    missingMessage.setImg(render, "COFFEE MISSING! PRESS SPACE TO START", FONT_PATH, 24);
    missingMessage.x = display.w / 2 - missingMessage.hitbox.w / 2;
    missingMessage.y = player.y + player.h + 100;

    // Set score counter properties
    scoreMessage.setImg(render, "SCORE 0", FONT_PATH, 24);
    scoreMessage.x = 25;
    scoreMessage.y = 25;

    // Set high score counter properties
    hiScoreMessage.setImg(render, ("HI SCORE "s + to_string(hiScore)).c_str(), FONT_PATH, 24);
    hiScoreMessage.x = scoreMessage.x;
    hiScoreMessage.y = scoreMessage.y + scoreMessage.hitbox.h + 10;

    // Set obstacle properties
    obstacleGap = player.h * 3.5;
    obstacleSpeed = display.w / 5.17;

    resetObstacles();

    GRAVITY_ACCELERATION = display.h * 1.3;

    player.setImg(render, playerImgPath.c_str());

    // Draw the scene
    playFrame();
    startMessage.draw();
    SDL_RenderPresent(render);
    // Wait for player to press space to start game
    waitForSpacePress();

    // Main game loop
    while (running)
    {
        playFrame();
    }

    player.destroy();
    obstacleTop->destroy();
    obstacleBottom->destroy();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}