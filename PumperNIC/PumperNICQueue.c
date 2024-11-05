#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#include <wait.h>

/*
 * Procesos: empleado hamburguesas, empleado menu vegano, 2 empleados papas fritas, empleado recibe pedidos y despacha,
 * cliente espera a ser atendido en una cola o se marcha, cliente VIP tiene prioridad.
 * politicas tomadas: Existen 3 combos: hamburguesa, menu vegano y papas. Para optimizar el despache los productores de combos son quienes despachan el pedido, 
 * el cliente lo toma si este coincide con su ID. Alternativamente el despache podria pasar por las manos del empleado que recibe los pedidos, 
 * pero es un paso innecesario en nuestra implementación. Si hubiesen combos con combinaciones de comidas, entonces esta seria una opcion viable.
 * */

//Definicion de key:----------------
#define KEY ((key_t)(1243))
//----------------------------------

//Tiempos de preparacion:-----------
#define TIEMPO_HAMBURGUESA 1
#define TIEMPO_MENU_VEGANO 1
#define TIEMPO_PAPAS 2
//----------------------------------

//Tipos de mensaje:-------------
#define PEDIDO_VIP 1
#define PEDIDO_NOVIP 2
#define HAMBURGUESA 3
#define VEGANO 4
#define PAPAS 5
#define FILA 6
//----------------------------------

//ID_cliente inicial:---------------
#define ID_CLIENTE_INICIAL 7
//----------------------------------

#define CANT_CLIENTES 10
#define CAPACIDAD 8

typedef struct{
    long tipo;
    int vip;		//0 = no VIP, 1 = VIP
    int combo;		//0 = Hamburguesa, 1 = MenuVegano, 2 = Papas fritas
    int ID_cliente;	//Identificador unico para cada cliente
} mensaje;

void preparar_hamburguesa(){
    int qid = msgget(KEY,IPC_CREAT|0666);
    mensaje orden;
    while(1){
		msgrcv(qid,&orden,sizeof(mensaje)-sizeof(long),HAMBURGUESA,0);
		printf("Preparando hamburguesa..\n");
		sleep(TIEMPO_HAMBURGUESA);
		orden.tipo = orden.ID_cliente;
		printf("Hamburguesa lista.\n");
		msgsnd(qid,&orden,sizeof(mensaje)-sizeof(long),0);
    }
    exit(0);
}

void preparar_menu_vegano(){
    int qid = msgget(KEY,IPC_CREAT|0666);
    mensaje orden;
    while(1){
		msgrcv(qid,&orden,sizeof(mensaje)-sizeof(long),VEGANO,0);
		printf("Preparando menu vegano..\n");
		sleep(TIEMPO_MENU_VEGANO);
		orden.tipo = orden.ID_cliente;
		printf("Menu vegano listo.\n");
		msgsnd(qid,&orden,sizeof(mensaje)-sizeof(long),0);
    }
    
    exit(0);
}

void preparar_papas(){
    int qid = msgget(KEY,IPC_CREAT|0666);
    mensaje orden;
    while(1){
		msgrcv(qid,&orden,sizeof(mensaje)-sizeof(long),PAPAS,0);
		printf("Preparando papas..\n");
		sleep(TIEMPO_PAPAS);
		orden.tipo = orden.ID_cliente;
		printf("Papas listas.\n");
		msgsnd(qid,&orden,sizeof(mensaje)-sizeof(long),0);
    }
    
    exit(0);
}

void recibir(){
    int qid = msgget(KEY,0666);
    mensaje pedido;

    while(1){
		sleep(1);
		msgrcv(qid,&pedido,sizeof(mensaje)-sizeof(long),-2,0); //Al pasar un numero negativo como tipo, se establece una prioridad para el tipo mas chico hasta el valor absoluto del parametro. Prioridad VIPs.
		printf("Atiendo cliente con ID: %i, VIP: %i, combo: %i.\n",pedido.ID_cliente,pedido.vip,pedido.combo);
		pedido.tipo = pedido.combo+HAMBURGUESA; //Los tipos van desde HAMBURGUESA hasta PAPAS
		msgsnd(qid,&pedido,sizeof(mensaje)-sizeof(long),0);
    }
    
    exit(0);
}

void cliente(int ID_cliente){
    int qid = msgget(KEY,0666);
    srand(getpid());
    mensaje pedido;
    mensaje fila;

    while(1){
		sleep(rand()%10);
		msgrcv(qid,&fila,sizeof(mensaje)-sizeof(long),6,0);
		pedido.vip = rand()%2;
		pedido.combo = rand()%3;
		pedido.ID_cliente = ID_cliente;
		pedido.tipo = PEDIDO_NOVIP-pedido.vip; //El atributo vip determina el tipo de mensaje, 2-vip(1) = PEDIDO_VIP, 2-no vip(0) = PEDIDO_NOVIP
		printf("Llega cliente con ID: %i, VIP: %i, combo: %i.\n",ID_cliente,pedido.vip,pedido.combo);
		msgsnd(qid,&pedido,sizeof(mensaje)-sizeof(long),0);
		msgrcv(qid,&pedido,sizeof(mensaje)-sizeof(long),ID_cliente,0);
		printf("Se va cliente con ID: %i, VIP: %i, combo: %i.\n",pedido.ID_cliente,pedido.vip,pedido.combo);
		msgsnd(qid,&fila,sizeof(mensaje)-sizeof(long),0);
    }
    
    exit(0);
}

int main(int argc, char **argv){
    //Se destruye la cola si es que ya esta creada, esto debido a que PumperNIC nunca finaliza su ejecucion(flujo continuo de clientes), y la cola no se destruye al final.
    int qid = msgget(KEY,IPC_CREAT|0666);
    msgctl(qid,IPC_RMID,NULL);

    //Se crea la cola
    qid = msgget(KEY,IPC_CREAT|0666);
    
    //Se inicializa ID_cliente de esta forma para que los tipos de mensaje no coincidan
    int ID_cliente = ID_CLIENTE_INICIAL;
    mensaje fila_max;
    fila_max.tipo = 6;
    
    for(int i = 0; i<CAPACIDAD ;i++)
	msgsnd(qid,&fila_max,sizeof(mensaje)-sizeof(long),0);
    
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
    for(int i = 0; i<CANT_CLIENTES; i++){
	if(fork()==0)
	    cliente(ID_cliente);
	ID_cliente++;
    }
    
	//espero por los clientes
    for(int i = 0; i<CANT_CLIENTES; i++)
		wait(NULL);
	
	//espero por los empleados
    for(int i = 0; i<5; i++)
		wait(NULL);
    
    //Se destruye la cola
    msgctl(qid,IPC_RMID,NULL);
    
    return 0;
}




