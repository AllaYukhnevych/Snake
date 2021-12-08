#pragma once
#include "LTexture.h"

LTexture none_texture;
LTexture snake_texture;
LTexture apple_texture;
LTexture wall_texture;
LTexture head_texture;
LTexture tail_texture;
LTexture textSnake;
LTexture text_score;
LTexture game_over;

bool loadMedia()
{
    bool success = true;

    if (!none_texture.loadFromFile("wp2409705.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!snake_texture.loadFromFile("snake.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!apple_texture.loadFromFile("apple.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!wall_texture.loadFromFile("wall.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!head_texture.loadFromFile("snake_head.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }

    if (!tail_texture.loadFromFile("snake_tail.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }

    music_game_ower = Mix_LoadMUS("game_over.wav");
    if (music_game_ower == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }


    gFont = TTF_OpenFont("SeratUltra-1GE24.ttf", 30);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        SDL_Color textColor = { 32, 32, 32 };
        if (!textSnake.loadFromRenderedText("Snake", textColor))
        {
            printf("Failed to render text texture!\n");
            success = false;
        }

    }

    SDL_Color textColor = { 255, 0, 0 };
    game_over.loadFromRenderedText("GAME OVER", textColor);
    return success;
}

