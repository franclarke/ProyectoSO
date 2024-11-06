#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: Cantidad de parametros invalida\n");
    } else {
        FILE *archivo;

        if (access(argv[1], F_OK) == 0) {
            printf("Error: El archivo ya existe\n");
            exit(EXIT_FAILURE);
        } else {
            archivo = fopen(argv[1], "w");
            if (archivo == NULL) {
                perror("Error al crear el archivo");
                exit(EXIT_FAILURE);
            }
            fclose(archivo);
            printf("El archivo se ha creado con exito\n");
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
