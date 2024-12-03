#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common_threads.h"
#include "zemaphore.h"

int num_chairs;
int num_barbers;
int waiting_customers = 0;

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
    int id = *((int *)arg);

    while (1) {
        Zem_wait(&customer_ready);
        Zem_wait(&mutex);
        waiting_customers--;
        Zem_post(&barber_ready);
        Zem_post(&mutex);

        printf("Barber %d is cutting hair.\n", id);
        sleep(1); 
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_chairs> <num_barbers>\n", argv[0]);
        return 1;
    }

    num_chairs = atoi(argv[1]);
    num_barbers = atoi(argv[2]);
    if (num_chairs <= 0 || num_barbers <= 0) {
        fprintf(stderr, "Number of chairs and barbers must be positive.\n");
        return 1;
    }

    pthread_t barber_threads[num_barbers];
    pthread_t customers[20]; // Up to 20 customers
    int barber_ids[num_barbers];
    int customer_ids[20];


    Zem_init(&barber_ready, 0);
    Zem_init(&customer_ready, 0);
    Zem_init(&mutex, 1);

   
    for (int i = 0; i < num_barbers; i++) {
        barber_ids[i] = i;
        Pthread_create(&barber_threads[i], NULL, barber, &barber_ids[i]);
    }

    for (int i = 0; i < 20; i++) {
        customer_ids[i] = i;
        Pthread_create(&customers[i], NULL, customer, &customer_ids[i]);
        sleep(rand() % 3); // Random arrival times
    }

   
    for (int i = 0; i < 20; i++) {
        Pthread_join(customers[i], NULL);
    }

    return 0;
}
