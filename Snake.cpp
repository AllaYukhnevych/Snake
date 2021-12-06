#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <ctime>



const int FIELD_CELL_TYPE_NONE = 0;
const int SNAKE_DIRECTION_UP = 0;
const int SNAKE_DIRECTION_RIGHT = 1;
const int SNAKE_DIRECTION_DOWN = 2;
const int SNAKE_DIRECTION_LEFT = 3;
const int FIELD_CELL_TYPE_APPLE = -1;
const int FIELD_CELL_TYPE_WALL = -2;
bool game_ower = false;

//створюємо вікно
SDL_Window* window = NULL;
SDL_Renderer* gRender = NULL;
SDL_Texture* none_texture = NULL;
SDL_Texture* snake_texture = NULL;
SDL_Texture* apple_texture = NULL;
SDL_Texture* wall_texture = NULL;
SDL_Rect none;

Mix_Music* music_game_ower = NULL;
//кількість клітинок
const int field_size_x = 35;
const int field_size_y = 25;
//розмір клітинки
const int cell_size = 32;

//позиція змійки
int snake_position_x = field_size_x / 2;
int snake_position_y = field_size_y / 2;

//довжина змійки
int snake_lenght = 4;
//напрямок змійки
int snake_direction = SNAKE_DIRECTION_RIGHT;

const int SCREEN_WIDTH = field_size_x * cell_size;
const int SCREEN_HEIGHT = field_size_y * cell_size;

//інформація, що міститься в клітинці
int field[field_size_y][field_size_x];
void add_apple();

//заповнення вікна
void clear_field()
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            field[j][i] = FIELD_CELL_TYPE_NONE;
        }
    }

    //положення змійки
    for (int i = 0; i < snake_lenght; i++)
    {
        field[snake_position_y][snake_position_x - i] = snake_lenght - i;
    }

    //малювання стіни
    for (int i = 0; i < field_size_x; i++)
    {
        field[0][i] = FIELD_CELL_TYPE_WALL;
        field[field_size_y - 1][i] = FIELD_CELL_TYPE_WALL;
    }

    for (int i = 0; i < field_size_y - 1; i++)
    {
        field[i][0] = FIELD_CELL_TYPE_WALL;
        field[i][field_size_x - 1] = FIELD_CELL_TYPE_WALL;
    }

    add_apple();
}




int init() {
    bool ok = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return 1;
    }

    gRender = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRender == NULL) {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        ok = false;
    }
    SDL_SetRenderDrawColor(gRender, 200, 249, 255, 0xFF);

    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags)) {
        std::cout << "Can't init image: " << IMG_GetError() << std::endl;
        ok = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        ok = false;
    }

    return 0;
}


//завантаження зображення
bool load() {
    bool ok = true;
    SDL_Surface* temp_surf = NULL;
    temp_surf = IMG_Load("none.png");
    if (temp_surf == NULL) {
        std::cout << "Can't load image: " << IMG_GetError() << std::endl;
        ok = false;
    }

    none_texture = SDL_CreateTextureFromSurface(gRender, temp_surf);
    if (none_texture == NULL) {
        std::cout << "Can't create texture from surface: " << SDL_GetError() << std::endl;
        ok = false;
    }

    SDL_Surface* snake_surf = NULL;
    snake_surf = IMG_Load("snake.png");
    if (snake_surf == NULL) {
        std::cout << "Can't load image: " << IMG_GetError() << std::endl;
        ok = false;
    }

    snake_texture = SDL_CreateTextureFromSurface(gRender, snake_surf);
    if (snake_texture == NULL) {
        std::cout << "Can't create texture from surface: " << SDL_GetError() << std::endl;
        ok = false;
    }

    SDL_Surface* apple_surf = NULL;
    apple_surf = IMG_Load("apple.png");
    if (apple_surf == NULL) {
        std::cout << "Can't load image: " << IMG_GetError() << std::endl;
        ok = false;
    }

    apple_texture = SDL_CreateTextureFromSurface(gRender, apple_surf);
    if (apple_texture == NULL) {
        std::cout << "Can't create texture from surface: " << SDL_GetError() << std::endl;
        ok = false;
    }

    SDL_Surface* wall_surf = NULL;
    wall_surf = IMG_Load("wall.png");
    if (wall_surf == NULL) {
        std::cout << "Can't load image: " << IMG_GetError() << std::endl;
        ok = false;
    }

    wall_texture = SDL_CreateTextureFromSurface(gRender, wall_surf);
    if (wall_texture == NULL) {
        std::cout << "Can't create texture from surface: " << SDL_GetError() << std::endl;
        ok = false;
    }
    SDL_FreeSurface(temp_surf);
    SDL_FreeSurface(snake_surf);
    SDL_FreeSurface(apple_surf);
    SDL_FreeSurface(wall_surf);

    //завантаження музики
    music_game_ower = Mix_LoadMUS("game_over.wav");
    if (music_game_ower == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        ok = false;
    }

    return ok;
}

