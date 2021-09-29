/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rosseto */
/* Laboratório: 8 */
/* Aluna: Bruna Santos de Andrade */
/* DRE: 118094365 */

/* Condicao Logica da Aplicação: A thread 2 deve sempre ser a primeira a imprimir e a thread 3 deve sempre ser a última, não importando a ordem das threads 1 e 4 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

/* Variaveis Globais*/
int x = 0;
int y = 0;
sem_t cond1, cond2, cond4;

void *thread_1 (void *t) {
    
    sem_wait(&cond2);

    printf("Fique a vontade.\n");

    sem_post(&cond1);
    sem_post(&cond2);

    pthread_exit(NULL);
}

void *thread_2 (void *t) {

    printf("Seja bem-vindo!\n");

    sem_post(&cond2);

    pthread_exit(NULL);
}

void *thread_3 (void *t) {

    sem_wait(&cond1);
    sem_wait(&cond4);
    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

void *thread_4 (void *t) {
    
    sem_wait(&cond2);
    
    printf("Sente-se por favor.\n");
    
    sem_post(&cond4);
    sem_post(&cond2);
    
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    int i;
    pthread_t tid[NTHREADS];

    // Inicializa os semáforos
    sem_init(&cond1, 0, 0);
    sem_init(&cond2, 0, 0);
    sem_init(&cond4, 0, 0);

    /* Criando as threads*/
     if(pthread_create(&tid[0], NULL, thread_1, NULL)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             exit(-1);
    }

    if(pthread_create(&tid[1], NULL, thread_2, NULL)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             exit(-1);
    }

    if(pthread_create(&tid[2], NULL, thread_3, NULL)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             exit(-1);
    }

    if(pthread_create(&tid[3], NULL, thread_4, NULL)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             exit(-1);
    }

    /* Espera as threads terminarem */
    for(i = 0; i < NTHREADS; i++) {
        if(pthread_join(tid[i], NULL)) {
            fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_join\n");
        };
    }

    return 0;
}