#pragma once
#include "Information_snake.h"

class Game {

private:
    Snake snake;
    bool game_ower = false;

public:
    Game() {};

    Game(Snake snake) { snake = this->snake; };

    void clear_field(Snake& snake);

    void draw_field(Snake& snake);

    void grow_snake(Snake& snake);

    void make_move(Snake& snake);

    int get_random_empty_cell(Snake& snake);

   void add_apple(Snake& snake);

    int event(Snake& snake);

};

void Game::clear_field(Snake& snake)
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            snake.field[j][i] = FIELD_CELL_TYPE_NONE;
        }
    }

    for (int i = 0; i < snake.snake_lenght; i++)
    {
        snake.field[snake.snake_position_y][snake.snake_position_x - i] = snake.snake_lenght - i;
    }

    // painting the wall 
    for (int i = 0; i < field_size_x; i++)
    {
        snake.field[0][i] = FIELD_CELL_TYPE_WALL;
        snake.field[field_size_y - 1][i] = FIELD_CELL_TYPE_WALL;
    }

    for (int i = 0; i < field_size_y - 1; i++)
    {
        snake.field[i][0] = FIELD_CELL_TYPE_WALL;
        snake.field[i][field_size_x - 1] = FIELD_CELL_TYPE_WALL;
    }

    add_apple(snake);
}

void Game::draw_field(Snake& snake)
{
    SDL_RenderPresent(gRender);
    SDL_RenderClear(gRender);

    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            switch (snake.field[j][i])
            {
            case FIELD_CELL_TYPE_NONE:
                none_texture.render(i * cell_size, j * cell_size);
                break;
            case FIELD_CELL_TYPE_APPLE:
                apple_texture.render(i * cell_size, j * cell_size);
                break;
            case FIELD_CELL_TYPE_WALL:
                wall_texture.render(i * cell_size, j * cell_size);
                break;
            default://snake
                snake_texture.render(i * cell_size, j * cell_size);
            }
        }
    }
    SDL_RenderPresent(gRender);
}

void Game::grow_snake(Snake& snake)
{
    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (snake.field[j][i] > FIELD_CELL_TYPE_NONE)
            {
                snake.field[j][i]++;
            }
        }
    }

}

void Game::make_move(Snake& snake)
{
    switch (snake.snake_direction)
    {
    case SNAKE_DIRECTION_UP:
        snake.snake_position_y--;
        if (snake.snake_position_y < 0)
        {
            snake.snake_position_y = field_size_y - 1;
        }
        break;
    case SNAKE_DIRECTION_RIGHT:
        snake.snake_position_x++;
        if (snake.snake_position_x > field_size_x - 1)
        {
            snake.snake_position_x = 0;
        }
        break;
    case SNAKE_DIRECTION_LEFT:
        snake.snake_position_x--;
        if (snake.snake_position_x < 0)
        {
            snake.snake_position_x = field_size_x - 1;
        }
        break;
    case SNAKE_DIRECTION_DOWN:
        snake.snake_position_y++;
        if (snake.snake_position_y > field_size_y - 1)
        {
            snake.snake_position_y = 0;
        }
        break;
    }

    if (snake.field[snake.snake_position_y][snake.snake_position_x] != FIELD_CELL_TYPE_NONE)
    {
        switch (snake.field[snake.snake_position_y][snake.snake_position_x])
        {
        case FIELD_CELL_TYPE_APPLE:
            snake.snake_lenght++;
            grow_snake(snake);
            add_apple(snake);
            break;
        default:

            game_ower = true;
            Mix_PlayMusic(music_game_ower, -1);
            SDL_Delay(3000);

        }
    }

    // snake head 
    snake.field[snake.snake_position_y][snake.snake_position_x] = snake.snake_lenght + 1;

    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (snake.field[j][i] > FIELD_CELL_TYPE_NONE)
            {
                snake.field[j][i]--;
            }
        }
    }

}

// determine the cell for the apple 
int Game::get_random_empty_cell(Snake& snake)
{
    // empty cell counter 
    int empty_cell_count = 0;
    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (snake.field[j][i] == FIELD_CELL_TYPE_NONE)
            {
                empty_cell_count++;
            }
        }
    }

    // random cell index. Random number from the sum of empty cells 
    int target_empty_cell_index = rand() % empty_cell_count;
    int empty_cell_index = 0;
    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (snake.field[j][i] == FIELD_CELL_TYPE_NONE)
            {
                if (empty_cell_index == target_empty_cell_index)
                {
                    return j * field_size_x + i;
                }
                empty_cell_index++;
            }
        }
    }
    return -1;
}

void Game::add_apple(Snake& snake)
{
    int apple_pos = get_random_empty_cell(snake);

    if (apple_pos != -1) {
        snake.field[apple_pos / field_size_x][apple_pos / field_size_x] = FIELD_CELL_TYPE_APPLE;
    }

}

int Game::event(Snake& snake)
{
    if (init() == 1) {
        return 1;
    }
    if (!loadMedia()) {
        quit();
        return 1;
    }

    srand(time(NULL));
    clear_field(snake);
    bool close = true;
    std::vector<int> snake_direction_queue;
    SDL_Event e;
    int new_snake_direction = snake.snake_direction;
    
    while (close) {

        while (SDL_PollEvent(&e) != NULL) {
            if (e.type == SDL_QUIT) {
                close = false;
            }

            int snake_direction_last = snake_direction_queue.empty() ? snake.snake_direction : snake_direction_queue.at(0);
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (snake_direction_last != SNAKE_DIRECTION_DOWN)
                {
                    new_snake_direction = SNAKE_DIRECTION_UP;
                }
                break;
            case SDLK_DOWN:
                if (snake_direction_last != SNAKE_DIRECTION_UP)
                {
                    new_snake_direction = SNAKE_DIRECTION_DOWN;
                }
                break;
            case SDLK_RIGHT:
                if (snake_direction_last != SNAKE_DIRECTION_LEFT)
                {
                    new_snake_direction = SNAKE_DIRECTION_RIGHT;
                }
                break;
            case SDLK_LEFT:
                if (snake_direction_last != SNAKE_DIRECTION_RIGHT)
                {
                    new_snake_direction = SNAKE_DIRECTION_LEFT;
                }
                break;
            }
        }
        snake.snake_direction = new_snake_direction;

        if (game_ower)
        {
            quit();
        }

        make_move(snake);

        draw_field(snake);

        SDL_Delay(100);
    }

    quit();
}