#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>


#define TIEMPO_HAMBURGUESA 2
#define TIEMPO_MENU_VEGANO 2
#define TIEMPO_PAPAS 4
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
int pipe_hamburguesa[2];
int pipe_vegano[2];
int pipe_papas[2];
int pipe_despacho[2];

typedef struct{
    int VIP;   //0 = no VIP, 1 = VIP
    int combo; //0 = Hamburguesa, 1 = MenuVegano
} pedido;

void preparar_hamburguesa(){
    close(pipe_pedido[0]);
    close(pipe_pedido[1]);
    close(pipe_despacho[0]);
    close(pipe_despacho[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_hamburguesa[0]);
    int hamburguesa = 1;
    while(1){
	printf("Preparando hamburguesa..\n");
	sleep(TIEMPO_HAMBURGUESA);
	write(pipe_hamburguesa[1],&hamburguesa,sizeof(int));
	close(pipe_hamburguesa[1]);
	printf("Hamburguesa lista.\n");
    }
}
void preparar_menu_vegano(){
    close(pipe_pedido[0]);
    close(pipe_pedido[1]);
    close(pipe_despacho[0]);
    close(pipe_despacho[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_vegano[0]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    int vegano = 1;
    while(1){
	printf("Preparando menu vegano..\n");
	sleep(TIEMPO_MENU_VEGANO);
	write(pipe_vegano[1],&vegano,sizeof(int));
	printf("Menu vegano listo.\n");
    }
}


void preparar_papas(){
    close(pipe_pedido[0]);
    close(pipe_pedido[1]);
    close(pipe_despacho[0]);
    close(pipe_despacho[1]);
    close(pipe_papas[0]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    int papas = 1;
    while(1){
	printf("Preparando papas fritas..\n");
	sleep(TIEMPO_PAPAS);
	write(pipe_papas[1],&papas,sizeof(int));
	printf("Papas fritas listas.\n");
    }
}

void recibir_despachar(){
    close(pipe_pedido[0]);
    close(pipe_despacho[1]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[1]);
    close(pipe_papas[1]);
    //cierra pipes
    close(pipe_pedido[1]);
    close(pipe_despacho[0]);
    close(pipe_papas[0]);
    close(pipe_vegano[0]);
    close(pipe_hamburguesa[0]);
    int despacho = 1;
    int vegano,papas,hamburguesa;
    pedido p;
    while(1){
	read(pipe_pedido[0],&p,sizeof(pedido));
	close(pipe_pedido[0]);
	//atiendo todos los vips
	/*while((p.VIP)==1){
	    if((p.combo)==0){
		read(pipe_hamburguesa[0],&hamburguesa,sizeof(int));
		read(pipe_papas[0],&papas,sizeof(int));
	    }
	    else{
		read(pipe_vegano[0],&vegano,sizeof(int));
		read(pipe_papas[0],&papas,sizeof(int));
	    }
	    write(pipe_despacho[1],&despacho,sizeof(int));
	    read(pipe_pedido[0],&p,sizeof(pedido));
	}
	close(pipe_pedido[0]);
	if(p.combo==0){
	    read(pipe_hamburguesa[0],&hamburguesa,sizeof(int));
	    read(pipe_papas[0],&papas,sizeof(int));
	}
	else{
	    read(pipe_vegano[0],&vegano,sizeof(int));
	    read(pipe_papas[0],&papas,sizeof(int));
	}
	write(pipe_despacho[1],&despacho,sizeof(int));*/
    }
}

void cliente(int VIP, int combo){
    //cerrar pipes
    printf("Llega cliente, VIP: %i, combo: %i.\n",VIP,combo);
    close(pipe_pedido[0]);
    close(pipe_despacho[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_hamburguesa[1]);
    int despacho;
    pedido p;
    p.VIP = VIP;
    p.combo = combo;
    write(pipe_pedido[1],&p,sizeof(pedido));
    close(pipe_pedido[1]);
    printf("Cliente esperando..\n");
    read(pipe_hamburguesa[0],&despacho,sizeof(int));
    close(pipe_hamburguesa[0]);
    printf("Se va cliente.\n");
}

int main(int argc, char **argv){
    if(pipe(pipe_pedido)==-1) return 1;
    if(pipe(pipe_hamburguesa)==-1) return 1;
    if(pipe(pipe_papas)==-1) return 1;
    if(pipe(pipe_vegano)==-1) return 1;
    if(pipe(pipe_despacho)==-1) return 1;
    
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
			recibir_despachar();
    while(1){
	r = rand();
	VIP = r%2;
	if(fork()==0){
	    cliente(VIP,rand()%2);
	    return 0; }
	sleep(r%10);
    }
    return 0;
}



