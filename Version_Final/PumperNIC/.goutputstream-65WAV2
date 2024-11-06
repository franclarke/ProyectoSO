#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>


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
int pipe_pedido_hamburguesa[2];
int pipe_pedido_vegano[2];
int pipe_pedido_papas[2];
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
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_vegano[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_papas[1]);
    int hamburguesa = 1;
    int pedidoH;
    while(1){
	read(pipe_pedido_hamburguesa[0],&pedidoH,sizeof(int));
	printf("Preparando hamburguesa..\n");
	sleep(TIEMPO_HAMBURGUESA);
	write(pipe_hamburguesa[1],&hamburguesa,sizeof(int));
	printf("Hamburguesa lista.\n");
    }
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
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
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_papas[1]);
    int vegano = 1;
    int pedidoV;
    while(1){
	read(pipe_pedido_vegano[0],&pedidoV,sizeof(int));
	printf("Preparando menu vegano..\n");
	sleep(TIEMPO_MENU_VEGANO);
	write(pipe_vegano[1],&vegano,sizeof(int));
	printf("Menu vegano listo.\n");
    }
    close(pipe_pedido_vegano[0]);
    close(pipe_vegano[1]);
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
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_vegano[0]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_papas[1]);
    int papas = 1;
    int pedidoP;
    while(1){
	read(pipe_pedido_papas[0],&pedidoP,sizeof(int));
	printf("Preparando papas fritas..\n");
	sleep(TIEMPO_PAPAS);
	write(pipe_papas[1],&papas,sizeof(int));
	printf("Papas fritas listas.\n");
    }
    close(pipe_pedido_papas[0]);
    close(pipe_papas[1]);
}

void recibir_despachar(){
    //cierra pipes
    close(pipe_pedido[1]);
    close(pipe_despacho[0]);
    close(pipe_papas[1]);
    close(pipe_vegano[1]);
    close(pipe_hamburguesa[1]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_vegano[0]);
    int despacho = 1;
    int vegano,papas,hamburguesa;
    int pedidoH,pedidoP,pedidoV = 1;
    pedido p;
    while(1){
	read(pipe_pedido[0],&p,sizeof(pedido));
	//atiendo todos los vips
	while(p.VIP==1){
	    if(p.combo==0){
		write(pipe_pedido_hamburguesa[1],&pedidoH,sizeof(int));
		write(pipe_pedido_papas[1],&pedidoP,sizeof(int));
		read(pipe_hamburguesa[0],&hamburguesa,sizeof(int));
		read(pipe_papas[0],&papas,sizeof(int));
	    }
	    else{
		write(pipe_pedido_vegano[1],&pedidoV,sizeof(int));
		write(pipe_pedido_papas[1],&pedidoP,sizeof(int));
		read(pipe_vegano[0],&vegano,sizeof(int));
		read(pipe_papas[0],&papas,sizeof(int));
	    }
	    write(pipe_despacho[1],&despacho,sizeof(int));
	    read(pipe_pedido[0],&p,sizeof(pedido));
	}
	if(p.combo==0){
	    write(pipe_pedido_hamburguesa[1],&pedidoH,sizeof(int));
	    write(pipe_pedido_papas[1],&pedidoP,sizeof(int));
	    read(pipe_hamburguesa[0],&hamburguesa,sizeof(int));
	    read(pipe_papas[0],&papas,sizeof(int));
	}
	else{
	    write(pipe_pedido_vegano[1],&pedidoV,sizeof(int));
	    write(pipe_pedido_papas[1],&pedidoP,sizeof(int));
	    read(pipe_vegano[0],&vegano,sizeof(int));
	    read(pipe_papas[0],&papas,sizeof(int));
	}
	write(pipe_despacho[1],&despacho,sizeof(int));
    }
}

void cliente(int VIP, int combo){
    //cerrar pipes
    close(pipe_pedido[0]);
    close(pipe_despacho[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    int despacho;
    pedido p;
    p.VIP = VIP;
    p.combo = combo;
    printf("Llega cliente, VIP: %i, combo: %i.\n",VIP,combo);
    write(pipe_pedido[1],&p,sizeof(pedido));
    close(pipe_pedido[1]);
    printf("Cliente esperando..\n");
    read(pipe_despacho[0],&despacho,sizeof(int));
    close(pipe_despacho[0]);
    printf("Se va cliente, VIP: %i, combo: %i.\n",VIP,combo);
}

int main(int argc, char **argv){
    if(pipe(pipe_pedido)==-1) return 1;
    if(pipe(pipe_hamburguesa)==-1) return 1;
    if(pipe(pipe_papas)==-1) return 1;
    if(pipe(pipe_vegano)==-1) return 1;
    if(pipe(pipe_pedido_hamburguesa)==-1) return 1;
    if(pipe(pipe_pedido_papas)==-1) return 1;
    if(pipe(pipe_pedido_vegano)==-1) return 1;
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
	sleep(r%3);
    }
    return 0;
}



