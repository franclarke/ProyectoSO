#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

int main (int argc, char ** argv) {
    if (argc != 3){
    printf("Error: Cantidad de parametros invalida\n");   
    } else {
        if(access(argv[1], F_OK) != 0){
            printf("Error: El archivo no existe\n");
            exit(EXIT_FAILURE);
        }
        char * texto;
        long codigo = strtol(argv[2], &texto,8);
        if(strcmp(texto,"")!=0){
            printf("Error: codigo de permisos invalido\n");
            exit(EXIT_FAILURE);
        }
        if(codigo>=0 && codigo<=511){
            char archivo[50] = "./";
            strcat(archivo,argv[1]);
            if(chmod(archivo,codigo)!=0){
                perror("Error al modificar los permisos\n");
                exit(EXIT_FAILURE);
            }
            printf("Permisos cambiados correctamente\n");
            exit(EXIT_SUCCESS);
        } else {
            printf("Error: codigo de permisos invalido\n");
            exit(EXIT_FAILURE);
        }
    }
}

