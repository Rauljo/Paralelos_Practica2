#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

void imprimirMatriz(double *matriz, int NA, int YB) {
    for (int i = 0; i < NA; i++) {
        for (int j = 0; j < YB; j++) {
            printf("%lf\t", matriz[i * YB + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv){
	int num_proc, rank;

	if (argc != 5){
		printf("Falta el argumento N x Y de A o B\n");
		return 1;
	}


	int NA = atoi(argv[1]);
	int YA = atoi(argv[2]);
	int NB = atoi(argv[3]);
	int YB = atoi(argv[4]);

    if (YA != NB){
		printf("Las matrices no son multiplicables\n");
		return 1;
	}

    double *sendA, *sendB, *recvC;

    sendA = (double *) malloc(NA * YA * sizeof(double));
    sendB = (double *) malloc(NB * YB * sizeof(double));

    recvC = (double *) malloc(NA * YB * sizeof(double));

    for (int i = 0; i < NA; i++){
			for (int j = 0; j < YA; j++){
				sendA[i*YA + j] = i*YA + j;
			}
		}

    for (int i = 0; i < NB; i++){
        for (int j = 0; j < YB; j++){
            sendB[i*YB + j] =  2 *(i*YB + j);
        }
    }

    double measure_overhead;

    double t_resolution = MPI_Wtick();
    double t_start = MPI_Wtime();
    double t_end = MPI_Wtime();
    measure_overhead = t_end - t_start;

    printf("Overhead: %lf\n", measure_overhead);

    double t_start;
    double t_end;
    

    t_start = MPI_Wtime();

    for (int i = 0; i < NA; i++){
        for (int j = 0; j < YB ; j++){
            recvC[i*YB + j] = 0;
            for (int k = 0; k < YA; k++){
                recvC[i*YB + j] += sendA[i*YA + k] * sendB[k*YB + j];
            }
        }
    }

    t_end = MPI_Wtime();

    double t_elapsed = (t_end - t_start) - measure_overhead;

    printf("The calculatio has taken %e globally.\n", t_elapsed);

    free(sendA);
    free(sendB);
    free(recvC);




}