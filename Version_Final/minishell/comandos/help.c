#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *nombre;
    const char *descripcion;
} comando_ayuda;

comando_ayuda comandos[] = {
    {"mkdir", "'mkdir NOMBRE_DIRECTORIO': Crear un directorio si no existe\n"},
    {"rmdir", "'rmdir NOMBRE_DIRECTORIO': Remover un directorio si existe\n"},
    {"mkfile", "'mkfile NOMBRE_ARCHIVO': Crear un nuevo archivo si no existe\n"},
    {"shdir", "'shdir NOMBRE_DIRECTORIO': Listar el contenido del directorio\n"},
    {"shfile", "'shfile NOMBRE_ARCHIVO': Muestra el contenido de un archivo\n"},
    {"chmod", "'chmod NOMBRE_ARCHIVO MODO': Modifica los permisos de un archivo mediante el modo octal (000 al 777)\n"}
};

void imprimir_ayuda() {
    printf("Comandos: \n");
    for (int i = 0; i < sizeof(comandos) / sizeof(comando_ayuda); i++) {
        printf("%s\n", comandos[i].nombre);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        imprimir_ayuda();
    } else if (argc == 2) {
        int found = 0;
        for (int i = 0; i < sizeof(comandos) / sizeof(comando_ayuda) && !found; i++) {
            if (strcmp(argv[1], comandos[i].nombre) == 0) {
                printf("%s", comandos[i].descripcion);
                found = 1;
            }
        }
        if (!found) {
            printf("Comando no encontrado\n");
        }
    } else {
        printf("Cantidad de parámetros inválida\n");
    }
    return 0;
}
