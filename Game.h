#pragma once
#include "Information_snake.h"
#include "Menu.h"

class Game {

private:
    Snake snake;
    bool g_over = false;
    bool game_pause = false;

public:

    Game() {};

    Game(Snake snake) { snake = this->snake; };

    void clear_field(Snake& snake);

    void draw_field(Snake& snake);

    void draw_score(Snake& snake);

    void grow_snake(Snake& snake);

    void start_game(Snake& snake);

    void finish_game();

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
        snake.field[snake.snake_position_y ][snake.snake_position_x - i] = snake.snake_lenght - i;
    }

    // painting the wall 
    for (int i = 0; i < field_size_x; i++)
    {
        if (i < 5 || field_size_x - i - 1 < 5) {
            snake.field[0][i] = FIELD_CELL_TYPE_WALL;
            snake.field[field_size_y - 1][i] = FIELD_CELL_TYPE_WALL;
        }
    }

    for (int i = 0; i < field_size_y - 1; i++)
    {
        if (i < 5|| field_size_y - i - 1 < 5) {
        snake.field[i][0] = FIELD_CELL_TYPE_WALL;
        snake.field[i][field_size_x - 1] = FIELD_CELL_TYPE_WALL;
        }
    }

    add_apple(snake);
}

void Game::draw_field(Snake& snake)
{
    SDL_RenderClear(gRender);
 
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            switch (snake.field[j][i])
            {
            case FIELD_CELL_TYPE_NONE:
                none_texture.render(i * cell_size, j * cell_size+text_height);
                break;
            case FIELD_CELL_TYPE_APPLE:
                apple_texture.render(i * cell_size, j * cell_size+ text_height);
                break;
            case FIELD_CELL_TYPE_WALL:
                wall_texture.render(i * cell_size, j * cell_size+ text_height);
                break;
            default://snake
                if (snake.field[j][i] == snake.snake_lenght+1) {
                   head_texture.render((i * cell_size ), (j * cell_size+ text_height));

                  // tail_texture.render(i * cell_size- cell_size*snake.snake_lenght, j *cell_size + text_height);
                    double degrees = 0;
                    switch (snake.snake_direction) {
                    case SNAKE_DIRECTION_UP:
                        degrees = 0;
                        head_texture.render((i * cell_size), (j * cell_size+ text_height), NULL, degrees);
                        break;
                    case SNAKE_DIRECTION_RIGHT:
                        degrees = 90;
                       head_texture.render((i * cell_size), (j * cell_size+ text_height), NULL, degrees);

                        break;
                    case SNAKE_DIRECTION_DOWN:
                        degrees = 180;
                        head_texture.render((i * cell_size), (j * cell_size+ text_height), NULL, degrees);
                        break;
                    case SNAKE_DIRECTION_LEFT:
                        degrees = -90;
                        head_texture.render((i * cell_size), (j * cell_size+ text_height), NULL, degrees);

                        //tail_texture.render(i * cell_size , j * cell_size + text_height);
                        break;
                    }
                }
               
                else {
                    snake_texture.render(i * cell_size, j * cell_size+ text_height);
                }
            }
        }
    }
    //evaluation conclusion 
    draw_score(snake);
    textSnake.render(50,20);
    SDL_RenderPresent(gRender);
}

void Game::draw_score(Snake& snake)
{
    SDL_Color textColor = { 0, 102, 0 };
    text_score.loadFromRenderedText("Score " + std::to_string(snake.score), textColor);
    text_score.render(950, 20);
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

void Game::start_game(Snake& snake)
{
    snake.snake_position_x = field_size_x / 2;
    snake.snake_position_y = field_size_y / 2;
    snake.snake_lenght = 4;
    snake.snake_direction = SNAKE_DIRECTION_RIGHT;
    snake.score = 0;
    g_over = false;
    game_pause = false;
    clear_field(snake);
}

void Game::finish_game()
{
    game_pause = false;
    g_over = true;
    game_over.render(500, 400);
    SDL_RenderPresent(gRender);
    SDL_Delay(2000);
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
            snake.score++;
            grow_snake(snake);
            add_apple(snake);
            break;
        default:
            finish_game();
        }
    }

    if (!g_over) {

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

        snake.field[snake.snake_position_y][snake.snake_position_x] = snake.snake_lenght + 1;
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
            if (snake.field[j][i] == FIELD_CELL_TYPE_NONE )
            {
                if (snake.field[j][i] != FIELD_CELL_TYPE_WALL)
                {
                    empty_cell_count++;
                }
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
    srand(time(NULL));
    if (init() == 1) {
        return 1;
    }
    if (!loadMedia()) {
        quit();
        return 1;
    }
    clear_field(snake);
    
    bool close = true;
    std::vector<int> snake_direction_queue;
    SDL_Event e;
    
    while (close) {

        while (SDL_PollEvent(&e) != NULL)
        {
            if (e.type == SDL_QUIT) {
                close = false;
            }
            if (game_pause == true)
            {
                switch (e.key.keysym.sym) {
                case SDLK_DOWN:
                    break;
                case SDLK_a:

                    break;

                case SDLK_RETURN:
                    game_pause = false;
                    break;
                }
            }
            else
            {
                int new_snake_direction = snake.snake_direction;
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

                case SDLK_F1:
                    game_pause = true;
                    break;

                }

                snake.snake_direction = new_snake_direction;
            }
        }
            

        

        if (!game_pause) {
            make_move(snake);
        }

        draw_field(snake);
        

        if (g_over)
        {
            finish_game();
            start_game(snake);
        }

        if (game_pause) {
            draw_menu();
        }

       SDL_Delay(100);
    }

    quit();
    return 0;
}

