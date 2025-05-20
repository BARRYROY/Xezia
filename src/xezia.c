
#include "xezia.h"

#define RENDER_H

int delay = 70;
int scoring = 0;
char score_update[32];
TTF_Font *font ;
char * game_name;
int mode = 1;

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

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
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

    SDL_SetWindowSize(window, 1000, 1000);

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

    loadFont(); //is this the place

    printf("Game is running ...\n");

    Uint8 alpha = (Uint8)(0.0667 * 255);

    SDL_SetRenderDrawColor(render,0x11, 0x11,0x11, alpha);

    SDL_RenderClear(render);

    SDL_RenderPresent(render);

    SDL_Delay(DELAY_TIME);

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

        

        //render_score(render, ((GRID_DIM * scale_x) / 6) , ((GRID_DIM * scale_x) / 2), scale_x);
       // if(mode == PLAY){
            //snake_ai();
        // }
        // else
        // { 
        //     delay = 50;     
        // }

        detect_apple();

        snake_ai();

        move_snake();
        
        detect_crash();

        SDL_RenderClear(render);

        //render_grid(render, grid_x, grid_y, grid_dim_scaled);

        render_grid(render, grid_x, grid_y, scale_x);

        render_snake(render, grid_x, grid_y, scale_x);

        render_apple(render, grid_x, grid_y, scale_x); 

        renderGameName(render, 300, 300, scale_x,font);

        //render_apple(render, grid_x, grid_x, scale_x); 

        //render_score(render, grid_x , grid_x, scale_x);

        renderScore(render, grid_x, grid_y, scale_x, font);

        SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0x00);

        SDL_RenderPresent(render);

        SDL_Delay(DELAY);

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
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 255, 255, 255); //0x55, 0x55, 0x55, 0x55);
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

