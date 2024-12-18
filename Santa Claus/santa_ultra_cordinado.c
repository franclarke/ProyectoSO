#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
#define RENOS 9
#define ELFOS_NECESARIOS 3
#define TIEMPO_RENOS 1
#define TIEMPO_ELFOS 1
 
sem_t trineo,santa_despierto,atendido,cant_elfos,cant_renos,max_elfos;
pthread_mutex_t acceso_cant_renos= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acceso_cant_elfos= PTHREAD_MUTEX_INITIALIZER;
 
void* santa(void* args){
	while(1){
		sem_wait(&santa_despierto);
		printf("Santa despierta\n");
		pthread_mutex_lock(&acceso_cant_renos);
		if(sem_trywait(&cant_renos)==-1){
			for(int i = 0; i<RENOS ; i++){
				printf("Se libero un reno..\n");
				sem_post(&trineo);
				sem_post(&cant_renos);
			}
		}
		else
			sem_post(&cant_renos);
		pthread_mutex_unlock(&acceso_cant_renos);
		pthread_mutex_lock(&acceso_cant_elfos);
		if(sem_trywait(&cant_elfos)==-1){
			for(int i = 0; i<ELFOS_NECESARIOS ; i++){
				printf("Se libero un elfo..\n");
				sem_post(&atendido);
				sem_post(&cant_elfos);
				sem_post(&max_elfos);
			}
		}
		else
			sem_post(&cant_elfos);
		pthread_mutex_unlock(&acceso_cant_elfos);	
	}
	
	return 0;
}
void* reno(void* args){
	while(1){
		pthread_mutex_lock(&acceso_cant_renos);
		sem_wait(&cant_renos);
		if(sem_trywait(&cant_renos)==-1){
			printf("Llego el ultimo reno.\n");
			sem_post(&santa_despierto);
		}
		else {
			sem_post(&cant_renos);
			printf("Hay un reno esperando..\n");
		}
		pthread_mutex_unlock(&acceso_cant_renos);
		sem_wait(&trineo);
		sleep(TIEMPO_RENOS);
	}
	
	return 0;
}
void* elfo(void* args){
	while(1){
		sem_wait(&max_elfos);
		pthread_mutex_lock(&acceso_cant_elfos);
		sem_wait(&cant_elfos);
		if(sem_trywait(&cant_elfos)==-1){
			printf("Llego el ultimo elfo..\n");
			sem_post(&santa_despierto);
		}
		else{
			sem_post(&cant_elfos);
			printf("Hay un elfo esperando a que santa le ayude..\n");
		}
		pthread_mutex_unlock(&acceso_cant_elfos);
		sem_wait(&atendido);
		sleep(TIEMPO_ELFOS);
	}
	return 0;
}
 
 
int main(int argc, char **argv){
	pthread_t t_santa, t_renos[RENOS], t_elfos[9];
 
	//inicializacion de semaforos
	sem_init(&trineo,0,0);
	sem_init(&atendido,0,0);
	sem_init(&santa_despierto,0,0);
	sem_init(&max_elfos,0,ELFOS_NECESARIOS);
	sem_init(&cant_renos, 0, RENOS);
	sem_init(&cant_elfos, 0, ELFOS_NECESARIOS);
 
	//creacion de santa
	pthread_create(&t_santa,NULL,santa,NULL);

	//creacion de los renos
	for (int i = 0; i < RENOS; i++){
		pthread_create(&t_renos[i], NULL, reno, NULL);
	}

	//creacion de los elfos
	for (int i = 0; i < 4; i++){
		pthread_create(&t_elfos[i], NULL, elfo, NULL);
	}
 
	//espero a santa
	pthread_join(t_santa,NULL);

	//espero a los renos
	for (int i = 0; i < RENOS; i++){
		pthread_join(t_renos[i], NULL);
	}

	//espero a los elfos
	for (int i = 0; i < 4; i++){
		pthread_join(t_elfos[i], NULL);
	}

	return 0;
}

