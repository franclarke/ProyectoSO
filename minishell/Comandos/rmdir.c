#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char ** argv){
    if (argc != 2){
        printf("Error: Cantidad de parametros invalida\n");   
    } else {
        if(rmdir(argv[1]) == 0){
            printf("Se removio el directorio de forma exitosa\n");
            exit(EXIT_SUCCESS);
        } else {
            perror("Error al remover el directorio");
            exit(EXIT_FAILURE);
        }
    }
}
