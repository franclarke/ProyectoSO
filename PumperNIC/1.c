#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
/*
 * Procesos: empleado hamburguesas, empleado menu vegano, 2 empleados papas fritas, empleado recibe pedidos y despacha,
 * cliente espera a ser atendido en una cola o se marcha, cliente VIP tiene prioridad.
 * politicas tomadas: Ante la llegada de un cliente VIP este tiene prioridad y hasta que su pedido no sea terminado los clientes no VIP no seran atendidos.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */
int pipe_pedido[2];
int pipe[];
int pipe[];
int pipe[];
int pipe[];
int pipe[];


typedef struct{
    int VIP;
    int combo; //0=Hamburguesa, 1=MenuVegano
} pedido;

void preparar_hamburguesa(){
    while(1){
	printf("Preparando hamburguesa..");
	sleep(2);
	printf("Hamburguesa lista.");
    }
}
void preparar_menu_vegano({
    while(1){
	printf("Preparando menu vegano..");
	sleep(2);
	printf("Menu vegano listo.");
    }
}


void preparar_papas(){
    while(1){
	printf("Preparando papas fritas..");
	sleep(4);
	printf("Papas fritas listas.");
    }
}

void recibir_pedido(){
    //cierra pipes
    while(1){
	read(pipe_pedido[1],&pedido,sizeof(struct pedido))
	
    }
}

void despachar(){
    
}

void cliente(int VIP, int combo){
    //cerrar pipes
    struct pedido;
    pedido->VIP = VIP;
    pedido->combo = combo;
    write(pipe_pedido[1],&pedido,sizeof(struct pedido));
    close(pipe_pedido[1]);
}

int main(int argc, char **argv){
    if(pipe(pipe_pedido)==-1) return 1;
    if(pipe(pipe)==-1) return 1;
    if(pipe(pipe)==-1) return 1;
    if(pipe(pipe)==-1) return 1;
    if(pipe(pipe)==-1) return 1;
    
    int VIP;
    int r;
    
    if(fork()==0)
	preparar_hamburguesa();
	else if(fork()==0)
	    preparar_menu_vegano();
	    else if(fork()==0)
		preparar_papas();
		else if(fork()==0)
		    preparar_papas();
		    else if(fork()==0)
			recibir_pedido();
    while(1){
	r = rand();
	VIP = r%2;
	sleep(r%10);
	if(fork()==0)
	    cliente(VIP,rand()%2);
    }
    return 0;
}


