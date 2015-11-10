#include <stdio.h>
#include <stdlib.h>

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
int *puzzle_has_contradiction(int row, int col, Puzzle *puzzle);

/* CELL METHODS */

Cell *init_cell(int value, int n);

int main(int argc, char **argv) {
  Puzzle *puzzle = init_puzzle(9);
  print_puzzle(puzzle);

  puzzle_has_contradiction(5, 5, puzzle);

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

  int i;
  for (i = 0; i < n*n; i++) {
    (puzzle->cells[i]).value = i;
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
 * Function: puzzle_has_contradiction
 * -------------------
 * Determines whether a contradiction exists in the given puzzle
 * Sweeps the row column, and square that contains the cell
 *
 * row: the row that contains the cell to be checked for contradition
 * col: the col that contains the cell to be checked for contradition
 * size: number of elements in a row/col of the puzzle
 *
 * returns: the pointer to the puzzle
 *
 */
int *puzzle_has_contradiction(int row, int col, Puzzle *puzzle) {

  Cell *newest_element = get_cell(row, col, puzzle);

  printf("Cell Value %d\n", newest_element->value);

  // int i;
  // for (i = 0; i < size; i++) {

  // }

}

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

