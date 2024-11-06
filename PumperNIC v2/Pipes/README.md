# Documentación Técnica de Código de `pumperNIC.c` - Proyecto Pumper Nic

## Propósito del Proyecto
El objetivo de este proyecto es simular la gestión de pedidos en una cadena de comida rápida, con una implementación eficiente y orientada a la priorización de clientes. La base de código está diseñada para coordinar el flujo de pedidos, preparación y entrega de diferentes tipos de combos mediante el uso de procesos y pipes en C, logrando una comunicación efectiva y priorización de clientes VIP en un ambiente simulado de alta concurrencia.

## Componentes Principales
El sistema se divide en tres roles principales: clientes, recibidor de pedidos y productores. A continuación, se describen los componentes y sus responsabilidades clave:

1. **Clientes**:
   - **Función**: Los clientes realizan pedidos de diferentes combos y esperan hasta que su pedido esté listo.
   - **Interacción**: Los clientes envían su pedido a través de pipes y esperan recibir el combo desde los pipes de productos. Un cliente puede ser VIP o no, y esta diferencia afecta el orden de atención.
   - **Simulación de Decisión de Espera**: Aproximadamente 1 de cada 10 clientes simula abandonar el local si hay mucha fila, regresando más tarde para intentar ser atendido.

2. **Recibidor de Pedidos**:
   - **Función**: Actúa como intermediario, gestionando los pedidos entrantes, priorizando a los clientes VIP y enviando órdenes a los productores.
   - **Interacción**: Verifica primero si hay pedidos VIP mediante un pipe no bloqueante. Si no hay VIP, atiende a clientes no VIP. Luego, envía una señal de preparación al productor correspondiente (hamburguesa, menú vegano o papas).
   - **Optimización de Flujo**: El recibidor no despacha los pedidos a los clientes, sino que los clientes los toman directamente del productor, reduciendo un paso en el flujo de trabajo y aumentando la eficiencia.

3. **Productores**:
   - **Función**: Cada productor prepara uno de los tres tipos de combos (hamburguesas, menú vegano o papas fritas) y deposita el producto en un pipe específico para que los clientes lo retiren.
   - **Interacción**: Los productores reciben una señal del recibidor de pedidos para comenzar la preparación de su producto correspondiente. Una vez listo, el combo se coloca en el pipe designado.

## Políticas Seleccionadas
1. **Manejo de Fila de Clientes**: Se implementa un mecanismo de prioridad, donde los clientes VIP tienen preferencia sobre los clientes regulares.
2. **Simulación de Salida de Clientes**: Aproximadamente el 10% de los clientes simula abandonar el local si la fila es larga y regresan después de un tiempo aleatorio.
3. **Tipos de Combos y Clientes**:
   - Tres tipos de combos (hamburguesa, menú vegano y papas fritas), identificados por los valores `0`, `1` y `2`, respectivamente.
   - Dos tipos de clientes: VIP (`1`) y no VIP (`0`).

## Implementación y Comunicación
La implementación utiliza un total de 9 pipes para la comunicación entre clientes, recibidor y productores, facilitando una arquitectura modular donde cada entidad tiene asignada su comunicación específica:

- **Pipes de Pedidos**: 
  - `pipe_pedidoNOVIP`: donde los clientes no VIP envían sus pedidos.
  - `pipe_pedidoVIP`: donde los clientes VIP envían sus pedidos.
  - `pipe_hay_clientes`: para notificar al recibidor sobre la llegada de un cliente.

- **Pipes de Órdenes**: 
  - `pipe_orden_hamburguesa`: comunica al productor de hamburguesas que debe preparar una.
  - `pipe_orden_vegano`: comunica al productor del menú vegano que debe preparar una orden.
  - `pipe_orden_papas`: comunica al productor de papas que debe comenzar su preparación.

- **Pipes de Productos Listos**:
  - `pipe_hamburguesa`: donde el productor de hamburguesas coloca los productos listos.
  - `pipe_vegano`: donde el productor de menú vegano deposita los pedidos terminados.
  - `pipe_papas`: donde el productor de papas coloca las papas fritas listas.

## Flujos de Interacción
1. **Cliente**: 
   - Envía su pedido al pipe de `pipe_pedidoNOVIP` o `pipe_pedidoVIP`, dependiendo de si es VIP o no.
   - Notifica al recibidor sobre su llegada mediante `pipe_hay_clientes`.
   - Una vez que su combo está listo, el cliente lo toma del pipe correspondiente (`pipe_hamburguesa`, `pipe_vegano` o `pipe_papas`).
  
2. **Recibidor de Pedidos**:
   - Escanea continuamente los pipes de `pipe_pedidoVIP` y `pipe_pedidoNOVIP` en modo no bloqueante.
   - Priorizando VIPs, envía la orden correspondiente al productor a través de los pipes `pipe_orden_hamburguesa`, `pipe_orden_vegano` o `pipe_orden_papas`.
   
3. **Productores**:
   - Reciben una señal de orden para preparar un producto específico y, tras completarlo, colocan el producto en el pipe correspondiente.


## Dificultades y Limitaciones Observadas
1. **Complejidad de Comunicación**: La cantidad de pipes aumenta la complejidad de la comunicación. Esto requiere un manejo cuidadoso para evitar condiciones de carrera y asegurar que los pedidos se procesen correctamente.
2. **Sin Garantía de Orden de Entrega**: La implementación no asegura que los clientes reciban sus combos en el mismo orden en que los solicitaron. Esto puede llevar a que un cliente regular tome el pedido de un VIP si está listo antes.
3. **Manejo Simplificado de Salida de Clientes**: El manejo de clientes que abandonan y regresan se simplificó mediante una probabilidad en lugar de comunicación entre procesos, para reducir la complejidad de la implementación.

## Tecnologías y Lenguajes de Programación Utilizados
Este proyecto está desarrollado en **C**, utilizando **pipes** para la comunicación inter-procesos y **procesos no bloqueantes** para la prioridad. La implementación se ejecuta en un entorno UNIX y está diseñada para optimizar el rendimiento en sistemas de múltiples núcleos.
