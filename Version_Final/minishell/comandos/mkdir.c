#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Cantidad de parámetros inválida\n");
    } else {
        if (mkdir(argv[1], S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0) {
            printf("Se creo el directorio de forma exitosa\n");
        } else {
            perror("Error al intentar crear el directorio");
            exit(1);
        }
    }
    return 0;
}
