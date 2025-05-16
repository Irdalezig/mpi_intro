/*
	Name: Send and Receive Message
	Copyright: 2021-22
	Author: Prof. Joao Costa
	Date: 13/05/22 10:28
	Description: 
	Simple experience to understand the Message Passing Interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[]) {

    MPI_Status status;
    int id, p, i, rounds;
    double secs;

    MPI_Init (&argc, &argv);

    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);

    if(argc != 2){
        if (!id) printf ("Command line: %s <n-rounds>\n", argv[0]);
        MPI_Finalize();
        exit (1);
    }
    rounds = atoi(argv[1]);

    MPI_Barrier (MPI_COMM_WORLD);
    secs = - MPI_Wtime();

    for(i = 0; i < rounds; i++){
        if(!id){
            MPI_Send(&i, 1, MPI_INT, 1, i, MPI_COMM_WORLD);
            MPI_Recv(&i, 1, MPI_INT, p-1, i, MPI_COMM_WORLD, &status);
        }
        else{
            MPI_Recv(&i, 1, MPI_INT, id-1, i, MPI_COMM_WORLD, &status);
            MPI_Send(&i, 1, MPI_INT, (id+1)%p, i, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier (MPI_COMM_WORLD);
    secs += MPI_Wtime();

    if(!id){
        printf("Rounds = %d, N Processes = %d, Time = %12.6f sec\n", rounds, p, secs);
        
        double avg_time_us = secs * 1e6 / (2 * rounds * p);
        printf("Average time per Send/Recv = %.2f us\n", avg_time_us);

        double latencia_us = (secs / (2 * rounds)) * 1e6;
        printf("Latência média de envio/receção = %.2f us\n", latencia_us);

        // Cálculo da largura de banda
        int tam_msg_bytes = sizeof(int); // tamanho da mensagem em bytes
        int total_msgs = 2 * rounds * p;
        double total_bytes = tam_msg_bytes * total_msgs;
        double bandwidth_bps = total_bytes / secs;
        double bandwidth_MBps = bandwidth_bps / (1024.0 * 1024.0);

        printf("Largura de banda aproximada = %.2f MB/s (%.2f bytes/s)\n", bandwidth_MBps, bandwidth_bps);
    }

    MPI_Finalize();
    return 0;
}
/*
  Testes de latência e largura de banda
   Compilação e execução:
	Compile: mpicc -o sendReceive sendReceive.c
	Run: mpirun -np 4 ./sendReceive 1000000
	Run: mpirun -np 4 ./sendReceive 10000000
	Run: mpirun -np 4 ./sendReceive 100000000
*/