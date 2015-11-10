#define unknown -1

typedef struct Cell{
    int value;
    int *possibility_list;
} Cell;

Cell *init_cell(int value, int n);

