#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELIMITADORES " \t\r\n\a"
#define MAX_ARGUMENTS 6
#define MAX_INPUT_SIZE 32
#define COMANDO_PREFIX "./Comandos/"

void ejecutarMetodo(char** argumentos) {
    pid_t pid = fork();  // Use fork instead of vfork for safety

    if (pid == 0) {
        char comando[MAX_INPUT_SIZE + sizeof(COMANDO_PREFIX)] = COMANDO_PREFIX;
        strncat(comando, argumentos[0], sizeof(comando) - strlen(COMANDO_PREFIX) - 1);
        if(execv(comando, argumentos) == -1) {
            perror("Error al ejecutar el comando");
            exit(EXIT_FAILURE);  // Prevent unintended continuation
        }
    } else if (pid < 0) {
        perror("Error al crear el proceso");
    } else {
        wait(NULL);
    }
}

int main () {
    int i;
    int ejecutar = 1;
    char entrada[MAX_INPUT_SIZE];
    char* argumentos[MAX_ARGUMENTS];

    printf("MiniShell \n");

    while(ejecutar) {
        printf("\033[0;32mMinishell:\033[0m ");
        if (fgets(entrada, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("Error al leer la entrada\n");
            continue;
        }

        // Remove newline character
        entrada[strcspn(entrada, "\n")] = 0;

        // Check for blank line or leading spaces
        if (strlen(entrada) == 0) {
            printf("Error: Linea en blanco\n");
            continue;
        } else if (entrada[0] == ' ') {
            printf("Error: El primer caracter es un espacio\n");
            continue;
        }

        // Tokenize input into arguments
        i = 0;
        char* argumento = strtok(entrada, DELIMITADORES);
        while(argumento != NULL && i < MAX_ARGUMENTS - 1) {
            argumentos[i] = argumento;
            i++;
            argumento = strtok(NULL, DELIMITADORES);
        }

        if (i >= MAX_ARGUMENTS) {
            printf("Error: Demasiados parametros\n");
        } else {
            argumentos[i] = NULL;
            if (strcmp(argumentos[0], "exit") == 0) {
                ejecutar = 0;
            } else {
                ejecutarMetodo(argumentos);
            }
        }
    }

    return 0;
}
