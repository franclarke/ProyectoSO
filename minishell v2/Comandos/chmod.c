#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_FILENAME_SIZE 50

int main (int argc, char **argv) {
    if (argc != 3) {
        printf("Cantidad de parámetros inválida\n");
    } else {
        if (access(argv[1], F_OK) != 0) {
            printf("El archivo no existe\n");
            exit(1);
        }
        char *texto;
        long codigo = strtol(argv[2], &texto, 8);
        if (*texto != '\0' || codigo < 0 || codigo > 0777) {
            printf("Codigo de permisos invalido\n");
            exit(1);
        }
        
        char archivo[MAX_FILENAME_SIZE];
        snprintf(archivo, MAX_FILENAME_SIZE, "./%s", argv[1]);

        if (chmod(archivo, codigo) != 0) {
            perror("Error al modificar los permisos");
            exit(1);
        }
        printf("Permisos cambiados correctamente\n");
    }
    return 0;
}
