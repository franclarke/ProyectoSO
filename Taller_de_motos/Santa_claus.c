#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
 
#define RENOS 9
#define ELFOS_NECESARIOS 3
 
sem_t trineo,santa_despierto,atendido,cant_elfos,cant_renos,max_elfos,max_renos;
pthread_mutex_t acceso_cant_renos= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acceso_cant_elfos= PTHREAD_MUTEX_INITIALIZER;
 
void* santa(void* args){
	while(1){
	sem_wait(&santa_despierto);
	pthread_mutex_lock(&acceso_cant_renos);
	if(sem_trywait(&cant_renos)==-1){
		for(int i = 0; i<RENOS ; i++)
			sem_post(&trineo);
		for(int i = 0; i<RENOS; i++)
			sem_post(&cant_renos);
	}
	else 
		sem_post(&cant_renos);
	pthread_mutex_unlock(&acceso_cant_renos);
	pthread_mutex_lock(&acceso_cant_elfos);
	if(sem_trywait(&cant_elfos)==-1){
		for(int i = 0; i<ELFOS_NECESARIOS ; i++)
			sem_post(&atendido);
		for(int i = 0; i<ELFOS_NECESARIOS ; i++)
			sem_post(&cant_elfos);
	}
	else 
		sem_post(&cant_elfos);
	pthread_mutex_unlock(&acceso_cant_elfos);
	}
}
void* reno(void* args){
	sem_wait(&max_renos);
	printf("Regreso un reno..\n");
	pthread_mutex_lock(&acceso_cant_renos);
	if(sem_trywait(&cant_renos)==-1){
		sem_post(&santa_despierto);
		printf("Los renos despertaron a santa\n");
	}
	pthread_mutex_unlock(&acceso_cant_renos);
	sem_wait(&trineo);
	printf("Se libero un reno..\n");
	sem_post(&max_renos);
	return 0;
}
void* elfo(void* args){
	sem_wait(&max_elfos);
	printf("Hay un elfo esperando a que santa le ayude..\n");
	pthread_mutex_lock(&acceso_cant_elfos);
	if(sem_trywait(&cant_elfos)==-1){
		sem_post(&santa_despierto);
		printf("Los elfos despertaron a santa\n");
	}
	pthread_mutex_unlock(&acceso_cant_elfos);
	sem_wait(&atendido);
	printf("Se libero un elfo..\n");
	sem_post(&max_elfos);
	return 0;
}
 
 
int main(int argc, char **argv){
	pthread_t t_santa, t_renos[RENOS], t_elfos[9];
 
	sem_init(&trineo,0,0);
	sem_init(&atendido,0,0);
	sem_init(&santa_despierto,0,0);
	sem_init(&max_elfos,0,ELFOS_NECESARIOS);
	sem_init(&max_renos,0,RENOS);
	sem_init(&cant_renos, 0, RENOS-1);
	sem_init(&cant_elfos, 0, ELFOS_NECESARIOS-1);
 
	pthread_create(&t_santa,NULL,santa,NULL);
	for (int i = 0; i < RENOS; i++){
		pthread_create(&t_renos[i], NULL, reno, NULL);
	}
	for (int i = 0; i < 9; i++){
		pthread_create(&t_elfos[i], NULL, elfo, NULL);
	}
 
	pthread_join(t_santa,NULL);
	for (int i = 0; i < RENOS; i++){
		pthread_join(t_renos[i], NULL);
	}
	for (int i = 0; i < 5; i++){
		pthread_join(t_elfos[i], NULL);
	}
 
 
	return 0;
}
