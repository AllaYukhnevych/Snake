
#include "ManagerTexture.h"
#include "Information_snake.h"
bool game_ower = false;

Snake snake;

void add_apple();

void clear_field()
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            snake.field[j][i] = FIELD_CELL_TYPE_NONE;
        }
    }

    //положення змійки
    for (int i = 0; i < snake.snake_lenght; i++)
    {
        snake.field[snake.snake_position_y][snake.snake_position_x - i] = snake.snake_lenght - i;
    }

    //малювання стіни
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

    add_apple();
}










void draw_field()
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
            default://змійка
                snake_texture.render(i * cell_size, j * cell_size);
            }
        }
    }
    SDL_RenderPresent(gRender);
}

void grow_snake()
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
//переміщення змійки
void make_move()
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
            grow_snake();
            add_apple();
            break;
        default:

            game_ower = true;
            Mix_PlayMusic(music_game_ower, -1);
            SDL_Delay(3000);

        }
    }

    //голова змійки
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


//визначення клітинки дл яблука
int get_random_empty_cell()
{
    //лічильник пустих клітинок
    int empty_cell_count = 0;
    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (snake.field[j][i] == FIELD_CELL_TYPE_NONE)
            {
                //якщо клітинка пуста, то сумується
                empty_cell_count++;
            }
        }
    }

    //індекс випадкової клітинки. Випадкове число від суми пустих клітинок
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

//додавання яблука
void add_apple()
{
    //розиція яблука
    int apple_pos = get_random_empty_cell();

    if (apple_pos != -1) {
        snake.field[apple_pos / field_size_x][apple_pos / field_size_x] = FIELD_CELL_TYPE_APPLE;
    }

}

void event()
{
    srand(time(NULL));
    std::vector<int> snake_direction_queue;
    bool close = true;
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
        make_move();
        //замальовка фона
        draw_field();
        //швидкість змійки
        SDL_Delay(100);
    }
}

int main(int argc, char** args) {
    if (init() == 1) {
        return 1;
    }
    if (!loadMedia()) {
        quit();
        return 1;
    }

    clear_field();

    event();
    quit();
    return 0;
};