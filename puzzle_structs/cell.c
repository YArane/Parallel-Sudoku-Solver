#include <stdio.h>
#include <stdlib.h>
#include "cell.h"


/* 
 * Function: init_cell
 * -------------------
 * initiates a cell
 *
 * value: the value of the cell
 * n: size of the puzzle
 *
 * returns: the pointer to the cell
 *
 */

Cell *init_cell(int value, int n){
        Cell *cell;
        // check malloc of struct
        if(!(cell = malloc(sizeof(Cell)))){
            printf("error allocating cell.\n");
            return NULL;
        }
        // check malloc of possibility pointer
        int *list;
        if(!(list = malloc(sizeof(int)*n))){
            printf("error allocating possibility list.\n");
        }
        cell->value = value;
        cell->possibility_list = list;
        return cell;
}


