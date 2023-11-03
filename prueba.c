#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// ssh curso166@ft3.cesga.es
// Compilar y ejecutar:
// module load gcc openmpi/4.1.1_ft3
// mpicc -o montecarlo_mpi -lm montecarlo_mpi.c
// sbatch montecarlo_mpi.sh montecarlo_mpi
// squeue
// cat myjob.*.out

#define N 64	


void imprimirMatriz(int** matriz, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
}



int main(int argc, char **argv) {

	//Lo primero que hacemos es reservar memoria para las matrices:
	int **matriz1 = (int **)malloc(N * sizeof(int *));
	int **matriz2 = (int **)malloc(N * sizeof(int *));
	if (matriz1 == NULL || matriz2 == NULL) {
		printf("No se pudo asignar memoria.\n");
		return 1;
	}

	for (int i = 0; i < N; i++) {
		matriz1[i] = (int *)malloc(N * sizeof(int));
		matriz2[i] = (int *)malloc(N * sizeof(int))
		if (matriz1[i] == NULL || matriz2[i] == NULL) {
		    printf("No se pudo asignar memoria.\n");
		    return 1;
		}
	}
	
	//Ahora vamos a inicializar MPI
	MPI_Init(&argc, &argv);

	int node=0, npes;
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &node);

	printf("Node %d (of %d) starting.\n", node, npes);
	
	//Ahora haremos la distribución de los datos con MPI_Scatter:
	MPI_Scatter(matriz1, 1, MPI_INT, local_data, 1, MPI_INT, 0, MPI_COMM_WORLD)
	
	printf("Desde el proceso %d, la matriz recibida es:\n", node);
	imprimirMatriz(local_data, 1, 1);

	MPI_Finalize();
}
