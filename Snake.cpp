#include <iostream>
#include <SDL.h>
#include <time.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <ctime>



const int FIELD_CELL_TYPE_NONE= 0;
const int SNAKE_DIRECTION_UP =0;
const int SNAKE_DIRECTION_RIGHT =1;
const int SNAKE_DIRECTION_DOWN =2;
const int SNAKE_DIRECTION_LEFT =3;
const int FIELD_CELL_TYPE_APPLE = -1;
bool game_ower = false;

//створюємо вікно
SDL_Window* window = NULL;
SDL_Renderer* gRender = NULL;
SDL_Texture* none_texture = NULL;
SDL_Texture* snake_texture = NULL;
SDL_Texture* apple_texture = NULL;
SDL_Rect none;

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

const int SCREEN_WIDTH = field_size_x* cell_size;
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

    SDL_FreeSurface(temp_surf);
    SDL_FreeSurface(snake_surf);
    SDL_FreeSurface(apple_surf);
  
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

            default://змійка
                none.x = i * cell_size;
                none.y = j * cell_size;
                SDL_RenderCopy(gRender, snake_texture, NULL, &none);

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
            add_apple();
            break;
        default:
            game_ower = true;
        }
    }

    //голова змійки
    field[snake_position_y][snake_position_x ] = snake_lenght+1;

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

void handle_keyboard(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP) {
            snake_direction = SNAKE_DIRECTION_UP;
        }
        if (e.key.keysym.sym == SDLK_DOWN) {
            snake_direction = SNAKE_DIRECTION_DOWN;
        }
        if (e.key.keysym.sym == SDLK_RIGHT) {
            snake_direction = SNAKE_DIRECTION_RIGHT;
        }
        if (e.key.keysym.sym == SDLK_LEFT) {
            snake_direction = SNAKE_DIRECTION_LEFT;
        }
    }
}

//визначення клітинки дл яблука
int get_random_empty_cell()
{

    srand(time(NULL));
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
    int target_empty_cell_index = std::rand() % empty_cell_count;
    for (int j = 0; j < field_size_y; j++)
    {
        for (int i = 0; i < field_size_x; i++)
        {
            if (field[j][i] == FIELD_CELL_TYPE_NONE)
            {
                if (target_empty_cell_index == target_empty_cell_index)
                {
                    return j * field_size_x * i;
                }
                target_empty_cell_index++;
            }
        }
    }
    return -1;
}

//додавання яблука
void add_apple()
{
    srand(time(NULL));
    //розиція яблука
    int apple_pos = get_random_empty_cell();

    if (apple_pos != -1) {
       field[apple_pos / field_size_x][apple_pos % field_size_x] = FIELD_CELL_TYPE_APPLE;
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

    SDL_Quit();
    IMG_Quit();
}

int main(int argc, char** args) {

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

        while (close) {
            while (SDL_PollEvent(&e) != NULL) {
                if (e.type == SDL_QUIT) {
                    close = false;
                }
            }
            if (game_ower)
            {
                quit();
        }
            make_move();
            add_apple();
            //замальовка фона
            draw_field();
            handle_keyboard(e);
            //швидкість змійки
            SDL_Delay(100);
        }

       
    quit();
    
    return 0;
};


