#include "xezia.h"

void render_score(SDL_Renderer* renderer, int x, int y, int scale_factor){
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    SDL_Rect score_board;
    int score_size =  (CELL_SIZE * scale_factor)* 2;
    score_board.h = score_size;
    score_board.w = score_size;
    score_board.x = x - 50 ;
    score_board.y = y  - 200;

    //init_score();

    SDL_RenderFillRect(renderer, &score_board);

}


void renderGameName(SDL_Renderer* screen, int scoring_x, int scoring_y, int scale_factor,TTF_Font* font){
    SDL_Surface * name_surface;
    SDL_Color color = {255, 255, 200,255};
    game_name = "Xezia";

    name_surface =TTF_RenderText_Solid(font, game_name, color);
    printf("name is : %s \n", game_name);
    if(!name_surface){
        printf("Error creating the title, %s. \n", TTF_GetError());
    }
        
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(screen, name_surface);
    if (text_texture == NULL) {
        printf("Error creating texture from surface: %s\n", SDL_GetError());
    } 

    SDL_Rect dst_rect;
    scoring_x = (SCREEN_WIDTH * scale_factor) / 2 ;
    dst_rect.x =  scoring_x;
    dst_rect.y = scoring_y;
    dst_rect.w = (name_surface->w * 2 ) * scale_factor;
    dst_rect.h = (name_surface->h * 2 ) * scale_factor;

    SDL_RenderCopy(screen, text_texture, NULL, &dst_rect);

    SDL_FreeSurface(name_surface);
    SDL_DestroyTexture(text_texture);
}


void renderScore(SDL_Renderer* screen, int scoring_x, int scoring_y, int scale_factor, TTF_Font * font){
    //TTF_Font *font = TTF_OpenFont("/Users/imarao/Downloads/thoodle/Thoodle Regular.ttf", 20);
    
    SDL_Rect scoring_board;
    int scoring_size = (CELL_SIZE * scale_factor);
    scoring_board.x = scoring_x + 1120;
    scoring_board.y = scoring_y + 200;
    scoring_board.h = scoring_size * 3 * scale_factor;
    scoring_board.w = scoring_size * 4 * scale_factor;

    SDL_Color color={255,255,255, 255};
    SDL_Surface *text_surface;

    sprintf(score_update , "%d", scoring);
    printf("The current score is %d.\n", scoring);
    printf("the score update is %s.\n", score_update);
    printf("the font  is : %s .\n", TTF_FontFaceStyleName(font));
    //printf("the font loc is : %s .\n", &font);
    if(!(text_surface=TTF_RenderText_Solid(font,score_update,color))) {
        printf("Error creating text surface: %s\n", TTF_GetError());
    } else {
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(screen, text_surface);
        if (text_texture == NULL) {
            printf("Error creating texture from surface: %s\n", SDL_GetError());
        } else {
            // Render the text texture to the screen at the given position
            SDL_RenderCopy(screen, text_texture, NULL, &scoring_board);
            SDL_DestroyTexture(text_texture);
        }
        SDL_FreeSurface(text_surface);
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

void render_snake(SDL_Renderer* renderer, int x, int y, int scale_factor){

    int seg_size = (GRID_DIM / GRID_SIZE) * scale_factor  ;
    SDL_Rect seg;
    seg.w = seg_size - 4;
    seg.h = seg_size - 4 ; 

    int bright = 255;
    Uint8 dull = 0x00;
    int b_dir = 0;

    snakePtr track  = head;
    while(track != NULL){
        SDL_SetRenderDrawColor(renderer,0x00, bright, dull, 255 );
        seg.x = x + (track->x * seg_size + 2);
        seg.y = y + (track->y * seg_size + 2);
        SDL_RenderFillRect(renderer, &seg);
        track = track->next;

        if(b_dir == 0) {
            bright = bright - 15;
            if(bright < 50){
                b_dir = 1;
                bright = dull;
            } 
        }
        if(b_dir == 1){
            bright = bright + 50;
            if(bright > 250){
                b_dir = 0;
            }
        }
    }

}
