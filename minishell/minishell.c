#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELIMITADORES " \t\r\n\a"
#define true 1
#define false 0

void ejecutarMetodo(char** argumentos){
	pid_t pid = vfork();

	if (pid == 0){
        char comando[32] = "./Comandos/";
        strcat(comando,argumentos[0]);
        if(execv(comando,argumentos) == -1)
			perror("Error al ejecutar el comando");
	}
	else if (pid < 0) {
		perror("Error al crear el proceso");
	}
	else {
	   	wait(NULL);
	}
}

int main () {
    int caracter;
    int i;
    int ejecutar = true;
    int leer;
    char entrada[32];
    char * argumentos[6];
    
    printf("MiniShell \n");
    
    while(ejecutar){
        i=0;
        printf("\033[0;32mMinishell:\033[0m");

        leer = true;

        //Leer la entrada del usuario
        while(leer){
            caracter = getchar();
            if(caracter=='\n'){
                entrada[i] = '\0';
                leer = false;
            }
            else{
                entrada[i] = caracter;
                i++;
            }
        }
        
        if(i==0){
           printf("Error: Linea en blanco\n");
        } else if (entrada[0]==' '){
            printf("Error: El primer caracter es un espacio\n");
        } else {            
            //Separar la entrada en los distintos argumentos
            i=0;
            char * argumento = (char*) strtok(entrada,DELIMITADORES);
            while(argumento != NULL && i<5) {
                argumentos[i]=argumento;
                i++;
                argumento = (char*) strtok(NULL,DELIMITADORES);
            }
            
            if(i>=5){
                printf("Error: Demasiados parametros\n");
            } else {
                argumentos[i]=NULL;
                if(strcmp(argumentos[0],"exit")==0)
                    ejecutar=false;
                else
                    ejecutarMetodo(argumentos);
            }
        }
    }

    return 0;
}
