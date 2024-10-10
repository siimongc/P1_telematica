# Proyecto Chat Cliente-Servidor en C

Este proyecto implementa una aplicación de **chat cliente-servidor** en lenguaje C, utilizando **sockets** y **hilos** (threads). La arquitectura está basada en un servidor que maneja múltiples clientes de manera concurrente.

## Estructura del Proyecto

- **Servidor**: El servidor es capaz de aceptar conexiones de múltiples clientes de forma simultánea y manejar la comunicación con cada uno de ellos en hilos separados.
- **Cliente**: El cliente se conecta al servidor, envía mensajes y recibe respuestas del servidor en un modelo de **echo**, es decir, el servidor envía de vuelta el mensaje recibido.

## Características

- **Modelo Cliente-Servidor**: El servidor gestiona las conexiones de varios clientes a través de hilos.
- **Concurrencia**: El servidor usa hilos (`pthread`) para manejar múltiples clientes conectados simultáneamente.
- **Comunicación bidireccional**: El cliente puede enviar mensajes al servidor, y el servidor responde con el mismo mensaje.
- **Uso de Sockets**: La comunicación entre cliente y servidor se realiza mediante la API de sockets de Berkeley (TCP/IP).

## Requisitos

- **Compilador GCC**: Asegúrate de tener instalado GCC.
- **Sistema operativo**: Funciona en sistemas tipo UNIX como Linux o macOS.

## Estructura de Archivos

- `servidor.c`: Código fuente del servidor.
- `cliente.c`: Código fuente del cliente.
- `Makefile`: Archivo para compilar y ejecutar el proyecto.
- `README.md`: Este archivo con la explicación del proyecto.

## Cómo Compilar y Ejecutar

El proyecto incluye un **Makefile** para facilitar la compilación y ejecución del código. A continuación, se detallan los pasos.

### 1. Compilar el proyecto

Para compilar tanto el servidor como el cliente, simplemente ejecuta:

```bash
make

Esto ejecutará las reglas del Makefile y compilará ambos archivos, generando los binarios servidor y cliente.

Esto ejecutará las reglas del Makefile y compilará ambos archivos, generando los binarios servidor y cliente.

2. Ejecutar el servidor
Para iniciar el servidor, utiliza el siguiente comando:

bash
Copiar código
make run-server
El servidor estará ahora a la espera de conexiones en el puerto 8080. Imprimirá un mensaje indicando que está escuchando en ese puerto:

bash
Copiar código
Servidor escuchando en el puerto 8080...
3. Ejecutar el cliente
Para iniciar el cliente, utiliza el siguiente comando:

bash
Copiar código
make run-client
El cliente intentará conectarse al servidor en localhost (IP 127.0.0.1) en el puerto 8080. Puedes ejecutar varios clientes simultáneamente para simular múltiples usuarios conectados al servidor.

4. Limpiar el proyecto
Si deseas eliminar los archivos binarios generados (servidor y cliente), puedes ejecutar:

bash
Copiar código
make clean
Este comando eliminará los archivos servidor y cliente, limpiando el directorio de compilaciones anteriores.

