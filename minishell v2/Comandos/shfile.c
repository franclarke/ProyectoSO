#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: Cantidad de parametros invalida\n");
    } else {
        FILE *archivo = fopen(argv[1], "r");

        if (archivo == NULL) {
            perror("Error al abrir el archivo");
            exit(EXIT_FAILURE);
        }

        int caracter;
        while ((caracter = fgetc(archivo)) != EOF) {
            putchar(caracter);
        }

        fclose(archivo);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
