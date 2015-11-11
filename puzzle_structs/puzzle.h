#include "cell.h"

typedef struct Puzzle{
    int size;
    Cell *cells;
}Puzzle;

Puzzle *init_puzzle(int size);

void print_puzzle(Puzzle *puzzle);

int puzzle_has_contradiction(int row, int col, Puzzle *puzzle);

Cell *get_cell(int row, int col, Puzzle *puzzle);

void set_cell_value(int row, int col, Puzzle *puzzle, int new_val);

int get_cell_value(int row, int col, Puzzle *puzzle);

void *find_square_row_and_col(int row, int col, int *square_row, int *square_col, Puzzle *puzzle);

void print_possibility_list(int row, int col, Puzzle *puzzle);

void fill_possibility_lists(Puzzle *puzzle);