#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pthread.h>

#define NUM_PHILOSOPHERS 5
#define NUM_MEALS 3

typedef enum { THINKING, HUNGRY, EATING } PhilosopherState;

int philosopherIds[NUM_PHILOSOPHERS];
PhilosopherState philosopherStates[NUM_PHILOSOPHERS];
pthread_t philosopherThreads[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;
int semId;

void init_semaphores() {
    semId = semget(IPC_PRIVATE, NUM_PHILOSOPHERS, IPC_CREAT | 0666);
    if (semId == -1) {
        perror("Failed to create semaphores");
        exit(1);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (semctl(semId, i, SETVAL, 1) == -1) {
            perror("Failed to set semaphore value");
            exit(1);
        }
    }
}

void check_availability(int philosopherId) {
    if (philosopherStates[philosopherId] == HUNGRY &&
        philosopherStates[(philosopherId + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS] != EATING &&
        philosopherStates[(philosopherId + 1) % NUM_PHILOSOPHERS] != EATING) {
        philosopherStates[philosopherId] = EATING;
        printf("philosopher # %d going to eat\n", philosopherId);
        struct sembuf semId_ops;
        semId_ops.sem_num = philosopherId;
        semId_ops.sem_op = 1;
        semId_ops.sem_flg = 0;
        if (semop(semId, &semId_ops, 1) == -1) {
            perror("Failed to perform semaphore V operation");
            exit(1);
        }
    }
}

void take_forks(int philosopherId) {
    pthread_mutex_lock(&mutex);
    philosopherStates[philosopherId] = HUNGRY;
    printf("philosopher # %d wants to eat\n", philosopherId);
    check_availability(philosopherId);
    pthread_mutex_unlock(&mutex);

    struct sembuf semId_ops;
    semId_ops.sem_num = philosopherId;
    semId_ops.sem_op = -1;
    semId_ops.sem_flg = 0;
    if (semop(semId, &semId_ops, 1) == -1) {
        perror("Failed to perform semaphore P operation");
        exit(1);
    }

    sleep(1); // Simulate time taken to pick up forks
}

void put_forks(int philosopherId) {
    pthread_mutex_lock(&mutex);
    philosopherStates[philosopherId] = THINKING;
    printf("Philosopher # %d done eating, now going to think\n", philosopherId);
    check_availability((philosopherId + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
    check_availability((philosopherId + 1) % NUM_PHILOSOPHERS);
    pthread_mutex_unlock(&mutex);

    struct sembuf semId_ops;
    semId_ops.sem_num = philosopherId;
    semId_ops.sem_op = 1;
    semId_ops.sem_flg = 0;
    if (semop(semId, &semId_ops, 1) == -1) {
        perror("Failed to perform semaphore V operation");
        exit(1);
    }

    sleep(1); // Simulate time taken to put down forks
}


void* philosopher(void* arg) {
    int philosopherId = *(int*)arg;

    for (int i = 0; i < NUM_MEALS; i++) {
        printf("Philosopher # %d is thinking\n", philosopherId);
        take_forks(philosopherId);
        sleep(2); // Simulate eating time
        put_forks(philosopherId);
    }

    return NULL;
}

int main() {
    init_semaphores();

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherIds[i] = i;
        philosopherStates[i] = THINKING;

        if (pthread_create(&philosopherThreads[i], NULL, philosopher, &philosopherIds[i]) != 0) {
            perror("Failed to create philosopher thread");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (pthread_join(philosopherThreads[i], NULL) != 0) {
            perror("Failed to join philosopher thread");
            exit(1);
        }
    }

    pthread_mutex_destroy(&mutex);
    semctl(semId, 0, IPC_RMID, 0);

    return 0;
}

