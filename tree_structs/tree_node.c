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

	printf("size of possibility_list: %lu\n", sizeof(possibility_list));

    printf("__> %d\n", (int)(sizeof(possibility_list)/(sizeof(int)*2)));
    int i, count=0;
    for(i=1;i<(int)sizeof(possibility_list);i++){
        if(possibility_list[i])
                count++;
    }
    return count;
}

void populate_children(TreeNode *node){
    Cell *cell = node->cell;
    int possibilities = number_of_possibilities(cell);
    printf("number of possibilities for cell %d is %d\n", cell->value, possibilities);
    TreeNode *children;

    if(!(children = malloc(sizeof(TreeNode)*possibilities))){
        printf("error allocating children of tree node.\n");
    }

    int i, j;
    for(i=0, j=1;i<possibilities;i++, j++)
        if(cell->possibility_list[j])
            children[i] = *init_node(init_cell(cell->possibility_list[j],\
					   	sizeof(cell->possibility_list)));

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
void build_tree(TreeNode *crt){
    populate_children(crt);
 
    int number_of_children = sizeof(crt->children)/sizeof(TreeNode);
	printf("recursed.\n");
    int i;

    for(i=0;i<number_of_children;i++){
        build_tree(&(crt->children[i]));
    }
}

void print_tree(TreeNode *crt){
	printf("---------------\n");
	printf("printing tree\n");				
    int number_of_children = sizeof(crt->children);
    int i;
    printf("# of children: %d\n", number_of_children);
    for(i=0;i<number_of_children;i++){
        printf("%d  ", crt->cell->value);
    }
}
