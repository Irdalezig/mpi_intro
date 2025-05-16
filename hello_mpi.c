#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int id, p, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o identificador do processo (rank)
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    // Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // Obtém o nome do processador onde o processo está a correr
    MPI_Get_processor_name(processor_name, &name_len);

    Cada processo imprime sua saudação
    MPI_Barrier(MPI_COMM_WORLD);
    printf("O Processo %d de %d envia cumprimentos a partir da máquina %s!\n",
           id, p, processor_name);

           
    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}

