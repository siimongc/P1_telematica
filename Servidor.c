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

typedef struct {
    int socket;
    char username[50];
    char target_username[50];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Enviar lista de usuarios conectados a un cliente
void send_user_list(int client_socket) {
    char message[BUFFER_SIZE] = "Usuarios conectados:\n";

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        strcat(message, clients[i].username);
        strcat(message, "\n");
    }
    pthread_mutex_unlock(&clients_mutex);

    send(client_socket, message, strlen(message), 0);
}

// Validar si un usuario existe
int user_exists(const char *username) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        if (strcmp(clients[i].username, username) == 0) {
            pthread_mutex_unlock(&clients_mutex);
            return 1;  // Usuario existe
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return 0;  // Usuario no existe
}

// Función para enviar mensaje entre pares
void send_message_to_client(char *message, char *target_username) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        if (strcmp(clients[i].username, target_username) == 0) {
            send(clients[i].socket, message, strlen(message), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Manejar a cada cliente
void *handle_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        // Enviar lista de usuarios conectados
        send_user_list(client->socket);

        // Pedir al cliente que elija con quién chatear
        send(client->socket, "Escribe el nombre de la persona con la que quieres hablar (presiona 'x' para salir del chat en cualquier momento): ", 91, 0);
        recv(client->socket, client->target_username, 50, 0);
        client->target_username[strcspn(client->target_username, "\n")] = '\0';  // Eliminar salto de línea

        // Comando para salir del chat
        if (strcmp(client->target_username, "x") == 0) {
            continue;  // Volver a la lista de usuarios
        }

        // Validar si el nombre existe
        if (!user_exists(client->target_username)) {
            send(client->socket, "Ese usuario no existe. Inténtalo de nuevo.\n", 42, 0);
            continue;  // Volver a pedir el nombre
        }

        // Iniciar el chat
        while (1) {
            bytes_received = recv(client->socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) break;  // El cliente se ha desconectado

            buffer[bytes_received] = '\0';

            // Comando para salir del chat actual
            if (strcmp(buffer, "x\n") == 0) {
                send(client->socket, "Has salido del chat.\n", 22, 0);
                break;  // Salir del chat y volver a la lista
            }

            // Formatear el mensaje con {usuario}: mensaje
            char formatted_message[BUFFER_SIZE];
            snprintf(formatted_message, BUFFER_SIZE, "{%s}: %s", client->username, buffer);

            // Enviar mensaje al destinatario
            send_message_to_client(formatted_message, client->target_username);
        }
    }

    // Desconectar al cliente
    close(client->socket);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        if (clients[i].socket == client->socket) {
            for (int j = i; j < client_count - 1; ++j) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    // Notificar a los clientes restantes que un usuario se ha desconectado
    send_user_list(client->socket);

    free(client);
    pthread_exit(NULL);
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid;
    socklen_t addr_size = sizeof(client_addr);

    // Crear socket del servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    while (1) {
        new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

        Client *client = (Client *)malloc(sizeof(Client));
        client->socket = new_socket;
        recv(new_socket, client->username, 50, 0);
        client->username[strcspn(client->username, "\n")] = '\0';  // Eliminar salto de línea

        pthread_mutex_lock(&clients_mutex);
        clients[client_count++] = *client;
        pthread_mutex_unlock(&clients_mutex);

        printf("Nuevo cliente conectado: %s\n", client->username);

        pthread_create(&tid, NULL, handle_client, (void *)client);
    }

    close(server_socket);
    return 0;
}
