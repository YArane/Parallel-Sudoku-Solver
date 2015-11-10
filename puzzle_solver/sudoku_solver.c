#include <stdio.h>
#include <stdlib.h>

/*
==============================================
===== SEE THE README FOR IMPORTANT INFO ======
==============================================
*/

typedef struct Cell{
  int value;
  int *possibility_list;
} Cell;

typedef struct Puzzle{
  int size;
  Cell *cells;
} Puzzle;

/* PUZZLE METHODS */

Puzzle *init_puzzle(int n);
void print_puzzle(Puzzle *puzzle);
int puzzle_has_contradiction(int row, int col, Puzzle *puzzle);
Cell *get_cell(int row, int col, Puzzle *puzzle);
Puzzle *fill_puzzle_with_values(Puzzle *puzzle);

/* CELL METHODS */

Cell *init_cell(int value, int n);

int main(int argc, char **argv) {
  Puzzle *puzzle = init_puzzle(9);
  puzzle = fill_puzzle_with_values(puzzle);
  print_puzzle(puzzle);

  int has_contradiction = puzzle_has_contradiction(5, 5, puzzle);

  printf("has contra %d\n", has_contradiction);
  return 0;
}

/*
==============================
======= PUZZLE METHODS =======
==============================
*/

/*
NOTE: ASSUMING ROW REPRESENTATION OF PUZZLE.

- Example: puzzle is an array composed of 9x9 = 81 Cells.
- The first 9 Cells correspond to the top row of the puzzle
- The next 9 Cells correspond to the second row of the puzzle
*/

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

// Only for testing! Doesn't build a real sudoku puzzle
Puzzle *fill_puzzle_with_values(Puzzle *puzzle) {
  int i, size = puzzle->size;
  for (i = 0; i < size*size; i++) {
    if (i % 3 == 0) {
      (puzzle->cells[i]).value = -1;
    } else {
      (puzzle->cells[i]).value = i;
    }
  } 
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
 * Prints the passed puzzle
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

/*
==============================
======= CELL METHODS =========
==============================
*/

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

