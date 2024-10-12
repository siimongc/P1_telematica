// servidor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

// Estructura para manejar clientes
typedef struct {
    int socket;
    char nombre[50];
} Cliente;

Cliente clientes[MAX_CLIENTS];  // Almacena los clientes conectados
int num_clientes = 0;           // Cantidad actual de clientes

pthread_mutex_t clientes_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger el acceso a la lista de clientes

// Función para verificar si un nombre ya está en uso
int nombre_en_uso(const char *nombre) {
    for (int i = 0; i < num_clientes; i++) {
        if (strcmp(clientes[i].nombre, nombre) == 0) {
            return 1;  // El nombre ya está en uso
        }
    }
    return 0;  // El nombre no está en uso
}

// Función para manejar la comunicación con cada cliente
void *handle_client(void *client_socket) {
    int sock = *((int *)client_socket);
    free(client_socket);  // Liberar la memoria asignada
    char buffer[BUFFER_SIZE];
    char nombre[50];
    int bytes_received;

    // Pedir nombre al cliente
    send(sock, "Ingrese su nombre: ", strlen("Ingrese su nombre: "), 0);
    bytes_received = recv(sock, nombre, 50, 0);
    nombre[bytes_received - 1] = '\0';  // Eliminar el salto de línea final

    // Verificar si el nombre ya está en uso
    pthread_mutex_lock(&clientes_mutex);
    if (nombre_en_uso(nombre)) {
        send(sock, "Nombre ya en uso. Conexión rechazada.\n", strlen("Nombre ya en uso. Conexión rechazada.\n"), 0);
        close(sock);
        pthread_mutex_unlock(&clientes_mutex);
        pthread_exit(NULL);
    }

    // Añadir al cliente a la lista
    strcpy(clientes[num_clientes].nombre, nombre);
    clientes[num_clientes].socket = sock;
    num_clientes++;
    pthread_mutex_unlock(&clientes_mutex);

    sprintf(buffer, "Bienvenido al chat, %s!\n", nombre);
    send(sock, buffer, strlen(buffer), 0);

    // Recibe y envía mensajes al cliente hasta que se desconecte
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Asegura que el mensaje esté correctamente terminado
        printf("[%s]: %s\n", nombre, buffer);
        send(sock, buffer, bytes_received, 0);  // Envía el mensaje de vuelta al cliente (echo)
    }

    // Eliminar al cliente de la lista al desconectarse
    pthread_mutex_lock(&clientes_mutex);
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].socket == sock) {
            // Mover el último cliente a la posición actual
            clientes[i] = clientes[num_clientes - 1];
            num_clientes--;
            break;
        }
    }
    pthread_mutex_unlock(&clientes_mutex);

    close(sock);
    printf("Cliente %s desconectado\n", nombre);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket, addr_len;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;

    // Crear el socket del servidor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Definir las propiedades del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Vincular el socket a la dirección y puerto
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    // Poner el servidor en modo escucha para aceptar conexiones
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    // Bucle principal para aceptar conexiones
    while (1) {
        addr_len = sizeof(client_addr);
        // Aceptar la conexión de un cliente
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) < 0) {
            perror("Error en accept");
            exit(EXIT_FAILURE);
        }

        printf("Nuevo cliente conectado\n");

        // Almacenar el socket del cliente dinámicamente
        int *client_sock = malloc(sizeof(int));
        *client_sock = client_socket;

        // Crear un nuevo hilo para manejar al cliente
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_sock) != 0) {
            perror("Error al crear el hilo");
        }
    }

    // Cerrar el socket del servidor
    close(server_socket);
    return 0;
}

