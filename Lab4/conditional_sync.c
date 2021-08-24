/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rosseto */
/* Laboratório: 4 */
/* Aluna: Bruna Santos de Andrade */
/* DRE: 118094365 */

/* Condicao Logica da Aplicação: A thread 2 deve sempre ser a primeira a imprimir e a thread 3 deve sempre ser a última, não importando a ordem das threads 1 e 4 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

/* Variaveis Globais*/
int x = 0;
int y = 0;
pthread_mutex_t x_mutex;
pthread_mutex_t y_mutex;
pthread_cond_t x_cond;
pthread_cond_t y_cond;

void *thread_1 (void *t) {
    pthread_mutex_lock(&x_mutex);
    if(x == 0) {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex);

    printf("Fique a vontade.\n");

    pthread_mutex_lock(&y_mutex);
    y++;
    if(y == 2) {
        pthread_cond_signal(&y_cond);
    }
    pthread_mutex_unlock(&y_mutex);

    pthread_exit(NULL);
}

void *thread_2 (void *t) {
    printf("Seja bem-vindo!\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

void *thread_3 (void *t) {
    pthread_mutex_lock(&y_mutex);
    if(y < 2) {
        pthread_cond_wait(&y_cond, &y_mutex);
    }
    pthread_mutex_unlock(&y_mutex);
    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

void *thread_4 (void *t) {
    pthread_mutex_lock(&x_mutex);
    if(x == 0) {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex);

    printf("Sente-se por favor.\n");

    pthread_mutex_lock(&y_mutex);
    y++;
    if(y == 2) {
        pthread_cond_signal(&y_cond);
    }
    pthread_mutex_unlock(&y_mutex);

    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    int i;
    pthread_t tid[NTHREADS];

    // Inicializa variaveis de condicao e mutex
    pthread_mutex_init(&x_mutex, NULL);
    pthread_mutex_init(&y_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);
    pthread_cond_init(&y_cond, NULL);

    /* Criando as threads*/
    pthread_create(&tid[0], NULL, thread_1, NULL);
    pthread_create(&tid[1], NULL, thread_2, NULL);
    pthread_create(&tid[2], NULL, thread_3, NULL);
    pthread_create(&tid[3], NULL, thread_4, NULL);

    /* Espera as threads terminarem */
    for(i = 0; i < NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    // Destroi variaveis de condicao e mutex
    pthread_mutex_destroy(&x_mutex);
    pthread_mutex_destroy(&y_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&y_cond);

    return 0;
}