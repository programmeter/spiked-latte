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
        SDL_PollEvent(&event);

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

void resetObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstacleTop = &get<0>(obstacles[i]);
        obstacleBottom = &get<1>(obstacles[i]);

        // Set attributes for top obstacle
        obstacleTop->height = winHeight / 2;
        obstacleTop->width = winWidth / 12;
        obstacleTop->x = winWidth + i * winWidth / NUM_OBSTACLES;
        obstacleTop->y = -(rand() % (winHeight / 3));

        // Set attributes for bottom obstacle
        obstacleBottom->height = obstacleTop->height;
        obstacleBottom->width = obstacleTop->width;
        obstacleBottom->x = obstacleTop->x;
        obstacleBottom->y = obstacleTop->y + obstacleTop->height + obstacleGap;

        obstacleTop->init(render, obstacleTopImgPath.c_str());
        obstacleBottom->init(render, obstacleBottomImgPath.c_str());

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
                playerSpeed = -player->height * 5.5 - score * 2;
            }
        }
    }

    playerSpeed += GRAVITY_ACCELERATION / displayRefreshRate;

    player->y += playerSpeed / displayRefreshRate;

    SDL_RenderClear(render);

    // Obstacle movement
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstacleTop = &get<0>(obstacles[i]);
        obstacleBottom = &get<1>(obstacles[i]);

        obstacleTop->x -= obstacleSpeed;
        obstacleTop->draw();

        obstacleBottom->x -= obstacleSpeed;
        obstacleBottom->draw();

        // Moves passed obstacles to front
        if (obstacleTop->x + obstacleTop->width < 0)
        {
            obstacleTop->x = get<0>(obstacles[lastObstacleIndex]).x + get<0>(obstacles[lastObstacleIndex]).width + winWidth / NUM_OBSTACLES;
            obstacleBottom->x = obstacleTop->x;
            get<2>(obstacles[i]) = false;

            lastObstacleIndex = i;
        }

        // Check if obstacle was passed (add score)
        if (player->x > obstacleTop->x + obstacleTop->width &&
            !get<2>(obstacles[i]))
        {
            score++;
            get<2>(obstacles[i]) = true;

            scoreMessage->init(render, ("SCORE "s + to_string(score)).c_str(), FONT_PATH, 24);

            // Set new high score
            if (score > hiScore)
            {
                hiScore = score;
                hiScoreMessage->init(render, ("HI SCORE "s + to_string(hiScore)).c_str(), FONT_PATH, 24);

                ofstream hiScoreFile(HI_SCORE_PATH);

                hiScoreFile << hiScore;

                hiScoreFile.close();
            }
        }

        // Collision with top obstacle
        if (player->x + player->width >= obstacleTop->x &&
            player->x <= obstacleTop->x + obstacleTop->width &&
            player->y >= obstacleTop->y &&
            player->y <= obstacleTop->y + obstacleTop->height)
        {
            collided = true;
        }

        // Collision with bottom obstacle
        if (player->x + player->width >= obstacleBottom->x &&
            player->x <= obstacleBottom->x + obstacleBottom->width &&
            player->y + player->width >= obstacleBottom->y)
        {
            collided = true;
        }

        // Out of bounds
        if (player->y + player->height <= 0 ||
            player->y >= winHeight)
        {
            collided = true;
            outOfBounds = true;
        }
    }

    scoreMessage->draw();
    hiScoreMessage->draw();

    if (collided)
    {
        if (outOfBounds)
        {
            missingMessage->draw();
            outOfBounds = false;
        }
        else
        {
            player->init(render, playerDeadImgPath.c_str());
            player->draw();
            deathMessage->draw();
        }

        player->x = winWidth / 2 - player->width / 2;
        player->y = winHeight / 2 + player->width / 2;

        playerSpeed = 0;
        score = 0;

        collided = false;
        player->init(render, playerImgPath.c_str());
        scoreMessage->init(render, "SCORE 0", FONT_PATH, 24);

        SDL_RenderPresent(render);

        waitForSpacePress();
        resetObstacles();
    }
    else
    {
        player->draw();
    }

    SDL_RenderPresent(render);

    // Lock the game to the monitor refresh rate
    SDL_Delay(1000 / displayRefreshRate);
}

#endif