#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#define TIEMPO_DESPERTAR 2
#define TIEMPO_CHASIS 5
#define TIEMPO_MOTOR 7
#define TIEMPO_PINTAR 3
#define TIEMPO_EXTRA 2

sem_t elfo_esperando,santa_despierto,reno_esperando;
pthread_mutex_t = PTHREAD_MUTEX_INITIALIZER;
int fila_elfos = 0;
int fila_renos = 0;

void* santa(void* args){
	while(1){
	sem_wait(&elfo_esperando);
	sem_wait(&elfo_esperando);
	sem_wait(&elfo_esperando);
	printf("Santa despierta.\n");
	sem_post(&santa_despierto);
	sem_post(&elfo_esperando);
	printf("Resolviendo dificultades de un elfo..");
	sleep(TIEMPO_DESPERTAR);
	printf("Elfo atendido.\n");
	//intenta atender otros elfos
	}
}
void* reno(void* args){
	while(1){
	sem_wait(&ruedas);
	sem_wait(&ruedas);
	printf("Armando chasis..\n");
	sleep(TIEMPO_CHASIS);
	printf("Chasis armado.\n");
	sem_post(&chasis);
	}
}
void* elfo(void* args){
	sem_wait(&fila_elfos);
	printf("Agregando motor..\n");
	sleep(TIEMPO_MOTOR);
	printf("Motor agregado.\n");

	sem_post(&elfo_esperando);
	sem_wait(&santa_despierto);
	printf("Elfo %i ha sido atendido",getpid());
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