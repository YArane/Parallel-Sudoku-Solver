#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "puzzle_generator.h"

static int source[81] ={
  1,3,4,5,2,8,6,9,7,
  2,5,6,7,9,1,3,4,8,
  7,8,9,3,4,6,1,2,5,
  3,1,2,4,6,7,5,8,9,
  5,4,7,9,8,3,2,1,6,
  9,6,8,1,5,2,7,3,4,
  4,7,5,2,1,9,8,6,3,
  8,9,1,6,3,5,4,7,2,
  6,2,3,8,7,4,9,5,1
};

void make_puzzle();

int main(int argc, char **argv){

        make_puzzle();           

                int i;

            for(i=1;i<=81;i++){
                    printf("%d", source[i-1]);
                    if(i%9 == 0 && i != 0){
                        printf("\n");
                    }
            }
            printf("\n");
            return 1;

      
        /*     ~~ write puzzle to file ~~    */
        FILE *file;
        if(!(file=fopen(argv[1], "w"))){
            printf("could not write to file.\n");
            return -1;
        }

        for(i=0;i<81;i++){
              char cell;
            sprintf(&cell, "%d", source[i]);
            fputs(&cell, file);
        }


        fclose(file);
        
        return 1;
}


Puzzle *create_puzzle(Puzzle *puzzle) {
  srand(time(NULL));

  int i, j, swap, trio;
  for(j=0; j<SHUFF; j++){

    trio = (rand() % 3)*3;
    swap = rand() & 1;

    switch(rand() % 6){
      /* swap rows */
      case 0:
        swap_row(trio+0, swap ? trio+1 : trio+2);
        break;
      case 1:
        swap_row(trio+1, swap ? trio+0 : trio+2);
        break;
      case 2:
        swap_row(trio+2, swap ? trio+0 : trio+1);
        break;

      /* swap cols */
      case 3:
        swap_col(trio+0, swap ? trio+1 : trio+2);
        break;
      case 4:
        swap_col(trio+1, swap ? trio+0 : trio+2);
        break;
      case 5:
        swap_col(trio+2, swap ? trio+0 : trio+1);
        break;
    }
  }

  int size = puzzle->size;
  for (i = 0; i < size * size; i++) {
    puzzle->cells[i].value = source[i];
    puzzle->cells[i].row = i/size;
    puzzle->cells[i].col = i%size;

  }

  return puzzle;
}

void swap_row(int from, int to)
{
  int *ptr1, *ptr2, i, temp;
  ptr1 = source+(9*from);
  ptr2 = source+(9*to);
  for(i=0; i<9; i++){
    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
    ptr1++; 
    ptr2++;
  }
}

void swap_col(int from, int to)
{
  int *ptr1, *ptr2, i, temp;
  ptr1 = source+from;
  ptr2 = source+to;
  for(i=0; i<9; i++){
    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
    ptr1+=9;
    ptr2+=9;
  }
}


void make_puzzle() {
  srand(time(NULL));

  int i, j, swap, trio;
  for(j=0; j<SHUFF; j++){

    trio = (rand() % 3)*3;
    swap = rand() & 1;

    switch(rand() % 6){
      /* swap rows */
      case 0:
        swap_row(trio+0, swap ? trio+1 : trio+2);
        break;
      case 1:
        swap_row(trio+1, swap ? trio+0 : trio+2);
        break;
      case 2:
        swap_row(trio+2, swap ? trio+0 : trio+1);
        break;

      /* swap cols */
      case 3:
        swap_col(trio+0, swap ? trio+1 : trio+2);
        break;
      case 4:
        swap_col(trio+1, swap ? trio+0 : trio+2);
        break;
      case 5:
        swap_col(trio+2, swap ? trio+0 : trio+1);
        break;
    }
  }
}
