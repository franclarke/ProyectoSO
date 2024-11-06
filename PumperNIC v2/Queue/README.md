# PumperNICQueue - Sistema de Gestión de Pedidos para Restaurante

## Descripción General

### Propósito del Proyecto

Este proyecto implementa un sistema de gestión de pedidos para una cadena de comida rápida utilizando **colas de mensajes** para la comunicación entre procesos. Su objetivo es gestionar el flujo de clientes y sus pedidos, priorizando a clientes VIP y optimizando la preparación y entrega de diferentes combos (hamburguesas, menús veganos y papas fritas). La implementación está diseñada para simular la experiencia de un restaurante real, donde los clientes hacen pedidos, esperan en una fila y reciben sus órdenes de acuerdo al tipo de pedido y su prioridad.

### Componentes Principales y Tecnologías Utilizadas

1. **Cliente:** Representa a un cliente que hace un pedido y espera su turno en la fila.
2. **Recibidor:** Procesa los pedidos de los clientes y los asigna al productor adecuado, priorizando a clientes VIP.
3. **Productores:** Empleados especializados en preparar cada tipo de combo (hamburguesas, menú vegano o papas fritas).

- **Tecnologías y Lenguaje:** La implementación está desarrollada en C, utilizando **colas de mensajes** de `SysV IPC` para la comunicación entre procesos. Se emplea **pthreads** para reducir la sobrecarga de procesos en el sistema, lo que mejora la eficiencia y escalabilidad.

---

## Políticas de Implementación

### Identificación Única y Mantenimiento de Estado

Cada cliente tiene un identificador único (`ID_cliente`), que asegura que cada pedido esté relacionado de forma exclusiva con el cliente que lo realizó. Esta política de ID único permite a cada cliente recibir su propio pedido sin conflictos, manteniendo una experiencia de atención personalizada y evitando errores en la entrega de pedidos.

### Tipos de Mensajes y Cola de Fila

Los mensajes se organizan en categorías para controlar el flujo de pedidos y gestionar prioridades:

- **PEDIDO_VIP y PEDIDO_NOVIP:** Diferencian entre pedidos de clientes VIP y no VIP. El tipo de mensaje indica la prioridad del pedido y determina el orden de atención.
- **HAMBURGUESA, VEGANO, PAPAS:** Mensajes enviados a los productores de cada combo, especificando el tipo de pedido a preparar.
- **FILA:** Define la capacidad máxima de la fila. Si la cantidad de clientes en la fila alcanza el límite, otros clientes deberán esperar hasta que se libere un lugar. Este mecanismo simula una fila real, donde los clientes pueden decidir volver más tarde si está llena.

### Priorización y Optimización del Flujo

Los clientes VIP tienen prioridad en el sistema. Esto se logra utilizando colas de mensajes con un **tipo de mensaje negativo** para que el `Recibidor` siempre priorice los pedidos de VIPs cuando están disponibles. Esta implementación simplifica la gestión de prioridades sin la necesidad de señales adicionales.

---

## Flujo de Mensajes y Comunicación

1. **Clientes:** Al entrar en el sistema, el cliente solicita un mensaje de tipo `FILA` para verificar si puede unirse a la cola. Si hay un espacio disponible, procede a enviar su pedido al `Recibidor` como `PEDIDO_VIP` o `PEDIDO_NOVIP`, según corresponda. Luego espera una respuesta del productor con un mensaje de tipo igual a su `ID_cliente`, garantizando que reciba su propio pedido. Al salir, libera el espacio de fila con otro mensaje de tipo `FILA`.

2. **Recibidor:** Este proceso recibe los mensajes `PEDIDO_VIP` y `PEDIDO_NOVIP`. Si hay pedidos simultáneos, prioriza los pedidos VIP mediante el tipo de mensaje negativo. Luego reenvía el mensaje al productor correspondiente (hamburguesas, menú vegano o papas) basado en el combo solicitado.

3. **Productores:** Los productores esperan mensajes según el tipo de combo que producen (`HAMBURGUESA`, `VEGANO` o `PAPAS`). Una vez recibido el mensaje, preparan el pedido y envían una respuesta de tipo `ID_cliente` al cliente que hizo el pedido, asegurando que cada cliente recibe el pedido correcto.

---

## Ventajas de Colas de Mensajes sobre Pipes

El uso de colas de mensajes en lugar de pipes ofrece varias ventajas para este sistema:

- **Priorización Simplificada:** El manejo de prioridades es más directo y eficiente mediante un `message type` negativo para atender VIPs antes que a clientes regulares. Implementar este nivel de control en pipes requeriría un manejo más complejo.
  
- **Control de la Fila:** La fila se implementa mediante un tipo de mensaje específico (`FILA`) que permite limitar el flujo de clientes, algo más complicado de lograr con pipes.

- **Optimización en la Entrega de Pedidos:** Las colas permiten que los productores envíen directamente el pedido al cliente, ahorrando un paso intermedio. En un sistema con pipes, esto requeriría mecanismos adicionales para redirigir los pedidos correctamente.

- **Manejo Eficiente de Múltiples Tipos de Mensaje:** En una cola de mensajes, es fácil categorizar los pedidos y hacer que cada componente escuche solo el tipo relevante, reduciendo la complejidad en comparación con pipes.

---

## Observaciones y Mejoras de la Implementación

Esta implementación optimiza el flujo de trabajo de la cadena de comida rápida y asegura una experiencia eficiente y personalizada para cada cliente:

- **Entrega Directa de Pedidos al Cliente:** Cada cliente recibe el pedido exacto que solicitó sin intermediarios, simplificando la implementación y asegurando una entrega precisa.
  
- **Prioridad Integrada y Simplificada:** La prioridad VIP es fácil de implementar y gestionar, eliminando la necesidad de usar pipes dedicados exclusivamente para controlar prioridades.
  
- **Gestión de la Fila con Tokens:** La fila se implementa mediante un sistema de tokens, asignando un espacio al cliente y limitando el número de clientes simultáneos. Esto asegura una gestión ordenada del flujo de clientes y evita la saturación del sistema.

Este sistema es ideal para aplicaciones de atención al cliente en tiempo real donde es necesario un control estricto del flujo y la priorización de usuarios. Además, su diseño con colas de mensajes permite escalar el número de clientes o productores según la demanda, brindando flexibilidad en la administración del servicio.
