#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char ** argv){
    if (argc != 2){
        printf("Error: Cantidad de parametros invalida\n");   
    } else {
        if(mkdir(argv[1], S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0){
            printf("Se creo el directorio de forma exitosa\n");
            exit(EXIT_SUCCESS);
        } else {
            perror("Error al crear el directorio");
            exit(EXIT_FAILURE);
        }
    }
}
