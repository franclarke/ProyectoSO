#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

sem_t semA;
sem_t semB;
sem_t semC;

void* A(void* arg){

	for(int i = 0;i<8;i++){
        sem_wait(&semA);
        printf("A");
        sem_post(&semC);
	}
    return 0;
}


void* B(void* arg){
    for (int i = 0; i < 6; i++){
		sem_wait(&semB);
		printf("B");
		sem_post(&semA);
    }
    return 0;
    }


void* C(void* arg){
	for(int i = 0; i<2;i++){
		for(int i = 0; i<3 ; i++)
		{
			sem_wait(&semC);
			sem_post(&semB);
		}
	sem_wait(&semC);
    printf("C");
    sem_post(&semA);
	}
	return 0;
    } 
    
int main(int argc, char **argv){

	pthread_t tA,tB,tC;
    sem_init(&semA,0,1);
    sem_init(&semB,0,0);
    sem_init(&semC,0,0);

    pthread_create(&tA,NULL,A,NULL);
    pthread_create(&tB,NULL,B,NULL);
    pthread_create(&tC,NULL,C,NULL);

    pthread_join(tA,NULL);
    pthread_join(tB,NULL);
    pthread_join(tC,NULL);


    return 0;

}


