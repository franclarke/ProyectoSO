#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>

#define TIEMPO_HAMBURGUESA 2
#define TIEMPO_MENU_VEGANO 2
#define TIEMPO_PAPAS 4
#define CANT_CLIENTES 10

/*
 * Procesos: empleado hamburguesas, empleado menu vegano, 2 empleados papas fritas, empleado recibe pedidos,
 * cliente espera a ser atendido en una cola o se marcha,cliente VIP tiene prioridad al ser atendido.
 * Politicas tomadas: 
 * -Los clientes toman su pedido por su cuenta, que puede ser de un VIP o no, esto debido a la complejidad que representa implementar con pipes un 
 * sistema donde cada cliente reciba su correspondiente pedido.
 * -Los clientes pueden esperar o marcharse de forma random.
 * */

typedef struct{
    int VIP;   //0 = no VIP, 1 = VIP
    int combo; //0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
} pedido;

//Declaraciones de pipes
int pipe_pedidoNOVIP[2];
int pipe_pedidoVIP[2];
int pipe_hay_clientes[2];
int pipe_hamburguesa[2];
int pipe_vegano[2];
int pipe_papas[2];
int pipe_orden_hamburguesa[2];
int pipe_orden_vegano[2];
int pipe_orden_papas[2];

void preparar_hamburguesa(){
    //cerrar pipes
    close(pipe_pedidoNOVIP[0]);
    close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[0]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_papas[1]);
    close(pipe_hay_clientes[0]);
    close(pipe_hay_clientes[1]);

    int hamburguesa = 1;
    int ordenH;

    while(1){
        read(pipe_orden_hamburguesa[0],&ordenH,sizeof(int));
        printf("Preparando hamburguesa..\n");
        sleep(TIEMPO_HAMBURGUESA);
        write(pipe_hamburguesa[1],&hamburguesa,sizeof(int));
        printf("Hamburguesa lista.\n");
    }

    close(pipe_orden_hamburguesa[0]);
    close(pipe_hamburguesa[1]);

    exit(0);
}

void preparar_menu_vegano(){
    //cerrar pipes
    close(pipe_pedidoNOVIP[0]);
    close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[0]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_papas[1]);
    close(pipe_hay_clientes[0]);
    close(pipe_hay_clientes[1]);

    int vegano = 1;
    int ordenV;

    while(1){
        read(pipe_orden_vegano[0],&ordenV,sizeof(int));
        printf("Preparando menu vegano..\n");
        sleep(TIEMPO_MENU_VEGANO);
        write(pipe_vegano[1],&vegano,sizeof(int));
        printf("Menu vegano listo.\n");
    }

    close(pipe_orden_vegano[0]);
    close(pipe_vegano[1]);

    exit(0);
}

void preparar_papas(){
    //cerrar pipes
    close(pipe_pedidoNOVIP[0]);
    close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[0]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_papas[0]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_papas[1]);
    close(pipe_hay_clientes[0]);
    close(pipe_hay_clientes[1]);

    int papas = 1;
    int ordenP;

    while(1){
        read(pipe_orden_papas[0],&ordenP,sizeof(int));
        printf("Preparando papas fritas..\n");
        sleep(TIEMPO_PAPAS);
        write(pipe_papas[1],&papas,sizeof(int));
        printf("Papas fritas listas.\n");
    }

    close(pipe_orden_papas[0]);
    close(pipe_papas[1]);

    exit(0);
}

