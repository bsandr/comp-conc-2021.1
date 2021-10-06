/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rosseto */
/* Laboratório: 9 */
/* Aluna: Bruna Santos de Andrade */
/* DRE: 118094365 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10
#define P 4
#define C 5

//Variáveis Globais
int buffer[N];
sem_t full, empty, fullBuffer;
sem_t mutexProd, mutexCons; 

void inicializeBuffer(int n) {
    int i;
    for(i = 0; i < n; i++) {
        buffer[i] = 0;
    }
}

void get(int id, int n) {
    static int out = 0;
    int item;
    sem_wait(&fullBuffer);
    for(int i = 0; i < n; i++) {
        sem_wait(&full);
        sem_wait(&mutexCons);
        item = buffer[out];
        printf("Consumidora %d: removeu item %d da posição %d\n", id, item, out);
        out = (out + 1) % N;
        sem_post(&mutexCons);
        sem_post(&empty);
        sleep(1);
    }
}

void put(int item, int id, int n) {
    static int in = 0;
    sem_wait(&empty);
    sem_wait(&mutexProd);
    buffer[in] = item;
    printf("Produtora %d: inseriu item %d na posição %d\n", id, item, in);
    in = (in + 1) % N;
    sem_post(&mutexProd);
    sem_post(&full);
    if((n - 1) == in) {
        sem_post(&fullBuffer);
    }
}

void *produce (void *arg) {
    int *id = (int *) arg;
    printf("Sou a thread produtora %d\n", *id);
    while(1) {
        put(*id, *id, N);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void *consume (void *arg) {
    int *id = (int *) arg;
    printf("Sou a thread consumidora %d\n", *id);
    while(1) {
        get(*id, N);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    pthread_t tid[P+C];
    int *id[P+C];
    int i;

    //aloca espaço para os ids das threads
    for(i = 0; i < P+C; i++) {
        id[i] = malloc(sizeof(int));
        if (id[i] == NULL){
            exit(-1);
        }
        *id[i] = i+1;
    }

    //inicializa o buffer
    inicializeBuffer(N);
    
    // Inicializa os semáforos
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N);
    sem_init(&fullBuffer, 0, 0);
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);

    //cria as threads produtoras
    for(i = 0; i < P; i++){
        if(pthread_create(&tid[i], NULL, produce, (void *) id[i])){
            exit(-1);
        }
    }

    //cria as threads consumidoras
    for(i = 0; i < C; i++){
        if(pthread_create(&tid[i + P], NULL, consume, (void *) id[i + P])){
            exit(-1);
        }
    }

    pthread_exit(NULL);
    return 1;
}