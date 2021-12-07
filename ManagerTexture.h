#pragma once
#include "LTexture.h"

LTexture none_texture;
LTexture snake_texture;
LTexture apple_texture;
LTexture wall_texture;

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
    return success;
}

