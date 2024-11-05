#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
#define RENOS 9				//cantidad de renos
#define ELFOS 9				//cantidad de elfos
#define ELFOS_NECESARIOS 3	//elfos necesarios para que santa los atienda
#define TIEMPO_RENOS 1
#define TIEMPO_ELFOS 1
 
sem_t trineo,santa_despierto,atendido,cant_elfos,cant_renos,max_elfos;
pthread_mutex_t mutex_renos= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_elfos= PTHREAD_MUTEX_INITIALIZER;
 
void* santa(void* args){
	while(1){
		sem_wait(&santa_despierto);
		printf("Santa despierta\n");
		pthread_mutex_lock(&mutex_renos);
		if(sem_trywait(&cant_renos)==-1){
			printf("Trineo armado\n");
			for(int i = 0; i<RENOS ; i++){
				sem_post(&trineo);
				sem_post(&cant_renos);
			}
		}
		else
			sem_post(&cant_renos);
		pthread_mutex_unlock(&mutex_renos);
		pthread_mutex_lock(&mutex_elfos);
		if(sem_trywait(&cant_elfos)==-1){
			printf("Elfos atendidos\n");
			for(int i = 0; i<ELFOS_NECESARIOS ; i++){
				sem_post(&atendido);
				sem_post(&cant_elfos);
			}
		}
		else
			sem_post(&cant_elfos);
		pthread_mutex_unlock(&mutex_elfos);
	}
	
	pthread_exit(NULL);
}
void* reno(void* args){
	while(1){
		sleep(TIEMPO_RENOS);
		pthread_mutex_lock(&mutex_renos);
		sem_wait(&cant_renos);
		if(sem_trywait(&cant_renos)==-1){
			printf("Llego el ultimo reno.\n");
			sem_post(&santa_despierto);
		}
		else {
			sem_post(&cant_renos);
			printf("Hay un reno esperando..\n");
		}
		pthread_mutex_unlock(&mutex_renos);
		sem_wait(&trineo);
	}
	
	pthread_exit(NULL);
}
void* elfo(void* args){
	while(1){
		sleep(TIEMPO_ELFOS);
		sem_wait(&max_elfos);
		pthread_mutex_lock(&mutex_elfos);
		sem_wait(&cant_elfos);
		if(sem_trywait(&cant_elfos)==-1){
			printf("Llego el ultimo elfo..\n");
			sem_post(&santa_despierto);
		}
		else{
			sem_post(&cant_elfos);
			printf("Hay un elfo esperando a que santa le ayude..\n");
		}
		pthread_mutex_unlock(&mutex_elfos);
		sem_wait(&atendido);
		pthread_mutex_lock(&mutex_elfos);
		sem_wait(&cant_elfos);
		if(sem_trywait(&cant_elfos)==-1){
			printf("El ultimo elfo se retira contento..\n");
			for(int i = 0; i<ELFOS_NECESARIOS ; i++){
				sem_post(&cant_elfos);
				sem_post(&max_elfos);
			}
		}
		else{
			sem_post(&cant_elfos);
			printf("Un elfo se retira contento..\n");
		}
		pthread_mutex_unlock(&mutex_elfos);
	}
	
	pthread_exit(NULL);
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
	for (int i = 0; i < ELFOS; i++){
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

	//destruccion de semaforos y mutex
	sem_destroy(&trineo);
	sem_destroy(&atendido);
	sem_destroy(&santa_despierto);
	sem_destroy(&max_elfos);
	sem_destroy(&cant_renos);
	sem_destroy(&cant_elfos);

	pthread_mutex_destroy(&mutex_renos);
	pthread_mutex_destroy(&mutex_elfos);

	return 0;
}

