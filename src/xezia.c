#include "xezia.h"

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
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_BORDERLESS |SDL_WINDOW_ALLOW_HIGHDPI 
                            );
    if(!window){
        printf("ERROR! Failed to create window : %s.\n", SDL_GetError());
        return 1;
    }

    int drawable_width, drawable_height;
    SDL_GL_GetDrawableSize(window, &drawable_width, &drawable_height);

    printf("drawable screen size is (%d, %d) \n", drawable_width, drawable_height);

    float scale_x = (float)drawable_width / SCREEN_WIDTH;
    float scale_y = (float)drawable_height / SCREEN_HEIGHT;

    printf(" defined screen size is (%d, %d) \n", SCREEN_WIDTH, SCREEN_HEIGHT);

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

    int grid_x = 0;
    int grid_y  = 0;
    grid_x  = ((drawable_width / 2) - ((GRID_DIM * scale_x) / 2)) ;
    grid_y = ((drawable_height / 2) - ((GRID_DIM * scale_x) / 2)) ;
    //int grid_dim_scaled = GRID_DIM * scale_x; keeping this if i might need it

    printf("scale_x, scale y -> (%f, %f) \n grid_x, grid_y -> (%d, %d)\n", scale_x, scale_y, grid_x, grid_y);

    SDL_Event event;
    int quit = 0;

    while(!quit){
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
        
        detect_crash();

        SDL_RenderClear(render);

        //render_grid(render, grid_x, grid_y, grid_dim_scaled);

        render_grid(render, grid_x, grid_y, scale_x);

        render_snake(render, grid_x, grid_y, scale_x);

        render_apple(render, grid_x, grid_x, scale_x); 

        SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);

        SDL_RenderPresent(render);

        SDL_Delay(95);

    }


    printf("shutting down...\n");
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// void render_grid(SDL_Renderer* renderer, int x, int y, int dim) {
//     SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//     for (int i = 0; i <= GRID_SIZE; i++) {
//         // Scaled lines
//         SDL_RenderDrawLine(renderer, x, y + i * (dim / GRID_SIZE), x + dim, y + i * (dim / GRID_SIZE));
//         SDL_RenderDrawLine(renderer, x + i * (dim / GRID_SIZE), y, x + i * (dim / GRID_SIZE), y + dim);
//     }
// }


void render_grid(SDL_Renderer* renderer, int x, int y, int scale_factor){
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF); //0x55, 0x55, 0x55, 0x55);
#if 0    
    SDL_Rect cell;
    cell.h = CELL_SIZE * scale_factor;
    cell.w = CELL_SIZE * scale_factor;

    for(int i = 0; i < GRID_SIZE ; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            cell.x = x + (i * CELL_SIZE * scale_factor);
            cell.y = y + (j * CELL_SIZE * scale_factor);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
#else
    SDL_Rect grid;
    grid.h = GRID_DIM * scale_factor;
    grid.w = GRID_DIM * scale_factor;
    grid.x = x;
    grid.y = y;
    SDL_RenderDrawRect(renderer, &grid);
#endif
}

void init_snake(){
    snakePtr new = malloc(sizeof(Snake));
    new->x = rand() % (GRID_SIZE/2) + (GRID_SIZE / 4 );
    new->y = 1;
    new->dir = rand() % SNAKE_LEFT;
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
    new->y = tail->y;
    //new->y = tail->y - 1;
    new->dir = tail->dir;
    new->next = NULL;
    tail->next = new;
    tail = new;
    
}

void render_snake(SDL_Renderer* renderer, int x, int y, int scale_factor){
    SDL_SetRenderDrawColor(renderer,0x00, 0xff, 0x00, 255 );

    int seg_size = (GRID_DIM / GRID_SIZE) * scale_factor  ;
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


void gen_apple( ){
    //we want to never generate the ball inside the snake
    bool in_loop = true;
    while(in_loop){
        newBall.x= rand() % GRID_SIZE ;
        newBall.y = rand() % GRID_SIZE ;

        snakePtr current = head;
        in_loop = false;

        while (current != NULL)
        {
            if(current->x == newBall.x && current->y == newBall.y){
                in_loop = true;
                break;
            }
            current = current->next;
        }
        
    }
    

}

void render_apple(SDL_Renderer* renderer, int x, int y, int scale_factor){
    //it will be nice to generate colors randomly 
    // Generate random RGB values (0-255)
    Uint8 r = 0xFF; 
    Uint8 g = 0x00; 
    Uint8 b = 0x00; 
    Uint8 a = 0xFF;        

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect ball;
    ball.x = x + (newBall.x * CELL_SIZE * scale_factor);
    ball.y = y + (newBall.y * CELL_SIZE * scale_factor);
    ball.h = CELL_SIZE * scale_factor;
    ball.w = CELL_SIZE * scale_factor;

    SDL_RenderFillRect(renderer, &ball);

}

void detect_apple(){

    if( (head->x == newBall.x ) &&(head->y == newBall.y)){ //asssignment ! comparison was the bug distorting the game we are now good
        gen_apple();
        increase_snake();
    }
}


void detect_crash(){

    if(head->x < 0 || head->x >= GRID_SIZE || head-> y < 0 || head->y >= GRID_SIZE){
        reset_snake();

    }
}

void reset_snake(){

    snakePtr track = head;
    snakePtr temp;

    while(track != NULL){
        temp = track->next;
        track = track->next;
        free(temp);
    }

    init_snake();
    increase_snake();
    increase_snake();
    gen_apple();
}
