#include <stdio.h>
#include "/opt/homebrew/Cellar/sdl2/2.30.8/include/SDL2/SDL.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#if 0
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define SCREEN_X 0
#define SCREEN_Y 0
#else
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define SCREEN_X 0
#define SCREEN_Y 0
#endif

#define GRID_SIZE 20
#define GRID_DIM 500
#define CELL_SIZE (GRID_DIM / GRID_SIZE)

typedef struct snake{
    int x;
    int y;
    int dir;
    struct snake* next;
}Snake;

typedef struct ball{
    int x;
    int y;
}Ball;

Ball newBall;
typedef Snake* snakePtr;

snakePtr head;
snakePtr tail;


enum{
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_RIGHT,
    SNAKE_LEFT,
    
}Direction;


void init_snake();
void increase_snake();
void render_grid(SDL_Renderer* renderer, int x, int y, int scale_factor );
void render_snake(SDL_Renderer* renderer, int x, int y, int scale_factor );
void move_snake();
void render_apple(SDL_Renderer* renderer, int x, int y, int scale_factor);
void gen_apple();
void detect_apple();
void detect_crash();
void reset_snake();
