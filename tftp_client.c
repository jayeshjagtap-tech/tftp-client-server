#include <stdio.h>          // Standard I/O functions
#include <string.h>         // String handling functions
#include <unistd.h>         // close(), _exit()
#include <arpa/inet.h>      // Socket related functions
#include <sys/time.h>       // struct timeval for timeout
#include "tftp.h"           // Common TFTP definitions
#include "tftp_client.h"    // Client-specific definitions

/* Function declarations to avoid implicit declaration errors */
void process_command(tftp_client_t *client, char *command);
void connect_to_server(tftp_client_t *client, char *ip, int port);
void put_file(tftp_client_t *client, char *filename);
void get_file(tftp_client_t *client, char *filename);
void disconnect(tftp_client_t *client);

void send_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode);
void receive_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode);

int main() {
    char command[256];                  // Buffer to store user input
    tftp_client_t client;               // Client structure
    memset(&client, 0, sizeof(client)); // Initialize client data

    /* Continuous command prompt */
    while (1) {
        printf("tftp> ");
        fgets(command, sizeof(command), stdin);     // Read command from user
        command[strcspn(command, "\n")] = 0;        // Remove newline character
        process_command(&client, command);          // Process command
    }
}

/* Identify command and call appropriate function */
void process_command(tftp_client_t *client, char *command) {

    if (!strncmp(command, "connect", 7)) {
        char ip[32];
        sscanf(command, "connect %s", ip);          // Extract IP address
        connect_to_server(client, ip, PORT);        // Create socket and connect
    }
    else if (!strncmp(command, "get", 3)) {
        char file[128];
        sscanf(command, "get %s", file);            // Extract filename
        get_file(client, file);                     // Send RRQ
    }
    else if (!strncmp(command, "put", 3)) {
        char file[128];
        sscanf(command, "put %s", file);            // Extract filename
        put_file(client, file);                     // Send WRQ
    }
    else if (!strcmp(command, "bye") || !strcmp(command, "quit")) {
        disconnect(client);                         // Close socket
        _exit(0);                                   // Exit client
    }
    else {
        printf("Invalid command\n");                // Unknown command
    }
}

/* Create UDP socket and configure server address */
void connect_to_server(tftp_client_t *client, char *ip, int port) {

    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);   // Create UDP socket

    struct timeval tv = { TIMEOUT_SEC, 0 };            // Timeout value
    setsockopt(client->sockfd, SOL_SOCKET, SO_RCVTIMEO,
               &tv, sizeof(tv));                       // Apply timeout

    client->server_addr.sin_family = AF_INET;          // IPv4
    client->server_addr.sin_port = htons(port);        // Server port
    inet_pton(AF_INET, ip, &client->server_addr.sin_addr); // Convert IP

    client->server_len = sizeof(client->server_addr); // Address size
    printf("Connected to server\n");
}

/* Send write request (upload file) */
void put_file(tftp_client_t *client, char *filename) {
    send_request(client->sockfd, client->server_addr, filename, WRQ);
}

/* Send read request (download file) */
void get_file(tftp_client_t *client, char *filename) {
    receive_request(client->sockfd, client->server_addr, filename, RRQ);
}

/* Close socket and end connection */
void disconnect(tftp_client_t *client) {
    close(client->sockfd);                             // Close UDP socket
    printf("Disconnected\n");
}

/* Build and send RRQ or WRQ packet */
void send_request(int sockfd, struct sockaddr_in server_addr,
                  char *filename, int opcode) {

    tftp_packet packet;
    memset(&packet, 0, sizeof(packet));                // Clear packet

    packet.opcode = htons(opcode);                     // Set opcode
    strcpy(packet.body.request.filename, filename);    // Set filename
    strcpy(packet.body.request.mode, "octet");         // Binary mode

    sendto(sockfd, &packet, sizeof(packet), 0,
           (struct sockaddr *)&server_addr,
           sizeof(server_addr));                       // Send packet
}

/* Send request and initiate file transfer */
void receive_request(int sockfd, struct sockaddr_in server_addr,
                     char *filename, int opcode) {

    tftp_packet packet;
    memset(&packet, 0, sizeof(packet));                // Clear packet

    packet.opcode = htons(opcode);                     // Set RRQ opcode
    strcpy(packet.body.request.filename, filename);    // Filename
    strcpy(packet.body.request.mode, "octet");         // Transfer mode

    sendto(sockfd, &packet, sizeof(packet), 0,
           (struct sockaddr *)&server_addr,
           sizeof(server_addr));                       // Send RRQ

    printf("File transfer started\n");
}
