# Problema de Sincronización de Santa Claus

## Descripción General
Este proyecto simula el problema de sincronización de "Santa Claus", en el cual Santa está durmiendo en su tienda en el Polo Norte y solo puede ser despertado por:
- Los nueve renos que regresan de sus vacaciones.
- Grupos de tres elfos que necesitan ayuda con problemas de producción de juguetes.

La solución utiliza hilos y semáforos para sincronizar las actividades de Santa, los renos y los elfos, garantizando el uso eficiente de los recursos y la prioridad de tareas. Santa prioriza atender a los renos sobre los elfos, ya que preparar el trineo es su responsabilidad principal.

## Resumen de la Implementación
Esta implementación utiliza seis semáforos y dos mutex:
- **Semáforos**:
  - `trineo`: Controla cuando los renos están listos para ser enganchados al trineo.
  - `atendido`: Permite que los elfos sean atendidos por Santa.
  - `santa_despierto`: Señala cuando Santa es despertado.
  - `max_elfos`: Limita la cantidad de elfos que pueden solicitar ayuda al mismo tiempo (máximo 3).
  - `cant_renos`: Controla el número de renos presentes.
  - `cant_elfos`: Controla el número de elfos esperando la ayuda de Santa.

- **Mutexes**:
  - `acceso_cant_renos`: Protege el contador de renos.
  - `acceso_cant_elfos`: Protege el contador de elfos.

### Valores Iniciales de los Semáforos
Los semáforos se inicializan de la siguiente manera:
- `trineo`: 0
- `atendido`: 0
- `santa_despierto`: 0
- `max_elfos`: 3
- `cant_renos`: 9
- `cant_elfos`: 3

## Componentes

### 1. Renos
- **Cantidad**: 9 renos.
- **Función**:
  Cada hilo de reno bloquea el mutex `acceso_cant_renos`, espera en el semáforo `cant_renos`, y verifica si es el último en llegar. Si es el último reno, despierta a Santa mediante el semáforo `santa_despierto`. En caso contrario, espera en el semáforo `trineo` para ser enganchado al trineo.
- **Comportamiento**:
  Una vez que los renos están enganchados al trineo, se van de vacaciones, regresando después de un tiempo determinado.

### 2. Elfos
- **Función**:
  Cada hilo de elfo espera en el semáforo `max_elfos` para asegurarse de que no más de tres elfos estén solicitando ayuda de Santa al mismo tiempo. Luego bloquea el mutex `acceso_cant_elfos` y espera en `cant_elfos` para controlar el número de elfos que necesitan asistencia. Si hay tres elfos esperando, se despierta a Santa a través de `santa_despierto`. Después de recibir ayuda, el semáforo `max_elfos` se restablece, permitiendo que el próximo grupo de elfos busque ayuda.
- **Comportamiento**:
  Cada elfo debe esperar en `atendido` para recibir la asistencia de Santa antes de volver a trabajar.

### 3. Santa Claus
- **Función**:
  Santa duerme hasta ser despertado por un reno o un elfo. Al despertar, verifica:
    - **Renos**: Si el último reno ha llegado, Santa los prioriza realizando nueve operaciones `signal` en ambos semáforos, `trineo` y `cant_renos`.
    - **Elfos**: Si hay tres elfos esperando, Santa los atiende realizando tres operaciones `signal` en `atendido` y `cant_elfos`.
- **Priorización**:
  Santa siempre prioriza atender a los renos sobre los elfos si ambos grupos están esperando su ayuda, asegurando que el trineo esté listo para la temporada navideña.

## Tecnologías Utilizadas
- **Lenguaje de Programación**: C
- **Librerías**: Se utilizan hilos POSIX (pthread) para la creación y gestión de hilos, y semáforos para la sincronización.

## Archivos del Proyecto
- `Santa_claus.c`: Contiene la implementación completa del problema de sincronización de Santa Claus, incluyendo la inicialización de semáforos, la creación de hilos y la lógica para gestionar las actividades de los renos, los elfos y Santa.

---

Este proyecto demuestra una sincronización compleja con un consumo mínimo de recursos, manteniendo una estructura de prioridad clara en un entorno multi-hilos.
