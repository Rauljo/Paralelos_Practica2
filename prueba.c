#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  int rank, size;
  int strip_size, A_row, A_col;
  double **A, **strip_A, *Adata, *stripdata;
  MPI_Datatype strip;
  int i,j;

  MPI_Init(&argc,&argv) ;

  MPI_Comm_rank(MPI_COMM_WORLD,&rank) ;
  MPI_Comm_size(MPI_COMM_WORLD,&size) ;

      if(rank == 0) {
                    A_row = 10;
                    A_col = 10;
                    /* calculate the strip size */
                    strip_size = A_row / size;

                    /* genarate Matrix A */
                    Adata = (double *)malloc(sizeof(double)*A_row*A_col);
                    A = (double **)malloc(sizeof(double*) * A_row);
                    for(i = 0; i < A_row; i++) {
                            A[i] = &(Adata[i*A_col]);
                    }
                    int k = 0;
                    for(i = 0; i < A_row; i++) {
                            for(j = 0; j < A_col; j++) {
                                    A[i][j] = k;
                                    k++;
                            }
                    }
            }
            
            printf("Hola desde %d nodo\n", rank);

            /* Broadcasting the row, column size of Matrix A as well as strip size and Matrix B*/
            MPI_Bcast(&A_row, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&A_col, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&strip_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

            /* defining a datatype for sub-matrix */
            MPI_Type_vector(strip_size, A_col, A_col, MPI_DOUBLE, &strip);
            MPI_Type_commit(&strip);

            stripdata = (double *)malloc(sizeof(double)*strip_size*A_col);
            strip_A = (double **)malloc(sizeof(double*)*strip_size);
            for(i= 0; i< strip_size; i++) {
                    strip_A[i] = &(stripdata[i*A_col]);
            }

            MPI_Scatter(Adata, 1, strip, &(strip_A[0][0]), 1, strip, 0, MPI_COMM_WORLD);
            
            printf("Duplicando desde %d nodo\n", rank);
            
            for i=0; i<strip_size; i++){
            	for (j=0; j<A_col;j++){
            		strip_A[i][j] *= 2
            	}
            }
            
            MPI_Gather(&(strip_A[0][0]), 1, strip, Adata, 1, 0, MPI_COMM_WORLD);
            
            

            for(i = 0; i < strip_size; i++) {
                    if(i == 0) {
                            printf("rank = %d\n", rank);
                    }
                    for(j = 0; j < A_col; j++) {
                            printf("%lf  ", strip_A[i][j]);
                    }
                    printf("\n");
            }
        

    MPI_Type_free(&strip);
    free(strip_A);
    free(stripdata);
    free(Adata);
    free(A);
    return 0;
}
