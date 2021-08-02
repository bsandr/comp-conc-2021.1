#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long long int num_elements;
int nthreads;
float *vetor;

struct values {
    float max_value;
    float min_value;
};

void *tarefa(void *arg) {
    long int id = (long int) arg;
    struct values *local_values;
    long int thread_block_size = num_elements / nthreads;
    long int thread_block_init = id * thread_block_size;
    long int thread_block_end;

    local_values = (struct values*) malloc(sizeof(struct values));
    if(local_values == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc");
        exit(1);
    }

    local_values->max_value = vetor[thread_block_init];
    local_values->min_value = vetor[thread_block_init];

    if(id == nthreads - 1) {
        thread_block_end = num_elements;
    } else {
        thread_block_end = thread_block_init + thread_block_size;
    }

    for(long int i = thread_block_init; i < thread_block_end; i++) {
        if(vetor[i] > local_values->max_value) {
            local_values->max_value = vetor[i];
        }

        if(vetor[i] < local_values->min_value) {
            local_values->min_value = vetor[i];
        }
    }

    pthread_exit((void *) local_values);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    struct values seq_values;
    struct values *return_values, *conc_values;
    double initial_time, end_time;
    pthread_t *tid;

    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads> \n", argv[0]);
        return 1;
    }

    num_elements = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    vetor = (float *) malloc(sizeof(float) * num_elements);
    if(vetor == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc");
        return 2;
    }

    for(long int i = 0; i < num_elements; i++) {
        vetor[i] = 1000.1/((rand() % 1000) + 1);
    }

    GET_TIME(initial_time);
    seq_values.max_value = vetor[0];
    seq_values.min_value = vetor[0];
    for(long int i = 0; i < num_elements; i++) {
        if(vetor[i] > seq_values.max_value) {
            seq_values.max_value = vetor[i];
        }

        if(vetor[i] < seq_values.min_value) {
            seq_values.min_value = vetor[i];
        }
    }
    GET_TIME(end_time);
    printf("Tempo sequencial: %lf\n", end_time - initial_time);

    GET_TIME(initial_time);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if( tid == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc\n");
        return 2;
    }

    for(long int i = 0; i < nthreads; i++) {
        if(pthread_create(tid + i, NULL, tarefa, (void *) i)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             return 3;
        }
    }

    conc_values = (struct values*) malloc(sizeof(struct values));
    if(conc_values == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc\n");
        return 2;
    }

    conc_values->max_value = vetor[0];
    conc_values->min_value = vetor[0];
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid + i), (void**) &return_values)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_join\n");
             return 3;
        }
        
        if(return_values->max_value > conc_values->max_value) {
            conc_values->max_value = return_values->max_value; 
        }

        if(return_values->min_value < conc_values->min_value) {
            conc_values->min_value = return_values->min_value; 
        }

        free(return_values);
    }
    GET_TIME(end_time);
    printf("Tempo concorrente: %lf\n", end_time - initial_time);

    printf("Max sequencial value: %f\n", seq_values.max_value);
    printf("Min sequencial value: %f\n", seq_values.min_value);
    printf("Max concurrent value: %f\n", conc_values->max_value);
    printf("Min concurrent value: %f\n", conc_values->min_value);

    free(vetor);
    free(tid);
    free(conc_values);

    return 0;
}