#include <stdio.h>
#include <stdlib.h>

typedef struct Cell{
  int value;
  int *possibility_list;
} Cell;

typedef struct Puzzle{
  int size;
  Cell *cell;
} Puzzle;

Cell *init_cell(int value, int n);
Puzzle *init_puzzle(int n);


int main(int argc, char **argv) {
  Puzzle *puzzle = init_puzzle(9);

  printf("puzzle created successfully\n");

  return 0;
}

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



