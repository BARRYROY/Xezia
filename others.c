/* The good thing is we have an AI running and playing snake */


/* The Ai is pretty confused debugging it and I can see the scaling isssue and the positions are mostly off*/

/* Learing more about HDPI  and to see how to resolve this on the Mac*/

/* Its pretty easy though */


/* looking foward to port this to windows and Linux */

#include "stdio.h"
#include <stdlib.h>

 typedef struct snake{
    int x, y ,dir;
    struct snake* next;
 }Snake;

 typedef Snake *snakePtr;

 snakePtr reverseList(snakePtr head);

int main(){

    snakePtr newSnake = malloc(sizeof(Snake));
    snakePtr newSnake1 = malloc(sizeof(Snake));
    snakePtr newSnake2 = malloc(sizeof(Snake));

    if (!newSnake || !newSnake1 || !newSnake2) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1; // Exit with error code
    }


    newSnake->x = 10;
    newSnake->y = 10;
    newSnake->dir = 2;
    newSnake->next = newSnake1;

    newSnake1->x = 10;
    newSnake1->y = 10;
    newSnake1->dir = 2;
    newSnake1->next = newSnake2;

    newSnake2->x = 10;
    newSnake2->y = 10;
    newSnake2->dir = 2;
    newSnake2->next = NULL;
    

    snakePtr ret = reverseList(newSnake);

    snakePtr temp = ret;
    while(temp != NULL){
        printf("snake at this point (%d, %d) and moving to direction %d \n ",  temp->x, temp-> y, temp->dir);
        temp = temp->next;
    }

    // Free allocated memory
    temp = ret;
    while (temp != NULL) {
        snakePtr next = temp->next;
        free(temp);
        temp = next;
    }

    return 0; // Exit successfully

}

snakePtr reverseList(snakePtr head)
{
    snakePtr current = head;
    snakePtr prev = head;
    snakePtr nextptr = NULL;

    while(current != NULL){
        nextptr = current->next;
        current->next = prev;
        prev = current;
        current = nextptr;
    }
    return prev;

}
