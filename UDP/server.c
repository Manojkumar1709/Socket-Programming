#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
int server_sock;
struct sockaddr_in server_addr, client_addr;
socklen_t client_len = sizeof(client_addr);
char buffer[BUFFER_SIZE];
// Create UDP socket
if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
// Server address setupmemset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
// Bind socket to address
if (bind(server_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
perror("Bind failed");
close(server_sock);
exit(EXIT_FAILURE);
}
printf("UDP Server is running on port %d...\n", PORT);
// Wait for the first message from the client
recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr,
&client_len);
printf("Client connected!\n");
while (1) {
printf("Enter message to send: ");
fgets(buffer, BUFFER_SIZE, stdin);
// Send message to client
sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr,
client_len);
printf("Message sent: %s", buffer);
}
close(server_sock);
return 0;}
