/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rosseto */
/* Laboratório: 5 */
/* Aluna: Bruna Santos de Andrade */
/* DRE: 118094365 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Variáveis Globais */
long int num_elements;
int nthreads;
int interacoes;
int *vector;
int counter = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Função Barreira */
void barreira(int thread_id) {
    printf("Thread %d entrou na barreira\n", thread_id);
    pthread_mutex_lock(&x_mutex);
    if(counter == (nthreads - 1)) {
        pthread_cond_broadcast(&x_cond);
        printf("Threads saíram da barreira\n");
        counter = 0;
    } else {
        counter++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex);
}

/* Tarefa executada pelas threads */
void *soma_vetor(void *arg) {
    long int id = (long int) arg;
    long int somaLocal = 0;

    for(int step = 0; step < interacoes; step++) {
        printf("Thread %ld: passo=%d\n", id, step);
        
        for(long int i = 0; i < num_elements; i ++) {
            somaLocal += vector[i];
        }

        barreira(id);

        vector[id] = rand() % 10;

        barreira(id);
    }

    pthread_exit((void *) somaLocal);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    //variáveis locais
    pthread_t *tid;
    long int retorno;

    // Inicializa variaveis de condicao e mutex
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    if(argc < 2) {
        fprintf(stderr, "Digite: %s <numero de threads> \n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);
    num_elements = atoi(argv[1]);
    interacoes = atoi(argv[1]);

    //aloca o vetor de inteiros
    vector = (int *) malloc(sizeof(int) * num_elements);
    if(vector == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc");
        return 2;
    }

    //inicializa o vetor
    for(long int i = 0; i < num_elements; i++) {
        vector[i] = rand() % 10;
    }

    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if( tid == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc\n");
        return 2;
    }

    //cria as threads
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_create(tid + i, NULL, soma_vetor, (void *) i)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             return 3;
        }
    }

    //espera as threads retornarem e verifica se os valores retornados são iguais
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid + i), (void**) &retorno)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_join\n");
             return 3;
        }

        printf("Thread %ld: Soma=%ld\n", i, retorno);
    }

    // Destroi variaveis de condicao e mutex
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

    //desaloca as variáveis
    free(vector);
    free(tid);

    return 0;
}