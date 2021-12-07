#pragma once
#include "ManagerTexture.h"

class Snake {
public:
    int snake_position_x = field_size_x / 2;
    int snake_position_y = field_size_y / 2;

    int snake_lenght = 4;

    int snake_direction = SNAKE_DIRECTION_RIGHT;
    int field[field_size_y][field_size_x];
};

