#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char ** argv) {
    if (argc == 1) {
        printf("Comandos disponibles: \n");
        printf("'mkdir'\n");
        printf("'rmdir'\n");
        printf("'mkfile'\n");
        printf("'shdir'\n");
        printf("'shfile'\n");
        printf("'chmod'\n");
        exit(EXIT_SUCCESS);
    } else if (argc == 2){
        if(strcmp(argv[1],"mkdir") == 0){
            printf("'mkdir [nombre de directorio]': Crear un directorio si no existe\n");
        } else if(strcmp(argv[1],"rmdir") == 0){
            printf("'rmdir [nombre de directorio]': Remover un directorio si existe\n");
        } else if(strcmp(argv[1],"mkfile") == 0){
            printf("'mkfile [nombre de archivo]': Crear un nuevo archivo si no existe\n");
        } else if(strcmp(argv[1],"shdir") == 0){
            printf("'shdir': Listar el contenido del directorio actual\n");
        } else if(strcmp(argv[1],"shfile") == 0){
            printf("'shfile [nombre de archivo]': Muestra el contenido de un archivo\n");
        } else if(strcmp(argv[1],"chmod") == 0){
            printf("'chmod [nombre de archivo] [modo]': Modifica los permisos de un archivo\n");
            printf("[modo]: 111...777\n");
            printf("permisos de propietario - permisos de grupos - permisos de otros\n");
            printf("1: Permisos de ejecucion\n");
            printf("2: Permisos de escritura\n");
            printf("4: Permisos de lectura\n");
            printf("La suma de 1-2-4 permite la combinacion de estos permisos para el grupo/propietario/otros\n");
        }
        exit(EXIT_SUCCESS);
    } else {
        printf("Error-%s: Cantidad de parametros invalida\n", argv[1]); 
        exit(EXIT_FAILURE);
    }
}
