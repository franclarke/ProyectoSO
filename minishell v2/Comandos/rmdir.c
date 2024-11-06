#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Cantidad de parámetros inválida\n");
    } else {
        if (rmdir(argv[1]) == 0) {
            printf("Se elimino el directorio exitosamente\n");
        } else {
            perror("Error al intentar eliminar el directorio");
            exit(1);
        }
    }
    return 0;
}
