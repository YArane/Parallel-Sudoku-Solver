#include <stdio.h>
#include <stdlib.h>
#include "../puzzle_structs/puzzle_generator.h"
//#include "../tree_structs/tree_node.h"
#include "../decision_tree/decision_tree.h"
#include <mpi.h>

/*
==============================================
===== SEE THE README FOR IMPORTANT INFO ======
==============================================
*/

void insert_some_empty_cells(Puzzle *puzzle);
void *read_matrix_data(char *filename, Puzzle *puzzle);

/* MAIN METHOD */

int main(int argc, char **argv) {

  Puzzle *puzzle = init_puzzle(9);
  /*puzzle = */create_puzzle(puzzle);

  print_puzzle(puzzle);

  if(argc == 1){
      insert_some_empty_cells(puzzle);
  }

  char *filename;
  if (argc > 1){
       filename = argv[1];   
       read_matrix_data(filename, puzzle);
  }  

  fill_possibility_lists(puzzle);
  //fill_in_obvious_cells(puzzle);
/*  print_puzzle(puzzle);

  printf("ORIGINAL LIST:\n");

  int i, j;
  for(i=0;i<9;i++){
          for(j=0;j<9;j++){
                  print_possibility_list(i, j, puzzle);
          }
  }*/


  Node *node = build_tree(puzzle); 

  return 0;
}

// For testing
void insert_some_empty_cells(Puzzle *puzzle) {
  printf("UH-OH\n");
  set_cell_value(0,0,puzzle,-1);
  set_cell_value(0,1,puzzle,-1);
  set_cell_value(1,0,puzzle,-1);
  set_cell_value(7,1,puzzle,-1);
  set_cell_value(4,2,puzzle,-1);
  set_cell_value(0,8,puzzle,-1);
  set_cell_value(3,4,puzzle,-1);
  set_cell_value(1,1,puzzle,-1);
  set_cell_value(2,3,puzzle,-1);
  // set_cell_value(4,4,puzzle,-1);
  // set_cell_value(1,3,puzzle,-1);
}

// User Input for matrix
void *read_matrix_data(char *filename, Puzzle *puzzle){
    FILE *file;

    printf("attempting to read file..\n");

    if(!(file=fopen(filename, "r"))){
        printf("error reading file.\n");
    }

    int count = 0;
    char c;
    int i = 0;
    int j = 0;
    while((c=fgetc(file)) != EOF){
        if(c == '\n') continue;
        j = count%9;
        i = count/9;
        if(c == '0'){
                set_cell_value(i, j, puzzle, -1); 
        }
        count++;
    }

}

