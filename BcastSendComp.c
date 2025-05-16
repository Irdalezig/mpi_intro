#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 1000000  // 1 milhão de inteiros (~4MB)

int main(int argc, char** argv) {
    int rank, size;
    int *data;
    double start, end, time_bcast, time_send;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Aloca o array em todos os processos
    data = (int*)malloc(ARRAY_SIZE * sizeof(int));
    if (data == NULL) {
        printf("Erro ao alocar memória!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Inicializa o array no processo 0
    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            data[i] = i;
        }
    }

    // ========================
    // MÉTODO 1: MPI_Bcast
    // ========================
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Bcast(data, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    time_bcast = end - start;

    // ============================
    // MÉTODO 2: MPI_Send / MPI_Recv
    // ============================
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    if (rank == 0) {
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(data, ARRAY_SIZE, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(data, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    time_send = end - start;

    // Apenas o processo 0 imprime os tempos
    if (rank == 0) {
        printf("\n--- Comparação de desempenho ---\n");
        printf("Array size: %d inteiros (~%.2f MB)\n", ARRAY_SIZE, (ARRAY_SIZE * sizeof(int)) / (1024.0 * 1024.0));
        printf("Tempo com MPI_Bcast = %.6f segundos\n", time_bcast);
        printf("Tempo com MPI_Send individual = %.6f segundos\n", time_send);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
