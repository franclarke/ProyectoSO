#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Tiempos en segundos de cada operación
#define TIEMPO_RUEDA 2
#define TIEMPO_CHASIS 5
#define TIEMPO_MOTOR 7
#define TIEMPO_PINTAR 3
#define TIEMPO_EXTRA 2

// Definición de semáforos
sem_t inicio, ruedas_completas, chasis_completo, motor_completo, pintura_completa;
pthread_mutex_t mutex_extra;  // Mutex para controlar el equipamiento extra

// Contador para las ruedas y para el equipamiento extra
int ruedas_armadas = 0;
int motos_pintadas = 0;

// Función para armar ruedas
void* armar_rueda(void* args) {
    while (1) {
        sem_wait(&inicio); // Espera para iniciar el proceso de armado de ruedas
        printf("Agregando rueda...\n");
        sleep(TIEMPO_RUEDA);
        printf("Rueda agregada.\n");
        
        // Incrementa el contador de ruedas y verifica si ya hay dos ruedas armadas
        ruedas_armadas++;
        if (ruedas_armadas == 2) {
            ruedas_armadas = 0;  // Resetea el contador de ruedas
            sem_post(&ruedas_completas);  // Señala que las ruedas están completas
        }
    }
}

// Función para armar el chasis
void* armar_chasis(void* args) {
    while (1) {
        sem_wait(&ruedas_completas); // Espera a que ambas ruedas estén listas
        printf("Armando chasis...\n");
        sleep(TIEMPO_CHASIS);
        printf("Chasis armado.\n");
        sem_post(&chasis_completo); // Señala que el chasis está completo
    }
}

// Función para armar el motor
void* armar_motor(void* args) {
    while (1) {
        sem_wait(&chasis_completo); // Espera a que el chasis esté listo
        printf("Agregando motor...\n");
        sleep(TIEMPO_MOTOR);
        printf("Motor agregado.\n");
        sem_post(&motor_completo); // Señala que el motor está completo
    }
}

// Función para pintar la moto de un color específico
void* pintar(void* args) {
    char* color = (char*)args;
    while (1) {
        sem_wait(&motor_completo); // Espera a que el motor esté completo
        printf("Pintando moto de %s...\n", color);
        sleep(TIEMPO_PINTAR);
        printf("Moto pintada de %s.\n", color);

        pthread_mutex_lock(&mutex_extra);  // Controla el acceso a la verificación de equipamiento extra
        motos_pintadas++;
        if (motos_pintadas % 2 == 0) {
            sem_post(&pintura_completa); // Señala que la moto debe recibir equipamiento extra
        } else {
            sem_post(&inicio); // Señala que la moto está lista y no requiere equipamiento extra
            sem_post(&inicio);
        }
        pthread_mutex_unlock(&mutex_extra);
    }
}

// Función para agregar equipamiento extra a una de cada dos motos
void* armar_extra(void* args) {
    while (1) {
        sem_wait(&pintura_completa); // Espera a que una moto necesite equipamiento extra
        printf("Agregando equipamiento extra...\n");
        sleep(TIEMPO_EXTRA);
        printf("Equipamiento extra colocado.\n");
        
        sem_post(&inicio); // Reinicia el flujo para la próxima moto
        sem_post(&inicio);
    }
}

int main(int argc, char **argv) {
    pthread_t operario_ruedas, operario_chasis, operario_motor, pintor_verde, pintor_rojo, operario_extra;
    char* colores[] = {"verde", "rojo"};
    
    // Inicialización de semáforos y mutex
    sem_init(&inicio, 0, 2);
    sem_init(&ruedas_completas, 0, 0);
    sem_init(&chasis_completo, 0, 0);
    sem_init(&motor_completo, 0, 0);
    sem_init(&pintura_completa, 0, 0);
    pthread_mutex_init(&mutex_extra, NULL);

    // Creación de hilos para cada operario
    pthread_create(&operario_ruedas, NULL, armar_rueda, NULL);
    pthread_create(&operario_chasis, NULL, armar_chasis, NULL);
    pthread_create(&operario_motor, NULL, armar_motor, NULL);
    pthread_create(&pintor_verde, NULL, pintar, (void*)colores[0]);
    pthread_create(&pintor_rojo, NULL, pintar, (void*)colores[1]);
    pthread_create(&operario_extra, NULL, armar_extra, NULL);

    // Espera a que los hilos terminen (en este caso, no terminarán, ya que es un proceso continuo)
    pthread_join(operario_ruedas, NULL);
    pthread_join(operario_chasis, NULL);
    pthread_join(operario_motor, NULL);
    pthread_join(pintor_verde, NULL);
    pthread_join(pintor_rojo, NULL);
    pthread_join(operario_extra, NULL);

    // Limpieza de recursos
    sem_destroy(&inicio);
    sem_destroy(&ruedas_completas);
    sem_destroy(&chasis_completo);
    sem_destroy(&motor_completo);
    sem_destroy(&pintura_completa);
    pthread_mutex_destroy(&mutex_extra);
    
    return 0;
}
