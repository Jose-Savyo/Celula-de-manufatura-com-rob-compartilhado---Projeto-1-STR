#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 3

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t mutex, cheio, vazio;

void *produtor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % 3 + 1);  // Tempo de produção

        int item = rand() % 100 + 1;  // Valor do item produzido

        sem_wait(&vazio);
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Produtor %d produziu: %d\n", id, item);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&cheio);
    }
}

void *consumidor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % 2 + 1);  // Tempo de processamento

        sem_wait(&cheio);
        sem_wait(&mutex);

        int item = buffer[out];
        printf("Consumidor %d consumiu: %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&vazio);
    }
}

int main() {
    srand(time(NULL));

    sem_init(&mutex, 0, 1);
    sem_init(&cheio, 0, 0);
    sem_init(&vazio, 0, BUFFER_SIZE);

    pthread_t produtores[NUM_PRODUTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];

    int prod_ids[NUM_PRODUTORES];
    int cons_ids[NUM_CONSUMIDORES];

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        prod_ids[i] = i;
        pthread_create(&produtores[i], NULL, produtor, &prod_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        cons_ids[i] = i;
        pthread_create(&consumidores[i], NULL, consumidor, &cons_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&cheio);
    sem_destroy(&vazio);

    return 0;
}

