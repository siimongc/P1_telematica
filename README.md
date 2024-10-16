# Proyecto de Chat Cliente-Servidor en C

## Introducción
Este proyecto implementa un sistema de chat basado en un modelo cliente-servidor utilizando el lenguaje C. Los clientes pueden conectarse al servidor y seleccionar con quién desean comunicarse de una lista de usuarios conectados. El servidor maneja múltiples clientes simultáneamente mediante hilos (pthread), permitiendo una comunicación privada entre pares.

El objetivo principal del proyecto es permitir la interacción entre los clientes de manera segura y eficiente, con funcionalidades como el cambio de chat, desconexiones controladas, y gestión de múltiples conexiones.

## diseño y requisitos

- El servidor debe permitir la conexión de nuevos usuarios.
- Los clientes deben ser notificados al conectarse.
- Los clientes deben autenticarse proporcionando un nombre de usuario.
- El servidor debe crear una lista de los usuarios conectados. 
- El usuario debe poder observar la lista de usuarios conectados.
- El servidor debe permitir que varios clientes se conecten al mismo tiempo.
- Los usuarios deben poder seleccionar con quién chatear.
- El servidor debe poder direccionar los mensajes al usuario destino.
- Los usuarios deben poder enviar y recibir mensajes entre sí.
- El servidor debe manejar múltiples clientes de manera concurrente.
- El usuario debe tener una interfaz (modo texto) para enviar y recibir mensajes.
- El servidor debe manejar errores de conexión y desconexión.
- El cliente debe poder actualizar la lista de usuarios.
- El usuario debe poder cerrar un chat sin desconectarse del servidor.
- El cliente debe poder desconectarse del servidor.
- El servidor debe notificar a todos los usurarios cualquier cambio en los estados de conexión.

## Desarrollo

### Características del Proyecto

- *Comunicación privada entre pares*: Cada cliente puede seleccionar con quién desea hablar de la lista de usuarios conectados.
- *Lista dinámica de usuarios*: Los clientes pueden ver los usuarios conectados en tiempo real.
- *Cambio de chat*: Los clientes pueden salir de una conversación y regresar a la lista de usuarios para seleccionar otro destinatario.

### Requisitos para del sistema

- *Compilador GCC*: Se necesita GCC para compilar el código.
- *Biblioteca pthread*: La comunicación entre clientes se maneja con hilos (pthread), por lo que es necesario contar con la biblioteca pthread.

### Estructura de Archivos

- servidor.c: Código fuente del servidor.
- cliente.c: Código fuente del cliente.
- Makefile: Archivo para compilar y ejecutar el proyecto.
- README.md: manual de usuario.

### Cómo Compilar y Ejecutar

El proyecto incluye un *Makefile* para facilitar la compilación y ejecución del código.

#### 1. Compilar el proyecto

Para compilar tanto el servidor como el cliente, simplemente ejecuta el siguiente comando en la terminal:

```bash
make
```

Esto ejecutará las reglas del Makefile y compilará los archivos, generando los binarios servidor y cliente.

#### 2. Ejecutar el servidor
Para iniciar el servidor, utiliza el siguiente comando:

```bash
make run-server
```

El servidor estará ahora a la espera de conexiones en el puerto 8080. Verás un mensaje en la terminal indicando que el servidor está escuchando:

```bash
Servidor escuchando en el puerto 8080...
```

#### 3. Ejecutar el cliente
Para iniciar un cliente, abre otra terminal y utiliza el siguiente comando:

```bash
make run-client
```
El cliente se conectará al servidor en localhost (127.0.0.1) en el puerto 8080. Al iniciar, el cliente te pedirá que ingreses tu nombre de usuario.

Puedes iniciar múltiples clientes para simular varias conexiones, cada cliente en una terminal diferente.

#### 4. Interacción entre clientes
Una vez que estés conectado, el cliente recibirá la lista de usuarios disponibles y se te pedirá que elijas con quién deseas chatear. Solo verás los mensajes que tú y la persona seleccionada intercambien.

Ejemplo de flujo del cliente:

```bash
Introduce tu nombre de usuario: simon
Ver lista de usuarios conectados:

Usuarios conectados:
maria
juan

Escribe el nombre de la persona con la que quieres hablar (o presiona 'x' para salir):
```

