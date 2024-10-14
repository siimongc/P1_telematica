// cliente.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Función para recibir mensajes en un hilo separado
void *receive_messages(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);
    }

    pthread_exit(NULL);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char username[50];
    pthread_t receive_thread;

    // Crear socket del cliente
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convertir la dirección IP en formato de red
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Error al convertir la dirección IP");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }

    // Introducir nombre de usuario
    printf("Introduce tu nombre: ");
    fgets(username, 50, stdin);
    send(sock, username, strlen(username), 0);

    // Crear hilo para recibir mensajes
    pthread_create(&receive_thread, NULL, receive_messages, (void *)&sock);

    // Enviar mensajes al servidor
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        // Mostrar la opción de salir
        if (strcmp(buffer, "x\n") == 0) {
            printf("Has salido del chat.\n");
        }
    }

    close(sock);
    return 0;
}
