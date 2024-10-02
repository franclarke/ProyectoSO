#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define true 1
#define false 0

int main (int argc, char ** argv) {
    if (argc != 2){
    printf("Error: Cantidad de parametros invalida\n");   
    } else {
       FILE *archivo;
       int caracter;
       int termine = false;
      
       archivo = fopen(argv[1],"r");
    
       if(archivo == NULL) {
          perror("Error al abrir el archivo");
          exit(EXIT_FAILURE);
       } while(!termine){
          caracter = fgetc(archivo);
          if( feof(archivo) )
             termine = true;
          else
          printf("%c", caracter);
       }
    
       fclose(archivo);
       exit(EXIT_SUCCESS);
    }
}
