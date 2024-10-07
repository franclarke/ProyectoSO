#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#define RENOS 9
#define ELFOS_NECESARIOS 3
#define TIEMPO_CHASIS 5
#define TIEMPO_MOTOR 7
#define TIEMPO_PINTAR 3
#define TIEMPO_EXTRA 2

sem_t trineo,santa_despierto,atendido,cant_elfos(2),cant_renos(8),max_elfos(3),max_renos(9);
pthread_mutex_t acceso_cant_renos= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acceso_cant_elfos= PTHREAD_MUTEX_INITIALIZER;

void* santa(void* args){
	while(1){
	sem_wait(&santa_despierto);
	printf("Santa despierta.\n");
	pthread_mutex_lock(%acceso_cant_renos);
	if(!try_wait(&cant_renos)){
		for(int i = 0; i<RENOS ; i++)
			sem_post(&trineo);
	}
	pthread_mutex_unlock(%acceso_cant_renos);
	pthread_mutex_lock(%acceso_cant_elfos);
	if(!try_wait(&cant_elfos)){
		for(int i = 0; i<ELFOS_NECESARIOS ; i++)
			sem_post(&atendido);
	}
	pthread_mutex_unlock(%acceso_cant_elfos);
	}
}
void* reno(void* args){
	sem_wait(max_renos);
	printf("Regreso un reno..");
	pthread_mutex_lock(%acceso_cant_renos);
	if(!try_wait(&cant_renos)){
		sem_post(&santa_despierto);
	}
	pthread_mutex_unlock(%acceso_cant_renos);
	sem_wait(&trineo);
	sem_post(max_renos);
}
void* elfo(void* args){
	sem_wait(max_elfos);
	printf("Hay un elfo esperando a que santa le ayude..");
	pthread_mutex_lock(%acceso_cant_elfos);
	if(!try_wait(&cant_elfos)){
		sem_post(&santa_despierto);
	}
	pthread_mutex_unlock(%acceso_cant_elfos);
	sem_post(&santa_despierto);
	sem_wait(&atendido); //Esperando a que santa lo termine de atender
	sem_post(max_elfos);
}

int main(int argc, char **argv){
	pthread_t operario_ruedas, operario_chasis,operario_motor,pintor_verde,pintor_rojo,operario_extra;
	char* colores[] = {"verde","rojo"};
	sem_init(&inicio,0,2);
	sem_init(&ruedas,0,0);
	sem_init(&chasis,0,0);
	sem_init(&motor,0,0);
	sem_init(&pintura,0,0);
	pthread_create(&operario_ruedas,NULL,armar_rueda,NULL);
	pthread_create(&operario_chasis,NULL,armar_chasis,NULL);
	pthread_create(&operario_motor,NULL,armar_motor,NULL);
	pthread_create(&pintor_verde,NULL,pintar,(void*)colores[0]);
	pthread_create(&pintor_rojo,NULL,pintar,(void*)colores[1]);
	pthread_create(&operario_extra,NULL,armar_extra,NULL);
	pthread_join(operario_ruedas,NULL);
	pthread_join(operario_chasis,NULL);
	pthread_join(operario_motor,NULL);
	pthread_join(pintor_verde,NULL);
	pthread_join(pintor_rojo,NULL);
	pthread_join(operario_extra,NULL);
	
	return 0;
}