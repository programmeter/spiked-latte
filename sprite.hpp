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
    int w = 0;
    int h = 0;

    void setImg(SDL_Renderer *paramRender, const char *imgPath, bool autoSetSize = false)
    {
        hitbox.w = w;
        hitbox.h = h;

        render = paramRender;

        surface = IMG_Load(imgPath);
        texture = SDL_CreateTextureFromSurface(render, surface);
        SDL_FreeSurface(surface);

        if (autoSetSize)
        {
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
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