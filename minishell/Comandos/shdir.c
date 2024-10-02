#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

int main (int argc, char ** argv){
    DIR * directorio = opendir(".");  //Se abre el directorio con un puntero a DIR y se comienza a leer en el directorio actual con opendir()
    
    if (directorio == NULL){
        printf("Error: No se pudo acceder al directorio");
        exit(EXIT_FAILURE);
    }
    
    struct dirent * entrada;       //Tiene informacion sobre el archivo actual que se esta obteniendo
    
    entrada = readdir(directorio); //Se comienza a leer el primer archivo
    
    while(entrada!=NULL){           //Se marca con NULL el fin del directorio
        if ( (strcmp(entrada->d_name, ".")!=0) && (strcmp(entrada->d_name, "..")!=0) )
            printf("%s\n",entrada->d_name);
        entrada = readdir(directorio);
    }
    
    closedir(directorio);
    
    exit(EXIT_SUCCESS);
}
