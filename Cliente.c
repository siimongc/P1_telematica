// cliente.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    char nombre[50];
    int bytes_received;

    // Crear el socket del cliente
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

    printf("Conectado al servidor.\n");

    // Enviar el nombre al servidor
    recv(sock, buffer, BUFFER_SIZE, 0);  // Recibir la solicitud de nombre
    printf("%s", buffer);
    fgets(nombre, 50, stdin);
    send(sock, nombre, strlen(nombre), 0);

    // Comunicación cliente-servidor
    while (1) {
        printf("Escriba un mensaje: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(sock, message, strlen(message), 0);

        // Recibir respuesta del servidor
        if ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) <= 0) {
            printf("Servidor desconectado o error al recibir\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    }

    // Cerrar el socket
    close(sock);
    printf("Conexión cerrada.\n");

    return 0;
}
