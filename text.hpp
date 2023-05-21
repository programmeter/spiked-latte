#ifndef TEXT_HPP
#define TEXT_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "common.hpp"

class Text
{
public:
    int x = 0;
    int y = 0;
    SDL_Rect hitbox;

    void init(SDL_Renderer *paramRender, const char *text, const char *fontPath, int fontSize)
    {
        render = paramRender;

        font = TTF_OpenFont(fontPath, fontSize);
        surface = TTF_RenderText_Solid(font, text, colorWhite); 
        texture = SDL_CreateTextureFromSurface(render, surface);

        SDL_QueryTexture(texture, NULL, NULL, &hitbox.w, &hitbox.h);

        SDL_FreeSurface(surface);
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
    TTF_Font *font;
    SDL_Renderer *render;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color colorWhite = {255, 255, 255};
};

#endif