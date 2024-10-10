#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

// Función que maneja la comunicación con cada cliente
void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Recibe y envía mensajes al cliente hasta que se desconecte
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Asegura que el mensaje esté correctamente terminado
        printf("Mensaje recibido del cliente: %s\n", buffer);
        send(sock, buffer, bytes_received, 0);  // Envía el mensaje de vuelta al cliente
    }

    // Cierra el socket del cliente cuando termina la conexión
    close(sock);
    printf("Cliente desconectado\n");
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
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len)) < 0) {
            perror("Error en accept");
            exit(EXIT_FAILURE);
        }

        printf("Nuevo cliente conectado\n");

        // Crear un nuevo hilo para manejar al cliente
        if (pthread_create(&thread_id, NULL, handle_client, (void*)&client_socket) != 0) {
            perror("Error al crear el hilo");
        }
    }

    // Cerrar el socket del servidor
    close(server_socket);
    return 0;
}
