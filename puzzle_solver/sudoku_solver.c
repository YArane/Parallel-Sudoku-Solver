#include <stdio.h>
#include <stdlib.h>
#include "../puzzle_structs/puzzle_generator.h"


/* MAIN METHOD */

int main(int argc, char **argv) {
  Puzzle *puzzle = init_puzzle(9);
  puzzle = create_puzzle(puzzle);
  print_puzzle(puzzle);

  int has_contradiction = puzzle_has_contradiction(5, 5, puzzle);

  printf("has contra %d\n", has_contradiction);
  return 0;
}
