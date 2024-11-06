#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_FILENAME_SIZE 50

int main (int argc, char **argv) {
    if (argc != 3) {
        printf("Error: Cantidad de parametros invalida\n");
    } else {
        if (access(argv[1], F_OK) != 0) {
            printf("Error: El archivo no existe\n");
            exit(EXIT_FAILURE);
        }
        char *endptr;
        long codigo = strtol(argv[2], &endptr, 8);
        if (*endptr != '\0' || codigo < 0 || codigo > 0777) {
            printf("Error: Codigo de permisos invalido\n");
            exit(EXIT_FAILURE);
        }
        
        char archivo[MAX_FILENAME_SIZE];
        snprintf(archivo, MAX_FILENAME_SIZE, "./%s", argv[1]);

        if (chmod(archivo, codigo) != 0) {
            perror("Error al modificar los permisos");
            exit(EXIT_FAILURE);
        }
        printf("Permisos cambiados correctamente\n");
    }
    return 0;
}
