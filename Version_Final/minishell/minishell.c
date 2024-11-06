#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELIMITADOR_TOKEN " \t\r\n\a"
#define MAX_ARGUMENTOS 6
#define MAX_INPUT_SIZE 32
#define COMANDO_PREF "./comandos/"

void ejecutarMetodo(char** argumentos) {
    pid_t pid = fork();

    if (pid == 0) {
        char comando[MAX_INPUT_SIZE + sizeof(COMANDO_PREF)] = COMANDO_PREF;
        strcat(comando,argumentos[0]);
        if(execv(comando, argumentos) == -1) {
            perror("Error al ejecutar el comando");
            exit(1);
        }
    } else if (pid < 0) {
        perror("Error al crear el proceso");
    } else {
        wait(NULL);
    }
}

int main () {
    int i, c;
    int ejecutar = 1;
    char entrada[MAX_INPUT_SIZE];
    char* argumentos[MAX_ARGUMENTOS];

    printf("MiniShell \n");

    while (ejecutar) {
        printf("Input: ");
        i = 0;
        
        while ((c = getchar()) != '\n' && i < MAX_INPUT_SIZE - 1) {
            entrada[i++] = c;
        }
        entrada[i] = '\0';

        // Verificación de entradas vacías o con espacios al principio.
        if (strlen(entrada) == 0) {
            printf("Linea en blanco\n");
        } else if (entrada[0] == ' ') {
            printf("El primer caracter es un espacio\n");
        } else {
            i = 0;  // Reiniciar el índice para la tokenización.
            char* argumento = strtok(entrada, DELIMITADOR_TOKEN);
            while (argumento != NULL && i < MAX_ARGUMENTOS - 1) {
                argumentos[i] = argumento;
                i++;
                argumento = strtok(NULL, DELIMITADOR_TOKEN);
            }

            if (i >= MAX_ARGUMENTOS) {
                printf("Demasiados parametros\n");
            } else {
                argumentos[i] = NULL;  // Asegurarse de que el último argumento sea NULL.
                if (strcmp(argumentos[0], "exit") == 0) {
                    ejecutar = 0;
                } else {
                    ejecutarMetodo(argumentos);
                }
            }
        }
    }

    return 0;
}
