#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>

/*
 * Procesos: empleado hamburguesas, empleado menu vegano, 2 empleados papas fritas, empleado recibe pedidos y despacha,
 * cliente espera a ser atendido en una cola o se marcha, cliente VIP tiene prioridad.
 * politicas tomadas: Existen 3 combos: hamburguesa, menu vegano y papas. Para optimizar el despache los productores de combos son quienes despachan el pedido, 
 * el cliente lo toma si este coincide con su numero de pedido. Alternativamente el despache podria pasar por las manos del empleado que recibe los pedidos, 
 * pero es un paso innecesario en nuestra implementación. Si hubiesen combos con combinaciones de comidas, entonces esta seria una opcion viable.
 * */

//definicion de key:----------------
#define KEY ((key_t)(1243))
//----------------------------------

//definicion de combos:-------------
#define HAMBURGUESA 1
#define VEGANO 2
#define PAPAS 3
//----------------------------------

//Tipo de mensaje de un pedido:-----
#define PEDIDO 4
//----------------------------------

//Tiempos de preparacion:-----------
#define TIEMPO_HAMBURGUESA 1
#define TIEMPO_MENU_VEGANO 1
#define TIEMPO_PAPAS 1
//----------------------------------

typedef struct{
	long tipo; 
    int VIP;     //0 = no VIP, 1 = VIP
    int combo;   //1 = Hamburguesa, 2 = Menu vegano, 3 = papas.
    int nro_pedido;
} mensaje;

void preparar_hamburguesa(){
    int qid = msgget(KEY,IPC_CREAT|0666);
    mensaje orden;
    while(1){
		msgrcv(qid,&orden,sizeof(mensaje)-sizeof(long),HAMBURGUESA,0);
		printf("Preparando hamburguesa..\n");
		sleep(TIEMPO_HAMBURGUESA);
		orden.tipo = orden.nro_pedido;
		printf("Hamburguesa lista.\n");
		msgsnd(qid,&orden,sizeof(mensaje)-sizeof(long),0);
    }
}

void preparar_menu_vegano(){
    int qid = msgget(KEY,IPC_CREAT|0666);
    mensaje orden;
    while(1){
		msgrcv(qid,&orden,sizeof(mensaje)-sizeof(long),VEGANO,0);
		printf("Preparando menu vegano..\n");
		sleep(TIEMPO_MENU_VEGANO);
		orden.tipo = orden.nro_pedido;
		printf("Menu vegano listo.\n");
		msgsnd(qid,&orden,sizeof(mensaje)-sizeof(long),0);
    }
}

void preparar_papas(){
    int qid = msgget(KEY,IPC_CREAT|0666);
    mensaje orden;
    while(1){
		msgrcv(qid,&orden,sizeof(mensaje)-sizeof(long),PAPAS,0);
		printf("Preparando papas..\n");
		sleep(TIEMPO_PAPAS);
		orden.tipo = orden.nro_pedido;
		printf("Papas listas.\n");
		msgsnd(qid,&orden,sizeof(mensaje)-sizeof(long),0);
    }
}

void recibir(){
	int qid = msgget(KEY,0666);
	mensaje pedido;
    while(1){
		//leo un pedido
		msgrcv(qid,&pedido,sizeof(mensaje)-sizeof(long),PEDIDO,0); //no se pq no recibe bien el msg
		//si es un pedido vip atiendo a todos los vips
		while(pedido.VIP==1){
			pedido.tipo = pedido.combo;
			msgsnd(qid,&pedido,sizeof(mensaje)-sizeof(long),0);
			msgrcv(qid,&pedido,sizeof(mensaje)-sizeof(long),PEDIDO,0);
		}
		//sino atiendo un no vip
		pedido.tipo = pedido.combo;
		msgsnd(qid,&pedido,sizeof(mensaje)-sizeof(long),0);
	}
}

void cliente(int VIP, int combo,int nro_pedido){
	int qid = msgget(KEY,0666);
	mensaje pedido;
	
	pedido.tipo = PEDIDO;
	pedido.VIP = VIP;
	pedido.combo = combo;
	pedido.nro_pedido = nro_pedido;
    printf("Llega cliente con numero de pedido: %i, VIP: %i, combo: %i.\n",nro_pedido,VIP,combo);
    msgsnd(qid,&pedido,sizeof(mensaje)-sizeof(long),0);
    printf("Cliente esperando..\n");
    msgrcv(qid,&pedido,sizeof(mensaje)-sizeof(long),nro_pedido,0);
    printf("Se va cliente con numero de pedido: %i, VIP: %i, combo: %i.\n",nro_pedido,VIP,combo);
}

int main(int argc, char **argv){
	
	srand(time(NULL));
	//Se crea la cola
    int qid = msgget(KEY,IPC_CREAT|0666);
    msgctl(qid,IPC_RMID,NULL);
    qid = msgget(KEY,IPC_CREAT|0666);//ESTO ESTA MAL PERO ES PARA ASEGURARSE DE QUE SE DESTRUYA LA COLA ANTES DE INICIAR
    //Se inicializa nro_pedido de esta forma para que los tipos de mensaje no coincidan
    int nro_pedido = PEDIDO+1;
    
    //Creacion de empleados
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
	
	//Creacion de clientes
	while(1){
		if(fork()==0){
			cliente(rand()%2,rand()%3+1,nro_pedido);
			exit(0);
		}
		nro_pedido++;
		sleep(rand()%3);
    }
    //waits faltan
    //Se destruye la cola
    msgctl(qid,IPC_RMID,NULL);
    
    return 0;
}




