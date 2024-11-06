#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Cantidad de parámetros inválida\n");
    } else {
        FILE *file;

        if (access(argv[1], F_OK) == 0) {
            printf("El archivo ya existe\n");
        } else {
            file = fopen(argv[1], "w");
            if (file == NULL) {
                perror("Error al intentar crear el archivo");
                exit(1);
            }
            fclose(file);
            printf("El archivo fue creado exitosamente\n");
        }
    }
    return 0;
}
