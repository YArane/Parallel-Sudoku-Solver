#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

static int size;

int get_number_possibilities(int cell[], int i);

int main(int argc, char **argv){
    
        /*     ~~ init MPI ~~    */
        
        int rank, np;
        if(MPI_SUCCESS != MPI_Init(&argc, &argv)){
            printf("error initializing MPI.\n");
            return -1;
        }
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &np);
     
                
        /*     ~~ Master Process ~~    */

        if(rank == 0){
            /*     ~~ Read Matrix ~~    */


            FILE *file;
            if(!(file = fopen(argv[1], "r")) || argc != 3){
                printf("usage: %s [matrix.txt], [size]\n", argv[0]);
                return -1;
            }
            
            size = atoi(argv[2]);
            int cell[size*size];
            char c;

            int i;
            while((c=fgetc(file)) != EOF){
               if(c == '\n') continue;
               cell[i++] = atoi(&c);
            }

            fclose(file);
            
            for(i=0;i<size;i++){
                printf("%d\n", get_number_of_possibilities(cell, i));
            }
                
        }


        /*     ~~ terminate MPI ~~    */
        
        if(MPI_SUCCESS != MPI_Finalize()){
            printf("error terminating MPI.\n");
            exit(-1);
        }
}


int get_number_of_possibilities(int cell[], int i){
    int possibilities[size+1];
    int count = 0;
    int k;
    for(k=1;k<size;k++){
        possibilities[k] = 1;
    }
    int row = col = i;
    do{
        // iterate over rows
        if(possibilities[cell[row]] == 1){
            possibilities[cell[row]] = 0;
            count++; 
        }
        // iterate over columns
        if(possibilities[cell[col]] == 1){
            possibilties[cell[col]] = 0;
            count++
        } 

        row = (row+1) % size;
        col = (col+size) % (size*size);

    }while(j%size != 0);
    
    return count; 
}