void recibir(){
    //cerrar pipes
    close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_vegano[0]);
    close(pipe_hay_clientes[1]);

    int ordenH,ordenP,ordenV = 1;
    pedido pedido_cliente;
    int hay_cliente;

    fcntl(pipe_pedidoNOVIP[0], F_SETFL,O_NONBLOCK);
    fcntl(pipe_pedidoVIP[0], F_SETFL,O_NONBLOCK);

    while(1){
        //Hay algun cliente
        read(pipe_hay_clientes[0],&hay_cliente,sizeof(int));
        //atiendo todos los vips
        if(read(pipe_pedidoVIP[0],&pedido_cliente,sizeof(pedido))!=-1){
            printf("Atiendo cliente VIP \n");
            if(pedido_cliente.combo==0)
                write(pipe_orden_hamburguesa[1],&ordenH,sizeof(int));
            else if(pedido_cliente.combo==1)
                write(pipe_orden_vegano[1],&ordenV,sizeof(int));
            else
                write(pipe_orden_papas[1],&ordenP,sizeof(int));
        }
        //Termino de atender vips y atiendo un no vip
        else if(read(pipe_pedidoNOVIP[0],&pedido_cliente,sizeof(pedido))!=-1){
            printf("Atiendo cliente no VIP \n");
            if(pedido_cliente.combo==0)
                write(pipe_orden_hamburguesa[1],&ordenH,sizeof(int));
            else if(pedido_cliente.combo==1)
                write(pipe_orden_vegano[1],&ordenV,sizeof(int));
            else
                write(pipe_orden_papas[1],&ordenP,sizeof(int));
        }
    }

    close(pipe_hay_clientes[0]);
    close(pipe_pedidoNOVIP[0]);
    close(pipe_pedidoVIP[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_orden_papas[1]);

    exit(0);
}

void cliente(){
    //cerrar pipes
    close(pipe_pedidoNOVIP[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_papas[1]);
    close(pipe_vegano[1]);
    close(pipe_orden_hamburguesa[0]);
    close(pipe_orden_papas[0]);
    close(pipe_orden_vegano[0]);
    close(pipe_orden_hamburguesa[1]);
    close(pipe_orden_papas[1]);
    close(pipe_orden_vegano[1]);
    close(pipe_hay_clientes[0]);

    srand(getpid());

    int despacho;
    int hay_cliente = 1;
    pedido pedido_cliente;

    while(1){
        sleep(rand()%10);
	if(rand()%10==9){
	    printf("Un cliente se va porque hay mucha fila, volvera mas tarde\n");
	    sleep(rand()%50);
	}
        pedido_cliente.VIP = rand()%2;
        pedido_cliente.combo = rand()%3;
        printf("Llega cliente, VIP: %i, combo: %i.\n",pedido_cliente.VIP,pedido_cliente.combo);
        if(pedido_cliente.VIP==0)
            write(pipe_pedidoNOVIP[1],&pedido_cliente,sizeof(pedido));
        else
            write(pipe_pedidoVIP[1],&pedido_cliente,sizeof(pedido));
        write(pipe_hay_clientes[1],&hay_cliente,sizeof(char));
        if(pedido_cliente.combo == 0)
            read(pipe_hamburguesa[0],&despacho,sizeof(int));
        else if(pedido_cliente.combo == 1)
            read(pipe_vegano[0],&despacho,sizeof(int));
        else
            read(pipe_papas[0],&despacho,sizeof(int));
        printf("Se va cliente, VIP: %i, combo: %i.\n",pedido_cliente.VIP,pedido_cliente.combo);
    }

    close(pipe_pedidoNOVIP[1]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hay_clientes[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_vegano[0]);
    close(pipe_papas[0]);

    exit(0);
}

int main(int argc, char **argv){
    //crear pipes
    if(pipe(pipe_pedidoNOVIP)==-1) return 1;
    if(pipe(pipe_hamburguesa)==-1) return 1;
    if(pipe(pipe_papas)==-1) return 1;
    if(pipe(pipe_vegano)==-1) return 1;
    if(pipe(pipe_orden_hamburguesa)==-1) return 1;
    if(pipe(pipe_orden_papas)==-1) return 1;
    if(pipe(pipe_orden_vegano)==-1) return 1;
    if(pipe(pipe_pedidoVIP)==-1) return 1;
    if(pipe(pipe_hay_clientes)==-1) return 1;
    
    //crear procesos empleados
    if(fork()==0)
	    preparar_hamburguesa();
	else if(fork()==0)
	    preparar_menu_vegano();
	else if(fork()==0)
	    preparar_papas();
    else if(fork()==0)
	    preparar_papas();
	else if(fork()==0)
	    recibir();
    
    //crear procesos clientes
    for(int i = 0; i<CANT_CLIENTES; i++){
	if(fork()==0)
	    cliente();
    }

    //espero que se vayan los clientes
    for(int i = 0; i<CANT_CLIENTES; i++)
	    wait(NULL);
    
    //termino los procesos empleados
    for(int i = 0; i<5; i++)
	    wait(NULL);
    
    return 0;
}



