#pragma once
#include "LTexture.h"


int menu_width = 500;
int menu_height = 300;
int const menu_position_x = (window_width - menu_width) / 2;
int const menu_position_y = (window_height - menu_height) / 2;


void draw_menu()
{
    menu_texture.render(menu_position_x, menu_position_y);
    SDL_RenderPresent(gRender);
}

