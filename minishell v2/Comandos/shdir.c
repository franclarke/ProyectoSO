#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv) {
    const char *ruta = (argc > 1) ? argv[1] : "."; //si no tengo args uso el directorio actual

    DIR *directorio = opendir(ruta);
    if (directorio == NULL) {
        perror("Error al intentar abrir el directorio");
        exit(1);
    }
    struct dirent *entrada;
    while ((entrada = readdir(directorio)) != NULL) {
        printf("%s\n", entrada->d_name);
    }

    closedir(directorio);
    return 0;
}
