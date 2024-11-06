#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define RENOS 9
#define ELFOS_NECESARIOS 3
#define TIEMPO_RENOS 1
#define TIEMPO_ELFOS 1

// Semaphores and Mutexes
sem_t trineo, santa_despierto, elfo_atendido, max_elfos;
pthread_mutex_t mutex_cant_renos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cant_elfos = PTHREAD_MUTEX_INITIALIZER;

// Counters
int renos_listos = 0;
int elfos_con_problema = 0;

// Santa function
void* santa(void* args) {
    while (1) {
        // Wait until Santa needs to wake up
        sem_wait(&santa_despierto);
        printf("Santa despierta\n");

        pthread_mutex_lock(&mutex_cant_renos);
        if (renos_listos == RENOS) {
            printf("Santa atiende a los renos\n");
            for (int i = 0; i < RENOS; i++) {
                sem_post(&trineo);  // Santa signals reindeer to prepare
            }
            renos_listos = 0;
        }
        pthread_mutex_unlock(&mutex_cant_renos);

        pthread_mutex_lock(&mutex_cant_elfos);
        if (elfos_con_problema == ELFOS_NECESARIOS) {
            printf("Santa atiende a los elfos\n");
            for (int i = 0; i < ELFOS_NECESARIOS; i++) {
                sem_post(&elfo_atendido);  // Santa helps elves
            }
            elfos_con_problema = 0;
            for (int i = 0; i < ELFOS_NECESARIOS; i++) {
                sem_post(&max_elfos);  // Allow other elves to get help
            }
        }
        pthread_mutex_unlock(&mutex_cant_elfos);
    }
    return NULL;
}

// Reindeer function
void* reno(void* args) {
    while (1) {
        sleep(TIEMPO_RENOS);  // Simulate time taken by reindeer to return

        pthread_mutex_lock(&mutex_cant_renos);
        renos_listos++;
        if (renos_listos == RENOS) {
            printf("Último reno llega, despertando a Santa\n");
            sem_post(&santa_despierto);
        } else {
            printf("Reno esperando en la cabaña\n");
        }
        pthread_mutex_unlock(&mutex_cant_renos);

        sem_wait(&trineo);  // Wait to be hooked to the sleigh
    }
    return NULL;
}

// Elf function
void* elfo(void* args) {
    while (1) {
        sem_wait(&max_elfos);  // Limit to 3 elves waiting at a time
        sleep(TIEMPO_ELFOS);  // Simulate toy-making issues

        pthread_mutex_lock(&mutex_cant_elfos);
        elfos_con_problema++;
        if (elfos_con_problema == ELFOS_NECESARIOS) {
            printf("Tres elfos necesitan ayuda, despertando a Santa\n");
            sem_post(&santa_despierto);
        } else {
            printf("Elfo esperando a que Santa le ayude\n");
        }
        pthread_mutex_unlock(&mutex_cant_elfos);

        sem_wait(&elfo_atendido);  // Wait for Santa's help
    }
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t t_santa, t_renos[RENOS], t_elfos[10];

    // Initialize semaphores
    sem_init(&trineo, 0, 0);
    sem_init(&elfo_atendido, 0, 0);
    sem_init(&santa_despierto, 0, 0);
    sem_init(&max_elfos, 0, ELFOS_NECESARIOS);

    // Create Santa thread
    pthread_create(&t_santa, NULL, santa, NULL);

    // Create Reindeer threads
    for (int i = 0; i < RENOS; i++) {
        pthread_create(&t_renos[i], NULL, reno, NULL);
    }

    // Create Elf threads
    for (int i = 0; i < 10; i++) {
        pthread_create(&t_elfos[i], NULL, elfo, NULL);
    }

    // Wait for Santa thread (in practice, this will run indefinitely)
    pthread_join(t_santa, NULL);

    // Wait for Reindeer threads
    for (int i = 0; i < RENOS; i++) {
        pthread_join(t_renos[i], NULL);
    }

    // Wait for Elf threads
    for (int i = 0; i < 10; i++) {
        pthread_join(t_elfos[i], NULL);
    }

    // Clean up semaphores
    sem_destroy(&trineo);
    sem_destroy(&elfo_atendido);
    sem_destroy(&santa_despierto);
    sem_destroy(&max_elfos);

    return 0;
}
