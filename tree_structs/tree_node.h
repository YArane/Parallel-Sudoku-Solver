#ifndef TREE_NODE_H
#define TREE_NODE_H
#include "../puzzle_structs/cell.h"

typedef struct TreeNode{
    Cell *cell;
    struct TreeNode *children;
} TreeNode;

void build_tree(TreeNode *crt);

TreeNode *init_node(Cell *cell);

void print_tree(TreeNode *root);
#endif