void init_score(){
    scoring = 0;
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

void free_snake(){
    snakePtr current = head;
    while(current){
        snakePtr temp = current;
        current = current->next;
        free(temp);
    }
}

// void render_snake(SDL_Renderer* renderer, int x, int y, int scale_factor){

//     int seg_size = (GRID_DIM / GRID_SIZE) * scale_factor  ;
//     SDL_Rect seg;
//     seg.w = seg_size - 4;
//     seg.h = seg_size - 4 ; 

//     int bright = 255;
//     Uint8 dull = 0x00;
//     int b_dir = 0;

//     snakePtr track  = head;
//     while(track != NULL){
//         SDL_SetRenderDrawColor(renderer,0x00, bright, dull, 255 );
//         seg.x = x + (track->x * seg_size + 2);
//         seg.y = y + (track->y * seg_size + 2);
//         SDL_RenderFillRect(renderer, &seg);
//         track = track->next;

//         if(b_dir == 0) {
//             bright = bright - 15;
//             if(bright < 50){
//                 b_dir = 1;
//                 bright = dull;
//             } 
//         }
//         if(b_dir == 1){
//             bright = bright + 50;
//             if(bright > 250){
//                 b_dir = 0;
//             }
//         }
//     }

// }

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

// void render_apple(SDL_Renderer* renderer, int x, int y, int scale_factor){
//     //it will be nice to generate colors randomly 
//     // Generate random RGB values (0-255)
//     Uint8 r = 0xFF; 
//     Uint8 g = 0x00; 
//     Uint8 b = 0x00; 
//     Uint8 a = 0xFF;        

    
//     SDL_SetRenderDrawColor(renderer, r, g, b, a);
//     SDL_Rect ball;
//     ball.x = x + (newBall.x * CELL_SIZE * scale_factor);
//     ball.y = y + (newBall.y * CELL_SIZE * scale_factor);
//     ball.h = CELL_SIZE * scale_factor;
//     ball.w = CELL_SIZE * scale_factor;

//     SDL_RenderFillRect(renderer, &ball);

// }

void detect_apple(){

    if( (head->x == newBall.x ) &&(head->y == newBall.y)){ //asssignment ! comparison was the bug distorting the game we are now good
        gen_apple();
        increase_snake();
        update_score();
    }
}


void detect_crash(){

    if(head->x < 0 || head->x >= GRID_SIZE || head-> y < 0 || head->y >= GRID_SIZE){
        reset_snake();

    }
    /* snake crashing into the tail */
    if(head->x == tail->x && head->y == tail->y){
        /* todo show menu , quit */
        reset_snake();
    }
    snakePtr track = head->next;
    while(track!=NULL){
        if(head->x == track->x && head->y == track->y) {
            reset_snake();
            break;
        }
        track = track->next;
    }
}

/* todo change escape to pause game instead of quiting */

void reset_snake(){

    snakePtr track = head;

    while(track != NULL){
        track = track->next;
    }

    init_snake();
    increase_snake();
    increase_snake();
    gen_apple();
    init_score();
}

// void snake_ai(){
//     int try_l = game_status(TRY_LEFT);
//     int try_r = game_status(TRY_RIGHT);
//     int try_f = game_status(TRY_FOWARD);

//     if(try_f >= try_l && try_f >= try_r){
//         //forward
//         //move_snake();
//     }else {
//         if(try_l > try_r) {
//             turn_left();
//         }else{
//             turn_right();
//         }
//     }
// }

void turn_left(){

    switch(head->dir){
        case SNAKE_DOWN:
            head->dir = SNAKE_RIGHT;
            break;
        case SNAKE_UP:
            head->dir = SNAKE_LEFT;
            break;
        case SNAKE_LEFT:
            head->dir = SNAKE_DOWN;
            break;
        case SNAKE_RIGHT:
            head->dir = SNAKE_UP;
            break;
    }
}

void turn_right(){

    switch(head->dir){
        case SNAKE_DOWN:
            head->dir = SNAKE_LEFT;
            break;
        case SNAKE_UP:
            head->dir = SNAKE_RIGHT;
            break;
        case SNAKE_LEFT:
            head->dir = SNAKE_UP;
            break;
        case SNAKE_RIGHT:
            head->dir = SNAKE_DOWN;
            break;
    }
}

// int  game_status(int try){

//     int ret = 0;
//     int try_x = head->x;
//     int try_y = head->y;

//     switch(head->dir){
//         case SNAKE_DOWN:
//             switch(try){
//                 case TRY_FOWARD:
//                     try_y ++;
//                     break;
//                 case TRY_LEFT:
//                     try_x++;
//                     // ret = -100;
//                     break;
//                 case TRY_RIGHT:
//                     try_x--;
//                     // ret = 100;
//                     break;
//             }
//             break;
//         case SNAKE_UP:
//             switch(try){
//                 case TRY_FOWARD:
//                     try_y--;
//                     // ret = 1;
//                     break;
//                 case TRY_LEFT:
//                     try_x--;
//                     // ret = 1;
//                     break;
//                 case TRY_RIGHT:
//                     try_x++;
//                     // ret = 1;
//                     break;
//             }
//             break;
//         case SNAKE_LEFT:
//             switch(try){
//                 case TRY_FOWARD:
//                     try_x--;
//                     // ret = 1;
//                     break;
//                 case TRY_LEFT:
//                     try_y++;
//                     // ret = 1;
//                     break;
//                 case TRY_RIGHT:
//                     try_y--;
//                     // ret = 1;
//                     break;
//             }
//             break;
//         case SNAKE_RIGHT:
//             switch(try){
//                 case TRY_FOWARD:
//                     try_x++;
//                     // ret = 1;
//                     break;
//                 case TRY_LEFT:
//                     try_y--;
//                     // ret = 1;
//                     break;
//                 case TRY_RIGHT:
//                     try_y++;
//                     // ret = 1;
//                     break;
//             }
//             break;
//     }
    
//     /* detect crah to the wall */
//     if((try_x < 0 || try_x > GRID_SIZE - 1 ) || 
//        (try_y < 0 || try_y > GRID_SIZE - 1 )){
//         ret += -100;
//     }

//     /* detect newBall */
//     if(try_x == newBall.x && try_y == newBall.y){
//         ret += 100;
//     }

//     /* get closer to the ball */
//     int diff_x = abs(head->x - newBall.x);
//     int diff_y = abs(head->y - newBall.y) ;
//     int diff_try_x = abs(head->x - try_x) ;
//     int diff_try_y = abs(head->x - try_y) ;

//     if(diff_try_x < diff_x || diff_try_y < diff_y){
//         ret += 5;
//     } 

//     /* detect tail */
//       snakePtr track = head->next;

//     while(track != NULL){
//         if((try_x == track->x) && (try_y == track->y)){
//             ret += -50;
//             break;
//         }
//         track = track->next;
//     }

//     return ret;

// }

void update_score(){
    scoring = scoring + 10;
    printf("update_score: the score is updated by 10.\n");

}

// void render_score(SDL_Renderer* renderer, int x, int y, int scale_factor){
    
//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
//     SDL_Rect score_board;
//     int score_size =  (CELL_SIZE * scale_factor)* 2;
//     score_board.h = score_size;
//     score_board.w = score_size;
//     score_board.x = x - 50 ;
//     score_board.y = y  - 200;

//     //init_score();

//     SDL_RenderFillRect(renderer, &score_board);

// }

void loadFont(){

    font = TTF_OpenFont("/Users/imarao/Downloads/arial/ARIAL.ttf", 20);
    if(!font){
        printf("failed to load font %s .\n", SDL_GetError());
    }
    printf("The font selected is : %s .\n", TTF_FontFaceStyleName(font));
}

// void renderGameName(SDL_Renderer* screen, int scoring_x, int scoring_y, int scale_factor,TTF_Font* font){
//     SDL_Surface * name_surface;
//     SDL_Color color = {255, 255, 200,255};
//     game_name = "Xezia";

//     name_surface =TTF_RenderText_Solid(font, game_name, color);
//     printf("name is : %s \n", game_name);
//     if(!name_surface){
//         printf("Error creating the title, %s. \n", TTF_GetError());
//     }
        
//     SDL_Texture *text_texture = SDL_CreateTextureFromSurface(screen, name_surface);
//     if (text_texture == NULL) {
//         printf("Error creating texture from surface: %s\n", SDL_GetError());
//     } 

//     SDL_Rect dst_rect;
//     dst_rect.x =  (SCREEN_WIDTH * scale_factor) / 2 ;
//     dst_rect.y = scoring_y;
//     dst_rect.w = (name_surface->w * 2 ) * scale_factor;
//     dst_rect.h = (name_surface->h * 2 ) * scale_factor;

//     SDL_RenderCopy(screen, text_texture, NULL, &dst_rect);

//     SDL_FreeSurface(name_surface);
//     SDL_DestroyTexture(text_texture);
// }


// void renderScore(SDL_Renderer* screen, int scoring_x, int scoring_y, int scale_factor, TTF_Font * font){
//     //TTF_Font *font = TTF_OpenFont("/Users/imarao/Downloads/thoodle/Thoodle Regular.ttf", 20);
    
//     SDL_Rect scoring_board;
//     int scoring_size = (CELL_SIZE * scale_factor);
//     scoring_board.x = scoring_x + 1120;
//     scoring_board.y = scoring_y + 200;
//     scoring_board.h = scoring_size * 3 * scale_factor;
//     scoring_board.w = scoring_size * 4 * scale_factor;

//     SDL_Color color={255,255,255, 255};
//     SDL_Surface *text_surface;

//     sprintf(score_update , "%d", scoring);
//     printf("The current score is %d.\n", scoring);
//     printf("the score update is %s.\n", score_update);
//     printf("the font  is : %s .\n", font);
//     printf("the font loc is : %s .\n", &font);
//     if(!(text_surface=TTF_RenderText_Solid(font,score_update,color))) {
//         printf("Error creating text surface: %s\n", TTF_GetError());
//     } else {
//         SDL_Texture *text_texture = SDL_CreateTextureFromSurface(screen, text_surface);
//         if (text_texture == NULL) {
//             printf("Error creating texture from surface: %s\n", SDL_GetError());
//         } else {
//             // Render the text texture to the screen at the given position
//             SDL_RenderCopy(screen, text_texture, NULL, &scoring_board);
//             SDL_DestroyTexture(text_texture);
//         }
//         SDL_FreeSurface(text_surface);
//     }
// }
