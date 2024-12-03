#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common_threads.h"
#include "zemaphore.h"

// Added: Number of philosophers
int N;
// Added: Fork semaphores
Zem_t *forks;
// Added: Table semaphore for Algorithm #3
Zem_t table;

// Added: Philosopher thread function
void *philosopher(void *arg) {
    int id = *((int *)arg);

    while (1) {
        printf("Philosopher %d is thinking...\n", id);

        // Algorithm #3: Allow at most N-1 philosophers to dine simultaneously
        Zem_wait(&table);                  // Try to sit at the table
        Zem_wait(&forks[id]);              // Pick up left fork
        Zem_wait(&forks[(id + 1) % N]);    // Pick up right fork

        printf("Philosopher %d is eating...\n", id);

        Zem_post(&forks[id]);              // Put down left fork
        Zem_post(&forks[(id + 1) % N]);    // Put down right fork
        Zem_post(&table);                  // Leave the table

        printf("Philosopher %d is done eating and thinking again...\n", id);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_philosophers>\n", argv[0]);
        return 1;
    }

    // Added: Parse the number of philosophers
    N = atoi(argv[1]);
    if (N < 3 || N > 20) {
        fprintf(stderr, "Number of philosophers must be between 3 and 20.\n");
        return 1;
    }

    pthread_t philosophers[N];
    int ids[N];
    forks = malloc(sizeof(Zem_t) * N);
    Zem_init(&table, N - 1); // Initialize table for Algorithm #3

    // Added: Initialize semaphores for each fork
    for (int i = 0; i < N; i++) {
        Zem_init(&forks[i], 1); // Each fork starts as available
    }

    // Added: Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        Pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosopher threads
    for (int i = 0; i < N; i++) {
        Pthread_join(philosophers[i], NULL);
    }

    free(forks);
    return 0;
}
