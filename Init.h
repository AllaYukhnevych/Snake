#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <ctime>

SDL_Window* window = NULL;
SDL_Renderer* gRender = NULL;
TTF_Font* gFont = NULL;
TTF_Font* gameFont = NULL;
const int field_size_x = 35;
const int field_size_y = 25;
const int cell_size = 32;
const int text_height = 60;
const int window_width = field_size_x * cell_size;
const int window_height = field_size_y * cell_size + text_height;

enum Position 
{
    FIELD_CELL_TYPE_NONE = 0,
    SNAKE_DIRECTION_RIGHT = 1, 
    SNAKE_DIRECTION_DOWN = 2,
    SNAKE_DIRECTION_LEFT = 3,
    SNAKE_DIRECTION_UP = 4,
    FIELD_CELL_TYPE_APPLE = -1,
    FIELD_CELL_TYPE_WALL = -2
};

int init() {
    bool ok = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return 1;
    }

    gRender = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRender == NULL) {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        ok = false;
    }
    SDL_SetRenderDrawColor(gRender, 138, 171, 0, 0xFF);

    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags)) {
        std::cout << "Can't init image: " << IMG_GetError() << std::endl;
        ok = false;
    }
    if (TTF_Init() !=0)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        ok = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        ok = false;
    }

    return 0;
}

void quit() {

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(gRender);

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}