// servidor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>  // Para manejar señales

#define PORT 8081
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char username[50];
    char target_username[50];  // Usuario con quien desea chatear
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
        send(client->socket, "\n Escribe el nombre de la persona con la que quieres hablar (presiona 'r' refrescar la lista de usuarios, 'x' para salir o 'q' para desconectarte del servidor): \n", 161, 0);
	recv(client->socket, client->target_username, 50, 0);
        client->target_username[strcspn(client->target_username, "\n")] = '\0';  // Eliminar salto de línea
    
        // Comando para salir del chat
        if (strcmp(client->target_username, "r") == 0) {
	    continue;
        } else if (strcmp(client->target_username, "q") == 0) {
	    break;
	    printf("Cliente desconectado: %s\n", client->username);
        } else if (!user_exists(client->target_username)) {
            send(client->socket, "Ese usuario no existe. Inténtalo de nuevo.\n", 42, 0);
            continue;  // Volver a pedir el nombre
        }


	// Mensaje de conexión establecida
        char connection_message[BUFFER_SIZE];
        snprintf(connection_message, BUFFER_SIZE, "Se ha iniciado un chat con '%s'.\n",client->target_username);
	char connection_message_client[BUFFER_SIZE];
	snprintf(connection_message_client, BUFFER_SIZE, "'%s' ha iniciado un chat contigo.\nSi quieres responder en el chat copia '%s' (si el chat no está iniciado).",client->username, client->username);
        
        // Enviar el mensaje de conexión a ambos usuarios
        send(client->socket, connection_message, strlen(connection_message), 0);
        send_message_to_client(connection_message_client, client->target_username);

        // Iniciar el chat
        while (1) {
            bytes_received = recv(client->socket, buffer, BUFFER_SIZE, 0);

            if (bytes_received <= 0) {  // El cliente se ha desconectado o hay un error
		break;  // Salir del ciclo para cerrar el socket del cliente
            }

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

        // Si se sale del ciclo, terminamos con el cliente
	if (bytes_received <= 0) {  // El cliente se ha desconectado o hay un error
            // Mostrar solo una vez que el cliente se ha desconectado
            printf("Cliente desconectado: %s\n", client->username);
            break;  // Salir del ciclo para cerrar el socket del cliente
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

    // Manejar la señal SIGPIPE para evitar que el servidor se cierre cuando un cliente se desconecta abruptamente
    signal(SIGPIPE, SIG_IGN);

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
