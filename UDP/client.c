#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define SERVER_IP "127.0.0.1" // Change this if server runs on another machine
#define BUFFER_SIZE 1024
int main() {
int client_sock;
struct sockaddr_in server_addr;
socklen_t addr_len = sizeof(server_addr);
char buffer[BUFFER_SIZE] = "Hello, server!"; // Initial message to register client
// Create UDP socket
if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
// Server address setup
memset(&server_addr, 0, sizeof(server_addr));server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
// Send an initial message to server
sendto(client_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);
printf("Client registered with server. Waiting for messages...\n");
while (1) {
memset(buffer, 0, BUFFER_SIZE);
// Receive message from server
recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr,
&addr_len);
printf("Received: %s", buffer);
}
close(client_sock);
return 0;
}
