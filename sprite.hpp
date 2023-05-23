#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

class Sprite
{

public:
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    void init(SDL_Renderer *paramRender, const char *imgPath, bool manuallySetSize = true)
    {
        hitbox.w = width;
        hitbox.h = height;

        render = paramRender;

        surface = IMG_Load(imgPath);
        texture = SDL_CreateTextureFromSurface(render, surface);
        SDL_FreeSurface(surface);

        if (!manuallySetSize)
        {
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        }
    }

    void destroy()
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(render);
    }

    void draw()
    {
        hitbox.x = x;
        hitbox.y = y;

        SDL_RenderCopy(render, texture, NULL, &hitbox);
    }

private:
    std::string imgPath;
    SDL_Renderer *render;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
};

#endif