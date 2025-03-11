#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file_contents(int client_socket, char *filename) {
    FILE *file = fopen(filename, "r");
    char buffer[BUFFER_SIZE];

    if (file == NULL) {
        strcpy(buffer, "ERROR: File not found.\n");
        send(client_socket, buffer, strlen(buffer), 0);
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }
    
    fclose(file);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char filename[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected!\n");

        // Receive filename from client
        memset(filename, 0, BUFFER_SIZE);
        recv(client_socket, filename, BUFFER_SIZE, 0);
        printf("Client requested file: %s\n", filename);

        // Send file contents to client
        send_file_contents(client_socket, filename);

        close(client_socket);
    }

    close(server_fd);
    return 0;
}

