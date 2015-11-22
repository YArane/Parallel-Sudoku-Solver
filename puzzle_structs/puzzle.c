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
    printf("error allocating cells for puzzle.\n");
    return NULL;
  }  

  int i;
  for (i = 0; i < n * n; i++) {
    puzzle->cells[i] = *init_cell(-1, n);
  }

  printf("Finished initializing Puzzle.\n");
  return puzzle;
}

void *find_square_row_and_col(int row, int col, int *square_row, int *square_col, Puzzle *puzzle) {

  if (row < 3) {
    *square_row = 0;
    //top 3 blocks
  } else if (row >= 3 && row < 6) {
    *square_row = 3;
  } else {
    *square_row = 6;
  }

  if (col < 3) {
    *square_col = 0;
    //top 3 blocks
  } else if (col >= 3 && col < 6) {
    *square_col = 3;
  } else {
    *square_col = 6;
  }
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
  //printf("SETTING CELL [%d][%d] to %d\n", row, col, new_val);
  Cell *cell = get_cell(row, col, puzzle);
  cell->value = new_val;
  int i;
  for(i=0;i<=puzzle->size;i++){
        cell->possibility_list[i] = 0;
  }
  fill_possibility_lists(puzzle);
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

  // used to find the 3x3 square that will be checked for validity
  //TODO: probably not the best way to check square validity, but it works!
  int square_row, square_col;
  find_square_row_and_col(row, col, &square_row, &square_col, puzzle);

  int i, j, k, traverse_count, row_val, col_val, grid_val;
  j = square_row;
  k = square_col;
  traverse_count = 0;
  for (i = 0; i < size; i++) {
    // check all elements on row
    row_val = get_cell(row, i, puzzle)->value;
    if (row_val != -1 && row_val == newest_element_value && i != col) {
      //printf("--> found contradicion! cell [%d][%d]\n", row_val, i); 
      return 1;
    }

    //check all elements on col
    col_val = get_cell(i, col, puzzle)->value;
    if (col_val != -1 && !col_val && col_val == newest_element_value && i != row) {
      //printf("--> found contradicion! cell [%d][%d]\n", row_val, i); 
      return 1;
    }

    //check 3x3 square
    grid_val = get_cell(j, k, puzzle)->value;
    if (grid_val != -1 && !grid_val && grid_val == newest_element_value && j != row && k != col) {
      //printf("--> found contradicion! cell [%d][%d]\n", row_val, i); 
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

int is_obvious(Cell *cell){
    int i;
    int temp = 0;
    for(i=1;i<sizeof(cell->possibility_list);i++){
        if(cell->possibility_list[i] == 1){
            if(temp == 0){
                temp = i;
            }else{
                return -1;
            }
        }
    }
    return temp;
}

void fill_in_obvious_cells(Puzzle *puzzle){
   int i, j;
   for(i=0;i<puzzle->size;i++){
        for(j=0;j<puzzle->size;j++){
            if(get_cell_value(i, j, puzzle) != -1)
                    continue;
            int value = is_obvious(get_cell(i, j, puzzle));
            if(value > 0){
                    printf("Ovbious cell: [%d][%d], value: %d\n", i, j, value);
                    set_cell_value(i, j, puzzle, value);
                    fill_possibilities_with_zeros(get_cell(i, j, puzzle));
                    fill_in_obvious_cells(puzzle);
                    return;
            }
        }
   }
}

void fill_possibility_lists(Puzzle *puzzle) {
  int row, col, i, j, k, square_row, square_col, row_val, col_val, grid_val, traverse_count;
  int size = puzzle->size;
  Cell target_cell;

    int error = 0;

  int valid_possibilities[size + 1];

  for (row = 0; row < size; row++) {
    for (col = 0; col < size; col++) {

    if(row ==2 && col == 4) error = 1; else error = 0;
      for (i = 0; i < size + 1; i++) valid_possibilities[i] = 1;
      
      target_cell = puzzle->cells[size * row + col];
      if (target_cell.value != -1) {
        // Don't find possibility list for already filled in cells
        /*for(i=0;i<size;i++){
            valid_possibilities[i] = 0;
        }*/
        continue;
      }

      find_square_row_and_col(row, col, &square_row, &square_col, puzzle);

      j = square_row;
      k = square_col;
      traverse_count = 0;


      for (i = 0; i < size; i++) {
        // Not available possibilities are crossed out by filling their cell in
        // valid_possibilities[] with 0

        // check all elements on row
        row_val = get_cell(row, i, puzzle)->value;
        if (row_val != -1) {
          valid_possibilities[row_val] = 0;
        }

        //check all elements on col
        col_val = get_cell(i, col, puzzle)->value;
        if (col_val != -1) {
          valid_possibilities[col_val] = 0;
        }

        //check 3x3 square
        grid_val = get_cell(j, k, puzzle)->value;
        //if (row_val != -1) {
        if(grid_val != -1){
          valid_possibilities[grid_val] = 0;
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

      // Possibility list at 0 is meaningless, does not represent anything
      target_cell.possibility_list[0] = 0;
      for (i = 1; i < size + 1; i++) {
        target_cell.possibility_list[i] = valid_possibilities[i];
      }

    }
  }
}

void print_possibility_list(int row, int col, Puzzle *puzzle) {
  int i, size = puzzle->size;
  int possibility;
  printf("[%d][%d] value: %d  ", row, col, get_cell(row, col, puzzle)->value);
  for (i = 1; i < size + 1; i++) {
    printf("%d ", get_cell(row, col, puzzle)->possibility_list[i]);
  }
  printf("\n");
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

  char *space = " ";
  printf("About to print Puzzle.\n");

  int size = puzzle->size;

  int i, counter = 0;
  int row_counter = 0;
  printf("%*s", 4, space);
  for (i = 0; i < size * size; i++) {
    if(puzzle->cells[i].value == -1){
        printf("[] ");
    }else{
        printf(" %d ", puzzle->cells[i].value);
    }
    counter++;
    if(counter%3 == 0){
        printf("|");
    }
    if (counter == 9) {
      row_counter++;
      if(row_counter%3 == 0){
            printf("\n");
            printf("%*s", 4, space);
            printf("------------------------------");
      }
      printf("\n");
      printf("%*s", 4, space);
      counter = 0;
    }
  }
  printf("\n");
}

int get_cell_row(Puzzle *puzzle, Cell *cell){
    int i, j;
    for(i=0;i<puzzle->size;i++){
        for(j=0;j<puzzle->size;j++){
            if(get_cell(i, j, puzzle) == cell)
                    return i;
        }
    }
    return -1;
}

int get_cell_column(Puzzle *puzzle, Cell *cell){
    int i, j;
   for(i=0;i<puzzle->size;i++){
        for(j=0;j<puzzle->size;j++){
            if(get_cell(i, j, puzzle) == cell)
                    return j;
        }
    }
    return -1;
}
void print_puzzle_by_level(Puzzle *puzzle, int level) {
 
   char *space = " ";
   printf("About to print Puzzle.\n");
 
   int size = puzzle->size;
 
   int i, counter = 0;
   int row_counter = 0;
   printf("%*s", level*4, space);
   for (i = 0; i < size * size; i++) {
     if(puzzle->cells[i].value == -1){
         printf("[] ");
     }else{
         printf(" %d ", puzzle->cells[i].value);
     }
     counter++;
     if(counter%3 == 0){
         printf("|");
     }
     if (counter == 9) {
       row_counter++;
       if(row_counter%3 == 0){
             printf("\n");
             printf("%*s", level*4, space);
             printf("------------------------------");
       }
       printf("\n");
       printf("%*s", level*4, space);
       counter = 0;
     }
   }
   printf("\n");
 }
