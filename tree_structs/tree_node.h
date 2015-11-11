#include "../puzzle_structs/cell.h"

typedef struct TreeNode{
    Cell *cell;
    struct TreeNode *children;
} TreeNode;

TreeNode *build_tree(TreeNode *crt);

void print_tree(TreeNode *root);
