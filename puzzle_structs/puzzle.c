#include <stdio.h>
#include <stdlib.h>
#include "puzzle.h"


/* 
 * Function: init_puzzle
 * -------------------
 * initiates a puzzle
 *
 * n: size of the puzzle
 *
 * returns: the pointer to the puzzle
 *
 */

Puzzle *init_puzzle(int n){
    Puzzle *puzzle;

    // check malloc of struct
    if(!(puzzle = malloc(sizeof(Puzzle)))){
        printf("error allocating puzzle.\n");
        return NULL;
    }
    
    puzzle->size = n;


}
