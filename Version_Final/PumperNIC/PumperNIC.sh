#!/bin/bash

# Nombre del archivo de salida
OUTPUT="PumperNIC"

# Archivo fuente
SRC_FILE="PumperNIC.c"

# Compilar el código
gcc -o $OUTPUT $SRC_FILE -Wall

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error: La compilación falló."
    exit 1
fi

# Ejecutar el programa compilado
./$OUTPUT