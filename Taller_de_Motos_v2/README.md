# Taller de Motos

## Descripción General
El proyecto **"Taller de Motos"** simula el proceso de ensamblaje en una fábrica de motos mediante el uso de programación concurrente en C. La implementación utiliza hilos y semáforos para representar a los operarios en la cadena de ensamblaje, asegurando que cada tarea se realice en el orden correcto y cumpla con las restricciones de sincronización necesarias en un entorno de producción limitado. Cada paso, desde el ensamblaje de ruedas hasta la aplicación de pintura y el equipamiento opcional, se realiza en secuencia, representando un flujo de trabajo completo y cíclico en el taller.

## Propósito del Proyecto
El propósito principal es modelar un flujo de ensamblaje secuencial y eficiente, donde los operarios colaboren en la producción de una moto completa. Este sistema de simulación es útil para comprender la sincronización de procesos y el uso de semáforos en entornos de recursos limitados, permitiendo a los ingenieros analizar y optimizar los flujos de trabajo.

## Componentes Principales
El proyecto contiene los siguientes componentes clave:

### Módulos de Ensamblaje de Componentes
- **armar_rueda**: Representa el proceso de ensamblaje de las ruedas, ejecutado por el primer operario. Este módulo arma dos ruedas por cada moto y, tras completarlas, emite una señal que permite al siguiente operario iniciar el armado del chasis.
- **armar_chasis**: Ejecutado por el segundo operario, este módulo espera la señal de que ambas ruedas han sido ensambladas antes de proceder con el armado del chasis.
- **armar_motor**: Una vez que el chasis está listo, el tercer operario ensambla el motor, emitiendo una señal para los operarios de pintura.

### Módulo de Pintura
- **pintar**: Hay dos hilos de pintura, uno para cada color (verde y rojo). Ambos hilos compiten por la señal del motor completo y el primero que recibe la señal pinta la moto de su color asignado. Esto permite flexibilidad en el color final de la moto sin necesidad de sincronización adicional entre pintores.

### Módulo de Equipamiento Extra
- **armar_extra**: Este módulo aplica un equipamiento adicional a una de cada dos motos producidas. Tras recibir la señal de pintura completa dos veces, el operario de este módulo añade el equipamiento extra a la moto, si corresponde, y vuelve a habilitar el inicio para el ensamblaje de la siguiente moto.

## Sincronización y Flujos de Señalización
La sincronización del flujo de ensamblaje se logra mediante cinco semáforos y un mutex:

- **Semáforos**:
  - **inicio**: Controla el inicio del ensamblaje de ruedas para cada moto nueva. Se inicializa en 2, permitiendo que dos ruedas sean ensambladas antes de pasar al siguiente operario.
  - **ruedas_completas**: Sincroniza el armado del chasis, emitiendo una señal cuando ambas ruedas están ensambladas.
  - **chasis_completo**: Controla el flujo entre el ensamblaje del chasis y el ensamblaje del motor.
  - **motor_completo**: Permite a los operarios de pintura comenzar su tarea una vez que el motor esté instalado.
  - **pintura_completa**: Controla el paso hacia el ensamblaje opcional de equipamiento extra.

- **Mutex de Equipamiento Extra**:
  - **mutex_extra**: Asegura que solo una de cada dos motos recibe equipamiento extra mediante un contador protegido por mutex. Esto garantiza que el sistema mantenga el flujo secuencial sin condiciones de carrera.

## Tecnologías y Lenguaje de Programación
- **Lenguaje de Programación**: C, utilizando la biblioteca estándar para operaciones de concurrencia, incluyendo `pthread` para la creación de hilos y `semaphore.h` para la sincronización.
- **Control de concurrencia**: Se implementa mediante semáforos y mutexes, controlando el acceso a los recursos y manteniendo la secuencia de operaciones.

## Interacción entre Componentes
1. **Flujo de Ensamblaje**:
   - El operario de ruedas inicia el proceso ensamblando dos ruedas, habilitando al operario del chasis una vez completadas.
   - El operario del chasis espera a que las dos ruedas estén listas antes de ensamblar el chasis y señalizar al operario del motor.
   - Una vez que el motor está ensamblado, los pintores compiten por la señal para pintar la moto en el color disponible.

2. **Condición de Equipamiento Extra**:
   - La lógica de equipamiento extra asegura que solo una de cada dos motos reciba equipamiento adicional. Esta condición se controla mediante el semáforo `pintura_completa` y un mutex, permitiendo al operario de extra agregar equipamiento solo cuando la condición es verdadera.

## Optimización de Recursos
La implementación está optimizada para utilizar el mínimo de semáforos necesarios, y el módulo de equipamiento extra actúa como un sincronizador final. Esto permite reducir el número de recursos utilizados y mantener un flujo de trabajo ordenado, esencial en un entorno de producción donde el espacio y los recursos están limitados.

## Conclusión
La base de código de **"Taller de Motos"** es un modelo efectivo de flujo de ensamblaje secuencial con sincronización eficiente mediante hilos y semáforos. Esta implementación asegura que los recursos se usen de manera óptima y que las tareas se realicen en el orden adecuado, simulando un proceso de fabricación fluido y estructurado.
