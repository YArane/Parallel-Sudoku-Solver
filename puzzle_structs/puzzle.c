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

  if (!(puzzle->cells = malloc(n * n * sizeof(Cell)))) {
    printf("erorr allocating cells for puzzle.\n");
    return NULL;
  }

  printf("Finished initializing Puzzle.\n");
  return puzzle;
}

/* 
 * Function: get_cell
 * -------------------
 * Returns the Cell at the specified row and col in the puzzle object.
 *
 * row: the row that contains the cell to be returned
 * col: the col that contains the cell to be returned
 * puzzle: the puzzle to extract the cell from
 *
 * returns: a Cell pointer
 *
 * NOTE: THE PUZZLE IS INDEXED FROM 0. Passing (0,0) returns the top-left corner.
 *
 */
Cell *get_cell(int row, int col, Puzzle *puzzle) {
  int size = puzzle->size;
  Cell *cell = &puzzle->cells[size * row + col];
  return cell;
}

/* 
 * Function: get_value
 * -------------------
 * Returns the Cell's value at the specified row and col in the puzzle object.
 *
 * row: the row that contains the cell to be returned
 * col: the col that contains the cell to be returned
 * puzzle: the puzzle to extract the cell from
 *
 * returns: the integer corresponding to the cell's value
 *
 * NOTE: THE PUZZLE IS INDEXED FROM 0. Passing (0,0) returns the top-left corner.
 *
 */
int get_cell_value(int row, int col, Puzzle *puzzle) {
  return get_cell(row, col, puzzle)->value;
  /*int size = puzzle->size;
  int val = puzzle->cells[size * row + col].value;
  return val; */
}

/* 
 * Function: set_cell_value
 * -------------------
 * Returns assigns the specified Cell's value at the specified row and col in the puzzle object to the new_val.
 *
 * row: the row that contains the cell to be returned
 * col: the col that contains the cell to be returned
 * puzzle: the puzzle to extract the cell from
 * new_val: the new_val to assign
 *
 * NOTE: THE PUZZLE IS INDEXED FROM 0. Passing (0,0) returns the top-left corner.
 *
 */
void set_cell_value(int row, int col, Puzzle *puzzle, int new_val) {
  get_cell(row, col, puzzle)->value = new_val;
/*  int size = puzzle->size;
  puzzle->cells[size * row + col].value = new_val;*/
}

/* 
 * Function: puzzle_has_contradiction
 * -------------------
 * Determines whether a contradiction exists in the given puzzle
 * Sweeps the row column, and square that contains the cell
 *
 * row: the row that contains the cell to be checked for contradition
 * col: the col that contains the cell to be checked for contradition
 * size: number of elements in a row/col of the puzzle
 *
 * returns: 1 if there is a contradiction, 0 otherwise
 *
 */
int puzzle_has_contradiction(int row, int col, Puzzle *puzzle) {

  Cell *newest_element = get_cell(row, col, puzzle);

  int newest_element_value = newest_element->value;
  int size = puzzle->size;
  printf("Cell Value %d\n", newest_element_value);

  // used to find the 3x3 square that will be checked for validity
  //TODO: probably not the best way to check square validity, but it works!
  int square_row, square_col;
  if (row < 3) {
    square_row = 0;
    //top 3 blocks
  } else if (row >= 3 && row < 6) {
    square_row = 3;
  } else {
    square_row = 6;
  }

  if (col < 3) {
    square_col = 0;
    //top 3 blocks
  } else if (row >= 3 && row < 6) {
    square_col = 3;
  } else {
    square_col = 6;
  }

  int i, j, k, traverse_count, row_val, col_val, grid_val;
  j = square_row;
  k = square_col;
  traverse_count = 0;
  for (i = 0; i < size; i++) {
    // check all elements on row
    row_val = get_cell(row, i, puzzle)->value;
    if (row_val != -1 && row_val == newest_element_value && i != col) {
      return 1;
    }

    //check all elements on col
    col_val = get_cell(i, col, puzzle)->value;
    if (col_val != -1 && !col_val && col_val == newest_element_value && i != row) {
      return 1;
    }

    //check 3x3 square
    grid_val = get_cell(j, k, puzzle)->value;
    if (row_val != -1 && !grid_val && grid_val == newest_element_value && j != row && k != col) {
      return 1;
    }

    traverse_count++;
    if (traverse_count > 2) {
      j = j + 1;
      k = square_col;
      traverse_count = 0;
    } else {
      k++;
    }
  }

  return 0;

}


/* 
 * Function: print_puzzle 
 * -------------------
 * prints the puzzle in a user-friendly fasion
 * 
 * puzzle: pointer to the puzzle
 *
 */
void print_puzzle(Puzzle *puzzle) {

  printf("About to print Puzzle.\n");

  int size = puzzle->size;

  int i, counter = 0;
  for (i = 0; i < size * size; i++) {
    printf("%d ", puzzle->cells[i].value);
    counter++;
    if (counter == 9) {
      printf("\n");
      counter = 0;
    }
  }
}
