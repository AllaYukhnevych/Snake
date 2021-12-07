#pragma once
#include "ManagerTexture.h"

class Snake {

private:
    // snake position 
    int snake_position_x = field_size_x / 2;
    int snake_position_y = field_size_y / 2;

    int snake_lenght = 4;
    // the initial direction of movement of the snake 
    int snake_direction = SNAKE_DIRECTION_RIGHT;

    int field[field_size_y][field_size_x];

public:
    Snake() {};
    ~Snake() {};

    friend class Game;
};

