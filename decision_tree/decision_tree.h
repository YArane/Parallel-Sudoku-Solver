#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "../puzzle_structs/puzzle.h"

typedef struct Node{
        Puzzle *instance;
        int number_of_children;
        struct Node *children;
}Node;


Node *build_tree(Puzzle *instance);

int get_number_of_possibilities(Cell *cell);

#endif
