#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#define TIEMPO_RUEDA 2
#define TIEMPO_CHASIS 5
#define TIEMPO_MOTOR 7
#define TIEMPO_PINTAR 3
#define TIEMPO_EXTRA 2

sem_t inicio,ruedas,chasis,motor,pintura;

void* armar_rueda(void* args){
	while(1){
	sem_wait(&inicio);
	printf("Agregando rueda..\n");
	sleep(TIEMPO_RUEDA);
	printf("Rueda agregada..\n");
	sem_post(&ruedas);
	}
}
void* armar_chasis(void* args){
	while(1){
	sem_wait(&ruedas);
	sem_wait(&ruedas);
	printf("Armando chasis..\n");
	sleep(TIEMPO_CHASIS);
	printf("Chasis armado.\n");
	sem_post(&chasis);
	}
}
void* armar_motor(void* args){
	while(1){
	sem_wait(&chasis);
	printf("Agregando motor..\n");
	sleep(TIEMPO_MOTOR);
	printf("Motor agregado.\n");
	sem_post(&motor);
	}
}
void* pintar(void* args){
	char* color = (char*)args;
	while(1){
	sem_wait(&motor);
	printf("Pintando moto de %s..\n",color);
	sleep(TIEMPO_PINTAR);
	printf("Moto pintada de %s.\n",color);
	sem_post(&pintura);
	}
}
void* armar_extra(){
	while(1){
	sem_wait(&pintura);
	sem_post(&inicio);
	sem_post(&inicio);
	sem_wait(&pintura);
	printf("Agregando equipamiento extra..\n");
	sleep(TIEMPO_EXTRA);
	printf("Equipamiento extra colocado.\n");
	sem_post(&inicio);
	sem_post(&inicio);
	}
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
