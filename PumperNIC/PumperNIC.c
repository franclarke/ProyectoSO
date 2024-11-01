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
 * -Los clientes pueden esperar o marcharse de forma random, esto debido a la complejidad de implementarlo con pipes.
 * 
 * */

typedef struct{
    int VIP;   //0 = no VIP, 1 = VIP
    int combo; //0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
} pedido;

int pipe_pedido[2];
int pipe_hamburguesa[2];
int pipe_vegano[2];
int pipe_papas[2];
int pipe_pedido_hamburguesa[2];
int pipe_pedido_vegano[2];
int pipe_pedido_papas[2];
int pipe_pedidoVIP[2];
int pipe_clientes[2];

void preparar_hamburguesa(){
    close(pipe_pedido[0]);
    close(pipe_pedido[1]);
    close(pipe_pedidoVIP[0]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_vegano[0]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_papas[1]);
    close(pipe_clientes[0]);
    close(pipe_clientes[1]);
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
    close(pipe_pedidoVIP[0]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_papas[1]);
    close(pipe_clientes[0]);
    close(pipe_clientes[1]);
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
    close(pipe_pedidoVIP[0]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_papas[0]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_vegano[0]);
    close(pipe_pedido_papas[1]);
    close(pipe_clientes[0]);
    close(pipe_clientes[1]);
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

void recibir(){
    //cierra pipes
    close(pipe_pedido[1]);
    close(pipe_pedidoVIP[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[0]);
    close(pipe_vegano[1]);
    close(pipe_papas[0]);
    close(pipe_papas[1]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_vegano[0]);
    close(pipe_clientes[1]);
    int pedidoH,pedidoP,pedidoV = 1;
    pedido p;
    int cliente;
    fcntl(pipe_pedido[0], F_SETFL,O_NONBLOCK);
    fcntl(pipe_pedidoVIP[0], F_SETFL,O_NONBLOCK);
    while(1){
	//Hay algun cliente
	read(pipe_clientes[0],&cliente,sizeof(int));
	//atiendo todos los vips
	if(read(pipe_pedidoVIP[0],&p,sizeof(pedido))!=-1){
	    printf("Atiendo cliente VIP \n");
	    if(p.combo==0)
		write(pipe_pedido_hamburguesa[1],&pedidoH,sizeof(int));
	    else if(p.combo==1)
		write(pipe_pedido_vegano[1],&pedidoV,sizeof(int));
	    else
		write(pipe_pedido_papas[1],&pedidoP,sizeof(int));
	}
	//Termino de atender vips y atiendo un no vip
	else if(read(pipe_pedido[0],&p,sizeof(pedido))!=-1){
	    printf("Atiendo cliente no VIP \n");
	    if(p.combo==0)
		write(pipe_pedido_hamburguesa[1],&pedidoH,sizeof(int));
	    else if(p.combo==1)
		write(pipe_pedido_vegano[1],&pedidoV,sizeof(int));
	    else
		write(pipe_pedido_papas[1],&pedidoP,sizeof(int));
	}
	else printf("Error no atiendo a nadie\n");
    }
    close(pipe_clientes[0]);
    close(pipe_pedido[0]);
    close(pipe_pedidoVIP[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_vegano[1]);
    close(pipe_pedido_papas[1]);
}

void cliente(){
    //cerrar pipes
    close(pipe_pedido[0]);
    close(pipe_hamburguesa[1]);
    close(pipe_papas[1]);
    close(pipe_vegano[1]);
    close(pipe_pedido_hamburguesa[0]);
    close(pipe_pedido_papas[0]);
    close(pipe_pedido_vegano[0]);
    close(pipe_pedido_hamburguesa[1]);
    close(pipe_pedido_papas[1]);
    close(pipe_pedido_vegano[1]);
    close(pipe_clientes[0]);
    srand(getpid());
    int despacho;
    int cliente = 1;
    pedido p;
    //La condicion del while simula que el cliente se vaya si hay mucha fila
    while(rand()%10!=9){
	sleep(rand()%10);
	p.VIP = rand()%2;
	p.combo = rand()%3;
	printf("Llega cliente, VIP: %i, combo: %i.\n",p.VIP,p.combo);
	if(p.VIP==0)
	    write(pipe_pedido[1],&p,sizeof(pedido));
	else
	    write(pipe_pedidoVIP[1],&p,sizeof(pedido));
	write(pipe_clientes[1],&cliente,sizeof(char));
	if(p.combo == 0)
	    read(pipe_hamburguesa[0],&despacho,sizeof(int));
	else if(p.combo == 1)
	    read(pipe_vegano[0],&despacho,sizeof(int));
	else
	    read(pipe_papas[0],&despacho,sizeof(int));
	printf("Se va cliente, VIP: %i, combo: %i.\n",p.VIP,p.combo);
    } 
    printf("Un cliente se va porque hay mucha fila\n");
    close(pipe_pedido[1]);
    close(pipe_pedidoVIP[1]);
    close(pipe_clientes[1]);
    close(pipe_hamburguesa[0]);
    close(pipe_vegano[0]);
    close(pipe_papas[0]);
    exit(0);
}

int main(int argc, char **argv){
    //crear pipes
    if(pipe(pipe_pedido)==-1) return 1;
    if(pipe(pipe_hamburguesa)==-1) return 1;
    if(pipe(pipe_papas)==-1) return 1;
    if(pipe(pipe_vegano)==-1) return 1;
    if(pipe(pipe_pedido_hamburguesa)==-1) return 1;
    if(pipe(pipe_pedido_papas)==-1) return 1;
    if(pipe(pipe_pedido_vegano)==-1) return 1;
    if(pipe(pipe_pedidoVIP)==-1) return 1;
    if(pipe(pipe_clientes)==-1) return 1;
    
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
	if(fork()==0){
	    cliente();
	}
    }
    //espero que se vayan los clientes
    for(int i = 0; i<CANT_CLIENTES; i++)
	wait(NULL);
    //termino los procesos empleados
    for(int i = 0; i<5; i++)
	wait(NULL);//Falta matar a todos los hijos
    
    return 0;
}



