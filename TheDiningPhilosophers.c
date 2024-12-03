#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common_threads.h"
#include "zemaphore.h"

// Number of philosophers
int N;
// Added: Fork semaphores
Zem_t *forks;

Zem_t table;


void *philosopher(void *arg) {
    int id = *((int *)arg);

    while (1) {
        printf("Philosopher %d is thinking...\n", id);

        Zem_wait(&table);                  
        Zem_wait(&forks[id]);              
        Zem_wait(&forks[(id + 1) % N]);    

        printf("Philosopher %d is eating...\n", id);

        Zem_post(&forks[id]);              
        Zem_post(&forks[(id + 1) % N]);    
        Zem_post(&table);                  

        printf("Philosopher %d is done eating and thinking again...\n", id);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_philosophers>\n", argv[0]);
        return 1;
    }

 
    N = atoi(argv[1]);
    if (N < 3 || N > 20) {
        fprintf(stderr, "Number of philosophers must be between 3 and 20.\n");
        return 1;
    }

    pthread_t philosophers[N];
    int ids[N];
    forks = malloc(sizeof(Zem_t) * N);
    Zem_init(&table, N - 1); 

    
    for (int i = 0; i < N; i++) {
        Zem_init(&forks[i], 1); 
    }


    for (int i = 0; i < N; i++) {
        ids[i] = i;
        Pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

 
    for (int i = 0; i < N; i++) {
        Pthread_join(philosophers[i], NULL);
    }

    free(forks);
    return 0;
}
