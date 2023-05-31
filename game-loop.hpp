#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <chrono>
#include <thread>

#include "sprite.hpp"
#include "common.hpp"
#include "conversions.hpp"

using namespace std;

SDL_Event event;
std::string obstacleTopImgPath = OBSTACLE_TOP_IMG_PATH;
std::string obstacleBottomImgPath = OBSTACLE_BOTTOM_IMG_PATH;

float playerSpeed = 0;
float GRAVITY_ACCELERATION;

// Index of last obstacle that was moved to front
int lastObstacleIndex = NUM_OBSTACLES - 1;

int score = 0;

bool collided = false;
bool outOfBounds = false;

void waitForSpacePress()
{
    this_thread::sleep_for(chrono::seconds(1));

    while (true)
    {
        SDL_WaitEvent(&event);

        if (event.type == SDL_QUIT)
        {
            running = false;
            break;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            break;
        }
    }
}

// Set obstacles to starting position
void resetObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstacleTop = &get<0>(obstacles[i]);
        obstacleBottom = &get<1>(obstacles[i]);

        // Set attributes for top obstacle
        obstacleTop->h = display.h / 2;
        obstacleTop->w = display.h / 6;
        obstacleTop->x = display.w + i * display.w / NUM_OBSTACLES + i * obstacleTop->w;
        obstacleTop->y = -(rand() % obstacleGap);

        // Set attributes for bottom obstacle
        obstacleBottom->h = obstacleTop->h;
        obstacleBottom->w = obstacleTop->w;
        obstacleBottom->x = obstacleTop->x;
        obstacleBottom->y = obstacleTop->y + obstacleTop->h + obstacleGap;

        obstacleTop->setImg(render, obstacleTopImgPath.c_str());
        obstacleBottom->setImg(render, obstacleBottomImgPath.c_str());

        get<2>(obstacles[i]) = false;

        lastObstacleIndex = NUM_OBSTACLES - 1;
    }
}

void playFrame()
{
    // Event management
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        // Jump
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && playerSpeed > 0)
            {
                playerSpeed = -player.h * 6.5 - score * 2;
            }
        }
    }

    playerSpeed += GRAVITY_ACCELERATION / displayRefreshRate;

    player.y += playerSpeed / displayRefreshRate;

    SDL_RenderClear(render);

    // Obstacle movement
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstacleTop = &get<0>(obstacles[i]);
        obstacleBottom = &get<1>(obstacles[i]);

        obstacleTop->x -= obstacleSpeed / displayRefreshRate;
        obstacleTop->draw();

        obstacleBottom->x -= obstacleSpeed / displayRefreshRate;
        obstacleBottom->draw();

        // Moves passed obstacles to front
        if (obstacleTop->x + obstacleTop->w < 0)
        {
            obstacleTop->x = get<0>(obstacles[lastObstacleIndex]).x + get<0>(obstacles[lastObstacleIndex]).w + display.w / NUM_OBSTACLES;
            obstacleTop->y = -(rand() % obstacleGap);

            obstacleBottom->x = obstacleTop->x;
            obstacleBottom->y = obstacleTop->y + obstacleTop->h + obstacleGap;
            
            get<2>(obstacles[i]) = false;

            lastObstacleIndex = i;
        }

        // Check if obstacle was passed (add score)
        if (player.x > obstacleTop->x + obstacleTop->w &&
            !get<2>(obstacles[i]))
        {
            score++;
            get<2>(obstacles[i]) = true;

            scoreMessage.setImg(render, ("SCORE "s + to_string(score)).c_str(), FONT_PATH, 24);

            // Set new high score
            if (score > hiScore)
            {
                hiScore = score;
                hiScoreMessage.setImg(render, ("HI SCORE "s + to_string(hiScore)).c_str(), FONT_PATH, 24);

                ofstream hiScoreFile(HI_SCORE_PATH);

                hiScoreFile << hiScore;

                hiScoreFile.close();
            }
        }

        // Collision with top obstacle
        if (player.x + player.w >= obstacleTop->x &&
            player.x <= obstacleTop->x + obstacleTop->w &&
            player.y >= obstacleTop->y &&
            player.y <= obstacleTop->y + obstacleTop->h)
        {
            collided = true;
        }

        // Collision with bottom obstacle
        if (player.x + player.w >= obstacleBottom->x &&
            player.x <= obstacleBottom->x + obstacleBottom->w &&
            player.y + player.w >= obstacleBottom->y)
        {
            collided = true;
        }

        // Out of bounds
        if (player.y + player.h <= 0 ||
            player.y >= display.h)
        {
            collided = true;
            outOfBounds = true;
        }
    }

    scoreMessage.draw();
    hiScoreMessage.draw();

    if (collided)
    {
        if (outOfBounds)
        {
            missingMessage.draw();
            outOfBounds = false;
        }
        else
        {
            player.setImg(render, playerDeadImgPath.c_str());
            player.draw();
            deathMessage.draw();
        }

        player.x = display.w / 2 - player.w / 2;
        player.y = display.h / 2 + player.w / 2;

        playerSpeed = 0;
        score = 0;

        collided = false;
        player.setImg(render, playerImgPath.c_str());
        scoreMessage.setImg(render, "SCORE 0", FONT_PATH, 24);

        SDL_RenderPresent(render);

        waitForSpacePress();
        resetObstacles();
    }
    else
    {
        player.draw();
    }

    SDL_RenderPresent(render);

    // Lock the game to the monitor refresh rate
    SDL_Delay(1000 / displayRefreshRate);
}

#endif