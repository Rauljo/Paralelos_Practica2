#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define REAL 1.5

void imprimirMatriz(double* matriz, int tamano) {
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            printf("%lf ", matriz[i * tamano + j]);
        }
        printf("\n");
    }
}

// Función para leer una matriz desde un archivo
double* leerMatrizDesdeArchivo(const char* nombreArchivo, int* tamano) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Lee el tamaño de la matriz desde la primera línea del archivo
    fscanf(archivo, "%d", tamano);

    int N = *tamano;

    // Reserva memoria para la matriz
    double* matriz = (double*)malloc(N * N * sizeof(double));
   

    // Lee los elementos de la matriz desde el archivo
    for (int i = 0; i < *tamano; i++) {
        for (int j = 0; j < *tamano; j++) {
            fscanf(archivo, "%lf", &matriz[i * N + j]);
        }
    }

    // Cierra el archivo
    fclose(archivo);

    return matriz;
}

int main(int argc, char** argv){
    int num_proc, rank;

    int tamanoMatriz;

    double *recA, *recB, *recC;
    double* matrizA;
    double* matrizB;
    double* matrizC;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){

        matrizA = leerMatrizDesdeArchivo("matrizA.txt", &tamanoMatriz);
        matrizB = leerMatrizDesdeArchivo("matrizB.txt", &tamanoMatriz);

        matrizC = (double*)malloc(tamanoMatriz * tamanoMatriz * sizeof(double));


    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&tamanoMatriz, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int N = tamanoMatriz;

    recA = (double*)malloc(N * N/num_proc * sizeof(double));
    recB = (double*)malloc(N * N/num_proc * sizeof(double));

    recC = (double*)malloc(N * N/num_proc * sizeof(double));

    int num_envios = N/num_proc;

    //Vamos a hacer envios iterativos de una fila cada vez a cada proceso. Al final, recA tendra filas alternas. 
    //En el tamaño no sé si poner N o N * num_proc
    for (int i = 0; i < num_envios; i++){
        MPI_Scatter(matrizA + (i*num_proc * N), N, MPI_DOUBLE, recA+(i* N), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(matrizB+(i * num_proc * N), N, MPI_DOUBLE, recB+(i* N), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    //Ahora hacemos los calculos

    for (int i = 0; i < num_envios; i++){
        for (int j = 0; j < N; j++){
            recC[i * N + j] = REAL * recA[i * N + j] + recB[i * N + j];
        }
    }

    //Ahora hacemos Gather iterativos igualmente
    for (int i=0; i < num_envios; i++){
        MPI_Gather(recC+(i * N), N, MPI_DOUBLE, matrizC+(i * num_proc * N), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0){
        printf("Matriz resultado C\n");
        imprimirMatriz(matrizC, N);

        free(matrizA);
        free(matrizB);
        free(matrizC);
    }

    free(recA);
    free(recB);
    free(recC);

    MPI_Finalize();

    return 0;





}