void draw_field()
{
    //замальовка фона
    none.x = 0;
    none.y = 0;
    none.w = cell_size;
    none.h = cell_size;

    SDL_RenderPresent(gRender);
    SDL_RenderClear(gRender);

    //вивід на екран
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            switch (field[j][i])
            {
            case FIELD_CELL_TYPE_NONE:
                none.x = i * cell_size;
                none.y = j * cell_size;
                SDL_RenderCopy(gRender, none_texture, NULL, &none);
                break;

            case FIELD_CELL_TYPE_APPLE:
                none.x = i * cell_size;
                none.y = j * cell_size;
                SDL_RenderCopy(gRender, apple_texture, NULL, &none);
                break;
            case FIELD_CELL_TYPE_WALL:
                none.x = i * cell_size;
                none.y = j * cell_size;
                SDL_RenderCopy(gRender, wall_texture, NULL, &none);
                break;
            default://змійка
                none.x = i * cell_size;
                none.y = j * cell_size;
                SDL_RenderCopy(gRender, snake_texture, NULL, &none);

            }
        }
    }
}

void grow_snake()
{
    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (field[j][i] > FIELD_CELL_TYPE_NONE)
            {
                field[j][i]++;
            }
        }
    }

}
//переміщення змійки
void make_move()
{
    switch (snake_direction)
    {
    case SNAKE_DIRECTION_UP:
        snake_position_y--;
        if (snake_position_y < 0)
        {
            snake_position_y = field_size_y - 1;
        }
        break;
    case SNAKE_DIRECTION_RIGHT:
        snake_position_x++;
        if (snake_position_x > field_size_x - 1)
        {
            snake_position_x = 0;
        }
        break;
    case SNAKE_DIRECTION_LEFT:
        snake_position_x--;
        if (snake_position_x < 0)
        {
            snake_position_x = field_size_x - 1;
        }
        break;
    case SNAKE_DIRECTION_DOWN:
        snake_position_y++;
        if (snake_position_y > field_size_y - 1)
        {
            snake_position_y = 0;
        }
        break;
    }

    if (field[snake_position_y][snake_position_x] != FIELD_CELL_TYPE_NONE)
    {
        switch (field[snake_position_y][snake_position_x])
        {
        case FIELD_CELL_TYPE_APPLE:
            snake_lenght++;
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
    field[snake_position_y][snake_position_x] = snake_lenght + 1;

    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (field[j][i] > FIELD_CELL_TYPE_NONE)
            {
                field[j][i]--;
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
            if (field[j][i] == FIELD_CELL_TYPE_NONE)
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
            if (field[j][i] == FIELD_CELL_TYPE_NONE)
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
        field[apple_pos / field_size_x][apple_pos / field_size_x] = FIELD_CELL_TYPE_APPLE;
    }
}



void quit() {

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(gRender);
    gRender = NULL;

    SDL_DestroyTexture(none_texture);
    none_texture = NULL;

    SDL_DestroyTexture(snake_texture);
    snake_texture = NULL;

    SDL_DestroyTexture(apple_texture);
    apple_texture = NULL;

    SDL_DestroyTexture(wall_texture);
    wall_texture = NULL;

    SDL_Quit();
    IMG_Quit();
}

int main(int argc, char** args) {

    srand(time(NULL));
    std::vector<int> snake_direction_queue;

    if (init() == 1) {
        return 1;
    }
    if (!load()) {
        quit();
        return 1;
    }
    //чисте поле
    clear_field();

    //закриття вікна
    bool close = true;
    SDL_Event e;
    int new_snake_direction = snake_direction;
    while (close) {

        while (SDL_PollEvent(&e) != NULL) {
            if (e.type == SDL_QUIT) {
                close = false;
            }

            int snake_direction_last = snake_direction_queue.empty() ? snake_direction : snake_direction_queue.at(0);
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
        snake_direction = new_snake_direction;


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


    quit();

    return 0;
};