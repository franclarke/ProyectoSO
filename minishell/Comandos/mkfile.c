#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char ** argv){
    if (argc != 2){
        printf("Error: Cantidad de parametros invalida\n");   
    } else {
        FILE *archivo;
        
        if(access(argv[1], F_OK) == 0){
            //El archivo ya existe
            printf("Error: El archivo ya existe\n");
            exit(EXIT_FAILURE);
        } else {
            //El archivo no existe
            archivo = fopen(argv[1],"w");   
            fclose(archivo);
            printf("El archivo se ha creado con exito\n");
        }
    }
}
