#ifndef CELL_H
#define CELL_H
#define unknown -1


typedef struct Cell{
    int value;
    int *possibility_list;
    int row;
    int col;
} Cell;

Cell *init_cell(int value, int n);

void fill_possibilities_with_zeros(Cell *cell);

void *create_mpi_cell();

#endif
