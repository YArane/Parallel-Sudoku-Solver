#include <stdio.h>
#include <stdlib.h>
#include "tree_node.h"


TreeNode *init_node(Cell *cell){
    TreeNode *node;

    if(!(node = malloc(sizeof(TreeNode)))){
        printf("error allocating node.\n");
    }

    node->cell = cell;
    node->children = NULL;

    return node;
}

int number_of_possibilities(Cell *cell){
    int *possibility_list = cell->possibility_list;
    
    int i, count=0;
    for(i=1;i<sizeof(possibility_list)/sizeof(int);i++){
        if(possibility_list[i])
                count++;
    }
    return count;
}

void populate_children(TreeNode *node){
    Cell *cell = node->cell;
    int possibilities = number_of_possibilities(cell);
    TreeNode *children;

    if(!(children = malloc(sizeof(TreeNode)*possibilities))){
        printf("error allocating children of tree node.\n");
    }

    int i, j;
    for(i=0, j=1;i<possibilities;i++, j++)
        if(cell->possibility_list[j])
            children[i] = *init_node(init_cell(cell->possibility_list[j], sizeof(cell->possibility_list)/sizeof(int)));

    node->children = children;    
}

/* Function: build_tree
 *
 * Recursively constructs a tree based on the possibility
 * list of the input node
 *
 * crt: input node
 *
 * returns: pointer to root of the tree
 */
TreeNode *build_tree(TreeNode *crt){
    populate_childrent(crt);
 
    int number_of_children = sizeof(crt->children)/sizeof(TreeNode);

    int i;
    for(i=0;i<number_of_children;i++){
        return build_tree(&(crt->children[i]));
    }
    return crt;
}

/*void print_tree(TreeNode *crt, char *acc){
    int number_of_children = sizeof(crt->children)/sizeof(TreeNode);

    int i;
    for(i=0;i<number_of_children;i++){
        printf("%s %d\n", crt->cell->value);
    }
}*/
