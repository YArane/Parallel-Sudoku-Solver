#include <stdio.h>
#include <stdlib.h>
#include "../puzzle_structs/puzzle_generator.h"

/*
==============================================
===== SEE THE README FOR IMPORTANT INFO ======
==============================================
*/

void insert_some_empty_cells(Puzzle *puzzle);

/* MAIN METHOD */

int main(int argc, char **argv) {
  Puzzle *puzzle = init_puzzle(9);
  puzzle = create_puzzle(puzzle);
  insert_some_empty_cells(puzzle);
  fill_possibility_lists(puzzle);
  print_puzzle(puzzle);

  int has_contradiction = puzzle_has_contradiction(5, 5, puzzle);

  print_possibility_list(0, 0, puzzle);

  printf("has contra %d\n", has_contradiction);
  return 0;
}

// For testing
void insert_some_empty_cells(Puzzle *puzzle) {
  set_cell_value(0,0,puzzle,-1);
  set_cell_value(0,1,puzzle,-1);
  set_cell_value(1,0,puzzle,-1);
  set_cell_value(7,1,puzzle,-1);
  set_cell_value(4,2,puzzle,-1);
  set_cell_value(0,8,puzzle,-1);
  set_cell_value(3,4,puzzle,-1);
  set_cell_value(1,1,puzzle,-1);
  set_cell_value(2,3,puzzle,-1);
  // set_cell_value(4,4,puzzle,-1);
  // set_cell_value(1,3,puzzle,-1);
}