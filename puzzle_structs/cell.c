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
  if(!(cell->possibility_list = malloc((n + 1) * sizeof(int)))){
    printf("error allocating possibility list.\n");
  }

  cell->value = value;
  return cell;
}


void fill_possibilities_with_zeros(Cell *cell){
    int i;
    int *list = cell->possibility_list;
    for(i=0;i<9;i++){
            list[i] = 0;
    }
}
