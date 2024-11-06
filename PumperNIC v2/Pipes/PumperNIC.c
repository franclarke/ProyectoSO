#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>
#include <string.h>
#include <sys/select.h>

#define TIEMPO_HAMBURGUESA 2
#define TIEMPO_MENU_VEGANO 2
#define TIEMPO_PAPAS 4
#define CANT_CLIENTES 10
#define TERMINATE -1

typedef struct {
    int VIP;   // 0 = no VIP, 1 = VIP
    int combo; // 0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
} pedido;

// Pipe declarations
int pipe_pedidoNOVIP[2];
int pipe_pedidoVIP[2];
int pipe_hay_clientes[2];
int pipe_hamburguesa[2];
int pipe_vegano[2];
int pipe_papas[2];
int pipe_orden_hamburguesa[2];
int pipe_orden_vegano[2];
int pipe_orden_papas[2];

void close_unneeded_pipes() {
    close(pipe_pedidoNOVIP[0]); close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[0]); close(pipe_pedidoVIP[1]);
    close(pipe_hay_clientes[0]); close(pipe_hay_clientes[1]);
    close(pipe_hamburguesa[0]); close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]); close(pipe_vegano[1]);
    close(pipe_papas[0]); close(pipe_papas[1]);
    close(pipe_orden_hamburguesa[0]); close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[0]); close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[0]); close(pipe_orden_papas[1]);
}

void preparar_hamburguesa() {
    close_unneeded_pipes();
    int hamburguesa = 1;
    int ordenH;
    while (1) {
        read(pipe_orden_hamburguesa[0], &ordenH, sizeof(int));
        if (ordenH == TERMINATE) break;
        printf("Preparando hamburguesa...\n");
        sleep(TIEMPO_HAMBURGUESA);
        write(pipe_hamburguesa[1], &hamburguesa, sizeof(int));
        printf("Hamburguesa lista.\n");
    }
    exit(0);
}

void preparar_menu_vegano() {
    close_unneeded_pipes();
    int vegano = 1;
    int ordenV;
    while (1) {
        read(pipe_orden_vegano[0], &ordenV, sizeof(int));
        if (ordenV == TERMINATE) break;
        printf("Preparando menu vegano...\n");
        sleep(TIEMPO_MENU_VEGANO);
        write(pipe_vegano[1], &vegano, sizeof(int));
        printf("Menu vegano listo.\n");
    }
    exit(0);
}

void preparar_papas() {
    close_unneeded_pipes();
    int papas = 1;
    int ordenP;
    while (1) {
        read(pipe_orden_papas[0], &ordenP, sizeof(int));
        if (ordenP == TERMINATE) break;
        printf("Preparando papas fritas...\n");
        sleep(TIEMPO_PAPAS);
        write(pipe_papas[1], &papas, sizeof(int));
        printf("Papas fritas listas.\n");
    }
    exit(0);
}

void recibir() {
    close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[1]);
    int ordenH = 1, ordenP = 1, ordenV = 1;
    pedido pedido_cliente;
    fd_set read_fds;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(pipe_pedidoNOVIP[0], &read_fds);
        FD_SET(pipe_pedidoVIP[0], &read_fds);

        int result = select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);
        if (result > 0) {
            if (FD_ISSET(pipe_pedidoVIP[0], &read_fds) && read(pipe_pedidoVIP[0], &pedido_cliente, sizeof(pedido)) != -1) {
                printf("Atiendo cliente VIP\n");
                if (pedido_cliente.combo == 0)
                    write(pipe_orden_hamburguesa[1], &ordenH, sizeof(int));
                else if (pedido_cliente.combo == 1)
                    write(pipe_orden_vegano[1], &ordenV, sizeof(int));
                else
                    write(pipe_orden_papas[1], &ordenP, sizeof(int));
            } else if (FD_ISSET(pipe_pedidoNOVIP[0], &read_fds) && read(pipe_pedidoNOVIP[0], &pedido_cliente, sizeof(pedido)) != -1) {
                printf("Atiendo cliente no VIP\n");
                if (pedido_cliente.combo == 0)
                    write(pipe_orden_hamburguesa[1], &ordenH, sizeof(int));
                else if (pedido_cliente.combo == 1)
                    write(pipe_orden_vegano[1], &ordenV, sizeof(int));
                else
                    write(pipe_orden_papas[1], &ordenP, sizeof(int));
            }
        }
    }
    exit(0);
}

void cliente() {
    srand(time(NULL) + getpid());
    int despacho;
    int hay_cliente = 1;
    pedido pedido_cliente;

    while (1) {
        sleep(rand() % 10);
        if (rand() % 10 == 9) {
            printf("Un cliente se va porque hay mucha fila, volverá más tarde\n");
            sleep(rand() % 50);
            continue;
        }
        pedido_cliente.VIP = rand() % 2;
        pedido_cliente.combo = rand() % 3;
        printf("Llega cliente, VIP: %d, combo: %d\n", pedido_cliente.VIP, pedido_cliente.combo);
        if (pedido_cliente.VIP == 0)
            write(pipe_pedidoNOVIP[1], &pedido_cliente, sizeof(pedido));
        else
            write(pipe_pedidoVIP[1], &pedido_cliente, sizeof(pedido));
        write(pipe_hay_clientes[1], &hay_cliente, sizeof(int));

        if (pedido_cliente.combo == 0)
            read(pipe_hamburguesa[0], &despacho, sizeof(int));
        else if (pedido_cliente.combo == 1)
            read(pipe_vegano[0], &despacho, sizeof(int));
        else
            read(pipe_papas[0], &despacho, sizeof(int));

        printf("Se va cliente, VIP: %d, combo: %d\n", pedido_cliente.VIP, pedido_cliente.combo);
    }
    exit(0);
}

int main(int argc, char **argv) {
    if (pipe(pipe_pedidoNOVIP) == -1 || pipe(pipe_hamburguesa) == -1 || pipe(pipe_papas) == -1 || pipe(pipe_vegano) == -1 ||
        pipe(pipe_orden_hamburguesa) == -1 || pipe(pipe_orden_papas) == -1 || pipe(pipe_orden_vegano) == -1 ||
        pipe(pipe_pedidoVIP) == -1 || pipe(pipe_hay_clientes) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    if (fork() == 0) preparar_hamburguesa();
    else if (fork() == 0) preparar_menu_vegano();
    else if (fork() == 0) preparar_papas();
    else if (fork() == 0) preparar_papas();
    else if (fork() == 0) recibir();

    for (int i = 0; i < CANT_CLIENTES; i++) {
        if (fork() == 0) cliente();
    }

    for (int i = 0; i < CANT_CLIENTES + 5; i++) {
        wait(NULL);
    }
    
    int terminate_signal = TERMINATE;
    write(pipe_orden_hamburguesa[1], &terminate_signal, sizeof(int));
    write(pipe_orden_vegano[1], &terminate_signal, sizeof(int));
    write(pipe_orden_papas[1], &terminate_signal, sizeof(int));

    return 0;
}
