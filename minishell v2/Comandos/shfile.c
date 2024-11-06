#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Cantidad de parámetros inválida\n");
    } else {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("Error al intentar abrir el archivo");
            exit(1);
        }
        int c;
        while ((c = fgetc(file)) != EOF) {
            printf("%c", c);
        }
        fclose(file);
        exit(0);
    }
    return 0;
}
