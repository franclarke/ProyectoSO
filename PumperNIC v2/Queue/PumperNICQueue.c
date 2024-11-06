#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define KEY ((key_t)(1243))

// Preparation times
#define TIEMPO_HAMBURGUESA 1
#define TIEMPO_MENU_VEGANO 1
#define TIEMPO_PAPAS 2

// Define message types with enums
typedef enum {
    PEDIDO_VIP = 1,
    PEDIDO_NOVIP,
    HAMBURGUESA,
    VEGANO,
    PAPAS,
    FILA
} message_type;

// Initial client ID
#define ID_CLIENTE_INICIAL 7

#define CANT_CLIENTES 10
#define CAPACIDAD 8

// Structure for the message queue
typedef struct {
    long tipo;
    int vip;        // 0 = non-VIP, 1 = VIP
    int combo;      // 0 = Hamburger, 1 = Vegan Menu, 2 = Fries
    int ID_cliente; // Unique ID for each client
} mensaje;

// Function prototypes
void* preparar_hamburguesa(void* arg);
void* preparar_menu_vegano(void* arg);
void* preparar_papas(void* arg);
void* recibir(void* arg);
void* cliente(void* arg);

// Main function
int main(int argc, char** argv) {
    int qid = msgget(KEY, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("Error creating message queue");
        exit(EXIT_FAILURE);
    }

    // Initialize client ID and queue capacity
    int ID_cliente = ID_CLIENTE_INICIAL;
    mensaje fila_max;
    fila_max.tipo = FILA;

    for (int i = 0; i < CAPACIDAD; i++) {
        if (msgsnd(qid, &fila_max, sizeof(mensaje) - sizeof(long), 0) == -1) {
            perror("Error initializing queue");
            msgctl(qid, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }
    }

    // Create threads for employees
    pthread_t thread_hamburguesa, thread_vegano, thread_papas1, thread_papas2, thread_recibir;
    pthread_create(&thread_hamburguesa, NULL, preparar_hamburguesa, NULL);
    pthread_create(&thread_vegano, NULL, preparar_menu_vegano, NULL);
    pthread_create(&thread_papas1, NULL, preparar_papas, NULL);
    pthread_create(&thread_papas2, NULL, preparar_papas, NULL);
    pthread_create(&thread_recibir, NULL, recibir, NULL);

    // Create threads for clients
    pthread_t client_threads[CANT_CLIENTES];
    for (int i = 0; i < CANT_CLIENTES; i++) {
        int* client_id = malloc(sizeof(int));
        *client_id = ID_cliente++;
        pthread_create(&client_threads[i], NULL, cliente, client_id);
    }

    // Wait for clients to finish
    for (int i = 0; i < CANT_CLIENTES; i++) {
        pthread_join(client_threads[i], NULL);
    }

    // Clean up the message queue
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        perror("Error deleting message queue");
    }

    return 0;
}

// Employee functions
void* preparar_hamburguesa(void* arg) {
    int qid = msgget(KEY, IPC_CREAT | 0666);
    mensaje orden;
    while (1) {
        msgrcv(qid, &orden, sizeof(mensaje) - sizeof(long), HAMBURGUESA, 0);
        printf("Preparing hamburger...\n");
        sleep(TIEMPO_HAMBURGUESA);
        orden.tipo = orden.ID_cliente;
        printf("Hamburger ready.\n");
        msgsnd(qid, &orden, sizeof(mensaje) - sizeof(long), 0);
    }
    return NULL;
}

void* preparar_menu_vegano(void* arg) {
    int qid = msgget(KEY, IPC_CREAT | 0666);
    mensaje orden;
    while (1) {
        msgrcv(qid, &orden, sizeof(mensaje) - sizeof(long), VEGANO, 0);
        printf("Preparing vegan menu...\n");
        sleep(TIEMPO_MENU_VEGANO);
        orden.tipo = orden.ID_cliente;
        printf("Vegan menu ready.\n");
        msgsnd(qid, &orden, sizeof(mensaje) - sizeof(long), 0);
    }
    return NULL;
}

void* preparar_papas(void* arg) {
    int qid = msgget(KEY, IPC_CREAT | 0666);
    mensaje orden;
    while (1) {
        msgrcv(qid, &orden, sizeof(mensaje) - sizeof(long), PAPAS, 0);
        printf("Preparing fries...\n");
        sleep(TIEMPO_PAPAS);
        orden.tipo = orden.ID_cliente;
        printf("Fries ready.\n");
        msgsnd(qid, &orden, sizeof(mensaje) - sizeof(long), 0);
    }
    return NULL;
}

void* recibir(void* arg) {
    int qid = msgget(KEY, 0666);
    mensaje pedido;
    while (1) {
        sleep(1);
        msgrcv(qid, &pedido, sizeof(mensaje) - sizeof(long), -PEDIDO_NOVIP, 0);
        printf("Attending client with ID: %i, VIP: %i, combo: %i.\n", pedido.ID_cliente, pedido.vip, pedido.combo);
        pedido.tipo = pedido.combo + HAMBURGUESA;
        msgsnd(qid, &pedido, sizeof(mensaje) - sizeof(long), 0);
    }
    return NULL;
}

void* cliente(void* arg) {
    int qid = msgget(KEY, 0666);
    srand(time(NULL) + *((int*)arg));
    mensaje pedido, fila;

    while (1) {
        sleep(rand() % 10);
        msgrcv(qid, &fila, sizeof(mensaje) - sizeof(long), FILA, 0);

        pedido.vip = rand() % 2;
        pedido.combo = rand() % 3;
        pedido.ID_cliente = *((int*)arg);
        pedido.tipo = PEDIDO_NOVIP - pedido.vip;

        printf("Client arrives with ID: %i, VIP: %i, combo: %i.\n", pedido.ID_cliente, pedido.vip, pedido.combo);
        msgsnd(qid, &pedido, sizeof(mensaje) - sizeof(long), 0);
        msgrcv(qid, &pedido, sizeof(mensaje) - sizeof(long), pedido.ID_cliente, 0);
        printf("Client leaves with ID: %i, VIP: %i, combo: %i.\n", pedido.ID_cliente, pedido.vip, pedido.combo);

        msgsnd(qid, &fila, sizeof(mensaje) - sizeof(long), 0);
    }
    free(arg);
    return NULL;
}
