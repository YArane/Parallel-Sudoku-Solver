#ifndef PUZZLE_GENERATOR_H
#define PUZZLE_GENERATOR_H

#include "puzzle.h"

#define SHUFF 10 /* Number of times to shuffle before printing */
#define HINTS 20 /* Approximate percentage of cells with initial hints */


void swap_row(int from, int to);

void swap_col(int from, int to);

Puzzle *create_puzzle(Puzzle *puzzle);
#endif
