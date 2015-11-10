#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 1  /* Number of sudokus to generate */
#define SHUFF 10     /* Number of times to shuffle before printing */
#define HINTS 20     /* Approximate percentage of cells with initial hints */

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
void set_cell_value(int row, int col, Puzzle *puzzle, int new_val);
int get_cell_value(int row, int col, Puzzle *puzzle);

/* CELL METHODS */

Cell *init_cell(int value, int n);

/* PUZZLE GENERATOR METHODS */

Puzzle *create_puzzle(Puzzle *puzzle);
void swap_row(int from, int to);
void swap_col(int from, int to);

/* MAIN METHOD */

int main(int argc, char **argv) {
  Puzzle *puzzle = init_puzzle(9);
  puzzle = create_puzzle(puzzle);
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

int get_cell_value(int row, int col, Puzzle *puzzle) {
  int size = puzzle->size;
  int val = puzzle->cells[size * row + col].value;
  return val; 
}

void set_cell_value(int row, int col, Puzzle *puzzle, int new_val) {
  int size = puzzle->size;
  puzzle->cells[size * row + col].value = new_val;
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

/*
==============================
=== PUZZLE GENERATOR CODE ====
==============================
*/

/*
You shouldn't worry about what's going on here. 
The only method that matters is create_puzzle, which
populates the Puzzle object with a valid solved puzzle.
*/

void swap_row(int from, int to);
void swap_col(int from, int to);
void print_mx();

static int source[81] ={
  1,3,4,5,2,8,6,9,7,
  2,5,6,7,9,1,3,4,8,
  7,8,9,3,4,6,1,2,5,
  3,1,2,4,6,7,5,8,9,
  5,4,7,9,8,3,2,1,6,
  9,6,8,1,5,2,7,3,4,
  4,7,5,2,1,9,8,6,3,
  8,9,1,6,3,5,4,7,2,
  6,2,3,8,7,4,9,5,1
};

Puzzle *create_puzzle(Puzzle *puzzle) {
  srand(time(NULL));

  int i, j, swap, trio;

  for(i=0; i<COUNT; i++){
    for(j=0; j<SHUFF; j++){

      trio = (rand() % 3)*3;
      swap = rand() & 1;

      switch(rand() % 6){
        /* swap rows */
        case 0:
          swap_row(trio+0, swap ? trio+1 : trio+2);
          break;
        case 1:
          swap_row(trio+1, swap ? trio+0 : trio+2);
          break;
        case 2:
          swap_row(trio+2, swap ? trio+0 : trio+1);
          break;

        /* swap cols */
        case 3:
          swap_col(trio+0, swap ? trio+1 : trio+2);
          break;
        case 4:
          swap_col(trio+1, swap ? trio+0 : trio+2);
          break;
        case 5:
          swap_col(trio+2, swap ? trio+0 : trio+1);
          break;
      }
    }
  }

  int size = puzzle->size;
  for (i = 0; i < size * size; i++) {
    puzzle->cells[i].value = source[i];
  }

  return puzzle;

}

void swap_row(int from, int to)
{
  int *ptr1, *ptr2, i, temp;
  ptr1 = source+(9*from);
  ptr2 = source+(9*to);
  for(i=0; i<9; i++){
    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
    ptr1++; 
    ptr2++;
  }
}

void swap_col(int from, int to)
{
  int *ptr1, *ptr2, i, temp;
  ptr1 = source+from;
  ptr2 = source+to;
  for(i=0; i<9; i++){
    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
    ptr1+=9;
    ptr2+=9;
  }
}