Enviar mensajes: Los mensajes aparecerán con el formato "{Usuario}: mensaje" en ambas consolas (tanto para el remitente como para el destinatario).

Cambiar de conversación: Puedes presionar x en cualquier momento para salir del chat actual y regresar a la lista de usuarios.

Salir: Si el cliente cierra la conexión abruptamente (por ejemplo, usando Ctrl + C), el servidor detectará la desconexión e informará al servidor que se ha desconectado.

#### 1. Limpiar el proyecto
Si deseas eliminar los archivos binarios generados (servidor y cliente), puedes ejecutar:

```bash
make clean
```

#### Ejemplo de Uso
Cliente 1 (simon):

```bash
Introduce tu nombre de usuario: simon
Usuarios conectados:
maria
juan

Escribe el nombre de la persona con la que quieres hablar (o presiona 'x' para salir): majo

{majo}: Hola Simon, ¿cómo estás?
{Simon}: ¡Todo bien! ¿Y tú?
```

Cliente 2 (maria):
```bash
Introduce tu nombre de usuario: maria
Usuarios conectados:
simon
juan

Escribe el nombre de la persona con la que quieres hablar (o presiona 'x' para salir): simon

{Simon}: ¡Todo bien! ¿Y tú?
{majo}: Hola Simon, ¿cómo estás?
```

## 3. Aspectos Logrados y No Logrados

### Aspectos Logrados:
- Concurrencia en la gestión de múltiples clientes.
- Envío y recepción de mensajes entre usuarios.
- Uso de un protocolo TCP, adecuado para la aplicación.
- Interfaz de usuario basada en consola para el cliente.
- Implementación básica de un protocolo de chat tipo texto.

### Aspectos No Logrados o pendientes:
- Autenticación robusta y gestión de sesiones.
- Manejo exhaustivo de errores y desconexiones inesperadas.
- La interfaz en consola no es la más amigable o visualmente atractiva.
- Mejora en el control y terminación de hilos para evitar fugas de recursos.

## 4. conclusión

Este proyecto ha sido una experiencia muy interesante en cuanto a la implementación de un sistema de chat usando el modelo cliente-servidor. Lo que hicimos fue utilizar el lenguaje C junto con sockets TCP para permitir que varios clientes se conecten y puedan chatear entre si, todo esto gestionado desde un servidor, que al final involucramos el uso de una instancia de AWS como se nos fue indicado durante el desarrollo de la materia con el primer taller. El hecho de usar hilos (threads) para manejar a los clientes hace que la comunicación entre ellos sea mas dinámica y eficiente, lo que es un punto importante porque permite que todos puedan estar interactuando a la vez sin que el sistema se sobrecargue.

Sin embargo, también nos hemos encontrado con algunos detalles que se pueden mejorar, en este caso es que en la implementación actual no tenemos un sistema de autenticación, lo que significa que cualquier persona puede conectarse sin tener que verificar su identidad, y mas considerando que dentro de la instancia e AWS, definimos una "regla de entrada" con un custom TCP, donde dejamos todos los puertos abiertos (0.0.0.0/0) para recibir la conexión. Esto no es ideal si lo pensamos para un uso real.  

Sería interesante que complemento a este proyecto se plantee un sistema de autenticacion que permita, primero, generar mas seguridad a la hora de conectarse con el servidor, y segundo, que permita que en caso de una descon
exion repetina, se pueda reestablecer la conexion con el servidor, manteniendo el historial, que en este caso puede ser desde un historial de un chat a un historial, por ejemplo, de las personas con las que se ha establecido una conexion.

En conclusion, este proyecto cumple bien con su objetivo principal, que era lograr que varios clientes puedan conectarse a un servidor y comunicarse entre si, y es una muy buena base sobre la cual seguir trabajando y añadiendo mejoras que lo puedan hacer más completo y fácil de usar.

## 5. Referencias
- [Documentación de Sockets en C](https://www.geeksforgeeks.org/socket-programming-in-c-cpp/)
- [Guía de Pthreads](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Referencia de GitHub para Makefiles](https://github.com/maks/MKdocs)
- [AWS Academy](https://awsacademy.instructure.com/)
- [Python Module of the Week - TCP Sockets](https://pymotw.com/2/socket/tcp.html)
- [Build a TCP Server Client with Sockets in Python](https://www.scaler.com/topics/cyber-security/build-a-tcp-server-client-with-sockets-in-python/)

