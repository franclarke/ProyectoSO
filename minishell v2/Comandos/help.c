#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *command;
    const char *description;
} HelpCommand;

HelpCommand helpCommands[] = {
    {"mkdir", "'mkdir [nombre de directorio]': Crear un directorio si no existe\n"},
    {"rmdir", "'rmdir [nombre de directorio]': Remover un directorio si existe\n"},
    {"mkfile", "'mkfile [nombre de archivo]': Crear un nuevo archivo si no existe\n"},
    {"shdir", "'shdir': Listar el contenido del directorio actual\n"},
    {"shfile", "'shfile [nombre de archivo]': Muestra el contenido de un archivo\n"},
    {"chmod", "'chmod [nombre de archivo] [modo]': Modifica los permisos de un archivo\n[modo]: 111...777\npermisos de propietario - permisos de grupos - permisos de otros\n1: Permisos de ejecucion\n2: Permisos de escritura\n4: Permisos de lectura\nLa suma de 1-2-4 permite la combinacion de estos permisos para el grupo/propietario/otros\n"}
};

void printGeneralHelp() {
    printf("Comandos disponibles: \n");
    for (int i = 0; i < sizeof(helpCommands) / sizeof(HelpCommand); i++) {
        printf("'%s'\n", helpCommands[i].command);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printGeneralHelp();
    } else if (argc == 2) {
        int found = 0;
        for (int i = 0; i < sizeof(helpCommands) / sizeof(HelpCommand); i++) {
            if (strcmp(argv[1], helpCommands[i].command) == 0) {
                printf("%s", helpCommands[i].description);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Error: Comando no encontrado\n");
        }
    } else {
        printf("Error: Cantidad de parametros invalida\n");
    }
    return 0;
}
