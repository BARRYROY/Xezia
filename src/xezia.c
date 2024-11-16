#include <stdio.h>
#include "/opt/homebrew/Cellar/sdl2/2.30.8/include/SDL2/SDL.h"
#include <time.h>

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
#define GRID_DIM 650
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
    
};

void init_snake();
void increase_snake();
void render_grid(SDL_Renderer* renderer, int x, int y);
void render_snake();
void move_snake();
void draw_apple(SDL_Renderer* renderer, int x, int y);
void gen_apple();
void detect_apple();

int main(){

    srand(time(NULL));

    init_snake();
    gen_apple();
    increase_snake();
    increase_snake();

    // initialize Sdl
    if( (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) < 0 ){
        printf("ERROR! Failed to initialize sdl: %s.\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window;
    SDL_Renderer* render;

    window=SDL_CreateWindow("Xezia",
                            SCREEN_X, SCREEN_Y,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_BORDERLESS 
                            );
    if(!window){
        printf("ERROR! Failed to create window : %s.\n", SDL_GetError());
        return 1;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!render){
        printf("ERROR! Failed to create render : %s.\n", SDL_GetError());
        return 1;
    }

    printf("Game is running ...\n");

    SDL_SetRenderDrawColor(render,0x11, 0x11,0x11, 0x11);

    SDL_RenderClear(render);

    SDL_RenderPresent(render);

    SDL_Delay(3000);

    int grid_x = (SCREEN_WIDTH / 2) - (GRID_DIM /2);
    int grid_y = (SCREEN_HEIGHT / 2) - (GRID_DIM /2);

    SDL_Event event;
    int quit = 0;

    while(!quit){
        printf("Game is running...\n");

        while(SDL_PollEvent(&event)){    
            switch(event.type){
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_KEYUP:
                    break;

                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        quit = 1;
                    }else {
                        switch(event.key.keysym.sym){
                            case SDLK_UP:
                                if(head->dir != SNAKE_DOWN){
                                    head->dir = SNAKE_UP;
                                }
                                break;
                            case SDLK_DOWN:
                                if(head->dir != SNAKE_UP){
                                    head->dir = SNAKE_DOWN;
                                }
                                break;

                            case SDLK_RIGHT:
                                if(head->dir != SNAKE_LEFT){
                                    head->dir = SNAKE_RIGHT;
                                }
                                break;

                            case SDLK_LEFT:
                                if(head->dir != SNAKE_RIGHT){
                                    head->dir = SNAKE_LEFT;
                                }
                                break;

                        }
                    } 
                    break;
                

            }
        }

        move_snake();

        detect_apple();

        SDL_RenderClear(render);

        render_grid(render, grid_x, grid_y);

        render_snake(render, grid_x, grid_y);

        draw_apple(render, grid_x, grid_x);

        SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);

        SDL_RenderPresent(render);

        SDL_Delay(100);

    }


    printf("shutting down...\n");
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void render_grid(SDL_Renderer* renderer, int x, int y){
    
    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x55);
    SDL_Rect cell;
    cell.h = CELL_SIZE;
    cell.w = CELL_SIZE;

    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            cell.x = x + (i * CELL_SIZE);
            cell.y = y + (j * CELL_SIZE);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

}

void init_snake(){
    snakePtr new = malloc(sizeof(Snake));
    new->x = rand() % (GRID_SIZE/2) + (GRID_SIZE / 4 );
    new->y = 1;
    new->dir = SNAKE_UP;
    new->next = NULL;

    head = new;
    tail = new;
}

void increase_snake(){
    snakePtr new = malloc(sizeof(Snake));
    if (!new) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new->x = tail->x;
    new->y = tail->y - 1;
    new->dir = tail->dir;
    new->next = NULL;
    tail->next = new;
    tail = new;
    
}

void render_snake(SDL_Renderer* renderer, int x, int y){
    SDL_SetRenderDrawColor(renderer,0x00, 0xff, 0x00, 255 );

    int seg_size = GRID_DIM / GRID_SIZE  ;
    SDL_Rect seg;
    seg.w = seg_size;
    seg.h = seg_size;

    snakePtr track  = head;
    while(track != NULL){
        seg.x = x + (track->x * seg_size);
        seg.y = y + (track->y * seg_size);
        SDL_RenderFillRect(renderer, &seg);
        track = track->next;
    }

}

/* a function to move snake directionally*/
void move_snake(){
    int prev_x = head->x;
    int prev_y = head->y;

    switch(head->dir){
        case SNAKE_UP:
            head->y--;
            break;
        case SNAKE_DOWN:
            head->y++;
            break;
        case SNAKE_LEFT:
            head->x--;
            break;
        case SNAKE_RIGHT:
            head->x++;
            break;
    }

    snakePtr track = head->next;
    /*int seg_index = 1;*/
    while (track != NULL) {
        int save_x = track->x;
        int save_y = track->y;

        track->x = prev_x;
        track->y = prev_y;
        /*printf("Seg %d moved to (%d, %d)\n", seg_index++, track->x, track->y);*/ 
        prev_x = save_x;
        prev_y = save_y;

        track = track->next;
    }
}


void gen_apple(){

    newBall.x= rand() % GRID_SIZE ;
    newBall.y = rand() % GRID_SIZE;

}

void draw_apple(SDL_Renderer* renderer, int x, int y){
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);
    //gen_apple();
    SDL_Rect ball;
    ball.x = x + newBall.x * CELL_SIZE;
    ball.y = y + newBall.y * CELL_SIZE;
    ball.h = CELL_SIZE;
    ball.w = CELL_SIZE;

    SDL_RenderFillRect(renderer, &ball);
}

void detect_apple(){

    if( (head->x == newBall.x ) &&(head->y = newBall.y)){
        gen_apple();
        increase_snake();
    }
}
