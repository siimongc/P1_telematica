# Nombre del compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -lpthread

# Archivos fuente
SERVER_SRC = Servidor.c
CLIENT_SRC = Cliente.c

# Archivos binarios
SERVER_BIN = servidor
CLIENT_BIN = cliente

# Objetivos
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compilar el servidor
$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# Compilar el cliente
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

# Ejecutar el servidor
run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

# Ejecutar el cliente
run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

# Limpiar los binarios
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

# Ayuda para ver las opciones
help:
	@echo "Opciones del Makefile:"
	@echo "  make            -> Compila tanto el servidor como el cliente"
	@echo "  make run-server -> Ejecuta el servidor"
	@echo "  make run-client -> Ejecuta el cliente"
	@echo "  make clean      -> Elimina los archivos binarios generados"
