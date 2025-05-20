#ifndef RENDER_H
#define RENDER_H
#include <stdio.h>
#include "/opt/homebrew/Cellar/sdl2/2.32.4/include/SDL2/SDL.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2/SDL_ttf.h"


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

#define GRID_SIZE 36
#define GRID_DIM 500
#define CELL_SIZE (GRID_DIM / GRID_SIZE)
#define SCORE_X_POS 80
#define SCORE_Y_POS 80
#define DELAY 70
#define DELAY_TIME 3000

/*todo*/
/*making it cross platform */
/*scaling factor in MacOs retina display shoulf be compiled only for MAC_OS*/
#ifndef WIN_API
#define WIN_API
#endif

extern int delay ;
extern int scoring;
extern char score_update[32];
extern TTF_Font *font ;
extern char * game_name;
extern int mode ;
enum playmode{PLAY, AI};

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

typedef struct score {
    int x;
    int y;
}Score;

Score newScore;
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

enum{
    TRY_FOWARD,
    TRY_LEFT,
    TRY_RIGHT
}State;

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
void snake_ai();
void turn_left();
void turn_right();
int game_status(int);
void update_score();
void render_score(SDL_Renderer* renderer, int x, int y, int scale_factor);
snakePtr reverseList(snakePtr head);
void renderScore(SDL_Renderer* screen, int scoring_x, int scoring_y, int scale_factor, TTF_Font*);
void loadFont();
void renderGameName(SDL_Renderer* screen, int , int , int scale_factor,TTF_Font*);
//void rewardPoints();
void init_score();

 #endif
