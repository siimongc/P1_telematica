# Proyecto de Chat Cliente-Servidor en C

Este proyecto implementa una aplicación de *chat cliente-servidor* en lenguaje C, utilizando *sockets* e *hilos*. Los clientes pueden conectarse a un servidor y tener conversaciones privadas entre pares, con la capacidad de cambiar entre diferentes usuarios.

## Características del Proyecto

- *Comunicación privada entre pares*: Cada cliente puede seleccionar con quién desea hablar de la lista de usuarios conectados.
- *Lista dinámica de usuarios*: Los clientes pueden ver los usuarios conectados en tiempo real.
- *Cambio de chat*: Los clientes pueden salir de una conversación y regresar a la lista de usuarios para seleccionar otro destinatario.

## Requisitos

- *Compilador GCC*: Se necesita GCC para compilar el código.
- *Biblioteca pthread*: La comunicación entre clientes se maneja con hilos (pthread), por lo que es necesario contar con la biblioteca pthread.

## Estructura de Archivos

- servidor.c: Código fuente del servidor.
- cliente.c: Código fuente del cliente.
- Makefile: Archivo para compilar y ejecutar el proyecto.
- README.md: manual de usuario.

## Cómo Compilar y Ejecutar

El proyecto incluye un *Makefile* para facilitar la compilación y ejecución del código.

### 1. Compilar el proyecto

Para compilar tanto el servidor como el cliente, simplemente ejecuta el siguiente comando en la terminal:

bash
make


Esto ejecutará las reglas del Makefile y compilará los archivos, generando los binarios servidor y cliente.

### 2. Ejecutar el servidor
Para iniciar el servidor, utiliza el siguiente comando:

bash
make run-server


El servidor estará ahora a la espera de conexiones en el puerto 8080. Verás un mensaje en la terminal indicando que el servidor está escuchando:

bash
Servidor escuchando en el puerto 8080...

### 3. Ejecutar el cliente
Para iniciar un cliente, abre otra terminal y utiliza el siguiente comando:

bash
make run-client

El cliente se conectará al servidor en localhost (127.0.0.1) en el puerto 8080. Al iniciar, el cliente te pedirá que ingreses tu nombre de usuario.

Puedes iniciar múltiples clientes para simular varias conexiones, cada cliente en una terminal diferente.

### 4. Interacción entre clientes
Una vez que estés conectado, el cliente recibirá la lista de usuarios disponibles y se te pedirá que elijas con quién deseas chatear. Solo verás los mensajes que tú y la persona seleccionada intercambien.

Ejemplo de flujo del cliente:

bash
Introduce tu nombre de usuario: simon
Ver lista de usuarios conectados:

Usuarios conectados:
maria
juan

Escribe el nombre de la persona con la que quieres hablar (o presiona 'x' para salir):


Enviar mensajes: Los mensajes aparecerán con el formato "{Usuario}: mensaje" en ambas consolas (tanto para el remitente como para el destinatario).

Cambiar de conversación: Puedes presionar x en cualquier momento para salir del chat actual y regresar a la lista de usuarios.

Salir: Si el cliente cierra la conexión abruptamente (por ejemplo, usando Ctrl + C), el servidor detectará la desconexión e informará al servidor que se ha desconectado.

### 1. Limpiar el proyecto
Si deseas eliminar los archivos binarios generados (servidor y cliente), puedes ejecutar:

bash
make clean


### Ejemplo de Uso
Cliente 1 (simon):

bash
Introduce tu nombre de usuario: simon
Usuarios conectados:
maria
juan

Escribe el nombre de la persona con la que quieres hablar (o presiona 'x' para salir): majo

{majo}: Hola Simon, ¿cómo estás?
{Simon}: ¡Todo bien! ¿Y tú?


Cliente 2 (maria):
bash
Introduce tu nombre de usuario: maria
Usuarios conectados:
simon
juan

Escribe el nombre de la persona con la que quieres hablar (o presiona 'x' para salir): simon

{Simon}: ¡Todo bien! ¿Y tú?
{majo}: Hola Simon, ¿cómo estás?
