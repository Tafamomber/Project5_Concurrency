#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common_threads.h"
#include "zemaphore.h"

// Number of chairs and waiting customers
int num_chairs;
int waiting_customers = 0;
// Semaphores for barber readiness, customer readiness, and mutex
Zem_t barber_ready;
Zem_t customer_ready;
Zem_t mutex;

void *customer(void *arg) {
    int id = *((int *)arg);

    Zem_wait(&mutex);
    if (waiting_customers < num_chairs) {
        waiting_customers++;
        printf("Customer %d is waiting.\n", id);
        Zem_post(&customer_ready);
        Zem_post(&mutex);

        Zem_wait(&barber_ready);
        printf("Customer %d is getting a haircut.\n", id);
    } else {
        printf("Customer %d left as no chairs were available.\n", id);
        Zem_post(&mutex);
    }
    return NULL;
}

void *barber(void *arg) {
    while (1) {
        Zem_wait(&customer_ready);
        Zem_wait(&mutex);
        waiting_customers--;
        Zem_post(&barber_ready);
        Zem_post(&mutex);

        printf("Barber is cutting hair.\n");
        sleep(1); 
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_chairs>\n", argv[0]);
        return 1;
    }

    num_chairs = atoi(argv[1]);
    if (num_chairs <= 0) {
        fprintf(stderr, "Number of chairs must be positive.\n");
        return 1;
    }

    pthread_t barber_thread;
    pthread_t customers[20]; // Up to 20 customers
    int ids[20];

    Zem_init(&barber_ready, 0);
    Zem_init(&customer_ready, 0);
    Zem_init(&mutex, 1);

    Pthread_create(&barber_thread, NULL, barber, NULL);

    for (int i = 0; i < 20; i++) {
        ids[i] = i;
        Pthread_create(&customers[i], NULL, customer, &ids[i]);
        sleep(rand() % 3); // Random arrival times
    }

    for (int i = 0; i < 20; i++) {
        Pthread_join(customers[i], NULL);
    }

    return 0;
}
