# Proyecto: MiniShell

## Descripción General

**MiniShell** es un proyecto que consiste en el desarrollo de una mini interfaz de línea de comandos (shell) que permite a los usuarios ejecutar comandos básicos en un entorno de sistema operativo Unix/Linux. La implementación simula la funcionalidad de una shell simple y permite gestionar archivos, directorios, permisos y otros comandos de utilidad. Está diseñado para ayudar a los usuarios a comprender el funcionamiento interno de una shell a un nivel básico, incluyendo la creación y gestión de procesos, el control de errores y el manejo de parámetros de entrada.

Este proyecto se construyó en C, aprovechando bibliotecas estándar de Unix para operaciones de entrada/salida, manipulación de archivos, y creación y sincronización de procesos.

## Estructura del Proyecto

### Componentes Principales

El proyecto **MiniShell** se compone de los siguientes archivos y módulos principales, cada uno implementado como un comando independiente que se puede invocar desde la shell:

1. **minishell.c**: 
   - Actúa como el archivo principal y la interfaz de usuario del proyecto.
   - Implementa un bucle de entrada que espera los comandos del usuario, los descompone en argumentos, y luego llama a los comandos correspondientes.
   - Utiliza el sistema de llamadas `fork` y `execv` para ejecutar cada comando como un nuevo proceso hijo.

2. **chmod.c**:
   - Cambia los permisos de un archivo especificado utilizando la llamada al sistema `chmod`.
   - Verifica que el archivo exista y valida el código de permisos proporcionado en formato octal.
   
3. **help.c**:
   - Proporciona una lista de comandos disponibles en la shell y detalles sobre cómo utilizarlos.
   - Admite la opción de mostrar ayuda detallada para cada comando.

4. **mkdir.c**:
   - Permite crear nuevos directorios con permisos específicos de lectura, escritura y ejecución para el propietario, el grupo y otros usuarios.

5. **mkfile.c**:
   - Crea archivos nuevos en el sistema de archivos, asegurando que el archivo no exista previamente.
   
6. **rmdir.c**:
   - Elimina un directorio específico si está vacío.

7. **shdir.c**:
   - Lista el contenido del directorio actual, excluyendo las entradas especiales `.` y `..`.

8. **shfile.c**:
   - Muestra el contenido de un archivo de texto, leyendo carácter por carácter.

### Interacción entre Componentes

- **Ejecución de Comandos**: `minishell.c` actúa como controlador central, recibiendo entradas de usuario y llamando a las funciones correspondientes en otros archivos de comandos. Para cada comando ingresado, `minishell.c` crea un proceso hijo usando `fork` y ejecuta el comando con `execv`.
- **Control de Errores**: Cada módulo realiza validaciones individuales y maneja errores específicos, asegurando que los problemas de entrada y de sistema sean capturados y reportados adecuadamente al usuario.
- **Modularidad**: Cada comando se encuentra en un archivo fuente independiente, lo que permite modificaciones y mejoras en los comandos sin afectar a otros componentes.
