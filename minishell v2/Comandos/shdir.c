#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv) {
    DIR *directorio = opendir(".");

    if (directorio == NULL) {
        perror("Error: No se pudo acceder al directorio");
        exit(EXIT_FAILURE);
    }

    struct dirent *entrada;
    while ((entrada = readdir(directorio)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0)
            printf("%s\n", entrada->d_name);
    }

    closedir(directorio);
    exit(EXIT_SUCCESS);
}
