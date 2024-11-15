#include <stdio.h>
#include "/opt/homebrew/Cellar/sdl2/2.30.8/include/SDL2/SDL.h"

#if 0
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define SCREEN_X 0
#define SCREEN_Y 0
#else
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 600
#define SCREEN_X 10
#define SCREEN_Y 10
#endif

#define GRID_SIZE 20
#define GRID_DIM 500

typedef struct snake{
    int x;
    int y;
    int dir;
    struct snake* next;
}Snake;

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

int main(){

    init_snake();

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

    //SDL_RenderDrawRect(renderer,&r);

    SDL_RenderPresent(render);

    SDL_Delay(3000);

    int grid_x = (SCREEN_WIDTH / 2) - (GRID_DIM /2);
    int grid_y = (SCREEN_HEIGHT / 2) - (GRID_DIM /2);

    SDL_Event event;
    int quit = 0;
//   int buttonClicked = 0;

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
                    }
                    break;
                

            }
        }

        

        SDL_RenderClear(render);

        render_grid(render, grid_x, grid_y);

        render_snake(render, grid_x, grid_y);

        SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);

        SDL_RenderPresent(render);

        SDL_Delay(10);

    }


    printf("shutting down...\n");
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


void render_grid(SDL_Renderer* renderer, int x, int y){
    int cell_size = GRID_DIM/GRID_SIZE;

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x55);
    SDL_Rect cell;
    cell.h = cell_size;
    cell.w = cell_size;

    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
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

    int seg_size = GRID_DIM/GRID_SIZE;
    SDL_Rect seg;
    seg.w = seg_size;
    seg.h = seg_size;

    snakePtr track  = head;
    while(track != NULL){
        seg.x = x + track->x *seg_size;
        seg.y = y + track->y *seg_size;
        SDL_RenderFillRect(renderer, &seg);
        track = track->next;
    }


}
