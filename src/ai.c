#include "xezia.h"

int  game_status(int try){

    int ret = 0;
    int try_x = head->x;
    int try_y = head->y;

    switch(head->dir){
        case SNAKE_DOWN:
            switch(try){
                case TRY_FOWARD:
                    try_y ++;
                    break;
                case TRY_LEFT:
                    try_x++;
                    // ret = -100;
                    break;
                case TRY_RIGHT:
                    try_x--;
                    // ret = 100;
                    break;
            }
            break;
        case SNAKE_UP:
            switch(try){
                case TRY_FOWARD:
                    try_y--;
                    // ret = 1;
                    break;
                case TRY_LEFT:
                    try_x--;
                    // ret = 1;
                    break;
                case TRY_RIGHT:
                    try_x++;
                    // ret = 1;
                    break;
            }
            break;
        case SNAKE_LEFT:
            switch(try){
                case TRY_FOWARD:
                    try_x--;
                    // ret = 1;
                    break;
                case TRY_LEFT:
                    try_y++;
                    // ret = 1;
                    break;
                case TRY_RIGHT:
                    try_y--;
                    // ret = 1;
                    break;
            }
            break;
        case SNAKE_RIGHT:
            switch(try){
                case TRY_FOWARD:
                    try_x++;
                    // ret = 1;
                    break;
                case TRY_LEFT:
                    try_y--;
                    // ret = 1;
                    break;
                case TRY_RIGHT:
                    try_y++;
                    // ret = 1;
                    break;
            }
            break;
    }
    
    /* detect crah to the wall */
    if((try_x < 0 || try_x > GRID_SIZE - 1 ) || 
       (try_y < 0 || try_y > GRID_SIZE - 1 )){
        ret += -100;
    }

    /* detect newBall */
    if(try_x == newBall.x && try_y == newBall.y){
        ret += 100;
    }

    /* get closer to the ball */
    int diff_x = abs(head->x - newBall.x);
    int diff_y = abs(head->y - newBall.y) ;
    int diff_try_x = abs(head->x - try_x) ;
    int diff_try_y = abs(head->x - try_y) ;

    if(diff_try_x < diff_x || diff_try_y < diff_y){
        ret += 5;
    } 

    /* detect tail */
    snakePtr track = head->next;

    while(track != NULL){
        if((try_x == track->x) && (try_y == track->y)){
            ret += -50;
            break;
        }
        track = track->next;
    }

    return ret;

}


void snake_ai(){
    int try_l = game_status(TRY_LEFT);
    int try_r = game_status(TRY_RIGHT);
    int try_f = game_status(TRY_FOWARD);

    if(try_f >= try_l && try_f >= try_r){
        //forward
        //move_snake();
    }else {
        if(try_l > try_r) {
            turn_left();
        }else{
            turn_right();
        }
    }
}
