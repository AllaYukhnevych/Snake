#pragma once
#include "LTexture.h"

LTexture none_texture;
LTexture snake_texture;
LTexture apple_texture;
LTexture wall_texture;
LTexture head_texture;
LTexture textSnake;
LTexture text_score;
LTexture game_over_texture;
LTexture menu_texture;
LTexture new_game_texture;

Mix_Music* music_game_ower = NULL;

bool loadMedia()
{
    bool success = true;

    if (!none_texture.loadFromFile("image/wp2409705.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!snake_texture.loadFromFile("image/snake.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!apple_texture.loadFromFile("image/apple.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!wall_texture.loadFromFile("image/wall.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }
    if (!head_texture.loadFromFile("image/snake_head.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }

    if (!menu_texture.loadFromFile("image/pause.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }

    if (!game_over_texture.loadFromFile("image/gameover.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }

    if (!new_game_texture.loadFromFile("image/new_game.png"))
    {
        printf("Failed to load arrow texture!\n");
        success = false;
    }

    music_game_ower = Mix_LoadMUS("image/game_over.wav");
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

     return success;
}

void draw_menu()
{
    menu_texture.render(300, 300);
    SDL_RenderPresent(gRender);
}