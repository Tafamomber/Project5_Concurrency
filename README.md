# Project5_Concurrency

File Descriptions
dining_philosophers.c:

Solves the Dining Philosophers problem.
Supports three algorithms for synchronization:
Sequential fork pickup (can deadlock).
Simultaneous fork pickup (avoids deadlock).
N-1 philosophers dining simultaneously (minimizes contention).
barbershop.c:

Single-barber solution to the barbershop problem.
Simulates a barber serving customers with limited waiting chairs.
multi_barbershop.c:

Extends the barbershop problem to multiple barbers.
Simulates concurrent service with shared resources.
common.h, common_threads.h, zemaphore.h:

Utility and synchronization libraries for semaphore-based solutions.
