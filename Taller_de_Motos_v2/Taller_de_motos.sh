#!/bin/bash

# Nombre del archivo de salida
OUTPUT="Taller_de_motos"

# Archivo fuente
SRC_FILE="Taller_de_motos.c"

# Compilar el código
gcc -o $OUTPUT $SRC_FILE -pthread -Wall

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error: La compilación falló."
    exit 1
fi

# Ejecutar el programa compilado
./$OUTPUT